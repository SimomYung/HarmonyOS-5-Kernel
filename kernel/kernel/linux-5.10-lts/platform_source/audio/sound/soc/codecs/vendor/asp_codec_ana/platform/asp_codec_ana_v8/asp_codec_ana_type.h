/*
 * asp_codec_ana_type.h -- codec driver
 *
 * Copyright (c) 2024 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */

#ifndef __ASP_CODEC_ANA_TYPE_H__
#define __ASP_CODEC_ANA_TYPE_H__

#include "asp_codec_ana_comm.h"

enum virtual_regist_offset {
	VIR_REG0_OFFSET = 0x0,
	VIR_ANA_REG_CNT,
};

/* virtual reg */
#define CODEC_VIR0_REG_ADDR ((PAGE_VIRCODEC_ANA) + (VIR_REG0_OFFSET))
#define ANA_AUL_EN_BIT 0
#define ANA_AUR_EN_BIT 1
#define ANA_MUX3_EN_BIT 2
#define ANA_MUX4_EN_BIT 3
#define ANA_MUX5_EN_BIT 4
#define ANA_AU_ULTR_EN_BIT 5
#define ANA_AU_MAD_EN_BIT 6
#define ANA_DACL_BIT 7
#define ANA_DACR_BIT 8
#define ANA_MAD_BIT 9
#define MIC3_BIAS1_BIT 10
#define MIC3_BIAS2_BIT 11
#define AUXMIC_BIAS1_BIT 12
#define AUXMIC_BIAS2_BIT 13

struct codec_ana_platform_data {
	struct codec_ana_data base;
};

#endif
