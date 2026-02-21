/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 * mcu_hc32l110_fw.c
 *
 * mcu_hc32l110 firmware driver
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
 * SPDX-License-Identifier: GPL-2.0
 */

#include "mcu_hc32l110.h"
#include "mcu_hc32l110_fw.h"
#include "mcu_hc32l110_i2c.h"
#include <chipset_common/hwpower/common_module/power_delay.h>
#include <chipset_common/hwpower/common_module/power_gpio.h>
#include <chipset_common/hwpower/common_module/power_printk.h>

#define hwlog_info(fmt, args...) pr_info("[mcu_hc32l110_fw]" fmt, ##args)
#define hwlog_err(fmt, args...) pr_err("[mcu_hc32l110_fw]" fmt, ##args)

static int g_cur_version = 0;
static int g_mcu_update_number = 0;
static u8* g_mcu_hc32l110_mtp_data = NULL;
static size_t g_mcu_hc32l110_mtp_data_size;
static struct mcu_upgrade mcu_update_delay_work;


static int get_mcu_img_fw_ver_id(int *ver)
{
	if (!g_mcu_hc32l110_mtp_data) {
		hwlog_err("get img ver err\n");
		return -EINVAL;
	}

	if (g_mcu_hc32l110_mtp_data_size <= MCU_FW_VER_ID_MASK) {
		hwlog_err("mcu img fw len err\n");
		return -EINVAL;
	}

	*ver = g_mcu_hc32l110_mtp_data[MCU_FW_VER_ID_MASK];
	return 0;
}

static int mcu_hc32l110_fw_get_version_id(struct mcu_hc32l110_device_info *di, int *ver)
{
	if (!di) {
		return -EINVAL;
	}

	if (!mcu_hc32l110_fw_get_ver_id(di)) {
		*ver = di->fw_ver_id;
		return 0;
	}
	return -EINVAL;
}

static int mcu_hc32l110_copy_firmware_to_mtp(const struct firmware *mcu_hc32l110_fw)
{
	int ret;
	int img_ver;
	if (!mcu_hc32l110_fw) {
		hwlog_err("invalid address value of mcu_hc32l110_fw!");
		return -EINVAL;
	}
	g_mcu_hc32l110_mtp_data_size = mcu_hc32l110_fw->size;
	g_mcu_hc32l110_mtp_data = kzalloc(g_mcu_hc32l110_mtp_data_size, GFP_KERNEL);
	if (!g_mcu_hc32l110_mtp_data) {
		hwlog_err("mcu_hc32l110 kzalloc of g_mcu_hc32l110_mtp_data failed!");
		return -ENOMEM;
	}
 
	ret = memcpy_s(g_mcu_hc32l110_mtp_data, g_mcu_hc32l110_mtp_data_size, mcu_hc32l110_fw->data, mcu_hc32l110_fw->size);
	if (ret) {
		hwlog_err("mcu_hc32l110 file_copy failed!");
		kfree(g_mcu_hc32l110_mtp_data);
		g_mcu_hc32l110_mtp_data = NULL;
		return ret;
	}
 
	ret = get_mcu_img_fw_ver_id(&img_ver);
	if (ret) {
		hwlog_err("%s:mcu_hc32l110 read version failed!!\n", __func__);
		kfree(g_mcu_hc32l110_mtp_data);
		g_mcu_hc32l110_mtp_data = NULL;
		return ret;
	}
 
	hwlog_info("mcu_hc32l110 read version %d", img_ver);
	return 0;
}

