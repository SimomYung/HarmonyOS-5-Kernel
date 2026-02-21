/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2021. All rights reserved.
 * Description: Atomic operations on c11 standard
 * Author: Huawei OS Kernel Lab
 * Create: Tue Nov 20 14:54:28 2018
 */
#ifndef ULIBS_INCLUDE_LIBHMSYNC_ATOMIC_C11_H
#define ULIBS_INCLUDE_LIBHMSYNC_ATOMIC_C11_H

#include <stdint.h>
#include <stdatomic.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * We use this when we have __STDC_VERSION__ >= 201112L and __STDC_NO_ATOMICS__
 * not defined
 */
/* XXX: We currently don't do anything if atomic is not lock-free */
typedef atomic_int raw_atomic_int_t;

#define RAW_ATOMIC_VAL_INIT(v) (v)

static inline void raw_atomic_int_init(raw_atomic_int_t *v, int val)
{
	/* atomic_init is nonreentrant function, use atomic_store instead */
	atomic_store_explicit(v, val, memory_order_relaxed);
}

static inline int raw_atomic_int_add_relaxed(raw_atomic_int_t *v, int val)
{
	return atomic_fetch_add_explicit(v, val, memory_order_relaxed) + val;
}

static inline int raw_atomic_int_add(raw_atomic_int_t *v, int val)
{
	return atomic_fetch_add_explicit(v, val, memory_order_seq_cst) + val;
}

static inline int raw_atomic_int_fetch_add_acq_rel(raw_atomic_int_t *v, int val)
{
	return atomic_fetch_add_explicit(v, val, memory_order_acq_rel);
}

static inline int raw_atomic_int_sub_relaxed(raw_atomic_int_t *v, int val)
{
	return atomic_fetch_sub_explicit(v, val, memory_order_relaxed) - val;
}

static inline int raw_atomic_int_sub(raw_atomic_int_t *v, int val)
{
	return atomic_fetch_sub_explicit(v, val, memory_order_seq_cst) - val;
}

static inline int raw_atomic_int_or(raw_atomic_int_t *v, int f)
{
	return atomic_fetch_or_explicit(v, f, memory_order_seq_cst);
}

static inline int raw_atomic_int_or_relaxed(raw_atomic_int_t *v, int f)
{
	return atomic_fetch_or_explicit(v, f, memory_order_relaxed);
}

static inline int raw_atomic_int_fetch_or_acq_rel(raw_atomic_int_t *v, int f)
{
	return atomic_fetch_or_explicit(v, f, memory_order_acq_rel);
}

static inline int raw_atomic_int_and(raw_atomic_int_t *v, int f)
{
	return atomic_fetch_and_explicit(v, f, memory_order_seq_cst);
}

static inline int raw_atomic_int_and_relaxed(raw_atomic_int_t *v, int f)
{
	return atomic_fetch_and_explicit(v, f, memory_order_relaxed);
}

static inline int raw_atomic_int_fetch_and_acq_rel(raw_atomic_int_t *v, int f)
{
	return atomic_fetch_and_explicit(v, f, memory_order_acq_rel);
}

static inline int raw_atomic_int_fetch_xor_acq_rel(raw_atomic_int_t *v, int f)
{
	return atomic_fetch_xor_explicit(v, f, memory_order_acq_rel);
}

static inline int raw_atomic_int_inc_relaxed(raw_atomic_int_t *v)
{
	return atomic_fetch_add_explicit(v, 1, memory_order_relaxed) + 1;
}

static inline int raw_atomic_int_inc(raw_atomic_int_t *v)
{
	return ++(*v); /* this is atomic and seq_cst */
}

static inline int raw_atomic_int_dec_relaxed(raw_atomic_int_t *v)
{
	return atomic_fetch_add_explicit(v, -1, memory_order_relaxed) - 1;
}

static inline int raw_atomic_int_dec(raw_atomic_int_t *v)
{
	return --(*v); /* this is atomic and seq_cst */
}

static inline int raw_atomic_int_set_relaxed(raw_atomic_int_t *v, int val)
{
	atomic_store_explicit(v, val, memory_order_relaxed);
	return val;
}

static inline int raw_atomic_int_set(raw_atomic_int_t *v, int val)
{
	*v = val; /* this is atomic and seq_cst */
	return val;
}

static inline int raw_atomic_int_read(raw_atomic_int_t *v)
{
	return *v; /* this is atomic and seq_cst */
}

static inline int raw_atomic_int_read_relaxed(raw_atomic_int_t *v)
{
	return atomic_load_explicit(v, memory_order_relaxed);
}

static inline int raw_atomic_int_read_acquire(raw_atomic_int_t *v)
{
	return atomic_load_explicit(v, memory_order_acquire);
}

static inline int raw_atomic_int_swap_relaxed(raw_atomic_int_t *v, int val)
{
	return atomic_exchange_explicit(v, val, memory_order_relaxed);
}

