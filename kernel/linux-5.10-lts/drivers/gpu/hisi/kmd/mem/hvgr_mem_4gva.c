/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2021-2022. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */

#include "hvgr_mem_4gva.h"

#include <linux/thread_info.h>
#include <linux/mman.h>
#include <linux/slab.h>
#include <linux/rcupdate.h>

#include "hvgr_kv.h"
#include "hvgr_mem_space.h"
#include "hvgr_mem_zone.h"
#include "hvgr_mem_opts.h"
#include "hvgr_mem_api.h"
#include "hvgr_log_api.h"

/* Reserve 4G va under 4G * HVGR_MEM_4GVA_BASE_MAX */
#define HVGR_MEM_4GVA_BASE_MAX	100

static int hvgr_mem_4gva_get_reserve(struct hvgr_mem_zone * const zone)
{
	struct hvgr_ctx *ctx = NULL;
	struct hvgr_mem_dev *dmem = NULL;
	struct hvgr_mem_4gva_cb *mem_4gva = NULL;
	struct hvgr_kv_node *node = NULL;
	struct hvgr_mem_process_res_cb *res_cb = NULL;

	if (unlikely(zone->pmem == NULL))
		return -1;

	ctx = container_of(zone->pmem, struct hvgr_ctx, mem_ctx);
	dmem = &ctx->gdev->mem_dev;

	mutex_lock(&dmem->mem_process_res_lock);
	node = hvgr_kv_get(&dmem->mem_process_res_rec, (uint64_t)ctx->mem_ctx.process_mm);
	if (node == NULL) {
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u 4gva find reserve cb node fail", ctx->id);
		mutex_unlock(&dmem->mem_process_res_lock);
		return -1;
	}
	res_cb = container_of(node, struct hvgr_mem_process_res_cb, kv_node);
	mem_4gva = res_cb->mem_4gva;
	if (mem_4gva == NULL) {
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u 4gva find reserve va fail", ctx->id);
		mutex_unlock(&dmem->mem_process_res_lock);
		return -1;
	}

	/* 4gva cb is inited before */
	atomic_inc(&mem_4gva->refcounter);

	ctx->same_va_4g_base_page = (uint32_t)(mem_4gva->va_space.start >> 32);
#ifdef CONFIG_LIBLINUX
	down_write(&ctx->mem_ctx.rw_4g_sem);
#endif
	zone->prv_data = mem_4gva;
#ifdef CONFIG_LIBLINUX
	up_write(&ctx->mem_ctx.rw_4g_sem);
#endif
	mutex_unlock(&dmem->mem_process_res_lock);

	hvgr_info(ctx->gdev, HVGR_MEM, "%s:%d ctx_%u (%d:%d) get 4gva reserve", __func__, __LINE__,
		ctx->id, ctx->tgid, ctx->pid);

	return 0;
}

static void hvgr_mem_4gva_put_reserve(struct hvgr_mem_ctx * const pmem)
{
	struct hvgr_ctx *ctx = container_of(pmem, struct hvgr_ctx, mem_ctx);
	struct hvgr_mem_dev *dmem = &ctx->gdev->mem_dev;
	struct hvgr_kv_node *node = NULL;
	struct hvgr_mem_process_res_cb *res_cb = NULL;
	struct hvgr_mem_4gva_cb *mem_4gva = NULL;

	mutex_lock(&dmem->mem_process_res_lock);
	node = hvgr_kv_get(&dmem->mem_process_res_rec, (uint64_t)ctx->mem_ctx.process_mm);
	if (node == NULL)
		goto out;

	res_cb = container_of(node, struct hvgr_mem_process_res_cb, kv_node);
	mem_4gva = res_cb->mem_4gva;
	if (mem_4gva == NULL)
		goto out;

	if (atomic_read(&mem_4gva->refcounter) > 0) {
		/* 4gva is inuse */
		atomic_dec(&mem_4gva->refcounter);
		goto out;
	}

	hvgr_mem_space_term(&mem_4gva->va_space);
	kfree(mem_4gva);

	res_cb->mem_4gva = NULL;
	hvgr_info(ctx->gdev, HVGR_MEM, "%s:%d ctx_%u (%d:%d) put 4gva reserve", __func__, __LINE__,
		ctx->id, ctx->tgid, ctx->pid);
out:
	ctx->same_va_4g_base_page = 0U;
	mutex_unlock(&dmem->mem_process_res_lock);
}

static bool hvgr_mem_4gva_init(struct hvgr_mem_zone * const zone,
	void * const para)
{
	if (hvgr_mem_4gva_get_reserve(zone) == 0)
		return true;

	return false;
}

