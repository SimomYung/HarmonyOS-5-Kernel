// SPDX-License-Identifier: GPL-2.0
/*
 * rhc32l110_fw.c
 *
 * rhc32l110 firmware driver
 *
 * Copyright (c) 2021-2021 Huawei Technologies Co., Ltd.
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

#include "rstm32g031.h"
#include "rhc32l110_fw.h"
#include "rstm32g031_i2c.h"
#include <chipset_common/hwpower/common_module/power_delay.h>
#include <chipset_common/hwpower/common_module/power_gpio.h>
#include <chipset_common/hwpower/common_module/power_printk.h>
#include <linux/firmware.h>

#define HWLOG_TAG rhc32l110_fw
HWLOG_REGIST();

static int rhc32l110_fw_retry_write_bootloader(struct rstm32g031_device_info *di, u8 *cmd)
{
	int i;

	/* retry max 10 times to wait i2c init ok */
	for (i = 0; i < RHC32L110_FW_ACK_COUNT; i++) {
		if (!rstm32g031_write_word_bootloader(di, cmd, RHC32L110_FW_CMD_SIZE))
			return 0;

		power_usleep(DT_USLEEP_2MS);
	}

	return -EIO;
}

static int rhc32l110_fw_write_cmd(struct rstm32g031_device_info *di, u16 cmd)
{
	int i;
	u8 ack;
	u8 buf[RHC32L110_FW_CMD_SIZE] = { 0 };

	/* fill cmd */
	buf[0] = cmd >> 8;
	buf[1] = cmd & 0xFF;

	if (rhc32l110_fw_retry_write_bootloader(di, buf))
		return -EIO;

	for (i = 0; i < RHC32L110_FW_ACK_COUNT; i++) {
		power_usleep(DT_USLEEP_1MS);
		ack = 0;
		(void)rstm32g031_read_word_bootloader(di, &ack, 1);
		hwlog_info("write_cmd:ack=%x\n", ack);
		if (ack == RHC32L110_FW_ACK_VAL) {
			hwlog_info("write_cmd succ: i=%d cmd=%x ack=%x\n", i, cmd, ack);
			return 0;
		}
	}

	hwlog_err("write_cmd fail: i=%d cmd=%x\n", i, cmd);
	return -EIO;
}

static int rhc32l110_fw_write_addr(struct rstm32g031_device_info *di, u32 addr)
{
	int i;
	u8 ack;
	u8 buf[RHC32L110_FW_ADDR_SIZE] = { 0 };

	/* fill address */
	buf[0] = addr >> 24;
	buf[1] = (addr >> 16) & 0xFF;
	buf[2] = (addr >> 8) & 0xFF;
	buf[3] = (addr >> 0) & 0xFF;
	buf[4] = buf[0];
	for (i = 1; i < RHC32L110_FW_ADDR_SIZE - 1; i++)
		buf[4] ^= buf[i];

	(void)rstm32g031_write_word_bootloader(di, buf, RHC32L110_FW_ADDR_SIZE);
	for (i = 0; i < RHC32L110_FW_ACK_COUNT; i++) {
		power_usleep(DT_USLEEP_1MS);
		ack = 0;
		(void)rstm32g031_read_word_bootloader(di, &ack, 1);
		hwlog_info("write_addr:ack=%x\n", ack);
		if (ack == RHC32L110_FW_ACK_VAL) {
			hwlog_info("write_addr succ: i=%d addr=%x ack=%x\n", i, addr, ack);
			return 0;
		}
	}

	hwlog_err("write_addr fail: i=%d addr=%x\n", i, addr);
	return -EIO;
}

static int rhc32l110_fw_erase(struct rstm32g031_device_info *di)
{
	int ret;
	int i;
	u8 ack;
	u8 buf[RHC32L110_FW_ERASE_SIZE] = { 0 };

	/* special erase */
	buf[0] = 0xFF;
	buf[1] = 0xFF;
	buf[2] = buf[0] ^ buf[1];

	ret = rhc32l110_fw_write_cmd(di, RHC32L110_FW_ERASE_CMD);
	if (ret)
		return ret;

	(void)rstm32g031_write_word_bootloader(di, buf, RHC32L110_FW_ERASE_SIZE);
	for (i = 0; i < RHC32L110_FW_ERASE_ACK_COUNT; i++) {
		power_msleep(DT_MSLEEP_500MS, 0, NULL);
		ack = 0;
		(void)rstm32g031_read_word_bootloader(di, &ack, 1);
		hwlog_info("erase:ack=%x\n", ack);
		if (ack == RHC32L110_FW_ACK_VAL) {
			hwlog_info("erase succ: i=%d ack=%x\n", i, ack);
			return 0;
		}
	}

	hwlog_err("erase fail\n");
	return -EIO;
}

