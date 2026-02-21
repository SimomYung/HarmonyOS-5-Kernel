/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2024. All rights reserved.
 * Description: dubai display stats
 */

#ifndef DUBAI_DISPLAY_STATS_H
#define DUBAI_DISPLAY_STATS_H

#include <chipset_common/dubai/dubai_display.h>

#ifdef CONFIG_POWER_DUBAI_LTPO_REFRESH_RATE
#include "platform_include/display/dkmd/dkmd_dubai_interface.h"
#endif

long dubai_ioctl_display(unsigned int cmd, void __user *argp);
void dubai_display_stats_init(void);

int dubai_display_register_ops(struct dubai_display_hist_ops *op);
int dubai_display_unregister_ops(void);

enum {
	RATE_1HZ = 0,
	RATE_5HZ,
	RATE_10HZ,
	RATE_15HZ,
	RATE_20HZ,
	RATE_24HZ,
	RATE_30HZ,
	RATE_36HZ,
	RATE_45HZ,
	RATE_60HZ,
	RATE_72HZ,
	RATE_90HZ,
	RATE_120HZ,
	RATE_144HZ,
};

struct refresh_rate_data {
	uint32_t count_1hz;
	uint32_t count_5hz;
	uint32_t count_10hz;
	uint32_t count_15hz;
	uint32_t count_20hz;
	uint32_t count_24hz;
	uint32_t count_30hz;
	uint32_t count_36hz;
	uint32_t count_45hz;
	uint32_t count_60hz;
	uint32_t count_72hz;
	uint32_t count_90hz;
	uint32_t count_120hz;
	uint32_t count_144hz;
} __packed;

struct refresh_rate_data_packing {
	uint32_t panel_id;
	uint64_t total_count_time_us;
	uint32_t full_dimming_refresh_count;
	struct refresh_rate_data refresh_count;
#ifdef CONFIG_POWER_DUBAI_LTPO_REFRESH_RATE
	uint32_t block_refresh_cnt[MULTI_BLOCK_NUMS];
#endif
} __packed;

uint64_t calculate_hist_delta(uint64_t a, uint64_t b);
#endif // DUBAI_DISPLAY_STATS_H
