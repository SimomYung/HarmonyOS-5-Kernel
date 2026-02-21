/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: implementation of tsan's needed functions and data types for uapps
 * Author: Huawei OS Kernel Lab
 * Create: Fri Nov 25 15:47:39 2022
 */
#ifndef TSAN_ALGO_H
#define TSAN_ALGO_H

#include <libstrict/strict.h>
#include <hongmeng/syscall.h>
#include <libhmsync/raw_mutex.h>
#include <libhmsync/raw_thread.h>

/* Limit for a id consecutively installing the wp */

#define uext_log(...)	hm_info(__VA_ARGS__)
#define uext_error(...)	hm_error(__VA_ARGS__)

typedef struct raw_mutex uext_lock_t;
typedef volatile uint64_t uext_atomic64_t;

/*
 *   The external functions used by 'uext_thread_get_id' must not be thread-sanitized,
 * otherwise infinite recursion happens
 */

#define TLS_UNREADY (-1)
static inline int
uext_thread_get_id(void)
{
	return thread_get_current_tid();
}

static inline void
uext_delay(unsigned int t)
{
	for (unsigned int i = 0; i < t; i++) {
		(void)sysfast_sched_yield();
	}
}

static inline void
uext_lock_init(uext_lock_t *lock)
{
	raw_mutex_init(lock);
}

static inline void
uext_lock(uext_lock_t *lock)
{
	raw_mutex_lock(lock);
}

static inline bool
uext_try_lock(uext_lock_t *lock)
{
	return (raw_mutex_trylock(lock) == E_HM_OK);
}

static inline void
uext_unlock(uext_lock_t *lock)
{
	raw_mutex_unlock(lock);
}

static inline uint64_t
uext_atomic64_read(uext_atomic64_t *v)
{
	return __atomic_load_n(v, __ATOMIC_SEQ_CST);
}

static inline void
uext_atomic64_write(uext_atomic64_t *v, uint64_t a)
{
	__atomic_store_n(v, a, __ATOMIC_SEQ_CST);
}

static inline void
uext_atomic64_add(uext_atomic64_t *v, uint64_t a)
{
	(void)__atomic_add_fetch(v, a, __ATOMIC_SEQ_CST);
}

static inline bool
uext_atomic64_cond_xchg(uext_atomic64_t *v, uint64_t old, uint64_t new_val)
{
	uint64_t old_val = old;
	__atomic_compare_exchange_n(v, &old_val, new_val, 0,
				    __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
	return old_val == old;
}

#endif
