/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2024-2025. All rights reserved.
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
#include <mali_kbase.h>
#include "mali_kbase_context_switchover.h"
#include <mali_kbase_mem.h>
#include "mali_kbase_mem_recycle_debugfs.h"
#include <linux/delay.h>

#define INIT_REFCOUNT 1
#define RECLAIMABLE_REFCOUNT 2

static bool kbase_switch_ratio_is_valid(uint32_t ratio, struct kbase_context * const ctx,
	struct kbase_switch_fs *fsinfo)
{
	if (ratio == 0 || ratio > 100) {
		kbase_switch_update_stat_info(fsinfo, CTX_STATUS_ERROR, "ratio is invalid.");
		return false;
	}

	return true;
}

static struct kbase_context *kbase_switch_get_ctx(struct kbase_context * const ctx,
	pid_t tgid, struct kbase_switch_fs *fsinfo)
{
	struct kbase_device *kbdev = NULL;
	struct kbase_context *pos = NULL;
	struct kbase_context *next = NULL;
	struct kbase_context *target = NULL;
	uint32_t ctx_pages = 0;
	uint32_t tmp_pages = 0;

	kbdev = ctx->kbdev;
	mutex_lock(&kbdev->kctx_list_lock);
	list_for_each_entry_safe(pos, next, &kbdev->kctx_list, kctx_list_link) {
		if (pos->tgid == tgid) { // one tgid may correspond to multiple ctx
			tmp_pages = atomic_read(&pos->kctx_sw.same_va_total_pages);
			if (tmp_pages > ctx_pages) {
				ctx_pages = tmp_pages;
				target = pos;
			}
		}
	}
	mutex_unlock(&kbdev->kctx_list_lock);

	if (target == NULL) {
		dev_err(kbdev->dev, "app ctx is null , tgid %d\n", tgid);
		kbase_switch_update_stat_info(fsinfo, CTX_STATUS_ERROR, "get main ctx failed.");
	}
	return target;
}

static int kbase_switch_add_ctx_to_list(struct kbase_device *gdev,
	pid_t tgid, struct kbase_context *ctx, struct kbase_switch_fs *fsinfo, ktime_t *begin)
{
	struct kbase_mem_context_sw_node *sw_node = NULL;

	sw_node = kzalloc(sizeof(*sw_node), GFP_KERNEL);
	if (unlikely(sw_node == NULL)) {
		dev_err(gdev->dev, "[ctx_sw] malloc sw node failed.");
		kbase_switch_update_stat_info(fsinfo, CTX_STATUS_ERROR, "add ctx failed.");
		return KBASE_SW_OPER_FAILED;
	}

	sw_node->tgid = tgid;
	sw_node->ctx = ctx;
	mutex_lock(&gdev->swapped_out_list_mutex);
	list_add(&sw_node->node, &gdev->swapped_out_list);
	mutex_unlock(&gdev->swapped_out_list_mutex);
	fsinfo->id = ctx->id;
	kbase_switch_fill_in_used_time(fsinfo, BG_DATA_CHECK, begin);
	return 0;
}

static long kbase_switch_sw_data_init(struct kbase_context *ctx, uint32_t ratio, struct kbase_switch_fs *fsinfo)
{
	uint32_t background_max_pages =
		max(atomic_read(&ctx->kctx_sw.same_va_total_pages) * ratio / 100, ctx->kctx_sw.first_reg_nents);
	dev_info(ctx->kbdev->dev, "[ctx_sw] ctx_%u first_reg_nents is :%d.\n",
			ctx->id, ctx->kctx_sw.first_reg_nents);
	ctx->kctx_sw.real_swaped_pages = 0;
	ctx->kctx_sw.extendid = vzalloc(background_max_pages * sizeof(*ctx->kctx_sw.extendid));
	if (unlikely(ctx->kctx_sw.extendid == NULL)) {
		dev_err(ctx->kbdev->dev, "[ctx_sw] ctx_%u extendid is null,nums:%u.\n",
			ctx->id, background_max_pages);
		kbase_switch_update_stat_info(fsinfo, CTX_STATUS_ERROR, "data init failed.");
		return KBASE_SW_OPER_FAILED;
	}

	ctx->kctx_sw.background_max_pages = background_max_pages;
	return 0;
}

static void kbase_switch_list_del(struct kbase_device *gdev,
	struct kbase_context *ctx)
{
	struct kbase_mem_context_sw_node *sw_node = NULL;
	struct kbase_mem_context_sw_node *tmp = NULL;

	mutex_lock(&gdev->swapped_out_list_mutex);
	list_for_each_entry_safe(sw_node, tmp, &gdev->swapped_out_list, node) {
		if (sw_node->ctx == ctx) {
			list_del(&sw_node->node);
			kfree(sw_node);
			break;
		}
	}
	mutex_unlock(&gdev->swapped_out_list_mutex);
}

static void kbase_switch_sw_data_release(struct kbase_context *ctx)
{
	if (ctx->kctx_sw.extendid != NULL)
		vfree(ctx->kctx_sw.extendid);

	ctx->kctx_sw.extendid = NULL;
	ctx->kctx_sw.background_max_pages = 0;
	ctx->kctx_sw.real_swaped_pages = 0;
	ctx->kctx_sw.first_reg_nents = 0;
}

