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
#ifdef CONFIG_DFX_MEMCHECK
#include <linux/dma-buf.h>
#endif
#ifdef CONFIG_LIBLINUX
#include <linux/proc_fs.h>
#endif
#include <trace/hooks/liblinux.h>

#include "page_pool.h"

static LIST_HEAD(pool_list);
static DEFINE_MUTEX(pool_list_lock);

#ifdef CONFIG_DFX_MEMCHECK
static atomic_long_t ion_total_cache = ATOMIC_INIT(0);

u64 dma_get_total_cache(void)
{
	return atomic_long_read(&ion_total_cache);
}

static void track_dmabuf_add(long nr_pages)
{
	atomic_long_add(nr_pages, &ion_total_cache);
}

static void track_dmabuf_sub(long nr_pages)
{
	atomic_long_sub(nr_pages, &ion_total_cache);
}
#endif

static inline struct page *dmabuf_page_pool_alloc_pages(
					struct dmabuf_page_pool *pool)
{
	if (fatal_signal_pending(current))
		return NULL;
	return alloc_pages(pool->gfp_mask, pool->order);
}

static inline void dmabuf_page_pool_free_pages(struct dmabuf_page_pool *pool,
					       struct page *page)
{
#ifdef CONFIG_LIBLINUX
	if (trace_ldk_rvh_dmabuf_page_pool_free_pages_enabled()) {
		trace_ldk_rvh_dmabuf_page_pool_free_pages(pool, page);
		return;
	}
#endif /* CONFIG_LIBLINUX */
	__free_pages(page, pool->order);
}

static void dmabuf_page_pool_add(struct dmabuf_page_pool *pool, struct page *page)
{
	int index;

	if (PageHighMem(page))
		index = POOL_HIGHPAGE;
	else
		index = POOL_LOWPAGE;

	mutex_lock(&pool->mutex);
	list_add_tail(&page->lru, &pool->items[index]);
	pool->count[index]++;
	trace_ldk_rvh_track_dmabuf_free_pages_mod(pool, index, 1 << pool->order);
	mutex_unlock(&pool->mutex);
	mod_node_page_state(page_pgdat(page), NR_KERNEL_MISC_RECLAIMABLE,
			    1 << pool->order);
	zone_page_state_add(1 << pool->order, page_zone(page),
			    NR_DMAHEAPCACHE_PAGES);
#ifdef CONFIG_DFX_MEMCHECK
	track_dmabuf_add(1 << pool->order);
#endif
}

static struct page *dmabuf_page_pool_remove(struct dmabuf_page_pool *pool, int index)
{
	struct page *page;

	mutex_lock(&pool->mutex);
	page = list_first_entry_or_null(&pool->items[index], struct page, lru);
	if (page) {
		trace_ldk_rvh_track_dmabuf_free_pages_mod(pool, index, -(1 << pool->order));
		pool->count[index]--;
		list_del(&page->lru);
		mod_node_page_state(page_pgdat(page), NR_KERNEL_MISC_RECLAIMABLE,
				    -(1 << pool->order));
		zone_page_state_add(-1 << pool->order, page_zone(page),
				    NR_DMAHEAPCACHE_PAGES);
#ifdef CONFIG_DFX_MEMCHECK
		track_dmabuf_sub(1 << pool->order);
#endif
	}
	mutex_unlock(&pool->mutex);

	return page;
}

#ifdef CONFIG_DMABUF_HEAPS_SYSTEM_BATCH
static void dmabuf_page_pool_account_batch(struct dmabuf_page_pool *pool,
				unsigned long nr_pages, struct list_head *pages_list)
{
	struct page *page;
	long nr_order = 1 << pool->order;

	if (!dma_zone_only) {
		list_for_each_entry(page, pages_list, lru) {
			mod_node_page_state(page_pgdat(page), NR_KERNEL_MISC_RECLAIMABLE, -nr_order);
			zone_page_state_add(-nr_order, page_zone(page), NR_DMAHEAPCACHE_PAGES);
		}
		return;
	}

	page = list_first_entry_or_null(pages_list, struct page, lru);
	if (!page)
		return;

	mod_node_page_state(page_pgdat(page), NR_KERNEL_MISC_RECLAIMABLE, -(nr_order * nr_pages));
	zone_page_state_add(-(nr_order * nr_pages), page_zone(page), NR_DMAHEAPCACHE_PAGES);
}

