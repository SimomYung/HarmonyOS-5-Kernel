/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2021-2022. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */
#include <securec.h>
#include <linux/dmabuf/dmabuf_swap.h>
#include <linux/kref.h>
#include <linux/ktime.h>
#include <linux/mm_lb/mm_lb.h>
#include "hvgr_mem_context_switchover.h"
#include "hvgr_log_api.h"
#include "hvgr_mem_pool.h"
#include "hvgr_mem_api.h"
#include "hvgr_memory_page.h"
#include "hvgr_asid_api.h"
#include "hvgr_mem_sc.h"
#include "hvgr_sc_api.h"
#include "hvgr_sc_api.h"
#include "hvgr_mem_sc_hal.h"
#include "hvgr_cq.h"
#include "hvgr_mem_pool.h"
#include "hvgr_dm_api.h"
#include "hvgr_mem_debugfs.h"

static bool hvgr_switch_ratio_is_valid(uint32_t ratio, struct hvgr_ctx * const ctx,
	struct hvgr_switch_fs *fsinfo)
{
	if (ratio == 0 || ratio > 100) {
		hvgr_err(ctx->gdev, HVGR_MEM, "[ctx_sw]ctx_%u ratio is invalid, tgid:%d ratio:%d",
				ctx->id, ctx->tgid, ratio);
		hvgr_switch_update_stat_info(fsinfo, CTX_STATUS_ERROR, "ratio is invalid.");
		return false;
	}

	return true;
}

static struct hvgr_ctx *hvgr_switch_get_app_ctx(struct hvgr_ctx * const ctx,
	pid_t tgid, struct hvgr_switch_fs *fsinfo)
{
	struct hvgr_device *gdev = NULL;
	struct hvgr_ctx *tmp = NULL;
	struct hvgr_ctx *ctx_tmp = NULL;
	struct hvgr_ctx *main_ctx = NULL;
	uint32_t ctx_pages = 0;
	uint32_t tmp_pages;

	gdev = ctx->gdev;
	mutex_lock(&gdev->dm_dev.ctx_list_mutex);
	list_for_each_entry_safe(ctx_tmp, tmp, &gdev->dm_dev.ctx_list, dev_link) {
		if (ctx_tmp->tgid == tgid) {
			tmp_pages = atomic_read(&ctx_tmp->ctx_sw.same_va_total_pages);
			if (tmp_pages > ctx_pages) {
				ctx_pages = tmp_pages;
				main_ctx = ctx_tmp;
			}
		}
	}

	if (main_ctx != NULL)
		if (!kref_get_unless_zero(&main_ctx->ctx_kref)) {
			hvgr_err(ctx->gdev, HVGR_MEM, "[ctx_sw] ref is 0,tgid%d", tgid);
			mutex_unlock(&gdev->dm_dev.ctx_list_mutex);
			hvgr_switch_update_stat_info(fsinfo, CTX_STATUS_ERROR, "ref is 0.");
			return NULL;
		}
	mutex_unlock(&gdev->dm_dev.ctx_list_mutex);

	if (main_ctx == NULL) {
		hvgr_err(ctx->gdev, HVGR_MEM, "[ctx_sw] app ctx is null,tgid%d", tgid);
		hvgr_switch_update_stat_info(fsinfo, CTX_STATUS_ERROR, "get main ctx failed.");
	}
	return main_ctx;
}

static long hvgr_switch_add_ctx_to_list(struct hvgr_device *gdev,
	pid_t tgid, struct hvgr_ctx *ctx, struct hvgr_switch_fs *fsinfo, ktime_t *begin)
{
	struct hvgr_mem_context_sw_node *sw_node = NULL;

	sw_node = kzalloc(sizeof(*sw_node), GFP_KERNEL);
	if (unlikely(sw_node == NULL)) {
		hvgr_err(gdev, HVGR_MEM, "[ctx_sw] malloc sw node failed.");
		hvgr_switch_update_stat_info(fsinfo, CTX_STATUS_ERROR, "add ctx failed.");
		return HVGR_SW_OPER_FAILED;
	}

	sw_node->tgid = tgid;
	sw_node->ctx = ctx;
	mutex_lock(&gdev->dm_dev.swapped_out_list_mutex);
	list_add(&sw_node->node, &gdev->dm_dev.swapped_out_list);
	mutex_unlock(&gdev->dm_dev.swapped_out_list_mutex);
	fsinfo->id = ctx->id;
	hvgr_switch_fill_in_used_time(fsinfo, DATA_CHECK, begin);
	return 0;
}

static void hvgr_switch_list_del(struct hvgr_device *gdev,
	struct hvgr_ctx *ctx)
{
	struct hvgr_mem_context_sw_node *sw_node = NULL;
	struct hvgr_mem_context_sw_node *tmp = NULL;
	bool node_exist = false;

	mutex_lock(&gdev->dm_dev.swapped_out_list_mutex);
	list_for_each_entry_safe(sw_node, tmp, &gdev->dm_dev.swapped_out_list, node) {
		if (sw_node->ctx == ctx) {
			list_del(&sw_node->node);
			node_exist = true;
			break;
		}
	}
	mutex_unlock(&gdev->dm_dev.swapped_out_list_mutex);

	if (node_exist)
		kfree(sw_node);
}

