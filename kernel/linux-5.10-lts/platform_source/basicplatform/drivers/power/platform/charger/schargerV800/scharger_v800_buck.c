/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Device driver for schargerV800 buck
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
#include <linux/workqueue.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/mfd/pmic_platform.h>
#include <linux/platform_drivers/usb/chip_usb.h>

#include <platform_include/basicplatform/linux/power/platform/coul/coul_merge_drv.h>
#include <platform_include/basicplatform/linux/power/platform/bci_battery.h>
#include <huawei_platform/power/huawei_charger.h>
#include <huawei_platform/power/wireless/wireless_transmitter.h>

#include "scharger_v800_buck.h"
#include "scharger_v800_watchdog.h"
#include "scharger_v800_adc.h"
#include "../hisi_fcp/hisi_fcp_drv.h"
#include "../scharger_dynamic_cv.h"

static int set_5v_otg_enable(int enable);
static int set_9v_otg_enable(int enable);

static int force_set_term_flag = CHG_STAT_DISABLE;
static int iin_set = CHG_ILIMIT_85;
static struct scharger_buck *g_buck = NULL;
static struct cv_info *g_cv_info = NULL;
static int usbovp_state = POWER_SW_OFF;
static int pswovp_state = POWER_SW_OFF;

/*
 * chg_limit_v800[CHG_LMT_NUM_V800]: the current limit when setting charger
 *                                   value, unit: mA
 * These arrays are set to replace the tedious "else if" statements,instead,
 * a concise "for" statement is used in this version, the items transferred from
 * the original Macro definition like #define CHG_ILIMIT_1100           1100 in
 * the arrays are the values to be given to the variables in the "for" loop
 */
static const int chg_limit_v800[CHG_LMT_NUM_V800] = { 85, 130, 200, 300, 400, 475, 600,
					       700, 800, 825, 1000, 1100, 1200,
					       1300, 1400, 1500, 1600, 1700,
					       1800, 1900, 2000, 2100, 2200,
					       2300, 2400, 2500, 2600, 2700,
					       2800, 2900, 3000, 3100 };

static struct opt_regs buck_common_v800_option_regs[] = {
	/* reg, mask, shift, val */
	reg_cfg(0x94e, 0xff, 0, 0x08),
	reg_cfg(0x3b0, 0xff, 0, 0x01),
	reg_cfg(0x9b6, 0xff, 0, 0x90),
	reg_cfg(0x95c, 0xff, 0, 0x9d),
	reg_cfg(0x966, 0xff, 0, 0x0d),
	reg_cfg(0x95f, 0xff, 0, 0x0e),
	reg_cfg(0x935, 0xff, 0, 0x01),
	reg_cfg(0x2035, 0xff, 0, 0x40),
};

static struct opt_regs buck_common_v801_option_regs[] = {
	/* reg, mask, shift, val */
	reg_cfg(0x3b0, 0xff, 0, 0x01),
	reg_cfg(0x9b6, 0xff, 0, 0x90),
	reg_cfg(0x2035, 0xff, 0, 0x40),
	reg_cfg(0x94e, 0xff, 0, 0x00),
	reg_cfg(0x948, 0xff, 0, 0x68),
	reg_cfg(0x966, 0xff, 0, 0x0d),
	reg_cfg(0x95f, 0xff, 0, 0x0e),
	reg_cfg(0x95c, 0xff, 0, 0x9d),
	reg_cfg(0x94e, 0xff, 0, 0x08),
};

static struct opt_regs buck_v800_5v_extra_opt_regs[] = {
	/* reg, mask, shift, val */
	reg_cfg(0x377, 0xff, 0, 0x15),
	reg_cfg(0x376, 0xff, 0, 0xB1),
	reg_cfg(0x375, 0xff, 0, 0x1C),
	reg_cfg(0x374, 0xff, 0, 0x04),
	reg_cfg(0x967, 0xff, 0, 0x57),
	reg_cfg(0x378, 0xff, 0, 0x37),
	reg_cfg(0x972, 0xff, 0, 0x07),
	reg_cfg(0x95D, 0xff, 0, 0x0D),
};

static struct opt_regs buck_v800_9v_extra_opt_regs[] = {
	/* reg, mask, shift, val */
	reg_cfg(0x967, 0xff, 0, 0x55),
	reg_cfg(0x378, 0xff, 0, 0x35),
	reg_cfg(0x95D, 0xff, 0, 0x0C),
	reg_cfg(0x377, 0xff, 0, 0x31),
	reg_cfg(0x376, 0xff, 0, 0x31),
	reg_cfg(0x375, 0xff, 0, 0x09),
	reg_cfg(0x972, 0xff, 0, 0x07),
	reg_cfg(0x374, 0xff, 0, 0x01),
};

static struct opt_regs v800_otg_5v_option_regs[] = {
	/* reg, mask, shift, val */
	reg_cfg( 0x330, 0xFF, 0, 0x1  ),
	reg_cfg( 0x935, 0xFF, 0, 0x06 ),
	reg_cfg( 0x37D, 0xFF, 0, 0x1  ),
	reg_cfg( 0x37E, 0xFF, 0, 0x9  ),
	reg_cfg( 0x37C, 0xFF, 0, 0x4  ),
	reg_cfg( 0x380, 0xFF, 0, 0x0  ),
	reg_cfg( 0x96D, 0xFF, 0, 0x13 ),
	reg_cfg( 0x96F, 0xFF, 0, 0xA3 ),
	reg_cfg( 0x966, 0xFF, 0, 0x8D ),
	reg_cfg( 0x973, 0xFF, 0, 0XB6 ),
	reg_cfg( 0x9A3, 0xFF, 0, 0x4  ),
	reg_cfg( 0x37B, 0xFF, 0, 0xB0 ),
	reg_cfg( 0x371, 0xFF, 0, 0x7  ),
	reg_cfg( 0x37F, 0xFF, 0, 0x30 ),
	reg_cfg( 0x974, 0xFF, 0, 0x8  ),
	reg_cfg( 0x96E, 0xFF, 0, 0xBD ),
	reg_cfg( 0x992, 0xFF, 0, 0x07 ),
	reg_cfg( 0X936, 0xFF, 0, 0X10 ),
	reg_cfg( 0x973, 0xFF, 0, 0XB6 ),
	reg_cfg( 0X938, 0xFF, 0, 0x00 ),
	reg_cfg( 0x381, 0xFF, 0, 0x44 ),
	reg_cfg( 0x909, 0xFF, 0, 0x00 ),
};

static struct opt_regs v800_otg_9v_option_regs[] = {
	/* reg, mask, shift, val */
	reg_cfg( 0x330, 0xFF, 0, 0x1  ),
	reg_cfg( 0x935, 0xFF, 0, 0x06 ),
	reg_cfg( 0x37D, 0xFF, 0, 0x1  ),
	reg_cfg( 0x37E, 0xFF, 0, 0x9  ),
	reg_cfg( 0x37C, 0xFF, 0, 0x4  ),
	reg_cfg( 0x380, 0xFF, 0, 0x0  ),
	reg_cfg( 0x96D, 0xFF, 0, 0x13 ),
	reg_cfg( 0x96F, 0xFF, 0, 0xA3 ),
	reg_cfg( 0x966, 0xFF, 0, 0x8D ),
	reg_cfg( 0x973, 0xFF, 0, 0XB4 ),
	reg_cfg( 0x9A3, 0xFF, 0, 0x4  ),
	reg_cfg( 0x37B, 0xFF, 0, 0xD0 ),
	reg_cfg( 0x371, 0xFF, 0, 0x1  ),
	reg_cfg( 0x37F, 0xFF, 0, 0x34 ),
	reg_cfg( 0x974, 0xFF, 0, 0x9  ),
	reg_cfg( 0x96E, 0xFF, 0, 0xAD ),
	reg_cfg( 0x992, 0xFF, 0, 0x27 ),
	reg_cfg( 0X936, 0xFF, 0, 0X50 ),
	reg_cfg( 0X938, 0xFF, 0, 0x20 ),
	reg_cfg( 0x381, 0xFF, 0, 0x44 ),
	reg_cfg( 0x909, 0xFF, 0, 0x00 ),
};

static struct opt_regs v800_otg_5v_exit_option_regs[] = {
	/* reg, mask, shift, val */
	reg_cfg( 0x935, 0xFF, 0, 0x00 ),
	reg_cfg( 0x37D, 0xFF, 0, 0x04 ),
	reg_cfg( 0x37E, 0xFF, 0, 0x1C ),
	reg_cfg( 0x37C, 0xFF, 0, 0x2C ),
	reg_cfg( 0x380, 0xFF, 0, 0x15 ),
	reg_cfg( 0x96D, 0xFF, 0, 0x23 ),
	reg_cfg( 0x96F, 0xFF, 0, 0xC3 ),
	reg_cfg( 0x966, 0xFF, 0, 0x09 ),
	reg_cfg( 0x973, 0xFF, 0, 0xA6 ),
	reg_cfg( 0x9A3, 0xFF, 0, 0x00 ),
	reg_cfg( 0x37B, 0xFF, 0, 0x65 ),
	reg_cfg( 0x371, 0xFF, 0, 0x09 ),
	reg_cfg( 0x37F, 0xFF, 0, 0xB1 ),
	reg_cfg( 0x974, 0xFF, 0, 0x08 ),
	reg_cfg( 0x96E, 0xFF, 0, 0xAB ),
	reg_cfg( 0x992, 0xFF, 0, 0x07 ),
	reg_cfg( 0X936, 0xFF, 0, 0X00 ),
	reg_cfg( 0X938, 0xFF, 0, 0X00 ),
	reg_cfg( 0x909, 0xFF, 0, 0x01 ),
	reg_cfg( 0x381, 0xFF, 0, 0x35 ),
};

static struct opt_regs v800_otg_9v_exit_option_regs[] = {
	/* reg, mask, shift, val */
	reg_cfg( 0x935, 0xFF, 0, 0x00 ),
	reg_cfg( 0x37D, 0xFF, 0, 0x04 ),
	reg_cfg( 0x37E, 0xFF, 0, 0x1C ),
	reg_cfg( 0x37C, 0xFF, 0, 0x2C ),
	reg_cfg( 0x380, 0xFF, 0, 0x15 ),
	reg_cfg( 0x96D, 0xFF, 0, 0x23 ),
	reg_cfg( 0x96F, 0xFF, 0, 0xC3 ),
	reg_cfg( 0x966, 0xFF, 0, 0x09 ),
	reg_cfg( 0x973, 0xFF, 0, 0xA6 ),
	reg_cfg( 0x9A3, 0xFF, 0, 0x00 ),
	reg_cfg( 0x37B, 0xFF, 0, 0x65 ),
	reg_cfg( 0x371, 0xFF, 0, 0x09 ),
	reg_cfg( 0x37F, 0xFF, 0, 0xB1 ),
	reg_cfg( 0x974, 0xFF, 0, 0x08 ),
	reg_cfg( 0x96E, 0xFF, 0, 0xAB ),
	reg_cfg( 0X936, 0xFF, 0, 0X00 ),
	reg_cfg( 0X938, 0xFF, 0, 0X00 ),
	reg_cfg( 0x909, 0xFF, 0, 0x01 ),
	reg_cfg( 0x381, 0xFF, 0, 0x35 ),
};

void scharger_v800_mode_lock(void)
{
	struct scharger_buck *buck = g_buck;

	if (buck == NULL) {
		scharger_err("%s scharger_buck is NULL!\n", __func__);
		return;
	}

	scharger_err("%s +\n", __func__);
	mutex_lock(&buck->mode_lock);
}

void scharger_v800_mode_unlock(void)
{
	struct scharger_buck *buck = g_buck;

	if (buck == NULL) {
		scharger_err("%s scharger_buck is NULL!\n", __func__);
		return;
	}

	mutex_unlock(&buck->mode_lock);
	scharger_err("%s -\n", __func__);
}

struct current_charger_mode* scharger_v800_get_current_charge_mode(void)
{
	if (g_buck == NULL) {
		scharger_err("%s g_buck is NULL\n", __func__);
		return NULL;
	}

	return &g_buck->current_charge_mode;
}

int is_current_charge_mode(enum charger_mode mode)
{
	int cab;

	if (g_buck == NULL) {
		scharger_err("%s g_buck is NULL\n", __func__);
		return 0;
	}

	if (mode == OTG5V || mode == OTG9V) {
		if (wireless_tx_get_tx_open_flag())
			cab = WIRELSEE;
		else
			cab = WIRED;
	} else if (mode == RSC11 || mode == RSC12 || mode == RSC14) {
		cab = WIRELSEE;
	} else if (mode == WDRC12) {
		cab = WIRED;
	} else {
		if (charge_get_charger_type() == CHARGER_TYPE_WIRELESS)
			cab = WIRELSEE;
		else
			cab = WIRED;
	}

	scharger_inf("%s mode = %d, cab = %d, curr_mode = %d, curr_cab = %d\n",
		__func__, mode, cab, g_buck->current_charge_mode.chg_mode, g_buck->current_charge_mode.cable_flag);
	if (g_buck->current_charge_mode.chg_mode == mode && g_buck->current_charge_mode.cable_flag == cab) {
		scharger_err("%s same charging type as the previous one, exit directly, mode = %d, cab = %d\n", __func__, mode, cab);
		return 1;
	}
	return 0;
}

void scharger_v800_set_dc_charging(bool dc_charging)
{
	struct scharger_buck *buck = g_buck;

	if (buck == NULL) {
		scharger_err("%s scharger_buck is NULL!\n", __func__);
		return;
	}
	buck->dc_charging = dc_charging;
	scharger_inf("%s %d\n", __func__, dc_charging);
}

int scharger_v800_set_current_charge_mode(enum charger_mode mode)
{
	int cab;

	if (g_buck == NULL) {
		scharger_err("%s g_buck is NULL\n", __func__);
		return -1;
	}

	if (mode == OTG5V || mode == OTG9V) {
		if (wireless_tx_get_tx_open_flag())
			cab = WIRELSEE;
		else
			cab = WIRED;
	} else if (mode == RSC11 || mode == RSC12 || mode == RSC14) {
		cab = WIRELSEE;
	}  else if (mode == WDRC12) {
		cab = WIRED;
	} else {
		if (charge_get_charger_type() == CHARGER_TYPE_WIRELESS)
			cab = WIRELSEE;
		else
			cab = WIRED;
	}

	g_buck->current_charge_mode.chg_mode = mode;
	g_buck->current_charge_mode.cable_flag = cab;

	scharger_inf("%s mode = %d, cab = %d\n", __func__, mode, cab);

	return 0;
}

static int scharger_v800_s1q4s_enable(struct scharger_buck *buck, bool enable)
{
	scharger_inf("%s enable:%u\n", __func__, enable);
	return regmap_update_bits(buck->regmap, CHG_BOOST_CFG_REG_6_REG, CHG_S1Q4S_EN_MSK,
					 enable << CHG_S1Q4S_EN_SHIFT);
}

static int scharger_v800_s1q4_enable(struct scharger_buck *buck, bool enable)
{
	scharger_inf("%s enable:%u\n", __func__, enable);
	return regmap_update_bits(buck->regmap, CHG_BOOST_CFG_REG_6_REG, CHG_S1Q4_EN_MSK,
					 enable << CHG_S1Q4_EN_SHIFT);
}

static void scharger_v800_close_s1q4_s1q4s(void)
{
	struct scharger_buck *buck = g_buck;

	if (buck == NULL) {
		scharger_err("%s buck is NULL\n", __func__);
		return;
	}

	scharger_inf("%s\n", __func__);
	if (buck->scharger_version == CHIP_ID_V800) {
		/* close S1Q4S, when CHARGER_TYPE_NONE */
		scharger_v800_s1q4s_enable(buck, 0);
	} else {
		/* close S1Q4 and S1Q4S, when CHARGER_TYPE_NONE */
		scharger_v800_s1q4_enable(buck, 0);
		scharger_v800_s1q4s_enable(buck, 0);
	}
}

