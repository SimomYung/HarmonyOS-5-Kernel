// SPDX-License-Identifier: GPL-2.0-only
/*
 * helper APIs for native buddy support
 *  - simple replacement of mm/vmscan.c
 */
#include <linux/mm.h>
#include <linux/mm.h>
#include <linux/cpuset.h>
#include <linux/kthread.h>
#include <linux/topology.h>

#ifdef CONFIG_LIBLINUX_SYS_UNION_SHRINKER
#include <linux/mm_inline.h>
#include <linux/delayacct.h>
#include <linux/sched/mm.h>
#include "mm_internal.h"
#endif
#include <securec.h>

#include "internal.h"	/* mm/internal.h */
#include "page_alloc.h"
#include "vmemmap.h"

static struct zone *get_zone_by_type(enum zone_type ztype)
{
	pg_data_t *pgdat = NODE_DATA(0);

	return &pgdat->node_zones[ztype];
}

/* adjust pgdata/zone present pages, count can be negative */
void adjust_present_page_count(struct page *pg, long count)
{
	struct zone *zone = page_zone(pg);
	unsigned long flags;

	if (!populated_zone(zone))
		panic("liblinux: %s: page zone abnormal, zone id=%d, pfn=0x%lx, count=%ld\n",
			__func__, page_zone_id(pg), page_to_pfn(pg), count);

	pgdat_resize_lock(zone->zone_pgdat, &flags);
	page_zone(pg)->present_pages += count;
	zone->zone_pgdat->node_present_pages += count;
	pgdat_resize_unlock(zone->zone_pgdat, &flags);
}

#define ZONE_EXTEND_ARRAY_LEN	16
static void __add_memblock(struct zonelist *zonelist, enum zone_type type,
			   unsigned long pfn, unsigned int order)
{
	struct page *pg = pfn_to_page(pfn);
	long nr_pages = 1 << order;
	unsigned long end_pfn = pfn + nr_pages;
	unsigned long iter_pfn;
	unsigned long cur_nr_pages;
	struct zone *zone = NULL;
	int ret;

	/* add section if not exist */
	for (iter_pfn = pfn; iter_pfn < end_pfn; iter_pfn += cur_nr_pages) {
		cur_nr_pages = min(end_pfn - iter_pfn,
				   SECTION_ALIGN_UP(iter_pfn + 1) - iter_pfn);
		if (!present_section(__pfn_to_section(iter_pfn))) {
			ret = sparse_add_section(0, iter_pfn, cur_nr_pages, NULL);
			if (ret < 0) {
				pr_warn("add section failed, pfn=0x%lx, order=%u, ret=%d\n",
					pfn, order, ret);
				return;
			}
		}
	}

	/* init pages */
	for_each_populated_zone(zone)
		memmap_init_zone_range(zone, pfn, pfn + nr_pages);

	if (dma_zone_only && page_zone_id(pg) != ZONE_DMA)
		panic("invalid pg zone id=%d, pfn=0x%lx, order=%u\n",
		      page_zone_id(pg), pfn, order);

	if (!pfn_valid(pfn) || page_zone(pg) != page_zone(pg + nr_pages - 1)) {
		panic("invalid cross-zone pfn=0x%lx, order=%d, zone_start=%pS/%d, zone_end=%pS/%d\n",
			page_to_pfn(pg), order,
			page_zone(pg), page_zonenum(pg),
			page_zone(pg+nr_pages), page_zonenum(pg+nr_pages));
	}

	/* adjust pgdata/zone present pages */
	adjust_present_page_count(pg, nr_pages);
	/*
	 * do what adjust_managed_page_count() does except updating managed_pages
	 * which has been done in __free_pages_core() up there
	 */
	totalram_pages_add(nr_pages);
#ifdef CONFIG_HIGHMEM
	if (PageHighMem(pg))
		totalhigh_pages_add(nr_pages);
#endif
	__free_pages_core(pg, order);
}

static DEFINE_MUTEX(add_memblock_lock);

/**
 * @brief lock add_memblock avoid recursive
 * @return true is recursive get lock
 * @return false first get lock
 */
