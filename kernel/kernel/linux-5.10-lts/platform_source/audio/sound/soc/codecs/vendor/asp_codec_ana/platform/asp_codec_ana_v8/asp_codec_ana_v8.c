/*
 * asp_codec_ana_v8.c -- ana_codec driver
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

#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/soc-dapm.h>
#include <sound/tlv.h>
#include <linux/regulator/consumer.h>
#include <linux/regulator/driver.h>
#include <linux/regulator/machine.h>
#include <linux/gpio.h>
#include <linux/of_gpio.h>

#include "audio_log.h"
#include "soc_ao_crg_interface.h"
#include "asoc_adapter.h"
#include "soc_mad.h"
#include "asp_codec_ana_type.h"
#include "asp_codec_ana.h"
#include "asp_codec_ana_reg_def.h"
#include "asp_codec_ana_comm.h"
#include "asp_codec_ana_utils.h"
#include "audio/asp_register.h"

#define LOG_TAG "asp_codec_ana_v8"

#define PMU_CODEC_VERSION                  0x36
#define PMUCODEC_START                     0x0
#define DEFAULT_AU_DRIVING_FORCE           5
#define MAX_AU_DRIVING_FORCE               0xF
#define DIG_IO_DS_CODEC_MASK               0xF
#define MICBISA1_SET_MASK                  0x22
#define MICBISA2_SET_MASK                  0x11
#define PGA_GAIN_STEP                      200 /* unit:0.01db */

#define SOC_AO_CRG_SCPERCLKEN0_ADDR_SIZE   0x4

static struct snd_soc_component *soc_codec;

static void __iomem *remaped_aocrg_scperclken0 = NULL;

static DECLARE_TLV_DB_SCALE(analog_mainpga_gain_tlv, 0, PGA_GAIN_STEP, 0);
static DECLARE_TLV_DB_SCALE(analog_auxpga_gain_tlv, 0, PGA_GAIN_STEP, 0);
static DECLARE_TLV_DB_SCALE(analog_mic3pga_gain_tlv, 0, PGA_GAIN_STEP, 0);

/* control name same in hardware/audio/audio_extension/audio_custom/audio_effect_util/audio_effect_util.h  */
#define ANALOG_KCONTROLS \
	SOC_SINGLE("ANA_EAR_PGA_MUTE",           CODEC_ANA_RW6_REG,         EAR_MUTE_OFFSET,                 0x1, 0), \
	SOC_SINGLE("ANA_EAR_PGA",                CODEC_ANA_RW26_REG,        EAR_GAIN_OFFSET,                 0xF, 0), \
	SOC_SINGLE("ANA_LINEOUT_PGA_MUTE",       CODEC_ANA_RW6_REG,         LOUT_MUTE_OFFSET,                0x1, 0), \
	SOC_SINGLE("ANA_LINEOUT_PGA",            CODEC_ANA_RW28_REG,        LOUT_GAIN_OFFSET,                0xF, 0), \
	SOC_SINGLE("ANA_MAINMIC_PGA_MUTE",       CODEC_ANA_RW2_REG,         MAINPGA_MUTE_OFFSET,             0x1, 0), \
	SOC_SINGLE("ANA_MAINPGA_BOOST",          CODEC_ANA_RW9_REG,         MAINPGA_BOOST_OFFSET,            0x1, 0), \
	SOC_SINGLE_TLV("ANA_MAINMIC_PGA",        CODEC_ANA_RW9_REG,         MAINPGA_GAIN_OFFSET,             0x7, 0, \
		analog_mainpga_gain_tlv), \
	SOC_SINGLE("ANA_AUXMIC_PGA_MUTE",        CODEC_ANA_RW2_REG,         AUXPGA_MUTE_OFFSET,              0x1, 0), \
	SOC_SINGLE("ANA_AUXPGA_BOOST",           CODEC_ANA_RW10_REG,        AUXPGA_BOOST_OFFSET,             0x1, 0), \
	SOC_SINGLE_TLV("ANA_AUXMIC_PGA",         CODEC_ANA_RW10_REG,        AUXPGA_GAIN_OFFSET,              0x7, 0, \
		analog_auxpga_gain_tlv), \
	SOC_SINGLE("ANA_MIC3_PGA_MUTE",          CODEC_ANA_RW2_REG,         MIC3PGA_MUTE_OFFSET,             0x1, 0), \
	SOC_SINGLE("ANA_MIC3PGA_BOOST",          CODEC_ANA_RW11_REG,        ADC3PGA_BOOST_OFFSET,            0x1, 0), \
	SOC_SINGLE_TLV("ANA_MIC3_PGA",           CODEC_ANA_RW11_REG,        MIC3PGA_GAIN_OFFSET,             0x7, 0, \
		analog_mic3pga_gain_tlv), \
	SOC_SINGLE("ANA_MIXER_EAR_L_PGA",        CODEC_ANA_RW25_REG,        CR_MIXER_EP_DACL_OFFSET,         0x1, 0), \
	SOC_SINGLE("ANA_MIXER_LINEOUT_L_PGA",    CODEC_ANA_RW27_REG,        LOMIX_BOOSTGAIN_DACL_OFFSET,     0x1, 0) \

#define ANALOG_MAD_KCONTROLS \
	SOC_SINGLE("MAD_MIC_PGA_GAIN",    CODEC_ANA_RW59_REG,  PGA_OFFSET,   0x1ff, 0), \
	SOC_SINGLE("MAD_CIC_GAIN",    SIF_MAD_CIC_GAIN_REG,  MAD_CIC_GAIN_OFFSET,   0x3f, 0) \

static struct snd_kcontrol_new codec_ana_snd_kcontrols[] = {
	ANALOG_KCONTROLS,
	ANALOG_MAD_KCONTROLS,
};

static int adc_drv_power_event(struct snd_soc_dapm_widget *w,
	struct snd_kcontrol *kcontrol, int event)
{
	unused_parameter(w);
	unused_parameter(kcontrol);

	IN_FUNCTION;

	switch (event) {
	case SND_SOC_DAPM_POST_PMU:
		usleep_range(10000, 11000);
		break;
	case SND_SOC_DAPM_PRE_PMD:
		break;
	default:
		AUDIO_LOGE("power mode event err: %d", event);
		break;
	}

	OUT_FUNCTION;

	return 0;
}

static const struct snd_kcontrol_new ear_mixer[] = {
	SOC_DAPM_SINGLE("DACL", CODEC_ANA_RW25_REG,
		MIXOUT_EAR_DACL_OFFSET, 1, 0),
	SOC_DAPM_SINGLE("DACR", CODEC_ANA_RW25_REG,
		MIXOUT_EAR_DACR_OFFSET, 1, 0),
	SOC_DAPM_SINGLE("DAC_ULTR", CODEC_ANA_RW25_REG,
		MIXOUT_EAR_DAC_ULTRA_OFFSET, 1, 0),
};

