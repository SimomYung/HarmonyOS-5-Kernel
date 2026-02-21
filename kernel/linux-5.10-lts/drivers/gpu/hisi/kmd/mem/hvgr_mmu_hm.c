/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2021-2022. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */

#include <linux/kernel.h>
#include <securec.h>

#include "hvgr_mmu_api.h"
#include "hvgr_log_api.h"

#ifndef va_to_vpfn
#define va_to_vpfn(va) ((va) >> 12)
#endif

#define hvgr_hm_mmu_pt_stats_add(c) do {	\
	atomic_add(1, &(c)->mem_ctx.used_pages);	\
	atomic_add(1, &(c)->gdev->mem_dev.used_pages);	\
	atomic_add(1, &(c)->mem_ctx.used_pt_pages);	\
	(c)->gdev->mem_dev.statics.mmu_alloc_pages++;	\
} while (0)

#define hvgr_hm_mmu_pt_stats_sub(c) do {	\
	atomic_sub(1, &(c)->mem_ctx.used_pages);	\
	atomic_sub(1, &(c)->gdev->mem_dev.used_pages);	\
	atomic_sub(1, &(c)->mem_ctx.used_pt_pages);	\
	(c)->gdev->mem_dev.statics.mmu_free_pages++;	\
} while (0)

#define HVGR_MMU_GPA_NUM (1u << 9u)
#define HVGR_MMU_TBL_PA_NUM (3u + (HVGR_MMU_GPA_NUM / 512u + 1u) + \
	(HVGR_MMU_GPA_NUM / (512u * 512u) + 1u))

#define HVGR_MMU_TBL_MAX 512U

/* max pte set up cnt 512 */
#define HVGR_HM_MMU_MAX_PTE_CNT_OFFSET	9u

/*
 * hvgr_mmu_stack is used for ctx tbl buffer pool.
 * hvgr_mmu_tbl_stack is used for hm call params, array is needed.
 *
 * if hvgr_mmu_tbl_stack layout changed, struct in hm must be changed.
 */
struct hvgr_mmu_stack {
	uint32_t top;
	uint32_t num;
	uint64_t array[HVGR_MMU_TBL_MAX];
};

struct hvgr_mmu_tbl_stack {
	uint32_t top;
	uint32_t num;
	uint64_t array[HVGR_MMU_TBL_PA_NUM];
};

struct hvgr_mmu_map_paras {
	uint64_t pgd_pa;
	uint64_t vpfn;
	uint64_t pages;
	/* MMU page table attr bits */
	uint64_t mmu_flag;
	/* soft define flags */
	uint64_t flags;
	uint32_t bitmap;
	struct hvgr_mmu_tbl_stack tbl_stack;
	uint64_t gpa_array[HVGR_MMU_GPA_NUM];
};

struct hvgr_mmu_unmap_paras {
	uint64_t pgd_pa;
	uint64_t vpfn;
	uint32_t pages;
	uint32_t bitmap;
	struct hvgr_mmu_tbl_stack tbl_stack;
};

/* stack implement for hvgr_mmu_stack */
static bool hvgr_hm_mmu_stack_is_empty(struct hvgr_mmu_stack *st)
{
	if (st == NULL)
		return false;

	return st->top == 0;
}

static bool hvgr_hm_mmu_stack_is_full(struct hvgr_mmu_stack *st)
{
	if (st == NULL)
		return false;

	return st->top == st->num;
}

static long hvgr_hm_mmu_stack_push(struct hvgr_mmu_stack *st, uint64_t pa)
{
	if (st == NULL || hvgr_hm_mmu_stack_is_full(st) || pa == 0)
		return -1;

	st->array[st->top++] = pa;
	return 0;
}

static uint64_t hvgr_hm_mmu_stack_pop(struct hvgr_mmu_stack *st)
{
	uint64_t pa = 0;

	if (st == NULL || hvgr_hm_mmu_stack_is_empty(st))
		return 0;

	pa = st->array[--st->top];
	st->array[st->top] = 0;
	return pa;
}

/* stack implement for hvgr_mmu_tbl_stack */
static bool hvgr_hm_mmu_tbl_stack_is_empty(struct hvgr_mmu_tbl_stack *st)
{
	if (st == NULL)
		return false;

	return st->top == 0;
}

