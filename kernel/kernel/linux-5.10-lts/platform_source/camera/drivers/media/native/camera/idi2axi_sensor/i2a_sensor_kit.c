/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022.
 * Description: idi2axi sensor kit, provide common interface
 *
 * This file is released under the GPLv2.
 */
#include "i2a_sensor_kit.h"
#include <linux/delay.h>

static LIST_HEAD(kit_drv_list);

void kit_register_driver(struct i2a_des_sensor_drv *drv)
{
	const char **name;

	if (!drv)
		return;
	list_add_tail(&drv->entry, &kit_drv_list);
	name = drv->sensor_names;
	while (name != NULL && *name != NULL) {
		pr_info("i2a des sensor driver for:%s registered", *name);
		++name;
	}
}

void kit_unregister_driver(struct i2a_des_sensor_drv *drv)
{
	const char **name;

	if (!drv)
		return;
	list_del_init(&drv->entry);
	name = drv->sensor_names;
	while (name != NULL && *name != NULL) {
		pr_info("i2a des sensor driver for:%s unregistered", *name);
		++name;
	}
}

struct i2a_des_sensor_drv *get_concrete_drv(const char *sensor_name)
{
	const char **name;
	struct i2a_des_sensor_drv *drv = NULL;
	int flag = 0;
	list_for_each_entry(drv, &kit_drv_list, entry) {
		name = drv->sensor_names;
		while (name != NULL && *name != NULL && strcmp(*name, sensor_name))
			++name;

		if (*name) {
			flag = 1;
			break;
		}
	}
	if (flag == 0)
		return NULL;
	return drv;
}
