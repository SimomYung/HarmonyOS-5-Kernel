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
#ifndef TC_DEVICE_NODE_H
#define TC_DEVICE_NODE_H
#include <linux/of.h>
#include "tc_client_driver.h"

struct device_node *get_device_node(const char *name);
int set_cdev_kobj_name(struct dev_node *container_node, struct dev_node *private_node, struct dev_node *client_node);
void set_devnode(struct class *driver_class);
#endif