static int scharger_usb_notifier_call(struct notifier_block *usb_nb,
				    unsigned long event, void *data)
{
	struct scharger_buck *buck = container_of(usb_nb, struct scharger_buck, usb_nb);

	if (buck == NULL) {
		scharger_err("%s buck is NULL\n", __func__);
		return -EINVAL;
	}

	scharger_inf("%s + , event:%d\n", __func__, event);

	switch (event) {
	case CHARGER_TYPE_SDP:
		break;
	case CHARGER_TYPE_CDP:
		break;
	case CHARGER_TYPE_DCP:
		break;
	case CHARGER_TYPE_UNKNOWN:
		break;
	case CHARGER_TYPE_NONE:
		scharger_v800_mode_lock();
		(void)scharger_v800_set_vusb_ovp(BUCK5V);
		scharger_v800_mode_unlock();
		break;
	default:
		break;
	}

	return NOTIFY_OK;
}

static int scharger_get_terminal_voltage(void)
{
	unsigned int cv_reg;
	unsigned int cv_reg_adj;
	unsigned int cv_val;
	unsigned int cv_val_adj;
	int ret = 0;
	struct scharger_buck *buck = g_buck;

	if (buck == NULL) {
		scharger_err("%s scharger_buck is NULL!\n", __func__);
		return -EINVAL;
	}

	/* transfer from code value to actual value */
	ret = regmap_read(buck->regmap, CHG_CHARGER_CFG_REG_11_REG, &cv_reg);
	if (ret < 0) {
		scharger_err("%s err\n", __func__);
		return -1;
	}
	cv_reg &= CHG_FAST_VCHG_MSK;
	cv_reg >>= CHG_FAST_VCHG_SHIFT;
	cv_val = (cv_reg * CHG_FAST_VCHG_STEP_8000UV + CHG_FAST_VCHG_BASE_UV) / UV_PER_MV;

	ret = regmap_read(buck->regmap, CHG_CHARGER_CFG_REG_2_REG, &cv_reg_adj);
	if (ret < 0) {
		scharger_err("%s err\n", __func__);
		return -1;
	}
	cv_reg_adj &= CHG_CV_ADJ_MSK;
	cv_reg_adj >>= CHG_CV_ADJ_SHIFT;
	cv_val_adj = cv_reg_adj;

	return (int)(cv_val + cv_val_adj);
}

static void opt_regs_set(struct scharger_buck *buck, struct opt_regs *opt, unsigned int len)
{
	unsigned int i;

	for (i = 0; i < len; i++)
		(void)regmap_update_bits(buck->regmap, opt[i].reg, opt[i].mask,
			 opt[i].val << opt[i].shift);
}

/* Parameters: buck common 5V */
static void scharger_buck_common_opt_param(struct scharger_buck *buck)
{
	if (buck->scharger_version == CHIP_ID_V800)
		opt_regs_set(buck, buck_common_v800_option_regs,
			ARRAY_SIZE(buck_common_v800_option_regs));
	else
		opt_regs_set(buck, buck_common_v801_option_regs,
			ARRAY_SIZE(buck_common_v801_option_regs));
}

/* Parameters: mode:BUCK5V/BUCK9V/OTG5V/OTG9V */
static void scharger_buck_otg_opt_param(enum charger_mode mode)
{
	struct scharger_buck *buck = g_buck;
	if (buck == NULL) {
		scharger_err("%s scharger_buck is NULL!\n", __func__);
		return;
	}

	switch (mode) {
	case BUCK5V:
		opt_regs_set(buck, buck_v800_5v_extra_opt_regs,
				ARRAY_SIZE(buck_v800_5v_extra_opt_regs));
		break;
	case BUCK9V:
		opt_regs_set(buck, buck_v800_9v_extra_opt_regs,
				ARRAY_SIZE(buck_v800_9v_extra_opt_regs));
		break;
	case OTG5V:
		opt_regs_set(buck, v800_otg_5v_option_regs,
				ARRAY_SIZE(v800_otg_5v_option_regs));
		break;
	case OTG9V:
		opt_regs_set(buck, v800_otg_9v_option_regs,
				ARRAY_SIZE(v800_otg_9v_option_regs));
		break;
	default:
		scharger_err("%s mode not match!\n", __func__);
		break;
	}
}

/* Parameters: mode:BUCK5V/BUCK9V/OTG5V/OTG9V */
static void scharger_buck_otg_exit_opt_param(enum charger_mode mode)
{
	struct scharger_buck *buck = g_buck;

	if (buck == NULL) {
		scharger_err("%s scharger_buck is NULL!\n", __func__);
		return;
	}

	switch (mode) {
	case OTG5V:
		opt_regs_set(buck, v800_otg_5v_exit_option_regs,
				ARRAY_SIZE(v800_otg_5v_exit_option_regs));
		break;
	case OTG9V:
		opt_regs_set(buck, v800_otg_9v_exit_option_regs,
				ARRAY_SIZE(v800_otg_9v_exit_option_regs));
		break;
	default:
		scharger_err("%s mode not match!\n", __func__);
		break;
	}
}

void scharger_v800_vbat_lv_handle(struct scharger_buck *buck)
{
	int ret;
	unsigned int val = 0;

	ret = regmap_read(buck->regmap, CHG_VBAT_LV_REG, &val);
	if (ret != 0)
		scharger_inf("%s:regmap_read fail ret=%d\n", __func__, ret);
	scharger_inf("%s:CHG_VBAT_LV_REG is 0x%x\n", __func__, val);
	ret = regmap_update_bits(buck->regmap, CHG_VBAT_LV_REG,
				CHG_VBAT_LV_MSK, VBAT_LV_SET1 << CHG_VBAT_LV_SHIFT);
	if (ret != 0)
		scharger_inf("%s:set VBAT_LV_REG fail\n", __func__);
}

static void __scharger_parse_sw_attr(struct device_node *np,
	const char *prop, struct power_sw_attr *sw_attr)
{
	int i, len, col;
	const char *str = NULL;

	len = of_property_count_strings(np, prop);
	if (len <= 0) {
		sw_attr->num = -1;
		return;
	}

	for (i = 0; i < len; i++) {
		if (of_property_read_string_index(np, prop, i,  &str))
			goto err;
		col = i % POWER_SW_ATTR_TOTAL;
		switch (col) {
		case POWER_SW_ATTR_LABEL:
			sw_attr->label = str;
			break;
		case POWER_SW_ATTR_INDEX:
			if (kstrtoint(str, 0, &sw_attr->num))
				goto err;
			break;
		case POWER_SW_ATTR_DFLT:
			if (kstrtoint(str, 0, &sw_attr->dflt))
				goto err;
			break;
		case POWER_SW_ATTR_EN:
			if (kstrtoint(str, 0, &sw_attr->en))
				goto err;
			break;
		default:
			goto err;
		}
	}
	return;

err:
	scharger_err("parse_sw_attr: synx err\n");
	sw_attr->num = -1;
}

static void scharger_parse_sw_attr(struct device_node *np, struct scharger_buck *buck)
{
	__scharger_parse_sw_attr(np, "usbovp_sw_flag", &buck->usbovp_sw_flag_attr);
	__scharger_parse_sw_attr(np, "pswovp_sw_flag", &buck->pswovp_sw_flag_attr);
	__scharger_parse_sw_attr(np, "usbovp_sw", &buck->usbovp_sw_attr);
	__scharger_parse_sw_attr(np, "pswovp_sw", &buck->pswovp_sw_attr);
}

static void scharger_parse_dts(struct device_node *np, struct scharger_buck *buck)
{
	struct device_node *batt_node = NULL;
	int ret;

	if (np == NULL || buck == NULL) {
		scharger_err("%s buck or np is NULL!\n", __func__);
		return;
	}

	ret = of_property_read_u32(np, "bat_comp", &(buck->param_dts.bat_comp));
	if (ret != 0) {
		buck->param_dts.bat_comp = BAT_COMP_DEFAULT_VAL;
		scharger_err("get bat_comp failed, use default val %u\n", buck->param_dts.bat_comp);
	}
	scharger_inf("prase_dts bat_comp=%u\n", buck->param_dts.bat_comp);

	ret = of_property_read_u32(np, "vclamp", &(buck->param_dts.vclamp));
	if (ret != 0) {
		buck->param_dts.vclamp = VCLAMP_DEFAULT_VAL;
		scharger_err("get vclamp failed, use default val %u\n", buck->param_dts.vclamp);
	}
	scharger_inf("prase_dts vclamp=%u\n", buck->param_dts.vclamp);

	/* r_charger_mode, 0: under the battery, 1: above the battery */
	ret = of_property_read_u32(np, "r_charger_mode", &(buck->param_dts.r_charger_mode));
	if (ret != 0) {
		buck->param_dts.r_charger_mode = 0;
		scharger_err("get r_charger_mode failed, use default val %u\n",
			 buck->param_dts.r_charger_mode);
	}
	scharger_inf("prase_dts r_charger_mode=%u\n", buck->param_dts.r_charger_mode);

	ret = of_property_read_u32(np, "r_charger_uohm", &(buck->param_dts.r_charger_uohm));
	if (ret != 0) {
		buck->param_dts.r_charger_uohm = R_UOHM_DEFAULT;
		scharger_err("get r_charger_uohm failed, use default val %u\n",
			 buck->param_dts.r_charger_uohm);
	}
	scharger_inf("prase_dts r_charger_uohm=%u\n", buck->param_dts.r_charger_uohm);

	ret = of_property_read_u32(np, "auto_shipmode_en", &(buck->param_dts.auto_shipmode_en));
	if (ret != 0) {
		buck->param_dts.auto_shipmode_en = 0;
		scharger_err("get auto_shipmode_en failed, use default val %u\n",
			 buck->param_dts.auto_shipmode_en);
	}
	scharger_inf("prase_dts auto_shipmode_en=%u\n", buck->param_dts.auto_shipmode_en);

	ret = of_property_read_u32(np, "auto_shipmode_thre", &(buck->param_dts.auto_shipmode_thre));
	if (ret != 0) {
		buck->param_dts.auto_shipmode_thre = BAT_ECO_2900MV;
		scharger_err("get auto_shipmode_thre failed, use default val %u\n",
			 buck->param_dts.auto_shipmode_thre);
	}
	scharger_inf("prase_dts auto_shipmode_thre=%u\n", buck->param_dts.auto_shipmode_thre);

	ret = of_property_read_u32(np, "default_vterm", &(buck->param_dts.default_vterm));
	if (ret != 0) {
		scharger_err("get default_vterm failed\n");
		buck->param_dts.default_vterm = CHG_FAST_VCHG_4400;
	}
	scharger_inf("prase_dts default_vterm=%d\n", buck->param_dts.default_vterm);

	scharger_parse_sw_attr(np, buck);
	batt_node = of_find_compatible_node(NULL, NULL, "hisilicon,bci_battery");
	if (batt_node != NULL) {
		if (of_property_read_u32(batt_node, "battery_board_type",
			 &buck->is_board_type) != 0) {
			scharger_err("get battery_board_type fail!\n");
			buck->is_board_type = BAT_BOARD_ASIC;
		}
	} else {
		scharger_err("get hisi_bci_battery fail!\n");
		buck->is_board_type = BAT_BOARD_ASIC;
	}
	scharger_inf("%s is_board_type %u!\n", __func__, buck->is_board_type);
}

/* ibat resisitance val for ADC */
static int ibat_res_sel(struct scharger_buck *buck, unsigned int r_uohm)
{
	int ret = 0;
	unsigned int val = 0;

	/* default 24A ibat gear, ibat resistance need double */
	r_uohm *= 2;

	if (r_uohm == R_UOHM_1000)
		val = 0; /* 0: set ibat resisitance 1mohm */
	else if (r_uohm == R_UOHM_2000)
		val = 1; /* 1: set ibat resisitance 2mohm */
	else if (r_uohm == R_UOHM_500)
		val = 2; /* 2: set ibat resisitance 0.5mohm */
	else
		scharger_err("%s: %u uohm, not expected\n", __func__, r_uohm);

	ret += regmap_update_bits(buck->regmap, CHG_BAT_RES_MODE_REG,
			 CHG_BAT_RES_MODE_MSK, buck->param_dts.r_charger_mode << CHG_BAT_RES_MODE_SHIFT);
	ret += regmap_update_bits(buck->regmap, CHG_IBAT_RES_SEL_REG,
			 CHG_IBAT_RES_SEL_MSK, val << CHG_IBAT_RES_SEL_SHIFT);
	ret += regmap_update_bits(buck->regmap, CHG_SC_PRO_CFG_REG_5_REG,
			 CHG_IBAT_OCP_SET_MSK, IBAT_OCP_20A << CHG_IBAT_OCP_SET_SHIFT);

	return ret;
}

static int scharger_get_charge_enable(struct scharger_buck *buck)
{
	unsigned int charge_state = 0;

	(void)regmap_read(buck->regmap, CHG_CHG_OTG_MODE_CFG_REG, &charge_state);

	if (charge_state == DA_CHG_EN)
		return CHG_ENABLE;
	else
		return CHG_DISABLE;
}

int scharger_v800_set_charge_enable(int enable)
{
	struct scharger_buck *buck = g_buck;
	int ret;

	scharger_inf("%s %d\n", __func__, enable);

	if (buck == NULL) {
		scharger_err("%s scharger_buck is NULL!\n", __func__);
		return -EINVAL;
	}

	/* invalidate charge enable on udp board */
	if ((buck->is_board_type == BAT_BOARD_UDP) && (enable == CHG_ENABLE))
		return 0;

	if (enable == CHG_ENABLE)
		ret = regmap_update_bits(buck->regmap, CHG_CHG_OTG_MODE_CFG_REG,
					CHG_CHG_OTG_MODE_CFG_MSK, DA_CHG_EN << CHG_CHG_OTG_MODE_CFG_SHIFT);
	else
		ret = regmap_update_bits(buck->regmap, CHG_CHG_OTG_MODE_CFG_REG,
					CHG_CHG_OTG_MODE_CFG_MSK, DA_CHG_OTG_DIS << CHG_CHG_OTG_MODE_CFG_SHIFT);
	return ret;
}

static int scharger_v800_set_otg_enable(int enable)
{
	struct scharger_buck *buck = g_buck;
	int ret;

	scharger_inf("%s %d\n", __func__, enable);

	if (buck == NULL) {
		scharger_err("%s scharger_buck is NULL!\n", __func__);
		return -EINVAL;
	}

	if (enable)
		ret = regmap_update_bits(buck->regmap, CHG_CHG_OTG_MODE_CFG_REG,
			CHG_CHG_OTG_MODE_CFG_MSK, DA_OTG_EN << CHG_CHG_OTG_MODE_CFG_SHIFT);
	else
		ret = regmap_update_bits(buck->regmap, CHG_CHG_OTG_MODE_CFG_REG,
				 CHG_CHG_OTG_MODE_CFG_MSK, DA_CHG_OTG_DIS << CHG_CHG_OTG_MODE_CFG_SHIFT);
	return ret;
}

static int scharger_v800_set_vbus_ovp(enum charger_mode mode)
{
	int ret;
	unsigned int ov_reg = 0;
	unsigned int ov_vol = 0;
	struct scharger_buck *buck = g_buck;

	if (buck == NULL) {
		scharger_err("%s scharger_buck is NULL!\n", __func__);
		return -EINVAL;
	}

	switch(mode) {
	case BUCK5V:
		ov_reg = 0; /* 0: set ovp to 6.5V */
		ov_vol = 6500;
		break;
	case BUCK9V:
		ov_reg = 1; /* 1: set ovp to 10.5V */
		ov_vol = 10500;
		break;
	default:
		scharger_err("%s mode not match!\n", __func__);
		return -1;
	}

	scharger_err("%s ov_vol = %umv!\n", __func__, ov_vol);
	ret = regmap_update_bits(buck->regmap, CHG_SYS_DET_CFG_REG_6_REG, CHG_VBUS_OVP_MSK,
		ov_reg << CHG_VBUS_OVP_SHIFT);
	return ret;
}

