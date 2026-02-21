// SPDX-License-Identifier: GPL-2.0
/*
 * bk96285_fw.c
 *
 * bk96285 fw driver
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

#include "bk96285.h"
#include "bk96285_chip.h"
#include "bk96285_fw.h"

#define HWLOG_TAG wireless_bk96285_fw
HWLOG_REGIST();

int bk96285_fw_sram_update(void *dev_data)
{
	int ret;
	struct bk96285_chip_info info = { 0 };

	ret = bk96285_get_chip_info(dev_data, &info);
	if (!ret)
		hwlog_info("[chip_info] chip_id=0x%x mtp_ver=0x%04x\n",
			info.chip_id, info.mtp_ver);

	return 0;
}
static u16 bk96285_pgm_xor(u8 *buffer, u16 len)
{
	u16 val = 0;
	u16 i;

	for (i = 0; i < len; i += BK96285_XOR_LEN)
		val ^= (u16)buffer[i] | (((u16)buffer[i + BK96285_MSG_BIT]) << BK96285_SHIFT_8BIT);

	return (val & BK96285_U16_MAX);
}

static u8 bk96285_pgm_state(struct bk96285_dev_info *di)
{
	u16 i;
	u8 state[BK96285_PGM_STATE_LEN] = { 0 };
	u16 sram_addr_h = BK96285_SRAM_ADDR_H;
	u16 sram_addr_l = BK96285_SRAM_ADDR_L;
	u8 addr_high[BK96285_HIGH_ADDR_NUM] = { 0 };

	addr_high[BK96285_HIGH_ADDR0] = sram_addr_h >> BK96285_HIGH_8BIT_ADDR;
	addr_high[BK96285_HIGH_ADDR1] = sram_addr_h & BK96285_LOW_8BIT_ADDR;

	bk96285_write_block(di, BK96285_ADDR_HIGH_BASE, addr_high, BK96285_HIGH_ADDR_NUM);
	for (i = 0; i < BK96285_RETRY_TIMES; i++) {
		power_usleep(DT_USLEEP_1MS);
		bk96285_read_block(di, sram_addr_l + BK96285_PGM_STATE_ADDR,
			state, BK96285_FOUR_BYTE_ALIGN);
		if ((state[0] != BK96285_PGM_STATE_BUSY) && (state[0] != BK96285_PGM_STATE_NONE))
			return state[0];
	}

	return BK96285_PGM_STATE_TIMEOUT;
}

static void bk96285_fw_ps_control(unsigned int ic_type, int scene, bool flag)
{
	static int ref_cnt;

	hwlog_info("[ps_control] ref_cnt=%d, flag=%d\n", ref_cnt, flag);
	if (flag)
		++ref_cnt;
	else if (--ref_cnt > 0)
		return;

	wlps_control(ic_type, scene, flag);
}

static u32 bk96285_func_crc32(u32 data, u32 crc_init)
{
	u32 crc_poly = BK96285_CRC_POLY;
	u32 crc_init_1;
	u32 crc_init_2;
	u8 i;

	for (i = 0; i < BK96285_32BIT; i++) {
		crc_init_1 = crc_init << BK96285_SHIFT_1BIT;
		crc_init_2 = crc_init >> BK96285_SHIFT_31BIT;
		crc_init = crc_init_1 ^ (((crc_init_2 & BIT(0)) ^ ((data >> i) & BIT(0))) ==
			1 ? BK96285_UINT_MAX & crc_poly : 0);
	}
	return crc_init;
}

static int bk96285_sys_reset_ctrl(struct bk96285_dev_info *di, bool en)
{
	if (en)
		return bk96285_write_byte(di, BK96285_REG_RST, BK96285_RST_PW0);

	return bk96285_write_byte(di, BK96285_REG_RST, BK96285_RST_PW1);
}

static int bk96285_bool_sel(struct bk96285_dev_info *di, bool en)
{
	int ret;
	u8 data[BK96285_BYTE_LEN] = { 0 };
	u16 addr_sel_h = BK96285_REG_ATE >> BK96285_HIGH_16BIT_ADDR;
	u16 addr_sel_l = BK96285_REG_ATE & BK96285_LOW_16BIT_ADDR;
	u8 addr_high[BK96285_HIGH_ADDR_NUM] = { 0 };

	addr_high[BK96285_HIGH_ADDR0] = addr_sel_h >> BK96285_HIGH_8BIT_ADDR;
	addr_high[BK96285_HIGH_ADDR1] = addr_sel_h & BK96285_LOW_8BIT_ADDR;

	bk96285_write_block(di, BK96285_ADDR_HIGH_BASE, addr_high, BK96285_HIGH_ADDR_NUM);

	ret = bk96285_read_block(di, addr_sel_l, data, BK96285_FOUR_BYTE_ALIGN);
	if (ret < 0)
		return ret;

	if (en)
		data[0] |= BK96285_ATE_BOOT_BIT;
	else
		data[0] &= BK96285_U8_MAX;

	return bk96285_write_block(di, addr_sel_l, data, BK96285_FOUR_BYTE_ALIGN);
}

static int bk96285_pgm_chip_erase(struct bk96285_dev_info *di)
{
	u8 state;
	bk96285_pgm_pkt_type pgm = { 0 };
	u16 sram_addr_h = BK96285_SRAM_ADDR_H;
	u16 sram_addr_l = BK96285_SRAM_ADDR_L;
	u8 addr_high[BK96285_HIGH_ADDR_NUM] = { 0 };
	u8 pgm_erase[BK96285_BYTE_LEN] = { 0 };

	addr_high[BK96285_HIGH_ADDR0] = sram_addr_h >> BK96285_HIGH_8BIT_ADDR;
	addr_high[BK96285_HIGH_ADDR1] = sram_addr_h & BK96285_LOW_8BIT_ADDR;

	bk96285_write_block(di, BK96285_ADDR_HIGH_BASE, addr_high, BK96285_HIGH_ADDR_NUM);

	pgm.type.xor = bk96285_pgm_xor(&pgm.value[BK96285_PGM_XOR_INDEX], BK96285_PGM_XOR_LEN);
	pgm.type.xor ^= BK96285_PGM_CMD_CHIP_ERASE << BK96285_SHIFT_8BIT;
	bk96285_write_block(di, sram_addr_l + BK96285_PGM_STATE_ADDR,
		&pgm.value[0], BK96285_PGM_HEADER_SIZE);
	bk96285_read_block(di, sram_addr_l + BK96285_PGM_STATE_ADDR, pgm_erase, BK96285_READ_LEN);
	bk96285_read_block(di, sram_addr_l + BK96285_PGM_STATE_ADDR, pgm_erase, BK96285_READ_LEN);

	pgm_erase[BK96285_MSG_BIT] = BK96285_PGM_CMD_CHIP_ERASE;
	bk96285_write_block(di, sram_addr_l + BK96285_PGM_STATE_ADDR, pgm_erase, BK96285_WRITE_LEN);
	state = bk96285_pgm_state(di);
	if (state != BK96285_PGM_STATE_DONE) {
		hwlog_err("pgm chip erase: state %02x\n", state);
		return -EINVAL;
	}
	return 0;
}

static void bk96285_pgm_info_prev(struct bk96285_dev_info *di, bk96285_pgm_pkt_type *pgm,
	u16 sram_addr_l)
{
	u8 pgm_info[BK96285_BYTE_LEN] = { 0 };
	u8 addr_high[BK96285_HIGH_ADDR_NUM] = { 0 };

	addr_high[BK96285_HIGH_ADDR0] = BK96285_SRAM_ADDR_H >> BK96285_HIGH_8BIT_ADDR;
	addr_high[BK96285_HIGH_ADDR1] = BK96285_SRAM_ADDR_H & BK96285_LOW_8BIT_ADDR;
	bk96285_write_block(di, BK96285_ADDR_HIGH_BASE, addr_high, BK96285_HIGH_ADDR_NUM);
	pgm->type.xor = bk96285_pgm_xor(&pgm->value[BK96285_PGM_XOR_INDEX], BK96285_PGM_XOR_LEN);
	pgm->type.xor ^= BK96285_PGM_CMD_INFO << BK96285_SHIFT_8BIT;
	bk96285_write_block(di, sram_addr_l + BK96285_PGM_STATE_ADDR,
		&pgm->value[0], BK96285_PGM_HEADER_SIZE);
	bk96285_read_block(di, sram_addr_l + BK96285_PGM_STATE_ADDR, pgm_info, BK96285_READ_LEN);
	bk96285_read_block(di, sram_addr_l + BK96285_PGM_STATE_ADDR, pgm_info, BK96285_READ_LEN);
	pgm_info[BK96285_MSG_BIT] = BK96285_PGM_CMD_INFO;
	bk96285_write_block(di, sram_addr_l + BK96285_PGM_STATE_ADDR, pgm_info, BK96285_WRITE_LEN);
}

static int bk96285_get_pgm_info(struct bk96285_dev_info *di, char *info)
{
	u8 tmp;
	u8 i;
	bk96285_pgm_pkt_type pgm = { 0 };
	u16 sram_addr_l = BK96285_SRAM_ADDR_L;

	(void)bk96285_pgm_info_prev(di, &pgm, sram_addr_l);
	tmp = bk96285_pgm_state(di);
	if (tmp != BK96285_PGM_STATE_DONE) {
		hwlog_err("pgm_info:: pgm info state %02x\n", tmp);
		return -EINVAL;
	}
	bk96285_read_block(di, sram_addr_l + BK96285_PGM_STATE_ADDR,
		(u8 *)pgm.value, BK96285_PGM_HEADER_SIZE);
	for (i = 0; i < pgm.type.len; i += BK96285_PGM_PKG_SIZE) {
		if (pgm.type.len - i > BK96285_PGM_PKG_SIZE)
			bk96285_read_block(di, sram_addr_l + i + BK96285_PGM_HEADER_SIZE,
				&pgm.type.msg[i], BK96285_PGM_PKG_SIZE);
		else
			bk96285_read_block(di, sram_addr_l + i + BK96285_PGM_HEADER_SIZE,
				&pgm.type.msg[i], pgm.type.len - i);
	}
	if (!bk96285_pgm_xor((u8 *)pgm.value, pgm.type.len + BK96285_PGM_HEADER_SIZE)) {
		tmp = 0;
		for (i = 0; i < pgm.type.len; i++) {
			if (pgm.type.msg[i] != 0) {
				info[tmp] = pgm.type.msg[i];
				tmp++;
			}
		}
	}
	return 0;
}

static void bk96285_pgm_write_data(struct bk96285_dev_info *di, u8 sel, bk96285_pgm_pkt_type *pgm, u16 len)
{
	u16 j;
	u16 sram_addr_l = BK96285_SRAM_ADDR_L;
	u8 pgm_write_cmd[BK96285_BYTE_LEN] = { 0 };

	for (j = 0; j < len; j += BK96285_16BIT) {
		if (len - j > BK96285_16BIT)
			bk96285_write_block(di, sram_addr_l + BK96285_PGM_STATE_ADDR + j,
				pgm->value + j, BK96285_16BIT);
		else
			bk96285_write_block(di, sram_addr_l + BK96285_PGM_STATE_ADDR + j,
				pgm->value + j, len - j);
	}
	bk96285_read_block(di, sram_addr_l + BK96285_PGM_STATE_ADDR,
		pgm_write_cmd, BK96285_READ_LEN);
	bk96285_read_block(di, sram_addr_l + BK96285_PGM_STATE_ADDR,
		pgm_write_cmd, BK96285_READ_LEN);

	pgm_write_cmd[BK96285_MSG_BIT] = sel;
	bk96285_write_block(di, sram_addr_l + BK96285_PGM_STATE_ADDR, pgm_write_cmd, BK96285_WRITE_LEN);
}

static int bk96285_pgm_write(struct bk96285_dev_info *di, u32 addr, const u8 *buffer, u16 len, u8 sel)
{
	u16 i;
	u8 state;
	u16 offset;
	bk96285_pgm_pkt_type pgm = { 0 };
	u8 addr_high[BK96285_HIGH_ADDR_NUM] = { 0 };

	addr_high[BK96285_HIGH_ADDR0] = BK96285_SRAM_ADDR_H >> BK96285_HIGH_8BIT_ADDR;
	addr_high[BK96285_HIGH_ADDR1] = BK96285_SRAM_ADDR_H & BK96285_LOW_8BIT_ADDR;
	bk96285_write_block(di, BK96285_ADDR_HIGH_BASE, addr_high, BK96285_HIGH_ADDR_NUM);
	for (i = 0; i < len; i += BK96285_PGM_FRIMWARE_SIZE) {
		(void)memset_s(pgm.value, BK96285_PGM_MSG_SIZE + BK96285_PGM_HEADER_SIZE,
			0, sizeof(pgm.value));
		pgm.type.addr = (addr + i) / BK96285_PGM_WORD;
		offset = BK96285_PGM_FRIMWARE_SIZE;
		if (len < (i + BK96285_PGM_FRIMWARE_SIZE))
			offset = len - i;

		pgm.type.len = offset / BK96285_PGM_WORD;
		(void)memcpy_s(pgm.type.msg, BK96285_PGM_MSG_SIZE, &buffer[i], offset);
		pgm.type.xor = bk96285_pgm_xor(&pgm.value[BK96285_PGM_XOR_INDEX],
			offset + BK96285_PGM_XOR_LEN);
		pgm.type.xor ^= (sel << BK96285_SHIFT_8BIT);
		(void)bk96285_pgm_write_data(di, sel, &pgm, offset + BK96285_PGM_HEADER_SIZE);
		state = bk96285_pgm_state(di);
		if (state != BK96285_PGM_STATE_DONE) {
			hwlog_err("pgm write: state %02x, cmd %02x i=%u\n", state, sel, i);
			return -EINVAL;
		}
	}
	hwlog_info("[pgm write] firmware write successful\n");
	return 0;
}

static int bk96285_pgm_set_margin(struct bk96285_dev_info *di, u8 buffer)
{
	u8 state;
	bk96285_pgm_pkt_type pgm = { 0 };
	u16 sram_addr_h = BK96285_SRAM_ADDR_H;
	u16 sram_addr_l = BK96285_SRAM_ADDR_L;
	u8 addr_high[BK96285_HIGH_ADDR_NUM] = { 0 };
	u8 margin_cmd[BK96285_BYTE_LEN] = { 0 };

	addr_high[BK96285_HIGH_ADDR0] = sram_addr_h >> BK96285_HIGH_8BIT_ADDR;
	addr_high[BK96285_HIGH_ADDR1] = sram_addr_h & BK96285_LOW_8BIT_ADDR;

	bk96285_write_block(di, BK96285_ADDR_HIGH_BASE, addr_high, BK96285_HIGH_ADDR_NUM);

	pgm.type.addr = 0x00;
	pgm.type.len = BK96285_PGM_TYPE_LEN;
	(void)memcpy_s(pgm.type.msg, BK96285_PGM_MSG_SIZE, &buffer, pgm.type.len);

	pgm.type.xor = bk96285_pgm_xor(&pgm.value[BK96285_PGM_XOR_INDEX],
		BK96285_PGM_XOR_LEN + BK96285_READ_LEN);
	pgm.type.xor ^= BK96285_PGM_CMD_MARGIN << BK96285_SHIFT_8BIT;

	bk96285_write_block(di, sram_addr_l + BK96285_PGM_STATE_ADDR,
		&pgm.value[0], BK96285_READ_LEN + BK96285_PGM_HEADER_SIZE);
	bk96285_read_block(di, sram_addr_l + BK96285_PGM_STATE_ADDR, margin_cmd, BK96285_READ_LEN);
	bk96285_read_block(di, sram_addr_l + BK96285_PGM_STATE_ADDR, margin_cmd, BK96285_READ_LEN);

	margin_cmd[BK96285_MSG_BIT] = BK96285_PGM_CMD_MARGIN;
	bk96285_write_block(di, sram_addr_l + BK96285_PGM_STATE_ADDR, margin_cmd, BK96285_WRITE_LEN);

	state = bk96285_pgm_state(di);
	if (state != BK96285_PGM_STATE_DONE) {
		hwlog_err("set margin: pgm access state %02x\n", state);
		return -EINVAL;
	}
	return 0;
}

static int bk96285_pgm_read_crc(struct bk96285_dev_info *di, u8 *crc, u16 crc_size, u16 addr, u16 len)
{
	u8 state;
	u16 i;
	u16 index;
	bk96285_pgm_pkt_type pgm = { 0 };
	u16 sram_addr_h = BK96285_SRAM_ADDR_H;
	u16 sram_addr_l = BK96285_SRAM_ADDR_L;
	u8 addr_high[BK96285_HIGH_ADDR_NUM] = { 0 };
	u8 crc_verify_cmd[BK96285_BYTE_LEN] = { 0 };

	addr_high[BK96285_HIGH_ADDR0] = sram_addr_h >> BK96285_HIGH_8BIT_ADDR;
	addr_high[BK96285_HIGH_ADDR1] = sram_addr_h & BK96285_LOW_8BIT_ADDR;

	bk96285_write_block(di, BK96285_ADDR_HIGH_BASE, addr_high, BK96285_HIGH_ADDR_NUM);

	pgm.type.addr = addr;
	pgm.type.len = len;
	pgm.type.xor = bk96285_pgm_xor(&pgm.value[BK96285_PGM_XOR_INDEX], BK96285_PGM_XOR_LEN);

	pgm.type.xor ^= BK96285_PGM_CMD_VERIFY << BK96285_SHIFT_8BIT;
	bk96285_write_block(di, sram_addr_l + BK96285_PGM_STATE_ADDR,
		&pgm.value[0], BK96285_PGM_HEADER_SIZE);
	bk96285_read_block(di, sram_addr_l + BK96285_PGM_STATE_ADDR, crc_verify_cmd, BK96285_READ_LEN);
	bk96285_read_block(di, sram_addr_l + BK96285_PGM_STATE_ADDR, crc_verify_cmd, BK96285_READ_LEN);

	crc_verify_cmd[BK96285_MSG_BIT] = BK96285_PGM_CMD_VERIFY;
	bk96285_write_block(di, sram_addr_l + BK96285_PGM_STATE_ADDR, crc_verify_cmd, BK96285_WRITE_LEN);
	power_usleep(DT_USLEEP_1MS);
	state = bk96285_pgm_state(di);
	if (state != BK96285_PGM_STATE_DONE) {
		hwlog_err("read crc: pgm read crc state %02x\n", state);
		return -EINVAL;
	}
	bk96285_read_block(di, sram_addr_l + BK96285_PGM_STATE_ADDR,
		(u8 *)pgm.value, BK96285_PGM_HEADER_SIZE);
	bk96285_read_block(di, sram_addr_l + BK96285_PGM_MSG_ADDR, pgm.type.msg, crc_size);
	if (!bk96285_pgm_xor((u8 *)pgm.value, crc_size + BK96285_PGM_HEADER_SIZE)) {
		index = 0;
		for (i = 0; i < crc_size; i++) {
			crc[index] = pgm.type.msg[i];
			index++;
		}
	}
	return 0;
}

static int bk96285_mcu_idle_ctrl(struct bk96285_dev_info *di, bool en)
{
	int i;
	u8 idle_data[BK96285_BYTE_LEN] = { 0 };
	u8 idle_result[BK96285_BYTE_LEN] = { 0 };
	u8 idle_clear[BK96285_BYTE_LEN] = { 0 };
	int ret = 0;

	if (!en)
		return -EINVAL;

	/* 0 1 2 3:read each byte */
	idle_data[0] = BK96285_IDLE_PW0 >> BK96285_HIGH_8BIT_ADDR;
	idle_data[1] = BK96285_IDLE_PW0 & BK96285_LOW_8BIT_ADDR;
	idle_data[2] = BK96285_IDLE_ADDR_HIGH >> BK96285_HIGH_8BIT_ADDR;
	idle_data[3] = BK96285_IDLE_ADDR_HIGH & BK96285_LOW_8BIT_ADDR;

	for (i = 0; i < BK96285_BYTE_LEN; i++)
		idle_result[i] = ((BK96285_IDLE_VAL >> (i * BK96285_SHIFT_8BIT)) & BK96285_LOW_8BIT_ADDR);

	ret += bk96285_write_block(di, BK96285_REG_IDLE, idle_data, BK96285_HIGH_ADDR_NUM);
	ret += bk96285_write_block(di, BK96285_ADDR_HIGH_BASE,
		idle_data + BK96285_HIGH_ADDR_NUM, BK96285_HIGH_ADDR_NUM);
	ret += bk96285_write_block(di, BK96285_IDLE_ADDR_LOW1, idle_result, BK96285_FOUR_BYTE_ALIGN);
	ret += bk96285_write_block(di, BK96285_IDLE_ADDR_LOW2, idle_clear, BK96285_FOUR_BYTE_ALIGN);
	power_usleep(DT_USLEEP_5MS);
	ret += bk96285_write_block(di, BK96285_IDLE_ADDR_LOW1, idle_result, BK96285_FOUR_BYTE_ALIGN);
	ret += bk96285_write_block(di, BK96285_IDLE_ADDR_LOW2, idle_clear, BK96285_FOUR_BYTE_ALIGN);
	return ret;
}

