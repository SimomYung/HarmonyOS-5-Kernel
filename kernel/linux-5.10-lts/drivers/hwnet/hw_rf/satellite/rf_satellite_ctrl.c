/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2024. All rights reserved.
 * Description: This module is used to proc satelite related ctrl.
 */

#include <securec.h>
#include <log/hw_log.h>
#include <linux/clk.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/gpio.h>
#include <linux/jiffies.h>
#include <chipset_common/hwpower/common_module/power_event_ne.h>
#include <linux/kernel.h>
#include <linux/ktime.h>
#include <linux/kthread.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/poll.h>
#include <linux/reboot.h>
#include <linux/regulator/consumer.h>
#include <linux/regulator/driver.h>
#include <linux/regulator/machine.h>
#include <linux/version.h>

#include "rf_satellite_ctrl.h"

#include <chipset_common/hwpower/hardware_ic/buck_boost.h>

#ifdef HWLOG_TAG
#undef HWLOG_TAG
#endif
#define HWLOG_TAG HW_RF_SATELITE_CTRL
HWLOG_REGIST();

enum i2c_type {
	DISABLE,
	ENABLE,
};

static struct regulator *g_pwr_ldo_regulator = NULL;
static bool g_is_rsmc_on = false;

static bool ldo_is_valid(int32_t ldo_num)
{
	if (ldo_num < 0) {
		hwlog_err("%s: can not get ldo", __func__);
		return false;
	}
	return true;
}

static int rf_regulator_enable(struct ps_core_s *cd,
								char *regulator_name, int32_t regulator_vol)
{
	int ret;
	int voltage;
	hwlog_info("%s: enter \n", __func__);
	g_pwr_ldo_regulator = devm_regulator_get(cd->dev, regulator_name);
	if (IS_ERR(g_pwr_ldo_regulator)) {
		hwlog_err("%s:No ldo found for regulator\n", __func__);
		return -1;
	}
	ret = regulator_set_voltage(g_pwr_ldo_regulator, regulator_vol, regulator_vol);
	if (ret != 0) {
		hwlog_err("%s:regulator_set_voltage fail,ret = %d\n", __func__, ret);
		return ret;
	}
	ret = regulator_set_mode(g_pwr_ldo_regulator, REGULATOR_MODE_NORMAL);
	if (ret != 0) {
		hwlog_err("%s:regulator_pwr_ldo_regulator fail,ret = %d\n", __func__, ret);
		return ret;
	}
	ret = regulator_enable(g_pwr_ldo_regulator);
	if (ret != 0) {
		hwlog_err("%s:regulator_enable fail,ret = %d\n", __func__, ret);
		return ret;
	}

	voltage = regulator_get_voltage(g_pwr_ldo_regulator);
	hwlog_info("%s: voltage %d\n", __func__, voltage);
	return 0;
}

static int32_t rsmc_on_proc_other(struct ps_core_s *cd)
{
	hwlog_info("%s: enter \n", __func__);
	if (cd == NULL) {
		hwlog_err("%s: core data is null", __func__);
		return -1;
	}

	if (gpio_is_valid(cd->sate_uefi_oled_en)) {
		hwlog_info("%s, sate_uefi_oled_en", __func__);
		gpio_set_value(cd->sate_uefi_oled_en, GPIO_LOW);
	}

	if (gpio_is_valid(cd->sate_rsmc_bbst_gpio_en)) {
		hwlog_info("%s, sate_rsmc_bbst_gpio_en", __func__);
		gpio_set_value(cd->sate_rsmc_bbst_gpio_en, GPIO_HIGH);
	}

	if (gpio_is_valid(cd->sate_rsmc_wifi_fem_en)) {
		hwlog_info("%s, sate_rsmc_wifi_fem_en", __func__);
		gpio_set_value(cd->sate_rsmc_wifi_fem_en, GPIO_HIGH);
	}

	if (gpio_is_valid(cd->sate_rsmc_pwr_en)) {
		hwlog_info("%s, sate_rsmc_pwr_en", __func__);
		gpio_set_value(cd->sate_rsmc_pwr_en, GPIO_HIGH);
	}

	if (gpio_is_valid(cd->sate_tts_rsmc_sw)) {
		hwlog_info("%s, sate_tts_rsmc_sw", __func__);
		gpio_set_value(cd->sate_tts_rsmc_sw, GPIO_LOW);
	}

	return 0;
}