static int scharger_v800_set_vbus_uvp(enum charger_mode mode)
{
	int ret = 0;
	unsigned int uv_reg;
	struct scharger_buck *buck = g_buck;
	unsigned int uv_vol;

	if (buck == NULL) {
		scharger_err("%s scharger_buck is NULL!\n", __func__);
		return -EINVAL;
	}

	switch(mode) {
	case BUCK5V:
		uv_reg = 0; /* 0: set uvp to 3.8V */
		uv_vol = 3800;
		break;
	case BUCK9V:
		uv_reg = 1; /* 1: set uvp to 7.4V */
		uv_vol = 7400;
		break;
	default:
		scharger_err("%s mode not match!\n", __func__);
		return -1;
	}

	scharger_err("%s uv_vol = %umv!\n", __func__, uv_vol);

	ret = regmap_update_bits(buck->regmap, CHG_REF_TOP_CFG_REG_3_REG, CHG_VBUS_UV_SET_MSK,
		uv_reg << CHG_VBUS_UV_SET_SHIFT);
	return ret;
}

unsigned int scharger_v800_get_sc_mode(void)
{
	struct scharger_buck *buck = g_buck;
	unsigned int mode = 0;

	if (buck == NULL) {
		scharger_err("%s scharger_buck is NULL!\n", __func__);
		return -EINVAL;
	}

	regmap_read(buck->regmap, CHG_DA_SC_MODE_REG, &mode);
	mode &= CHG_DA_SC_MODE_MSK;
	mode >>= CHG_DA_SC_MODE_SHIFT;
	return mode;
}

int scharger_v800_set_vusb_ovp(enum charger_mode mode)
{
	int ret = 0;
	unsigned int reg_val;
	struct scharger_buck *buck = g_buck;
	unsigned int ovp_sel;

	if (buck == NULL) {
		scharger_err("%s scharger_buck is NULL!\n", __func__);
		return -EINVAL;
	}

	switch(mode) {
	case BUCK5V:
		reg_val = 0; /* set vusb ovp 6v */
		ovp_sel = 6000;
		break;
	case BUCK9V:
		reg_val = 1; /* set vusb ovp 11v */
		ovp_sel = 11000;
		break;
	case SC11:
		reg_val = 7; /* set vusb ovp 6.5v */
		ovp_sel = 6500;
		break;
	case SC21:
	case WDRC12:
		reg_val = 1; /* set vusb ovp 11v */
		ovp_sel = 11000;
		break;
	case SC41:
		reg_val = 4; /* set vusb ovp 20v */
		ovp_sel = 20000;
		break;
	case OTG5V:
		reg_val = 0; /* set vusb ovp 6v */
		ovp_sel = 6000;
		break;
	case OTG9V:
		reg_val = 1; /* set vusb ovp 11v */
		ovp_sel = 11000;
		break;
	default:
		scharger_err("%s mode not match!\n", __func__);
		return -1;
	}

	scharger_inf("%s mode:%d, usb ovp reg_val:%umv\n", __func__, mode, ovp_sel);
	ret = regmap_update_bits(buck->regmap, CHG_USB_OVP_CFG_REG_2_REG, CHG_USB_OVP_SET_MSK,
		reg_val << CHG_USB_OVP_SET_SHIFT);
	if (ret != 0)
		scharger_err("%s set vusb ovp fail\n", __func__);

	return ret;
}

int scharger_v800_set_vpsw_ovp(enum charger_mode mode)
{
	int ret = 0;
	unsigned int reg_val;
	struct scharger_buck *buck = g_buck;
	unsigned int ovp_sel;

	if (buck == NULL) {
		scharger_err("%s scharger_buck is NULL!\n", __func__);
		return -EINVAL;
	}

	switch(mode) {
	case BUCK5V:
		reg_val = 0; /* set vpsw ovp 6v */
		ovp_sel = 6000;
		break;
	case BUCK9V:
		reg_val = 1; /* set vpsw ovp 11v */
		ovp_sel = 11000;
		break;
	case SC11:
		reg_val = 7; /* set vpsw ovp 6.5v */
		ovp_sel = 6500;
		break;
	case SC21:
		reg_val = 1; /* set vpsw ovp 11v */
		ovp_sel = 11000;
		break;
	case SC41:
		reg_val = 4; /* set vpsw ovp 20v */
		ovp_sel = 20000;
		break;
	case RSC11:
		reg_val = 0; /* set vpsw ovp 7.5v */
		ovp_sel = 7500;
		break;
	case RSC12:
		reg_val = 1; /* set vpsw ovp 11v */
		ovp_sel = 11000;
		break;
	case RSC14:
		reg_val = 4; /* set vpsw ovp 20v */
		ovp_sel = 20000;
		break;
	case OTG5V:
		reg_val = 0; /* set vpsw ovp 6v */
		ovp_sel = 6000;
		break;
	case OTG9V:
		reg_val = 1; /* set vusb ovp 11v */
		ovp_sel = 11000;
		break;
	default:
		scharger_err("%s mode not match!\n", __func__);
		return -1;
	}

	scharger_inf("%s mode:%d, psw ovp reg_val:%umv\n", __func__, mode, ovp_sel);
	ret = regmap_update_bits(buck->regmap, CHG_PSW_OVP_CFG_REG_2_REG, CHG_PSW_OVP_SET_MSK,
		reg_val << CHG_PSW_OVP_SET_SHIFT);
	if (ret != 0)
		scharger_err("%s set vpsw ovp fail\n", __func__);

	return ret;
}

int scharger_set_vusb_vpsw_ovp(enum charger_mode mode)
{
	int ret;

	if (charge_get_charger_type() == CHARGER_TYPE_WIRELESS)
		ret = scharger_v800_set_vpsw_ovp(mode);
	else
		ret = scharger_v800_set_vusb_ovp(mode);

	return ret;
}

int scharger_set_q1q1_ss_buck_mode(enum q1q1_ss_buck_mode mode)
{
	int ret = 0;
	struct scharger_buck *buck = g_buck;

	if (buck == NULL) {
		scharger_err("%s scharger_buck is NULL!\n", __func__);
		return -EINVAL;
	}

	ret += regmap_update_bits(buck->regmap, Q1Q1_SS_BUCK_REG, DA_SYS_RESVO2_MSK,
		mode << DA_SYS_RESVO2_SHIFT);

	scharger_inf("%s mode:%d\n", __func__, mode);

	return ret;
}

static int scharger_v800_set_s1q5_s1q6(enum charger_mode mode)
{
	int ret = 0;
	struct scharger_buck *buck = g_buck;

	if (buck == NULL) {
		scharger_err("%s scharger_buck is NULL!\n", __func__);
		return -EINVAL;
	}

	switch(mode) {
	case BUCK5V:
		/* close S1Q6 */
		ret += regmap_update_bits(buck->regmap, CHG_BOOST_CFG_REG_6_REG, CHG_S1Q6_EN_MSK,
			0 << CHG_S1Q6_EN_SHIFT);
		/* set S1Q6 driver to default gears */
		ret += regmap_update_bits(buck->regmap, CHG_SC_TOP_CFG_REG_7_REG, CHG_S1Q6_DRIVER_MSK,
			S1Q6_DRIVER_DEFAULT << CHG_S1Q6_DRIVER_SHIFT);
		/* open S1Q5S */
		ret += regmap_update_bits(buck->regmap, CHG_BOOST_CFG_REG_5_REG, CHG_S1Q5S_EN_MSK,
			1 << CHG_S1Q5S_EN_SHIFT);
		mdelay(2);
		/* close S1Q5S */
		ret += regmap_update_bits(buck->regmap, CHG_BOOST_CFG_REG_5_REG, CHG_S1Q5S_EN_MSK,
			0 << CHG_S1Q5S_EN_SHIFT);
		/* open S1Q5 */
		ret += regmap_update_bits(buck->regmap, CHG_BOOST_CFG_REG_6_REG, CHG_S1Q5_EN_MSK,
			0 << CHG_S1Q5_EN_SHIFT);
		scharger_inf("%s mode:%d,close S1Q6 and open S1Q5\n", __func__, mode);
		break;
	case BUCK9V:
		/* set S1Q6 driver to buck5/9V switch gears */
		ret += regmap_update_bits(buck->regmap, CHG_SC_TOP_CFG_REG_7_REG, CHG_S1Q6_DRIVER_MSK,
			S1Q6_DRIVER_WEAKEST << CHG_S1Q6_DRIVER_SHIFT);
		/* close S1Q5 */
		ret += regmap_update_bits(buck->regmap, CHG_BOOST_CFG_REG_6_REG, CHG_S1Q5_EN_MSK,
			1 << CHG_S1Q5_EN_SHIFT);
		/* open S1Q6 */
		ret += regmap_update_bits(buck->regmap, CHG_BOOST_CFG_REG_6_REG, CHG_S1Q6_EN_MSK,
			1 << CHG_S1Q6_EN_SHIFT);
		mdelay(25);
		scharger_inf("%s mode:%d,close S1Q5 and open S1Q6\n", __func__, mode);
		break;
	default:
		return -1;
	}

	return ret;
}

static int scharger_v800_set_vbus_vset(enum charger_mode mode)
{
	unsigned int data;
	struct scharger_buck *buck = g_buck;

	if (buck == NULL) {
		scharger_err("%s scharger_buck is NULL!\n", __func__);
		return -EINVAL;
	}

	switch(mode) {
	case BUCK5V:
		buck->buck_vbus_set = VBUS_VSET_5V;
		data = 0; /* 0: set vbus to 5V */
		break;
	case BUCK9V:
		buck->buck_vbus_set = VBUS_VSET_9V;
		data = 1; /* 1: set vbus to 9V */
		break;
	default:
		scharger_err("%s mode not match!\n", __func__);
		return -1;
	}

	scharger_err("%s, vbus_vset:%u\n", __func__, buck->buck_vbus_set);

	return regmap_update_bits(buck->regmap, CHG_VBUS_BKVSET_REG,
				 CHG_VBUS_VSET_MSK, data << CHG_VBUS_VSET_SHIFT);
}

int scharger_get_vusb_vpsw(void)
{
	int vol;
	if (charge_get_charger_type() == CHARGER_TYPE_WIRELESS)
		vol = scharger_v800_get_vpsw();
	else
		vol = scharger_v800_get_vusb();
	scharger_inf("%s, charger_type:%u, vol:%d\n", __func__, charge_get_charger_type(), vol);

	return vol;
}

/* can only be called for buck_init */
static int buck_init_5v(void)
{
	int ret;
	struct scharger_buck *buck = g_buck;

	if (buck == NULL) {
		scharger_err("%s scharger_buck is NULL!\n", __func__);
		return -EINVAL;
	}

	if (is_current_charge_mode(BUCK5V))
		return 0;

	ret = scharger_v800_set_vusb_ovp(BUCK5V);
	ret += scharger_v800_set_vpsw_ovp(BUCK5V);
	ret += scharger_v800_set_vbus_ovp(BUCK5V);
	scharger_v800_close_s1q4_s1q4s();
	scharger_v800_set_sc_mode(CHG_BUCK_MODE);

	scharger_buck_common_opt_param(buck);

	ret += scharger_v800_set_vbus_vset(BUCK5V);
	ret += scharger_v800_set_vbus_uvp(BUCK5V);
	ret += scharger_set_q1q1_ss_buck_mode(AUTO_MODE);
	ret += scharger_v800_set_buck_mode_enable(CHG_ENABLE);
	scharger_buck_otg_opt_param(BUCK5V);
	ret += scharger_v800_set_current_charge_mode(BUCK5V);
	return ret;
}

/* can only be called for buck_init */
static int buck_init_9v(void)
{
	int ret = 0;
	int vol;
	int vbat;
	int count = VBUS_CHECK_CNT;
	struct scharger_buck *buck = g_buck;

	if (buck == NULL) {
		scharger_err("%s scharger_buck is NULL!\n", __func__);
		return -EINVAL;
	}

	if (is_current_charge_mode(BUCK9V))
		return 0;

	vbat = scharger_v800_get_vbat();
	if (vbat < VBAT_3000MV) {
		scharger_err("%s:vbat %d is less than 3v, set vbus 9V fail, hold on buck 5v\n", __func__, vbat);
		return -1;
	}

	while(count--) {
		vol = scharger_get_vusb_vpsw();
		if(vol >= VBUS_8100MV) {
			break;
		}
		mdelay(40);
	}

	if (count == -1) {
		scharger_err("%s:vbus %d is less than 8v, set vbus 9V fail, hold on buck 5v\n", __func__, vol);
		return -1;
	}

	ret = scharger_v800_set_vusb_ovp(BUCK5V);
	ret += scharger_v800_set_vpsw_ovp(BUCK5V);
	ret += scharger_v800_set_buck_mode_enable(CHG_DISABLE);
	ret += scharger_set_q1q1_ss_buck_mode(FORCE_OPEN);
	ret += scharger_v800_set_vbus_vset(BUCK9V);
	scharger_buck_otg_opt_param(BUCK9V);
	ret += scharger_v800_set_current_charge_mode(BUCK9V);
	return ret;
}

/* Parameters: vbus_set voltage: 5V/9V */
int buck_init(u32 value)
{
	int ret;
	struct scharger_buck *buck = g_buck;

	if (buck == NULL) {
		scharger_err("%s scharger_buck is NULL!\n", __func__);
		return -EINVAL;
	}

	scharger_err("%s, set vbus:%u\n", __func__, value);

	scharger_v800_mode_lock();
	if (value < VBUS_VSET_9V) {
		ret = buck_init_5v();
		queue_delayed_work(system_power_efficient_wq, &buck->buck_init_5v_work,
			msecs_to_jiffies(BUCK_INIT_5V_WORK_FIRST_DELAY));
	} else if (value < VBUS_VSET_12V) {
		ret = buck_init_9v();
		queue_delayed_work(system_power_efficient_wq, &buck->buck_init_9v_work,
			msecs_to_jiffies(BUCK_INIT_9V_WORK_DELAY));
	} else {
		scharger_inf("%s:not support 12v buck\n", __func__);
		scharger_v800_mode_unlock();
		return -1;
	}
	scharger_v800_mode_unlock();
	return ret;
}

static int set_vbus_vset(u32 value)
{
	struct scharger_buck *buck = g_buck;

	if (buck == NULL) {
		scharger_err("%s scharger_buck is NULL!\n", __func__);
		return -EINVAL;
	}

	if (buck->dc_charging) {
		scharger_inf("%s %dV, direct charging pending, Buck operation failed!\n", __func__, value);
		return -EINVAL;
	}

	return buck_init(value);
}

static int get_vbus_vset(int *value)
{
	struct scharger_buck *buck = g_buck;

	if (!buck || !value)
		return -EINVAL;

	switch (buck->current_charge_mode.chg_mode) {
	case BUCK5V:
		*value = ADAPTER_5V;
		break;
	case BUCK9V:
		*value = ADAPTER_9V;
		break;
	default:
		return -EINVAL;
	}

	return 0;
}

void scharger_v800_set_anti_reverbst_reset(struct scharger_buck *buck)
{
	(void)regmap_update_bits(buck->regmap, CHG_BUCK_OTG_TOP_CFG_REG_1_REG,
				CHG_REVERBST_RST_MSK,
				1 << CHG_REVERBST_RST_SHIFT);
	queue_delayed_work(system_power_efficient_wq, &buck->reverbst_work,
			   msecs_to_jiffies(REVERBST_DELAY_ON));
}