static bool hvgr_hm_mmu_tbl_stack_is_full(struct hvgr_mmu_tbl_stack *st)
{
	if (st == NULL)
		return false;

	return st->top == st->num;
}

static long hvgr_hm_mmu_tbl_stack_push(struct hvgr_mmu_tbl_stack *st, uint64_t pa)
{
	if (st == NULL || hvgr_hm_mmu_tbl_stack_is_full(st) || pa == 0)
		return -1;

	st->array[st->top++] = pa;
	return 0;
}

static uint64_t hvgr_hm_mmu_tbl_stack_pop(struct hvgr_mmu_tbl_stack *st)
{
	uint64_t pa = 0;

	if (st == NULL || hvgr_hm_mmu_tbl_stack_is_empty(st))
		return 0;

	pa = st->array[--st->top];
	st->array[st->top] = 0;
	return pa;
}

static uint64_t hvgr_hm_mmu_alloc_pt(struct hvgr_ctx *ctx)
{
	struct hvgr_mmu_stack *st = ctx->mem_ctx.tbl_buf;
	uint64_t entry = 0;
	struct page *page = NULL;

	mutex_lock(&ctx->mem_ctx.tbl_lock);
	if (!hvgr_hm_mmu_stack_is_empty(st)) {
		entry = hvgr_hm_mmu_stack_pop(st);
		mutex_unlock(&ctx->mem_ctx.tbl_lock);
		return entry;
	}
	mutex_unlock(&ctx->mem_ctx.tbl_lock);

	page = alloc_pt_page(0);
	if (page == NULL) {
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u MMU alloc pt fail", ctx->id);
		return entry;
	}
	entry = page_to_phys(page);
	hvgr_hm_mmu_pt_stats_add(ctx);

	return entry;
}

static void hvgr_hm_mmu_free_pt(struct hvgr_ctx *ctx, uint64_t entry, struct list_head *free_list)
{
	struct hvgr_mmu_stack *st = ctx->mem_ctx.tbl_buf;
	struct page *page = NULL;

	if (entry == 0)
		return;

	mutex_lock(&ctx->mem_ctx.tbl_lock);
	if (!hvgr_hm_mmu_stack_is_full(st)) {
		(void)hvgr_hm_mmu_stack_push(st, entry);
		mutex_unlock(&ctx->mem_ctx.tbl_lock);
		return;
	}
	mutex_unlock(&ctx->mem_ctx.tbl_lock);

	page = phys_to_page(entry);
	if (free_list != NULL) {
		list_add(&page->lru, free_list);
	} else {
		free_pt_page(page, 0);
		hvgr_hm_mmu_pt_stats_sub(ctx);
	}
}

static void hvgr_hm_mmu_free_tbl_buf(struct hvgr_ctx *ctx)
{
	struct hvgr_mem_ctx *pmem = &ctx->mem_ctx;
	uint64_t entry;
	struct page *page = NULL;

	if (pmem->tbl_buf == NULL)
		return;

	mutex_lock(&pmem->tbl_lock);
	while (!hvgr_hm_mmu_stack_is_empty(pmem->tbl_buf)) {
		entry = hvgr_hm_mmu_stack_pop(pmem->tbl_buf);
		page = phys_to_page(entry);
		free_pt_page(page, 0);
		hvgr_hm_mmu_pt_stats_sub(ctx);
	}
	mutex_unlock(&pmem->tbl_lock);

	vfree(pmem->tbl_buf);
	pmem->tbl_buf = NULL;
}

static void hvgr_hm_mmu_free_tbl_stack(struct hvgr_ctx *ctx, struct hvgr_mmu_tbl_stack *tst)
{
	uint64_t entry;

	while (!hvgr_hm_mmu_tbl_stack_is_empty(tst)) {
		entry = hvgr_hm_mmu_tbl_stack_pop(tst);
		hvgr_hm_mmu_free_pt(ctx, entry, NULL);
	}
}

static void hvgr_hm_mmu_revert_free_tbl(struct hvgr_ctx *ctx, struct hvgr_mmu_tbl_stack *tst,
	struct list_head *free_list)
{
	uint64_t entry;

