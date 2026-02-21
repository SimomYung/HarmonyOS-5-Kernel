/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Compatible with GLIBC
 * Author: Huawei OS Kernel Lab
 * Create: Thu Mar 10 18:02:08 2022
 */
#ifndef _OVERRIDE_STRING_H
#define _OVERRIDE_STRING_H

/*
 * To adapt to GLIBC,
 * for GLIBC doesn't need to define the following macros to use strdup,
 * but native MUSL need.
 */
#if !(defined(_POSIX_SOURCE) || defined(_POSIX_C_SOURCE) \
	|| defined(_XOPEN_SOURCE) || defined(_GNU_SOURCE) \
	|| defined(_BSD_SOURCE))
char *strdup (const char *);
#endif

#include <muslorigin/string.h>

#endif
