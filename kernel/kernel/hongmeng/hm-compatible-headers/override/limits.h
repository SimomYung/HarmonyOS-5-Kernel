/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Compatible with GLIBC
 * Author: Huawei OS Kernel Lab
 * Create: Thu Mar 10 19:40:45 2022
 */
#ifndef _OVERRIDE_LIMITS_H
#define _OVERRIDE_LIMITS_H

/*
 * To adapt to GLIBC,
 * for GLIBC doesn't need to define the following macros to use NAME_MAX,
 * but native MUSL need.
 */
#if !(defined(_POSIX_SOURCE) || defined(_POSIX_C_SOURCE) \
	|| defined(_XOPEN_SOURCE) || defined(_GNU_SOURCE) || defined(_BSD_SOURCE))
# ifndef NAME_MAX
#  define NAME_MAX 255
# endif
#endif

#include <muslorigin/limits.h>

#endif