static bool kbase_region_is_reclaimable(struct kbase_context *kctx, struct kbase_va_region *reg)
{
	lockdep_assert_held(&kctx->reg_lock);
	if (reg->is_free || reg->va_refcnt > RECLAIMABLE_REFCOUNT ||
		reg->va_refcnt == INIT_REFCOUNT || atomic_read(&reg->gpu_alloc->kernel_mappings) > 0) {
		return false;
	}
	if (reg->gpu_alloc->permanent_map != NULL && reg->gpu_alloc->permanent_map->addr != NULL) {
		return false;
	}
	if (atomic_read(&reg->gpu_alloc->alias_count) > 0) {
		dev_info(kctx->kbdev->dev, "alloc is ref by alias, skip\n");
		return false;
	}

	return true;
}

static long kbase_switch_resume_reg_gpu_pt(struct kbase_context *kctx, struct kbase_va_region *reg)
{
	int ret = -1;
	struct kbase_mem_phy_alloc *alloc = NULL;
	const enum kbase_caller_mmu_sync_info mmu_sync_info = CALLER_MMU_ASYNC;

	alloc = reg->gpu_alloc;
	ret = kbase_mmu_insert_pages(kctx->kbdev, &kctx->mmu, reg->start_pfn,
						kbase_get_gpu_phy_pages(reg),
						kbase_reg_current_backed_size(reg), reg->flags,
						kctx->as_nr, alloc->group_id, mmu_sync_info, reg, false, alloc->header_page_number);
	if (ret != 0) {
		dev_err(kctx->kbdev->dev, "kbase_mmu_insert_pages failed!\n");
	}
	return ret;
}

static bool is_native_type(struct kbase_mem_phy_alloc *alloc)
{
	if (!alloc) {
		return false;
	}

	return alloc->type == KBASE_MEM_TYPE_NATIVE;
}

static int kbase_switch_teardown_ptes(struct kbase_context *ctx, struct page **page_array,
    struct kbase_switch_fs *fsinfo, ktime_t *begin)
{
	u32 idx;
	int ret = 0;
	struct rb_root *rbtree = &ctx->reg_rbtree_same;
	struct rb_node *next = rb_first(rbtree);
	struct kbase_va_region *reg = NULL;
	uint32_t start_index = 0;
	struct kbase_mem_phy_alloc *alloc = NULL;

	while(next != NULL) {
		reg = rb_entry(next, struct kbase_va_region, rblink);
		next = rb_next(next);
		if (kbase_is_region_invalid_or_free(reg)) {
			continue;
		}
		alloc = reg->gpu_alloc;
		if (!is_native_type(alloc)) {
			dev_dbg(ctx->kbdev->dev, "alloc is not native type!\n");
			continue;
		}
		if (reg->nr_pages == reg->gpu_alloc->nents && kbase_region_is_reclaimable(ctx, reg)) {
			if (unlikely(start_index + reg->nr_pages > ctx->kctx_sw.background_max_pages)) {
				dev_info(ctx->kbdev->dev, "ctx_%u reg index is invalid, cur:%u,nums:%lu, total:%u.",
					ctx->id, start_index, reg->nr_pages, ctx->kctx_sw.background_max_pages);
				break;
			}
			ret = kbase_mmu_teardown_pages(ctx->kbdev, &ctx->mmu, reg->start_pfn, alloc->pages,
					alloc->nents, ctx->as_nr, false);
			if (ret != 0) {
				dev_err(ctx->kbdev->dev, "kbase_mmu_teardown_pages failed! ret = %d\n", ret);
				kbase_switch_update_stat_info(fsinfo, CTX_STATUS_ERROR, "teardown pages failed.");
				return ret;
			}
			for (idx = 0; idx < alloc->nents; idx++) {
				page_array[start_index++] = as_page(alloc->pages[idx]);
			}
			ret = kbase_reg_vma_zap_pte(ctx, reg, reg->start_pfn << PAGE_SHIFT, (reg->nr_pages << PAGE_SHIFT));
			if (ret != 0) {
				dev_err(ctx->kbdev->dev, "kbase_reg_vma_zap_pte failed! ret = %d\n", ret);
				kbase_switch_update_stat_info(fsinfo, CTX_STATUS_ERROR, "zap pte failed.");
				if (kbase_switch_resume_reg_gpu_pt(ctx, reg) != 0)
					return KBASE_SW_OPER_FAILED_NEED_CLOSE_CTX;
				return KBASE_SW_OPER_FAILED;
			}
			ctx->kctx_sw.real_swaped_pages += reg->nr_pages;

			mutex_lock(&ctx->kctx_sw.swapped_list_mutex);
			list_add_tail(&reg->swapped_node, &ctx->kctx_sw.swapped_list);
			reg->swap_out = true;
			mutex_unlock(&ctx->kctx_sw.swapped_list_mutex);

			if (atomic_read(&ctx->kctx_sw.ctx_foreground_flag) == 1) {
				dev_warn(ctx->kbdev->dev, "[ctx_sw] ctx_%u tgid:%u set foreground flag", ctx->id, ctx->tgid);
				ret = KBASE_SW_OPER_FAILED;
				break;
			}
		}
	}
	if (ctx->kctx_sw.real_swaped_pages == 0) {
		dev_err(ctx->kbdev->dev, "[ctx_sw] ctx_%u tgid:%u no pages swaped.", ctx->id, ctx->tgid);
		return KBASE_SW_OPER_FAILED;
	}
	kbase_switch_fill_in_used_time(fsinfo, TAKE_DOWN_PAGE, begin);

