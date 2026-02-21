/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Device driver for schargerV800 adc
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/err.h>
#include <linux/io.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/of_address.h>
#include <linux/of_gpio.h>
#include <linux/delay.h>
#include <linux/regmap.h>
#include <linux/module.h>
#include <linux/platform_device.h>

#include "scharger_v800_regs.h"
#include "scharger_v800_adc.h"
#include "scharger_v800_buck.h"

#define scharger_err(fmt, args...) pr_err("[schargerV800]" fmt, ## args)
#define scharger_evt(fmt, args...) pr_warn("[schargerV800]" fmt, ## args)
#define scharger_inf(fmt, args...) pr_info("[schargerV800]" fmt, ## args)
#define scharger_dbg(fmt, args...) pr_debug("[schargerV800]" fmt, ## args)
#define T_V_ARRAY_LENGTH	31

#ifndef bit
#define bit(x)                          (1 << (x))
#endif

enum adc_mode {
	SINGLE_MODE,
	LOOP_MODE,
	EIS_MODE,
};

struct scharger_adc {
	struct device *dev;
	struct regmap *regmap;
	enum adc_mode mode;
	struct wakeup_source *wake_lock;
	struct mutex adc_conv_lock;
	int batt_temp_compensation_en;
	int vol_offset;
};

static struct scharger_adc *g_adc = NULL;

int scharger_v800_get_voltage_offset(void)
{
	u8 data;
	if (g_adc == NULL) {
		scharger_err("%s adc is null.\n", __func__);
		return -1;
	}
	scharger_efuse_read(SCHARGER_ADC_OFFSET_EFUSE_ADDR, &data);
	/* Bit7 is the sign bit. Bit[0:6] is the data bit. Offset = data * 2mv */
	if (data & bit(7))
		g_adc->vol_offset = 0 - (data & 0x7f) * 2;
	else
		g_adc->vol_offset = (data & 0x7f) * 2;

	return 0;
}

/* mode: single = 0, loop = 1 */
static void adc_loop_mode_set(struct scharger_adc *adc , u32 en)
{
	int ret;

	ret = regmap_update_bits(adc->regmap, CHG_HKADC_CTRL1_REG,
			 CHG_HKADC_SEQ_LOOP_MSK, en << CHG_HKADC_SEQ_LOOP_SHIFT);
	if (ret != 0)
		scharger_err("%s, en %d error %d\n", __func__, en, ret);
}

static void adc_enable(struct scharger_adc *adc , u32 en)
{
	int ret;

	ret = regmap_update_bits(adc->regmap, CHG_HKADC_EN_REG,
			 CHG_HKADC_EN_MSK, en << CHG_HKADC_EN_SHIFT);
	if (ret != 0)
		scharger_err("%s, eis_en %d error %d\n", __func__, en, ret);
}

static void adc_loop_channel_set(struct scharger_adc *adc, int en)
{
	int ret = 0;
	unsigned int val_l = 0;
	unsigned int val_h = 0;

	if (en != 0) {
		/* select all channel */
		val_l = 0xff;
		val_h = 0x0f;
	}
	ret += regmap_write(adc->regmap, CHG_HKADC_SEQ_CH_L_REG, val_l);
	ret += regmap_write(adc->regmap, CHG_HKADC_SEQ_CH_H_REG, val_h);
	if (ret != 0)
		scharger_err("%s, error %d\n", __func__, ret);
}

static void adc_channel_set(struct scharger_adc *adc , unsigned int ch)
{
	unsigned int val_l = 0x00;
	unsigned int val_h = 0x00;
	int ret = 0;

	if (ch < CHG_ADC_CH_L_MAX)
		val_l = (1 << ch);
	else
		val_h = (1 << (ch - CHG_ADC_CH_L_MAX));

	ret += regmap_write(adc->regmap, CHG_HKADC_SEQ_CH_L_REG, val_l);
	ret += regmap_write(adc->regmap, CHG_HKADC_SEQ_CH_H_REG, val_h);
	if (ret != 0)
		scharger_err("%s, ch %d error %d\n", __func__, ch, ret);
}

