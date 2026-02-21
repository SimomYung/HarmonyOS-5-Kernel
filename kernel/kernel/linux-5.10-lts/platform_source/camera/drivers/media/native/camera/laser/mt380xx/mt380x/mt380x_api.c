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

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/time.h>
#include <linux/interrupt.h>

#include "mt380x.h"
#include "mt380x_platform.h"
#include "mt380x_firmware.h"
#include "mt380x_api.h"

#define STATUS_TOF_CONFIDENT 0
#define STATUS_TOF_SEMI_CONFIDENT 6
#define STATUS_TOF_NOT_CONFIDENT 7
#define STATUS_TOF_NO_OBJECT 255
#define FIRMWARE_SEGMENTATION_NUMBER 128
#define FW_NORMAL_STATE_FLAG_1 0x66
#define FW_NORMAL_STATE_FLAG_2 0x77

mt380x_error mt380x_power_on(mt380x_dev dev)
{
	mt380x_error status = MT380X_ERROR_NONE;

	status = gpio_direction_output(dev->xshut_gpio, 0);
	if (status != MT380X_ERROR_NONE) {
		mt380x_errmsg("Chip Power ON Failed status = %d\n", status);
		return MT380X_ERROR_POWER_ON;
	}
	mdelay(2);
	status = gpio_direction_output(dev->xshut_gpio, 1);
	if (status != MT380X_ERROR_NONE) {
		mt380x_errmsg("Chip Power ON Failed status = %d\n", status);
		return MT380X_ERROR_POWER_ON;
	}
	mdelay(5);
	return status;
}

mt380x_error mt380x_power_off(mt380x_dev dev)
{
	mt380x_error status = MT380X_ERROR_NONE;

	status = gpio_direction_output(dev->xshut_gpio, 0);
	if (status != MT380X_ERROR_NONE) {
		mt380x_errmsg("Chip Power OFF Failed status = %d\n", status);
		return MT380X_ERROR_POWER_OFF;
	}

	return status;
}

mt380x_error mt380x_wait_cpu_ready(mt380x_dev dev)
{
	uint8_t stat = 0;
	int retry = 0;

	do {
		mdelay(1);
		(void)mt380x_read_byte(dev, MT380X_REG_DEV_STAT, &stat);
		if ((stat & 0x01) == 0)
			return MT380X_ERROR_NONE;
	} while (retry++ < MT380X_MAX_WAIT_RETRY);

	if (retry >= MT380X_MAX_WAIT_RETRY) {
		mt380x_errmsg("CPU Busy stat = %d\n", stat);
		return MT380X_ERROR_CPU_BUSY;
	}
	return MT380X_ERROR_NONE;
}

mt380x_error mt380x_read_chip_version(mt380x_dev dev)
{
	uint8_t chipid[2];

	mt380x_error status = mt380x_read_multibytes(dev, MT380X_REG_CHIPID_BASE, chipid, 2);
	if (status != MT380X_ERROR_NONE) {
		mt380x_errmsg("read reg failed status = %d\n", status);
		return -EINVAL;
	}
	mt380x_infomsg("ChipVersion: %02x %02x\n", chipid[1], chipid[0]);
	if (chipid[1] == 0x38 && chipid[0] == 0x02)
		return 0;
	else
		return -1;
}

void mt380x_read_fw_version(mt380x_dev dev)
{
	mt380x_error status = MT380X_ERROR_NONE;
	uint8_t fw_version[2];

	status = mt380x_write_byte(dev, 0x10, 0x06);
	if (status != MT380X_ERROR_NONE) {
		mt380x_errmsg("write reg failed status = %d\n", status);
		return;
	}
	status = mt380x_write_byte(dev, 0x03, 0x03);
	if (status != MT380X_ERROR_NONE) {
		mt380x_errmsg("write reg failed status = %d\n", status);
		return;
	}
	mdelay(5);
	status = mt380x_read_multibytes(dev, MT380X_REG_SCRATCH_PAD_BASE, fw_version, 2);
	mt380x_infomsg("fw_version: major=%02x minor=%02x\n", fw_version[1], fw_version[0]);
	if (status != MT380X_ERROR_NONE)
		mt380x_errmsg("read reg Failed status = %d\n", status);
}

