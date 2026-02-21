/*
 * asp_codec_play_path_with_pa_alg.c -- codec driver
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

#include <sound/soc.h>
#include "audio_log.h"
#include "asp_codec_type.h"
#include "asp_codec_store.h"

#define LOG_TAG "asp_codec_play_path_with_pa_alg"

#define S1_I2S_CFG_KCONTROLS \
	SOC_SINGLE("FS_S1_IF_I2S", \
		I2S1_CTRL_REG, FS_I2S1_OFFSET, \
		max_val_on_bit(FS_I2S1_LEN), 0), \
	SOC_SINGLE("I2S1_FUNC_MODE", \
		I2S1_CTRL_REG, I2S1_FUNC_MODE_OFFSET, \
		max_val_on_bit(I2S1_FUNC_MODE_LEN), 0), \
	SOC_SINGLE("I2S1_I2S_DIRECT_MODE", \
		I2S1_CTRL_REG, I2S1_DIRECT_LOOP_OFFSET, \
		max_val_on_bit(I2S1_DIRECT_LOOP_LEN), 0), \
	SOC_SINGLE("I2S1_FRAME_MODE", \
		I2S1_CTRL_REG, I2S1_FRAME_MODE_OFFSET, \
		max_val_on_bit(I2S1_FRAME_MODE_LEN), 0), \
	SOC_SINGLE("I2S1_RX_CLK_SEL", \
		I2S1_CTRL_REG, I2S1_RX_CLK_SEL_OFFSET, \
		max_val_on_bit(I2S1_RX_CLK_SEL_LEN), 0), \
	SOC_SINGLE("I2S1_TX_CLK_SEL", \
		I2S1_CTRL_REG, I2S1_TX_CLK_SEL_OFFSET, \
		max_val_on_bit(I2S1_TX_CLK_SEL_LEN), 0) \

#define S1_TDM_CFG_KCONTROLS \
	SOC_SINGLE("I2S1_TDM_TX_SLOT_SEL_I0", \
		I2S1_TDM_CTRL0_REG, S1_TDM_RX_SLOT_SEL_I0_OFFSET, \
		max_val_on_bit(S1_TDM_RX_SLOT_SEL_I0_LEN), 0), \
	SOC_SINGLE("I2S1_TDM_TX_SLOT_SEL_V0", \
		I2S1_TDM_CTRL0_REG, S1_TDM_RX_SLOT_SEL_V0_OFFSET, \
		max_val_on_bit(S1_TDM_RX_SLOT_SEL_V0_LEN), 0), \
	SOC_SINGLE("I2S1_TDM_TX_SLOT_SEL_I1", \
		I2S1_TDM_CTRL0_REG, S1_TDM_RX_SLOT_SEL_I1_OFFSET, \
		max_val_on_bit(S1_TDM_RX_SLOT_SEL_I1_LEN), 0), \
	SOC_SINGLE("I2S1_TDM_TX_SLOT_SEL_V1", \
		I2S1_TDM_CTRL0_REG, S1_TDM_RX_SLOT_SEL_V1_OFFSET, \
		max_val_on_bit(S1_TDM_RX_SLOT_SEL_V1_LEN), 0), \
	SOC_SINGLE("I2S1_TDM_TX_SLOT_SEL_I2", \
		I2S1_TDM_CTRL1_REG, S1_TDM_RX_SLOT_SEL_I2_OFFSET, \
		max_val_on_bit(S1_TDM_RX_SLOT_SEL_I2_LEN), 0), \
	SOC_SINGLE("I2S1_TDM_TX_SLOT_SEL_V2", \
		I2S1_TDM_CTRL1_REG, S1_TDM_RX_SLOT_SEL_V2_OFFSET, \
		max_val_on_bit(S1_TDM_RX_SLOT_SEL_V2_LEN), 0), \
	SOC_SINGLE("I2S1_TDM_TX_SLOT_SEL_I3", \
		I2S1_TDM_CTRL1_REG, S1_TDM_RX_SLOT_SEL_I3_OFFSET, \
		max_val_on_bit(S1_TDM_RX_SLOT_SEL_I3_LEN), 0), \
	SOC_SINGLE("I2S1_TDM_TX_SLOT_SEL_V3", \
		I2S1_TDM_CTRL1_REG, S1_TDM_RX_SLOT_SEL_V3_OFFSET, \
		max_val_on_bit(S1_TDM_RX_SLOT_SEL_V3_LEN), 0), \
	SOC_SINGLE("I2S1_TDM_RX_CLK_SEL", \
		I2S1_TDM_CTRL0_REG, S1_TDM_RX_CLK_SEL_OFFSET, \
		max_val_on_bit(S1_TDM_RX_CLK_SEL_LEN), 0), \
	SOC_SINGLE("I2S1_TDM_TX_CLK_SEL", \
		I2S1_TDM_CTRL0_REG, S1_TDM_TX_CLK_SEL_OFFSET, \
		max_val_on_bit(S1_TDM_TX_CLK_SEL_LEN), 0), \
	SOC_SINGLE("I2S1_TDM_DIRECT_MODE", \
		I2S1_TDM_CTRL0_REG, S1_TDM_DIRECT_LOOP_OFFSET, \
		max_val_on_bit(S1_TDM_DIRECT_LOOP_LEN), 0), \
	SOC_SINGLE("I2S1_TDM_FRAME_MODE", \
		I2S1_TDM_CTRL0_REG, S1_TDM_FRAME_MODE_OFFSET, \
		max_val_on_bit(S1_TDM_FRAME_MODE_LEN), 0), \
	SOC_SINGLE("FS_S1_TDM", \
		FS_CTRL6_REG, FS_S1_TDM_OFFSET, \
		max_val_on_bit(FS_S1_TDM_LEN), 0) \

#define MIXER4_GAIN_KCONTROLS \
	SOC_SINGLE("SPA0_MIXER4_S1_GAIN", \
		SPA0_MIXER4_CTRL0_REG, SPA0_MIXER4_GAIN1_OFFSET, \
		max_val_on_bit(SPA0_MIXER4_GAIN1_LEN), 0), \
	SOC_SINGLE("SPA0_MIXER4_S2_GAIN", \
		SPA0_MIXER4_CTRL0_REG, SPA0_MIXER4_GAIN2_OFFSET, \
		max_val_on_bit(SPA0_MIXER4_GAIN2_LEN), 0), \
	SOC_SINGLE("SPA0_MIXER4_S3_GAIN", \
		SPA0_MIXER4_CTRL0_REG, SPA0_MIXER4_GAIN3_OFFSET, \
		max_val_on_bit(SPA0_MIXER4_GAIN3_LEN), 0), \
	SOC_SINGLE("SPA0_MIXER4_S4_GAIN", \
		SPA0_MIXER4_CTRL0_REG, SPA0_MIXER4_GAIN4_OFFSET, \
		max_val_on_bit(SPA0_MIXER4_GAIN4_LEN), 0), \
	SOC_SINGLE("SPA1_MIXER4_S1_GAIN", \
		SPA1_MIXER4_CTRL0_REG, SPA1_MIXER4_GAIN1_OFFSET, \
		max_val_on_bit(SPA1_MIXER4_GAIN1_LEN), 0), \
	SOC_SINGLE("SPA1_MIXER4_S2_GAIN", \
		SPA1_MIXER4_CTRL0_REG, SPA1_MIXER4_GAIN2_OFFSET, \
		max_val_on_bit(SPA1_MIXER4_GAIN2_LEN), 0), \
	SOC_SINGLE("SPA1_MIXER4_S3_GAIN", \
		SPA1_MIXER4_CTRL0_REG, SPA1_MIXER4_GAIN3_OFFSET, \
		max_val_on_bit(SPA1_MIXER4_GAIN3_LEN), 0), \
	SOC_SINGLE("SPA1_MIXER4_S4_GAIN", \
		SPA1_MIXER4_CTRL0_REG, SPA1_MIXER4_GAIN4_OFFSET, \
		max_val_on_bit(SPA1_MIXER4_GAIN4_LEN), 0), \
	SOC_SINGLE("SPA2_MIXER4_S1_GAIN", \
		SPA2_MIXER4_CTRL0_REG, SPA2_MIXER4_GAIN1_OFFSET, \
		max_val_on_bit(SPA2_MIXER4_GAIN1_LEN), 0), \
	SOC_SINGLE("SPA2_MIXER4_S2_GAIN", \
		SPA2_MIXER4_CTRL0_REG, SPA2_MIXER4_GAIN2_OFFSET, \
		max_val_on_bit(SPA2_MIXER4_GAIN2_LEN), 0), \
	SOC_SINGLE("SPA2_MIXER4_S3_GAIN", \
		SPA2_MIXER4_CTRL0_REG, SPA2_MIXER4_GAIN3_OFFSET, \
		max_val_on_bit(SPA2_MIXER4_GAIN3_LEN), 0), \
	SOC_SINGLE("SPA2_MIXER4_S4_GAIN", \
		SPA2_MIXER4_CTRL0_REG, SPA2_MIXER4_GAIN4_OFFSET, \
		max_val_on_bit(SPA2_MIXER4_GAIN4_LEN), 0), \
	SOC_SINGLE("SPA3_MIXER4_S1_GAIN", \
		SPA3_MIXER4_CTRL0_REG, SPA3_MIXER4_GAIN1_OFFSET, \
		max_val_on_bit(SPA3_MIXER4_GAIN1_LEN), 0), \
	SOC_SINGLE("SPA3_MIXER4_S2_GAIN", \
		SPA3_MIXER4_CTRL0_REG, SPA3_MIXER4_GAIN2_OFFSET, \
		max_val_on_bit(SPA3_MIXER4_GAIN2_LEN), 0), \
	SOC_SINGLE("SPA3_MIXER4_S3_GAIN", \
		SPA3_MIXER4_CTRL0_REG, SPA3_MIXER4_GAIN3_OFFSET, \
		max_val_on_bit(SPA3_MIXER4_GAIN3_LEN), 0), \
	SOC_SINGLE("SPA3_MIXER4_S4_GAIN", \
		SPA3_MIXER4_CTRL0_REG, SPA3_MIXER4_GAIN4_OFFSET, \
		max_val_on_bit(SPA3_MIXER4_GAIN4_LEN), 0) \

#define MIXER4_FS_KCONTROLS \
	SOC_SINGLE("FS_SPA0_MIXER4", \
		FS_CTRL6_REG, FS_SPA0_MIXER4_OFFSET, \
		max_val_on_bit(FS_SPA0_MIXER4_LEN), 0), \
	SOC_SINGLE("FS_SPA1_MIXER4", \
		FS_CTRL6_REG, FS_SPA1_MIXER4_OFFSET, \
		max_val_on_bit(FS_SPA1_MIXER4_LEN), 0), \
	SOC_SINGLE("FS_SPA2_MIXER4", \
		FS_CTRL6_REG, FS_SPA2_MIXER4_OFFSET, \
		max_val_on_bit(FS_SPA2_MIXER4_LEN), 0), \
	SOC_SINGLE("FS_SPA3_MIXER4", \
		FS_CTRL8_REG, FS_SPA3_MIXER4_OFFSET, \
		max_val_on_bit(FS_SPA3_MIXER4_LEN), 0) \

#define MIXER2_ULTR_GAIN_KCONTROLS \
	SOC_SINGLE("ULTR0_MIXER2_S1_GAIN", \
		ULTR0_MIXER2_CTRL_REG, ULTR0_MIXER2_GAIN1_OFFSET, \
		max_val_on_bit(ULTR0_MIXER2_GAIN1_LEN), 0), \
	SOC_SINGLE("ULTR0_MIXER2_S2_GAIN", \
		ULTR0_MIXER2_CTRL_REG, ULTR0_MIXER2_GAIN2_OFFSET, \
		max_val_on_bit(ULTR0_MIXER2_GAIN2_LEN), 0), \
	SOC_SINGLE("ULTR1_MIXER2_S1_GAIN", \
		ULTR1_MIXER2_CTRL_REG, ULTR1_MIXER2_GAIN1_OFFSET, \
		max_val_on_bit(ULTR1_MIXER2_GAIN1_LEN), 0), \
	SOC_SINGLE("ULTR1_MIXER2_S2_GAIN", \
		ULTR1_MIXER2_CTRL_REG, ULTR1_MIXER2_GAIN2_OFFSET, \
		max_val_on_bit(ULTR1_MIXER2_GAIN2_LEN), 0), \
	SOC_SINGLE("ULTR2_MIXER2_S1_GAIN", \
		ULTR2_MIXER2_CTRL_REG, ULTR2_MIXER2_GAIN1_OFFSET, \
		max_val_on_bit(ULTR2_MIXER2_GAIN1_LEN), 0), \
	SOC_SINGLE("ULTR2_MIXER2_S2_GAIN", \
		ULTR2_MIXER2_CTRL_REG, ULTR2_MIXER2_GAIN2_OFFSET, \
		max_val_on_bit(ULTR2_MIXER2_GAIN2_LEN), 0), \
	SOC_SINGLE("ULTR3_MIXER2_S1_GAIN", \
		ULTR3_MIXER2_CTRL_REG, ULTR3_MIXER2_GAIN1_OFFSET, \
		max_val_on_bit(ULTR3_MIXER2_GAIN1_LEN), 0), \
	SOC_SINGLE("ULTR3_MIXER2_S2_GAIN", \
		ULTR3_MIXER2_CTRL_REG, ULTR3_MIXER2_GAIN2_OFFSET, \
		max_val_on_bit(ULTR3_MIXER2_GAIN2_LEN), 0) \

#define FS_DN_FIFO_KCONTROLS \
	SOC_SINGLE("FS_SPA_1_UP_AFIFO", \
		FS_CTRL0_REG, FS_SPA1_UP_AFIFO_OFFSET, \
		max_val_on_bit(FS_SPA1_UP_AFIFO_LEN), 0), \
	SOC_SINGLE("FS_SPA_2_UP_AFIFO", \
		FS_CTRL0_REG, FS_SPA2_UP_AFIFO_OFFSET, \
		max_val_on_bit(FS_SPA2_UP_AFIFO_LEN), 0), \
	SOC_SINGLE("FS_SPA_3_UP_AFIFO", \
		FS_CTRL0_REG, FS_SPA3_UP_AFIFO_OFFSET, \
		max_val_on_bit(FS_SPA3_UP_AFIFO_LEN), 0), \
	SOC_SINGLE("FS_SPA_4_UP_AFIFO", \
		FS_CTRL0_REG, FS_SPA4_UP_AFIFO_OFFSET, \
		max_val_on_bit(FS_SPA4_UP_AFIFO_LEN), 0), \
	SOC_SINGLE("FS_AUDIO_0_DN_AFIFO", \
		FS_AUDIO_0_DN_AFIFO_REG, FS_AUDIO_0_DN_AFIFO_OFFSET, \
		max_val_on_bit(FS_AUDIO_0_DN_AFIFO_LEN), 0), \
	SOC_SINGLE("FS_AUDIO_1_DN_AFIFO", \
		FS_AUDIO_1_DN_AFIFO_REG, FS_AUDIO_1_DN_AFIFO_OFFSET, \
		max_val_on_bit(FS_AUDIO_1_DN_AFIFO_LEN), 0), \
	SOC_SINGLE("FS_AUDIO_2_DN_AFIFO", \
		FS_CTRL0_REG, FS_AUDIO_2_DN_AFIFO_OFFSET, \
		max_val_on_bit(FS_AUDIO_2_DN_AFIFO_LEN), 0), \
	SOC_SINGLE("FS_AUDIO_3_DN_AFIFO", \
		FS_AUDIO_3_DN_AFIFO_REG, FS_AUDIO_3_DN_AFIFO_OFFSET, \
		max_val_on_bit(FS_AUDIO_3_DN_AFIFO_LEN), 0), \
	SOC_SINGLE("FS_CODEC_0_DN_AFIFO", \
		FS_CTRL0_REG, FS_CODEC_0_DN_AFIFO_OFFSET, \
		max_val_on_bit(FS_CODEC_0_DN_AFIFO_LEN), 0), \
	SOC_SINGLE("FS_CODEC_1_DN_AFIFO", \
		FS_CTRL0_REG, FS_CODEC_1_DN_AFIFO_OFFSET, \
		max_val_on_bit(FS_CODEC_1_DN_AFIFO_LEN), 0), \
	SOC_SINGLE("FS_CODEC_2_DN_AFIFO", \
		FS_CTRL0_REG, FS_CODEC_2_DN_AFIFO_OFFSET, \
		max_val_on_bit(FS_CODEC_2_DN_AFIFO_LEN), 0), \
	SOC_SINGLE("FS_CODEC_3_DN_AFIFO", \
		FS_CODEC_3_DN_AFIFO_REG, FS_CODEC_3_DN_AFIFO_OFFSET, \
		max_val_on_bit(FS_CODEC_3_DN_AFIFO_LEN), 0), \
	SOC_SINGLE("FS_BT_L_DN_AFIFO", \
		FS_CTRL0_REG, FS_BT_L_DN_AFIFO_OFFSET, \
		max_val_on_bit(FS_BT_L_DN_AFIFO_LEN), 0), \
	SOC_SINGLE("FS_BT_R_DN_AFIFO", \
		FS_CTRL0_REG, FS_BT_R_DN_AFIFO_OFFSET, \
		max_val_on_bit(FS_BT_R_DN_AFIFO_LEN), 0), \
	SOC_SINGLE("FS_ULTR_UP_AFIFO", \
		FS_ULTR_UP_AFIFO_REG, FS_ULTR_UP_AFIFO_OFFSET, \
		max_val_on_bit(FS_ULTR_UP_AFIFO_LEN), 0) \

#define FS_DN_PGA_KCONTROLS \
	SOC_SINGLE("FS_AUDIO_DN_0_PGA", \
		FS_CTRL2_REG, FS_AUDIO_0_DN_PGA_OFFSET, \
		max_val_on_bit(FS_AUDIO_0_DN_PGA_LEN), 0), \
	SOC_SINGLE("FS_AUDIO_DN_1_PGA", \
		FS_CTRL2_REG, FS_AUDIO_1_DN_PGA_OFFSET, \
		max_val_on_bit(FS_AUDIO_1_DN_PGA_LEN), 0), \
	SOC_SINGLE("FS_AUDIO_DN_2_PGA", \
		FS_CTRL2_REG, FS_AUDIO_2_DN_PGA_OFFSET, \
		max_val_on_bit(FS_AUDIO_2_DN_PGA_LEN), 0), \
	SOC_SINGLE("FS_AUDIO_DN_3_PGA", \
		FS_CTRL8_REG, FS_AUDIO_3_DN_PGA_OFFSET, \
		max_val_on_bit(FS_AUDIO_3_DN_PGA_LEN), 0), \
	SOC_SINGLE("FS_CODEC_DN_0_PGA", \
		FS_CTRL2_REG, FS_CODEC_0_DN_PGA_OFFSET, \
		max_val_on_bit(FS_CODEC_0_DN_PGA_LEN), 0), \
	SOC_SINGLE("FS_CODEC_DN_1_PGA", \
		FS_CTRL2_REG, FS_CODEC_1_DN_PGA_OFFSET, \
		max_val_on_bit(FS_CODEC_1_DN_PGA_LEN), 0), \
	SOC_SINGLE("FS_CODEC_DN_2_PGA", \
		FS_CTRL2_REG, FS_CODEC_2_DN_PGA_OFFSET, \
		max_val_on_bit(FS_CODEC_2_DN_PGA_LEN), 0), \
	SOC_SINGLE("FS_CODEC_DN_3_PGA", \
		FS_CTRL8_REG, FS_CODEC_3_DN_PGA_OFFSET, \
		max_val_on_bit(FS_CODEC_3_DN_PGA_LEN), 0) \

#define DN_PGA_BYPASS_KCONTROLS \
	SOC_SINGLE("AUDIO_DN_0_PGA_BYPASS", \
		AUDIO_0_DN_PGA_CTRL_REG, AUDIO_0_DN_PGA_BYPASS_OFFSET, \
		max_val_on_bit(AUDIO_0_DN_PGA_BYPASS_LEN), 0), \
	SOC_SINGLE("AUDIO_DN_1_PGA_BYPASS", \
		AUDIO_1_DN_PGA_CTRL_REG, AUDIO_1_DN_PGA_BYPASS_OFFSET, \
		max_val_on_bit(AUDIO_1_DN_PGA_BYPASS_LEN), 0), \
	SOC_SINGLE("AUDIO_DN_2_PGA_BYPASS", \
		AUDIO_2_DN_PGA_CTRL_REG, AUDIO_2_DN_PGA_BYPASS_OFFSET, \
		max_val_on_bit(AUDIO_2_DN_PGA_BYPASS_LEN), 0), \
	SOC_SINGLE("AUDIO_DN_3_PGA_BYPASS", \
		AUDIO_3_DN_PGA_CTRL_REG, AUDIO_3_DN_PGA_BYPASS_OFFSET, \
		max_val_on_bit(AUDIO_3_DN_PGA_BYPASS_LEN), 0), \
	SOC_SINGLE("CODEC_DN_0_PGA_BYPASS", \
		CODEC_0_DN_PGA_CTRL_REG, CODEC_0_DN_PGA_BYPASS_OFFSET, \
		max_val_on_bit(CODEC_0_DN_PGA_BYPASS_LEN), 0), \
	SOC_SINGLE("CODEC_DN_1_PGA_BYPASS", \
		CODEC_1_DN_PGA_CTRL_REG, CODEC_1_DN_PGA_BYPASS_OFFSET, \
		max_val_on_bit(CODEC_1_DN_PGA_BYPASS_LEN), 0), \
	SOC_SINGLE("CODEC_DN_2_PGA_BYPASS", \
		CODEC_2_DN_PGA_CTRL_REG, CODEC_2_DN_PGA_BYPASS_OFFSET, \
		max_val_on_bit(CODEC_2_DN_PGA_BYPASS_LEN), 0), \
	SOC_SINGLE("CODEC_DN_3_PGA_BYPASS", \
		CODEC_3_DN_PGA_CTRL_REG, CODEC_3_DN_PGA_BYPASS_OFFSET, \
		max_val_on_bit(CODEC_3_DN_PGA_BYPASS_LEN), 0), \
	SOC_SINGLE("ULTR_DN_PGA_BYPASS", \
		ULTR_DN_PGA_CTRL_REG, ULTR_DN_PGA_BYPASS_OFFSET, \
		max_val_on_bit(ULTR_DN_PGA_BYPASS_LEN), 0) \

#define DN_PGA_GAIN_KCONTROLS \
	SOC_SINGLE("AUDIO_DN_0_PGA_GAIN", \
		AUDIO_0_DN_PGA_CTRL_REG, AUDIO_0_DN_PGA_GAIN_OFFSET, \
		max_val_on_bit(AUDIO_0_DN_PGA_GAIN_LEN), 0), \
	SOC_SINGLE("AUDIO_DN_1_PGA_GAIN", \
		AUDIO_1_DN_PGA_CTRL_REG, AUDIO_1_DN_PGA_GAIN_OFFSET, \
		max_val_on_bit(AUDIO_1_DN_PGA_GAIN_LEN), 0), \
	SOC_SINGLE("AUDIO_DN_2_PGA_GAIN", \
		AUDIO_2_DN_PGA_CTRL_REG, AUDIO_2_DN_PGA_GAIN_OFFSET, \
		max_val_on_bit(AUDIO_2_DN_PGA_GAIN_LEN), 0), \
	SOC_SINGLE("AUDIO_DN_3_PGA_GAIN", \
		AUDIO_3_DN_PGA_CTRL_REG, AUDIO_3_DN_PGA_GAIN_OFFSET, \
		max_val_on_bit(AUDIO_3_DN_PGA_GAIN_LEN), 0), \
	SOC_SINGLE("CODEC_DN_0_PGA_GAIN", \
		CODEC_0_DN_PGA_CTRL_REG, CODEC_0_DN_PGA_GAIN_OFFSET, \
		max_val_on_bit(CODEC_0_DN_PGA_GAIN_LEN), 0), \
	SOC_SINGLE("CODEC_DN_1_PGA_GAIN", \
		CODEC_1_DN_PGA_CTRL_REG, CODEC_1_DN_PGA_GAIN_OFFSET, \
		max_val_on_bit(CODEC_1_DN_PGA_GAIN_LEN), 0), \
	SOC_SINGLE("CODEC_DN_2_PGA_GAIN", \
		CODEC_2_DN_PGA_CTRL_REG, CODEC_2_DN_PGA_GAIN_OFFSET, \
		max_val_on_bit(CODEC_2_DN_PGA_GAIN_LEN), 0), \
	SOC_SINGLE("CODEC_DN_3_PGA_GAIN", \
		CODEC_3_DN_PGA_CTRL_REG, CODEC_3_DN_PGA_GAIN_OFFSET, \
		max_val_on_bit(CODEC_3_DN_PGA_GAIN_LEN), 0), \
	SOC_SINGLE("ULTR_DN_PGA_GAIN", \
		ULTR_DN_PGA_CTRL_REG, ULTR_DN_PGA_GAIN_OFFSET, \
		max_val_on_bit(ULTR_DN_PGA_GAIN_LEN), 0) \

#define DN_SRC_KCONTROLS \
	SOC_SINGLE("AUDIO_DN_0_SRCUP_DOUT", \
		FS_CTRL3_REG, FS_AUDIO_0_DN_SRCUP_DOUT_OFFSET, \
		max_val_on_bit(FS_AUDIO_0_DN_SRCUP_DOUT_LEN), 0), \
	SOC_SINGLE("AUDIO_DN_0_SRCUP_DIN", \
		FS_CTRL3_REG, FS_AUDIO_0_DN_SRCUP_DIN_OFFSET, \
		max_val_on_bit(FS_AUDIO_0_DN_SRCUP_DIN_LEN), 0), \
	SOC_SINGLE("AUDIO_DN_0_SRCUP_MODE", \
		SRCUP_CTRL0_REG, AUDIO_0_DN_SRCUP_SRC_MODE_OFFSET, \
		max_val_on_bit(AUDIO_0_DN_SRCUP_SRC_MODE_LEN), 0), \
	SOC_SINGLE("AUDIO_DN_1_SRCUP_DOUT", \
		FS_CTRL3_REG, FS_AUDIO_1_DN_SRCUP_DOUT_OFFSET, \
		max_val_on_bit(FS_AUDIO_1_DN_SRCUP_DOUT_LEN), 0), \
	SOC_SINGLE("AUDIO_DN_1_SRCUP_DIN", \
		FS_CTRL3_REG, FS_AUDIO_1_DN_SRCUP_DIN_OFFSET, \
		max_val_on_bit(FS_AUDIO_1_DN_SRCUP_DIN_LEN), 0), \
	SOC_SINGLE("AUDIO_DN_1_SRCUP_MODE", \
		SRCUP_CTRL0_REG, AUDIO_1_SRCUP_SRC_MODE_OFFSET, \
		max_val_on_bit(AUDIO_1_SRCUP_SRC_MODE_LEN), 0), \
	SOC_SINGLE("AUDIO_DN_2_SRCUP_DOUT", \
		FS_CTRL7_REG, FS_AUDIO_2_DN_SRCUP_DOUT_OFFSET, \
		max_val_on_bit(FS_AUDIO_2_DN_SRCUP_DOUT_LEN), 0), \
	SOC_SINGLE("AUDIO_DN_2_SRCUP_DIN", \
		FS_CTRL7_REG, FS_AUDIO_2_DN_SRCUP_DIN_OFFSET, \
		max_val_on_bit(FS_AUDIO_2_DN_SRCUP_DIN_LEN), 0), \
	SOC_SINGLE("AUDIO_DN_2_SRCUP_MODE", \
		SRCUP_CTRL0_REG, AUDIO_2_SRCUP_SRC_MODE_OFFSET, \
		max_val_on_bit(AUDIO_2_SRCUP_SRC_MODE_LEN), 0), \
	SOC_SINGLE("AUDIO_DN_3_SRCUP_DOUT", \
		FS_CTRL8_REG, FS_AUDIO_3_DN_SRCUP_DOUT_OFFSET, \
		max_val_on_bit(FS_AUDIO_3_DN_SRCUP_DOUT_LEN), 0), \
	SOC_SINGLE("AUDIO_DN_3_SRCUP_DIN", \
		FS_CTRL8_REG, FS_AUDIO_3_DN_SRCUP_DIN_OFFSET, \
		max_val_on_bit(FS_AUDIO_3_DN_SRCUP_DIN_LEN), 0), \
	SOC_SINGLE("AUDIO_DN_3_SRCUP_MODE", \
		SRCUP_CTRL2_REG, AUDIO_3_SRCUP_SRC_MODE_OFFSET, \
		max_val_on_bit(AUDIO_3_SRCUP_SRC_MODE_LEN), 0), \
	SOC_SINGLE("CODEC_DN_0_SRCUP_DOUT", \
		FS_CTRL3_REG, FS_CODEC_0_DN_SRCUP_DOUT_OFFSET, \
		max_val_on_bit(FS_CODEC_0_DN_SRCUP_DOUT_LEN), 0), \
	SOC_SINGLE("CODEC_DN_0_SRCUP_DIN", \
		FS_CTRL3_REG, FS_CODEC_0_DN_SRCUP_DIN_OFFSET, \
		max_val_on_bit(FS_CODEC_0_DN_SRCUP_DIN_LEN), 0), \
	SOC_SINGLE("CODEC_DN_0_SRCUP_MODE", \
		SRCUP_CTRL0_REG, CODEC_0_DN_SRCUP_SRC_MODE_OFFSET, \
		max_val_on_bit(CODEC_0_DN_SRCUP_SRC_MODE_LEN), 0), \
	SOC_SINGLE("CODEC_DN_1_SRCUP_DOUT", \
		FS_CTRL3_REG, FS_CODEC_1_DN_SRCUP_DOUT_OFFSET, \
		max_val_on_bit(FS_CODEC_1_DN_SRCUP_DOUT_LEN), 0), \
	SOC_SINGLE("CODEC_DN_1_SRCUP_DIN", \
		FS_CTRL3_REG, FS_CODEC_1_DN_SRCUP_DIN_OFFSET, \
		max_val_on_bit(FS_CODEC_1_DN_SRCUP_DIN_LEN), 0), \
	SOC_SINGLE("CODEC_DN_1_SRCUP_MODE", \
		SRCUP_CTRL0_REG, CODEC_1_DN_SRCUP_SRC_MODE_OFFSET, \
		max_val_on_bit(CODEC_1_DN_SRCUP_SRC_MODE_LEN), 0), \
	SOC_SINGLE("CODEC_DN_2_SRCUP_DOUT", \
		FS_CTRL3_REG, FS_CODEC_2_DN_SRCUP_DOUT_OFFSET, \
		max_val_on_bit(FS_CODEC_2_DN_SRCUP_DOUT_LEN), 0), \
	SOC_SINGLE("CODEC_DN_2_SRCUP_DIN", \
		FS_CTRL3_REG, FS_CODEC_2_DN_SRCUP_DIN_OFFSET, \
		max_val_on_bit(FS_CODEC_2_DN_SRCUP_DIN_LEN), 0), \
	SOC_SINGLE("CODEC_DN_2_SRCUP_MODE", \
		SRCUP_CTRL0_REG, CODEC_2_DN_SRCUP_SRC_MODE_OFFSET, \
		max_val_on_bit(CODEC_2_DN_SRCUP_SRC_MODE_LEN), 0), \
	SOC_SINGLE("CODEC_DN_3_SRCUP_DOUT", \
		FS_CTRL8_REG, FS_CODEC_3_DN_SRCUP_DOUT_OFFSET, \
		max_val_on_bit(FS_CODEC_3_DN_SRCUP_DOUT_LEN), 0), \
	SOC_SINGLE("CODEC_DN_3_SRCUP_DIN", \
		FS_CTRL8_REG, FS_CODEC_3_DN_SRCUP_DIN_OFFSET, \
		max_val_on_bit(FS_CODEC_3_DN_SRCUP_DIN_LEN), 0), \
	SOC_SINGLE("CODEC_DN_3_SRCUP_MODE", \
		SRCUP_CTRL2_REG, CODEC_3_DN_SRCUP_SRC_MODE_OFFSET, \
		max_val_on_bit(CODEC_3_DN_SRCUP_SRC_MODE_LEN), 0) \

static const struct snd_kcontrol_new audio_play_kcontrols[] = {
	S1_I2S_CFG_KCONTROLS,
	S1_TDM_CFG_KCONTROLS,
	MIXER4_GAIN_KCONTROLS,
	MIXER4_FS_KCONTROLS,
	MIXER2_ULTR_GAIN_KCONTROLS,
	FS_DN_FIFO_KCONTROLS,
	FS_DN_PGA_KCONTROLS,
	DN_PGA_BYPASS_KCONTROLS,
	DN_PGA_GAIN_KCONTROLS,
	DN_SRC_KCONTROLS,
};

/* mixer widget */
static const struct snd_kcontrol_new spa0_mixer4_controls[] = {
	SOC_DAPM_SINGLE("I2S2_RX_L", SPA0_MIXER4_CTRL0_REG,
		SPA0_MIXER4_IN1_MUTE_OFFSET, 1, 1),
	SOC_DAPM_SINGLE("CODEC_DN_0", SPA0_MIXER4_CTRL0_REG,
		SPA0_MIXER4_IN2_MUTE_OFFSET, 1, 1),
	SOC_DAPM_SINGLE("AUDIO_DN_0", SPA0_MIXER4_CTRL0_REG,
		SPA0_MIXER4_IN3_MUTE_OFFSET, 1, 1),
	SOC_DAPM_SINGLE("SIDETONE", SPA0_MIXER4_CTRL0_REG,
		SPA0_MIXER4_IN4_MUTE_OFFSET, 1, 1),
};

