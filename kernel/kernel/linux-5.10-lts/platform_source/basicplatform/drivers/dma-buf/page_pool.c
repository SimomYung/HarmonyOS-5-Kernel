// SPDX-License-Identifier: GPL-2.0
/*
 * DMA BUF page pool system
 *
 * Copyright (C) 2020 Linaro Ltd.
 *
 * Based on the ION page pool code
 * Copyright (C) 2011 Google, Inc.
 */

#include <linux/freezer.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/swap.h>
#include <linux/sched/signal.h>

#include <trace/hooks/liblinux.h>

#include "page_pool.h"
#include "iofast.h"

#define DMABUF_ALLOC_CHUNK_SIZE 64u
#define gfp_mask_no_reclaim(gfp_mask) (((gfp_mask) | __GFP_NOWARN | __GFP_NORETRY) & ~__GFP_RECLAIM)

static bool dmabuf_page_pool_check_native_gfp(void)
{
	struct zone *zone = NULL;
	unsigned long wmark = 0;
	unsigned long free = 0;

	for_each_populated_zone(zone) {
		wmark += min_wmark_pages(zone);
		free += zone_page_state(zone, NR_FREE_PAGES);
	}

	if (free > (native_gfp_waterline * wmark))
		return true;
	else
		return false;
}

static void dmabuf_page_account(struct page *page)
{
	int i;
	long nr_order = 1 << compound_order(page);

	/* adjust managed/present pages */
	adjust_present_page_count(page, -nr_order);
	adjust_managed_page_count(page, -nr_order);
	liblinux_iofast_stat_move(LIBLINUX_IOFAST_PAGE_DMA_BUF,
				  LIBLINUX_IOFAST_PAGE_BUDDY, nr_order);
	for (i = 0; i < nr_order; i++)
		set_page_zone(page + i, ZONE_IOFAST);
}

#ifdef CONFIG_DMABUF_HEAPS_SYSTEM_BATCH
static void dmabuf_page_account_batch(unsigned long nr_pages, struct list_head *pages_list)
{
	int i;
	struct page *page;

	if (!dma_zone_only) {
		list_for_each_entry(page, pages_list, lru)
			dmabuf_page_account(page);
		return;
	}

	page = list_first_entry_or_null(pages_list, struct page, lru);
	if (!page)
		return;

	long nr_order = 1 << compound_order(page);

	/* adjust managed/present pages */
	adjust_present_page_count(page, -(nr_order * nr_pages));
	adjust_managed_page_count(page, -(nr_order * nr_pages));
	liblinux_iofast_stat_move(LIBLINUX_IOFAST_PAGE_DMA_BUF,
				  LIBLINUX_IOFAST_PAGE_BUDDY, (nr_order * nr_pages));
	list_for_each_entry(page, pages_list, lru) {
		for (i = 0; i < nr_order; i++)
			set_page_zone(page + i, ZONE_IOFAST);
	}
}
#endif

struct page *dmabuf_page_pool_alloc_fill(struct dmabuf_page_pool *pool, unsigned long *count)
{
	struct page *rpage = NULL;
	struct page *page_array[MAX_ARRAY_LENGTH];
	unsigned int page_num = 0;
	int i;
	struct task_struct *p = current;
	*count = 1;
 
	if (fatal_signal_pending(p))
		return NULL;

	if (dmabuf_page_pool_check_native_gfp()) {
		/* alloc pages from freelist no direct or kswapd reclaim */
		dmabuf_systrace_begin("dmabuf normal buddy order:%u", pool->order);
		rpage = alloc_pages(gfp_mask_no_reclaim(pool->gfp_mask), pool->order);
		if (rpage) {
			dmabuf_page_account(rpage);
			dmabuf_systrace_end();
			return rpage;
		}
		dmabuf_systrace_end();
	}

	dmabuf_systrace_begin("dmabuf normal iofast order:%u", pool->order);
	rpage = liblinux_iofast_alloc_pages(pool->gfp_mask, pool->order,
					       MAX_ARRAY_LENGTH, (void **)page_array,
					       &page_num, LIBLINUX_IOFAST_PAGE_DMA_BUF);
	dmabuf_systrace_end();
	for (i = 1; i < page_num; i++) {
		(*count)++;
		dmabuf_page_pool_free(pool, page_array[i]);
	}

