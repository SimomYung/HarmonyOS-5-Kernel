/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Basic data types for UDK
 * Author: Huawei OS Kernel Lab
 * Create: Wed Aug 14 19:00:09 2019
 */
#ifndef	__UDK_TYPES_H__
#define	__UDK_TYPES_H__

/* Use SDK */
#include <sys/types.h>
#include <stdint.h>
#include <hmasm/types.h>
#if defined(MODULE) || defined(UDK_MODULE)
/* Types used by the driver */
#include <stdbool.h>
typedef __u8	u8;
typedef __u16	u16;
typedef __u32	u32;
typedef __u64	u64;
typedef __s8	s8;
typedef __s16	s16;
typedef __s32	s32;
typedef __s64	s64;
#endif

#endif /* __UDK_TYPES_H__ */
