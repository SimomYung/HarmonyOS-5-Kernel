/* SPDX-License-Identifier: GPL-2.0 */
/*
 * cam_eeprom_module_adapter.h
 *
 * cam_eeprom_module_adapter header file
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

#ifndef _CAM_EEPROM_MODULE_H_
#define _CAM_EEPROM_MODULE_H_

#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/of_address.h>
#include <linux/bitops.h>
#include <linux/mutex.h>

struct cam_eeprom_module_device_info {
	struct mutex lock;
	struct i2c_client *client;
	struct device *dev;
	u32 cam_index;
	u32 eeprom_capacity;
	u16 base_addr;
	u16 otp_read_addr;
	u16 otp_write_addr;
	bool otp_enable;
};

#define MAX_DEVICE_NAME_SIZE   20
#define MAX_SEGMENT_SIZE       768
#define MAX_HEADER_SIZE        256
#define MAX_OTP_ENABLE_SIZE    32
#define WRITE_INTERVAL_MS      5

#endif /* _CAM_EEPROM_MODULE_H_ */