static struct hvgr_ctx *hvgr_switch_get_ctx_and_del_list(struct hvgr_ctx * const ctx,
	pid_t tgid)
{
	struct hvgr_mem_context_sw_node *sw_node = NULL;
	struct hvgr_mem_context_sw_node *tmp = NULL;
	struct hvgr_ctx *sw_ctx = NULL;
	bool node_exist = false;

	mutex_lock(&ctx->gdev->dm_dev.swapped_out_list_mutex);
	list_for_each_entry_safe(sw_node, tmp, &ctx->gdev->dm_dev.swapped_out_list, node) {
		if (sw_node->tgid == tgid) {
			sw_ctx = sw_node->ctx;
			/* ref_cnt is 0,close ctx process will complete the rest of the process */
			if (!kref_get_unless_zero(&sw_ctx->ctx_kref)) {
				hvgr_err(ctx->gdev, HVGR_MEM, "[ctx_sw]ctx_%u tgid_%u kref is 0.",
					ctx->id, tgid);
				mutex_unlock(&ctx->gdev->dm_dev.swapped_out_list_mutex);
				return NULL;
			}
			list_del(&sw_node->node);
			node_exist = true;
			break;
		}
	}
	mutex_unlock(&ctx->gdev->dm_dev.swapped_out_list_mutex);

	if (node_exist)
		kfree(sw_node);

	return sw_ctx;
}

enum context_swtich_status hvgr_switch_get_status(struct hvgr_ctx *ctx)
{
	return ctx->ctx_sw.status;
}

static void hvgr_switch_set_status(struct hvgr_ctx *ctx,
	enum context_swtich_status status)
{
	ctx->ctx_sw.status = status;
}

static bool hvgr_switch_ctx_in_background(struct hvgr_ctx * const ctx,
	struct hvgr_switch_fs *fsinfo)
{
	if (hvgr_switch_get_status(ctx) != CTX_IS_FORE_GROUND) {
		hvgr_switch_update_stat_info(fsinfo, CTX_STATUS_ERROR,
			"ctx has been reclaimed.");
		hvgr_debug(ctx->gdev, HVGR_MEM, "[ctx_sw]ctx_%u has been reclaimed.", ctx->id);
		return false;
	}

	if (hvgr_cq_ctx_is_active(ctx)) {
		hvgr_switch_update_stat_info(fsinfo, CTX_STATUS_ERROR, "ctx is active.");
		return false;
	}

	if (atomic_read(&ctx->ctx_sw.ctx_foreground_flag) == 1) {
		hvgr_switch_update_stat_info(fsinfo, CTX_STATUS_ERROR, "ctx sw to front.");
		return false;
	}

	return true;
}


static long hvgr_switch_sw_data_init(struct hvgr_ctx *ctx, uint32_t ratio,
	struct hvgr_switch_fs *fsinfo)
{
	uint32_t background_max_pages = atomic_read(&ctx->ctx_sw.same_va_total_pages) * ratio / 100;

	ctx->ctx_sw.real_swaped_pages = 0;

	ctx->ctx_sw.extendid = vzalloc(background_max_pages * sizeof(*ctx->ctx_sw.extendid));
	if (unlikely(ctx->ctx_sw.extendid == NULL)) {
		hvgr_err(ctx->gdev, HVGR_MEM, "[ctx_sw] ctx_%u extendid is null,nums:%u.",
			ctx->id, background_max_pages);
		hvgr_switch_update_stat_info(fsinfo, CTX_STATUS_ERROR, "data init failed.");
		return HVGR_SW_OPER_FAILED;
	}

	ctx->ctx_sw.background_max_pages = background_max_pages;
	return 0;
}

static void hvgr_switch_free_sc_pool(struct hvgr_ctx *ctx, struct list_head *pool_list,
	uint32_t *recycle_pages)
{
	struct hvgr_sc_mem_pool *sc_pool = NULL;
	struct list_head *node = NULL;
	struct list_head *next_node = NULL;

	if (pool_list == NULL)
		return;

	list_for_each_safe(node, next_node, pool_list) {
		sc_pool = container_of(node, struct hvgr_sc_mem_pool, node);
		hvgr_mem_free_memctx_pool(ctx, &sc_pool->pool, recycle_pages);
	}
}

static void hvgr_switch_free_pool(struct hvgr_ctx *ctx, uint32_t *recycle_pages)
{
	uint32_t index;

	for (index = 0; index < MEM_POOL_NUM; index++)
		hvgr_mem_free_memctx_pool(ctx, &ctx->mem_ctx.mem_pool[index], recycle_pages);

}

static void hvgr_switch_flush_cache(struct hvgr_ctx *ctx, uint32_t *recycle_pages,
	struct hvgr_switch_fs *fsinfo, ktime_t *begin)
{

	hvgr_switch_fill_in_used_time(fsinfo, CLEAN_POOL_CYCLE, begin);
	hvgr_switch_fill_in_page_info(*recycle_pages, fsinfo, ctx);
}

static long hvgr_switch_resume_area_gpu_pt(struct hvgr_ctx *ctx,
	struct hvgr_mem_area *area)
{
	uint32_t level = 0;
	struct hvgr_mmu_setup_paras setup;