static int bk96285_fw_write_sram_data(struct bk96285_dev_info *di, u8 *addr_high)
{
	int i;
	int ret;
	int pgm_pkg_num;
	int pgm_pkg_remaind;
	int pgm_pkg_count = 0;

	pgm_pkg_num = sizeof(g_bk96285_fw_sram) / BK96285_PGM_PKG_SIZE;
	pgm_pkg_remaind = sizeof(g_bk96285_fw_sram) % BK96285_PGM_PKG_SIZE;
	hwlog_info("[write_sram] bk96285 pgm num = %d remaind = %d\n", pgm_pkg_num, pgm_pkg_remaind);

	ret = bk96285_mcu_idle_ctrl(di, true);
	if (ret < 0) {
		hwlog_err("write_sram: mcu_idle_ctrl failed\n");
		return -EINVAL;
	}

	bk96285_write_block(di, BK96285_ADDR_HIGH_BASE, addr_high, BK96285_HIGH_ADDR_NUM);
	for (i = 0; i < sizeof(g_bk96285_fw_sram); i += BK96285_PGM_PKG_SIZE) {
		if ((pgm_pkg_count == pgm_pkg_num) && (pgm_pkg_remaind != 0)) {
			ret = bk96285_write_block(di, BK96285_SRAM_ADDR_L + i, &g_bk96285_fw_sram[i], pgm_pkg_remaind);
			if (ret < 0) {
				hwlog_err("program_prev_process: bk96285_write_block failed\n");
				return -EINVAL;
			}
			break;
		}
		ret = bk96285_write_block(di, BK96285_SRAM_ADDR_L + i, &g_bk96285_fw_sram[i], BK96285_PGM_PKG_SIZE);
		if (ret < 0) {
			hwlog_err("program_prev_process: bk96285_write_block failed\n");
			return -EINVAL;
		}
		pgm_pkg_count++;
	}
	return ret;
}

