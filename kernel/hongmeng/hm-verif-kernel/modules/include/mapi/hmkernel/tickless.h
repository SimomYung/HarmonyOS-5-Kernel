/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Header file of tickless
 * Author: Huawei OS Kernel Lab
 * Create: Thu Sep 17 19:54:15 2020
 */

#ifndef MAPI_HMKERNEL_TICKLESS_H
#define MAPI_HMKERNEL_TICKLESS_H

#include <hmkernel/timer/vtimer.h>

extern int tick_suspend(unsigned int cpu);
extern int tick_resume(unsigned int cpu);

#endif