mt380x_error mt380x_set_prepare(mt380x_dev dev)
{
	mt380x_error status = MT380X_ERROR_NONE;

	status += mt380x_write_byte(dev, 0x00, 0x00);
	status += mt380x_write_byte(dev, 0x02, 0x02);
	status += mt380x_write_byte(dev, 0x04, 0x51);
	status += mt380x_write_byte(dev, 0x41, 0x00);
	status += mt380x_write_byte(dev, 0x42, 0x00);
	return status;
}

mt380x_error mt380x_flash_fw(mt380x_dev dev, uint16_t fw_size)
{
	uint16_t i;
	uint8_t j = 0;
	mt380x_error status = MT380X_ERROR_NONE;

	while (fw_size >= FIRMWARE_SEGMENTATION_NUMBER) { // firmware segmentation number:128
		for (i = 0; i < FIRMWARE_SEGMENTATION_NUMBER / 16; i++)
			status += mt380x_write_multibytes(dev, 0x43,
				&(mt380x_firmware_data[i * 16 + j * FIRMWARE_SEGMENTATION_NUMBER]),
				16);
		fw_size -= FIRMWARE_SEGMENTATION_NUMBER;
		j++;
	}

	for (i = 0; i < fw_size; i++) {
		status += mt380x_write_byte(dev, 0x43,
			mt380x_firmware_data[i + j * FIRMWARE_SEGMENTATION_NUMBER]);
	}
	return status;
}

mt380x_error mt380x_check_fw_status(mt380x_dev dev)
{
	uint8_t val = 0;
	uint8_t trytimes;
	mt380x_error status = MT380X_ERROR_FW_FAILURE;

	for (trytimes = 0; trytimes < 100; trytimes++) {
		mdelay(1);
		status = mt380x_read_byte(dev, MT380X_REG_SCRATCH_PAD_BASE, &val);
		// fw Normal state
		if (val == FW_NORMAL_STATE_FLAG_1 || val == FW_NORMAL_STATE_FLAG_2) {
			mdelay(10);
			return MT380X_ERROR_NONE;
		}
	}
	return status == MT380X_ERROR_NONE ? MT380X_ERROR_FW_FAILURE : status;
}

mt380x_error mt380x_init_firmware(mt380x_dev dev)
{
	uint16_t fw_size = 0;
	mt380x_error status = MT380X_ERROR_NONE;

	// load fw
	fw_size = firmware_loading(dev);
	if (!fw_size) {
		mt380x_errmsg("Firmware Load Failed!\n");
		return MT380X_ERROR_FW_FAILURE;
	}

	// prepare flash fw
	status = mt380x_set_prepare(dev);
	if (status != MT380X_ERROR_NONE) {
		mt380x_errmsg("prepare flash fw fail, status = %d\n", status);
		return MT380X_ERROR_FW_FAILURE;
	}

	// flash fw
	status = mt380x_flash_fw(dev, fw_size);
	if (status != MT380X_ERROR_NONE) {
		mt380x_errmsg("flash fw fail, status = %d\n", status);
		return MT380X_ERROR_FW_FAILURE;
	}

	// send fw download ok msg to IC
	status = mt380x_write_byte(dev, 0x04, 0x11);
	status += mt380x_write_byte(dev, 0x02, 0x00);
	status += mt380x_write_byte(dev, 0x00, 0x01);
	if (status != MT380X_ERROR_NONE) {
		mt380x_errmsg("flash fw after, status = %d\n", status);
		return MT380X_ERROR_FW_FAILURE;
	}

	// check run
	status = mt380x_check_fw_status(dev);
	if (status == MT380X_ERROR_NONE) {
		mt380x_infomsg("fw is ok\n");
		return status;
	}
	mt380x_errmsg("Download Firmware Failed\n");
	return MT380X_ERROR_FW_FAILURE;
}

void mt380x_get_sys_tof_offset(mt380x_dev dev, int8_t *tof_offset)
{
	mt380x_error status = MT380X_ERROR_NONE;

	status += mt380x_write_byte(dev, 0x10, 0x00);
	status += mt380x_write_byte(dev, 0x11, 0x01);
	status += mt380x_write_byte(dev, 0x12, 0x0b);
	status += mt380x_write_byte(dev, 0x03, 0x03);
	mdelay(5);
	status += mt380x_read_byte(dev, 0x13, (int8_t *)tof_offset);
	mt380x_infomsg(" *tof_offset = %d\n", (int32_t)(*tof_offset));
	if (status != MT380X_ERROR_NONE)
		mt380x_errmsg("read reg Failed status = %d\n", status);
}