int32_t rsmc_on_proc(struct ps_core_s *cd)
{
	hwlog_info("%s: enter \n", __func__);
	int32_t ret;
	if (cd == NULL) {
		hwlog_err("%s: core data is null", __func__);
		return -1;
	}

	if (g_is_rsmc_on) {
		hwlog_err("%s: state is on", __func__);
		return 0;
	}

#ifdef CONFIG_SCSI_UFS_RSMC
	if (cd->rsmc_ufs_lp_mode_support) {
		//ICL北斗使能时，关闭低功耗模式
		ufshcd_auto_hibern8_close();
		hwlog_info("%s, close ufs_lp_mode", __func__);
	}
#endif

	if (gpio_is_valid(cd->sate_rsmc_gpio_mipi)) {
		hwlog_info("%s, sate_rsmc_gpio_mipi", __func__);
		gpio_set_value(cd->sate_rsmc_gpio_mipi, GPIO_HIGH);
	}

	ret = rsmc_on_proc_other(cd);
	if (ret != 0) {
		hwlog_err("%s:rsmc_on_proc_other fail,ret = %d\n", __func__, ret);
		return ret;
	}

	if (ldo_is_valid(cd->sate_rsmc_pwr_ldo_vol)) {
		hwlog_info("%s: ldo config, voltage %d\n", __func__, cd->sate_rsmc_pwr_ldo_vol);
		ret = rf_regulator_enable(cd, "sate_rsmc_pwr_ldo", cd->sate_rsmc_pwr_ldo_vol);
		if (ret != 0) {
			hwlog_err("%s:rf_regulator_enable fail,ret = %d\n", __func__, ret);
			return ret;
		}
	}

	if (cd->sate_rsmc_i2c_on_volt >= 0 && cd->sate_rsmc_i2c_index >= 0) {
		hwlog_info("%s, buckboost config, index %d, voltage %d\n", __func__,
			cd->sate_rsmc_i2c_index, cd->sate_rsmc_i2c_on_volt);
		ret = buckboost_set_enable(ENABLE, BBST_USER_BOOST_RSMC, cd->sate_rsmc_i2c_index);
		if (ret != 1)
			hwlog_err("%s:buckboost_set_enable fail,ret = %d\n", __func__, ret);
		ret = buckboost_set_vout(cd->sate_rsmc_i2c_on_volt, BBST_USER_BOOST_RSMC, cd->sate_rsmc_i2c_index);
		if (ret != 0)
			hwlog_err("%s:buckboost_set_vout fail,ret = %d\n", __func__, ret);
	} else {
		hwlog_info("%s, buckboost param invaild", __func__);
	}

	g_is_rsmc_on = true;
	return 0;
}

static int32_t rsmc_off_proc_other(struct ps_core_s *cd)
{
	hwlog_info("%s: enter \n", __func__);
	if (cd == NULL) {
		hwlog_err("%s: core data is null", __func__);
		return -1;
	}

	if (gpio_is_valid(cd->sate_uefi_oled_en)) {
		hwlog_info("%s, sate_uefi_oled_en", __func__);
		gpio_set_value(cd->sate_uefi_oled_en, GPIO_LOW);
	}

	if (gpio_is_valid(cd->sate_rsmc_bbst_gpio_en)) {
		hwlog_info("%s, sate_rsmc_bbst_gpio_en", __func__);
		gpio_set_value(cd->sate_rsmc_bbst_gpio_en, GPIO_HIGH);
	}

	if (gpio_is_valid(cd->sate_rsmc_wifi_fem_en)) {
		hwlog_info("%s, sate_rsmc_wifi_fem_en", __func__);
		gpio_set_value(cd->sate_rsmc_wifi_fem_en, GPIO_LOW);
	}

	if (gpio_is_valid(cd->sate_rsmc_pwr_en)) {
		hwlog_info("%s, sate_rsmc_pwr_en", __func__);
		gpio_set_value(cd->sate_rsmc_pwr_en, GPIO_HIGH);
	}

	if (gpio_is_valid(cd->sate_tts_rsmc_sw)) {
		hwlog_info("%s, sate_tts_rsmc_sw", __func__);
		gpio_set_value(cd->sate_tts_rsmc_sw, GPIO_LOW);
	}

	return 0;
}