static const struct snd_kcontrol_new spa1_mixer4_controls[] = {
	SOC_DAPM_SINGLE("I2S2_RX_R", SPA1_MIXER4_CTRL0_REG,
		SPA1_MIXER4_IN1_MUTE_OFFSET, 1, 1),
	SOC_DAPM_SINGLE("CODEC_DN_1", SPA1_MIXER4_CTRL0_REG,
		SPA1_MIXER4_IN2_MUTE_OFFSET, 1, 1),
	SOC_DAPM_SINGLE("AUDIO_DN_1", SPA1_MIXER4_CTRL0_REG,
		SPA1_MIXER4_IN3_MUTE_OFFSET, 1, 1),
	SOC_DAPM_SINGLE("SIDETONE", SPA1_MIXER4_CTRL0_REG,
		SPA1_MIXER4_IN4_MUTE_OFFSET, 1, 1),
};

static const struct snd_kcontrol_new spa2_mixer4_controls[] = {
	SOC_DAPM_SINGLE("I2S2_RX_L", SPA2_MIXER4_CTRL0_REG,
		SPA2_MIXER4_IN1_MUTE_OFFSET, 1, 1),
	SOC_DAPM_SINGLE("CODEC_DN_2", SPA2_MIXER4_CTRL0_REG,
		SPA2_MIXER4_IN2_MUTE_OFFSET, 1, 1),
	SOC_DAPM_SINGLE("AUDIO_DN_2", SPA2_MIXER4_CTRL0_REG,
		SPA2_MIXER4_IN3_MUTE_OFFSET, 1, 1),
	SOC_DAPM_SINGLE("SIDETONE", SPA2_MIXER4_CTRL0_REG,
		SPA2_MIXER4_IN4_MUTE_OFFSET, 1, 1),
};