static void adc_start(struct scharger_adc *adc)
{
	int ret;

	ret = regmap_update_bits
		(adc->regmap, CHG_HKADC_START_REG, CHG_HKADC_START_MSK, 1 << CHG_HKADC_START_SHIFT);
	if (ret != 0)
		scharger_err("%s, error %d\n", __func__, ret);
}

static void adc_read_request(struct scharger_adc *adc)
{
	int ret;

	ret = regmap_update_bits(adc->regmap, CHG_HKADC_RD_SEQ_REG,
			 CHG_HKADC_RD_SEQ_MSK, 1 << CHG_HKADC_RD_SEQ_SHIFT);
	if (ret != 0)
		scharger_err("%s error %d\n", __func__, ret);
}

static void adc_read_data(struct scharger_adc *adc, int ch, unsigned short *adc_data)
{
	int ret;

	ret = regmap_bulk_read(adc->regmap, REG_ADC_DATA_BASE + (ADC_DATA_LEN * ch),
			 (u8 *)adc_data, ADC_DATA_LEN);
	if (ret != 0){
		scharger_err("%s error %d\n", __func__, ret);
		*adc_data = 0;
	}
}

static void adc_loop_mode_enable(struct scharger_adc * adc, u32 en)
{
	if (adc->mode == (LOOP_MODE ^ en)) {
		scharger_err("%s en %d, pre mode %d\n", __func__, en, adc->mode);
		adc_loop_mode_set(adc, en);
		adc_enable(adc, en);
		adc_loop_channel_set(adc, en);
		if (en) {
			adc_start(adc);
			adc->mode = LOOP_MODE;
		} else {
			adc->mode = SINGLE_MODE;
		}
	}
}

static int adc_complete(struct scharger_adc * adc)
{
	int ret;
	unsigned int val = 0;
	int time = 10;

	while(!(val & CHG_HKADC_DATA_VALID_MSK) && time){
		mdelay(1);
		ret = regmap_read(adc->regmap, CHG_HKADC_DATA_VALID_REG, &val);
		if (ret != 0)
			scharger_err("%s error %d\n", __func__, ret);
		time--;
	}

	if (time == 0)
		return -1;
	return 0;
}

static int adc_get_val_single(struct scharger_adc * adc, u32 ch, unsigned short *data)
{
	int timeout;
	int ret = 0;

	adc_enable(adc, 1);
	adc_channel_set(adc, ch);
	adc_start(adc);

	timeout = adc_complete(adc);
	if (timeout) {
		*data = 0;
		ret = -EINVAL;
		goto out;
	}

	adc_read_data(adc, ch, data);
out:
	adc_enable(adc, 0);
	return ret;
}

static int scharger_get_adc_value(struct scharger_adc * adc, u32 ch, unsigned short *data)
{
	int ret = 0;

	if (adc == NULL) {
		scharger_err("%s adc is null.\n", __func__);
		*data = 0;
		return -EINVAL;
	}

	mutex_lock(&adc->adc_conv_lock);
	__pm_stay_awake(adc->wake_lock);

	if (adc->mode == LOOP_MODE) {
		adc_read_request(adc);
		adc_read_data(adc, ch, data);
	} else {
		/* single mode */
		ret = adc_get_val_single(adc, ch, data);
	}

	__pm_relax(adc->wake_lock);
	mutex_unlock(&adc->adc_conv_lock);
	return ret;
}

int scharger_v800_get_sbu1_res(void)
{
	int val = 0;

	if (g_adc == NULL) {
		scharger_err("%s adc is null.\n", __func__);
		return 0;
	}

	regmap_write(g_adc->regmap, CHG_TYPE_CFG_REG_4_REG, SBU1_1UA_DET);
	scharger_get_adc_value(g_adc, CHG_ADC_CH_SBU, (unsigned short*)&val);
	regmap_write(g_adc->regmap, CHG_TYPE_CFG_REG_4_REG, SBU_DET_DIS);

	/*
	 * code val to actual val
	 * res = hkadc_data * 5.4 * 1000 / 4096 / irsel (kohom);
	 * if res bigger than 100kohom, use 10uA retest.
	 */
	val = val * 5400 / 4096 / SCHARGER_1UA;
	if (val < SCHARGER_100KOHOM) {
		regmap_write(g_adc->regmap, CHG_TYPE_CFG_REG_4_REG, SBU1_10UA_DET);
		scharger_get_adc_value(g_adc, CHG_ADC_CH_SBU, (unsigned short*)&val);
		regmap_write(g_adc->regmap, CHG_TYPE_CFG_REG_4_REG, SBU_DET_DIS);
		val = val * 5400 / 4096 / SCHARGER_10UA;
	}

	scharger_inf("[%s] %u\n", __func__, val);
	return val;
}