	return rpage;
}

#ifdef CONFIG_DMABUF_HEAPS_SYSTEM_BATCH
static unsigned int dmabuf_page_iofast_merge_alloc(struct dmabuf_page_pool *pool,
			unsigned long nr_pages, struct list_head *pages_list)
{
	unsigned int i;
	unsigned int page_num;
	unsigned int nr_to_list = 0;
	struct page *page_array_impl[MAX_ARRAY_LENGTH] = {0};
	struct page **page_array = page_array_impl;
	unsigned long nr_to_alloc = nr_pages;

	if (nr_to_alloc > MAX_ARRAY_LENGTH) {
		page_array = vzalloc(sizeof(struct page *) * nr_to_alloc);
		if (!page_array) {
			pr_err("%s: page_array vzalloc failed\n", __func__);
			return nr_to_list;
		}
	} else {
		nr_to_alloc = MAX_ARRAY_LENGTH;
	}

	page_num = liblinux_iofast_palloc_burst(pool->gfp_mask, pool->order,
				LIBLINUX_IOFAST_PAGE_DMA_BUF, nr_to_alloc, (void **)page_array);
	if (!page_num)
		goto out;

	nr_to_list = min(nr_pages, page_num);
	for (i = 0; i < nr_to_list; i++)
		list_add_tail(&page_array[i]->lru, pages_list);

	for (i = nr_to_list; i < page_num; i++)
		dmabuf_page_pool_free(pool, page_array[i]);

out:
	if (nr_to_alloc > MAX_ARRAY_LENGTH)
		vfree(page_array);
	return nr_to_list;
}

static unsigned long dmabuf_pages_alloc_batch(struct dmabuf_page_pool *pool,
					    	unsigned long nr_pages, struct list_head *pages_list)
{
	struct page *tmp = NULL;
	struct page *page = NULL;
	unsigned long nr_to_alloc;
	unsigned long nr_populated;
	unsigned long nr_allocated = 0;
	LIST_HEAD(chunk);

	if (pool->order == 0) {
		dmabuf_systrace_begin("dmabuf batch buddy order:%u", pool->order);
		while (nr_allocated < nr_pages && dmabuf_page_pool_check_native_gfp()) {
			if (fatal_signal_pending(current))
				goto failed;

			nr_to_alloc = min(DMABUF_ALLOC_CHUNK_SIZE, nr_pages - nr_allocated);
			nr_populated = alloc_pages_bulk_list(gfp_mask_no_reclaim(pool->gfp_mask),
										nr_to_alloc, &chunk);
			if (nr_populated == 0)
				break;

			dmabuf_page_account_batch(nr_populated, &chunk);
			nr_allocated += nr_populated;
			list_splice_tail_init(&chunk, pages_list);
		}
		dmabuf_systrace_end();
	} else {
		dmabuf_systrace_begin("dmabuf batch buddy order:%u", pool->order);
		while (nr_allocated < nr_pages && dmabuf_page_pool_check_native_gfp()) {
			if (fatal_signal_pending(current))
				goto failed;

			page = alloc_pages(gfp_mask_no_reclaim(pool->gfp_mask), pool->order);
			if (!page)
				break;

			dmabuf_page_account(page);
			nr_allocated++;
			list_add_tail(&page->lru, pages_list);
		}
		dmabuf_systrace_end();
	}

	if (nr_allocated == nr_pages) {
		return nr_allocated;
	}

	dmabuf_systrace_begin("dmabuf batch iofast order:%u", pool->order);
	if (!(pool->extend_flag & ZONE_EXTEND_FROM_IOFAST_ONLY) || pool->order < HIGH_MISS_RATIO_ORDER) {
		while (nr_allocated < nr_pages) {
			unsigned int page_num;

			if (fatal_signal_pending(current))
				goto failed;

			page_num = dmabuf_page_iofast_merge_alloc(pool, nr_pages - nr_allocated, pages_list);
			if (!page_num)
				break;
			nr_allocated += page_num;
		}
	}
	dmabuf_systrace_end();

	return nr_allocated;

failed:
	list_for_each_entry_safe(page, tmp, pages_list, lru) {
		list_del(&page->lru);
		__free_pages(page, compound_order(page));
	}
	dmabuf_systrace_end();

	return 0;
}
#endif