int32_t rsmc_off_proc(struct ps_core_s *cd)
{
	hwlog_info("%s: enter \n", __func__);
	int ret;
	if (cd == NULL) {
		hwlog_err("%s: core data is null", __func__);
		return -1;
	}

	if (!g_is_rsmc_on) {
		hwlog_err("%s: state is off", __func__);
		return 0;
	}

#ifdef CONFIG_SCSI_UFS_RSMC
	if (cd->rsmc_ufs_lp_mode_support) {
		//ICL北斗去使能时，打开低功耗模式
		ufshcd_auto_hibern8_open();
		hwlog_info("%s, open ufs_lp_mode", __func__);
	}
#endif

	if (gpio_is_valid(cd->sate_rsmc_gpio_mipi)) {
		hwlog_info("%s, sate_rsmc_gpio_mipi", __func__);
		gpio_set_value(cd->sate_rsmc_gpio_mipi, GPIO_LOW);
	}

	if (g_pwr_ldo_regulator != NULL) {
		ret = regulator_disable(g_pwr_ldo_regulator);
		hwlog_info("%s:pwr_ldo_regulator disable,ret = %d\n", __func__, ret);
		devm_regulator_put(g_pwr_ldo_regulator);
		g_pwr_ldo_regulator = NULL;
	}

	if (cd->sate_rsmc_i2c_off_volt >= 0 && cd->sate_rsmc_i2c_index >= 0) {
		hwlog_info("%s, buckboost config, index %d, voltage %d\n", __func__,
			cd->sate_rsmc_i2c_index, cd->sate_rsmc_i2c_off_volt);
		ret = buckboost_set_vout(cd->sate_rsmc_i2c_off_volt, BBST_USER_BOOST_RSMC, cd->sate_rsmc_i2c_index);
		if (ret != 0)
			hwlog_err("%s:buckboost_set_vout fail,ret = %d\n", __func__, ret);
		ret = buckboost_set_enable(DISABLE, BBST_USER_BOOST_RSMC, cd->sate_rsmc_i2c_index);
		if (ret != 1)
			hwlog_err("%s:buckboost_set_enable fail,ret = %d\n", __func__, ret);
	} else {
		hwlog_info("%s, buckboost param invaild", __func__);
	}

	ret = rsmc_off_proc_other(cd);
	if (ret != 0) {
		hwlog_err("%s:rsmc_off_proc_other fail,ret = %d\n", __func__, ret);
		return ret;
	}

	g_is_rsmc_on = false;
	return 0;
}

static int32_t tts_on_proc_other(struct ps_core_s *cd)
{
	hwlog_info("%s: enter \n", __func__);
	if (cd == NULL) {
		hwlog_err("%s: core data is null", __func__);
		return -1;
	}

	if (gpio_is_valid(cd->sate_tts_rf_bp_bst_en)) {
		hwlog_info("%s, sate_tts_rf_bp_bst_en", __func__);
		gpio_set_value(cd->sate_tts_rf_bp_bst_en, GPIO_HIGH);
	}

	if (gpio_is_valid(cd->sate_tts_rsmc_sw)) {
		hwlog_info("%s, sate_tts_rsmc_sw", __func__);
		gpio_set_value(cd->sate_tts_rsmc_sw, GPIO_HIGH);
	}

	return 0;
}

