/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Header file about kconsole kvic
 * Author: Huawei OS Kernel Lab
 * Create: Thu Nov 05 07:20:19 2020
 */

#ifndef TTY_KCONSOLE_KVIC_H
#define TTY_KCONSOLE_KVIC_H

#include "kconsole.h"

int kconsole_init_kvic(struct kconsole_device *kconsole);

void kconsole_uninit_kvic(const struct kconsole_device *kconsole);

#endif
