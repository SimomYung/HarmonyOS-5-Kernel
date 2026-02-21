/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Compatible with GLIBC
 * Author: Huawei OS Kernel Lab
 * Create: Thu Mar 10 18:02:08 2022
 */
#ifndef _OVERRIDE_FCNTL_H
#define _OVERRIDE_FCNTL_H

#ifndef _GNU_SOURCE
/*
 * To adapt to GLIBC,
 * for GLIBC doesn't need to define _GNU_SOURCE to use off_t,
 * but native MUSL need.
 */
#define loff_t off_t
#endif

#include <muslorigin/fcntl.h>

#define LOCK_EX		2 /* exclusive lock */
#define LOCK_UN		8 /* remove lock */

#endif