static int bk96285_fw_write_sram(struct bk96285_dev_info *di)
{
	int ret;
	char info[BK96285_PGM_INFO_SIZE];
	u8 addr_high[BK96285_HIGH_ADDR_NUM] = { 0 };

	ret = bk96285_write_dword_mask(di, BK96285_RX_CMD_ADDR,
		BK96285_RX_CMD_MCU_HALT_MASK, BK96285_RX_CMD_MCU_HALT_SHIFT,
		BK96285_RX_CMD_EN);
	if (ret) {
		hwlog_err("write_sram: en mcu halt failed\n");
		return ret;
	}

	power_usleep(DT_USLEEP_10MS); /* typically 10ms */
	addr_high[BK96285_HIGH_ADDR0] = BK96285_SRAM_ADDR_H >> BK96285_HIGH_8BIT_ADDR;
	addr_high[BK96285_HIGH_ADDR1] = BK96285_SRAM_ADDR_H & BK96285_LOW_8BIT_ADDR;
	ret = bk96285_fw_write_sram_data(di, addr_high);
	if (ret < 0) {
		hwlog_err("program_prev_process: write_sram_data failed\n");
		return ret;
	}
	ret = bk96285_bool_sel(di, true);
	ret |= bk96285_sys_reset_ctrl(di, true);
	if (ret < 0) {
		hwlog_err("program_prev_process: sys_reset_ctrl failed\n");
		return ret;
	}

	ret = bk96285_get_pgm_info(di, info);
	hwlog_info("[program_prev_process] info = %s\n", info);
	if (info[0] != BK96285_PGM_VER) {
		hwlog_err("program_prev_process: pgm run failed\n");
		return -EINVAL;
	}
	hwlog_info("[program_prev_process] pgm run sucessfully\n");

	return ret;
}

