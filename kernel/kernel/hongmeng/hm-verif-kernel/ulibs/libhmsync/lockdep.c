/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Implementation of lockdep
 * Author: Huawei OS Kernel Lab
 * Create: Sat Jul 22 11:43:54 2023
 */
#include <libhmsync/raw_thread.h>
#include <libhmsrv_sys/hm_timer.h>

#include "raw_thread_impl.h"
#include "lockdep_impl.h"

#ifdef CONFIG_LOCKDEP

unsigned long g_nr_lock_classes;
static struct lockdep_lock_class_s g_lock_classes[MAX_LOCKDEP_KEYS];
static struct raw_mutex g_lock_classes_lock = RAW_MUTEX_INITIALIZER;

#ifdef CONFIG_LOCKSTAT
static struct lock_class_stats g_lock_stats[MAX_LOCKDEP_KEYS];
int g_lock_stat = 0;

static __u64 lockstat_clock(void)
{
	struct timeval tv;
	__u64 curr_time = 0;
	int err;

	err = hm_gettimeofday(&tv, NULL);
	if (err < 0) {
		hm_error("lockstat: get curr_time failed.\n");
	} else {
		curr_time = tv.tv_sec * USEC_PER_SEC + tv.tv_usec;
	}

	return curr_time;
}

static void lock_time_inc(struct lock_time *lt, __u64 time)
{
	/* !lt->nr mean first time update */
	if (time < lt->min || !lt->nr) {
		lt->min = time;
	}

	if (time > lt->max) {
		lt->max = time;
	}

	lt->total += time;
	lt->nr++;
}

static struct lock_class_stats *get_lock_stats(struct lockdep_lock_class_s *class)
{
	struct lock_class_stats *ret = NULL;

	if (class != NULL) {
		ret = &g_lock_stats[class - g_lock_classes];
	}

	return ret;
}

static inline struct lockdep_lock_class_s *hlock_class(struct held_lock *hlock)
{
	struct lockdep_lock_class_s *ret = NULL;

	if (hlock->class_idx > 0) {
		ret = g_lock_classes + hlock->class_idx - 1;
	} else {
		hm_error("lockstat: wrong class_idx.\n");
	}

	return ret;
}

static void lock_release_holdtime(struct held_lock *hlock)
{
	struct lock_class_stats *stats = NULL;
	__u64 holdtime;

	if (!g_lock_stat) {
		return;
	}

	holdtime = lockstat_clock() - hlock->holdtime_stamp;
	stats = get_lock_stats(hlock_class(hlock));
	lock_time_inc(&stats->holdtime, holdtime);
}
#else
static int g_lock_stat = 0;

static inline void lock_release_holdtime(struct held_lock *hlock)
{
	UNUSED(hlock);
}
#endif

static struct lockdep_lock_class_s *look_up_lock_class(struct lockdep_map_s *lock)
{
	unsigned int i;
	struct lockdep_lock_class_s *ret = NULL;

	for (i = 0; i < g_nr_lock_classes; i++) {
		if (g_lock_classes[i].key == lock->key) {
			ret = &g_lock_classes[i];
			break;
		}
	}

	return ret;
}

static struct lockdep_lock_class_s *register_lock_class(struct lockdep_map_s *lock,
							const char *name)
{
	struct lockdep_lock_class_s *class = NULL;
	int ret = E_HM_OK;

	class = look_up_lock_class(lock);
	if (class != NULL) {
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		if (g_nr_lock_classes >= MAX_LOCKDEP_KEYS) {
			hm_error("lockstat: nr_lock_classes over size.\n");
			ret = E_HM_INVAL;
		}
	}

	if (ret == E_HM_OK) {
		class = g_lock_classes + g_nr_lock_classes;
		g_nr_lock_classes++;
		class->key = lock->key;
		class->name = name;
	}

	return class;
}

