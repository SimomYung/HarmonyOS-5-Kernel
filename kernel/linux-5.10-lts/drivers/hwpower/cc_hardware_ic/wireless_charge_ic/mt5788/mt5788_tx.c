// SPDX-License-Identifier: GPL-2.0
/*
 * mt5788_tx.c
 *
 * mt5788 tx driver
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

#define HWLOG_TAG wireless_mt5788_tx
HWLOG_REGIST();

static const char * const g_mt5788_tx_irq_name[] = {
	/* [n]: n means bit in registers */
	[6] = "full_brg",
	[7] = "half_brg",
	[8] = "ss_pkt",
	[9] = "id_pkt",
	[10] = "cfg_pkt",
	[11] = "power_trans",
	[12] = "power_remove",
	[13] = "tx_stop_work",
	[14] = "tx_start_work",
	[15] = "ask_rev",
	[16] = "other_ac",
	[17] = "ept",
	[18] = "ping",
	[19] = "cep_timeout",
	[20] = "rpp_timeout",
	[21] = "tx_rpp_err",
	[22] = "tx_otp",
	[23] = "ovp",
	[24] = "ping_ovp",
	[25] = "tx_lvp",
	[26] = "ocp",
	[27] = "tx_ocp_ping",
	[28] = "brg_ocp",
	[29] = "cc_ocp",
	[30] = "fod_pre",
	[31] = "fod",
};

static const char * const g_mt5788_tx_ept_name[] = {
	/* [n]: n means bit in registers */
	[0]  = "tx_ept_src_ovp",
	[1]  = "tx_ept_src_ocp",
	[2]  = "tx_ept_src_otp",
	[3]  = "tx_ept_src_fod",
	[4]  = "tx_ept_src_cmd",
	[5]  = "tx_ept_src_rx",
	[6]  = "tx_ept_src_cep_timeout",
	[7]  = "tx_ept_src_rpp_timeout",
	[8]  = "tx_ept_src_rx_rst",
	[9]  = "tx_ept_src_sys_err",
	[10] = "tx_ept_src_ping_timeout",
	[11] = "tx_ept_src_ss",
	[12] = "tx_ept_src_id",
	[13] = "tx_ept_src_cfg",
	[14] = "tx_ept_src_cfg_cnt",
	[15] = "tx_ept_src_pch",
	[16] = "tx_ept_src_xid",
	[17] = "tx_ept_src_nego",
	[18] = "tx_ept_src_nego_timeout",
};

static bool mt5788_tx_is_in_tx_mode(void *dev_data)
{
	int ret;
	u32 mode = 0;

	ret = mt5788_read_dword(dev_data, MT5788_TX_SYS_MODE_ADDR, &mode);
	if (ret) {
		hwlog_err("is_in_tx_mode: get op_mode failed\n");
		return false;
	}

	if (mode & MT5788_TX_SYS_MODE_TX)
		return true;

	return false;
}

static bool mt5788_tx_is_in_rx_mode(void *dev_data)
{
	int ret;
	u32 mode = 0;

	ret = mt5788_read_dword(dev_data, MT5788_TX_SYS_MODE_ADDR, &mode);
	if (ret) {
		hwlog_err("is_in_rx_mode: get op_mode failed\n");
		return false;
	}

	if (mode & MT5788_TX_SYS_MODE_RX)
		return true;

	return false;
}

static int mt5788_tx_set_tx_open_flag(bool enable, void *dev_data)
{
	struct mt5788_dev_info *di = dev_data;

	if (!di)
		return -ENODEV;

	di->g_val.tx_open_flag = enable;
	return 0;
}

static void mt5788_tx_chip_reset(void *dev_data)
{
	int ret;

	ret = mt5788_write_byte_mask(dev_data, MT5788_TX_CMD_ADDR,
		MT5788_TX_CMD_CHIP_RST_MASK, MT5788_TX_CMD_CHIP_RST_SHIFT,
		MT5788_TX_CMD_VALUE);
	if (ret) {
		hwlog_err("chip_reset: failed\n");
		return;
	}

	hwlog_info("[chip_reset] succ\n");
}

