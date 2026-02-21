/*
 * mt5788_fw.c
 *
 * mt5788 mtp, sram driver
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

#include "mt5788.h"
#include "mt5788_fw.h"

#define HWLOG_TAG wireless_mt5788_fw
HWLOG_REGIST();

#define MT5788_MTP_CRC_CHECK_TIMES     5
#define MT5788_MTP_PGM_CHECK_TIMES     50
#define MT5788_LOAD_BL_NORMAL          0

int mt5788_fw_sram_update(void *dev_data)
{
	int ret;
	struct mt5788_chip_info info = { 0 };
	struct mt5788_dev_info *di = dev_data;

	if (!di)
		return -ENODEV;

	ret = mt5788_get_chip_info(di, &info);
	if (!ret)
		hwlog_info("[chip_info] chip_id=0x%x, mtp_version=0x%x\n",
			info.chip_id, info.mtp_ver);

	return 0;
}

int mt5788_fw_get_mtp_ver(struct mt5788_dev_info *di, u32 *mtp_ver)
{
	return mt5788_read_dword(di, MT5788_MTP_VER_ADDR, mtp_ver);
}

static void mt5788_fw_ps_control(unsigned int ic_type, int scene, bool flag)
{
	static int ref_cnt;

	hwlog_info("[ps_control] ref_cnt=%d, flag=%d\n", ref_cnt, flag);
	if (flag)
		++ref_cnt;
	else if (--ref_cnt > 0)
		return;

	wlps_control(ic_type, scene, flag);
}

static int mt5788_fw_sram_write_data(struct mt5788_dev_info *di, u16 addr, u8 *data, u32 len)
{
	int ret;
	u32 size_to_write = 0;
	u32 remaining = len;
	u32 sram_addr = addr;
	u32 wr_already = 0;
	u8 wr_buff[MT5788_FW_PAGE_SIZE] = { 0 };

	while (remaining > 0) {
		if (remaining > MT5788_FW_PAGE_SIZE)
			size_to_write = MT5788_FW_PAGE_SIZE;
		else
			size_to_write = remaining;

		(void)memcpy_s(wr_buff, MT5788_FW_PAGE_SIZE, data + wr_already, size_to_write);
		ret = mt5788_write_block(di, sram_addr, wr_buff, size_to_write);
		if (ret)
			return ret;

		sram_addr += size_to_write;
		wr_already += size_to_write;
		remaining -= size_to_write;
	}
	return 0;
}

static int mt5788_fw_check_mtp_version(struct mt5788_dev_info *di)
{
	int ret;
	u32 mtp_ver = 0;

	ret = mt5788_fw_get_mtp_ver(di, &mtp_ver);
	if (ret)
		return ret;

	hwlog_info("[check_mtp_version] mtp_ver=0x%04x\n", mtp_ver);
	if (mtp_ver != di->fw_mtp.mtp_ver)
		return -ENXIO;

	return 0;
}

static int mt5788_fw_check_mtp_crc(struct mt5788_dev_info *di)
{
	u8 crc_result = 0;
	int i;
	int ret;

	ret = mt5788_write_word(di, MT5788_MTP_CRC_ADDR, di->fw_mtp.mtp_crc);
	ret += mt5788_write_word(di, MT5788_MTP_TOTAL_LENGTH_ADDR, di->fw_mtp.mtp_len);
	ret += mt5788_write_byte(di, MT5788_MTP_CRC_CHECK, MT5788_MTP_CRC_CHECK_VAL);
	for (i = 0; i < MT5788_MTP_CRC_CHECK_TIMES; i++) {
		power_usleep(DT_USLEEP_10MS);
		ret += mt5788_read_byte(di, MT5788_MTP_CRC_RESULT, &crc_result);
		if (ret) {
			hwlog_err("check_mtp_crc: read failed\n");
			return ret;
		}
		if (crc_result == MT5788_MTP_CRC_CHK_FAIL) {
			hwlog_err("check_mtp_crc: failed\n");
			return -EIO;
		} else if (crc_result == MT5788_MTP_CRC_CHK_SUCC) {
			hwlog_info("[check_mtp_crc:] success\n");
			return 0;
		}
	}
	hwlog_err("check_mtp_crc: timeout 0x%x\n", crc_result);
	return -EIO;
}

static int mt5788_fw_load_bootloader_enable_map(struct mt5788_dev_info *di, int type)
{
	int ret;

	/* config bootloader start environment */
	ret = mt5788_write_byte(di, 0x5800, 0x07);
	ret += mt5788_write_byte(di, 0x5244, 0x57);
	ret += mt5788_write_byte(di, 0x5200, 0x20);
	ret += mt5788_write_byte(di, 0x5209, 0x01);
	return ret;
}