mt380x_error mt380x_set_period(mt380x_dev dev, uint32_t period)
{
	mt380x_error status = MT380X_ERROR_NONE;

	status += mt380x_write_byte(dev, 0x10, 0x01);
	status += mt380x_write_byte(dev, 0x11, 0x01);
	status += mt380x_write_byte(dev, 0x12, 0x16);
	status += mt380x_write_byte(dev, 0x13, (uint8_t)period);
	status += mt380x_write_byte(dev, 0x03, 0x03);
	mdelay(5);
	if (status != MT380X_ERROR_NONE)
		mt380x_errmsg("write reg Failed status = %d\n", status);
	return status;
}

mt380x_error mt380x_single_measure(mt380x_dev dev)
{
	mt380x_error status = MT380X_ERROR_NONE;
	uint8_t interrupt_status = 0;

	status = mt380x_wait_cpu_ready(dev);
	if (status != MT380X_ERROR_NONE) {
		mt380x_errmsg("CPU Abnormal Single Measure!status = %d\n", status);
		return MT380X_ERROR_SINGLE_CMD;
	}
	status = mt380x_get_and_clear_interrupt_state(dev, &interrupt_status);
	if (status != MT380X_ERROR_NONE) {
		mt380x_errmsg("clear interrupt fail status = %d\n", status);
		return MT380X_ERROR_SINGLE_CMD;
	}
	status = mt380x_write_byte(dev, MT380X_REG_CMD, MT380X_SINGLE_RANGE_CMD);
	if (status != MT380X_ERROR_NONE) {
		mt380x_errmsg("Single measure  Failed status = %d\n", status);
		return MT380X_ERROR_SINGLE_CMD;
	}

	return status;
}

mt380x_error mt380x_begin_continuous_measure(mt380x_dev dev)
{
	mt380x_error status = MT380X_ERROR_NONE;
	uint8_t interrupt_status = 0;

	status = mt380x_wait_cpu_ready(dev);
	if (status != MT380X_ERROR_NONE) {
		mt380x_errmsg("CPU Abnormal Continuous Measure!status = %d\n", status);
		return MT380X_ERROR_CONTINUOUS_CMD;
	}
	status = mt380x_get_and_clear_interrupt_state(dev, &interrupt_status);
	if (status != MT380X_ERROR_NONE) {
		mt380x_errmsg("clear interrupt Failed status = %d\n", status);
		return MT380X_ERROR_CONTINUOUS_CMD;
	}
	status = mt380x_write_byte(dev, MT380X_REG_CMD, MT380X_CONTINUOUS_RANGE_CMD);
	if (status != MT380X_ERROR_NONE) {
		mt380x_errmsg("Continuous Measure Failed status = %d\n", status);
		return MT380X_ERROR_CONTINUOUS_CMD;
	}

	return status;
}

mt380x_error mt380x_end_continuous_measure(mt380x_dev dev)
{
	mt380x_error status = MT380X_ERROR_NONE;
	uint8_t interrupt_status = 0;

	status = mt380x_get_and_clear_interrupt_state(dev, &interrupt_status);
	if (status != MT380X_ERROR_NONE)
		mt380x_errmsg("clear interrupt Failed status = %d\n", status);
	status = mt380x_write_byte(dev, MT380X_REG_CMD, MT380X_STOP_RANGE_CMD);
	if (status != MT380X_ERROR_NONE) {
		mt380x_errmsg("stop Measure Failed status = %d\n", status);
		return MT380X_ERROR_STOP_CMD;
	}

	return status;
}

