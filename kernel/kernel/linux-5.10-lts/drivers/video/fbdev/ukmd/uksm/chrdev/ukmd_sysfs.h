/**
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
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

#ifndef UKMD_SYSFS_H
#define UKMD_SYSFS_H

#include <linux/platform_device.h>

#define UKMD_SYSFS_ATTRS_NUM 64

struct ukmd_attr {
	int32_t sysfs_index;
	struct attribute *sysfs_attrs[UKMD_SYSFS_ATTRS_NUM];
	struct attribute_group sysfs_attr_group;
};

void ukmd_sysfs_init(struct ukmd_attr *attrs);
void ukmd_sysfs_attrs_append(struct ukmd_attr *attrs, struct attribute *attr);
int32_t ukmd_sysfs_create(struct device *dev, struct ukmd_attr *attrs);
void ukmd_sysfs_remove(struct device *dev, struct ukmd_attr *attrs);

#endif