int32_t tts_on_proc(struct ps_core_s *cd)
{
	hwlog_info("%s: enter \n", __func__);
	int32_t ret;

	if (cd == NULL) {
		hwlog_err("%s: core data is null", __func__);
		return -1;
	}

	ret = tts_on_proc_other(cd);
	if (ret != 0) {
		hwlog_err("%s:tts_on_proc_other fail,ret = %d\n", __func__, ret);
		return ret;
	}

	if (ldo_is_valid(cd->sate_tts_pwr_ldo_vol)) {
		hwlog_info("%s: ldo config, voltage %d\n", __func__, cd->sate_tts_pwr_ldo_vol);
		ret = rf_regulator_enable(cd, "sate_tts_pwr_ldo", cd->sate_tts_pwr_ldo_vol);
		if (ret != 0) {
			hwlog_err("%s:rf_regulator_enable fail,ret = %d\n", __func__, ret);
			return ret;
		}
	}

	if (cd->sate_tts_i2c_on_volt >= 0 && cd->sate_tts_i2c_index >= 0) {
		hwlog_info("%s, buckboost config, index %d, voltage %d\n", __func__,
			cd->sate_tts_i2c_index, cd->sate_tts_i2c_on_volt);
		ret = buckboost_set_enable(ENABLE, BBST_USER_BOOST_HW_SATE, cd->sate_tts_i2c_index);
		if (ret != 1)
			hwlog_err("%s:buckboost_set_enable fail,ret = %d\n", __func__, ret);
		ret = buckboost_set_vout(cd->sate_tts_i2c_on_volt, BBST_USER_BOOST_HW_SATE, cd->sate_tts_i2c_index);
		if (ret != 0)
			hwlog_err("%s:buckboost_set_vout fail,ret = %d\n", __func__, ret);
		/* Notifies PMU，AP not control bypass boost. */
		power_event_bnc_notify(POWER_BNT_BYPASSBOOST, POWER_NE_BYPASSBOOST_CP_CTRL, &cd->sate_tts_i2c_index);
	} else {
		hwlog_info("%s, buckboost param invaild", __func__);
	}
	msleep(10);    // APT control delay time
	/* config gpio to high after buckboost config when tts start */
	if (gpio_is_valid(cd->sate_tts_i2c_pwr_sw)) {
		hwlog_info("%s, sate_tts_i2c_pwr_sw", __func__);
		gpio_set_value(cd->sate_tts_i2c_pwr_sw, GPIO_HIGH);
	}

	return 0;
}

static int32_t tts_off_proc_other(struct ps_core_s *cd)
{
	hwlog_info("%s: enter \n", __func__);
	if (cd == NULL) {
		hwlog_err("%s: core data is null", __func__);
		return -1;
	}

	if (gpio_is_valid(cd->sate_tts_rsmc_sw)) {
		hwlog_info("%s, sate_tts_rsmc_sw", __func__);
		gpio_set_value(cd->sate_tts_rsmc_sw, GPIO_LOW);
	}

	if (gpio_is_valid(cd->sate_tts_rf_bp_bst_en)) {
		hwlog_info("%s, sate_tts_rf_bp_bst_en", __func__);
		gpio_set_value(cd->sate_tts_rf_bp_bst_en, GPIO_HIGH);
	}

	return 0;
}

int32_t tts_off_proc(struct ps_core_s *cd)
{
	hwlog_info("%s: enter \n", __func__);
	int32_t ret;
	if (cd == NULL) {
		hwlog_err("%s: core data is null", __func__);
		return -1;
	}

	if (g_pwr_ldo_regulator != NULL) {
		ret = regulator_disable(g_pwr_ldo_regulator);
		hwlog_info("%s:pwr_ldo_regulator disable,ret = %d\n", __func__, ret);
		devm_regulator_put(g_pwr_ldo_regulator);
		g_pwr_ldo_regulator = NULL;
	}

	/* config gpio to low before buckboost config when tts stop */
	if (gpio_is_valid(cd->sate_tts_i2c_pwr_sw)) {
		hwlog_info("%s, sate_tts_i2c_pwr_sw", __func__);
		gpio_set_value(cd->sate_tts_i2c_pwr_sw, GPIO_LOW);
	}
	msleep(10);    // APT control delay time
	if (cd->sate_tts_i2c_off_volt >= 0 && cd->sate_tts_i2c_index >= 0) {
		hwlog_info("%s, buckboost config, index %d, voltage %d\n", __func__,
			cd->sate_tts_i2c_index, cd->sate_tts_i2c_off_volt);
		/* Notifies PMU，AP restore bypass boost status. */
		power_event_bnc_notify(POWER_BNT_BYPASSBOOST, POWER_NE_BYPASSBOOST_AP_CTRL, &cd->sate_tts_i2c_index);
		ret = buckboost_set_vout(cd->sate_tts_i2c_off_volt, BBST_USER_BOOST_HW_SATE, cd->sate_tts_i2c_index);
		if (ret != 0)
			hwlog_err("%s:buckboost_set_vout fail,ret = %d\n", __func__, ret);
		ret = buckboost_set_enable(DISABLE, BBST_USER_BOOST_HW_SATE, cd->sate_tts_i2c_index);
		if (ret != 1)
			hwlog_err("%s:buckboost_set_enable fail,ret = %d\n", __func__, ret);
	} else {
		hwlog_info("%s, buckboost param invaild", __func__);
	}

	ret = tts_off_proc_other(cd);
	if (ret != 0) {
		hwlog_err("%s:tts_off_proc_other fail,ret = %d\n", __func__, ret);
		return ret;
	}

	return 0;
}

