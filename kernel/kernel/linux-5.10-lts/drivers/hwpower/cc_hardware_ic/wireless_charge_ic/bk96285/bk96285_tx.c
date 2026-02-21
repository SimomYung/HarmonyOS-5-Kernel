// SPDX-License-Identifier: GPL-2.0
/*
 * bk96285_tx.c
 *
 * bk96285 tx driver
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

#define HWLOG_TAG wireless_bk96285_tx
HWLOG_REGIST();

static const char * const g_bk96285_tx_irq_name[] = {
	/* [n]: n means bit in registers */
	[0]  = "power_on",
	[1]  = "ss_pkt",
	[2]  = "id_pkt",
	[3]  = "cfg_pkt",
	[4]  = "otp_change",
	[6]  = "recv_ask",
	[7]  = "recv_pkt",
	[8]  = "ocp",
	[9]  = "ovp",
	[10] = "uvp",
	[11] = "otp",
	[12] = "ac_detect",
	[13] = "half_bridge",
	[14] = "full_bridge",
	[15] = "ppp_fail",
	[16] = "bridge_ocp",
	[17] = "ept",
	[18] = "enter_tx",
	[19] = "fsk_sent",
	[20] = "remove_power",
	[21] = "pt",
	[22] = "ping",
	[23] = "cep_timeout",
	[24] = "rpp_timeout",
	[25] = "rpp_type_error",
	[26] = "ping_ovp",
	[27] = "ping_ocp",
	[28] = "fod",
	[29] = "detect_rx",
	[30] = "no_rx_detected",
	[31] = "cc_mode",
};

static const char * const g_bk96285_tx_ept_name[] = {
	/* [n]: n means bit in registers */
	[0]  = "ept_otp",
	[1]  = "ept_fod",
	[2]  = "ept_rx_ept",
	[3]  = "ept_cep_timeout",
	[4]  = "ept_rpp_timeout",
	[5]  = "ept_ping_timeout",
	[6]  = "ept_ac_det",
	[7]  = "ept_power_hold_off",
	[8]  = "ept_xid",
};

static bool bk96285_tx_is_in_tx_mode(void *dev_data)
{
	int ret;
	u32 mode = 0;

	ret = bk96285_read_dword(dev_data, BK96285_SYS_MODE_ADDR, &mode);
	if (ret) {
		hwlog_err("is_tx_mode: get op_mode failed\n");
		return false;
	}

	return mode & BK96285_SYS_MODE_TX;
}

static bool bk96285_tx_is_in_rx_mode(void *dev_data)
{
	int ret;
	u32 mode = 0;

	ret = bk96285_read_dword(dev_data, BK96285_SYS_MODE_ADDR, &mode);
	if (ret) {
		hwlog_err("is_tx_mode: get op_mode failed\n");
		return false;
	}

	return mode & BK96285_SYS_MODE_RX;
}

static int bk96285_tx_set_tx_open_flag(bool enable, void *dev_data)
{
	struct bk96285_dev_info *di = dev_data;

	if (!di)
		return -ENODEV;

	di->g_val.tx_open_flag = enable;
	return 0;
}

static void bk96285_tx_chip_reset(void *dev_data)
{
	int ret;
	struct bk96285_dev_info *di = dev_data;

	if (!di)
		return;

	di->need_ignore_irq = true;
	ret = bk96285_write_dword(dev_data, BK96285_TX_CMD_ADDR, BK96285_TX_CMD_SYS_RST);
	if (ret) {
		hwlog_err("chip_reset: set cmd failed\n");
		goto exit;
	}

	power_msleep(DT_MSLEEP_10MS, 0, NULL);
	hwlog_info("[chip_reset] succ\n");

exit:
	di->need_ignore_irq = false;
}

static int bk96285_tx_set_bridge(unsigned int v_ask, unsigned int type, void *dev_data)
{
	int ret = 0;
	u8 mask, shift, value;
	struct bk96285_dev_info *di = dev_data;

	if (!di)
		return -ENODEV;

	switch (type) {
	case WLTX_PING_HALF_BRIDGE:
		mask = BK96285_TX_PING_BRG_MASK;
		shift = BK96285_TX_PING_BRG_SHIFT;
		value = BK96285_TX_HALF_BRG;
		break;
	case WLTX_PING_FULL_BRIDGE:
		mask = BK96285_TX_PING_BRG_MASK;
		shift = BK96285_TX_PING_BRG_SHIFT;
		value = BK96285_TX_FULL_BRG;
		break;
	case WLTX_PT_HALF_BRIDGE:
		mask = BK96285_TX_PT_BRG_MASK;
		shift = BK96285_TX_PT_BRG_SHIFT;
		value = BK96285_TX_HALF_BRG;
		break;
	case WLTX_PT_FULL_BRIDGE:
		mask = BK96285_TX_PT_BRG_MASK;
		shift = BK96285_TX_PT_BRG_SHIFT;
		value = BK96285_TX_FULL_BRG;
		break;
	default:
		return -EINVAL;
	}

	ret += bk96285_write_dword(di, BK96285_TX_CMD_ADDR, BK96285_TX_CMD_BRG_MANUAL);
	ret += bk96285_write_byte_mask(di, BK96285_TX_BRG_CTRL_ADDR, mask, shift, value);

	return ret;
}

