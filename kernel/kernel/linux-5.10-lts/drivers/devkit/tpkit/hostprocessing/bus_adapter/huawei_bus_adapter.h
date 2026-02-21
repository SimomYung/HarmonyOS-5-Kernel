/*
 * Huawei Touchscreen Driver
 *
 * Copyright (c) 2024-2024 Huawei Technologies Co., Ltd.
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

#ifndef __BUS_ADAPTER_H__
#define __BUS_ADAPTER_H__

#include "huawei_thp.h"
#include <linux/i2c.h>
#include <linux/device.h>
 
enum thp_bus_type {
	SPI_BUS_TYPE = 0,
	I2C_BUS_TYPE,
};
 
struct spi_bus_msg {
	struct spi_device *sdev;
	struct spi_message *msg;
};
 
struct i2c_bus_msg {
	struct i2c_client *client;
	struct i2c_msg *msg;
	u16 len;
};

struct thp_driver_ops {
	/* differential bus type register and probe */
	int (*register_driver)(u32 panel_id);
	int (*unregister_driver)(u32 panel_id);
};

struct thp_dev_register_ops {
	/* thp device register */
	int (*thp_register_dev)(void *dev);
};

struct thp_bus_transfer_ops {
	/* bus transfer */
	int (*thp_bus_sync)(void *message);
	int (*thp_bus_write)(void *tx_buf, u16 length);
	int (*thp_bus_read)(void *reg_addr, u16 reg_len, u8 *rx_buf, u16 len);
};

struct thp_bus_adapter {
	struct thp_driver_ops driver_ops;
	struct thp_dev_register_ops tp_register_ops;
	struct thp_bus_transfer_ops bus_transfer_ops;
};

struct thp_bus_adapter* thp_bus_transfer_adapter(enum thp_bus_type type);
struct thp_bus_adapter* get_bus_adapter(void);
#endif