static int32_t rf_rsmc_parse_dts_config_other(struct device_node *dev_node, struct ps_core_s *cd)
{
	hwlog_info("%s: enter \n", __func__);
	int32_t value;
	if ((dev_node == NULL) || (cd == NULL)) {
		hwlog_err("%s: input null\n", __func__);
		return -1;
	}

	value = of_get_named_gpio(dev_node, "sate_uefi_oled_en", 0);
	hwlog_info("%s, sate_uefi_oled_en:%d\n", __func__, value);
	cd->sate_uefi_oled_en = value;

	value = of_get_named_gpio(dev_node, "sate_rsmc_bbst_gpio_en", 0);
	hwlog_info("%s, sate_rsmc_bbst_gpio_en:%d\n", __func__, value);
	cd->sate_rsmc_bbst_gpio_en = value;

	value = of_get_named_gpio(dev_node, "sate_rsmc_wifi_fem_en", 0);
	hwlog_info("%s, sate_rsmc_wifi_fem_en:%d\n", __func__, value);
	cd->sate_rsmc_wifi_fem_en = value;

	value = of_get_named_gpio(dev_node, "sate_rsmc_pwr_en", 0);
	hwlog_info("%s, sate_rsmc_pwr_en:%d\n", __func__, value);
	cd->sate_rsmc_pwr_en = value;

	value = of_get_named_gpio(dev_node, "sate_tts_rsmc_sw", 0);
	hwlog_info("%s, sate_tts_rsmc_sw:%d\n", __func__, value);
	cd->sate_tts_rsmc_sw = value;

	return 0;
}

static int32_t rf_rsmc_parse_dts_config(struct device_node *dev_node, struct ps_core_s *cd)
{
	hwlog_info("%s: enter \n", __func__);
	int32_t value;
	int32_t ret;
	if ((dev_node == NULL) || (cd == NULL)) {
		hwlog_err("%s: input null\n", __func__);
		return -1;
	}

	value = of_get_named_gpio(dev_node, "sate_rsmc_gpio_mipi", 0);
	hwlog_info("%s, sate_rsmc_gpio_mipi:%d\n", __func__, value);
	cd->sate_rsmc_gpio_mipi = value;

	ret = rf_rsmc_parse_dts_config_other(dev_node, cd);
	if (ret != 0) {
		hwlog_err("%s:rf_rsmc_parse_dts_config_other fail,ret = %d\n", __func__, ret);
		return ret;
	}

	cd->rsmc_ufs_lp_mode_support = of_property_read_bool(dev_node, "sate_rsmc_ufs_lp_mode");
	hwlog_info("%s, sate_rsmc_ufs_lp_mode :%d\n", __func__, cd->rsmc_ufs_lp_mode_support);

	ret = of_property_read_u32(dev_node, "sate_rsmc_i2c_index",
		(unsigned int *)(&cd->sate_rsmc_i2c_index));
	hwlog_info("%s, sate_rsmc_i2c_index:%d\n", __func__, cd->sate_rsmc_i2c_index);
	if (ret != 0) {
		cd->sate_rsmc_i2c_index = -1;
		hwlog_info("%s: failed to get sate_rsmc_i2c_index,err:%d\n", __func__, ret);
	}

	ret = of_property_read_u32(dev_node, "sate_rsmc_i2c_on_volt",
		(unsigned int *)(&cd->sate_rsmc_i2c_on_volt));
	hwlog_info("%s, sate_rsmc_i2c_on_volt:%d\n", __func__, cd->sate_rsmc_i2c_on_volt);
	if (ret != 0) {
		cd->sate_rsmc_i2c_on_volt = -1;
		hwlog_info("%s: failed to get sate_rsmc_i2c_on_volt,err:%d\n", __func__, ret);
	}

	ret = of_property_read_u32(dev_node, "sate_rsmc_i2c_off_volt",
		(unsigned int *)(&cd->sate_rsmc_i2c_off_volt));
	hwlog_info("%s, sate_rsmc_i2c_off_volt:%d\n", __func__, cd->sate_rsmc_i2c_off_volt);
	if (ret != 0) {
		cd->sate_rsmc_i2c_off_volt = -1;
		hwlog_info("%s: failed to get sate_rsmc_i2c_off_volt,err:%d\n", __func__, ret);
	}

	ret = of_property_read_u32(dev_node, "sate_rsmc_pwr_ldo_vol",
		(unsigned int *)(&cd->sate_rsmc_pwr_ldo_vol));
	hwlog_info("%s, sate_rsmc_pwr_ldo_vol:%d\n", __func__, cd->sate_rsmc_pwr_ldo_vol);
	if (ret != 0) {
		cd->sate_rsmc_pwr_ldo_vol = -1;
		hwlog_info("%s: failed to get sate_rsmc_pwr_ldo_vol,err:%d\n", __func__, ret);
	}
	return 0;
}

