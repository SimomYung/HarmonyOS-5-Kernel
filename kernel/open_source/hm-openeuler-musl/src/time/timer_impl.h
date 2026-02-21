/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Hidden implementation for timer interfaces
 * Author: Huawei OS Kernel Lab
 * Create: Thu Oct 27 16:01:19 2022
 */
#ifndef _TIMER_IMPL_H
#define _TIMER_IMPL_H

#include <time.h>
#include <limits.h>
#include <string.h>

/* check UAF of timer_id */
hidden int __check_timer_id(timer_t *);

#endif
