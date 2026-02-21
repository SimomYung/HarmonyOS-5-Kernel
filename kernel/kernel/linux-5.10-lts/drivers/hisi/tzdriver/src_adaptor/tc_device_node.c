/*
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
 */
#include "tc_device_node.h"
#include <linux/of_platform.h>
#include <linux/of.h>
#include "tc_ns_log.h"
#include "tc_current_info.h"

struct device_node *get_device_node(const char *name)
{
	if (!name)
		return NULL;
	return of_find_compatible_node(NULL, NULL, name);
}

int set_cdev_kobj_name(struct dev_node *container_node, struct dev_node *private_node, struct dev_node *client_node)
{
	if (!container_node || !private_node || !client_node)
		return -1;

	private_node->char_dev.kobj.name = NULL;
	client_node->char_dev.kobj.name = NULL;
	container_node->char_dev.kobj.name = NULL;
	return 0;
}

static char *tee_devnode(struct device *dev, umode_t *mode)
{
	if (!dev || !mode)
		return NULL;

	if (strcmp(dev_name(dev), TC_NS_CVM_DEV) == 0)
		*mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;

	return NULL;
}

void set_devnode(struct class *driver_class)
{
	if (driver_class)
		driver_class->devnode = tee_devnode;
}
