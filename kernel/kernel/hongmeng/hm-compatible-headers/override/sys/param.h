/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Compatible with GLIBC
 * Author: Huawei OS Kernel Lab
 * Create: Thu Mar 10 18:02:08 2022
 */
#ifndef _OVERRIDE_SYS_PARAM_H
#define _OVERRIDE_SYS_PARAM_H

#include <muslorigin/sys/param.h>
/*
 * To adapt to GLIBC,
 * for GLIBC defines the following macro when sys/param.h is included,
 * but native MUSL not.
 */
#ifndef MAX_INPUT
#define MAX_INPUT 255
#endif

#endif
