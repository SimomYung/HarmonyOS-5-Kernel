/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Description: hibernation for uapi
 * Create: Mon Mar 11 14:46:36 2024
 */

#ifndef UAPI_HMKERNEL_DRIVERS_HIBERNATE_H
#define UAPI_HMKERNEL_DRIVERS_HIBERNATE_H

#include "hibernate_def.h"

#define __HIBERNATE_DIV_ROUND_UP(n, d)		(((n) + (d) - 1) / (d))
#define __HIBERNATE_LAST_EXTENT			(1ULL << 63)
#define __HIBERNATE_INVALID_VALUE		(-1ULL)
#define __HIBERNATE_BITS_PER_UNIT		(sizeof(unsigned long) * 8)
#define __HIBERNATE_BITMAP_MASK			(__HIBERNATE_BITS_PER_UNIT - 1)
#define __HIBERNATE_BITMAP_LEN(bm)		(sizeof(struct __hibernate_memory_bitmap) + \
						 __HIBERNATE_DIV_ROUND_UP((bm)->len, __HIBERNATE_BITS_PER_UNIT) * \
						 sizeof(unsigned long))

static inline __bool __hibernate_is_last_extent(const struct __hibernate_memory_bitmap *bm)
{
	return (bm->pfn & __HIBERNATE_LAST_EXTENT) != 0;
}

static inline __u64 __hibernate_bm_get_pfn(const struct __hibernate_memory_bitmap *bm, __u64 off)
{
	return ((bm->pfn & ~__HIBERNATE_LAST_EXTENT) > off) ?
	       ((bm->pfn & ~__HIBERNATE_LAST_EXTENT) - off - 1U) : __HIBERNATE_INVALID_VALUE;
}

static inline __u64 __hibernate_bm_get_off(const struct __hibernate_memory_bitmap *bm, __paddr_t pfn)
{
	return (((bm->pfn & ~__HIBERNATE_LAST_EXTENT) > pfn) && (pfn >= ((bm->pfn & ~__HIBERNATE_LAST_EXTENT) - bm->len))) ?
	       ((bm->pfn & ~__HIBERNATE_LAST_EXTENT) - pfn - 1U) : __HIBERNATE_INVALID_VALUE;
}

static inline struct __hibernate_memory_bitmap *__hibernate_next_memory_bitmap(struct __hibernate_memory_bitmap *bm)
{
	struct __hibernate_memory_bitmap *next_bm = NULL;
	if (!__hibernate_is_last_extent(bm)) {
		next_bm = ptr_cast(ptr_add(bm, __HIBERNATE_BITMAP_LEN(bm)), struct __hibernate_memory_bitmap);
	}

	return next_bm;
}

#endif
