/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Basic implementation related with lockdep
 * Author: Huawei OS Kernel Lab
 * Create: Tue Oct 10 23:06:06 2023
 */
#ifndef ULIBS_LIBHMSYNC_LOCKDEP_IMPL_H
#define ULIBS_LIBHMSYNC_LOCKDEP_IMPL_H

#include <libhmsync/lockdep.h>
#include <hmasm/types.h>
#include <sys/types.h>

#define MAX_LOCK_DEPTH 4
#define MAX_LOCKDEP_KEYS_BITS 13
#define MAX_LOCKDEP_KEYS ((1UL << MAX_LOCKDEP_KEYS_BITS) - 1)

#ifdef CONFIG_DEBUG_BUILD
#define lock_dep_map_init(lock) \
do { \
	mem_zero_s(lock->dep_map); \
} while(0)

#else
#define lock_dep_map_init(lock) do {} while (0)
#endif

#ifdef CONFIG_LOCKDEP
struct held_lock {
	unsigned long class_idx;
	struct lockdep_map_s *instance;
#ifdef CONFIG_LOCKSTAT
	unsigned long waittime_stamp;
	unsigned long holdtime_stamp;
#endif
};

#ifdef CONFIG_LOCKSTAT
struct lock_time {
	__u64 min;
	__u64 max;
	__u64 total;
	unsigned long nr;
};

struct lock_class_stats {
	struct lock_time waittime;
	struct lock_time holdtime;
};

struct lockstat_ctx {
	char *buf;
	size_t size;
	size_t used;
	int err;
};

#define lockstat_ctx_init(ctx, data, len) \
	struct lockstat_ctx ctx_s = { \
		.buf = (data), \
		.size = (len), \
		.used = 0, \
		.err = 0, \
	}; \
	struct lockstat_ctx *(ctx) = &ctx_s

#define lockstat_write_data(ctx, format, ...) do {				\
	if ((ctx)->err == 0) {							\
		if ((ctx)->size <= (ctx)->used) { (ctx)->err = E_HM_POSIX_FAULT; }	\
		if ((ctx)->err == 0) {						\
			int werr = snprintf_s((ctx)->buf + (ctx)->used,		\
					      (ctx)->size - (ctx)->used,	\
					      ((ctx)->size - (ctx)->used) - 1U,	\
					      (format), ##__VA_ARGS__);		\
			if (werr < 0) { (ctx)->err = E_HM_POSIX_FAULT; }		\
			else { (ctx)->used += (size_t)(unsigned int)werr; }	\
		}								\
	}									\
} while (__false)

#endif /* CONFIG_LOCKSTAT */

void lockdep_init_map(struct lockdep_map_s *lock, const char *name,
		      struct lockdep_lock_class_key_s *key);
void lock_acquire(struct lockdep_map_s *lock);
void lock_release(struct lockdep_map_s *lock);

#ifdef CONFIG_LOCKSTAT
void lock_contended(struct lockdep_map_s *lock);
void lock_acquired(struct lockdep_map_s *lock);
#endif /* CONFIG_LOCKSTAT */

#endif /* CONFIG_LOCKDEP */
#endif