mt380x_error mt380x_get_measure_datas(mt380x_dev dev)
{
	mt380x_error status = MT380X_ERROR_NONE;
	uint8_t buf[32];
	uint16_t nearlimit;
	uint16_t farlimit;
	uint32_t noise;
	uint16_t ref_tof;
	uint32_t peak1;
	uint32_t integral_times;
	uint8_t confidence;
	int16_t cal_millimeter;
	uint8_t interrupt_status = 0;

	status = mt380x_get_and_clear_interrupt_state(dev, &interrupt_status);
	if (status != MT380X_ERROR_NONE) {
		mt380x_errmsg("clear interrupt Failed status = %d\n", status);
		return MT380X_ERROR_GET_DATA;
	}

	status += mt380x_read_multibytes(dev, MT380X_REG_SCRATCH_PAD_BASE, buf, 16); // 16:length
	status += mt380x_read_multibytes(dev, MT380X_REG_SCRATCH_PAD_BASE + MOVE_BIT_16,
		buf + MOVE_BIT_16, 16); // 16:length
	if (status != MT380X_ERROR_NONE) {
		mt380x_errmsg("Get Range Data Failed status = %d\n", status);
		return MT380X_ERROR_GET_DATA;
	}
	// nearlimit:buf[19,20]
	nearlimit = *((uint8_t *)(buf + 19)) + ((*((uint8_t *)(buf + 20))) << MOVE_BIT_8);
	// farlimit:buf[21,22]
	farlimit = *((uint8_t *)(buf + 21)) + ((*((uint8_t *)(buf + 22))) << MOVE_BIT_8);
	// ref_tof:buf[5,6]
	ref_tof = *((uint8_t *)(buf + 5)) + ((*((uint8_t *)(buf + 6))) << MOVE_BIT_8);
	// noise:buf[17,18]
	noise = *((uint8_t *)(buf + 17)) + ((*((uint8_t *)(buf + 18))) << MOVE_BIT_8);
	integral_times = *((uint8_t *)(buf + 0)) + ((*((uint8_t *)(buf + 1))) << MOVE_BIT_8) +
		((*((uint8_t *)(buf + 2))) << MOVE_BIT_16); // integral_times:buf[0,1,2]
	// peak1:buf[9,10,11,12]
	peak1 = *((uint8_t *)(buf + 9)) + ((*((uint8_t *)(buf + 10))) << MOVE_BIT_8) +
		((*((uint8_t *)(buf + 11))) << MOVE_BIT_16) +
		((*((uint8_t *)(buf + 12))) << MOVE_BIT_24);
	confidence = *((uint8_t *)(buf + 29)); // confidence:buf[29]
	// cal_millimeter:buf[25,26]
	cal_millimeter = *((uint8_t *)(buf + 25)) + ((*((uint8_t *)(buf + 26))) << MOVE_BIT_8);

	dev->range_data.obj_range = cal_millimeter;
	dev->range_data.confidence = confidence;
	dev->range_data.noise = noise;
	mt380x_infomsg("obj_range:%d, confidence:%u, nearlimit:%u, farlimit:%u, noise:%u, peak1:%u, integral_times:%u, ref_tof:%u\n",
		dev->range_data.obj_range, dev->range_data.confidence, nearlimit,
		farlimit, noise, peak1, integral_times, ref_tof);
	if (confidence >= 70) { // available data threshold of confidence
		// when cal_millimeter is a negative value
		if (cal_millimeter <= 0) {
			// obj_range is default value
			dev->range_data.obj_range = 5;
			// nearlimit is default value
			nearlimit = 15;
			// farlimit is default value
			farlimit = 35;
		}
		dev->range_data.range_status = STATUS_TOF_CONFIDENT;
		if (cal_millimeter < 50 && peak1 < 800000) // component constraints data
			dev->range_data.range_status = STATUS_TOF_NO_OBJECT;
	} else if (confidence >= 0) {
		dev->range_data.range_status = STATUS_TOF_NO_OBJECT;
	}

	mt380x_infomsg("obj_range:%d, nearlimit:%u, farlimit:%u\n",
		dev->range_data.obj_range, nearlimit, farlimit);

	return status;
}

mt380x_error mt380x_get_and_clear_interrupt_state(mt380x_dev dev, uint8_t *stat)
{
	mt380x_error status = MT380X_ERROR_NONE;

	status = mt380x_read_byte(dev, MT380X_REG_INTR_STAT, stat);
	if (status != MT380X_ERROR_NONE) {
		mt380x_errmsg("read byte fail\n");
		return status;
	}
	mt380x_infomsg("*stat:%u\n", *stat);
	if ((*stat) & 0x01)
		mt380x_infomsg("interrupt cleared succ\n");
	return status;
}

mt380x_error mt380x_interrupt_enable(mt380x_dev dev)
{
	int loop = 0;
	uint8_t enable = 0;

	do {
		(void)mt380x_read_byte(dev, MT380X_REG_INTR_MASK, &enable);
		enable |=  0x05;
		(void)mt380x_write_byte(dev, MT380X_REG_INTR_MASK, enable);
		(void)mt380x_read_byte(dev, MT380X_REG_INTR_MASK, &enable);
		if ((enable & 0x05) != 0)
			return MT380X_ERROR_NONE;
		loop++;
	} while (loop < MT380X_MAX_WAIT_RETRY);
	if (loop >= MT380X_MAX_WAIT_RETRY) {
		mt380x_errmsg("enable interrupt Failed\n");
		return MT380X_ERROR_ENABLE_INTR;
	}
	return MT380X_ERROR_NONE;
}

