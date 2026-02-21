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

#ifndef __SPI_BUS_CONFIG_PARSE_H__
#define __SPI_BUS_CONFIG_PARSE_H__
#include "huawei_thp.h"
#include <linux/of_gpio.h>
#include <linux/of.h>

int thp_parse_spi_config(struct device_node *spi_cfg_node,
	struct thp_core_data *cd);

#endif