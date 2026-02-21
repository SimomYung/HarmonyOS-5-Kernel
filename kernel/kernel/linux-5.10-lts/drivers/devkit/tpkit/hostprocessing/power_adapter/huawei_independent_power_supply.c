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

int thp_poweron_ctrl(struct thp_poweron_timing *poweron_timing, struct thp_device *tdev, int status)
{
	int ret;

	thp_log_info("%s: poweron_timing->delay_after_first_pull_up %u\n", __func__,
		poweron_timing->delay_after_first_pull_up);
	ret = thp_power_supply_ctrl(poweron_timing->first_pull_up, status, 
		poweron_timing->delay_after_first_pull_up, tdev->thp_core->panel_id);
	if (ret)
		thp_log_err("%s:vcc fail\n", __func__);
	thp_log_info("%s: poweron_timing->delay_after_second_pull_up %u\n", __func__,
		poweron_timing->delay_after_second_pull_up);
	ret = thp_power_supply_ctrl(poweron_timing->second_pull_up, status,
		poweron_timing->delay_after_second_pull_up, tdev->thp_core->panel_id);
	if (ret)
		thp_log_err("%s:iovdd fail\n", __func__);
	return ret;
}

int thp_poweroff_ctrl(struct thp_poweroff_timing *poweroff_timing, struct thp_device *tdev, int status)
{
	int ret;

	thp_log_info("%s: poweroff_timing->delay_after_first_pull_down %u\n", __func__,
		poweroff_timing->delay_after_first_pull_down);
	ret = thp_power_supply_ctrl(poweroff_timing->first_pull_down, status,
		poweroff_timing->delay_after_first_pull_down, tdev->thp_core->panel_id);
	if (ret)
		thp_log_err("%s:vcc fail\n", __func__);
	thp_log_info("%s: poweroff_timing->delay_after_second_pull_down %u\n", __func__,
		poweroff_timing->delay_after_second_pull_down);
	ret = thp_power_supply_ctrl(poweroff_timing->second_pull_down, status,
		poweroff_timing->delay_after_second_pull_down, tdev->thp_core->panel_id);
	if (ret)
		thp_log_err("%s:iovdd fail\n", __func__);
	return ret;
}

#ifdef CONFIG_HUAWEI_THP_MTK
int thp_power_on_for_mtk(struct thp_poweroff_timing *poweroff_timing, struct thp_device *tdev)
{
	int ret;

	pinctrl_select_state(tdev->thp_core->pctrl, tdev->thp_core->mtk_pinctrl.reset_low);
	ret = thp_poweron_ctrl(poweron_timing, tdev, THP_POWER_ON);
	if (ret)
		thp_log_err("%s:power on fail\n", __func__);
	if (!cd->not_support_cs_control)
		pinctrl_select_state(tdev->thp_core->pctrl, tdev->thp_core->mtk_pinctrl.cs_high);
	pinctrl_select_state(tdev->thp_core->pctrl, tdev->thp_core->mtk_pinctrl.reset_high);
	thp_do_time_delay(tdev->timing_config.boot_reset_hi_delay_ms);
	return ret;
}

int power_off_for_mtk(struct thp_poweroff_timing *poweroff_timing, struct thp_device *tdev)
{
	int ret;
	struct thp_core_data *cd = thp_get_core_data(tdev->thp_core->panel_id);
	if (cd->support_control_cs_off) {
		pinctrl_select_state(tdev->thp_core->pctrl, tdev->thp_core->pins_idle);
		thp_log_info("%s:cs off\n", __func__);
	} else {
		pinctrl_select_state(tdev->thp_core->pctrl, tdev->thp_core->mtk_pinctrl.reset_low);
	}
	thp_do_time_delay(tdev->timing_config.suspend_reset_after_delay_ms);
	ret = thp_poweroff_ctrl(poweroff_timing, tdev, THP_POWER_OFF);
	if (ret)
		thp_log_err("%s:power ctrl fail\n", __func__);
	return ret;
}
#endif

#ifdef CONFIG_HUAWEI_THP_QCOM
int thp_power_on_for_qcom(struct thp_poweron_timing *poweron_timing, struct thp_device *tdev)
{
	int ret;

	gpio_direction_input(tdev->gpios->irq_gpio);
	gpio_direction_output(tdev->gpios->rst_gpio, GPIO_LOW);
	ret = thp_poweron_ctrl(poweron_timing, tdev, THP_POWER_ON);
	if (ret)
		thp_log_err("%s:power ctrl fail\n", __func__);
	mdelay(poweron_timing->delay_before_rst_pull_up);
	gpio_set_value(tdev->gpios->rst_gpio, GPIO_HIGH);
	if (poweron_timing->need_control_cs_before_poweron)
		gpio_set_value(tdev->gpios->cs_gpio, GPIO_HIGH);
	if (tdev->thp_core->support_control_cs_off && (!IS_ERR_OR_NULL(tdev->thp_core->qcom_pinctrl.cs_high))) {
		pinctrl_select_state(tdev->thp_core->pctrl, tdev->thp_core->qcom_pinctrl.cs_high);
		thp_log_info("%s: cs to high\n", __func__);
	}
	return ret;
}
#endif