static void *dmabuf_page_pool_alloc_pages(struct dmabuf_page_pool *pool)
{
	unsigned long count = 0;
	
	return dmabuf_page_pool_alloc_fill(pool, &count);
}

static inline void dmabuf_page_pool_free_pages(struct dmabuf_page_pool *pool,
					       struct page *page)
{
#if defined(CONFIG_LIBLINUX) && defined(CONFIG_HISI_GPU)
	if (unlikely(test_bit(PG_hvgr, &page->flags))) {
		WARN_ON_ONCE(true);
		return;
	}
#endif
	liblinux_iofast_free_pages(page, pool->order, LIBLINUX_IOFAST_PAGE_DMA_BUF);
}

#ifdef CONFIG_DMABUF_HEAPS_SYSTEM_BATCH
static unsigned long dmabuf_page_pool_fetch_batch(struct dmabuf_page_pool *pool,
							unsigned long nr_pages, struct list_head *batch_pages)
{
	unsigned long nr_allocated;

	nr_allocated = liblinux_dmabuf_page_pool_remove_batch(
					pool, POOL_HIGHPAGE, nr_pages, batch_pages);
	if (nr_allocated < nr_pages)
		nr_allocated += liblinux_dmabuf_page_pool_remove_batch(
			pool, POOL_LOWPAGE, nr_pages - nr_allocated, batch_pages);

	return nr_allocated;
}

#ifdef CONFIG_DMABUF_HEAPS_SYSTEM_MULTI_WMARK
static atomic64_t dma_pool_empty = ATOMIC64_INIT(0);

s64 get_dma_pool_empty_value(void)
{
	return atomic64_read(&dma_pool_empty);
}
#endif

unsigned long dmabuf_page_pool_alloc_batch(struct dmabuf_page_pool *pool,
					unsigned long nr_pages, struct list_head *batch_pages)
{
	unsigned long nr_allocated;

	if (WARN_ON(!pool))
		return NULL;

	dmabuf_systrace_begin("dmabuf batch pagepool order:%u, nr_pages:%lu", pool->order, nr_pages);
	nr_allocated = dmabuf_page_pool_fetch_batch(pool, nr_pages, batch_pages);
	dmabuf_systrace_end();
	if (nr_allocated < nr_pages) {
#ifdef CONFIG_DMABUF_HEAPS_SYSTEM_MULTI_WMARK
		atomic64_inc(&dma_pool_empty);
#endif
		nr_allocated += dmabuf_pages_alloc_batch(pool, nr_pages - nr_allocated, batch_pages);
	}

	return nr_allocated;
}
#endif

static struct page *dmabuf_page_pool_fetch(struct dmabuf_page_pool *pool)
{
	struct page *page = NULL;

	dmabuf_systrace_begin("dmabuf normal pagepool order:%u", pool->order);
	page = liblinux_dmabuf_page_pool_remove(pool, POOL_HIGHPAGE);
	if (!page)
		page = liblinux_dmabuf_page_pool_remove(pool, POOL_LOWPAGE);
	dmabuf_systrace_end();

	return page;
}

struct page *__override dmabuf_page_pool_alloc(struct dmabuf_page_pool *pool)
{
	struct page *page = NULL;

	if (WARN_ON(!pool))
		return NULL;

	page = dmabuf_page_pool_fetch(pool);
	if (!page)
		page = dmabuf_page_pool_alloc_pages(pool);

	return page;
}

#ifdef CONFIG_DMABUF_NPU_HUGE_POOL
unsigned long dmabuf_page_pool_npu_huge_alloc_batch(struct dmabuf_page_pool *huge_pool,
		struct dmabuf_page_pool *pool, unsigned long nr_pages, struct list_head *batch_pages)
{
	struct page *page = NULL;
	unsigned long nr_allocated = 0;