static bool bk96285_tx_check_rx_disconnect(void *dev_data)
{
	struct bk96285_dev_info *di = dev_data;

	if (!di)
		return true;

	if (di->tx_ept_type & BK96285_TX_EPT_SRC_CEP_TIMEOUT) {
		di->tx_ept_type &= ~BK96285_TX_EPT_SRC_CEP_TIMEOUT;
		hwlog_info("[check_rx_disconnect] rx disconnect\n");
		return true;
	}

	return false;
}

static int bk96285_tx_get_ping_interval(u16 *ping_interval, void *dev_data)
{
	return bk96285_read_word(dev_data, BK96285_TX_PING_INTERVAL_ADDR, ping_interval);
}

static int bk96285_tx_set_ping_interval(u16 ping_interval, void *dev_data)
{
	if ((ping_interval < BK96285_TX_PING_INTERVAL_MIN) ||
		(ping_interval > BK96285_TX_PING_INTERVAL_MAX)) {
		hwlog_err("set_ping_interval: para out of range\n");
		return -EINVAL;
	}

	return bk96285_write_word(dev_data, BK96285_TX_PING_INTERVAL_ADDR, ping_interval);
}

static int bk96285_tx_get_ping_freq(u16 *ping_freq, void *dev_data)
{
	int ret;

	ret = bk96285_read_word(dev_data, BK96285_TX_PING_FREQ_ADDR, ping_freq);
	if (ret)
		return ret;
	*ping_freq = (u16)(*ping_freq / BK96285_TX_PING_FREQ_UNIT);

	return 0;
}

static int bk96285_tx_set_ping_freq(u16 ping_freq, void *dev_data)
{
	int dbg, ret;

	dbg = wlic_dbg_tx_get_param(WLTX_PARAM_PING_FREQ);
	if (dbg >= 0)
		ping_freq = (u16)dbg;
	if ((ping_freq < BK96285_TX_PING_FREQ_MIN) ||
		(ping_freq > BK96285_TX_PING_FREQ_MAX)) {
		hwlog_err("set_ping_frequency: para out of range\n");
		return -EINVAL;
	}

	ret = bk96285_write_word(dev_data, BK96285_TX_PING_FREQ_ADDR,
		(u16)(ping_freq * BK96285_TX_PING_FREQ_UNIT));
	ret += bk96285_write_dword(dev_data, BK96285_TX_CMD_ADDR, BK96285_TX_CMD_PWM_CHANGE);
	return ret;
}

static int bk96285_tx_set_ping_duty(u16 ping_duty, void *dev_data)
{
	int dbg;

	dbg = wlic_dbg_tx_get_param(WLTX_PARAM_PING_DUTY);
	if (dbg >= 0)
		ping_duty = (u16)dbg;
	if ((ping_duty < BK96285_TX_PING_DUTY_MIN) ||
		(ping_duty > BK96285_TX_PING_DUTY_MAX)) {
		hwlog_err("set_ping_duty: para out of range\n");
		return -EINVAL;
	}

	hwlog_info("set_ping_duty: duty=%u\n", ping_duty);
	return bk96285_write_word(dev_data, BK96285_TX_PING_DUTY_ADDR,
		(u16)(ping_duty * BK96285_TX_PING_DUTY_UNIT / BK96285_TX_PING_DUTY_MAX));
}

static int bk96285_tx_get_min_fop(u16 *fop, void *dev_data)
{
	int ret;

	ret = bk96285_read_word(dev_data, BK96285_TX_MIN_FOP_ADDR, fop);
	if (ret)
		return ret;

	*fop = (u16)(*fop / BK96285_TX_FOP_UNIT);
	return 0;
}

static int bk96285_tx_set_min_fop(u16 fop, void *dev_data)
{
	int dbg, ret;

	dbg = wlic_dbg_tx_get_param(WLTX_PARAM_PT_FOP_MIN);
	if (dbg >= 0)
		fop = (u16)dbg;
	if ((fop < BK96285_TX_MIN_FOP) || (fop > BK96285_TX_MAX_FOP)) {
		hwlog_err("set_min_fop: para out of range\n");
		return -EINVAL;
	}

	ret = bk96285_write_word(dev_data, BK96285_TX_MIN_FOP_ADDR, (u16)(fop * BK96285_TX_FOP_UNIT));
	ret += bk96285_write_dword(dev_data, BK96285_TX_CMD_ADDR, BK96285_TX_CMD_PWM_CHANGE);
	return ret;
}

static int bk96285_tx_get_max_fop(u16 *fop, void *dev_data)
{
	int ret;

	ret = bk96285_read_word(dev_data, BK96285_TX_MAX_FOP_ADDR, fop);
	if (ret)
		return ret;

	*fop = (u16)(*fop / BK96285_TX_FOP_UNIT);
	return 0;
}

