/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2021-2022. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */

#include "hvgr_mem_api.h"

#include <linux/kernel.h>
#include <linux/mman.h>
#include <linux/slab.h>
#include <linux/dma-mapping.h>
#include <linux/highmem.h>
#include <linux/module.h>
#include <linux/statfs.h>
#include <linux/vmalloc.h>
#include <linux/uaccess.h>
#include <linux/of.h>
#include <linux/lockdep.h>
#include <linux/rcupdate.h>
#include <linux/sched/mm.h>
#include <linux/mm.h>
#include <securec.h>
#include <linux/bitmap.h>

#include "hvgr_kmd_defs.h"
#include "hvgr_ioctl_mem.h"
#include "hvgr_regmap.h"
#include "hvgr_mem_opts.h"
#include "hvgr_mem_zone.h"
#include "hvgr_mmu_api.h"
#include "hvgr_mem_drv.h"
#include "hvgr_mem_4gva.h"
#include "hvgr_mem_sameva.h"
#include "hvgr_mem_jit.h"
#include "hvgr_mem_sparse.h"
#include "hvgr_mem_replay.h"
#include "hvgr_mem_gpu.h"
#include "hvgr_mem_import.h"
#include "hvgr_mem_2m.h"
#include "hvgr_mem_alias.h"
#include "hvgr_memory_page.h"
#include "hvgr_mem_debugfs.h"
#include "hvgr_mem_pool.h"
#include "hvgr_mem_space.h"
#include "hvgr_kv.h"
#include "hvgr_dm_api.h"
#include "hvgr_log_api.h"
#include "hvgr_asid_api.h"
#include "hvgr_pm_api.h"
#include "hvgr_debugfs_api.h"
#include "hvgr_protect_mode.h"
#include "hvgr_mem_interval_tree.h"
#include "hvgr_platform_api.h"
#if defined(HVGR_STATE_BUNDLE2) && HVGR_STATE_BUNDLE2 > 0
#include "hvgr_mcu_common.h"
#endif

struct hvgr_mem_area *hvgr_mem_new_area(struct hvgr_mem_ctx *mem_ctx,
	struct hvgr_mem_alloc_para * const para)
{
	struct hvgr_mem_area *area = NULL;
	struct hvgr_mem_zone *zone = hvgr_mem_zone_get_zone(mem_ctx, para->property);

	if (unlikely(zone == NULL))
		return NULL;

	area = hvgr_mem_alloc_area();
	if (area == NULL)
		return NULL;

	area->zone = zone;
	area->property = para->property;
	area->attribute = para->attribute;
	area->pages = para->pages;
	area->max_pages = para->max_pages;
	area->extent = para->extent;
	area->gva = para->va;
	area->phy_pages = para->phy_pages;
	if(area->property == HVGR_MEM_PROPERTY_2M_PAGE)
		area->order = HVGR_2M_PAGE_ORDER;

	area->is_freed = false;
	area->mmap_cnt = 0;
	area->pt_map_nr = 0;
	area->pt_unmap_nr = 0;
	area->is_record = false;
	atomic_set(&area->import.data.umm.free_tlb_cnt, 0);
	atomic_set(&area->import.data.umm.restore_tlb_cnt, 0);
	area->import.data.umm.cnt = 0;
	atomic_set(&area->growable_pages, 0);

	/* init 1 is gpu pgtable ref, must do kref_put after destroy pgtable */
	kref_init(&area->kref);

	return area;
}

void hvgr_mem_delete_area(struct hvgr_mem_area * const area)
{
	if (area == NULL)
		return;

	hvgr_mem_free_area(area);
}

static struct hvgr_mem_area *hvgr_mem_alloc_import(struct hvgr_ctx * const ctx,
	struct hvgr_mem_alloc_para * const para, struct hvgr_mem_area_va *area_va, bool user_buf)
{
	struct hvgr_mem_ctx *pmem = NULL;
	struct hvgr_mem_area *area = NULL;

	pmem = &ctx->mem_ctx;
	hvgr_systrace_begin("%s lock", __func__);
	mutex_lock(&pmem->area_mutex);
	hvgr_systrace_end();
	hvgr_systrace_begin("%s 0x%lx:%u", __func__, para->va, para->pages);
	area = hvgr_mem_zone_find_area_by_gva(pmem, para->va);
	if (area == NULL || area->property != HVGR_MEM_PROPERTY_IMPORT) {
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u alloc import mem invalid va 0x%lx",
			ctx->id, para->va);
		mutex_unlock(&pmem->area_mutex);
		hvgr_systrace_end();
		return NULL;
	}

	if (hvgr_mem_zone_alloc_memory(area) != 0) {
		hvgr_err(ctx->gdev, HVGR_MEM,
			"ctx_%u alloc import mem fail. prot=%lu:%lx, page:%lu/%lu va 0x%lx",
			ctx->id, area->property, area->attribute,
			area->pages, area->max_pages, area->gva);
		mutex_unlock(&pmem->area_mutex);
		hvgr_systrace_end();
		return NULL;
	}

	if (area_va != NULL) {
		area_va->gva = area->gva;
		area_va->kva = 0U;
	}

	if (user_buf)
		area->user_buf = true;
	hvgr_info(ctx->gdev, HVGR_MEM,
		"ctx_%u alloc import mem prot=%lu:0x%lx, page:%lu/%lu uva/gva=0x%lx",
		ctx->id, area->property, area->attribute, area->pages,
		area->max_pages, area->gva);
	mutex_unlock(&pmem->area_mutex);

	hvgr_systrace_end();
	return area;
}

#ifdef CONFIG_LIBLINUX
static bool hvgr_mem_max_pages_check(struct hvgr_ctx * const ctx, uint64_t max_pages)
{
	if (unlikely(max_pages > ctx->gdev->mem_dev.threshold_page)) {
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u alloc max_pages %llu > %llu",
			ctx->id, max_pages, ctx->gdev->mem_dev.threshold_page);
		return false;
	}

	return true;
}
#endif

static bool hvgr_mem_allocate_check_param(struct hvgr_ctx * const ctx,
	struct hvgr_mem_alloc_para * const para)
{
	if ((ctx == NULL) || (para == NULL))
		return false;

	if (para->max_pages < para->pages || para->max_pages == 0U) {
		hvgr_err(ctx->gdev, HVGR_MEM,
			"ctx_%u prot=%lu:%lx err pages num %lu:%lu",
			ctx->id, para->property, para->attribute, para->pages, para->max_pages);
		return false;
	}

#ifdef CONFIG_LIBLINUX
	if (!hvgr_mem_max_pages_check(ctx, (uint64_t)para->max_pages))
		return false;
#endif
	if ((para->attribute & HVGR_MEM_ATTR_COHERENT_SYS_REQ) != 0 &&
		!hvgr_mem_is_cpu_coherent(ctx->gdev)) {
		hvgr_err(ctx->gdev, HVGR_MEM,
			"ctx_%u prot=%lu:%lx need coherent system in no coherent dev.",
			ctx->id, para->property, para->attribute);
		return false;
	}

	if ((para->attribute & HVGR_MEM_ATTR_COHERENT_SYSTEM) != 0 &&
		!hvgr_mem_is_cpu_coherent(ctx->gdev))
		para->attribute &= (~HVGR_MEM_ATTR_COHERENT_SYSTEM);

	if ((para->attribute & HVGR_MEM_ATTR_GPU_EXEC) != 0 &&
		(para->attribute & HVGR_MEM_ATTR_GPU_WR) != 0) {
		hvgr_err(ctx->gdev, HVGR_MEM,
			"ctx_%u prot=%lu:%lx gpu exe is conflict with gpu write.",
			ctx->id, para->property, para->attribute);
		return false;
	}

	return true;
}

struct hvgr_mem_area *hvgr_mem_allocate(struct hvgr_ctx * const ctx,
	struct hvgr_mem_alloc_para * const para, struct hvgr_mem_area_va *area_va, bool user_buf)
{
	long ret;
	uint64_t gva;
	struct hvgr_mem_area *area = NULL;
	struct hvgr_mem_ctx *mem_ctx = &ctx->mem_ctx;

	hvgr_systrace_begin("%s %lu:0x%lx %lu:%lu",
		__func__, para->property, para->attribute, para->pages, para->max_pages);
	if (!hvgr_mem_allocate_check_param(ctx, para)) {
		hvgr_systrace_end();
		return NULL;
	}

	if (para->property == HVGR_MEM_PROPERTY_IMPORT) {
		area = hvgr_mem_alloc_import(ctx, para, area_va, user_buf);
		hvgr_systrace_end();
		return area;
	}

	if (hvgr_ctx_flag(ctx, HVGR_CTX_FLAG_COMPAT) && para->property == HVGR_MEM_PROPERTY_4G_VA)
		para->property = HVGR_MEM_PROPERTY_SAME_VA;

	area = hvgr_mem_new_area(mem_ctx, para);
	if (unlikely(area == NULL)) {
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u alloc area for prot=%lu:%lx FAIL.",
			ctx->id, para->property, para->attribute);
		hvgr_systrace_end();
		return NULL;
	}

	if (hvgr_mem_need_alloc_pages(area)) {
		ret = hvgr_mem_alloc_pages(area);
		if (ret != 0)
			goto alloc_pages_fail;
	}

	ret = hvgr_mem_zone_alloc_memory(area);
	if (ret != 0)
		goto alloc_mem_fail;

	if ((area->attribute & HVGR_MEM_ATTR_KMD_ACCESS) != 0 && area->kva != 0)
		memset_s(u64_to_ptr(area->kva), area->pages << PAGE_SHIFT,
			0, area->pages << PAGE_SHIFT);

	hvgr_info(ctx->gdev, HVGR_MEM,
		"%s:%d ctx_%u alloc mem area_ptr 0x%lx prot=%lu:0x%lx, page:%lu/%lu uva/gva=0x%lx mmu_flag=0x%lx",
		__func__, __LINE__, ctx->id, (uint64_t)area, area->property, area->attribute, area->pages,
		area->max_pages, area->gva, area->mmu_flag);

	hvgr_systrace_begin("%s lock", __func__);
	mutex_lock(&mem_ctx->area_mutex);
	hvgr_systrace_end();
	if (area_va != NULL) {
		/* for same va, gva is the area cookie */
		area_va->gva = area->gva;
		area_va->kva = area->kva;
	}

	area->user_buf = user_buf;

	if (hvgr_mem_rec_area_in_mmap(area)) {
		gva = hvgr_mem_pend_area_add(mem_ctx, area);
		if (gva == 0)
			goto add_area_fail;

		if (area_va != NULL)
			area_va->gva = gva;
	} else {
		area->node.key = area->gva;
		area->node.size = area->max_pages << PAGE_SHIFT;
		ret = hvgr_mem_add_to_map_rec(ctx, area);
		if (unlikely(ret != 0)) {
			hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u mem alloc add map_rec fail", ctx->id);
			goto add_area_fail;
		}
	}

	mutex_unlock(&mem_ctx->area_mutex);
	hvgr_systrace_end();
	return area;
add_area_fail:
	if ((area->attribute & HVGR_MEM_MASK_KMD_ACCESS) != 0U)
		hvgr_mem_unmap_kva(area);
	mutex_unlock(&mem_ctx->area_mutex);
alloc_mem_fail:
	if (hvgr_mem_need_alloc_pages(area))
		(void)hvgr_mem_free_pages(area);
alloc_pages_fail:
	hvgr_mem_delete_area(area);
	hvgr_systrace_end();
	return NULL;
}