static bool add_memblock_mutex_lock_recursive(struct mutex *lock)
{
	switch (mutex_trylock_recursive(lock)) {
	case MUTEX_TRYLOCK_RECURSIVE:
		return true;
	case MUTEX_TRYLOCK_SUCCESS:
		return false;
	case MUTEX_TRYLOCK_FAILED:
		mutex_lock(lock);
		return false;
	default:
		BUG();
	}
}

static void __add_memblock_locked(struct zonelist *zonelist, enum zone_type type,
								unsigned long pfn, unsigned int order)
{
	bool is_recursive = add_memblock_mutex_lock_recursive(&add_memblock_lock);
	__add_memblock(zonelist, type, pfn, order);
	if (!is_recursive)
		mutex_unlock(&add_memblock_lock);
}

static unsigned long
__extend_from_dmabuf(struct zonelist *zonelist, int order, gfp_t gfp_mask)
{
	struct page *reclaimed_pages[ZONE_EXTEND_ARRAY_LEN];
	unsigned long nr_pages = 0;
	unsigned int ret_num = 0;
	unsigned long pfn;
	unsigned long size;
	unsigned int len;
	unsigned int i;

	/* Only when ldk has more than one dma zone, it should skip DMA type */
	if (!dma_zone_only && (gfp_mask & __GFP_DMA))
		return 0;

	len = ((1 << order) < ZONE_EXTEND_ARRAY_LEN) ?
		 (ZONE_EXTEND_ARRAY_LEN/(1 << order)) : 1;
	ret_num = dmabuf_page_pool_reclaim(order, reclaimed_pages, len);
	for (i = 0; i < ret_num; i++) {
		pfn = page_to_pfn(reclaimed_pages[i]);
		order = compound_order(reclaimed_pages[i]);
		size = 1 << (order + PAGE_SHIFT);
		__prepare_pages_info(pfn_to_virt(pfn), size, gfp_zone(gfp_mask));
		__add_memblock_locked(zonelist, gfp_zone(gfp_mask), pfn, order);
		nr_pages += (1UL << order);
	}

	return nr_pages;
}

#ifdef CONFIG_LIBLINUX_SYS_UNION_SHRINKER
/*
 * currently, shrinker will shrink freeable >> (MIDDLE_PRIORITY) at a time, not all.
 */
#define MIDDLE_PRIORITY (6)
/*
 * Only do shrinking when the system mem is below the waterline
 */
#define SYSTEM_MEM_LOW_WATERLINE (32 * 1024) /* 128MB */
#define DEFAULT_TIMES_OF_SKIP (16)
/*
 * Current, only used in ldk-kalloc thread
 * if we used in multi-thread, please replace it with atomic_t
 */
static unsigned int g_shrinker_skip_times = 0;
static unsigned int g_shrinker_reclaimed_zero = 0;

static unsigned long
__extend_from_shrinkers(struct zonelist *zonelist, int order, gfp_t gfp_mask)
{
	if (liblinux_get_system_free_pages() > SYSTEM_MEM_LOW_WATERLINE) {
		return 0;
	}

	if (g_shrinker_skip_times > 0 ) {
		g_shrinker_skip_times--;
		return 0;
	}

	struct scan_control sc = {
		.gfp_mask = gfp_mask,
		.reclaim_idx = gfp_zone(gfp_mask),
		.priority = MIDDLE_PRIORITY,
		.nr_reclaimed = 0,
	};

	shrink_zones(zonelist, &sc);
	/*
	 * Avoid ineffective reclaiming action, Add skip mechanism:
	 *   Skip g_shrinker_skip_times if nr_reclaimed is zero, and
	 *   g_shrinker_skip_times will get bigger and bigger while
	 *   the multiple consecutive times that nr_reclaimed is zero occurs.
	 */
	if (sc.nr_reclaimed == 0) {
		g_shrinker_reclaimed_zero++;
		g_shrinker_skip_times += DEFAULT_TIMES_OF_SKIP * g_shrinker_reclaimed_zero;
	} else {
		g_shrinker_reclaimed_zero = 0;
	}

	return sc.nr_reclaimed;
}
#endif