	setup.flags = area->flags;
	setup.gva = area->gva;
	setup.pages = area->pages;
	setup.page_array = area->page_array;
	setup.mmu_flag = area->mmu_flag;

	if (hvgr_mmu_set_up(ctx, &setup, &level, area) != 0)
		return HVGR_SW_OPER_FAILED;

	return 0;
}

static long hvgr_switch_resume_swaped_area_pt(struct hvgr_ctx *ctx)
{
	struct hvgr_mem_area *swap_area = NULL;
	struct hvgr_mem_area *tmp_area = NULL;
	long ret;

	mutex_lock(&ctx->ctx_sw.swapped_list_mutex);
	list_for_each_entry_safe(swap_area, tmp_area, &ctx->ctx_sw.swapped_list, swaped_node) {
		ret = hvgr_switch_resume_area_gpu_pt(ctx, swap_area);
		if (ret != 0) {
			hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u 0x%lx pages %lu mmu set up fail",
				ctx->id, swap_area->gva, swap_area->pages);
			mutex_unlock(&ctx->ctx_sw.swapped_list_mutex);
			return HVGR_SW_OPER_FAILED_NEED_CLOSE_CTX;
		}
		list_del(&swap_area->swaped_node);
	}
	mutex_unlock(&ctx->ctx_sw.swapped_list_mutex);
	return 0;
}

static long hvgr_switch_swap_out_page(struct hvgr_ctx *ctx,
	uint32_t *arrayed_out_pages, struct page **page_array)
{
	uint32_t real_pages = ctx->ctx_sw.real_swaped_pages;
	uint32_t batchs = real_pages / BATCH_SWAP_SIZE;
	uint32_t idx;
	long ret;

	for (idx = 0; idx < batchs; idx++) {
		ret = mm_gpu_page_array_out(&page_array[idx * BATCH_SWAP_SIZE], BATCH_SWAP_SIZE,
			&ctx->ctx_sw.extendid[idx * BATCH_SWAP_SIZE]);
		if (ret != 0) {
			hvgr_err(ctx->gdev, HVGR_MEM, "[ctx_sw]ctx_%u array out %u failed,ret:%d.", ctx->id, idx, ret);
			return HVGR_SW_OPER_FAILED;
		}
		*arrayed_out_pages += BATCH_SWAP_SIZE;

		if (atomic_read(&ctx->ctx_sw.ctx_foreground_flag) == 1)
			return HVGR_SW_OPER_FAILED;
	}

	if (real_pages % BATCH_SWAP_SIZE)  {
		ret = mm_gpu_page_array_out(&page_array[idx * BATCH_SWAP_SIZE],
			real_pages % BATCH_SWAP_SIZE, &ctx->ctx_sw.extendid[idx * BATCH_SWAP_SIZE]);
		if (ret != 0) {
			hvgr_err(ctx->gdev, HVGR_MEM, "[ctx_sw]ctx_%u array out final failed,ret:%d.", ctx->id, ret);
			return HVGR_SW_OPER_FAILED;
		}
	}

	*arrayed_out_pages = real_pages;
	return 0;
}

void hvgr_switch_free_extid_batch(uint32_t start_batch, uint32_t page_num, struct hvgr_ctx * const ctx)
{
	uint32_t idx;
	uint32_t max_batches = page_num / BATCH_SWAP_SIZE;

	for (idx = start_batch; idx < max_batches; idx++)
		mm_gpu_page_array_free_extid(BATCH_SWAP_SIZE,
			&ctx->ctx_sw.extendid[idx * BATCH_SWAP_SIZE]);

	if (page_num % BATCH_SWAP_SIZE)
		mm_gpu_page_array_free_extid(page_num % BATCH_SWAP_SIZE,
			&ctx->ctx_sw.extendid[idx * BATCH_SWAP_SIZE]);
}

void hvgr_switch_free_extid(uint32_t start, uint32_t page_num, struct hvgr_ctx * const ctx)
{
	uint32_t idx;
	uint32_t max_batches = page_num / BATCH_SWAP_SIZE;

	for (idx = 0; idx < max_batches; idx++)
		mm_gpu_page_array_free_extid(BATCH_SWAP_SIZE,
			&ctx->ctx_sw.extendid[start + idx * BATCH_SWAP_SIZE]);

	if (page_num % BATCH_SWAP_SIZE)
		mm_gpu_page_array_free_extid(page_num % BATCH_SWAP_SIZE,
			&ctx->ctx_sw.extendid[start + idx * BATCH_SWAP_SIZE]);
}

static long hvgr_switch_swap_in_page(struct hvgr_ctx *ctx, struct page **page_array,
	struct hvgr_switch_fs *fsinfo, ktime_t *begin)
{
	uint32_t real_pages = ctx->ctx_sw.real_swaped_pages;
	uint32_t batchs = real_pages / BATCH_SWAP_SIZE;
	long ret;
	uint32_t idx;

	for (idx = 0; idx < batchs; idx++) {
		ret = mm_gpu_page_array_in(&page_array[idx * BATCH_SWAP_SIZE], (int)BATCH_SWAP_SIZE,
			&ctx->ctx_sw.extendid[idx * BATCH_SWAP_SIZE]);
		if (ret != 0) {
			hvgr_err(ctx->gdev, HVGR_MEM, "[ctx_sw]ctx_%u array in fail, idx:%u ret:%d",
				ctx->id, idx, ret);
			goto free_extid;
		}
	}