int scharger_v800_get_sbu2_res(void)
{
	int val = 0;
	int ret = 0;

	if (g_adc == NULL) {
		scharger_err("%s adc is null.\n", __func__);
		return 0;
	}

	ret += regmap_write(g_adc->regmap, CHG_TYPE_CFG_REG_4_REG, SBU2_1UA_DET);
	ret += scharger_get_adc_value(g_adc, CHG_ADC_CH_SBU, (unsigned short*)&val);
	ret += regmap_write(g_adc->regmap, CHG_TYPE_CFG_REG_4_REG, SBU_DET_DIS);

	/*
	 * code val to actual val
	 * res = hkadc_data * 5.4 * 1000 / 4096 / irsel (kohom);
	 * if res bigger than 100kohom, use 10uA retest.
	 */
	val = val * 5400 / 4096 / SCHARGER_1UA;
	if (val < SCHARGER_100KOHOM) {
		ret += regmap_write(g_adc->regmap, CHG_TYPE_CFG_REG_4_REG, SBU2_10UA_DET);
		ret += scharger_get_adc_value(g_adc, CHG_ADC_CH_SBU, (unsigned short*)&val);
		ret += regmap_write(g_adc->regmap, CHG_TYPE_CFG_REG_4_REG, SBU_DET_DIS);
		val = val * 5400 / 4096 / SCHARGER_10UA;
	}
	if (ret != 0) {
		scharger_err("%s fail\n", __func__);
		return 0;
	}
	scharger_inf("[%s] %d\n", __func__, val);
	return val;
}

int scharger_v800_get_dp_res(void)
{
	int val = 0;
	int ret = 0;

	if (g_adc == NULL) {
		scharger_err("%s adc is null.\n", __func__);
		return 0;
	}

	ret += regmap_write(g_adc->regmap, CHG_FCP_TOP_CFG_REG_REG, DP_1UA_DET);
	ret += scharger_get_adc_value(g_adc, CHG_ADC_CH_VDPDN, (unsigned short*)&val);
	ret += regmap_write(g_adc->regmap, CHG_FCP_TOP_CFG_REG_REG, DP_DET_DIS);

	/*
	 * code val to actual val
	 * res = hkadc_data * 5.4 * 1000 / 4096 / irsel (kohom);
	 * if res bigger than 100kohom, use 10uA retest.
	 */
	val = val * 5400 / 4096 / SCHARGER_1UA;
	if (val < SCHARGER_100KOHOM) {
		ret += regmap_write(g_adc->regmap, CHG_FCP_TOP_CFG_REG_REG, DP_10UA_DET);
		ret += scharger_get_adc_value(g_adc, CHG_ADC_CH_VDPDN, (unsigned short*)&val);
		ret += regmap_write(g_adc->regmap, CHG_FCP_TOP_CFG_REG_REG, DP_DET_DIS);
		val = val * 5400 / 4096 / SCHARGER_10UA;
	}
	if (ret != 0) {
		scharger_err("%s fail\n", __func__);
		return 0;
	}
	scharger_inf("[%s] %d\n", __func__, val);
	return val;
}

int scharger_v800_get_ibat(void)
{
	int val = 0;
	int ibat;

	if (g_adc == NULL) {
		scharger_err("%s adc is null.\n", __func__);
		return 0;
	}

	(void)scharger_get_adc_value(g_adc, CHG_ADC_CH_IBAT, (unsigned short*)&val);
	/*
	 * code val to actual val
	 * ibat = hkadc_data * 10 * 1.8 * 1000 / 4096 (mA);
	 */
	ibat = val * 1800 * 10 / 4096;
	/* When the ibat gear is 24A, ibat value needs to be doubled. */
	ibat *= 2;

	return ibat;
}

