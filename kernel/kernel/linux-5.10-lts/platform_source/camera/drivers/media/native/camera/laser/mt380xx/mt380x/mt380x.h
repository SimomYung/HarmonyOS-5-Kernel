/*
 * Linux kernel modules for MT380X FlightSense TOF sensor
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 1 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 */

#ifndef MT380X_H
#define MT380X_H

#include <linux/mutex.h>
#include <linux/workqueue.h>
#include <linux/miscdevice.h>
#include <linux/time.h>
#include <platform_include/camera/native/laser_cfg.h>
#include "mt380x_def.h"
#include "hw_laser_dmd.h"

#define MT380X_CHIP_ADDR 0xB0

#define MT380X_REG_DEV_STAT 0x08
#define MT380X_REG_INTR_STAT 0x09
#define MT380X_REG_INTR_MASK 0x05
#define MT380X_REG_DEV_ADDR 0x07
#define MT380X_REG_CMD 0x03
#define MT380X_REG_SCRATCH_PAD_BASE 0x10
#define MT380X_REG_CHIPID_BASE 0xB9

#define MT380X_CROSSTALK_TRIM_CMD 0x04
#define MT380X_SINGLE_RANGE_CMD 0x01
#define MT380X_CONTINUOUS_RANGE_CMD 0x02
#define MT380X_STOP_RANGE_CMD 0x00

#define MT380X_MAX_WAIT_RETRY 5
#define MOVE_BIT_8 8
#define MOVE_BIT_16 16
#define MOVE_BIT_24 24

#define MT380X_ERROR_NONE (0)
#define MT380X_ERROR_CPU_BUSY (-1)
#define MT380X_ERROR_ENABLE_INTR (-2)
#define MT380X_ERROR_CROSSTALK_CALIB (-3)
#define MT380X_ERROR_OFFSET_CALIB (-4)
#define MT380X_ERROR_CROSSTALK_CONFIG (-5)
#define MT380X_ERROR_SINGLE_CMD (-6)
#define MT380X_ERROR_CONTINUOUS_CMD (-7)
#define MT380X_ERROR_GET_DATA (-8)
#define MT380X_ERROR_STOP_CMD (-9)
#define MT380X_ERROR_IRQ_STATE (-10)
#define MT380X_ERROR_FW_FAILURE (-11)
#define MT380X_ERROR_POWER_ON (-12)
#define MT380X_ERROR_POWER_OFF (-13)
#define MT380X_ERROR_SET_TEMP (-14)
#define MT380X_ERROR_OFFSET_CONFIG (-15)

struct mt380x_measurement_data {
	int16_t obj_range;
	uint32_t confidence;
	uint8_t range_status;
	uint32_t noise;
};

struct mt380x_dev_info {
	char dev_info[100];
};

struct mt380x_crosstalk_calib_data {
	int8_t crosstalk_position;
	uint16_t crosstalk_ratio;
};

struct mt380x_offset_calib_data {
	int16_t offset_cal;
	int16_t not_use;
};

struct mt380x_data {
	struct device *dev;
	const char *dev_name;
	struct mt380x_measurement_data range_data;
	struct mt380x_crosstalk_calib_data crosstalk_data;
	struct mt380x_offset_calib_data offset_data;
	struct i2c_client *client;
	struct input_dev *input_dev;
	struct miscdevice miscdev;
	int irq_gpio;
	int xshut_gpio;
	int irq;
	uint32_t enable_chip;
	uint32_t debug_enable;
	uint8_t intr_state;
	uint8_t fwdl_status;
	int8_t crosstalk_position_config;
	uint16_t crosstalk_ratio_config;
	int8_t offset_config;
	const char *program_version;
	uint32_t period;
	struct mutex work_mutex;
};

enum MT380X_INT_STATUS {
	MT380X_INTR_DISABLED = 0,
	MT380X_INTR_ENABLED,
};

#define mt380x_infomsg(str, args...) \
	pr_info("%s: " str, __func__, ##args)
#define mt380x_dbgmsg(str, args...) \
	pr_debug("%s: " str, __func__, ##args)
#define mt380x_errmsg(str, args...) \
	pr_err("%s: " str, __func__, ##args)

#endif