static unsigned long dmabuf_page_pool_remove_batch(struct dmabuf_page_pool *pool,
					int index, unsigned long nr_pages, struct list_head *batch_pages)
{
	int count;
	int waterline;
	int count_above_waterline;
	struct list_head *items = &pool->items[index];
	long nr_order = 1 << pool->order;
	unsigned long nr_allocated = 0;
	struct page *page;
	LIST_HEAD(pages_list);

	waterline = dmabuf_page_pool_get_shrink_waterline(pool);
	mutex_lock(&pool->mutex);
	count = pool->count[index];
	if (count <= nr_pages) {
		count_above_waterline = (count > waterline) ? (count - waterline) : 0;
		dmabuf_page_pool_account_batch(pool, count, items);
		trace_ldk_rvh_track_dmabuf_free_pages_mod(pool, index, -(nr_order * count_above_waterline));
#ifdef CONFIG_DFX_MEMCHECK
		track_dmabuf_sub(nr_order * count);
#endif
		list_splice_tail_init(items, batch_pages);
		nr_allocated += count;
		pool->count[index] = 0;
	} else {
		list_for_each_entry(page, items, lru) {
			nr_allocated++;
			if (nr_allocated == nr_pages)
				break;
		}
		list_cut_position(&pages_list, items, &page->lru);

		count_above_waterline = (count > waterline) ? min((int)nr_allocated, (count - waterline)) : 0;
		dmabuf_page_pool_account_batch(pool, nr_allocated, &pages_list);
		trace_ldk_rvh_track_dmabuf_free_pages_mod(pool, index, -(nr_order * count_above_waterline));
#ifdef CONFIG_DFX_MEMCHECK
		track_dmabuf_sub(nr_order * nr_allocated);
#endif
		list_splice_tail_init(&pages_list, batch_pages);
		pool->count[index] -= nr_allocated;
	}
	mutex_unlock(&pool->mutex);

	return nr_allocated;
}

unsigned long liblinux_dmabuf_page_pool_remove_batch(struct dmabuf_page_pool *pool,
					int index, unsigned long nr_pages, struct list_head *batch_pages)
{
	return dmabuf_page_pool_remove_batch(pool, index, nr_pages, batch_pages);
}
#endif

#ifdef CONFIG_LIBLINUX
struct page *liblinux_dmabuf_page_pool_remove(struct dmabuf_page_pool *pool, int index)
{
	return dmabuf_page_pool_remove(pool, index);
}
#endif

static struct page *dmabuf_page_pool_fetch(struct dmabuf_page_pool *pool)
{
	struct page *page = NULL;

	page = dmabuf_page_pool_remove(pool, POOL_HIGHPAGE);
	if (!page)
		page = dmabuf_page_pool_remove(pool, POOL_LOWPAGE);

	return page;
}

#ifdef CONFIG_LIBLINUX_OVERRIDE
noinline
#endif
struct page *dmabuf_page_pool_alloc(struct dmabuf_page_pool *pool)
{
	struct page *page = NULL;

	if (WARN_ON(!pool))
		return NULL;
	page = dmabuf_page_pool_fetch(pool);
	if (!page)
		page = dmabuf_page_pool_alloc_pages(pool);
	return page;
}
EXPORT_SYMBOL_GPL(dmabuf_page_pool_alloc);

void dmabuf_page_pool_free(struct dmabuf_page_pool *pool, struct page *page)
{
	if (WARN_ON(pool->order != compound_order(page)))
		return;

	dmabuf_page_pool_add(pool, page);
}
EXPORT_SYMBOL_GPL(dmabuf_page_pool_free);

