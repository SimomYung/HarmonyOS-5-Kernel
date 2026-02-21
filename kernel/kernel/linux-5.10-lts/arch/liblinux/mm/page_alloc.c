// SPDX-License-Identifier: GPL-2.0-only
/*
 * Manages pages allocated to liblinux
 */
#include <linux/spinlock.h>
#include <linux/mm.h>
#include <linux/mmzone.h>
#include <linux/oom.h>

#include <uapi/linux/mman.h>
#include <liblinux/sched.h>

#include "page_alloc.h"
#include "internal.h"	/* mm/internal.h */
#include "page_alloc.h"
#include "vmemmap.h"

#define CREATE_TRACE_POINTS
#include <trace/events/oom.h>

#undef CREATE_TRACE_POINTS
#include <trace/hooks/liblinux.h>

/* from mm/oom_kill.c */
DEFINE_MUTEX(oom_lock);
bool out_of_memory(struct oom_control *oc)
{
	/*
	 * ldk don't need do oom killer. because ldk only see part of system-wide mem.
	 * here we assume oom killer always done sucess for `order = 0`.
	 * it will cause that `alloc_pages` retry until sucess.
	 */
	if (oc != NULL && oc->order == 0) {
		return true;
	}
	return false;
}

unsigned long zone_reclaimable_pages(struct zone *zone)
{
	/*
	 * Assume we can always reclaim all zone-managed pages.
	 *
	 * If there are not enough reclaimable pages, wmark check
	 * in `should_reclaim_retry()` may failed, which will cause
	 * direct-reclaim terminated early.
	 */
	return zone_managed_pages(zone);
}

static void *__extend_page_pool_alloc(struct liblinux_page_allocator *page_pool, size_t extend_size, size_t alloc_size,
				      gfp_t gfp_mask);

static struct liblinux_page_allocator *page_pool_main = NULL;
static struct liblinux_page_allocator *page_pool_cache = NULL;
static struct liblinux_page_allocator *page_pool_dma = NULL;
static struct liblinux_page_allocator *page_pool_rsv = NULL;

extern unsigned long lnxbase_shrink_mem(int size);
static void __page_pool_reclaim_handler(struct work_struct *work);

struct work_struct reclaim_work = __WORK_INITIALIZER(reclaim_work, __page_pool_reclaim_handler);

static bool page_pool_should_reclaim(const struct liblinux_page_allocator *page_pool)
{
	if (page_pool->should_reclaim == NULL) {
		return false;
	}
	return (page_pool->should_reclaim() != 0);
}

static void __page_pool_reclaim_handler(struct work_struct *work)
{
	if (work == NULL) {
		pr_err("page pool reclaim invalid work\n");
		return;
	}
	synchronize_rcu();
	(void)lnxbase_shrink_mem(-1);
}

static void __page_pool_reclaim_check(const struct liblinux_page_allocator *page_pool)
{
	if (page_pool_should_reclaim(page_pool)) {
		schedule_work(&reclaim_work);
	}
}

static int __mempool_shrink_notify(const struct liblinux_page_allocator *_allocator,
				   unsigned long vaddr, unsigned long long paddr, unsigned long size)
{
	if (_allocator != page_pool_main && _allocator != page_pool_cache && _allocator != page_pool_dma) {
		return -EINVAL;
	}

	return liblinux_pal_page_alloc_unpopulate(virt_to_page(vaddr),
						  ALIGN(size, PAGE_SIZE) / PAGE_SIZE * sizeof(struct page));
}

struct liblinux_page_allocator *default_page_allocator(void)
{
	BUG_ON(page_pool_main == NULL);
	return page_pool_main;
}