	if (real_pages % BATCH_SWAP_SIZE) {
		ret = mm_gpu_page_array_in(&page_array[idx * BATCH_SWAP_SIZE],
			(int)real_pages % BATCH_SWAP_SIZE,
			&ctx->ctx_sw.extendid[idx * BATCH_SWAP_SIZE]);
		if (ret != 0) {
			hvgr_err(ctx->gdev, HVGR_MEM, "[ctx_sw]ctx_%u array in final fail, idx:%u, ret:%d",
				ctx->id, idx, ret);
			goto free_extid;
		}
	}
	hvgr_switch_fill_in_used_time(fsinfo, ARRAY_IN_PAGE, begin);
	return 0;

free_extid:
	if (idx < batchs)
		hvgr_switch_free_extid_batch(idx + 1, real_pages - (idx + 1) * BATCH_SWAP_SIZE, ctx);
	hvgr_switch_update_stat_info(fsinfo, CTX_STATUS_ERROR, "array in failed.");
	return HVGR_SW_OPER_FAILED;
}

static bool hvgr_switch_area_can_reclaim(struct hvgr_mem_area *area)
{
	if (hvgr_mem_area_ref_read(area) > 2U || area->is_freed ||
		(hvgr_mem_area_ref_read(area) == 1U && area->mmap_cnt > 0U))
		return false;
	return true;
}

static long hvgr_switch_zap_ptes(struct hvgr_ctx *ctx, struct page **page_array,
	struct hvgr_switch_fs *fsinfo, ktime_t *begin)
{
	u32 idx;
	long ret;
	struct rb_root *rbtree = &ctx->mem_ctx.area_rec.root;
	struct rb_node *next = rb_first(rbtree);
	struct hvgr_kv_node *kvnode = NULL;
	struct hvgr_mem_area *area = NULL;
	uint32_t start_index = 0;
	struct hvgr_mmu_takedown_paras paras;

	while (next != NULL) {
		kvnode = rb_entry(next, struct hvgr_kv_node, node);
		area = container_of(kvnode, struct hvgr_mem_area, node);
		next = rb_next(next);
		if ((area->property == HVGR_MEM_PROPERTY_SAME_VA) &&
			((area->attribute & HVGR_MEM_MASK_KMD_ACCESS) == 0) &&
			(hvgr_switch_area_can_reclaim(area)) &&
			(area->order == 0)) {
			if (unlikely(start_index + area->pages > ctx->ctx_sw.background_max_pages)) {
				hvgr_info(ctx->gdev, HVGR_MEM, "ctx_%u area index is invalid, cur:%u,nums:%u, total:%u.",
					ctx->id, start_index, area->pages, ctx->ctx_sw.background_max_pages);
				break;
			}

			paras.va = area->gva;
			paras.pages = area->pages;
			paras.bitmap = 0U;
			INIT_LIST_HEAD(&paras.free_list);

			ret = hvgr_mmu_take_down(ctx, &paras, area);
			if (ret != 0) {
				hvgr_err(ctx->gdev, HVGR_MEM, "[ctx_sw] ctx_%u 0x%lx pages %lu take down fail %d",
					ctx->id, area->gva, area->pages, ret);
				hvgr_switch_update_stat_info(fsinfo, CTX_STATUS_ERROR, "take down failed.");
				return HVGR_SW_OPER_FAILED;
			}
			hvgr_mmu_free_pt_pages(ctx, &paras.free_list);

			ret = hvgr_mem_zap_ptes(ctx, area->uva, (area->pages << PAGE_SHIFT));
			if (ret != 0) {
				hvgr_switch_update_stat_info(fsinfo, CTX_STATUS_ERROR, "zap pte failed.");
				if (hvgr_switch_resume_area_gpu_pt(ctx, area) != 0)
					return HVGR_SW_OPER_FAILED_NEED_CLOSE_CTX;
				return HVGR_SW_OPER_FAILED;
			}

			for (idx = 0; idx < area->pages; idx++)
				page_array[start_index++] = area->page_array[idx];

			ctx->ctx_sw.real_swaped_pages += area->pages;
			mutex_lock(&ctx->ctx_sw.swapped_list_mutex);
			list_add_tail(&area->swaped_node, &ctx->ctx_sw.swapped_list);
			mutex_unlock(&ctx->ctx_sw.swapped_list_mutex);
			if (atomic_read(&ctx->ctx_sw.ctx_foreground_flag) == 1)
				return HVGR_SW_OPER_FAILED;
		}
	}

	if (ctx->ctx_sw.real_swaped_pages == 0) {
		hvgr_err(ctx->gdev, HVGR_MEM, "[ctx_sw] ctx_%u tgid:%u no pages swaped.", ctx->id, ctx->tgid);
		return HVGR_SW_OPER_FAILED;
	}

	hvgr_info(ctx->gdev, HVGR_MEM, "[ctx_sw] ctx_%u swaped page is %u", ctx->id, ctx->ctx_sw.real_swaped_pages);
	hvgr_switch_fill_in_used_time(fsinfo, TAKE_DOWN_PAGE, begin);
	return 0;
}

