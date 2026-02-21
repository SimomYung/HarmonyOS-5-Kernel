/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Add linux/types.h to compat linux interface
 * Author: Huawei OS Kernel Lab
 * Create: Sat Sep 11 18:10:40 2021
 */

#ifndef _LINUX_TYPES_H_
#define _LINUX_TYPES_H_

#include <asm/types.h>

#ifndef __kernel_uid21_t
typedef unsigned int	__kernel_uid32_t;
#endif

#ifndef __ASSEMBLY__

#include <linux/posix_types.h>

#ifdef __CHECKER__
#define __bitwise__ __attribute__((bitwise))
#else
#define __bitwise__
#endif
#define __bitwise __bitwise__

typedef __u16 __bitwise __le16;
typedef __u16 __bitwise __be16;
typedef __u32 __bitwise __le32;
typedef __u32 __bitwise __be32;
typedef __u64 __bitwise __le64;
typedef __u64 __bitwise __be64;

typedef __u16 __bitwise __sum16;

#define __aligned_be64 __be64 __attribute__((aligned(8)))

#endif
#endif
