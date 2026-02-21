/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Compatible with GLIBC
 * Author: Huawei OS Kernel Lab
 * Create: Sat May 07 15:04:16 2022
 */
#ifndef _OVERRIDE_PTHREAD_H
#define _OVERRIDE_PTHREAD_H

#include <muslorigin/pthread.h>

/*
 * To adapt to GLIBC,
 * for GLIBC defines the following macros when pthread.h is included,
 * but native MUSL not.
 */
#define PTHREAD_MUTEX_RECURSIVE_NP PTHREAD_MUTEX_RECURSIVE

#endif