static int bk96285_tx_get_full_bridge_ith(u16 *ith, void *dev_data)
{
	struct bk96285_dev_info *di = dev_data;

	if (!ith)
		return -EINVAL;

	if (!di)
		*ith = BK96285_TX_FULL_BRI_ITH;
	else
		*ith = (u16)di->full_bridge_ith;
	return 0;
}

static int bk96285_tx_set_max_fop(u16 fop, void *dev_data)
{
	int dbg, ret;

	dbg = wlic_dbg_tx_get_param(WLTX_PARAM_PT_FOP_MAX);
	if (dbg >= 0)
		fop = (u16)dbg;
	if ((fop < BK96285_TX_MIN_FOP) || (fop > BK96285_TX_MAX_FOP)) {
		hwlog_err("set_max_fop: para out of range\n");
		return -EINVAL;
	}

	ret = bk96285_write_word(dev_data, BK96285_TX_MAX_FOP_ADDR, (u16)(fop * BK96285_TX_FOP_UNIT));
	ret += bk96285_write_dword(dev_data, BK96285_TX_CMD_ADDR, BK96285_TX_CMD_PWM_CHANGE);
	return ret;
}

static int bk96285_tx_set_min_duty(int duty, void *dev_data)
{
	int dbg, ret;

	dbg = wlic_dbg_tx_get_param(WLTX_PARAM_PT_DUTY_MIN);
	if (dbg >= 0)
		duty = dbg;
	if ((duty < BK96285_TX_PT_DUTY_MIN_DEFAULT) ||
		(duty > BK96285_TX_PT_DUTY_MAX_DEFAULT)) {
		hwlog_err("set_min_duty: para out of range\n");
		return -EINVAL;
	}

	ret = bk96285_write_word(dev_data, BK96285_TX_PT_DUTY_MIN_ADDR,
		(u16)(duty * BK96285_TX_PT_DUTY_UNIT / BK96285_TX_PT_DUTY_MAX));
	ret += bk96285_write_dword(dev_data, BK96285_TX_CMD_ADDR, BK96285_TX_CMD_PWM_CHANGE);
	return ret;
}

static int bk96285_tx_set_max_duty(int duty, void *dev_data)
{
	int dbg, ret;

	dbg = wlic_dbg_tx_get_param(WLTX_PARAM_PT_DUTY_MAX);
	if (dbg >= 0)
		duty = dbg;
	if ((duty < BK96285_TX_PT_DUTY_MIN_DEFAULT) ||
		(duty > BK96285_TX_PT_DUTY_MAX_DEFAULT)) {
		hwlog_err("set_max_duty: para out of range\n");
		return -EINVAL;
	}

	ret = bk96285_write_word(dev_data, BK96285_TX_PT_DUTY_MAX_ADDR,
		(u16)(duty * BK96285_TX_PT_DUTY_UNIT / BK96285_TX_PT_DUTY_MAX));
	ret += bk96285_write_dword(dev_data, BK96285_TX_CMD_ADDR, BK96285_TX_CMD_PWM_CHANGE);
	return ret;
}

static int bk96285_tx_get_fop(u16 *fop, void *dev_data)
{
	int ret;

	ret = bk96285_read_word(dev_data, BK96285_TX_FOP_FREQ_ADDR, fop);
	*fop = *fop / BK96285_TX_FOP_UNIT;
	return ret;
}

static int bk96285_tx_get_cep(s8 *cep, void *dev_data)
{
	return bk96285_read_byte(dev_data, BK96285_TX_CEP_ADDR, cep);
}

static int bk96285_tx_get_duty(u8 *duty, void *dev_data)
{
	int ret;

	if (!duty) {
		hwlog_err("get_duty: para null\n");
		return -EINVAL;
	}

	ret = bk96285_read_byte(dev_data, BK96285_TX_CURR_DUTY_ADDR, duty);
	*duty = *duty * BK96285_TX_PT_DUTY_MAX / BK96285_TX_PT_DUTY_UNIT;
	return ret;
}

static int bk96285_tx_get_ptx(u32 *ptx, void *dev_data)
{
	return bk96285_read_word(dev_data, BK96285_TX_POWER_ADDR, (u16 *)ptx);
}

static int bk96285_tx_get_prx(u32 *prx, void *dev_data)
{
	return bk96285_read_word(dev_data, BK96285_RX_POWER_ADDR, (u16 *)prx);
}

static int bk96285_tx_get_ploss(s32 *ploss, void *dev_data)
{
	s16 val = 0;

	if (!ploss || bk96285_read_word(dev_data, BK96285_TX_PLOSS_ADDR, &val))
		return -EINVAL;

	*ploss = val;
	return 0;
}

static int bk96285_tx_get_temp(s16 *chip_temp, void *dev_data)
{
	int ret;

	ret = bk96285_read_word(dev_data, BK96285_TX_CHIP_TEMP_ADDR, chip_temp);
	*chip_temp = *chip_temp / BK96285_TX_TEMP_UNIT;

	return ret;
}

