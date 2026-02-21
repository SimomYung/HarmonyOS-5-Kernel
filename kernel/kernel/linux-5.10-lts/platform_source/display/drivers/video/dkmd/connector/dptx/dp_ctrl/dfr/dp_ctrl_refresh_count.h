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

#ifndef __DP_CTRL_REFRESH_COUNT_H__
#define __DP_CTRL_REFRESH_COUNT_H__

#include <linux/types.h>

struct dpu_connector;
struct panel_refresh_statistic_info;

int32_t get_dp_refresh_count(uint32_t port_id, struct panel_refresh_statistic_info *refresh_stat_info);
int32_t dp_set_refresh_statistic(struct dpu_connector *connector, const void *value);
int32_t dp_get_stat_clear_flag(struct dpu_connector *connector, const void *value);

#endif