static const struct snd_kcontrol_new analog_au_en_mixer[] = {
	SOC_DAPM_SINGLE("ANA_AUL_EN", CODEC_VIR0_REG_ADDR, ANA_AUL_EN_BIT, 1, 0),
	SOC_DAPM_SINGLE("ANA_AUR_EN", CODEC_VIR0_REG_ADDR, ANA_AUR_EN_BIT, 1, 0),
	SOC_DAPM_SINGLE("ANA_MUX3_EN", CODEC_VIR0_REG_ADDR, ANA_MUX3_EN_BIT, 1, 0),
	SOC_DAPM_SINGLE("ANA_MUX4_EN", CODEC_VIR0_REG_ADDR, ANA_MUX4_EN_BIT, 1, 0),
	SOC_DAPM_SINGLE("ANA_MUX5_EN", CODEC_VIR0_REG_ADDR, ANA_MUX5_EN_BIT, 1, 0),
	SOC_DAPM_SINGLE("ANA_AU_ULTR_EN", CODEC_VIR0_REG_ADDR, ANA_AU_ULTR_EN_BIT, 1, 0),
	SOC_DAPM_SINGLE("ANA_AU_MAD_EN", CODEC_VIR0_REG_ADDR, ANA_AU_MAD_EN_BIT, 1, 0),
};

static const struct snd_kcontrol_new lineout_mixer[] = {
	SOC_DAPM_SINGLE("DACL", CODEC_ANA_RW27_REG,
		LOMIX_SEL_DACL_OFFSET, 1, 0),
	SOC_DAPM_SINGLE("DACR", CODEC_ANA_RW27_REG,
		LOMIX_SEL_DACR_OFFSET, 1, 0),
	SOC_DAPM_SINGLE("DAC_ULTR", CODEC_ANA_RW27_REG,
		LOMIX_SEL_DAC_ULTRA_OFFSET, 1, 0),
};

static const struct snd_kcontrol_new analog_adc1_mixer[] = {
	SOC_DAPM_SINGLE("DACL", CODEC_ANA_RW12_REG,
		ADCL_MIXIN_DACL_OFFSET, 1, 0),
	SOC_DAPM_SINGLE("DACR", CODEC_ANA_RW12_REG,
		ADCL_MIXIN_DACR_OFFSET, 1, 0),
	SOC_DAPM_SINGLE("MAIN_MIC", CODEC_ANA_RW12_REG,
		ADCL_MIXIN_MICPGA1_OFFSET, 1, 0),
	SOC_DAPM_SINGLE("AUX_MIC", CODEC_ANA_RW12_REG,
		ADCL_MIXIN_MICPGA2_OFFSET, 1, 0),
};

static const struct snd_kcontrol_new analog_adc2_mixer[] = {
	SOC_DAPM_SINGLE("DACL", CODEC_ANA_RW14_REG,
		ADCR_MIXIN_DACL_OFFSET, 1, 0),
	SOC_DAPM_SINGLE("DACR", CODEC_ANA_RW14_REG,
		ADCR_MIXIN_DACR_OFFSET, 1, 0),
	SOC_DAPM_SINGLE("MAIN_MIC", CODEC_ANA_RW14_REG,
		ADCR_MIXIN_MICPGA1_OFFSET, 1, 0),
	SOC_DAPM_SINGLE("AUX_MIC", CODEC_ANA_RW14_REG,
		ADCR_MIXIN_MICPGA2_OFFSET, 1, 0),
};

static const struct snd_kcontrol_new analog_adc3_mixer[] = {
	SOC_DAPM_SINGLE("DACL", CODEC_ANA_RW16_REG,
		ADC3_MIXIN_DACL_OFFSET, 1, 0),
	SOC_DAPM_SINGLE("DACUL", CODEC_ANA_RW16_REG,
		ADC3_MIXIN_ULTRA_OFFSET, 1, 0),
	SOC_DAPM_SINGLE("MAIN_MIC", CODEC_ANA_RW16_REG,
		ADC3_MIXIN_MICPGA1_OFFSET, 1, 0),
	SOC_DAPM_SINGLE("MIC3", CODEC_ANA_RW16_REG,
		ADC3_MIXIN_MICPGA3_OFFSET, 1, 0),
};

static int analog_au_mixer_event(struct snd_soc_dapm_widget *w,
	struct snd_kcontrol *kcontrol, int event)
{
	struct snd_soc_component *codec = snd_soc_dapm_to_component(w->dapm);

	IN_FUNCTION;

	switch (event) {
	case SND_SOC_DAPM_PRE_PMU:
		/* codec ana/au en */
		snd_soc_component_update_bits(codec, CLK_EN_CFG_REG,
			BIT(CODEC_ANA_EN_OFFSET) | BIT(SIF_EN_OFFSET),
			BIT(CODEC_ANA_EN_OFFSET) | BIT(SIF_EN_OFFSET));
		break;
	case SND_SOC_DAPM_POST_PMD:
		/* codec ana/au dis */
		snd_soc_component_update_bits(codec, CLK_EN_CFG_REG,
			BIT(CODEC_ANA_EN_OFFSET) | BIT(SIF_EN_OFFSET), 0);
		break;
	default:
		AUDIO_LOGE("power mode event err: %d", event);
		break;
	}

	OUT_FUNCTION;

	return 0;
}

static const char *const analog_dacl_mux_texts[] = {
	"AUL",
	"ADC1",
	"ADC2",
	"ADC3",
};

static const struct soc_enum analog_dacl_mux_enum =
	SOC_ENUM_SINGLE(DAC_CHAN_CTRL_REG, DACL_DIN_SEL_OFFSET,
		ARRAY_SIZE(analog_dacl_mux_texts), analog_dacl_mux_texts);

static const struct snd_kcontrol_new analog_dacl_mux =
	SOC_DAPM_ENUM("Mux", analog_dacl_mux_enum);

static const char *const analog_mux3_texts[] = {
	"ADC1",
	"AUL",
	"AUR",
};

static const struct soc_enum analog_mux3_enum =
	SOC_ENUM_SINGLE(ADC_CHAN_CTRL_REG, ADCL_DOUT_SEL_OFFSET,
		ARRAY_SIZE(analog_mux3_texts), analog_mux3_texts);

static const struct snd_kcontrol_new analog_mux3 =
	SOC_DAPM_ENUM("Mux", analog_mux3_enum);

static const char *const analog_mux4_texts[] = {
	"ADC2",
	"AUL",
	"AUR",
};

static const struct soc_enum analog_mux4_enum =
	SOC_ENUM_SINGLE(ADC_CHAN_CTRL_REG, ADCR_DOUT_SEL_OFFSET,
		ARRAY_SIZE(analog_mux4_texts), analog_mux4_texts);