int scharger_v800_get_vbat(void)
{
	int val = 0;

	if (g_adc == NULL) {
		scharger_err("%s adc is null.\n", __func__);
		return 0;
	}

	(void)scharger_get_adc_value(g_adc, CHG_ADC_CH_VBAT, (unsigned short*)&val);

	/* when channel data less than 0x31F, vbat is 0 */
	if (val < 0x31F) {
		scharger_err("%s channel data less then 0x31F\n", __func__);
		return 0;
	}

	/*
	 * code val to actual val
	 * VBAT = hkadc_data * 1.8 * 1000 * 2 / 4096 + 1.8 * 1000 mV;
	 */
	val = val * 1800 * 2 / 4096 + 1800;
	return val;
}

int scharger_v800_get_vbus(void)
{
	int val = 0;

	if (g_adc == NULL) {
		scharger_err("%s adc is null.\n", __func__);
		return 0;
	}

	(void)scharger_get_adc_value(g_adc, CHG_ADC_CH_VBUS, (unsigned short*)&val);

	/* when channel data less than 0x7F, vbus is 0 */
	if (val < 0x7F)
		return 0;

	/*
	 * code val to actual val
	 * VBUS = hkadc_data * 1.8 * 1000 * 13 / 4096 + 1.8 * 1000 mV;
	 */
	val = val * 1800 * 13 / 4096 + 1800 - g_adc->vol_offset;
	if (val < 0)
		val = 0;
	return val;
}

int scharger_v800_get_vsysh(void)
{
	int val = 0;

	if (g_adc == NULL) {
		scharger_err("%s adc is null.\n", __func__);
		return 0;
	}

	(void)scharger_get_adc_value(g_adc, CHG_ADC_CH_VSYSH, (unsigned short*)&val);

	/* when channel data less than 0x31F, vbat is 0 */
	if (val < 0x31F)
		return 0;

	/*
	 * code val to actual val
	 * VBAT = hkadc_data * 1.8 * 1000 * 2 / 4096 + 1.8 * 1000 mV;
	 */
	val = val * 1800 * 2 / 4096 + 1800;

	return val;
}

int scharger_v800_get_vpsw(void)
{
	int val = 0;

	if (g_adc == NULL) {
		scharger_err("%s adc is null.\n", __func__);
		return 0;
	}

	(void)scharger_get_adc_value(g_adc, CHG_ADC_CH_VPSW, (unsigned short*)&val);

	/* when channel data less than 0x7F, vpsw is 0 */
	if (val < 0x7F)
		return 0;

	/*
	 * code val to actual val
	 * VPSW = hkadc_data * 1.8 * 1000 * 13 / 4096 + 1.8 * 1000 mV;
	 */
	val = val * 1800 * 13 / 4096 + 1800 - g_adc->vol_offset;
	if (val < 0)
		val = 0;
	return val;
}

int scharger_v800_get_vusb(void)
{
	int val = 0;

	if (g_adc == NULL) {
		scharger_err("%s adc is null.\n", __func__);
		return 0;
	}

	(void)scharger_get_adc_value(g_adc, CHG_ADC_CH_VUSB, (unsigned short*)&val);

	/* when channel data less than 0x7F, vusb is 0 */
	if (val < 0x7F)
		return 0;

	/*
	 * code val to actual val
	 * VUSB = hkadc_data * 1.8 * 1000 * 13 / 4096 + 1.8 * 1000 mV;
	 */
	val = val * 1800 * 13 / 4096 + 1800 - g_adc->vol_offset;
	if (val < 0)
		val = 0;
	return val;
}

