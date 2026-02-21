/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Compatible with GLIBC
 * Author: Huawei OS Kernel Lab
 * Create: Sat May 07 15:04:16 2022
 */
#ifndef _GLIBC_INC_BITS_TYPESIZES_H
#define _GLIBC_INC_BITS_TYPESIZES_H

/*
 * To adapt to GLIBC,
 * for GLIBC defines the following macros when bits/typesizes.h is included,
 * but native MUSL not.
 */
#if defined __x86_64__ && defined __ILP32__
# define __SYSCALL_SLONG_TYPE	__SQUAD_TYPE
#else
# define __SYSCALL_SLONG_TYPE	__SLONGWORD_TYPE
#endif

#define __OFF64_T_TYPE		__SQUAD_TYPE
#define __TIME_T_TYPE		__SYSCALL_SLONG_TYPE

#endif
