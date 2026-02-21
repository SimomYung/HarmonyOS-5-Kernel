/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2021. All rights reserved.
 * Description: Atomic operations related with gcc builtin
 * Author: Huawei OS Kernel Lab
 * Create: Mon Nov 19 15:00:03 2018
 */
#ifndef ULIBS_INCLUDE_LIBHMSYNC_ATOMIC_GCC47_H
#define ULIBS_INCLUDE_LIBHMSYNC_ATOMIC_GCC47_H

#ifndef __clang__
#define GCC_VERSION (__GNUC__ * 10000           \
		     + __GNUC_MINOR__ * 100     \
		     + __GNUC_PATCHLEVEL__)

#if GCC_VERSION < 40700
#error GCC is too old
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	int val;
} raw_atomic_int_t;

#define RAW_ATOMIC_VAL_INIT(v) {.val = (v)}

static inline void raw_atomic_int_init(raw_atomic_int_t *v, int val)
{
	__atomic_store_n(&v->val, val, __ATOMIC_SEQ_CST);
}

static inline int raw_atomic_int_add_relaxed(raw_atomic_int_t *v, int val)
{
	return __atomic_add_fetch(&v->val, val, __ATOMIC_RELAXED);
}

static inline int raw_atomic_int_add(raw_atomic_int_t *v, int val)
{
	return __atomic_add_fetch(&v->val, val, __ATOMIC_SEQ_CST);
}

static inline int raw_atomic_int_fetch_add_acq_rel(raw_atomic_int_t *v, int val)
{
	return __atomic_fetch_add(&v->val, val, __ATOMIC_ACQ_REL);
}

static inline int raw_atomic_int_sub_relaxed(raw_atomic_int_t *v, int val)
{
	return __atomic_sub_fetch(&v->val, val, __ATOMIC_RELAXED);
}

static inline int raw_atomic_int_sub(raw_atomic_int_t *v, int val)
{
	return __atomic_sub_fetch(&v->val, val, __ATOMIC_SEQ_CST);
}

static inline int raw_atomic_int_or(raw_atomic_int_t *v, int f)
{
	return __atomic_fetch_or(&v->val, f, __ATOMIC_SEQ_CST);
}

static inline int raw_atomic_int_or_relaxed(raw_atomic_int_t *v, int f)
{
	return __atomic_fetch_or(&v->val, f, __ATOMIC_RELAXED);
}

static inline int raw_atomic_int_fetch_or_acq_rel(raw_atomic_int_t *v, int f)
{
	return __atomic_fetch_or(&v->val, f, __ATOMIC_ACQ_REL);
}

static inline int raw_atomic_int_and(raw_atomic_int_t *v, int f)
{
	return __atomic_fetch_and(&v->val, f, __ATOMIC_SEQ_CST);
}

static inline int raw_atomic_int_and_relaxed(raw_atomic_int_t *v, int f)
{
	return __atomic_fetch_and(&v->val, f, __ATOMIC_RELAXED);
}

static inline int raw_atomic_int_fetch_and_acq_rel(raw_atomic_int_t *v, int f)
{
	return __atomic_fetch_and(&v->val, f, __ATOMIC_ACQ_REL);
}

static inline int raw_atomic_int_fetch_xor_acq_rel(raw_atomic_int_t *v, int f)
{
	return __atomic_fetch_xor(&v->val, f, __ATOMIC_ACQ_REL);
}

static inline int raw_atomic_int_inc_relaxed(raw_atomic_int_t *v)
{
	return __atomic_add_fetch(&v->val, 1, __ATOMIC_RELAXED);
}

static inline int raw_atomic_int_inc(raw_atomic_int_t *v)
{
	return __atomic_add_fetch(&v->val, 1, __ATOMIC_SEQ_CST);
}

static inline int raw_atomic_int_dec_relaxed(raw_atomic_int_t *v)
{
	return __atomic_sub_fetch(&v->val, 1, __ATOMIC_RELAXED);
}

static inline int raw_atomic_int_dec(raw_atomic_int_t *v)
{
	return __atomic_sub_fetch(&v->val, 1, __ATOMIC_SEQ_CST);
}