static int32_t rf_tts_parse_dts_config(struct device_node *dev_node, struct ps_core_s *cd)
{
	hwlog_info("%s: enter \n", __func__);
	int32_t value;
	int32_t ret;
	if ((dev_node == NULL) || (cd == NULL)) {
		hwlog_err("%s: input null\n", __func__);
		return -1;
	}

	value = of_get_named_gpio(dev_node, "sate_tts_i2c_pwr_sw", 0);
	hwlog_info("%s, sate_tts_i2c_pwr_sw:%d\n", __func__, value);
	cd->sate_tts_i2c_pwr_sw = value;

	value = of_get_named_gpio(dev_node, "sate_tts_rf_bp_bst_en", 0);
	hwlog_info("%s, sate_tts_rf_bp_bst_en:%d\n", __func__, value);
	cd->sate_tts_rf_bp_bst_en = value;

	ret = of_property_read_u32(dev_node, "sate_tts_i2c_index",
		(unsigned int *)(&cd->sate_tts_i2c_index));
	hwlog_info("%s, sate_tts_i2c_index:%d\n", __func__, cd->sate_tts_i2c_index);
	if (ret != 0) {
		cd->sate_tts_i2c_index = -1;
		hwlog_info("%s: failed to get sate_tts_i2c_index,err:%d\n", __func__, ret);
	}

	ret = of_property_read_u32(dev_node, "sate_tts_i2c_on_volt",
		(unsigned int *)(&cd->sate_tts_i2c_on_volt));
	hwlog_info("%s, sate_tts_i2c_on_volt:%d\n", __func__, cd->sate_tts_i2c_on_volt);
	if (ret != 0) {
		cd->sate_tts_i2c_on_volt = -1;
		hwlog_info("%s: failed to get sate_tts_i2c_on_volt,err:%d\n", __func__, ret);
	}

	ret = of_property_read_u32(dev_node, "sate_tts_i2c_off_volt",
		(unsigned int *)(&cd->sate_tts_i2c_off_volt));
	hwlog_info("%s, sate_tts_i2c_off_volt:%d\n", __func__, cd->sate_tts_i2c_off_volt);
	if (ret != 0) {
		cd->sate_tts_i2c_off_volt = -1;
		hwlog_info("%s: failed to get sate_tts_i2c_off_volt,err:%d\n", __func__, ret);
	}

	ret = of_property_read_u32(dev_node, "sate_tts_pwr_ldo_vol",
		(unsigned int *)(&cd->sate_tts_pwr_ldo_vol));
	hwlog_info("%s, sate_tts_pwr_ldo_vol:%d\n", __func__, cd->sate_tts_pwr_ldo_vol);
	if (ret != 0) {
		cd->sate_tts_pwr_ldo_vol = -1;
		hwlog_info("%s: failed to get sate_tts_pwr_ldo_vol,err:%d\n", __func__, ret);
	}
	return 0;
}

