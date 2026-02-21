/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2021. All rights reserved.
 * Description: Generic
 * Author: Huawei OS Kernel Lab
 * Create: Mon Nov 19 15:00:03 2018
 */
#ifndef ULIBS_INCLUDE_LIBHMSYNC_BITOPS_GENERIC_H
#define ULIBS_INCLUDE_LIBHMSYNC_BITOPS_GENERIC_H

#include <stdbool.h>
#include <hongmeng/compiler.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef BITS_TO_LONGLONGS
#define BITS_PER_LONGLONG	(8U * (unsigned int)sizeof(unsigned long long))
#define BITS_TO_LONGLONGS(n_bits)	\
	(((n_bits) % (typeof(n_bits))BITS_PER_LONGLONG) == (typeof(n_bits))0 ?	\
	 ((n_bits) / (typeof(n_bits))BITS_PER_LONGLONG) :			\
	 ((n_bits) / (typeof(n_bits))BITS_PER_LONGLONG + (typeof(n_bits))1))
#endif

#define DEFINE_RAW_BITMAP(name, n_bits) unsigned long long name[BITS_TO_LONGLONGS(n_bits)]

#define __mask_of(v, m, o)		\
	({typeof(v) __one = (typeof(v))1u;	\
	((((__one) << (m)) - __one) << (o));})

#define mask_offset(v, m, o) \
	({typeof(v) ma = __mask_of(v, m, o); \
	(v) & ma;})

#define unmask_offset(v, m, o)	\
	({typeof(v) ___v = (v);	\
	typeof(___v) ma = __mask_of(___v, m, o);\
	___v & ~ma;})

#define mask(v, n) mask_offset(v, n, 0u)

#define unmask(v, n) unmask_offset(v, n, 0u)

/* from LSB */
static inline __always_inline unsigned int
raw_bitmap_find_first_zero(const unsigned long long *bitmap,
			   unsigned int sz)
{
	unsigned int long_sz = BITS_TO_LONGLONGS(sz), off;
	unsigned int bit_off = 0;

	for (off = 0; off < long_sz; off++) {
		if (bitmap[off] == (unsigned long long)-1) {
			continue;
		}
		bit_off = (unsigned int)(__builtin_ffsll((long long)(~bitmap[off])) - 1);
		break;
	}

	bit_off += BITS_PER_LONGLONG * off;
	return bit_off >= sz ? sz : bit_off;
}

static inline bool raw_bitmap_is_full(const unsigned long long *bitmap,
				     unsigned int sz)
{
	return raw_bitmap_find_first_zero(bitmap, sz) == sz;
}

/* from LSB, search scope is bits [start+1, last] */
static inline __always_inline __pure unsigned int
raw_bitmap_find_next_zero(const unsigned long long *bitmap,
			  unsigned int sz,
			  unsigned int _start)
{
	unsigned int long_sz = BITS_TO_LONGLONGS(sz), off;
	unsigned int bit_off;
	unsigned long long tmp;
	unsigned int start = _start;

	if (++start >= sz) {
		return sz;
	}
	off = start / BITS_PER_LONGLONG;
	bit_off = (start & (BITS_PER_LONGLONG - 1U));
	tmp = bitmap[off] | ((1ULL << bit_off) - 1U);
	if (tmp != (unsigned long long)-1) {
		bit_off = (unsigned int)__builtin_ffsll((long long)(~tmp)) - 1;
		goto found;
	}

	for (off = off + 1U; off < long_sz; off++) {
		if (bitmap[off] == (unsigned long long)-1) {
			continue;
		}
		bit_off = (unsigned int)__builtin_ffsll((long long)(~bitmap[off])) - 1;
		break;
	}

found:
	bit_off += BITS_PER_LONGLONG * off;
	return bit_off >= sz ? sz : bit_off;
}

/* from LSB */
static inline __always_inline unsigned int
raw_bitmap_find_first_one(const unsigned long long *bitmap,
			  unsigned int sz)
{
	unsigned int long_sz = BITS_TO_LONGLONGS(sz), off;
	unsigned int bit_off = 0;

	for (off = 0; off < long_sz; off++) {
		if (bitmap[off] == 0ULL) {
			continue;
		}
		bit_off = (unsigned int)__builtin_ffsll((long long)(bitmap[off])) - 1;
		break;
	}

	bit_off += BITS_PER_LONGLONG * off;
	return bit_off >= sz ? sz : bit_off;
}