int page_pool_init(struct liblinux_page_allocator **main_pool)
{
	page_pool_main = liblinux_pal_page_allocator_get("common");
	if (page_pool_main == NULL) {
		panic("page_pool init failed\n");
		return -ENOMEM;
	}
	if (page_pool_main != NULL) {
		page_pool_main->shrink = __mempool_shrink_notify;
	}
	page_pool_cache = liblinux_pal_page_allocator_get("pagecache");
	if (page_pool_cache != NULL) {
		page_pool_cache->shrink = __mempool_shrink_notify;
	}

	page_pool_dma = liblinux_pal_page_allocator_get("dma");
	if (page_pool_dma != NULL) {
		/* only for swiotlb now */
		page_pool_dma->shrink = __mempool_shrink_notify;
	} else {
		pr_err("dma page pool was not configured\n");
	}

	page_pool_rsv = liblinux_pal_page_allocator_get("rsv");
	if (page_pool_rsv != NULL) {
		if (page_pool_rsv->extend != NULL) {
			page_pool_rsv->extend(page_pool_rsv, get_order(SZ_128K), NULL,
					      LIBLINUX_PAL_GFP_NORETRY | LIBLINUX_PAL_GFP_NOEXTRA);
		}
	} else {
		pr_err("rsv page pool was not configured\n");
	}

	if (main_pool != NULL) {
		*main_pool = page_pool_main;
	}

	return 0;
}

static void *__extend_page_pool_alloc(struct liblinux_page_allocator *page_pool, size_t extend_size, size_t alloc_size,
				      gfp_t gfp_mask)
{
	unsigned long rvaddr = 0;
	int ret;
	size_t real_size = extend_size;
	int flags = __gfp_mask_to_page_pool_flag(gfp_mask);

#ifdef CONFIG_LIBLINUX_NATIVE_BUDDY
	/*
	 * native buddy only use pool for large-order alloc,
	 * skip extra alloc to save cpu time and memory.
	 */
	flags |= LIBLINUX_PAL_GFP_NOEXTRA;
#endif /* CONFIG_LIBLINUX_NATIVE_BUDDY */

	/* extend 64K at least */
	extend_size = ALIGN(extend_size, SZ_64K);

	/* add to page pool */
	if (page_pool->extend_alloc != NULL) {
		/* always no retry with 64K force align */
		ret = page_pool->extend_alloc(page_pool, get_order(extend_size), get_order(alloc_size), &rvaddr,
					      (int)((unsigned int)flags | LIBLINUX_PAL_GFP_NORETRY));
		if (ret < 0) {
#ifdef CONFIG_LIBLINUX_MM_DEBUG
			if ((real_size >= SZ_64K) && ((gfp_mask & __GFP_NORETRY) == 0)) {
				/* dump stack on first failure to determine which driver requrie large memory */
				pr_err("extend alloc failed for alloc=0x%zx, extend=0x%zx, gfp_mask=%x, ret=%d\n",
				       alloc_size, extend_size, gfp_mask, ret);
				liblinux_pal_thread_dump(0);
			}
#endif
			/* fallback to 4K */
			extend_size = ALIGN(real_size, SZ_4K);
			ret = page_pool->extend_alloc(page_pool, get_order(extend_size), get_order(alloc_size),
						      &rvaddr, flags);
			if (ret < 0) {
				pr_err("extend alloc failed for alloc=0x%zx, extend=0x%zx, real=0x%zx, gfp_mask=%x, ret=%d\n",
				       alloc_size, extend_size, real_size, gfp_mask, ret);
				goto err_pool;
			}
		}
	}

	if (alloc_size != 0) {
		return (void *)rvaddr;
	}

err_pool:
	return NULL;
}

static void __init_single_page(struct page *page, unsigned long pfn, unsigned long zone, int nid)
{
	mm_zero_struct_page(page);
	set_page_links(page, zone, nid, pfn);
	init_page_count(page);
	page_mapcount_reset(page);
	page_cpupid_reset_last(page);
	page_kasan_tag_reset(page);

	INIT_LIST_HEAD(&page->lru);
#ifdef WANT_PAGE_VIRTUAL
	/* The shift won't overflow because ZONE_NORMAL is below 4G. */
	if (!is_highmem_idx(zone))
		set_page_address(page, __va(pfn << PAGE_SHIFT));
#endif
#ifdef CONFIG_TASK_PROTECT_LRU
	set_page_num(page, 0);
#endif
}