long hvgr_mem_free_area_nolock(struct hvgr_ctx * const ctx, struct hvgr_mem_area *area)
{
	if (hvgr_mem_in_pend_area(&ctx->mem_ctx, area)) {
		hvgr_err(ctx->gdev, HVGR_MEM, "%s:%d ctx_%u free area 0x%lx:0x%lx:0x%lx:0x%lx:0x%lx in pend_area",
			__func__, __LINE__, ctx->id, area->property, area->attribute, area->pages,
			area->max_pages, area->gva);
		return -1;
	}

	hvgr_info(ctx->gdev, HVGR_MEM, "%s:%d ctx_%u ioctl free mem prot=%lu:0x%lx, page:%lu/%lu uva/gva=0x%lx",
		__func__, __LINE__, ctx->id, area->property, area->attribute, area->pages,
		area->max_pages, area->gva);

	hvgr_mem_zone_free_memory(area);

	return 0;
}

long hvgr_mem_free(struct hvgr_ctx * const ctx, uint64_t gva)
{
	struct hvgr_mem_area *area = NULL;

	if (unlikely(ctx == NULL) || (gva == 0))
		return -EINVAL;

	hvgr_systrace_begin("%s lock", __func__);
	mutex_lock(&ctx->mem_ctx.area_mutex);
	hvgr_systrace_end();
	hvgr_systrace_begin("%s 0x%lx", __func__, gva);
	area = hvgr_mem_zone_find_area_by_gva(&ctx->mem_ctx, gva);
	if (area == NULL) {
		hvgr_err(ctx->gdev, HVGR_MEM, "%s:%d ctx_%u no area for gva 0x%llx",
			__func__, __LINE__, ctx->id, gva);
		mutex_unlock(&ctx->mem_ctx.area_mutex);
		hvgr_systrace_end();
		return -EINVAL;
	}
	hvgr_mem_del_from_map_rec(ctx, area);
	mutex_unlock(&ctx->mem_ctx.area_mutex);

	(void)hvgr_mem_free_area_nolock(ctx, area);
	hvgr_systrace_end();
	return 0;
}

long hvgr_jit_mem_free(struct hvgr_ctx * const ctx,
	uint64_t gva)
{
	struct hvgr_mem_area *area = NULL;

	if ((ctx == NULL) || (gva == 0))
		return -EINVAL;

	mutex_lock(&ctx->mem_ctx.area_mutex);
	area = hvgr_mem_zone_find_area_by_gva(&ctx->mem_ctx, gva);
	if (area == NULL) {
		mutex_unlock(&ctx->mem_ctx.area_mutex);
		return -EINVAL;
	}
	if (area->property != HVGR_MEM_PROPERTY_JIT) {
		mutex_unlock(&ctx->mem_ctx.area_mutex);
		return 0;
	}
	hvgr_mem_del_from_map_rec(ctx, area);
	mutex_unlock(&ctx->mem_ctx.area_mutex);

	return hvgr_mem_free_area_nolock(ctx, area);
}

void hvgr_mem_sync_page(struct hvgr_ctx * const ctx, struct page *page, uint64_t offset,
	uint64_t size, uint64_t direct)
{
	dma_addr_t dma_addr = hvgr_mem_get_page_dma_addr(page);

	dma_addr += offset;

	if (direct == HVGR_MEM_SYNC_TO_CPU)
		dma_sync_single_for_cpu(ctx->gdev->dev, dma_addr, size, DMA_BIDIRECTIONAL);
	else if (direct == HVGR_MEM_SYNC_TO_DEV)
		dma_sync_single_for_device(ctx->gdev->dev, dma_addr, size, DMA_BIDIRECTIONAL);
}

static struct hvgr_mem_area *hvgr_mem_get_mmap_src_area(struct hvgr_ctx * const ctx, uint64_t *uva,
	uint64_t size)
{
	struct vm_area_struct *vma = NULL;
	struct hvgr_mem_cpu_map *cpu_map = NULL;
	uint64_t offset;

	vma = find_vma_intersection(current->mm, *uva, (*uva + size));
	if (vma == NULL || vma->vm_start > *uva || vma->vm_ops != hvgr_mem_get_vm_opts() ||
		vma->vm_private_data == NULL) {
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u get mmap area key 0x%lx:0x%lx fail.",
			ctx->id, *uva, size);
		return NULL;
	}

	cpu_map = vma->vm_private_data;
	if (cpu_map->ctx != ctx) {
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u get cpu map key 0x%lx:0x%lx ctx not us.",
			ctx->id, *uva, size);
		return NULL;
	}

	offset = *uva - vma->vm_start + (vma->vm_pgoff << PAGE_SHIFT);
	*uva = offset;

	return cpu_map->area;
}

static void hvgr_mem_sync_pages(struct hvgr_ctx * const ctx, struct page **page_arr,
	uint64_t va, uint64_t offset_page, uint64_t offset_page_byte,
	uint64_t size, uint64_t sync_pages, uint64_t direct)
{
	uint64_t sync_size;
	uint64_t i;

	sync_size = (uint64_t)min(((uint64_t)PAGE_SIZE - offset_page_byte), size);
	hvgr_mem_sync_page(ctx, page_arr[offset_page], offset_page_byte, sync_size, direct);

	for (i = 1; sync_pages > 2 && i < sync_pages - 1; i++)
		hvgr_mem_sync_page(ctx, page_arr[offset_page + i], 0, PAGE_SIZE, direct);

	if (sync_pages > 1) {
		sync_size = ((va + size - 1) & (PAGE_SIZE - 1)) + 1;
		hvgr_mem_sync_page(ctx, page_arr[offset_page + sync_pages - 1], 0, sync_size,
			direct);
	}
}

long hvgr_mem_sync(struct hvgr_ctx * const ctx, uint64_t va, uint64_t size, uint64_t direct)
{
	struct hvgr_mem_area *sync_area = NULL;
	struct page **page_arr = NULL;
	uint64_t offset;
	uint64_t offset_page;
	uint64_t offset_page_byte;
	uint64_t sync_pages;
#ifdef CONFIG_LIBLINUX_HVGR_BATCH_SYNC_OPT
	bool sync_batch = true;
#endif
	long ret;

	hvgr_systrace_begin("%s 0x%lx:%lu", __func__, va, size);
	down_read(hvgr_mem_mm_map_lock());
	hvgr_systrace_begin("%s lock", __func__);
	mutex_lock(&ctx->mem_ctx.area_mutex);
	hvgr_systrace_end();
	sync_area = hvgr_mem_zone_find_area_by_gva_range(&ctx->mem_ctx, va);
	if (sync_area == NULL) {
		sync_area = hvgr_mem_get_mmap_src_area(ctx, &va, size);
		if (sync_area == NULL) {
			hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u sync no area with key 0x%lx.",
				ctx->id, va);
			ret = -EINVAL;
			goto exit_unlock;
		}
	}

	if (sync_area->property == HVGR_MEM_PROPERTY_DRIVER) {
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u sync unsupport prop %lu",
			ctx->id, sync_area->property);
		ret = -EINVAL;
		goto exit_unlock;
	}

	if (atomic_read(&sync_area->import.data.umm.free_tlb_cnt) != 0) {
		hvgr_err(ctx->gdev, HVGR_MEM, "dma buffer page was freed, memory sync was not allowed");
		ret = -EINVAL;
		goto exit_unlock;
	}

	if ((sync_area->attribute & HVGR_MEM_ATTR_CPU_CACHEABLE) == 0) {
		hvgr_info(ctx->gdev, HVGR_MEM, "ctx_%u sync area 0x%lx 0x%lx:0x%lx no cpu cache",
			ctx->id, sync_area->gva, sync_area->property, sync_area->attribute);
		ret = 0;
		goto exit_unlock;
	}

	if (size > (sync_area->max_pages << PAGE_SHIFT)) {
		hvgr_err(ctx->gdev, HVGR_MEM,
			"ctx_%u sync area with key 0x%lx size 0x%lx out of range.",
			ctx->id, va, size);
		ret = -EINVAL;
		goto exit_unlock;
	}

	offset = va - sync_area->gva;
	offset_page = offset >> PAGE_SHIFT;
	offset_page_byte = offset & (PAGE_SIZE - 1);
	sync_pages = (size + offset_page_byte + (PAGE_SIZE - 1)) >> PAGE_SHIFT;

	if (offset_page >= sync_area->pages || (offset_page + sync_pages) > sync_area->pages) {
		hvgr_err(ctx->gdev, HVGR_MEM,
			"ctx_%u Sync area with key 0x%lx sync pages out of 0x%lx.",
			ctx->id, va, sync_area->pages);
		ret = -EINVAL;
		goto exit_unlock;
	}

	page_arr = sync_area->page_array;
#ifndef CONFIG_LIBLINUX_HVGR_BATCH_SYNC_OPT
	hvgr_mem_sync_pages(ctx, page_arr, va, offset_page, offset_page_byte, size,
		sync_pages, direct);
#else
	if (sync_batch)
		hvgr_mem_sync_page_batch(ctx, page_arr, va, offset_page, offset_page_byte, size,
			sync_pages, direct);
	else
		hvgr_mem_sync_pages(ctx, page_arr, va, offset_page, offset_page_byte, size,
			sync_pages, direct);
#endif
	hvgr_info(ctx->gdev, HVGR_MEM,
		"ctx_%u sync mem prot=%lu:0x%lx, page:%lu/%lu uva/gva=0x%lx uva=0x%lx size=0x%lx direct=%lu",
		ctx->id, sync_area->property, sync_area->attribute, sync_area->pages,
		sync_area->max_pages, sync_area->gva, va, size, direct);
	ret = 0;
exit_unlock:
	mutex_unlock(&ctx->mem_ctx.area_mutex);
	up_read(hvgr_mem_mm_map_lock());
	hvgr_systrace_end();
	return ret;
}

long hvgr_mem_config_proc(struct hvgr_ctx * const ctx, struct hvgr_mem_cfg_para *para)
{
	struct hvgr_mem_ctx *pmem = NULL;

	if (unlikely(ctx == NULL || para == NULL))
		return -EINVAL;

	pmem = &ctx->mem_ctx;

	if (!hvgr_mem_replay_config(pmem, para))
		goto replay_fail;

	if (!hvgr_mem_driver_config(pmem))
		goto driver_fail;

	if (!hvgr_ctx_flag(ctx, HVGR_CTX_FLAG_COMPAT)) {
		if (!hvgr_mem_4gva_config(pmem))
			goto va4g_fail;
	}

	if (!hvgr_mem_sameva_config(pmem))
		goto sameva_fail;

	if (!hvgr_mem_jit_config(pmem, para))
		goto jit_fail;

	if (!hvgr_mem_sparse_config(pmem))
		goto sparse_fail;

	if (!hvgr_mem_gpu_config(pmem))
		goto gpu_fail;

	if (!hvgr_mem_import_config(pmem))
		goto import_fail;

	if (!hvgr_mem_alias_config(pmem))
		goto alias_fail;

	if (!hvgr_mem_2m_config(pmem))
		goto alias_fail;

	hvgr_info(ctx->gdev, HVGR_MEM, "%s:%d ctx_%u ioctl cfg ok", __func__, __LINE__, ctx->id);
	return 0;
alias_fail:
import_fail:
gpu_fail:
	hvgr_mem_sparse_deconfig(pmem);
sparse_fail:
	hvgr_mem_jit_deconfig(pmem);
jit_fail:
sameva_fail:
	hvgr_mem_4gva_deconfig(pmem);
va4g_fail:
driver_fail:
replay_fail:
	return -EPERM;
}