	return ret;
}

enum context_swtich_status kbase_switch_get_status(struct kbase_context *ctx)
{
	return ctx->kctx_sw.status;
}

static void kbase_switch_set_status(struct kbase_context *ctx,
	enum context_swtich_status status)
{
	ctx->kctx_sw.status = status;
}

static void kbase_switch_remove_swapped_node(struct kbase_context *ctx)
{
	struct kbase_va_region *swap_reg = NULL;
	struct kbase_va_region *tmp_reg = NULL;

	if (list_empty(&ctx->kctx_sw.swapped_list) != 0)
		return;

	mutex_lock(&ctx->kctx_sw.swapped_list_mutex);
	list_for_each_entry_safe(swap_reg, tmp_reg, &ctx->kctx_sw.swapped_list, swapped_node) {
		list_del(&swap_reg->swapped_node);
		swap_reg->swap_out = false;
	}
	mutex_unlock(&ctx->kctx_sw.swapped_list_mutex);
}

static long kbase_switch_resume_swaped_area_pt(struct kbase_context *ctx)
{
	struct kbase_va_region *swap_reg = NULL;
	struct kbase_va_region *tmp_reg = NULL;
	long ret;

	mutex_lock(&ctx->kctx_sw.swapped_list_mutex);
	list_for_each_entry_safe(swap_reg, tmp_reg, &ctx->kctx_sw.swapped_list, swapped_node) {
		ret = kbase_switch_resume_reg_gpu_pt(ctx, swap_reg);
		if (ret != 0) {
			dev_err(ctx->kbdev->dev, "kbase_switch_resume_reg_gpu_pt fail!\n");
			mutex_unlock(&ctx->kctx_sw.swapped_list_mutex);
			return KBASE_SW_OPER_FAILED_NEED_CLOSE_CTX;
		}
		list_del(&swap_reg->swapped_node);
		swap_reg->swap_out = false;
	}
	mutex_unlock(&ctx->kctx_sw.swapped_list_mutex);
	return 0;
}

static long kbase_switch_swap_out_page(struct kbase_context *ctx, uint32_t *arrayed_out_pages, struct page **page_array)
{
	uint32_t real_pages = ctx->kctx_sw.real_swaped_pages;
	uint32_t batchs = real_pages / BATCH_SWAP_SIZE;
	uint32_t idx;
	long ret;

	for (idx = 0; idx < batchs; idx++) {
		ret = mm_gpu_page_array_out(&page_array[idx * BATCH_SWAP_SIZE], BATCH_SWAP_SIZE,
			&ctx->kctx_sw.extendid[idx * BATCH_SWAP_SIZE]);
		if (ret != 0) {
			dev_err(ctx->kbdev->dev, "[ctx_sw]ctx_%u array out %u failed,ret:%ld.\n", ctx->id, idx, ret);
			return KBASE_SW_OPER_FAILED;
		}
		*arrayed_out_pages += BATCH_SWAP_SIZE;

		if (atomic_read(&ctx->kctx_sw.ctx_foreground_flag) == 1) {
			dev_err(ctx->kbdev->dev, "[ctx_sw]ctx_%u tgid:%d set foreground flag\n", ctx->id, ctx->tgid);
			return KBASE_SW_OPER_FAILED;
		}
	}

	if ((real_pages % BATCH_SWAP_SIZE) != 0U) {
		ret = mm_gpu_page_array_out(&page_array[idx * BATCH_SWAP_SIZE],
			real_pages % BATCH_SWAP_SIZE, &ctx->kctx_sw.extendid[idx * BATCH_SWAP_SIZE]);
		if (ret != 0) {
			dev_err(ctx->kbdev->dev, "[ctx_sw]ctx_%u array out final failed,ret:%ld.\n", ctx->id, ret);
			return KBASE_SW_OPER_FAILED;
		}
	}

	*arrayed_out_pages = real_pages;
	return 0;
}

static void kbase_switch_free_extid_batch(uint32_t start_batch, uint32_t page_num,
	struct kbase_context * const ctx)
{
	uint32_t idx;
	uint32_t max_batches = page_num / BATCH_SWAP_SIZE;

	for (idx = start_batch; idx < max_batches; idx++) {
		mm_gpu_page_array_free_extid(BATCH_SWAP_SIZE, &ctx->kctx_sw.extendid[idx * BATCH_SWAP_SIZE]);
	}

	if ((page_num % BATCH_SWAP_SIZE) != 0U) {
		mm_gpu_page_array_free_extid(page_num % BATCH_SWAP_SIZE, &ctx->kctx_sw.extendid[idx * BATCH_SWAP_SIZE]);
	}
}

