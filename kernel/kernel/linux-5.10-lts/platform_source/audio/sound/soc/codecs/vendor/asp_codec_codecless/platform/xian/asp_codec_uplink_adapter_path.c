/*
 * asp_codec_uplink_adapter_path.c -- codec driver
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

#include <linux/gpio.h>
#include <linux/of_gpio.h>

#include "audio_log.h"
#include "asp_codec_type.h"
#include "asp_codec_store.h"

#define LOG_TAG "asp_codec_uplink_adapter_path"

#define UP_SRC_KCONTROLS \
	SOC_SINGLE("VOICE_L_UP_SRC_CLKEN", \
		CODEC_CLK_EN1_REG, VOICE_L_UP_SRCDN_CLKEN_OFFSET, \
		max_val_on_bit(VOICE_L_UP_SRCDN_CLKEN_LEN), 0), \
	SOC_SINGLE("VOICE_L_UP_SRC_DOUT", \
		FS_CTRL4_REG, FS_VOICE_UP_L_SRCDN_DOUT_OFFSET, \
		max_val_on_bit(FS_VOICE_UP_L_SRCDN_DOUT_LEN), 0), \
	SOC_SINGLE("VOICE_L_UP_SRC_DIN", \
		FS_CTRL4_REG, FS_VOICE_UP_L_SRCDN_DIN_OFFSET, \
		max_val_on_bit(FS_VOICE_UP_L_SRCDN_DIN_LEN), 0), \
	SOC_SINGLE("VOICE_L_UP_SRC_MODE", \
		SRCDN_CTRL0_REG, VOICE_L_UP_SRCDN_SRC_MODE_OFFSET, \
		max_val_on_bit(VOICE_L_UP_SRCDN_SRC_MODE_LEN), 0), \
	SOC_SINGLE("VOICE_R_UP_SRC_CLKEN", \
		CODEC_CLK_EN1_REG, VOICE_R_UP_SRCDN_CLKEN_OFFSET, \
		max_val_on_bit(VOICE_R_UP_SRCDN_CLKEN_LEN), 0), \
	SOC_SINGLE("VOICE_R_UP_SRC_DOUT", \
		FS_CTRL4_REG, FS_VOICE_UP_R_SRCDN_DOUT_OFFSET, \
		max_val_on_bit(FS_VOICE_UP_R_SRCDN_DOUT_LEN), 0), \
	SOC_SINGLE("VOICE_R_UP_SRC_DIN", \
		FS_CTRL4_REG, FS_VOICE_UP_R_SRCDN_DIN_OFFSET, \
		max_val_on_bit(FS_VOICE_UP_R_SRCDN_DIN_LEN), 0), \
	SOC_SINGLE("VOICE_R_UP_SRC_MODE", \
		SRCDN_CTRL0_REG, VOICE_R_UP_SRCDN_SRC_MODE_OFFSET, \
		max_val_on_bit(VOICE_R_UP_SRCDN_SRC_MODE_LEN), 0), \
	SOC_SINGLE("AUDIO_L_UP_SRC_CLKEN", \
		CODEC_CLK_EN1_REG, AUDIO_L_UP_SRCDN_CLKEN_OFFSET, \
		max_val_on_bit(AUDIO_L_UP_SRCDN_CLKEN_LEN), 0), \
	SOC_SINGLE("AUDIO_L_UP_SRC_DOUT", \
		FS_CTRL4_REG, FS_AUDIO_UP_L_SRCDN_DOUT_OFFSET, \
		max_val_on_bit(FS_AUDIO_UP_L_SRCDN_DOUT_LEN), 0), \
	SOC_SINGLE("AUDIO_L_UP_SRC_DIN", \
		FS_CTRL4_REG, FS_AUDIO_UP_L_SRCDN_DIN_OFFSET, \
		max_val_on_bit(FS_AUDIO_UP_L_SRCDN_DIN_LEN), 0), \
	SOC_SINGLE("AUDIO_L_UP_SRC_MODE", \
		SRCDN_CTRL0_REG, AUDIO_L_UP_SRCDN_SRC_MODE_OFFSET, \
		max_val_on_bit(AUDIO_L_UP_SRCDN_SRC_MODE_LEN), 0), \
	SOC_SINGLE("AUDIO_R_UP_SRC_CLKEN", \
		CODEC_CLK_EN1_REG, AUDIO_R_UP_SRCDN_CLKEN_OFFSET, \
		max_val_on_bit(AUDIO_R_UP_SRCDN_CLKEN_LEN), 0), \
	SOC_SINGLE("AUDIO_R_UP_SRC_DOUT", \
		FS_CTRL4_REG, FS_AUDIO_UP_R_SRCDN_DOUT_OFFSET, \
		max_val_on_bit(FS_AUDIO_UP_R_SRCDN_DOUT_LEN), 0), \
	SOC_SINGLE("AUDIO_R_UP_SRC_DIN", \
		FS_CTRL4_REG, FS_AUDIO_UP_R_SRCDN_DIN_OFFSET, \
		max_val_on_bit(FS_AUDIO_UP_R_SRCDN_DIN_LEN), 0), \
	SOC_SINGLE("AUDIO_R_UP_SRC_MODE", \
		SRCDN_CTRL0_REG, AUDIO_R_UP_SRCDN_SRC_MODE_OFFSET, \
		max_val_on_bit(AUDIO_R_UP_SRCDN_SRC_MODE_LEN), 0), \
	SOC_SINGLE("MIC3_UP_SRC_CLKEN", \
		CODEC_CLK_EN1_REG, MIC3_UP_SRCDN_CLKEN_OFFSET, \
		max_val_on_bit(MIC3_UP_SRCDN_CLKEN_LEN), 0), \
	SOC_SINGLE("MIC3_UP_SRC_DOUT", \
		FS_CTRL5_REG, FS_MIC3_SRCDN_DOUT_OFFSET, \
		max_val_on_bit(FS_MIC3_SRCDN_DOUT_LEN), 0), \
	SOC_SINGLE("MIC3_UP_SRC_MODE", \
		SRCDN_CTRL0_REG, MIC3_UP_SRCDN_SRC_MODE_OFFSET, \
		max_val_on_bit(MIC3_UP_SRCDN_SRC_MODE_LEN), 0), \
	SOC_SINGLE("MIC4_UP_SRC_CLKEN", \
		CODEC_CLK_EN1_REG, MIC4_UP_SRCDN_CLKEN_OFFSET, \
		max_val_on_bit(MIC4_UP_SRCDN_CLKEN_LEN), 0), \
	SOC_SINGLE("MIC4_UP_SRC_DOUT", \
		FS_CTRL5_REG, FS_MIC4_SRCDN_DOUT_OFFSET, \
		max_val_on_bit(FS_MIC4_SRCDN_DOUT_LEN), 0), \
	SOC_SINGLE("MIC4_UP_SRC_MODE", \
		SRCDN_CTRL0_REG, MIC4_UP_SRCDN_SRC_MODE_OFFSET, \
		max_val_on_bit(MIC4_UP_SRCDN_SRC_MODE_LEN), 0), \
	SOC_SINGLE("MIC5_UP_SRC_CLKEN", \
		CODEC_CLK_EN1_REG, MIC5_UP_SRCDN_CLKEN_OFFSET, \
		max_val_on_bit(MIC5_UP_SRCDN_CLKEN_LEN), 0), \
	SOC_SINGLE("MIC5_UP_SRC_DOUT", \
		FS_CTRL5_REG, FS_MIC5_SRCDN_DOUT_OFFSET, \
		max_val_on_bit(FS_MIC5_SRCDN_DOUT_LEN), 0), \
	SOC_SINGLE("MIC5_UP_SRC_MODE", \
		SRCDN_CTRL0_REG, MIC5_UP_SRCDN_SRC_MODE_OFFSET, \
		max_val_on_bit(MIC5_UP_SRCDN_SRC_MODE_LEN), 0), \
	SOC_SINGLE("MIC6_UP_SRC_CLKEN", \
		CODEC_CLK_EN1_REG, MIC6_UP_SRCDN_CLKEN_OFFSET, \
		max_val_on_bit(MIC6_UP_SRCDN_CLKEN_LEN), 0), \
	SOC_SINGLE("MIC6_UP_SRC_DOUT", \
		FS_CTRL5_REG, FS_MIC6_SRCDN_DOUT_OFFSET, \
		max_val_on_bit(FS_MIC6_SRCDN_DOUT_LEN), 0), \
	SOC_SINGLE("MIC6_UP_SRC_MODE", \
		SRCDN_CTRL0_REG, MIC6_UP_SRCDN_SRC_MODE_OFFSET, \
		max_val_on_bit(MIC6_UP_SRCDN_SRC_MODE_LEN), 0) \

static const struct snd_kcontrol_new uplink_adapter_snd_kcontrols[] = {
	UP_SRC_KCONTROLS,
};

static const char * const au_up_l_mux_texts[] = {
	"MIC1_ADC",
	"MIC2_ADC",
	"I2S2_RX_L",
	"MIXER4_SPA0_DOUT",
	"AUDIO_0_SRCUP_DOUT",
	"MIC3_ADC",
	"MIC4_ADC",
	"SPA0_MIXER2_DOUT",
};

static const struct soc_enum au_up_l_mux_enum =
	SOC_ENUM_SINGLE(CODEC_DIN_MUX0_REG, AU_UP_L_DIN_SEL_OFFSET,
		ARRAY_SIZE(au_up_l_mux_texts), au_up_l_mux_texts);

static const struct snd_kcontrol_new dapm_au_up_l_mux_controls =
	SOC_DAPM_ENUM("Mux", au_up_l_mux_enum);

static const char * const au_up_r_mux_texts[] = {
	"MIC1_ADC",
	"MIC2_ADC",
	"I2S2_RX_R",
	"MIXER4_SPA1_DOUT",
	"AUDIO_1_SRCUP_DOUT",
	"MIC3_ADC",
	"MIC4_ADC",
	"SPA1_MIXER2_DOUT",
};

static const struct soc_enum au_up_r_mux_enum =
	SOC_ENUM_SINGLE(CODEC_DIN_MUX0_REG, AU_UP_R_DIN_SEL_OFFSET,
		ARRAY_SIZE(au_up_r_mux_texts), au_up_r_mux_texts);

static const struct snd_kcontrol_new dapm_au_up_r_mux_controls =
	SOC_DAPM_ENUM("Mux", au_up_r_mux_enum);

static const char * const vo_up_l_mux_texts[] = {
	"MIC1_ADC",
	"MIC2_ADC",
	"I2S2_RX_L",
	"MIXER4_SPA0_DOUT",
	"CODEC_0_SRCUP_DOUT",
	"MIC3_ADC",
	"MIC4_ADC",
	"SPA2_MIXER2_DOUT",
};

static const struct soc_enum vo_up_l_mux_enum =
	SOC_ENUM_SINGLE(CODEC_DIN_MUX0_REG, VO_UP_L_DIN_SEL_OFFSET,
		ARRAY_SIZE(vo_up_l_mux_texts), vo_up_l_mux_texts);

static const struct snd_kcontrol_new dapm_vo_up_l_mux_controls =
	SOC_DAPM_ENUM("Mux", vo_up_l_mux_enum);

static const char * const vo_up_r_mux_texts[] = {
	"MIC1_ADC",
	"MIC2_ADC",
	"I2S2_RX_R",
	"MIXER4_SPA1_DOUT",
	"CODEC_1_SRCUP_DOUT",
	"MIC3_ADC",
	"MIC4_ADC",
	"SPA3_MIXER2_DOUT",
};

static const struct soc_enum vo_up_r_mux_enum =
	SOC_ENUM_SINGLE(CODEC_DIN_MUX0_REG, VO_UP_R_DIN_SEL_OFFSET,
		ARRAY_SIZE(vo_up_r_mux_texts), vo_up_r_mux_texts);

static const struct snd_kcontrol_new dapm_vo_up_r_mux_controls =
	SOC_DAPM_ENUM("Mux", vo_up_r_mux_enum);

static const char * const mic3_mux_texts[] = {
	"MIC1_ADC",
	"MIC2_ADC",
	"MIC3_ADC",
	"MIC4_ADC",
	"MIC5_ADC",
	"MIC6_ADC",
	"MIC7_ADC",
	"MIC8_ADC",
};

static const struct soc_enum mic3_mux_enum =
	SOC_ENUM_SINGLE(CODEC_DIN_MUX0_REG, MIC3_DIN_SEL_OFFSET,
		ARRAY_SIZE(mic3_mux_texts), mic3_mux_texts);

static const struct snd_kcontrol_new dapm_mic3_mux_controls =
	SOC_DAPM_ENUM("Mux", mic3_mux_enum);

static const char * const mic4_mux_texts[] = {
	"MIC1_ADC",
	"MIC2_ADC",
	"MIC3_ADC",
	"MIC4_ADC",
	"MIC5_ADC",
	"MIC6_ADC",
	"MIC7_ADC",
	"MIC8_ADC",
};

static const struct soc_enum mic4_mux_enum =
	SOC_ENUM_SINGLE(CODEC_DIN_MUX0_REG, MIC4_DIN_SEL_OFFSET,
		ARRAY_SIZE(mic4_mux_texts), mic4_mux_texts);

static const struct snd_kcontrol_new dapm_mic4_mux_controls =
	SOC_DAPM_ENUM("Mux", mic4_mux_enum);

static const char * const mic5_mux_texts[] = {
	"MIC1_ADC",
	"MIC2_ADC",
	"MIC3_ADC",
	"MIC4_ADC",
	"MIC5_ADC",
	"MIC6_ADC",
	"MIC7_ADC",
	"MIC8_ADC",
	"SPA0_DOUT"
};

static const struct soc_enum mic5_mux_enum =
	SOC_ENUM_SINGLE(CODEC_DIN_MUX1_REG, MIC5_DIN_SEL_OFFSET,
		ARRAY_SIZE(mic5_mux_texts), mic5_mux_texts);

static const struct snd_kcontrol_new dapm_mic5_mux_controls =
	SOC_DAPM_ENUM("Mux", mic5_mux_enum);

static const char * const mic6_mux_texts[] = {
	"MIC1_ADC",
	"MIC2_ADC",
	"MIC3_ADC",
	"MIC4_ADC",
	"MIC5_ADC",
	"MIC6_ADC",
	"MIC7_ADC",
	"MIC8_ADC",
	"SPA1_DOUT"
};

static const struct soc_enum mic6_mux_enum =
	SOC_ENUM_SINGLE(CODEC_DIN_MUX1_REG, MIC6_DIN_SEL_OFFSET,
		ARRAY_SIZE(mic6_mux_texts), mic6_mux_texts);

static const struct snd_kcontrol_new dapm_mic6_mux_controls =
	SOC_DAPM_ENUM("Mux", mic6_mux_enum);

static const char * const ultr_up_mux_texts[] = {
	"DMIC1_L",
	"DMIC1_R",
	"DMIC2_L",
	"DMIC2_R",
	"DMIC3_L",
	"DMIC3_R",
	"AU0_L",
	"AU0_R",
	"AU0_MIC3",
};

static const struct soc_enum ultr_up_mux_enum =
	SOC_ENUM_SINGLE(CODEC_ADC1_DC_OFFSET_REG, ADC_ULTR_CIC_DIN_SEL_OFFSET,
		ARRAY_SIZE(ultr_up_mux_texts), ultr_up_mux_texts);

static const struct snd_kcontrol_new dapm_ultr_up_mux_controls =
	SOC_DAPM_ENUM("Mux", ultr_up_mux_enum);

static const char * const adc1_din_mux_texts[] = {
	"CODEC_0_DN",
	"ADC1_CIC",
};

static const struct soc_enum adc1_din_mux_enum =
	SOC_ENUM_SINGLE(CODEC_DIN_MUX0_REG, ADC1_DIN_SEL_OFFSET,
		ARRAY_SIZE(adc1_din_mux_texts), adc1_din_mux_texts);

static const struct snd_kcontrol_new dapm_adc1_din_mux_controls =
	SOC_DAPM_ENUM("Mux", adc1_din_mux_enum);

static const char * const adc2_din_mux_texts[] = {
	"CODEC_1_DN",
	"ADC2_CIC",
};

static const struct soc_enum adc2_din_mux_enum =
	SOC_ENUM_SINGLE(CODEC_DIN_MUX0_REG, ADC2_DIN_SEL_OFFSET,
		ARRAY_SIZE(adc2_din_mux_texts), adc2_din_mux_texts);

static const struct snd_kcontrol_new dapm_adc2_din_mux_controls =
	SOC_DAPM_ENUM("Mux", adc2_din_mux_enum);

static const char * const adc3_din_mux_texts[] = {
	"AUDIO_0_DN",
	"ADC3_CIC",
};

static const struct soc_enum adc3_din_mux_enum =
	SOC_ENUM_SINGLE(CODEC_DIN_MUX0_REG, ADC3_DIN_SEL_OFFSET,
		ARRAY_SIZE(adc3_din_mux_texts), adc3_din_mux_texts);

static const struct snd_kcontrol_new dapm_adc3_din_mux_controls =
	SOC_DAPM_ENUM("Mux", adc3_din_mux_enum);

static const char * const adc4_din_mux_texts[] = {
	"AUDIO_1_DN",
	"ADC4_CIC",
};

static const struct soc_enum adc4_din_mux_enum =
	SOC_ENUM_SINGLE(CODEC_DIN_MUX0_REG, ADC4_DIN_SEL_OFFSET,
		ARRAY_SIZE(adc4_din_mux_texts), adc4_din_mux_texts);

static const struct snd_kcontrol_new dapm_adc4_din_mux_controls =
	SOC_DAPM_ENUM("Mux", adc4_din_mux_enum);

static const char * const adc1_cic6_mux_texts[] = {
	"AU0_L",
	"AU0_R",
	"AU0_MIC3",
	"DMIC1_L",
};

static const struct soc_enum adc1_cic6_mux_enum =
	SOC_ENUM_SINGLE(CODEC_ADC1_DC_OFFSET_REG, ADC1_CIC_DIN_SEL_OFFSET,
		ARRAY_SIZE(adc1_cic6_mux_texts), adc1_cic6_mux_texts);

static const struct snd_kcontrol_new adc1_cic6_mux_controls =
	SOC_DAPM_ENUM("Mux", adc1_cic6_mux_enum);

static const char * const adc2_cic6_mux_texts[] = {
	"AU0_L",
	"AU0_R",
	"AU0_MIC3",
	"DMIC1_R",
};

static const struct soc_enum adc2_cic6_mux_enum =
	SOC_ENUM_SINGLE(CODEC_ADC2_DC_OFFSET_REG, ADC2_CIC_DIN_SEL_OFFSET,
		ARRAY_SIZE(adc2_cic6_mux_texts), adc2_cic6_mux_texts);

static const struct snd_kcontrol_new adc2_cic6_mux_controls =
	SOC_DAPM_ENUM("Mux", adc2_cic6_mux_enum);

static const char * const adc3_cic6_mux_texts[] = {
	"AU0_L",
	"AU0_R",
	"AU0_MIC3",
	"DMIC2_L",
};

static const struct soc_enum adc3_cic6_mux_enum =
	SOC_ENUM_SINGLE(CODEC_ADC3_DC_OFFSET_REG, ADC3_CIC_DIN_SEL_OFFSET,
		ARRAY_SIZE(adc3_cic6_mux_texts), adc3_cic6_mux_texts);

static const struct snd_kcontrol_new adc3_cic6_mux_controls =
	SOC_DAPM_ENUM("Mux", adc3_cic6_mux_enum);

static const char * const adc4_cic6_mux_texts[] = {
	"AU0_L",
	"AU0_R",
	"AU0_MIC3",
	"DMIC2_R",
};

static const struct soc_enum adc4_cic6_mux_enum =
	SOC_ENUM_SINGLE(CODEC_ADC4_DC_OFFSET_REG, ADC4_CIC_DIN_SEL_OFFSET,
		ARRAY_SIZE(adc4_cic6_mux_texts), adc4_cic6_mux_texts);

static const struct snd_kcontrol_new adc4_cic6_mux_controls =
	SOC_DAPM_ENUM("Mux", adc4_cic6_mux_enum);

static const char * const adc5_cic6_mux_texts[] = {
	"AU0_L",
	"AU0_R",
	"AU0_MIC3",
	"DMIC3_L",
};

static const struct soc_enum adc5_cic6_mux_enum =
	SOC_ENUM_SINGLE(CODEC_ADC5_DC_OFFSET_REG, ADC5_CIC_DIN_SEL_OFFSET,
		ARRAY_SIZE(adc5_cic6_mux_texts), adc5_cic6_mux_texts);

static const struct snd_kcontrol_new adc5_cic6_mux_controls =
	SOC_DAPM_ENUM("Mux", adc5_cic6_mux_enum);

static const char * const adc6_cic6_mux_texts[] = {
	"AU0_L",
	"AU0_R",
	"AU0_MIC3",
	"DMIC3_R",
};

static const struct soc_enum adc6_cic6_mux_enum =
	SOC_ENUM_SINGLE(CODEC_ADC6_DC_OFFSET_REG, ADC6_CIC_DIN_SEL_OFFSET,
		ARRAY_SIZE(adc6_cic6_mux_texts), adc6_cic6_mux_texts);

static const struct snd_kcontrol_new adc6_cic6_mux_controls =
	SOC_DAPM_ENUM("Mux", adc6_cic6_mux_enum);

#define UP_MUX_WIDGET \
	SND_SOC_DAPM_MUX("AUDIO_UP_L_MUX", \
		SND_SOC_NOPM, 0, 0, &dapm_au_up_l_mux_controls), \
	SND_SOC_DAPM_MUX("AUDIO_UP_R_MUX", \
		SND_SOC_NOPM, 0, 0, &dapm_au_up_r_mux_controls), \
	SND_SOC_DAPM_MUX("VOICE_UP_L_MUX", \
		SND_SOC_NOPM, 0, 0, &dapm_vo_up_l_mux_controls), \
	SND_SOC_DAPM_MUX("VOICE_UP_R_MUX", \
		SND_SOC_NOPM, 0, 0, &dapm_vo_up_r_mux_controls), \
	SND_SOC_DAPM_MUX("MIC3_MUX", \
		SND_SOC_NOPM, 0, 0, &dapm_mic3_mux_controls), \
	SND_SOC_DAPM_MUX("MIC4_MUX", \
		SND_SOC_NOPM, 0, 0, &dapm_mic4_mux_controls), \
	SND_SOC_DAPM_MUX("MIC5_MUX", \
		SND_SOC_NOPM, 0, 0, &dapm_mic5_mux_controls), \
	SND_SOC_DAPM_MUX("MIC6_MUX", \
		SND_SOC_NOPM, 0, 0, &dapm_mic6_mux_controls), \
	SND_SOC_DAPM_MUX("ULTR_UP_MUX", \
		SND_SOC_NOPM, 0, 0, &dapm_ultr_up_mux_controls), \
	SND_SOC_DAPM_MUX("ADC1_DIN_MUX", \
		SND_SOC_NOPM, 0, 0, &dapm_adc1_din_mux_controls), \
	SND_SOC_DAPM_MUX("ADC2_DIN_MUX", \
		SND_SOC_NOPM, 0, 0, &dapm_adc2_din_mux_controls), \
	SND_SOC_DAPM_MUX("ADC3_DIN_MUX", \
		SND_SOC_NOPM, 0, 0, &dapm_adc3_din_mux_controls), \
	SND_SOC_DAPM_MUX("ADC4_DIN_MUX", \
		SND_SOC_NOPM, 0, 0, &dapm_adc4_din_mux_controls), \
	SND_SOC_DAPM_MUX("ADC1_CIC6_MUX", \
		SND_SOC_NOPM, 0, 0, &adc1_cic6_mux_controls), \
	SND_SOC_DAPM_MUX("ADC2_CIC6_MUX", \
		SND_SOC_NOPM, 0, 0, &adc2_cic6_mux_controls), \
	SND_SOC_DAPM_MUX("ADC3_CIC6_MUX", \
		SND_SOC_NOPM, 0, 0, &adc3_cic6_mux_controls), \
	SND_SOC_DAPM_MUX("ADC4_CIC6_MUX", \
		SND_SOC_NOPM, 0, 0, &adc4_cic6_mux_controls), \
	SND_SOC_DAPM_MUX("ADC5_CIC6_MUX", \
		SND_SOC_NOPM, 0, 0, &adc5_cic6_mux_controls), \
	SND_SOC_DAPM_MUX("ADC6_CIC6_MUX", \
		SND_SOC_NOPM, 0, 0, &adc6_cic6_mux_controls) \

static const struct snd_kcontrol_new dapm_ultrasonic_up_switch_controls =
	SOC_DAPM_SINGLE("ENABLE", CODEC_VIR1_REG_ADDR, ULTRASONIC_UP_BIT, 1, 0);

static int ultrasonic_up_power_event(struct snd_soc_dapm_widget *w,
	struct snd_kcontrol *kcontrol, int event)
{
	struct snd_soc_component *codec = snd_soc_dapm_to_component(w->dapm);

	AUDIO_LOGI("ultrasonic_up power event %d", event);
	switch (event) {
	case SND_SOC_DAPM_PRE_PMU:
		snd_soc_component_update_bits_with_lock(codec, CODEC_CLK_EN0_REG,
			BIT(ULTR_UP_AFIFO_CLKEN_OFFSET), 0);
		udelay(CLEAR_FIFO_DELAY_LEN_US);
		snd_soc_component_update_bits_with_lock(codec, CODEC_CLK_EN2_REG,
			BIT(ADC_ULTR_CLKEN_OFFSET), BIT(ADC_ULTR_CLKEN_OFFSET));
		snd_soc_component_update_bits_with_lock(codec, CODEC_CLK_EN4_REG,
			BIT(ULTR_HPF_CLKEN_OFFSET), BIT(ULTR_HPF_CLKEN_OFFSET));
		snd_soc_component_update_bits_with_lock(codec, CODEC_CLK_EN0_REG,
			BIT(ULTR_UP_AFIFO_CLKEN_OFFSET), BIT(ULTR_UP_AFIFO_CLKEN_OFFSET));
		snd_soc_component_update_bits_with_lock(codec, ULTR_ADC_CTRL0_REG,
			BIT(ULTR_ADC_HBFVD_BYPASS_EN_OFFSET) |
			mask_on_bit(ULTR_ADC_CIC_GAIN_LEN, ULTR_ADC_CIC_GAIN_OFFSET),
			BIT(ULTR_ADC_HBFVD_BYPASS_EN_OFFSET) | (0x14 << ULTR_ADC_CIC_GAIN_OFFSET));
		break;
	case SND_SOC_DAPM_POST_PMD:
		snd_soc_component_update_bits_with_lock(codec, CODEC_CLK_EN0_REG,
			BIT(ULTR_UP_AFIFO_CLKEN_OFFSET), 0);
		snd_soc_component_update_bits_with_lock(codec, CODEC_CLK_EN4_REG,
			BIT(ULTR_HPF_CLKEN_OFFSET), 0);
		snd_soc_component_update_bits_with_lock(codec, CODEC_CLK_EN2_REG,
			BIT(ADC_ULTR_CLKEN_OFFSET), 0);
		break;
	default:
		AUDIO_LOGW("power event err:%d", event);
		break;
	}

	return 0;
}

#define ULTR_UP_SWITCH_WIDGET \
	SND_SOC_DAPM_SWITCH_E("ULTRASONIC_UP_SWITCH", SND_SOC_NOPM, 0, 0, \
		&dapm_ultrasonic_up_switch_controls, ultrasonic_up_power_event, \
		(SND_SOC_DAPM_PRE_PMU | SND_SOC_DAPM_POST_PMD)) \

static const struct snd_kcontrol_new dapm_bt_voice_32k_src_controls =
	SOC_DAPM_SINGLE("ENABLE", CODEC_VIR0_REG_ADDR, BT_VOICE_UP_32K_BIT, 1, 0);
static const struct snd_kcontrol_new dapm_bt_audio_48k_controls =
	SOC_DAPM_SINGLE("ENABLE", CODEC_VIR0_REG_ADDR, BT_AUDIO_UP_48K_BIT, 1, 0);

static void mic34_up_48k_set(struct snd_soc_component *codec)
{
	snd_soc_component_update_bits(codec, FS_CTRL1_REG,
		mask_on_bit(FS_MIC3_UP_AFIFO_LEN, FS_MIC3_UP_AFIFO_OFFSET) |
		mask_on_bit(FS_MIC4_UP_AFIFO_LEN, FS_MIC4_UP_AFIFO_OFFSET),
		0x3 << FS_MIC3_UP_AFIFO_OFFSET | 0x3 << FS_MIC4_UP_AFIFO_OFFSET);
	snd_soc_component_update_bits(codec, FS_CTRL2_REG,
		mask_on_bit(FS_MIC3_UP_PGA_LEN, FS_MIC3_UP_PGA_OFFSET) |
		mask_on_bit(FS_MIC4_UP_PGA_LEN, FS_MIC4_UP_PGA_OFFSET),
		0x3 << FS_MIC3_UP_PGA_OFFSET | 0x3 << FS_MIC4_UP_PGA_OFFSET);
}

static void audio_mic34_pga_clk_set(struct snd_soc_component *codec)
{
	snd_soc_component_update_bits_with_lock(codec, AUDIO_UP_PGA_CLKEN_REG,
		BIT(AUDIO_UP_L_PGA_CLKEN_OFFSET) | BIT(AUDIO_UP_R_PGA_CLKEN_OFFSET),
		BIT(AUDIO_UP_L_PGA_CLKEN_OFFSET) | BIT(AUDIO_UP_R_PGA_CLKEN_OFFSET));
	snd_soc_component_update_bits_with_lock(codec, MIC3_UP_PGA_CLKEN_REG, BIT(MIC3_UP_PGA_CLKEN_OFFSET),
		BIT(MIC3_UP_PGA_CLKEN_OFFSET));
	snd_soc_component_update_bits_with_lock(codec, CODEC_CLK_EN1_REG, BIT(MIC4_UP_PGA_CLKEN_OFFSET),
		BIT(MIC4_UP_PGA_CLKEN_OFFSET));
}

static void audio_mic34_pga_clk_clear(struct snd_soc_component *codec)
{
	snd_soc_component_update_bits_with_lock(codec, AUDIO_UP_PGA_CLKEN_REG,
		BIT(AUDIO_UP_L_PGA_CLKEN_OFFSET) | BIT(AUDIO_UP_R_PGA_CLKEN_OFFSET), 0);
	snd_soc_component_update_bits_with_lock(codec, MIC3_UP_PGA_CLKEN_REG, BIT(MIC3_UP_PGA_CLKEN_OFFSET), 0);
	snd_soc_component_update_bits_with_lock(codec, CODEC_CLK_EN1_REG, BIT(MIC4_UP_PGA_CLKEN_OFFSET), 0);
}

static void voice_mic56_up_32k_set(struct snd_soc_component *codec)
{
	snd_soc_component_update_bits(codec, SRCDN_CTRL0_REG,
		mask_on_bit(VOICE_L_UP_SRCDN_SRC_MODE_LEN, VOICE_L_UP_SRCDN_SRC_MODE_OFFSET) |
		mask_on_bit(VOICE_R_UP_SRCDN_SRC_MODE_LEN, VOICE_R_UP_SRCDN_SRC_MODE_OFFSET),
		0x4 << VOICE_L_UP_SRCDN_SRC_MODE_OFFSET | 0x4 << VOICE_R_UP_SRCDN_SRC_MODE_OFFSET);
	snd_soc_component_update_bits(codec, FS_CTRL1_REG,
		mask_on_bit(FS_VOICE_L_UP_AFIFO_LEN, FS_VOICE_L_UP_AFIFO_OFFSET) |
		mask_on_bit(FS_VOICE_R_UP_AFIFO_LEN, FS_VOICE_R_UP_AFIFO_OFFSET),
		0x2 << FS_VOICE_L_UP_AFIFO_OFFSET | 0x2 << FS_VOICE_R_UP_AFIFO_OFFSET);
	snd_soc_component_update_bits(codec, FS_VOICE_UP_PGA_REG,
		mask_on_bit(FS_VOICE_L_UP_PGA_LEN, FS_VOICE_L_UP_PGA_OFFSET) |
		mask_on_bit(FS_VOICE_R_UP_PGA_LEN, FS_VOICE_R_UP_PGA_OFFSET),
		0x2 << FS_VOICE_L_UP_PGA_OFFSET | 0x2 << FS_VOICE_R_UP_PGA_OFFSET);
	snd_soc_component_update_bits(codec, FS_CTRL4_REG,
		mask_on_bit(FS_VOICE_UP_L_SRCDN_DIN_LEN, FS_VOICE_UP_L_SRCDN_DIN_OFFSET) |
		mask_on_bit(FS_VOICE_UP_R_SRCDN_DIN_LEN, FS_VOICE_UP_R_SRCDN_DIN_OFFSET), 0);
	snd_soc_component_update_bits(codec, FS_CTRL4_REG,
		mask_on_bit(FS_VOICE_UP_L_SRCDN_DOUT_LEN, FS_VOICE_UP_L_SRCDN_DOUT_OFFSET) |
		mask_on_bit(FS_VOICE_UP_R_SRCDN_DOUT_LEN, FS_VOICE_UP_R_SRCDN_DOUT_OFFSET),
		0x2 << FS_VOICE_UP_L_SRCDN_DOUT_OFFSET | 0x2 << FS_VOICE_UP_R_SRCDN_DOUT_OFFSET);
	snd_soc_component_update_bits(codec, MIC56_UP_SRCDN_SRC_MODE_REG,
		mask_on_bit(MIC5_UP_SRCDN_SRC_MODE_LEN, MIC5_UP_SRCDN_SRC_MODE_OFFSET) |
		mask_on_bit(MIC6_UP_SRCDN_SRC_MODE_LEN, MIC6_UP_SRCDN_SRC_MODE_OFFSET), 0);
	snd_soc_component_update_bits(codec, FS_MIC56_UP_AFIFO_REG,
		mask_on_bit(FS_MIC5_UP_AFIFO_LEN, FS_MIC5_UP_AFIFO_OFFSET) |
		mask_on_bit(FS_MIC6_UP_AFIFO_LEN, FS_MIC6_UP_AFIFO_OFFSET),
		0x2 << FS_MIC5_UP_AFIFO_OFFSET | 0x2 << FS_MIC6_UP_AFIFO_OFFSET);
	snd_soc_component_update_bits(codec, FS_MIC56_UP_PGA_REG,
		mask_on_bit(FS_MIC5_UP_PGA_LEN, FS_MIC5_UP_PGA_OFFSET) |
		mask_on_bit(FS_MIC6_UP_PGA_LEN, FS_MIC6_UP_PGA_OFFSET),
		0x2 << FS_MIC5_UP_PGA_OFFSET | 0x2 << FS_MIC6_UP_PGA_OFFSET);
	snd_soc_component_update_bits(codec, FS_MIC56_SRCDN_DOUT_REG,
		mask_on_bit(FS_MIC5_SRCDN_DOUT_LEN, FS_MIC5_SRCDN_DOUT_OFFSET) |
		mask_on_bit(FS_MIC6_SRCDN_DOUT_LEN, FS_MIC6_SRCDN_DOUT_OFFSET),
		0x2 << FS_MIC5_SRCDN_DOUT_OFFSET | 0x2 << FS_MIC6_SRCDN_DOUT_OFFSET);
}

static void voice_mic56_src_pga_clk_set(struct snd_soc_component *codec)
{
	snd_soc_component_update_bits_with_lock(codec, VOICE_UP_PGA_CLKEN_REG,
		BIT(VOICE_UP_L_PGA_CLKEN_OFFSET) | BIT(VOICE_UP_R_PGA_CLKEN_OFFSET),
		BIT(VOICE_UP_L_PGA_CLKEN_OFFSET) | BIT(VOICE_UP_R_PGA_CLKEN_OFFSET));
	snd_soc_component_update_bits(codec, CODEC_CLK_EN1_REG,
		BIT(VOICE_UP_L_SRCDN_CLKEN_OFFSET) | BIT(VOICE_UP_R_SRCDN_CLKEN_OFFSET),
		BIT(VOICE_UP_L_SRCDN_CLKEN_OFFSET) | BIT(VOICE_UP_R_SRCDN_CLKEN_OFFSET));
	snd_soc_component_update_bits_with_lock(codec, MIC56_UP_PGA_CLKEN_REG,
		BIT(MIC5_UP_PGA_CLKEN_OFFSET) | BIT(MIC6_UP_PGA_CLKEN_OFFSET),
		BIT(MIC5_UP_PGA_CLKEN_OFFSET) | BIT(MIC6_UP_PGA_CLKEN_OFFSET));
	snd_soc_component_update_bits(codec, MIC56_UP_SRCDN_CLKEN_REG,
		BIT(MIC5_UP_SRCDN_CLKEN_OFFSET) | BIT(MIC6_UP_SRCDN_CLKEN_OFFSET),
		BIT(MIC5_UP_SRCDN_CLKEN_OFFSET) | BIT(MIC6_UP_SRCDN_CLKEN_OFFSET));
}

static void voice_mic56_fifo_clk_clear(struct snd_soc_component *codec)
{
	snd_soc_component_update_bits_with_lock(codec, CODEC_CLK_EN0_REG,
		BIT(VOICE_L_UP_AFIFO_CLKEN_OFFSET) | BIT(VOICE_R_UP_AFIFO_CLKEN_OFFSET), 0);
	snd_soc_component_update_bits_with_lock(codec, MIC56_UP_AFIFO_CLKEN_REG,
		BIT(MIC5_UP_AFIFO_CLKEN_OFFSET) | BIT(MIC6_UP_AFIFO_CLKEN_OFFSET), 0);
}

static void voice_mic56_fifo_clk_set(struct snd_soc_component *codec)
{
	snd_soc_component_update_bits_with_lock(codec, CODEC_CLK_EN0_REG,
		BIT(VOICE_L_UP_AFIFO_CLKEN_OFFSET) | BIT(VOICE_R_UP_AFIFO_CLKEN_OFFSET),
		BIT(VOICE_L_UP_AFIFO_CLKEN_OFFSET) | BIT(VOICE_R_UP_AFIFO_CLKEN_OFFSET));
	snd_soc_component_update_bits_with_lock(codec, MIC56_UP_AFIFO_CLKEN_REG,
		BIT(MIC5_UP_AFIFO_CLKEN_OFFSET) | BIT(MIC6_UP_AFIFO_CLKEN_OFFSET),
		BIT(MIC5_UP_AFIFO_CLKEN_OFFSET) | BIT(MIC6_UP_AFIFO_CLKEN_OFFSET));
}

static void voice_mic56_src_pga_clk_clear(struct snd_soc_component *codec)
{
	snd_soc_component_update_bits(codec, MIC56_UP_PGA_CLKEN_REG,
		BIT(MIC6_UP_PGA_CLKEN_OFFSET) | BIT(MIC5_UP_PGA_CLKEN_OFFSET), 0);
	snd_soc_component_update_bits_with_lock(codec, VOICE_UP_PGA_CLKEN_REG,
		BIT(VOICE_UP_L_PGA_CLKEN_OFFSET) | BIT(VOICE_UP_R_PGA_CLKEN_OFFSET), 0);
	snd_soc_component_update_bits(codec, CODEC_CLK_EN1_REG,
		BIT(VOICE_UP_L_SRCDN_CLKEN_OFFSET) | BIT(VOICE_UP_R_SRCDN_CLKEN_OFFSET), 0);
	snd_soc_component_update_bits(codec, MIC56_UP_SRCDN_CLKEN_REG,
		BIT(MIC5_UP_SRCDN_CLKEN_OFFSET) | BIT(MIC6_UP_SRCDN_CLKEN_OFFSET), 0);
}

static int bt_audio_48k_power_event(struct snd_soc_dapm_widget *w,
	struct snd_kcontrol *kcontrol, int event)
{
	struct snd_soc_component *codec = snd_soc_dapm_to_component(w->dapm);

	IN_FUNCTION;
	AUDIO_LOGI("bt_audio 48k power event %d", event);
	switch (event) {
	case SND_SOC_DAPM_PRE_PMU:
		snd_soc_component_update_bits(codec, CODEC_SW_RST_N_REG,
			BIT(RST_4MIC_AUDIO_ACCESS_IRQ_OFFSET), BIT(RST_4MIC_AUDIO_ACCESS_IRQ_OFFSET));
		mic34_up_48k_set(codec);
		audio_mic34_pga_clk_set(codec);
		snd_soc_component_update_bits_with_lock(codec, CODEC_CLK_EN0_REG,
			BIT(AUDIO_L_UP_AFIFO_CLKEN_OFFSET) | BIT(AUDIO_R_UP_AFIFO_CLKEN_OFFSET) |
			BIT(MIC3_UP_AFIFO_CLKEN_OFFSET) | BIT(MIC4_UP_AFIFO_CLKEN_OFFSET), 0);
		snd_soc_component_update_bits(codec, CODEC_SW_RST_N_REG, BIT(RST_4MIC_AUDIO_ACCESS_IRQ_OFFSET), 0);
		udelay(CLEAR_FIFO_DELAY_LEN_US);
		snd_soc_component_update_bits_with_lock(codec, CODEC_CLK_EN0_REG,
			BIT(AUDIO_L_UP_AFIFO_CLKEN_OFFSET) | BIT(AUDIO_R_UP_AFIFO_CLKEN_OFFSET) |
			BIT(MIC3_UP_AFIFO_CLKEN_OFFSET) | BIT(MIC4_UP_AFIFO_CLKEN_OFFSET),
			BIT(AUDIO_L_UP_AFIFO_CLKEN_OFFSET) | BIT(AUDIO_R_UP_AFIFO_CLKEN_OFFSET) |
			BIT(MIC3_UP_AFIFO_CLKEN_OFFSET) | BIT(MIC4_UP_AFIFO_CLKEN_OFFSET));
		break;
	case SND_SOC_DAPM_POST_PMD:
		snd_soc_component_update_bits_with_lock(codec, CODEC_CLK_EN0_REG,
			BIT(AUDIO_L_UP_AFIFO_CLKEN_OFFSET) | BIT(AUDIO_R_UP_AFIFO_CLKEN_OFFSET) |
			BIT(MIC3_UP_AFIFO_CLKEN_OFFSET) | BIT(MIC4_UP_AFIFO_CLKEN_OFFSET), 0);
		audio_mic34_pga_clk_clear(codec);
		break;
	default:
		AUDIO_LOGW("bt_audio 48k power event err: %d", event);
		break;
	}

	OUT_FUNCTION;
	return 0;
}

static int bt_voice_32k_power_event(struct snd_soc_dapm_widget *w,
	struct snd_kcontrol *kcontrol, int event)
{
	struct snd_soc_component *codec = snd_soc_dapm_to_component(w->dapm);

	IN_FUNCTION;
	AUDIO_LOGI("bt_voice 32k power event %d", event);

	switch (event) {
	case SND_SOC_DAPM_PRE_PMU:
		voice_mic56_up_32k_set(codec);
		voice_mic56_src_pga_clk_set(codec);
		voice_mic56_fifo_clk_clear(codec);
		udelay(CLEAR_FIFO_DELAY_LEN_US);
		voice_mic56_fifo_clk_set(codec);
		break;
	case SND_SOC_DAPM_POST_PMD:
		voice_mic56_fifo_clk_clear(codec);
		voice_mic56_src_pga_clk_clear(codec);
		break;
	default:
		AUDIO_LOGW("bt_voice up 32k power event err: %d", event);
		break;
	}

	OUT_FUNCTION;
	return 0;
}

#define BT_UP_SRC_WIDGET \
	SND_SOC_DAPM_SWITCH_E("BT_AUDIO_UP_48K_SWITCH", SND_SOC_NOPM, 0, 0, \
		&dapm_bt_audio_48k_controls, bt_audio_48k_power_event, \
		(SND_SOC_DAPM_PRE_PMU | SND_SOC_DAPM_POST_PMD)), \
	SND_SOC_DAPM_SWITCH_E("BT_VOICE_UP_32K_SWITCH", SND_SOC_NOPM, 0, 0, \
		&dapm_bt_voice_32k_src_controls, bt_voice_32k_power_event, \
		(SND_SOC_DAPM_PRE_PMU | SND_SOC_DAPM_POST_PMD)) \

static const struct snd_soc_dapm_widget uplink_adapter_dapm_widgets[] = {
	UP_MUX_WIDGET,
	ULTR_UP_SWITCH_WIDGET,
	BT_UP_SRC_WIDGET,
};

static const struct snd_codec_resource uplink_adapter_resource = {
	NULL,
	uplink_adapter_snd_kcontrols,
	ARRAY_SIZE(uplink_adapter_snd_kcontrols),
	uplink_adapter_dapm_widgets,
	ARRAY_SIZE(uplink_adapter_dapm_widgets),
};

int __init add_uplink_adapter_path_in_list(void)
{
	return add_codec_platform_dapm_list(&uplink_adapter_resource);
}

arch_initcall(add_uplink_adapter_path_in_list);