long hvgr_mem_deconfig_proc(struct hvgr_ctx * const ctx)
{
	struct hvgr_mem_ctx *pmem = NULL;

	if (ctx == NULL)
		return -EINVAL;

	pmem = &ctx->mem_ctx;
	mutex_lock(&pmem->area_mutex);
	if (!hvgr_ctx_flag(ctx, HVGR_CTX_FLAG_COMPAT))
		hvgr_mem_4gva_deconfig(pmem);
	hvgr_mem_jit_deconfig(pmem);
	mutex_unlock(&pmem->area_mutex);
#if hvgr_version_lt(HVGR_V300)
	hvgr_mem_protect_jcd_term(ctx);
#endif
	hvgr_info(ctx->gdev, HVGR_MEM, "%s:%d ctx_%u ioctl decfg ok", __func__, __LINE__, ctx->id);
	return 0;
}

static long hvgr_mem_init_process_res_cb(struct hvgr_ctx * const ctx)
{
	struct hvgr_mem_process_res_cb *res_cb = NULL;
	struct hvgr_kv_node *node = NULL;

	mutex_lock(&ctx->gdev->mem_dev.mem_process_res_lock);
	node = hvgr_kv_get(&ctx->gdev->mem_dev.mem_process_res_rec,
		(uint64_t)ctx->mem_ctx.process_mm);
	if (node != NULL) {
		res_cb = container_of(node, struct hvgr_mem_process_res_cb, kv_node);
		atomic_inc(&res_cb->ref_cnt);
		goto out;
	}

	res_cb = kvzalloc(sizeof(*res_cb), GFP_KERNEL);
	if (res_cb == NULL) {
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u alloc reserve cb fail", ctx->id);
		mutex_unlock(&ctx->gdev->mem_dev.mem_process_res_lock);
		return -ENOMEM;
	}

	res_cb->kv_node.key = (uint64_t)ctx->mem_ctx.process_mm;
	(void)hvgr_kv_add(&ctx->gdev->mem_dev.mem_process_res_rec, &res_cb->kv_node);
	atomic_set(&res_cb->ref_cnt, 1);
	atomic_set(&res_cb->rsv_cnt, 0);
out:
	ctx->mem_ctx.reserve_cb = res_cb;
#if defined(MALEOON_2D_MCU_FEATURE) && MALEOON_2D_MCU_FEATURE != 0
	mutex_init(&ctx->mem_ctx.reserve_cb->mcu_2d_mutex);
#endif
	mutex_unlock(&ctx->gdev->mem_dev.mem_process_res_lock);
	return 0;
}

static void hvgr_mem_term_process_res_cb(struct hvgr_ctx * const ctx)
{
	struct hvgr_mem_process_res_cb *res_cb = NULL;

	mutex_lock(&ctx->gdev->mem_dev.mem_process_res_lock);
	res_cb = ctx->mem_ctx.reserve_cb;
	if (res_cb != NULL && atomic_dec_and_test(&res_cb->ref_cnt)) {
		hvgr_kv_del(&ctx->gdev->mem_dev.mem_process_res_rec,
			res_cb->kv_node.key);
		kvfree(res_cb);
		ctx->mem_ctx.reserve_cb = NULL;
	}
	mutex_unlock(&ctx->gdev->mem_dev.mem_process_res_lock);
}

static unsigned long hvgr_mem_jit_shrink_count(struct shrinker *shrink,
	struct shrink_control *shrink_c)
{
	struct hvgr_mem_ctx *pmem = NULL;
	struct hvgr_mem_area *area = NULL;
	struct list_head *entry = NULL;
	unsigned long pages = 0;

	pmem = container_of(shrink, struct hvgr_mem_ctx, jit_shrink);

	mutex_lock(&pmem->jit_shrink_mutex);
	list_for_each_prev(entry, &pmem->jit_pool_head) {
		area = list_entry(entry, struct hvgr_mem_area, jit_node);
		pages += area->pages;
	}
	mutex_unlock(&pmem->jit_shrink_mutex);

	return pages;
}

static long hvgr_mem_jit_shrink(struct hvgr_ctx * const ctx, struct hvgr_mem_area *area,
	uint64_t pages)
{
	long ret;
	uint32_t policy_id = 0;
	uint64_t diff_page = area->pages - pages;
	uint64_t va_offset = (pages << PAGE_SHIFT);
	struct hvgr_mmu_takedown_paras paras;

	paras.va = (area->gva + va_offset);
	paras.pages = diff_page;
	paras.bitmap = 0U;
	INIT_LIST_HEAD(&paras.free_list);

	ret = hvgr_mmu_take_down(ctx, &paras, area);
	if (ret != 0) {
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u resize 0x%lx pages %lu take down fail %d",
			ctx->id, area->gva, pages, ret);
		return -ENOMEM;
	}

	hvgr_mem_flush_pt(ctx, (area->gva + va_offset), diff_page, WITH_FLUSH_L2, paras.bitmap);
	hvgr_mmu_free_pt_pages(ctx, &paras.free_list);


	hvgr_mem_free_page(ctx->gdev->dev, (area->page_array + pages), (uint32_t)diff_page,
		area->order, policy_id);
	atomic_sub((int)diff_page, &ctx->mem_ctx.used_pages);
	atomic_sub((int)diff_page, &ctx->gdev->mem_dev.used_pages);

	area->pages = pages;
	area->phy_pages = pages;

	return 0;
}

static unsigned long hvgr_mem_jit_shrink_free(struct shrinker *shrink,
	struct shrink_control *shrink_c)
{
	struct hvgr_mem_ctx *pmem = NULL;
	struct hvgr_mem_area *area = NULL;
	struct list_head *entry = NULL;
	struct hvgr_ctx *ctx = NULL;
	unsigned long free_pages = 0;

	pmem = container_of(shrink, struct hvgr_mem_ctx, jit_shrink);
	ctx = container_of(pmem, struct hvgr_ctx, mem_ctx);

	mutex_lock(&pmem->jit_shrink_mutex);
	list_for_each_prev(entry, &pmem->jit_pool_head) {
		area = list_entry(entry, struct hvgr_mem_area, jit_node);
		free_pages += area->pages;
		if (hvgr_mem_jit_shrink(ctx, area, 0u) != 0) {
			free_pages = -1;
			hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u jit shrinker free fail", ctx->id);
			goto unlock_out;
		}

		if (free_pages > shrink_c->nr_to_scan)
			break;
	}
unlock_out:
	mutex_unlock(&pmem->jit_shrink_mutex);

	return free_pages;
}

static void hvgr_mem_jit_pool_init(struct hvgr_ctx * const ctx)
{
	int ret;
	struct hvgr_mem_ctx *pmem = &ctx->mem_ctx;

	mutex_init(&pmem->jit_shrink_mutex);

	mutex_lock(&pmem->jit_shrink_mutex);
	INIT_LIST_HEAD(&pmem->jit_pool_head);
	INIT_LIST_HEAD(&pmem->jit_using_head);
	mutex_unlock(&pmem->jit_shrink_mutex);

	pmem->jit_shrink.count_objects = hvgr_mem_jit_shrink_count;
	pmem->jit_shrink.scan_objects = hvgr_mem_jit_shrink_free;
	pmem->jit_shrink.seeks = DEFAULT_SEEKS;
	pmem->jit_shrink.batch = 0;

	ret = register_shrinker(&pmem->jit_shrink);
	if (ret != 0)
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u reg jit shrinker fail %d", ctx->id, ret);
}

long hvgr_mem_init_proc(struct hvgr_ctx * const ctx)
{
	struct hvgr_mem_ctx *pmem = NULL;
	unsigned long bitmap_val = ~1UL;

	if (ctx == NULL)
		return -EINVAL;

	pmem = &ctx->mem_ctx;
	if (!hvgr_kv_init(&pmem->area_rec) ||
		!hvgr_kv_init(&pmem->area_k_rec))
		return -EPERM;

	if (!hvgr_mem_zone_init_proc(pmem))
		return -ENOMEM;

	if (hvgr_ctx_init_mem_pool(ctx) != 0) {
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u mem pool init fail", ctx->id);
		goto init_ctx_pool_fail;
	}


	mutex_init(&pmem->mmu_mutex);
	mutex_init(&pmem->area_mutex);
	mutex_init(&pmem->pend_area_mutex);
	mutex_init(&pmem->profile_mutex);
	mutex_init(&pmem->area_k_mutex);
	mutex_init(&pmem->jit_mutex);
#ifdef CONFIG_LIBLINUX
	init_rwsem(&pmem->rw_4g_sem);
#endif
	atomic_set(&pmem->used_pages, 0);
	atomic_set(&pmem->used_grow_pages, 0);
	atomic_set(&pmem->used_pt_pages, 0);
	atomic_set(&pmem->used_kmd_pages, 0);
	pmem->bd_dmabuf_flag = false;

	bitmap_copy(pmem->pend_area_bitmap, &bitmap_val, BITS_PER_LONG);
	pmem->itree = RB_ROOT_CACHED;
	if (hvgr_mmu_init_proc(ctx) != 0) {
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u mmu init fail", ctx->id);
		goto init_mmu_fail;
	}

	hvgr_mem_jit_pool_init(ctx);

	if (ctx->kfile != NULL) {
		rcu_read_lock();
		mmgrab(current->mm);
		rcu_assign_pointer(pmem->process_mm, current->mm);
		rcu_read_unlock();
	}

	INIT_LIST_HEAD(&pmem->dma_buf_rec_head);
	if (hvgr_mem_init_process_res_cb(ctx) != 0) {
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u mem init res fail", ctx->id);
		goto init_res_fail;
	}

	return 0;
init_res_fail:
	if (ctx->kfile != NULL) {
		rcu_read_lock();
		mmdrop(current->mm);
		rcu_read_unlock();
	}
	unregister_shrinker(&pmem->jit_shrink);
	hvgr_mmu_term_proc(ctx);
init_mmu_fail:
	hvgr_ctx_deinit_mem_pool(ctx, MEM_POOL_NUM);
init_ctx_pool_fail:
	hvgr_mem_zone_term_proc(pmem);
	return -ENOMEM;
}

static void hvgr_mem_free_leak_area(struct hvgr_ctx * const ctx, struct hvgr_kv_map *area_rec)
{
	struct rb_root *rbtree = &area_rec->root;
	struct rb_node *rbnode = rb_first(rbtree);
	struct hvgr_kv_node *kvnode = NULL;
	struct hvgr_mem_area *area = NULL;

	while (rbnode != NULL) {
		kvnode = rb_entry(rbnode, struct hvgr_kv_node, node);
		area = container_of(kvnode, struct hvgr_mem_area, node);
		hvgr_mem_del_from_map_rec(ctx, area);
		if (area->property == HVGR_MEM_PROPERTY_IMPORT)
			hvgr_mem_import_free_when_term(area);
		else
			(void)hvgr_mem_free_area_nolock(ctx, area);

		rbnode = rb_first(rbtree);
	}
}

void hvgr_mem_free_area_by_property(struct hvgr_ctx * const ctx,
	struct hvgr_kv_map *area_rec, uint64_t prot)
{
	struct rb_root *rbtree = &area_rec->root;
	struct rb_node *next = rb_first(rbtree);
	struct hvgr_kv_node *kvnode = NULL;
	struct hvgr_mem_area *area = NULL;