static struct kbase_context *kbase_switch_get_background_ctx(struct kbase_context * const ctx, pid_t tgid)
{
	struct kbase_mem_context_sw_node *sw_node = NULL;
	struct kbase_mem_context_sw_node *tmp = NULL;
	struct kbase_context *sw_ctx = NULL;
	mutex_lock(&ctx->kbdev->kctx_list_lock);
	mutex_lock(&ctx->kbdev->swapped_out_list_mutex);
	list_for_each_entry_safe(sw_node, tmp, &ctx->kbdev->swapped_out_list, node) {
		if (sw_node->tgid == tgid) {
			sw_ctx = sw_node->ctx;
			break;
		}
	}
	mutex_unlock(&ctx->kbdev->swapped_out_list_mutex);
	mutex_unlock(&ctx->kbdev->kctx_list_lock);
	return sw_ctx;
}

static int kbase_switch_alloc_pages(struct kbase_context *ctx, struct kbase_switch_fs *fsinfo)
{
	struct kbase_va_region *swap_reg = NULL;
	struct kbase_va_region *tmp_reg = NULL;
	struct kbase_mem_phy_alloc *alloc = NULL;
	int ret;

	list_for_each_entry_safe(swap_reg, tmp_reg, &ctx->kctx_sw.swapped_list, swapped_node) {
		alloc = swap_reg->gpu_alloc;
		if (alloc) {
			ret = kbase_alloc_phy_pages_helper(alloc, swap_reg->nr_pages);
			if(ret != 0) {
				dev_err(ctx->kbdev->dev, "[ctx_sw], alloc %lu pages failed, reg:%p.", swap_reg->nr_pages, swap_reg);
				kbase_switch_update_stat_info(fsinfo, CTX_STATUS_ERROR, "alloc_phy_pages failed.");
				return ret;
			}
		}
	}

	return 0;
}

static long kbase_switch_set_up_page_table(struct kbase_context *ctx, struct page **page_array, uint32_t *set_up_pages,
	struct kbase_switch_fs *fsinfo, ktime_t *begin)
{
	u32 idx;
	long ret = 0;
	uint32_t level = 0;
	struct kbase_va_region *swap_reg = NULL;
	struct kbase_va_region *tmp_reg = NULL;
	struct kbase_mem_phy_alloc *alloc = NULL;
	uint32_t start_idx = 0;

	const enum kbase_caller_mmu_sync_info mmu_sync_info = CALLER_MMU_ASYNC;
	kbase_switch_fill_in_used_time(fsinfo, FG_DATA_CHECK, begin);

	if (list_empty(&ctx->kctx_sw.swapped_list) != 0) {
		dev_err(ctx->kbdev->dev, "swapped_list is null\n");
		return KBASE_SW_OPER_FAILED;
	}
		
	mutex_lock(&ctx->kctx_sw.swapped_list_mutex);
	if(kbase_switch_alloc_pages(ctx, fsinfo) != 0) {
		dev_err(ctx->kbdev->dev, "kbase_switch_alloc_pages failed\n");
		goto exit;
	}

	list_for_each_entry_safe(swap_reg, tmp_reg, &ctx->kctx_sw.swapped_list, swapped_node) {
		if (unlikely(start_idx + swap_reg->nr_pages > ctx->kctx_sw.background_max_pages)) {
			dev_err(ctx->kbdev->dev, "ctx_%u start:%u add pages:%lu bigger total:%u.",
				ctx->id, start_idx, swap_reg->nr_pages, ctx->kctx_sw.background_max_pages);
			goto exit;
		}

		alloc = swap_reg->gpu_alloc;
		if (alloc == NULL) {
			dev_err(ctx->kbdev->dev, "alloc is null ctx->tgid is %p tgid is %d\n", ctx, ctx->tgid);
			return -1;
		}

		ret = kbase_mmu_insert_pages(ctx->kbdev, &ctx->mmu, swap_reg->start_pfn,
					kbase_get_gpu_phy_pages(swap_reg),
					kbase_reg_current_backed_size(swap_reg), swap_reg->flags,
					ctx->as_nr, alloc->group_id, mmu_sync_info, swap_reg, false, alloc->header_page_number);
		if (ret != 0) {
			dev_err(ctx->kbdev->dev, "kbase_mmu_insert_pages fialed! start_pfn = 0x%lx nents = %lu flags is %lu\
				as_nr = %d group_id = %u\n", swap_reg->start_pfn, kbase_reg_current_backed_size(swap_reg),
				swap_reg->flags, ctx->as_nr, alloc->group_id);
			kbase_switch_update_stat_info(fsinfo, CTX_STATUS_ERROR, "mmu_insert_pages failed.");
			goto exit;
		}

		for (idx = 0; idx < alloc->nents; idx++)
			page_array[start_idx + idx] = as_page(alloc->pages[idx]);
		start_idx += alloc->nents;
		list_del(&swap_reg->swapped_node);
		swap_reg->swap_out = false;
	}
	mutex_unlock(&ctx->kctx_sw.swapped_list_mutex);
	*set_up_pages = start_idx;
	kbase_switch_fill_in_used_time(fsinfo, SETUP_ALLOC_PAGE, begin);
	return 0;
exit:
	mutex_unlock(&ctx->kctx_sw.swapped_list_mutex);
	return KBASE_SW_OPER_FAILED;
}

