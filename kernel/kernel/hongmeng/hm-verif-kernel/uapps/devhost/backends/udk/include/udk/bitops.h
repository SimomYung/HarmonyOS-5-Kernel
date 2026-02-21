/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Declare interface about bitops
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jul 12 15:02:57 2021
 */

#ifndef __UDK_BITOPS_H__
#define __UDK_BITOPS_H__

#include <hmasm/page.h>
#include <limits.h>
#include <libmem/utils.h>
#include <udk/errno.h>
#include <vsync/atomic.h>

/* left shift n-bits for a value */
#define UDK_BIT_LS(v, n)		((v) << (n))
/* right shift n-bits for a value */
#define UDK_BIT_RS(v, n)		((v) >> (n))

/* set bit n only */
#define UDK_BIT(n)			UDK_BIT_LS(1UL, n)

/* mask a value */
#define UDK_MASK(v, m)			((v) & (m))
/* unmask a value */
#define UDK_UNMASK(v, m)		((v) & ~(m))
/* make mask with n-bits width */
#define UDK_MAKE_MASK(n)		(UDK_BIT(n) - 1UL)
/* generate mask in bit range [a, b] */
#define UDK_GEN_MASK(a, b)		\
	UDK_UNMASK(UDK_MAKE_MASK((b) + 1UL), UDK_MAKE_MASK(a))

/* mask with the mask start from bit n for a value */
#define udk_mask_bits(v, m, n)		UDK_MASK(v, UDK_BIT_LS(m, n))
/* unmask with the mask start from bit n for a value */
#define udk_unmask_bits(v, m, n)	UDK_UNMASK(v, UDK_BIT_LS(m, n))
/* get data in mask start from bit n for a value */
#define udk_get_bits(v, m, n)		UDK_MASK(UDK_BIT_RS(v, n), m)
/* set data in mask start from bit n for a value */
#define udk_set_bits(v, d, m, n)	\
	(udk_unmask_bits(v, m, n) | UDK_BIT_LS(UDK_MASK(d, m), n))

/* set bit n for a value */
#define udk_set_bit(v, n)		((v) | UDK_BIT(n))
/* clear bit n for a value */
#define udk_clear_bit(v, n)		UDK_UNMASK(v, UDK_BIT(n))
/* test bit n (0 or 1) for a value */
#define udk_test_bit(v, n)		(UDK_MASK(v, UDK_BIT(n)) != 0UL)

static inline int udk_size_to_order(unsigned long size)
{
	if (size == 0UL || (size & (size - 1U)) != 0) {
		return -EINVAL;
	}

	return (__builtin_ffsl((long)(size)) - 1);
}

/* size to order without page shift
 * e.g. udk_size_to_page_order(1) = 0
 *	udk_size_to_page_order(4095) = 0
 *	udk_size_to_page_order(4096) = 0
 *	udk_size_to_page_order(4097) = 1
 *	udk_size_to_page_order(32K) = 3 */
static inline int udk_size_to_page_order(unsigned long size)
{
	unsigned long long size_ull;

	if (size == 0U) {
		return 0;
	}

	size_ull = ((unsigned long long)size - 1ULL) >> PAGE_SHIFT;
	return (int)(sizeof(unsigned long long) * BITS_PER_BYTE - __builtin_clzll(size_ull));
}

static inline unsigned long udk_order_to_size(unsigned int order)
{
	if (order >= sizeof(unsigned long) * BITS_PER_BYTE) {
		return ULONG_MAX;
	}

	return UDK_BIT_LS(1UL, order);
}

#define bit(nr)          (1UL << (nr))
#define __is(x)          ____is(x)
#define ____is(x)        (((x) != ((typeof(x))0)) ? __true : __false)
#ifndef BITS_PER_LONG
#define BITS_PER_LONG    (8UL * sizeof(unsigned long))
#endif
#define BIT_WORD(nr) ((nr) / BITS_PER_LONG)
#define BIT_MASK(nr) (1UL << ((nr) % BITS_PER_LONG))
#ifndef BITS_TO_LONGS
#define BITS_TO_LONGS(n_bits)  (((n_bits) + BITS_PER_LONG - 1U) / BITS_PER_LONG)
#endif

#define WORD_BYTE_32 32
#define WORD_BYTE_64 64

/* clear a bit atomically */
static inline void bit_clear(unsigned long *addr, unsigned int nr)
{
	addr += BIT_WORD(nr);

	if (BITS_PER_LONG == WORD_BYTE_32) {
		vatomic32_and((vatomic32_t *)addr, ~((vuint32_t)BIT_MASK(nr)));
	} else if (BITS_PER_LONG == WORD_BYTE_64) {
		vatomic64_and((vatomic64_t *)addr, ~((vuint64_t)BIT_MASK(nr)));
	} else {
		hm_error("odd BITS_PER_LONG %lu\n", (unsigned long)BITS_PER_LONG);
	}
}

/* set a bit atomically */
static inline void bit_set(unsigned long *addr, unsigned int nr)
{
	addr += BIT_WORD(nr);

	if (BITS_PER_LONG == WORD_BYTE_32) {
		vatomic32_or((vatomic32_t *)addr, (vuint32_t)BIT_MASK(nr));
	} else if (BITS_PER_LONG == WORD_BYTE_64) {
		vatomic64_or((vatomic64_t *)addr, (vuint64_t)BIT_MASK(nr));
	} else {
		hm_error("odd BITS_PER_LONG %lu\n", (unsigned long)BITS_PER_LONG);
	}
}

static inline bool bit_test(unsigned long *addr, unsigned int nr)
{
	return 1UL & (addr[BIT_WORD(nr)] >> (nr & (BITS_PER_LONG - 1)));
}

static inline unsigned long atomic_long_get_or(unsigned long *addr, unsigned long mask)
{
	unsigned long old = 0;

	if (BITS_PER_LONG == WORD_BYTE_32) {
		old = vatomic32_get_or((vatomic32_t *)addr, (vuint32_t)mask);
	} else if (BITS_PER_LONG == WORD_BYTE_64) {
		old = vatomic64_get_or((vatomic64_t *)addr, (vuint64_t)mask);
	} else {
		hm_error("odd BITS_PER_LONG %lu\n", (unsigned long)BITS_PER_LONG);
	}

	return old;
}

/* test and set a bit atomically */
static inline bool test_and_set_bit(unsigned long *addr, unsigned int nr)
{
	unsigned long old;
	unsigned long mask = BIT_MASK(nr);

	addr += BIT_WORD(nr);
	if (*addr & mask) {
		return true;
	}

	old = atomic_long_get_or(addr, mask);

	return !!(old & mask);
}

static inline unsigned long atomic_long_get_and(unsigned long *addr, unsigned long mask)
{
	unsigned long old = 0;

	if (BITS_PER_LONG == WORD_BYTE_32) {
		old = vatomic32_get_and((vatomic32_t *)addr, ~((vuint32_t)mask));
	} else if (BITS_PER_LONG == WORD_BYTE_64) {
		old = vatomic64_get_and((vatomic64_t *)addr, ~((vuint64_t)mask));
	} else {
		hm_error("odd BITS_PER_LONG %lu\n", (unsigned long)BITS_PER_LONG);
	}

	return old;
}

/* test and clear a bit atomically */
static inline bool test_and_clear_bit(unsigned long *addr, unsigned int nr)
{
	unsigned long old;
	unsigned long mask = BIT_MASK(nr);

	addr += BIT_WORD(nr);
	if (!(*addr & mask)) {
		return false;
	}

	old = atomic_long_get_and(addr, mask);

	return !!(old & mask);
}

#endif