static int mt5788_tx_set_bridge(unsigned int v_ask, unsigned int type, void *dev_data)
{
	int ret = 0;

	switch (type) {
	case WLTX_PING_HALF_BRIDGE:
		ret = mt5788_write_byte_mask(dev_data, MT5788_TX_BRIDGE_MODE_ADDR, MT5788_TX_PING_BRIDGE_MASK,
			MT5788_TX_PING_BRIDGE_SHIFT, MT5788_TX_PING_HALF_BRIDGE);
		break;
	case WLTX_PING_FULL_BRIDGE:
		ret = mt5788_write_byte_mask(dev_data, MT5788_TX_BRIDGE_MODE_ADDR, MT5788_TX_PING_BRIDGE_MASK,
			MT5788_TX_PING_BRIDGE_SHIFT, MT5788_TX_PING_FULL_BRIDGE);
		break;
	case WLTX_PT_HALF_BRIDGE:
		/* not support change bridge to half when v_ask is low */
		if (v_ask > MT5788_TX_SWITCH_BRI_CTL_VTH) {
			hwlog_info("[set_bridge] v_ask=%d, not set half-bridge\n", v_ask);
			break;
		}
		ret = mt5788_write_byte_mask(dev_data, MT5788_TX_BRIDGE_MODE_ADDR, MT5788_TX_PT_BRIDGE_MASK,
			MT5788_TX_PT_BRIDGE_SHIFT, MT5788_TX_PT_HALF_BRIDGE);
		break;
	case WLTX_PT_FULL_BRIDGE:
		ret = mt5788_write_byte_mask(dev_data, MT5788_TX_BRIDGE_MODE_ADDR, MT5788_TX_PT_BRIDGE_MASK,
			MT5788_TX_PT_BRIDGE_SHIFT, MT5788_TX_PT_FULL_BRIDGE);
		break;
	default:
		return -EINVAL;
	}

	ret += mt5788_write_byte(dev_data, MT5788_TX_BRIDGE_ACESS_ADDR, MT5788_TX_BRIDGE_ACESS_AP_CTRL);
	ret += mt5788_write_dword_mask(dev_data, MT5788_TX_CMD_ADDR,
		MT5788_TX_CMD_BRG_SET_MASK, MT5788_TX_CMD_BRG_SET_SHIFT, MT5788_TX_CMD_VALUE);
	if (ret)
		hwlog_err("set_bridge: failed\n");
	return ret;
}

static bool mt5788_tx_check_rx_disconnect(void *dev_data)
{
	struct mt5788_dev_info *di = dev_data;

	if (!di)
		return true;

	if (di->tx_ept_type & MT5788_TX_EPT_CEP_TIMEOUT) {
		di->tx_ept_type &= ~MT5788_TX_EPT_CEP_TIMEOUT;
		hwlog_info("[check_rx_disconnect] rx disconnect\n");
		return true;
	}

	return false;
}

static int mt5788_tx_get_ping_interval(u16 *ping_interval, void *dev_data)
{
	return mt5788_read_word(dev_data, MT5788_TX_PING_INTERVAL_ADDR, ping_interval);
}

static int mt5788_tx_set_ping_interval(u16 ping_interval, void *dev_data)
{
	if ((ping_interval < MT5788_TX_PING_INTERVAL_MIN) ||
		(ping_interval > MT5788_TX_PING_INTERVAL_MAX)) {
		hwlog_err("set_ping_interval: para out of range\n");
		return -EINVAL;
	}

	return mt5788_write_word(dev_data, MT5788_TX_PING_INTERVAL_ADDR, ping_interval);
}

static int mt5788_tx_get_ping_freq(u16 *ping_freq, void *dev_data)
{
	int ret;

	ret = mt5788_read_word(dev_data, MT5788_TX_PING_FREQ_ADDR, ping_freq);
	*ping_freq = *ping_freq / MT5788_TX_PING_FREQ_STEP;
	return ret;
}

static int mt5788_tx_set_ping_freq(u16 ping_freq, void *dev_data)
{
	int dbg;

	dbg = wlic_dbg_tx_get_param(WLTX_PARAM_PING_FREQ);
	if (dbg >= 0)
		ping_freq = (u16)dbg;

	if ((ping_freq < MT5788_TX_PING_FREQ_MIN) ||
		(ping_freq > MT5788_TX_PING_FREQ_MAX)) {
		hwlog_err("set_ping_frequency: para out of range\n");
		return -EINVAL;
	}

	ping_freq = ping_freq * MT5788_TX_PING_FREQ_STEP;
	return mt5788_write_word(dev_data, MT5788_TX_PING_FREQ_ADDR, ping_freq);
}

static int mt5788_tx_set_ping_duty(u16 ping_duty, void *dev_data)
{
	int dbg;

	dbg = wlic_dbg_tx_get_param(WLTX_PARAM_PING_DUTY);
	if (dbg >= 0)
		ping_duty = (u16)dbg;
	if ((ping_duty < MT5788_TX_PING_DUTY_MIN) ||
		(ping_duty > MT5788_TX_PING_DUTY_MAX)) {
		hwlog_err("set_ping_duty: para out of range\n");
		return -EINVAL;
	}

	return mt5788_write_word(dev_data, MT5788_TX_PING_DUTY_ADDR, ping_duty);
}

static int mt5788_tx_get_min_fop(u16 *fop, void *dev_data)
{
	int ret;

	ret = mt5788_read_word(dev_data, MT5788_TX_MIN_FOP_FREQ_ADDR, fop);
	*fop = *fop / MT5788_TX_FOP_STEP;
	return ret;
}