	while (next != NULL) {
		kvnode = rb_entry(next, struct hvgr_kv_node, node);
		area = container_of(kvnode, struct hvgr_mem_area, node);
		next = rb_next(next);
		if (area->property == prot) {
			hvgr_mem_del_from_map_rec(ctx, area);
			if (area->property == HVGR_MEM_PROPERTY_IMPORT)
				hvgr_mem_import_free_when_term(area);
			else
				(void)hvgr_mem_free_area_nolock(ctx, area);
		}
	}
}

static void hvgr_mem_unbind_all_area_by_property(struct hvgr_ctx * const ctx,
	struct hvgr_kv_map *area_rec, uint64_t prot)
{
	struct rb_root *rbtree = &area_rec->root;
	struct rb_node *next = rb_first(rbtree);
	struct hvgr_kv_node *kvnode = NULL;
	struct hvgr_mem_area *area = NULL;

	while (next != NULL) {
		kvnode = rb_entry(next, struct hvgr_kv_node, node);
		area = container_of(kvnode, struct hvgr_mem_area, node);
		next = rb_next(next);
		if ((area->property == prot) && area->kmd_sparse_bind)
			(void)hvgr_mem_sparse_unbind_nolock(ctx, area->gva, area->pages);
	}
}

static void hvgr_mem_free_k_area(struct hvgr_ctx * const ctx, struct hvgr_kv_map *area_k_rec)
{
	struct rb_root *rbtree = &area_k_rec->root;
	struct rb_node *rbnode = rb_first(rbtree);
	struct hvgr_kv_node *kvnode = NULL;
	struct hvgr_mem_area *area = NULL;

	while (rbnode != NULL) {
		kvnode = rb_entry(rbnode, struct hvgr_kv_node, node);
		area = container_of(kvnode, struct hvgr_mem_area, k_node);

		hvgr_kv_del(area_k_rec, area->k_node.key);
		if (area->kva != 0U) {
			vunmap(u64_to_ptr(area->kva));
			area->kva = 0U;
		}

		hvgr_mem_area_ref_put(area);
		rbnode = rb_first(rbtree);
	}
}

void hvgr_mem_free_area_when_gpu_fault(struct hvgr_ctx * const ctx, uint64_t prot)
{
	struct hvgr_mem_ctx * const pmem = &ctx->mem_ctx;

	mutex_lock(&pmem->area_mutex);
	if (!hvgr_kv_is_empty(&pmem->area_rec)) {
		hvgr_debug(ctx->gdev, HVGR_MEM, "ctx_%u free prot %u when gpu fault.", ctx->id, prot);
		hvgr_mem_free_area_by_property(ctx, &pmem->area_rec, prot);
	}
	mutex_unlock(&pmem->area_mutex);
}

void hvgr_mem_unbind_all_when_gpu_fault(struct hvgr_ctx * const ctx, uint64_t prot)
{
	struct hvgr_mem_ctx * const pmem = &ctx->mem_ctx;

	mutex_lock(&pmem->area_mutex);
	if (!hvgr_kv_is_empty(&pmem->area_rec)) {
		hvgr_debug(ctx->gdev, HVGR_MEM, "ctx_%u unbind prot %u when gpu fault.", ctx->id, prot);
		hvgr_mem_unbind_all_area_by_property(ctx, &pmem->area_rec, prot);
	}
	mutex_unlock(&pmem->area_mutex);
}

static void hvgr_mem_jit_check_leak(struct hvgr_ctx * const ctx)
{
	struct hvgr_mem_ctx * const pmem = &ctx->mem_ctx;

	if (list_empty(&pmem->jit_using_head) == 0)
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u jit using head not free", ctx->id);

	if (list_empty(&pmem->jit_pool_head) == 0)
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u jit pool head not free", ctx->id);
}

static void hvgr_mem_clear_pend_area(struct hvgr_mem_ctx * const pmem)
{
	uint32_t index;
	struct hvgr_mem_area *area = NULL;

	mutex_lock(&pmem->pend_area_mutex);
	for (index = 0; index < ARRAY_SIZE(pmem->pend_areas); index++) {
		area = pmem->pend_areas[index];
		if (area == NULL)
			continue;

		pmem->pend_areas[index] = NULL;

		if ((area->attribute & HVGR_MEM_ATTR_KMD_ACCESS) != 0)
			hvgr_mem_unmap_kva(area);

		hvgr_mem_area_ref_put(area);
	}

	mutex_unlock(&pmem->pend_area_mutex);
}

void hvgr_mem_term_proc(struct hvgr_ctx * const ctx)
{
	struct hvgr_mem_ctx * const pmem = &ctx->mem_ctx;

	unregister_shrinker(&pmem->jit_shrink);

	mutex_lock(&pmem->area_mutex);
	hvgr_mem_clear_pend_area(pmem);

	if (!hvgr_kv_is_empty(&pmem->area_rec)) {
		hvgr_info(ctx->gdev, HVGR_MEM, "ctx_%u alloc memory not free", ctx->id);
		hvgr_mem_free_leak_area(ctx, &pmem->area_rec);
	}

	mutex_lock(&pmem->area_k_mutex);
	if (!hvgr_kv_is_empty(&pmem->area_k_rec)) {
		hvgr_info(ctx->gdev, HVGR_MEM, "ctx_%u k_area not free", ctx->id);
		hvgr_mem_free_k_area(ctx, &pmem->area_k_rec);
	}
	mutex_unlock(&pmem->area_k_mutex);

	/* sparse may free other zone mem, term it before others */
	hvgr_mem_sparse_term(ctx);
	mutex_unlock(&pmem->area_mutex);
	hvgr_mem_jit_check_leak(ctx);
	hvgr_del_bg_dmabuf_ctx_from_global_list(ctx);
	hvgr_mem_zone_term_proc(pmem);
	hvgr_mmu_term_proc(ctx);
	hvgr_mem_term_process_res_cb(ctx);
	hvgr_mem_remove_profile(ctx);
	hvgr_ctx_deinit_mem_pool(ctx, MEM_POOL_NUM);
	rcu_read_lock();
	if(rcu_dereference(pmem->process_mm) != NULL) {
		mmdrop(pmem->process_mm);
		rcu_assign_pointer(pmem->process_mm, NULL);
	}
	rcu_read_unlock();
}

bool hvgr_mem_is_cpu_coherent(struct hvgr_device * const gdev)
{
	return ((gdev->mem_dev.system_coherency == COHERENCY_ACE_LITE) ||
		(gdev->mem_dev.system_coherency == COHERENCY_ACE));
}

static void hvgr_mem_coherency_init(struct hvgr_device * const gdev)
{
	uint32_t value;
	int ret;

	gdev->mem_dev.system_coherency = COHERENCY_NONE;

	ret = of_property_read_u32(gdev->dev->of_node, "system-coherency",
		&value);
	if (ret != 0) {
		hvgr_debug(gdev, HVGR_MEM, "No system-coherency cfg");
		return;
	}

	if (value != COHERENCY_NONE && value != COHERENCY_ACE_LITE && value != COHERENCY_ACE) {
		hvgr_err(gdev, HVGR_MEM, "Unsupport system-coherency cfg 0x%x", value);
		return;
	}

	gdev->mem_dev.system_coherency = value;
	hvgr_info(gdev, HVGR_MEM, "system-coherency cfg 0x%x", value);
}

static int hvgr_mem_dma_mask_init(struct hvgr_device * const gdev)
{
	int ret;
	uint32_t pa_width = (gdev->dm_dev.dev_reg.mmu_features >> 8) & 0xFF;

	ret = dma_set_mask(gdev->dev, DMA_BIT_MASK(pa_width));
	if (ret != 0) {
		hvgr_err(gdev, HVGR_MEM, "dma set mask fail %d", ret);
		return ret;
	}

	ret = dma_set_coherent_mask(gdev->dev, DMA_BIT_MASK(pa_width));
	if (ret != 0) {
		hvgr_err(gdev, HVGR_MEM, "dma set coherent mask fail %d", ret);
		return ret;
	}

	return 0;
}




int hvgr_mem_init(struct hvgr_device * const gdev)
{
	struct hvgr_mem_dev *mem_dev = &gdev->mem_dev;
#ifdef CONFIG_LIBLINUX
#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0))
	unsigned long totalram = (unsigned long)totalram_pages;
#else
	unsigned long totalram = totalram_pages();
#endif

	mem_dev->threshold_page = (totalram * 3U) / 5U;
	hvgr_debug(gdev, HVGR_MEM, "total page %llu threshold page %llu",
		totalram, mem_dev->threshold_page);
#endif

	hvgr_mem_coherency_init(gdev);

#ifdef CONFIG_HVGR_ASYNC_MEM_RECYCLE
	if (hvgr_mem_recycle_init(gdev) != 0)
		return -1;
#endif

	if (hvgr_mem_area_slab_init() != 0)
		goto area_slab_init_fail;

	if(hvgr_mem_dma_mask_init(gdev) != 0)
		goto dma_mask_init_fail;

	if (!hvgr_kv_init(&mem_dev->mem_process_res_rec))
		goto dma_mask_init_fail;

	mutex_init(&mem_dev->mem_process_res_lock);
	spin_lock_init(&mem_dev->mmu_hw_lock);
	atomic_set(&mem_dev->used_pages, 0);

	if (hvgr_mem_sparse_alloc_zero_page(gdev) != 0)
		goto dma_mask_init_fail;

	if (hvgr_dev_init_mem_pool(gdev) != 0)
		goto pool_dev_init_fail;

	hvgr_mtrack_debugfs_init(gdev);



#if hvgr_version_lt(HVGR_V300)
	(void)hvgr_mem_protect_jcd_pool_init(gdev);
#endif

	hvgr_ion_mem_reclaim_init(gdev);

	if (hvgr_mmu_init(gdev) != 0)
		goto mmu_init_fail;

	if ((int)hvgr_mmu_irq_init(gdev))
		goto mmut_irq_init_fail;

	return 0;

mmut_irq_init_fail:
	hvgr_mmu_term(gdev);
mmu_init_fail:
#if hvgr_version_lt(HVGR_V300)
	hvgr_mem_protect_jcd_pool_term(gdev);
#endif
	hvgr_mtrack_debugfs_term(gdev);
	hvgr_dev_deinit_mem_pool(gdev, MEM_POOL_NUM);
pool_dev_init_fail:
	hvgr_mem_sparse_free_zero_page(gdev);
dma_mask_init_fail:
	hvgr_mem_area_slab_term();
area_slab_init_fail:
#ifdef CONFIG_HVGR_ASYNC_MEM_RECYCLE
	hvgr_mem_recycle_term(gdev);
#endif
	return -1;
}

void hvgr_mem_term(struct hvgr_device * const gdev)
{
	struct hvgr_mem_dev *mem_dev = &gdev->mem_dev;

	hvgr_mtrack_debugfs_term(gdev);

	hvgr_dev_deinit_mem_pool(gdev, MEM_POOL_NUM);

	hvgr_mem_sparse_free_zero_page(gdev);

	if (!hvgr_kv_is_empty(&mem_dev->mem_process_res_rec))
		hvgr_err(gdev, HVGR_MEM, "Some process 4gva do not free");

	hvgr_mem_area_slab_term();

	hvgr_mmu_term(gdev);

#if hvgr_version_lt(HVGR_V300)
	if (!hvgr_mem_protect_jcd_pool_term(gdev))
		hvgr_info(gdev, HVGR_MEM, "protect jcd pool do not need term");
#endif

#ifdef CONFIG_HVGR_ASYNC_MEM_RECYCLE
	hvgr_mem_recycle_term(gdev);
#endif

}