static inline int raw_atomic_int_set_relaxed(raw_atomic_int_t *v, int val)
{
	__atomic_store_n((volatile int*)&v->val, val, __ATOMIC_RELAXED);
	return val;
}

static inline int raw_atomic_int_set(raw_atomic_int_t *v, int val)
{
	__atomic_store_n(&v->val, val, __ATOMIC_SEQ_CST);
	return val;
}

static inline int raw_atomic_int_read_relaxed(raw_atomic_int_t *v)
{
	return __atomic_load_n((volatile int*)&v->val, __ATOMIC_RELAXED);
}

static inline int raw_atomic_int_read_acquire(raw_atomic_int_t *v)
{
	return __atomic_load_n((volatile int*)&v->val, __ATOMIC_ACQUIRE);
}

static inline int raw_atomic_int_read(raw_atomic_int_t *v)
{
	return __atomic_load_n(&v->val, __ATOMIC_SEQ_CST);
}

static inline int raw_atomic_int_swap_relaxed(raw_atomic_int_t *v, int val)
{
	return __atomic_exchange_n((volatile int *)&v->val, val, __ATOMIC_RELAXED);
}

static inline int raw_atomic_int_swap_acq_rel(raw_atomic_int_t *v, int val)
{
	return __atomic_exchange_n((volatile int *)&v->val, val, __ATOMIC_ACQ_REL);
}

static inline int raw_atomic_int_swap(raw_atomic_int_t *v, int val)
{
	return __atomic_exchange_n((volatile int *)&v->val, val, __ATOMIC_SEQ_CST);
}

static inline int raw_atomic_int_cas_relaxed(raw_atomic_int_t *v, int old_v, int new_v)
{
	__atomic_compare_exchange_n(&v->val, &old_v, new_v,
				0, __ATOMIC_RELAXED, __ATOMIC_RELAXED);
	return old_v;
}

static inline int raw_atomic_int_cas_acquire(raw_atomic_int_t *v, int old_v, int new_v)
{
	__atomic_compare_exchange_n(&v->val, &old_v, new_v,
				    0, __ATOMIC_ACQUIRE, __ATOMIC_RELAXED);
	return old_v;
}

