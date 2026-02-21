/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2021. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jun 7 03:10:16 2018
 */
#ifndef AARCH64_UAPI_ASM_TYPES_H
#define AARCH64_UAPI_ASM_TYPES_H

#ifndef __ASSEMBLY__

/* Don't pollute namespace: use __ leading type names */

typedef signed char __s8;
typedef unsigned char __u8;

typedef signed short __s16;
typedef unsigned short __u16;

typedef int __s32;
typedef unsigned int __u32;

typedef long long __s64;
typedef unsigned long long __u64;
typedef unsigned long __uptr_t;
typedef signed long __sptr_t;

#define __ptr_to_u64(ptr) ((__u64)(__uptr_t)(ptr))

#define __PRIx64	"llx"
#define __PRId64	"lld"
#define __PRIu64	"llu"
#define __PRIx32	"x"
#define __PRId32	"d"
#define __PRIu32	"u"

#ifdef __SIZE_TYPE__
typedef __SIZE_TYPE__ __kernel_size_t;
#else
typedef unsigned long __kernel_size_t;
#endif

#endif

#endif
