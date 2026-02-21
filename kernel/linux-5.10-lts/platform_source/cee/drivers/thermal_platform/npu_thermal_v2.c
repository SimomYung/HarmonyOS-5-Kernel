/*
 * npu_thermal_v2.c
 *
 * npu thermal calculation module
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/devfreq_cooling.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/thermal.h>
#include <securec.h>
#include <platform_include/cee/linux/lpm_thermal.h>
#include "npu_thermal.h"

/* Local function to check if thermal zone matches cooling devices */
static int npu_thermal_match_cdev(struct thermal_cooling_device *cdev,
				  char *cdev_name)
{
	if (strlen(cdev->type) == 0)
		return -EINVAL;

	if (strncmp(cdev->type, cdev_name, sizeof(cdev->type) - 1) == 0)
		return 0;

	return -ENODEV;
}

/* Callback to bind cooling device to thermal zone */
static int npu_cdev_bind(struct thermal_zone_device *tz,
			 struct thermal_cooling_device *cdev)
{
	struct npu_thermal_zone *pzone = tz->devdata;
	struct nputh_data *param = pzone->param;
	unsigned long max_state;
	int ret = 0;

	cdev->ops->get_max_state(cdev, &max_state);

	if (npu_thermal_match_cdev(cdev, param->cdev_name) != 0)
		return 0;
#ifdef CONFIG_THERMAL_GOV_DYNAMIC_INTERVAL_CTRL
	ret = npu_thermal_creat_attrs(tz);
	if (ret != 0)
		pr_err("do not create dyn ctrl attrs group\n");
#endif
	ret = thermal_zone_bind_cooling_device(tz, 0, cdev, max_state, 0, THERMAL_WEIGHT_DEFAULT);
	if (ret != 0)
		dev_err(&cdev->device, "%s bind fail: %d!\n", cdev->type, ret);

	return ret;
}

/* Callback to unbind cooling device from thermal zone */
static int npu_cdev_unbind(struct thermal_zone_device *tz,
			   struct thermal_cooling_device *cdev)
{
	struct npu_thermal_zone *pzone = tz->devdata;
	struct nputh_data *param = pzone->param;
	int ret = -EINVAL;

	if (npu_thermal_match_cdev(cdev, param->cdev_name))
		return 0;

#ifdef CONFIG_THERMAL_GOV_DYNAMIC_INTERVAL_CTRL
	npu_thermal_destroy_attrs(tz);
#endif
	ret = thermal_zone_unbind_cooling_device(tz, 0, cdev);
	if (ret != 0)
		dev_err(&cdev->device, "%s unbind fail: %d!\n", cdev->type, ret);

	return ret;
}

/* Callback to get current temperature */
static int npu_thermal_get_temp(struct thermal_zone_device *tz, int *temp)
{
	int id, ret;

	id = ipa_get_tsensor_id("npu");
	if (id < 0)
		return -ENODEV;

	ret = ipa_get_sensor_value((unsigned int)id, temp);
	if (ret != 0)
		return -EINVAL;

	return 0;
}

#ifdef CONFIG_THERMAL_GOV_DYNAMIC_INTERVAL_CTRL
static int g_shell_temp = 0;
void notify_npu_shell_temp(char *type, int temp)
{
	if (type == NULL)
		return;

	if (!strncasecmp(type, IPA_SENSOR_SHELL, THERMAL_NAME_LENGTH))
		g_shell_temp = temp;
}

static void dynamic_update_ref_interval(struct nputh_data *param)
{
	int i, calib_value;

	if (g_shell_temp == 0 || !param->dyn_step_wise_enabled) {
		param->intervals = param->dyn_intervals[0];
		param->temp = param->ori_trip_temp;
		return;
	}

	calib_value = param->dyn_intervals[0][0] - param->ori_trip_temp;
	for (i = 0; i < param->dyn_ref_temp_num; i++) {
		if (g_shell_temp <= param->dyn_ref_temp[i]) {
			break;
		}
	}
	param->intervals = param->dyn_intervals[i];
	param->temp = param->intervals[0] - calib_value;
}
#endif /* CONFIG_THERMAL_GOV_DYNAMIC_INTERVAL_CTRL */

/* Callback to get temperature interval */
static int npu_thermal_get_interval(struct thermal_zone_device *tz,
				    int trip, int *interval)
{
	int i;
	struct npu_thermal_zone *pzone = tz->devdata;

#ifdef CONFIG_THERMAL_GOV_DYNAMIC_INTERVAL_CTRL
	dynamic_update_ref_interval(pzone->param);
#endif

	(void)trip;
	for (i = 0; i < pzone->param->num_intervals; i++) {
		if (tz->temperature < pzone->param->intervals[i])
			break;
	}
	*interval = i;

	return 0;
}

