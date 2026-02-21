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

#include "huawei_power_adapter.h"
#include "huawei_tdev_power_func.h"
#include "huawei_power_platform_resource.h"

struct thp_power_adapter *thp_power_adapt(enum thp_power_mode power_mode)
{
	struct thp_power_adapter *power_adapter = kzalloc(sizeof(struct thp_power_adapter), GFP_KERNEL);
	if (power_adapter == NULL) {
		thp_log_err("failed to kzalloc power_adapter\n");
		return NULL;
	}
	power_adapter->ops = kzalloc(sizeof(struct thp_power_ops), GFP_KERNEL);
	if (power_adapter->ops == NULL) {
		thp_power_adapter_release(power_adapter);
		thp_log_err("failed to kzalloc power_adapter->ops\n");
		return NULL;
	}

	switch (power_mode) {
	case USE_INDEPENDENT_POWER:
		thp_log_info("%s:adapt indepent power\n", __func__);
		power_adapter->ops->init = independent_power_init;
		power_adapter->ops->release = independent_power_release;
		power_adapter->ops->poweron = use_independent_power_on;
		power_adapter->ops->poweroff = use_independent_power_off;
		break;

	case MULTI_PANEL_USE_SINGEL_POWER:
		thp_log_info("%s:adapt common power\n", __func__);
		power_adapter->ops->init = common_power_init;
		power_adapter->ops->release = common_power_release;
		power_adapter->ops->poweron = multi_panel_use_common_power_on;
		power_adapter->ops->poweroff = multi_panel_use_common_power_off;
		break;

	case TP_USE_TDDI_POWER:
		thp_log_info("%s:adapt tddi power\n", __func__);
		power_adapter->ops->poweron = tp_use_tddi_power_on;
		power_adapter->ops->poweroff = tp_use_tddi_power_off;
		break;
	default:
		thp_log_err("%s:invalid value, please check dts config\n", __func__);
		thp_power_adapter_release(power_adapter);
		return NULL;
	}

	return power_adapter;
}

struct thp_power_adapter *power_adapter_init(int power_type)
{
	thp_log_info("%s:called\n", __func__);
	return thp_power_adapt(power_type);
}
EXPORT_SYMBOL(power_adapter_init);

void thp_power_adapter_release(struct thp_power_adapter *power_adapter)
{
	thp_log_info("%s:called\n", __func__);
	kfree(power_adapter->ops);
	power_adapter->ops = NULL;
	kfree(power_adapter);
	power_adapter = NULL;
	return;
}
EXPORT_SYMBOL(thp_power_adapter_release);