static bool hvgr_mem_4gva_check_para(struct hvgr_mem_area * const area)
{
	struct hvgr_ctx *ctx = NULL;

	if (unlikely((area == NULL) || (area->zone == NULL)))
		return false;

	if ((area->attribute & HVGR_MEM_MASK_GPU_ACCESS) == 0 ||
		((area->attribute & HVGR_MEM_ATTR_GROWABLE) != 0) ||
		((area->attribute & HVGR_MEM_ATTR_MEM_SPARSE) != 0)) {
		ctx = container_of(area->zone->pmem, struct hvgr_ctx, mem_ctx);
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u 4gva attribute INVALID. 0x%lx",
			ctx->id, area->attribute);
		return false;
	}

	area->max_pages = area->pages;

	return true;
}

static struct vm_area_struct *hvgr_mem_4gva_create_vma(struct hvgr_ctx *ctx,
	struct hvgr_mem_4gva_cb *mem_4gva, struct hvgr_mem_area * const area)
{
	struct vm_area_struct *new_vma = NULL;

	new_vma = kzalloc(sizeof(*new_vma), GFP_KERNEL);
	if (unlikely(new_vma == NULL)) {
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u 4G va alloc new vma fail", ctx->id);
		return NULL;
	}

	new_vma->vm_start = area->uva;
	new_vma->vm_end = new_vma->vm_start + (area->max_pages << PAGE_SHIFT);
	new_vma->vm_flags = (VM_SHARED | VM_DONTCOPY | VM_DONTDUMP | VM_DONTEXPAND | VM_IO |
		VM_PFNMAP);

	if ((area->attribute & HVGR_MEM_ATTR_CPU_RD) != 0)
		new_vma->vm_flags |= VM_READ;

	if ((area->attribute & HVGR_MEM_ATTR_CPU_WR) != 0)
		new_vma->vm_flags |= VM_WRITE;

	new_vma->vm_page_prot = vm_get_page_prot(new_vma->vm_flags);

	if ((area->attribute & HVGR_MEM_ATTR_CPU_CACHEABLE) == 0)
		new_vma->vm_page_prot = pgprot_writecombine(new_vma->vm_page_prot);

#ifndef CONFIG_LIBLINUX
	new_vma->vm_ops = hvgr_mem_get_vm_opts();
#endif

	rcu_read_lock();
	new_vma->vm_mm = rcu_dereference(ctx->mem_ctx.process_mm);
	rcu_read_unlock();

	return new_vma;
}

static void hvgr_mem_4gva_destroy_vma(struct vm_area_struct *vma)
{
	if (vma != NULL)
		kfree(vma);
}

static long hvgr_mem_4gva_map_uva(struct hvgr_ctx *ctx, struct hvgr_mem_area * const area)
{
	struct hvgr_setup_cpu_mmu_paras paras;

	paras.addr = area->uva;
	paras.page_array = area->page_array;
	paras.page_nums = area->pages;
#ifdef CONFIG_LIBLINUX
	paras.vm_page_prot = area->vm_page_prot;
#endif

	return hvgr_mem_setup_cpu_mmu(area->vma, &paras);
}

static void hvgr_mem_4gva_unmap_uva(struct hvgr_ctx *ctx, struct hvgr_mem_area *area,
	unsigned long address, unsigned long size)
{
	struct vm_area_struct *vma = area->vma;

#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0))
	(void)zap_vma_ptes(vma, address, size);
#else
	zap_vma_ptes(vma, address, size);
#endif
}

static bool hvgr_mem_4g_rsv_is_fine(struct hvgr_mem_4gva_cb *mem_4gva)
{
	if (mem_4gva->cpu_map == NULL)
		return false;

	return (mem_4gva->cpu_map->count == 1);
}

static inline void hvgr_mem_4gva_unlock(struct hvgr_ctx *ctx)
{
#ifdef CONFIG_LIBLINUX
	up_read(&ctx->mem_ctx.rw_4g_sem);
#else
	hvgr_mem_mm_write_unlock(ctx->mem_ctx.process_mm);
#endif
}