int scharger_v800_set_buck_mode_enable(u32 enable)
{
	struct scharger_buck *buck = g_buck;

	if (buck == NULL) {
		scharger_err("%s scharger_buck is NULL!\n", __func__);
		return -EINVAL;
	}

	scharger_err("%s, en:%u\n", __func__, enable);

	if (enable == 1) {
		scharger_v800_set_anti_reverbst_reset(buck);
		buck->batt_ovp_cnt_30s = 0;
	}

	(void)regmap_update_bits(buck->regmap, CHG_SC_BUCK_ENB_REG,
				 CHG_HIZ_ENABLE_MSK, !enable << CHG_HIZ_ENABLE_SHIFT);

	return 0;
}

static int scharger_set_recharge_vol(struct scharger_buck *buck, unsigned int vrechg)
{
	return regmap_update_bits(buck->regmap, CHG_CHARGER_CFG_REG_13_REG,
				 CHG_VRECHG_MSK, vrechg << CHG_VRECHG_SHIFT);
}

static int scharger_v800_set_fast_safe_timer(struct scharger_buck *buck,
	 unsigned int safe_timer)
{
	return regmap_update_bits(buck->regmap, CHG_CHARGER_CFG_REG_4_REG,
				 CHG_FASTCHG_TIMER_MSK, safe_timer << CHG_FASTCHG_TIMER_SHIFT);
}

static int scharger_set_bat_comp(struct scharger_buck *buck, int value)
{
	unsigned int reg;
	int value_set = value;

	if (buck == NULL) {
		scharger_err("%s scharger_buck is NULL!\n", __func__);
		return -EINVAL;
	}
	scharger_inf("%s value_set %d\n", __func__, value_set);

	if (value < BAT_COMP_MOHM_MIN)
		value_set = BAT_COMP_MOHM_MIN;
	else if (value > BAT_COMP_MOHM_MAX)
		value_set = BAT_COMP_MOHM_MAX;

	reg = (unsigned int)(value_set / BAT_COMP_MOHM_STEP);

	return regmap_update_bits(buck->regmap, CHG_CHARGER_CFG_REG_5_REG, CHG_IR_SET_MSK, reg << CHG_IR_SET_SHIFT);
}

/*
 * Description: check whether VINDPM or IINDPML
 * return value: TRUE means VINDPM or IINDPM
 *               FALSE means NoT DPM
 */
int scharger_check_input_dpm_state(void)
{
	unsigned int reg = 0;
	int ret;
	struct scharger_buck *buck = g_buck;

	if (buck == NULL) {
		scharger_err("%s scharger_buck is NULL!\n", __func__);
		return -EINVAL;
	}

	ret = regmap_read(buck->regmap, CHG_CHARGER_RO_REG_0_REG, &reg);
	if (ret < 0) {
		scharger_err("%s err\n", __func__);
		return FALSE;
	}

	if ((reg & CHG_IN_DPM_STATE_MSK) == CHG_IN_DPM_STATE_MSK) {
		scharger_inf("CHG_STATUS0_REG:%x in vdpm state\n", reg);
		return TRUE;
	}

	return FALSE;
}

/* return value: TRUE means acl, FALSE means NoT acl */
static int scharger_check_input_acl_state(void)
{
	unsigned int reg = 0;
	int ret = 0;
	struct scharger_buck *buck = g_buck;

	if (buck == NULL) {
		scharger_err("%s scharger_buck is NULL!\n", __func__);
		return -EINVAL;
	}

	ret += regmap_update_bits(buck->regmap, CHG_CHARGER_CFG_REG_0_REG,
				 CHG_ACL_PRT_EN_MSK, ACL_RPT_EN << CHG_ACL_RPT_EN_SHIFT);
	ret += regmap_read(buck->regmap, CHG_CHARGER_RO_REG_0_REG, &reg);
	ret += regmap_update_bits(buck->regmap, CHG_CHARGER_CFG_REG_0_REG,
				 CHG_ACL_PRT_EN_MSK, ACL_RPT_EN << CHG_ACL_RPT_EN_SHIFT);
	if (ret) {
		scharger_err("%s err\n", __func__);
		return FALSE;
	}

	if ((reg & CHG_IN_ACL_STATE_MSK) == CHG_IN_ACL_STATE_MSK) {
		scharger_inf("CHG_STATUS0_REG:%x in acl state\n", reg);
		return TRUE;
	}

	return FALSE;
}

/* return value: TRUE means therm, FALSE means NoT therm */
static int scharger_check_therm_state(struct scharger_buck *buck)
{
	unsigned int reg = 0;
	int ret;

	if (buck == NULL) {
		scharger_err("%s scharger_buck is NULL!\n", __func__);
		return -EINVAL;
	}

	ret = regmap_read(buck->regmap, CHG_CHARGER_RO_REG_1_REG, &reg);
	if (ret < 0) {
		scharger_err("%s err\n", __func__);
		return FALSE;
	}

	if ((reg & CHG_IN_THERM_STATE_MSK) == CHG_IN_THERM_STATE_MSK)
		return TRUE;
	return FALSE;
}

int scharger_v800_set_term_enable(int enable)
{
	int vbatt_mv, term_mv, chg_state, dpm, acl, therm;
	bool is_vbat_term;
	struct scharger_buck *buck = g_buck;

	scharger_inf("%s enable %d\n", __func__, enable);

	if (buck == NULL) {
		scharger_err("%s scharger_buck is NULL!\n", __func__);
		return -EINVAL;
	}

	if (force_set_term_flag == CHG_STAT_ENABLE) {
		scharger_inf("Charger is in the production line test phase!\n");
		return 0;
	}

	if (enable != 0) {
		dpm = scharger_check_input_dpm_state();
		acl = scharger_check_input_acl_state();
		therm = scharger_check_therm_state(buck);
		term_mv = scharger_get_terminal_voltage();
		vbatt_mv = coul_drv_battery_voltage();
		/* ref vol 100 */
		is_vbat_term = (abs(term_mv - vbatt_mv) > 100) ? TRUE : FALSE;

		chg_state = (is_vbat_term || dpm == TRUE ||
						 acl == TRUE || therm == TRUE);
		scharger_inf("%s enable:%d, dpm:%d,acl:%d,therm:%d,term_mv:%d,"
						"is_vbat_term:%d,chg_state:%d\n",
						__func__, enable, dpm, acl, therm, term_mv,
						is_vbat_term, chg_state);
		if (chg_state != 0) {
			scharger_inf("%s enable:%d, chg_state:%d, but in dpm or acl or thermal state\n",
				     __func__, enable, chg_state);
			enable = 0;
		} else {
			scharger_inf("%s enable:%d\n", __func__, enable);
		}
	}

	return regmap_update_bits(buck->regmap, CHG_CHARGER_CFG_REG_2_REG,
				 CHG_EN_TERM_MSK, !!enable << CHG_EN_TERM_SHIFT);
}

static int scharger_v800_set_input_current(int cin_limit)
{
	unsigned int i;
	int max;
	unsigned int i_in_limit = 0;
	int cin_limit_set = cin_limit;
	struct scharger_buck *buck = g_buck;

	scharger_inf("%s cin_limit %d\n", __func__, cin_limit);

	if (buck == NULL) {
		scharger_err("%s scharger_buck is NULL!\n", __func__);
		return -EINVAL;
	}

	if (buck->buck_vbus_set < VBUS_VSET_9V)
		max = CHG_ILIMIT_2500;
	else if (buck->buck_vbus_set < VBUS_VSET_12V)
		max = CHG_ILIMIT_1400;
	else
		max = CHG_ILIMIT_1200;

	buck->input_current = cin_limit_set;

	if (cin_limit_set > max) {
		scharger_err("%s cin_limit %d, max %d, vbus set is %u\n",
			      __func__, cin_limit_set, max, buck->buck_vbus_set);
		cin_limit_set = max;
	}

	for (i = 0; i < CHG_LMT_NUM_V800; i++) {
		if (cin_limit_set >= chg_limit_v800[i])
			i_in_limit = i;
		else
			break;
	}
	scharger_inf("%s:cin_limit_set %d ma, reg is set 0x%x, buck_vbus_set %u\n", __func__,
		      cin_limit_set, i_in_limit, buck->buck_vbus_set);

	iin_set = cin_limit_set;
	(void)regmap_update_bits(buck->regmap, CHG_BUCK_ILIMIT_REG,
				CHG_ILIMIT_MSK, i_in_limit << CHG_ILIMIT_SHIFT);
	return 0;
}

static int scharger_v800_set_charge_current(int charge_current)
{
	unsigned int i_chg_limit;
	/* Chip limit */
	int min_curr = CHG_FAST_ICHG_100MA;
	int max_curr = CHG_FAST_ICHG_2500MA;
	struct scharger_buck *buck = g_buck;

	if (buck == NULL) {
		scharger_err("%s scharger_buck is NULL!\n", __func__);
		return -EINVAL;
	}
	scharger_inf("%s, charge current:%d\n", __func__, charge_current);

	if (charge_current > max_curr)
		charge_current = max_curr;
	else if (charge_current < min_curr)
		charge_current = min_curr;

	i_chg_limit = (unsigned int)((charge_current / CHG_FAST_ICHG_STEP_100) - 1);

	return regmap_update_bits(buck->regmap, CHG_CHARGER_CFG_REG_3_REG,
				 CHG_FAST_ICHG_MSK, i_chg_limit << CHG_FAST_ICHG_SHIFT);
}

static int __set_terminal_voltage(int charge_voltage)
{
	int ret = 0;
	unsigned int reg;
	unsigned int val_adj;
	struct scharger_buck *buck = g_buck;

	if (buck == NULL) {
		scharger_err("%s scharger_buck is NULL!\n", __func__);
		return -EINVAL;
	}

	scharger_inf("%s set terminal voltage:%d\n", __func__, charge_voltage);

	buck->term_vol_mv = (unsigned int)charge_voltage;
	if (charge_voltage < CHG_FAST_VCHG_MIN)
		return regmap_update_bits(buck->regmap, CHG_CHARGER_CFG_REG_11_REG,
					 CHG_FAST_VCHG_4100MV_MSK, 1 << CHG_FAST_VCHG_4100MV_SHIFT);
	else if (charge_voltage > CHG_FAST_VCHG_MAX)
		buck->term_vol_mv = CHG_FAST_VCHG_MAX;

	regmap_update_bits(buck->regmap, CHG_CHARGER_CFG_REG_11_REG,
				 CHG_FAST_VCHG_4100MV_MSK, 0 << CHG_FAST_VCHG_4100MV_SHIFT);

	/* transfer from code value to actual value */
	reg = (buck->term_vol_mv - CHG_FAST_VCHG_MIN) * UV_PER_MV / CHG_FAST_VCHG_STEP_8000UV;
	ret += regmap_update_bits(buck->regmap, CHG_CHARGER_CFG_REG_11_REG,
				 CHG_FAST_VCHG_MSK, reg << CHG_FAST_VCHG_SHIFT);

	/* cv adjust */
	val_adj = (buck->term_vol_mv - CHG_FAST_VCHG_MIN) % (CHG_FAST_VCHG_STEP_8000UV / UV_PER_MV);
	scharger_inf("%s reg:%u, val_adj:%u\n", __func__, reg, val_adj);
	ret += regmap_update_bits(buck->regmap, CHG_CHARGER_CFG_REG_2_REG,
				CHG_CV_ADJ_MSK, val_adj << CHG_CV_ADJ_SHIFT);

	return ret;
}

static int scharger_v800_set_terminal_voltage(int charge_voltage)
{
	int ret = 0;

	scharger_inf("%s charge_voltage:%d\n", __func__, charge_voltage);

	if (g_cv_info->cv_ori_val != charge_voltage) {
		g_cv_info->cv_trim_flag = 0;
		scharger_err("%s The vterm:%d has changed, reset vterm:%d\n", __func__,
				 g_cv_info->cv_ori_val, charge_voltage);
	}
	if (g_cv_info->cv_trim_flag == 1) {
		charge_voltage = g_cv_info->cv_new_set_val;
		scharger_err("%s The vterm:%d has already been trimmed, use the trimmed value:%d\n",
				 __func__, g_cv_info->cv_ori_val, g_cv_info->cv_new_set_val);
	}

	ret = __set_terminal_voltage(charge_voltage);

	return ret;
}

/*
 * set the terminal current in charging process
 * (min value is 400ma for scharger ic bug)
 */
static int scharger_v800_set_terminal_current(int term_current)
{
	unsigned int i_term;
	struct scharger_buck *buck = g_buck;

	if (buck == NULL) {
		scharger_err("%s scharger_buck is NULL!\n", __func__);
		return -EINVAL;
	}

	if (term_current < CHG_TERM_ICHG_400MA)
		i_term = 0; /* 0: 200mA */
	else if (term_current < CHG_TERM_ICHG_800MA)
		i_term = 1; /* 1: 400mA */
	else if (term_current < CHG_TERM_ICHG_1200MA)
		i_term = 2; /* 2: 800mA */
	else
		i_term = 3; /* 3: 1200mA */

	/* for chip bug */
	i_term = 2; /* 2: default 800mA */

	return regmap_update_bits(buck->regmap, CHG_CHARGER_CFG_REG_12_REG,
				 CHG_TERM_ICHG_MSK, i_term << CHG_TERM_ICHG_SHIFT);
}

static int scharger_set_batfet_ctrl(struct scharger_buck *buck, u32 status, int bat)
{
	int ret = 0;

	if (bat == BATTERY_H) {
		ret = regmap_update_bits(buck->regmap, CHG_CHARGER_BATFET_H_CTRL_REG,
			 CHG_CHARGER_BATFET_H_CTRL_MSK, status << CHG_CHARGER_BATFET_H_CTRL_SHIFT);
	} else if (bat == BATTERY_L) {
		ret = regmap_update_bits(buck->regmap, CHG_CHARGER_BATFET_L_CTRL_REG,
			 CHG_CHARGER_BATFET_L_CTRL_MSK, status << CHG_CHARGER_BATFET_L_CTRL_SHIFT);
	} else {
		scharger_err("%s bat is invalid!\n", __func__);
		ret = -1;
	}

	return ret;
}

int scharger_v800_gate1_enable(bool enable)
{
	struct scharger_buck *buck = g_buck;
	int ret = 0;

	if (!buck) {
		scharger_err("%s scharger_buck is NULL!\n", __func__);
		return -EINVAL;
	}
	scharger_inf("%s enable:%u\n", __func__, enable);

	if (enable) {
		ret += regmap_update_bits(buck->regmap, CHG_USB_OVP_CFG_REG_1_REG,
			CHG_GATE_VDROPOVP_SET_MSK, 1 << CHG_GATE_VDROPOVP_SET_SHIFT);
		ret += regmap_update_bits(buck->regmap, CHG_USB_OVP_CFG_REG_3_REG,
			CHG_OVP_GATE1_VDROPOVP_EN_MSK, 1 << CHG_OVP_GATE1_VDROPOVP_EN_SHIFT);
		ret += regmap_update_bits(buck->regmap, CHG_USB_OVP_CFG_REG_3_REG,
			CHG_GATE1_EN_MSK, 1 << CHG_GATE1_EN_SHIFT);
	} else {
		ret += regmap_update_bits(buck->regmap, CHG_USB_OVP_CFG_REG_3_REG,
			CHG_GATE1_EN_MSK, 0 << CHG_GATE1_EN_SHIFT);
	}

	return ret;
}

/*
 * Parameters: value:vclamp mv
 * return value:  0-success or others-fail
 */
static int scharger_set_vclamp(struct scharger_buck *buck, int value)
{
	unsigned int reg;

	if (value < CHG_IR_VCLAMP_MIN)
		value = CHG_IR_VCLAMP_MIN;
	else if (value > CHG_IR_VCLAMP_MAX)
		value = CHG_IR_VCLAMP_MAX;

	reg = (unsigned int)(value / CHG_IR_VCLAMP_STEP);

	return regmap_update_bits(buck->regmap, CHG_CHARGER_CFG_REG_13_REG, CHG_CHG_VCLAMP_MSK,
		 reg << CHG_CHG_VCLAMP_SHIFT);
}

