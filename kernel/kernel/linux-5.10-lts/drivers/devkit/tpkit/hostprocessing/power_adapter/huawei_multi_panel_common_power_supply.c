/*
 * Huawei Touchscreen Driver
 *
 * Copyright (c) 2024-2024 Huawei Technologies Co., Ltd.
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
 
#include "huawei_power_platform_resource.h"
#include "huawei_thp.h"
#include "huawei_tdev_power_func.h"
 
static struct thp_poweron_timing common_poweron_timing;
static struct thp_poweroff_timing common_poweroff_timing;
 
struct common_power_control g_thp_common_power[] = {
	{
		.power_on_flag = 0,
		.power_on = touch_driver_power_on_for_seq0,
		.power_off = touch_driver_power_off_for_seq0,
	},
};
 
struct common_power_control *get_common_power_control(u8 power_sequence)
{
	if (power_sequence >= ARRAY_SIZE(g_thp_common_power)) {
		thp_log_err("%s: invalid power_sequence, use default 0\n");
		return &g_thp_common_power[0];
	}
	return &g_thp_common_power[power_sequence];
}
EXPORT_SYMBOL(get_common_power_control);
 
int touch_driver_power_on_for_seq0(struct thp_device *tdev)
{
	struct thp_core_data *cd = NULL;
 
	thp_log_info("%s:called\n", __func__);
 
	if (!tdev) {
		thp_log_err("%s: invalid input\n", __func__);
		return -EINVAL;
	}
 
	cd = tdev->thp_core;
	if (!cd->not_support_cs_control)
		gpio_set_value(tdev->gpios->cs_gpio, GPIO_HIGH);
 
#ifdef CONFIG_HUAWEI_THP_QCOM
	if (cd->support_control_cs_off &&
		(!IS_ERR_OR_NULL(cd->qcom_pinctrl.cs_high))) {
		pinctrl_select_state(tdev->thp_core->pctrl,
			tdev->thp_core->qcom_pinctrl.cs_high);
		thp_log_info("%s: cs to high\n", __func__);
	}
#endif
 
	gpio_direction_output(tdev->gpios->rst_gpio, GPIO_LOW);
	if (!g_thp_common_power[cd->power_cfg.power_sequence].power_on_flag) {
		g_thp_common_power[cd->power_cfg.power_sequence].power_on_flag++;
		thp_common_power_on(tdev, &common_poweron_timing);
	}
	thp_log_info("%s pull up tp ic reset\n", __func__);
	gpio_direction_output(tdev->gpios->rst_gpio, GPIO_HIGH);
	thp_do_time_delay(tdev->timing_config.boot_reset_hi_delay_ms);
	return NO_ERR;
}
 
int touch_driver_power_off_for_seq0(struct thp_device *tdev)
{
	struct thp_core_data *cd = NULL;
 
	thp_log_info("%s:called\n", __func__);
	if (!tdev) {
		thp_log_err("%s: invalid input\n", __func__);
		return -EINVAL;
	}
	cd = tdev->thp_core;
 
	gpio_set_value(tdev->gpios->rst_gpio, GPIO_LOW);
	thp_do_time_delay(tdev->timing_config.suspend_reset_after_delay_ms);
	if (g_thp_common_power[cd->power_cfg.power_sequence].power_on_flag) {
		g_thp_common_power[cd->power_cfg.power_sequence].power_on_flag--;
		/* iovdd delay 0ms, vcc delay 0ms */
		thp_common_power_off(tdev, &common_poweroff_timing);
	}
	if (!cd->not_support_cs_control)
		gpio_set_value(tdev->gpios->cs_gpio, GPIO_LOW);
 
#ifdef CONFIG_HUAWEI_THP_QCOM
	if (cd->support_control_cs_off &&
		(!IS_ERR_OR_NULL(cd->qcom_pinctrl.cs_low))) {
		pinctrl_select_state(tdev->thp_core->pctrl,
			tdev->thp_core->qcom_pinctrl.cs_low);
		thp_log_info("%s: cs to low\n", __func__);
	}
#endif
	return NO_ERR;
}
 
u32 thp_get_panel_for_common_power_ctrl(struct thp_core_data *cd)
{
	u32 i;
	u32 panel_index = 0;
 
	for (i = 0; i <= cd->dual_thp; i++) {
		if (thp_get_core_data(i)->power_cfg.common_power_flag)
			panel_index = i;
	}
	return panel_index;
}

void thp_common_poweron_supply_ctrl(struct thp_poweron_timing *poweron_timing,
	u32 panel_id)
{
	int ret;
 
	ret = thp_power_supply_ctrl(poweron_timing->first_pull_up, THP_POWER_ON,
		poweron_timing->delay_after_first_pull_up, panel_id);
	if (ret)
		thp_log_err("%s:op IOVDD to status %d fail\n", __func__, THP_POWER_ON);
	ret = thp_power_supply_ctrl(poweron_timing->second_pull_up, THP_POWER_ON,
		poweron_timing->delay_after_second_pull_up, panel_id);
	if (ret)
		thp_log_err("%s:op VCC to status %d fail\n", __func__, THP_POWER_ON);
	if (poweron_timing->third_pull_up && poweron_timing->forth_pull_up) {
		ret = thp_power_supply_ctrl(poweron_timing->third_pull_up, THP_POWER_ON,
			poweron_timing->delay_after_third_pull_up, panel_id);
		if (ret)
			thp_log_err("%s:op IOVDD_S to status %d fail\n", __func__, THP_POWER_ON);
		ret = thp_power_supply_ctrl(poweron_timing->forth_pull_up, THP_POWER_ON,
			poweron_timing->delay_after_forth_pull_up, panel_id);
		if (ret)
			thp_log_err("%s:op VCC_S to status %d fail\n", __func__, THP_POWER_ON);
	}
	return;
}

