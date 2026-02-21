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

int tp_use_tddi_power_on(struct thp_device *tdev, struct thp_poweron_timing *g_poweron_timing)
{
	thp_log_info("%s: called\n", __func__);
	if (!g_poweron_timing || !tdev) {
		thp_log_err("%s:null ptr happen, please check parameter\n", __func__);
		return -ENOMEM;
	}

	if (!tdev->thp_core->not_support_cs_control)
		gpio_set_value(tdev->gpios->cs_gpio, GPIO_HIGH);
#ifdef CONFIG_HUAWEI_THP_QCOM
	if (tdev->thp_core->support_control_cs_off &&
		(!IS_ERR_OR_NULL(tdev->thp_core->qcom_pinctrl.cs_high))) {
		pinctrl_select_state(tdev->thp_core->pctrl, tdev->thp_core->qcom_pinctrl.cs_high);
		thp_log_info("%s: cs to high\n", __func__);
	}
#endif
	gpio_set_value(tdev->gpios->rst_gpio, GPIO_HIGH);
	return NO_ERR;
}
EXPORT_SYMBOL(tp_use_tddi_power_on);

int tp_use_tddi_power_off(struct thp_device *tdev, struct thp_poweroff_timing *g_poweroff_timing)
{
	thp_log_info("%s: called\n", __func__);
	if (!g_poweroff_timing || !tdev) {
		thp_log_err("%s:null ptr happen, please check parameter\n", __func__);
		return -ENOMEM;
	}

	gpio_set_value(tdev->gpios->rst_gpio, GPIO_LOW);
	if (!tdev->thp_core->not_support_cs_control)
		gpio_set_value(tdev->gpios->cs_gpio, GPIO_LOW);
#ifdef CONFIG_HUAWEI_THP_QCOM
	if (tdev->thp_core->support_control_cs_off &&
		(!IS_ERR_OR_NULL(tdev->thp_core->qcom_pinctrl.cs_low))) {
		pinctrl_select_state(tdev->thp_core->pctrl, tdev->thp_core->qcom_pinctrl.cs_low);
		thp_log_info("%s: cs to low\n", __func__);
	}
#endif
	return NO_ERR;
}
EXPORT_SYMBOL(tp_use_tddi_power_off);