static inline int raw_atomic_int_swap_acq_rel(raw_atomic_int_t *v, int val)
{
	return atomic_exchange_explicit(v, val, memory_order_acq_rel);
}

static inline int raw_atomic_int_swap(raw_atomic_int_t *v, int val)
{
	return atomic_exchange(v, val);
}

static inline int raw_atomic_int_cas_relaxed(raw_atomic_int_t *v, int old_v, int new_v)
{
	atomic_compare_exchange_strong_explicit(v, &old_v, new_v,
						memory_order_relaxed,
						memory_order_relaxed);
	return old_v;
}

static inline int raw_atomic_int_cas_acquire(raw_atomic_int_t *v, int old_v, int new_v)
{
	atomic_compare_exchange_strong_explicit(v, &old_v, new_v,
						memory_order_acquire,
						memory_order_relaxed);
	return old_v;
}

static inline int raw_atomic_int_cas(raw_atomic_int_t *v, int old_v, int new_v)
{
	/*
	 * According to
	 * https://en.cppreference.com/w/c/atomic/atomic_compare_exchange
	 *
	 * The weak forms of the functions are allowed to fail spuriously,
	 * that is, act as if *obj != *expected even if they are equal.
	 * When a compare-and-exchange is in a loop, the weak version will
	 * yield better performance on some platforms. When a weak
	 * compare-and-exchange would require a loop and a strong one would
	 * not, the strong one is preferable.
	 */
	(void)atomic_compare_exchange_strong(v, &old_v, new_v);
	return old_v;
}

static inline int raw_atomic_int_try_cmpxchg(raw_atomic_int_t *v, int *old, int new_v)
{
	int r;
	int rc = 0;
	int o = *old;

	r = raw_atomic_int_cas(v, *old, new_v);
	if (r != o) {
		*old = r;
		rc = -1;
	}
	return rc;
}

static inline int raw_atomic_int_inc_unless_negative(raw_atomic_int_t *v)
{
	bool negative = false;
	int c = raw_atomic_int_read(v);

	do {
		if (c < 0) {
			negative = true;
			break;
		}
	} while (raw_atomic_int_try_cmpxchg(v, &c, c + 1) == -1);
	return negative ? -1 : 0;
}

static inline int raw_atomic_int_dec_unless_positive(raw_atomic_int_t *v)
{
	bool positive = false;
	int c = raw_atomic_int_read(v);

	do {
		if (c > 0) {
			positive = true;
			break;
		}
	} while (raw_atomic_int_try_cmpxchg(v, &c, c - 1) == -1);

	return positive ? -1 : 0;
}

typedef atomic_long raw_atomic_long_t;

static inline void raw_atomic_long_init(raw_atomic_long_t *v, long val)
{
	/* atomic_init is nonreentrant function, use atomic_store instead */
	atomic_store_explicit(v, val, memory_order_relaxed);
}

static inline long raw_atomic_long_add_relaxed(raw_atomic_long_t *v, long val)
{
	return atomic_fetch_add_explicit(v, val, memory_order_relaxed) + val;
}

static inline long raw_atomic_long_add(raw_atomic_long_t *v, long val)
{
	return atomic_fetch_add_explicit(v, val, memory_order_seq_cst) + val;
}

static inline long raw_atomic_long_sub_relaxed(raw_atomic_long_t *v, long val)
{
	return atomic_fetch_sub_explicit(v, val, memory_order_relaxed) - val;
}

static inline long raw_atomic_long_sub(raw_atomic_long_t *v, long val)
{
	return atomic_fetch_sub_explicit(v, val, memory_order_seq_cst) - val;
}

static inline long raw_atomic_long_or(raw_atomic_long_t *v, long f)
{
	return atomic_fetch_or_explicit(v, f, memory_order_seq_cst);
}

static inline long raw_atomic_long_or_relaxed(raw_atomic_long_t *v, long f)
{
	return atomic_fetch_or_explicit(v, f, memory_order_relaxed);
}

static inline long raw_atomic_long_and(raw_atomic_long_t *v, long f)
{
	return atomic_fetch_and_explicit(v, f, memory_order_seq_cst);
}

static inline long raw_atomic_long_and_relaxed(raw_atomic_long_t *v, long f)
{
	return atomic_fetch_and_explicit(v, f, memory_order_relaxed);
}

static inline int raw_atomic_long_inc_relaxed(raw_atomic_long_t *v)
{
	return atomic_fetch_add_explicit(v, 1, memory_order_relaxed) + 1;
}

static inline long raw_atomic_long_inc(raw_atomic_long_t *v)
{
	return ++(*v); /* this is atomic and seq_cst */
}

static inline int raw_atomic_long_dec_relaxed(raw_atomic_long_t *v)
{
	return atomic_fetch_add_explicit(v, -1, memory_order_relaxed) - 1;
}

static inline long raw_atomic_long_dec(raw_atomic_long_t *v)
{
	return --(*v); /* this is atomic and seq_cst */
}