long hvgr_mem_rw(struct hvgr_ctx * const ctx, uint64_t gva, uint64_t *data, bool write)
{
	struct hvgr_mem_area *area = NULL;
	uint64_t offset;
	uint64_t offset_page;
	uint64_t offset_page_byte;
	struct page *page = NULL;
	dma_addr_t dma_addr;
	char *addr_tar = NULL;
	void *addr = NULL;

	if (data == NULL) {
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u mem_rw input data is null.", ctx->id);
		return -EINVAL;
	}

	mutex_lock(&ctx->mem_ctx.area_mutex);
	area = hvgr_mem_zone_find_area_by_gva_range(&ctx->mem_ctx, gva);
	if (area == NULL) {
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u mem_rw no area for gva=0x%lx.",
			ctx->id, gva);
		mutex_unlock(&ctx->mem_ctx.area_mutex);
		return -EINVAL;
	}

	if ((write && (area->attribute & HVGR_MEM_ATTR_CPU_WR) == 0) ||
		(!write && (area->attribute & HVGR_MEM_ATTR_CPU_RD) == 0)) {
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u mem_rw gva=0x%lx %lu:0x%lx attr err.",
			ctx->id, gva, area->property, area->attribute);
		mutex_unlock(&ctx->mem_ctx.area_mutex);
		return -EINVAL;
	}

	if ((gva + sizeof(*data)) > (area->gva + (area->pages << PAGE_SHIFT))) {
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u mem_rw gva=0x%lx %lu:0x%lx invalid.",
			ctx->id, gva, area->property, area->attribute);
		mutex_unlock(&ctx->mem_ctx.area_mutex);
		return -EINVAL;
	}

	offset = gva - area->gva;
	offset_page = offset >> PAGE_SHIFT;
	offset_page_byte = offset & (PAGE_SIZE - 1);

	page = area->page_array[offset_page];
	addr = kmap_atomic(page);
	addr_tar = ((char *)addr) + offset_page_byte;

	if (write) {
		*((uint64_t *)addr_tar) = *data;
		dma_addr = hvgr_mem_get_page_dma_addr(page);
		dma_sync_single_for_device(ctx->gdev->dev, dma_addr, PAGE_SIZE, DMA_TO_DEVICE);
	}
	else {
		*data = *((uint64_t *)addr_tar);
	}

	kunmap_atomic(addr);
	mutex_unlock(&ctx->mem_ctx.area_mutex);
	return 0;
}

static struct hvgr_mem_area *hvgr_mem_new_import_area(struct hvgr_ctx * const ctx,
	struct hvgr_mem_alloc_para *area_para)
{
	struct hvgr_mem_area *area = NULL;

	area = hvgr_mem_new_area(&ctx->mem_ctx, area_para);
	if (area == NULL) {
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u import get area fail 0x%lx",
			ctx->id, area_para->attribute);
		return NULL;
	}

	if (hvgr_mem_alloc_pages(area) != 0) {
		hvgr_mem_free_area(area);
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u import alloc page_array fail %lu",
			ctx->id, area_para->max_pages);
		return NULL;
	}

	area->gva = hvgr_mem_zone_alloc_gva(&ctx->mem_ctx, ZONE_TYPE_IMPORT, 0U,
		(area->max_pages << PAGE_SHIFT));
	if (area->gva == 0) {
		hvgr_mem_area_ref_put(area);
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u import alloc va fail %lu",
			ctx->id, area_para->max_pages);
		return NULL;
	}

	area->uva = area->gva;
	area->user_buf = true;
	return area;
}

struct hvgr_mem_area *hvgr_mem_import(struct hvgr_ctx * const ctx,
	struct hvgr_mem_import_para * const para, uint64_t *uva, uint64_t *gva)
{
	struct hvgr_mem_area *area = NULL;
	struct hvgr_mem_alloc_para area_para = {HVGR_MEM_PROPERTY_IMPORT, para->attribute, 0U,
		para->pages, 0U, 0U, 0U};

	*gva = 0UL;

	if (para->pages == 0 || (para->attribute & HVGR_MEM_MASK_GPU_ACCESS) == 0 ||
		(para->attribute & HVGR_MEM_ATTR_GPU_EXEC) != 0 ||
		(para->attribute & HVGR_MEM_ATTR_GROWABLE) != 0 ||
		(para->attribute & HVGR_MEM_ATTR_KMD_ACCESS) != 0 ||
		((para->attribute & HVGR_MEM_ATTR_SECURE) != 0 &&
		(para->attribute & HVGR_MEM_MASK_UMD_ACCESS) != 0) ||
		(para->attribute & HVGR_MEM_MASK_COHERENT) != 0 ||
		(para->attribute & HVGR_MEM_ATTR_MEM_SPARSE) != 0) {
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u import para error, 0x%lx:0x%lx",
			ctx->id, para->pages, para->attribute);
		return NULL;
	}

#ifdef CONFIG_LIBLINUX
	if (!hvgr_mem_max_pages_check(ctx, para->pages))
		return NULL;
#endif
	if (para->gva == 0UL) {
		area = hvgr_mem_new_import_area(ctx, &area_para);
		if (area == NULL)
			return NULL;

		mutex_lock(&ctx->mem_ctx.area_mutex);
		area->node.key = area->gva;
		area->node.size = area->max_pages << PAGE_SHIFT;
		if (unlikely(hvgr_mem_add_to_map_rec(ctx, area) != 0)) {
			hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u import add to map_rec fail", ctx->id);
			goto first_import_fail;
		}
	} else {
		mutex_lock(&ctx->mem_ctx.area_mutex);
		area = hvgr_mem_zone_find_area_by_gva(&ctx->mem_ctx, para->gva);
		if (area == NULL) {
			mutex_unlock(&ctx->mem_ctx.area_mutex);
			hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u import no area for va 0x%lx",
				ctx->id, para->gva);
			return NULL;
		}
	}

	if (hvgr_mem_zone_import_memory(area, para->fd) != 0) {
		hvgr_err(ctx->gdev, HVGR_MEM,
			"ctx_%u import FAIL. prot=%lu:%lx, page:%lu/%lu fd=%d gva=0x%lx",
			ctx->id, area->property, area->attribute,
			area->pages, area->max_pages, para->fd, para->gva);
		if (para->gva == 0UL) {
			hvgr_mem_del_from_map_rec(ctx, area);
			goto first_import_fail;
		}
		mutex_unlock(&ctx->mem_ctx.area_mutex);
		return NULL;
	}

	*uva = 0U;
	*gva = area->gva;
	hvgr_info(ctx->gdev, HVGR_MEM,
		"ctx_%u import mem prot=%lu:0x%lx, page:%lu/%lu uva/gva=0x%lx fd=%d",
		ctx->id, area->property, area->attribute, area->pages, area->max_pages, area->gva,
		para->fd);

	mutex_unlock(&ctx->mem_ctx.area_mutex);
	hvgr_add_bg_dmabuf_ctx_to_global_list(ctx);
	return area;
first_import_fail:
	mutex_unlock(&ctx->mem_ctx.area_mutex);
	hvgr_mem_zone_free_gva(area->zone->pmem, ZONE_TYPE_IMPORT, area->gva,
		(area->max_pages << PAGE_SHIFT));
	hvgr_mem_area_ref_put(area);
	return NULL;
}

static long hvgr_mem_resize_shrink(struct hvgr_ctx * const ctx, struct hvgr_mem_area *area,
	uint64_t pages)
{
	long ret;
	uint64_t diff_page = area->pages - pages;
	uint64_t va_offset = (pages << PAGE_SHIFT);
	struct hvgr_mmu_takedown_paras paras;

	area->pages = pages;
	area->phy_pages = pages;

	paras.va = (area->gva + va_offset);
	paras.pages = diff_page;
	paras.bitmap = 0U;
	INIT_LIST_HEAD(&paras.free_list);

	ret = hvgr_mmu_take_down(ctx, &paras, area);
	if (ret != 0) {
		area->pages += diff_page;
		area->phy_pages += diff_page;
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u resize 0x%lx pages %lu take down fail %d",
			ctx->id, area->gva, pages, ret);
		return -ENOMEM;
	}

	hvgr_mem_flush_pt(ctx, (area->gva + va_offset), diff_page, WITH_FLUSH_L2, paras.bitmap);
	hvgr_mmu_free_pt_pages(ctx, &paras.free_list);

	if (area->property == HVGR_MEM_PROPERTY_SAME_VA) {
#if defined(CONFIG_LIBLINUX) && !defined(CONFIG_LIBLINUX_CDC)
		ret = hvgr_mem_vma_zap_pte(ctx, (area->uva + va_offset),
			(diff_page << PAGE_SHIFT));
		if (ret != 0) {
			struct hvgr_mmu_setup_paras setup;
			uint32_t level = 0U;

			setup.flags = area->flags;
			setup.gva = area->gva + (area->pages << PAGE_SHIFT);
			setup.pages = diff_page;
			setup.page_array = area->page_array + area->pages;
			setup.mmu_flag = area->mmu_flag;

			(void)hvgr_mmu_set_up(ctx, &setup, &level, area);

			area->pages += diff_page;
			area->phy_pages += diff_page;
			hvgr_mem_flush_pt(ctx, area->gva, area->pages, WITHOUT_FLUSH_L2, level);
			hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u resize 0x%lx pages %lu zap fail %d",
				ctx->id, area->gva, pages, ret);
			return -ENOMEM;
		}
#else
		unmap_mapping_range(ctx->kfile->f_inode->i_mapping,
			(loff_t)(area->uva + va_offset), (loff_t)(diff_page << PAGE_SHIFT), 1);
#endif
	}

	hvgr_mem_pool_ctx_free(ctx, diff_page, (void **)(area->page_array + pages),
		(((area->attribute & HVGR_MEM_ATTR_CPU_CACHEABLE) != 0 ? HVGR_PAGE_SYNC : 0)),
		area->attribute);
	return 0;
}

long hvgr_mem_resize_expand(struct hvgr_ctx * const ctx, struct hvgr_mem_area *area,
	uint64_t pages)
{
	long ret;
	struct hvgr_mmu_setup_paras setup;
	uint64_t diff_page = pages - area->pages;
	struct page **page_arr = area->page_array + area->pages;
	uint32_t level = 0;

	ret = hvgr_mem_pool_ctx_alloc(ctx, diff_page, (void **)page_arr, area->attribute);
	if (ret != 0) {
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u resize 0x%lx pages %lu alloc fail %d",
			ctx->id, area->gva, pages, ret);
		return -ENOMEM;
	}

	setup.flags = area->flags;
	setup.gva = area->gva + (area->pages << PAGE_SHIFT);
	setup.pages = diff_page;
	setup.page_array = page_arr;
	setup.mmu_flag = area->mmu_flag;

	ret = hvgr_mmu_set_up(ctx, &setup, &level, area);
	if (ret != 0) {
		/* flush expand size */
		hvgr_mem_flush_pt(ctx, area->gva + (area->pages << PAGE_SHIFT),
			diff_page, WITHOUT_FLUSH_L2, level);
		hvgr_mem_pool_ctx_free(ctx, diff_page, (void **)page_arr, 0,
			area->attribute);
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u resize 0x%lx pages %lu map gva fail %d",
			ctx->id, area->gva, pages, ret);
		return -ENOMEM;
	}

	area->pages = pages;
	area->phy_pages = pages;

	hvgr_mem_flush_pt(ctx, area->gva, area->pages, WITHOUT_FLUSH_L2, level);
	return 0;
}

