/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Declare functions and symbols for kernel bitops
 * Author: Huawei OS Kernel Lab
 * Create: Tue Oct 08 10:31:07 2019
 */
#ifndef MAPI_HMKERNEL_BITOPS_H
#define MAPI_HMKERNEL_BITOPS_H

#include <lib/string.h>
#include <hmkernel/types.h>
#include <hmkernel/bitops/bits.h>

#define BITS_PER_BYTE		8U
#define BITS_PER_BYTE_SHIFT	3U

#ifndef BITS_TO_LONGS
#define BITS_TO_LONGS(n_bits)			\
	(((n_bits) % U(BITS_PER_LONG)) == 0U ?	\
	 (n_bits) / U(BITS_PER_LONG) :		\
	 (n_bits) / U(BITS_PER_LONG) + 1U)
#endif

#define BIT_RANGE(start, end) (((BIT((end) - (start)) - 1UL) << (start)) | BIT(end))

#define __MASK_OF(v, m, o)		\
	({typeof(v) __one = (typeof(v))1u;	\
	((((__one) << (m)) - __one) << (o));})

#define MASK_OFFSET(v, m, o) \
	({typeof(v) ma = __MASK_OF(v, m, o); \
	(v) & ma;})

#define UNMASK_OFFSET(v, m, o) \
	({typeof(v) ___v = (v); \
	typeof(___v) ma = __MASK_OF(___v, m, o);\
	___v & ~ma;})

#define MASK(v, n) MASK_OFFSET(v, n, 0u)

#define UNMASK(v, n) UNMASK_OFFSET(v, n, 0u)

/*
 * Set `m` bits with shift `o` in value `v` by value `nv`.
 * e.g.: SET_BITS_OFFSET(5, 2, 1, 1) = 3.
 */
#define SET_BITS_OFFSET(v, m, o, nv) \
	({typeof(v) ___tv = (v); \
	 typeof(v) ___ntv = (typeof(v))(nv); \
	 ___tv = UNMASK_OFFSET(___tv, m, o); \
	 ___ntv = MASK(___ntv, m); \
	 ___tv | (___ntv << (o));})

/*
 * Get `m` bits with shift `o` in value `v`.
 * e.g.: EXTRACT_BITS_OFFSET(5, 2, 1) = 2.
 */
#define EXTRACT_BITS_OFFSET(v, m, o) \
	({typeof(v) ___tv = (v); \
	 ___tv = (___tv >> (o)); \
	 MASK(___tv, m);})

#define IS_ALIGNED_ORDER(v, order) ((MASK((v), (order)) == (typeof(v))0u) ? true : false)

/*
 * Start definition of bitmap.
 *
 * For all bit-to-position operations:
 * type of retval is unsigned int.
 * 0 is position 0.
 * A position larger than range of input means invalid.
 * (unsigned int)(-1) can be used as a common invalid retval.
 */
#define DEFINE_BITMAP(name, n_bits) unsigned long name[BITS_TO_LONGS(n_bits)]
#define DEFINE_BITMAP_WRITABLE(name, n_bits) DEFINE_BITMAP(name, n_bits) __rw_after_init

static inline void
__set_bit(unsigned long *addr, unsigned int nr)
{
	unsigned long __mask;
	unsigned long *p = NULL;

	__mask = BIT(nr % U(BITS_PER_LONG));
	p = &addr[nr / U(BITS_PER_LONG)];
	*p |= __mask;
}

static inline void
__clear_bit(unsigned long *addr, unsigned int nr)
{
	unsigned long __mask;
	unsigned long *p = NULL;

	__mask = BIT(nr % U(BITS_PER_LONG));
	p = &addr[nr / U(BITS_PER_LONG)];
	*p &= ~__mask;
}

static inline bool
__is_bit_set(unsigned long *addr, unsigned int nr)
{
	unsigned long __mask = BIT(nr % U(BITS_PER_LONG));
	unsigned long *p = &addr[nr / U(BITS_PER_LONG)];

	return __IS((*p & __mask));
}

JAM_INLINE unsigned int
__bitmap_find_next_zero_lsb(const unsigned long *bitmap,
			    unsigned int sz, unsigned int start);

JAM_INLINE unsigned int
__bitmap_find_next_one_lsb(const unsigned long *bitmap,
			   unsigned int sz, unsigned int start);

