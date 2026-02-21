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

#include "huawei_bus_adapter.h"
#include "huawei_spi_bus_platform.h"

struct thp_bus_adapter *g_thp_bus_adapter;

struct thp_bus_adapter *get_bus_adapter(void)
{
	thp_log_info("%s:called\n", __func__);
    return g_thp_bus_adapter;
}

int use_spi_bus_interface(void *message)
{
    int ret;
    struct spi_bus_msg *spi_msg = (struct spi_bus_msg*) message;

    thp_log_info("%s:called\n", __func__);
	if (!spi_msg) {
		return -EINVAL;
	}
    ret = thp_spi_sync(spi_msg->sdev,
        spi_msg->msg);
    return ret;
}

int use_i2c_bus_interface(void *message)
{
    int ret;
    struct i2c_bus_msg *i2c_msg = (struct i2c_bus_msg*) message;
 
    thp_log_info("%s:called\n", __func__);
	if (!i2c_msg) {
		return -EINVAL;
	}
    ret = i2c_transfer(i2c_msg->client->adapter,
        i2c_msg->msg, i2c_msg->len);
    return ret;
}

int thp_spi_write(void *tx_buf, u16 length)
{
    return NO_ERR;
}

int thp_spi_read(void *reg_addr, u16 reg_len, u8 *rx_buf, u16 len)
{
    return NO_ERR;
}

int thp_i2c_write(void *tx_buf, u16 length)
{
    return NO_ERR;
}

int thp_i2c_read(void *reg_addr, u16 reg_len, u8 *rx_buf, u16 len)
{
    return NO_ERR;
}

struct thp_bus_adapter* thp_bus_transfer_adapter(enum thp_bus_type type)
{
    g_thp_bus_adapter = kzalloc(sizeof(struct thp_bus_adapter), GFP_KERNEL);
    if (!g_thp_bus_adapter) {
        thp_log_err("%s:thp bus adapter is NULL\n", __func__);
        return NULL;
    }
 
    switch(type){
        case SPI_BUS_TYPE:
            g_thp_bus_adapter->driver_ops.register_driver = register_spi_driver;
            g_thp_bus_adapter->bus_transfer_ops.thp_bus_sync = use_spi_bus_interface;
            g_thp_bus_adapter->bus_transfer_ops.thp_bus_write = thp_spi_write;
            g_thp_bus_adapter->bus_transfer_ops.thp_bus_read = thp_spi_read;
            g_thp_bus_adapter->driver_ops.unregister_driver = unregister_spi_driver;
        break;
        case I2C_BUS_TYPE:
            g_thp_bus_adapter->bus_transfer_ops.thp_bus_sync = use_i2c_bus_interface;
            g_thp_bus_adapter->bus_transfer_ops.thp_bus_write = thp_i2c_write;
            g_thp_bus_adapter->bus_transfer_ops.thp_bus_read = thp_i2c_read;
        break;
        default:
            thp_log_err("%s: bus type config is not match\n", __func__);
            kfree(g_thp_bus_adapter);
            return NULL;
    }

    return g_thp_bus_adapter;
}