static int mt5788_fw_disable_watchdog(struct mt5788_dev_info *di)
{
	u8 val0 = 0;
	u8 val1 = 0;
	int ret;
	int cnt = 24;

	do {
		ret = mt5788_write_byte(di, MT5788_MTP_WDT_ADDR, MT5788_MTP_WDT_CLOSE_EN);
		ret += mt5788_read_byte(di, MT5788_MTP_WDT_ADDR, &val0);
		power_usleep(DT_USLEEP_1MS);
		ret += mt5788_read_byte(di, MT5788_MTP_WDT_ADDR, &val1);
		if ((ret == 0) && (val0 == MT5788_MTP_WDT_CLOSE_FLAG) &&
			(val1 == MT5788_MTP_WDT_CLOSE_FLAG))
			return 0;
	} while (--cnt > 0);
	return -EIO;
}

static int mt5788_fw_load_bootloader(struct mt5788_dev_info *di, int type)
{
	int ret;
	u16 chipid = 0;
	u16 addr = MT5788_BTLOADER_ADDR;
	u8 *data = (u8 *)g_mt5788_bootloader;
	u32 size = MT5788_FW_BTL_SIZE;
	u16 chipid_addr = MT5788_MTP_CHIPID_ADDR;

	ret = mt5788_fw_disable_watchdog(di);
	if (ret) {
		hwlog_err("load bootloader: diable watchdog failed %d\n", ret);
		return ret;
	}

	ret = mt5788_fw_load_bootloader_enable_map(di, type);
	if (ret) {
		hwlog_err("load bootloader: enable map failed %d\n", ret);
		return ret;
	}

	ret = mt5788_fw_sram_write_data(di, addr, data, size);
	if (ret) {
		hwlog_err("load bootloader: load sram data failed %d\n", ret);
		return ret;
	}

	ret = mt5788_write_byte(di, 0x5200, 0x90); /* start bootloader */
	if (ret) {
		hwlog_err("load bootloader: start failed\n");
		return -EIO;
	}
	
	power_msleep(DT_MSLEEP_10MS, 0, NULL); /* typically 10ms */
	ret = mt5788_read_word(di, chipid_addr, &chipid);
	if (ret || (chipid != MT5788_MTP_CHIPID_VALUE)) {
		hwlog_err("load bootloader: run bootloader failed,chipid=0x%x\n", chipid);
		return -EIO;
	}

	hwlog_info("load bootloader: success\n");
	return 0;
}

static int mt5788_fw_load_mtp(struct mt5788_dev_info *di, const u8 *mtp_data, u16 mtp_size)
{
	u32 i;
	int ret;
	u16 wr_already = 0;
	u16 status = 0;
	u16 addr = 0;
	u16 checksum;
	u16 remaining = mtp_size;
	u16 size_to_write;
	u8 buff[MT5788_MTP_BUFF_SIZE] = { 0 };

	while (remaining > 0) {
		(void)memset_s(buff, MT5788_MTP_BUFF_SIZE, 0, MT5788_MTP_BUFF_SIZE);
		size_to_write = remaining > MT5788_MTP_BUFF_SIZE ?
			MT5788_MTP_BUFF_SIZE : remaining;
		(void)memcpy_s(buff, MT5788_MTP_BUFF_SIZE, (mtp_data + wr_already), size_to_write);
		checksum = addr + size_to_write;
		for (i = 0; i < size_to_write; i++)
			checksum += buff[i];

		ret = mt5788_write_word(di, MT5788_MTP_PGM_ADDR_ADDR, addr);
		ret += mt5788_write_word(di, MT5788_MTP_PGM_LEN_ADDR, size_to_write);
		ret += mt5788_write_word(di, MT5788_MTP_PGM_CHKSUM_ADDR, checksum);
		ret += mt5788_fw_sram_write_data(di, MT5788_MTP_PGM_DATA_ADDR, buff, size_to_write);
		ret += mt5788_write_word(di, MT5788_MTP_PGM_CMD_ADDR, MT5788_MTP_PGM_CMD);
		if (ret) {
			hwlog_err("load_mtp: write failed\n");
			return ret;
		}

		for (i = 0; i < MT5788_MTP_PGM_CHECK_TIMES; i++) {
			ret = mt5788_read_word(di, MT5788_MTP_PGM_CMD_ADDR, &status);
			if (ret) {
				hwlog_err("load_mtp: read failed\n");
				return ret;
			}

			if (status == MT5788_MTP_PGM_SUCESS)
				break;

			power_usleep(DT_USLEEP_2MS);
		}

		if (status != MT5788_MTP_PGM_SUCESS) {
			hwlog_err("load_mtp: status failed\n");
			return -EIO;
		}

		wr_already += size_to_write;
		addr += size_to_write;
		remaining -= size_to_write;
	}

	return 0;
}