JAM_INLINE unsigned int
bitmap_find_first_one_lsb(const unsigned long *bitmap, unsigned int sz);

JAM_INLINE unsigned int
bitmap_find_first_zero_lsb(const unsigned long *bitmap, unsigned int sz);

static inline __always_inline
unsigned int bitmap_find_next_zero_lsb(unsigned long *bitmap,
				       unsigned int sz, unsigned int start)
{
	unsigned int ret = 0U;
	unsigned int _start = start;

	_start++;
	if (_start >= sz) {
		ret = sz;
	} else {
		ret = __bitmap_find_next_zero_lsb(bitmap, sz, _start);
	}
	return ret;
}

static inline __always_inline
unsigned int bitmap_find_next_one_lsb(unsigned long *bitmap,
				      unsigned int sz, unsigned int start)
{
	unsigned int ret = 0U;
	unsigned int _start = start;

	_start++;
	if (_start >= sz) {
		ret = sz;
	} else {
		ret = __bitmap_find_next_one_lsb(bitmap, sz, _start);
	}
	return ret;
}

JAM_INLINE unsigned int
__bitmap_find_next_zero_msb(const unsigned long *bitmap,
			    unsigned int sz, unsigned int start);

JAM_INLINE unsigned int
__bitmap_find_next_one_msb(const unsigned long *bitmap,
			   unsigned int sz, unsigned int start);

JAM_INLINE unsigned int
bitmap_find_first_one_msb(const unsigned long *bitmap, unsigned int sz);

JAM_INLINE unsigned int
bitmap_find_first_zero_msb(const unsigned long *bitmap, unsigned int sz);

static inline __always_inline
unsigned int bitmap_find_next_zero_msb(unsigned long *bitmap,
				       unsigned int sz, unsigned int start)
{
	unsigned int ret = 0U;
	unsigned int _start = start;

	_start--;
	if (_start >= sz) {
		ret = sz;
	} else {
		ret = __bitmap_find_next_zero_msb(bitmap, sz, _start);
	}
	return ret;
}

static inline __always_inline
unsigned int bitmap_find_next_one_msb(unsigned long *bitmap,
				      unsigned int sz, unsigned int start)
{
	unsigned int ret = 0U;
	unsigned int _start = start;

	_start--;
	if (_start >= sz) {
		ret = sz;
	} else {
		ret = __bitmap_find_next_one_msb(bitmap, sz, _start);
	}
	return ret;
}

#define for_each_set_bit(bit, addr, size)				\
	for ((bit) = bitmap_find_first_one_lsb((addr), (size));		\
	     (bit) < (size);						\
	     (bit) = bitmap_find_next_one_lsb((addr), (size), (bit)))

#define for_each_set_bit_from(bit, addr, size) \
	for ((bit) = __bitmap_find_next_one_lsb((addr), (size), (bit));	\
	     (bit) < (size);						\
	     (bit) = bitmap_find_next_one_lsb((addr), (size), (bit)))

#define for_each_clear_bit(bit, addr, size)				\
	for ((bit) = bitmap_find_first_zero_lsb((addr), (size));	\
	     (bit) < (size);						\
	     (bit) = bitmap_find_next_zero_lsb((addr), (size), (bit)))

#define for_each_clear_bit_from(bit, addr, size)			\
	for ((bit) = __bitmap_find_next_zero_lsb((addr), (size), (bit));\
	     (bit) < (size);						\
	     (bit) = bitmap_find_next_zero_lsb((addr), (size), (bit)))

#define for_each_set_bit_msb(bit, addr, size)				\
	for ((bit) = bitmap_find_first_one_msb((addr), (size));		\
	     (bit) < (size);						\
	     (bit) = bitmap_find_next_one_msb((addr), (size), (bit)))

static inline void bitmap_set_all(unsigned long *bitmap, unsigned int sz)
{
	unsigned int len = BITS_TO_LONGS(sz);
	unsigned int off;

	for (off = 0U; off < len; off++) {
		bitmap[off] = ~0UL;
	}
}

static inline void bitmap_clear_all(unsigned long *bitmap, unsigned int sz)
{
	size_t len = BITS_TO_LONGS((unsigned long)sz) * sizeof(unsigned long);

	bzero(bitmap, len);
}

#endif