mt380x_error mt380x_init(mt380x_dev dev)
{
	mt380x_error status = MT380X_ERROR_NONE;

	status = mt380x_wait_cpu_ready(dev);
	if (status != MT380X_ERROR_NONE) {
		mt380x_errmsg("Internal CPU busy!\n");
		return status;
	}
	status = mt380x_interrupt_enable(dev);
	if (status != MT380X_ERROR_NONE) {
		mt380x_errmsg("interrupt enable failed!\n");
		return status;
	}
	status = mt380x_init_firmware(dev);
	if (status != MT380X_ERROR_NONE) {
		mt380x_errmsg("Download Firmware Failed!\n");
		return status;
	}
	mt380x_read_fw_version(dev);

	return status;
}

mt380x_error mt380x_get_sys_temperature_enable(mt380x_dev dev)
{
	mt380x_error status = MT380X_ERROR_NONE;
	uint8_t value = 0;

	status += mt380x_write_byte(dev, 0x10, 0x00);
	status += mt380x_write_byte(dev, 0x11, 0x01);
	status += mt380x_write_byte(dev, 0x12, 0x15);
	status += mt380x_write_byte(dev, 0x03, 0x03);
	mdelay(5);
	status += mt380x_read_byte(dev, 0x13, &value);
	if (status != MT380X_ERROR_NONE)
		mt380x_errmsg("read byte fail status = %d\n", status);
	return status;
}

mt380x_error mt380x_begin_crosstalk_calibration(mt380x_dev dev)
{
	mt380x_error status = MT380X_ERROR_NONE;

	status = mt380x_wait_cpu_ready(dev);
	if (status != MT380X_ERROR_NONE) {
		mt380x_errmsg("CPU Abnormal CROSSTALK Calibrating status = %d\n", status);
		return MT380X_ERROR_CROSSTALK_CALIB;
	}
	status = mt380x_get_sys_temperature_enable(dev);
	if (status != MT380X_ERROR_NONE) {
		mt380x_errmsg("sys temperature enable Failed status = %d\n", status);
		return MT380X_ERROR_CROSSTALK_CALIB;
	}
	status = mt380x_write_byte(dev, MT380X_REG_CMD, MT380X_CROSSTALK_TRIM_CMD);
	if (status != MT380X_ERROR_NONE) {
		mt380x_errmsg("CROSSTALK Calibration Failed status = %d\n", status);
		return MT380X_ERROR_CROSSTALK_CALIB;
	}

	return status;
}

mt380x_error mt380x_total_millimeter_calculate(mt380x_dev dev, int32_t *total_m)
{
	uint8_t range_count = 0;
	uint8_t buf[2];
	uint8_t stat = 0;
	uint16_t mm = 0;
	mt380x_error status = MT380X_ERROR_NONE;

	while (range_count < 60) { // use data of 40~60
		status = mt380x_get_and_clear_interrupt_state(dev, &stat);
		if (status != MT380X_ERROR_NONE) {
			mt380x_errmsg("clear interrupt Failed status = %d\n", status);
			return status;
		}
		if ((stat & 0x01) != 0x01) {
			mdelay(5);
			continue;
		}

		if (range_count >= 40) {
			status |= mt380x_read_multibytes(dev, 0x29, buf, 2);
			if (status != MT380X_ERROR_NONE) {
				mt380x_errmsg("read multibytes Failed status = %d\n", status);
				return status;
			}
			mm = *((uint8_t *)(buf + 0)) | ((*((uint8_t *)(buf + 1))) << 8);
			mt380x_infomsg("offset cali, mm = %d\n", mm);
			*total_m += mm;
		}
		range_count++;
	}

	return status;
}

