/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2021-2022. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */

#include "hvgr_mem_api.h"

#include <linux/mman.h>
#include <linux/vmalloc.h>
#include <linux/rwsem.h>
#include <linux/mm.h>
#include <linux/delay.h>
#include <linux/bitops.h>
#include <linux/rcupdate.h>
#include <linux/bitmap.h>
#include <linux/slab.h>

#include "hvgr_defs.h"
#include "hvgr_dm_api.h"
#include "hvgr_mmu_api.h"
#include "hvgr_mem_pool.h"
#include "hvgr_mem_zone.h"
#include "hvgr_asid_api.h"
#include "hvgr_log_api.h"
#include "hvgr_mem_import.h"
#include "hvgr_mem_interval_tree.h"
#include "hvgr_mem_context_switchover.h"

static struct kmem_cache *hvgr_mem_area_slab;
int hvgr_mem_area_slab_init(void)
{
	hvgr_mem_area_slab = kmem_cache_create("hvgr_mem_area",
		(unsigned int)sizeof(struct hvgr_mem_area), 0, SLAB_HWCACHE_ALIGN, NULL);
	if (!hvgr_mem_area_slab)
		return -ENOMEM;

	return 0;
}

void hvgr_mem_area_slab_term(void)
{
	kmem_cache_destroy(hvgr_mem_area_slab);
}

struct hvgr_mem_area *hvgr_mem_alloc_area(void)
{
	return kmem_cache_zalloc(hvgr_mem_area_slab, GFP_KERNEL);
}

void hvgr_mem_free_area(struct hvgr_mem_area *area)
{
	kmem_cache_free(hvgr_mem_area_slab, area);
}

bool hvgr_mem_need_alloc_pages(struct hvgr_mem_area *area)
{
	if (area->property == HVGR_MEM_PROPERTY_SPARSE)
		return false;

	return true;
}

bool hvgr_mem_rec_area_in_mmap(struct hvgr_mem_area *area)
{
	if (area->property == HVGR_MEM_PROPERTY_DRIVER ||
		area->property == HVGR_MEM_PROPERTY_SAME_VA)
		return true;

	return false;
}

bool hvgr_mem_opt_init(struct hvgr_mem_zone * const zone,
	void * const para)
{
	return true;
}

bool hvgr_mem_opt_term(struct hvgr_mem_zone * const zone)
{
	return true;
}

bool hvgr_mem_opt_check_para(struct hvgr_mem_area * const area)
{
	struct hvgr_ctx *ctx = NULL;

	if (unlikely((area == NULL) || (area->zone == NULL)))
		return false;

	ctx = container_of(area->zone->pmem, struct hvgr_ctx, mem_ctx);
	if (unlikely(area->attribute == 0)) {
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u attribute INVALID, 0x%lx:0x%lx",
			ctx->id, area->property, area->attribute);
		return false;
	}

	if (area->attribute & HVGR_MEM_ATTR_GROWABLE) {
		if (area->pages > area->max_pages) {
			hvgr_err(ctx->gdev, HVGR_MEM,
				"ctx_%u pages INVALID, 0x%lx:0x%lx 0x%lx:0x%lx",
				ctx->id, area->property, area->attribute,
				area->pages, area->max_pages);
			return false;
		}
	} else {
		area->max_pages = area->pages;
	}

	return true;
}

long hvgr_mem_opt_alloc(struct hvgr_mem_area * const area)
{
	return 0;
}

#define HVGR_MEM_DEFAULT_GFP GFP_HIGHUSER

long hvgr_mem_alloc_pages(struct hvgr_mem_area * const area)
{
	struct hvgr_ctx *ctx = NULL;

	if (unlikely(area == NULL))
		return -EINVAL;

	ctx = container_of(area->zone->pmem, struct hvgr_ctx, mem_ctx);
	if (unlikely(area->page_array != NULL)) {
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u alloc pages but page_array is not NULL",
			ctx->id);
		return -EINVAL;
	}

	area->gfp = HVGR_MEM_DEFAULT_GFP;
	area->page_array = hvgr_mem_alloc_page_array(ctx, area);
	if (area->page_array == NULL)
		return -ENOMEM;

	return 0;
}

long hvgr_mem_free_pages(struct hvgr_mem_area * const area)
{
	struct hvgr_ctx *ctx = NULL;

	if (unlikely(area == NULL || area->page_array == NULL)) {
		struct hvgr_device *gdev = hvgr_get_device();

		hvgr_err(gdev, HVGR_MEM, "free pages para INVALID, area %s NULL",
			(area == NULL ? "is" : "is not"));
		return -EINVAL;
	}

#ifdef CONFIG_HVGR_MMU_HM
	hvgr_hm_area_check_ppl(area);
#endif

	ctx = container_of(area->zone->pmem, struct hvgr_ctx, mem_ctx);
	hvgr_mem_free_page_array(ctx, area);
	return 0;
}