static void kbase_switch_free_pages(uint32_t swap_pages, struct kbase_context *ctx,
	struct kbase_switch_fs *fsinfo, ktime_t *begin)
{
	struct kbase_va_region *swap_reg = NULL;
	struct kbase_va_region *tmp_reg = NULL;
	struct kbase_mem_phy_alloc *alloc = NULL;

	kbase_switch_fill_in_used_time(fsinfo, ARRAY_OUT_PAGE, begin);
	mutex_lock(&ctx->kctx_sw.swapped_list_mutex);
	list_for_each_entry_safe(swap_reg, tmp_reg, &ctx->kctx_sw.swapped_list, swapped_node) {
		alloc = swap_reg->gpu_alloc;
		if (alloc) {
			kbase_free_phy_pages_helper(alloc, alloc->nents);
		}
	}
	mutex_unlock(&ctx->kctx_sw.swapped_list_mutex);
	kbase_switch_fill_in_used_time(fsinfo, FREE_PAGE, begin);
	fsinfo->real_swaped_pages = swap_pages;
	kbase_switch_update_stat_info(fsinfo, CTX_IS_BACK_GROUND, "switch to background succeeded");
}

static long kbase_switch_swap_in_page(struct kbase_context *ctx, struct page **page_array,
	struct kbase_switch_fs *fsinfo, ktime_t *begin)
{
	uint32_t real_pages = ctx->kctx_sw.real_swaped_pages;
	uint32_t batchs = real_pages / BATCH_SWAP_SIZE;
	long ret;
	uint32_t idx;

	for (idx = 0; idx < batchs; idx++) {
		ret = mm_gpu_page_array_in(&page_array[idx * BATCH_SWAP_SIZE], (int)BATCH_SWAP_SIZE,
			&ctx->kctx_sw.extendid[idx * BATCH_SWAP_SIZE]);
		if (ret != 0) {
			dev_err(ctx->kbdev->dev, "[ctx_sw]ctx_%u array in fail, idx:%u ret:%ld", ctx->id, idx, ret);
			goto free_extid;
		}
	}

	if ((real_pages % BATCH_SWAP_SIZE) != 0U) {
		ret = mm_gpu_page_array_in(&page_array[idx * BATCH_SWAP_SIZE],
			(int)real_pages % BATCH_SWAP_SIZE,
			&ctx->kctx_sw.extendid[idx * BATCH_SWAP_SIZE]);
		if (ret != 0) {
			dev_err(ctx->kbdev->dev, "[ctx_sw]ctx_%u array in final fail, idx:%u, ret:%ld", ctx->id, idx, ret);
			goto free_extid;
		}
	}
	kbase_switch_fill_in_used_time(fsinfo, ARRAY_IN_PAGE, begin);

	return 0;

free_extid:
	if (idx < batchs)
		kbase_switch_free_extid_batch(idx, real_pages - idx * BATCH_SWAP_SIZE, ctx);
	kbase_switch_update_stat_info(fsinfo, CTX_STATUS_ERROR, "array in failed.");
	return KBASE_SW_OPER_FAILED;
}

static long kbase_mem_pool_shrink_pages(struct kbase_context *ctx)
{
	long reclaim_pages = 0;

	for (int i = 0; i < MEMORY_GROUP_MANAGER_NR_GROUPS; i++) {
		if (&ctx->mem_pools.small[i] != NULL) {
			dev_dbg(ctx->kbdev->dev, "small pool id is %d pool->cur_size = %lu, max_size = %lu\n",
				i, ctx->mem_pools.small[i].cur_size, ctx->mem_pools.small[i].max_size);
			reclaim_pages += kbase_mem_pool_shrink(&ctx->mem_pools.small[i], ctx->mem_pools.small[i].cur_size);
		}
		if (&ctx->mem_pools.large[i] != NULL) {
			dev_dbg(ctx->kbdev->dev, "large pool id is %d pool->cur_size = %lu, max_size = %lu\n",
				i, ctx->mem_pools.large[i].cur_size, ctx->mem_pools.large[i].max_size);
			reclaim_pages += kbase_mem_pool_shrink(&ctx->mem_pools.large[i], ctx->mem_pools.large[i].cur_size);
		}
	}
	dev_info(ctx->kbdev->dev, "kbase_mem_pool_shrink_pages finish reclaim_pages = %ld\n", reclaim_pages);
	return reclaim_pages;
}

static bool kbase_queue_group_is_idle(struct kbase_context * const ctx,
	struct kbase_switch_fs *fsinfo)
{
	struct kbase_device *kbdev = ctx->kbdev;
	struct kbase_csf_scheduler *scheduler = &kbdev->csf.scheduler;
	struct kbase_context *kctx;

	lockdep_assert_held(&scheduler->lock);
	for (int i = 0; i < KBASE_QUEUE_GROUP_PRIORITY_COUNT; ++i) {
		list_for_each_entry(kctx, &scheduler->runnable_kctxs, csf.link) {
			struct kbase_queue_group *group;
			if (kctx == ctx) {
				list_for_each_entry(group, &kctx->csf.sched.runnable_groups[i], link) {
					if (group->run_state != KBASE_CSF_GROUP_SUSPENDED_ON_IDLE) {
						dev_err(kbdev->dev, "run_state is not idle, group is active!\n");
						return false;
					}
				}
			}
		}
	}