static int mt5788_tx_set_min_fop(u16 fop, void *dev_data)
{
	int dbg;

	dbg = wlic_dbg_tx_get_param(WLTX_PARAM_PT_FOP_MIN);
	if (dbg >= 0)
		fop = (u16)dbg;
	if ((fop < MT5788_TX_MIN_FOP) || (fop > MT5788_TX_MAX_FOP)) {
		hwlog_err("set_min_fop: para out of range\n");
		return -EINVAL;
	}

	fop = fop * MT5788_TX_FOP_STEP;
	return mt5788_write_word(dev_data, MT5788_TX_MIN_FOP_FREQ_ADDR, fop);
}

static int mt5788_tx_get_max_fop(u16 *fop, void *dev_data)
{
	int ret;

	ret = mt5788_read_word(dev_data, MT5788_TX_MAX_FOP_FREQ_ADDR, fop);
	*fop = *fop / MT5788_TX_FOP_STEP;
	return ret;
}

static int mt5788_tx_set_max_fop(u16 fop, void *dev_data)
{
	int dbg;

	dbg = wlic_dbg_tx_get_param(WLTX_PARAM_PT_FOP_MAX);
	if (dbg >= 0)
		fop = (u16)dbg;
	if ((fop < MT5788_TX_MIN_FOP) || (fop > MT5788_TX_MAX_FOP)) {
		hwlog_err("set_max_fop: para out of range\n");
		return -EINVAL;
	}

	fop *= MT5788_TX_FOP_STEP;
	return mt5788_write_word(dev_data, MT5788_TX_MAX_FOP_FREQ_ADDR, fop);
}

static int mt5788_tx_set_min_duty(int duty, void *dev_data)
{
	int dbg;

	dbg = wlic_dbg_tx_get_param(WLTX_PARAM_PT_DUTY_MIN);
	if (dbg >= 0)
		duty = dbg;
	if ((duty < MT5788_TX_PT_DUTY_MIN_DEFAULT) ||
		(duty > MT5788_TX_PT_DUTY_MAX_DEFAULT)) {
		hwlog_err("set_min_duty: para out of range\n");
		return -EINVAL;
	}

	return mt5788_write_word(dev_data, MT5788_TX_PT_DUTY_MIN_ADDR, (u16)duty);
}

static int mt5788_tx_set_max_duty(int duty, void *dev_data)
{
	int dbg;

	dbg = wlic_dbg_tx_get_param(WLTX_PARAM_PT_DUTY_MAX);
	if (dbg >= 0)
		duty = dbg;
	if ((duty < MT5788_TX_PT_DUTY_MIN_DEFAULT) ||
		(duty > MT5788_TX_PT_DUTY_MAX_DEFAULT)) {
		hwlog_err("set_max_duty: para out of range\n");
		return -EINVAL;
	}

	return mt5788_write_word(dev_data, MT5788_TX_PT_DUTY_MAX_ADDR, (u16)duty);
}

static int mt5788_tx_get_fop(u16 *fop, void *dev_data)
{
	int ret;

	ret = mt5788_read_word(dev_data, MT5788_TX_FOP_ADDR, fop);
	*fop = *fop / MT5788_TX_FOP_STEP;
	return ret;
}

static int mt5788_tx_get_cep(s8 *cep, void *dev_data)
{
	int ret;

	ret = mt5788_read_byte(dev_data, MT5788_TX_CEP_VAL_ADDR, cep);
	*cep = *cep / MT5788_TX_CEP_VAL_STEP;
	return ret;
}

static int mt5788_tx_get_duty(u8 *duty, void *dev_data)
{
	u16 value = 0;

	if (!duty || mt5788_read_word(dev_data, MT5788_TX_DUTY_READ_ADDR, &value))
		return -EINVAL;

	*duty = (u8)value;
	return 0;
}

static int mt5788_tx_get_ptx(u32 *ptx, void *dev_data)
{
	return mt5788_read_dword(dev_data, MT5788_TX_PTX_ADDR, ptx);
}

static int mt5788_tx_get_prx(u32 *prx, void *dev_data)
{
	return mt5788_read_dword(dev_data, MT5788_TX_PRX_ADDR, prx);
}

static int mt5788_tx_get_ploss_id(u8 *id, void *dev_data)
{
	return mt5788_read_word(dev_data, MT5788_TX_FOD_TH_ID_ADDR, (u16 *)id);
}

static int mt5788_tx_get_ploss(s32 *ploss, void *dev_data)
{
	return mt5788_read_dword(dev_data, MT5788_TX_PLOSS_ADDR, (u32 *)ploss);
}

static int mt5788_tx_get_temp(s16 *chip_temp, void *dev_data)
{
	return mt5788_read_word(dev_data, MT5788_TX_TEMP_ADDR, chip_temp);
}

static int mt5788_tx_get_vin(u16 *tx_vin, void *dev_data)
{
	return mt5788_read_word(dev_data, MT5788_TX_VIN_ADDR, tx_vin);
}

static int mt5788_tx_get_vrect(u16 *tx_vrect, void *dev_data)
{
	return mt5788_read_word(dev_data, MT5788_TX_VBRG_ADDR, tx_vrect);
}

static int mt5788_tx_get_iin(u16 *tx_iin, void *dev_data)
{
	return mt5788_read_word(dev_data, MT5788_TX_IIN_ADDR, tx_iin);
}