	if (WARN_ON((!huge_pool) || (!pool)))
		return nr_allocated;

	page = dmabuf_page_pool_fetch(huge_pool);
	if (!page) {
		page = dmabuf_page_pool_fetch(pool);
		if (!page)
			page = dmabuf_page_pool_alloc_pages(huge_pool);
	}

	if (page) {
		list_add_tail(&page->lru, batch_pages);
		nr_allocated++;
	}

	return nr_allocated;
}

struct page *dmabuf_page_npu_huge_pool_alloc(struct dmabuf_page_pool *huge_pool, struct dmabuf_page_pool *pool)
{
	struct page *page = NULL;

	if (WARN_ON((!huge_pool) || (!pool)))
		return NULL;

	page = dmabuf_page_pool_fetch(huge_pool);
	if (!page) {
		page = dmabuf_page_pool_fetch(pool);
		if (!page)
			page = dmabuf_page_pool_alloc_pages(huge_pool);
	}
	return page;
}
#endif /* CONFIG_DMABUF_NPU_HUGE_POOL */

/* TODO: Should remove this waterline, right now for prevent `pingpong` cases */
#define DMA_BUF_SHRINK_WATERLINE (64)
int dmabuf_page_pool_get_shrink_waterline(struct dmabuf_page_pool *pool)
{
	return (pool->reclaim_flag == DMABUF_PAGE_POOL_RECLAME_ALL) ? 0 : DMA_BUF_SHRINK_WATERLINE;
}

static int dmabuf_page_pool_total_water(struct dmabuf_page_pool *pool, bool high)
{
	int count = 0;
	int shrink_waterline = dmabuf_page_pool_get_shrink_waterline(pool);
	/* Remove the waterline size of pool which will not be shrinked */
	if (pool->count[POOL_LOWPAGE] > shrink_waterline) {
		count += pool->count[POOL_LOWPAGE] - shrink_waterline;
	}

	if (high && pool->count[POOL_HIGHPAGE] > shrink_waterline) {
		count += pool->count[POOL_HIGHPAGE] - shrink_waterline;
	}

	return count << pool->order;
}

static void hook_dmabuf_page_pool_do_shrink(void *data, struct dmabuf_page_pool *pool, bool high,
				      int nr_to_scan, int *freed_out)
{
	int freed = 0;

	if (nr_to_scan == 0) {
		*freed_out = dmabuf_page_pool_total_water(pool, high);
		return;
	}

	while (freed < nr_to_scan) {
		struct page *page = NULL;
		int shrink_waterline = dmabuf_page_pool_get_shrink_waterline(pool);
		/* Try to free low pages first */
		if (pool->count[POOL_LOWPAGE] > shrink_waterline) {
			page = liblinux_dmabuf_page_pool_remove(pool, POOL_LOWPAGE);
		}
		if ((!page) && (pool->count[POOL_HIGHPAGE] > shrink_waterline)) {
			page = liblinux_dmabuf_page_pool_remove(pool, POOL_HIGHPAGE);
		}
		if (!page)
			break;

		dmabuf_page_pool_free_pages(pool, page);
		freed += (1 << pool->order);
	}

	*freed_out = freed;
}
INIT_VENDOR_HOOK(ldk_rvh_dmabuf_page_pool_do_shrink, hook_dmabuf_page_pool_do_shrink);

static void hook_dmabuf_page_pool_free_pages(void *data, struct dmabuf_page_pool *pool, struct page *page)
{
	liblinux_iofast_free_pages(page, pool->order, LIBLINUX_IOFAST_PAGE_DMA_BUF);
}
INIT_VENDOR_HOOK(ldk_rvh_dmabuf_page_pool_free_pages, hook_dmabuf_page_pool_free_pages);

static void hook_track_dmabuf_free_pages_mod(void *data, struct dmabuf_page_pool *pool, int index, int cnt)
{
	if (pool->count[index] > dmabuf_page_pool_get_shrink_waterline(pool))
		liblinux_mod_dmaheap_free_pages(cnt);
}
INIT_VENDOR_HOOK(ldk_rvh_track_dmabuf_free_pages_mod, hook_track_dmabuf_free_pages_mod);
