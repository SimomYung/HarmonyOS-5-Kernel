// SPDX-License-Identifier: GPL-2.0+
/*
 * fm1210_swi.c
 *
 * fm1210_swi driver
 *
 * Copyright (c) 2025-2025 Huawei Technologies Co., Ltd.
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

#include "fm1210_swi.h"
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/moduleparam.h>
#include <linux/sched.h>
#include <uapi/linux/sched/types.h>
#include <linux/of_gpio.h>
#include <linux/platform_device.h>
#include <linux/cdev.h>
#include <linux/delay.h>
#include <linux/compat.h>
#include <linux/uaccess.h>
#include <linux/ctype.h>
#include <linux/time.h>
#include <huawei_platform/log/hw_log.h>
#include "fm1210_api.h"

#ifdef HWLOG_TAG
#undef HWLOG_TAG
#endif
#define HWLOG_TAG fm1210_swi
HWLOG_REGIST();

#ifndef us2cycles
#define us2cycles(x)  (((((x) * 0x10c7UL) * loops_per_jiffy * HZ) + \
	(1UL << 31)) >> 32)
#endif

void fm1210_udelay(uint32_t us_delay)
{
	cycles_t start = get_cycles();
	cycles_t cnt_gap = 0;
	cycles_t cnt_delay = us2cycles(us_delay);

	while (cnt_gap < cnt_delay)
		cnt_gap = (get_cycles() - start);
}

void fm1210_gpio_dir_input(int onewire_gpio)
{
	gpio_direction_input(onewire_gpio);
}

void fm1210_gpio_dir_output(int onewire_gpio, uint8_t value)
{
	gpio_direction_output(onewire_gpio, value);
}

void fm1210_gpio_set_value(int onewire_gpio, uint8_t value)
{
	gpio_set_value(onewire_gpio, value);
}

int fm1210_gpio_get_value(int onewire_gpio)
{
	return gpio_get_value(onewire_gpio);
}

uint8_t fm1210_ow_dev_reset(struct fm1210_dev *di)
{
	int gpio_id;

	if (!di)
		return -ENODEV;

	gpio_id = di->onewire_gpio;

	fm1210_gpio_dir_output(gpio_id, HIGH);
	fm1210_gpio_set_value(gpio_id, LOW);
	fm1210_udelay(di->fm1210_swi.ow_reset_start_delay);
	fm1210_gpio_set_value(gpio_id, HIGH);
	fm1210_udelay(di->fm1210_swi.ow_reset_end_delay);

	return 0;
}

static void fm1210_write_bit_8m(struct fm1210_dev *di, uint8_t bit_data)
{
	int gpio_id;

	if (!di)
		return;

	gpio_id = di->onewire_gpio;

	fm1210_gpio_dir_output(gpio_id, LOW);
	if (bit_data) {
		hwlog_debug("[%s] write bit %u\n", __func__, bit_data);
		fm1210_udelay(di->fm1210_swi.ow_write_high_delay);
		fm1210_gpio_set_value(gpio_id, HIGH);
		fm1210_udelay(di->fm1210_swi.ow_write_low_delay - di->fm1210_swi.ow_write_high_delay);
	} else {
		hwlog_debug("[%s] write bit 0\n", __func__);
		fm1210_udelay(di->fm1210_swi.ow_write_low_delay);
	}
	fm1210_gpio_set_value(gpio_id, HIGH);
	fm1210_udelay(di->fm1210_swi.ow_write_end_delay);
}

static uint8_t fm1210_read_bit_8m(struct fm1210_dev *di)
{
	uint8_t bit_data = 1;
	int gpio_id;

	if (!di)
		return -1;

	gpio_id = di->onewire_gpio;

	fm1210_gpio_dir_output(gpio_id, LOW);
	fm1210_udelay(di->fm1210_swi.ow_read_start_delay);
	fm1210_gpio_dir_input(gpio_id);
	fm1210_udelay(di->fm1210_swi.ow_read_sample_delay);
	bit_data = fm1210_gpio_get_value(gpio_id);
	fm1210_udelay(di->fm1210_swi.ow_read_end_delay);
	fm1210_gpio_dir_output(gpio_id, HIGH);
	return bit_data;
}

void fm1210_write_byte_8m(struct fm1210_dev *di, uint8_t val)
{
	uint8_t i;

	if (!di)
		return;

	/* writes byte, one bit at a time */
	for (i = 0; i < BIT_OF_BYTE; i++)
		fm1210_write_bit_8m(di, (val >> (BIT_OFFSET - i)) & 0x01); /* write bit in temp into */
	udelay(FM1210_OW_WRITE_BYTE_DELAY);
}

uint8_t fm1210_read_byte_8m(struct fm1210_dev *di)
{
	uint8_t i;
	uint8_t value = 0;

	if (!di)
		return -EINVAL;

	for (i = 0; i < BIT_OF_BYTE; i++)
		if (fm1210_read_bit_8m(di))
			value |= 0x01 << (BIT_OFFSET - i); /* reads byte in, one byte at a time and then shifts it left */
	udelay(FM1210_OW_READ_BYTE_DELAY);
	return value;
}
