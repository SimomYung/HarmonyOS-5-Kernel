/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Compatible with GLIBC
 * Author: Huawei OS Kernel Lab
 * Create: Thu Mar 10 18:02:08 2022
 */
#ifndef _OVERRIDE_DIRENT_H
#define _OVERRIDE_DIRENT_H

#include <muslorigin/dirent.h>

/*
 * To adapt to GLIBC,
 * for GLIBC defines the following macros when dirent.h is included,
 * but native MUSL not.
 */
#ifndef MAX_INPUT
#define MAX_INPUT 255
#endif

#ifndef NAME_MAX
#define NAME_MAX 255
#endif

#endif