static const struct snd_kcontrol_new spa3_mixer4_controls[] = {
	SOC_DAPM_SINGLE("I2S2_RX_R", SPA3_MIXER4_CTRL0_REG,
		SPA3_MIXER4_IN1_MUTE_OFFSET, 1, 1),
	SOC_DAPM_SINGLE("CODEC_DN_3", SPA3_MIXER4_CTRL0_REG,
		SPA3_MIXER4_IN2_MUTE_OFFSET, 1, 1),
	SOC_DAPM_SINGLE("AUDIO_DN_3", SPA3_MIXER4_CTRL0_REG,
		SPA3_MIXER4_IN3_MUTE_OFFSET, 1, 1),
	SOC_DAPM_SINGLE("SIDETONE", SPA3_MIXER4_CTRL0_REG,
		SPA3_MIXER4_IN4_MUTE_OFFSET, 1, 1),
};

static const struct snd_kcontrol_new ultr0_mixer2_controls[] = {
	SOC_DAPM_SINGLE("SPA0_DOUT", SPA0_DOUT_CTRL_REG,
		SPA0_DOUT_OFFSET, 1, 0),
	SOC_DAPM_SINGLE("ULTR_DOUT", ULTR0_MIXER2_CTRL_REG,
		ULTR0_MIXER2_IN2_MUTE_OFFSET, 1, 1),
};

