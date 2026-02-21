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

#include "huawei_thp.h"
#include <linux/of_gpio.h>
#include <linux/of.h>

static uint32_t tp_parse_property_u32(struct device_node *dn, char *property)
{
	uint32_t temp = 0;
	if (of_property_read_u32(dn, property, &temp)) {
		hwlog_err("%s:read %s fail\n", __func__, property);
	} else {
		hwlog_info("%s:read %s = %d.\n", __func__, property, temp);
	}
	return temp;
}

#define POWER_CONFIG_NAME_MAX 20
static void thp_paser_pmic_power(struct device_node *thp_node,
	struct thp_core_data *cd, int power_id)
{
	const char *power_name = NULL;
	char config_name[POWER_CONFIG_NAME_MAX] = {0};
	struct thp_power_supply *power = NULL;
	int rc;

	power_name = thp_power_id2name(power_id);
	power = &cd->thp_powers[power_id];
	(void)snprintf(config_name, (POWER_CONFIG_NAME_MAX - 1),
		"%s-value", power_name);
	rc = of_property_read_u32(thp_node, config_name,
		&power->pmic_power.value);
	if (rc)
		thp_log_err("%s:failed to get %s\n",
			__func__, config_name);
	(void)snprintf(config_name, (POWER_CONFIG_NAME_MAX - 1),
		"%s-ldo-num", power_name);
	rc = of_property_read_u32(thp_node, config_name,
		&power->pmic_power.ldo_num);
	if (rc)
		thp_log_err("%s:failed to get %s\n",
			__func__, config_name);
	(void)snprintf(config_name, (POWER_CONFIG_NAME_MAX - 1),
		"%s-pmic-num", power_name);
	rc = of_property_read_u32(thp_node, config_name,
		&power->pmic_power.pmic_num);
	if (rc)
		thp_log_err("%s:failed to get %s\n",
			__func__, config_name);
	thp_log_info("%s: to get %d, %d,%d\n", __func__,
		power->pmic_power.ldo_num,
		power->pmic_power.pmic_num,
		power->pmic_power.value);
}

int thp_parse_one_power(struct device_node *thp_node,
	struct thp_core_data *cd, int power_id)
{
	const char *power_name = NULL;
	char config_name[POWER_CONFIG_NAME_MAX] = {0};
	struct thp_power_supply *power = NULL;
	int rc;

	power_name = thp_power_id2name(power_id);
	power = &cd->thp_powers[power_id];

	rc = snprintf(config_name, POWER_CONFIG_NAME_MAX - 1,
		"%s-type", power_name);

	thp_log_info("%s:parse power: %s\n", __func__, config_name);

	rc = of_property_read_u32(thp_node, config_name, &power->type);
	if (rc || power->type == THP_POWER_UNUSED) {
		thp_log_info("%s: power %s type not config or 0, unused\n",
			__func__, config_name);
		return 0;
	}

	switch (power->type) {
	case THP_POWER_GPIO:
		snprintf(config_name, POWER_CONFIG_NAME_MAX - 1,
			"%s-gpio", power_name);
		power->gpio = of_get_named_gpio(thp_node, config_name, 0);
		if (!gpio_is_valid(power->gpio)) {
			thp_log_err("%s:failed to get %s\n",
				__func__, config_name);
			return -ENODEV;
		}
		break;
	case THP_POWER_LDO:
		snprintf(config_name, POWER_CONFIG_NAME_MAX - 1,
			"%s-value", power_name);
		rc = of_property_read_u32(thp_node, config_name,
				&power->ldo_value);
		if (rc) {
			thp_log_err("%s:failed to get %s\n",
				__func__, config_name);
			return rc;
		}
		break;
	case THP_POWER_PMIC:
		thp_paser_pmic_power(thp_node, cd, power_id);
		break;
	default:
		thp_log_err("%s: invaild power type %d", __func__, power->type);
		break;
	}

	return 0;
}
EXPORT_SYMBOL(thp_parse_one_power);

void thp_parse_power_on_sequece(struct device_node *thp_node,
	struct thp_timing_config *timing)
{
	timing->poweron_timing.first_pull_up = tp_parse_property_u32(thp_node, "power_first_pull_up");
	timing->poweron_timing.second_pull_up = tp_parse_property_u32(thp_node, "power_second_pull_up");
	timing->poweron_timing.third_pull_up = tp_parse_property_u32(thp_node, "power_third_pull_up");
	timing->poweron_timing.forth_pull_up = tp_parse_property_u32(thp_node, "power_forth_pull_up");
}

void thp_parse_power_on_delay_sequece(struct device_node *thp_node,
	struct thp_timing_config *timing)
{
	timing->poweron_timing.delay_after_first_pull_up = tp_parse_property_u32(thp_node, "delay_after_first_pull_up");
	timing->poweron_timing.delay_after_second_pull_up = tp_parse_property_u32(thp_node, "delay_after_second_pull_up");
	timing->poweron_timing.delay_after_third_pull_up = tp_parse_property_u32(thp_node, "delay_after_third_pull_up");
	timing->poweron_timing.delay_after_forth_pull_up = tp_parse_property_u32(thp_node, "delay_after_forth_pull_up");
}

void thp_parse_power_off_sequece(struct device_node *thp_node,
	struct thp_timing_config *timing)
{
	timing->poweroff_timing.first_pull_down = tp_parse_property_u32(thp_node, "power_first_pull_down");
	timing->poweroff_timing.second_pull_down = tp_parse_property_u32(thp_node, "power_second_pull_down");
	timing->poweroff_timing.third_pull_down = tp_parse_property_u32(thp_node, "power_third_pull_down");
	timing->poweroff_timing.forth_pull_down = tp_parse_property_u32(thp_node, "power_forth_pull_down");
}

void thp_parse_power_off_delay_sequece(struct device_node *thp_node,
	struct thp_timing_config *timing)
{
	timing->poweroff_timing.delay_after_first_pull_down = tp_parse_property_u32(thp_node, "delay_after_first_pull_down");
	timing->poweroff_timing.delay_after_second_pull_down = tp_parse_property_u32(thp_node, "delay_after_second_pull_down");
	timing->poweroff_timing.delay_after_third_pull_down = tp_parse_property_u32(thp_node, "delay_after_third_pull_down");
	timing->poweroff_timing.delay_after_forth_pull_down = tp_parse_property_u32(thp_node, "delay_after_forth_pull_down");
}

int thp_parse_power_timing(struct device_node *thp_node,
	struct thp_timing_config *timing)
{
	thp_parse_power_on_sequece(thp_node, timing);
	thp_parse_power_on_delay_sequece(thp_node, timing);
	thp_parse_power_off_sequece(thp_node, timing);
	thp_parse_power_off_delay_sequece(thp_node, timing);

	return 0;
}
EXPORT_SYMBOL(thp_parse_power_timing);

int thp_parse_power_config(struct device_node *thp_node,
	struct thp_core_data *cd)
{
	int rc;
	u32 value = 0;
	int i;

	cd->not_control_power = 0;
	rc = of_property_read_u32(thp_node, "not_control_power",
		&value);
	if (!rc) {
		cd->not_control_power = value;
		thp_log_info("%s: not_control_power: %u\n",
			__func__, value);
		return 0;
	}

	for (i = 0; i < THP_POWER_ID_MAX; i++) {
		rc = thp_parse_one_power(thp_node, cd, i);
		if (rc)
			return rc;
	}

	return 0;
}
EXPORT_SYMBOL(thp_parse_power_config);