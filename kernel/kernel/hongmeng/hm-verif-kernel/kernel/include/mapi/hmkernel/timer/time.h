/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2022. All rights reserved.
 * Description: Implementation of time
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jul 31 15:21:43 2020
 */

#ifndef MAPI_HMKERNEL_TIME_H
#define MAPI_HMKERNEL_TIME_H

#include <hmkernel/types.h>

/* calculate total tock from the clock source to nsec, it is monotonic time */
extern struct time_mono_t time_get_mono(void);

#endif