	return true;
}

static bool kbase_switch_ctx_in_background(struct kbase_context * const ctx,
	struct kbase_switch_fs *fsinfo)
{
	if (kbase_switch_get_status(ctx) != CTX_IS_FORE_GROUND) {
		kbase_switch_update_stat_info(fsinfo, CTX_STATUS_ERROR,	"ctx has been reclaimed.");
		dev_info(ctx->kbdev->dev, "[ctx_sw]ctx_%u has been reclaimed. status is %d\n",
			ctx->id, kbase_switch_get_status(ctx));
		return false;
	}

	if (!kbase_queue_group_is_idle(ctx, fsinfo)) {
		kbase_switch_update_stat_info(fsinfo, CTX_STATUS_ERROR,	"queue group is not idle.");
		dev_info(ctx->kbdev->dev, "[ctx_sw]ctx_%u is not idle\n", ctx->id);
		return false;
	}

	if (atomic_read(&ctx->kctx_sw.ctx_foreground_flag) == 1) {
		kbase_switch_update_stat_info(fsinfo, CTX_STATUS_ERROR, "ctx sw to front.");
		dev_info(ctx->kbdev->dev, "[ctx_sw]ctx_%u is set foreground_flag\n", ctx->id);
		return false;
	}

	return true;
}

long kbase_switch_to_frontground(struct kbase_context * const ctx, pid_t tgid)
{
	struct kbase_context *app_context = NULL;
	struct page **sw_page_array = NULL;
	uint32_t set_up_pages = 0;
	long ret = KBASE_SW_OPER_FAILED;
	struct kbase_switch_fs fsinfo;
	ktime_t begin = 0;

	kbase_switch_prev_ktime_init(&begin);
	(void)memset_s(&fsinfo, sizeof(fsinfo), 0, sizeof(fsinfo));
	fsinfo.tgid = tgid;

	app_context = kbase_switch_get_background_ctx(ctx, tgid);
	if (unlikely(app_context == NULL)) {
		dev_err(ctx->kbdev->dev, "[ctx_sw]ctx or kctx_sw is null.");
		ret = 0;
		goto para_invalid;
	}

	atomic_set(&app_context->kctx_sw.ctx_foreground_flag, 1);
	mutex_lock(&app_context->kctx_sw.context_switch_mutex);
	if (kbase_switch_get_status(app_context) != CTX_IS_BACK_GROUND) {
		kbase_switch_update_stat_info(&fsinfo, CTX_STATUS_ERROR, "ctx status invalid.");
		dev_err(ctx->kbdev->dev, "[ctx_sw]ctx is not background. status is %d", kbase_switch_get_status(app_context));
		ret = 0;
		goto quick_unlock;
	}

	sw_page_array = vmalloc(app_context->kctx_sw.background_max_pages * sizeof(struct page *));
	if (unlikely(sw_page_array == NULL)) {
		dev_err(ctx->kbdev->dev, "[ctx_sw] alloc sw page array failed.");
		goto free_swapped_page_array;
	}

	if (kbase_switch_set_up_page_table(app_context, sw_page_array, &set_up_pages, &fsinfo, &begin) != 0) {
		kbase_switch_update_stat_info(&fsinfo, CTX_STATUS_ERROR, "set up pt failed.");
		goto free_swapped_page_array;
	}

	if (set_up_pages != app_context->kctx_sw.real_swaped_pages) {
		dev_err(ctx->kbdev->dev, "[ctx_sw] ctx_%u pages unmatched,%u %u",
			app_context->id, set_up_pages, app_context->kctx_sw.real_swaped_pages);
		kbase_switch_update_stat_info(&fsinfo, CTX_STATUS_ERROR, "pages not match.");
		goto pages_no_match;
	}

	if (kbase_switch_swap_in_page(app_context, sw_page_array, &fsinfo, &begin) != 0) {
		goto release_switch_data;
	}

	kbase_switch_sw_data_release(app_context);
	vfree(sw_page_array);
	kbase_switch_set_status(app_context, CTX_IS_FORE_GROUND);

	app_context->kctx_sw.is_background = false;
	atomic_set(&app_context->kctx_sw.ctx_foreground_flag, 0);

	kbase_switch_update_stat_info(&fsinfo, CTX_IS_FORE_GROUND, "switch to frontground succeeded");
	kbase_switch_update_front_info(app_context, &fsinfo);
	mutex_unlock(&app_context->kctx_sw.context_switch_mutex);
	kbase_switch_list_del(app_context->kbdev, app_context);
	return 0;

free_swapped_page_array:
	kbase_switch_remove_swapped_node(app_context);
pages_no_match:
	kbase_switch_free_extid_batch(0, app_context->kctx_sw.real_swaped_pages, app_context);
release_switch_data:
	kbase_switch_sw_data_release(app_context);
	kbase_switch_set_status(app_context, CTX_IS_FORE_GROUND);
quick_unlock:
	atomic_set(&app_context->kctx_sw.ctx_foreground_flag, 0);
	kbase_switch_update_front_info(app_context, &fsinfo);
	if (sw_page_array != NULL)
		vfree(sw_page_array);
	mutex_unlock(&app_context->kctx_sw.context_switch_mutex);
	kbase_switch_list_del(ctx->kbdev, app_context); 
para_invalid:
	return ret;
}