static long hvgr_mem_4gva_alloc(struct hvgr_mem_area * const area)
{
	uint64_t addr;
	uint64_t size;
	struct hvgr_mem_4gva_cb *mem_4gva = NULL;
	struct hvgr_ctx *ctx = container_of(area->zone->pmem, struct hvgr_ctx, mem_ctx);
#ifndef CONFIG_LIBLINUX
	int lock_ret;
#endif

#ifdef CONFIG_LIBLINUX
	down_read(&ctx->mem_ctx.rw_4g_sem);
#else
	lock_ret = hvgr_mem_mm_write_killable_lock(ctx->mem_ctx.process_mm);
	if (unlikely(lock_ret != 0)) {
		hvgr_debug(ctx->gdev, HVGR_MEM, "ctx_%d mm wr lock fail %d", ctx->id, lock_ret);
		return -EINVAL;
	}
#endif

	mem_4gva = (struct hvgr_mem_4gva_cb *)area->zone->prv_data;
	if (unlikely(mem_4gva == NULL)) {
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u 4G va alloc cb is NULL", ctx->id);
		goto prv_data_fail;
	}

	if (!hvgr_mem_4g_rsv_is_fine(mem_4gva)) {
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u 4G rsv alloc is not fine", ctx->id);
		goto prv_data_fail;
	}

	/* Alloc memory */
	size = area->max_pages << PAGE_SHIFT;
	if (hvgr_mem_space_split(&mem_4gva->va_space, 0, size, &addr) != 0) {
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u 4G va alloc va FAIL. %lu/%lu",
			ctx->id, area->pages, area->max_pages);
		goto split_addr_fail;
	}

	area->gva = addr;
	if ((area->attribute & HVGR_MEM_MASK_UMD_ACCESS) != 0) {
		area->uva = addr;
		area->vma = hvgr_mem_4gva_create_vma(ctx, mem_4gva, area);
		if (area->vma == NULL)
			goto create_vma_fail;
#ifdef CONFIG_LIBLINUX
		area->vm_page_prot= area->vma->vm_page_prot;
#endif
		if (hvgr_mem_4gva_map_uva(ctx, area) != 0) {
			hvgr_err(ctx->gdev, HVGR_MEM,
				"ctx_%u 4G va map uva FAIL. uva = %llx, %lu/%lu",
				ctx->id, area->uva, area->pages, area->max_pages);
			goto map_uva_fail;
		}

		hvgr_mem_area_ref_get(area);
	}

	hvgr_mem_attr2flag(area);
	if (!hvgr_mem_map_gva(NULL, area)) {
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u 4G va map gva FAIL. gva = %llx, %lu/%lu",
			ctx->id, area->gva, area->pages, area->max_pages);
		if ((area->attribute & HVGR_MEM_MASK_UMD_ACCESS) != 0)
			goto map_gva_fail;
		else
			goto create_vma_fail;
	}

	hvgr_mem_4gva_unlock(ctx);
	return 0;
map_gva_fail:
	hvgr_mem_4gva_unmap_uva(ctx, area, area->uva, (area->max_pages << PAGE_SHIFT));
	hvgr_mem_area_ref_put(area);
map_uva_fail:
	hvgr_mem_4gva_destroy_vma(area->vma);
	area->vma = NULL;
create_vma_fail:
	(void)hvgr_mem_space_joint(&mem_4gva->va_space, addr, size);
split_addr_fail:
prv_data_fail:
	hvgr_mem_4gva_unlock(ctx);
	return -EINVAL;
}

static long hvgr_mem_4gva_free(struct hvgr_mem_area * const area)
{
	uint64_t addr;
	uint64_t size;
	struct hvgr_ctx *ctx = container_of(area->zone->pmem, struct hvgr_ctx, mem_ctx);
	struct hvgr_mem_4gva_cb *mem_4gva = NULL;
#ifdef CONFIG_LIBLINUX
	down_read(&ctx->mem_ctx.rw_4g_sem);
#else
	hvgr_mem_mm_write_lock(ctx->mem_ctx.process_mm);
#endif
	mem_4gva = (struct hvgr_mem_4gva_cb *)area->zone->prv_data;

	if (hvgr_ctx_flag(ctx, HVGR_CTX_FLAG_DYING) || unlikely(mem_4gva == NULL)) {
		hvgr_info(ctx->gdev, HVGR_MEM, "%s:%d ctx_%u dying or 4G va free cb %p",
			__func__, __LINE__, ctx->id, mem_4gva);
		/*
		 * when process was killed, mem_4gva is null. user has not chance to free this area.
		 * we have to free the area's pages here called from hvgr_mem_free_leak_area.
		 * hvgr_mem_free_leak_area ignore the 'return false'.
		 */

		if ((area->attribute & HVGR_MEM_MASK_UMD_ACCESS) != 0) {
			hvgr_mem_4gva_unmap_uva(ctx, area, area->uva,
				(area->max_pages << PAGE_SHIFT));
			hvgr_mem_4gva_destroy_vma(area->vma);
			area->vma = NULL;
			hvgr_mem_area_ref_put(area);
		}
		hvgr_mem_unmap_gva(NULL, area);
		hvgr_mem_4gva_unlock(ctx);
		hvgr_mem_area_ref_put(area);
		return 0;
	}

	if (!hvgr_mem_4g_rsv_is_fine(mem_4gva)) {
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u 4G rsv free is not fine", ctx->id);
		hvgr_mem_4gva_unlock(ctx);
		return -EINVAL;
	}

	addr = area->gva;
	size = area->max_pages << PAGE_SHIFT;

	if ((area->attribute & HVGR_MEM_MASK_UMD_ACCESS) != 0) {
		hvgr_mem_4gva_unmap_uva(ctx, area, area->uva,
			(area->max_pages << PAGE_SHIFT));
		hvgr_mem_4gva_destroy_vma(area->vma);
		area->vma = NULL;
		hvgr_mem_area_ref_put(area);
	}

	hvgr_mem_unmap_gva(NULL, area);
	if (hvgr_mem_space_joint(&mem_4gva->va_space, addr, size) != 0) {
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u 4G va free (0x%lx:0x%lx) space joint fail",
			ctx->id, addr, size);
		hvgr_mem_4gva_unlock(ctx);
		return -EINVAL;
	}

	hvgr_mem_4gva_unlock(ctx);
	hvgr_mem_area_ref_put(area);
	return 0;
}

