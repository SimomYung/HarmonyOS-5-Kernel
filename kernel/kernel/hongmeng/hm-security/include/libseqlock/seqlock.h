/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Provide sequence lock functions for shared hash table
 * Author: Huawei OS Kernel Lab
 * Create: Thu Apr 25 10:47:48 2019
 */
#ifndef ULIBS_LIBSEQLOCK_H
#define ULIBS_LIBSEQLOCK_H
// libc
#include <stdint.h>
#include <stdbool.h>
// hongmeng
#include <hmkernel/strict.h>
#include <hmkernel/barrier.h>
#include <hongmeng/compiler.h>
#include <hmkernel/processor.h>

typedef uint32_t seq_t;

static inline seq_t seq_read(seq_t *seq)
{
	return *(volatile seq_t *)seq;
}

static inline bool seq_is_readable(seq_t seq)
{
	return ____NOT(seq & (seq_t)1);
}

#if __STDC_VERSION__ >= 201112L && !defined(__STDC_NO_ATOMICS__)
#include <stdatomic.h>
#define sht_cas(PTR, OLD, NEW)                                      \
	atomic_compare_exchange_weak_explicit(PTR, OLD, NEW,        \
					      memory_order_relaxed, \
					      memory_order_relaxed)
#elif defined(__GNUC__) || defined(__clang__)
#define sht_cas(PTR, OLD, NEW)                        \
	__atomic_compare_exchange_n(PTR, OLD, NEW, 1, \
				    __ATOMIC_RELAXED, \
				    __ATOMIC_RELAXED)
#else
raw_static_assert(0, "no atomic support");
#endif

static inline void seq_lock(seq_t *seq)
{
	seq_t old_seq = seq_read(seq);

	while (!seq_is_readable(old_seq) || !sht_cas(seq, &old_seq, old_seq + (seq_t)1)) {
		cpu_relax();
		old_seq = seq_read(seq);
	}
}

static inline void seq_unlock(seq_t *seq)
{
	(*seq)++;
}

static inline void seq_write_start(seq_t *seq)
{
	seq_lock(seq);
	__smp_wmb();
}

static inline void seq_write_end(seq_t *seq)
{
	__smp_wmb();
	seq_unlock(seq);
}

static inline void seq_try_no_barrier(seq_t *seq, seq_t *res)
{
	while (true) {
		*res = seq_read(seq);
		if (seq_is_readable(*res)) {
			break;
		}
		cpu_relax();
	}
}

static inline seq_t seq_try(seq_t *seq)
{
	seq_t s;

	seq_try_no_barrier(seq, &s);
	__smp_rmb();

	return s;
}

static inline bool seq_need_retry_raw(seq_t *seq, seq_t expected)
{
	return seq_read(seq) != expected;
}

static inline bool seq_need_retry(seq_t *seq, seq_t expected)
{
	__smp_rmb();
	return seq_need_retry_raw(seq, expected);
}

#endif /* ifndef ULIBS_LIBSEQLOCK_H */
