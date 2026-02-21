// SPDX-License-Identifier: GPL-2.0
/*
 * cam_eeprom_i2c.c
 *
 * i2c interface for cam_eeprom module
 *
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
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

#include "cam_eeprom_i2c.h"
#include <securec.h>
#include <huawei_platform/log/hw_log.h>

#define HWLOG_TAG cam_eeprom_i2c
HWLOG_REGIST();

int i2c_read_block_without_cmd(struct i2c_client *client,
	u8 *buf, u16 buf_len)
{
	struct i2c_msg msg;

	if (!client || !buf) {
		hwlog_err("client or buf is null\n");
		return -EINVAL;
	}
	if (!((&client->dev)->driver_data)) {
		hwlog_err("client driver_data is null\n");
		return -EINVAL;
	}

	msg.addr = client->addr;
	msg.flags = I2C_M_RD;
	msg.buf = buf;
	msg.len = buf_len;

	if (i2c_transfer(client->adapter, &msg, 1) != 1) {
		hwlog_err("addr=%x read fail\n", client->addr);
		return -EIO;
	}

	return 0;
}

int i2c_write_block(struct i2c_client *client, u8 *buf, u8 buf_len)
{
	struct i2c_msg msg[LEN_WR_MSG];

	if (!client || !buf) {
		hwlog_err("client or buf is null\n");
		return -EINVAL;
	}

	if (!((&client->dev)->driver_data)) {
		hwlog_err("client driver_data is null\n");
		return -EINVAL;
	}

	msg[0].addr = client->addr;
	msg[0].flags = 0;
	msg[0].buf = buf;
	msg[0].len = buf_len;

	/* i2c_transfer return number of messages transferred */
	if (i2c_transfer(client->adapter, msg, LEN_WR_MSG) != LEN_WR_MSG) {
		hwlog_err("addr=%x write fail\n", client->addr);
		return -EIO;
	}

	return 0;
}

int i2c_u16_write_page(struct i2c_client *client, u16 reg, u8 *buf, u8 buf_len)
{
	int ret;
	u8 page_buf[PAGE_SIZE + LEN_U16_REG] = {0};

	page_buf[0] = reg >> U8_BITES;
 	page_buf[1] = reg & U8_MASK;
	ret = memcpy_s(page_buf + LEN_U16_REG, PAGE_SIZE, buf, buf_len);
	if (ret != 0) {
		hwlog_err("memcpy_s fail\n");
		return -EIO;
	}

	return i2c_write_block(client, page_buf, sizeof(page_buf));
}

int i2c_read_block(struct i2c_client *client, u8 *cmd, u8 cmd_len,
	u8 *buf, u16 buf_len)
{
	struct i2c_msg msg[LEN_RD_MSG];
	struct cam_star_module_device_info *di = NULL;

	if (!client || !cmd || !buf) {
		hwlog_err("client or cmd or buf is null\n");
		return -EINVAL;
	}
	if (!((&client->dev)->driver_data)) {
		hwlog_err("client driver_data is null\n");
		return -EINVAL;
	}

	/* the cmd to be written before reading data */
	/* flags: 0 is write data, from master to slave */
	msg[0].addr = client->addr;
	msg[0].flags = 0;
	msg[0].buf = cmd;
	msg[0].len = cmd_len;

	/* flags: I2C_M_RD is read data, from slave to master */
	msg[1].addr = client->addr;
	msg[1].flags = I2C_M_RD;
	msg[1].buf = buf;
	msg[1].len = buf_len;

	/* i2c_transfer return number of messages transferred */
	if (i2c_transfer(client->adapter, msg, LEN_RD_MSG) != LEN_RD_MSG) {
		hwlog_err("addr=%x read fail\n", client->addr);
		return -EIO;
	}

	return 0;
}

int i2c_u16_write_byte(struct i2c_client *client, u16 reg, u8 data)
{
	u8 buf[LEN_U16_REG + LEN_U8_DAT] = { 0 };
	/*
	 * buf[0]: high-8bit slave register address
	 * buf[1]: low-8bit slave register address
	 * buf[2]: data to be written
 	 */
	buf[0] = reg >> U8_BITES;
	buf[1] = reg & U8_MASK;
	buf[2] = data;

	if (i2c_write_block(client, buf, sizeof(buf))) {
		hwlog_err("addr=%x write byte fail\n", client->addr);
		return -EIO;
	}

	return 0;
}

int i2c_u16_read_byte(struct i2c_client *client, u16 reg, u8 *data)
{
	u8 cmd[LEN_U16_REG] = { 0 };
	u8 buf[LEN_U8_DAT] = { 0 };

	/* cmd[0]: high-8bit register, cmd[1]: low-8bit register */
 	cmd[0] = reg >> U8_BITES;
 	cmd[1] = reg & U8_MASK;

	if (i2c_read_block(client, cmd, sizeof(cmd), buf, sizeof(buf))) {
		hwlog_err("addr=%x read byte fail\n", client->addr);
		return -EIO;
	}

	/* buf[0]: 8bit data */
	*data = buf[0];
	return 0;
}

int i2c_u16_read_block(struct i2c_client *client, u16 reg, u8 *data, u16 buf_len)
{
	u8 cmd[LEN_U16_REG] = { 0 };

	/* cmd[0]: high-8bit register, cmd[1]: low-8bit register */
	cmd[0] = reg >> U8_BITES;
	cmd[1] = reg & U8_MASK;

	if (i2c_read_block(client, cmd, sizeof(cmd), data, buf_len)) {
		hwlog_err("addr=%x read byte fail\n", client->addr);
		return -EIO;
	}

	return 0;
}