static const struct snd_kcontrol_new ultr1_mixer2_controls[] = {
	SOC_DAPM_SINGLE("SPA1_DOUT", SPA1_DOUT_CTRL_REG,
		SPA1_DOUT_OFFSET, 1, 0),
	SOC_DAPM_SINGLE("ULTR_DOUT", ULTR1_MIXER2_CTRL_REG,
		ULTR1_MIXER2_IN2_MUTE_OFFSET, 1, 1),
};

static const struct snd_kcontrol_new ultr2_mixer2_controls[] = {
	SOC_DAPM_SINGLE("SPA2_DOUT", SPA2_DOUT_CTRL_REG,
		SPA2_DOUT_OFFSET, 1, 0),
	SOC_DAPM_SINGLE("ULTR_DOUT", ULTR2_MIXER2_CTRL_REG,
		ULTR2_MIXER2_IN2_MUTE_OFFSET, 1, 1),
};

static const struct snd_kcontrol_new ultr3_mixer2_controls[] = {
	SOC_DAPM_SINGLE("SPA3_DOUT", SPA3_DOUT_CTRL_REG,
		SPA3_DOUT_OFFSET, 1, 0),
	SOC_DAPM_SINGLE("ULTR_DOUT", ULTR3_MIXER2_CTRL_REG,
		ULTR3_MIXER2_IN2_MUTE_OFFSET, 1, 1),
};

static const char * const i2s1_tx_l_mux_texts[] = {
	"AU_UP_L",
	"NULL",
	"SPA0_MIXER2_DOUT",
};

static const struct soc_enum i2s1_tx_l_mux_enum =
	SOC_ENUM_SINGLE(CODEC_DIN_MUX0_REG, I2S1_TX_L_SEL_OFFSET,
		ARRAY_SIZE(i2s1_tx_l_mux_texts), i2s1_tx_l_mux_texts);

static const struct snd_kcontrol_new dapm_i2s1_tx_l_mux_controls =
	SOC_DAPM_ENUM("Mux", i2s1_tx_l_mux_enum);

static const char * const i2s1_tx_r_mux_texts[] = {
	"AU_UP_R",
	"NULL",
	"SPA1_MIXER2_DOUT",
};

static const struct soc_enum i2s1_tx_r_mux_enum =
	SOC_ENUM_SINGLE(CODEC_DIN_MUX0_REG, I2S1_TX_R_SEL_OFFSET,
		ARRAY_SIZE(i2s1_tx_r_mux_texts), i2s1_tx_r_mux_texts);

static const struct snd_kcontrol_new dapm_i2s1_tx_r_mux_controls =
	SOC_DAPM_ENUM("Mux", i2s1_tx_r_mux_enum);

static const char * const s1_tdm_tx_0_mux_texts[] = {
	"CODEC_0_SRCUP_DOUT",
	"SPA0_MIXER2_DOUT",
};

static const struct soc_enum s1_tdm_tx_0_mux_enum =
	SOC_ENUM_SINGLE(SPA_AFIFO_UP_DIN_CTRL6_REG, BM_I2S1_TDM128_SLOT0_DIN_SEL_OFFSET,
		ARRAY_SIZE(s1_tdm_tx_0_mux_texts), s1_tdm_tx_0_mux_texts);

static const struct snd_kcontrol_new dapm_s1_tdm_tx_0_mux_controls =
	SOC_DAPM_ENUM("Mux", s1_tdm_tx_0_mux_enum);

static const char * const s1_tdm_tx_1_mux_texts[] = {
	"CODEC_1_SRCUP_DOUT",
	"SPA1_MIXER2_DOUT",
};

static const struct soc_enum s1_tdm_tx_1_mux_enum =
	SOC_ENUM_SINGLE(SPA_AFIFO_UP_DIN_CTRL6_REG, BM_I2S1_TDM128_SLOT1_DIN_SEL_OFFSET,
		ARRAY_SIZE(s1_tdm_tx_1_mux_texts), s1_tdm_tx_1_mux_texts);

static const struct snd_kcontrol_new dapm_s1_tdm_tx_1_mux_controls =
	SOC_DAPM_ENUM("Mux", s1_tdm_tx_1_mux_enum);

static const char * const s1_tdm_tx_2_mux_texts[] = {
	"AUDIO_0_SRCUP_DOUT",
	"SPA2_MIXER2_DOUT",
};

static const struct soc_enum s1_tdm_tx_2_mux_enum =
	SOC_ENUM_SINGLE(CODEC_DIN_MUX1_REG, BM_I2S1_TDM_TX_L_DIN_SEL_OFFSET,
		ARRAY_SIZE(s1_tdm_tx_2_mux_texts), s1_tdm_tx_2_mux_texts);

static const struct snd_kcontrol_new dapm_s1_tdm_tx_2_mux_controls =
	SOC_DAPM_ENUM("Mux", s1_tdm_tx_2_mux_enum);

static const char * const s1_tdm_tx_3_mux_texts[] = {
	"AUDIO_1_SRCUP_DOUT",
	"SPA3_MIXER2_DOUT",
};

static const struct soc_enum s1_tdm_tx_3_mux_enum =
	SOC_ENUM_SINGLE(CODEC_DIN_MUX1_REG, BM_I2S1_TDM_TX_R_DIN_SEL_OFFSET,
		ARRAY_SIZE(s1_tdm_tx_3_mux_texts), s1_tdm_tx_3_mux_texts);

static const struct snd_kcontrol_new dapm_s1_tdm_tx_3_mux_controls =
	SOC_DAPM_ENUM("Mux", s1_tdm_tx_3_mux_enum);

static const char * const spa1_din_mux_texts[] = {
	"I2S1_DIN",
	"SPA0_DOUT",
};

static const struct soc_enum spa1_din_mux_enum =
	SOC_ENUM_SINGLE(SPA_AFIFO_UP_DIN_CTRL6_REG, SPA1_DIN_SEL_OFFSET,
		ARRAY_SIZE(spa1_din_mux_texts), spa1_din_mux_texts);

static const struct snd_kcontrol_new dapm_spa1_din_mux_controls =
	SOC_DAPM_ENUM("Mux", spa1_din_mux_enum);

static const char * const spa2_din_mux_texts[] = {
	"I2S1_DIN",
	"SPA1_DOUT",
};

static const struct soc_enum spa2_din_mux_enum =
	SOC_ENUM_SINGLE(SPA_AFIFO_UP_DIN_CTRL6_REG, SPA2_DIN_SEL_OFFSET,
		ARRAY_SIZE(spa2_din_mux_texts), spa2_din_mux_texts);

static const struct snd_kcontrol_new dapm_spa2_din_mux_controls =
	SOC_DAPM_ENUM("Mux", spa2_din_mux_enum);

static const char * const spa3_din_mux_texts[] = {
	"I2S1_DIN",
	"SPA2_DOUT",
};

static const struct soc_enum spa3_din_mux_enum =
	SOC_ENUM_SINGLE(SPA_AFIFO_UP_DIN_CTRL6_REG, SPA3_DIN_SEL_OFFSET,
		ARRAY_SIZE(spa3_din_mux_texts), spa3_din_mux_texts);

static const struct snd_kcontrol_new dapm_spa3_din_mux_controls =
	SOC_DAPM_ENUM("Mux", spa3_din_mux_enum);

static const char * const spa4_din_mux_texts[] = {
	"I2S1_DIN",
	"SPA3_DOUT",
};

static const struct soc_enum spa4_din_mux_enum =
	SOC_ENUM_SINGLE(SPA_AFIFO_UP_DIN_CTRL6_REG, SPA4_DIN_SEL_OFFSET,
		ARRAY_SIZE(spa4_din_mux_texts), spa4_din_mux_texts);

static const struct snd_kcontrol_new dapm_spa4_din_mux_controls =
	SOC_DAPM_ENUM("Mux", spa4_din_mux_enum);

static const struct snd_kcontrol_new dapm_s1_tx_tdm_switch_controls =
	SOC_DAPM_SINGLE("ENABLE", CODEC_VIR1_REG_ADDR, S1_TX_TDM_BIT, 1, 0);
static const struct snd_kcontrol_new dapm_s1_tx_i2s_switch_controls =
	SOC_DAPM_SINGLE("ENABLE", CODEC_VIR1_REG_ADDR, S1_TX_I2S_BIT, 1, 0);
static const struct snd_kcontrol_new dapm_audio_2pa_dn_switch_controls =
	SOC_DAPM_SINGLE("ENABLE", CODEC_VIR1_REG_ADDR, AUDIO_DN_2PA_IN_BIT, 1, 0);
static const struct snd_kcontrol_new dapm_audio_4pa_dn_switch_controls =
	SOC_DAPM_SINGLE("ENABLE", CODEC_VIR1_REG_ADDR, AUDIO_DN_4PA_IN_BIT, 1, 0);