	while (!hvgr_hm_mmu_tbl_stack_is_empty(tst)) {
		entry = hvgr_hm_mmu_tbl_stack_pop(tst);
		hvgr_hm_mmu_free_pt(ctx, entry, free_list);
	}
}

static int hvgr_hm_mmu_revert_insert(struct hvgr_ctx *ctx,
	uint64_t pgd_pa, struct hvgr_mmu_takedown_paras *paras)
{
	struct hvgr_mmu_unmap_paras unmap = {0};
	uint64_t vpfn = va_to_vpfn(paras->va);
	uint32_t pg_num = HVGR_MMU_GPA_NUM;
	uint64_t pg_index;
	uint32_t pages = (uint32_t)paras->pages;
	uint32_t cnt = pages / HVGR_MMU_GPA_NUM;
	uint32_t i;
	int ret = 0;

	unmap.tbl_stack.num = HVGR_MMU_TBL_PA_NUM;
	unmap.pgd_pa = pgd_pa;

	for (i = 0; i < cnt + 1; i++) {
		if (i == cnt)
			pg_num = pages % HVGR_MMU_GPA_NUM;

		if (pg_num == 0)
			break;

		pg_index = vpfn + i * HVGR_MMU_GPA_NUM;
		unmap.pages = pg_num;
		unmap.vpfn = pg_index;

		ret = liblinux_ext_hvgr_mmu_tbl_unmap(&unmap);
		hvgr_hm_mmu_revert_free_tbl(ctx, &unmap.tbl_stack, &paras->free_list);
		if (ret != 0) {
			hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u pfn 0x%llx unmap fail.",
					ctx->id, vpfn);
			paras->bitmap = 0U;
			return ret;
		}
		paras->bitmap |= unmap.bitmap;
	}

	return ret;
}

static void hvgr_hm_mmu_free_pgd(struct hvgr_ctx * const ctx)
{
	struct hvgr_mmu_unmap_paras unmap = {0};
	int ret = 0;
	uint32_t i;
	uint32_t cnt = HVGR_MMU_GPA_NUM * HVGR_MMU_GPA_NUM;

	unmap.tbl_stack.num = HVGR_MMU_TBL_PA_NUM;
	unmap.pgd_pa = ctx->mem_ctx.pgd_pa;

	for (i = 0; i < cnt; i++) {
		/* stack maybe full, until pgd_pa == 0 */
		ret = liblinux_ext_hvgr_mmu_tbl_free_pgd(&unmap);
		hvgr_hm_mmu_free_tbl_stack(ctx, &unmap.tbl_stack);
		if (ret != 0) {
			hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u free pgd fail.", ctx->id);
			break;
		}
		if (unmap.pgd_pa == 0)
			break;
	}

	return;
}

static long hvgr_hm_mmu_alloc_tbl(struct hvgr_ctx *ctx, struct hvgr_mmu_map_paras *map_paras)
{
	uint32_t i;
	uint64_t entry;
	struct hvgr_mmu_tbl_stack *tst = &map_paras->tbl_stack;

	tst->top = 0;
	tst->num = HVGR_MMU_TBL_PA_NUM;

	for (i = 0; i < tst->num; i++) {
		entry = hvgr_hm_mmu_alloc_pt(ctx);
		if (entry == 0) {
			hvgr_hm_mmu_free_tbl_stack(ctx, tst);
			return -1;
		}

		if (hvgr_hm_mmu_tbl_stack_push(tst, entry) != 0) {
			hvgr_hm_mmu_free_tbl_stack(ctx, tst);
			return -1;
		}
	}

	hvgr_info(ctx->gdev, HVGR_MEM, "hm ctx_%u alloc tbl ok", ctx->id);
	return 0;
}

static long hvgr_hm_mmu_init_setup_paras(
	struct hvgr_ctx *ctx,
	struct hvgr_mmu_setup_paras *paras,
	struct hvgr_mmu_map_paras *map_paras,
	uint32_t pg_index,
	uint32_t pg_num)
{
	uint32_t i;

	if (hvgr_hm_mmu_tbl_stack_is_empty(&map_paras->tbl_stack))
		return -1;

	map_paras->pgd_pa = ctx->mem_ctx.pgd_pa;
	map_paras->vpfn = va_to_vpfn(paras->gva)+ pg_index;
	map_paras->pages = pg_num;
	map_paras->mmu_flag = paras->mmu_flag;
	map_paras->flags = paras->flags;

	for (i = 0; i < map_paras->pages; i++)
		map_paras->gpa_array[i] = (uint64_t)page_to_phys(paras->page_array[pg_index + i]);

	return 0;
}

