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

struct mutex thp_power_mutex;
struct mutex thp_power_adapter_mutex;

void thp_power_lock(void)
{
	thp_log_debug("%s: get power lock", __func__);
	mutex_lock(&thp_power_mutex);
}
EXPORT_SYMBOL(thp_power_lock);

void thp_power_unlock(void)
{
	thp_log_debug("%s: power unlock", __func__);
	mutex_unlock(&thp_power_mutex);
}
EXPORT_SYMBOL(thp_power_unlock);

const char *thp_power_id2name(enum thp_power_id id)
{
	return !is_invaild_power_id(id) ? thp_power_name[id] : 0;
}
EXPORT_SYMBOL(thp_power_id2name);

int thp_power_supply_ctrl(enum thp_power_id power_id,
	int status, unsigned int delay_ms, u32 panel_id)
{
	struct thp_core_data *cd = thp_get_core_data(panel_id);
	struct thp_power_supply *power = NULL;
	int rc = 0;

	if (is_invaild_power_id(power_id)) {
		thp_log_err("%s: invalid power id %d", __func__, power_id);
		return -EINVAL;
	}

	power = &cd->thp_powers[power_id];
	if (power->type == THP_POWER_UNUSED)
		goto exit;

	thp_log_info("%s:power %s %s\n", __func__,
		thp_power_id2name(power_id), status ? "on" : "off");

	if (!power->use_count) {
		thp_log_err("%s:regulator %s not gotten yet\n", __func__,
				thp_power_id2name(power_id));
		return -ENODEV;
	}
	switch (power->type) {
	case THP_POWER_LDO:
		if (IS_ERR_OR_NULL(power->regulator)) {
			thp_log_err("%s:fail to get %s\n", __func__,
				thp_power_id2name(power_id));
			return -ENODEV;
		}
		rc = status ? regulator_enable(power->regulator) :
			regulator_disable(power->regulator);
		break;
	case THP_POWER_GPIO:
		gpio_direction_output(power->gpio, status ? 1 : 0);
		break;

#if (!defined CONFIG_HUAWEI_THP_QCOM) && (!defined CONFIG_HUAWEI_THP_MTK)
	case THP_POWER_PMIC:
		tp_pmic_ldo_set.pmic_power_state = (status ? 1 : 0);
		rc = hw_pmic_power_cfg(TP_PMIC_REQ, &tp_pmic_ldo_set);
		if (rc)
			thp_log_err("%s:pmic %s failed, %d\n", __func__,
				thp_power_id2name(power_id), rc);
		break;
#endif

	default:
		thp_log_err("%s: invalid power type %d\n",
			__func__, power->type);
		break;
	}
exit:
	if (delay_ms)
		mdelay(delay_ms);
	return rc;
}
EXPORT_SYMBOL(thp_power_supply_ctrl);

int thp_power_supply_put(enum thp_power_id power_id, u32 panel_id)
{
	struct thp_core_data *cd = thp_get_core_data(panel_id);
	struct thp_power_supply *power = NULL;

	if (is_invaild_power_id(power_id)) {
		thp_log_err("%s: invalid power id %d", __func__, power_id);
		return -EINVAL;
	}

	power = &cd->thp_powers[power_id];
	if (power->type == THP_POWER_UNUSED)
		return 0;
	if ((--power->use_count) > 0)
		return 0;

	switch (power->type) {
	case THP_POWER_LDO:
		if (IS_ERR_OR_NULL(power->regulator)) {
			thp_log_err("%s:fail to get %s\n", __func__,
				thp_power_id2name(power_id));
			return -ENODEV;
		}
		regulator_put(power->regulator);
		break;
	case THP_POWER_GPIO:
		gpio_direction_output(power->gpio, 0);
		gpio_free(power->gpio);
		break;
	case THP_POWER_PMIC:
		thp_log_err("%s: power supply %d\n", __func__, power->type);
		break;

	default:
		thp_log_err("%s: invalid power type %d\n",
			__func__, power->type);
		break;
	}
	return 0;
}
EXPORT_SYMBOL(thp_power_supply_put);

