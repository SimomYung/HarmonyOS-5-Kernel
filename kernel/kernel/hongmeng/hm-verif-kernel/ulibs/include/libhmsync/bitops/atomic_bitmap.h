/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: atomic bitmap
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jun 23 20:27:03 2024
 */

#ifndef ULIBS_INCLUDE_LIBHMSYNC_BITOPS_ATOMIC_BITMAP_H
#define ULIBS_INCLUDE_LIBHMSYNC_BITOPS_ATOMIC_BITMAP_H

#include "generic.h"

static inline uint64_t get_bit_val_from_u64(uint64_t val, unsigned int index)
{
	return (val & (1ULL << index)) >> index;
}

static inline int atomic_bitmap_set_bit(vatomic64_t *bitmap, unsigned int sz, unsigned int _bit_off)
{
	int ret = 0;
	unsigned int long_off = _bit_off / BITS_PER_LONGLONG;
	unsigned int bit_off = _bit_off;
	uint64_t val;

	if (bit_off >= sz) {
		ret = -1;
	}

	if (ret == 0) {
		bit_off &= (BITS_PER_LONGLONG - 1U);
		do {
			val = vatomic64_read_acq(&bitmap[long_off]);
			if (get_bit_val_from_u64(val, bit_off) == 1) {
				ret = -1;
			}
		} while (ret == 0 && vatomic64_cmpxchg_rel(&bitmap[long_off], val, (val | (1ULL << bit_off))) != val);
	}

	return ret;
}

static inline int atomic_bitmap_clear_bit(vatomic64_t *bitmap, unsigned int sz, unsigned int _bit_off)
{
	int ret = 0;
	unsigned int long_off = _bit_off / BITS_PER_LONGLONG;
	unsigned int bit_off = _bit_off;
	uint64_t val;

	if (bit_off >= sz) {
		ret = -1;
	}

	if (ret == 0) {
		bit_off &= (BITS_PER_LONGLONG - 1U);
		do {
			val = vatomic64_read_acq(&bitmap[long_off]);
			if (get_bit_val_from_u64(val, bit_off) == 0) {
				ret = -1;
			}
		} while (ret == 0 && vatomic64_cmpxchg_rel(&bitmap[long_off], val, (val & ~(1ULL << bit_off)))
			!= val);
	}

	return ret;
}

static inline uint64_t atomic_bitmap_get_bit(vatomic64_t *bitmap, unsigned int sz, unsigned int _bit_off)
{
	unsigned int long_off = _bit_off / BITS_PER_LONGLONG;
	unsigned int bit_off = _bit_off;
	bit_off &= (BITS_PER_LONGLONG - 1U);
	return get_bit_val_from_u64(vatomic64_read_rlx(&bitmap[long_off]), bit_off);
}

static inline unsigned int atomic_bitmap_find_next_one(vatomic64_t *bitmap, unsigned int sz,
		unsigned int start)
{
	unsigned int long_sz = BITS_TO_LONGLONGS(sz);
	unsigned int off = start / BITS_PER_LONGLONG;
	unsigned int bit_off = start & (BITS_PER_LONGLONG - 1U);
	uint64_t val = 0;
	bool found = false;

	val = vatomic64_read_rlx(&bitmap[off]) & ~((1ULL << bit_off) - 1);
	if (val != 0) {
		bit_off = (unsigned int)__builtin_ffsll(val) - 1;
		found = true;
	}

	if (!found) {
		for (off = off + 1; off < long_sz; off++) {
			val = vatomic64_read_rlx(&bitmap[off]);
			if (val == 0) {
				continue;
			}

			bit_off = (unsigned int)(__builtin_ffsll((long long)val) - 1);
			break;
		}
	}
	bit_off += BITS_PER_LONGLONG * off;
	return bit_off >= sz ? sz : bit_off;
}

#endif