static long hvgr_hm_mmu_map(struct hvgr_ctx *ctx, struct hvgr_mmu_setup_paras *paras,
	struct hvgr_mmu_map_paras *map_paras, uint32_t gpa_num, uint32_t gpa_index)
{
	long ret;

	/* alloc tbl can not in mmu_mutex */
	ret = hvgr_hm_mmu_alloc_tbl(ctx, map_paras);
	if (ret != 0) {
		hvgr_err(ctx->gdev, HVGR_MEM, "hm ctx_%u alloc tbl failed", ctx->id);
		return ret;
	}

	ret = hvgr_hm_mmu_init_setup_paras(ctx, paras, map_paras, gpa_index, gpa_num);
	if (ret != 0) {
		hvgr_err(ctx->gdev, HVGR_MEM, "hm ctx_%u init map paras failed", ctx->id);
		return ret;
	}

	mutex_lock(&ctx->mem_ctx.mmu_mutex);
	ret = liblinux_ext_hvgr_mmu_tbl_map(map_paras);
	hvgr_hm_mmu_free_tbl_stack(ctx, &map_paras->tbl_stack);
	if (ret != 0)
		hvgr_err(ctx->gdev, HVGR_MEM, "hm ctx_%u tlb map failed", ctx->id);

	mutex_unlock(&ctx->mem_ctx.mmu_mutex);
	return ret;
}

/*
 * pt map\unmap\ppl no need lock,
 * 1. in area_mutex,
 * 2. area is not in area_rec, no critical section.
 */
static void hvgr_hm_area_pt_map_add(struct hvgr_mem_area *area, uint64_t pages)
{
	area->pt_map_nr += pages;
}

static void hvgr_hm_area_pt_unmap_add(struct hvgr_mem_area *area, uint64_t pages)
{
	area->pt_unmap_nr += pages;
}

void hvgr_hm_area_check_ppl(struct hvgr_mem_area *area)
{
	struct hvgr_device *gdev = hvgr_get_device();

	if (area->pt_map_nr == area->pt_unmap_nr)
		return;

	hvgr_err(gdev, HVGR_MEM, "%s check failed, area info ptmap:%u/%u, prot=%lu:0x%lx, "
		"page:%lu/%lu, uva/gva=0x%lx", __func__, area->pt_map_nr, area->pt_unmap_nr,
		area->property, area->attribute, area->pages, area->max_pages, area->gva);
}

long hvgr_mmu_set_up(struct hvgr_ctx *ctx, struct hvgr_mmu_setup_paras *paras,
	uint32_t *bitmap, struct hvgr_mem_area *area)
{
	uint32_t gpa_num = HVGR_MMU_GPA_NUM;
	struct hvgr_mmu_map_paras map_paras = {0};
	uint32_t gpa_index;
	uint32_t cnt = paras->pages >> HVGR_HM_MMU_MAX_PTE_CNT_OFFSET;
	long ret = 0;
	uint32_t i;
	struct hvgr_mmu_takedown_paras takedown_para = {0};

	if (ctx == NULL)
		return -1;

	for (i = 0; i < cnt; i++) {
		gpa_index = i << HVGR_HM_MMU_MAX_PTE_CNT_OFFSET;
		ret = hvgr_hm_mmu_map(ctx, paras, &map_paras, gpa_num, gpa_index);
		if (ret != 0) {
			/* failed then flush all */
			*bitmap = 0;
			goto fail_proc;
		}
		*bitmap |= map_paras.bitmap;
	}

	gpa_num = paras->pages & 0x1ff;
	if (gpa_num != 0) {
		gpa_index = i << HVGR_HM_MMU_MAX_PTE_CNT_OFFSET;
		ret = hvgr_hm_mmu_map(ctx, paras, &map_paras, gpa_num, gpa_index);
		if (ret != 0) {
			*bitmap = 0;
			goto fail_proc;
		}
	}
	*bitmap |= map_paras.bitmap;

	hvgr_hm_area_pt_map_add(area, paras->pages);
	hvgr_info(ctx->gdev, HVGR_MEM, "hm ctx_%u mmu set up ok", ctx->id);

	return ret;
fail_proc:
	takedown_para.va = paras->gva;
	takedown_para.pages = paras->pages;
	takedown_para.bitmap = 0U;
	INIT_LIST_HEAD(&takedown_para.free_list);

	(void)hvgr_hm_mmu_revert_insert(ctx, ctx->mem_ctx.pgd_pa, &takedown_para);
	hvgr_mmu_free_pt_pages(ctx, &takedown_para.free_list);
	hvgr_err(ctx->gdev, HVGR_MEM, "hm ctx_%u mmu mmap %lu:0x%lx:%lu:%lu fail recover",
		ctx->id, area->property, area->attribute, area->pages, area->max_pages);
	return ret;
}