static int rhc32l110_fw_write_data(struct rstm32g031_device_info *di,
	const u8 *data, int len)
{
	int i;
	u8 ack;
	u8 checksum = len - 1;
	u8 buf[RHC32L110_FW_PAGE_SIZE + 2] = {0};

	if ((len > RHC32L110_FW_PAGE_SIZE) || (len <= 0)) {
		hwlog_err("data len illegal, len=%d\n", len);
		return -EINVAL;
	}

	/* buf content: (len of data - 1) + data + checksum */
	buf[0] = len - 1;
	for (i = 1; i <= len; i++) {
		buf[i] = data[i - 1];
		checksum ^= buf[i];
	}
	buf[len + 1] = checksum;

	(void)rstm32g031_write_word_bootloader(di, buf, len + 2);
	for (i = 0; i < RHC32L110_FW_ACK_COUNT; i++) {
		power_usleep(DT_USLEEP_20MS);
		ack = 0;
		(void)rstm32g031_read_word_bootloader(di, &ack, 1);
		hwlog_info("write_data:ack=%x\n", ack);
		if (ack == RHC32L110_FW_ACK_VAL) {
			hwlog_info("write_data succ: i=%d len=%d ack=%x\n", i, len, ack);
			return 0;
		}
	}

	hwlog_err("write_data fail\n");
	return -EIO;
}

static int rhc32l110_fw_write_mtp_data(struct rstm32g031_device_info *di,
	const u8 *mtp_data, int mtp_size)
{
	int ret;
	int size;
	int offset = 0;
	int remaining = mtp_size;
	u32 addr = RHC32L110_FW_MTP_ADDR;

	while (remaining > 0) {
		ret = rhc32l110_fw_write_cmd(di, RHC32L110_FW_WRITE_CMD);
		ret += rhc32l110_fw_write_addr(di, addr);
		size = (remaining > RHC32L110_FW_PAGE_SIZE) ? RHC32L110_FW_PAGE_SIZE : remaining;
		ret += rhc32l110_fw_write_data(di, mtp_data + offset, size);
		if (ret) {
			hwlog_err("write mtp data fail\n");
			return ret;
		}

		offset += size;
		remaining -= size;
		addr += size;
	}

	return 0;
}

static void rhc32l110_fw_program_begin(struct rstm32g031_device_info *di)
{
	/* enable pin pull high */
	(void)gpio_direction_output(di->gpio_enable, 1);
	power_usleep(DT_USLEEP_5MS);

	/* reset pin pull low */
	(void)gpio_direction_output(di->gpio_reset, 1);
	power_usleep(DT_USLEEP_100US);

	/* reset pin pull high */
	(void)gpio_direction_output(di->gpio_reset, 0);
	power_msleep(DT_MSLEEP_50MS, 0, NULL);
}

static void rhc32l110_fw_program_end(struct rstm32g031_device_info *di)
{
	/* enable pin pull low */
	(void)gpio_direction_output(di->gpio_enable, 0);

	/* reset pin pull low */
	(void)gpio_direction_output(di->gpio_reset, 1);
	power_usleep(DT_USLEEP_100US);

	/* reset pin pull high */
	(void)gpio_direction_output(di->gpio_reset, 0);

	/* wait for program jump */
	power_msleep(DT_MSLEEP_100MS, 0, NULL);
}

static int rhc32l110_fw_update_mtp(struct rstm32g031_device_info *di,
	const u8 *mtp_data, int mtp_size)
{
	int i;

	if (!mtp_data || mtp_size <= 0)
		return -EINVAL;

	for (i = 0; i < RHC32L110_FW_RETRY_COUNT; i++) {
		hwlog_info("mtp update begin\n");
		di->in_fw_update = true;
		rhc32l110_fw_program_begin(di);
		if (rhc32l110_fw_erase(di))
			continue;
		if (rhc32l110_fw_write_mtp_data(di, mtp_data, mtp_size))
			continue;
		rhc32l110_fw_program_end(di);
		di->in_fw_update = false;
		if (rhc32l110_fw_get_ver_id(di) || (di->fw_ver != di->mtp_ver))
			continue;
		hwlog_info("mtp update end, retry time: %d\n", i + 1);
		return 0;
	}
	di->in_fw_update = false;
	return -EINVAL;
}

