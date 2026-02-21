/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022.
 * Description: idi2axi i2c interface
 *
 * This file is released under the GPLv2.
 */
#ifndef I2A_I2C_H
#define I2A_I2C_H
#include <linux/types.h>
#include <linux/list.h>
#include <linux/mutex.h>
#include <linux/i2c.h>
#include "i2a_sensor_common.h"

struct i2a_des_i2c_data {
	struct device *dev;
	struct mutex mtx;
};

int cci_read_bytes(struct i2a_i2c_client *iclient, uint16_t reg, uint8_t *out_data, uint32_t len);
int cci_write_bytes(struct i2a_i2c_client *iclient, uint16_t reg, uint8_t *in_data, uint32_t len);
int cci_read_integer(struct i2a_i2c_client *iclient, uint16_t reg, uint8_t *out_data, uint32_t len);
int cci_write_integer(struct i2a_i2c_client *iclient, uint16_t reg, uint32_t in_data, uint32_t len);
#define cci_read(client, reg, ptr) cci_read_integer(client, reg, (uint8_t*)(ptr), sizeof(*(ptr)))
#define cci_write(client, reg, val) cci_write_integer(client, reg, (uint32_t)(val), sizeof(val))
#define cci_write_byte(client, reg, val) cci_write_integer(client, reg, (uint32_t)(val), 1)

#endif /* end of include guard: I2A_I2C_H */