/* Callback to get trip point type */
static int npu_thermal_get_trip_type(struct thermal_zone_device *tz,
				     int trip, enum thermal_trip_type *type)
{
	struct npu_thermal_zone *pzone = tz->devdata;
	struct nputh_data *param = pzone->param;

	if (trip != 0)
		return -EINVAL;

	*type = param->type;

	return 0;
}

/* Callback to get trip point temperature */
static int npu_thermal_get_trip_temp(struct thermal_zone_device *tz,
				     int trip, int *temp)
{
	struct npu_thermal_zone *pzone = tz->devdata;
	struct nputh_data *param = pzone->param;

	if (trip != 0)
		return -EINVAL;

	*temp = param->temp;

	return 0;
}

/* Callback to set trip point temperature */
static int npu_thermal_set_trip_temp(struct thermal_zone_device *tz,
				     int trip, int temp)
{
	struct npu_thermal_zone *pzone = tz->devdata;
	struct nputh_data *param = pzone->param;

	if (trip != 0)
		return -EINVAL;

	param->temp = temp;
	dev_err(&tz->device, "new trip temp:%lu\n", param->temp);
	return 0;
}

/* Callback to get critical trip point temperature */
static int npu_thermal_get_crit_temp(struct thermal_zone_device *tz,
				     int *temp)
{
	struct npu_thermal_zone *pzone = tz->devdata;
	struct nputh_data *param = pzone->param;

	if (param->type == THERMAL_TRIP_CRITICAL) {
		*temp = param->temp;
		return 0;
	}

	return -EINVAL;
}

static struct thermal_zone_device_ops thdev_ops = {
	.bind = npu_cdev_bind,
	.unbind = npu_cdev_unbind,
	.get_temp = npu_thermal_get_temp,
	.get_interval = npu_thermal_get_interval,
	.get_trip_type = npu_thermal_get_trip_type,
	.get_trip_temp = npu_thermal_get_trip_temp,
	.get_crit_temp = npu_thermal_get_crit_temp,
	.set_trip_temp = npu_thermal_set_trip_temp,
};

static struct thermal_zone_params npu_thermal_params = {
	.governor_name = "interval_ctrl",
	.no_hwmon = true,
};

static void npu_thermal_work(struct work_struct *work)
{
	struct npu_thermal_zone *pzone = NULL;
	struct thermal_zone_device *tz = NULL;
	int polling_delay;

	pzone = container_of(work, struct npu_thermal_zone, thermal_work.work);
	tz = pzone->thermal_dev;

	if (tz->mode == THERMAL_DEVICE_DISABLED) {
		polling_delay = pzone->param->polling_delay;
		goto mod_work;
	}

	thermal_zone_device_update(pzone->thermal_dev, THERMAL_EVENT_UNSPECIFIED);
	if (tz->temperature > pzone->param->temp)
		polling_delay = pzone->param->passive_delay;
	else
		polling_delay = pzone->param->polling_delay;

mod_work:
	mod_delayed_work(system_freezable_power_efficient_wq,
			 &pzone->thermal_work, msecs_to_jiffies((unsigned int)polling_delay));
}

static int trip_point_type_parse(const char *tmp_str, size_t str_len,
				 struct nputh_data *param)
{
	if (strncmp(tmp_str, "active", str_len) == 0)
		param->type = THERMAL_TRIP_ACTIVE;
	else if (strncmp(tmp_str, "passive", str_len) == 0)
		param->type = THERMAL_TRIP_PASSIVE;
	else if (strncmp(tmp_str, "hot", str_len) == 0)
		param->type = THERMAL_TRIP_HOT;
	else if (strncmp(tmp_str, "critical", str_len) == 0)
		param->type = THERMAL_TRIP_CRITICAL;
	else
		return -EINVAL;

	return 0;
}

static int trips_cdev_name_parse(struct device_node *np, struct nputh_data *param)
{
	const char *tmp_str = NULL;
	int ret;

	if (of_property_read_string(np, "trip-cdev-name", &tmp_str))
		return -EINVAL;

	if (strlen(tmp_str) >= THERMAL_NAME_LENGTH)
		return -EINVAL;

	ret = strncpy_s(param->cdev_name, sizeof(param->cdev_name),
			tmp_str, sizeof(param->cdev_name) - 1);
	if (ret != EOK)
		return -EINVAL;
	return 0;
}

