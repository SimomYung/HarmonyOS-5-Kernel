/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Export type definition to kernel mapi
 * Author: Huawei OS Kernel Lab
 * Create: Sat Jul 20 15:19:58 2019
 */
#ifndef MAPI_HMKERNEL_TYPES_H
#define MAPI_HMKERNEL_TYPES_H
#ifndef __ASSEMBLY__

#include <uapi/hmkernel/types.h>

#ifndef NULL
#define NULL __NULL
#endif

typedef __bool  bool;

#define true	__true
#define false	__false

static inline uptr_t __u64_to_uintptr(u64 v)
{
	return (uptr_t)(v);
}

#define ptr_to_u64(ptr)			((u64)(uptr_t)(ptr))
#define u64_to_ptr(val, type)		((type *)(__u64_to_uintptr(val)))
#define u64_to_uptr(val)		u64_to_ptr(val, void __user)

#define ptr_to_long(ptr)		((long)(uptr_t)(ptr))
#define ptr_to_ulong(ptr)		((unsigned long)(uptr_t)(ptr))
#define ptr_to_vaddr(ptr)		((vaddr_t)(uptr_t)(ptr))
#define vaddr_to_ptr(vaddr, type)	((type *)(uptr_t)(vaddr))
#define ulong_to_ptr(val, type)		((type *)(uptr_t)(val))

#define ptr_to_void(ptr) 		((void *)(uptr_t)(ptr))
#define ptr_from_void(ptr, type) 	((type *)(uptr_t)(ptr))
#define ptr_cast(ptr, type) 		ptr_from_void(ptr_to_void(ptr), type)

#define u_to_u32(v) ({						\
	u32 __v = 0u;						\
	size_t __s = sizeof(typeof(v));				\
	switch (__s) {						\
	case 8: __v = (u32)((u64)(v) & 0xffffffffULL); break;	\
	case 4: __v = (u32)(v); break;				\
	case 2: __v = (u32)(u16)(v); break;			\
	default: __v = (u32)(u8)(v); break;			\
	} __v;							\
})

#define u_to_u16(v) ({						\
	u16 __v = 0u;						\
	size_t __s = sizeof(typeof(v));				\
	switch (__s) {						\
	case 8: __v = (u16)((u64)(v) & 0xffff); break;		\
	case 4: __v = (u16)((u32)(v) & 0xffff); break;		\
	case 2: __v = (u16)(v); break;				\
	default: __v = (u16)(u8)(v); break;			\
	} __v;							\
})

#ifndef BITS_PER_UINT
#define BITS_PER_UINT	32
#endif

#define INT_MAX        (~0U>>1)
#define UINT_MAX       (~0U)
#define UPTR_MAX       (~0UL)

#ifndef __DEFINED_size_t
#define __DEFINED_size_t
typedef __kernel_size_t size_t;
#endif

#endif /* __ASSEMBLY__ */
#endif