static int mt5788_fw_chip_reset(struct mt5788_dev_info *di)
{
	int ret;

	ret = mt5788_write_byte(di, 0x5244, 0x57);
	ret += mt5788_write_byte(di, 0x5209, 0x00);
	ret += mt5788_write_byte(di, 0x5200, 0x80);
	power_usleep(DT_USLEEP_10MS);
	return ret;
}

static int mt5788_fw_check_mtp(void *dev_data)
{
	int ret = 0;
	u16 chip_id = 0;
	struct mt5788_dev_info *di = dev_data;

	if (!di)
		return -ENODEV;

	if (di->g_val.mtp_latest)
		return 0;

	if (power_cmdline_is_factory_mode() && mt5788_rx_is_tx_exist(di) &&
		!mt5788_fw_check_mtp_version(di))
		goto succ;

	mt5788_disable_irq_nosync(di);
	mt5788_fw_ps_control(di->ic_type, WLPS_RX_EXT_PWR, true);
	wlps_control(WLTRX_IC_MAIN, WLPS_RX_SW, false);
	power_msleep(DT_MSLEEP_100MS, 0, NULL); /* for power on */

	ret = mt5788_get_chip_id(di, &chip_id);
	if (ret || (chip_id != MT5788_CHIP_ID)) {
		ret = EINVAL;
		goto exit;
	}

	ret = mt5788_fw_check_mtp_version(di);
	if (ret) {
		hwlog_err("check_mtp: mtp_ver mismatch\n");
		goto exit;
	}

	ret = mt5788_fw_check_mtp_crc(di);
	if (ret) {
		hwlog_err("check_mtp: mtp_crc mismatch\n");
		goto exit;
	}

succ:
	di->g_val.mtp_latest = true;
	hwlog_info("[check_mtp] mtp latest\n");

exit:
	mt5788_enable_irq(di);
	mt5788_fw_ps_control(di->ic_type, WLPS_RX_EXT_PWR, false);
	return ret;
}

static int mt5788_fw_erase_mtp(struct mt5788_dev_info *di)
{
	int ret;
	u16 result = 0;
	int cnt = MT5788_MTP_ERASE_CNT;

	ret = mt5788_write_word(di, MT5788_MTP_ERASE_ADDR, MT5788_MTP_ERASE_EN);
	do {
		ret = mt5788_read_word(di, MT5788_MTP_ERASE_ADDR, &result);
		if (ret)
			goto exit;
		if (result == MT5788_MTP_ERASE_SUCC)
			return 0;
		power_usleep(MT5788_MTP_ERASE_DEALY);
	} while (--cnt > 0);

exit:
	hwlog_err("fw_erase_mtp: failed\n");
	return ret;
}

