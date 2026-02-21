/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Sat Jul 20 15:19:58 2019
 */
#ifndef AARCH64_MAPI_ASM_TYPES_H
#define AARCH64_MAPI_ASM_TYPES_H

#include <uapi/hmasm/types.h>

#ifndef __ASSEMBLY__

typedef signed char s8;
typedef unsigned char u8;

typedef signed short s16;
typedef unsigned short u16;

typedef int s32;
typedef unsigned int u32;

typedef long long s64;
typedef unsigned long long u64;

#define PRIx64 __PRIx64
#define PRIx32 __PRIx32
#define PRIu64 __PRIu64
#define PRIu32 __PRIu32

#if defined(__LP64__) || defined(__ILP32__)
/*
 * Don't need this in uapi: user app can be compiled
 * with other data model
 */
typedef unsigned long uptr_t;
typedef signed long ptr_t;
#else
# error Only support LP64 and ILP32
#endif

#endif

#if defined(__LP64__)
# define BITS_PER_LONG		64
# define BITS_PER_LONG_LONG	64
#elif defined(__ILP32__)
# define BITS_PER_LONG		32
# define BITS_PER_LONG_LONG	64
#else
# error Only support LP64
#endif

#endif