static int bk96285_tx_get_vin(u16 *tx_vin, void *dev_data)
{
	return bk96285_read_word(dev_data, BK96285_TX_VIN_ADDR, tx_vin);
}

static int bk96285_tx_get_vrect(u16 *tx_vrect, void *dev_data)
{
	return bk96285_read_word(dev_data, BK96285_TX_VRECT_ADDR, tx_vrect);
}

static int bk96285_tx_get_iin(u16 *tx_iin, void *dev_data)
{
	return bk96285_read_word(dev_data, BK96285_TX_IIN_ADDR, tx_iin);
}

static int bk96285_tx_get_curfod_iin(u16 *tx_iin, void *dev_data)
{
	return bk96285_read_word(dev_data, BK96285_TX_CURFOD_IIN_ADDR, tx_iin);
}

static int bk96285_tx_set_ilimit(u16 tx_ilim, void *dev_data)
{
	int dbg;

	dbg = wlic_dbg_tx_get_param(WLTX_PARAM_PT_ILIMIT);
	if (dbg >= 0)
		tx_ilim = (u16)dbg;
	if ((tx_ilim < BK96285_TX_ILIM_MIN) || (tx_ilim > BK96285_TX_ILIM_MAX)) {
		hwlog_err("set_ilimit: out of range\n");
		return -EINVAL;
	}

	hwlog_info("set ilimit=%dmA\n", tx_ilim);
	return bk96285_write_word(dev_data, BK96285_TX_ILIM_ADDR, tx_ilim);
}

static int bk96285_tx_set_ploss(u8 rx_type, void *dev_data)
{
	int ret;
	u8 *tx_fod = NULL;
	struct bk96285_dev_info *di = dev_data;

	if (!di)
		return -ENODEV;

	if (di->tx_fod_fmt == WLTX_FOD_STATUS_FMT_OLD) {
		ret = bk96285_write_word(di, BK96285_TX_PLOSS_TH0_ADDR,
			(u16)di->tx_fod.ploss_th0);
		ret += bk96285_write_word(di, BK96285_TX_PLOSS_TH1_ADDR,
			(u16)di->tx_fod.ploss_th1);
		ret += bk96285_write_word(di, BK96285_TX_PLOSS_TH3_ADDR,
			(u16)di->tx_fod.ploss_th2);
	} else if (di->tx_fod_fmt == WLTX_FOD_STATUS_FMT_NEW) {
		tx_fod = wltx_get_fod_ploss_th(di->ic_type,
			rx_type, BK96285_TX_FOD_LEN);
		if (!tx_fod)
			return -EPERM;
		ret = bk96285_write_block(di, BK96285_TX_FOD_ADDR,
			tx_fod, BK96285_TX_FOD_LEN);
	} else {
		hwlog_err("set_ploss: get error\n");
		return -EINVAL;
	}

	ret += bk96285_write_byte(di, BK96285_TX_PLOSS_CNT_ADDR,
		(u8)di->tx_fod.ploss_cnt);
	ret += bk96285_write_dword(dev_data, BK96285_TX_CMD_ADDR,
		BK96285_TX_CMD_FOD_UPDATE);
	if (ret) {
		hwlog_err("set_ploss: failed\n");
		return ret;
	}
	return 0;
}

static int bk96285_tx_stop_config(void *dev_data)
{
	struct bk96285_dev_info *di = dev_data;

	if (!di)
		return -ENODEV;

	di->g_val.tx_stop_chrg = true;
	return 0;
}

static void bk96285_tx_prevfod_chk(int flag, void *dev_data)
{
	struct bk96285_dev_info *di = dev_data;

	if (!di)
		return;

	hwlog_info("[prevfod_chk] flag=%d\n", flag);
	switch (flag) {
	case WLTX_PREVFOD_CHK_START:
		(void)bk96285_tx_set_min_duty(di->tx_fodchk_duty_min, di);
		(void)bk96285_tx_set_max_duty(di->tx_fodchk_duty_max, di);
		break;
	case WLTX_PREVFOD_CHK_STOP:
		(void)bk96285_tx_set_min_duty(di->duty_min, di);
		(void)bk96285_tx_set_max_duty(di->duty_max, di);
		break;
	default:
		break;
	}
}

static int bk96285_sw2tx(struct bk96285_dev_info *di)
{
	int ret;
	int i;
	u32 mode = 0;

	di->need_ignore_irq = true;
	/* 500ms=25ms*20 timeout for switching to tx mode */
	for (i = 0; i < 20; i++) {
		power_msleep(DT_MSLEEP_25MS, 0, NULL);
		ret = bk96285_get_mode(di, &mode);
		if (ret) {
			hwlog_err("sw2tx: get mode failed\n");
			continue;
		}
		if (mode & BK96285_SYS_MODE_TX) {
			power_msleep(DT_MSLEEP_100MS, 0, NULL);
			hwlog_info("sw2tx: succ, cnt=%d\n", i);
			ret = 0;
			goto exit;
		}
		ret = bk96285_write_dword(di, BK96285_TX_CMD_ADDR, BK96285_TX_CMD_EN_TX);
		if (ret)
			hwlog_err("sw2tx: write cmd(sw2tx) failed\n");
	}
	hwlog_err("sw2tx: failed, cnt=%d\n", i);
	ret = -ENXIO;

exit:
	di->need_ignore_irq = false;
	return ret;
}