static const struct snd_kcontrol_new dapm_iv_2pa_48k_switch_controls =
	SOC_DAPM_SINGLE("ENABLE", CODEC_VIR1_REG_ADDR, IV_2PA_BIT, 1, 0);

static const struct snd_kcontrol_new dapm_ec_2pa_switch_controls =
	SOC_DAPM_SINGLE("ENABLE", CODEC_VIR1_REG_ADDR, EC_2PA_BIT, 1, 0);

static const struct snd_kcontrol_new dapm_ec_4pa_switch_controls =
	SOC_DAPM_SINGLE("ENABLE", CODEC_VIR1_REG_ADDR, EC_4PA_BIT, 1, 0);
static const struct snd_kcontrol_new dapm_iv_4pa_switch_controls =
	SOC_DAPM_SINGLE("ENABLE", CODEC_VIR1_REG_ADDR, IV_4PA_BIT, 1, 0);
static const struct snd_kcontrol_new dapm_carkit_48k_switch_controls =
	SOC_DAPM_SINGLE("ENABLE", CODEC_VIR1_REG_ADDR, CARKIT_BIT, 1, 0);
static const struct snd_kcontrol_new dapm_s1_tx_tdm256_switch_controls =
	SOC_DAPM_SINGLE("ENABLE", CODEC_VIR1_REG_ADDR, S1_TX_TDM256_BIT, 1, 0);

static const struct snd_kcontrol_new dapm_a2dp_offload_switch_controls =
	SOC_DAPM_SINGLE("ENABLE", CODEC_VIR1_REG_ADDR, A2DP_OFFLOAD_BIT, 1, 0);

static const struct snd_kcontrol_new dapm_ultrasonic_dn_switch_controls =
	SOC_DAPM_SINGLE("ENABLE", CODEC_VIR1_REG_ADDR, ULTRASONIC_DN_BIT, 1, 0);

static int s1_tx_tdm_power_event(struct snd_soc_dapm_widget *w,
	struct snd_kcontrol *kcontrol, int event)
{
	struct snd_soc_component *codec = snd_soc_dapm_to_component(w->dapm);

	switch (event) {
	case SND_SOC_DAPM_PRE_PMU:
		snd_soc_component_update_bits(codec, I2S1_TDM_CTRL1_REG,
			max_val_on_bit(S1_I2S_TDM_MODE_LEN) << S1_I2S_TDM_MODE_OFFSET, 0x1 << S1_I2S_TDM_MODE_OFFSET);
		snd_soc_component_update_bits(codec, I2S1_TDM_CTRL0_REG, BIT(S1_TDM_IF_EN_OFFSET), BIT(S1_TDM_IF_EN_OFFSET));
		snd_soc_component_update_bits(codec, I2S1_CTRL_REG, BIT(I2S1_FRAME_MODE_OFFSET), BIT(I2S1_FRAME_MODE_OFFSET));
		snd_soc_component_update_bits(codec, I2S1_TDM_CTRL0_REG,
			mask_on_bit(S1_TDM_FRAME_MODE_LEN, S1_TDM_FRAME_MODE_OFFSET), 0x1 << S1_TDM_FRAME_MODE_OFFSET);
		break;
	case SND_SOC_DAPM_POST_PMD:
		snd_soc_component_update_bits(codec, I2S1_TDM_CTRL1_REG,
			max_val_on_bit(S1_I2S_TDM_MODE_LEN) << S1_I2S_TDM_MODE_OFFSET, 0);
		snd_soc_component_update_bits(codec, I2S1_TDM_CTRL0_REG, BIT(S1_TDM_IF_EN_OFFSET), 0);
		snd_soc_component_update_bits(codec, I2S1_CTRL_REG, BIT(I2S1_FRAME_MODE_OFFSET), 0);
		break;
	default:
		AUDIO_LOGW("power event err: %d", event);
		break;
	}

	return 0;
}

static int s1_tx_i2s_power_event(struct snd_soc_dapm_widget *w,
	struct snd_kcontrol *kcontrol, int event)
{
	struct snd_soc_component *codec = snd_soc_dapm_to_component(w->dapm);

	switch (event) {
	case SND_SOC_DAPM_PRE_PMU:
		snd_soc_component_update_bits(codec, I2S1_TDM_CTRL1_REG,
			max_val_on_bit(S1_I2S_TDM_MODE_LEN) << S1_I2S_TDM_MODE_OFFSET, 0);

		break;
	case SND_SOC_DAPM_POST_PMD:
		snd_soc_component_update_bits(codec, I2S1_TDM_CTRL1_REG,
			max_val_on_bit(S1_I2S_TDM_MODE_LEN) << S1_I2S_TDM_MODE_OFFSET, 0);
		break;
	default:
		break;
	}
    AUDIO_LOGI("power event: %d", event);
	return 0;
}

static int s1_tx_tdm256_power_event(struct snd_soc_dapm_widget *w,
	struct snd_kcontrol *kcontrol, int event)
{
	struct snd_soc_component *codec = snd_soc_dapm_to_component(w->dapm);

	switch (event) {
	case SND_SOC_DAPM_PRE_PMU:
		snd_soc_component_update_bits(codec, I2S1_TDM_CTRL1_REG,
			max_val_on_bit(S1_I2S_TDM_MODE_LEN) << S1_I2S_TDM_MODE_OFFSET, 0x3 << S1_I2S_TDM_MODE_OFFSET);
		break;
	case SND_SOC_DAPM_POST_PMD:
		snd_soc_component_update_bits(codec, I2S1_TDM_CTRL1_REG,
			max_val_on_bit(S1_I2S_TDM_MODE_LEN) << S1_I2S_TDM_MODE_OFFSET, 0);
		break;
	default:
		AUDIO_LOGW("power event err: %d", event);
		break;
	}

	return 0;
}

static int audio_2pa_dn_power_event(struct snd_soc_dapm_widget *w,
	struct snd_kcontrol *kcontrol, int event)
{
	struct snd_soc_component *codec = snd_soc_dapm_to_component(w->dapm);

	switch (event) {
	case SND_SOC_DAPM_PRE_PMU:
		snd_soc_component_update_bits_with_lock(codec, CODEC_CLK_EN0_REG,
			BIT(AUDIO_0_DN_AFIFO_CLKEN_OFFSET) | BIT(AUDIO_1_DN_AFIFO_CLKEN_OFFSET), 0);
		udelay(CLEAR_FIFO_DELAY_LEN_US);
		snd_soc_component_update_bits_with_lock(codec, CODEC_CLK_EN0_REG,
			BIT(AUDIO_0_DN_AFIFO_CLKEN_OFFSET) | BIT(AUDIO_1_DN_AFIFO_CLKEN_OFFSET),
			BIT(AUDIO_0_DN_AFIFO_CLKEN_OFFSET) | BIT(AUDIO_1_DN_AFIFO_CLKEN_OFFSET));
		break;
	case SND_SOC_DAPM_POST_PMD:
		snd_soc_component_update_bits_with_lock(codec, CODEC_CLK_EN0_REG,
			BIT(AUDIO_0_DN_AFIFO_CLKEN_OFFSET) | BIT(AUDIO_1_DN_AFIFO_CLKEN_OFFSET), 0);
		break;
	default:
		break;
	}
	AUDIO_LOGI("power event: %d", event);

	return 0;
}

static void spa4_dn_fifo_clk_clear(struct snd_soc_component *codec)
{
	snd_soc_component_update_bits_with_lock(codec, CODEC_CLK_EN5_REG,
		BIT(AUDIO_3_DN_AFIFO_CLKEN_OFFSET), 0);
	snd_soc_component_update_bits_with_lock(codec, CODEC_CLK_EN0_REG,
		BIT(AUDIO_0_DN_AFIFO_CLKEN_OFFSET) | BIT(AUDIO_1_DN_AFIFO_CLKEN_OFFSET) |
		BIT(AUDIO_2_DN_AFIFO_CLKEN_OFFSET), 0);
}

static void spa4_dn_fifo_clk_set(struct snd_soc_component *codec)
{
	snd_soc_component_update_bits_with_lock(codec, CODEC_CLK_EN5_REG,
		BIT(AUDIO_3_DN_AFIFO_CLKEN_OFFSET), BIT(AUDIO_3_DN_AFIFO_CLKEN_OFFSET));
	snd_soc_component_update_bits_with_lock(codec, CODEC_CLK_EN0_REG,
		BIT(AUDIO_0_DN_AFIFO_CLKEN_OFFSET) | BIT(AUDIO_1_DN_AFIFO_CLKEN_OFFSET) |
		BIT(AUDIO_2_DN_AFIFO_CLKEN_OFFSET), BIT(AUDIO_0_DN_AFIFO_CLKEN_OFFSET) |
		BIT(AUDIO_1_DN_AFIFO_CLKEN_OFFSET) | BIT(AUDIO_2_DN_AFIFO_CLKEN_OFFSET));
}

static int audio_4pa_dn_power_event(struct snd_soc_dapm_widget *w,
	struct snd_kcontrol *kcontrol, int event)
{
	struct snd_soc_component *codec = snd_soc_dapm_to_component(w->dapm);

	switch (event) {
	case SND_SOC_DAPM_PRE_PMU:
		spa4_dn_fifo_clk_clear(codec);
		udelay(CLEAR_FIFO_DELAY_LEN_US);
		spa4_dn_fifo_clk_set(codec);
		break;
	case SND_SOC_DAPM_POST_PMD:
		spa4_dn_fifo_clk_clear(codec);
		break;
	default:
		break;
	}
	AUDIO_LOGI("power event: %d", event);

	return 0;
}

static int iv_2pa_48k_power_event(struct snd_soc_dapm_widget *w,
	struct snd_kcontrol *kcontrol, int event)
{
	struct snd_soc_component *codec = snd_soc_dapm_to_component(w->dapm);

	switch (event) {
	case SND_SOC_DAPM_PRE_PMU:
		snd_soc_component_update_bits_with_lock(codec, CODEC_CLK_EN0_REG,
			BIT(SPA5_UP_AFIFO_CLKEN_OFFSET), 0);
		snd_soc_component_update_bits_with_lock(codec, CODEC_CLK_EN5_REG,
			BIT(SPA7_UP_AFIFO_CLKEN_OFFSET), 0);
		udelay(CLEAR_FIFO_DELAY_LEN_US);
		snd_soc_component_update_bits_with_lock(codec, CODEC_CLK_EN0_REG, BIT(SPA5_UP_AFIFO_CLKEN_OFFSET),
			BIT(SPA5_UP_AFIFO_CLKEN_OFFSET));
		snd_soc_component_update_bits_with_lock(codec, CODEC_CLK_EN5_REG, BIT(SPA7_UP_AFIFO_CLKEN_OFFSET),
			BIT(SPA7_UP_AFIFO_CLKEN_OFFSET));
		break;
	case SND_SOC_DAPM_POST_PMD:
		snd_soc_component_update_bits_with_lock(codec, CODEC_CLK_EN0_REG,
			BIT(SPA5_UP_AFIFO_CLKEN_OFFSET), 0);
		snd_soc_component_update_bits_with_lock(codec, CODEC_CLK_EN5_REG,
			BIT(SPA7_UP_AFIFO_CLKEN_OFFSET), 0);
		break;
	default:
		break;
	}
	AUDIO_LOGI("power event: %d", event);

	return 0;
}

static int ec_2pa_power_event(struct snd_soc_dapm_widget *w,
	struct snd_kcontrol *kcontrol, int event)
{
	struct snd_soc_component *codec = snd_soc_dapm_to_component(w->dapm);