static int dmabuf_page_pool_total(struct dmabuf_page_pool *pool, bool high)
{
	int count = pool->count[POOL_LOWPAGE];

	if (high)
		count += pool->count[POOL_HIGHPAGE];

	return count << pool->order;
}

struct dmabuf_page_pool *dmabuf_page_pool_create(gfp_t gfp_mask, unsigned int order)
{
	struct dmabuf_page_pool *pool = kmalloc(sizeof(*pool), GFP_KERNEL);
	int i;

	if (!pool)
		return NULL;

	for (i = 0; i < POOL_TYPE_SIZE; i++) {
		pool->count[i] = 0;
		INIT_LIST_HEAD(&pool->items[i]);
	}
	pool->gfp_mask = gfp_mask | __GFP_COMP;
	pool->order = order;
	pool->reclaim_flag = 0;
	mutex_init(&pool->mutex);

	mutex_lock(&pool_list_lock);
	list_add(&pool->list, &pool_list);
	mutex_unlock(&pool_list_lock);

	return pool;
}
EXPORT_SYMBOL_GPL(dmabuf_page_pool_create);

void dmabuf_page_pool_destroy(struct dmabuf_page_pool *pool)
{
	struct page *page;
	int i;

	/* Remove us from the pool list */
	mutex_lock(&pool_list_lock);
	list_del(&pool->list);
	mutex_unlock(&pool_list_lock);

	/* Free any remaining pages in the pool */
	for (i = 0; i < POOL_TYPE_SIZE; i++) {
		while ((page = dmabuf_page_pool_remove(pool, i)))
			dmabuf_page_pool_free_pages(pool, page);
	}

	kfree(pool);
}
EXPORT_SYMBOL_GPL(dmabuf_page_pool_destroy);

#ifdef CONFIG_LIBLINUX
#define DMA_BUF_SHRINK_WATERLINE (64)
unsigned int dmabuf_page_pool_reclaim(unsigned int order, struct page **reclaimed_pages, unsigned int len)
{
	struct dmabuf_page_pool *pool;
	unsigned long nr_needed = (unsigned long)(len << order);
	unsigned long nr_reclaimed = 0;
	unsigned int reclaimed = 0;

	mutex_lock(&pool_list_lock);
	list_for_each_entry(pool, &pool_list, list) {
		if (pool->order < order)
			continue;

		while (nr_reclaimed < nr_needed) {
			struct page *page = NULL;
			/* Try to free low pages first */
			if (pool->count[POOL_LOWPAGE] > DMA_BUF_SHRINK_WATERLINE)
				page = dmabuf_page_pool_remove(pool, POOL_LOWPAGE);

			if ((!page) && (pool->count[POOL_HIGHPAGE] > DMA_BUF_SHRINK_WATERLINE))
				page = dmabuf_page_pool_remove(pool, POOL_HIGHPAGE);

			if (!page)
				break;

			reclaimed_pages[reclaimed] = page;
			reclaimed++;
			nr_reclaimed += (1UL << pool->order);
		}

		if (reclaimed == len)
			break;
	}
	mutex_unlock(&pool_list_lock);
	liblinux_iofast_stat_move(LIBLINUX_IOFAST_PAGE_BUDDY, LIBLINUX_IOFAST_PAGE_DMA_BUF, nr_reclaimed);

	return reclaimed;
}

static int page_pool_show(struct seq_file *s, void *d)
{
	struct dmabuf_page_pool *pool = NULL;
	int i;

	mutex_lock(&pool_list_lock);
	list_for_each_entry(pool, &pool_list, list) {
		seq_printf(s, "%d order %u highmem pages %lu total\n",
				pool->count[POOL_HIGHPAGE], pool->order,
				(PAGE_SIZE << pool->order) * (pool->count[POOL_HIGHPAGE]));
		seq_printf(s, "%d order %u lowmem pages %lu total\n",
				pool->count[POOL_LOWPAGE], pool->order,
				(PAGE_SIZE << pool->order) * (pool->count[POOL_LOWPAGE]));
	}
	mutex_unlock(&pool_list_lock);

	return 0;
}