/* NTC Table */
static int t_v_table[][2] = {
	{ -273, 4095}, { -40, 3764}, { -36, 3689}, { -32, 3602}, { -28, 3500},
	{ -24, 3387}, { -20, 3261}, { -16, 3122}, { -12, 2973}, { -8, 2814},
	{ -4, 2650}, { 0, 2480}, { 4, 2308}, { 8, 2136}, { 12, 1967},
	{ 16, 1803}, { 20, 1646}, { 24, 1497}, { 28, 1360}, { 32, 1230},
	{ 36, 1111}, { 40, 1001}, { 44, 903}, { 48, 812}, { 52, 729},
	{ 56, 655}, { 60, 590}, { 64, 531}, { 74, 406}, { 84, 313},
	{ 125, 110}, { 0, 0},
};
/* NTC Table length */
static int charger_adc_to_temp(int temp_volt)
{
	int temprature = 0;
	int i;

	if (temp_volt >= t_v_table[0][1])
		return t_v_table[0][0];

	if (temp_volt <= t_v_table[T_V_ARRAY_LENGTH - 1][1])
		return t_v_table[T_V_ARRAY_LENGTH - 1][0];

	/* else */
	for (i = 0; i < T_V_ARRAY_LENGTH; i++) {
		if (temp_volt == t_v_table[i][1])
			return t_v_table[i][0];

		if (temp_volt > t_v_table[i][1])
			break;
	}
	if (i == 0)
		return t_v_table[0][0];

	if ((t_v_table[i][1] - t_v_table[i - 1][1]) != 0)
		temprature = t_v_table[i - 1][0] +
			(long)(temp_volt - t_v_table[i - 1][1]) *
				(t_v_table[i][0] - t_v_table[i - 1][0]) /
				(t_v_table[i][1] - t_v_table[i - 1][1]);

	return temprature;
}

static u32 battery_temp_compensation(struct scharger_adc *di, u32 adc_code)
{
	if (di->batt_temp_compensation_en == 0)
		goto no_compensation;

no_compensation:
	return adc_code;
}

int scharger_v800_get_tsbat(void)
{
	int ret = 0;
	u32 adc_code = 0;
	u32 adc_code_new;
	int tbat;

	if (g_adc == NULL) {
		scharger_err("%s adc is null.\n", __func__);
		return 0;
	}

	ret = scharger_get_adc_value(g_adc, CHG_ADC_CH_TSBAT, (unsigned short*)&adc_code);
	if (ret != 0) {
		scharger_err("[%s]get ts_bat fail,ret:%d\n", __func__, ret);
		return 0;
	}
	adc_code_new = battery_temp_compensation(g_adc, adc_code);
	tbat = charger_adc_to_temp(adc_code_new);
	return tbat;
}

int scharger_v800_get_tdie(void)
{
	int val = 0;

	if (g_adc == NULL) {
		scharger_err("%s adc is null.\n", __func__);
		return 0;
	}

	(void)scharger_get_adc_value(g_adc, CHG_ADC_CH_TDIE, (unsigned short*)&val);

	/*
	 * code val to actual temperature val
	 * V_tdie = (hkadc_data * 1800) / 4096  mV;
	 * 1437277872: base value, for chip set
	 * 1000000: conversion scale
	 * 3406962: the reference value
	 */
	val = (val  * 1800) / 4096;
	val = (int)(1437277872 - val * 1000000) / (3406962);

	return val;
}

static int scharger_v800_get_ibus_in_chg_mode(unsigned int ibus1, unsigned int ibus2, unsigned int ibus_ref, unsigned int ilimit)
{
	struct current_charger_mode *charger_mode;
	int val = 0;

	charger_mode = scharger_v800_get_current_charge_mode();
	if(charger_mode == NULL) {
		scharger_err("%s charger_mode is NULL!\n", __func__);
		return 0;
	}

	if(charger_mode->chg_mode == BUCK5V || charger_mode->chg_mode == BUCK9V ||
		 charger_mode->chg_mode == SC11 || charger_mode->chg_mode == SC21 ||
		charger_mode->chg_mode == SC41) {
		/* forward ibus formula: (ibus*10*130*40)/6/ibus_ref, unit: mA */
		ibus1 = (ibus1 * 10 * 130 * 40) / 6 / ibus_ref;
		ibus2 = (ibus2 * 10 * 130 * 40) / 6 / ibus_ref;
	} else {
		/* reverse ibus formula: (ibus*11*130*20)/3/ibus_ref, unit: mA */
		ibus1 = (ibus1 * 11 * 130 * 20) / 3 / ibus_ref;
		ibus2 = (ibus2 * 11 * 130 * 20) / 3 / ibus_ref;
	}

	val = ibus1 + ibus2;

	/* When in BUCK/OTG mode, ibus needs to divide by 2 */
	if(charger_mode->chg_mode == BUCK5V || charger_mode->chg_mode == BUCK9V ||
		charger_mode->chg_mode == OTG5V || charger_mode->chg_mode == OTG9V)
		val /= 2;

	if (charger_mode->chg_mode == BUCK5V || charger_mode->chg_mode == BUCK9V) {
		ilimit &= CHG_ILIMIT_MSK;
		ilimit >>= CHG_ILIMIT_SHIFT;
		if (ilimit < CHG_ILIMIT_600MA_REG)
				val = val / CHG_IBUS_DIV;
	}

	/* 50: factory adjust 50mA */
	if (charger_mode->chg_mode == SC21 || charger_mode->chg_mode == SC41) {
		if (val < 50)
			val = 0;
		else
			val -= 50;
	}
	scharger_inf("[%s] mode=%u, ilimit:0x%x, val=%d, ibus1=%u, ibus2=%u\n",
		__func__, charger_mode->chg_mode, ilimit, val, ibus1, ibus2);

	return val;
}