static void _lockdep_init_map(struct lockdep_map_s *lock, const char *name,
			      struct lockdep_lock_class_key_s *key)
{
	if (name == NULL || key == NULL) {
		hm_error("lockstat: lock-class name or key is NULL.\n");
	} else {
		raw_mutex_lock(&g_lock_classes_lock);
		lock->key = key;
		(void)register_lock_class(lock, name);
		raw_mutex_unlock(&g_lock_classes_lock);
	}
}

void lockdep_init_map(struct lockdep_map_s *lock, const char *name,
		      struct lockdep_lock_class_key_s *key)
{
	_lockdep_init_map(lock, name, key);
}

static void _lock_acquire(struct lockdep_map_s *lock)
{
	struct raw_thread *curr = raw_thread_self();
	struct lockdep_lock_class_s *class = NULL;
	struct held_lock *hlock = NULL;
	unsigned int depth;
	int class_idx;
	int ret = E_HM_OK;

	class = look_up_lock_class(lock);
	if (class == NULL) {
		hm_error("lockstat: lookup lock class failed.\n");
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		depth = curr->lockdep_depth;
		if (depth >= MAX_LOCK_DEPTH) {
			hm_error("lockstat: lockdep_depth is over max lock depth.\n");
			ret = E_HM_INVAL;
		}
	}

	if (ret == E_HM_OK) {
		class_idx = class - g_lock_classes + 1;
		hlock = curr->held_locks + depth;
		hlock->class_idx = class_idx;
		hlock->instance = lock;
#ifdef CONFIG_LOCKSTAT
		hlock->waittime_stamp = 0;
		hlock->holdtime_stamp = lockstat_clock();
#endif
		curr->lockdep_depth++;
	}

	return;
}

void lock_acquire(struct lockdep_map_s *lock)
{
	_lock_acquire(lock);
}

static bool match_held_lock(struct held_lock *hlock, struct lockdep_map_s *lock)
{
	bool ret = false;

	if (hlock->instance == lock) {
		ret = true;
	}

	return ret;
}

static struct held_lock *find_held_lock(struct raw_thread *curr,
					struct lockdep_map_s *lock,
					unsigned int depth, int *idx)
{
	struct held_lock *ret = NULL;
	struct held_lock *hlock = NULL;
	int i;

	i = depth - 1;
	hlock = curr->held_locks + i;
	for ( ; i >= 0; i--, hlock--) {
		if (match_held_lock(hlock, lock)) {
			ret = hlock;
			break;
		}
	}

	*idx = i;
	return ret;
}

static void _lock_release(struct lockdep_map_s *lock)
{
	struct raw_thread *curr = raw_thread_self();
	struct held_lock *hlock = NULL;
	unsigned int depth;
	int i;
	int ret = E_HM_OK;

	depth = curr->lockdep_depth;
	if (depth == 0) {
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		hlock = find_held_lock(curr, lock, depth, &i);
		if (hlock != NULL) {
			lock_release_holdtime(hlock);
			curr->lockdep_depth = i;
		}
	}

	return;
}

void lock_release(struct lockdep_map_s *lock)
{
	_lock_release(lock);
}

#ifdef CONFIG_LOCKSTAT
static void _lock_contended(struct lockdep_map_s *lock)
{
	struct raw_thread *curr = raw_thread_self();
	struct held_lock *hlock = NULL;
	unsigned int depth;
	int i;
	int ret = E_HM_OK;

	depth = curr->lockdep_depth;
	if (depth == 0) {
		hm_error("lockstat: no hlock to contended.\n");
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		hlock = find_held_lock(curr, lock, depth, &i);
		if (hlock == NULL) {
			hm_error("lockstat: find hlock failed.\n");
			ret = E_HM_INVAL;
		}
	}

	if (ret == E_HM_OK) {
		hlock->waittime_stamp = lockstat_clock();
	}

	return;
}

