/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
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

#ifndef DKMD_DUBAI_INTERFACE_H
#define DKMD_DUBAI_INTERFACE_H

#include <linux/types.h>
#define PANEL_PARTITION_NUMS   20
#define MULTI_BLOCK_NUMS       (((PANEL_PARTITION_NUMS + 1) * PANEL_PARTITION_NUMS) / 2)

enum frm_rate_type {
	FRM_RATE_1HZ,
	FRM_RATE_5HZ,
	FRM_RATE_10HZ,
	FRM_RATE_15HZ,
	FRM_RATE_20HZ,
	FRM_RATE_24HZ,
	FRM_RATE_30HZ,
	FRM_RATE_36HZ,
	FRM_RATE_45HZ,
	FRM_RATE_60HZ,
	FRM_RATE_72HZ,
	FRM_RATE_90HZ,
	FRM_RATE_120HZ,
	FRM_RATE_144HZ,
	FRM_RATE_GEARS_NUM
};

struct panel_refresh_statistic_info {
	uint64_t total_count_time_us;
	uint32_t full_dimming_refresh_cnt;
	uint32_t refresh_cnt[FRM_RATE_GEARS_NUM];
	uint32_t block_refresh_cnt[MULTI_BLOCK_NUMS];
};

extern struct dpu_panel_info *get_panel_info(uint32_t panel_id);
extern int32_t get_panel_refresh_count(uint32_t panel_id, struct panel_refresh_statistic_info *refresh_stat_info);

#endif