static inline int raw_atomic_int_cas(raw_atomic_int_t *v, int old_v, int new_v)
{
	/*
	 * &v->val: ptr
	 * &old_v:  exptected
	 * new_v:   desired
	 * 0:       strong
	 * __ATOMIC_SEQ_CST: success_memmodel
	 * __ATOMIC_SEQ_CST: failure_memmodel
	 *
	 * The weak form is allowed to fail spuriously, we need a loop
	 * if weak form is used, so we use strong form to avoid this
	 * loop, which is also the recommanded way according to manual.
	 */
	(void)__atomic_compare_exchange_n(&v->val, &old_v, new_v, 0,
					  __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
	return old_v;
}

static inline int raw_atomic_int_try_cmpxchg(raw_atomic_int_t *v, int *old, int new_v)
{
	int ret = 0;
	int r, o = *old;

	r = raw_atomic_int_cas(v, *old, new_v);
	if (r != o) {
		*old = r;
		ret = -1;
	}
	return ret;
}

static inline int raw_atomic_int_inc_unless_negative(raw_atomic_int_t *v)
{
	int ret = 0;
	int c = raw_atomic_int_read(v);

	do {
		if (c < 0) {
			ret = -1;
			break;
		}
	} while (raw_atomic_int_try_cmpxchg(v, &c, c + 1) == -1);
	return ret;
}

static inline int raw_atomic_int_dec_unless_positive(raw_atomic_int_t *v)
{
	int ret = 0;
	int c = raw_atomic_int_read(v);

	do {
		if (c > 0) {
			ret = -1;
			break;
		}
	} while (raw_atomic_int_try_cmpxchg(v, &c, c - 1) == -1);
	return ret;
}

typedef struct {
	long val;
} raw_atomic_long_t;

static inline void raw_atomic_long_init(raw_atomic_long_t *v, long val)
{
	__atomic_store_n(&v->val, val, __ATOMIC_SEQ_CST);
}

static inline long raw_atomic_long_add_relaxed(raw_atomic_long_t *v, long val)
{
	return __atomic_add_fetch(&v->val, val, __ATOMIC_RELAXED);
}

static inline long raw_atomic_long_add(raw_atomic_long_t *v, long val)
{
	return __atomic_add_fetch(&v->val, val, __ATOMIC_SEQ_CST);
}

static inline long raw_atomic_long_sub_relaxed(raw_atomic_long_t *v, long val)
{
	return __atomic_sub_fetch(&v->val, val, __ATOMIC_RELAXED);
}

static inline long raw_atomic_long_sub(raw_atomic_long_t *v, long val)
{
	return __atomic_sub_fetch(&v->val, val, __ATOMIC_SEQ_CST);
}

static inline long raw_atomic_long_or(raw_atomic_long_t *v, long f)
{
	return __atomic_fetch_or(&v->val, f, __ATOMIC_SEQ_CST);
}

static inline long raw_atomic_long_or_relaxed(raw_atomic_long_t *v, long f)
{
	return __atomic_fetch_or(&v->val, f, __ATOMIC_RELAXED);
}

static inline long raw_atomic_long_and(raw_atomic_long_t *v, long f)
{
	return __atomic_fetch_and(&v->val, f, __ATOMIC_SEQ_CST);
}

static inline long raw_atomic_long_and_relaxed(raw_atomic_long_t *v, long f)
{
	return __atomic_fetch_and(&v->val, f, __ATOMIC_RELAXED);
}


static inline long raw_atomic_long_inc_relaxed(raw_atomic_long_t *v)
{
	return __atomic_add_fetch(&v->val, 1, __ATOMIC_RELAXED);
}

static inline long raw_atomic_long_inc(raw_atomic_long_t *v)
{
	return __atomic_add_fetch(&v->val, 1, __ATOMIC_SEQ_CST);
}

static inline long raw_atomic_long_dec_relaxed(raw_atomic_long_t *v)
{
	return __atomic_sub_fetch(&v->val, 1, __ATOMIC_RELAXED);
}

static inline long raw_atomic_long_dec(raw_atomic_long_t *v)
{
	return __atomic_sub_fetch(&v->val, 1, __ATOMIC_SEQ_CST);
}

static inline long raw_atomic_long_set(raw_atomic_long_t *v, long val)
{
	__atomic_store_n(&v->val, val, __ATOMIC_SEQ_CST);
	return val;
}

static inline long raw_atomic_long_set_relaxed(raw_atomic_long_t *v, long val)
{
	__atomic_store_n((volatile long*)&v->val, val, __ATOMIC_RELAXED);
	return val;
}

static inline long raw_atomic_long_read(raw_atomic_long_t *v)
{
	return __atomic_load_n(&v->val, __ATOMIC_SEQ_CST);
}

static inline long raw_atomic_long_read_relaxed(raw_atomic_long_t *v)
{
	return __atomic_load_n((volatile long*)&v->val, __ATOMIC_RELAXED);
}

static inline long raw_atomic_long_swap_relaxed(raw_atomic_long_t *v, long val)
{
	return __atomic_exchange_n((volatile long *)&v->val, val, __ATOMIC_RELAXED);
}

static inline long raw_atomic_long_swap(raw_atomic_long_t *v, long val)
{
	return __atomic_exchange_n((volatile long *)&v->val, val, __ATOMIC_SEQ_CST);
}

static inline long raw_atomic_long_cas(raw_atomic_long_t *v, long old_v, long new_v)
{
	/*
	 * &v->val: ptr
	 * &old_v:  exptected
	 * new_v:   desired
	 * 0:       strong
	 * __ATOMIC_SEQ_CST: success_memmodel
	 * __ATOMIC_SEQ_CST: failure_memmodel
	 *
	 * The weak form is allowed to fail spuriously, we need a loop
	 * if weak form is used, so we use strong form to avoid this
	 * loop, which is also the recommanded way according to manual.
	 */
	(void)__atomic_compare_exchange_n(&v->val, &old_v, new_v, 0,
					  __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
	return old_v;
}

typedef unsigned long long ullong;
typedef struct {
	ullong val;
} raw_atomic_ullong_t;

static inline void raw_atomic_ullong_init(raw_atomic_ullong_t *v, ullong val)
{
	__atomic_store_n(&v->val, val, __ATOMIC_SEQ_CST);
}

static inline
ullong raw_atomic_ullong_add_relaxed(raw_atomic_ullong_t *v, ullong val)
{
	return __atomic_add_fetch(&v->val, val, __ATOMIC_RELAXED);
}

static inline ullong raw_atomic_ullong_add(raw_atomic_ullong_t *v, ullong val)
{
	return __atomic_add_fetch(&v->val, val, __ATOMIC_SEQ_CST);
}

static inline
ullong raw_atomic_ullong_sub_relaxed(raw_atomic_ullong_t *v, ullong val)
{
	return __atomic_sub_fetch(&v->val, val, __ATOMIC_RELAXED);
}

static inline ullong raw_atomic_ullong_sub(raw_atomic_ullong_t *v, ullong val)
{
	return __atomic_sub_fetch(&v->val, val, __ATOMIC_SEQ_CST);
}

static inline ullong raw_atomic_ullong_inc_relaxed(raw_atomic_ullong_t *v)
{
	return __atomic_add_fetch(&v->val, 1, __ATOMIC_RELAXED);
}

static inline ullong raw_atomic_ullong_inc(raw_atomic_ullong_t *v)
{
	return __atomic_add_fetch(&v->val, 1, __ATOMIC_SEQ_CST);
}

static inline ullong raw_atomic_ullong_dec_relaxed(raw_atomic_ullong_t *v)
{
	return __atomic_sub_fetch(&v->val, 1, __ATOMIC_RELAXED);
}

static inline ullong raw_atomic_ullong_dec(raw_atomic_ullong_t *v)
{
	return __atomic_sub_fetch(&v->val, 1, __ATOMIC_SEQ_CST);
}

static inline ullong raw_atomic_ullong_read(raw_atomic_ullong_t *v)
{
	return __atomic_load_n(&v->val, __ATOMIC_SEQ_CST);
}

static inline ullong raw_atomic_ullong_read_relaxed(raw_atomic_ullong_t *v)
{
	return __atomic_load_n((volatile ullong*)&v->val, __ATOMIC_RELAXED);
}

static inline ullong raw_atomic_ullong_set(raw_atomic_ullong_t *v, ullong val)
{
	__atomic_store_n(&v->val, val, __ATOMIC_SEQ_CST);
	return val;
}

static inline
ullong raw_atomic_ullong_set_relaxed(raw_atomic_ullong_t *v, ullong val)
{
	__atomic_store_n((volatile ullong*)&v->val, val, __ATOMIC_RELAXED);
	return val;
}

#define __raw_atomic_swap_relexed(ptr, val)				\
	__atomic_exchange_n((volatile typeof(*(ptr)) *)(ptr), (val),	\
			    __ATOMIC_RELAXED)

#define __raw_atomic_compare_exchange_relaxed(ptr, oval, nval)		\
({									\
	typeof(*(ptr)) ____old = (oval);				\
									\
	__atomic_compare_exchange_n((volatile typeof(*(ptr)) *)(ptr),	\
				    &____old, (nval), 0,		\
				    __ATOMIC_RELAXED, __ATOMIC_RELAXED);\
	____old;							\
})

#define __raw_atomic_and_relaxed(ptr, val)				\
	__atomic_fetch_and((volatile typeof(*(ptr)) *)(ptr), val,	\
			   __ATOMIC_RELAXED)

#define __raw_atomic_or_relaxed(ptr, val)				\
	__atomic_fetch_or((volatile typeof(*(ptr)) *)(ptr), val,	\
			  __ATOMIC_RELAXED)

#ifdef __cplusplus
}
#endif

#endif