static int bk96285_fw_check_mtp_version(struct bk96285_dev_info *di)
{
	int ret;
	u16 mtp_ver = 0;

	ret = bk96285_fw_get_mtp_ver(di, &mtp_ver);
	if (ret)
		return ret;

	hwlog_info("[version_check] mtp_ver=0x%04x\n", mtp_ver);
	if (mtp_ver != di->fw_mtp.mtp_ver)
		return -ENXIO;

	return 0;
}

static int bk96285_check_fw_checksum(struct bk96285_dev_info *di)
{
	u32 fw_checksum = 0;

	bk96285_read_dword(di, BK96285_CHECKSUM_ADDR, (u32 *)&fw_checksum);
	if (fw_checksum != BK96285_FW_CHECKSUM) {
		hwlog_err("fw_checksum: bk96285 get firmware checksum fail\n");
		return -EIO;
	}

	return 0;
}

static int bk96285_fw_check_mtp_crc(struct bk96285_dev_info *di, u8 margin, const u8 *buf, u16 len)
{
	int i, ret;
	u32 data32;
	u32 read_crc = 0;
	u8 tmp_buf[BK96285_PGM_WORD] = { 0 };

	ret = bk96285_pgm_set_margin(di, margin);
	if (ret < 0) {
		hwlog_err("check_mtp_crc: set margin fail\n");
		return ret;
	}

	ret = bk96285_pgm_read_crc(di, (u8 *)(&read_crc), BK96285_CRC_SIZE, BK96285_CRC_ADDR,
			BK96285_CHIP_SIZE / BK96285_PGM_WORD);
	if (ret < 0) {
		hwlog_err("check_mtp_crc: pgm read crc failed");
		return -EINVAL;
	}

	data32 = BK96285_UINT_MAX;
	for (i = 0; i < BK96285_CHIP_SIZE; i += BK96285_PGM_WORD) {
		(void)memset_s(tmp_buf, BK96285_PGM_WORD, 0, BK96285_PGM_WORD);
		if (len - i >= BK96285_PGM_WORD)
			ret = memcpy_s(tmp_buf, BK96285_PGM_WORD, buf + i, BK96285_PGM_WORD);
		else if (len - i > 0)
			ret = memcpy_s(tmp_buf, BK96285_PGM_WORD, buf + i, len - i);
		else
			ret = EOK;
		if (ret != EOK) {
			hwlog_err("check_mtp_crc: memcpy_s failed");
			return -EINVAL;
		}
		data32 = bk96285_func_crc32(*(u32 *)tmp_buf, data32);
	}

	if (data32 != read_crc) {
		hwlog_err("check_mtp_crc: data = 0x%08x read = 0x%08x check crc fail\n",
			data32, read_crc);
		return -EINVAL;
	}

	return 0;
}