long hvgr_mem_resize(struct hvgr_ctx * const ctx, uint64_t gva, uint64_t pages)
{
	struct hvgr_mem_area *area = NULL;
	long ret = -EINVAL;
	uint32_t resize_ref_limit;

	down_write(hvgr_mem_mm_map_lock());
	mutex_lock(&ctx->mem_ctx.area_mutex);
	area = hvgr_mem_zone_find_area_by_gva(&ctx->mem_ctx, gva);
	if (area == NULL) {
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u resize no area for 0x%lx", ctx->id, gva);
		goto fail;
	}

	if (hvgr_ctx_flag(ctx, HVGR_CTX_FLAG_COMPAT)) {
		if (area->property != HVGR_MEM_PROPERTY_SAME_VA &&
			area->property != HVGR_MEM_PROPERTY_GPU_MEM) {
			hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u resize unsupport mem for 0x%lx %lu",
				ctx->id, gva, area->property);
			goto fail;
		}
	} else {
		if (area->property != HVGR_MEM_PROPERTY_SAME_VA) {
			hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u resize unsupport mem for 0x%lx %lu",
				ctx->id, gva, area->property);
			goto fail;
		}
	}

	if((area->attribute & HVGR_MEM_ATTR_GROWABLE) == 0) {
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u resize not grow mem 0x%lx %lu:0x%lx",
			ctx->id, gva, area->property, area->attribute);
		goto fail;
	}

	/* same va ref init 2, ref > init val cant be resized */
	if (area->property == HVGR_MEM_PROPERTY_GPU_MEM)
		resize_ref_limit = 1U;
	else
		resize_ref_limit = 2U;

	/* area is referenced before or mapped on cpu side, can't by resized */
	if (hvgr_mem_area_ref_read(area) > resize_ref_limit) {
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u can't be resized for 0x%lx", ctx->id, gva);
		goto fail;
	}

	if (pages > area->max_pages) {
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u resize 0x%lx pages err %lu/%lu",
			ctx->id, gva, pages, area->max_pages);
		goto fail;
	}

	if (pages == area->pages) {
		mutex_unlock(&ctx->mem_ctx.area_mutex);
		up_write(hvgr_mem_mm_map_lock());
		return 0;
	}
	if (pages < area->pages)
		ret = hvgr_mem_resize_shrink(ctx, area, pages);
	else
		ret = hvgr_mem_resize_expand(ctx, area, pages);

	hvgr_info(ctx->gdev, HVGR_MEM,
		"ctx_%u resize mem prot=%lu:0x%lx, page:%lu/%lu uva/gva=0x%lx area to page:%lu %d",
		ctx->id, area->property, area->attribute, area->pages, area->max_pages,
		area->gva, pages, ret);
fail:
	mutex_unlock(&ctx->mem_ctx.area_mutex);
	up_write(hvgr_mem_mm_map_lock());
	return ret;
}

long hvgr_mem_change_mem_attr(struct hvgr_ctx * const ctx, uint64_t gva, uint64_t attr)
{
	struct hvgr_mem_area *area = NULL;
	long ret = -EINVAL;

	if (gva == 0 || attr == 0)
		return -EINVAL;

	mutex_lock(&ctx->mem_ctx.area_mutex);
	area = hvgr_mem_zone_find_area_by_gva(&ctx->mem_ctx, gva);
	if (area == NULL) {
		mutex_unlock(&ctx->mem_ctx.area_mutex);
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u change attr no area for 0x%lx", ctx->id, gva);
		return -EINVAL;
	}

	/* Until now, only sparse mem and gpu mem can change attribute */
	switch (area->property) {
	case HVGR_MEM_PROPERTY_SPARSE:
		ret = hvgr_sparse_mem_change_attr(ctx, area, attr);
		if (ret != 0)
			hvgr_err(ctx->gdev, HVGR_MEM,
				"ctx_%u change sparse mem attr fail, attr=%lx:0x%lx, gva=0x%lx, %d",
				ctx->id, attr, area->attribute, gva, ret);
		break;
	case HVGR_MEM_PROPERTY_GPU_MEM:
		ret = hvgr_gpu_mem_change_attr(ctx, area, attr);
		if (ret != 0)
			hvgr_err(ctx->gdev, HVGR_MEM,
				"ctx_%u change gpu mem attr fail, attr=%lx:0x%lx, gva=0x%lx, %d",
				ctx->id, attr, area->attribute, gva, ret);
		break;
	default:
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u area prop 0x%lx don't support attr change",
			ctx->id, area->property);
		break;
	}

	mutex_unlock(&ctx->mem_ctx.area_mutex);
	return ret;
}

static void hvgr_mem_vm_open(struct vm_area_struct *vma)
{
	struct hvgr_mem_cpu_map *cpu_map = vma->vm_private_data;

	cpu_map->count++;

	hvgr_info(cpu_map->ctx->gdev, HVGR_MEM, "%s:%d ctx_%u area_ptr 0x%lx uva 0x%lx count %u",
		__func__, __LINE__, cpu_map->ctx->id, cpu_map->area,
		(cpu_map->area != NULL ? cpu_map->area->uva : 0U), cpu_map->count);
}

static bool hvgr_mem_free_when_ctx_term(struct vm_area_struct *vma)
{
	struct hvgr_mem_cpu_map *cpu_map = vma->vm_private_data;
	struct task_struct *task = NULL;
	struct mm_struct *mm = vma->vm_mm;
	bool ret = false;

	if (cpu_map->area != NULL && cpu_map->area->property == HVGR_MEM_PROPERTY_DRIVER)
		return false;
#if !defined(CONFIG_LIBLINUX)
	rcu_read_lock();
	task = pid_task(find_vpid(cpu_map->ctx->tgid), PIDTYPE_PID);
	if (task == NULL) {
		ret = true;
		if (!IS_ERR(mm) && (atomic_read(&mm->mm_users) > 0))
			ret = false;
	} else {
		ret = false;
		if ((task->flags & PF_EXITING) != 0)
			ret = true;
	}
	rcu_read_unlock();
#else
	if ((current->flags & PF_EXITING) != 0)
		ret = true;
#endif
	return ret;
}


static void hvgr_mem_vm_close_destroy_4g(struct vm_area_struct *vma)
{
	struct hvgr_device *gdev = hvgr_get_device();
	size_t nr_pages = vma_pages(vma);
	struct hvgr_mem_cpu_map *cpu_map = vma->vm_private_data;
	struct hvgr_kv_node *node = NULL;
	struct hvgr_mem_process_res_cb *res_cb = NULL;
	struct hvgr_ctx *tmp = NULL;
	struct hvgr_ctx *ctx_tmp = NULL;

	if ((vma->vm_start & HVGR_MEM_4G_MASK) != 0 || nr_pages < HVGR_MEM_4G_PAGES) {
		hvgr_err(gdev, HVGR_MEM, "vm_close 0x%lx~0x%lx is_4g_rsv but not 4g align or size",
			vma->vm_start, vma->vm_end);
		return;
	}

	mutex_lock(&gdev->dm_dev.ctx_list_mutex);
	list_for_each_entry_safe(ctx_tmp, tmp, &gdev->dm_dev.ctx_list, dev_link) {
		if ((uint64_t)ctx_tmp->mem_ctx.process_mm ==
			(uint64_t)cpu_map->ctx->mem_ctx.process_mm) {
			hvgr_info(gdev, HVGR_MEM, "%s:%d tgid %d set ctx_%u dying",
				__func__, __LINE__, (int)cpu_map->ctx->tgid, ctx_tmp->id);
#ifdef CONFIG_LIBLINUX
			down_write(&ctx_tmp->mem_ctx.rw_4g_sem);
#endif
			ctx_tmp->mem_ctx.zones[HVGR_MEM_PROPERTY_4G_VA].prv_data = NULL;
#ifdef CONFIG_LIBLINUX
			up_write(&ctx_tmp->mem_ctx.rw_4g_sem);
#endif
			if (!hvgr_mem_free_when_ctx_term(vma))
				hvgr_mem_4gva_free_all_area(ctx_tmp);
			hvgr_ctx_flag_set(ctx_tmp, HVGR_CTX_FLAG_DYING);
			hvgr_ctx_flag_clear(ctx_tmp, HVGR_CTX_FLAG_CFGED);
		}
	}
	mutex_unlock(&gdev->dm_dev.ctx_list_mutex);

	mutex_lock(&gdev->mem_dev.mem_process_res_lock);
	node = hvgr_kv_get(&gdev->mem_dev.mem_process_res_rec,
		(uint64_t)cpu_map->ctx->mem_ctx.process_mm);
	if (node != NULL) {
		res_cb = container_of(node, struct hvgr_mem_process_res_cb, kv_node);
		if (res_cb->mem_4gva != NULL) {
			hvgr_mem_space_term(&res_cb->mem_4gva->va_space);
			kfree(res_cb->mem_4gva);
			res_cb->mem_4gva = NULL;
			hvgr_info(gdev, HVGR_MEM, "ctx_%u (%d:%d) destroy 4g space vm_close 0x%lx~0x%lx",
				cpu_map->ctx->id, cpu_map->ctx->tgid, cpu_map->ctx->pid,
				vma->vm_start, vma->vm_end, (int)cpu_map->ctx->tgid);
		}
	}
	mutex_unlock(&gdev->mem_dev.mem_process_res_lock);

	kfree(cpu_map);
}

static void hvgr_mem_vm_close_free(struct vm_area_struct *vma)
{
	struct hvgr_mem_cpu_map *cpu_map = vma->vm_private_data;
	struct hvgr_mem_area *area = cpu_map->area;
	struct hvgr_mem_area *area_tmp = NULL;

	if (hvgr_mem_free_when_ctx_term(vma))
		return;

	mutex_lock(&cpu_map->ctx->mem_ctx.area_mutex);
	area_tmp = hvgr_mem_zone_find_area_by_gva(&cpu_map->ctx->mem_ctx, area->uva);
	if (unlikely(area_tmp == NULL)) {
		hvgr_err(cpu_map->ctx->gdev, HVGR_MEM,
			"vm_close_free ctx_%u cant find area 0x%lx 0x%lx:0x%lx %lu:%lu %s %s vma 0x%lx~0x%lx in area_rec",
			cpu_map->ctx->id, area->uva, area->property, area->attribute, area->pages,
			area->max_pages, (area->is_freed ? "freed" : "no freed"),
			(area->is_record ? "record" : "no record"), vma->vm_start, vma->vm_end);
		if (vma->vm_file != NULL)
			hvgr_err(cpu_map->ctx->gdev, HVGR_MEM, "vma vm_file cnt %ld prv %s same to ctx",
				file_count(vma->vm_file),
				((uint64_t)vma->vm_file->private_data == (uint64_t)cpu_map->ctx) ?
				"is" : "is not");
		else
			hvgr_err(cpu_map->ctx->gdev, HVGR_MEM, "vma vm_file is null");
		mutex_unlock(&cpu_map->ctx->mem_ctx.area_mutex);
		return;
	}

	hvgr_mem_del_from_map_rec(cpu_map->ctx, area);
	area->is_freed = true;
	mutex_unlock(&cpu_map->ctx->mem_ctx.area_mutex);

	/* drv mem ref_put by the alloc user */
	if (area->property == HVGR_MEM_PROPERTY_SAME_VA) {
		hvgr_mem_unmap_gva(NULL, area);
		hvgr_mem_area_ref_put(cpu_map->area);
	}

	hvgr_info(cpu_map->ctx->gdev, HVGR_MEM,
		"%s:%d ctx_%u del area 0x%lx 0x%lx:0x%lx %lu:%lu from area_rec",
		__func__, __LINE__, cpu_map->ctx->id, area->uva, area->property, area->attribute,
		area->pages, area->max_pages);
}