static int scharger_otg_switch_mode(int enable)
{
	int ret = 0;
	struct scharger_buck *buck = g_buck;

	if (buck == NULL) {
		scharger_err("%s scharger_buck is NULL!\n", __func__);
		return -EINVAL;
	}
	scharger_inf("%s enable %d\n", __func__, enable);

	ret += regmap_update_bits(buck->regmap, CHG_BUCK_OTG_TOP_CFG_REG_38_REG,
				 CHG_OTG_SWITCH_MASK, !!enable << CHG_OTG_SWITCH_SHIFT);

	return ret;
}

static int scharger_set_otg_current(int value)
{
	unsigned int reg;
	unsigned int temp_current_ma;
	struct scharger_buck *buck = g_buck;

	if (buck == NULL) {
		scharger_err("%s scharger_buck is NULL!\n", __func__);
		return -EINVAL;
	}

	temp_current_ma = (unsigned int)value;

	if (temp_current_ma < BOOST_LIM_MIN || temp_current_ma > BOOST_LIM_MAX)
		scharger_inf("set otg current %dmA is out of range!\n", value);
	if (temp_current_ma < BOOST_LIM_1000)
		reg = 0; /* 0: set otg current 500mA */
	else if (temp_current_ma < BOOST_LIM_1500)
		reg = 1; /* 1: set otg current 1000mA */
	else if (temp_current_ma < BOOST_LIM_2000)
		reg = 2; /* 2: set otg current 1500mA */
	else
		reg = 3; /* 3: set otg current 2000mA */

	scharger_inf("otg current reg is set 0x%x\n", reg);
	return regmap_update_bits(buck->regmap, CHG_BUCK_OTG_TOP_CFG_REG_32_REG, CHG_OTG_LIM_MSK,
				 reg << CHG_OTG_LIM_SHIFT);
}

int scharger_v800_set_sc_mode(enum scharger_v800_sc_mode mode)
{
	struct scharger_buck *buck = g_buck;

	if (buck == NULL) {
		scharger_err("%s scharger_buck is NULL!\n", __func__);
		return -EINVAL;
	}

	scharger_inf("%s sc_mode:%d\n", __func__, mode);
	return regmap_update_bits(buck->regmap, CHG_DA_SC_MODE_REG, CHG_DA_SC_MODE_MSK,
			 mode << CHG_DA_SC_MODE_SHIFT);
}

int scharger_v800_set_usb_ovp_mode(enum scharger_v800_usb_ovp_mode mode)
{
	struct scharger_buck *buck = g_buck;

	if (buck == NULL) {
		scharger_err("%s scharger_buck is NULL!\n", __func__);
		return -EINVAL;
	}
	scharger_inf("%s mode:%d\n", __func__, mode);

	if (mode == USBOVP_FORCE_OPEN)
		scharger_v800_set_watchdog_timer(WATCHDOG_TIMER_40_S);

	return regmap_update_bits(buck->regmap, CHG_USBOVP_CTRL_REG,
				 CHG_USBOVP_CTRL_MSK, mode << CHG_USBOVP_CTRL_SHIFT);
}

unsigned int scharger_v800_get_usb_ovp_mode(void)
{
	struct scharger_buck *buck = g_buck;
	unsigned int usbovp_mode;

	if (buck == NULL) {
		scharger_err("%s scharger_buck is NULL!\n", __func__);
		return -EINVAL;
	}

	regmap_read(buck->regmap, CHG_USBOVP_CTRL_REG, &usbovp_mode);

	return usbovp_mode;
}

int scharger_v800_set_psw_ovp_mode(enum scharger_v800_psw_ovp_mode mode)
{
	struct scharger_buck *buck = g_buck;

	if (buck == NULL) {
		scharger_err("%s scharger_buck is NULL!\n", __func__);
		return -EINVAL;
	}
	scharger_inf("%s mode:%d\n", __func__, mode);
	return regmap_update_bits(buck->regmap, CHG_PSWOVP_CTRL_REG,
				 CHG_PSWOVP_CTRL_MSK, mode << CHG_PSWOVP_CTRL_SHIFT);
}

static int scharger_enable_bat_gd_for_udp_board(void)
{
	struct scharger_buck *buck = g_buck;

	if (buck == NULL) {
		scharger_err("%s scharger_buck is NULL!\n", __func__);
		return -EINVAL;
	}

	if (buck->is_board_type != BAT_BOARD_UDP)
		return EOK;

	scharger_inf("%s boardtype:%d\n", __func__, buck->is_board_type);

	/* Enable bat gd shield for udp board */
	return regmap_update_bits(buck->regmap, CHG_SYS_DET_CFG_REG_0_REG,
		 CHG_BAT_GD_SHIELD_MSK, FORCE_BAT_GD_HIGH << CHG_BAT_GD_SHIELD_SHIFT);
}

unsigned int scharger_v800_get_psw_ovp_mode(void)
{
	struct scharger_buck *buck = g_buck;
	unsigned int usbpsw_mode;

	if (buck == NULL) {
		scharger_err("%s scharger_buck is NULL!\n", __func__);
		return -EINVAL;
	}

	regmap_read(buck->regmap, CHG_PSWOVP_CTRL_REG, &usbpsw_mode);

	return usbpsw_mode;
}

static int set_5v_otg_enable(int enable)
{
	int ret = 0;
	int en;
	unsigned int usb_ovp_mode;
	unsigned int psw_ovp_mode;

	en = !!enable;

	if (en) {
		scharger_v800_mode_lock();

		if (is_current_charge_mode(OTG5V)) {
			scharger_v800_mode_unlock();
			return 0;
		}
		if (is_current_charge_mode(OTG9V)) {
			scharger_v800_mode_unlock();
			ret += set_9v_otg_enable(0);
			scharger_v800_mode_lock();
		}

		scharger_buck_otg_opt_param(OTG5V);
		(void)scharger_enable_bat_gd_for_udp_board();

		if (wireless_tx_get_tx_open_flag()) {
			ret += scharger_v800_set_psw_ovp_mode(PSWOVP_REVERSE_MODE);
			ret += scharger_v800_set_vpsw_ovp(OTG5V);
		} else {
			ret += scharger_v800_set_usb_ovp_mode(USBOVP_REVERSE_MODE);
			ret += scharger_v800_set_vusb_ovp(OTG5V);
		}

		/* enable OTG */
		ret += scharger_v800_set_otg_enable(en);
		ret += scharger_v800_set_current_charge_mode(OTG5V);

		scharger_v800_mode_unlock();
	} else {
		/* disable OTG */
		ret += scharger_v800_set_otg_enable(en);

		psw_ovp_mode = scharger_v800_get_psw_ovp_mode();
		if (psw_ovp_mode == PSWOVP_REVERSE_MODE) {
			ret += scharger_v800_set_psw_ovp_mode(PSWOVP_FORWARD_MODE);
			ret += scharger_v800_set_vpsw_ovp(BUCK5V);
		} else {
			usb_ovp_mode = scharger_v800_get_usb_ovp_mode();
			if (usb_ovp_mode == USBOVP_REVERSE_MODE) {
				ret += scharger_v800_set_usb_ovp_mode(USBOVP_FORWARD_MODE);
				ret += scharger_v800_set_vusb_ovp(BUCK5V);
			}
		}
		scharger_buck_otg_exit_opt_param(OTG5V);
		mdelay(35);
		ret += buck_init(VBUS_VSET_5V);
	}

	return ret;
}

static int set_9v_otg_enable(int enable)
{
	int ret = 0;
	int en;
	unsigned int usb_ovp_mode;
	unsigned int psw_ovp_mode;

	en = !!enable;
	if (en) {
		scharger_v800_mode_lock();

		if (is_current_charge_mode(OTG9V)) {
			scharger_v800_mode_unlock();
			return 0;
		}
		if (is_current_charge_mode(OTG5V)) {
			scharger_v800_mode_unlock();
			ret += set_5v_otg_enable(0);
			scharger_v800_mode_lock();
		}

		scharger_buck_otg_opt_param(OTG9V);

		if (wireless_tx_get_tx_open_flag()) {
			ret += scharger_v800_set_psw_ovp_mode(PSWOVP_REVERSE_MODE);
			ret += scharger_v800_set_vpsw_ovp(OTG9V);
		} else {
			ret += scharger_v800_set_usb_ovp_mode(USBOVP_REVERSE_MODE);
			ret += scharger_v800_set_vusb_ovp(OTG9V);
		}

		/* enable OTG */
		ret += scharger_v800_set_otg_enable(en);
		ret += scharger_v800_set_current_charge_mode(OTG9V);

		scharger_v800_mode_unlock();
	} else {
		/* disable OTG */
		ret += scharger_v800_set_otg_enable(en);

		psw_ovp_mode = scharger_v800_get_psw_ovp_mode();
		if (psw_ovp_mode == PSWOVP_REVERSE_MODE) {
			ret += scharger_v800_set_psw_ovp_mode(PSWOVP_FORWARD_MODE);
			ret += scharger_v800_set_vpsw_ovp(BUCK5V);
		} else {
			usb_ovp_mode = scharger_v800_get_usb_ovp_mode();
			if (usb_ovp_mode == USBOVP_REVERSE_MODE) {
				ret += scharger_v800_set_usb_ovp_mode(USBOVP_FORWARD_MODE);
				ret += scharger_v800_set_vusb_ovp(BUCK5V);
			}
		}
		scharger_buck_otg_exit_opt_param(OTG9V);
		mdelay(35);
		ret += buck_init(VBUS_VSET_5V);
	}

	return ret;
}

static int set_otg_disable(void)
{
	struct current_charger_mode *charger_mode = NULL;
	int ret;

	charger_mode = scharger_v800_get_current_charge_mode();
	if(charger_mode == NULL) {
		scharger_err("%s charger_mode is NULL!\n", __func__);
		return -1;
	}

	if (charger_mode->chg_mode == OTG9V)
		ret = set_9v_otg_enable(0);
	else
		ret = set_5v_otg_enable(0);

	return ret;
}

static int scharger_set_otg_mode(int mode)
{
	int ret = 0;
	int vusb, vpsw;
	int cnt = OTG_PORT_VOL_DETECT_CNT;

	scharger_inf("%s mode=%d\n", __func__, mode);

	/* It is forbidden to open otg when vusb or vpsw is greater than 5500mv, decet 3 * 50 ms */
	while (mode && cnt--) {
		vusb = scharger_v800_get_vusb();
		vpsw = scharger_v800_get_vpsw();
		if (vpsw < 5500 && vusb < 5500)
			break;
		mdelay(50);
	}
	if (cnt == 0) {
		scharger_inf("%s fail. vpsw:%d, vusb:%d, vol is greater than 5500mv\n", __func__, vpsw, vusb);
		return -1;
	}

	if (mode == OTG_5V)
		ret = set_5v_otg_enable(1);
	else if (mode == OTG_9V)
		ret = set_9v_otg_enable(1);
	else
		ret = set_otg_disable();

	return ret;
}

int scharger_v800_dc_switch_to_buck_mode(void)
{
	int ret = 0;
	struct scharger_buck *buck = g_buck;

	if (buck == NULL) {
		scharger_err("%s scharger_buck is NULL!\n", __func__);
		return -EINVAL;
	}

	scharger_inf("%s\n", __func__);

	/* usb plugin enable */
	ret += regmap_update_bits(buck->regmap, CHG_USB_OVP_CFG_REG_2_REG, CHG_USB_PLUGIN_EN_MSK,
			USB_PLUGIN_EN << CHG_USB_PLUGIN_EN_SHIFT);
	ret += scharger_v800_set_charge_enable(CHG_DISABLE);
	ret += scharger_v800_set_fast_safe_timer(buck, CHG_FASTCHG_TIMER_20H);
	ret += scharger_v800_set_term_enable(CHG_TERM_DIS);
	ret += scharger_v800_set_input_current(CHG_ILIMIT_475);
	ret += scharger_v800_set_charge_current(CHG_FAST_ICHG_500MA);
	ret += scharger_v800_set_terminal_voltage(buck->param_dts.default_vterm);
	ret += scharger_v800_set_terminal_current(CHG_TERM_ICHG_200MA);
	ret += scharger_v800_set_watchdog_timer(WATCHDOG_TIMER_40_S);
	mdelay(35);
	ret += buck_init(VBUS_VSET_5V);
	if (ret != 0)
		scharger_err("%s err!\n", __func__);

	return 0;
}

static int scharger_buck_default_param(void)
{
	int ret = 0;
	struct scharger_buck *buck = g_buck;

	if (buck == NULL) {
		scharger_err("%s buck is NULL!\n", __func__);
		return -EINVAL;
	}

	ret += scharger_v800_set_charge_enable(CHG_DISABLE);
	ret += scharger_set_recharge_vol(buck, CHG_RECHG_150);
	ret += scharger_v800_set_fast_safe_timer(buck, CHG_FASTCHG_TIMER_20H);
	ret += scharger_v800_set_term_enable(CHG_TERM_DIS);
	ret += scharger_v800_set_input_current(CHG_ILIMIT_475);
	ret += scharger_v800_set_charge_current(CHG_FAST_ICHG_500MA);
	ret += scharger_v800_set_terminal_voltage(buck->param_dts.default_vterm);
	ret += scharger_v800_set_terminal_current(CHG_TERM_ICHG_200MA);
	ret += scharger_v800_set_watchdog_timer(WATCHDOG_TIMER_40_S);
	ret += scharger_set_batfet_ctrl(buck, CHG_BATFET_EN, BATTERY_L);
	/* IR compensation voatge clamp,IR compensation resistor setting */
	ret += scharger_set_bat_comp(buck, buck->param_dts.bat_comp);
	ret += scharger_set_vclamp(buck, buck->param_dts.vclamp);
	ret += scharger_set_otg_current(BOOST_LIM_1000);
	if (ret)
		return -1;

	return 0;
}

static int scharger_chip_init(struct charge_init_data *init_crit)
{
	int ret = 0;
	struct scharger_buck *buck = g_buck;

	if (init_crit == NULL || buck == NULL) {
		scharger_err("%s buck or charge_init_data is NULL!\n", __func__);
		return -EINVAL;
	}

	scharger_inf("%s, vbus %d\n", __func__, init_crit->vbus);

	if (buck->dc_charging) {
		scharger_inf("%s %dV, direct charging pending, Buck operation failed!\n", __func__, init_crit->vbus);
		return -EINVAL;
	}

	switch (init_crit->vbus) {
	case ADAPTER_5V:
		ret += ibat_res_sel(buck, buck->param_dts.r_charger_uohm);
		ret += buck_init(VBUS_VSET_5V);
		break;
	case ADAPTER_9V:
		ret += buck_init(VBUS_VSET_9V);
		break;
	default:
		scharger_err("%s: init mode err\n", __func__);
		return -EINVAL;
	}

	ret += scharger_buck_default_param();

	return ret;
}

/* 0-success or others-fail */
static int scharger_device_check(void)
{
	int ret;
	struct scharger_buck *buck = g_buck;

	if (buck == NULL) {
		scharger_err("%s scharger_buck is NULL!\n", __func__);
		return -EINVAL;
	}

	/* byte length of version_0 chip id is 4 */
	ret = regmap_bulk_read(buck->regmap, CHG_VERSION0_RO_REG_0_REG, &buck->chip_id, 4);
	if (ret != 0) {
		scharger_err("[%s]:read chip_id fail\n", __func__);
		return CHARGE_IC_BAD;
	}
	/* byte length of version_4 charger version is 2 */
	ret = regmap_bulk_read(buck->regmap, CHG_VERSION4_RO_REG_0_REG, &buck->scharger_version, 2);
	if (ret != 0) {
		scharger_err("[%s]:read scharger_version fail\n", __func__);
		return CHARGE_IC_BAD;
	}
	scharger_inf("%s, chip id:0x%x, version:0x%x\n",
		 __func__, buck->chip_id, buck->scharger_version);

	return CHARGE_IC_GOOD;
}