bool hvgr_mem_map_kva(struct hvgr_mem_area * const area)
{
	void *addr = NULL;
	struct hvgr_ctx *ctx = NULL;
	pgprot_t prot = PAGE_KERNEL;
	hvgr_mem_area_ref_get(area);

	if ((area->attribute & HVGR_MEM_ATTR_CPU_CACHEABLE) == 0)
		prot = pgprot_writecombine(prot);

	addr = vmap(area->page_array, (uint32_t)area->pages, VM_MAP, prot);
	if (addr == NULL) {
		ctx = container_of(area->zone->pmem, struct hvgr_ctx, mem_ctx);
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u vmap kva fail", ctx->id);
		udelay(1);
		addr = vmap(area->page_array, (uint32_t)area->pages, VM_MAP, prot);
		if (addr == NULL) {
			hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u vmap kva fail", ctx->id);
			hvgr_mem_area_ref_put(area);
			return false;
		}
	}

	area->kva = ptr_to_u64(addr);

	hvgr_mem_k_area_add(area);
	return true;
}

void hvgr_mem_unmap_kva(struct hvgr_mem_area * const area)
{
	if (area->kva != 0U) {
		hvgr_mem_k_area_del(area);
		vunmap(u64_to_ptr(area->kva));
		area->kva = 0U;
		hvgr_mem_area_ref_put(area);
	}
}

void hvgr_mem_flush_pt_serial(struct hvgr_ctx * const ctx,
	uint64_t gva, uint64_t pages, uint32_t flag, uint32_t level)
{
	long ret;
	unsigned long flags;
	bool ctx_is_in_runpool = false;

	spin_lock_irqsave(&ctx->gdev->cq_dev.schedule_lock, flags);
	ctx_is_in_runpool = hvgr_cq_retain_active_ctx(ctx->gdev, ctx);
	if (!ctx_is_in_runpool) {
		spin_unlock_irqrestore(&ctx->gdev->cq_dev.schedule_lock, flags);
		return;
	}

	ret = hvgr_mmu_hal_flush_pt(ctx, gva, (uint32_t)pages, level);
	if (ret != 0)
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%d flush pt 0x%lx 0x%lx fail %d",
			ctx->id, gva, pages, ret);
	if (flag == WITH_FLUSH_L2)
		hvgr_mmu_flush_l2_caches(ctx->gdev);

	hvgr_cq_sched_release_ctx_without_lock(ctx->gdev, ctx);
	spin_unlock_irqrestore(&ctx->gdev->cq_dev.schedule_lock, flags);
}

bool hvgr_mem_map_gva(struct hvgr_ctx *ctx, struct hvgr_mem_area * const area)
{
	long ret;
	struct hvgr_mem_ctx *pmem = NULL;
	struct hvgr_mmu_setup_paras mmu_para = {0};
	uint32_t level = 0;

	if (unlikely((area == NULL) || (area->zone == NULL) ||
		(area->zone->pmem == NULL) || (area->gva == 0))) {
		struct hvgr_device *gdev = hvgr_get_device();

		hvgr_err(gdev, HVGR_MEM, "map gva para INVALID");
		return false;
	}

	pmem = area->zone->pmem;
	if (ctx == NULL)
		ctx = container_of(pmem, struct hvgr_ctx, mem_ctx);

	mmu_para.gva = area->gva;
	mmu_para.page_array = area->page_array;
	mmu_para.pages = area->pages;
	mmu_para.mmu_flag = area->mmu_flag;
	mmu_para.flags = area->flags;

	ret = hvgr_mmu_set_up(ctx, &mmu_para, &level, area);
	if (ret != 0) {
		hvgr_mem_flush_pt(ctx, area->gva, area->pages, WITHOUT_FLUSH_L2, level);
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u setup mmu for 0x%lx %lu 0x%lx fail ret=%d",
			ctx->id, area->gva, area->pages, area->mmu_flag, ret);
		return false;
	}

	hvgr_mem_flush_pt(ctx, area->gva, area->pages, WITHOUT_FLUSH_L2, level);
	return true;
}