static inline long raw_atomic_long_set_relaxed(raw_atomic_long_t *v, long val)
{
	atomic_store_explicit(v, val, memory_order_relaxed);
	return val;
}

static inline long raw_atomic_long_set(raw_atomic_long_t *v, long val)
{
	atomic_store_explicit(v, val, memory_order_seq_cst);
	return val;
}

static inline long raw_atomic_long_read(raw_atomic_long_t *v)
{
	return *v; /* this is atomic and seq_cst */
}

static inline long raw_atomic_long_read_relaxed(raw_atomic_long_t *v)
{
	return atomic_load_explicit(v, memory_order_relaxed);
}

static inline long raw_atomic_long_swap_relaxed(raw_atomic_long_t *v, long val)
{
	return atomic_exchange_explicit(v, val, memory_order_relaxed);
}

static inline long raw_atomic_long_swap(raw_atomic_long_t *v, long val)
{
	return atomic_exchange(v, val);
}

static inline long raw_atomic_long_cas(raw_atomic_long_t *v, long old_v, long new_v)
{
	/*
	 * According to
	 * https://en.cppreference.com/w/c/atomic/atomic_compare_exchange
	 *
	 * The weak forms of the functions are allowed to fail spuriously,
	 * that is, act as if *obj != *expected even if they are equal.
	 * When a compare-and-exchange is in a loop, the weak version will
	 * yield better performance on some platforms. When a weak
	 * compare-and-exchange would require a loop and a strong one would
	 * not, the strong one is preferable.
	 */
	(void)atomic_compare_exchange_strong(v, &old_v, new_v);
	return old_v;
}

typedef unsigned long long ullong;
typedef atomic_ullong raw_atomic_ullong_t;

static inline void raw_atomic_ullong_init(raw_atomic_ullong_t *v, ullong val)
{
	/* atomic_init is nonreentrant function, use atomic_store instead */
	atomic_store_explicit(v, val, memory_order_relaxed);
}

static inline
ullong raw_atomic_ullong_add_relaxed(raw_atomic_ullong_t *v, ullong val)
{
	return atomic_fetch_add_explicit(v, val, memory_order_relaxed) + val;
}

static inline ullong raw_atomic_ullong_add(raw_atomic_ullong_t *v, ullong val)
{
	return atomic_fetch_add_explicit(v, val, memory_order_seq_cst) + val;
}

static inline
ullong raw_atomic_ullong_sub_relaxed(raw_atomic_ullong_t *v, ullong val)
{
	return atomic_fetch_sub_explicit(v, val, memory_order_relaxed) - val;
}

static inline ullong raw_atomic_ullong_sub(raw_atomic_ullong_t *v, ullong val)
{
	return atomic_fetch_sub_explicit(v, val, memory_order_seq_cst) - val;
}

static inline ullong raw_atomic_ullong_inc_relaxed(raw_atomic_ullong_t *v)
{
	return atomic_fetch_add_explicit(v, 1, memory_order_relaxed) + 1;
}

static inline ullong raw_atomic_ullong_inc(raw_atomic_ullong_t *v)
{
	return ++(*v); /* this is atomic and seq_cst */
}

static inline ullong raw_atomic_ullong_dec_relaxed(raw_atomic_ullong_t *v)
{
	return atomic_fetch_add_explicit(v, -1, memory_order_relaxed) - 1;
}

static inline ullong raw_atomic_ullong_dec(raw_atomic_ullong_t *v)
{
	return --(*v); /* this is atomic and seq_cst */
}

static inline ullong raw_atomic_ullong_read(raw_atomic_ullong_t *v)
{
	return *v; /* this is atomic and seq_cst */
}

static inline ullong raw_atomic_ullong_read_relaxed(raw_atomic_ullong_t *v)
{
	return atomic_load_explicit(v, memory_order_relaxed);
}

static inline ullong raw_atomic_ullong_set(raw_atomic_ullong_t *v, ullong val)
{
	atomic_store_explicit(v, val, memory_order_seq_cst);
	return val;
}

static inline
ullong raw_atomic_ullong_set_relaxed(raw_atomic_ullong_t *v, ullong val)
{
	atomic_store_explicit(v, val, memory_order_relaxed);
	return val;
}

#define __raw_atomic_swap_relexed(ptr, val)				\
	atomic_exchange_explicit((ptr), (val), memory_order_relaxed)

#define __raw_atomic_compare_exchange_relaxed(ptr, oval, nval)		\
({									\
	typeof(*(ptr)) ____old = (oval);				\
									\
	atomic_compare_exchange_strong_explicit((ptr), &____old, (nval),\
						memory_order_relaxed,	\
						memory_order_relaxed);	\
	____old;							\
})

#define __raw_atomic_and_relaxed(ptr, val)				\
	atomic_fetch_and_explicit((ptr), val, memory_order_relaxed)

#define __raw_atomic_or_relaxed(ptr, val)				\
	atomic_fetch_or_explicit((ptr), val, memory_order_relaxed)

#ifdef __cplusplus
}
#endif

#endif