	switch (event) {
	case SND_SOC_DAPM_PRE_PMU:
		snd_soc_component_update_bits_with_lock(codec, CODEC_CLK_EN0_REG,
			BIT(SPA1_UP_AFIFO_CLKEN_OFFSET) | BIT(SPA2_UP_AFIFO_CLKEN_OFFSET), 0);
		udelay(CLEAR_EC_FIFO_DELAY_LEN_US);
		snd_soc_component_update_bits_with_lock(codec, CODEC_CLK_EN0_REG,
			BIT(SPA1_UP_AFIFO_CLKEN_OFFSET) | BIT(SPA2_UP_AFIFO_CLKEN_OFFSET),
			BIT(SPA1_UP_AFIFO_CLKEN_OFFSET) | BIT(SPA2_UP_AFIFO_CLKEN_OFFSET));
		break;
	case SND_SOC_DAPM_POST_PMD:
		snd_soc_component_update_bits_with_lock(codec, CODEC_CLK_EN0_REG,
			BIT(SPA1_UP_AFIFO_CLKEN_OFFSET) | BIT(SPA2_UP_AFIFO_CLKEN_OFFSET), 0);
		break;
	default:
		break;
	}
	AUDIO_LOGI("power event: %d", event);

	return 0;
}

static int ec_4pa_power_event(struct snd_soc_dapm_widget *w,
	struct snd_kcontrol *kcontrol, int event)
{
	struct snd_soc_component *codec = snd_soc_dapm_to_component(w->dapm);

	switch (event) {
	case SND_SOC_DAPM_PRE_PMU:
		snd_soc_component_update_bits_with_lock(codec, CODEC_CLK_EN0_REG,
			BIT(SPA1_UP_AFIFO_CLKEN_OFFSET) | BIT(SPA2_UP_AFIFO_CLKEN_OFFSET) |
			BIT(SPA3_UP_AFIFO_CLKEN_OFFSET) | BIT(SPA4_UP_AFIFO_CLKEN_OFFSET), 0);
		udelay(CLEAR_EC_FIFO_DELAY_LEN_US);
		snd_soc_component_update_bits_with_lock(codec, CODEC_CLK_EN0_REG,
			BIT(SPA1_UP_AFIFO_CLKEN_OFFSET) | BIT(SPA2_UP_AFIFO_CLKEN_OFFSET) |
			BIT(SPA3_UP_AFIFO_CLKEN_OFFSET) | BIT(SPA4_UP_AFIFO_CLKEN_OFFSET),
			BIT(SPA1_UP_AFIFO_CLKEN_OFFSET) | BIT(SPA2_UP_AFIFO_CLKEN_OFFSET) |
			BIT(SPA3_UP_AFIFO_CLKEN_OFFSET) | BIT(SPA4_UP_AFIFO_CLKEN_OFFSET));
		break;
	case SND_SOC_DAPM_POST_PMD:
		snd_soc_component_update_bits_with_lock(codec, CODEC_CLK_EN0_REG,
			BIT(SPA1_UP_AFIFO_CLKEN_OFFSET) | BIT(SPA2_UP_AFIFO_CLKEN_OFFSET) |
			BIT(SPA3_UP_AFIFO_CLKEN_OFFSET) | BIT(SPA4_UP_AFIFO_CLKEN_OFFSET), 0);
		break;
	default:
		break;
	}
	AUDIO_LOGI("power event: %d", event);

	return 0;
}

static void spa4_up_fifo_clk_clear(struct snd_soc_component *codec)
{
	snd_soc_component_update_bits_with_lock(codec, CODEC_CLK_EN0_REG,
		BIT(SPA5_UP_AFIFO_CLKEN_OFFSET) | BIT(SPA6_UP_AFIFO_CLKEN_OFFSET), 0);
	snd_soc_component_update_bits_with_lock(codec, CODEC_CLK_EN5_REG,
		BIT(SPA7_UP_AFIFO_CLKEN_OFFSET) | BIT(SPA8_UP_AFIFO_CLKEN_OFFSET), 0);
}

static void spa4_up_fifo_clk_set(struct snd_soc_component *codec)
{
	snd_soc_component_update_bits_with_lock(codec, CODEC_CLK_EN0_REG,
		BIT(SPA5_UP_AFIFO_CLKEN_OFFSET) | BIT(SPA6_UP_AFIFO_CLKEN_OFFSET),
		BIT(SPA5_UP_AFIFO_CLKEN_OFFSET) | BIT(SPA6_UP_AFIFO_CLKEN_OFFSET));
	snd_soc_component_update_bits_with_lock(codec, CODEC_CLK_EN5_REG,
		BIT(SPA7_UP_AFIFO_CLKEN_OFFSET) | BIT(SPA8_UP_AFIFO_CLKEN_OFFSET),
		BIT(SPA7_UP_AFIFO_CLKEN_OFFSET) | BIT(SPA8_UP_AFIFO_CLKEN_OFFSET));
}

static int iv_4pa_power_event(struct snd_soc_dapm_widget *w,
	struct snd_kcontrol *kcontrol, int event)
{
	struct snd_soc_component *codec = snd_soc_dapm_to_component(w->dapm);

	switch (event) {
	case SND_SOC_DAPM_PRE_PMU:
		snd_soc_component_update_bits_with_lock(codec, CODEC_SW_RST_N_REG,
			BIT(RST_4MIC_DN_ACCESS_IRQ_OFFSET) | BIT(CODEC_SW_RST_N_OFFSET),
			BIT(RST_4MIC_DN_ACCESS_IRQ_OFFSET) | BIT(CODEC_SW_RST_N_OFFSET));
		spa4_up_fifo_clk_clear(codec);
		udelay(CLEAR_FIFO_DELAY_LEN_US);
		spa4_up_fifo_clk_set(codec);
		snd_soc_component_update_bits_with_lock(codec, CODEC_SW_RST_N_REG,
			BIT(RST_4MIC_DN_ACCESS_IRQ_OFFSET), 0);
		break;
	case SND_SOC_DAPM_POST_PMD:
		spa4_up_fifo_clk_clear(codec);
		break;
	default:
		break;
	}
	AUDIO_LOGI("power event: %d", event);

	return 0;
}

static int carkit_48k_power_event(struct snd_soc_dapm_widget *w,
	struct snd_kcontrol *kcontrol, int event)
{
	struct snd_soc_component *codec = snd_soc_dapm_to_component(w->dapm);

	switch (event) {
	case SND_SOC_DAPM_PRE_PMU:
		snd_soc_component_update_bits_with_lock(codec, CODEC_CLK_EN0_REG,
			BIT(AUDIO_0_DN_AFIFO_CLKEN_OFFSET) | BIT(AUDIO_1_DN_AFIFO_CLKEN_OFFSET), 0);
		udelay(CLEAR_FIFO_DELAY_LEN_US);
		snd_soc_component_update_bits_with_lock(codec, CODEC_CLK_EN0_REG,
			BIT(AUDIO_0_DN_AFIFO_CLKEN_OFFSET) | BIT(AUDIO_1_DN_AFIFO_CLKEN_OFFSET),
			BIT(AUDIO_0_DN_AFIFO_CLKEN_OFFSET) | BIT(AUDIO_1_DN_AFIFO_CLKEN_OFFSET));
		break;
	case SND_SOC_DAPM_POST_PMD:
		snd_soc_component_update_bits_with_lock(codec, CODEC_CLK_EN0_REG,
			BIT(AUDIO_0_DN_AFIFO_CLKEN_OFFSET) | BIT(AUDIO_1_DN_AFIFO_CLKEN_OFFSET), 0);
		break;
	default:
		AUDIO_LOGW("power event err:%d", event);
		break;
	}

	return 0;
}

static int a2dp_offload_power_event(struct snd_soc_dapm_widget *w,
	struct snd_kcontrol *kcontrol, int event)
{
	struct snd_soc_component *codec = snd_soc_dapm_to_component(w->dapm);

	switch (event) {
	case SND_SOC_DAPM_PRE_PMU:
		snd_soc_component_update_bits_with_lock(codec, CODEC_CLK_EN4_REG,
			BIT(BT_L_DN_AFIFO_CLKEN_OFFSET) | BIT(BT_R_DN_AFIFO_CLKEN_OFFSET), 0);
		udelay(CLEAR_FIFO_DELAY_LEN_US);
		snd_soc_component_update_bits_with_lock(codec, CODEC_CLK_EN4_REG,
			BIT(BT_L_DN_AFIFO_CLKEN_OFFSET) | BIT(BT_R_DN_AFIFO_CLKEN_OFFSET),
			BIT(BT_L_DN_AFIFO_CLKEN_OFFSET) | BIT(BT_R_DN_AFIFO_CLKEN_OFFSET));
		break;
	case SND_SOC_DAPM_POST_PMD:
		snd_soc_component_update_bits_with_lock(codec, CODEC_CLK_EN4_REG,
			BIT(BT_L_DN_AFIFO_CLKEN_OFFSET) | BIT(BT_R_DN_AFIFO_CLKEN_OFFSET), 0);
		break;
	default:
		AUDIO_LOGW("power event err:%d", event);
		break;
	}

	return 0;
}

static int ultrasonic_dn_power_event(struct snd_soc_dapm_widget *w,
	struct snd_kcontrol *kcontrol, int event)
{
	struct snd_soc_component *codec = snd_soc_dapm_to_component(w->dapm);

	switch (event) {
	case SND_SOC_DAPM_PRE_PMU:
		snd_soc_component_update_bits_with_lock(codec, CODEC_CLK_EN0_REG,
			BIT(ULTR_DN_AFIFO_CLKEN_OFFSET), 0);
		udelay(CLEAR_FIFO_DELAY_LEN_US);
		snd_soc_component_update_bits_with_lock(codec, CODEC_CLK_EN0_REG,
			BIT(ULTR_DN_AFIFO_CLKEN_OFFSET), BIT(ULTR_DN_AFIFO_CLKEN_OFFSET));
		break;
	case SND_SOC_DAPM_POST_PMD:
		snd_soc_component_update_bits_with_lock(codec, CODEC_CLK_EN0_REG,
			BIT(ULTR_DN_AFIFO_CLKEN_OFFSET), 0);
		break;
	default:
		AUDIO_LOGW("power event err:%d", event);
		break;
	}

	return 0;
}

#define AUDIO_INPUT_OUTPUT_WIDGET \
	SND_SOC_DAPM_INPUT("I2S1_RX_INPUT"), \
	SND_SOC_DAPM_OUTPUT("SMARTPA_UP_FIFO_OUTPUT"), \
	SND_SOC_DAPM_INPUT("AUDIO_DN_FIFO_INPUT"), \
	SND_SOC_DAPM_INPUT("CODEC_DN_FIFO_INPUT"), \
	SND_SOC_DAPM_OUTPUT("I2S1_TX_OUTPUT"), \
	SND_SOC_DAPM_INPUT("CARKIT_INPUT"), \
	SND_SOC_DAPM_OUTPUT("CARKIT_OUTPUT"), \
	SND_SOC_DAPM_INPUT("A2DP_OFFLOAD_INPUT"), \
	SND_SOC_DAPM_OUTPUT("A2DP_OFFLOAD_OUTPUT"), \
	SND_SOC_DAPM_INPUT("ULTRA_INPUT"), \
	SND_SOC_DAPM_OUTPUT("ULTRA_OUTPUT"), \
	SND_SOC_DAPM_INPUT("SPK_SPA_INPUT"), \
	SND_SOC_DAPM_INPUT("RCV_SPA_INPUT"), \
	SND_SOC_DAPM_OUTPUT("SPA_OUTPUT"), \
	SND_SOC_DAPM_INPUT("AUDIO_UP_EC_INPUT"), \
	SND_SOC_DAPM_INPUT("CODEC_PLAY_INPUT"), \
	SND_SOC_DAPM_OUTPUT("CODEC_PLAY_OUTPUT") \