void __prepare_pages_info(void *addr, size_t size, unsigned long zone)
{
	struct page *page = NULL;
	size_t pgcnt;
	size_t i;

	page = virt_to_page(addr);
	pgcnt = (size >> PAGE_SHIFT);

	for (i = 0; i < pgcnt; i++) {
		unsigned long pfn = page_to_pfn(page + i);
#ifdef CONFIG_LIBLINUX_NATIVE_BUDDY
		/* for native buddy, page zone_idx should be set by pfn range */
		if (!dma_zone_only && zone <= ZONE_NORMAL)
			/* assume pfn < 4G as ZONE_DMA */
			zone = pfn < (SZ_4G >> PAGE_SHIFT) ? ZONE_DMA : ZONE_NORMAL;
#endif /* CONFIG_LIBLINUX_NATIVE_BUDDY */
		__init_single_page(page + i, pfn, zone, 0);
	}
}

static void *__page_pool_alloc(struct liblinux_page_allocator *page_pool, unsigned order,
			       gfp_t gfp_mask)
{
	size_t size = 1UL << (order + PAGE_SHIFT);
	void *kva = NULL;

	if (page_pool->alloc == NULL) {
		return NULL;
	}

	kva = page_pool->alloc(page_pool, order, NULL);
	if ((kva == NULL) && (gfp_mask & __GFP_DIRECT_RECLAIM)
			  && (page_pool_should_reclaim(page_pool))) {
		drop_slab();
		kva = page_pool->alloc(page_pool, order, NULL);
	}
	if (kva == NULL) {
		/*
		 * oops, there is no free page we can use.
		 * so, let's extend the pool and update watermark value.
		 */
		kva = __extend_page_pool_alloc(page_pool, size, size, gfp_mask);
		if (kva == NULL)
			return NULL;
	}

	return kva;
}

static void *__page_pool_alloc_locked(struct liblinux_page_allocator *page_pool,
				      unsigned order, gfp_t gfp_mask)
{
	void *kva = NULL;

	liblinux_ctx_leave();
	kva = __page_pool_alloc(page_pool, order, gfp_mask);
	liblinux_ctx_enter();

	return kva;
}

static struct page *__alloc_pages_pool(struct liblinux_page_allocator *page_pool,
					   gfp_t gfp_mask, enum zone_type zone,
					   unsigned order)
{
	size_t size = 1UL << (order + PAGE_SHIFT);
	struct page *page = NULL;
	void *kva = NULL;

	BUG_ON(page_pool == NULL);

	kva = __page_pool_alloc_locked(page_pool, order, gfp_mask);
	if (kva == NULL)
		return NULL;

	__prepare_pages_info(kva, size, zone);

	page = virt_to_page(kva);

	kasan_alloc_pages(page, order);

	if (gfp_mask & __GFP_ZERO)
		memset(kva, 0, size);

	if (order && (gfp_mask & __GFP_COMP))
		prep_compound_page(page, order);

	if (gfp_mask & LIBLINUX_PAL_GFP_RESERVE)
		set_bit(PG_rsv, &page->flags);

	trace_ldk_rvh_dfx_call_mem(LIBLINUX_DFX_PAGE_ALLOC, kva, 1UL << (order + PAGE_SHIFT), NULL);

	return page;
}

struct page *__alloc_pages_internal(gfp_t gfp_mask, unsigned order)
{
	struct liblinux_page_allocator *page_pool = page_pool_main;
	enum zone_type zone = ZONE_NORMAL;

	if (gfp_mask & GFP_DMA) {
		if (page_pool_dma == NULL) {
			return NULL;
		}
		page_pool = page_pool_dma;
		zone = ZONE_DMA;
	} else if (gfp_mask & LIBLINUX_PAL_GFP_RESERVE){
		if (page_pool_rsv == NULL) {
			return NULL;
		}
		page_pool = page_pool_rsv;
	}

	return __alloc_pages_pool(page_pool, gfp_mask, zone, order);
}
EXPORT_SYMBOL(__alloc_pages_internal);

struct page *__alloc_pages_internal_cache(gfp_t gfp_mask, unsigned order)
{
	struct liblinux_page_allocator *page_pool;

	if (unlikely(order >= MAX_ORDER)) {
		WARN_ON_ONCE(!(gfp_mask & __GFP_NOWARN));
		return NULL;
	}

	page_pool = page_pool_cache == NULL ? page_pool_main : page_pool_cache;
	return __alloc_pages_pool(page_pool, gfp_mask, ZONE_NORMAL, order);
}
EXPORT_SYMBOL(__alloc_pages_internal_cache);