static int mt5788_fw_program_mtp(struct mt5788_dev_info *di, const u8 *mtp_data, u16 mtp_size)
{
	int ret;

	if (di->g_val.mtp_latest)
		return 0;

	mt5788_disable_irq_nosync(di);
	mt5788_fw_ps_control(di->ic_type, WLPS_PROC_OTP_PWR, true);
	(void)mt5788_chip_enable(true, di);
	power_msleep(DT_MSLEEP_100MS, 0, NULL); /* for power on */

	ret = mt5788_fw_load_bootloader(di, MT5788_LOAD_BL_NORMAL);
	if (ret)
		goto exit;

	ret = mt5788_fw_erase_mtp(di);
	if (ret)
		goto exit;

	ret = mt5788_fw_load_mtp(di, mtp_data, mtp_size);
	if (ret)
		goto exit;

	power_usleep(DT_USLEEP_10MS);
	ret = mt5788_fw_chip_reset(di);
	if (ret) {
		hwlog_err("fw_program_mtp: reset mcu fail\r\n");
		goto exit;
	}

	power_usleep(DT_USLEEP_10MS);
	mt5788_fw_ps_control(di->ic_type, WLPS_PROC_OTP_PWR, false);
	power_usleep(DT_USLEEP_10MS); /* for power off, typically 5ms */
	mt5788_fw_ps_control(di->ic_type, WLPS_PROC_OTP_PWR, true);
	power_msleep(DT_MSLEEP_100MS, 0, NULL); /* for power on, typically 50ms */

	ret = mt5788_fw_check_mtp_version(di);
	ret += mt5788_fw_check_mtp_crc(di);
	if (ret) {
		hwlog_err("fw_program_mtp: failed\n");
		goto exit;
	}

	di->g_val.mtp_latest = true;
	hwlog_info("[fw_program_mtp] succ\n");

exit:
	mt5788_fw_chip_reset(di);
	mt5788_enable_irq(di);
	mt5788_fw_ps_control(di->ic_type, WLPS_PROC_OTP_PWR, false);
	return ret;
}

static void mt5788_fw_mtp_check_work(struct work_struct *work)
{
	int i;
	int ret;
	struct mt5788_dev_info *di = container_of(work,
		struct mt5788_dev_info, mtp_check_work.work);

	if (!di)
		return;

	if (!power_cmdline_is_factory_mode() && mt5788_rx_is_tx_exist(di)) {
		hwlog_info("[mtp_check_work] tx exist\n");
		di->g_val.mtp_chk_complete = true;
		return;
	}

	power_wakeup_lock(di->fw_wakelock, false);
	di->g_val.mtp_chk_complete = false;
	ret = mt5788_fw_check_mtp(di);
	if (!ret) {
		hwlog_info("[mtp_check_work] succ\n");
		goto exit;
	}

	/* program for 3 times until it's ok */
	for (i = 0; i < 3; i++) {
		ret = mt5788_fw_program_mtp(di, g_mt5788_fw, (u16)ARRAY_SIZE(g_mt5788_fw));
		if (ret)
			continue;

		hwlog_info("[mtp_check_work] update mtp succ, cnt=%d\n", i + 1);
		goto exit;
	}

	hwlog_err("mtp_check_work: update mtp failed\n");

exit:
	di->g_val.mtp_chk_complete = true;
	power_wakeup_unlock(di->fw_wakelock, false);
}

void mt5788_fw_mtp_check(struct mt5788_dev_info *di)
{
	u32 mtp_check_delay;

	if (power_cmdline_is_powerdown_charging_mode() ||
		(!power_cmdline_is_factory_mode() && mt5788_rx_is_tx_exist(di))) {
		di->g_val.mtp_chk_complete = true;
		return;
	}

	if (!power_cmdline_is_factory_mode())
		mtp_check_delay = di->mtp_check_delay.user;
	else
		mtp_check_delay = di->mtp_check_delay.fac;

	INIT_DELAYED_WORK(&di->mtp_check_work, mt5788_fw_mtp_check_work);
	di->fw_wakelock = power_wakeup_source_register(di->dev, "wl_fw_wakelock");
	schedule_delayed_work(&di->mtp_check_work, msecs_to_jiffies(mtp_check_delay));
}

static int mt5788_fw_rx_program_mtp(unsigned int proc_type, void *dev_data)
{
	int ret;
	struct mt5788_dev_info *di = dev_data;

	if (!di)
		return -ENODEV;

	hwlog_info("[rx_program_mtp] type=%u\n", proc_type);

	if (!di->g_val.mtp_chk_complete)
		return -EPERM;

	di->g_val.mtp_chk_complete = false;
	ret = mt5788_fw_program_mtp(di, g_mt5788_fw, (u16)ARRAY_SIZE(g_mt5788_fw));
	if (!ret)
		hwlog_info("[rx_program_mtp] succ\n");

	di->g_val.mtp_chk_complete = true;

	return ret;
}

