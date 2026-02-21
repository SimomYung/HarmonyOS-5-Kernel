/*
 * asp_codec_type.h -- codec driver
 *
 * Copyright (c) 2023 Huawei Technologies Co., Ltd.
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

#ifndef __ASP_CODEC_TYPE_H__
#define __ASP_CODEC_TYPE_H__

#include "asp_codec_reg.h"
#include "asp_codec_reg_adapter.h"
#include "asp_codec_comm.h"

enum virtual_reg_offset {
	VIR0_REG_OFFSET = 0x0,
	VIR1_REG_OFFSET,
	VIR2_REG_OFFSET,
	VIR3_REG_OFFSET,
	VIR_REG_CNT,
};

/* virtual reg */
#define CODEC_VIR0_REG_ADDR ((PAGE_VIRCODEC) + (VIR0_REG_OFFSET))
#define AUDIO_UP_O_BIT 0
#define VOICE_UP_O_BIT 1
#define MIC34_UP_O_BIT 2
#define MIC56_UP_O_BIT 3
#define MIC78_UP_O_BIT 4
#define AUDIO_UP_2MIC_48K_BIT 5
#define VOICE_UP_2MIC_48K_BIT 6
#define VOICE_UP_2MIC_32K_BIT 7
#define AUDIO_UP_4MIC_48K_BIT 8
#define VOICE_UP_4MIC_48K_BIT 9
#define VOICE_UP_4MIC_32K_BIT 10
#define AUDIO_UP_6MIC_48K_BIT 11
#define BT_VOICE_UP_32K_BIT   12
#define BT_AUDIO_UP_48K_BIT   13
#define MDM_5G_L_UP_PGA_GAIN_BIT 14
#define MDM_5G_L_UP_PGA_BYPASS_BIT 15
#define MDM_5G_R_UP_PGA_GAIN_BIT 16
#define MDM_5G_R_UP_PGA_BYPASS_BIT 17
#define MDM_5G_UP_L_DIN_SEL_BIT 18
#define MDM_5G_UP_R_DIN_SEL_BIT 19
#define ADC1_CIC_6STAGE_AU_SEL_BIT 20
#define ADC2_CIC_6STAGE_AU_SEL_BIT 21
#define ADC3_CIC_6STAGE_AU_SEL_BIT 22
#define ADC4_CIC_6STAGE_AU_SEL_BIT 23
#define AU0_ADCL_EN_BIT 24
#define AU0_ADCR_EN_BIT 25
#define MDM_5G_L_UP_SRCDN_SRC_MODE_BIT 26
#define MDM_5G_R_UP_SRCDN_SRC_MODE_BIT 27
#define FS_MDM_5G_L_SRCDN_DOUT_BIT 28
#define FS_MDM_5G_R_SRCDN_DOUT_BIT 29
#define FS_MDM_5G_L_UP_PGA_BIT 30
#define FS_MDM_5G_R_UP_PGA_BIT 31

#define CODEC_VIR1_REG_ADDR   ((PAGE_VIRCODEC) + (VIR1_REG_OFFSET))
#define AUDIO_DN_4PA_IN_BIT 0
#define A2DP_OFFLOAD_BIT 1
#define ULTRASONIC_DN_BIT 2
#define ULTRASONIC_UP_BIT 3
#define EC_2PA_BIT 4
#define EC_4PA_BIT 5
#define AUDIO_DN_2PA_IN_BIT 6
#define S1_TX_TDM_BIT 9
#define S1_TX_I2S_BIT  10
#define IV_2PA_BIT 11
#define IV_4PA_BIT 12
#define CARKIT_BIT 13
#define S1_TX_TDM256_BIT 14
#define MAD_LP_SINGLE_MIC_BIT 16
#define MAD_NORM_SINGLE_MIC_BIT 18
#define S2_TX_L_BIT 20
#define S2_TX_R_BIT 21
#define S2_RX_L_BIT 22
#define S2_RX_R_BIT 23
#define FS_MDM_5G_L_UP_AFIFO_BIT 24
#define FS_MDM_5G_R_UP_AFIFO_BIT 25
#define MDM_5G_R_UP_AFIFO_CLKEN_BIT 26
#define MDM_5G_R_UP_PGA_CLKEN_BIT 27
#define SIDETONE_SWITCH_CLKEN_BIT 29

#define CODEC_VIR2_REG_ADDR   ((PAGE_VIRCODEC) + (VIR2_REG_OFFSET))
#define S2_TX_L_SRCDN_BIT 1
#define S2_TX_R_SRCDN_BIT 2
#define S2_RX_L_SRC_BIT 3
#define S2_RX_R_SRC_BIT 4
#define S2_PLL_TRK_BIT 5
#define S2_TX_BIT 6
#define S2_ULTRA_BIT 7
#define S2_ULTRA_DN_48K_BIT 8
#define AUDIO_AXI2TDM_INTF1_BIT 9
#define AUDIO_AXI2TDM_INTF2_BIT 10
#define AUDIO_AXI2TDM_INTF3_BIT 11
#define WAKEUP_MAIN_MIC_SWITCH_BIT 12
#define MDM5G_UP_48K_SWITCH_BIT 13
#define VOICE_UP_4MIC_8K_BIT 14
#define I2S2_BLUETOOTH_LOOP_BIT 15
#define SPK_2PA_BYPASS_BIT 16
#define RCV_2PA_BYPASS_BIT 17
#define IV_2PA_SLOT_BIT 18
#define IV_4PA_SLOT_BIT 19
#define SPK_4PA_BYPASS_BIT 20
#define RCV_4PA_BYPASS_BIT 21
#define SPA0_DOUT_BIT 22
#define SPA1_DOUT_BIT 23
#define SPA2_DOUT_BIT 24
#define SPA3_DOUT_BIT 25
#define BT_SRCUP_8k 26
#define BT_SRCUP_16k 27
#define BT_SRCUP_32k 28
#define CODEC_DN_3PA_IN_BIT 29
#define SPK_PA_BYPASS_BIT 30
#define RCV_PA_BYPASS_BIT 31

#define CODEC_VIR3_REG_ADDR   ((PAGE_VIRCODEC) + (VIR3_REG_OFFSET))
#define SPA_PROTECT_WAY_CLK_EN_BIT 0
#define SPA0_BYPASS_BIT 1
#define SPA1_BYPASS_BIT 2
#define SPA2_BYPASS_BIT 3
#define SPA3_BYPASS_BIT 4
#define SPA1_PRE_PROCESS_CG_EN_BIT 9
#define SPA2_PRE_PROCESS_CG_EN_BIT 10
#define SPA3_PRE_PROCESS_CG_EN_BIT 11
#define SPA4_PRE_PROCESS_CG_EN_BIT 12

struct codec_platform_data {
	struct codec_data base;
	struct clk *sif_clk;
};

#endif