unsigned int scharger_v800_device_version(void)
{
	struct scharger_buck *buck = g_buck;

	if (buck == NULL) {
		scharger_err("%s scharger_buck is NULL!\n", __func__);
		return -EINVAL;
	}

	return buck->scharger_version;
}

static int scharger_get_charge_current(void)
{
	int ret;
	unsigned int reg = 0;
	int charge_current;
	struct scharger_buck *buck = g_buck;

	if (buck == NULL) {
		scharger_err("%s scharger_buck is NULL!\n", __func__);
		return -EINVAL;
	}

	ret = regmap_read(buck->regmap, CHG_CHARGER_CFG_REG_3_REG, &reg);
	if (ret != 0) {
		scharger_err("read mask fail\n");
		return CHG_FAST_ICHG_00MA;
	}

	charge_current = (int)((reg + 1) * CHG_FAST_ICHG_STEP_100);

	return charge_current;
}

/*
 * Parameters:   enable:terminal enable or not
 *               0&1:dbc control. 2:original charger procedure
 * return value:  0-success or others-fail
 */
static int scharger_force_set_term_enable(int enable)
{
	struct scharger_buck *buck = g_buck;

	if (buck == NULL) {
		scharger_err("%s scharger_buck is NULL!\n", __func__);
		return -EINVAL;
	}

	if ((enable == 0) || (enable == 1)) {
		force_set_term_flag = CHG_STAT_ENABLE;
	} else {
		force_set_term_flag = CHG_STAT_DISABLE;
		return 0;
	}
	return regmap_update_bits(buck->regmap, CHG_CHARGER_CFG_REG_2_REG,
				 CHG_EN_TERM_MSK, !!enable << CHG_EN_TERM_SHIFT);
}

static int scharger_get_charge_state(unsigned int *state)
{
	unsigned int reg0 = 0;
	unsigned int reg1 = 0;
	unsigned int reg2 = 0;
	int ret = 0;
	struct scharger_buck *buck = g_buck;

	if (buck == NULL || state == NULL) {
		scharger_err("%s scharger_buck is NULL!\n", __func__);
		return -EINVAL;
	}

	*state = 0;
	ret += regmap_read(buck->regmap, CHG_BUCK_OTG_TOP_RO_REG_45_REG, &reg0);
	ret += regmap_read(buck->regmap, CHG_CHARGER_RO_REG_0_REG, &reg1);
	ret += regmap_read(buck->regmap, CHG_WDT_TIMEOUT_REG, &reg2);
	if (ret != 0) {
		scharger_err("[%s]read charge status reg fail\n", __func__);
		return -1;
	}

	if ((reg0 & CHG_BUCK_OK) != CHG_BUCK_OK)
		*state |= CHG_STATE_NOT_PG;
	if (((reg1 & CHG_STAT_CHARGE_DONE) == CHG_STAT_CHARGE_DONE))
		*state |= CHG_STATE_CHRG_DONE;
	if ((reg2 & WATCHDOG_OK) != WATCHDOG_OK)
		*state |= CHG_STATE_WDT_FAULT;

	scharger_inf("%s >>> reg0:0x%x, reg1 0x%x, reg2 0x%x, state 0x%x\n",
		     __func__, reg0, reg1, reg2, *state);
	return 0;
}

/*
 * return value: charger state:
 *               1:Charge Termination Done
 *               0:Not Charging and buck is closed;Pre-charge;Fast-charg;
 *              -1:error
 */
static int scharger_get_charger_state(void)
{
	int ret;
	unsigned int data = 0;
	int state = 0;
	struct scharger_buck *buck = g_buck;

	if (buck == NULL) {
		scharger_err("%s scharger_buck is NULL!\n", __func__);
		return -EINVAL;
	}

	ret = regmap_read(buck->regmap, CHG_CHARGER_RO_REG_0_REG, &data);
	if (ret) {
		scharger_err("[%s]:read charge status reg fail\n", __func__);
		return -1;
	}

	if ((data & CHG_STAT_CHARGE_DONE) == CHG_STAT_CHARGE_DONE)
		state = 1;

	return state;
}

static int scharger_set_batfet_disable(int disable)
{
	int ret = 0;
	struct scharger_buck *buck = g_buck;

	if (buck == NULL) {
		scharger_err("%s scharger_buck is NULL!\n", __func__);
		return -EINVAL;
	}
	scharger_inf("%s disable %d\n", __func__, disable);

	ret += scharger_set_batfet_ctrl(buck, !disable, BATTERY_L);

	return ret;
}

/* set the charger hiz close watchdog */
static int scharger_set_charger_hiz(int enable)
{
	scharger_inf("%s enable %d\n", __func__, enable);
	return scharger_v800_set_buck_mode_enable(!enable);
}

static int dummy_fun_1(void)
{
	return 0;
}

static int dummy_fun_2(int val)
{
	return 0;
}

static int scharger_soft_vbatt_ovp_protect(void)
{
	struct scharger_buck *buck = g_buck;
	int vbatt_mv, vbatt_max;

	if (buck == NULL) {
		scharger_err("%s scharger_buck is NULL!\n", __func__);
		return -EINVAL;
	}
	vbatt_mv = coul_drv_battery_voltage();
	vbatt_max = coul_drv_battery_vbat_max();
	if (vbatt_mv >= min(CHG_VBATT_SOFT_OVP_MAX, chg_vbatt_cv_103(vbatt_max))) {
		buck->batt_ovp_cnt_30s++;
		if (buck->batt_ovp_cnt_30s == CHG_VBATT_SOFT_OVP_CNT) {
			scharger_set_charger_hiz(1);
			scharger_err("%s:vbat:%d,vbat_max:%d,cv_mv:%u,ovp_cnt:%u,shutdown buck\n",
				     __func__, vbatt_mv, vbatt_max, buck->term_vol_mv, buck->batt_ovp_cnt_30s);
			buck->batt_ovp_cnt_30s = 0;
		}
	} else {
		buck->batt_ovp_cnt_30s = 0;
	}
	return 0;
}

void scharger_v800_set_boot_weaksource_flag(void)
{
	unsigned int reg_val;

	reg_val = pmic_read_reg(WEAKSOURCE_FLAG_REG);
	reg_val |= WAEKSOURCE_FLAG;
	(void)pmic_write_reg(WEAKSOURCE_FLAG_REG, reg_val);
}

/*
 * limit buck current to 470ma according to rboost count
 * Return: 0: do nothing; 1:limit buck current 470ma
 */
static int scharger_rboost_buck_limit(void)
{
	struct scharger_buck *buck = g_buck;

	if (buck == NULL) {
		scharger_err("%s scharger_buck is NULL!\n", __func__);
		return -EINVAL;
	}

	if (buck->reverbst_cnt > ILIMIT_RBOOST_CNT) {
		scharger_v800_set_boot_weaksource_flag();
		scharger_inf("%s:rboost cnt:%d\n", __func__, buck->reverbst_cnt);
		return 1;
	}
	buck->reverbst_cnt = 0;
	return 0;
}

static int scharger_get_input_current_set(void)
{
	return iin_set;
}

static int scharger_set_auto_shipmode_thre(struct scharger_buck *buck, unsigned int thre)
{
	unsigned int bat_eco_val;

	if (thre < BAT_ECO_2600MV)
		bat_eco_val = 4; /* 4: set bat eco gear 2500mV */
	else if (thre < BAT_ECO_2750MV)
		bat_eco_val = 5; /* 5: set bat eco gear 2600mV */
	else if (thre < BAT_ECO_2900MV)
		bat_eco_val = 6; /* 6: set bat eco gear 2750mV */
	else if (thre < BAT_ECO_3000MV)
		bat_eco_val = 7; /* 7: set bat eco gear 2900mV */
	else if (thre < BAT_ECO_3100MV)
		bat_eco_val = 0; /* 0: set bat eco gear 3000mV */
	else if (thre < BAT_ECO_3200MV)
		bat_eco_val = 1; /* 1: set bat eco gear 3100mV */
	else if (thre < BAT_ECO_3300MV)
		bat_eco_val = 2; /* 2: set bat eco gear 3200mV */
	else
		bat_eco_val = 3; /* 3: set bat eco gear 3300mV */

	return regmap_update_bits(buck->regmap, CHG_SYS_DET_CFG_REG_4_REG,
			 CHG_BAT_ECO_SET_MSK, bat_eco_val << CHG_BAT_ECO_SET_SHIFT);
}

static int scharger_set_ship_mode_enable(bool enable)
{
	int ret = 0;
	struct scharger_buck *buck = g_buck;

	if (buck == NULL) {
		scharger_err("%s scharger_buck is NULL!\n", __func__);
		return -EINVAL;
	}

	if (buck->param_dts.auto_shipmode_en == 1) {
		ret += scharger_set_auto_shipmode_thre(buck, buck->param_dts.auto_shipmode_thre);
		ret += regmap_update_bits(buck->regmap, CHG_AUTO_SHIP_MODE_REG, CHG_AUTO_SHIP_MODE_MSK,
			 1 << CHG_AUTO_SHIP_MODE_SHIFT);
	} else {
		ret += regmap_write(buck->regmap, CHG_DEBUG_WRITE_PRO_REG, WRITE_PRO_DISABLE);
		/* 0: set ship mode delay 16s */
		ret += regmap_update_bits(buck->regmap, CHG_COUL_TOP_CFG_REG_8_REG,
			 CHG_SHIP_MODE_DELAY_MSK, 0 << CHG_SHIP_MODE_DELAY_SHIFT);
		ret += regmap_write(buck->regmap, CHG_DEBUG_WRITE_PRO_REG, WRITE_PRO_ENABLE);
	}
	ret += regmap_update_bits(buck->regmap, CHG_SHIP_MODE_ENB_REG,
		 CHG_SHIP_MODE_ENB_MSK, !enable << CHG_SHIP_MODE_ENB_SHIFT);
	if (ret != 0)
		scharger_err("[%s]:set ship mode %d fail\n", __func__, enable);

	return ret;
}

static int scharger_set_low_power_mode_enable(bool enable)
{
	return 0;
}

/*
 * V800 shutdown the buck ovp when some fault occurs.
 * In some scenarios, we use vusb replase vbus.
 */
static bool scharger_need_vusb_replace_vbus(void)
{
	return true;
}

static const char *scharger_get_dev_name(void)
{
	return CHARGE_DEV_SCHARGERV800;
}

static void reverbst_delay_work(struct work_struct *work)
{
	struct scharger_buck *buck = g_buck;

	if (buck == NULL) {
		scharger_err("%s scharger_buck is NULL!\n", __func__);
		return;
	}

	(void)regmap_update_bits(buck->regmap, CHG_BUCK_OTG_TOP_CFG_REG_1_REG,
				CHG_REVERBST_RST_MSK,
				0 << CHG_REVERBST_RST_SHIFT);
}

static int scharger_adc_get_vbus(unsigned int *val)
{
	if (val == NULL) {
		scharger_err("%s: input val is NULL\n", __func__);
		return -EINVAL;
	}
	*val = (unsigned int)scharger_v800_get_vbus();

	return 0;
}

static int scharger_adc_get_vusb(int *val)
{
	if (val == NULL) {
		scharger_err("%s: input val is NULL\n", __func__);
		return -EINVAL;
	}
	*val = scharger_v800_get_vusb();

	return 0;
}

static int scharger_stop_charge_config(void)
{
	struct scharger_buck *buck = g_buck;

	if (buck == NULL) {
		scharger_err("%s scharger_buck is NULL!\n", __func__);
		return -EINVAL;
	}

	buck->reverbst_cnt = 0;

	return 0;
}

static int scharger_efuse_read_enable(struct scharger_buck *buck, u32 enable)
{
	int ret = 0;
	u32 en;

	en = !!enable;
	ret = regmap_update_bits(buck->regmap, EFUSE_TESTBUS_CFG_REG, EFUSE_TESTBUS_EN_MSK,
					en << EFUSE_TESTBUS_EN_SHIFT);
	if (ret) {
		scharger_err("%s enable efuse testbus fail\n", __func__);
		return -1;
	}

	return ret;
}
static int scharger_v800_ldo33_enable(struct scharger_buck *buck, unsigned int enable)
{
	int ret;

	scharger_inf("%s enable=%u\n", __func__, enable);
	ret = regmap_update_bits(buck->regmap, CHG_SYS_LOGIC_CFG_REG_4_REG, CHG_LDO33_EN_MSK,
		 enable << CHG_LDO33_EN_SHIFT);
	/* delay 10ms */
	if (enable)
		mdelay(10);

	return ret;
}

int scharger_efuse_read(u8 addr, u8 *value)
{
	int ret = 0;
	u32 val = 0;
	struct scharger_buck *buck = g_buck;

	if (buck == NULL) {
		scharger_err("[%s]buck is null.\n", __func__);
		return -ENOMEM;
	}

	mutex_lock(&buck->efuse_lock);
	scharger_v800_ldo33_enable(buck, 1);
	scharger_efuse_read_enable(buck, EFUSE_RD_ENABLE);
	ret = regmap_update_bits(buck->regmap, EFUSE_TESTBUS_CFG_REG, EFUSE_TESTBUS_SEL_MSK,
				addr << EFUSE_TESTBUS_SEL_SHIFT);
	ret += regmap_read(buck->regmap, EFUSE_TESTBUS_RDATA_REG, &val);
	if (ret) {
		mutex_unlock(&buck->efuse_lock);
		scharger_err("%s read efuse testbus data fail\n", __func__);
		return -1;
	}
	*value = (u8)val;
	scharger_efuse_read_enable(buck, EFUSE_RD_DISABLE);
	scharger_v800_ldo33_enable(buck, 0);
	mutex_unlock(&buck->efuse_lock);

	return ret;
}

static int scharger_efuse_write(u8 addr, u8 value, u8 mask)
{
	struct scharger_buck *buck = g_buck;
	int ret = 0;
	u8 rd_val = 0;
	u8 wr_val = 0;
	u8 bit_addr_h = 0;
	u8 bit_addr_l = 0;
	u16 bit_0_cfg = 0;
	int i;
	unsigned long flags;

	scharger_inf("%s addr=0x%x, value=0x%x, mask=0x%x\n", __func__, addr, value, mask);

	if (buck == NULL) {
		scharger_err("[%s]buck is null.\n", __func__);
		return -ENXIO;
	}

	ret += scharger_efuse_read(addr, &rd_val);
	wr_val = rd_val & (~mask);
	wr_val |= value & mask;

	mutex_lock(&buck->efuse_lock);
	ret += scharger_v800_ldo33_enable(buck, 1);
	ret += regmap_write(buck->regmap, EFUSE_ADDR_CAL_SEL_REG, 1);
	ret += regmap_write(buck->regmap, EFUSE_PDOB_SEL_REG, 0);
	ret += regmap_write(buck->regmap, EFUSE_MODE_REG, EFUSE_MODE_WR_MODE);
	ret += regmap_write(buck->regmap, EFUSE_RDEN_REG, 0);
	ret += regmap_write(buck->regmap, EFUSE_PGMEN_REG, 1);

	bit_0_cfg = addr * REG_BIT_NUM;
	for (i = 0 ; i < REG_BIT_NUM ; i++) {
		if (((wr_val & (0x1 << i)) != 0) && !((rd_val & (0x1 << i)) != 0)) {
			bit_addr_h = ((bit_0_cfg + i) & BIT_ADDR_H_MSK) >> REG_BIT_NUM;
			bit_addr_l = (bit_0_cfg + i) & BIT_ADDR_L_MSK;
			scharger_inf("%s bit_addr=%d, bit_addr_h=0x%x, bit_addr_l=0x%x\n",
				__func__, bit_0_cfg + i, bit_addr_h, bit_addr_l);
			ret += regmap_write(buck->regmap, EFUSE_ADDR_H_REG, (unsigned int)bit_addr_h);
			ret += regmap_write(buck->regmap, EFUSE_ADDR_REG, (unsigned int)bit_addr_l);
			__pm_stay_awake(buck->efuse_write_wake_lock);
			spin_lock_irqsave(&(buck->efuse_write_lock), flags);
			ret += regmap_write(buck->regmap, EFUSE_AEN_REG, 1);
			/* delay 10us */
			udelay(10);
			ret += regmap_write(buck->regmap, EFUSE_AEN_REG, 0);
			spin_unlock_irqrestore(&(buck->efuse_write_lock), flags);
			__pm_relax(buck->efuse_write_wake_lock);
			mdelay(1);
		}
	}

	ret += regmap_write(buck->regmap, EFUSE_RDEN_REG, 0);
	ret += regmap_write(buck->regmap, EFUSE_PGMEN_REG, 0);
	ret += regmap_write(buck->regmap, EFUSE_MODE_REG, EFUSE_MODE_WR_MODE_EXIT);
	ret += regmap_write(buck->regmap, EFUSE_RD_CTRL_REG, 1);
	/* delay 10us */
	udelay(10);
	ret += regmap_write(buck->regmap, EFUSE_RD_CTRL_REG, 0);
	ret += scharger_v800_ldo33_enable(buck, 0);
	mutex_unlock(&buck->efuse_lock);

	return ret;
}