static void hvgr_mem_vm_close(struct vm_area_struct *vma)
{
	struct hvgr_mem_cpu_map *cpu_map = vma->vm_private_data;

	hvgr_info(cpu_map->ctx->gdev, HVGR_MEM, "%s:%d ctx_%u area_ptr 0x%lx area uva 0x%lx count %u vm_start 0x%lx",
		__func__, __LINE__, cpu_map->ctx->id, (uint64_t)cpu_map->area, (cpu_map->area != NULL ? cpu_map->area->uva : 0U),
		cpu_map->count, vma->vm_start);

	hvgr_systrace_begin("%s vma 0x%lx~0x%lx", __func__, vma->vm_start, vma->vm_end);
	if (--cpu_map->count) {
		hvgr_systrace_end();
		return;
	}

#if defined(CONFIG_LIBLINUX) && !defined(CONFIG_LIBLINUX_CDC)
	if (cpu_map->ctx->ctx_sw.is_background) {
		hvgr_debug(cpu_map->ctx->gdev, HVGR_MEM, "ctx_%u is background, uva 0x%lx count %u vm_start 0x%lx",
			cpu_map->ctx->id, (cpu_map->area != NULL ? cpu_map->area->uva : 0U),
			cpu_map->count, vma->vm_start);
	}
#endif
	/* 4g rsv */
	if (cpu_map->is_4g_rsv) {
		hvgr_mem_vm_close_destroy_4g(vma);
		hvgr_systrace_end();
		return;
	}

	/* same va, drv mem */
	if (cpu_map->close_free)
		hvgr_mem_vm_close_free(vma);
	/* unmap gva */
	cpu_map->area->mmap_cnt--;
	hvgr_mem_area_ref_put(cpu_map->area);
	hvgr_info(cpu_map->ctx->gdev, HVGR_MEM, "%s:%d ctx_%u vm close 0x%lx",
		__func__, __LINE__, cpu_map->ctx->id, vma->vm_start);

	kfree(cpu_map);

	hvgr_systrace_end();
}

#if !defined(CONFIG_LIBLINUX)
#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0))
int hvgr_mem_vm_fault(struct vm_fault *vmf)
{
	int ret = VM_FAULT_SIGBUS;
#else
vm_fault_t hvgr_mem_vm_fault(struct vm_fault *vmf)
{
	vm_fault_t ret = VM_FAULT_SIGBUS;
#endif
	struct hvgr_device *gdev = hvgr_get_device();
	struct vm_area_struct *vma = vmf->vma;
	struct hvgr_mem_cpu_map *cpu_map = vma->vm_private_data;
	uint64_t page_offset;
	uint64_t start_addr;
	phys_addr_t phys;
	pgprot_t page_prot = vma->vm_page_prot;

	if (cpu_map == NULL || cpu_map->area == NULL || cpu_map->ctx == NULL) {
		hvgr_err(gdev, HVGR_MEM, "vm_fault 0x%lx addr 0x%lx has no cpu_map",
			vma->vm_start, vmf->address);
		return VM_FAULT_SIGBUS;
	}

	hvgr_info(gdev, HVGR_MEM, "%s:%d vm_fault area 0x%lx %lu fault 0x%lx 0x%lx",
		__func__, __LINE__, cpu_map->area->gva, cpu_map->area->pages, vma->vm_pgoff,
		vmf->address);

	mutex_lock(&cpu_map->ctx->mem_ctx.area_mutex);
	if (atomic_read(&cpu_map->area->import.data.umm.free_tlb_cnt) != 0) {
		hvgr_err(gdev, HVGR_MEM,
			"vm_fault dma buffer page was freed, do not allow memory growable");
		goto err_exit;
	}

	page_offset = vma->vm_pgoff - (cpu_map->area->gva >> PAGE_SHIFT);
	if ((page_offset >= cpu_map->area->pages) ||
		((vmf->address - vma->vm_start) >=
			((cpu_map->area->pages - page_offset) << PAGE_SHIFT))) {
		hvgr_err(gdev, HVGR_MEM, "vm_fault vma 0x%lx~0x%lx area 0x%lx %lu 0x%lx fault 0x%lx out of range",
			vma->vm_start, vma->vm_end, cpu_map->area->gva, cpu_map->area->pages,
			vma->vm_pgoff, vmf->address);
		goto err_exit;
	}

	start_addr = vma->vm_start;

	while (page_offset < cpu_map->area->pages && start_addr < vma->vm_end) {
		phys = page_to_phys(cpu_map->area->page_array[page_offset]);
#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0))
		(void)page_prot;
		ret = vm_insert_pfn(vma, start_addr, PFN_DOWN(phys));
		if (ret < 0 && ret != -EBUSY) {
#else
		ret = vmf_insert_pfn_prot(vma, start_addr, PFN_DOWN(phys), page_prot);
		if (ret != VM_FAULT_NOPAGE) {
#endif
			hvgr_err(gdev, HVGR_MEM, "vm_fault addr 0x%lx insert addr 0x%lx fail %d",
				vmf->address, start_addr, (int)ret);
			goto err_exit;
		}

		page_offset++;
		start_addr += PAGE_SIZE;
	}

	ret = VM_FAULT_NOPAGE;
err_exit:
	mutex_unlock(&cpu_map->ctx->mem_ctx.area_mutex);
	return ret;
}
#endif

static int hvgr_mem_vm_split(struct vm_area_struct *vma, unsigned long addr)
{
	/* not allow split hvgr vma */
	struct hvgr_device *gdev = hvgr_get_device();

	hvgr_err(gdev, HVGR_MEM, "%s:%d tgid %d pid %d vm_start 0x%lx vm_end 0x%lx addr 0x%lx",
		__func__, __LINE__, current->tgid, current->pid, vma->vm_start, vma->vm_end, addr);

	hvgr_dump_stack();
	return -EINVAL;
}

static int hvgr_mem_vm_remap(struct vm_area_struct *vma)
{
	/* not allow split hvgr vma */
	struct hvgr_device *gdev = hvgr_get_device();

	hvgr_err(gdev, HVGR_MEM, "%s:%d tgid %d pid %d, vm_start 0x%lx vm_end 0x%lx",
		__func__, __LINE__, current->tgid, current->pid, vma->vm_start, vma->vm_end);
	hvgr_dump_stack();
	return -EINVAL;
}

/* kmd_vm_ops */
const struct vm_operations_struct hvgr_vm_opts = {
	.open  = hvgr_mem_vm_open,
	.close = hvgr_mem_vm_close,
	.fault = hvgr_mem_vm_fault,
	.split = hvgr_mem_vm_split,
	.mremap = hvgr_mem_vm_remap,
};

const struct vm_operations_struct *hvgr_mem_get_vm_opts(void)
{
	return &hvgr_vm_opts;
}

unsigned long hvgr_mem_get_unmapped_area(struct hvgr_ctx *ctx, unsigned long len,
	unsigned long pgoff, unsigned long flags)
{
	struct vm_unmapped_area_info area_info;
	struct mm_struct *mm = current->mm;

	area_info.flags = VM_UNMAPPED_AREA_TOPDOWN;
	area_info.length = len;
#if !defined(CONFIG_LIBLINUX)
	area_info.low_limit = FIRST_USER_ADDRESS;
	area_info.high_limit = min_t(unsigned long, mm->mmap_base, HVGR_MEM_SAMEVA_TOP);
#else
	area_info.low_limit = (1UL << 12);
	area_info.high_limit = HVGR_MEM_SAMEVA_TOP;
	if (pgoff == PFN_DOWN(HVGR_MEM_4G_RSV_OFFSET))
		area_info.flags |= VM_PERSISTED_MAP;
#endif
	area_info.align_mask = 0U;
	if (pgoff == PFN_DOWN(HVGR_MEM_4G_RSV_OFFSET))
		area_info.align_mask = HVGR_MEM_4G_MASK;
	area_info.align_offset = 0U;

	return vm_unmapped_area(&area_info);
}

static bool hvgr_mem_check_vma(struct hvgr_ctx * const ctx, struct vm_area_struct *const vma)
{
	size_t nr_pages = vma_pages(vma);
	if (nr_pages == 0) {
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u vma has 0 pages", ctx->id);
		return false;
	}

	if (!(vma->vm_flags & VM_SHARED)) {
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u vma has no VM_SHARED", ctx->id);
		return false;
	}

	if (!(vma->vm_flags & VM_READ))
		vma->vm_flags &= ~VM_MAYREAD;
	if (!(vma->vm_flags & VM_WRITE))
		vma->vm_flags &= ~VM_MAYWRITE;

	return true;
}

static int hvgr_mem_cpu_mmap(struct hvgr_ctx *ctx, struct vm_area_struct *vma,
	struct hvgr_mem_area *area, bool close_free, bool is_4g_rsv)
{
	struct hvgr_mem_cpu_map *cpu_map = NULL;

	cpu_map = kzalloc(sizeof(*cpu_map), GFP_KERNEL);
	if (cpu_map == NULL) {
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u kalloc cpu_map fail", ctx->id);
		return -ENOMEM;
	}

	vma->vm_flags |= (VM_PFNMAP | VM_DONTCOPY | VM_DONTDUMP | VM_DONTEXPAND | VM_IO);
	vma->vm_ops = &hvgr_vm_opts;

	if (area != NULL) {
		if ((area->attribute & HVGR_MEM_ATTR_CPU_CACHEABLE) == 0)
			vma->vm_page_prot = pgprot_writecombine(vma->vm_page_prot);

		hvgr_mem_area_ref_get(area);
		cpu_map->area = area;
	}

	cpu_map->ctx = ctx;
	cpu_map->close_free = close_free;
	cpu_map->is_4g_rsv = is_4g_rsv;
	cpu_map->count = 1U;
	vma->vm_private_data = cpu_map;

	return 0;
}

static int hvgr_mem_create_4g_space(struct hvgr_ctx *ctx, struct vm_area_struct *vma)
{
	size_t nr_pages = vma_pages(vma);
	struct hvgr_mem_dev *dev_mem = &ctx->gdev->mem_dev;
	struct hvgr_mem_4gva_cb *mem_4gva = NULL;
	struct hvgr_kv_node *node = NULL;
	struct hvgr_mem_process_res_cb *res_cb = NULL;

	if ((vma->vm_flags & (VM_READ | VM_WRITE | VM_MAYREAD | VM_MAYWRITE)) != 0) {
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u 4g has rd/wr", ctx->id);
		goto vma_err;
	}

	if ((vma->vm_start & HVGR_MEM_4G_MASK) != 0 || nr_pages != HVGR_MEM_4G_PAGES) {
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u 4g 0x%lx:0x%lx out of range",
			ctx->id, vma->vm_start, vma->vm_end);
		goto vma_err;
	}

	mutex_lock(&dev_mem->mem_process_res_lock);
	node = hvgr_kv_get(&dev_mem->mem_process_res_rec, (uint64_t)ctx->mem_ctx.process_mm);
	if (node == NULL) {
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u %d create 4g find reserve cb fail",
			ctx->id, (int)ctx->tgid);
		goto get_kv_fail;
	}

	res_cb = container_of(node, struct hvgr_mem_process_res_cb, kv_node);
	if (res_cb->mem_4gva != NULL) {
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u (%d:%d) 0x%lx create 4g already exist",
			ctx->id, ctx->tgid, ctx->pid, (uint64_t)ctx->mem_ctx.process_mm);
		goto get_kv_fail;
	}

	mem_4gva = kzalloc(sizeof(*mem_4gva), GFP_KERNEL);
	if (unlikely(mem_4gva == NULL)) {
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u create 4g kalloc fail", ctx->id);
		goto get_kv_fail;
	}
	mem_4gva->va_space.start = vma->vm_start;
