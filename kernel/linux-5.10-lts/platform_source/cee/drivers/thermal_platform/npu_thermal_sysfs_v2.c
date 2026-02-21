/*
 * npu_thermal_sysfs_v2.c
 *
 * npu thermal calculation sysfs module
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

#include <securec.h>
#include <linux/device.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/thermal.h>

#include <platform_include/cee/linux/lpm_thermal.h>
#include "npu_thermal.h"

static ssize_t dyn_ctrl_enable_show(struct device *dev, struct device_attribute *attr,
				    char *buf);
static ssize_t dyn_ctrl_enable_store(struct device *dev, struct device_attribute *attr,
				     const char *buf, size_t count);

static struct device_attribute g_dyn_ctrl_attrs[] = {
	{
		.attr = {.name = "dyn_ctrl_enable", .mode = 0640, },
		.show = dyn_ctrl_enable_show,
		.store = dyn_ctrl_enable_store,
	},
};

static struct attribute_group g_dyn_ctrl_attr_group = {
	.name = "dyn_ctrl",
	.attrs = NULL,
};

static int dyn_ctrl_enable(struct thermal_zone_device *tz)
{
	struct npu_thermal_zone *pzone = tz->devdata;

	mutex_lock(&tz->lock);
	pzone->param->dyn_step_wise_enabled = true;
	mutex_unlock(&tz->lock);

	return 0;
}

static int dyn_ctrl_disable(struct thermal_zone_device *tz)
{
	struct npu_thermal_zone *pzone = tz->devdata;

	mutex_lock(&tz->lock);
	pzone->param->dyn_step_wise_enabled = false;
	mutex_unlock(&tz->lock);

	return 0;
}

static ssize_t dyn_ctrl_enable_show(struct device *dev,
				    struct device_attribute *attr,
				    char *buf)
{
	struct thermal_zone_device *tz = to_thermal_zone(dev);
	struct npu_thermal_zone *pzone = tz->devdata;

	return (ssize_t)sprintf_s(buf, PAGE_SIZE, "%s\n",
				  pzone->param->dyn_step_wise_enabled ?
				  MODE_ENABLED : MODE_DISABLED);
}

static ssize_t dyn_ctrl_enable_store(struct device *dev,
				     struct device_attribute *attr,
				     const char *buf, size_t count)
{
	struct thermal_zone_device *tz = to_thermal_zone(dev);
	int ret;

	if (strncmp(buf, MODE_ENABLED, sizeof(MODE_ENABLED) - 1) == 0)
		ret = dyn_ctrl_enable(tz);
	else if (strncmp(buf, MODE_DISABLED, sizeof(MODE_DISABLED) - 1) == 0)
		ret = dyn_ctrl_disable(tz);
	else
		ret = -EINVAL;

	if (ret)
		return ret;

	return count;
}

void npu_thermal_destroy_attrs(struct thermal_zone_device *tz)
{
	if (!tz)
		return;

	device_remove_groups(&tz->device,
			     &tz->device.groups[tz->dyn_interval_attr_index]);

	/* tz->dyn_interval_attr_group->attrs = g_dyn_ctrl_attr_group.attrs
	 * keep g_dyn_ctrl_attr_group, and just free its attrs
	 */
	kfree(tz->dyn_interval_attr_group->attrs);
	tz->dyn_interval_attr_group->attrs = NULL;
	tz->dyn_interval_attr_group = NULL;
}

int npu_thermal_creat_attrs(struct thermal_zone_device *tz)
{
	struct attribute **attrs;
	int attr_nums, indx, ret;

	if (!tz)
		return -EINVAL;
	if (tz->ori_attr_groups_size == 0) {
		pr_err("%s dyn ctrl attrs init err for zero length\n",
		       __func__);
		return -EINVAL;
	}

	attr_nums = ARRAY_SIZE(g_dyn_ctrl_attrs);
	attrs = kcalloc(attr_nums + 1, sizeof(*attrs), GFP_KERNEL);
	if (!attrs) {
		pr_err("%s dyn ctrl attrs kcalloc err\n",
		       __func__);
		return -ENOMEM;
	}
	for (indx = 0; indx < ARRAY_SIZE(g_dyn_ctrl_attrs); indx++)
		attrs[indx] = &g_dyn_ctrl_attrs[indx].attr;
	/* last set NULL */
	attrs[attr_nums] = NULL;
	g_dyn_ctrl_attr_group.attrs = attrs;
	tz->dyn_interval_attr_group = &g_dyn_ctrl_attr_group;

	for (indx = tz->ori_attr_groups_size - 1; indx < cal_extra_attr_group_size(tz); indx++)
		if (tz->device.groups[indx] == NULL)
			break;
	tz->dyn_interval_attr_index = indx;
	tz->device.groups[tz->dyn_interval_attr_index] = tz->dyn_interval_attr_group;
	ret = device_add_groups(&tz->device,
				&tz->device.groups[tz->dyn_interval_attr_index]);
	if (ret) {
		pr_err("%s dyn ctrl attrs group creating fails\n",
		       __func__);
		npu_thermal_destroy_attrs(tz);
	}

	return ret;
}