long kbase_switch_to_background(struct kbase_context * const ctx,
	struct kbase_ioctl_para_ctx_sw * const para)
{
	struct kbase_device *kbdev = ctx->kbdev;
	struct kbase_context *kctx = NULL;
	uint32_t swap_out_pages = 0;
	uint32_t all_recycle_pages = 0;
	struct page **sw_page_array = NULL;
	struct kbase_switch_fs fsinfo;
	ktime_t begin = 0;

	long ret = KBASE_SW_OPER_FAILED;
	long ret1;

	kbase_switch_prev_ktime_init(&begin);

	(void)memset_s(&fsinfo, sizeof(fsinfo), 0, sizeof(fsinfo));
	fsinfo.tgid = para->in.tgid;

	if (!kbase_switch_ratio_is_valid(para->in.ratio, ctx, &fsinfo)) {
		dev_err(kbdev->dev, "swtich ratio is error!\n");
		goto para_invalid;
	}
	kctx = kbase_switch_get_ctx(ctx, para->in.tgid, &fsinfo);
	if (unlikely(kctx == NULL)) {
		goto para_invalid;
	}

	kbase_gpu_vm_lock(kctx);
	mutex_lock(&kctx->kctx_sw.context_switch_mutex);
	kctx->kctx_sw.is_background = true;
	if (!kbase_switch_ctx_in_background(kctx, &fsinfo)) {
		goto quick_exit;
	}

	if (kbase_switch_add_ctx_to_list(kbdev, para->in.tgid, kctx, &fsinfo, &begin) != 0) {
		goto quick_exit;
	}

	if (kbase_switch_sw_data_init(kctx, para->in.ratio, &fsinfo) != 0) {
		goto del_ctx_list;
	}

	sw_page_array = vmalloc(kctx->kctx_sw.background_max_pages * sizeof(struct page *));
	if (unlikely(sw_page_array == NULL)) {
		dev_err(kbdev->dev, "vmalloc sw_page_array failed!\n");
		kbase_switch_update_stat_info(&fsinfo, CTX_STATUS_ERROR, "sw init failed.");
		goto release_sw_data;
	}

	fsinfo.same_va_total_pages = (uint32_t)atomic_read(&kctx->kctx_sw.same_va_total_pages);
	ret = kbase_switch_teardown_ptes(kctx, sw_page_array, &fsinfo, &begin);
	if (ret != 0) {
		kbase_switch_update_stat_info(&fsinfo, CTX_STATUS_ERROR, "teardown_ptes failed.");
		goto resume_pte;
	}

	if (kbase_switch_swap_out_page(kctx, &swap_out_pages, sw_page_array) != 0) {
		kbase_switch_update_stat_info(&fsinfo, CTX_STATUS_ERROR, "array out page failed.");
		goto release_array_out_data;
	}

	kbase_switch_free_pages(swap_out_pages, kctx, &fsinfo, &begin);
	vfree(sw_page_array);
	all_recycle_pages += swap_out_pages;
	para->out.background_pages = all_recycle_pages;
	kbase_switch_set_status(kctx, CTX_IS_BACK_GROUND);
	kbase_switch_list_add_fsinfo(kctx, &fsinfo);
	mutex_unlock(&kctx->kctx_sw.context_switch_mutex);
	kbase_gpu_vm_unlock(kctx);
	kbase_mem_pool_shrink_pages(kctx); // free pool all pages
	dev_info(kbdev->dev, "tgid : %u switch background success ctx->used_pages = %d\n",
		para->in.tgid, atomic_read(&kctx->used_pages));
	return 0;

release_array_out_data:
	if (swap_out_pages != 0)
		kbase_switch_free_extid_batch(0, swap_out_pages, kctx);
resume_pte:
	kbase_switch_set_status(kctx, CTX_STATUS_ERROR);
	ret1 = kbase_switch_resume_swaped_area_pt(kctx);
	if (ret == KBASE_SW_OPER_FAILED_NEED_CLOSE_CTX || ret1 != 0) {
		kbase_switch_remove_swapped_node(kctx);
		ret = KBASE_SW_OPER_FAILED_NEED_CLOSE_CTX;
	} else {
		ret = KBASE_SW_OPER_FAILED;
	}
    vfree(sw_page_array);
release_sw_data:
	kbase_switch_sw_data_release(kctx);
del_ctx_list:
	kbase_switch_list_del(kbdev, kctx);
	kbase_switch_set_status(kctx, CTX_IS_FORE_GROUND);
quick_exit:
	if (kctx->kctx_sw.status != CTX_IS_BACK_GROUND)
		kctx->kctx_sw.is_background = false;
	kbase_switch_list_add_fsinfo(kctx, &fsinfo);
	mutex_unlock(&kctx->kctx_sw.context_switch_mutex);
	kbase_gpu_vm_unlock(kctx);
para_invalid:
	para->out.background_pages = 0;
	pr_err("[mali] [ctx_sw]switch to background failed,%u,:%ld.", para->in.tgid, ret);
	return ret;
}

