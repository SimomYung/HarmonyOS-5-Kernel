/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Exception
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jun 17 17:07:55 2022
 */

#ifndef UAPI_HMKERNEL_EXCEPTION_H
#define UAPI_HMKERNEL_EXCEPTION_H

#include <hmkernel/types.h>

/* dump range regster exception */
#define DUMP_MIN_NUM_AROUD_REG                 (-512L)
#define DUMP_MAX_NUM_AROUD_REG                 (1024L)
#define DUMP_DEFAULT_NUM_LEFT_AROUD_REG        (-5L)
#define DUMP_DEFAULT_NUM_RIGHT_AROUD_REG       (7L)

#endif