void lock_contended(struct lockdep_map_s *lock)
{
	if (!g_lock_stat) {
		return;
	}

	_lock_contended(lock);
}

static void _lock_acquired(struct lockdep_map_s *lock)
{
	struct raw_thread *curr = raw_thread_self();
	struct held_lock *hlock = NULL;
	struct lock_class_stats *stats;
	unsigned int depth;
	__u64 now, waittime = 0;
	int i;
	int ret = E_HM_OK;

	depth = curr->lockdep_depth;
	if (depth == 0) {
		hm_error("lockstat: no hlock to acquired.\n");
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		hlock = find_held_lock(curr, lock, depth, &i);
		if (hlock == NULL) {
			hm_error("lockstat: find hlock failed.\n");
			ret = E_HM_INVAL;
		}
	}

	if (ret == E_HM_OK) {
		if (hlock->waittime_stamp) {
			now = lockstat_clock();
			waittime = now - hlock->waittime_stamp;
			hlock->holdtime_stamp = now;
		}

		if (waittime) {
			stats = get_lock_stats(hlock_class(hlock));
			lock_time_inc(&stats->waittime, waittime);
		}
	}

	return;
}

void lock_acquired(struct lockdep_map_s *lock)
{
	if (!g_lock_stat) {
		return;
	}

	_lock_acquired(lock);
}
#endif /* CONFIG_LOCKSTAT */
#endif /* CONFIG_LOCKDEP */

#ifdef CONFIG_LOCKSTAT
int lockdep_fill_lockstat(char *data, size_t data_len)
{
	int err;
	unsigned long i = 0;

	lockstat_ctx_init(ctx, data, data_len);

	lockstat_write_data(ctx, "%40s %20s %15s %15s %15s %15s %15s %15s %15s %15s\n",
			    "class-name",
			    "class-key",
			    "waittime-min",
			    "waittime-max",
			    "waittime-total",
			    "waittime-nr",
			    "holdtime-min",
			    "holdtime-max",
			    "holdtime-total",
			    "holdtime-nr");

	for (; i < g_nr_lock_classes; i++) {
		lockstat_write_data(ctx, "%40s %20p %15lu %15lu %15lu %15lu %15lu %15lu %15lu %15lu\n",
				    g_lock_classes[i].name, g_lock_classes[i].key, g_lock_stats[i].waittime.min,
				    g_lock_stats[i].waittime.max, g_lock_stats[i].waittime.total,
				    g_lock_stats[i].waittime.nr,
				    g_lock_stats[i].holdtime.min, g_lock_stats[i].holdtime.max,
				    g_lock_stats[i].holdtime.total,
				    g_lock_stats[i].holdtime.nr);
	}
	err = ctx->err < 0 ? ctx->err : (int)(unsigned int)ctx->used;

	return err;
}

void lockdep_clear_lockstat(void)
{
	NOFAIL(memset_s(g_lock_stats, sizeof(g_lock_stats), 0, sizeof(g_lock_stats)));
}

int lockdep_fill_lockstat_status(char *data, size_t data_len)
{
	int err;

	lockstat_ctx_init(ctx, data, data_len);
	lockstat_write_data(ctx, "%d\n", g_lock_stat);
	err = ctx->err < 0 ? ctx->err : (int)(unsigned int)ctx->used;

	return err;
}

void lockdep_set_lockstat_status(unsigned long status)
{
	g_lock_stat = (int)status;
	return;
}
#else
int lockdep_fill_lockstat(char *data, size_t data_len)
{
	UNUSED(data, data_len);
	return E_HM_INVAL;
}

void lockdep_clear_lockstat(void)
{
	return;
}

int lockdep_fill_lockstat_status(char *data, size_t data_len)
{
	UNUSED(data, data_len);
	return E_HM_INVAL;
}

void lockdep_set_lockstat_status(unsigned long status)
{
	UNUSED(status);
	return;
}
#endif /* CONFIG_LOCKSTAT */