void thp_common_poweroff_supply_ctrl(struct thp_poweroff_timing *poweroff_timing,
	u32 panel_id)
{
	int ret;
 
	ret = thp_power_supply_ctrl(poweroff_timing->first_pull_down, THP_POWER_OFF,
		poweroff_timing->delay_after_first_pull_down, panel_id);
	if (ret)
		thp_log_err("%s:op IOVDD to status %d fail\n", __func__, THP_POWER_OFF);
	ret = thp_power_supply_ctrl(poweroff_timing->second_pull_down, THP_POWER_OFF,
		poweroff_timing->delay_after_second_pull_down, panel_id);
	if (ret)
		thp_log_err("%s:op VCC to status %d fail\n", __func__, THP_POWER_OFF);
	if (poweroff_timing->third_pull_down && poweroff_timing->forth_pull_down) {
		ret = thp_power_supply_ctrl(poweroff_timing->third_pull_down, THP_POWER_ON,
			poweroff_timing->delay_after_third_pull_down, panel_id);
		if (ret)
			thp_log_err("%s:op IOVDD_S to status %d fail\n", __func__, THP_POWER_ON);
		ret = thp_power_supply_ctrl(poweroff_timing->forth_pull_down, THP_POWER_ON,
			poweroff_timing->delay_after_forth_pull_down, panel_id);
		if (ret)
			thp_log_err("%s:op VCC_S to status %d fail\n", __func__, THP_POWER_ON);
	}
	return;
}

int thp_common_power_on(struct thp_device *tdev, struct thp_poweron_timing *g_poweron_timing)
{
	struct thp_core_data *cd = NULL;
	u32 panel_index;
 
	thp_log_info("%s:called\n", __func__);
 
	if (!tdev) {
		thp_log_err("%s: invalid input\n", __func__);
		return -EINVAL;
	}
 
	cd = tdev->thp_core;
	panel_index = thp_get_panel_for_common_power_ctrl(cd);
 
	thp_common_poweron_supply_ctrl(g_poweron_timing, panel_index);
 
	return NO_ERR;
}

int thp_common_power_off(struct thp_device *tdev, struct thp_poweroff_timing *g_poweroff_timing)
{
	struct thp_core_data *cd = NULL;
	u32 panel_index;
 
	thp_log_info("%s:called\n", __func__);
 
	if (!tdev) {
		thp_log_err("%s: invalid input\n", __func__);
		return -EINVAL;
	}
 
	cd = tdev->thp_core;
	panel_index = thp_get_panel_for_common_power_ctrl(cd);
 
	thp_common_poweroff_supply_ctrl(g_poweroff_timing, panel_index);

	return NO_ERR;
}

int thp_init_power_gpio(struct thp_device *tdev)
{
	struct thp_core_data *cd = tdev->thp_core;
	static int flag = 0;

	cd->power_cfg.common_power_flag = false;
	if (flag) {
		thp_log_info("%s: power is inited\n", __func__);
		return -EINVAL;
	}
	flag++;
	cd->power_cfg.common_power_flag = true;
	thp_power_supply_get(THP_VCC, cd->panel_id);
	thp_power_supply_get(THP_IOVDD, cd->panel_id);
	thp_power_supply_get(THP_VCC_S, cd->panel_id);
	thp_power_supply_get(THP_IOVDD_S, cd->panel_id);
	return NO_ERR;
}

int common_power_init(struct thp_device *tdev)
{
	int ret;

	thp_log_info("%s: called\n", __func__);

	thp_power_lock();
	ret = thp_init_power_gpio(tdev);
	thp_power_unlock();
	return ret;
}

int common_power_release(struct thp_device *tdev)
{
	return NO_ERR;
}

int multi_panel_use_common_power_on(struct thp_device *tdev,
	struct thp_poweron_timing *g_poweron_timing)
{
	if (!g_poweron_timing || !tdev) {
		thp_log_err("%s:null ptr happen, please check parameter\n", __func__);
		return -ENOMEM;
	}
 
	common_poweron_timing = *g_poweron_timing;
	thp_power_lock();
	g_thp_common_power[tdev->thp_core->power_cfg.power_sequence].power_on(tdev);
	thp_power_unlock();
	return NO_ERR;
}
EXPORT_SYMBOL(multi_panel_use_common_power_on);
 
int multi_panel_use_common_power_off(struct thp_device *tdev,
	struct thp_poweroff_timing *g_poweroff_timing)
{
	if (!g_poweroff_timing || !tdev) {
		thp_log_err("%s:null ptr happen, please check parameter\n", __func__);
		return -ENOMEM;
	}
 
	common_poweroff_timing = *g_poweroff_timing;
	thp_power_lock();
	g_thp_common_power[tdev->thp_core->power_cfg.power_sequence].power_off(tdev);
	thp_power_unlock();
	return NO_ERR;
}
EXPORT_SYMBOL(multi_panel_use_common_power_off);
