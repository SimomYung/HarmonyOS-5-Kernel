/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Compatible with GLIBC
 * Author: Huawei OS Kernel Lab
 * Create: Sat May 07 15:04:16 2022
 */
#ifndef _OVERRIDE_SIGNAL_H
#define _OVERRIDE_SIGNAL_H

#include <muslorigin/signal.h>

/*
 * To adapt to GLIBC,
 * for GLIBC defines the following macros when signal.h is included,
 * but native MUSL not.
 */
typedef union sigval sigval_t;
typedef void (*__sighandler_t) (int);

#endif
