/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Compatible with GLIBC
 * Author: Huawei OS Kernel Lab
 * Create: Sat May 07 15:04:16 2022
 */
#ifndef _GLIBC_INC_BITS_TYPES_H
#define _GLIBC_INC_BITS_TYPES_H

/*
 * To adapt to GLIBC,
 * for GLIBC defines the following macros when bits/types.h is included,
 * but native MUSL not.
 */
#include <features.h>
#include <bits/reg.h>

#if __WORDSIZE == 64
typedef long int __quad_t;
#else
__extension__ typedef long long int __quad_t;
#endif

#define __SLONGWORD_TYPE	long int

#if __WORDSIZE == 32
# define __SQUAD_TYPE		__quad_t
# define __STD_TYPE		__extension__ typedef
#elif __WORDSIZE == 64
# define __SQUAD_TYPE		long int
# define __STD_TYPE		typedef
#else
# error
#endif

#include <bits/typesizes.h>

__STD_TYPE __OFF64_T_TYPE __off64_t;
__STD_TYPE __TIME_T_TYPE __time_t;

#endif