#if defined(HVGR_STATE_BUNDLE2) && HVGR_STATE_BUNDLE2 > 0
	mem_4gva->va_space.size = HVGR_4GVA_SIZE_3G;
#else
	mem_4gva->va_space.size = HVGR_4GVA_SIZE_4G;
#endif

#if defined(MALEOON_2D_MCU_FEATURE) && MALEOON_2D_MCU_FEATURE != 0
	if (ctx->set_uni_render == MALEOON_2D_MCU_ENABLE)
		mem_4gva->va_space.size = HVGR_4GVA_SIZE_3G;
#endif
	hvgr_info(ctx->gdev, HVGR_MEM, "ctx_%u 4gva size is 0x%llx", mem_4gva->va_space.size);
	if (hvgr_mem_space_init(&mem_4gva->va_space) != 0) {
		kfree(mem_4gva);
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u create 4g init space fail", ctx->id);
		goto get_kv_fail;
	}

	res_cb->mem_4gva = mem_4gva;
	if (hvgr_mem_cpu_mmap(ctx, vma, NULL, false, true) != 0) {
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u create 4g cpu mmap fail", ctx->id);
		goto cpu_map_fail;
	}

	res_cb->mem_4gva->cpu_map = vma->vm_private_data;
	mutex_unlock(&dev_mem->mem_process_res_lock);
	hvgr_info(ctx->gdev, HVGR_MEM, "ctx_%u (%d:%d) 0x%lx create 4g space 0x%lx",
		ctx->id, ctx->tgid, ctx->pid, (uint64_t)ctx->mem_ctx.process_mm, vma->vm_start);

	return 0;
cpu_map_fail:
	hvgr_mem_space_term(&res_cb->mem_4gva->va_space);
	kfree(res_cb->mem_4gva);
	res_cb->mem_4gva = NULL;
get_kv_fail:
	mutex_unlock(&dev_mem->mem_process_res_lock);
vma_err:
	return -EINVAL;
}

static bool hvgr_mem_check_area(struct hvgr_ctx *ctx, struct vm_area_struct *vma,
	struct hvgr_mem_area *area)
{
	if (((vma->vm_flags & VM_READ) != 0 &&
		(area->attribute & HVGR_MEM_ATTR_CPU_RD) == 0) ||
		((vma->vm_flags & VM_WRITE) != 0 &&
		(area->attribute & HVGR_MEM_ATTR_CPU_WR) == 0)) {
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u area attr 0x%lx diff with vm_flags 0x%lx",
			ctx->id, area->attribute, vma->vm_flags);
		return false;
	}

	return true;
}

static int hvgr_mem_mmap_bitmap_area(struct hvgr_ctx *ctx, struct vm_area_struct *vma)
{
	struct hvgr_mem_area *area = NULL;
	struct hvgr_mem_cpu_map *cpu_map = NULL;
	uint32_t index = (uint32_t)(vma->vm_pgoff - PFN_DOWN(HVGR_MEM_BITMAP_START_OFFSET));
	int ret;
	size_t nr_pages = vma_pages(vma);

	area = hvgr_mem_pend_area_del(&ctx->mem_ctx, index);
	if (area == NULL) {
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u has no pend area for %u", ctx->id, index);
		return -EINVAL;
	}

	hvgr_info(ctx->gdev, HVGR_MEM, "%s:%d ctx_%u get pend area 0x%lx gva 0x%lx for %u",
		__func__, __LINE__, ctx->id, (uint64_t)area, area->gva, index);

	if (nr_pages != area->max_pages) {
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u area %lu max_pages %lu != vma %lu",
			ctx->id, area->property, area->max_pages, nr_pages);
		goto area_check_fail;
	}

	if (!hvgr_mem_check_area(ctx, vma, area))
		goto area_check_fail;

	area->gva = vma->vm_start;
	if(area->property == HVGR_MEM_PROPERTY_SAME_VA) {
		if (!hvgr_mem_map_gva(NULL, area))
			goto area_check_fail;
	}

	area->uva = vma->vm_start;
	ret = hvgr_mem_cpu_mmap(ctx, vma, area, true, false);
	if (ret != 0) {
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u cpu map 0x%lx fail",
			ctx->id, area->uva);
		goto cpu_map_fail;
	}

	vma->vm_pgoff = (area->uva >> PAGE_SHIFT);

	mutex_lock(&ctx->mem_ctx.area_mutex);
	area->node.key = area->uva;
	area->node.size = area->max_pages << PAGE_SHIFT;
	if (unlikely(hvgr_mem_add_to_map_rec(ctx, area) != 0)) {
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u mmap add map_rec fail", ctx->id);
		mutex_unlock(&ctx->mem_ctx.area_mutex);
		cpu_map = vma->vm_private_data;
		vma->vm_private_data = NULL;
		vma->vm_ops = NULL;
		kfree(cpu_map);
		hvgr_mem_area_ref_put(area);
		goto cpu_map_fail;
	}

	mutex_unlock(&ctx->mem_ctx.area_mutex);
	hvgr_info(ctx->gdev, HVGR_MEM, "%s:%d ctx_%u mmap area %p 0x%lx to va 0x%lx",
		__func__, __LINE__, ctx->id, area, (index << PAGE_SHIFT), area->gva);

	return 0;
cpu_map_fail:
	if(area->property == HVGR_MEM_PROPERTY_SAME_VA)
		hvgr_mem_unmap_gva(NULL, area);
area_check_fail:
	if(area->property == HVGR_MEM_PROPERTY_SAME_VA)
		hvgr_mem_area_ref_put(area);
	return -EINVAL;
}

static int hvgr_mem_mmap_gpu_area(struct hvgr_ctx *ctx, struct vm_area_struct *vma)
{
	struct hvgr_mem_area *area = NULL;
	int ret;
	size_t nr_pages = vma_pages(vma);
	uint64_t offset_pages;

	mutex_lock(&ctx->mem_ctx.area_mutex);
	area = hvgr_mem_zone_find_area_by_gva_range(&ctx->mem_ctx, (vma->vm_pgoff << PAGE_SHIFT));
	if (area == NULL) {
		mutex_unlock(&ctx->mem_ctx.area_mutex);
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u try map invalid va 0x%lx",
			ctx->id, (vma->vm_pgoff << PAGE_SHIFT));
		return -EINVAL;
	}

	if (area->property == HVGR_MEM_PROPERTY_JIT ||
		area->property == HVGR_MEM_PROPERTY_DRIVER ||
		area->property == HVGR_MEM_PROPERTY_SPARSE ||
		area->property == HVGR_MEM_PROPERTY_ALIAS ||
		(area->attribute & HVGR_MEM_ATTR_KMD_ACCESS) != 0) {
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u area 0x%lx 0x%lx:0x%lx unsupport map",
			ctx->id, area->gva, area->property, area->attribute);
		mutex_unlock(&ctx->mem_ctx.area_mutex);
		return -EINVAL;
	}

	if (area->property == HVGR_MEM_PROPERTY_IMPORT &&
		atomic_read(&area->import.data.umm.free_tlb_cnt) != 0) {
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u area 0x%lx dma buf freed, map not allowed",
			ctx->id, area->gva);
		mutex_unlock(&ctx->mem_ctx.area_mutex);
		return -EINVAL;
	}

	offset_pages = vma->vm_pgoff - (area->gva >> PAGE_SHIFT);
	if ((area->pages <= offset_pages) || (nr_pages > (area->pages - offset_pages))) {
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u area 0x%lx 0x%lx:0x%lx:0x%lx 0x%lx 0x%lx out of range",
			ctx->id, area->gva, area->property, area->attribute, area->pages,
			nr_pages, vma->vm_pgoff);
		mutex_unlock(&ctx->mem_ctx.area_mutex);
		return -EINVAL;
	}

	if (!hvgr_mem_check_area(ctx, vma, area)) {
		mutex_unlock(&ctx->mem_ctx.area_mutex);
		return -EINVAL;
	}

	ret = hvgr_mem_cpu_mmap(ctx, vma, area, false, false);

	area->mmap_cnt++;

	hvgr_info(ctx->gdev, HVGR_MEM, "%s:%d ctx_%u area gva 0x%lx %lu:0x%lx map 0x%lx to 0x%lx ret %d",
		__func__, __LINE__, ctx->id, area->gva, area->property, area->attribute, (vma->vm_pgoff << PAGE_SHIFT),
		vma->vm_start, ret);
	mutex_unlock(&ctx->mem_ctx.area_mutex);

	return ret;
}

int hvgr_mem_mmap(struct hvgr_ctx * const ctx, struct vm_area_struct *vma)
{
	if (hvgr_mem_is_illegal_mm(ctx)) {
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u illegal mm", ctx->id);
		return -EINVAL;
	}

	if (!hvgr_mem_check_vma(ctx, vma))
		return -EINVAL;

	hvgr_info(ctx->gdev, HVGR_MEM, "%s:%d ctx_%u vm_start 0x%lx vm_end 0x%lx vma_pages %lu vm_pgoff 0x%lx vm_flags 0x%lx",
		__func__, __LINE__, ctx->id, vma->vm_start, vma->vm_end, vma_pages(vma), vma->vm_pgoff,
		vma->vm_flags);

	/* 4g rsv */
	if (vma->vm_pgoff == PFN_DOWN(HVGR_MEM_4G_RSV_OFFSET))
		return hvgr_mem_create_4g_space(ctx, vma);

	/* same va, drv mem */
	if (vma->vm_pgoff >= PFN_DOWN(HVGR_MEM_BITMAP_START_OFFSET) &&
		vma->vm_pgoff < PFN_DOWN(HVGR_MEM_BITMAP_END_OFFSET))
		return hvgr_mem_mmap_bitmap_area(ctx, vma);

	/* user map gva */
	return hvgr_mem_mmap_gpu_area(ctx, vma);
}

void hvgr_mem_free_kernel_area(struct hvgr_ctx * const ctx, struct hvgr_mem_area *area)
{
	int lock_ret;

	lock_ret = hvgr_mem_mm_write_killable_lock(ctx->mem_ctx.process_mm);
	if (unlikely(lock_ret != 0)) {
		hvgr_err(ctx->gdev, HVGR_MEM, "%s ctx_%d mm wr lock fail %d",
			__func__, ctx->id, lock_ret);
		return;
	}

	if (hvgr_mem_in_pend_area_and_del(&ctx->mem_ctx, area)) {
		hvgr_mem_unmap_kva(area);
		hvgr_mem_area_ref_put(area);
		hvgr_mem_mm_write_unlock(ctx->mem_ctx.process_mm);
		return;
	}

	if (area->property == HVGR_MEM_PROPERTY_DRIVER) {
		hvgr_mem_unmap_kva(area);
		hvgr_mem_area_ref_put(area);
	} else {
		hvgr_mem_unmap_kva(area);
	}

	hvgr_mem_mm_write_unlock(ctx->mem_ctx.process_mm);
}