static int page_pool_open(struct inode *inode, struct file *file)
{
	return single_open(file, page_pool_show, inode->i_private);
}

static const struct proc_ops page_pool_proc_ops = {
	.proc_open	= page_pool_open,
	.proc_read	= seq_read,
	.proc_lseek	= seq_lseek,
	.proc_release	= single_release,
};

static int __init page_pool_proc_init(void)
{
	proc_create("dmaheap_pagepool", S_IFREG | S_IRUGO | S_IWUSR,
			NULL, &page_pool_proc_ops);
	return 0;
}
late_initcall(page_pool_proc_init);
#endif

int dmabuf_page_pool_do_shrink(struct dmabuf_page_pool *pool, gfp_t gfp_mask,
				      int nr_to_scan)
{
	int freed = 0;
	bool high;

	if (current_is_kswapd())
		high = true;
	else
		high = !!(gfp_mask & __GFP_HIGHMEM);

#ifdef CONFIG_LIBLINUX
	if (trace_ldk_rvh_dmabuf_page_pool_do_shrink_enabled()) {
		trace_ldk_rvh_dmabuf_page_pool_do_shrink(pool, high, nr_to_scan, &freed);
		return freed;
	}
#endif /* CONFIG_LIBLINUX */

	if (nr_to_scan == 0)
		return dmabuf_page_pool_total(pool, high);

	while (freed < nr_to_scan) {
		struct page *page;

		/* Try to free low pages first */
		page = dmabuf_page_pool_remove(pool, POOL_LOWPAGE);
		if (!page)
			page = dmabuf_page_pool_remove(pool, POOL_HIGHPAGE);

		if (!page)
			break;

		dmabuf_page_pool_free_pages(pool, page);
		freed += (1 << pool->order);
	}

	return freed;
}

static int dmabuf_page_pool_shrink(gfp_t gfp_mask, int nr_to_scan)
{
	struct dmabuf_page_pool *pool;
	int nr_total = 0;
	int nr_freed;
	int only_scan = 0;

	if (!nr_to_scan)
		only_scan = 1;

	mutex_lock(&pool_list_lock);
	list_for_each_entry(pool, &pool_list, list) {
		if (only_scan) {
			nr_total += dmabuf_page_pool_do_shrink(pool,
							       gfp_mask,
							       nr_to_scan);
		} else {
			nr_freed = dmabuf_page_pool_do_shrink(pool,
							      gfp_mask,
							      nr_to_scan);
			nr_to_scan -= nr_freed;
			nr_total += nr_freed;
			if (nr_to_scan <= 0)
				break;
		}
	}
	mutex_unlock(&pool_list_lock);

	return nr_total;
}

static unsigned long dmabuf_page_pool_shrink_count(struct shrinker *shrinker,
						   struct shrink_control *sc)
{
	return dmabuf_page_pool_shrink(sc->gfp_mask, 0);
}

static unsigned long dmabuf_page_pool_shrink_scan(struct shrinker *shrinker,
						  struct shrink_control *sc)
{
	if (sc->nr_to_scan == 0)
		return 0;
	return dmabuf_page_pool_shrink(sc->gfp_mask, sc->nr_to_scan);
}

struct shrinker pool_shrinker = {
	.count_objects = dmabuf_page_pool_shrink_count,
	.scan_objects = dmabuf_page_pool_shrink_scan,
	.seeks = DEFAULT_SEEKS,
	.batch = 0,
#ifdef CONFIG_LIBLINUX
	.is_stand_alone = true,
#endif /* CONFIG_LIBLINUX */
};

static int dmabuf_page_pool_init_shrinker(void)
{
	return register_shrinker(&pool_shrinker);
}
module_init(dmabuf_page_pool_init_shrinker);
MODULE_LICENSE("GPL v2");