/* from LSB, search scope is bits [start+1, last] */
static inline __always_inline __pure unsigned int
raw_bitmap_find_next_one(const unsigned long long *bitmap,
			 unsigned int sz,
			 unsigned int _start)
{
	unsigned int long_sz = BITS_TO_LONGLONGS(sz), off;
	unsigned int bit_off;
	unsigned long long tmp;
	unsigned int start = _start;

	if (++start >= sz) {
		return sz;
	}
	off = start / BITS_PER_LONGLONG;
	bit_off = start & (BITS_PER_LONGLONG - 1U);
	tmp = bitmap[off] & ~((1ULL << bit_off) - 1U);
	if (tmp != 0ULL) {
		bit_off = (unsigned int)__builtin_ffsll((long long)tmp) - 1;
		goto found;
	}

	for (off = off + 1U; off < long_sz; off++) {
		if (bitmap[off] == 0ULL) {
			continue;
		}
		bit_off = (unsigned int)__builtin_ffsll((long long)bitmap[off]) - 1;
		break;
	}

found:
	bit_off += BITS_PER_LONGLONG * off;
	return bit_off >= sz ? sz : bit_off;
}

/* bit_off counts from LSB */
static inline int
raw_bitmap_set_bit(unsigned long long *bitmap,
		   unsigned int sz,
		   unsigned int _bit_off)
{
	int ret = 0;
	unsigned int long_off = _bit_off / BITS_PER_LONGLONG;
	unsigned int bit_off = _bit_off;

	if (bit_off >= sz) {
		ret = -1;
	}

	if (ret == 0) {
		bit_off &= (BITS_PER_LONGLONG - 1U);
		bitmap[long_off] |= (1ULL << bit_off);
	}
	return ret;
}

/* from LSB */
static inline __always_inline unsigned int
raw_bitmap_find_last_one(const unsigned long long *bitmap,
			  unsigned int sz)
{
	unsigned int long_sz = BITS_TO_LONGLONGS(sz), off;
	unsigned int bit_off = 0;
	unsigned long long fix = 0ULL;

	if (long_sz == 0U) {
		return 0U;
	}

	for (off = long_sz - 1; off >= 0; off--) {
		if (bitmap[off] == 0ULL) {
			continue;
		}

		bit_off = (unsigned int)__builtin_clzll((long long)(bitmap[off]));
		break;
	}

	bit_off = BITS_PER_LONGLONG * (off + 1U) - bit_off - 1U;

	return bit_off >= sz ? sz : bit_off;
}

/* bit_off counts from LSB */
static inline int
raw_bitmap_clear_bit(unsigned long long *bitmap,
		     unsigned int sz,
		     unsigned int _bit_off)
{
	int ret = 0;
	unsigned int long_off = _bit_off / BITS_PER_LONGLONG;
	unsigned int bit_off = _bit_off;

	if (bit_off >= sz) {
		ret = -1;
	}

	if (ret == 0) {
		bit_off &= (BITS_PER_LONGLONG - 1U);
		bitmap[long_off] &= ~(1ULL << bit_off);
	}
	return ret;
}

/* bit_off counts from LSB */
static inline int
raw_bitmap_test_bit(const unsigned long long *bitmap,
		    unsigned int sz,
		    unsigned int _bit_off)
{
	int ret = -1;
	unsigned int long_off;
	unsigned int bit_off = _bit_off;

	if (bit_off >= sz) {
		ret = 0;
	}

	if (ret == -1) {
		long_off = (bit_off / BITS_PER_LONGLONG);
		bit_off &= (BITS_PER_LONGLONG - 1U);
		ret = ((bitmap[long_off] & (1ULL << bit_off)) != 0ULL);
	}
	return ret;
}

/* Warn: search idx range: [start + 1, size - 1], "START" is NOT included */
static unsigned int raw_bitmap_find_next_zero_area_offset(unsigned long long *map,
			unsigned int size, unsigned int _start, unsigned int nr)
{
	unsigned int index;
	unsigned int end;
	unsigned int i;
	unsigned int start = _start;

	if (nr == 0) {
		return size;
	}

	while (true) {
		index = raw_bitmap_find_next_zero(map, size, start);

		end = index + nr;
		if (end > size) {
			return end;
		}

		i = raw_bitmap_find_next_one(map, end, index);
		if (i < end) {
			start = i + 1;
			continue;
		}

		break;
	}
	return index;
}

#ifdef __cplusplus
}
#endif

#endif