int thp_ldo_power_supply_get(struct thp_core_data *cd, enum thp_power_id power_id, struct thp_power_supply *power)
{
	int ret;

	power->regulator = regulator_get(&cd->sdev->dev,
		thp_power_id2name(power_id));
	if (IS_ERR_OR_NULL(power->regulator)) {
		thp_log_err("%s:fail to get %s\n", __func__,
			thp_power_id2name(power_id));
		return -ENODEV;
	}

	ret = regulator_set_voltage(power->regulator, power->ldo_value,
			power->ldo_value);
	if (ret) {
		regulator_put(power->regulator);
		thp_log_err("%s:fail to set %s valude %d\n", __func__,
				thp_power_id2name(power_id),
				power->ldo_value);
	}
	return ret;
}

#if (!defined CONFIG_HUAWEI_THP_QCOM) && (!defined CONFIG_HUAWEI_THP_MTK)
void thp_pmic_power_supply_get(struct thp_power_supply *power)
{
	thp_log_info("%s call %d,%d,%d\n", __func__,
		power->pmic_power.pmic_num, power->pmic_power.ldo_num,
		power->pmic_power.value);
	tp_pmic_ldo_set.pmic_num = power->pmic_power.pmic_num;
	tp_pmic_ldo_set.pmic_power_type = power->pmic_power.ldo_num;
	tp_pmic_ldo_set.pmic_power_voltage = power->pmic_power.value;
}
#endif

int thp_power_supply_get(enum thp_power_id power_id, u32 panel_id)
{
	struct thp_core_data *cd = thp_get_core_data(panel_id);
	struct thp_power_supply *power = NULL;
	int ret;

	if (is_invaild_power_id(power_id)) {
		thp_log_err("%s: invalid power id %d", __func__, power_id);
		return -EINVAL;
	}

	power = &cd->thp_powers[power_id];
	if (power->type == THP_POWER_UNUSED)
		return 0;
	if (power->use_count) {
		power->use_count++;
		return 0;
	}
	switch (power->type) {
	case THP_POWER_LDO:
		ret = thp_ldo_power_supply_get(cd, power_id, power);
		if (ret)
			return ret;
		break;
	case THP_POWER_GPIO:
		ret = gpio_request(power->gpio, thp_power_id2name(power_id));
		if (ret) {
			thp_log_err("%s:request gpio %d for %s failed\n",
				__func__, power->gpio,
				thp_power_id2name(power_id));
			return ret;
		}
		break;

#if (!defined CONFIG_HUAWEI_THP_QCOM) && (!defined CONFIG_HUAWEI_THP_MTK)
	case THP_POWER_PMIC:
		thp_pmic_power_supply_get(power);
		break;
#endif

	default:
		thp_log_err("%s: invalid power type %d\n",
			__func__, power->type);
		break;
	}
	power->use_count++;
	return 0;
}
EXPORT_SYMBOL(thp_power_supply_get);

int thp_power_init(u32 panel_id)
{
	int ret;

	thp_log_info("%s: called\n", __func__);
	ret = thp_power_supply_get(THP_VCC, panel_id);
	if (ret)
		thp_log_err("%s:vcc fail\n", __func__);
	ret = thp_power_supply_get(THP_IOVDD, panel_id);
	if (ret)
		thp_log_err("%s:iovdd fail\n", __func__);
	return 0;
}
EXPORT_SYMBOL(thp_power_init);

int thp_power_release(u32 panel_id)
{
	int ret;

	thp_log_info("%s: called\n", __func__);

	ret = thp_power_supply_put(THP_VCC, panel_id);
	if (ret)
		thp_log_err("%s: fail to release vcc power\n", __func__);
	ret = thp_power_supply_put(THP_IOVDD, panel_id);
	if (ret)
		thp_log_err("%s: fail to release power\n", __func__);
	return ret;
}
EXPORT_SYMBOL(thp_power_release);

int thp_power_module_register(void)
{
	thp_log_info("%s: g_thp_power_module_init Enter\n", __func__);
	mutex_init(&thp_power_mutex);

	return 0;
}
EXPORT_SYMBOL(thp_power_module_register);

void thp_power_module_exit(void)
{
	thp_log_info("%s: g_thp_power_module_init Enter\n", __func__);
	mutex_destroy(&thp_power_mutex);
}
EXPORT_SYMBOL(thp_power_module_exit);