static int get_dieid(struct scharger_buck *buck, u8 *dieid, u32 len)
{
	int i;
	int err;
	u8 dieid_addr[SCHARGER_DIEID_LENGTH] = {
				SCHARGER_DIEID_BYTE0,
				SCHARGER_DIEID_BYTE1,
				SCHARGER_DIEID_BYTE2,
				SCHARGER_DIEID_BYTE3,
				SCHARGER_DIEID_BYTE4,
				SCHARGER_DIEID_BYTE5,
				SCHARGER_DIEID_BYTE6,
				SCHARGER_DIEID_BYTE7
	};

	if (dieid == NULL) {
		scharger_err("%s: dieid is null\n", __func__);
		return -1;
	}
	if (len < SCHARGER_DIEID_LENGTH) {
		scharger_err("%s: mem length is not enough!\n", __func__);
		return -1;
	}

	err = memset_s(dieid, len, 0, len);
	if (err != EOK)
		scharger_err("[%s]memset_s fail, err=%d\n", __func__, err);

	for (i = 0 ; i < SCHARGER_DIEID_LENGTH; i++)
		scharger_efuse_read(dieid_addr[i], &dieid[i]);

	return 0;
}

static int scharger_v800_get_dieid(char *dieid, unsigned int len)
{
	int ret = 0;
	errno_t err;
	u8 val[SCHARGER_DIEID_LENGTH];
	struct scharger_buck *buck = g_buck;

	if (buck == NULL) {
		scharger_err("[%s]buck is null.\n", __func__);
		return -ENOMEM;
	}
	if (dieid == NULL) {
		scharger_err("%s: dieid is null\n", __func__);
		return -1;
	}
	if (len < SCHARGER_DIEID_STRING_LENGTH) {
		scharger_err("%s: mem length is not enough!\n", __func__);
		return -1;
	}
	err = memset_s(dieid, len, 0, len);
	if (err != EOK)
		scharger_err("[%s]memset_s fail, err=%d\n", __func__, err);

	get_dieid(buck, val, SCHARGER_DIEID_LENGTH);
	ret = snprintf_s(dieid, len, len - 1,
				 "\r\nHi6526v800:0x%02x%02x%02x%02x%02x%02x%02x%02x\r\n",
				 val[7], val[6], val[5], val[4], val[3],
				 val[2], val[1], val[0]);
	if (ret < 0)
		scharger_err("%s, offset %d, snprintf_s error\n",
			     __func__, ret);

	return 0;
}

static int cv_get_dieid(char *dieid, unsigned int len)
{
	struct scharger_buck *buck = g_buck;

	if (buck == NULL || dieid == NULL) {
		scharger_err("[%s]buck or dieid is null.\n", __func__);
		return -ENOMEM;
	}
	return get_dieid(buck, dieid, len);
}

static struct charge_device_ops scharger_ops = {
	.chip_init = scharger_chip_init,
	.get_dieid = scharger_v800_get_dieid,
	.dev_check = scharger_device_check,
	.set_input_current = scharger_v800_set_input_current,
	.set_charge_current = scharger_v800_set_charge_current,
	.get_charge_current = scharger_get_charge_current,
	.set_terminal_voltage = scharger_v800_set_terminal_voltage,
	.get_terminal_voltage = scharger_get_terminal_voltage,
	.set_terminal_current = scharger_v800_set_terminal_current,
	.set_term_enable = scharger_v800_set_term_enable,
	.set_force_term_enable = scharger_force_set_term_enable,
	.set_charge_enable = scharger_v800_set_charge_enable,
	.get_charge_state = scharger_get_charge_state,
	.get_charger_state = scharger_get_charger_state,
	.set_watchdog_timer = scharger_v800_set_watchdog_timer,
	.reset_watchdog_timer = scharger_v800_reset_watchdog_timer,
	.set_batfet_disable = scharger_set_batfet_disable,
	.set_charger_hiz = scharger_set_charger_hiz,
	.get_ibus = scharger_v800_get_ibus,
	.get_vbus = scharger_adc_get_vbus,
	.get_vbat_sys = dummy_fun_1,
	.check_input_dpm_state = scharger_check_input_dpm_state,
	.check_input_vdpm_state = scharger_check_input_dpm_state,
	.check_input_idpm_state = scharger_check_input_acl_state,
	.set_dpm_voltage = dummy_fun_2,
	.set_vbus_vset = set_vbus_vset,
	.get_vbus_vset = get_vbus_vset,
	.set_uvp_ovp = dummy_fun_1,
	.soft_vbatt_ovp_protect = scharger_soft_vbatt_ovp_protect,
	.rboost_buck_limit = scharger_rboost_buck_limit,
	.stop_charge_config = scharger_stop_charge_config,
	.set_otg_enable = dummy_fun_2,
	.set_otg_current = scharger_set_otg_current,
	.set_otg_switch_mode_enable = scharger_otg_switch_mode,
	.get_iin_set = scharger_get_input_current_set,
	.get_vbat = scharger_v800_get_vbat,
	.get_vusb = scharger_adc_get_vusb,
	.set_ship_mode_enable = scharger_set_ship_mode_enable,
	.set_low_power_mode_enable = scharger_set_low_power_mode_enable,
	.need_vusb_replace_vbus = scharger_need_vusb_replace_vbus,
	.get_dev_name = scharger_get_dev_name,
};

static struct charger_otg_device_ops scharger_otg_ops = {
	.dev_name = "schargerv800",
	.otg_set_charger_enable = scharger_v800_set_charge_enable,
	.otg_set_enable = scharger_set_otg_mode,
	.otg_set_current = scharger_set_otg_current,
	.otg_set_watchdog_timer = scharger_v800_set_watchdog_timer,
	.otg_reset_watchdog_timer = scharger_v800_reset_watchdog_timer,
	.otg_set_switch_mode = scharger_otg_switch_mode,
};

static struct fcp_buck_ops scharger_fcp_buck_ops = {
	.set_vbus_vset = buck_init,
	.get_vbus = scharger_v800_get_vbus,
};

static void scharger_fcp_init(void)
{
	int ret;

	scharger_fcp_buck_ops.regs.irq_vusb_plugout.reg = FCP_VUSB_PLUGOUT_REG;
	scharger_fcp_buck_ops.regs.irq_vusb_plugout.mask = FCP_VUSB_PLUGOUT_MSK;
	scharger_fcp_buck_ops.regs.cntl = FCP_CNTL_REG;
	scharger_fcp_buck_ops.regs.statuis = FCP_STATUIS_REG;
	scharger_fcp_buck_ops.regs.fcp_ctrl = FCP_FCP_CTRL_REG;
	scharger_fcp_buck_ops.regs.isr1 = FCP_ISR1_REG;
	scharger_fcp_buck_ops.regs.isr2 = FCP_ISR2_REG;
	scharger_fcp_buck_ops.regs.fcp_irq3 = FCP_FCP_IRQ3_REG;
	scharger_fcp_buck_ops.regs.fcp_irq4 = FCP_FCP_IRQ4_REG;
	scharger_fcp_buck_ops.regs.cmd = FCP_CMD_REG;
	scharger_fcp_buck_ops.regs.addr = FCP_ADDR_REG;
	scharger_fcp_buck_ops.regs.fcp_rdata0 = FCP_FCP_RDATA0_REG;
	scharger_fcp_buck_ops.regs.data0 = FCP_DATA0_REG;
	scharger_fcp_buck_ops.regs.fcp_adap_ctrl = FCP_FCP_ADAP_CTRL_REG;
	scharger_fcp_buck_ops.regs.fcp_soft_rst_ctrl = FCP_FCP_SOFT_RST_CTRL_REG;
	scharger_fcp_buck_ops.regs.fcp_length = FCP_FCP_LENGTH_REG;
	ret = scharger_fcp_buck_ops_register(&scharger_fcp_buck_ops);
	if (ret != 0)
		scharger_err("register fcp buck ops failed\n");
}

static struct power_log_ops scharger_log_ops = {
	.dev_name = "schargerv800",
	.dump_log_head = scharger_v800_get_register_head,
	.dump_log_content = scharger_v800_dump_register,
};

static int get_chg_mode(void)
{
	return (is_current_charge_mode(BUCK5V) || is_current_charge_mode(BUCK9V));
}

static int get_chg_enable(void)
{
	struct scharger_buck *buck = g_buck;

	if (buck == NULL) {
		scharger_err("[%s]buck is null.\n", __func__);
		return -ENOMEM;
	}

	return scharger_get_charge_enable(buck);
}

static int cv_set_vterm(int cv_ori, int cv_new_set)
{
	int cv_cur;
	int cv_set;
	int ret;

	if (cv_new_set - cv_ori > CV_TRIM_PROTECT) {
		g_cv_info->cv_trim_flag = 0;
		scharger_err("%s cv_new_set is too large,cv_new_set:%d, cv_ori:%d, stop cv trim\n",
			 __func__, cv_new_set, cv_ori);
		return -1;
	}

	cv_set = cv_new_set;
	if (cv_new_set - cv_ori > CV_TRIM_MAX) {
		cv_set = cv_ori + CV_TRIM_MAX;
		scharger_err("%s cv_new_set is too large, use %d\n", cv_set);
	}

	cv_cur = scharger_get_terminal_voltage();
	if (cv_cur != cv_ori) {
		g_cv_info->cv_trim_flag = 0;
		scharger_err("%s vterm changed cv_cur:%d, cv_ori:%d, stop cv trim\n", __func__, cv_cur, cv_ori);
		return -1;
	}

	g_cv_info->cv_ori_val = cv_ori;
	g_cv_info->cv_new_set_val = cv_set;

	ret = __set_terminal_voltage(cv_set);
	if (ret) {
		g_cv_info->cv_trim_flag = 0;
		scharger_err("%s set vterm fail\n", __func__);
		return -1;
	}

	g_cv_info->cv_trim_flag = 1;

	return 0;
}

static void clear_cv_trim_flag(void)
{
	g_cv_info->cv_trim_flag = 0;
}

static void reset_cv_value(void)
{
	int ret;

	if (g_cv_info->cv_trim_flag == 1) {
		ret = __set_terminal_voltage(g_cv_info->cv_ori_val);
		if (ret)
			scharger_err("%s set vterm fail\n", __func__);
	}
	g_cv_info->cv_new_set_val = 0;
	g_cv_info->cv_trim_flag = 0;
	g_cv_info->cv_ori_val = 0;
}

static struct scharger_cv_ops g_cv_ops = {
	.get_charge_done_state = scharger_get_charger_state,
	.get_chg_mode = get_chg_mode,
	.get_chg_enable = get_chg_enable,
	.get_vterm = scharger_get_terminal_voltage,
	.set_vterm = cv_set_vterm,
	.clear_cv_trim_flag = clear_cv_trim_flag,
	.reset_cv_value = reset_cv_value,
	.get_dieid = cv_get_dieid,
};

static int scharger_set_usbovp_sw(struct power_sw_attr *attr, int status)
{
	int ret = 0;

	if (!attr || !attr->dev) {
		scharger_err("%s attr/dev null\n", __func__);
		return -ENODEV;
	}

	if (status == POWER_SW_ON) {
		ret += scharger_v800_set_usb_ovp_mode(USBOVP_FORWARD_MODE);
	} else if (status == POWER_SW_OFF) {
		ret = scharger_v800_set_usb_ovp_mode(USBOVP_FORCE_CLOSE);
	} else {
		scharger_err("%s err input=%d\n", __func__, status);
		return -EINVAL;
	}

	return ret;
}

static int scharger_get_usbovp_sw(struct power_sw_attr *attr)
{
	int ret;
	unsigned int val = 0;
	struct scharger_buck *buck = NULL;

	if (!attr || !attr->dev) {
		scharger_err("%s attr/dev null\n", __func__);
		return -ENODEV;
	}

	buck = attr->dev;

	ret = regmap_read(buck->regmap, CHG_USBOVP_CTRL_REG, &val);
	if (ret)
		return POWER_SW_ON;

	val &= CHG_USBOVP_CTRL_MSK;
	scharger_dbg("%s usb ovp sw state=0x%x\n", __func__, val);

	if (val == USBOVP_FORWARD_MODE || val == USBOVP_FORCE_OPEN)
		return POWER_SW_ON;
	else
		return POWER_SW_OFF;
}

static void scharger_free_usbovp_sw(struct power_sw_attr *attr)
{
}

static int scharger_set_pswovp_sw(struct power_sw_attr *attr, int status)
{
	int ret = 0;

	if (!attr || !attr->dev) {
		scharger_err("%s attr/dev null\n", __func__);
		return -ENODEV;
	}

	if (status == POWER_SW_ON) {
		ret += scharger_v800_set_psw_ovp_mode(PSWOVP_FORWARD_MODE);
	} else if (status == POWER_SW_OFF) {
		ret = scharger_v800_set_psw_ovp_mode(PSWOVP_FORCE_CLOSE);
	} else {
		scharger_err("%s err input=%d\n", __func__, status);
		return -EINVAL;
	}

	return ret;
}

static int scharger_get_pswovp_sw(struct power_sw_attr *attr)
{
	int ret;
	unsigned int val = 0;
	struct scharger_buck *buck = NULL;

	if (!attr || !attr->dev) {
		scharger_err("%s attr/dev null\n", __func__);
		return -ENODEV;
	}

	buck = attr->dev;

	ret = regmap_read(buck->regmap, CHG_PSWOVP_CTRL_REG, &val);
	if (ret)
		return POWER_SW_ON;

	val &= CHG_PSWOVP_CTRL_MSK;
	scharger_inf("%s psw ovp sw state=%d\n", __func__, val);

	if (val == PSWOVP_FORWARD_MODE || val == PSWOVP_FORCE_OPEN)
		return POWER_SW_ON;
	else
		return POWER_SW_OFF;
}

static void scharger_free_pswovp_sw(struct power_sw_attr *attr)
{
}

static int scharger_set_usbovp_sw_flag(struct power_sw_attr *attr, int status)
{
	usbovp_state = status;
	if (status == POWER_SW_ON)
		return scharger_v800_set_usb_ovp_mode(USBOVP_FORWARD_MODE);	
	return 0;
}

static int scharger_get_usbovp_sw_flag(struct power_sw_attr *attr)
{
	return usbovp_state;
}

static int scharger_set_pswovp_sw_flag(struct power_sw_attr *attr, int status)
{
	pswovp_state = status;
	if (status == POWER_SW_ON)
		return scharger_v800_set_psw_ovp_mode(PSWOVP_FORWARD_MODE);
	return 0;
}

