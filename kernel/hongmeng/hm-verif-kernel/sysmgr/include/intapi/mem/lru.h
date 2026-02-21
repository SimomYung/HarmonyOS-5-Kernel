/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: LRU
 * Author: Huawei OS Kernel Lab
 * Create: Sat Mar 27 18:15:54 2021
 */
#ifndef SYSMGR_MEM_LRU_H
#define SYSMGR_MEM_LRU_H

#ifdef CONFIG_USE_PREEMPT_PROCESS
#include <libhmsync/uspinlock.h>
#else
#include <libhmsync/raw_mutex.h>
#endif
#include <libhmsync/atomic.h>
#include <libhmsync/raw_bitlock.h>
#include <lib/dlist.h>
#include <libstrict/strict.h>

enum lru_type_e {
	__LRU_ACTIVE_LIST,
	__LRU_INACTIVE_LIST,
	__LRU_INEVICTABLE_LIST,
	__NR_LRU_LISTS
};

enum lru_page_type_e {
	__LRU_FILE,
	__LRU_ANON,
#ifdef CONFIG_MEM_PURGEABLE
	__LRU_PURGEABLE,
#endif
	__NR_LRU_PAGE_TYPES
};

struct lru_cache_s {
	struct dlist_node lru_list[__NR_LRU_LISTS];
	raw_atomic_ullong_t nr[__NR_LRU_LISTS];
#ifdef CONFIG_USE_PREEMPT_PROCESS
	struct uspinlock_s lock;
#else
	struct raw_mutex mutex;
#endif
	enum lru_page_type_e page_type;
};

struct lru_s {
	struct lru_cache_s file_lru_cache;
	struct lru_cache_s anon_lru_cache;
#ifdef CONFIG_MEM_PURGEABLE
	struct lru_cache_s purgeable_lru_cache;
#endif
};

enum scan_reason {
	RECLAIM_BY_DEFAULT,
	RECLAIM_BY_KSWAPD,
	RECLAIM_BY_ZSWAPD,
};

struct scan_control {
	unsigned char reason;
	unsigned char priority;
	unsigned char rsv1;
	unsigned char rsv2;
	unsigned int nid;

	union {
		struct {
			unsigned int reclaim_lock:1; /* must place in bit0 */
			unsigned int reclaim_low:1;
			unsigned int reclaim_fs:1;
			unsigned int reclaim_lru_anon:1;
			unsigned int reclaim_lru_file:1;
			unsigned int reclaim_lru_purgeable:1;
		};
		unsigned int flags;
	};

	/*
	 * one reclaim event may contain many reclaim actions
	 */
	unsigned long nr_to_reclaim; /* nr_pages to scan during one reclaim action */
	unsigned long nr_target; /* total target nr_pages, it is const value during one reclaim event */
	unsigned long nr_scanned; /* total scaned nr_pages, it is incremented during one reclaim event */
	unsigned long nr_reclaimed; /* total reclaimed nr_pages, it is incremented during one reclaim event */

	bool (*stop_scan)(struct scan_control *sc);
};

static inline void scan_control_lock(struct scan_control *sc)
{
	raw_bitlock_spin_lock((int *)&sc->flags, 0);
}

static inline void scan_control_unlock(struct scan_control *sc)
{
	raw_bitlock_spin_unlock((int *)&sc->flags, 0);
}

#ifdef CONFIG_MEM_OPT_SWITCH
static inline bool lru_shrink_should_stop(struct scan_control *sc)
{
	bool stop = false;

	if (sc->nr_reclaimed >= sc->nr_target) {
		stop = true;
	}

	if ((sc->stop_scan != NULL) && sc->stop_scan(sc)) {
		stop = true;
	}

	return stop;
}
#else
static inline bool lru_shrink_should_stop(struct scan_control *sc)
{
	return false;
}
#endif

void lru_cache_init(struct lru_s *lru_cache);
#ifdef CONFIG_LRU_ANON
size_t lru_anon_shrink(struct lru_s *lru, struct scan_control *sc);
size_t lru_active_anon_shrink(struct lru_s *lru, struct scan_control *sc);
size_t lru_inactive_anon_shrink(struct lru_s *lru, struct scan_control *sc);
#ifdef CONFIG_MEM_PURGEABLE
size_t lru_shrink_purgeable(struct lru_s *lru, struct scan_control *sc);
#else
static inline size_t lru_shrink_purgeable(struct lru_s *lru, struct scan_control *sc)
{
	UNUSED(lru, sc);
	return 0;
}
#endif
#else
static inline size_t lru_anon_shrink(struct lru_s *lru, struct scan_control *sc)
{
	UNUSED(lru, sc);
	return 0;
}
static inline size_t lru_active_anon_shrink(struct lru_s *lru, struct scan_control *sc)
{
	UNUSED(lru, sc);
	return 0;
}
static inline size_t lru_inactive_anon_shrink(struct lru_s *lru, struct scan_control *sc)
{
	UNUSED(lru, sc);
	return 0;
}
#endif

#ifdef CONFIG_MEM_PURGEABLE
size_t lru_drop_purgeable_cache(void);
#endif

#endif