static const struct snd_kcontrol_new analog_mux4 =
	SOC_DAPM_ENUM("Mux", analog_mux4_enum);

static const char *const analog_mux5_texts[] = {
	"ADC3",
	"AUL",
	"AUR",
};

static const struct soc_enum analog_mux5_enum =
	SOC_ENUM_SINGLE(ADC_CHAN_CTRL_REG, ADC_MIC3_DOUT_SEL_OFFSET,
		ARRAY_SIZE(analog_mux5_texts), analog_mux5_texts);

static const struct snd_kcontrol_new analog_mux5 =
	SOC_DAPM_ENUM("Mux", analog_mux5_enum);

static const struct snd_kcontrol_new analog_dacl_switch[] = {
	SOC_DAPM_SINGLE("DACL", CODEC_VIR0_REG_ADDR, ANA_DACL_BIT, 1, 0),
};

static const struct snd_kcontrol_new analog_rcv_switch[] = {
	SOC_DAPM_SINGLE("ENABLE", CODEC_ANA_RW6_REG, EAR_PD_OFFSET, 1, 1),
};

static const struct snd_kcontrol_new analog_lout_switch[] = {
	SOC_DAPM_SINGLE("ENABLE", CODEC_ANA_RW6_REG, LOUT_PD_OFFSET, 1, 1),
};

static const struct snd_kcontrol_new mic3_bias1_switch[] = {
	SOC_DAPM_SINGLE("ENABLE", CODEC_VIR0_REG_ADDR, MIC3_BIAS1_BIT, 1, 0),
};

static const struct snd_kcontrol_new mic3_bias2_switch[] = {
	SOC_DAPM_SINGLE("ENABLE", CODEC_VIR0_REG_ADDR, MIC3_BIAS2_BIT, 1, 0),
};

static const struct snd_kcontrol_new auxmic_bias1_switch[] = {
	SOC_DAPM_SINGLE("ENABLE", CODEC_VIR0_REG_ADDR, AUXMIC_BIAS1_BIT, 1, 0),
};

static const struct snd_kcontrol_new auxmic_bias2_switch[] = {
	SOC_DAPM_SINGLE("ENABLE", CODEC_VIR0_REG_ADDR, AUXMIC_BIAS2_BIT, 1, 0),
};

static int micbias1_mic_event(struct snd_soc_dapm_widget *w,
	struct snd_kcontrol *kcontrol, int event)
{
	unused_parameter(kcontrol);
	struct snd_soc_component *codec = snd_soc_dapm_to_component(w->dapm);

	IN_FUNCTION;
	switch (event) {
	case SND_SOC_DAPM_PRE_PMU:
		snd_soc_component_update_bits(codec, CODEC_ANA_RW7_REG,
			MICBISA1_SET_MASK, 0x0);
		break;
	case SND_SOC_DAPM_POST_PMD:
		snd_soc_component_update_bits(codec, CODEC_ANA_RW7_REG,
			MICBISA1_SET_MASK, 0xff);
		break;
	default:
		AUDIO_LOGE("power mode event err: %d", event);
		break;
	}
	OUT_FUNCTION;

	return 0;
}

static int micbias2_mic_event(struct snd_soc_dapm_widget *w,
	struct snd_kcontrol *kcontrol, int event)
{
	unused_parameter(kcontrol);
	struct snd_soc_component *codec = snd_soc_dapm_to_component(w->dapm);

	IN_FUNCTION;
	switch (event) {
	case SND_SOC_DAPM_PRE_PMU:
		snd_soc_component_update_bits(codec, CODEC_ANA_RW7_REG, MICBISA2_SET_MASK, 0x0);
		break;
	case SND_SOC_DAPM_POST_PMD:
		snd_soc_component_update_bits(codec, CODEC_ANA_RW7_REG, MICBISA2_SET_MASK, 0xff);
		break;
	default:
		AUDIO_LOGE("power mode event err: %d", event);
		break;
	}
	OUT_FUNCTION;

	return 0;
}

static const char *const adc_loopback_mux_texts[] = {
	"NORMAL",
	"DA_AD_LOOP",
	"DA_MIC3_LOOP",
};

static const struct soc_enum adc_loopback_mux_enum =
	SOC_ENUM_SINGLE(SIF_LOOPBACK_CFG_REG, ADC_LOOPBACK_OFFSET,
		ARRAY_SIZE(adc_loopback_mux_texts), adc_loopback_mux_texts);

static const struct snd_kcontrol_new adc_loopback_mux =
	SOC_DAPM_ENUM("Mux", adc_loopback_mux_enum);

/* dac_loopback mux */
static const char *const dac_loopback_mux_texts[] = {
	"NORMAL",
	"DOUT_SDIN",
	"MIC3_SDIN",
};

static const struct soc_enum dac_loopback_mux_enum =
	SOC_ENUM_SINGLE(SIF_LOOPBACK_CFG_REG, DAC_LOOPBACK_OFFSET,
		ARRAY_SIZE(dac_loopback_mux_texts), dac_loopback_mux_texts);

static const struct snd_kcontrol_new dac_loopback_mux =
	SOC_DAPM_ENUM("Mux", dac_loopback_mux_enum);

static int smt_ibias_supply_power_event(struct snd_soc_dapm_widget *w,
	struct snd_kcontrol *kcontrol, int event)
{
	unused_parameter(kcontrol);
	struct snd_soc_component *codec = snd_soc_dapm_to_component(w->dapm);

	IN_FUNCTION;

	switch (event) {
	case SND_SOC_DAPM_PRE_PMU:
		ibias_work_enable(codec, true);
		break;
	case SND_SOC_DAPM_POST_PMD:
		ibias_work_enable(codec, false);
		break;
	default:
		AUDIO_LOGE("power mode event err: %d", event);
		break;
	}

	OUT_FUNCTION;

	return 0;
}

static const char * const mad_mic_mux_texts[] = {
	"NULL",
	"MAIN_MIC",
	"AUX_MIC",
	"NULL",
	"MIC3",
};

static const struct soc_enum mad_mic_mux_enum =
	SOC_ENUM_SINGLE(CODEC_ANA_RW58_REG, MAD_PGA_SEL_OFFSET,
		ARRAY_SIZE(mad_mic_mux_texts), mad_mic_mux_texts);

static const struct snd_kcontrol_new mad_mic_mux_controls =
	SOC_DAPM_ENUM("Mux", mad_mic_mux_enum);

static const struct snd_kcontrol_new au_mad_switch =
	SOC_DAPM_SINGLE("ENABLE", CODEC_VIR0_REG_ADDR, ANA_MAD_BIT, 1, 0);