static void mcu_hc32l110_update_work(struct work_struct* work)
{
	int ret;
	struct mcu_upgrade* mcu = container_of(work, struct mcu_upgrade, mcu_upgrade_work.work);
	if (!mcu || !mcu->di) {
		return;
	}
	if (g_cur_version == 0) {
		ret = mcu_hc32l110_fw_get_version_id(mcu->di, &g_cur_version);
		set_mcu_fw_ver(ret ? 0 : g_cur_version);
	}
	if (mcu->di->hard_reset_none) {
		hwlog_info("hard reset did not be supported,can not update!");
		return;
	}
	static const char* mcu_hc32l110_fw_name = "mcu_hc32l110_fw.bin";
	const struct firmware *mcu_hc32l110_fw = NULL;
	ret = request_firmware(&mcu_hc32l110_fw, mcu_hc32l110_fw_name, mcu->di->dev);
	g_mcu_update_number++;

	if (ret || mcu_hc32l110_fw == NULL) {
		hwlog_err("mcu_hc32l110 request firmware failed!\n");
		if (g_mcu_update_number <= MAX_UPDATE_NUMBER) {
			(void)schedule_delayed_work(&mcu_update_delay_work.mcu_upgrade_work, msecs_to_jiffies(MCU_UPDATE_DELAY_TIME));
		}
		return;
	}

	if (mcu_hc32l110_copy_firmware_to_mtp(mcu_hc32l110_fw)) {
		release_firmware(mcu_hc32l110_fw);
		return;
	}

	if (mcu_hc32l110_fw_update_latest_mtp(mcu->di)) {
		hwlog_err("mcu_hc32l110 upgrade failed!!");
	}
	kfree(g_mcu_hc32l110_mtp_data);
	g_mcu_hc32l110_mtp_data = NULL;
	release_firmware(mcu_hc32l110_fw);
	hwlog_info("mcu_hc32l110 schedule_delayed_work success!!!");
}

void mcu_hc32l110_update_check(struct mcu_hc32l110_device_info *di)
{
	mcu_update_delay_work.di = di;
	INIT_DELAYED_WORK(&mcu_update_delay_work.mcu_upgrade_work, mcu_hc32l110_update_work);
	(void)schedule_delayed_work(&mcu_update_delay_work.mcu_upgrade_work, msecs_to_jiffies(MCU_UPDATE_DELAY_TIME));
	
	return;
}

static int mcu_hc32l110_fw_retry_write_bootloader(struct mcu_hc32l110_device_info *di, u8 *cmd)
{
	int i;

	/* retry max 10 times to wait i2c init ok */
	for (i = 0; i < MCU_HC32L110_FW_ACK_COUNT; i++) {
		if (!mcu_hc32l110_write_word_bootloader(di, cmd, MCU_HC32L110_FW_CMD_SIZE)) {
			return 0;
		}

		power_usleep(DT_USLEEP_2MS);
	}

	return -EIO;
}

static int mcu_hc32l110_fw_write_cmd(struct mcu_hc32l110_device_info *di, u16 cmd)
{
	int i;
	u8 ack;
	u8 buf[MCU_HC32L110_FW_CMD_SIZE] = { 0 };

	/* fill cmd */
	buf[0] = cmd >> 8;
	buf[1] = cmd & 0xFF;

	if (mcu_hc32l110_fw_retry_write_bootloader(di, buf)) {
		return -EIO;
	}

	for (i = 0; i < MCU_HC32L110_FW_ACK_COUNT; i++) {
		power_usleep(DT_USLEEP_1MS);
		ack = 0;
		(void)mcu_hc32l110_read_word_bootloader(di, &ack, 1);
		hwlog_info("write_cmd:ack=%x\n", ack);
		if (ack == MCU_HC32L110_FW_ACK_VAL) {
			hwlog_info("write_cmd succ: i=%d cmd=%x ack=%x\n", i, cmd, ack);
			return 0;
		}
	}

	hwlog_err("write_cmd fail: i=%d cmd=%x\n", i, cmd);
	return -EIO;
}