void __free_pages_internal(struct page *page, unsigned int order)
{
	struct liblinux_page_allocator *page_pool = page_pool_main;
	int ret = 0;

	kasan_free_pages(page, order);

	trace_ldk_rvh_free_pages((void *)page, order, &ret);
	if (ret == 1) {
		return;
	}

	if (page_zonenum(page) == ZONE_DMA) {
		page_pool = page_pool_dma;
	}
	if (test_bit(PG_rsv, &page->flags)) {
		page_pool = page_pool_rsv;
	}

	trace_ldk_rvh_dfx_call_mem(LIBLINUX_DFX_PAGE_FREE, page_address(page),
				   1UL << (order + PAGE_SHIFT), NULL);

	liblinux_ctx_leave();
	page_pool->free(page_pool, (unsigned long)page_address(page), order);
	liblinux_ctx_enter();

	__page_pool_reclaim_check(page_pool);
}

static void *__extend_page_pool_alloc_exact(struct liblinux_page_allocator *page_pool, size_t size, size_t alloc_size,
					  gfp_t gfp_mask)
{
	unsigned long rvaddr = 0;
	int ret;
	int flags = __gfp_mask_to_page_pool_flag(gfp_mask);

	if (page_pool->extend_alloc_exact == NULL) {
		return NULL;
	}

	ret = page_pool->extend_alloc_exact(page_pool, size, alloc_size, &rvaddr, flags);
	if (ret < 0) {
		return NULL;
	}

	if (alloc_size != 0) {
		return (void *)rvaddr;
	}

	return NULL;
}

static void *__page_pool_alloc_exact(struct liblinux_page_allocator *page_pool, unsigned long size,
			       gfp_t gfp_mask)
{
	void *kva = NULL;

	if (page_pool->alloc_exact == NULL) {
		return NULL;
	}

	/* alloc first, because alloc_pages_exact may be used frequently for small size */
	kva = page_pool->alloc_exact(page_pool, size, NULL);
	if ((kva == NULL) && (gfp_mask & __GFP_DIRECT_RECLAIM)
			  && (page_pool_should_reclaim(page_pool))) {
		drop_slab();
		kva = page_pool->alloc_exact(page_pool, size, NULL);
	}
	if (kva == NULL) {
		kva = __extend_page_pool_alloc_exact(page_pool, size, size, gfp_mask);
		if (kva == 0) {
			printk(KERN_ERR "page pool extend failed\n");
			return NULL;
		}
	}

	return kva;
}

static void *__page_pool_alloc_exact_locked(struct liblinux_page_allocator *page_pool,
					    unsigned long size, gfp_t gfp_mask)
{
	void *kva = NULL;

	liblinux_ctx_leave();
	kva = __page_pool_alloc_exact(page_pool, size, gfp_mask);
	liblinux_ctx_enter();

	return kva;
}

void *__alloc_pages_pool_exact(gfp_t gfp_mask, unsigned long size)
{
	struct liblinux_page_allocator *page_pool = page_pool_main;
	unsigned long zone = ZONE_NORMAL;
	void *kva = NULL;

	if (gfp_mask & GFP_DMA) {
		if (page_pool_dma == NULL) {
			return NULL;
		}
		page_pool = page_pool_dma;
		zone = ZONE_DMA;
	}

	BUG_ON(page_pool == NULL);
	size = PAGE_ALIGN(size);

	kva = __page_pool_alloc_exact_locked(page_pool, size, gfp_mask);
	if (kva == NULL)
		return NULL;

	kasan_unpoison_shadow(kva, size);

	if (gfp_mask & __GFP_ZERO)
		memset(kva, 0, size);

	__prepare_pages_info(kva, size, zone);

	trace_ldk_rvh_dfx_call_mem(LIBLINUX_DFX_PAGE_ALLOC, kva, size, NULL);

	return kva;
}

void *__populate_vmemmap(unsigned long long pa, unsigned long size, unsigned long zone)
{
	void *addr;
	int ret;

	if ((pa + size < pa) || (!PAGE_ALIGNED(pa)) || (!PAGE_ALIGNED(size))) {
		return NULL;
	}
	if ((pa < PHYS_OFFSET) || (pa + size - PHYS_OFFSET > LIBLINUX_KMAP_SIZE)) {
		return NULL;
	}

	addr = phys_to_virt(pa);
	ret = vmemmap_pg_bitmap_update((void *)virt_to_page(addr),
		ALIGN(size, PAGE_SIZE) / PAGE_SIZE * sizeof(struct page), PG_POPULATE);
	if (ret < 0) {
		printk(KERN_ERR "Failed to populate vmemmap page, ret = %d\n", ret);
		return NULL;
	}

	__prepare_pages_info(addr, size, zone);
	return addr;
}