void kbase_context_switch_term(struct kbase_context * const sw_ctx)
{
	struct page **sw_page_array = NULL;
	uint32_t set_up_pages = 0;
	struct kbase_switch_fs fsinfo;
	ktime_t begin = 0;

	kbase_switch_prev_ktime_init(&begin);
	(void)memset_s(&fsinfo, sizeof(fsinfo), 0, sizeof(fsinfo));

	kbase_gpu_vm_lock(sw_ctx);
	mutex_lock(&sw_ctx->kctx_sw.context_switch_mutex);
	if (kbase_switch_get_status(sw_ctx) != CTX_IS_BACK_GROUND)
		goto release_lock;
	kbase_switch_list_del(sw_ctx->kbdev, sw_ctx);

	fsinfo.tgid = sw_ctx->tgid;
	sw_page_array = vmalloc(sw_ctx->kctx_sw.background_max_pages * sizeof(struct page *));
	if (unlikely(sw_page_array == NULL)) {
		dev_err(sw_ctx->kbdev->dev, "close ctx alloc sw page array failed.\n");
		goto free_swapped_page_array;
	}

	if (kbase_switch_set_up_page_table(sw_ctx, sw_page_array, &set_up_pages, &fsinfo, &begin) != 0) {
		dev_err(sw_ctx->kbdev->dev, "close ctx kbase_switch_set_up_page_table failed.\n");
		goto free_swapped_page_array;
	}

	if (set_up_pages != sw_ctx->kctx_sw.real_swaped_pages) {
		dev_err(sw_ctx->kbdev->dev, "[close ctx]ctx_%u pages unmatched,%u %u\n",
			sw_ctx->id, set_up_pages, sw_ctx->kctx_sw.real_swaped_pages);
		goto pages_no_match;
	}

	if (kbase_switch_swap_in_page(sw_ctx, sw_page_array, &fsinfo, &begin) != 0) {
		dev_err(sw_ctx->kbdev->dev, "kbase_switch_swap_in_page failed!\n");
		goto release_switch_data;
	}

	kbase_switch_sw_data_release(sw_ctx);
	vfree(sw_page_array);
	kbase_switch_set_status(sw_ctx, CTX_STATUS_TERM);
	kbase_switch_update_stat_info(&fsinfo, CTX_STATUS_TERM, "kill ctx succeeded");
	kbase_switch_update_front_info(sw_ctx, &fsinfo);
	mutex_unlock(&sw_ctx->kctx_sw.context_switch_mutex);
	kbase_gpu_vm_unlock(sw_ctx);

	return;

free_swapped_page_array:
	kbase_switch_remove_swapped_node(sw_ctx);
pages_no_match:
	kbase_switch_free_extid_batch(0, sw_ctx->kctx_sw.real_swaped_pages, sw_ctx);
release_switch_data:
	kbase_switch_sw_data_release(sw_ctx);
	if (sw_page_array != NULL)
		vfree(sw_page_array);
	kbase_switch_set_status(sw_ctx, CTX_IS_FORE_GROUND);
	kbase_switch_update_stat_info(&fsinfo, CTX_IS_FORE_GROUND, "kill ctx failed.");
	kbase_switch_update_front_info(sw_ctx, &fsinfo);
release_lock:
	mutex_unlock(&sw_ctx->kctx_sw.context_switch_mutex);
	kbase_gpu_vm_unlock(sw_ctx);
}

uint32_t kbase_switch_get_ctx_pages(struct kbase_context *ctx)
{
	uint32_t pages = 0;
	struct rb_root *rbtree = NULL;
	struct rb_node *parent = NULL;
	struct kbase_va_region *reg = NULL;
	uint32_t nent_pages = 0;

	kbase_gpu_vm_lock(ctx);
	rbtree = &ctx->reg_rbtree_same;
	parent = rb_first(rbtree);
	while (parent != NULL) {
		reg = rb_entry(parent, struct kbase_va_region, rblink);
		parent = rb_next(parent);
		if (!kbase_is_region_invalid_or_free(reg) && reg->nr_pages && reg->gpu_alloc != NULL) {
			if (!is_native_type(reg->gpu_alloc) || (reg->nr_pages != reg->gpu_alloc->nents)) {
				continue;
			}
			nent_pages += reg->gpu_alloc->nents;
			if (!ctx->kctx_sw.first_reg_nents) {
				ctx->kctx_sw.first_reg_nents = reg->gpu_alloc->nents;
			}
		}  
	}
	kbase_gpu_vm_unlock(ctx);
	atomic_set(&ctx->kctx_sw.same_va_total_pages, nent_pages);
	return nent_pages;
}

int kbase_context_switch_init(struct kbase_context *kctx)
{
	struct kbase_context_switch *kctx_sw = NULL;
	if (WARN_ON(!kctx))
		return -EINVAL;

	if (WARN_ON(!kctx->kbdev))
		return -EINVAL;

	kctx->kctx_sw.first_reg_nents = 0;
	INIT_LIST_HEAD(&kctx->kctx_sw.swapped_list);
	mutex_init(&kctx->kctx_sw.context_switch_mutex);
	mutex_init(&kctx->kctx_sw.swapped_list_mutex);
	atomic_set(&kctx->kctx_sw.same_va_total_pages, 0);
	atomic_set(&kctx->kctx_sw.ctx_foreground_flag, 0);

	return 0;
}