static int mcu_hc32l110_fw_write_addr(struct mcu_hc32l110_device_info *di, u32 addr)
{
	int i;
	u8 ack;
	u8 buf[MCU_HC32L110_FW_ADDR_SIZE] = { 0 };

	/* fill address */
	buf[0] = addr >> 24;
	buf[1] = (addr >> 16) & 0xFF;
	buf[2] = (addr >> 8) & 0xFF;
	buf[3] = (addr >> 0) & 0xFF;
	buf[4] = buf[0];
	for (i = 1; i < MCU_HC32L110_FW_ADDR_SIZE - 1; i++) {
		buf[4] ^= buf[i];
	}

	(void)mcu_hc32l110_write_word_bootloader(di, buf, MCU_HC32L110_FW_ADDR_SIZE);
	for (i = 0; i < MCU_HC32L110_FW_ACK_COUNT; i++) {
		power_usleep(DT_USLEEP_1MS);
		ack = 0;
		(void)mcu_hc32l110_read_word_bootloader(di, &ack, 1);
		hwlog_info("write_addr:ack=%x\n", ack);
		if (ack == MCU_HC32L110_FW_ACK_VAL) {
			hwlog_info("write_addr succ: i=%d addr=%x ack=%x\n", i, addr, ack);
			return 0;
		}
	}

	hwlog_err("write_addr fail: i=%d addr=%x\n", i, addr);
	return -EIO;
}

static int mcu_hc32l110_fw_erase(struct mcu_hc32l110_device_info *di)
{
	int ret;
	int i;
	u8 ack;
	u8 buf[MCU_HC32L110_FW_ERASE_SIZE] = { 0 };

	/* special erase */
	buf[0] = 0xFF;
	buf[1] = 0xFF;
	buf[2] = buf[0] ^ buf[1];

	ret = mcu_hc32l110_fw_write_cmd(di, MCU_HC32L110_FW_ERASE_CMD);
	if (ret) {
		return ret;
	}

	(void)mcu_hc32l110_write_word_bootloader(di, buf, MCU_HC32L110_FW_ERASE_SIZE);
	for (i = 0; i < MCU_HC32L110_FW_ERASE_ACK_COUNT; i++) {
		power_msleep(DT_MSLEEP_500MS, 0, NULL);
		ack = 0;
		(void)mcu_hc32l110_read_word_bootloader(di, &ack, 1);
		hwlog_info("erase:ack=%x\n", ack);
		if (ack == MCU_HC32L110_FW_ACK_VAL) {
			hwlog_info("erase succ: i=%d ack=%x\n", i, ack);
			return 0;
		}
	}

	hwlog_err("erase fail\n");
	return -EIO;
}