static bool is_mad_init(void)
{
	unsigned int aocrg_scperclken0_data;

	if (!remaped_aocrg_scperclken0) {
		AUDIO_LOGE("ao_crg addr ioremap failed");
		return false;
	}

	aocrg_scperclken0_data = (unsigned int)readl(remaped_aocrg_scperclken0);
	aocrg_scperclken0_data &= BIT(SOC_AO_CRG_SCPEREN0_gt_clk_mad_32k_START);
	if (aocrg_scperclken0_data != BIT(SOC_AO_CRG_SCPEREN0_gt_clk_mad_32k_START)) {
		AUDIO_LOGE("asp mad is not ready");
		return false;
	} else {
		return true;
	}
}

static int au_mad_mic_event(struct snd_soc_dapm_widget *w,
	struct snd_kcontrol *kcontrol, int event)
{
	struct snd_soc_component *codec = snd_soc_dapm_to_component(w->dapm);

	if (!is_mad_init())
		return 0;

	AUDIO_LOGI("power mode event: %d", event);
	switch (event) {
	case SND_SOC_DAPM_PRE_PMU:
		soc_mad_select_din(LOW_FREQ_MODE);
		soc_mad_request_pinctrl_state(LOW_FREQ_MODE);
		snd_soc_component_write_adapter(codec, CTRL_REG_DIG_IO_DS_SEL1_CODEC_CFG_REG, 0x1);
		break;
	case SND_SOC_DAPM_POST_PMD:
		soc_mad_select_din(HIGH_FREQ_MODE);
		soc_mad_release_pinctrl_state(LOW_FREQ_MODE);
		snd_soc_component_write_adapter(codec, CTRL_REG_DIG_IO_DS_SEL1_CODEC_CFG_REG, 0x0);
		break;
	default:
		AUDIO_LOGE("power mode event err: %d", event);
		break;
	}

	return 0;
}

static int mad_mic_mux_event(struct snd_soc_dapm_widget *w,
	struct snd_kcontrol *kcontrol, int event)
{
	unused_parameter(kcontrol);
	struct snd_soc_component *codec = snd_soc_dapm_to_component(w->dapm);

	AUDIO_LOGI("power mode event: %d", event);
	switch (event) {
	case SND_SOC_DAPM_PRE_PMU:
		snd_soc_component_write_adapter(codec, CODEC_ANA_RW60_REG, 0x3F);
		snd_soc_component_update_bits(codec, CODEC_ANA_RW1_REG, 0x3 << AVREF_SEL_OFFSET, 0x2);
		snd_soc_component_update_bits(codec, CODEC_ANA_RW62_REG, BIT(DIN_MAD_MIC_OFFSET),
			BIT(DIN_MAD_MIC_OFFSET));
		snd_soc_component_update_bits(codec, CODEC_ANA_RW58_REG, BIT(VCM_MAD_LP_EN_OFFSET),
			BIT(VCM_MAD_LP_EN_OFFSET));
		break;
	case SND_SOC_DAPM_POST_PMD:
		snd_soc_component_update_bits(codec, CODEC_ANA_RW62_REG, BIT(DIN_MAD_MIC_OFFSET), 0x0);
		snd_soc_component_update_bits(codec, CODEC_ANA_RW58_REG, BIT(VCM_MAD_LP_EN_OFFSET), 0x0);
		break;
	default:
		AUDIO_LOGE("power mode event err: %d", event);
		break;
	}

	return 0;
}

static int au_mad_switch_event(struct snd_soc_dapm_widget *w,
	struct snd_kcontrol *kcontrol, int event)
{
	unused_parameter(kcontrol);
	struct snd_soc_component *codec = snd_soc_dapm_to_component(w->dapm);

	AUDIO_LOGI("power mode event: %d", event);
	switch (event) {
	case SND_SOC_DAPM_PRE_PMU:
		snd_soc_component_write_adapter(codec, CODEC_ANA_RW60_REG, 0x02);
		usleep_range(10, 11);
		snd_soc_component_update_bits(codec, CODEC_ANA_RW60_REG, BIT(PD_MUTE_MAD_OFFSET), 0);
		usleep_range(200, 220);
		snd_soc_component_update_bits(codec, SIF_MAD_REG,
			BIT(MAD_CIC_CLKEN_OFFSET) | BIT(SIF_MAD_ADC_EN_OFFSET) | BIT(SIF_MAD_EN_OFFSET),
			BIT(MAD_CIC_CLKEN_OFFSET) | BIT(SIF_MAD_ADC_EN_OFFSET) | BIT(SIF_MAD_EN_OFFSET));
		break;
	case SND_SOC_DAPM_POST_PMD:
		snd_soc_component_update_bits(codec, CODEC_ANA_RW60_REG, BIT(PD_MUTE_MAD_OFFSET), BIT(PD_MUTE_MAD_OFFSET));
		snd_soc_component_update_bits(codec, SIF_MAD_REG,
			BIT(MAD_CIC_CLKEN_OFFSET) | BIT(SIF_MAD_ADC_EN_OFFSET) | BIT(SIF_MAD_EN_OFFSET), 0);
		break;
	default:
		AUDIO_LOGE("power mode event err: %d", event);
		break;
	}

	return 0;
}

static int mad_drv_power_event(struct snd_soc_dapm_widget *w,
	struct snd_kcontrol *kcontrol, int event)
{
	struct snd_soc_component *codec = snd_soc_dapm_to_component(w->dapm);

	AUDIO_LOGI("power mode event: %d", event);
	switch (event) {
	case SND_SOC_DAPM_POST_PMU:
		snd_soc_component_update_bits(codec, CODEC_ANA_RW1_REG, 0x3 << AVREF_SEL_OFFSET, 0x1);
		usleep_range(200, 220);
		break;
	case SND_SOC_DAPM_PRE_PMD:
		break;
	default:
		AUDIO_LOGE("power mode event err: %d", event);
		break;
	}

	return 0;
}

static int analog_dacl_switch_event(struct snd_soc_dapm_widget *w,
	struct snd_kcontrol *kcontrol, int event)
{
	struct snd_soc_component *codec = snd_soc_dapm_to_component(w->dapm);

	IN_FUNCTION;

	switch (event) {
	case SND_SOC_DAPM_POST_PMU:
		/* dem/dacl clk enable */
		snd_soc_component_update_bits(codec, DAC_CHAN_CTRL_REG,
			BIT(DACL_DWA_EN_OFFSET), BIT(DACL_DWA_EN_OFFSET));
		snd_soc_component_update_bits(codec, CODEC_ANA_RW4_REG,
			BIT(DACS_PD_OFFSET), 0);
		break;
	case SND_SOC_DAPM_PRE_PMD:
		/* dem/dacl clk disable */
		snd_soc_component_update_bits(codec, DAC_CHAN_CTRL_REG,
			BIT(DACL_DWA_EN_OFFSET), 0);
		snd_soc_component_update_bits(codec, CODEC_ANA_RW4_REG,
			BIT(DACS_PD_OFFSET), BIT(DACS_PD_OFFSET));
		break;
	default:
		AUDIO_LOGE("power mode event err: %d", event);
		break;
	}

