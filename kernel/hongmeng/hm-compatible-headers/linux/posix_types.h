/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Add linux/posix_types.h to compat linux interface
 * Author: Huawei OS Kernel Lab
 * Create: Wed Nov 17 11:31:42 2021
 */

#ifndef _LINUX_POSIX_TYPES_H
#define _LINUX_POSIX_TYPES_H

#include <asm/posix_types.h>

#ifndef __kernel_pid_t
typedef int		__kernel_pid_t;
#endif

#endif