static int bk96285_fw_check_mtp(void *dev_data)
{
	int ret;
	struct bk96285_dev_info *di = dev_data;

	if (!di)
		return -ENODEV;

	bk96285_disable_irq_nosync(di);
	bk96285_fw_ps_control(di->ic_type, WLPS_RX_EXT_PWR, true);
	wlps_control(WLTRX_IC_MAIN, WLPS_RX_SW, false);
	power_msleep(DT_MSLEEP_100MS, 0, NULL); /* for power on, typically 50ms */

	ret = bk96285_fw_check_mtp_version(di);
	if (ret) {
		hwlog_err("check_mtp: mtp_ver mismatch\n");
		goto exit;
	}

	ret = bk96285_check_fw_checksum(di);
	if (ret) {
		hwlog_err("check_mtp: checksum fail\n");
		goto exit;
	}
succ:
	di->g_val.mtp_latest = true;
	hwlog_info("[check_mtp] mtp latest\n");
exit:
	bk96285_enable_irq(di);
	bk96285_fw_ps_control(di->ic_type, WLPS_RX_EXT_PWR, false);
	return ret;
}

static int bk96285_fw_program(struct bk96285_dev_info *di, const u8 *buf, u16 len)
{
	bk96285_pgm_chip_erase(di);

	return bk96285_pgm_write(di, 0, buf, len, BK96285_PGM_CMD_WRITE_CODE);
}

