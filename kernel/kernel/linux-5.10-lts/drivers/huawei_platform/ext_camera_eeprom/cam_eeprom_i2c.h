/* SPDX-License-Identifier: GPL-2.0 */
/*
 * cam_eeprom_i2c.h
 *
 * cam_eeprom_i2c header file
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

#ifndef _CAM_EEPROM_I2C_H_
#define _CAM_EEPROM_I2C_H_

#include <linux/i2c.h>

#define LEN_U8_DAT       1
#define LEN_WR_MSG       1
#define LEN_RD_MSG       2
#define LEN_U16_REG      2
#define PAGE_SIZE        64
#define U8_MASK          0xFF
#define U8_BITES         8

int i2c_read_block_without_cmd(struct i2c_client *client, u8 *buf, u16 buf_len);
int i2c_write_block(struct i2c_client *client, u8 *buf, u8 buf_len);
int i2c_u16_write_page(struct i2c_client *client, u16 reg, u8 *buf, u8 buf_len);
int i2c_read_block(struct i2c_client *client, u8 *cmd, u8 cmd_len, u8 *buf, u16 buf_len);
int i2c_u16_write_byte(struct i2c_client *client, u16 reg, u8 data);
int i2c_u16_read_byte(struct i2c_client *client, u16 reg, u8 *data);
int i2c_u16_read_block(struct i2c_client *client, u16 reg, u8 *data, u16 buf_len);

#endif /* _CAM_EEPROM_I2C_H_ */
