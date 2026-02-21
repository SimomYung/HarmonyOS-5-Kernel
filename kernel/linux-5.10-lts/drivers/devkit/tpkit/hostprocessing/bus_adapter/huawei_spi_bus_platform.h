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

#ifndef __SPI_BUS_PLATFORM_H__
#define __SPI_BUS_PLATFORM_H__
#include "huawei_thp.h"

extern struct spi_driver g_thp_spi_driver[];

int thp_spi_sync(struct spi_device *spi, struct spi_message *message);
int thp_bus_lock(struct thp_device *tdev);
void thp_bus_unlock(struct thp_device *tdev);
int thp_set_spi_max_speed(unsigned int speed, u32 panel_id);
int thp_setup_spi(struct thp_core_data *cd);
int thp_spi_sync_alloc_mem(struct thp_core_data *cd);
long thp_spi_sync_common(const void __user *data,
	unsigned int lock_status, u32 panel_id);
int thp_set_spi_com_mode(struct thp_core_data *cd, u8 spi_com_mode);
int thp_spi_transfer(struct thp_core_data *cd,
	char *tx_buf, char *rx_buf, unsigned int len, unsigned int lock_status);
long thp_ioctl_spi_sync(const void __user *data,
	unsigned int lock_status, u32 panel_id);
long thp_ioctl_spi_sync_in_suspend(const void __user *data,
	unsigned int lock_status, u32 panel_id);
long thp_ioctl_set_spi_speed(unsigned long arg, u32 panel_id);
void thp_spi_cs_set(u32 control);
void thp_spi_cs_set_sub_panel(u32 control);
int register_spi_driver(u32 panel_id);
int unregister_spi_driver(u32 panel_id);

#endif