/*
 * Copyright (c) 2024-2027 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#ifndef __CLK_COMB_DVFS_H__
#define __CLK_COMB_DVFS_H__

#include <linux/version.h>
#include <linux/clk.h>
#include <linux/hwspinlock.h>
#include "clk.h"
#ifdef CONFIG_PERI_DVFS
#include "dvfs/peri-volt-internal.h"
#endif

#define FREQ_OFFSET_ADD	1000000  /* For 184.444M to 185M */
#define LOW_TEMPERATURE_PROPERTY	1
#define ELOW_TEMPERATURE	0xE558
#define FREQ_CONVERSION	 1000
#define MAX_COMB_DVFS_NUM 4
#define CLOCK_DISABLE_LEVEL 0xFF

enum {
	NORMAL_TEMPRATURE = 0,
	LOW_TEMPERATURE,
};

struct hi3xxx_comb_dvfs_clock {
	struct clk_hw hw;
	const char *link;
	u32 pvp_id;
	u8  dvfs_clk_id;
	u8  clk_vote_id;
	u8 low_temperature_property;
	u32 low_temperature_freq;
};

struct level_freq {
	u32 pvp_id;
	u32 volt;
	u64 sensitive_freq[MAX_CLK_NUM];
};

struct hi3xxx_comb_dvfs {
	u32 id;
	const char *name;
	struct level_freq sensitive_level_freqs[MAX_LEVEL_NUM];
	u8  level_num;
	const struct comb_clock *comb_clks;
	u64 cur_freqs[MAX_CLK_NUM];
	u64 tmp_freq;
	u8  cur_level;
	u8  clk_num;
};

#endif
