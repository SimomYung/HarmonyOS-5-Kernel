/*
 * battery_state.c
 *
 * battery_state driver
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
 */

#include "battery_state.h"
#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/ioctl.h>
#include <linux/platform_device.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/poll.h>
#include <linux/delay.h>
#include <linux/compat.h>
#include <linux/of.h>
#include <linux/notifier.h>
#include <linux/workqueue.h>
#include <securec.h>
#include <chipset_common/hwpower/common_module/power_supply_interface.h>
#include <huawei_platform/hwpower/common_module/power_platform_macro.h>
#include <huawei_platform/log/hw_log.h>
#include "smartpakit/smartpakit.h"

#define HWLOG_TAG battery_state
#define MOD_NAME "battery_state"
#define UEVENT_REPORT_NAME "pa_battery_state"

#define BATTERY_DEFAULT_LOWER_LIMIT 5
#define BATTERY_DEFAULT_UPPER_LIMIT 8

HWLOG_REGIST();

static void adjust_current(struct work_struct *work)
{
	struct battery_state_priv *priv = container_of(work, struct battery_state_priv, adjust_work);
	int new_battery_state = 0;
	int ret;

	ret = power_supply_get_int_property_value(POWER_PLATFORM_BAT_PSY_NAME,
		POWER_SUPPLY_PROP_CAPACITY, &new_battery_state);
	if (ret) {
		hwlog_err("get new battery state failed");
		return;
	}
	if (priv->battery_state == new_battery_state) {
		return;
	}

	hwlog_info("battery state change to %d from %d", new_battery_state, priv->battery_state);
	audio_misc_report_uevent(UEVENT_REPORT_NAME, "CHANGE");
	if (priv->battery_state == 0 && new_battery_state <= priv->lower_limit) {
		smartpakit_i2c_write_bgc_reg(true, true);
		hwlog_info("set smartpa current to 2A");
	} else if (priv->battery_state > priv->lower_limit && new_battery_state <= priv->lower_limit) {
		smartpakit_i2c_write_bgc_reg(true, true);
		hwlog_info("set smartpa current to 2A");
	} else if (priv->battery_state < priv->upper_limit && new_battery_state >= priv->upper_limit) {
		smartpakit_i2c_write_bgc_reg(false, true);
		hwlog_info("set smartpa current to 4A");
	}
	priv->battery_state = new_battery_state;
}

static int battery_state_notifier_call(struct notifier_block *nb,
	unsigned long event, void *data)
{
	struct battery_state_priv *priv = container_of(nb, struct battery_state_priv, battery_state_notifier);
	struct power_supply *psy = data;

	if (strcmp(psy->desc->name, POWER_PLATFORM_BAT_PSY_NAME)) {
		hwlog_info("%s is not target, ignore", POWER_PLATFORM_BAT_PSY_NAME);
		return NOTIFY_OK;
	}
	schedule_work(&priv->adjust_work);
	return NOTIFY_OK;
}

void force_adjust_current(void)
{
	struct battery_state_priv *priv = audio_misc_module_get_priv(MOD_NAME);
	int ret;

	if (priv == NULL) {
		hwlog_err("get priv failed");
		return;
	}
	if (priv->battery_state == 0) {
		ret = power_supply_get_int_property_value(POWER_PLATFORM_BAT_PSY_NAME,
			POWER_SUPPLY_PROP_CAPACITY, &(priv->battery_state));
		if (ret) {
			hwlog_err("get new battery state failed");
			return;
		}
		hwlog_info("cur battery state is %d", priv->battery_state);
	}
	if (priv->battery_state <= priv->lower_limit) {
		smartpakit_i2c_write_bgc_reg(true, false);
		hwlog_info("force set smartpa current to 2A");
	} else {
		smartpakit_i2c_write_bgc_reg(false, false);
		hwlog_info("force set smartpa current to 4A");
	}
}

static void battery_state_remove(void *priv)
{
	struct battery_state_priv *vs_priv = (struct battery_state_priv *)priv;

	power_supply_unreg_notifier(&vs_priv->battery_state_notifier);
	kfree(vs_priv);
	vs_priv = NULL;
}

static const struct audio_misc_module_ops battery_state_ops = {
	.remove = battery_state_remove,
};

int battery_state_probe(struct device *dev, struct device_node *np)
{
	int ret;
	struct battery_state_priv *priv = NULL;
	uint32_t tmp = 0;

	hwlog_info("%s: in", __func__);
	priv = kzalloc(sizeof(*priv), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;
	priv->battery_state = 0;
	priv->lower_limit = BATTERY_DEFAULT_LOWER_LIMIT;
	priv->upper_limit = BATTERY_DEFAULT_UPPER_LIMIT;
	if (!of_property_read_u32(np, "lower_limit", &tmp))
		priv->lower_limit = tmp;
	else
		hwlog_warn("lower_limit not config, use default value %d", BATTERY_DEFAULT_LOWER_LIMIT);
	if (!of_property_read_u32(np, "upper_limit", &tmp))
		priv->upper_limit = tmp;
	else
		hwlog_warn("upper_limit not config, use default value %d", BATTERY_DEFAULT_UPPER_LIMIT);
	INIT_WORK(&priv->adjust_work, adjust_current);
	priv->battery_state_notifier.notifier_call = battery_state_notifier_call;
	ret = power_supply_reg_notifier(&priv->battery_state_notifier);
	if (ret) {
		hwlog_err("%s: register battery failed", __func__);
		goto battery_err;
	}

	ret = audio_misc_module_constructor(MOD_NAME, priv, -1, &battery_state_ops);
	if (ret) {
		ret = -ENOMEM;
		goto mod_err;
	}
	return ret;

mod_err:
	power_supply_unreg_notifier(&priv->battery_state_notifier);
battery_err:
	kfree(priv);
	priv = NULL;
	return ret;
}