	OUT_FUNCTION;

	return 0;
}

#define ANA_INPUT_OUTPUT_WIDGET \
	SND_SOC_DAPM_INPUT("SMT_ANA_DAC_SDATA_INPUT"), \
	SND_SOC_DAPM_INPUT("MAINMIC_INPUT"), \
	SND_SOC_DAPM_INPUT("AUXMIC_INPUT"), \
	SND_SOC_DAPM_INPUT("MIC3_INPUT"), \
	SND_SOC_DAPM_OUTPUT("ANA_EAR_OUTPUT"), \
	SND_SOC_DAPM_OUTPUT("ANA_AU_OUTPUT") \

#define ANA_PGA_WIDGET \
	SND_SOC_DAPM_PGA("ANA_MAIN_PGA", CODEC_ANA_RW2_REG, MAINPGA_PD_OFFSET, 1, NULL, 0), \
	SND_SOC_DAPM_PGA("ANA_AUX_PGA", CODEC_ANA_RW2_REG, AUXPGA_PD_OFFSET, 1, NULL, 0), \
	SND_SOC_DAPM_PGA("ANA_MIC3_PGA", CODEC_ANA_RW2_REG, MIC3PGA_PD_OFFSET, 1, NULL, 0) \

#define ANA_MIXER_WIDGET \
	SND_SOC_DAPM_MIXER("ANA_EAR_MIXER", CODEC_ANA_RW6_REG, MIXOUT_EAR_PD_OFFSET, 1, \
		ear_mixer, ARRAY_SIZE(ear_mixer)), \
	SND_SOC_DAPM_MIXER("ANA_LINEOUT_MIXER", CODEC_ANA_RW6_REG, LOMIXER_PD_OFFSET, 1, \
		lineout_mixer, ARRAY_SIZE(lineout_mixer)), \
	SND_SOC_DAPM_MIXER("ANA_ADC1_MIXER", CODEC_ANA_RW3_REG, ADCL_PD_OFFSET, 1, \
		analog_adc1_mixer, ARRAY_SIZE(analog_adc1_mixer)), \
	SND_SOC_DAPM_MIXER("ANA_ADC2_MIXER", CODEC_ANA_RW3_REG, ADCR_PD_OFFSET, 1, \
		analog_adc2_mixer, ARRAY_SIZE(analog_adc2_mixer)), \
	SND_SOC_DAPM_MIXER("ANA_ADC3_MIXER", CODEC_ANA_RW3_REG, ADC3_PD_OFFSET, 1, \
		analog_adc3_mixer, ARRAY_SIZE(analog_adc3_mixer)), \
	SND_SOC_DAPM_MIXER_E("ANA_AU_EN_MIXER", SND_SOC_NOPM, 0, 0, \
		analog_au_en_mixer, ARRAY_SIZE(analog_au_en_mixer), \
		analog_au_mixer_event, (SND_SOC_DAPM_PRE_PMU | SND_SOC_DAPM_POST_PMD)) \

#define ANA_MUX_WIDGET \
	SND_SOC_DAPM_MUX("ANA_DAC_L_MUX", CLK_EN_CFG_REG, DACL_EN_OFFSET, 0, &analog_dacl_mux), \
	SND_SOC_DAPM_MUX("ANA_MUX3_MUX", CLK_EN_CFG_REG, ADCL_EN_OFFSET, 0, &analog_mux3), \
	SND_SOC_DAPM_MUX("ANA_MUX4_MUX", CLK_EN_CFG_REG, ADCR_EN_OFFSET, 0, &analog_mux4), \
	SND_SOC_DAPM_MUX("ANA_MUX5_MUX", CLK_EN_CFG_REG, ADC_MIC3_EN_OFFSET, 0, &analog_mux5) \

#define ANA_SWITCH_WIDGET \
	SND_SOC_DAPM_SWITCH_E("ANA_DAC_L_SWITCH", SND_SOC_NOPM, 0, 0, analog_dacl_switch, \
		analog_dacl_switch_event, (SND_SOC_DAPM_POST_PMU | SND_SOC_DAPM_PRE_PMD)), \
	SND_SOC_DAPM_SWITCH("ANA_RCV_SWITCH", CODEC_ANA_RW6_REG, EPVCM_PD_OFFSET, 1, analog_rcv_switch), \
	SND_SOC_DAPM_SWITCH("ANA_LOUT_SWITCH", SND_SOC_NOPM, 0, 0, analog_lout_switch), \
	SND_SOC_DAPM_SWITCH("MIC3_BIAS1_SWITCH", SND_SOC_NOPM, 0, 0, mic3_bias1_switch), \
	SND_SOC_DAPM_SWITCH("MIC3_BIAS2_SWITCH", SND_SOC_NOPM, 0, 0, mic3_bias2_switch), \
	SND_SOC_DAPM_SWITCH("AUXMIC_BIAS1_SWITCH", SND_SOC_NOPM, 0, 0, auxmic_bias1_switch), \
	SND_SOC_DAPM_SWITCH("AUXMIC_BIAS2_SWITCH", SND_SOC_NOPM, 0, 0, auxmic_bias2_switch) \

#define ANA_MIC_WIDGET \
	SND_SOC_DAPM_MIC("MICBIAS1_MIC", micbias1_mic_event), \
	SND_SOC_DAPM_MIC("MICBIAS2_MIC", micbias2_mic_event) \

#define ANA_OUT_DRV_WIDGET \
	SND_SOC_DAPM_OUT_DRV_E("ANA_ADC1_DRV", CODEC_ANA_RW3_REG, ADCL_MUTE_OFFSET, 1, NULL, 0, \
		adc_drv_power_event, (SND_SOC_DAPM_POST_PMU | SND_SOC_DAPM_PRE_PMD)), \
	SND_SOC_DAPM_OUT_DRV_E("ANA_ADC2_DRV", CODEC_ANA_RW3_REG, ADCR_MUTE_OFFSET, 1, NULL, 0, \
		adc_drv_power_event, (SND_SOC_DAPM_POST_PMU | SND_SOC_DAPM_PRE_PMD)), \
	SND_SOC_DAPM_OUT_DRV_E("ANA_ADC3_DRV", CODEC_ANA_RW3_REG, ADC3_MUTE_OFFSET, 1, NULL, 0, \
		adc_drv_power_event, (SND_SOC_DAPM_POST_PMU | SND_SOC_DAPM_PRE_PMD)) \

#define LOOP_MUX_WIDGET \
	SND_SOC_DAPM_MUX("ADC_LOOP_MUX", SND_SOC_NOPM, 0, 0, &adc_loopback_mux), \
	SND_SOC_DAPM_MUX("DAC_LOOPBACK_MUX", SND_SOC_NOPM, 0, 0, &dac_loopback_mux) \