static bool hvgr_mem_4gva_term(struct hvgr_mem_zone * const zone)
{
#ifdef CONFIG_LIBLINUX
	down_write(&zone->pmem->rw_4g_sem);
#endif
	if (zone->prv_data == NULL) {
#ifdef CONFIG_LIBLINUX
		up_write(&zone->pmem->rw_4g_sem);
#endif
		return true;
	}
	zone->prv_data = NULL;
#ifdef CONFIG_LIBLINUX
	up_write(&zone->pmem->rw_4g_sem);
#endif
	hvgr_mem_4gva_put_reserve(zone->pmem);
	return true;
}

static const struct hvgr_zone_opts g_mem_zone_4geva = {
	.init = hvgr_mem_4gva_init,
	.term = hvgr_mem_4gva_term,
	.check_para = hvgr_mem_4gva_check_para,
	.alloc = hvgr_mem_4gva_alloc,
	.free = hvgr_mem_4gva_free,
};

bool hvgr_mem_4gva_config(struct hvgr_mem_ctx * const pmem)
{
	if (unlikely(pmem == NULL))
		return false;

	return hvgr_mem_zone_create(pmem, HVGR_MEM_PROPERTY_4G_VA,
		&g_mem_zone_4geva, NULL);
}

void hvgr_mem_4gva_deconfig(struct hvgr_mem_ctx * const pmem)
{
	struct hvgr_mem_zone * const zone = &pmem->zones[HVGR_MEM_PROPERTY_4G_VA];
	struct hvgr_ctx *ctx = container_of(pmem, struct hvgr_ctx, mem_ctx);

	if (!hvgr_kv_is_empty(&pmem->area_rec)) {
		hvgr_info(ctx->gdev, HVGR_MEM, "ctx_%u free 4g va when defconfig.", ctx->id);
		hvgr_mem_free_area_by_property(ctx, &pmem->area_rec, HVGR_MEM_PROPERTY_4G_VA);
	}
#ifdef CONFIG_LIBLINUX
	down_write(&ctx->mem_ctx.rw_4g_sem);
#endif
	if (unlikely(zone->prv_data == NULL)) {
#ifdef CONFIG_LIBLINUX
		up_write(&ctx->mem_ctx.rw_4g_sem);
#endif
		return;
	}

	zone->prv_data = NULL;
#ifdef CONFIG_LIBLINUX
	up_write(&ctx->mem_ctx.rw_4g_sem);
#endif
	hvgr_mem_4gva_put_reserve(zone->pmem);
}

static void hvgr_mem_4gva_free_easy(struct hvgr_ctx *ctx, struct hvgr_mem_area *area)
{
	if ((area->attribute & HVGR_MEM_MASK_UMD_ACCESS) != 0) {
		hvgr_mem_4gva_unmap_uva(ctx, area, area->uva,
			(area->max_pages << PAGE_SHIFT));
		hvgr_mem_4gva_destroy_vma(area->vma);
		area->vma = NULL;
		hvgr_mem_area_ref_put(area);
	}

	hvgr_mem_unmap_gva(NULL, area);
	hvgr_mem_area_ref_put(area);
}

void hvgr_mem_4gva_free_all_area(struct hvgr_ctx *ctx)
{
	struct hvgr_mem_ctx *pmem = &ctx->mem_ctx;
	struct rb_root *rbtree = NULL;
	struct rb_node *next = NULL;
	struct hvgr_kv_node *kvnode = NULL;
	struct hvgr_mem_area *area = NULL;

	mutex_lock(&pmem->area_mutex);
	rbtree = &pmem->area_rec.root;
	next = rb_first(rbtree);

	while (next != NULL) {
		kvnode = rb_entry(next, struct hvgr_kv_node, node);
		area = container_of(kvnode, struct hvgr_mem_area, node);
		next = rb_next(next);
		if (area->property == HVGR_MEM_PROPERTY_4G_VA) {
			hvgr_mem_del_from_map_rec(ctx, area);
			hvgr_mem_4gva_free_easy(ctx, area);
		}
	}
	mutex_unlock(&pmem->area_mutex);
}
