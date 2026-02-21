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
#include <trace/events/dma_buf.h>

#include "page_pool.h"
#include "iofast.h"

#define gfp_mask_no_reclaim(gfp_mask) (((gfp_mask) | __GFP_NOWARN | __GFP_NORETRY) & ~__GFP_RECLAIM)

static const char * const alloc_buf_log[] = {
	"strat alloc page",
	"end alloc page",
};

static const char * const iofast_alloc_buf_log[] = {
	"strat iofast alloc",
	"end iofast alloc",
};


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

static void dmabuf_page_pool_record_large(struct dmabuf_page_pool *pool, const char *record_log)
{
	if (atomic64_read(&pool->large_size_cnt) > 0) {
        pr_err("%s: pid %d comm=%s  %s order:%u\n", __func__, current->pid, current->comm,
            record_log, pool->order);
    }
    return;
}

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
		trace_dma_buf_alloc_log(pool->order, alloc_buf_log[0], p); // begin
		rpage = alloc_pages(gfp_mask_no_reclaim(pool->gfp_mask), pool->order);
		if (rpage) {
			/* adjust managed/present pages */
			adjust_present_page_count(rpage, -(1 << pool->order));
			adjust_managed_page_count(rpage, -(1 << pool->order));
			liblinux_iofast_stat_move(LIBLINUX_IOFAST_PAGE_DMA_BUF, LIBLINUX_IOFAST_PAGE_BUDDY, (1UL << pool->order));

			for (i = 0; i < 1 << pool->order; i++)
				set_page_zone(rpage + i, ZONE_IOFAST);
			trace_dma_buf_alloc_log(pool->order, alloc_buf_log[1], p); // end
			return rpage;
		}
	}
	trace_dma_buf_alloc_log(pool->order, iofast_alloc_buf_log[0], p); // begin
	dmabuf_page_pool_record_large(pool, iofast_alloc_buf_log[0]);
	rpage = liblinux_iofast_alloc_pages(pool->gfp_mask, pool->order,
					       MAX_ARRAY_LENGTH, (void **)page_array,
					       &page_num, LIBLINUX_IOFAST_PAGE_DMA_BUF);
	dmabuf_page_pool_record_large(pool, iofast_alloc_buf_log[1]);
	trace_dma_buf_alloc_log(pool->order, iofast_alloc_buf_log[1], p); // end
	for (i = 1; i < page_num; i++) {
		(*count)++;
		dmabuf_page_pool_free(pool, page_array[i]);
	}
	return rpage;
}

static void *dmabuf_page_pool_alloc_pages(struct dmabuf_page_pool *pool)
{
	unsigned long count = 0;
	
	return dmabuf_page_pool_alloc_fill(pool, &count);
}

static inline void dmabuf_page_pool_free_pages(struct dmabuf_page_pool *pool,
					       struct page *page)
{
	liblinux_iofast_free_pages(page, pool->order, LIBLINUX_IOFAST_PAGE_DMA_BUF);
}

static struct page *dmabuf_page_pool_fetch(struct dmabuf_page_pool *pool)
{
	struct page *page = NULL;

	page = liblinux_dmabuf_page_pool_remove(pool, POOL_HIGHPAGE);
	if (!page)
		page = liblinux_dmabuf_page_pool_remove(pool, POOL_LOWPAGE);

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
static int dmabuf_page_pool_get_shrink_waterline(struct dmabuf_page_pool *pool)
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