static unsigned long
__extend_zone(struct zonelist *zonelist, int extend_order, gfp_t gfp_mask)
{
	unsigned long dmaheapcache = global_zone_page_state(NR_DMAHEAPCACHE_PAGES);
	unsigned long long extend_array[ZONE_EXTEND_ARRAY_LEN] = { 0 };
	unsigned int pa_num = 0;
	unsigned int fail_num = 0;
	unsigned long nr_pages = 0;
	unsigned int i;
	unsigned int pool_flag = __gfp_mask_to_page_pool_flag(gfp_mask);
	int ret;
	unsigned long ktime_b, ktime_e = 0;

	if (dmaheapcache > dmaheap_waterline) {
		nr_pages = __extend_from_dmabuf(zonelist, extend_order, gfp_mask);
		if (nr_pages > 0)
			return nr_pages;
	}

	/*
	 * when ldk treat all mem as dma zone:
	 * alloc mem from sysmgr normal_zone, so it also can alloc mem from sysmgr dma_zone
	 */
	if (dma_zone_only)
		pool_flag = __gfp_mask_to_page_pool_flag(gfp_mask & (~GFP_DMA));

	ret = liblinux_iofast_alloc_pfn_v(extend_order,
					 extend_array, ZONE_EXTEND_ARRAY_LEN,
					 pool_flag,
					 &pa_num);
	if (ret < 0) {
		return 0;
	}

	ktime_b = liblinux_get_time_us();
	for (i = 0; i < pa_num; i++) {
		unsigned long pfn = PHYS_PFN(extend_array[i]);
		unsigned int order = extend_array[i] & (~PAGE_MASK);
		unsigned long size = 1 << (order + PAGE_SHIFT);
		if (__populate_vmemmap(PFN_PHYS(pfn), size, gfp_zone(gfp_mask)) == NULL) {
			pr_err("__extend_zone populate vmemmap error 0x%lx 0x%lx\n", pfn, size);
			/*
			 * The fail_num is less than i, so overwrite extend_array to
			 * record the memory that failed to prepared vmemmap.
			 */
			extend_array[fail_num++] = extend_array[i];
			continue;
		}
		__add_memblock_locked(zonelist, gfp_zone(gfp_mask), pfn, order);
		nr_pages += (1UL << order);
	}
	ktime_e = liblinux_get_time_us();
	liblinux_iofast_mm_map_time_stat(ktime_e - ktime_b);
	liblinux_iofast_stat_inc(LIBLINUX_IOFAST_PAGE_BUDDY, nr_pages);
	atomic_long_add(nr_pages, &g_native_gfp_alloc);

	if (fail_num > 0) {
		ret = liblinux_iofast_free_pfn_v(extend_array, fail_num);
		if (ret != 0)
			pr_warn("__extend_zone: free pfn failed\n");
	}

	return nr_pages;
}

unsigned long try_to_free_pages(struct zonelist *zonelist, int order,
				gfp_t gfp_mask, nodemask_t *nodemask)
{
	return __extend_zone(zonelist, order, gfp_mask);
}

#ifdef CONFIG_NUMA
# error "liblinux: kswapd not support NUMA!"
#endif

void wakeup_kswapd(struct zone *zone, gfp_t gfp_flags, int order,
		   enum zone_type highest_zoneidx)
{
	pg_data_t *pgdat;
	enum zone_type curr_idx;

	if (!managed_zone(zone))
		return;

	if (!cpuset_zone_allowed(zone, gfp_flags))
		return;

	pgdat = zone->zone_pgdat;
	curr_idx = READ_ONCE(pgdat->kswapd_highest_zoneidx);

	if (curr_idx == MAX_NR_ZONES || curr_idx < highest_zoneidx)
		WRITE_ONCE(pgdat->kswapd_highest_zoneidx, highest_zoneidx);

	if (READ_ONCE(pgdat->kswapd_order) < order)
		WRITE_ONCE(pgdat->kswapd_order, order);

	if (!waitqueue_active(&pgdat->kswapd_wait))
		return;

	wake_up_interruptible(&pgdat->kswapd_wait);
}