static int mt5788_tx_get_curfod_iin(u16 *tx_iin, void *dev_data)
{
	return mt5788_read_word(dev_data, MT5788_TX_CURFOD_IIN_ADDR, tx_iin);
}

static int mt5788_tx_set_ilimit(u16 tx_ilim, void *dev_data)
{
	int dbg;

	dbg = wlic_dbg_tx_get_param(WLTX_PARAM_PT_ILIMIT);
	if (dbg >= 0)
		tx_ilim = (u16)dbg;
	if ((tx_ilim < MT5788_TX_ILIM_MIN) || (tx_ilim > MT5788_TX_ILIM_MAX)) {
		hwlog_err("set_ilimit: out of range\n");
		return -EINVAL;
	}

	return mt5788_write_word(dev_data, MT5788_TX_ILIM_ADDR, tx_ilim);
}

static int mt5788_tx_set_ploss(u8 rx_type, void *dev_data)
{
	int ret;
	u8 *tx_fod = NULL;
	struct mt5788_dev_info *di = dev_data;

	if (!di)
		return -ENODEV;

	if (di->tx_fod_fmt == WLTX_FOD_STATUS_FMT_OLD) {
		ret = mt5788_write_word(di, MT5788_TX_PLOSS_TH0_ADDR,
			(u16)di->tx_fod.ploss_th0);
		ret += mt5788_write_word(di, MT5788_TX_PLOSS_TH1_ADDR,
			(u16)di->tx_fod.ploss_th1);
		ret += mt5788_write_word(di, MT5788_TX_PLOSS_TH2_ADDR,
			(u16)di->tx_fod.ploss_th2);
		ret += mt5788_write_word(di, MT5788_TX_PLOSS_TH3_ADDR,
			(u16)di->tx_fod.ploss_th3);
	} else if (di->tx_fod_fmt == WLTX_FOD_STATUS_FMT_NEW) {
		tx_fod = wltx_get_fod_ploss_th(di->ic_type,
			rx_type, MT5788_TX_FOD_LEN);
		if (!tx_fod)
			return -EPERM;
		ret = mt5788_write_block(di, MT5788_TX_FOD_ADDR,
			tx_fod, MT5788_TX_FOD_LEN);
	} else {
		hwlog_err("tx_fod_fmt: get error\n");
		return -EINVAL;
	}

	ret += mt5788_write_word(di, MT5788_TX_PLOSS_CNT_ADDR,
		(u16)di->tx_fod.ploss_cnt);
	ret += mt5788_write_dword_mask(di, MT5788_TX_CMD_ADDR,
		MT5788_TX_CMD_FOD_EN_MASK, MT5788_TX_CMD_FOD_EN_SHIFT,
		MT5788_TX_CMD_VALUE);
	if (ret) {
		hwlog_err("init_fod_para: failed\n");
		return ret;
	}

	return 0;
}

static int mt5788_tx_stop_config(void *dev_data)
{
	struct mt5788_dev_info *di = dev_data;

	if (!di)
		return -ENODEV;

	di->g_val.tx_stop_chrg = true;
	return 0;
}

static void mt5788_tx_prevfod_chk(int flag, void *dev_data)
{
	struct mt5788_dev_info *di = dev_data;

	if (!di)
		return;

	hwlog_info("[prevfod_chk] flag=%d\n", flag);
	switch (flag) {
	case WLTX_PREVFOD_CHK_START:
		(void)mt5788_tx_set_min_duty(di->tx_fodchk_duty_min, di);
		(void)mt5788_tx_set_max_duty(di->tx_fodchk_duty_max, di);
		break;
	case WLTX_PREVFOD_CHK_STOP:
		(void)mt5788_tx_set_min_duty(di->duty_min, di);
		(void)mt5788_tx_set_max_duty(di->duty_max, di);
		break;
	default:
		break;
	}
}

static int mt5788_sw2tx(struct mt5788_dev_info *di)
{
	int ret;
	int i;
	u32 mode = 0;

	/* 500ms=25ms*20 timeout for switching to tx mode */
	for (i = 0; i < 20; i++) {
		power_msleep(DT_MSLEEP_25MS, 0, NULL);
		ret = mt5788_get_mode(di, &mode);
		if (ret) {
			hwlog_err("sw2tx: get mode failed\n");
			continue;
		}

		if (mode & MT5788_TX_SYS_MODE_TX) {
			hwlog_info("[sw2tx] succ, cnt=%d\n", i);
			return 0;
		}

		ret = mt5788_write_dword_mask(di, MT5788_TX_CMD_ADDR,
			MT5788_TX_CMD_ENTER_TX_MASK, MT5788_TX_CMD_ENTER_TX_SHIFT,
			MT5788_TX_CMD_VALUE);
		if (ret)
			hwlog_err("sw2tx: write cmd(sw2tx) failed\n");
	}
	hwlog_err("sw2tx: failed, cnt=%d\n", i);
	return -ENXIO;
}