#define SUPPLY_WIDGET \
	SND_SOC_DAPM_SUPPLY("SMT_IBIAS_SUPPLY", SND_SOC_NOPM, 0, 0, \
		smt_ibias_supply_power_event, (SND_SOC_DAPM_PRE_PMU | SND_SOC_DAPM_POST_PMD)) \

#define ANA_MAD_WIDGET \
	SND_SOC_DAPM_INPUT("MAD_INPUT"), \
	SND_SOC_DAPM_INPUT("MAD_AUX_INPUT"), \
	SND_SOC_DAPM_MIC("AU_MAD_MIC", au_mad_mic_event), \
	SND_SOC_DAPM_MUX_E("MAD_MIC_MUX", SND_SOC_NOPM, 0, 0, &mad_mic_mux_controls, \
		mad_mic_mux_event, (SND_SOC_DAPM_PRE_PMU | SND_SOC_DAPM_POST_PMD)), \
	SND_SOC_DAPM_SWITCH_E("AU_MAD_SWITCH", SND_SOC_NOPM, 0, 0, &au_mad_switch, \
		au_mad_switch_event, (SND_SOC_DAPM_PRE_PMU | SND_SOC_DAPM_POST_PMD)), \
	SND_SOC_DAPM_OUT_DRV_E("MAD_ADC1_DRV", CODEC_ANA_RW3_REG, ADCL_MUTE_OFFSET, 1, NULL, 0, \
		mad_drv_power_event, (SND_SOC_DAPM_POST_PMU | SND_SOC_DAPM_PRE_PMD)), \
	SND_SOC_DAPM_OUTPUT("MAD_OUTPUT"), \
	SND_SOC_DAPM_OUTPUT("MAD_AUX_OUTPUT") \

static struct snd_soc_dapm_widget codec_ana_dapm_widgets[] = {
	ANA_INPUT_OUTPUT_WIDGET,
	ANA_PGA_WIDGET,
	ANA_MIXER_WIDGET,
	ANA_MUX_WIDGET,
	ANA_SWITCH_WIDGET,
	ANA_MIC_WIDGET,
	ANA_OUT_DRV_WIDGET,
	LOOP_MUX_WIDGET,
	SUPPLY_WIDGET,
	ANA_MAD_WIDGET,
};

#define ANA_AUDIO_PLAY_EARPHONE \
	{ "ANA_AU_EN_MIXER",     "ANA_AUL_EN",   "SMT_ANA_DAC_SDATA_INPUT" }, \
	{ "ANA_DAC_L_MUX",       "AUL",          "ANA_AU_EN_MIXER" }, \
	{ "ANA_DAC_L_MUX",       "ADC1",         "ANA_MUX3_MUX" }, \
	{ "ANA_DAC_L_MUX",       "ADC2",         "ANA_MUX4_MUX" }, \
	{ "ANA_DAC_L_MUX",       "ADC3",         "ANA_MUX5_MUX" }, \
	{ "ANA_DAC_L_SWITCH",    "DACL",         "ANA_DAC_L_MUX" }, \
	{ "ADC_LOOP_MUX",        "NORMAL",       "ANA_DAC_L_SWITCH" }, \
	{ "ANA_EAR_MIXER",       "DACL",         "ADC_LOOP_MUX" }, \
	{ "ANA_EAR_MIXER",       "DAC_ULTR",     "ADC_LOOP_MUX" }, \
	{ "ANA_LINEOUT_MIXER",   "DACL",         "ADC_LOOP_MUX" }, \
	{ "ANA_LINEOUT_MIXER",   "DAC_ULTR",     "ADC_LOOP_MUX" }, \
	{ "ANA_RCV_SWITCH",      "ENABLE",       "ANA_EAR_MIXER" }, \
	{ "ANA_LOUT_SWITCH",     "ENABLE",       "ANA_LINEOUT_MIXER" }, \
	{ "ANA_EAR_OUTPUT",      NULL,           "ANA_RCV_SWITCH" }, \
	{ "ANA_EAR_OUTPUT",      NULL,           "ANA_LOUT_SWITCH" } \

#define ANA_AUDIO_CAP \
	{ "MAINMIC_INPUT",       NULL,           "MICBIAS1_MIC" }, \
	{ "AUXMIC_BIAS1_SWITCH", "ENABLE",       "MICBIAS1_MIC" }, \
	{ "AUXMIC_INPUT",        NULL,           "AUXMIC_BIAS1_SWITCH" }, \
	{ "AUXMIC_BIAS2_SWITCH", "ENABLE",       "MICBIAS2_MIC" }, \
	{ "AUXMIC_INPUT",        NULL,           "AUXMIC_BIAS2_SWITCH" }, \
	{ "MIC3_BIAS2_SWITCH",   "ENABLE",       "MICBIAS2_MIC" }, \
	{ "MIC3_INPUT",          NULL,           "MIC3_BIAS2_SWITCH" }, \
	{ "MIC3_BIAS1_SWITCH",   "ENABLE",       "MICBIAS1_MIC" }, \
	{ "MIC3_INPUT",          NULL,           "MIC3_BIAS1_SWITCH" }, \
	{ "ANA_MAIN_PGA",        NULL,           "MAINMIC_INPUT" }, \
	{ "ANA_AUX_PGA",         NULL,           "AUXMIC_INPUT" }, \
	{ "ANA_MIC3_PGA",        NULL,           "MIC3_INPUT" }, \
	{ "ANA_ADC1_MIXER",      "DACL",         "ADC_LOOP_MUX" }, \
	{ "ANA_ADC1_MIXER",      "MAIN_MIC",     "ANA_MAIN_PGA" }, \
	{ "ANA_ADC1_MIXER",      "AUX_MIC",      "ANA_AUX_PGA" }, \
	{ "ANA_ADC2_MIXER",      "DACL",         "ADC_LOOP_MUX" }, \
	{ "ANA_ADC2_MIXER",      "MAIN_MIC",     "ANA_MAIN_PGA" }, \
	{ "ANA_ADC2_MIXER",      "AUX_MIC",      "ANA_AUX_PGA" }, \
	{ "ANA_ADC1_DRV",        NULL,           "ANA_ADC1_MIXER" }, \
	{ "ANA_ADC2_DRV",        NULL,           "ANA_ADC2_MIXER" }, \
	{ "ANA_MUX3_MUX",        "ADC1",         "ANA_ADC1_DRV" }, \
	{ "ANA_MUX4_MUX",        "ADC2",         "ANA_ADC2_DRV" }, \
	{ "ANA_MUX3_MUX",        "ADC1",         "ADC_LOOP_MUX" }, \
	{ "ANA_MUX4_MUX",        "ADC2",         "ADC_LOOP_MUX" }, \
	{ "ANA_ADC3_MIXER",      "DACL",         "ADC_LOOP_MUX" }, \
	{ "ANA_ADC3_MIXER",      "MIC3",         "ANA_MIC3_PGA" }, \
	{ "ANA_ADC3_MIXER",      "MAIN_MIC",     "ANA_MAIN_PGA" }, \
	{ "ANA_ADC3_DRV",        NULL,           "ANA_ADC3_MIXER" }, \
	{ "ANA_MUX5_MUX",        "ADC3",         "ANA_ADC3_DRV" }, \
	{ "ANA_AU_EN_MIXER",     "ANA_MUX3_EN",  "ANA_MUX3_MUX" }, \
	{ "ANA_AU_EN_MIXER",     "ANA_MUX4_EN",  "ANA_MUX4_MUX" }, \
	{ "ANA_AU_EN_MIXER",     "ANA_MUX5_EN",  "ANA_MUX5_MUX" }, \
	{ "ANA_AU_OUTPUT",       NULL,           "ANA_AU_EN_MIXER" } \

