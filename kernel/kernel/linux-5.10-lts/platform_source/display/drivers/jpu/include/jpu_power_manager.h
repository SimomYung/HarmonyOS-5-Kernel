/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: jpegd drv.
 * Create: 2023-10-31
 */

#ifndef JPU_POWER_MANAGER_H
#define JPU_POWER_MANAGER_H

#include "jpu.h"

int32_t jpu_on(struct jpu_data_type *jpu_device);
int32_t jpu_off(struct jpu_data_type *jpu_device);
int32_t jpu_pre_off(struct jpu_data_type *jpu_device);
int32_t jpu_aft_on(struct jpu_data_type *jpu_device);
#endif