static int mt5788_tx_set_open_loop(bool enable, void *dev_data)
{
	hwlog_info("[set_open_loop] %s\n", enable ? "enable" : "disable");
	return mt5788_write_dword_mask(dev_data, MT5788_TX_CTRL_ADDR,
		MT5788_TX_CTRL_OPEN_LOOP_MASK, MT5788_TX_CTRL_OPEN_LOOP_SHIFT,
		enable ? MT5788_TX_CTRL_EN : MT5788_TX_CTRL_DIS);
}

static int mt5788_tx_set_ping_ocp(int value, struct mt5788_dev_info *di)
{
	int dbg;

	dbg = wlic_dbg_tx_get_param(WLTX_PARAM_PING_OCP);
	if (dbg >= 0)
		value = dbg;
	hwlog_info("[set_ping_ocp] %d\n", value);
	return mt5788_write_word(di, MT5788_TX_PING_OCP_TH_ADDR, (u16)value);
}

static int mt5788_tx_set_pt_ocp(int value, struct mt5788_dev_info *di)
{
	int dbg;

	dbg = wlic_dbg_tx_get_param(WLTX_PARAM_PT_OCP);
	if (dbg >= 0)
		value = dbg;
	hwlog_info("[set_pt_ocp] %d\n", value);
	return mt5788_write_word(di, MT5788_TX_PT_OCP_ADDR, (u16)value);
}

static int mt5788_tx_set_iin_cali(struct mt5788_dev_info *di)
{
	int ret;

	if (!di->tx_icali_para)
		return 0;

	ret = mt5788_write_block(di, MT5788_TX_IIN_CALI_ADDR, di->tx_icali_para,
		MT5788_TX_IIN_CALI_LEN);
	ret += mt5788_write_dword_mask(di, MT5788_TX_CMD_ADDR,
		MT5788_TX_TMD_SET_CALI_PARAM_MASK, MT5788_TX_CMD_SET_CALI_PARAM_SHIFT,
		MT5788_TX_CMD_VALUE);
	return ret;
}

static int mt5788_tx_set_bridge_ctrl(struct mt5788_dev_info *di)
{
	int ret;

	ret = mt5788_write_word(di, MT5788_TX_BRIDGE_DUTY_ADDR,
		(u16)di->bridge_ctrl[MT5788_BRIDGE_DUTY]);
	ret += mt5788_write_word(di, MT5788_TX_BRIDGE_FREQ_ADDR,
		(u16)di->bridge_ctrl[MT5788_BRIDGE_FREQ]);

	hwlog_info("[set_bridge_ctrl] 0x%x:%d, 0x%x:%d\n",
		MT5788_TX_BRIDGE_DUTY_ADDR, di->bridge_ctrl[MT5788_BRIDGE_DUTY],
		MT5788_TX_BRIDGE_FREQ_ADDR, di->bridge_ctrl[MT5788_BRIDGE_FREQ]);
	return ret;
}

static void mt5788_tx_select_init_para(struct mt5788_dev_info *di,
	unsigned int client)
{
	di->tx_init_para.ocp_th = di->tx_pt_ocp_th;
	di->tx_init_para.ping_ocp_th = MT5788_TX_PING_OCP_TH_DEFAULT;
	di->tx_init_para.ping_duty = MT5788_TX_PING_DUTY;

	switch (client) {
	case WLTX_CLIENT_UI:
		di->tx_init_para.ping_freq = di->tx_ping_freq;
		di->tx_init_para.ping_duty = di->tx_ping_duty;
		di->tx_init_para.ping_ocp_th = di->ping_ocp_th;
		di->tx_init_para.ping_interval = MT5788_TX_PING_INTERVAL;
		break;
	case WLTX_CLIENT_COIL_TEST:
		di->tx_init_para.ping_freq = di->coil_test_ping_freq;
		di->tx_init_para.ping_interval = MT5788_TX_PING_INTERVAL_COIL_TEST;
		break;
	case WLTX_CLIENT_BAT_HEATING:
		di->tx_init_para.ping_freq = MT5788_TX_PING_FREQ_BAT_HEATING;
		di->tx_init_para.ping_interval = MT5788_TX_PING_INTERVAL_BAT_HEATING;
		di->tx_init_para.ocp_th = MT5788_TX_BAT_HEATING_OCP1_TH;
		break;
	default:
		di->tx_init_para.ping_freq = di->tx_ping_freq;
		di->tx_init_para.ping_interval = MT5788_TX_PING_INTERVAL;
		break;
	}
}