#define AUDIO_MIXER_WIDGET \
	SND_SOC_DAPM_MIXER("SPA0_MIXER4", CODEC_CLK_EN2_REG, \
		MIXER4_SPA0_CLKEN_OFFSET, 0, spa0_mixer4_controls, \
		ARRAY_SIZE(spa0_mixer4_controls)), \
	SND_SOC_DAPM_MIXER("SPA1_MIXER4", CODEC_CLK_EN2_REG, \
		MIXER4_SPA1_CLKEN_OFFSET, 0, spa1_mixer4_controls, \
		ARRAY_SIZE(spa1_mixer4_controls)), \
	SND_SOC_DAPM_MIXER("SPA2_MIXER4", CODEC_CLK_EN2_REG, \
		MIXER4_SPA2_CLKEN_OFFSET, 0, spa2_mixer4_controls, \
		ARRAY_SIZE(spa2_mixer4_controls)), \
	SND_SOC_DAPM_MIXER("SPA3_MIXER4", CODEC_CLK_EN5_REG, \
		MIXER4_SPA3_CLKEN_OFFSET, 0, spa3_mixer4_controls, \
		ARRAY_SIZE(spa3_mixer4_controls)), \
	SND_SOC_DAPM_MIXER("ULTR0_MIXER2", CODEC_CLK_EN2_REG, \
		MIXER2_ULTR0_CLKEN_OFFSET, 0, ultr0_mixer2_controls, \
		ARRAY_SIZE(ultr0_mixer2_controls)), \
	SND_SOC_DAPM_MIXER("ULTR1_MIXER2", CODEC_CLK_EN2_REG, \
		MIXER2_ULTR1_CLKEN_OFFSET, 0, ultr1_mixer2_controls, \
		ARRAY_SIZE(ultr1_mixer2_controls)), \
	SND_SOC_DAPM_MIXER("ULTR2_MIXER2", CODEC_CLK_EN2_REG, \
		MIXER2_ULTR2_CLKEN_OFFSET, 0, ultr2_mixer2_controls, \
		ARRAY_SIZE(ultr2_mixer2_controls)), \
	SND_SOC_DAPM_MIXER("ULTR3_MIXER2", CODEC_CLK_EN5_REG, \
		MIXER2_ULTR3_CLKEN_OFFSET, 0, ultr3_mixer2_controls, \
		ARRAY_SIZE(ultr3_mixer2_controls)) \

#define AUDIO_MUX_WIDGET \
	SND_SOC_DAPM_MUX("I2S1_TX_L_MUX", \
		SND_SOC_NOPM, 0, 0, &dapm_i2s1_tx_l_mux_controls), \
	SND_SOC_DAPM_MUX("I2S1_TX_R_MUX", \
		SND_SOC_NOPM, 0, 0, &dapm_i2s1_tx_r_mux_controls), \
	SND_SOC_DAPM_MUX("I2S1_TDM_TX_0_MUX", \
		SND_SOC_NOPM, 0, 0, &dapm_s1_tdm_tx_0_mux_controls), \
	SND_SOC_DAPM_MUX("I2S1_TDM_TX_1_MUX", \
		SND_SOC_NOPM, 0, 0, &dapm_s1_tdm_tx_1_mux_controls), \
	SND_SOC_DAPM_MUX("I2S1_TDM_TX_2_MUX", \
		SND_SOC_NOPM, 0, 0, &dapm_s1_tdm_tx_2_mux_controls), \
	SND_SOC_DAPM_MUX("I2S1_TDM_TX_3_MUX", \
		SND_SOC_NOPM, 0, 0, &dapm_s1_tdm_tx_3_mux_controls), \
	SND_SOC_DAPM_MUX("SPA1_DIN_MUX", \
		SND_SOC_NOPM, 0, 0, &dapm_spa1_din_mux_controls), \
	SND_SOC_DAPM_MUX("SPA2_DIN_MUX", \
		SND_SOC_NOPM, 0, 0, &dapm_spa2_din_mux_controls), \
	SND_SOC_DAPM_MUX("SPA3_DIN_MUX", \
		SND_SOC_NOPM, 0, 0, &dapm_spa3_din_mux_controls), \
	SND_SOC_DAPM_MUX("SPA4_DIN_MUX", \
		SND_SOC_NOPM, 0, 0, &dapm_spa4_din_mux_controls) \

#define AUDIO_SWITCH_WIDGET \
	SND_SOC_DAPM_SWITCH_E("I2S1_TX_TDM_SWITCH", SND_SOC_NOPM, 0, 0, \
		&dapm_s1_tx_tdm_switch_controls, s1_tx_tdm_power_event, \
		(SND_SOC_DAPM_PRE_PMU | SND_SOC_DAPM_POST_PMD)), \
	SND_SOC_DAPM_SWITCH_E("I2S1_TX_I2S_SWITCH", SND_SOC_NOPM, 0, 0, \
		&dapm_s1_tx_i2s_switch_controls, s1_tx_i2s_power_event, \
		(SND_SOC_DAPM_PRE_PMU | SND_SOC_DAPM_POST_PMD)), \
	SND_SOC_DAPM_SWITCH_E("I2S1_TX_TDM256_SWITCH", SND_SOC_NOPM, 0, 0, \
		&dapm_s1_tx_tdm256_switch_controls, s1_tx_tdm256_power_event, \
		(SND_SOC_DAPM_PRE_PMU | SND_SOC_DAPM_POST_PMD)), \
	SND_SOC_DAPM_SWITCH_E("AUDIO_2PA_DN_SWITCH", SND_SOC_NOPM, 0, 0, \
		&dapm_audio_2pa_dn_switch_controls, audio_2pa_dn_power_event, \
		(SND_SOC_DAPM_PRE_PMU | SND_SOC_DAPM_POST_PMD)), \
	SND_SOC_DAPM_SWITCH_E("AUDIO_4PA_DN_SWITCH", SND_SOC_NOPM, 0, 0, \
		&dapm_audio_4pa_dn_switch_controls, audio_4pa_dn_power_event, \
		(SND_SOC_DAPM_PRE_PMU | SND_SOC_DAPM_POST_PMD)), \
	SND_SOC_DAPM_SWITCH_E("IV_2PA_48K_SWITCH", SND_SOC_NOPM, 0, 0, \
		&dapm_iv_2pa_48k_switch_controls, iv_2pa_48k_power_event, \
		(SND_SOC_DAPM_PRE_PMU | SND_SOC_DAPM_POST_PMD)), \
	SND_SOC_DAPM_SWITCH_E("EC_2PA_SWITCH", SND_SOC_NOPM, 0, 0, \
		&dapm_ec_2pa_switch_controls, ec_2pa_power_event, \
		(SND_SOC_DAPM_PRE_PMU | SND_SOC_DAPM_POST_PMD)), \
	SND_SOC_DAPM_SWITCH_E("EC_4PA_SWITCH", SND_SOC_NOPM, 0, 0, \
		&dapm_ec_4pa_switch_controls, ec_4pa_power_event, \
		(SND_SOC_DAPM_PRE_PMU | SND_SOC_DAPM_POST_PMD)), \
	SND_SOC_DAPM_SWITCH_E("ASP_CODEC_IV_4PA_SWITCH", SND_SOC_NOPM, 0, 0, \
		&dapm_iv_4pa_switch_controls, iv_4pa_power_event, \
		(SND_SOC_DAPM_PRE_PMU | SND_SOC_DAPM_POST_PMD)), \
	SND_SOC_DAPM_SWITCH_E("CARKIT_48K_SWITCH", SND_SOC_NOPM, 0, 0, \
		&dapm_carkit_48k_switch_controls, carkit_48k_power_event, \
		(SND_SOC_DAPM_PRE_PMU | SND_SOC_DAPM_POST_PMD)), \
	SND_SOC_DAPM_SWITCH_E("A2DP_OFFLOAD_SWITCH", SND_SOC_NOPM, 0, 0, \
		&dapm_a2dp_offload_switch_controls, a2dp_offload_power_event, \
		(SND_SOC_DAPM_PRE_PMU | SND_SOC_DAPM_POST_PMD)), \
	SND_SOC_DAPM_SWITCH_E("ULTRASONIC_DN_SWITCH", SND_SOC_NOPM, 0, 0, \
		&dapm_ultrasonic_dn_switch_controls, ultrasonic_dn_power_event, \
		(SND_SOC_DAPM_PRE_PMU | SND_SOC_DAPM_POST_PMD)) \

#define SRC_WIDGET \
	SND_SOC_DAPM_PGA_S("AUDIO_DN_0_SRC_EN", 0, CODEC_CLK_EN2_REG, \
		AUDIO_0_DN_SRCUP_CLKEN_OFFSET, 0, NULL, 0), \
	SND_SOC_DAPM_PGA_S("AUDIO_DN_1_SRC_EN", 0, CODEC_CLK_EN2_REG, \
		AUDIO_1_DN_SRCUP_CLKEN_OFFSET, 0, NULL, 0), \
	SND_SOC_DAPM_PGA_S("AUDIO_DN_2_SRC_EN", 0, CODEC_CLK_EN2_REG, \
		AUDIO_2_DN_SRCUP_CLKEN_OFFSET, 0, NULL, 0), \
	SND_SOC_DAPM_PGA_S("AUDIO_DN_3_SRC_EN", 0, CODEC_CLK_EN5_REG, \
		AUDIO_3_DN_SRCUP_CLKEN_OFFSET, 0, NULL, 0), \
	SND_SOC_DAPM_PGA_S("CODEC_DN_0_SRC_EN", 0, CODEC_CLK_EN2_REG, \
		CODEC_0_DN_SRCUP_CLKEN_OFFSET, 0, NULL, 0), \
	SND_SOC_DAPM_PGA_S("CODEC_DN_1_SRC_EN", 0, CODEC_CLK_EN2_REG, \
		CODEC_1_DN_SRCUP_CLKEN_OFFSET, 0, NULL, 0), \
	SND_SOC_DAPM_PGA_S("CODEC_DN_2_SRC_EN", 0, CODEC_CLK_EN2_REG, \
		CODEC_2_DN_SRCUP_CLKEN_OFFSET, 0, NULL, 0), \
	SND_SOC_DAPM_PGA_S("CODEC_DN_3_SRC_EN", 0, CODEC_CLK_EN5_REG, \
		CODEC_3_DN_SRCUP_CLKEN_OFFSET, 0, NULL, 0) \

