/*
 * Copyright (c) 2024-2025 Huawei Technologies Co., Ltd.
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
#ifndef __CLK_VOTE_XPU_H_
#define __CLK_VOTE_XPU_H_

#include "clk.h"
#include <linux/clk.h>

#define WIDTH_MASK(width) (BIT(width) - 1)
#define FREQ_HZ_CONV_MHZ  (1000000UL)
#define FREQ_CONVERSION	  (1000UL)
#define HZ_TO_MHZ(hz)  ((hz) / FREQ_HZ_CONV_MHZ)
#define MHZ_TO_HZ(mhz)  ((mhz) * FREQ_HZ_CONV_MHZ)
#define XPU_VOTE_TIMEOUT  30000 // 30ms

#define LOW_TEMPERATURE_CFG_NUM  3

struct vote_xpu_driver_cfg {
	void __iomem *pmc_base_addr;
	u32 temperature_flag_addr;
	u32 temperature_flag_mask;
	u32 normal_temperature_val;
};

struct hi3xxx_freq_vote_reg {
	u32 addr;
	u8  mask_bit;
	u8  freq_shift;
	u32 freq_mask;
};

struct hi3xxx_freq_intr_stat {
	u32 addr;
	u8 stat_bit;
};

struct hi3xxx_freq_stat_reg {
	u32 addr;
	u8  freq_shift;
	u8  err_code_shift;
	u32 freq_mask;
	u32 err_code_mask;
};

struct hi3xxx_vote_xpu_clk {
	struct clk_hw hw;
	void __iomem *base_addr;
	u32 xpu_clk_id;
	struct hi3xxx_freq_vote_reg vote_reg;
	struct hi3xxx_freq_stat_reg stat_reg;
	u32 result_freq_reg;  /* Voting results of all masters */
	struct hi3xxx_freq_intr_stat intr_reg; /* freq vote intr stat */
	u32 low_temperature_freq;
	bool low_temperature_property;
	bool always_on;
	u32 cur_freq;
};

#endif
