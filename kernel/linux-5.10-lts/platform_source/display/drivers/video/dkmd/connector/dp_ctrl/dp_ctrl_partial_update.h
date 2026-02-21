/**
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#ifndef __DP_CTRL_PARTIAL_UPDATE_H__
#define __DP_CTRL_PARTIAL_UPDATE_H__

#include <linux/types.h>

struct dpu_connector;

int32_t dp_ctrl_partial_update(struct dpu_connector *connector, const void *value);
#endif