static int mcu_hc32l110_fw_write_data(struct mcu_hc32l110_device_info *di,
	const u8 *data, int len)
{
	int i;
	u8 ack;
	u8 checksum = len - 1;
	u8 buf[MCU_HC32L110_FW_PAGE_SIZE + 2] = {0};

	if (!data) {
		hwlog_err("data err \n");
		return -EINVAL;
	}

	if ((len > MCU_HC32L110_FW_PAGE_SIZE) || (len <= 0)) {
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

	(void)mcu_hc32l110_write_word_bootloader(di, buf, len + 2);
	for (i = 0; i < MCU_HC32L110_FW_ACK_COUNT; i++) {
		power_usleep(DT_USLEEP_20MS);
		ack = 0;
		(void)mcu_hc32l110_read_word_bootloader(di, &ack, 1);
		hwlog_info("write_data:ack=%x\n", ack);
		if (ack == MCU_HC32L110_FW_ACK_VAL) {
			hwlog_info("write_data succ: i=%d len=%d ack=%x\n", i, len, ack);
			return 0;
		}
	}

	hwlog_err("write_data fail\n");
	return -EIO;
}

static int mcu_hc32l110_fw_write_mtp_data(struct mcu_hc32l110_device_info *di,
	const u8 *mtp_data, int mtp_size)
{
	int offset = 0;
	int remaining = mtp_size;
	u32 addr = MCU_HC32L110_FW_MTP_ADDR;

	while (remaining > 0) {
		int ret;
		int size;
		ret = mcu_hc32l110_fw_write_cmd(di, MCU_HC32L110_FW_WRITE_CMD);
		ret += mcu_hc32l110_fw_write_addr(di, addr);
		size = (remaining > MCU_HC32L110_FW_PAGE_SIZE) ? MCU_HC32L110_FW_PAGE_SIZE : remaining;
		ret += mcu_hc32l110_fw_write_data(di, mtp_data + offset, size);
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

static int mcu_hc32l110_fw_program_begin(struct mcu_hc32l110_device_info *di)
{
	if (!di) {
		return -EINVAL;
	}

	/* mcu boot0 to low, soc pull high */
	mcu_hc32l110_hard_reset(di, HARD_RESET_FLAG_BOOT);
	power_msleep(DT_MSLEEP_500MS, 0, NULL);
	return 0;
}

static int mcu_hc32l110_fw_program_end(struct mcu_hc32l110_device_info *di)
{
	if (!di) {
		return -EINVAL;
	}

	hwlog_info("APP hard reset after upgrading\n");
	power_msleep(DT_MSLEEP_100MS, 0, NULL);
	mcu_hc32l110_hard_reset(di, HARD_RESET_FLAG_APP);
	power_msleep(DT_MSLEEP_2S, 0, NULL);

	return 0;
}

static int mcu_hc32l110_fw_notify_update_result(struct mcu_hc32l110_device_info *di)
{
	if (!di) {
		return -EINVAL;
	}

	int fw_version_id;
	int img_version_id;

	if (mcu_hc32l110_fw_get_version_id(di, &fw_version_id)) {
		hwlog_err("%s:get mcu_fw_ver err\n", __func__);
		set_mcu_fw_ver(0);
		return -EINVAL;
	}

	set_mcu_fw_ver(fw_version_id);
	if (get_mcu_img_fw_ver_id(&img_version_id)) {
		hwlog_err("%s:get soc_img_ver err\n", __func__);
		return -EINVAL;
	}

	if (fw_version_id == img_version_id) {
		hwlog_info("mcu update succeed\n");
		return 0;
	} else {
		hwlog_err("mcu ver check err: mcu_fw_ver:[%d], soc_img_ver:[%d]\n", fw_version_id, img_version_id);
		return -EINVAL;
	}
}

int mcu_hc32l110_fw_check_bootloader_mode(struct mcu_hc32l110_device_info *di)
{
	int ret;
	u8 ack = 0;
	u8 dat = 0;

	/* write ver cmd and wait ack */
	ret = mcu_hc32l110_fw_write_cmd(di, MCU_HC32L110_FW_GET_VER_CMD);
	if (ret) {
		hwlog_err("not work bootloader mode\n");
		return -EIO;
	}

	/* get data and wait ack */
	ret += mcu_hc32l110_read_word_bootloader(di, &ack, 1);
	ret += mcu_hc32l110_read_word_bootloader(di, &dat, 1);
	hwlog_info("mcu ack : %x ack=0x%x\n", dat, ack);
	if (ret) {
		hwlog_err("not work bootloader mode\n");
		return -EIO;
	}

	hwlog_info("work bootloader mode\n");
	return 0;
}
static int mcu_hc32l110_fw_update_mtp(struct mcu_hc32l110_device_info *di,
	const u8 *mtp_data, int mtp_size)
{
	if (!di) {
		return -EINVAL;
	}

	int i;
	for (i = 0; i < MCU_HC32L110_FW_RETRY_COUNT; i++) {
		hwlog_info("mcu update begin\n");
		if (mcu_hc32l110_fw_program_begin(di)) {
			continue;
		}
		if (mcu_hc32l110_fw_check_bootloader_mode(di)) {
			continue;
		}
		if (mcu_hc32l110_fw_erase(di)) {
			continue;
		}
		if (mcu_hc32l110_fw_write_mtp_data(di, mtp_data, mtp_size)) {
			continue;
		}
		if (mcu_hc32l110_fw_program_end(di)) {
			continue;
		}
		if (mcu_hc32l110_fw_notify_update_result(di)) {
			continue;
		}
		hwlog_info("mcu update end:retry time:%d\n", i + 1);
		return 0;
	}
	// /* enable pin pull low */
	(void)gpio_direction_output(di->gpio_boot0, 0);
	return -EINVAL;
}

int mcu_hc32l110_fw_get_dev_id(struct mcu_hc32l110_device_info *di)
{
	if (!di) {
		return -EINVAL;
	}
	int ret;
	u8 mcu_id_info[MCU_HC32L110_DATA_INFO_LEN] = {0};
	ret = mcu_hc32l110_read_block(di, MCU_HC32L110_FW_DEV_ID_REG,
		mcu_id_info, MCU_HC32L110_DATA_INFO_LEN); 
	if (ret) {
        hwlog_err("I2C read dev_id fail\n");
		return ret;
	}

	di->fw_dev_id = mcu_id_info[MCU_HC32L110_DATA_DEVICE_ID];
	hwlog_info("fw_dev_id: [%x]=0x%x\n", MCU_HC32L110_FW_DEV_ID_REG, di->fw_dev_id);

	if (di->fw_dev_id != 0x69) {
		return -EINVAL;
	}
	return 0;
}

int mcu_hc32l110_fw_get_ver_id(struct mcu_hc32l110_device_info *di)
{
	if (!di) {
		return -EINVAL;
	}
	u8 id = 0;
	int ret;

	ret = mcu_hc32l110_read_byte(di, MCU_HC32L110_FW_VER_ID_REG, &id);
	if (ret) {
		return ret;
	}
	di->fw_ver_id = id;
	hwlog_info("fw_ver_id: [%x]=0x%x\n", MCU_HC32L110_FW_VER_ID_REG, id);
	return 0;
}

int mcu_hc32l110_fw_get_boot_ver_id(struct mcu_hc32l110_device_info *di)
{
	if (!di) {
		return -EINVAL;
	}
	u8 id = 0;
	int ret;

	ret = mcu_hc32l110_read_byte(di, MCU_HC32L110_FW_BOOT_VER_ID_REG, &id);
	if (ret) {
		return ret;
	}
	di->fw_boot_ver_id = id;
	hwlog_info("fw_boot_ver_id: [%x]=0x%x\n", MCU_HC32L110_FW_BOOT_VER_ID_REG, id);
	return 0;
}

int mcu_hc32l110_fw_update_empty_mtp(struct mcu_hc32l110_device_info *di)
{
	int ret;
	if (!di) {
		return -EINVAL;
	}
	ret = mcu_hc32l110_fw_check_bootloader_mode(di);
	if (ret) {
		return ret;
	}
	return mcu_hc32l110_fw_update_mtp(di, g_mcu_hc32l110_mtp_data, g_mcu_hc32l110_mtp_data_size);
}

int mcu_hc32l110_fw_update_latest_mtp(struct mcu_hc32l110_device_info *di)
{
	if (!di) {
		return -EINVAL;
	}
	int fw_version_id;
	int ret;
	ret = get_mcu_img_fw_ver_id(&fw_version_id);
	ret |= mcu_hc32l110_fw_get_version_id(di, &g_cur_version);
	if ((ret) || (di->fw_ver_id == 0) || (di->fw_ver_id == 0xFF)) {
		hwlog_info("begin force update, ret=%d, di->fw_ver_id=%d\n", ret, di->fw_ver_id);
		return mcu_hc32l110_fw_update_mtp(di, g_mcu_hc32l110_mtp_data, g_mcu_hc32l110_mtp_data_size);
	}

	if (g_cur_version >= fw_version_id) {
		hwlog_info("no need update_latest_mtp: ver_id=%x mtp_ver=%x\n", di->fw_ver_id, fw_version_id);
		set_mcu_fw_ver(g_cur_version);
		return 0;
	}

	hwlog_info("normal update: ver %02X -> %02X\n", di->fw_ver_id, fw_version_id);
	return mcu_hc32l110_fw_update_mtp(di, g_mcu_hc32l110_mtp_data, g_mcu_hc32l110_mtp_data_size);
}

int mcu_hc32l110_fw_update_online_mtp(struct mcu_hc32l110_device_info *di,
	u8 *mtp_data, int mtp_size, int ver_id)
{
	if (!di) {
		return -EINVAL;
	}
	int ret;

	if (mcu_hc32l110_fw_get_ver_id(di)) {
		return -EINVAL;
	}
	if (di->fw_ver_id >= ver_id) {
		return 0;
	}
	hwlog_info("need update mtp: ver_id=%x mtp_ver=%x\n", di->fw_ver_id, ver_id);
	ret = mcu_hc32l110_fw_update_mtp(di, mtp_data, mtp_size);
	ret += mcu_hc32l110_fw_get_ver_id(di);
	return ret;
}
