/*
 * Huawei sparklink core code
 *
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
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
#include <securec.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/sysfs.h>
#include <linux/fs.h>
#include <linux/platform_device.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/of.h>
#include "sparklink.h"

#define POWR_MAX_LEN 5
#define VERSION_MAX_LEN 20
#define RINGING_ACK_LEN 2
#define RINGING_ACK_STATE_MAX 4
#define TRIGGER_UPDATE_LEN 15
#define UPDATE_IN_AIR_MAX_LEN 10

static struct sparklink_uart_ops *g_uart_interface = NULL;
static struct sparklink_driver_ops *g_driver_interface = NULL;

static ssize_t sparklink_power_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	unsigned int val = 0;
	ssize_t ret;

	if (!buf || count > POWR_MAX_LEN || !g_driver_interface) {
		sparklink_errmsg("power_store parameter is invalid\n");
		return -EINVAL;
	}

	if (SPARKLINK_STATE_UPDATING == g_driver_interface->device_get_state()) {
		sparklink_errmsg("not allow reset when 1162  upgrading firmware\n");
		return -EINVAL;
	}

	ret = kstrtouint(buf, 0, &val);
	if (ret < 0 || val > SPARKLINK_STATE_MAX) {
		sparklink_errmsg("power_store err mode = %u, ret =%d\n", val, ret);
		return -EINVAL;
	}

	ret = g_driver_interface->device_powermode(val);
	if (ret < 0) {
		sparklink_errmsg("power_store set powermode fail ret = %d\n", ret);
		return ret;
	}

	return count;
}

static ssize_t sparklink_power_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	int ret;

	if (!buf || !g_driver_interface) {
		sparklink_errmsg("power_state parameter is invalid\n");
		return 0;
	}

	ret = g_driver_interface->device_get_state();
	return snprintf_s(buf, POWR_MAX_LEN, POWR_MAX_LEN - 1, "%d\n", ret);
}

static DEVICE_ATTR(sparklink_power, S_IWUSR | S_IRUSR | S_IRUGO,
	sparklink_power_show, sparklink_power_store);

static ssize_t sparklink_version_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	int ret;

	if (!buf) {
		sparklink_errmsg("version show buf invalid\n");
		return 0;
	}

	if (g_driver_interface == NULL) {
		sparklink_errmsg("version show no device\n");
		return snprintf_s(buf, VERSION_MAX_LEN, VERSION_MAX_LEN - 1, "version:no_device");
	}

	ret = g_driver_interface->device_version();
	if (ret < 0)
		return snprintf_s(buf, VERSION_MAX_LEN, VERSION_MAX_LEN - 1, "version:NULL");

	return snprintf_s(buf, VERSION_MAX_LEN, VERSION_MAX_LEN - 1, "version:0x%x", ret);
}

static DEVICE_ATTR(sparklink_version, S_IRUSR | S_IRUGO, sparklink_version_show, NULL);

static ssize_t sparklink_log_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return 0;
}

static ssize_t sparklink_log_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	return -EINVAL;
}

static DEVICE_ATTR(sparklink_log, S_IWUSR | S_IRUSR | S_IRUGO,
	sparklink_log_show, sparklink_log_store);

static ssize_t sparklink_uart_channel_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	return 0;
}

static ssize_t sparklink_uart_channel_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	unsigned int val = 0;
	ssize_t ret;

	if (!buf || count > SPARKLINK_UART_MAX_LEN || !g_driver_interface) {
		sparklink_errmsg("power_sparklink_uart_channel_store parameter is invalid\n");
		return -EINVAL;
	}

	ret = kstrtouint(buf, 0, &val);
	if (ret < 0 || val >= SPARKLINK_UART_CHANNEL_MAX) {
		sparklink_errmsg("sparklink_uart_channel_store err mode = %u, ret = %d\n", val, ret);
		return -EINVAL;
	}

	ret = g_driver_interface->device_uart_switch_channel(val);
	if (ret < 0) {
		sparklink_errmsg("sparklink_uart_channel_store set uart channel fail ret = %d\n", ret);
		return ret;
	}

	return count;
}

static DEVICE_ATTR(sparklink_uart_channel, S_IWUSR | S_IRUSR | S_IRUGO,
	sparklink_uart_channel_show, sparklink_uart_channel_store);

static ssize_t sparklink_ringing_ack_store(struct device *dev,
    struct device_attribute *attr, const char *buf, size_t count)
{
	int ret;
	unsigned int val = 0;

	if (!buf || count > RINGING_ACK_LEN || !g_driver_interface) {
		sparklink_errmsg("sparklink_ringing_store parameter is invalid\n");
		return -EINVAL;
	}

	ret = kstrtouint(buf, 0, &val);
	if (ret < 0 || val > RINGING_ACK_STATE_MAX) {
		sparklink_errmsg("sparklink_ringing_ack err mode = %d, ret = %d\n", val, ret);
		return -EINVAL;
	}

	ret = g_driver_interface->device_ringing_ack(val);
	if (ret < 0) {
		sparklink_errmsg("sparklink_ringing_store failed\n");
		return -EINVAL;
	}

	return count;
}

static DEVICE_ATTR(sparklink_ringing_ack, S_IWUSR | S_IWGRP , NULL, sparklink_ringing_ack_store);

static ssize_t sparklink_trigger_update_store(struct device *dev,
    struct device_attribute *attr, const char *buf, size_t count)
{
	int ret;

	if (!buf || count != TRIGGER_UPDATE_LEN || !g_driver_interface) {
		sparklink_errmsg("sparklink_trigger_update parameter is invalid\n");
		return -EINVAL;
	}

	if (!strncmp(buf, "trigger update", TRIGGER_UPDATE_LEN - 1)) {
		sparklink_errmsg("sparklink update triggered, update start");
		ret = g_driver_interface->ota_fwupdate();
	} else {
		sparklink_errmsg("error cmd trigger update");
	}

	return count;
}

static DEVICE_ATTR(sparklink_trigger_update, S_IWUSR | S_IRUSR, NULL, sparklink_trigger_update_store);

static ssize_t sparklink_update_in_air_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	int ret;
	u32 val = 0;

	if (!buf) {
		sparklink_errmsg("update_in_air show buf invalid\n");
		return 0;
	}

	if (g_driver_interface == NULL) {
		sparklink_errmsg("update_in_air show no device\n");
		return snprintf_s(buf, UPDATE_IN_AIR_MAX_LEN, UPDATE_IN_AIR_MAX_LEN - 1, "erro");
	}

	ret = g_driver_interface->device_get_update_in_air_state(&val);
	if (ret < 0)
		return snprintf_s(buf, UPDATE_IN_AIR_MAX_LEN, UPDATE_IN_AIR_MAX_LEN - 1, "erro");

	return snprintf_s(buf, UPDATE_IN_AIR_MAX_LEN, UPDATE_IN_AIR_MAX_LEN - 1, "%04d", val);
}

static ssize_t sparklink_update_in_air_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	unsigned int val = 0;
	ssize_t ret;

	if (!buf || count > UPDATE_IN_AIR_PARAM_MAX_LEN || !g_driver_interface) {
		sparklink_errmsg("update_in_air parameter is invalid\n");
		return -EINVAL;
	}

	ret = kstrtouint(buf, 0, &val);
	if (ret < 0 || val >= UPDATE_IN_AIR_PARAM_MAX) {
		sparklink_errmsg("update_in_air err param = %u, ret = %d\n", val, ret);
		return -EINVAL;
	}

	ret = g_driver_interface->device_update_in_air_set_mode(val);
	if (ret < 0) {
		sparklink_errmsg("set pmu state fail ret = %d\n", ret);
		return ret;
	}

	return count;
}

static DEVICE_ATTR(sparklink_update_in_air, S_IWUSR | S_IRUSR | S_IRUGO,
	sparklink_update_in_air_show, sparklink_update_in_air_store);

static ssize_t sparklink_record_power_state_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	return 0;
}

static ssize_t sparklink_record_power_state_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	unsigned int val = 0;
	ssize_t ret;

	if (!buf || count > SPARKLINK_POWER_ONOFF_MAX_LEN || !g_driver_interface) {
		sparklink_errmsg("record_power_state parameter is invalid\n");
		return -EINVAL;
	}

	ret = kstrtouint(buf, 0, &val);
	if (ret < 0 ||val >= SPARKLINK_POWER_ONOFF_MAX) {
		sparklink_errmsg("record_power_state err pram = %x, ret = %d\n", val, ret);
		return -EINVAL;
	}

	ret = g_driver_interface->device_record_power_state(val);
	if (ret < 0) {
		sparklink_errmsg("record_power_state fail ret = %d\n", ret);
		return ret;
	}

	return count;
}

static DEVICE_ATTR(sparklink_record_power_state, S_IWUSR | S_IRUSR | S_IRUGO,
	sparklink_record_power_state_show, sparklink_record_power_state_store);

static struct attribute *sparklink_attributes[] = {
	&dev_attr_sparklink_power.attr,
	&dev_attr_sparklink_version.attr,
	&dev_attr_sparklink_log.attr,
	&dev_attr_sparklink_uart_channel.attr,
	&dev_attr_sparklink_ringing_ack.attr,
	&dev_attr_sparklink_trigger_update.attr,
#ifndef CONFIG_FACTORY_MODE
	&dev_attr_sparklink_update_in_air.attr,
	&dev_attr_sparklink_record_power_state.attr,
#endif
	NULL
};

static const struct attribute_group sparklink_attr_group = {
	.attrs = sparklink_attributes,
};

int sparklink_uart_ops_register(struct sparklink_uart_ops *ops)
{
	if (!ops)
		return -EINVAL;

	g_uart_interface = ops;
	return 0;
}

int sparklink_uart_ops_unregister(void)
{
	g_uart_interface = NULL;
	return 0;
}

int sparklink_driver_ops_register(struct sparklink_driver_ops *ops)
{
	if (!ops)
		return -EINVAL;

	g_driver_interface = ops;
	return 0;
}

int sparklink_driver_ops_unregister(void)
{
	g_driver_interface = NULL;
	return 0;
}

struct sparklink_uart_ops *sparklink_get_uart_ops(void)
{
	return g_uart_interface;
}

static int sparklink_core_probe(struct platform_device *pdev)
{
	int err;

	sparklink_infomsg("sparklink_core_probe enter\n");
	if (!pdev) {
		sparklink_infomsg("pdev is NULL\n");
		return -ENODEV;
	}

	err = sysfs_create_group(&pdev->dev.kobj, &sparklink_attr_group);
	if (err < 0) {
		sparklink_errmsg("sysfs create error %d\n", err);
		goto sysfs_create_fail;
	}

	sparklink_infomsg("sparklink_core_probe successfully\n");

sysfs_create_fail:
	return err;
}

static int sparklink_core_remove(struct platform_device *pdev)
{
	if (!pdev)
		return -ENODEV;

	sysfs_remove_group(&pdev->dev.kobj, &sparklink_attr_group);
	return 0;
}

static const struct of_device_id sparklink_core_match_table[] = {
	{ .compatible = "huawei,sparklink", },
	{ },
};

MODULE_DEVICE_TABLE(of, sparklink_core_match_table);

struct platform_driver sparklink_core_drv_pf = {
	.probe = sparklink_core_probe,
	.remove = sparklink_core_remove,
	.driver = {
		.name = "sparklink_core_platform",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(sparklink_core_match_table),
	},
};

module_platform_driver(sparklink_core_drv_pf);

MODULE_AUTHOR("Huawei Technologies Co., Ltd.");
MODULE_DESCRIPTION("huawei sparklink core");
MODULE_LICENSE("GPL v2");