int power_off_not_for_mtk(struct thp_poweroff_timing *poweroff_timing, struct thp_device *tdev)
{
	int ret;

	if (poweroff_timing->need_control_cs_before_poweroff)
		gpio_direction_output(tdev->gpios->cs_gpio, GPIO_LOW);
	if (poweroff_timing->delay_before_rst_pull_down)
		thp_do_time_delay(poweroff_timing->delay_before_rst_pull_down);
	gpio_direction_output(tdev->gpios->rst_gpio, GPIO_LOW);
	if (poweroff_timing->delay_after_rst_pull_down)
		thp_do_time_delay(poweroff_timing->delay_after_rst_pull_down);
	ret = thp_poweroff_ctrl(poweroff_timing, tdev, THP_POWER_OFF);
	if (ret)
		thp_log_err("%s:power ctrl fail\n", __func__);
	if (poweroff_timing->need_control_cs_after_poweroff)
		gpio_direction_output(tdev->gpios->cs_gpio, GPIO_LOW);
	return ret;
}

int thp_power_on(struct thp_poweron_timing *poweron_timing, struct thp_device *tdev)
{
	int ret = 0;

	thp_log_info("%s:called\n", __func__);

#if defined(CONFIG_HUAWEI_THP_MTK)
	thp_power_on_for_mtk(poweron_timing, tdev);
#elif defined(CONFIG_HUAWEI_THP_QCOM)
	thp_power_on_for_qcom(poweron_timing, tdev);
#else
	if (poweron_timing->need_control_cs_before_poweron)
		gpio_direction_output(tdev->gpios->cs_gpio, GPIO_HIGH);
	gpio_direction_output(tdev->gpios->rst_gpio, GPIO_LOW);
	if (poweron_timing->need_control_cs_after_rst)
		gpio_direction_output(tdev->gpios->cs_gpio, GPIO_LOW);
	ret = thp_poweron_ctrl(poweron_timing, tdev, THP_POWER_ON);
	if (poweron_timing->need_control_cs_after_poweron)
		gpio_set_value(tdev->gpios->cs_gpio, GPIO_HIGH);
	gpio_direction_output(tdev->gpios->rst_gpio, GPIO_HIGH);
	if (poweron_timing->delay_after_rst_pull_up)
		mdelay(poweron_timing->delay_after_rst_pull_up);
#endif

	return ret;
}
EXPORT_SYMBOL(thp_power_on);

int thp_power_off(struct thp_poweroff_timing *poweroff_timing, struct thp_device *tdev)
{
	int ret = 0;
	struct thp_core_data *cd = NULL;
	bool flag;

	flag = (tdev == NULL || tdev->thp_core == NULL);
	if (flag) {
		thp_log_err("%s: null ptr\n", __func__);
		return -EINVAL;
	}
	cd = thp_get_core_data(tdev->thp_core->panel_id);
	if (cd->power_cfg.power_supply_mode)
		return NO_ERR;
	thp_log_info("%s pull down tp ic reset\n", __func__);
#ifndef CONFIG_HUAWEI_THP_MTK
	ret = power_off_not_for_mtk(poweroff_timing, tdev);
#ifdef CONFIG_HUAWEI_THP_QCOM
	if (cd->support_control_cs_off && (!IS_ERR_OR_NULL(cd->qcom_pinctrl.cs_low))) {
		pinctrl_select_state(tdev->thp_core->pctrl, tdev->thp_core->qcom_pinctrl.cs_low);
		thp_log_info("%s: cs to low\n", __func__);
	}
#endif
#else
	ret = power_off_for_mtk(poweroff_timing, tdev);
#endif
	return ret;
}
EXPORT_SYMBOL(thp_power_off);

int independent_power_init(struct thp_device *tdev)
{
	int ret;

	ret = thp_power_supply_get(THP_VCC, tdev->thp_core->panel_id);
	if (ret)
		thp_log_err("%s:vcc fail\n", __func__);
	ret = thp_power_supply_get(THP_IOVDD, tdev->thp_core->panel_id);
	if (ret)
		thp_log_err("%s:iovdd fail\n", __func__);
	return ret;
}

int independent_power_release(struct thp_device *tdev)
{
	thp_log_info("%s: called\n", __func__);
	thp_power_supply_put(THP_VCC, tdev->thp_core->panel_id);
	thp_power_supply_put(THP_IOVDD, tdev->thp_core->panel_id);
	return NO_ERR;
}

int use_independent_power_on(struct thp_device *tdev, struct thp_poweron_timing* g_poweron_timing)
{
	int ret;

	thp_log_info("%s: called\n", __func__);
	if (!g_poweron_timing || !tdev) {
		thp_log_err("%s:null ptr happen, please check parameter\n", __func__);
		return -ENOMEM;
	}
	/* ic power on */
	ret = thp_power_on(g_poweron_timing, tdev);
	return ret;
}
EXPORT_SYMBOL(use_independent_power_on);

int use_independent_power_off(struct thp_device *tdev, struct thp_poweroff_timing* g_poweroff_timing)
{
	int ret;

	thp_log_info("%s: called\n", __func__);
	if (!g_poweroff_timing || !tdev) {
		thp_log_err("%s:null ptr happen, please check parameter\n", __func__);
		return -ENOMEM;
	}
	/* ic power off */
	ret = thp_power_off(g_poweroff_timing, tdev);
	return ret;
}
EXPORT_SYMBOL(use_independent_power_off);
