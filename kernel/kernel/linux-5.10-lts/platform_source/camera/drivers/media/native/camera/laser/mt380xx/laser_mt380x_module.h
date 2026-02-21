/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 * Description: laser vi5300 driver
 * Create: 2025-05-16
 */

#ifndef LASER_MODULE_H
#define LASER_MODULE_H

#include <linux/module.h>
#include <linux/i2c.h>
#include <platform_include/camera/native/laser_cfg.h>
#include <mt380x/mt380x_module.h>
#include <../hw_laser_subdev.h>

#define laser_err(fmt, ...) pr_err("[laser]ERROR: " fmt "\n", ##__VA_ARGS__)
#define laser_info(fmt, ...) pr_info("[laser]INFO: " fmt "\n", ##__VA_ARGS__)
#define laser_dbg(fmt, ...) pr_debug("[laser]DBG: " fmt "\n", ##__VA_ARGS__)

typedef struct _tag_laser_module_intf_t {
	char *laser_name;
	int (*data_init)(struct i2c_client *client,
		const struct i2c_device_id *id);
	int (*data_remove)(struct i2c_client *client);
	long (*laser_ioctl)(void *hw_data, unsigned int cmd, void *p);
} laser_module_intf_t;

#endif