void hvgr_mem_unmap_gva(struct hvgr_ctx *ctx, struct hvgr_mem_area * const area)
{
	struct hvgr_mem_ctx *pmem = NULL;
	struct hvgr_mmu_takedown_paras paras;

	if (unlikely((area == NULL) || (area->zone == NULL) ||
		(area->zone->pmem == NULL) || (area->gva == 0))) {
		struct hvgr_device *gdev = hvgr_get_device();

		hvgr_err(gdev, HVGR_MEM, "unmap gva para INVALID");
		return;
	}

	pmem = area->zone->pmem;
	if (ctx == NULL)
		ctx = container_of(pmem, struct hvgr_ctx, mem_ctx);

	paras.va = area->gva;
	paras.pages = area->pages;
	paras.bitmap = 0U;
	INIT_LIST_HEAD(&paras.free_list);

	(void)hvgr_mmu_take_down(ctx, &paras, area);
	hvgr_mem_flush_pt(ctx, area->gva, area->pages, WITH_FLUSH_L2, paras.bitmap);
	hvgr_mmu_free_pt_pages(ctx, &paras.free_list);
}

long hvgr_mem_zap_ptes(struct hvgr_ctx *ctx, unsigned long address, unsigned long size)
{
	long ret = 0;

#if defined(CONFIG_LIBLINUX) && !defined(CONFIG_LIBLINUX_CDC)
	hvgr_mem_mm_write_lock(ctx->mem_ctx.process_mm);

	ret = hvgr_mem_vma_zap_pte(ctx, address, size);

	hvgr_mem_mm_write_unlock(ctx->mem_ctx.process_mm);
	if (ret != 0)
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%d tgid:%u zap ptes fail 0x%lx 0x%lx ret:%d",
			ctx->id, ctx->tgid, address, size, ret);
#endif
	return ret;
}

#if !defined(CONFIG_LIBLINUX) || defined(CONFIG_LIBLINUX_CDC)
long hvgr_mem_setup_cpu_mmu(struct vm_area_struct *vma, struct hvgr_setup_cpu_mmu_paras *paras)
{
	uint32_t idx;
	int ret;
	phys_addr_t phys;
	unsigned long addr = paras->addr;
	struct hvgr_device *gdev = hvgr_get_device();

	for (idx = 0; idx < paras->page_nums; idx++) {
		if (unlikely(paras->page_array[idx] == NULL)) {
			hvgr_err(gdev, HVGR_MEM, "insert pages page_%u is NULL", idx);
			goto error;
		}

		phys = page_to_phys(paras->page_array[idx]);
#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0))
		ret = vm_insert_pfn(vma, addr, PFN_DOWN(phys));
#else
#ifdef CONFIG_LIBLINUX
		ret = remap_pfn_range(vma, addr, PFN_DOWN(phys),
			PAGE_SIZE, paras->vm_page_prot);
#else
		ret = remap_pfn_range(vma, addr, PFN_DOWN(phys),
			PAGE_SIZE, vma->vm_page_prot);
#endif
#endif
		if (ret != 0) {
			hvgr_err(gdev, HVGR_MEM, "insert pages vm insert pfn fail %d", ret);
			goto error;
		}
		addr += PAGE_SIZE;
	}

	return 0;
error:
	if (idx != 0) {
#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0))
		(void)zap_vma_ptes(vma, paras->addr, PAGE_SIZE * idx);
#else
		zap_vma_ptes(vma, paras->addr, PAGE_SIZE * idx);
#endif
	}

	return -1;
}
#endif

void hvgr_mem_attr2flag(struct hvgr_mem_area * const area)
{
	struct hvgr_mem_ctx *pmem = NULL;
	struct hvgr_ctx *ctx = NULL;

	pmem = area->zone->pmem;
	ctx = container_of(pmem, struct hvgr_ctx, mem_ctx);

	area->mmu_flag = hvgr_mmu_get_flags(ctx->gdev, area->attribute);
}

void hvgr_mem_area_ref_free(struct kref *kref)
{
	struct hvgr_mem_area *area = container_of(kref, struct hvgr_mem_area, kref);
	struct hvgr_ctx *ctx = container_of(area->zone->pmem, struct hvgr_ctx, mem_ctx);

	if (hvgr_mem_need_alloc_pages(area)) {
		if (area->property == HVGR_MEM_PROPERTY_IMPORT)
			hvgr_mem_import_free_pages(area);
#if defined(CONFIG_LIBLINUX) && !defined(CONFIG_LIBLINUX_CDC)
		else if ((area->property == HVGR_MEM_PROPERTY_SAME_VA) &&
			ctx->ctx_sw.is_background)
			hvgr_switch_free_swapped_area(area);
#endif
		else
			(void)hvgr_mem_free_pages(area);
	}

	hvgr_info(ctx->gdev, HVGR_MEM, "%s:%d ctx_%u free area_ptr 0x%lx area gva 0x%lx %lu:0x%lx %lu:%lu",
		__func__, __LINE__, ctx->id, area, area->gva, area->property, area->attribute,
		area->pages, area->max_pages);

	if (area->is_record)
		hvgr_err(ctx->gdev, HVGR_MEM, "%s:%d ctx_%u area 0x%lx %lu:0x%lx %lu:%lu free but in map_rec",
			__func__, __LINE__, ctx->id, area->gva, area->property, area->attribute,
			area->pages, area->max_pages);
	hvgr_mem_free_area(area);
}