static int mt5788_tx_set_init_para(struct mt5788_dev_info *di)
{
	int ret;

	ret = mt5788_sw2tx(di);
	if (ret) {
		hwlog_err("set_init_para: sw2tx failed\n");
		return ret;
	}

	ret = mt5788_tx_set_pt_ocp(di->tx_init_para.ocp_th, di);
	ret += mt5788_write_word(di, MT5788_TX_PT_OVP_ADDR, MT5788_TX_PT_OVP_TH);
	ret += mt5788_write_byte(di, MT5788_TX_IIN_CHIP_ADDR, (u8)di->tx_iin_chip);
	ret += mt5788_write_dword(di, MT5788_TX_INT_EN_ADDR, MT5788_TX_INT_EN_ALL);
	ret += mt5788_write_word(di, MT5788_TX_LVP_ADDR, MT5788_TX_LVP_TH);
	ret += mt5788_write_dword_mask(di, MT5788_TX_CTRL_ADDR,
		MT5788_TX_CTRL_FOD_EN_MASK, MT5788_TX_CTRL_FOD_EN_SHIFT, MT5788_TX_CTRL_EN);
	ret += mt5788_write_dword_mask(di, MT5788_TX_CTRL_ADDR,
		MT5788_TX_CTRL_CEP_MASK, MT5788_TX_CTRL_CEP_SHIFT, di->tx_pt_cep_ctrl);
	ret += mt5788_tx_set_ping_freq(di->tx_init_para.ping_freq, di);
	ret += mt5788_tx_set_ping_duty(di->tx_init_para.ping_duty, di);
	ret += mt5788_tx_set_ping_ocp(di->tx_init_para.ping_ocp_th, di);
	ret += mt5788_tx_set_min_fop(MT5788_TX_MIN_FOP, di);
	ret += mt5788_tx_set_max_fop(MT5788_TX_MAX_FOP, di);
	ret += mt5788_tx_set_min_duty(di->duty_min, di);
	ret += mt5788_tx_set_max_duty(di->duty_max, di);
	ret += mt5788_tx_set_ping_interval(di->tx_init_para.ping_interval, di);
	ret += mt5788_tx_set_iin_cali(di);
	ret += mt5788_tx_set_bridge_ctrl(di);
	if (ret) {
		hwlog_err("set_init_para: write failed\n");
		return -EIO;
	}

	return 0;
}

static int mt5788_tx_chip_init(unsigned int client, void *dev_data)
{
	struct mt5788_dev_info *di = dev_data;

	if (!di)
		return -ENODEV;

	mt5788_tx_chip_reset(di);

	di->irq_cnt = 0;
	di->g_val.irq_abnormal = false;
	di->g_val.tx_stop_chrg = false;
	mt5788_enable_irq_wake(di);
	mt5788_enable_irq(di);

	mt5788_tx_select_init_para(di, client);
	return mt5788_tx_set_init_para(di);
}

static int mt5788_tx_enable_tx_mode(bool enable, void *dev_data)
{
	int ret;

	if (enable)
		ret = mt5788_write_dword_mask(dev_data, MT5788_TX_CMD_ADDR,
			MT5788_TX_CMD_START_WORK_MASK, MT5788_TX_CMD_START_WORK_SHIFT,
			MT5788_TX_CMD_VALUE);
	else
		ret = mt5788_write_dword_mask(dev_data, MT5788_TX_CMD_ADDR,
			MT5788_TX_CMD_STOP_WORK_MASK, MT5788_TX_CMD_STOP_WORK_SHIFT,
			MT5788_TX_CMD_VALUE);

	if (ret)
		hwlog_err("%s tx_mode failed\n", enable ? "enable" : "disable");

	return ret;
}

static void mt5788_tx_show_ept_type(u32 ept)
{
	unsigned int i;

	hwlog_info("[show_ept_type] type=0x%02x", ept);
	for (i = 0; i < ARRAY_SIZE(g_mt5788_tx_ept_name); i++) {
		if (ept & BIT(i))
			hwlog_info("[show_ept_type] %s\n", g_mt5788_tx_ept_name[i]);
	}
}

static int mt5788_tx_get_ept_type(struct mt5788_dev_info *di, u32 *ept)
{
	int ret;

	if (!ept) {
		hwlog_err("get_ept_type: para null\n");
		return -EINVAL;
	}

	ret = mt5788_read_dword(di, MT5788_TX_EPT_TYPE_ADDR, ept);
	if (ret) {
		hwlog_err("get_ept_type: read failed\n");
		return ret;
	}

	mt5788_tx_show_ept_type(*ept);
	return 0;
}

static int mt5788_tx_clear_ept_src(struct mt5788_dev_info *di)
{
	return mt5788_write_dword(di, MT5788_TX_EPT_TYPE_ADDR, MT5788_TX_EPT_CLEAR);
}

static void mt5788_tx_ept_handler(struct mt5788_dev_info *di)
{
	int ret;

	ret = mt5788_tx_get_ept_type(di, &di->tx_ept_type);
	ret += mt5788_tx_clear_ept_src(di);
	if (ret)
		return;

	switch (di->tx_ept_type) {
	case MT5788_TX_EPT_RX:
	case MT5788_TX_EPT_CEP_TIMEOUT:
	case MT5788_TX_EPT_RX_RST:
	case MT5788_TX_EPT_SS:
		di->tx_ept_type &= ~MT5788_TX_EPT_CEP_TIMEOUT;
		power_event_bnc_notify(POWER_BNT_WLTX, POWER_NE_WLTX_CEP_TIMEOUT, NULL);
		break;
	case MT5788_TX_EPT_PT_OCP:
	case MT5788_TX_EPT_FOD:
		di->tx_ept_type &= ~MT5788_TX_EPT_FOD;
		power_event_bnc_notify(POWER_BNT_WLTX, POWER_NE_WLTX_TX_FOD, NULL);
		break;
	default:
		break;
	}
}

