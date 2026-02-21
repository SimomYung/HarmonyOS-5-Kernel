/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Compatible with GLIBC
 * Author: Huawei OS Kernel Lab
 * Create: Thu Mar 10 18:02:08 2022
 */
#ifndef _OVERRIDE_STDLIB_H
#define _OVERRIDE_STDLIB_H

#include <muslorigin/stdlib.h>

/*
 * To adapt to GLIBC,
 * for GLIBC includes the following header file when stdlib.h is included,
 * but native MUSL not.
 */
#if defined __USE_MISC || defined __USE_XOPEN_EXTENDED
# include <sys/types.h>
#endif

#endif