mt380x_error mt380x_begin_offset_calibration(mt380x_dev dev, uint32_t param)
{
	mt380x_error status = MT380X_ERROR_NONE;
	int32_t total_m = 0;
	int16_t offset = 0;
	int8_t tof_offset = 0;

	disable_irq(dev->irq);
	status = mt380x_begin_continuous_measure(dev);
	if (status != MT380X_ERROR_NONE) {
		mt380x_errmsg("begin continuous measure Failed status = %d\n", status);
		enable_irq(dev->irq);
		return MT380X_ERROR_OFFSET_CALIB;
	}
	status = mt380x_total_millimeter_calculate(dev, &total_m);
	if (status != MT380X_ERROR_NONE) {
		mt380x_errmsg("total millimeter calculat Failed status = %d\n", status);
		enable_irq(dev->irq);
		return MT380X_ERROR_OFFSET_CALIB;
	}
	status = mt380x_end_continuous_measure(dev);
	if (status != MT380X_ERROR_NONE) {
		mt380x_errmsg("Offset Calibtration Stop Failed status = %d\n", status);
		enable_irq(dev->irq);
		return MT380X_ERROR_OFFSET_CALIB;
	}
	offset = total_m / 20;
	mt380x_get_sys_tof_offset(dev, &tof_offset);
	int16_t temp = param - offset + tof_offset;

	mt380x_infomsg("temp = %d\n", temp);
	mt380x_infomsg("param = %u, offset = %d, tof_offset = %d\n", param, offset, tof_offset);
	if (temp >= -128 && temp <= 127) {
		dev->offset_data.offset_cal = temp;
	} else {
		enable_irq(dev->irq);
		return MT380X_ERROR_OFFSET_CALIB;
	}
	enable_irq(dev->irq);
	return status;
}

mt380x_error mt380x_config_offset_params(mt380x_dev dev)
{
	mt380x_error status = MT380X_ERROR_NONE;

	status = mt380x_wait_cpu_ready(dev);
	if (status != MT380X_ERROR_NONE) {
		mt380x_errmsg("CPU Abnormal Configing Offset Failed status = %d\n", status);
		return MT380X_ERROR_OFFSET_CONFIG;
	}

	status += mt380x_write_byte(dev, 0x10, 0x01);
	status += mt380x_write_byte(dev, 0x11, 0x01);
	status += mt380x_write_byte(dev, 0x12, 0x0b);
	status += mt380x_write_byte(dev, 0x13, *((int8_t *)(&dev->offset_config)));
	status += mt380x_write_byte(dev, 0x03, 0x03);
	mdelay(5);

	return status;
}

mt380x_error mt380x_get_crosstalk_params(mt380x_dev dev)
{
	mt380x_error status = MT380X_ERROR_NONE;
	uint8_t val = 0;
	uint16_t crosstalk_peak;
	uint8_t buf[5];

	status = mt380x_read_byte(dev, 0x2E, &val);
	if (status == MT380X_ERROR_NONE && val == 0x88) {
		status |= mt380x_read_multibytes(dev, 0x34, buf, 5);
		if (status != MT380X_ERROR_NONE) {
			mt380x_errmsg("Get CrossTalk Data Failed status = %d\n", status);
			return MT380X_ERROR_CROSSTALK_CALIB;
		}
		dev->crosstalk_data.crosstalk_position = *((uint8_t *)(buf + 0));
		crosstalk_peak = *((uint8_t *)(buf + 1)) + ((*((uint8_t *)(buf + 2))) << 8);
		dev->crosstalk_data.crosstalk_ratio = *((uint8_t *)(buf + 3));
		mt380x_infomsg("crosstalk_peak = %u\n", crosstalk_peak);
	} else {
		mt380x_errmsg("CROSSTALK Calibration Failed status = %d, val = 0x%02x\n",
			status, val);
		return MT380X_ERROR_CROSSTALK_CALIB;
	}

	return status;
}

mt380x_error mt380x_config_crosstalk_params(mt380x_dev dev)
{
	mt380x_error status = MT380X_ERROR_NONE;

	status = mt380x_wait_cpu_ready(dev);
	if (status != MT380X_ERROR_NONE) {
		mt380x_errmsg("CPU Abnormal Configing CROSSTALK Failed status = %d\n", status);
		return MT380X_ERROR_CROSSTALK_CONFIG;
	}

	status += mt380x_write_byte(dev, 0x10, 0x01);
	status += mt380x_write_byte(dev, 0x11, 0x01);
	status += mt380x_write_byte(dev, 0x12, 0x13);
	status += mt380x_write_byte(dev, 0x13, (uint8_t)(dev->crosstalk_position_config));
	status += mt380x_write_byte(dev, 0x03, 0x03);
	mdelay(5);

	status += mt380x_write_byte(dev, 0x10, 0x01);
	status += mt380x_write_byte(dev, 0x11, 0x01);
	status += mt380x_write_byte(dev, 0x12, 0x14);
	status += mt380x_write_byte(dev, 0x13, (uint8_t)(dev->crosstalk_ratio_config));
	status += mt380x_write_byte(dev, 0x03, 0x03);
	mdelay(5);

	return status;
}