static enum zone_type kswapd_highest_zoneidx(pg_data_t *pgdat,
		enum zone_type prev_highest_zoneidx)
{
	enum zone_type curr_idx = READ_ONCE(pgdat->kswapd_highest_zoneidx);

	return curr_idx == MAX_NR_ZONES ? prev_highest_zoneidx : curr_idx;
}

static bool pgdat_balanced(pg_data_t *pgdat, int order, int highest_zoneidx)
{
	int i;
	unsigned long mark = -1;
	struct zone *zone;

	/*
	 * Check watermarks bottom-up as lower zones are more likely to
	 * meet watermarks.
	 */
	for (i = 0; i <= highest_zoneidx; i++) {
		zone = pgdat->node_zones + i;

		if (!managed_zone(zone))
			continue;

		mark = high_wmark_pages(zone);
		if (zone_watermark_ok_safe(zone, order, mark, highest_zoneidx))
			return true;
	}

	/*
	 * If a node has no populated zone within highest_zoneidx, it does not
	 * need balancing by definition. This can happen if a zone-restricted
	 * allocation tries to wake a remote kswapd.
	 */
	if (mark == -1)
		return true;

	return false;
}

#ifdef CONFIG_LDK_KALLOCD_PRIO_OPTIMIZE
enum prio_status {
	PRIO_NORMAL = 0,
	PRIO_HIGH = 1,
};

static void change_thread_prio(pg_data_t *pgdat, unsigned long free)
{
	static unsigned int prio_status = PRIO_NORMAL;
	struct zone *zone = get_zone_by_type(ZONE_DMA);
	unsigned long wmark = min_wmark_pages(zone);

	if (free < wmark) {
		if (prio_status == PRIO_NORMAL) {
			(void)liblinux_pal_thread_set_vip_prio(
				&task_thread_info(pgdat->kswapd)->thread_handle,
				PRIO_HIGH);
			prio_status = PRIO_HIGH;
			pr_info("%s: kallocd prio change to HIGH\n", __func__);
		}
	} else if (free > wmark) {
		if (prio_status == PRIO_HIGH) {
			(void)liblinux_pal_thread_set_vip_prio(
				&task_thread_info(pgdat->kswapd)->thread_handle,
				PRIO_NORMAL);
			prio_status = PRIO_NORMAL;
			pr_info("%s: kallocd prio change to NORMAL\n", __func__);
		}
	}
}

static void kswapd_above_wmark_sleep(pg_data_t *pgdat)
{
	DEFINE_WAIT(wait);

	if (kthread_should_stop())
		return;

	prepare_to_wait(&pgdat->kswapd_wait, &wait, TASK_INTERRUPTIBLE);
	(void)schedule_timeout(HZ/2);
	finish_wait(&pgdat->kswapd_wait, &wait);
}

#define LOW_WMARK_OFFSET 2560   /* 10M */
static void kswapd_prio_optimze(pg_data_t *pgdat)
{
	if (!dma_zone_only) {
		return;
	}

	bool noslow = false;
	struct zone *zone = get_zone_by_type(ZONE_DMA);
	unsigned long nr_free = zone_page_state(zone, NR_FREE_PAGES);
	unsigned long wmark = low_wmark_pages(zone);
	if (nr_free > wmark + LOW_WMARK_OFFSET) {
		noslow = true;
	}

	change_thread_prio(pgdat, nr_free);
	if (pgdat->kswapd_failures < MAX_RECLAIM_RETRIES && noslow) {
		kswapd_above_wmark_sleep(pgdat);
	}
}
#endif