static int bk96285_fw_program_mtp(struct bk96285_dev_info *di, const u8 *mtp_data, u16 mtp_size)
{
	int ret;

	hwlog_info("[program mtp] start\n");
	di->fw_program = true;
	bk96285_disable_irq_nosync(di);
	bk96285_fw_ps_control(di->ic_type, WLPS_PROC_OTP_PWR, true);
	power_msleep(DT_MSLEEP_100MS, 0, NULL); /* for power on, typically 50ms */

	ret = bk96285_fw_write_sram(di);
	if (ret < 0) {
		hwlog_err("write_sram: fail\n");
		goto program_fail;
	}

	ret = bk96285_fw_program(di, mtp_data, mtp_size);
	if (ret < 0) {
		hwlog_err("fw_program: fail\n");
		goto program_fail;
	}

	ret = bk96285_fw_check_mtp_crc(di, BK96285_CRC_CHECK_MARGIN0, mtp_data, mtp_size);
	if (ret < 0) {
		hwlog_err("check_mtp_crc: margin 0 check crc fail\n");
		goto program_fail;
	}

	ret = bk96285_fw_check_mtp_crc(di, BK96285_CRC_CHECK_MARGIN7, mtp_data, mtp_size);
	if (ret < 0) {
		hwlog_err("check_mtp_crc: margin 7 check crc fail\n");
		goto program_fail;
	}

	hwlog_info("[program mtp] successful\n");

program_fail:
	bk96285_enable_irq(di);
	bk96285_fw_ps_control(di->ic_type, WLPS_PROC_OTP_PWR, false);
	di->fw_program = false;
	return ret;
}

