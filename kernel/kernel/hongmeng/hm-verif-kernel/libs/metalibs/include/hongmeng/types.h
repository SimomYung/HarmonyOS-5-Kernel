/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Definition of ulibs types
 * Author: Huawei OS Kernel Lab
 * Create: Mon Mar 4 16:46:18 2019
 */
#ifndef ULIBS_HONGMENG_TYPES_H
#define ULIBS_HONGMENG_TYPES_H

#include <stdlib.h>
#include <stdint.h>

static inline uintptr_t __u64_to_uintptr(unsigned long long v)
{
	return (uintptr_t)(v);
}

#define ptr_to_type(ptr, type)	((type)(uintptr_t)(ptr))
#define ptr_to_u64(ptr)		ptr_to_type(ptr, uint64_t)
#define ptr_to_size_t(ptr)	ptr_to_type(ptr, size_t)
#define ptr_to_long(ptr)	ptr_to_type(ptr, long)
#define ptr_to_ulong(ptr)	ptr_to_type(ptr, unsigned long)

#define ptr_to_void(ptr) 		((void *)(uintptr_t)(ptr))
#define ptr_from_void(ptr, type) 	({void *__p = ptr; (type *)(uintptr_t)(__p); })
#define ptr_cast(ptr, type)		ptr_from_void(ptr_to_void(ptr), type)

#define ulong_to_ptr(ulong, type)	({type *__p; __p = (type *)(uintptr_t)(ulong); __p;})
#define size_t_to_ptr(size_t, type)     ({type *__p; __p = (type *)(uintptr_t)(size_t); __p;})
#define void_to_ptr(ptr, type)		({void *__p = ptr; (type *)(uintptr_t)(__p);})
#define u64_to_ptr(val, type)		((type *)__u64_to_uintptr(val))

#define __true (_Bool)(1 == 1)
#define __false (_Bool)(1 != 1)

#define HM_SWAB_BYTE_1_SHIFT	8
#define HM_SWAB_BYTE_3_SHIFT	24

static inline uint32_t u32_to_cpu(uint32_t val)
{
	uint32_t ret = 0U;
#if ((__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__) && defined (__aarch64__))
	ret =	(((uint32_t)(val) & (uint32_t)0x000000ffUL) << HM_SWAB_BYTE_3_SHIFT) |
		(((uint32_t)(val) & (uint32_t)0x0000ff00UL) << HM_SWAB_BYTE_1_SHIFT) |
		(((uint32_t)(val) & (uint32_t)0x00ff0000UL) >> HM_SWAB_BYTE_1_SHIFT) |
		(((uint32_t)(val) & (uint32_t)0xff000000UL) >> HM_SWAB_BYTE_3_SHIFT);
#else
	ret = val;
#endif
	return ret;
}
#endif