uint64_t hvgr_mem_pend_area_add(struct hvgr_mem_ctx *mem_ctx, struct hvgr_mem_area *area)
{
	struct hvgr_ctx *ctx = container_of(mem_ctx, struct hvgr_ctx, mem_ctx);
	uint32_t index;
	uint64_t gva;

	mutex_lock(&mem_ctx->pend_area_mutex);
	if (bitmap_empty(mem_ctx->pend_area_bitmap, BITS_PER_LONG)) {
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u pend %lu area bitmap is empty",
			ctx->id, area->property);
		mutex_unlock(&mem_ctx->pend_area_mutex);
		return 0U;
	}

	index = (uint32_t)find_first_bit(mem_ctx->pend_area_bitmap, BITS_PER_LONG);
	bitmap_clear(mem_ctx->pend_area_bitmap, index, 1);
	mem_ctx->pend_areas[index] = area;

	gva = (uint64_t)index + (uint64_t)PFN_DOWN(HVGR_MEM_BITMAP_START_OFFSET);
	gva <<= PAGE_SHIFT;
	area->gva = gva;
	mutex_unlock(&mem_ctx->pend_area_mutex);

	return gva;
}

struct hvgr_mem_area *hvgr_mem_pend_area_del(struct hvgr_mem_ctx *mem_ctx, long index)
{
	struct hvgr_mem_area *area = NULL;

	if (index >= BITS_PER_LONG || index <= 0)
		return NULL;

	mutex_lock(&mem_ctx->pend_area_mutex);
	area = mem_ctx->pend_areas[index];
	mem_ctx->pend_areas[index] = NULL;
	bitmap_set(mem_ctx->pend_area_bitmap, (uint32_t)index, 1);
	mutex_unlock(&mem_ctx->pend_area_mutex);

	return area;
}

bool hvgr_mem_in_pend_area(struct hvgr_mem_ctx *mem_ctx, struct hvgr_mem_area *area)
{
	uint32_t index;
	bool ret = false;

	mutex_lock(&mem_ctx->pend_area_mutex);
	for (index = 0; index < ARRAY_SIZE(mem_ctx->pend_areas); index++) {
		if (area == mem_ctx->pend_areas[index]) {
			ret = true;
			break;
		}
	}
	mutex_unlock(&mem_ctx->pend_area_mutex);
	return ret;
}

bool hvgr_mem_in_pend_area_and_del(struct hvgr_mem_ctx *mem_ctx, struct hvgr_mem_area *area)
{
	uint32_t index;
	bool ret = false;

	mutex_lock(&mem_ctx->pend_area_mutex);
	for (index = 0; index < ARRAY_SIZE(mem_ctx->pend_areas); index++) {
		if (area == mem_ctx->pend_areas[index]) {
			mem_ctx->pend_areas[index] = NULL;
			bitmap_set(mem_ctx->pend_area_bitmap, index, 1);
			ret = true;
			break;
		}
	}
	mutex_unlock(&mem_ctx->pend_area_mutex);
	return ret;
}

/* check current->mm in ioctl alloc and hvgr_mmap */
bool hvgr_mem_is_illegal_mm(struct hvgr_ctx * const ctx)
{
	rcu_read_lock();
	if (rcu_dereference(ctx->mem_ctx.process_mm) != current->mm) {
		rcu_read_unlock();
		return true;
	}
	rcu_read_unlock();

	return false;
}

void hvgr_mem_k_area_add(struct hvgr_mem_area *area)
{
	struct hvgr_mem_ctx *mem_ctx = area->zone->pmem;

	mutex_lock(&mem_ctx->area_k_mutex);
	area->k_node.key = area->kva;
	(void)hvgr_kv_add(&mem_ctx->area_k_rec, &area->k_node);
	mutex_unlock(&mem_ctx->area_k_mutex);
}