long hvgr_mmu_take_down(struct hvgr_ctx * const ctx, struct hvgr_mmu_takedown_paras *paras,
	struct hvgr_mem_area *area)
{
	int ret;
	/* The default value 1 means it should be sent to HW in MMU_LEVEL_3 */
	paras->bitmap = 1U;

	if (ctx == NULL)
		return -1;

	mutex_lock(&ctx->mem_ctx.mmu_mutex);
	ret = hvgr_hm_mmu_revert_insert(ctx, ctx->mem_ctx.pgd_pa, paras);
	mutex_unlock(&ctx->mem_ctx.mmu_mutex);

	hvgr_hm_area_pt_unmap_add(area, paras->pages);
	return (long)ret;
}

long hvgr_mmu_init_proc(struct hvgr_ctx * const ctx)
{
	struct hvgr_mem_ctx *pmem = &ctx->mem_ctx;
	struct page *page = NULL;
	long ret = 0;

	mutex_lock(&pmem->mmu_mutex);
	pmem->tbl_buf = vzalloc(sizeof(struct hvgr_mmu_stack));
	if (unlikely(pmem->tbl_buf == NULL)) {
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u MMU alloc tbl buf fail", ctx->id);
		ret = -1;
		goto failed;
	}
	pmem->tbl_buf->num = HVGR_MMU_TBL_MAX;
	mutex_init(&pmem->tbl_lock);

	page = alloc_pgd_page(0);
	if (unlikely(page == NULL)) {
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u MMU alloc pgd fail", ctx->id);
		ret = -1;
		goto failed;
	}

	pmem->pgd_pa = page_to_phys(page);
	hvgr_hm_mmu_pt_stats_add(ctx);

	mutex_unlock(&pmem->mmu_mutex);
	hvgr_info(ctx->gdev, HVGR_MEM, "ctx_%u %s ok", ctx->id, __func__);
	return 0;

failed:
	if (pmem->tbl_buf != NULL) {
		vfree(pmem->tbl_buf);
		pmem->tbl_buf = NULL;
	}
	mutex_unlock(&pmem->mmu_mutex);
	return ret;
}

void hvgr_mmu_term_proc(struct hvgr_ctx * const ctx)
{
	struct hvgr_mem_ctx *pmem = &ctx->mem_ctx;
	struct page *page = NULL;

	mutex_lock(&pmem->mmu_mutex);
	if (pmem->pgd_pa != 0) {
		hvgr_hm_mmu_free_pgd(ctx);
		page = phys_to_page(pmem->pgd_pa);
		free_pgd_page(page, 0);
		hvgr_hm_mmu_pt_stats_sub(ctx);
		pmem->pgd_pa = 0;
	}
	if (pmem->tbl_buf != NULL)
		hvgr_hm_mmu_free_tbl_buf(ctx);
	mutex_unlock(&pmem->mmu_mutex);
}

void hvgr_mmu_flush_tlb(struct hvgr_ctx * const ctx)
{
	(void)hvgr_mmu_hal_enable(ctx);
}

void hvgr_mmu_free_pt_pages(struct hvgr_ctx * const ctx, struct list_head *free_list)
{
	struct page *p = NULL;
	struct page *tmp = NULL;

	list_for_each_entry_safe(p, tmp, free_list, lru) {
		list_del_init(&p->lru);
		free_pt_page(p, 0);
		hvgr_hm_mmu_pt_stats_sub(ctx);
	}
}