static void hvgr_switch_free_swapped_area_page_array(struct hvgr_ctx *ctx)
{
	struct hvgr_mem_area *swap_area = NULL;
	struct hvgr_mem_area *tmp_area = NULL;

	if (list_empty(&ctx->ctx_sw.swapped_list))
		return;

	mutex_lock(&ctx->ctx_sw.swapped_list_mutex);
	list_for_each_entry_safe(swap_area, tmp_area, &ctx->ctx_sw.swapped_list, swaped_node) {
		kvfree(swap_area->page_array);  // when kill ctx, avoid double free memory
		swap_area->page_array = NULL;
		list_del(&swap_area->swaped_node);
	}
	mutex_unlock(&ctx->ctx_sw.swapped_list_mutex);
}

static long hvgr_switch_set_up_page_table(struct hvgr_ctx *ctx, struct page **page_array, uint32_t *set_up_pages,
	struct hvgr_switch_fs *fsinfo, ktime_t *begin)
{
	u32 idx;
	long ret = 0;
	uint32_t level = 0;
	struct hvgr_mmu_setup_paras setup;
	struct hvgr_mem_area *swap_area = NULL;
	struct hvgr_mem_area *tmp_area = NULL;
	uint32_t start_idx = 0;

	hvgr_switch_fill_in_used_time(fsinfo, DATA_CHECK_II, begin);

	if (list_empty(&ctx->ctx_sw.swapped_list))
		return HVGR_SW_OPER_FAILED;

	mutex_lock(&ctx->ctx_sw.swapped_list_mutex);
	list_for_each_entry_safe(swap_area, tmp_area, &ctx->ctx_sw.swapped_list, swaped_node) {
		if (unlikely(start_idx + swap_area->pages > ctx->ctx_sw.background_max_pages)) {
			hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u start:%u add pages:%u bigger total:%u.",
				ctx->id, start_idx, swap_area->pages, ctx->ctx_sw.background_max_pages);
			goto exit;
		}

		ret = hvgr_mem_pool_ctx_alloc(ctx, swap_area->pages, (void **)swap_area->page_array,
			swap_area->attribute);
		if (ret != 0) {
			hvgr_err(ctx->gdev, HVGR_MEM, "[ctx_sw], alloc %u pages failed, uva:%lx.",
				swap_area->pages, swap_area->uva);
			hvgr_switch_update_stat_info(fsinfo, CTX_STATUS_ERROR, "set up failed.");
			goto exit;
		}

		setup.flags = swap_area->flags;
		setup.gva = swap_area->gva;
		setup.pages = swap_area->pages;
		setup.page_array = swap_area->page_array;
		setup.mmu_flag = swap_area->mmu_flag;
		ret = hvgr_mmu_set_up(ctx, &setup, &level, swap_area);
		if (ret != 0) {
			hvgr_mem_flush_pt(ctx, swap_area->gva, swap_area->pages, WITHOUT_FLUSH_L2, level);
			hvgr_err(ctx->gdev, HVGR_MEM, "[ctx_sw] ctx_%u 0x%lx pages %lu mmu set up fail",
				ctx->id, swap_area->gva, swap_area->pages);
			hvgr_mem_pool_ctx_free(ctx, swap_area->pages, (void **)swap_area->page_array, 0,
				swap_area->attribute);
			hvgr_switch_update_stat_info(fsinfo, CTX_STATUS_ERROR, "set up failed.");
			goto exit;
		}

		hvgr_mem_flush_pt(ctx, swap_area->gva, swap_area->pages, WITHOUT_FLUSH_L2, level);

		for (idx = 0; idx < swap_area->pages; idx++)
			page_array[start_idx + idx] = swap_area->page_array[idx];
		start_idx += swap_area->pages;
		list_del(&swap_area->swaped_node);
	}
	mutex_unlock(&ctx->ctx_sw.swapped_list_mutex);
	*set_up_pages = start_idx;
	hvgr_switch_fill_in_used_time(fsinfo, SETUP_ALLOC_PAGE, begin);
	return 0;
exit:
	mutex_unlock(&ctx->ctx_sw.swapped_list_mutex);
	return HVGR_SW_OPER_FAILED;
}

static void hvgr_switch_free_page(uint32_t swap_pages, struct hvgr_ctx *ctx,
	struct hvgr_switch_fs *fsinfo, ktime_t *begin)
{
	struct hvgr_mem_area *swap_area = NULL;
	struct hvgr_mem_area *tmp_area = NULL;
	uint32_t policy_id = 0;

	hvgr_switch_fill_in_used_time(fsinfo, ARRAY_OUT_PAGE, begin);
	mutex_lock(&ctx->ctx_sw.swapped_list_mutex);
	list_for_each_entry_safe(swap_area, tmp_area, &ctx->ctx_sw.swapped_list, swaped_node) {
		hvgr_mem_free_page(ctx->gdev->dev, swap_area->page_array, (uint32_t)swap_area->pages,
			swap_area->order, policy_id);
		atomic_sub((int)swap_area->pages, &ctx->mem_ctx.used_pages);
		atomic_sub((int)swap_area->pages, &ctx->gdev->mem_dev.used_pages);
	}
	mutex_unlock(&ctx->ctx_sw.swapped_list_mutex);
	hvgr_switch_fill_in_used_time(fsinfo, FREE_PAGE, begin);
	fsinfo->real_swaped_pages = swap_pages;
	hvgr_switch_update_stat_info(fsinfo, CTX_IS_BACK_GROUND, "switch to background succeeded");
}