void hvgr_mem_k_area_del(struct hvgr_mem_area *area)
{
	struct hvgr_mem_ctx *mem_ctx = area->zone->pmem;

	mutex_lock(&mem_ctx->area_k_mutex);
	hvgr_kv_del(&mem_ctx->area_k_rec, area->k_node.key);
	mutex_unlock(&mem_ctx->area_k_mutex);
}

#ifdef CONFIG_HVGR_MMU_HM
void hvgr_mem_unmap_by_gva(struct hvgr_ctx * const ctx, uint64_t va, uint32_t pages,
	struct hvgr_mem_area *area)
{
	long ret;
	struct hvgr_mmu_takedown_paras paras;

	paras.va = va;
	paras.pages = pages;
	paras.bitmap = 0U;
	INIT_LIST_HEAD(&paras.free_list);

	ret = hvgr_mmu_take_down(ctx, &paras, area);
	if (unlikely(ret != 0)) {
		hvgr_err(ctx->gdev, HVGR_MEM, "%s ctx_%u unmap 0x%lx:%u fail",
			__func__, ctx->id, va, pages);
		return;
	}

	hvgr_mem_flush_pt(ctx, va, pages, WITH_FLUSH_L2, paras.bitmap);
	hvgr_mmu_free_pt_pages(ctx, &paras.free_list);
}
#endif

long hvgr_mem_sc_get_policy_info(struct hvgr_ctx * const ctx, uint64_t policy_info,
	uint32_t info_num, void **infos)
{
	if (infos != NULL)
		*infos = NULL;
	return 0;
}
void hvgr_mem_sc_update_policy(struct hvgr_ctx * const ctx, void *policy_info,
	uint32_t info_num)
{
}

long hvgr_mem_vmap_user_addr(struct hvgr_ctx * const ctx, uint64_t gpu_addr,
	void **target_addr, uint64_t *offset, uint32_t len)
{
	struct hvgr_mem_area *area = NULL;
	pgprot_t prot = PAGE_KERNEL;
	uint64_t end_addr = 0;

	if (target_addr == NULL || offset == NULL) {
		hvgr_err(ctx->gdev, HVGR_MEM, "input para is invalid!");
		return -EINVAL;
	}

	area = hvgr_mem_zone_find_area_by_gva_range(&ctx->mem_ctx, gpu_addr);
	if (area == NULL) {
		hvgr_err(ctx->gdev, HVGR_MEM, "map user addr no area for gva=0x%lx.", gpu_addr);
		return -EINVAL;
	}

	if (((area->attribute & HVGR_MEM_ATTR_CPU_WR) == 0) ||
		((area->attribute & HVGR_MEM_ATTR_CPU_RD) == 0)) {
		hvgr_err(ctx->gdev, HVGR_MEM, "map user addr gva=0x%lx %lu:0x%lx attr err.",
			gpu_addr, area->property, area->attribute);
		return -EINVAL;
	}

	end_addr = gpu_addr + (area->page_offset << PAGE_SHIFT) + len;
	if ((gpu_addr < area->gva) || (end_addr > (area->gva + (area->pages << PAGE_SHIFT)))) {
		hvgr_err(ctx->gdev, HVGR_MEM,
			"gpu_addr 0x%lx area err gva = 0x%lx page_offset = 0x%lx pages = 0x%lx.",
			gpu_addr, area->gva, area->page_offset, area->pages);
		return -EINVAL;
	}

	*offset = gpu_addr - area->gva + (area->page_offset << PAGE_SHIFT);
	prot = pgprot_writecombine(prot);
	*target_addr = vmap(area->page_array, (unsigned int)area->pages, VM_MAP, prot);

	return 0;
}

void hvgr_mem_vunmap_user_addr(const void *gpu_addr)
{
	if (gpu_addr == NULL)
		return;

	vunmap(gpu_addr);
}

long hvgr_mem_add_to_map_rec(struct hvgr_ctx * const ctx, struct hvgr_mem_area *area)
{
	if (unlikely(!hvgr_kv_add(&ctx->mem_ctx.area_rec, &area->node))) {
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u area 0x%llx %lu:0x%llx:%lu:%lu map rec fail",
			ctx->id, area->gva, area->property, area->attribute, area->pages,
			area->max_pages);
		return -1;
	}

	area->is_record = true;
	return 0;
}

void hvgr_mem_del_from_map_rec(struct hvgr_ctx * const ctx, struct hvgr_mem_area *area)
{
	hvgr_kv_del(&ctx->mem_ctx.area_rec, area->node.key);
	area->is_record = false;
}