static int bk96285_fw_rx_program_mtp(unsigned int proc_type, void *dev_data)
{
	int ret;
	struct bk96285_dev_info *di = dev_data;

	if (!di)
		return -ENODEV;

	hwlog_info("[rx_program_mtp] type=%u\n", proc_type);

	if (!di->g_val.mtp_chk_complete)
		return -EPERM;

	di->g_val.mtp_chk_complete = false;
	ret = bk96285_fw_program_mtp(di, g_bk96285_fw_mtp, di->fw_mtp.mtp_len);
	if (!ret)
		hwlog_info("[rx_program_mtp] succ\n");

	di->g_val.mtp_chk_complete = true;

	return ret;
}

static int bk96285_fw_get_mtp_status(unsigned int *status, void *dev_data)
{
	int ret;
	struct bk96285_dev_info *di = dev_data;

	if (!di || !status)
		return -EINVAL;

	di->g_val.mtp_chk_complete = false;
	ret = bk96285_fw_check_mtp(di);
	if (!ret)
		*status = WIRELESS_FW_PROGRAMED;
	else
		*status = WIRELESS_FW_ERR_PROGRAMED;

	di->g_val.mtp_chk_complete = true;

	return 0;
}

static void bk96285_fw_mtp_check_work(struct work_struct *work)
{
	int i;
	int ret;
	struct bk96285_dev_info *di = container_of(work, struct bk96285_dev_info, mtp_check_work.work);

	if (!di)
		return;

	if (!power_cmdline_is_factory_mode() && bk96285_rx_is_tx_exist(di)) {
		hwlog_info("[mtp_check_work] tx exist\n");
		di->g_val.mtp_chk_complete = true;
		return;
	}

	power_wakeup_lock(di->fw_wakelock, false);
	di->g_val.mtp_chk_complete = false;
	ret = bk96285_fw_check_mtp(di);
	if (!ret) {
		hwlog_info("[mtp_check_work] succ\n");
		goto exit;
	}

	hwlog_info("[mtp_check_work] mtp_data[0]=0x%x\n", g_bk96285_fw_mtp[0]);
	/* program for 3 times until it's ok */
	for (i = 0; i < 3; i++) {
		ret = bk96285_fw_program_mtp(di, g_bk96285_fw_mtp, di->fw_mtp.mtp_len);
		if (ret)
			continue;
		hwlog_info("[mtp_check_work] update mtp succ, cnt=%d\n", i);
		goto exit;
	}
	hwlog_err("mtp_check_work: update mtp failed\n");

exit:
	di->g_val.mtp_chk_complete = true;
	power_wakeup_unlock(di->fw_wakelock, false);
}