int scharger_v800_get_ibus(void)
{
	unsigned short reg_data = 0;
	unsigned int ibus1 = 0;
	unsigned int ibus2 = 0;
	unsigned int ibus_ref = 0;
	unsigned int ilimit = 0;
	int ret = 0;

	if (g_adc == NULL) {
		scharger_err("%s adc is null.\n", __func__);
		return 0;
	}

	ret += scharger_get_adc_value(g_adc, CHG_ADC_CH_IBUS1, &reg_data);
	ibus1 = (unsigned int)reg_data;
	ret += scharger_get_adc_value(g_adc, CHG_ADC_CH_IBUS2, &reg_data);
	ibus2 = (unsigned int)reg_data;
	ret += scharger_get_adc_value(g_adc, CHG_ADC_CH_IBUS_REF, &reg_data);
	ibus_ref = (unsigned int)reg_data;
	ret += regmap_read(g_adc->regmap, CHG_BUCK_ILIMIT_REG, &ilimit);
	if (ret != 0) {
		scharger_err("[%s]get ibus fail\n", __func__);
		return 0;
	}

	/* When (ibus_ref * 1.8) / 4096 is less than 0.5, we consider ibus to be 0 */
	if ((ibus_ref * 18) / 4096  < 5) {
		scharger_inf("[%s] ibus_ref is small value, ibus=0, ibus1=%u, ibus2=%u\n",
			__func__, ibus1, ibus2);
		return 0;
	}

	return scharger_v800_get_ibus_in_chg_mode(ibus1, ibus2, ibus_ref, ilimit);
}

static int scharger_adc_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct scharger_adc *adc = NULL;
	int ret = 0;

	adc = devm_kzalloc(dev, sizeof(*adc), GFP_KERNEL);
	if (adc == NULL) {
		scharger_err("[%s]adc is null.\n", __func__);
		return -ENOMEM;
	}

	adc->wake_lock = wakeup_source_register(dev, "scharger v800 adc wakelock");
	mutex_init(&adc->adc_conv_lock);

	adc->dev = dev;
	adc->regmap = dev_get_regmap(dev->parent, NULL);
	if (adc->regmap == NULL) {
		scharger_err("Parent regmap unavailable.\n");
		return -ENXIO;
	}

	adc_loop_mode_enable(adc, 0);
	g_adc = adc;

	scharger_err("[%s] succ\n", __func__);

	return ret;
}

static void scharger_adc_shutdown(struct platform_device *pdev)
{
}

const static struct of_device_id scharger_v800_adc_of_match[] = {
	{
		.compatible = "hisilicon,scharger-v800-adc",
		.data = NULL,
	},
	{},
};

static struct platform_driver scharger_v800_adc_driver = {
	.driver = {
		.name	= "scharger_v800_adc",
		.owner  = THIS_MODULE,
		.of_match_table = scharger_v800_adc_of_match,
	},
	.probe	= scharger_adc_probe,
	.shutdown	= scharger_adc_shutdown,
};

static int __init scharger_adc_init(void)
{
	return platform_driver_register(&scharger_v800_adc_driver);
}

static void __exit scharger_adc_exit(void)
{
	platform_driver_unregister(&scharger_v800_adc_driver);
}

subsys_initcall_sync(scharger_adc_init);
module_exit(scharger_adc_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("schargerV800 adc driver");