static int rhc32l110_fw_check_bootloader_mode(struct rstm32g031_device_info *di)
{
	int ret;
	u8 ack = 0;
	u8 data = 0;

	/* write ver cmd and wait ack */
	ret = rhc32l110_fw_write_cmd(di, RHC32L110_FW_GET_VER_CMD);
	if (ret) {
		hwlog_err("not work bootloader mode\n");
		return -EIO;
	}

	/* get data and wait ack */
	ret = rstm32g031_read_word_bootloader(di, &data, 1);
	ret += rstm32g031_read_word_bootloader(di, &ack, 1);
	hwlog_info("get_data=%x ack=0x%x\n", data, ack);
	hwlog_info("get data and wait ack ret=%d\n", ret);
	if (ret) {
		hwlog_err("not work bootloader mode\n");
		return -EIO;
	}

	hwlog_info("work bootloader mode\n");
	return 0;
}

int rhc32l110_fw_enter_bootloader_mode(struct rstm32g031_device_info *di)
{
	/* try to enter bootloader mode */
	rhc32l110_fw_program_begin(di);
	return rhc32l110_fw_check_bootloader_mode(di);
}

int rhc32l110_fw_get_ver_id(struct rstm32g031_device_info *di)
{
	u8 id = 0;
	int ret;

	ret = rstm32g031_read_byte(di, RHC32L110_FW_PATCH_VER_REG, &id);
	if (ret)
		return -EINVAL;
	di->fw_ver = id;

	ret = rstm32g031_read_byte(di, RHC32L110_FW_SUB_VER_REG, &id);
	if (ret)
		return -EINVAL;
	di->fw_ver |= id << 8; /* sub ver shift 8 bit */

	hwlog_info("rhc32l110 get_fw_ver:%x\n", di->fw_ver);
	return 0;
}

int rhc32l110_get_firmware(struct rstm32g031_device_info *di)
{
	static const char *rhc32l110_fw_name = "RSCP_HC32L110_FW.bin";
	const struct firmware *rhc32l110_fw = NULL;
	int ret;

	if (!di || !di->dev) {
		hwlog_err("di or dev is null\n");
		return -EINVAL;
	}

	ret = request_firmware(&rhc32l110_fw, rhc32l110_fw_name, di->dev);
	if (ret || !rhc32l110_fw || rhc32l110_fw->size < RHC32L110_FW_VER_LOW_INDEX) {
		hwlog_err("rhc32l110 request firmware failed\n");
		return -EINVAL;
	}

	di->fw_size = rhc32l110_fw->size;
	di->fw_data = kzalloc(di->fw_size, GFP_KERNEL);
	if (!di->fw_data) {
		hwlog_err("rhc32l110 kzalloc of fw_data failed\n");
		goto fw_data_init_fail;
	}
	ret = memcpy_s(di->fw_data, di->fw_size, rhc32l110_fw->data, rhc32l110_fw->size);
	if (ret) {
		hwlog_err("rhc32l110 fw cpy failed\n");
		kfree(di->fw_data);
		di->fw_data = NULL;
		goto fw_data_init_fail;
	}

	di->mtp_ver = di->fw_data[RHC32L110_FW_VER_HIGH_INDEX] << 8 |
		di->fw_data[RHC32L110_FW_VER_LOW_INDEX];
	hwlog_info("fw size:0x%x, mtp ver:%d\n", di->fw_size, di->mtp_ver);
	release_firmware(rhc32l110_fw);
	return 0;

fw_data_init_fail:
	di->fw_size = 0;
	release_firmware(rhc32l110_fw);
	return -ENOMEM;
}

int rhc32l110_fw_update_empty_mtp(struct rstm32g031_device_info *di)
{
	if (rhc32l110_fw_check_bootloader_mode(di))
		return 0;

	return rhc32l110_fw_update_mtp(di, di->fw_data, di->fw_size);
}

int rhc32l110_fw_update_latest_mtp(struct rstm32g031_device_info *di)
{
	if (!di || !di->fw_data)
		return -EINVAL;

	if (di->fw_ver >= di->mtp_ver) {
		hwlog_info("no need update_latest_mtp: fw_ver=%x mtp_ver=%x\n", di->fw_ver, di->mtp_ver);
		return 0;
	}

	hwlog_info("need update mtp: fw_ver=%x mtp_ver=%x\n", di->fw_ver, di->mtp_ver);
	return rhc32l110_fw_update_mtp(di, di->fw_data, di->fw_size);
}

int rhc32l110_fw_update_online_mtp(struct rstm32g031_device_info *di,
	u8 *mtp_data, int mtp_size, int ver_id)
{
	int ret;

	if (di->fw_ver >= ver_id)
		return 0;

	hwlog_info("need update mtp: ver_id=%x mtp_ver=%x\n", di->fw_ver, ver_id);
	ret = rhc32l110_fw_update_mtp(di, mtp_data, mtp_size);
	ret += rhc32l110_fw_get_ver_id(di);
	return ret;
}