static int bk96285_tx_set_open_loop(bool enable, void *dev_data)
{
	u32 val = enable ? BK96285_TX_OPEN_LOOP_EN : 0; /* 1:en open loop 0:disable open loop */

	/*
	 * in openloop mode, rx cannot adjust the tx parameter. fop,duty etc.
	 * tx not stop when rx send cet/rpp timeout.
	 */
	hwlog_info("[set_open_loop] %s\n", enable ? "enable" : "disable");
	return bk96285_write_dword_mask(dev_data, BK96285_TX_CTRL_ADDR,
		BK96285_TX_CTRL_OPENLOOP_MASK, BK96285_TX_CTRL_OPENLOOP_SHIFT, val);
}

static int bk96285_tx_set_ping_ocp(int value, struct bk96285_dev_info *di)
{
	int dbg;

	dbg = wlic_dbg_tx_get_param(WLTX_PARAM_PING_OCP);
	if (dbg >= 0)
		value = dbg;
	hwlog_info("[set_ping_ocp] %d\n", value);
	return bk96285_write_word(di, BK96285_TX_PING_OCP_TH_ADDR, (u16)value);
}

static int bk96285_tx_set_pt_ocp(int value, struct bk96285_dev_info *di)
{
	int dbg;
	int ret;

	dbg = wlic_dbg_tx_get_param(WLTX_PARAM_PT_OCP);
	if (dbg >= 0)
		value = dbg;
	hwlog_info("[set_pt_ocp] %d\n", value);
	ret = bk96285_write_word(di, BK96285_TX_PT_OCP_TH_ADDR, (u16)value);

	return ret;
}

static int bk96285_tx_set_pt_ovp(int value, struct bk96285_dev_info *di)
{
	int ret;

	hwlog_info("[set_pt_ovp] %d\n", value);
	ret = bk96285_write_word(di, BK96285_TX_PT_OVP_TH_ADDR, (u16)value);
	ret += bk96285_write_dword(di, BK96285_TX_CMD_ADDR, BK96285_TX_CMD_OVP_CHANGE);

	return ret;
}

static void bk96285_tx_print_reg_val(struct bk96285_dev_info *di)
{
	u16 reg;
	u32 val = 0;

	for (reg = 0; reg < BK96285_TX_REG_NUM; reg += BK96285_REG_STEP) {
		bk96285_read_dword(di, reg, &val);
		hwlog_info("[print_reg_val] reg[0x%x]=0x%x\n", reg, val);
	}
}

static void bk96285_tx_select_init_para(struct bk96285_dev_info *di,
	unsigned int client)
{
	di->tx_init_para.ocp_th = di->tx_pt_ocp_th;
	di->tx_init_para.ping_ocp_th = BK96285_TX_PING_OCP_TH_DEFAULT;
	di->tx_init_para.ping_duty = BK96285_TX_PING_DUTY;

	switch (client) {
	case WLTX_CLIENT_UI:
		di->tx_init_para.ping_freq = di->tx_ping_freq;
		di->tx_init_para.ping_duty = di->tx_ping_duty;
		di->tx_init_para.ping_ocp_th = di->ping_ocp_th;
		di->tx_init_para.ping_interval = BK96285_TX_PING_INTERVAL;
		break;
	case WLTX_CLIENT_COIL_TEST:
		di->tx_init_para.ping_freq = di->coil_test_ping_freq;
		di->tx_init_para.ping_interval = BK96285_COIL_TEST_PING_INTERVAL;
		di->tx_init_para.ping_duty = di->coil_test_ping_duty;
		break;
	case WLTX_CLIENT_BAT_HEATING:
		di->tx_init_para.ping_freq = BK96285_BAT_HEATING_PING_FREQ;
		di->tx_init_para.ping_interval = BK96285_BAT_HEATING_PING_INTERVAL;
		di->tx_init_para.ocp_th = BK96285_TX_BAT_HEATING_OCP_TH;
		break;
	default:
		di->tx_init_para.ping_freq = di->tx_ping_freq;
		di->tx_init_para.ping_interval = BK96285_TX_PING_INTERVAL;
		break;
	}
}