static void hvgr_switch_data_release(struct hvgr_ctx *ctx)
{
	if (ctx->ctx_sw.extendid)
		vfree(ctx->ctx_sw.extendid);

	ctx->ctx_sw.extendid = NULL;
	ctx->ctx_sw.background_max_pages = 0;
}

uint32_t hvgr_switch_get_ctx_pages(struct hvgr_ctx *ctx)
{
	struct rb_root *rbtree = NULL;
	struct rb_node *next = NULL;
	struct hvgr_kv_node *kvnode = NULL;
	struct hvgr_mem_area *area = NULL;
	uint32_t pages = 0;

	mutex_lock(&ctx->mem_ctx.area_mutex);
	rbtree = &ctx->mem_ctx.area_rec.root;
	next = rb_first(rbtree);

	while (next != NULL) {
		kvnode = rb_entry(next, struct hvgr_kv_node, node);
		area = container_of(kvnode, struct hvgr_mem_area, node);
		next = rb_next(next);
		if (area->property == HVGR_MEM_PROPERTY_SAME_VA)
			pages += area->pages;
	}
	mutex_unlock(&ctx->mem_ctx.area_mutex);
	atomic_set(&ctx->ctx_sw.same_va_total_pages, pages);
	return pages;
}

long hvgr_switch_to_background(struct hvgr_ctx * const ctx,
	hvgr_ioctl_para_ctx_sw * const para)
{
	struct hvgr_device *gdev =  ctx->gdev;
	struct hvgr_ctx *sw_ctx = NULL;
	uint32_t swap_out_pages = 0;
	uint32_t all_recycle_pages = 0;
	uint32_t ratio = para->in.ratio;
	uint32_t tgid = para->in.tgid;
	struct page **sw_page_array = NULL;
	struct hvgr_switch_fs fsinfo;
	ktime_t begin = 0;
	long ret = HVGR_SW_OPER_FAILED;
	long ret1;

	hvgr_switch_prev_ktime_init(&begin);

	(void)memset_s(&fsinfo, sizeof(fsinfo), 0, sizeof(fsinfo));
	fsinfo.tgid = tgid;
	if (!hvgr_switch_ratio_is_valid(ratio, ctx, &fsinfo))
		goto para_invalid;

	sw_ctx = hvgr_switch_get_app_ctx(ctx, tgid, &fsinfo);
	if (unlikely(sw_ctx == NULL))
		goto para_invalid;

	mutex_lock(&sw_ctx->mem_ctx.area_mutex);
	sw_ctx->ctx_sw.is_background = true;
	if (!hvgr_switch_ctx_in_background(sw_ctx, &fsinfo))
		goto release_lock;

	if (hvgr_switch_add_ctx_to_list(gdev, tgid, sw_ctx, &fsinfo, &begin) != 0)
		goto release_lock;

	hvgr_switch_set_status(sw_ctx, CTX_IS_SWITCHING);

	hvgr_cq_check_ctx_is_active(sw_ctx);

	if (hvgr_switch_sw_data_init(sw_ctx, ratio, &fsinfo) != 0)
		goto del_ctx_list;

	sw_page_array = vmalloc(sw_ctx->ctx_sw.background_max_pages * sizeof(struct page *));
	if (unlikely(sw_page_array == NULL)) {
		hvgr_switch_update_stat_info(&fsinfo, CTX_STATUS_ERROR, "sw init failed.");
		goto release_sw_data;
	}

	hvgr_switch_free_pool(sw_ctx, &all_recycle_pages);
	hvgr_switch_flush_cache(sw_ctx, &all_recycle_pages, &fsinfo, &begin);

	ret = hvgr_switch_zap_ptes(sw_ctx, sw_page_array, &fsinfo, &begin);
	if (ret != 0) {
		hvgr_switch_update_stat_info(&fsinfo, CTX_STATUS_ERROR, "zap pte failed.");
		goto resume_pte;
	}

	if (hvgr_switch_swap_out_page(sw_ctx, &swap_out_pages, sw_page_array) != 0) {
		hvgr_switch_update_stat_info(&fsinfo, CTX_STATUS_ERROR, "array out page failed.");
		goto release_array_out_data;
	}

	hvgr_switch_free_page(swap_out_pages, sw_ctx, &fsinfo, &begin);
	vfree(sw_page_array);
	all_recycle_pages += swap_out_pages;
	para->out.background_pages = all_recycle_pages;
	hvgr_switch_set_status(sw_ctx, CTX_IS_BACK_GROUND);
	hvgr_switch_list_add_fsinfo(sw_ctx, &fsinfo);
	mutex_unlock(&sw_ctx->mem_ctx.area_mutex);
	kref_put(&sw_ctx->ctx_kref, hvgr_ctx_kref_release);
	return 0;

release_array_out_data:
	if (swap_out_pages != 0)
		hvgr_switch_free_extid_batch(0, swap_out_pages, sw_ctx);
resume_pte:
	hvgr_switch_set_status(sw_ctx, CTX_STATUS_ERROR);
	ret1 = hvgr_switch_resume_swaped_area_pt(sw_ctx);
	if (ret == HVGR_SW_OPER_FAILED_NEED_CLOSE_CTX || ret1 != 0) {
		hvgr_switch_free_swapped_area_page_array(sw_ctx);
		ret = HVGR_SW_OPER_FAILED_NEED_CLOSE_CTX;
	} else {
		ret = HVGR_SW_OPER_FAILED;
	}

	vfree(sw_page_array);
release_sw_data:
	hvgr_switch_data_release(sw_ctx);
del_ctx_list:
	hvgr_switch_list_del(gdev, sw_ctx);
	hvgr_switch_set_status(sw_ctx, CTX_IS_FORE_GROUND);
release_lock:
	/* The process has been switched to the background. The status cannot be set to false. */
	if (hvgr_switch_get_status(sw_ctx) != CTX_IS_BACK_GROUND)
		sw_ctx->ctx_sw.is_background = false;
	hvgr_switch_list_add_fsinfo(sw_ctx, &fsinfo);
	mutex_unlock(&sw_ctx->mem_ctx.area_mutex);
	kref_put(&sw_ctx->ctx_kref, hvgr_ctx_kref_release);
para_invalid:
	para->out.background_pages = 0;
	hvgr_debug(gdev, HVGR_MEM, "[ctx_sw]switch to background failed,%u,:%u.", tgid, ret);
	return ret;
}