static int mt5788_tx_clear_irq(struct mt5788_dev_info *di, u32 itr)
{
	int ret;

	ret = mt5788_write_dword(di, MT5788_TX_INT_CLEAR_ADDR, itr);
	ret += mt5788_write_dword_mask(di, MT5788_TX_CMD_ADDR,
		MT5788_TX_CMD_CLEAR_INT_MASK, MT5788_TX_CMD_CLEAR_INT_SHIFT, MT5788_TX_CMD_VALUE);
	return ret;
}

static void mt5788_tx_ask_pkt_handler(struct mt5788_dev_info *di)
{
	if (di->irq_val & MT5788_TX_INT_SS_PKG_RCVD) {
		di->irq_val &= ~MT5788_TX_INT_SS_PKG_RCVD;
		if (di->g_val.qi_hdl && di->g_val.qi_hdl->hdl_qi_ask_pkt)
			di->g_val.qi_hdl->hdl_qi_ask_pkt(di);
	}

	if (di->irq_val & MT5788_TX_INT_ID_PKT_RCVD) {
		di->irq_val &= ~MT5788_TX_INT_ID_PKT_RCVD;
		if (di->g_val.qi_hdl && di->g_val.qi_hdl->hdl_qi_ask_pkt)
			di->g_val.qi_hdl->hdl_qi_ask_pkt(di);
	}

	if (di->irq_val & MT5788_TX_INT_CFG_PKT_RCVD) {
		di->irq_val &= ~MT5788_TX_INT_CFG_PKT_RCVD;
		if (di->g_val.qi_hdl && di->g_val.qi_hdl->hdl_qi_ask_pkt)
			di->g_val.qi_hdl->hdl_qi_ask_pkt(di);
		power_event_bnc_notify(POWER_BNT_WLTX, POWER_NE_WLTX_GET_CFG, NULL);
	}

	if (di->irq_val & MT5788_TX_INT_ASK_PKT_RCVD) {
		di->irq_val &= ~MT5788_TX_INT_ASK_PKT_RCVD;
		if (di->g_val.qi_hdl && di->g_val.qi_hdl->hdl_non_qi_ask_pkt)
			di->g_val.qi_hdl->hdl_non_qi_ask_pkt(di);
	}
}

static void mt5788_tx_show_irq(u32 intr)
{
	unsigned int i;

	hwlog_info("[tx_show_irq] irq=0x%04x\n", intr);
	for (i = 0; i < ARRAY_SIZE(g_mt5788_tx_irq_name); i++) {
		if (intr & BIT(i))
			hwlog_info("[tx_show_irq] %s\n", g_mt5788_tx_irq_name[i]);
	}
}

static int mt5788_tx_get_interrupt(struct mt5788_dev_info *di, u32 *intr)
{
	int ret;

	ret = mt5788_read_dword(di, MT5788_TX_INT_FLAG_ADDR, intr);
	if (ret)
		return ret;

	mt5788_tx_show_irq(*intr);
	return 0;
}

static void mt5788_tx_mode_irq_recheck(struct mt5788_dev_info *di)
{
	int ret;
	u32 irq_val = 0;

	if (gpio_get_value(di->gpio_int))
		return;

	hwlog_info("[tx_mode_irq_recheck] gpio_int low, re-check irq\n");
	ret = mt5788_tx_get_interrupt(di, &irq_val);
	if (ret)
		return;

	mt5788_tx_clear_irq(di, MT5788_TX_CLEAR_ALL_INT);
}