static int mt5788_mtp_crc_calc(const u8 *buf, u16 mtp_size)
{
	int i, j;
	u16 crc = MT5788_MTP_CRC_INIT;

	/* 2byte cycle, 1byte = 8bit */
	for (j = 0; j < mtp_size; j += 2) {
		crc ^= (buf[j + 1] << 8);
		for (i = 0; i < 8; i++)
			crc = (crc & MT5788_MTP_CRC_HIGHEST_BIT) ?
				(((crc << 1) & POWER_MASK_WORD) ^ MT5788_MTP_CRC_SEED) : (crc << 1);
		crc ^= (buf[j] << 8);
		for (i = 0; i < 8; i++)
			crc = (crc & MT5788_MTP_CRC_HIGHEST_BIT) ?
				(((crc << 1) & POWER_MASK_WORD) ^ MT5788_MTP_CRC_SEED) : (crc << 1);
	}

	hwlog_info("[mtp_crc_calc] crc=0x%x\n", crc);
	return crc;
}

static int mt5788_fw_get_mtp_status(unsigned int *status, void *dev_data)
{
	int ret;
	struct mt5788_dev_info *di = dev_data;

	if (!di || !status)
		return -EINVAL;

	di->g_val.mtp_chk_complete = false;
	ret = mt5788_fw_check_mtp(di);
	if (!ret)
		*status = WIRELESS_FW_PROGRAMED;
	else
		*status = WIRELESS_FW_ERR_PROGRAMED;

	di->g_val.mtp_chk_complete = true;

	return 0;
}

static ssize_t mt5788_fw_write(void *dev_data, const char *buf, size_t size)
{
	int hdr_size;
	u16 crc;
	struct power_fw_hdr *hdr = NULL;
	struct mt5788_dev_info *di = dev_data;

	if (!di || !buf)
		return -EINVAL;

	hdr = (struct power_fw_hdr *)buf;
	hdr_size = sizeof(struct power_fw_hdr);
	crc = mt5788_mtp_crc_calc((const u8 *)hdr + hdr_size, (u16)hdr->bin_size);
	hwlog_info("[fw_write] bin_size=%ld version_id=0x%x crc_id=0x%x\n",
		hdr->bin_size, hdr->version_id, hdr->crc_id);

	if ((hdr->unlock_val != WLTRX_UNLOCK_VAL) || (hdr->fw_size != hdr->bin_size) ||
		(hdr->crc_id != crc)) {
		hwlog_err("fw_write: config mismatch\n");
		return -EINVAL;
	}

	di->g_val.mtp_latest = false;
	di->fw_mtp.mtp_ver = hdr->version_id;
	di->fw_mtp.mtp_crc = hdr->crc_id;
	di->fw_mtp.mtp_len = hdr->bin_size;

	(void)mt5788_fw_check_mtp(di);
	if (di->g_val.mtp_latest)
		return size;

	(void)mt5788_fw_program_mtp(di, (const u8 *)hdr + hdr_size, (u16)hdr->bin_size);
	return size;
}

static bool mt5788_fw_is_mtp_chk_complete(void *dev_data)
{
	struct mt5788_dev_info *di = dev_data;

	if (!di) {
		hwlog_err("is_mtp_chk_complete: di null\n");
		return false;
	}

	return di->g_val.mtp_chk_complete;
}

static struct wireless_fw_ops g_mt5788_fw_ops = {
	.ic_name                = "mt5788",
	.program_fw             = mt5788_fw_rx_program_mtp,
	.get_fw_status          = mt5788_fw_get_mtp_status,
	.check_fw               = mt5788_fw_check_mtp,
	.write_fw               = mt5788_fw_write,
	.is_mtp_chk_complete    = mt5788_fw_is_mtp_chk_complete,
};

int mt5788_fw_ops_register(struct wltrx_ic_ops *ops, struct mt5788_dev_info *di)
{
	if (!ops || !di)
		return -ENODEV;

	ops->fw_ops = kzalloc(sizeof(*(ops->fw_ops)), GFP_KERNEL);
	if (!ops->fw_ops)
		return -ENODEV;

	di->fw_mtp.mtp_ver = MT5788_FW_VER;
	di->fw_mtp.mtp_crc = MT5788_FW_CRC;
	di->fw_mtp.mtp_len = (u16)ARRAY_SIZE(g_mt5788_fw);
	(void)memcpy_s(ops->fw_ops, sizeof(g_mt5788_fw_ops), &g_mt5788_fw_ops, sizeof(g_mt5788_fw_ops));
	ops->fw_ops->dev_data = (void *)di;
	return wireless_fw_ops_register(ops->fw_ops, di->ic_type);
}
