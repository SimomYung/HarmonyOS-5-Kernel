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

#include <linux/string.h>
#include <linux/kernel.h>
#include <linux/firmware.h>

#include "mt380x_firmware.h"

uint8_t mt380x_firmware_data[FIRMWARE_NUM];

uint32_t firmware_loading(mt380x_dev dev)
{
	const struct firmware *mt380x_firmware = NULL;
	const char *fw_name = "mt380x_ranging_firmware.bin";
	uint32_t data_size;
	int32_t err;

	err = request_firmware(&mt380x_firmware, fw_name, dev->dev);
	if (err || !mt380x_firmware) {
		mt380x_errmsg("Firmware request failed err = %d\n", err);
		return 0;
	}
	mt380x_infomsg("Firmware request succeeded!\n");
	data_size = (uint32_t)mt380x_firmware->size;
	mt380x_dbgmsg("mt380x_firmware size:%u\n", data_size);
	if (mt380x_firmware->size > FIRMWARE_NUM) {
		mt380x_errmsg("memcpy_s failed, mt380x_firmware->size:%u", mt380x_firmware->size);
		release_firmware(mt380x_firmware);
		return 0;
	}
	if (memcpy_s(mt380x_firmware_data, FIRMWARE_NUM,
		mt380x_firmware->data, mt380x_firmware->size) != EOK) {
		mt380x_errmsg("memcpy_s failed, mt380x_firmware->size:%u", mt380x_firmware->size);
		release_firmware(mt380x_firmware);
		return 0;
	}
	release_firmware(mt380x_firmware);
	return data_size;
}