static int bk96285_tx_set_init_para(struct bk96285_dev_info *di)
{
	int ret;

	ret = bk96285_sw2tx(di);
	if (ret) {
		hwlog_err("set_init_para: sw2tx failed\n");
		return ret;
	}

	ret = bk96285_tx_set_pt_ocp(di->tx_init_para.ocp_th, di);
	ret += bk96285_write_dword(di, BK96285_TX_IRQ_EN_ADDR, BK96285_TX_IRQ_VAL);
	ret += bk96285_tx_set_ping_freq(di->tx_init_para.ping_freq, di);
	ret += bk96285_tx_set_ping_duty(di->tx_init_para.ping_duty, di);
	ret += bk96285_tx_set_ping_ocp(di->tx_init_para.ping_ocp_th, di);
	ret += bk96285_tx_set_min_fop(BK96285_TX_MIN_FOP, di);
	ret += bk96285_tx_set_max_fop(BK96285_TX_MAX_FOP, di);
	ret += bk96285_tx_set_min_duty(di->duty_min, di);
	ret += bk96285_tx_set_max_duty(di->duty_max, di);
	ret += bk96285_write_byte(di, BK96285_TX_FSK_DELAY_ADDR, BK96285_TX_FSK_DELAY);
	ret += bk96285_tx_set_ping_interval(di->tx_init_para.ping_interval, di);
	ret += bk96285_write_dword(di, BK96285_TX_CMD_ADDR, BK96285_TX_CMD_BRG_AUTO);
	ret += bk96285_write_dword(di, BK96285_TX_CMD_ADDR, BK96285_TX_CMD_FOD_ENABLE);
	if (ret) {
		hwlog_err("set_init_para: write failed\n");
		return -EIO;
	}

	if (di->dbg_param.print_log)
		bk96285_tx_print_reg_val(di);

	return 0;
}

static int bk96285_tx_chip_init(unsigned int client, void *dev_data)
{
	struct bk96285_dev_info *di = dev_data;
	u32 data = 0;

	if (!di)
		return -ENODEV;

	(void)bk96285_read_dword(di, BK96285_SYS_MODE_ADDR, &data);
	if ((client == WLTX_CLIENT_LIGHTSTRAP) && (data & BK96285_TX_PT_PHASE)) {
		hwlog_info("[chip_init] already in sink_pt state\n");
		power_event_bnc_notify(POWER_BNT_WLTX, POWER_NE_WLTX_GET_CFG, NULL);
		return 0;
	}
	bk96285_tx_chip_reset(di);

	di->irq_cnt = 0;
	di->g_val.irq_abnormal = false;
	di->g_val.tx_stop_chrg = false;
	bk96285_enable_irq_wake(di);
	bk96285_enable_irq(di);

	(void)bk96285_set_product_type(di, di->product_type);
	bk96285_tx_select_init_para(di, client);
	return bk96285_tx_set_init_para(di);
}

static int bk96285_tx_enable_tx_mode(bool enable, void *dev_data)
{
	int ret;

	if (enable)
		ret = bk96285_write_dword(dev_data, BK96285_TX_CMD_ADDR, BK96285_TX_CMD_START_PING);
	else
		ret = bk96285_write_dword(dev_data, BK96285_TX_CMD_ADDR, BK96285_TX_CMD_STOP_PING);

	if (ret)
		hwlog_err("%s tx_mode failed\n", enable ? "enable" : "disable");

	return ret;
}

static void bk96285_tx_show_ept_type(struct bk96285_dev_info *di, u16 ept)
{
	unsigned int i;
	char **ept_name = NULL;
	int len;

	hwlog_info("[show_ept_type] type=0x%02x", ept);

	ept_name = (char **)g_bk96285_tx_ept_name;
	len = ARRAY_SIZE(g_bk96285_tx_ept_name);

	for (i = 0; i < len; i++) {
		if (ept & BIT(i))
			hwlog_info("[tx_ept] %s\n", ept_name[i]);
	}
}

static int bk96285_tx_get_ept_type(struct bk96285_dev_info *di, u32 *ept)
{
	int ret;

	if (!ept) {
		hwlog_err("get_ept_type: para null\n");
		return -EINVAL;
	}

	ret = bk96285_read_dword(di, BK96285_TX_EPT_SRC_ADDR, ept);
	if (ret) {
		hwlog_err("get_ept_type: read failed\n");
		return ret;
	}

	bk96285_tx_show_ept_type(di, *ept);
	return 0;
}

static int bk96285_tx_clear_ept_src(struct bk96285_dev_info *di)
{
	return bk96285_write_dword(di, BK96285_TX_EPT_SRC_ADDR, BK96285_TX_EPT_SRC_CLR);
}

static void bk96285_tx_ept_handler(struct bk96285_dev_info *di)
{
	int ret;
	u16 q_detect = 0;

	ret = bk96285_tx_get_ept_type(di, &di->tx_ept_type);
	ret += bk96285_tx_clear_ept_src(di);
	if (ret)
		return;

	switch (di->tx_ept_type) {
	case BK96285_TX_EPT_SRC_RX_EPT:
	case BK96285_TX_EPT_SRC_CEP_TIMEOUT:
		di->tx_ept_type &= ~BK96285_TX_EPT_SRC_CEP_TIMEOUT;
		power_event_bnc_notify(POWER_BNT_WLTX, POWER_NE_WLTX_CEP_TIMEOUT, NULL);
		break;
	case BK96285_TX_EPT_SRC_FOD:
		di->tx_ept_type &= ~BK96285_TX_EPT_SRC_FOD;
		power_event_bnc_notify(POWER_BNT_WLTX, POWER_NE_WLTX_TX_FOD, NULL);
		break;
	default:
		break;
	}
}