static int scharger_get_pswovp_sw_flag(struct power_sw_attr *attr)
{
	return pswovp_state;
}

static void scharger_pswovp_sw_flag_register(struct scharger_buck *buck)
{
	struct power_sw sw;

	if (buck->pswovp_sw_flag_attr.num < 0)
		return;

	sw.attr.dev = (void *)buck;
	sw.attr.num = buck->pswovp_sw_flag_attr.num;
	sw.attr.dflt = buck->pswovp_sw_flag_attr.dflt;
	sw.attr.en = buck->pswovp_sw_flag_attr.en;
	sw.attr.label = buck->pswovp_sw_flag_attr.label;
	sw.set = scharger_set_pswovp_sw_flag;
	sw.get = scharger_get_pswovp_sw_flag;
	sw.free = scharger_free_pswovp_sw;

	(void)power_sw_register(sw.attr.num, &sw);
}

static void scharger_usbovp_sw_flag_register(struct scharger_buck *buck)
{
	struct power_sw sw;

	if (buck->usbovp_sw_flag_attr.num < 0)
		return;

	sw.attr.dev = (void *)buck;
	sw.attr.num = buck->usbovp_sw_flag_attr.num;
	sw.attr.dflt = buck->usbovp_sw_flag_attr.dflt;
	sw.attr.en = buck->usbovp_sw_flag_attr.en;
	sw.attr.label = buck->usbovp_sw_flag_attr.label;
	sw.set = scharger_set_usbovp_sw_flag;
	sw.get = scharger_get_usbovp_sw_flag;
	sw.free = scharger_free_usbovp_sw;

	(void)power_sw_register(sw.attr.num, &sw);
}

static void scharger_pswovp_sw_register(struct scharger_buck *buck)
{
	struct power_sw sw;

	if (buck->pswovp_sw_attr.num < 0)
		return;

	sw.attr.dev = (void *)buck;
	sw.attr.num = buck->pswovp_sw_attr.num;
	sw.attr.dflt = buck->pswovp_sw_attr.dflt;
	sw.attr.en = buck->pswovp_sw_attr.en;
	sw.attr.label = buck->pswovp_sw_attr.label;
	sw.set = scharger_set_pswovp_sw;
	sw.get = scharger_get_pswovp_sw;
	sw.free = scharger_free_pswovp_sw;

	(void)power_sw_register(sw.attr.num, &sw);
}

static void scharger_usbovp_sw_register(struct scharger_buck *buck)
{
	struct power_sw sw;

	if (buck->usbovp_sw_attr.num < 0)
		return;

	sw.attr.dev = (void *)buck;
	sw.attr.num = buck->usbovp_sw_attr.num;
	sw.attr.dflt = buck->usbovp_sw_attr.dflt;
	sw.attr.en = buck->usbovp_sw_attr.en;
	sw.attr.label = buck->usbovp_sw_attr.label;
	sw.set = scharger_set_usbovp_sw;
	sw.get = scharger_get_usbovp_sw;
	sw.free = scharger_free_usbovp_sw;

	(void)power_sw_register(sw.attr.num, &sw);
}

static void scharger_ovpsw_register(struct scharger_buck *buck)
{
	scharger_pswovp_sw_register(buck);
	scharger_usbovp_sw_register(buck);
	scharger_pswovp_sw_flag_register(buck);
	scharger_usbovp_sw_flag_register(buck);
}

static void scharger_v800_buck_init_5v_delay_work(struct work_struct *work)
{
	struct scharger_buck *buck = g_buck;
	unsigned int sc_ss_ok = 0;
	unsigned int vusb_uvlo = 0;
	unsigned int vpsw_uvlo = 0;

	if (buck == NULL) {
		scharger_err("[%s]buck is null.\n", __func__);
		return;
	}
	regmap_read(buck->regmap, FCP_VUSB_PLUGOUT_REG, &vusb_uvlo);
	regmap_read(buck->regmap, FCP_VUSB_PLUGOUT_REG, &vpsw_uvlo);

	scharger_inf("%s\n", __func__);

	scharger_v800_mode_lock();
	/* exit buck mode or adapter plugout, cancel buck init 5v work */
	if ((buck->current_charge_mode.chg_mode != BUCK5V) ||
			(((vusb_uvlo & CHG_VUSB_UVLO_MSK) == CHG_VUSB_UVLO_MSK) &&
			((vpsw_uvlo & CHG_VPSW_UVLO_MSK) == CHG_VPSW_UVLO_MSK))) {
		scharger_v800_mode_unlock();
		scharger_inf("%s chg mode not buck5v or plugout, vusb_uvlo = 0x%x, vpsw_uvlo = 0x%x\n", __func__, vusb_uvlo, vpsw_uvlo);
		return;
	}
	regmap_read(buck->regmap, CHG_AD_STATUS_RSV3_REG, &sc_ss_ok);
	scharger_inf("%s sc_ss_ok:0x%x\n", __func__, sc_ss_ok);
	if ((sc_ss_ok & CHG_SC_SS_OK_MSK) != CHG_SC_SS_OK_MSK) {
		queue_delayed_work(system_power_efficient_wq, &buck->buck_init_5v_work,
			msecs_to_jiffies(BUCK_INIT_5V_WORK_DELAY));
		scharger_v800_mode_unlock();
		return;
	}
	scharger_v800_s1q4s_enable(buck, 1);
	mdelay(3);
	scharger_v800_s1q4s_enable(buck, 0);
	scharger_v800_s1q4_enable(buck, 1);
	(void)scharger_v800_set_s1q5_s1q6(BUCK5V);
	scharger_v800_mode_unlock();
}

static void scharger_v800_buck_init_9v_delay_work(struct work_struct *work)
{
	struct scharger_buck *buck = g_buck;
	unsigned int sc_ss_ok = 0;
	unsigned int vusb_uvlo = 0;
	unsigned int vpsw_uvlo = 0;

	if (buck == NULL) {
		scharger_err("[%s]buck is null.\n", __func__);
		return;
	}
	regmap_read(buck->regmap, FCP_VUSB_PLUGOUT_REG, &vusb_uvlo);
	regmap_read(buck->regmap, FCP_VUSB_PLUGOUT_REG, &vpsw_uvlo);

	scharger_inf("%s\n", __func__);

	scharger_v800_mode_lock();
	/* exit buck mode or adapter plugout, cancel enable buck init 9v delay work */
	if ((buck->current_charge_mode.chg_mode != BUCK9V) ||
			(((vusb_uvlo & CHG_VUSB_UVLO_MSK) == CHG_VUSB_UVLO_MSK) &&
			((vpsw_uvlo & CHG_VPSW_UVLO_MSK) == CHG_VPSW_UVLO_MSK))) {
		scharger_v800_mode_unlock();
		scharger_inf("%s chg mode not buck9v or plugout, vusb_uvlo = 0x%x, vpsw_uvlo = 0x%x\n", __func__, vusb_uvlo, vpsw_uvlo);
		return;
	}
	regmap_read(buck->regmap, CHG_AD_STATUS_RSV3_REG, &sc_ss_ok);
	scharger_inf("%s sc_ss_ok:0x%x\n", __func__, sc_ss_ok);
	if ((sc_ss_ok & CHG_SC_SS_OK_MSK) != CHG_SC_SS_OK_MSK) {
		queue_delayed_work(system_power_efficient_wq, &buck->buck_init_9v_work,
		   msecs_to_jiffies(BUCK_INIT_9V_WORK_DELAY));
		scharger_v800_mode_unlock();
		return;
	}
	scharger_v800_s1q4s_enable(buck, 1);
	mdelay(3);
	scharger_v800_s1q4s_enable(buck, 0);
	scharger_v800_s1q4_enable(buck, 1);
	(void)scharger_v800_set_s1q5_s1q6(BUCK9V);
	(void)scharger_set_vusb_vpsw_ovp(BUCK9V);
	(void)scharger_v800_set_vbus_uvp(BUCK9V);
	(void)scharger_v800_set_vbus_ovp(BUCK9V);
	(void)scharger_v800_set_buck_mode_enable(CHG_ENABLE);
	scharger_v800_mode_unlock();
}

static void scharger_v800_buck_ocp_trim_work(struct work_struct *work)
{
	struct scharger_buck *buck =
		 container_of(work, struct scharger_buck, buck_ocp_trim_work.work);
	u8 val = 0;
	int vbat;

	if (buck == NULL) {
		scharger_err("[%s]buck is null.\n", __func__);
		return;
	}

	scharger_inf("%s\n", __func__);
	scharger_efuse_read(EFUSE_ADDR_43, &val);
	/* if buck ocp trim value is 0x2/0x3/0x4/0x5/0x6/0x7, change the value to 0xf */
	if (!(((val & EFUSE_BUCK_OCP_MSK) >> EFUSE_BUCK_OCP_SHIFT) == 0x2 ||
		((val & EFUSE_BUCK_OCP_MSK) >> EFUSE_BUCK_OCP_SHIFT) == 0x3 ||
		((val & EFUSE_BUCK_OCP_MSK) >> EFUSE_BUCK_OCP_SHIFT) == 0x4 ||
		((val & EFUSE_BUCK_OCP_MSK) >> EFUSE_BUCK_OCP_SHIFT) == 0x5 ||
		((val & EFUSE_BUCK_OCP_MSK) >> EFUSE_BUCK_OCP_SHIFT) == 0x6 ||
		((val & EFUSE_BUCK_OCP_MSK) >> EFUSE_BUCK_OCP_SHIFT) == 0x7)) {
		scharger_inf("%s val=0x%x, work exit\n", __func__, val);
		return;
	}

	vbat = coul_drv_battery_voltage();
	/* vbat 4000mV */
	if (vbat > 4000) {
		scharger_inf("%s val=0x%x\n", __func__, val);
		scharger_efuse_write(EFUSE_ADDR_43, EFUSE_BUCK_OCP_TRIM_VAL << EFUSE_BUCK_OCP_SHIFT, EFUSE_BUCK_OCP_MSK);
		scharger_efuse_read(EFUSE_ADDR_43, &val);
		scharger_inf("%s change the value to 0x%x\n", __func__, val);
	} else {
		queue_delayed_work(system_power_efficient_wq, &buck->buck_ocp_trim_work,
			msecs_to_jiffies(BUCK_OCP_TRIM_WORK_DELAY));
	}
}

static int scharger_buck_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct scharger_buck *buck = NULL;
	int ret;

	scharger_err("%s +\n", __func__);
	buck = devm_kzalloc(dev, sizeof(*buck), GFP_KERNEL);
	if (buck == NULL) {
		scharger_err("[%s]buck is null.\n", __func__);
		return -ENOMEM;
	}

	buck->dev = dev;
	buck->regmap = dev_get_regmap(dev->parent, NULL);
	if (buck->regmap == NULL) {
		scharger_err("Parent regmap unavailable.\n");
		return -ENXIO;
	}

	g_cv_info = (struct cv_info *)devm_kzalloc(dev, sizeof(struct cv_info), GFP_KERNEL);
	if (g_cv_info == NULL) {
		scharger_err("[%s]g_cv_info is null.\n", __func__);
		return -ENOMEM;
	}

	buck->efuse_write_wake_lock = wakeup_source_register(dev, "scharger v800 efuse write wakelock");
	spin_lock_init(&buck->efuse_write_lock);
	mutex_init(&buck->mode_lock);
	mutex_init(&buck->efuse_lock);

	scharger_parse_dts(dev->of_node, buck);
	INIT_WORK(&buck->irq_work, scharger_v800_irq_work);
	INIT_DELAYED_WORK(&buck->buck_init_5v_work, scharger_v800_buck_init_5v_delay_work);
	INIT_DELAYED_WORK(&buck->buck_init_9v_work, scharger_v800_buck_init_9v_delay_work);
	INIT_DELAYED_WORK(&buck->reverbst_work, reverbst_delay_work);
	INIT_DELAYED_WORK(&buck->buck_irq_usb_ovp_work, scharger_v800_buck_irq_usb_ovp_work);
	INIT_DELAYED_WORK(&buck->buck_irq_psw_ovp_work, scharger_v800_buck_irq_psw_ovp_work);
	INIT_DELAYED_WORK(&buck->buck_ocp_trim_work, scharger_v800_buck_ocp_trim_work);

	buck->usb_nb.notifier_call = scharger_usb_notifier_call;
	ret = chip_charger_type_notifier_register(&buck->usb_nb);
	if (ret)
		scharger_err("%s notifier_register failed\n", __func__);

	scharger_v800_vbat_lv_handle(buck);
	scharger_v800_chg_irq_init(buck);
	scharger_v800_sc_irq_init(buck);

	ret = charge_ops_register(&scharger_ops, BUCK_IC_TYPE_THIRDPARTY);
	if (ret != 0)
		scharger_err("register charge ops failed!\n");
#ifdef CONFIG_HUAWEI_CHARGER_AP
	ret = charger_otg_ops_register(&scharger_otg_ops);
	if (ret != 0)
		scharger_err("register charger_otg ops failed\n");
#endif
	scharger_ovpsw_register(buck);
#ifdef CONFIG_SCHARGER_DYNAMIC_CV
	ret = scharger_cv_ops_register(&g_cv_ops);
	if (ret)
		scharger_err("register cv ops failed\n");
#endif
	scharger_fcp_init();
	scharger_log_ops.dev_data = (void *)buck;
	power_log_ops_register(&scharger_log_ops);

	g_buck = buck;
	scharger_device_check();
	buck->term_vol_mv = (unsigned int)scharger_get_terminal_voltage();
	scharger_v800_get_voltage_offset();
	scharger_v800_set_current_charge_mode(UNKNOWN);

	queue_delayed_work(system_power_efficient_wq, &buck->buck_ocp_trim_work,
			msecs_to_jiffies(BUCK_OCP_TRIM_WORK_DELAY));
	scharger_err("%s -\n", __func__);
	return 0;
}

static int scharger_buck_suspend(struct platform_device *pdev, pm_message_t state)
{
	int ret;
	struct scharger_buck *buck = g_buck;

	if (buck == NULL) {
		scharger_err("%s scharger_buck is NULL!\n", __func__);
		return -EINVAL;
	}

	/* v800: before enter eco, mask buck_vbus_uvp_irq to avoid irq report */
	ret = regmap_update_bits(buck->regmap, BUCK_VBUS_UVP_IRQ_REG,
		 BUCK_VBUS_UVP_MASK, 1 << BUCK_VBUS_UVP_SHIFT);
	return ret;
}

static int scharger_buck_resume(struct platform_device *pdev)
{
	int ret;
	struct scharger_buck *buck = g_buck;

	if (buck == NULL) {
		scharger_err("%s scharger_buck is NULL!\n", __func__);
		return -EINVAL;
	}

	ret = regmap_update_bits(buck->regmap, BUCK_VBUS_UVP_IRQ_REG,
		 BUCK_VBUS_UVP_MASK, 0 << BUCK_VBUS_UVP_SHIFT);
	return ret;
}

static void scharger_buck_shutdown(struct platform_device *pdev)
{
}

const static struct of_device_id of_scharger_buck_match_tbl[] = {
	{
		.compatible = "hisilicon,scharger-v800-buck",
		.data = NULL,
	},
	{},
};

static struct platform_driver scharger_buck_driver = {
	.driver = {
		.name = "scharger_v800_buck",
		.owner = THIS_MODULE,
		.of_match_table = of_scharger_buck_match_tbl,
	},
	.probe = scharger_buck_probe,
	.shutdown = scharger_buck_shutdown,
	.suspend = scharger_buck_suspend,
	.resume = scharger_buck_resume,
};

static int __init scharger_buck_init(void)
{
	return platform_driver_register(&scharger_buck_driver);
}

static void __exit scharger_buck_exit(void)
{
	platform_driver_unregister(&scharger_buck_driver);
}

fs_initcall(scharger_buck_init);
module_exit(scharger_buck_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("schargerV800 buck driver");
