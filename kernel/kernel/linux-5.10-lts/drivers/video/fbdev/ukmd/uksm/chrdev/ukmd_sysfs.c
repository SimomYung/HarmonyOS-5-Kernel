/**
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#include "ukmd_log.h"
#include "ukmd_sysfs.h"

void ukmd_sysfs_init(struct ukmd_attr *attrs)
{
	uint32_t i;

	if (unlikely(!attrs)) {
		ukmd_pr_err("attrs is null");
		return;
	}

	attrs->sysfs_index = 0;
	for (i = 0; i < UKMD_SYSFS_ATTRS_NUM; i++)
		attrs->sysfs_attrs[i] = NULL;

	attrs->sysfs_attr_group.attrs = attrs->sysfs_attrs;
}

void ukmd_sysfs_attrs_append(struct ukmd_attr *attrs, struct attribute *attr)
{
	if (unlikely(!attrs || !attr)) {
		ukmd_pr_err("attrs or attr is null");
		return;
	}

	if (attrs->sysfs_index >= UKMD_SYSFS_ATTRS_NUM || attrs->sysfs_index < 0) {
		ukmd_pr_err("sysfs_atts_num %d is out of range %d!", attrs->sysfs_index, UKMD_SYSFS_ATTRS_NUM);
		return;
	}

	attrs->sysfs_attrs[attrs->sysfs_index++] = attr;
}

int32_t ukmd_sysfs_create(struct device *dev, struct ukmd_attr *attrs)
{
	int32_t ret = -1;

	if (unlikely(!attrs || !dev)) {
		ukmd_pr_err("dev or attrs is null");
		return ret;
	}

	ret = sysfs_create_group(&dev->kobj, &(attrs->sysfs_attr_group));
	if (ret)
		ukmd_pr_err("sysfs group creation failed, error=%d!", ret);

	return ret;
}

void ukmd_sysfs_remove(struct device *dev, struct ukmd_attr *attrs)
{
	if (unlikely(!attrs || !dev)) {
		ukmd_pr_err("attrs is null");
		return;
	}

	sysfs_remove_group(&dev->kobj, &(attrs->sysfs_attr_group));

	ukmd_sysfs_init(attrs);
}