static int bk96285_tx_clear_irq(struct bk96285_dev_info *di, u32 itr)
{
	int ret;

	ret = bk96285_write_dword(di, BK96285_TX_IRQ_CLR_ADDR, itr);
	ret += bk96285_write_dword(di, BK96285_TX_CMD_ADDR, BK96285_TX_CMD_CLR_IRQ);
	return ret;
}

static void bk96285_tx_ask_pkt_handler(struct bk96285_dev_info *di)
{
	u8 val = 0;

	if (di->irq_val & BK96285_TX_IRQ_SS_PKG_RCVD) {
		di->irq_val &= ~BK96285_TX_IRQ_SS_PKG_RCVD;
		if (di->g_val.qi_hdl && di->g_val.qi_hdl->hdl_qi_ask_pkt)
			di->g_val.qi_hdl->hdl_qi_ask_pkt(di);
	}

	if (di->irq_val & BK96285_TX_IRQ_ID_PKT_RCVD) {
		di->irq_val &= ~BK96285_TX_IRQ_ID_PKT_RCVD;
		if (di->g_val.qi_hdl && di->g_val.qi_hdl->hdl_qi_ask_pkt)
			di->g_val.qi_hdl->hdl_qi_ask_pkt(di);
	}

	if (di->irq_val & BK96285_TX_IRQ_CFG_PKT_RCVD) {
		di->irq_val &= ~BK96285_TX_IRQ_CFG_PKT_RCVD;
		if (di->g_val.qi_hdl && di->g_val.qi_hdl->hdl_qi_ask_pkt)
			di->g_val.qi_hdl->hdl_qi_ask_pkt(di);
		power_event_bnc_notify(POWER_BNT_WLTX, POWER_NE_WLTX_GET_CFG, NULL);
	}

	if (di->irq_val & BK96285_TX_IRQ_RX_PKT_RCVD) {
		di->irq_val &= ~BK96285_TX_IRQ_RX_PKT_RCVD;
		if (di->g_val.qi_hdl && di->g_val.qi_hdl->hdl_non_qi_ask_pkt)
			di->g_val.qi_hdl->hdl_non_qi_ask_pkt(di);
	}
}

static void bk96285_tx_show_irq(struct bk96285_dev_info *di, u32 intr)
{
	unsigned int i;
	int len;
	char **irq_name = NULL;

	hwlog_info("[show_irq] irq=0x%04x\n", intr);
	irq_name = (char **)g_bk96285_tx_irq_name;
	len = ARRAY_SIZE(g_bk96285_tx_irq_name);

	for (i = 0; i < len; i++) {
		if (intr & BIT(i))
			hwlog_info("[tx_irq] %s\n", irq_name[i]);
	}
}

static int bk96285_tx_get_interrupt(struct bk96285_dev_info *di, u32 *intr)
{
	int ret;

	ret = bk96285_read_dword(di, BK96285_TX_IRQ_ADDR, intr);
	if (ret)
		return ret;

	bk96285_tx_show_irq(di, *intr);
	return 0;
}

static void bk96285_tx_mode_irq_recheck(struct bk96285_dev_info *di)
{
	int ret;
	u32 irq_val = 0;

	if (gpio_get_value(di->gpio_int))
		return;

	hwlog_info("[tx_mode_irq_recheck] gpio_int low, re-check irq\n");
	ret = bk96285_tx_get_interrupt(di, &irq_val);
	if (ret)
		return;

	bk96285_tx_clear_irq(di, BK96285_TX_IRQ_CLR_ALL);
}

void bk96285_tx_mode_irq_handler(struct bk96285_dev_info *di)
{
	int ret;

	if (!di)
		return;

	ret = bk96285_tx_get_interrupt(di, &di->irq_val);
	if (ret) {
		hwlog_err("irq_handler: get irq failed, clear\n");
		bk96285_tx_clear_irq(di, BK96285_TX_IRQ_CLR_ALL);
		goto rechk_irq;
	}

	bk96285_tx_clear_irq(di, di->irq_val);
	bk96285_tx_ask_pkt_handler(di);

	if (di->irq_val & BK96285_TX_IRQ_START_PING) {
		di->irq_val &= ~BK96285_TX_IRQ_START_PING;
		power_event_bnc_notify(POWER_BNT_WLTX, POWER_NE_WLTX_PING_RX, NULL);
	}
	if (di->irq_val & BK96285_TX_IRQ_EPT_PKT_RCVD) {
		di->irq_val &= ~BK96285_TX_IRQ_EPT_PKT_RCVD;
		bk96285_tx_ept_handler(di);
	}
	if (di->irq_val & BK96285_TX_IRQ_AC_DET) {
		di->irq_val &= ~BK96285_TX_IRQ_AC_DET;
		power_event_bnc_notify(POWER_BNT_WLTX, POWER_NE_WLTX_RCV_DPING, NULL);
	}
	if (di->irq_val & BK96285_TX_IRQ_RPP_TIMEOUT) {
		di->irq_val &= ~BK96285_TX_IRQ_RPP_TIMEOUT;
		power_event_bnc_notify(POWER_BNT_WLTX, POWER_NE_WLTX_RP_DM_TIMEOUT, NULL);
	}
	if (di->irq_val & BK96285_TX_IRQ_REMOVE_POWER) {
		di->irq_val &= ~BK96285_TX_IRQ_REMOVE_POWER;
		bk96285_tx_ept_handler(di);
	}

rechk_irq:
	bk96285_tx_mode_irq_recheck(di);
}