static int balance_pgdat(pg_data_t *pgdat, int order, int highest_zoneidx)
{
	struct zonelist *zonelist = NULL;
	unsigned long nr_reclaimed = 0;
	gfp_t gfp_mask = GFP_KERNEL;
	gfp_t gfp_mask_extend = 0;

	if (highest_zoneidx < ZONE_NORMAL) {
		gfp_mask |= __GFP_DMA;
	}
	zonelist = node_zonelist(pgdat->node_id, gfp_mask);

	gfp_mask_extend |= gfp_mask;
	gfp_mask_extend |= __GFP_KSWAPD_SELF;
	nr_reclaimed = __extend_zone(zonelist, order, gfp_mask_extend);
	if (!nr_reclaimed) {
		pgdat->kswapd_failures++;
#ifdef CONFIG_LDK_KALLOCD_PRIO_OPTIMIZE
		kswapd_prio_optimze(pgdat);
#endif
	} else {
		pgdat->kswapd_failures = 0;
	}

#ifdef CONFIG_LIBLINUX_SYS_UNION_SHRINKER
	nr_reclaimed = __extend_from_shrinkers(zonelist, order, gfp_mask);
	if (!nr_reclaimed) {
		pgdat->kswapd_failures++;
	} else {
		pgdat->kswapd_failures = 0;
	}
#endif

	return order;
}

static bool prepare_kswapd_sleep(pg_data_t *pgdat, int order,
				 int highest_zoneidx)
{
	if (waitqueue_active(&pgdat->pfmemalloc_wait))
		wake_up_all(&pgdat->pfmemalloc_wait);

	/* Hopeless node, leave it to direct reclaim */
	if (pgdat->kswapd_failures >= MAX_RECLAIM_RETRIES)
		return true;

	if (pgdat_balanced(pgdat, order, highest_zoneidx)) {
		return true;
	}

	return false;
}

static void kswapd_try_to_sleep(pg_data_t *pgdat, int alloc_order, int reclaim_order,
		unsigned int highest_zoneidx)
{
	long remaining = 0;
	DEFINE_WAIT(wait);

	if (kthread_should_stop())
		return;

	prepare_to_wait(&pgdat->kswapd_wait, &wait, TASK_INTERRUPTIBLE);

	if (prepare_kswapd_sleep(pgdat, reclaim_order, highest_zoneidx)) {
		remaining = schedule_timeout(HZ/10);
		if (remaining) {
			WRITE_ONCE(pgdat->kswapd_highest_zoneidx,
					kswapd_highest_zoneidx(pgdat,
						highest_zoneidx));
			if (READ_ONCE(pgdat->kswapd_order) < reclaim_order)
				WRITE_ONCE(pgdat->kswapd_order, reclaim_order);
		}
		finish_wait(&pgdat->kswapd_wait, &wait);
		prepare_to_wait(&pgdat->kswapd_wait, &wait, TASK_INTERRUPTIBLE);
	}

	if (!remaining &&
	    prepare_kswapd_sleep(pgdat, reclaim_order, highest_zoneidx)) {
		if (!kthread_should_stop())
			schedule();
	}
	finish_wait(&pgdat->kswapd_wait, &wait);
}

static int kswapd(void *p)
{
	unsigned int alloc_order, reclaim_order;
	unsigned int highest_zoneidx = MAX_NR_ZONES - 1;
	pg_data_t *pgdat = (pg_data_t*)p;
	struct task_struct *tsk = current;

	tsk->flags |= PF_MEMALLOC | PF_SWAPWRITE | PF_KSWAPD;

	WRITE_ONCE(pgdat->kswapd_order, 0);
	WRITE_ONCE(pgdat->kswapd_highest_zoneidx, MAX_NR_ZONES);
	for ( ; ; ) {
		alloc_order = reclaim_order = READ_ONCE(pgdat->kswapd_order);
		highest_zoneidx = kswapd_highest_zoneidx(pgdat,
							 highest_zoneidx);
kswapd_try_sleep:
		kswapd_try_to_sleep(pgdat, alloc_order, reclaim_order,
					highest_zoneidx);
		alloc_order = reclaim_order = READ_ONCE(pgdat->kswapd_order);
		highest_zoneidx = kswapd_highest_zoneidx(pgdat,
							 highest_zoneidx);
		WRITE_ONCE(pgdat->kswapd_order, 0);
		WRITE_ONCE(pgdat->kswapd_highest_zoneidx, MAX_NR_ZONES);

		if (kthread_should_stop())
			break;

#ifdef CONFIG_MEMORY_MONITOR
		kswapd_monitor_wake_up_queue();
#endif

		reclaim_order = balance_pgdat(pgdat, alloc_order, highest_zoneidx);

		if (reclaim_order < alloc_order)
			goto kswapd_try_sleep;
	}

	tsk->flags &= ~(PF_MEMALLOC | PF_SWAPWRITE | PF_KSWAPD);

	return 0;
}