#define ANA_OM \
	{ "ADC_LOOP_MUX",      "DA_AD_LOOP",    "ANA_DAC_L_SWITCH" }, \
	{ "ADC_LOOP_MUX",      "DA_MIC3_LOOP",  "ANA_DAC_L_SWITCH" }, \
	{ "DAC_LOOPBACK_MUX",  "MIC3_SDIN",     "ANA_MUX3_MUX" }, \
	{ "DAC_LOOPBACK_MUX",  "MIC3_SDIN",     "ANA_MUX4_MUX" }, \
	{ "DAC_LOOPBACK_MUX",  "MIC3_SDIN",     "ANA_MUX5_MUX" }, \
	{ "DAC_LOOPBACK_MUX",  "DOUT_SDIN",     "ANA_MUX3_MUX" }, \
	{ "DAC_LOOPBACK_MUX",  "DOUT_SDIN",     "ANA_MUX4_MUX" }, \
	{ "DAC_LOOPBACK_MUX",  "DOUT_SDIN",     "ANA_MUX5_MUX" }, \
	{ "ANA_AU_EN_MIXER",   NULL,            "DAC_LOOPBACK_MUX" }, \
	{ "ANA_AU_EN_MIXER",   NULL,            "DAC_LOOPBACK_MUX" } \

#define ANA_IBIAS_SUPPLY \
	{ "ANA_AU_EN_MIXER",   NULL,   "SMT_IBIAS_SUPPLY" } \

#define ANA_ULTR_DOWNLINK \
	{ "ANA_AU_EN_MIXER",    "ANA_AU_ULTR_EN",   "SMT_ANA_DAC_SDATA_INPUT" } \

#define ANA_MAD_LOW_FREQ \
	{ "AU_MAD_MIC",          NULL,           "MAD_INPUT" }, \
	{ "MAD_MIC_MUX",         "MAIN_MIC",     "AU_MAD_MIC" }, \
	{ "MAD_MIC_MUX",         "AUX_MIC",      "AU_MAD_MIC" }, \
	{ "MAD_MIC_MUX",         "MIC3",         "AU_MAD_MIC" }, \
	{ "AU_MAD_SWITCH",       "ENABLE",       "MAD_MIC_MUX" }, \
	{ "MAD_OUTPUT",          NULL,           "AU_MAD_SWITCH"} \

#define ANA_MAD_LOW_FREQ_AUX \
	{ "MAD_AUX_INPUT",       NULL,              "MICBIAS1_MIC" }, \
	{ "ANA_AU_EN_MIXER",     "ANA_AU_MAD_EN",   "MAD_AUX_INPUT"}, \
	{ "MAD_AUX_OUTPUT",      NULL,              "ANA_AU_EN_MIXER" } \

static struct snd_soc_dapm_route codec_ana_route_map[] = {
	ANA_IBIAS_SUPPLY,
	ANA_AUDIO_PLAY_EARPHONE,
	ANA_AUDIO_CAP,
	ANA_ULTR_DOWNLINK,
	ANA_OM,
	ANA_MAD_LOW_FREQ,
	ANA_MAD_LOW_FREQ_AUX,
};

static void asp_codec_ana_enable_ibias(struct snd_soc_component *codec, int on)
{
	IN_FUNCTION;

	if (codec == NULL) {
		AUDIO_LOGE("codec is null");
		return;
	}

	if (on) {
		snd_soc_component_update_bits(codec, CODEC_ANA_RW10_REG,
			BIT(DISCHARGE_DISABLE_OFFSET), BIT(DISCHARGE_DISABLE_OFFSET));
		snd_soc_component_update_bits(codec, CODEC_ANA_RW12_REG,
			BIT(IDEL_TONE_CTRL_OFFSET), BIT(IDEL_TONE_CTRL_OFFSET));
		snd_soc_component_update_bits(codec, CODEC_ANA_RW3_REG,
			BIT(ADCL_MUTE_OFFSET) | BIT(ADCR_MUTE_OFFSET) |
			BIT(ADC3_MUTE_OFFSET), BIT(ADCL_MUTE_OFFSET) |
			BIT(ADCR_MUTE_OFFSET) | BIT(ADC3_MUTE_OFFSET));
		snd_soc_component_write(codec, CODEC_ANA_RW1_REG, 0x07);
		msleep(6);
		snd_soc_component_write(codec, CODEC_ANA_RW1_REG, 0x03);
		snd_soc_component_write(codec, CODEC_ANA_RW1_REG, 0x01);
	} else {
		snd_soc_component_write(codec, CODEC_ANA_RW1_REG, 0x04);
	}

	OUT_FUNCTION;
}

static void cfg_au_driving_force(struct snd_soc_component *codec)
{
	unsigned int value = DEFAULT_AU_DRIVING_FORCE;
	unsigned int dts_cfg = 0;
	struct device_node *np = codec->dev->of_node;

	if (!of_property_read_u32(np, "au_driving_force", &dts_cfg)) {
		if (dts_cfg <= MAX_AU_DRIVING_FORCE)
			value = dts_cfg;
	}

	snd_soc_component_update_bits(codec, CTRL_REG_DIG_IO_DS_CODEC_CFG_REG, DIG_IO_DS_CODEC_MASK, value);
	AUDIO_LOGI("au_driving_force is %u", value);
}