#ifdef CONFIG_THERMAL_GOV_DYNAMIC_INTERVAL_CTRL
static void dynamic_step_wise_data_destory(struct nputh_data *param)
{
	int i;

	if (param->dyn_ref_temp != NULL) {
		(void *)memset_s(param->dyn_ref_temp,
				 param->dyn_ref_temp_num,
				 0, param->dyn_ref_temp_num);
		kfree(param->dyn_ref_temp);
		param->dyn_ref_temp = NULL;
	}
	if (param->dyn_intervals != NULL) {
		for (i = 1; i < param->dyn_trips_num; i++) {
			(void *)memset_s(param->dyn_intervals[i],
					 param->num_intervals,
					 0, param->num_intervals);
			kfree(param->dyn_intervals[i]);
			param->dyn_intervals[i] = NULL;
		}
		(void *)memset_s(param->dyn_intervals,
				 param->dyn_trips_num,
				 0, param->dyn_trips_num);
		kfree(param->dyn_intervals);
	}
	param->dyn_trips_num = 0;
	param->dyn_ref_temp_num = 0;
	param->dyn_step_wise_enabled = false;
}

static int dynamic_trips_parse(struct device_node *np, struct nputh_data *param, unsigned int index)
{
	int err = 0;
	char dyn_trips_temp_path[THERMAL_ZONE_NAME_LENGTH] = {0};
	param->dyn_intervals[index] = kzalloc(sizeof(int) * param->num_intervals, GFP_KERNEL);
	if (param->dyn_intervals[index] == NULL) {
		pr_err("%s kzalloc dyn_intervals[%u] failed!\n", __func__, index);
		goto err_parse_trips;
	}
	(void *)memset_s(param->dyn_intervals[index],
			 param->num_intervals, 0, param->num_intervals);
	err = sprintf_s(dyn_trips_temp_path, THERMAL_ZONE_NAME_LENGTH, "%s%u",
			"thermal-interval", index);
	if (err < 0) {
		pr_err("%s compose dyn trips path failed!\n", __func__);
		goto err_parse_trips;
	}
	err = of_property_read_u32_array(np, dyn_trips_temp_path,
					 param->dyn_intervals[index], param->num_intervals);
	if (err != 0) {
		pr_err("%s parse %s failed!\n", __func__, dyn_trips_temp_path);
		goto err_parse_trips;
	}

	return 0;

err_parse_trips:
	return -EINVAL;
}

static int dynamic_temp_interval_data_parse(struct device_node *np,
					    struct nputh_data *param)
{
	int i, ret, temp_data;

	if (np == NULL)
		return -EINVAL;

	if (param == NULL)
		return -EINVAL;

	ret = of_property_read_s32(np, "dynamic-trips-num", &temp_data);
	if (ret != 0) {
		pr_err("%s parse dynamic-trips-num failed!\n", __func__);
		goto err_parse_step_wise;
	}
	param->dyn_trips_num = temp_data + 1;

	ret = of_property_read_s32(np, "dynamic-ref-temp-num", &temp_data);
	if (ret != 0) {
		pr_err("%s parse dynamic-ref-temp-num failed!\n", __func__);
		goto err_parse_step_wise;
	}
	param->dyn_ref_temp_num = temp_data;

	param->dyn_ref_temp = kzalloc(sizeof(int) * param->dyn_ref_temp_num, GFP_KERNEL);
	if (param->dyn_ref_temp == NULL) {
		pr_err("%s kzalloc dyn_ref_temp failed!\n", __func__);
		goto err_parse_step_wise;
	}
	ret = of_property_read_u32_array(np, "dynamic-ref-temp",
					 param->dyn_ref_temp,
					 (size_t)param->dyn_ref_temp_num);
	if (ret != 0) {
		pr_err("%s parse dynamic-ref-temp failed!\n", __func__);
		goto err_parse_step_wise;
	}

	param->dyn_intervals = (int **)kzalloc(sizeof(int *) * param->dyn_trips_num, GFP_KERNEL);
	if (param->dyn_intervals == NULL) {
		pr_err("%s kzalloc dyn_intervals failed!\n", __func__);
		goto err_parse_step_wise;
	}

	param->dyn_intervals[0] = param->intervals;
	for (i = 1; i < param->dyn_trips_num; i++) {
		ret = dynamic_trips_parse(np, param, i);
		if (ret != 0)
			goto err_parse_step_wise;
	}

	param->ori_trip_temp = param->temp;
	param->dyn_step_wise_enabled = true;

	return 0;

err_parse_step_wise:
	dynamic_step_wise_data_destory(param);
	return -EINVAL;
}
#endif /* CONFIG_THERMAL_GOV_DYNAMIC_INTERVAL_CTRL */

