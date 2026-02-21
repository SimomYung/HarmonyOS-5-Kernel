/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022.
 * Description: idi2axi sensor kit interface
 *
 * This file is released under the GPLv2.
 */
#ifndef I2A_SENSOR_KIT_H
#define I2A_SENSOR_KIT_H
#include <linux/types.h>
#include <linux/list.h>
#include <media/v4l2-ioctl.h>
#include <platform_include/camera/native/sensor_cfg.h>
#include "i2a_i2c.h"
#include "i2a_sensor_common.h"

struct i2a_des_sensor_ops {
	int (*match_id)(struct i2a_i2c_client *, struct sensor_des_port_cfg *);
	int (*stream_on)(struct i2a_i2c_client *, struct sensor_des_port_cfg *);
	int (*stream_off)(struct i2a_i2c_client *, struct sensor_des_port_cfg *);
	int (*init_i2c)(struct i2a_i2c_client *, struct sensor_des_port_cfg *);
	int (*get_uds_info)(struct i2a_i2c_client *, struct sensor_uds_info *);
};

struct i2a_des_sensor_drv {
	struct list_head entry;
	const char **sensor_names;
	struct i2a_des_sensor_ops *ops;
};

struct i2a_des_sensor_drv *get_concrete_drv(const char *sensor_name);
void kit_register_driver(struct i2a_des_sensor_drv *drv);
void kit_unregister_driver(struct i2a_des_sensor_drv *drv);

#endif /* end of include guard: I2A_SENSOR_KIT_H */
