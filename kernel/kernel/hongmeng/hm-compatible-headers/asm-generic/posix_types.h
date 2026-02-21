/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Add asm-generic/posix_types.h to compat linux interface
 * Author: Huawei OS Kernel Lab
 * Create: Wed Feb 09 15:59:54 2022
 */

#ifndef __ASM_GENERIC_POSIX_TYPES_H
#define __ASM_GENERIC_POSIX_TYPES_H

#ifndef __kernel_long_t
typedef long		__kernel_long_t;
typedef unsigned long	__kernel_ulong_t;
#endif

typedef __kernel_long_t	__kernel_time_t;
typedef long long       __kernel_loff_t;

#ifndef __kernel_old_dev_t
typedef unsigned int	__kernel_old_dev_t;
#endif

#endif