void bk96285_fw_mtp_check(struct bk96285_dev_info *di)
{
	u32 mtp_check_delay;

	if (power_cmdline_is_powerdown_charging_mode() ||
		(!power_cmdline_is_factory_mode() && bk96285_rx_is_tx_exist(di))) {
		di->g_val.mtp_chk_complete = true;
		return;
	}

	if (!power_cmdline_is_factory_mode())
		mtp_check_delay = di->mtp_check_delay.user;
	else
		mtp_check_delay = di->mtp_check_delay.fac;

	INIT_DELAYED_WORK(&di->mtp_check_work, bk96285_fw_mtp_check_work);
	di->fw_wakelock = power_wakeup_source_register(di->dev, "wl_fw_wakelock");
	schedule_delayed_work(&di->mtp_check_work, msecs_to_jiffies(mtp_check_delay));
}

static bool bk96285_fw_is_mtp_chk_complete(void *dev_data)
{
	struct bk96285_dev_info *di = dev_data;

	if (!di) {
		hwlog_err("is_mtp_chk_complete: di null\n");
		return false;
	}

	return di->g_val.mtp_chk_complete;
}

static ssize_t bk96285_fw_write(void *dev_data, const char *buf, size_t size)
{
	int hdr_size;
	u16 crc;
	struct power_fw_hdr *hdr = NULL;
	struct bk96285_dev_info *di = dev_data;

	if (!di || !buf)
		return -EINVAL;

	hdr = (struct power_fw_hdr *)buf;
	hdr_size = sizeof(struct power_fw_hdr);
	hwlog_info("[fw_write] bin_size=%ld version_id=0x%x crc_id=0x%x\n",
		hdr->bin_size, hdr->version_id, hdr->crc_id);

	if ((hdr->unlock_val != WLTRX_UNLOCK_VAL) ||
		(hdr->bin_size > BK96285_FW_MTP_SIZE) || (hdr->bin_size == 0)) {
		hwlog_err("fw_write: config mismatch\n");
		return -EINVAL;
	}

	di->g_val.mtp_latest = false;
	di->fw_mtp.mtp_ver = hdr->version_id;
	di->fw_mtp.mtp_len = hdr->bin_size;

	(void)bk96285_fw_check_mtp(di);
	if (di->g_val.mtp_latest)
		return size;

	di->g_val.mtp_chk_complete = false;
	(void)bk96285_fw_program_mtp(di, (const u8 *)hdr + hdr_size, (u16)hdr->bin_size);
	di->g_val.mtp_chk_complete = true;
	return size;
}

static struct wireless_fw_ops g_bk96285_fw_ops = {
	.ic_name                = "bk96285",
	.program_fw             = bk96285_fw_rx_program_mtp,
	.get_fw_status          = bk96285_fw_get_mtp_status,
	.check_fw               = bk96285_fw_check_mtp,
	.write_fw               = bk96285_fw_write,
	.is_mtp_chk_complete    = bk96285_fw_is_mtp_chk_complete,
};

int bk96285_fw_ops_register(struct wltrx_ic_ops *ops, struct bk96285_dev_info *di)
{
	if (!ops || !di)
		return -ENODEV;

	ops->fw_ops = kzalloc(sizeof(*(ops->fw_ops)), GFP_KERNEL);
	if (!ops->fw_ops)
		return -ENODEV;

	di->fw_mtp.mtp_len = (u16)ARRAY_SIZE(g_bk96285_fw_mtp);
	di->fw_mtp.mtp_ver = BK96285_FW_MTP_VER;
	memcpy_s(ops->fw_ops, sizeof(g_bk96285_fw_ops), &g_bk96285_fw_ops, sizeof(g_bk96285_fw_ops));
	ops->fw_ops->dev_data = (void *)di;
	return wireless_fw_ops_register(ops->fw_ops, di->ic_type);
}
