/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Fri Oct 25 09:28:44 2019
 */

#ifndef __UDKTEST_SECTIONS_H__
#define __UDKTEST_SECTIONS_H__

int udktest__device(void);
int udktest__driver(void);
int udktest__shmlock(void *ctx, int cmd, unsigned long arg);
int udktest__mm(void);
int udktest__mempool(void);
int udktest__thread(void);

#include "udktest__fops.h"

#endif /* __UDKTEST_SECTIONS_H__ */