void *liblinux_mm_prepare_map(unsigned long long pa, unsigned long size)
{
	enum zone_type zone = dma_zone_only ? ZONE_DMA : ZONE_NORMAL;

	return __populate_vmemmap(pa, size, zone);
}

void liblinux_mm_prepare_unmap(void *va, unsigned long size)
{
	unsigned long pgcnt;
	struct page *page;
	if ((((unsigned long)va + size) < (unsigned long)va) ||
	    (!PAGE_ALIGNED((unsigned long)va)) || (!PAGE_ALIGNED(size))) {
		return;
	}
	if (((unsigned long)va < PAGE_OFFSET) ||
	    ((unsigned long)va + size - PAGE_OFFSET > LIBLINUX_KMAP_SIZE)) {
		return;
	}

	page = virt_to_page(va);
	pgcnt = (size >> PAGE_SHIFT);
	memset(page, 0, pgcnt * sizeof(struct page));
	(void)vmemmap_pg_bitmap_update((void *)virt_to_page(va),
		ALIGN(size, PAGE_SIZE) / PAGE_SIZE * sizeof(struct page), PG_UNPOPULATE);

	return;
}

unsigned long __override nr_free_buffer_pages(void)
{
	/*
	 * We cannot get the free memory of the whole system in liblinux currently.
	 * And we found that it was all used in the initialization process of each module.
	 * In this case, it can be considered a static value.
	 * So we return the totalram_pages which is not much different from the actual freepages.
	 */
	return totalram_pages();
}
EXPORT_SYMBOL_GPL(nr_free_buffer_pages);

/*
 * See zone_batchsize in mm/page_alloc.c
 *
 * LDK does not manage the entire system's memory,
 * then the value of zone_managed_pages is much smaller than the actual value.
 *
 * To keep the pageset of zone consistent with that in Linux,
 * we use totalram_pages instead of zone_managed_pages to update it again.
 */
static int __zone_batchsize(struct zone *zone)
{
	int batch;

	batch = totalram_pages() / 1024;
	/* But no more than a meg. */
	if (batch * PAGE_SIZE > 1024 * 1024)
		batch = (1024 * 1024) / PAGE_SIZE;
	batch /= 4;
	if (batch < 1)
		batch = 1;

	batch = rounddown_pow_of_two(batch + batch/2) - 1;

	return batch;
}

static void __pageset_update(struct per_cpu_pages *pcp, unsigned long high,
		unsigned long batch)
{
	pcp->batch = 1;
	smp_wmb();

	pcp->high = high;
	smp_wmb();

	pcp->batch = batch;
}

static void __pageset_set_batch(struct per_cpu_pageset *p, unsigned long batch)
{
	__pageset_update(&p->pcp, 6 * batch, max(1UL, 1 * batch));
}

static void __zone_pcp_update(struct zone *zone)
{
	unsigned int cpu;
	for_each_possible_cpu(cpu)
		__pageset_set_batch(per_cpu_ptr(zone->pageset, cpu), __zone_batchsize(zone));
}

static void hook_zone_pageset_init_late(void *args, int unuse)
{
	struct zone *zone;
	for_each_populated_zone(zone)
		__zone_pcp_update(zone);
}
INIT_VENDOR_HOOK(ldk_vh_zone_pageset_init_late, hook_zone_pageset_init_late);

static void hook_hisp_alloc_largest_pages(void *args, unsigned int order, gfp_t *gfp_mask)
{
	/* To improve the success rate of applying for large orders, when the Ldk buddy
	 * does not have large orders, the Ldk buddy directly applies for large order
	 * memory from the sysmgr.
	 */
	if (order >= 4)
		*gfp_mask |= __GFP_DIRECT_RECLAIM;
}
INIT_VENDOR_HOOK(ldk_rvh_hisp_alloc_largest_pages, hook_hisp_alloc_largest_pages);