void mt5788_tx_mode_irq_handler(struct mt5788_dev_info *di)
{
	int ret;

	if (!di)
		return;

	ret = mt5788_tx_get_interrupt(di, &di->irq_val);
	if (ret) {
		hwlog_err("irq_handler: get irq failed, clear\n");
		mt5788_tx_clear_irq(di, MT5788_TX_CLEAR_ALL_INT);
		goto rechk_irq;
	}

	mt5788_tx_clear_irq(di, di->irq_val);
	mt5788_tx_ask_pkt_handler(di);

	if (di->irq_val & MT5788_TX_INT_START_PING) {
		di->irq_val &= ~MT5788_TX_INT_START_PING;
		power_event_bnc_notify(POWER_BNT_WLTX, POWER_NE_WLTX_PING_RX, NULL);
	}
	if (di->irq_val & MT5788_TX_INT_EPT) {
		di->irq_val &= ~MT5788_TX_INT_EPT;
		mt5788_tx_ept_handler(di);
	}
	if (di->irq_val & MT5788_TX_INT_AC_DET) {
		di->irq_val &= ~MT5788_TX_INT_AC_DET;
		power_event_bnc_notify(POWER_BNT_WLTX, POWER_NE_WLTX_RCV_DPING, NULL);
	}
	if (di->irq_val & MT5788_TX_INT_RPP_TIMEOUT) {
		di->irq_val &= ~MT5788_TX_INT_RPP_TIMEOUT;
		power_event_bnc_notify(POWER_BNT_WLTX, POWER_NE_WLTX_RP_DM_TIMEOUT, NULL);
	}

	if (di->irq_val & MT5788_TX_INT_TX_PING_OCP) {
		di->irq_val &= ~MT5788_TX_INT_TX_PING_OCP;
		/* not send ept when ping ocp */
		power_event_bnc_notify(POWER_BNT_WLTX, POWER_NE_WLTX_TX_FOD, NULL);
	}

	if (di->irq_val & MT5788_TX_INT_TX_FOD) {
		di->irq_val &= ~MT5788_TX_INT_TX_FOD;
		power_event_bnc_notify(POWER_BNT_WLTX, POWER_NE_WLTX_TX_FOD, NULL);
	}
rechk_irq:
	mt5788_tx_mode_irq_recheck(di);
}

static void mt5788_tx_debug_register(struct mt5788_dev_info *di)
{
	char dir_name[WLTRX_DEBUG_DIR_LEN] = { 0 };

	if (wlic_dbg_tx_get_dirname(di->ic_type, dir_name, WLTRX_DEBUG_DIR_LEN) < 0)
		return;

	power_dbg_ops_register(dir_name, "param", di, wlic_dbg_tx_param_show, wlic_dbg_tx_param_store);
}

static struct wltx_ic_ops g_mt5788_tx_ic_ops = {
	.get_dev_node           = mt5788_dts_dev_node,
	.fw_update              = mt5788_fw_sram_update,
	.chip_init              = mt5788_tx_chip_init,
	.chip_reset             = mt5788_tx_chip_reset,
	.chip_enable            = mt5788_chip_enable,
	.mode_enable            = mt5788_tx_enable_tx_mode,
	.set_open_flag          = mt5788_tx_set_tx_open_flag,
	.set_stop_cfg           = mt5788_tx_stop_config,
	.prevfod_chk            = mt5788_tx_prevfod_chk,
	.is_rx_discon           = mt5788_tx_check_rx_disconnect,
	.is_in_tx_mode          = mt5788_tx_is_in_tx_mode,
	.is_in_rx_mode          = mt5788_tx_is_in_rx_mode,
	.get_vrect              = mt5788_tx_get_vrect,
	.get_vin                = mt5788_tx_get_vin,
	.get_iin                = mt5788_tx_get_iin,
	.get_temp               = mt5788_tx_get_temp,
	.get_fop                = mt5788_tx_get_fop,
	.get_cep                = mt5788_tx_get_cep,
	.get_duty               = mt5788_tx_get_duty,
	.get_ptx                = mt5788_tx_get_ptx,
	.get_prx                = mt5788_tx_get_prx,
	.get_ploss              = mt5788_tx_get_ploss,
	.get_ploss_id           = mt5788_tx_get_ploss_id,
	.get_ping_freq          = mt5788_tx_get_ping_freq,
	.get_ping_interval      = mt5788_tx_get_ping_interval,
	.get_min_fop            = mt5788_tx_get_min_fop,
	.get_max_fop            = mt5788_tx_get_max_fop,
	.get_full_bridge_ith    = NULL,
	.get_curfod_iin         = mt5788_tx_get_curfod_iin,
	.set_ping_freq          = mt5788_tx_set_ping_freq,
	.set_ping_interval      = mt5788_tx_set_ping_interval,
	.set_min_fop            = mt5788_tx_set_min_fop,
	.set_max_fop            = mt5788_tx_set_max_fop,
	.set_ilim               = mt5788_tx_set_ilimit,
	.set_bridge             = mt5788_tx_set_bridge,
	.set_min_duty           = mt5788_tx_set_min_duty,
	.set_max_duty           = mt5788_tx_set_max_duty,
	.set_ploss              = mt5788_tx_set_ploss,
	.set_open_loop          = mt5788_tx_set_open_loop,
	.get_dbg_info           = wlic_tx_get_dbg_info,
};

int mt5788_tx_ops_register(struct wltrx_ic_ops *ops, struct mt5788_dev_info *di)
{
	if (!ops || !di)
		return -ENODEV;

	ops->tx_ops = kzalloc(sizeof(*(ops->tx_ops)), GFP_KERNEL);
	if (!ops->tx_ops)
		return -ENODEV;

	mt5788_tx_debug_register(di);
	(void)memcpy_s(ops->tx_ops, sizeof(g_mt5788_tx_ic_ops), &g_mt5788_tx_ic_ops, sizeof(g_mt5788_tx_ic_ops));
	ops->tx_ops->dev_data = (void *)di;
	return wltx_ic_ops_register(ops->tx_ops, di->ic_type);
}