long hvgr_switch_to_frontground(struct hvgr_ctx * const ctx, pid_t tgid)
{
	struct hvgr_ctx *app_context = NULL;
	struct page **sw_page_array = NULL;
	uint32_t set_up_pages = 0;
	long ret = HVGR_SW_OPER_FAILED;
	ktime_t begin = 0;
	struct hvgr_switch_fs fsinfo;

	hvgr_switch_prev_ktime_init(&begin);
	(void)memset_s(&fsinfo, sizeof(fsinfo), 0, sizeof(fsinfo));
	fsinfo.tgid = tgid;

	app_context = hvgr_switch_get_ctx_and_del_list(ctx, tgid);
	if (unlikely(app_context == NULL)) {
		hvgr_debug(ctx->gdev, HVGR_MEM, "[ctx_sw]ctx is null.");
		ret = 0;
		goto para_invalid;
	}

	atomic_set(&app_context->ctx_sw.ctx_foreground_flag, 1);
	mutex_lock(&app_context->mem_ctx.area_mutex);
	if (hvgr_switch_get_status(app_context) != CTX_IS_BACK_GROUND) {
		hvgr_switch_update_stat_info(&fsinfo, CTX_STATUS_ERROR, "ctx status invalid.");
		hvgr_debug(ctx->gdev, HVGR_MEM, "[ctx_sw]ctx is not background.");
		ret = 0;
		goto release_lock;
	}

	sw_page_array = vmalloc(app_context->ctx_sw.background_max_pages * sizeof(struct page *));
	if (unlikely(sw_page_array == NULL)) {
		hvgr_err(ctx->gdev, HVGR_MEM, "[ctx_sw] alloc sw page array failed.");
		hvgr_switch_update_stat_info(&fsinfo, CTX_STATUS_ERROR, "page array alloc failed.");
		goto free_swapped_page_array;
	}

	if (hvgr_switch_set_up_page_table(app_context, sw_page_array, &set_up_pages,
		&fsinfo, &begin) != 0) {
		hvgr_switch_update_stat_info(&fsinfo, CTX_STATUS_ERROR, "set up pt failed.");
		goto free_swapped_page_array;
	}

	if (set_up_pages != app_context->ctx_sw.real_swaped_pages) {
		hvgr_err(ctx->gdev, HVGR_MEM, "[ctx_sw] ctx_%u pages unmatched,%u %u",
			app_context->id, set_up_pages, app_context->ctx_sw.real_swaped_pages);
		hvgr_switch_update_stat_info(&fsinfo, CTX_STATUS_ERROR, "pages not match.");
		goto pages_no_match;
	}

	if (hvgr_switch_swap_in_page(app_context, sw_page_array, &fsinfo, &begin) != 0)
		goto release_switch_data;

	hvgr_switch_data_release(app_context);
	vfree(sw_page_array);
	hvgr_switch_set_status(app_context, CTX_IS_FORE_GROUND);

	app_context->ctx_sw.is_background = false;
	atomic_set(&app_context->ctx_sw.ctx_foreground_flag, 0);

	hvgr_switch_update_stat_info(&fsinfo, CTX_IS_FORE_GROUND,
		"switch to frontground succeeded");
	hvgr_switch_update_front_info(app_context, &fsinfo);
	mutex_unlock(&app_context->mem_ctx.area_mutex);
	kref_put(&app_context->ctx_kref, hvgr_ctx_kref_release);
	return 0;

free_swapped_page_array:
	hvgr_switch_free_swapped_area_page_array(app_context);
pages_no_match:
	hvgr_switch_free_extid_batch(0, app_context->ctx_sw.real_swaped_pages, app_context);
release_switch_data:
	hvgr_switch_data_release(app_context);
	if (sw_page_array)
		vfree(sw_page_array);
	hvgr_switch_set_status(app_context, CTX_IS_FORE_GROUND);
release_lock:
	atomic_set(&app_context->ctx_sw.ctx_foreground_flag, 0);
	hvgr_switch_update_front_info(app_context, &fsinfo);
	mutex_unlock(&app_context->mem_ctx.area_mutex);
	kref_put(&app_context->ctx_kref, hvgr_ctx_kref_release);
para_invalid:
	hvgr_debug(ctx->gdev, HVGR_MEM, "[ctx_sw]switch to frontground failed, %u:%u.", tgid, ret);
	return ret;
}