static struct nputh_data*
	npu_thermal_parse_dt(struct platform_device *pdev)
{
	struct nputh_data *param = NULL;
	struct device_node *np = pdev->dev.of_node;
	int tmp_data = 0;
	const char *tmp_str = NULL;

	if (np == NULL)
		return NULL;

	param = devm_kzalloc(&pdev->dev, sizeof(*param), GFP_KERNEL);
	if (param == NULL)
		return NULL;

	if (of_property_read_s32(np, "polling-delay", &tmp_data))
		goto err_parse_dt;
	param->polling_delay = tmp_data;

	if (of_property_read_s32(np, "polling-delay-passive", &tmp_data))
		goto err_parse_dt;
	param->passive_delay = tmp_data;

	if (of_property_read_s32(np, "trip-temp", &tmp_data))
		goto err_parse_dt;
	param->temp = tmp_data;

	if (of_property_read_string(np, "trip-type", &tmp_str))
		goto err_parse_dt;

	if (trip_point_type_parse(tmp_str, strlen(tmp_str), param))
		goto err_parse_dt;

	if (trips_cdev_name_parse(np, param))
		goto err_parse_dt;

	if (of_property_read_s32(np, "num-intervals", &tmp_data))
		goto err_parse_dt;
	param->num_intervals = tmp_data;

	param->intervals = kzalloc(sizeof(int) * param->num_intervals, GFP_KERNEL);
	if (param->intervals == NULL)
		goto err_parse_dt;
	if (of_property_read_u32_array(np, "thermal-interval",
				       param->intervals,
				       (size_t)param->num_intervals))
		goto err_parse_dt;
	if (param->intervals == NULL)
		goto err_parse_dt;

#ifdef CONFIG_THERMAL_GOV_DYNAMIC_INTERVAL_CTRL
	if (dynamic_temp_interval_data_parse(np, param))
		pr_err("%s parse dynamic temp interval data failed!\n", __func__);
#endif /* CONFIG_THERMAL_GOV_DYNAMIC_INTERVAL_CTRL */

	return param;

err_parse_dt:
	dev_err(&pdev->dev, "Parsing device tree data error.\n");
	return NULL;
}

static int npu_thermal_probe(struct platform_device *pdev)
{
	struct npu_thermal_zone *pzone = NULL;
	struct nputh_data *param = NULL;
	int ret = 0;

	param = npu_thermal_parse_dt(pdev);
	if (param == NULL)
		return -EINVAL;

	pzone = devm_kzalloc(&pdev->dev, sizeof(*pzone), GFP_KERNEL);
	if (pzone == NULL)
		return -ENOMEM;

	pzone->param = param;
	INIT_DEFERRABLE_WORK(&pzone->thermal_work, npu_thermal_work);

	pzone->thermal_dev =
		thermal_zone_device_register(NPU_THERMAL_NAME, 1, BIT(0), pzone, &thdev_ops,
					     &npu_thermal_params, 0, 0);

	if (IS_ERR(pzone->thermal_dev)) {
		dev_err(&pdev->dev, "Register thermal zone device failed.\n");
		ret = (int)PTR_ERR(pzone->thermal_dev);
		goto out;
	}
	dev_info(&pdev->dev, "Thermal zone device registered.\n");

	platform_set_drvdata(pdev, pzone);
	ret = thermal_zone_device_enable(pzone->thermal_dev);
	if (param->polling_delay != 0)
		queue_delayed_work(system_freezable_power_efficient_wq,
			&pzone->thermal_work,
			msecs_to_jiffies((unsigned int)param->polling_delay));
	return ret;
out:
	return ret;
}

static int npu_thermal_remove(struct platform_device *pdev)
{
	struct npu_thermal_zone *pzone = platform_get_drvdata(pdev);

#ifdef CONFIG_THERMAL_GOV_DYNAMIC_INTERVAL_CTRL
	pzone->param->intervals = pzone->param->dyn_intervals[0];
	dynamic_step_wise_data_destory(pzone->param);
#endif
	kfree(pzone->param->intervals);
	kfree(pzone->param);
	thermal_zone_device_unregister(pzone->thermal_dev);
	cancel_delayed_work(&pzone->thermal_work);
	return 0;
}

#ifdef CONFIG_OF
static const struct of_device_id npu_thermal_match[] = {
	{ .compatible = "ithermal,npu_thermal" },
	{},
};
MODULE_DEVICE_TABLE(of, npu_thermal_match);
#endif

static struct platform_driver npu_thermal_driver = {
	.driver = {
		.name = NPU_THERMAL_NAME,
		.of_match_table = of_match_ptr(npu_thermal_match),
	},
	.probe = npu_thermal_probe,
	.remove = npu_thermal_remove,
};

module_platform_driver(npu_thermal_driver);

MODULE_DESCRIPTION("npu thermal driver");
MODULE_LICENSE("GPL v2");