int32_t rf_sateliete_parse_dts_config(struct device_node *dev_node, struct ps_core_s *cd)
{
	hwlog_info("%s: enter \n", __func__);
	int32_t ret;
	if ((dev_node == NULL) || (cd == NULL)) {
		hwlog_err("%s: input null\n", __func__);
		return -1;
	}

	ret = rf_rsmc_parse_dts_config(dev_node, cd);
	if (ret != 0) {
		hwlog_err("%s: parse rsmc fail\n", __func__);
		return -1;
	}

	ret = rf_tts_parse_dts_config(dev_node, cd);
	if (ret != 0) {
		hwlog_err("%s: parse tts fail\n", __func__);
		return -1;
	}
	ret = of_property_read_u32(dev_node, "hisi,rf_adc_channel_1",
		(unsigned int *)(&cd->rf_adc_channel_1));
	if (ret != 0) {
		cd->rf_adc_channel_1 = -1;
		hwlog_info("%s: failed to get rf_adc_channel_1,err:%d\n", __func__, ret);
	}

	ret = of_property_read_u32(dev_node, "hisi,rf_adc_channel_2",
		(unsigned int *)(&cd->rf_adc_channel_2));
	if (ret != 0) {
		cd->rf_adc_channel_2 = -1;
		hwlog_info("%s: failed to get rf_adc_channel_2,err:%d\n", __func__, ret);
	}

	return 0;
}

static int32_t rf_satelite_config_init_other(struct ps_core_s *cd)
{
	int32_t rc;

	if (cd == NULL) {
		hwlog_info("%s: input null\n", __func__);
		return -1;
	}

	rc = rf_init_gpio(cd->sate_tts_i2c_pwr_sw, "sate_tts_i2c_pwr_sw", GPIO_LOW, GPIO_DIRECTION_OUTPUT);
	if (rc != 0) {
		hwlog_err("%s: init gpio %d failed %d\n", __func__, cd->sate_tts_i2c_pwr_sw, rc);
		return rc;
	}

	rc = rf_init_gpio(cd->sate_uefi_oled_en, "sate_uefi_oled_en", GPIO_LOW, GPIO_DIRECTION_OUTPUT);
	if (rc != 0) {
		hwlog_err("%s: init gpio %d failed %d\n", __func__, cd->sate_uefi_oled_en, rc);
		return rc;
	}

	rc = rf_init_gpio(cd->sate_rsmc_bbst_gpio_en, "sate_rsmc_bbst_gpio_en", GPIO_HIGH, GPIO_DIRECTION_OUTPUT);
	if (rc != 0) {
		hwlog_err("%s: init gpio %d failed %d\n", __func__, cd->sate_rsmc_bbst_gpio_en, rc);
		return rc;
	}

	rc = rf_init_gpio(cd->sate_rsmc_wifi_fem_en, "sate_rsmc_wifi_fem_en", GPIO_LOW, GPIO_DIRECTION_OUTPUT);
	if (rc != 0) {
		hwlog_err("%s: init gpio %d failed %d\n", __func__, cd->sate_rsmc_wifi_fem_en, rc);
		return rc;
	}

	rc = rf_init_gpio(cd->sate_rsmc_pwr_en, "sate_rsmc_pwr_en", GPIO_HIGH, GPIO_DIRECTION_OUTPUT);
	if (rc != 0) {
		hwlog_err("%s: init gpio %d failed %d\n", __func__, cd->sate_rsmc_pwr_en, rc);
		return rc;
	}

	rc = rf_init_gpio(cd->sate_tts_rsmc_sw, "sate_tts_rsmc_sw", GPIO_LOW, GPIO_DIRECTION_OUTPUT);
	if (rc != 0) {
		hwlog_err("%s: init gpio %d failed %d\n", __func__, cd->sate_tts_rsmc_sw, rc);
		return rc;
	}

	rc = rf_init_gpio(cd->sate_tts_rf_bp_bst_en, "sate_tts_rf_bp_bst_en", GPIO_HIGH, GPIO_DIRECTION_OUTPUT);
	if (rc != 0) {
		hwlog_err("%s: init gpio %d failed %d\n", __func__, cd->sate_tts_rf_bp_bst_en, rc);
		return rc;
	}

	return 0;
}