void hvgr_switch_close_ctx(struct hvgr_ctx * const sw_ctx)
{
	struct page **sw_page_array = NULL;
	uint32_t set_up_pages = 0;
	ktime_t begin = 0;
	struct hvgr_switch_fs fsinfo;

	hvgr_switch_prev_ktime_init(&begin);
	(void)memset_s(&fsinfo, sizeof(fsinfo), 0, sizeof(fsinfo));

	mutex_lock(&sw_ctx->mem_ctx.area_mutex);
	if (hvgr_switch_get_status(sw_ctx) != CTX_IS_BACK_GROUND)
		goto release_lock;
	hvgr_switch_list_del(sw_ctx->gdev, sw_ctx);

	fsinfo.tgid = sw_ctx->tgid;
	sw_page_array = vmalloc(sw_ctx->ctx_sw.background_max_pages * sizeof(struct page *));
	if (unlikely(sw_page_array == NULL)) {
		hvgr_err(sw_ctx->gdev, HVGR_MEM, "close ctx alloc sw page array failed.");
		goto free_swapped_page_array;
	}

	if (hvgr_switch_set_up_page_table(sw_ctx, sw_page_array, &set_up_pages, &fsinfo, &begin) != 0)
		goto free_swapped_page_array;

	if (set_up_pages != sw_ctx->ctx_sw.real_swaped_pages) {
		hvgr_err(sw_ctx->gdev, HVGR_MEM, "[close ctx]ctx_%u pages unmatched,%u %u",
			sw_ctx->id, set_up_pages, sw_ctx->ctx_sw.real_swaped_pages);
		goto pages_no_match;
	}

	if (hvgr_switch_swap_in_page(sw_ctx, sw_page_array, &fsinfo, &begin) != 0)
		goto release_switch_data;

	hvgr_switch_data_release(sw_ctx);
	vfree(sw_page_array);
	hvgr_switch_set_status(sw_ctx, CTX_IS_FORE_GROUND);
	hvgr_switch_update_stat_info(&fsinfo, CTX_IS_FORE_GROUND,
		"kill ctx succeeded");
	hvgr_switch_update_front_info(sw_ctx, &fsinfo);
	mutex_unlock(&sw_ctx->mem_ctx.area_mutex);
	return;

free_swapped_page_array:
	hvgr_switch_free_swapped_area_page_array(sw_ctx);
pages_no_match:
	hvgr_switch_free_extid_batch(0, sw_ctx->ctx_sw.real_swaped_pages, sw_ctx);
release_switch_data:
	hvgr_switch_data_release(sw_ctx);
	if (sw_page_array)
		vfree(sw_page_array);
	hvgr_switch_set_status(sw_ctx, CTX_IS_FORE_GROUND);
	hvgr_switch_update_stat_info(&fsinfo, CTX_IS_FORE_GROUND,
		"kill ctx failed.");
	hvgr_switch_update_front_info(sw_ctx, &fsinfo);
release_lock:
	mutex_unlock(&sw_ctx->mem_ctx.area_mutex);
}

void hvgr_switch_free_swapped_area(struct hvgr_mem_area * const area)
{
	struct hvgr_ctx *ctx = container_of(area->zone->pmem, struct hvgr_ctx, mem_ctx);
	struct hvgr_mem_area *swap_area = NULL;
	struct hvgr_mem_area *tmp_area = NULL;
	uint32_t start = 0;
	uint32_t totals = ctx->ctx_sw.real_swaped_pages;
	uint32_t idx;
	bool area_is_swapped = false;

	if (list_empty(&ctx->ctx_sw.swapped_list))
		goto exit;

	mutex_lock(&ctx->ctx_sw.swapped_list_mutex);
	list_for_each_entry_safe(swap_area, tmp_area, &ctx->ctx_sw.swapped_list, swaped_node) {
		if (swap_area != area) {
			start += swap_area->pages;
			continue;
		}

		if (totals < start + swap_area->pages)
			break;

		hvgr_switch_free_extid(start, swap_area->pages, ctx);
		area_is_swapped = true;
		for (idx = start; idx < totals - swap_area->pages - start; idx++)
			ctx->ctx_sw.extendid[idx] = ctx->ctx_sw.extendid[idx + swap_area->pages];
		ctx->ctx_sw.real_swaped_pages -= swap_area->pages;
		list_del(&swap_area->swaped_node);
		hvgr_debug(ctx->gdev, HVGR_MEM, "[ctx_sw]ctx_%u tgid:%u background free va:0x%lx, 0x%lx",
			ctx->id, ctx->tgid, swap_area->uva, swap_area->pages);
		break;
	}
	mutex_unlock(&ctx->ctx_sw.swapped_list_mutex);
exit:
	if (!area_is_swapped)
		(void)hvgr_mem_free_pages(area);
}