int kswapd_run(int nid)
{
	pg_data_t *pgdat = NODE_DATA(nid);
	int ret = 0;

	if (pgdat->kswapd)
		return 0;

	pgdat->kswapd = kthread_run(kswapd, pgdat, "ldk-kallocd%d", nid);
	if (IS_ERR(pgdat->kswapd)) {
		/* failure at boot is fatal */
		BUG_ON(system_state < SYSTEM_RUNNING);
		pr_err("Failed to start kswapd on node %d\n", nid);
		ret = PTR_ERR(pgdat->kswapd);
		pgdat->kswapd = NULL;
		return ret;
	}
#ifndef CONFIG_LDK_KALLOCD_PRIO_OPTIMIZE
#define SCHED_VIP_PRIORITY_KSWAPD 1  /* displayed as 41 */
	/* set the kswapd as vip */
	ret = liblinux_pal_thread_set_vip_prio(
			&task_thread_info(pgdat->kswapd)->thread_handle,
			SCHED_VIP_PRIORITY_KSWAPD);
#endif
	return ret;
}

static int __init kswapd_init(void)
{
	int nid;
	for_each_node_state(nid, N_MEMORY)
		kswapd_run(nid);
	return 0;
}
early_initcall(kswapd_init)

/*
 * shrinker of buddy allocator
 */
static unsigned long __buddy_shrinker_query(void *priv)
{
	struct zone *zone = NULL;
	unsigned long nr_free = 0;

	for_each_populated_zone(zone) {
		unsigned long wmark = high_wmark_pages(zone);
		unsigned long free = zone_page_state(zone, NR_FREE_PAGES);
		/* can reclaim to high wmark at most */
		if (free > wmark)
			nr_free += (free - wmark);
	}
	return nr_free;
}

#define BUDDY_SHRINKER_STEP	256
static int get_shrink_count_by_order(int order)
{
	return ((1 << order) < BUDDY_SHRINKER_STEP) ? (BUDDY_SHRINKER_STEP/(1 << order)) : 1;
}

static bool zone_check_free_enough(struct zone *zone, int order)
{
	unsigned long free = zone_page_state(zone, NR_FREE_PAGES);
	unsigned long wmark = high_wmark_pages(zone);
	/* free blocks of each order in buddyinfo */
	unsigned long blocks = zone->free_area[order].nr_free;

	if (free > (wmark + (1 << order)) && blocks > 0) {
		return true;
	} else {
		return false;
	}
}

static unsigned long zone_do_shrink_order(int order, gfp_t gfp_mask)
{
	void *vaddr = NULL;
	enum zone_type zone;
	struct page *pg = NULL;
	unsigned long long pa, paddr;
	unsigned long nr_pages = 1 << order;

	pg = alloc_pages(gfp_mask, order);
	if (pg == NULL)
		return 0;
	paddr = page_to_phys(pg);
	vaddr = page_address(pg);
	zone = page_zonenum(pg);
	pa = paddr | order;

	/* adjust managed/present pages */
	adjust_present_page_count(pg, -nr_pages);
	adjust_managed_page_count(pg, -nr_pages);
	liblinux_mm_prepare_unmap(vaddr, nr_pages * PAGE_SIZE);
	if (liblinux_iofast_free_pfn_v(&pa, 1) != 0) {
		pr_err("zone%u do shrink order%d failed\n",(unsigned int)zone, order);
		(void)__populate_vmemmap(paddr, nr_pages * PAGE_SIZE, zone);
		adjust_present_page_count(pg, nr_pages);
		adjust_managed_page_count(pg, nr_pages);
		__free_pages_core(pg, order);
		return 0;
	}
	liblinux_iofast_stat_dec(LIBLINUX_IOFAST_PAGE_BUDDY, nr_pages);
	atomic_long_add(nr_pages, &g_native_gfp_free);

	return nr_pages;
}

