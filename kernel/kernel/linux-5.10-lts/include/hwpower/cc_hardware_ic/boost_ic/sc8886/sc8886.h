/* SPDX-License-Identifier: GPL-2.0 */
/*
 * sc8886.h
 *
 * sc8886 header file
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

#ifndef _SC8886_H_
#define _SC8886_H_

#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/i2c.h>
#include <linux/gpio.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/of_address.h>
#include <linux/of_gpio.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/bitops.h>
#include <chipset_common/hwpower/common_module/power_log.h>
#include <chipset_common/hwpower/reverse_charge/reverse_charge.h>

#define CHIP_DEV_NAME_LEN                     32
#define SC8886_IDLE_STATE                     1
#define SC8886_REVERSE_5V                     5000
#define SC8886_REVERSE_9V                     9000
#define SC8886_REVERSE_3A                     3000
#define SC8886_REVERSE_2A                     2000
#define SC8886_REVERSE_IBUS_LIMIT             4200
#define SC8666_OTG_VOL_RANGE_HIGH             19520
#define SC8666_OTG_VOL_RANGE_LOW              3000
#define SC8886_OTG_VOL_RANGE_OFFSET           1280

struct sc8886_device_info {
	struct i2c_client *client;
	struct device *dev;
	struct work_struct irq_work;
	struct workqueue_struct *int_wq;
	char name[CHIP_DEV_NAME_LEN];
	u8 chip_id;
	int chip_already_init;
	int boost_en;
};

struct sc8886_log_data {
	u8 chrg_stat0;
	u8 chrg_stat1;
};

/* OTG_VOLT_REG1 reg=0x07 */
#define SC8886_OTG_VOLT_SET_HIGH                0x07
#define SC8886_OTG_VOLT_SET_HIGH_MASK           0x3F /* 0b00111111 */
#define SC8886_OTG_VOLT_RESOLUTION              8 /* OTG voltage ranges from 3000mV to 20800mV, with 8mV resolution. */

/* OTG_VOLT_REG0 reg=0x06 */
#define SC8886_OTG_VOLT_SET_LOW                 0x06
#define SC8886_OTG_VOLT_SET_LOW_SHIFT           2
#define SC8886_OTG_VOLT_SET_LOW_MASK            0xFC /* 0b11111100 */
#define SC8886_OTG_VOLT_SET_LOW_BITS            6

/* OTG_CUR_REG1 reg=0x09 */
#define SC8886_OTG_CUR_SET_HIGH                 0x09
#define SC8886_OTG_CUR_SET_HIGH_MASK            0x7F /* 0b01111111 */
#define SC8886_OTG_CUR_RESOLUTION               50 /* OTG current ranges from 50mA to 6400mA, with 50mA resolution. */

/* OTG_CUR_REG1 reg=0x08 */
#define SC8886_OTG_CUR_SET_LOW                  0x08 /* Reserved */

/* Charge Log */
#define SC8886_CHARGE_STATUS_REG0               0x20
#define SC8886_CHARGE_STATUS_REG1               0x21

/* Charge Option1() Register_1 */
#define SC8886_CHARGE_OP1_REG1                  0x31
#define SC8886_PTM_PINSEL_SHIFT                 0
#define SC8886_PTM_PINSEL_MASK                  1

/* CHARGE OPTION3 REG0 reg=0x34 */
#define SC8886_CHARGE_OP3_REG0                  0x34
#define SC8886_OTG_MODE_SHIFT                   5
#define SC8886_OTG_MODE_MASK                    BIT(5)
#define SC8886_OTG_RANGE_LOW_SHIFT              2
#define SC8886_OTG_RANGE_LOW_MASK               BIT(2)

/* CHARGE OPTION3 REG1 reg=0x35 */
#define SC8886_CHARGE_OP3_REG1                  0x35
#define SC8886_ENABLE_OTG_SHIFT                 4
#define SC8666_ENABLE_OTG_MASK                  BIT(4)

/* DEVICE_ID reg=0x2f */
#define SC8886_DEVICE_ID_REG                    0x2F

#define SC8886_DEVICE_ID                        0x66
#define BQ25713_DEVICE_ID                       0x88

#endif /* _SC8886_H_ */