int32_t rf_satelite_config_init(struct ps_core_s *cd)
{
	int32_t rc;

	if (cd == NULL) {
		hwlog_info("%s: input null\n", __func__);
		return -1;
	}

	rc = rf_init_gpio(cd->sate_rsmc_gpio_mipi, "sate_rsmc_gpio_mipi", GPIO_LOW, GPIO_DIRECTION_OUTPUT);
	if (rc != 0) {
		hwlog_err("%s: init gpio %d failed %d\n",
			__func__, cd->sate_rsmc_gpio_mipi, rc);
		return rc;
	}

	rc = rf_satelite_config_init_other(cd);
	if (rc != 0) {
		hwlog_err("%s:rf_satelite_config_init_other fail,rc = %d\n", __func__, rc);
		return rc;
	}

	return 0;
}

int32_t get_hkadc_value(unsigned int cmd, int channel)
{
	int32_t value;
	value = lpm_adc_get_value(channel);
	hwlog_info("%s: channel %d, value: %d\n", __func__, channel, value);
	if (value < 0)
		hwlog_err("%s: get adc value fail\n", __func__);

	return value;
}

static int adcin_data_remap(int adcin_value)
{
	int retval;

	if (adcin_value < HKADC_DATA_GRADE0)
		retval = BOARDID_UNKNOWN;
	else if (adcin_value <= HKADC_DATA_GRADE1)
		retval = BOARDID_VALUE0;
	else if (adcin_value <= HKADC_DATA_GRADE2)
		retval = BOARDID_VALUE1;
	else if (adcin_value <= HKADC_DATA_GRADE3)
		retval = BOARDID_VALUE2;
	else if (adcin_value <= HKADC_DATA_GRADE4)
		retval = BOARDID_VALUE3;
	else if (adcin_value <= HKADC_DATA_GRADE5)
		retval = BOARDID_VALUE4;
	else if (adcin_value <= HKADC_DATA_GRADE6)
		retval = BOARDID_VALUE5;
	else if (adcin_value <= HKADC_DATA_GRADE7)
		retval = BOARDID_VALUE6;
	else if (adcin_value <= HKADC_DATA_GRADE8)
		retval = BOARDID_VALUE7;
	else if (adcin_value <= HKADC_DATA_GRADE9)
		retval = BOARDID_VALUE8;
	else if (adcin_value <= HKADC_DATA_GRADE10)
		retval = BOARDID_VALUE9;
	else
		retval = BOARDID_UNKNOWN;

	return retval;
}

static int get_adc_value(int channel)
{
	int adcin;
	int adcin_remap = 0;
	hwlog_info("adcin:%d\n", adcin);
	adcin = lpm_adc_get_value(channel);
	hwlog_info("adcin:%d\n", adcin);
	adcin_remap = adcin_data_remap(adcin);
	if (adcin_remap == BOARDID_UNKNOWN) {
		hwlog_err("[BDID]hkadc data error!\n");
		return -1;
	}
	return adcin_remap;
}

int32_t get_rf_boardid(struct ps_core_s *cd)
{
	hwlog_info("%s :enter\n", __func__);

	int32_t board_id;
	int bid1;
	int bid2;
	int i = 0;
	if (cd->rf_adc_channel_1 < 0 || cd->rf_adc_channel_2 < 0) {
		hwlog_err("%s : parma invaild!\n", __func__);
		return -1;
	}
	bid1 = get_adc_value(cd->rf_adc_channel_1);
	bid2 = get_adc_value(cd->rf_adc_channel_2);
	if (bid1 == -1 || bid2 == -1) {
		hwlog_err("get boardid fail!\n");
		return -1;
	}

	board_id = bid1 * 10 + bid2;
	hwlog_info("[BDID/ASIC]boardid: %d <%d.%d>\n", board_id, bid1, bid2);

	return board_id;
}