static ssize_t bk96285_rx_print_reg_val_store(void *dev_data, const char *buf, size_t size)
{
	u32 print_log = 0;
	struct bk96285_dev_info *di = NULL;

	di = (struct bk96285_dev_info *)dev_data;
	if (!di)
		return -ENODEV;

	hwlog_info("[print_reg_val_store] input=%s\n", buf);
	if (sscanf_s(buf, "%u", &print_log) <= 0) {
		hwlog_err("print_reg_val_store: unable to parse input %s\n", buf);
		return -EINVAL;
	}

	di->dbg_param.print_log = print_log ? true : false;
	return (ssize_t)size;
}

static void bk96285_tx_debug_register(struct bk96285_dev_info *di)
{
	char dir_name[WLTRX_DEBUG_DIR_LEN] = { 0 };

	if (wlic_dbg_tx_get_dirname(di->ic_type, dir_name, WLTRX_DEBUG_DIR_LEN) < 0)
		return;

	power_dbg_ops_register(dir_name, "param", di, wlic_dbg_tx_param_show, wlic_dbg_tx_param_store);
	power_dbg_ops_register(dir_name, "print_reg_val", di, NULL, bk96285_rx_print_reg_val_store);
}

static struct wltx_ic_ops g_bk96285_tx_ic_ops = {
	.get_dev_node           = bk96285_dts_dev_node,
	.fw_update              = bk96285_fw_sram_update,
	.chip_init              = bk96285_tx_chip_init,
	.chip_reset             = bk96285_tx_chip_reset,
	.chip_enable            = bk96285_chip_enable,
	.mode_enable            = bk96285_tx_enable_tx_mode,
	.set_open_flag          = bk96285_tx_set_tx_open_flag,
	.set_stop_cfg           = bk96285_tx_stop_config,
	.prevfod_chk            = bk96285_tx_prevfod_chk,
	.is_rx_discon           = bk96285_tx_check_rx_disconnect,
	.is_in_tx_mode          = bk96285_tx_is_in_tx_mode,
	.is_in_rx_mode          = bk96285_tx_is_in_rx_mode,
	.get_vrect              = bk96285_tx_get_vrect,
	.get_vin                = bk96285_tx_get_vin,
	.get_iin                = bk96285_tx_get_iin,
	.get_curfod_iin         = bk96285_tx_get_curfod_iin,
	.get_temp               = bk96285_tx_get_temp,
	.get_fop                = bk96285_tx_get_fop,
	.get_cep                = bk96285_tx_get_cep,
	.get_duty               = bk96285_tx_get_duty,
	.get_ptx                = bk96285_tx_get_ptx,
	.get_prx                = bk96285_tx_get_prx,
	.get_ploss              = bk96285_tx_get_ploss,
	.get_ploss_id           = NULL,
	.get_ping_freq          = bk96285_tx_get_ping_freq,
	.get_ping_interval      = bk96285_tx_get_ping_interval,
	.get_min_fop            = bk96285_tx_get_min_fop,
	.get_max_fop            = bk96285_tx_get_max_fop,
	.get_full_bridge_ith    = bk96285_tx_get_full_bridge_ith,
	.set_ping_freq          = bk96285_tx_set_ping_freq,
	.set_ping_interval      = bk96285_tx_set_ping_interval,
	.set_min_fop            = bk96285_tx_set_min_fop,
	.set_max_fop            = bk96285_tx_set_max_fop,
	.set_ilim               = bk96285_tx_set_ilimit,
	.set_bridge             = bk96285_tx_set_bridge,
	.set_min_duty           = bk96285_tx_set_min_duty,
	.set_max_duty           = bk96285_tx_set_max_duty,
	.set_ploss              = bk96285_tx_set_ploss,
	.set_open_loop          = bk96285_tx_set_open_loop,
	.get_dbg_info           = wlic_tx_get_dbg_info,
	.set_ping_duty          = bk96285_tx_set_ping_duty,
};

int bk96285_tx_ops_register(struct wltrx_ic_ops *ops, struct bk96285_dev_info *di)
{
	if (!ops || !di)
		return -ENODEV;

	ops->tx_ops = kzalloc(sizeof(*(ops->tx_ops)), GFP_KERNEL);
	if (!ops->tx_ops)
		return -ENODEV;

	bk96285_tx_debug_register(di);
	memcpy_s(ops->tx_ops, sizeof(g_bk96285_tx_ic_ops),
		&g_bk96285_tx_ic_ops, sizeof(g_bk96285_tx_ic_ops));
	ops->tx_ops->dev_data = (void *)di;
	return wltx_ic_ops_register(ops->tx_ops, di->ic_type);
}