static void asp_codec_ana_reg_init(struct snd_soc_component *codec)
{
	IN_FUNCTION;

	snd_soc_component_update_bits(codec, CODEC_ANA_RW12_REG,
		BIT(ADCL_MIXIN_DACL_OFFSET) | BIT(ADCL_MIXIN_DACR_OFFSET) |
		BIT(ADCL_MIXIN_MICPGA1_OFFSET) | BIT(ADCL_MIXIN_MICPGA2_OFFSET), 0);

	snd_soc_component_update_bits(codec, CODEC_ANA_RW16_REG,
		BIT(ADC3_MIXIN_DACL_OFFSET) | BIT(ADC3_MIXIN_ULTRA_OFFSET) |
		BIT(ADC3_MIXIN_MICPGA1_OFFSET) | BIT(ADC3_MIXIN_MICPGA3_OFFSET), 0);

	snd_soc_component_update_bits(codec, CODEC_ANA_RW25_REG,
		BIT(MIXOUT_EAR_DACL_OFFSET) | BIT(MIXOUT_EAR_DACR_OFFSET) |
		BIT(MIXOUT_EAR_DAC_ULTRA_OFFSET) | BIT(CR_MIXER_EP_DACL_OFFSET) |
		BIT(CR_MIXER_EP_DACR_OFFSET) | BIT(CR_MIXER_EP_DAC_ULTRA_OFFSET), 0);

	snd_soc_component_update_bits(codec, CODEC_ANA_RW37_REG,
		max_val_on_bit(DAC_CHOP_CLK_SEL_LEN) << DAC_CHOP_CLK_SEL_OFFSET,
		0x3 << DAC_CHOP_CLK_SEL_OFFSET);

	snd_soc_component_update_bits(codec, CODEC_ANA_RW49_REG,
		max_val_on_bit(IB05_LOMIX_ADJ_LEN) << IB05_LOMIX_ADJ_OFFSET,
		0x7 << IB05_LOMIX_ADJ_OFFSET);

	snd_soc_component_update_bits(codec, CODEC_ANA_RW36_REG,
		max_val_on_bit(ADC_CHOP_CLK_SEL_LEN) << ADC_CHOP_CLK_SEL_OFFSET,
		0x3 << ADC_CHOP_CLK_SEL_OFFSET);

	snd_soc_component_update_bits(codec, CODEC_ANA_RW14_REG,
		max_val_on_bit(ADCR_IDEL_TONE_LEN) << ADCR_IDEL_TONE_OFFSET,
		0x1 << ADCR_IDEL_TONE_OFFSET);

	snd_soc_component_update_bits(codec, CODEC_ANA_RW11_REG,
		max_val_on_bit(MIC3_IDEL_TONE_LEN) << MIC3_IDEL_TONE_OFFSET,
		0x1 << MIC3_IDEL_TONE_OFFSET);
	
	snd_soc_component_update_bits(codec, CODEC_ANA_RW47_REG,
		max_val_on_bit(IB05_LO_ADJ_LEN) << IB05_LO_ADJ_OFFSET,
		0x7 << IB05_LO_ADJ_OFFSET);

	OUT_FUNCTION;
}

static void reset_codec_ana(struct snd_soc_component *codec)
{
	/* smartstar codec reset */
	snd_soc_component_write(codec, CTRL_REG_PMU_SOFT_RST_REG, 0x22);
	usleep_range(10000, 11000);
	snd_soc_component_write(codec, CTRL_REG_PMU_SOFT_RST_REG, 0x2F);
	snd_soc_component_write(codec, CTRL_REG_DIG_IO_DS_SEL1_CODEC_CFG_REG, 0x0);
}

static void asp_codec_ana_priv_data_init(struct snd_soc_component *codec)
{
	struct codec_ana_data *priv = NULL;

	priv = snd_soc_component_get_drvdata(codec);
	if (priv == NULL)
		AUDIO_LOGE("priv is null");

	soc_codec = codec;
	cfg_au_driving_force(codec);

	remaped_aocrg_scperclken0 = ioremap(SOC_AO_CRG_SCPERCLKEN0_ADDR(SOC_ACPU_AO_CRG_BASE_ADDR),
		SOC_AO_CRG_SCPERCLKEN0_ADDR_SIZE);
	if (!remaped_aocrg_scperclken0)
		AUDIO_LOGE("ao_crg addr ioremap failed");
}

static void asp_codec_ana_priv_data_deinit(struct snd_soc_component *codec)
{
	struct codec_ana_data *priv = NULL;

	priv = snd_soc_component_get_drvdata(codec);
	if (priv == NULL)
		AUDIO_LOGE("priv is null");

	soc_codec = NULL;

	if (remaped_aocrg_scperclken0) {
		iounmap(remaped_aocrg_scperclken0);
		remaped_aocrg_scperclken0 = NULL;
	}
}

static void asp_codec_ana_shutdown(struct platform_device *dev)
{
	AUDIO_LOGI("Begin");

	if (soc_codec == NULL) {
		AUDIO_LOGE("parameter is NULL");
		return;
	}

	/* close ibias and Avref */
	asp_codec_ana_enable_ibias(soc_codec, false);

	/* reset codec */
	snd_soc_component_write_adapter(soc_codec, CTRL_REG_PMU_SOFT_RST_REG, 0x22);

	AUDIO_LOGI("End");
}

static struct asp_codec_ana_reg_page g_ana_reg_array[] = {
	{PAGE_ANACODEC, PAGE_ANACODEC_END, false},
	{PAGE_ANACTRL, PAGE_ANACTRL_END, false},
	{PAGE_ANAHKADC, PAGE_ANAHKADC_END, false},
	{PAGE_VIRCODEC_ANA, PAGE_VIRCODEC_ANA_END, true},
};

struct ana_codec_resource g_ana_codec_resource = {
	.controls = &codec_ana_snd_kcontrols[0],
	.controls_num = ARRAY_SIZE(codec_ana_snd_kcontrols),
	.widgets = &codec_ana_dapm_widgets[0],
	.widgets_num = ARRAY_SIZE(codec_ana_dapm_widgets),
	.routes = &codec_ana_route_map[0],
	.routes_num = ARRAY_SIZE(codec_ana_route_map),
};

static struct hicodec_dump_reg_entry ana_dump_table[] = {
	{ "ANA CODEC",  PAGE_ANACODEC, PAGE_ANACODEC_END,  1 },
};

static struct hicodec_dump_reg_info ana_dump_info = {
	.entry = ana_dump_table,
	.count = ARRAY_SIZE(ana_dump_table),
};

struct codec_ana_platform_data g_ana_codec_data = {
	.base.codec_ops = {
		.ana_reset_codec = reset_codec_ana,
		.ana_reg_init = asp_codec_ana_reg_init,
		.enable_ibias = asp_codec_ana_enable_ibias,
		.ana_priv_data_init = asp_codec_ana_priv_data_init,
		.ana_priv_data_deinit = asp_codec_ana_priv_data_deinit,
		.shutdown = asp_codec_ana_shutdown,
	},
	.base.platform_reg_array = &g_ana_reg_array[0],
	.base.platform_reg_size = ARRAY_SIZE(g_ana_reg_array),
	.base.v_codec_num = VIR_ANA_REG_CNT,
	.base.codec_resource = &g_ana_codec_resource,
	.base.debug_reg_info = &ana_dump_info,
};

static_t int __init codec_ana_data_init(void)
{
	asp_ana_codec_driver_init(&(g_ana_codec_data.base));
	return 0;
}

device_initcall(codec_ana_data_init);