#define PA_PGA_WIDGET \
	SND_SOC_DAPM_PGA_S("AUDIO_0_DN_PGA", 0, CODEC_CLK_EN1_REG, \
		AUDIO_0_DN_PGA_CLKEN_OFFSET, 0, NULL, 0), \
	SND_SOC_DAPM_PGA_S("AUDIO_1_DN_PGA", 0, CODEC_CLK_EN1_REG, \
		AUDIO_1_DN_PGA_CLKEN_OFFSET, 0, NULL, 0), \
	SND_SOC_DAPM_PGA_S("AUDIO_2_DN_PGA", 0, CODEC_CLK_EN1_REG, \
		AUDIO_2_DN_PGA_CLKEN_OFFSET, 0, NULL, 0), \
	SND_SOC_DAPM_PGA_S("AUDIO_3_DN_PGA", 0, CODEC_CLK_EN5_REG, \
		AUDIO_3_DN_PGA_CLKEN_OFFSET, 0, NULL, 0), \
	SND_SOC_DAPM_PGA_S("CODEC_0_DN_PGA", 0, CODEC_CLK_EN1_REG, \
		CODEC_0_DN_PGA_CLKEN_OFFSET, 0, NULL, 0), \
	SND_SOC_DAPM_PGA_S("CODEC_1_DN_PGA", 0, CODEC_CLK_EN1_REG, \
		CODEC_1_DN_PGA_CLKEN_OFFSET, 0, NULL, 0), \
	SND_SOC_DAPM_PGA_S("CODEC_2_DN_PGA", 0, CODEC_CLK_EN1_REG, \
		CODEC_2_DN_PGA_CLKEN_OFFSET, 0, NULL, 0), \
	SND_SOC_DAPM_PGA_S("CODEC_3_DN_PGA", 0, CODEC_CLK_EN5_REG, \
		CODEC_3_DN_PGA_CLKEN_OFFSET, 0, NULL, 0), \
	SND_SOC_DAPM_PGA_S("ULTR_DN_PGA", 0, CODEC_CLK_EN1_REG, \
		ULTR_DN_PGA_CLKEN_OFFSET, 0, NULL, 0) \

static int pll_supply_power_mode_event(struct snd_soc_dapm_widget *w,
	struct snd_kcontrol *kcontrol, int event)
{
	struct snd_soc_component *codec = NULL;
	struct codec_data *priv = NULL;

	codec = snd_soc_dapm_to_component(w->dapm);
	priv = snd_soc_component_get_drvdata(codec);
	if (priv->platform_type == UDP_PLATFORM) {
		switch (event) {
		case SND_SOC_DAPM_PRE_PMU:
			priv->pm.have_dapm = true;
			break;
		case SND_SOC_DAPM_POST_PMD:
			priv->pm.have_dapm = false;
			break;
		default:
			AUDIO_LOGE("power mode event err: %d", event);
			break;
		}
	} else {
		AUDIO_LOGI("fpga clk is always on\n");
	}

	return 0;
}

static int i2s1_supply_power_mode_event(struct snd_soc_dapm_widget *w,
	struct snd_kcontrol *kcontrol, int event)
{
	struct snd_soc_component *codec = snd_soc_dapm_to_component(w->dapm);
	unused_parameter(kcontrol);
 
	switch (event) {
	case SND_SOC_DAPM_PRE_PMU:
		snd_soc_component_update_bits_with_lock(codec, CODEC_CLK_EN0_REG,
			BIT(I2S1_PCM_CLKEN_OFFSET), BIT(I2S1_PCM_CLKEN_OFFSET));
		break;
	case SND_SOC_DAPM_POST_PMD:
		snd_soc_component_update_bits_with_lock(codec, CODEC_CLK_EN0_REG,
			BIT(I2S1_PCM_CLKEN_OFFSET), 0);
		break;
	default:
		break;
	}
	AUDIO_LOGI("power event: %d", event);
 
	return 0;
}

#define AUDIO_DRV_WIDGET \
	SND_SOC_DAPM_OUT_DRV("I2S1_TX_DRV", \
		I2S1_CTRL_REG, I2S1_IF_TX_ENA_OFFSET, 0, NULL, 0) \

#define SUPPLY_WIDGET \
	SND_SOC_DAPM_SUPPLY_S("I2S1_SUPPLY", \
		0, SND_SOC_NOPM, 0, 0, i2s1_supply_power_mode_event, \
		(SND_SOC_DAPM_PRE_PMU | SND_SOC_DAPM_POST_PMD)), \
	SND_SOC_DAPM_SUPPLY_S("I2S1_RX_SUPPLY", \
		0, I2S1_CTRL_REG, I2S1_IF_RX_ENA_OFFSET, 0, NULL, 0), \
	SND_SOC_DAPM_SUPPLY_S("PLL", \
		0, SND_SOC_NOPM, 0, 0, pll_supply_power_mode_event, \
		(SND_SOC_DAPM_PRE_PMU | SND_SOC_DAPM_POST_PMD))

static int spa_bypass_power_event(struct snd_soc_dapm_widget *w,
	struct snd_kcontrol *kcontrol, int event)
{
	struct snd_soc_component *codec = snd_soc_dapm_to_component(w->dapm);
	AUDIO_LOGI("spa bypass power event enter %d", event);
	unused_parameter(kcontrol);

	switch (event) {
		case SND_SOC_DAPM_PRE_PMU:
			snd_soc_component_update_bits_with_lock(codec, SPA_PROTECT_WAY_CLK_EN_REG,
				BIT(SPA_PROTECT_WAY_CLK_EN_OFFSET), BIT(SPA_PROTECT_WAY_CLK_EN_OFFSET));
			snd_soc_component_update_bits_with_lock(codec, SPA_BYPASS_CTRL_REG,
				BIT(SPA0_BYPASS_OFFSET) | BIT(SPA1_BYPASS_OFFSET) | BIT(SPA2_BYPASS_OFFSET) | BIT(SPA3_BYPASS_OFFSET),
				BIT(SPA0_BYPASS_OFFSET) | BIT(SPA1_BYPASS_OFFSET) | BIT(SPA2_BYPASS_OFFSET) | BIT(SPA3_BYPASS_OFFSET));
			snd_soc_component_update_bits_with_lock(codec, SPA1_PARAMS_PASSTHROUGH_EN_REG,
				BIT(SPA1_PARAMS_PASSTHROUGH_EN_OFFSET), BIT(SPA1_PARAMS_PASSTHROUGH_EN_OFFSET));
			snd_soc_component_update_bits_with_lock(codec, SPA2_PARAMS_PASSTHROUGH_EN_REG,
				BIT(SPA2_PARAMS_PASSTHROUGH_EN_OFFSET), BIT(SPA2_PARAMS_PASSTHROUGH_EN_OFFSET));
			snd_soc_component_update_bits_with_lock(codec, SPA3_PARAMS_PASSTHROUGH_EN_REG,
				BIT(SPA3_PARAMS_PASSTHROUGH_EN_OFFSET), BIT(SPA3_PARAMS_PASSTHROUGH_EN_OFFSET));
			snd_soc_component_update_bits_with_lock(codec, SPA4_PARAMS_PASSTHROUGH_EN_REG,
				BIT(SPA4_PARAMS_PASSTHROUGH_EN_OFFSET), BIT(SPA3_PARAMS_PASSTHROUGH_EN_OFFSET));
			snd_soc_component_update_bits_with_lock(codec, SPA1_CG_EN_CFG,
				max_val_on_bit(SPA1_CFG_EN_LEN), 0 << SPA1_PRE_PROCESS_CG_EN_OFFSET);
			snd_soc_component_update_bits_with_lock(codec, SPA2_CG_EN_CFG,
				max_val_on_bit(SPA2_CFG_EN_LEN), 0 << SPA2_PRE_PROCESS_CG_EN_OFFSET);
			snd_soc_component_update_bits_with_lock(codec, SPA3_CG_EN_CFG,
				max_val_on_bit(SPA3_CFG_EN_LEN), 0 << SPA3_PRE_PROCESS_CG_EN_OFFSET);
			snd_soc_component_update_bits_with_lock(codec, SPA4_CG_EN_CFG,
				max_val_on_bit(SPA4_CFG_EN_LEN), 0 << SPA4_PRE_PROCESS_CG_EN_OFFSET);
			snd_soc_component_update_bits_with_lock(codec, SPA1_PARAMS_PASSTHROUGH_EN_REG,
				BIT(SPA1_PARAMS_PASSTHROUGH_EN_OFFSET), 0);
			snd_soc_component_update_bits_with_lock(codec, SPA2_PARAMS_PASSTHROUGH_EN_REG,
				BIT(SPA2_PARAMS_PASSTHROUGH_EN_OFFSET), 0);
			snd_soc_component_update_bits_with_lock(codec, SPA3_PARAMS_PASSTHROUGH_EN_REG,
				BIT(SPA3_PARAMS_PASSTHROUGH_EN_OFFSET), 0);
			snd_soc_component_update_bits_with_lock(codec, SPA4_PARAMS_PASSTHROUGH_EN_REG,
				BIT(SPA4_PARAMS_PASSTHROUGH_EN_OFFSET), 0);
			break;
		case SND_SOC_DAPM_POST_PMD:
			snd_soc_component_update_bits_with_lock(codec, SPA_BYPASS_CTRL_REG,
				BIT(SPA0_BYPASS_OFFSET) | BIT(SPA1_BYPASS_OFFSET) |
				BIT(SPA2_BYPASS_OFFSET) | BIT(SPA3_BYPASS_OFFSET), 0);
			 snd_soc_component_update_bits_with_lock(codec, SPA_PROTECT_WAY_CLK_EN_REG,
				BIT(SPA_PROTECT_WAY_CLK_EN_OFFSET), 0);
			break;
		default:
			AUDIO_LOGW("power event err:%d", event);
			break;
	}

	return 0;
}

static const struct snd_kcontrol_new dapm_spk_pa_bypass_switch_controls =
	SOC_DAPM_SINGLE("ENABLE", CODEC_VIR2_REG_ADDR, SPK_PA_BYPASS_BIT, 1, 0);
static const struct snd_kcontrol_new dapm_rcv_pa_bypass_switch_controls =
	SOC_DAPM_SINGLE("ENABLE", CODEC_VIR2_REG_ADDR, RCV_PA_BYPASS_BIT, 1, 0);

#define SPK_PA_BYPASS_SWITCH_WIDGET \
	SND_SOC_DAPM_SWITCH_E("SPK_PA_BYPASS_SWITCH", SND_SOC_NOPM, 0, 0, \
		&dapm_spk_pa_bypass_switch_controls, spa_bypass_power_event, \
		(SND_SOC_DAPM_PRE_PMU | SND_SOC_DAPM_POST_PMD)) \

#define RCV_PA_BYPASS_SWITCH_WIDGET \
	SND_SOC_DAPM_SWITCH_E("RCV_PA_BYPASS_SWITCH", SND_SOC_NOPM, 0, 0, \
		&dapm_rcv_pa_bypass_switch_controls, spa_bypass_power_event, \
		(SND_SOC_DAPM_PRE_PMU | SND_SOC_DAPM_POST_PMD)) \

static const struct snd_soc_dapm_widget audio_dapm_widgets[] = {
	AUDIO_INPUT_OUTPUT_WIDGET,
	AUDIO_MIXER_WIDGET,
	AUDIO_MUX_WIDGET,
	AUDIO_SWITCH_WIDGET,
	SRC_WIDGET,
	PA_PGA_WIDGET,
	AUDIO_DRV_WIDGET,
	SUPPLY_WIDGET,
	SPK_PA_BYPASS_SWITCH_WIDGET,
	RCV_PA_BYPASS_SWITCH_WIDGET,
};

static const struct snd_codec_resource audio_resource = {
	"ASP_CODEC_PA_WITH_PA_ALG",
	audio_play_kcontrols,
	ARRAY_SIZE(audio_play_kcontrols),
	audio_dapm_widgets,
	ARRAY_SIZE(audio_dapm_widgets),
};

int __init add_pa_path_in_list(void)
{
	return add_codec_base_dapm_list(&audio_resource);
}

device_initcall(add_pa_path_in_list);