static unsigned long zone_do_shrink(int order, unsigned long req_nr, bool is_dma_zone)
{
	gfp_t gfp_mask = __GFP_NORETRY | __GFP_NOWARN | __GFP_MEMALLOC;
	int count = get_shrink_count_by_order(order);
	unsigned long nr_pages;
	unsigned long reclaimed = 0;
	struct zone *zone = NULL;
	int i = 0;

	if (is_dma_zone) {
		zone = get_zone_by_type(ZONE_DMA);
		gfp_mask |= __GFP_DMA;
	} else {
		zone = get_zone_by_type(ZONE_NORMAL);
	}

	for (i = 0; i < count && reclaimed < req_nr; i++) {
		if (!zone_check_free_enough(zone, order)) {
			break;
		}

		nr_pages = zone_do_shrink_order(order, gfp_mask);
		if (nr_pages == 0) {
			break;
		}

		reclaimed += nr_pages;
	}

	return reclaimed;
}

static unsigned long __buddy_shrinker_shrink(void *priv, int req_nr)
{
	unsigned long reclaimed = 0;
	unsigned long actual_req = 0;
	unsigned long max_req = 0;
	int order, max_order;

	/* reclaim to high_wmark at most */
	max_req = __buddy_shrinker_query(NULL);
	if (req_nr > 0 && max_req > req_nr) {
		actual_req = req_nr;
	} else {
		actual_req = max_req;
	}

	/* find maximum order for reclaim */
	max_order = get_order(actual_req << PAGE_SHIFT);
	if (max_order > MAX_ORDER)
		max_order = MAX_ORDER - 1;

	if ((1 << max_order) > actual_req)
		max_order--;

	/*
	 * reclaim from each order with each step
	 *   - this will balance the high-order and low-order allocation
	 *   - but may increase the overhead of reclaim
	 */
	while (reclaimed < actual_req && __buddy_shrinker_query(NULL) > 0) {
		unsigned long nr_pages = 0;
		/* do reclaim */
		for (order = max_order;
				(reclaimed + nr_pages < actual_req) && (order >= 0); order--) {
			if (!dma_zone_only) {
				nr_pages += zone_do_shrink(order, (actual_req - reclaimed), false);
				if (reclaimed + nr_pages >= actual_req)
					break;
			}

			nr_pages += zone_do_shrink(order, (actual_req - reclaimed - nr_pages), true);
		}
		if (nr_pages == 0) {
			/* break if no memory be freed */
			break;
		}
		reclaimed += nr_pages;
	}

	return reclaimed;
}

static int __init buddy_shrinker_init(void)
{
	struct liblinux_pal_sub_shrinker shrinker;

	shrinker.query = __buddy_shrinker_query;
	shrinker.shrink = __buddy_shrinker_shrink;
	shrinker.priv = NULL;

	liblinux_pal_sub_shrinker_register(NULL, shrinker);

	return 0;
}
early_initcall(buddy_shrinker_init)

/*
 * only used by `__alloc_pages_direct_reclaim` at ldk:
 * mem info of devhost is not sufficient, do extend mem from sysmgr first.
 * drain_all_pages is heavyweight, and it affect the performance of subsequent alloc_pages.
 * so do nothing here.
 */
__override void drain_all_pages(struct zone *zone)
{
}

#ifdef CONFIG_LIBLINUX_HIBERNATE
static void zone_clean_all_free_pages(struct zone *zone)
{
	unsigned int order = 0;
	unsigned int t = 0;
	struct page *page;
	void *addr;

	spin_lock(&zone->lock);
	for_each_migratetype_order(order, t) {
		list_for_each_entry(page, &zone->free_area[order].free_list[t], lru) {
			addr = page_address(page);
			(void)memset_s(addr, PAGE_SIZE << order, 0, PAGE_SIZE << order);
		}
	}
	spin_unlock(&zone->lock);
}

void pm_clean_all_free_pages(void)
{
	struct zone *zone = NULL;

	for_each_populated_zone(zone) {
		zone_clean_all_free_pages(zone);
	}
}
#endif
