// SPDX-License-Identifier: GPL-2.0
/*
 * bk96285_rx.c
 *
 * bk96285 rx driver
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

#define HWLOG_TAG wireless_bk96285_rx
HWLOG_REGIST();

#define BK96285_VFC_MAP_SIZE      4
#define BK96285_RX_CM_LEN         3
#define BK96285_RX_LDO_ON_RETRY   5

static const char * const g_bk96285_rx_irq_name[] = {
	/* [n]: n means bit in registers */
	[0]  = "rx_power_on",
	[1]  = "ss_pkg",
	[2]  = "id_cfg",
	[3]  = "cfg_pkg",
	[4]  = "otp",
	[6]  = "fsk_ack",
	[7]  = "fsk_pkg",
	[8]  = "ocp",
	[9]  = "vout_ovp",
	[10] = "lvp",
	[11] = "otp",
	[12] = "ac_det",
	[13] = "half_brg",
	[14] = "full_brg",
	[15] = "ppp_fail",
	[16] = "ldo_scp",
	[17] = "ldo_opp",
	[18] = "ldo_ops",
	[19] = "fc_succ",
	[20] = "fc_timeout",
	[21] = "watchdog_timeout",
	[22] = "full_sync",
	[23] = "half_sync",
	[24] = "diode_mode",
	[26] = "rx_ready",
	[27] = "ldo_on",
	[28] = "ldo_off",
	[29] = "clamp_ovp",
	[30] = "ngate_ovp",
};

static struct bk96285_vfc_map const rx_vfc_nonscx_map[BK96285_VFC_MAP_SIZE] = {
	{ 5000, 5000 }, { 9000, 9000 }, { 12000, 9000 }, { 15000, 9000 }, /* mV */
};
static struct bk96285_vfc_map const rx_vfc_scx_map[BK96285_VFC_MAP_SIZE] = {
	{ 5000, 5000 }, { 9000, 9000 }, { 12000, 12000 }, { 15000, 15000 }, /* mV */
};

static int bk96285_rx_set_dummy_iload(struct bk96285_dev_info *di, unsigned int init_type);

static int bk96285_rx_get_temp(int *temp, void *dev_data)
{
	u16 val = 0;
	int ret;

	ret =  bk96285_read_word(dev_data, BK96285_RX_CHIP_TEMP_ADDR, &val);
	*temp = (int)val / BK96285_RX_CHIP_TEMP_STEP;
	return ret;
}

static int bk96285_rx_get_fop(int *fop, void *dev_data)
{
	int ret;

	ret = bk96285_read_word(dev_data, BK96285_RX_FOP_ADDR, (u16 *)fop);
	if (ret)
		return ret;

	*fop = (*fop / BK96285_RX_FOP_STEP);
	return 0;
}

static int bk96285_rx_get_cep(int *cep, void *dev_data)
{
	int ret;
	s8 val = 0;

	ret = bk96285_read_byte(dev_data, BK96285_RX_CEP_VAL_ADDR, (u8 *)&val);
	*cep = val;
	return ret;
}

static int bk96285_rx_get_vrect(int *vrect, void *dev_data)
{
	return bk96285_read_word(dev_data, BK96285_RX_VRECT_ADDR, (u16 *)vrect);
}

static int bk96285_rx_get_ping_fop(int *fop, void *dev_data)
{
	int ret;

	ret = bk96285_read_word(dev_data, BK96285_RX_PING_FOP_VAL_ADDR, (u16 *)fop);
	*fop /= BK96285_RX_PING_FOP_VAL_UNIT;
	return ret;
}

static int bk96285_rx_get_ping_vrect(int *vrect, void *dev_data)
{
	return bk96285_read_word(dev_data, BK96285_RX_PING_VRECT_VAL_ADDR, (u16 *)vrect);
}

static int bk96285_rx_get_vout(int *vout, void *dev_data)
{
	return bk96285_read_word(dev_data, BK96285_RX_VOUT_ADC_ADDR, (u16 *)vout);
}

static int bk96285_rx_get_iout(int *iout, void *dev_data)
{
	return bk96285_read_word(dev_data, BK96285_RX_IOUT_ADDR, (u16 *)iout);
}

static void bk96285_rx_get_iavg(int *iavg, void *dev_data)
{
	struct bk96285_dev_info *di = dev_data;

	if (!di)
		return;

	wlic_get_rx_iavg(di->ic_type, iavg);
}

static void bk96285_rx_get_imax(int *imax, void *dev_data)
{
	struct bk96285_dev_info *di = dev_data;

	if (!di) {
		if (imax)
			*imax = WLIC_DEFAULT_RX_IMAX;
		return;
	}
	wlic_get_rx_imax(di->ic_type, imax);
}

static int bk96285_rx_get_rx_vout_reg(int *vreg, void *dev_data)
{
	return bk96285_read_word(dev_data, BK96285_RX_VOUT_ADDR, (u16 *)vreg);
}

static int bk96285_rx_get_vfc_reg(int *vfc_reg, void *dev_data)
{
	return bk96285_read_word(dev_data, BK96285_RX_FC_VTX_ADDR, (u16 *)vfc_reg);
}

static void bk96285_rx_show_irq(u32 intr)
{
	u32 i;

	for (i = 0; i < ARRAY_SIZE(g_bk96285_rx_irq_name); i++) {
		if (intr & BIT(i))
			hwlog_info("[rx_show_irq] %s\n", g_bk96285_rx_irq_name[i]);
	}
}

static int bk96285_rx_get_interrupt(struct bk96285_dev_info *di, u32 *intr)
{
	int ret;

	ret = bk96285_read_dword(di, BK96285_RX_INTR_FLAG_ADDR, intr);
	if (ret)
		return ret;

	hwlog_info("[get_interrupt] irq=0x%08x\n", *intr);
	bk96285_rx_show_irq(*intr);
	return 0;
}

static int bk96285_rx_clear_irq(struct bk96285_dev_info *di, u32 intr)
{
	int ret;

	ret = bk96285_write_dword(di, BK96285_RX_INTR_CLR_ADDR, intr);
	if (ret) {
		hwlog_err("clear_irq: failed\n");
		return ret;
	}

	ret = bk96285_write_dword_mask(di, BK96285_RX_CMD_ADDR,
		BK96285_RX_CMD_CLEAR_IRQ_MASK, BK96285_RX_CMD_CLEAR_IRQ_SHIFT,
		BK96285_RX_CMD_EN);

	return ret;
}

static void bk96285_sleep_enable(bool enable, void *dev_data)
{
	int gpio_val;
	struct bk96285_dev_info *di = dev_data;

	if (!di || di->g_val.irq_abnormal)
		return;

	gpio_set_value(di->gpio_sleep_en,
		enable ? WLTRX_IC_SLEEP_ENABLE : WLTRX_IC_SLEEP_DISABLE);
	gpio_val = gpio_get_value(di->gpio_sleep_en);
	hwlog_info("[sleep_enable] gpio %s now\n", gpio_val ? "high" : "low");
}


static bool bk96285_is_sleep_enable(void *dev_data)
{
	int gpio_val;
	struct bk96285_dev_info *di = dev_data;

	if (!di)
		return false;

	gpio_val = gpio_get_value(di->gpio_sleep_en);
	return gpio_val == WLTRX_IC_SLEEP_ENABLE;
}

static void bk96285_rx_chip_reset(void *dev_data)
{
	int ret;
	struct bk96285_dev_info *di = dev_data;

	if (!di)
		return;

	di->need_ignore_irq = true;
	ret = bk96285_write_dword_mask(dev_data, BK96285_RX_CMD_ADDR,
		BK96285_RX_CMD_RESET_MASK, BK96285_RX_CMD_RESET_SHIFT, BK96285_RX_CMD_EN);
	if (ret) {
		hwlog_err("chip_reset: set cmd failed\n");
		goto exit;
	}

	hwlog_info("[chip_reset] succ\n");
exit:
	di->need_ignore_irq = false;
}

static void bk96285_rx_set_fod_offset(u8 offset, void *dev_data)
{
	struct bk96285_dev_info *di = dev_data;

	if (!di)
		return;

	bk96285_write_byte(di, BK96285_RX_FOD_C0_ADDR, offset);
	bk96285_write_byte(di, BK96285_RX_FOD_C1_ADDR, offset);
	bk96285_write_byte(di, BK96285_RX_FOD_C2_ADDR, offset);
	bk96285_write_byte(di, BK96285_RX_FOD_C3_ADDR, offset);
	bk96285_write_dword_mask(dev_data, BK96285_RX_CMD_ADDR,
		BK96285_RX_CMD_UPDATE_FOD_MASK, BK96285_RX_CMD_UPDATE_FOD_SHIFT,
		BK96285_RX_CMD_EN);
}

static void bk96285_rx_set_turbo_chg_flag(bool flag, void *dev_data)
{
	struct bk96285_dev_info *di = dev_data;

	if (!di)
		return;

	hwlog_info("[set_turbo_chg_flag] %s\n", flag ? "true" : "false");
	wlic_iout_select_para(di->ic_type, WLICI_FACTOR_TURBO, flag);
}

static void bk96285_rx_set_cur_pmode_name(const char *cur_pmode_name, void *dev_data)
{
	struct bk96285_dev_info *di = dev_data;

	if (!di || !cur_pmode_name)
		return;

	hwlog_info("[set_cur_pmode_name] %s\n", cur_pmode_name);
	wlic_iout_set_cur_pmode_name(di->ic_type, cur_pmode_name);
}

static int bk96285_rx_set_rx_vout(int vout, void *dev_data)
{
	int ret;
	int count = 0;
	struct bk96285_dev_info *di = dev_data;

	if (!di)
		return -EINVAL;

	if (!di->g_val.vfc_set_complete) {
		hwlog_err("set_rx_vout: vfc is not set complete\n");
		return -EINVAL;
	}

	if ((vout < BK96285_RX_VOUT_MIN) || (vout > BK96285_RX_VOUT_MAX)) {
		hwlog_err("set_rx_vout: out of range\n");
		return -EINVAL;
	}

	/* When the ldo off, vout setting does not take effect */
	while (!di->ldo_on && (vout <= WLRX_IC_HIGH_VOUT1) &&
		(count++ < BK96285_RX_LDO_ON_RETRY))
		power_usleep(DT_USLEEP_20MS);

	ret = bk96285_write_word(di, BK96285_RX_VOUT_ADDR, (u16)vout);
	if (ret) {
		hwlog_err("set_rx_vout: write addr failed\n");
		return ret;
	}

	ret = bk96285_write_dword_mask(di, BK96285_RX_CMD_ADDR,
		BK96285_RX_CMD_SET_VOUT_MASK, BK96285_RX_CMD_SET_VOUT_SHIFT,
		BK96285_RX_CMD_EN);
	if (ret) {
		hwlog_err("set_rx_vout: write cmd failed\n");
		return ret;
	}

	return 0;
}

static void bk96285_ask_mode_cfg(struct bk96285_dev_info *di, u8 cm, u8 polarity)
{
	int ret;
	u8 askcap;

	askcap = (cm << BK96285_RX_CM_SHIFT) | (polarity << BK96285_RX_CM_POLY_SHIFT) |
		BK96285_RX_CM_CTRL_BY_AP;
	hwlog_info("[ask_mode_cfg] cap=0x%x\n", askcap);
	ret = bk96285_write_byte(di, BK96285_RX_CM_CFG_ADDR, askcap);
	if (ret)
		hwlog_err("ask_mode_cfg: failed\n");
}

static int bk96285_set_intfr_mode_cfg(struct bk96285_dev_info *di)
{
	int i;
	u8 cm_cfg, polar_cfg;

	if ((di->cm_intfr_cfg_level <= 0) || (di->sysfs_cm_type <= 0))
		return -EINVAL;

	for (i = 0; i < di->cm_intfr_cfg_level; i++) {
		if (di->sysfs_cm_type == (int)di->cm_intfr_cfg[i].type) {
			cm_cfg = di->cm_intfr_cfg[i].cm;
			polar_cfg = di->cm_intfr_cfg[i].polar;
			bk96285_ask_mode_cfg(di, cm_cfg, polar_cfg);
			return 0;
		}
	}

	return -EINVAL;
}

static void bk96285_set_mode_cfg(struct bk96285_dev_info *di, int vfc, bool ctrl)
{
	u8 cm_cfg, polar_cfg;

	if (!bk96285_set_intfr_mode_cfg(di) || !ctrl)
		return;

	if (vfc <= WLRX_IC_HIGH_VOUT1) {
		cm_cfg = di->rx_mod_cfg.cm.l_volt;
		polar_cfg = di->rx_mod_cfg.polar.l_volt;
	} else if (vfc < WLRX_IC_HIGH_VOUT2) {
		cm_cfg = di->rx_mod_cfg.cm.fac_h_volt;
		polar_cfg = di->rx_mod_cfg.polar.fac_h_volt;
	} else {
		cm_cfg = di->rx_mod_cfg.cm.h_volt;
		polar_cfg = di->rx_mod_cfg.polar.h_volt;
	}
	bk96285_ask_mode_cfg(di, cm_cfg, polar_cfg);
}

static int bk96285_rx_set_cm(int cm_type, void *dev_data)
{
	int vfc_reg = 0;
	struct bk96285_dev_info *di = dev_data;

	if (!di || (di->cm_intfr_cfg_level <= 0))
		return -EINVAL;

	hwlog_info("[rx_set_cm] cm_type: %d\n", cm_type);
	di->sysfs_cm_type = cm_type;
	(void)bk96285_rx_get_vfc_reg(&vfc_reg, di);
	bk96285_set_mode_cfg(di, vfc_reg, true);
	return 0;
}

static int bk96285_rx_get_vmldo_by_vfc_map(int vfc, void *dev_data)
{
	int i;
	struct bk96285_vfc_map const *map = NULL;
	struct bk96285_dev_info *di = dev_data;

	if (!di)
		return -ENODEV;

	if (di->is_scx_mode || !di->need_ctrl_brg)
		map = (struct bk96285_vfc_map *)rx_vfc_scx_map;
	else
		map = (struct bk96285_vfc_map *)rx_vfc_nonscx_map;
	for (i = 0; i < BK96285_VFC_MAP_SIZE; i++) {
		if (vfc == map[i].vpa) {
			hwlog_info("[get_vmldo] vmldo=%dmv\n", map[i].vmldo);
			return map[i].vmldo;
		}
	}

	hwlog_err("get_vmldo: mismatch\n");
	return -EINVAL;
}

static int bk96285_rx_set_vmldo(struct bk96285_dev_info *di, u16 vfc)
{
	int vmldo;

	vmldo = bk96285_rx_get_vmldo_by_vfc_map(vfc, di);
	if (vmldo < 0)
		return -EINVAL;

	return bk96285_write_word(di, BK96285_RX_FC_VRX_ADDR, (u16)vmldo);
}

static int bk96285_rx_send_fc_cmd(struct bk96285_dev_info *di, int vfc)
{
	int ret;

	if (!bk96285_rx_is_tx_exist(di))
		return -EPERM;

	ret = bk96285_write_word(di, BK96285_RX_FC_VTX_ADDR, vfc);
	if (ret) {
		hwlog_err("send_fc_cmd: set fc_volt failed\n");
		return ret;
	}

	ret = bk96285_write_dword_mask(di, BK96285_RX_CMD_ADDR,
		BK96285_RX_CMD_SEND_FC_MASK, BK96285_RX_CMD_SEND_FC_SHIFT, BK96285_RX_CMD_EN);
	if (ret) {
		hwlog_err("send_fc_cmd: send fc_cmd failed\n");
		return ret;
	}

	return 0;
}

static bool bk96285_rx_is_fc_succ(struct bk96285_dev_info *di, int vset)
{
	int i;
	int cnt;

	if (!bk96285_rx_is_tx_exist(di))
		return false;

	cnt = BK96285_RX_FC_VPA_TIMEOUT / BK96285_RX_FC_VPA_SLEEP_TIME;
	for (i = 0; i < cnt; i++) {
		if ((di->g_val.rx_stop_chrg && (vset > WLRX_IC_DFLT_VOUT)) ||
			!di->can_boost || !bk96285_rx_is_tx_exist(di))
			return false;
		power_msleep(BK96285_RX_FC_VPA_SLEEP_TIME, 0, NULL); /* wait for vout stability */
		if (di->fc_succ) {
			hwlog_info("[is_fc_succ] succ, cost_time: %dms\n",
				(i + 1) * BK96285_RX_FC_VPA_SLEEP_TIME);
			return true;
		}
	}

	hwlog_err("is_fc_succ: timeout\n");
	return false;
}

static bool bk96285_rx_is_vmldo_set_succ(struct bk96285_dev_info *di, int vfc)
{
	int i;
	int cnt;
	int vout = 0;
	int vmldo;

	vmldo = bk96285_rx_get_vmldo_by_vfc_map(vfc, di);
	if (vmldo < 0)
		return false;

	cnt = BK96285_RX_FC_VRX_TIMEOUT / BK96285_RX_FC_VRX_SLEEP_TIME;
	for (i = 0; i < cnt; i++) {
		if ((di->g_val.rx_stop_chrg && (vfc > WLRX_IC_DFLT_VOUT)) ||
			!bk96285_rx_is_tx_exist(di) || bk96285_is_sleep_enable(di))
			return false;
		power_msleep(BK96285_RX_FC_VRX_SLEEP_TIME, 0, NULL); /* wait for vout stability */
		(void)bk96285_rx_get_vout(&vout, di);
		if (vout >= (vmldo - 200)) { /* 200: maximum deviation */
			hwlog_info("[is_vmldo_set_succ] succ, cost_time: %dms\n",
				(i + 1) * BK96285_RX_FC_VRX_SLEEP_TIME);
			return true;
		}
	}

	hwlog_err("is_vmldo_set_succ: timeout\n");
	return false;
}

static bool bk96285_rx_is_cp_open(struct bk96285_dev_info *di)
{
	int rx_ratio;
	int rx_vset = 0;
	int rx_vout = 0;
	int cp_vout;

	if (!charge_pump_is_cp_open(CP_TYPE_MAIN)) {
		hwlog_err("rx_is_cp_open: failed\n");
		return false;
	}

	rx_ratio = charge_pump_get_cp_ratio(CP_TYPE_MAIN);
	(void)bk96285_rx_get_rx_vout_reg(&rx_vset, di);
	(void)bk96285_rx_get_vout(&rx_vout, di);
	cp_vout = charge_pump_get_cp_vout(CP_TYPE_MAIN);
	cp_vout = (cp_vout > 0) ? cp_vout : wldc_get_ls_vbus();

	hwlog_info("[is_cp_open] [rx] ratio:%d vset:%d vout:%d [cp] vout:%d\n",
		rx_ratio, rx_vset, rx_vout, cp_vout);
	if ((cp_vout * rx_ratio) < (rx_vout - BK96285_RX_FC_VOUT_ERR_LTH))
		return false;
	if ((cp_vout * rx_ratio) > (rx_vout + BK96285_RX_FC_VOUT_ERR_UTH))
		return false;

	return true;
}

static int bk96285_rx_check_cp_mode(struct bk96285_dev_info *di)
{
	int i;
	int cnt;
	int ret;

	ret = charge_pump_set_cp_mode(CP_TYPE_MAIN);
	if (ret) {
		hwlog_err("check_cp_mode: set cp_mode failed\n");
		return ret;
	}
	cnt = BK96285_RX_BPCP_TIMEOUT / BK96285_RX_BPCP_SLEEP_TIME;
	for (i = 0; i < cnt; i++) {
		(void)power_msleep(DT_MSLEEP_50MS, 0, NULL);
		if (bk96285_rx_is_cp_open(di)) {
			hwlog_info("[check_cp_mode] set cp_mode succ\n");
			return 0;
		}
		if (di->g_val.rx_stop_chrg) {
			hwlog_err("check_cp_mode: wlc already stopped\n");
			return -EPERM;
		}
	}

	hwlog_err("check_cp_mode: set cp_mode timeout\n");
	return -EIO;
}

static int bk96285_rx_set_half_bridge(struct bk96285_dev_info *di)
{
	return bk96285_write_byte(di, BK96285_RX_BRIDGE_CTRL_ADDR, BK96285_RX_HALF_BRIDGE);
}

static int bk96285_rx_set_full_bridge(struct bk96285_dev_info *di)
{
	return bk96285_write_byte(di, BK96285_RX_BRIDGE_CTRL_ADDR, BK96285_RX_FULL_BRIDGE);
}

static int bk96285_rx_set_boost_mode(struct bk96285_dev_info *di, int vfc)
{
	if (!di->need_ctrl_brg)
		return 0;

	if (power_cmdline_is_factory_mode() && di->is_scx_mode)
		return bk96285_rx_set_half_bridge(di);

	if (vfc >= WLRX_IC_HIGH_VOUT2)
		return bk96285_rx_set_full_bridge(di);

	return bk96285_rx_set_half_bridge(di);
}

static int bk96285_rx_set_vtx(struct bk96285_dev_info *di, int vfc)
{
	int i;
	int ret;

	di->can_boost = true;

	for (i = 0; i < BK96285_RX_FC_VPA_RETRY_CNT; i++) {
		if ((di->g_val.rx_stop_chrg && (vfc > WLRX_IC_DFLT_VOUT)) ||
			!di->can_boost || !bk96285_rx_is_tx_exist(di))
			break;
		di->fc_succ = false;
		ret = bk96285_rx_send_fc_cmd(di, vfc);
		if (ret) {
			hwlog_err("set_vfc: send fc_cmd failed\n");
			continue;
		}
		hwlog_info("[set_vfc] send fc_cmd sucess, cnt: %d\n", i);
		if (bk96285_rx_is_fc_succ(di, vfc)) {
			if (!bk96285_rx_is_vmldo_set_succ(di, vfc))
				continue;
			if (vfc < RX_HIGH_VOUT2)
				(void)charge_pump_set_bp_mode(CP_TYPE_MAIN);
			bk96285_set_mode_cfg(di, vfc, di->rx_mod_cfg.ctrl_by_ap);
			if (vfc == RX_HIGH_VOUT2)
				bk96285_rx_set_dummy_iload(di, RX_HIGH_VOUT2);
			hwlog_info("[set_vfc] succ\n");
			return 0;
		}
	}

	return -EINVAL;
}

static int bk96285_rx_set_vfc(int vfc, void *dev_data)
{
	int ret;
	int vfc_reg = 0;
	struct bk96285_dev_info *di = dev_data;

	if (!di)
		return -ENODEV;

	bk96285_rx_get_vfc_reg(&vfc_reg, di);
	if (vfc == vfc_reg)
		return 0;

	di->g_val.vfc_set_complete = false;
	hwlog_info("[set_vfc] vfc=%d\n", vfc);

	if ((vfc >= RX_HIGH_VOUT2) && wldc_need_cp_div2()) {
		ret = bk96285_rx_check_cp_mode(di);
		if (ret)
			goto out;
		power_msleep(DT_MSLEEP_100MS, 0, NULL); /* wait for vout stability */
	}

	ret = bk96285_rx_set_boost_mode(di, vfc);
	if (ret)
		goto out;

	ret = bk96285_rx_set_vmldo(di, vfc);
	if (ret) {
		hwlog_err("[set_vfc] send fc_vmldo failed\n");
		goto out;
	}

	ret = bk96285_rx_set_vtx(di, vfc);
out:
	di->g_val.vfc_set_complete = true;
	return ret;
}

static void bk96285_rx_set_scx_mode(bool scx_mode, void *dev_data)
{
	struct bk96285_dev_info *di = dev_data;

	if (!di)
		return;

	di->is_scx_mode = scx_mode;
}

static void bk96285_rx_send_ept(int ept_type, void *dev_data)
{
	int ret;

	switch (ept_type) {
	case HWQI_EPT_CHRG_COMPLETE:
	case HWQI_EPT_ERR_VRECT:
	case HWQI_EPT_ERR_VOUT:
		break;
	default:
		return;
	}
	hwlog_info("[send_ept] ept=0x%x\n", ept_type);
	ret = bk96285_write_dword(dev_data, BK96285_RX_EPT_MSG_ADDR, (u8)ept_type);
	ret += bk96285_write_dword_mask(dev_data, BK96285_RX_CMD_ADDR,
		BK96285_RX_CMD_SEND_EPT_MASK, BK96285_RX_CMD_SEND_EPT_SHIFT,
		BK96285_RX_CMD_EN);
	if (ret)
		hwlog_err("send_ept: failed\n");
}

bool bk96285_rx_is_tx_exist(void *dev_data)
{
	int ret;
	u32 mode = 0;

	ret = bk96285_get_mode(dev_data, &mode);
	if (ret) {
		hwlog_err("is_tx_exist: get mode failed\n");
		return false;
	}

	if (mode & BK96285_SYS_MODE_RX)
		return true;

	return false;
}

static int bk96285_rx_kick_watchdog(void *dev_data)
{
	return bk96285_write_dword_mask(dev_data, BK96285_RX_CMD_ADDR,
		BK96285_RX_CMD_KICK_WD_MASK, BK96285_RX_CMD_KICK_WD_SHIFT, BK96285_RX_CMD_EN);
}

static int bk96285_rx_get_fod(char *fod_str, int len, void *dev_data)
{
	int i;
	int ret;
	char tmp[BK96285_RX_FOD_TMP_STR_LEN] = { 0 };
	u8 fod_arr[BK96285_RX_FOD_LEN] = { 0 };

	if (!fod_str || (len < WLRX_IC_FOD_COEF_LEN))
		return -EINVAL;

	ret = bk96285_read_block(dev_data, BK96285_RX_FOD_ADDR,
		fod_arr, BK96285_RX_FOD_LEN);
	if (ret)
		return ret;

	for (i = 0; i < BK96285_RX_FOD_LEN; i++) {
		(void)snprintf_s(tmp, BK96285_RX_FOD_TMP_STR_LEN,
			BK96285_RX_FOD_TMP_STR_LEN - 1, "%x ", fod_arr[i]);
		ret = strncat_s(fod_str, len, tmp, strlen(tmp));
		if (ret != EOK)
			hwlog_err("rx_get_fod: failed\n");
	}

	return strlen(fod_str);
}

static int bk96285_rx_set_fod(const char *fod_str, void *dev_data)
{
	int ret;
	char *cur = (char *)fod_str;
	char *token = NULL;
	int i;
	u8 val = 0;
	const char *sep = " ,";
	u8 fod_arr[BK96285_RX_FOD_LEN] = { 0 };

	if (!fod_str) {
		hwlog_err("set_fod: input fod_str null\n");
		return -EINVAL;
	}

	for (i = 0; i < BK96285_RX_FOD_LEN; i++) {
		token = strsep(&cur, sep);
		if (!token) {
			hwlog_err("set_fod: input fod_str number err\n");
			return -EINVAL;
		}
		ret = kstrtou8(token, POWER_BASE_DEC, &val);
		if (ret) {
			hwlog_err("set_fod: input fod_str type err\n");
			return -EINVAL;
		}
		fod_arr[i] = val;
		hwlog_info("[set_fod] fod[%d]=0x%x\n", i, fod_arr[i]);
	}

	ret = bk96285_write_block(dev_data, BK96285_RX_FOD_ADDR,
		fod_arr, BK96285_RX_FOD_LEN);
	ret += bk96285_write_dword_mask(dev_data, BK96285_RX_CMD_ADDR,
		BK96285_RX_CMD_UPDATE_FOD_MASK, BK96285_RX_CMD_UPDATE_FOD_SHIFT,
		BK96285_RX_CMD_EN);
	return ret;
}

static int bk96285_rx_enable_ldo_opp_vbst(bool vbst_en, void *dev_data)
{
	int ret;
	struct bk96285_dev_info *di = dev_data;

	if (!di)
		return -ENODEV;

	if (vbst_en) {
		ret = bk96285_write_word(di, BK96285_RX_LDO_OPP_VBST_ADDR,
			di->ldo_opp_vbst.vbst);
		ret += bk96285_write_word(di, BK96285_RX_LDO_OPP_ITHL_ADDR,
			di->ldo_opp_vbst.iout_lth);
		ret += bk96285_write_word(di, BK96285_RX_LDO_OPP_ITHH_ADDR,
			di->ldo_opp_vbst.iout_hth);
	} else {
		ret = bk96285_write_word(di, BK96285_RX_LDO_OPP_VBST_ADDR, 0);
	}

	return ret;
}

static int bk96285_rx_set_vdrop_cfg(struct bk96285_dev_info *di, u8 *data, u8 len)
{
	return bk96285_write_block(di, BK96285_RX_LDO_CFG_ADDR, data, len);
}

static int bk96285_rx_set_dummy_iload(struct bk96285_dev_info *di, unsigned int init_type)
{
	int ret = 0;
	u16 iload_no_mod;
	u16 iload_mod;

	if (di->dummy_len <= 0)
		return 0;

	switch (init_type) {
	case WLRX_IC_5VBUCK_CHIP_INIT:
		iload_no_mod = di->dummy_iload[BK96285_DUMMY_ILOAD_5V_NO_MOD];
		iload_mod = di->dummy_iload[BK96285_DUMMY_ILOAD_5V_MOD];
		break;
	case WLRX_IC_9VBUCK_CHIP_INIT:
		iload_no_mod = di->dummy_iload[BK96285_DUMMY_ILOAD_9V_NO_MOD];
		iload_mod = di->dummy_iload[BK96285_DUMMY_ILOAD_9V_MOD];
		break;
	case WLRX_IC_SC_CHIP_INIT:
		iload_no_mod = di->dummy_iload[BK96285_DUMMY_ILOAD_SC_NO_MOD];
		iload_mod = di->dummy_iload[BK96285_DUMMY_ILOAD_SC_MOD];
		break;
	case WIRELESS_FODCHK_CHIP_INIT:
		iload_no_mod = di->dummy_iload[BK96285_DUMMY_ILOAD_FODCHK_NO_MOD];
		iload_mod = di->dummy_iload[BK96285_DUMMY_ILOAD_FODCHK_MOD];
		break;
	case RX_HIGH_VOUT2:
		iload_no_mod = di->dummy_iload[BK96285_DUMMY_ILOAD_12V_NO_MOD];
		iload_mod = di->dummy_iload[BK96285_DUMMY_ILOAD_12V_MOD];
		if (iload_mod == 0)
			return 0;
		break;
	default:
		return 0;
	}

	hwlog_info("[set_dummy_iload] %d %d\n", iload_no_mod, iload_mod);
	ret += bk96285_write_word(di, BK96285_RX_DUMMY_LOAD_NO_MOD_ADDR, iload_no_mod);
	ret += bk96285_write_word(di, BK96285_RX_DUMMY_LOAD_MOD_ADDR, iload_mod);
	ret += bk96285_write_dword_mask(di, BK96285_RX_CMD_ADDR,
		BK96285_RX_CMD_DUMM_UPT_MASK, BK96285_RX_CMD_DUMM_UPT_SHIFT,
		BK96285_RX_CMD_EN);
	return ret;
}

static int bk96285_rx_set_offset_unit(struct bk96285_dev_info *di, int val)
{
	int ret;

	hwlog_info("[set_offset_unit] %d\n", val);
	ret = bk96285_write_byte(di, BK96285_RX_FOD_OFFSET_ADDR, (u8)val);
	ret += bk96285_write_dword_mask(di, BK96285_RX_CMD_ADDR,
		BK96285_RX_CMD_UPDATE_FOD_MASK, BK96285_RX_CMD_UPDATE_FOD_SHIFT,
		BK96285_RX_CMD_EN);
	return ret;
}

static int bk96285_rx_set_gain_unit(struct bk96285_dev_info *di, int val)
{
	int ret;

	hwlog_info("[set_gain_unit] %d\n", val);
	ret = bk96285_write_byte(di, BK96285_RX_GAIN_UNIT_ADDR, (u8)val);
	ret += bk96285_write_dword_mask(di, BK96285_RX_CMD_ADDR,
		BK96285_RX_CMD_UPDATE_FOD_MASK, BK96285_RX_CMD_UPDATE_FOD_SHIFT,
		BK96285_RX_CMD_EN);
	return ret;
}

static int bk96285_rx_5vbuck_chip_init(struct bk96285_dev_info *di, unsigned int init_type)
{
	int ret = 0;

	hwlog_info("[5vbuck_chip_init] chip init\n");
	ret += bk96285_rx_set_vdrop_cfg(di, di->rx_ldo_cfg.l_volt, BK96285_RX_LDO_CFG_LEN);
	ret += bk96285_rx_set_dummy_iload(di, WLRX_IC_5VBUCK_CHIP_INIT);
	ret += bk96285_write_word(di, BK96285_RX_FC_TIMEOUT_ADDR, BK96285_RX_FC_TIMEOUT_DFT);
	ret += bk96285_rx_set_offset_unit(di, di->rx_fod_offset);
	ret += bk96285_rx_set_gain_unit(di, BK96285_RX_GAIN_UNIT_DEFAULT);
	bk96285_set_mode_cfg(di, init_type, di->rx_mod_cfg.ctrl_by_ap);
	return ret;
}

static int bk96285_rx_init_fod_coef(struct bk96285_dev_info *di, unsigned int tx_type)
{
	int vfc;
	int vrx;
	int ret;
	int vfc_reg = 0;
	u8 *rx_fod = NULL;

	(void)bk96285_rx_get_vfc_reg(&vfc_reg, di);
	hwlog_info("[init_fod_coef] vfc_reg: %dmV\n", vfc_reg);
	vrx = bk96285_rx_get_vmldo_by_vfc_map(vfc_reg, di);
	if (vrx < 9000) /* (0, 9)V, set 5v fod */
		vfc = 5000;
	else if (vrx < 15000) /* [9, 15)V, set 9V fod */
		vfc = 9000;
	else if (vrx < 18000) /* [15, 18)V, set 15V fod */
		vfc = 15000;
	else
		return -EINVAL;

	rx_fod = wlrx_get_fod_ploss_th(di->ic_type, vfc, tx_type, BK96285_RX_FOD_LEN);
	if (!rx_fod)
		return -EINVAL;

	ret = bk96285_write_block(di, BK96285_RX_FOD_ADDR, rx_fod, BK96285_RX_FOD_LEN);
	ret += bk96285_write_dword_mask(di, BK96285_RX_CMD_ADDR,
		BK96285_RX_CMD_UPDATE_FOD_MASK, BK96285_RX_CMD_UPDATE_FOD_SHIFT,
		BK96285_RX_CMD_EN);
	return ret;
}

static int bk96285_rx_fodchk_chip_init(struct bk96285_dev_info *di)
{
	int ret = 0;
	u8 offset_unit = 0;
	u8 gain_unit = 0;
	u8 offset0 = 0;

	ret += bk96285_rx_set_dummy_iload(di, WIRELESS_FODCHK_CHIP_INIT);
	ret += bk96285_write_word(di, BK96285_RX_DROP_MIN_ADDR, 0);
	ret += bk96285_write_word(di, BK96285_RX_DROP_MAX_ADDR, 0);
	ret += bk96285_write_word(di, BK96285_RX_V_OFFSET_ADDR, 0);
	ret += bk96285_write_byte(di, BK96285_RX_FOD_C0_ADDR, 0);
	ret += bk96285_write_byte(di, BK96285_RX_FOD_C1_ADDR, 0);
	ret += bk96285_write_byte(di, BK96285_RX_FOD_C2_ADDR, 0);
	ret += bk96285_write_byte(di, BK96285_RX_FOD_C3_ADDR, 0);
	ret += bk96285_write_dword_mask(di, BK96285_RX_CMD_ADDR,
		BK96285_RX_CMD_UPDATE_FOD_MASK, BK96285_RX_CMD_UPDATE_FOD_SHIFT,
		BK96285_RX_CMD_EN);
	bk96285_ask_mode_cfg(di, di->fodchk_ask_cap, BK96285_RX_CM_POSITIVE);
	ret += bk96285_write_word_mask(di, BK96285_RX_CTRL_ADDR,
		BK96285_RX_FASTRPP_EN_MASK, BK96285_RX_FASTRPP_EN_SHIFT,
		BK96285_RX_CTRL_EN);
	ret += bk96285_rx_set_gain_unit(di, di->prevfod_gain_unit);
	if (ret) {
		hwlog_err("fodchk_chip_init: write failed\n");
		return -EIO;
	}

	bk96285_read_byte(di, BK96285_RX_GAIN_UNIT_ADDR, &gain_unit);
	bk96285_read_byte(di, BK96285_RX_FOD_OFFSET_ADDR, &offset_unit);
	bk96285_read_byte(di, BK96285_RX_FOD_C0_ADDR, &offset0);
	hwlog_info("[fodchk_chip_init] gain_unit[%u] offset_unit[%u] offset0[%u]\n",
		gain_unit, offset_unit, offset0);

	return 0;
}

static int bk96285_rx_dflt_chip_init(struct bk96285_dev_info *di, unsigned int tx_type)
{
	int ret;

	hwlog_info("dflt_chip_init\n");
	di->g_val.rx_stop_chrg = false;
	ret = bk96285_write_dword(di, BK96285_RX_WDG_TIME_ADDR,
		BK96285_RX_WDG_TIMES / BK96285_RX_WDG_TIME_STEP);
	ret += bk96285_write_word(di, BK96285_RX_FC_SUCC_THR_ADDR,
		di->rx_vfc_diff);
	ret += bk96285_write_byte(di, BK96285_RX_OTP_THR_ADDR,
		BK96285_RX_OTP_THR_DEFAULT);
	ret += bk96285_write_word_mask(di, BK96285_RX_CTRL_ADDR,
		BK96285_RX_FASTRPP_EN_MASK, BK96285_RX_FASTRPP_EN_SHIFT,
		BK96285_RX_CTRL_DISABLE);
	if (di->rx_ldo_opp_en)
		ret += bk96285_write_word_mask(di, BK96285_RX_CTRL_ADDR,
			BK96285_RX_MLDO_OPP_EN_MASK, BK96285_RX_MLDO_OPP_SHIFT,
			di->rx_ldo_opp_en);
	if (ret) {
		hwlog_err("dflt_chip_init: write fail\n");
		return -EIO;
	}

	return 0;
}

static int bk96285_rx_chip_init(unsigned int init_type, unsigned int tx_type, void *dev_data)
{
	int ret = 0;
	struct bk96285_dev_info *di = dev_data;

	if (!di)
		return -ENODEV;

	switch (init_type) {
	case WLRX_IC_DFLT_CHIP_INIT:
		ret += bk96285_rx_dflt_chip_init(di, tx_type);
		/* fall through */
	case WLRX_IC_5VBUCK_CHIP_INIT:
		ret += bk96285_rx_5vbuck_chip_init(di, WLRX_IC_5VBUCK_CHIP_INIT);
		ret += bk96285_rx_init_fod_coef(di, tx_type);
		break;
	case WLRX_IC_9VBUCK_CHIP_INIT:
		hwlog_info("[chip_init] 9v chip init\n");
		ret += bk96285_rx_set_vdrop_cfg(di, di->rx_ldo_cfg.m_volt, BK96285_RX_LDO_CFG_LEN);
		ret += bk96285_rx_set_dummy_iload(di, WLRX_IC_9VBUCK_CHIP_INIT);
		ret += bk96285_rx_init_fod_coef(di, tx_type);
		break;
	case WLRX_IC_SC_CHIP_INIT:
		hwlog_info("[chip_init] sc chip init\n");
		ret += bk96285_rx_set_vdrop_cfg(di, di->rx_ldo_cfg.sc, BK96285_RX_LDO_CFG_LEN);
		ret += bk96285_rx_set_dummy_iload(di, WLRX_IC_SC_CHIP_INIT);
		ret += bk96285_rx_init_fod_coef(di, tx_type);
		break;
	case WIRELESS_FODCHK_CHIP_INIT:
		ret += bk96285_rx_fodchk_chip_init(di);
		break;
	default:
		hwlog_err("chip_init: input para invalid\n");
		break;
	}

	return ret;
}

static void bk96285_rx_stop_charging(void *dev_data)
{
	struct bk96285_dev_info *di = dev_data;

	if (!di)
		return;

	di->g_val.rx_stop_chrg = true;
	wlic_iout_stop_sample(di->ic_type);

	if (!di->g_val.irq_abnormal)
		return;

	if (wlrx_get_wired_channel_state() != WIRED_CHANNEL_ON) {
		hwlog_info("[stop_charging] irq_abnormal, keep rx_sw on\n");
		di->g_val.irq_abnormal = true;
		wlps_control(di->ic_type, WLPS_RX_SW, true);
	} else {
		di->irq_cnt = 0;
		di->g_val.irq_abnormal = false;
		bk96285_enable_irq_wake(di);
		bk96285_enable_irq(di);
		hwlog_info("[stop_charging] wired channel on, enable irq\n");
	}
}

static int bk96285_rx_data_rcvd_handler(struct bk96285_dev_info *di)
{
	int ret;
	int i;
	u8 cmd;
	u8 buff[HWQI_PKT_LEN] = { 0 };

	ret = bk96285_read_register(di, BK96285_TRX_FSK_PKG_HEADER_ADDR,
		buff, HWQI_PKT_LEN);
	if (ret) {
		hwlog_err("data_rcvd_handler: read received data failed\n");
		return ret;
	}

	cmd = buff[HWQI_PKT_CMD];
	hwlog_info("[data_rcvd_handler] cmd: 0x%x\n", cmd);
	for (i = HWQI_PKT_DATA; i < HWQI_PKT_LEN; i++)
		hwlog_info("[data_rcvd_handler] data: 0x%x\n", buff[i]);

	switch (cmd) {
	case HWQI_CMD_TX_ALARM:
	case HWQI_CMD_NAK:
	case HWQI_CMD_GET_TX_VERSION:
	case HWQI_CMD_GET_TX_SN:
		di->irq_val &= ~BK96285_RX_IRQ_FSK_PKG_RECV;
		if (di->g_val.qi_hdl &&
			di->g_val.qi_hdl->hdl_non_qi_fsk_pkt)
			di->g_val.qi_hdl->hdl_non_qi_fsk_pkt(buff, HWQI_PKT_LEN, di);
		if (cmd == HWQI_CMD_NAK)
			di->can_boost = false;
		break;
	default:
		break;
	}
	return 0;

}

void bk96285_rx_abnormal_irq_handler(struct bk96285_dev_info *di)
{
	static struct timespec64 ts64_timeout;
	struct timespec64 ts64_interval;
	struct timespec64 ts64_now;

	ts64_now = power_get_current_kernel_time64();
	ts64_interval.tv_sec = 0;
	ts64_interval.tv_nsec = WIRELESS_INT_TIMEOUT_TH * NSEC_PER_MSEC;

	if (!di)
		return;

	hwlog_info("[handle_abnormal_irq] irq_cnt = %d\n", ++di->irq_cnt);
	/* power on irq occurs first time, so start monitor now */
	if (di->irq_cnt == 1) {
		ts64_timeout = timespec64_add_safe(ts64_now, ts64_interval);
		if (ts64_timeout.tv_sec == TIME_T_MAX) {
			di->irq_cnt = 0;
			hwlog_err("handle_abnormal_irq: time overflow\n");
			return;
		}
	}

	if (timespec64_compare(&ts64_now, &ts64_timeout) < 0)
		return;

	if (di->irq_cnt < WIRELESS_INT_CNT_TH) {
		di->irq_cnt = 0;
		return;
	}

	di->g_val.irq_abnormal = true;
	wlps_control(di->ic_type, WLPS_RX_SW, true);
	bk96285_disable_irq_wake(di);
	bk96285_disable_irq_nosync(di);
	gpio_set_value(di->gpio_sleep_en, RX_SLEEP_EN_DISABLE);
	hwlog_err("handle_abnormal_irq: more than %d irq in %ds, disable irq\n",
		WIRELESS_INT_CNT_TH, WIRELESS_INT_TIMEOUT_TH / POWER_MS_PER_S);
}

static void bk96285_rx_ready_handler(struct bk96285_dev_info *di)
{
	if (wlrx_get_wired_channel_state() == WIRED_CHANNEL_ON) {
		hwlog_err("rx_ready_handler: wired channel on, ignore\n");
		return;
	}

	hwlog_info("[rx_ready_handler] rx ready, goto wireless charging\n");
	di->g_val.rx_stop_chrg = false;
	di->irq_cnt = 0;
	power_event_bnc_notify(POWER_BNT_WLRX, POWER_NE_WLRX_PREV_READY, NULL);
	power_msleep(DT_MSLEEP_50MS, 0, NULL); /* delay 50ms for volt steady */
	gpio_set_value(di->gpio_sleep_en, WLTRX_IC_SLEEP_DISABLE);
	wlic_iout_start_sample(di->ic_type);
	power_event_bnc_notify(POWER_BNT_WLRX, POWER_NE_WLRX_READY, NULL);
}

static void bk96285_rx_power_on_handler(struct bk96285_dev_info *di)
{
	u8 rx_ss = 0;
	int vrect = 0;
	int pwr_flag = WLRX_IC_PWR_ON_NOT_GOOD;

	if (wlrx_get_wired_channel_state() == WIRED_CHANNEL_ON) {
		hwlog_err("rx_power_on_handler: wired channel on, ignore\n");
		return;
	}

	(void)bk96285_set_product_type(di, di->product_type);
	(void)bk96285_rx_5vbuck_chip_init(di, WLRX_IC_5VBUCK_CHIP_INIT);
	bk96285_rx_abnormal_irq_handler(di);
	bk96285_read_byte(di, BK96285_RX_SS_VAL_ADDR, &rx_ss);
	bk96285_rx_get_vrect(&vrect, di);
	hwlog_info("[rx_power_on_handler] ss=%u vrect=%d\n", rx_ss, vrect);
	if ((rx_ss > di->rx_ss_good_lth) && (rx_ss <= BK96285_RX_SS_MAX))
		pwr_flag = WLRX_IC_PWR_ON_GOOD;
	di->ldo_on = false;
	power_event_bnc_notify(POWER_BNT_WLRX, POWER_NE_WLRX_PWR_ON, &pwr_flag);
}

static void bk96285_rx_fault_irq_handler(struct bk96285_dev_info *di)
{
	if (di->irq_val & BK96285_RX_IRQ_OCP) {
		di->irq_val &= ~BK96285_RX_IRQ_OCP;
		power_event_bnc_notify(POWER_BNT_WLRX, POWER_NE_WLRX_OCP, NULL);
	}

	if (di->irq_val & BK96285_RX_IRQ_VRECT_OVP) {
		di->irq_val &= ~BK96285_RX_IRQ_VRECT_OVP;
		power_event_bnc_notify(POWER_BNT_WLRX, POWER_NE_WLRX_OVP, NULL);
	}

	if (di->irq_val & BK96285_RX_IRQ_MLDO_OVP) {
		di->irq_val &= ~BK96285_RX_IRQ_MLDO_OVP;
		power_event_bnc_notify(POWER_BNT_WLRX, POWER_NE_WLRX_OVP, NULL);
	}

	if (di->irq_val & BK96285_RX_IRQ_OTP) {
		di->irq_val &= ~BK96285_RX_IRQ_OTP;
		power_event_bnc_notify(POWER_BNT_WLRX, POWER_NE_WLRX_OTP, NULL);
	}

	if (di->irq_val & BK96285_RX_IRQ_OPP_H) {
		di->irq_val &= ~BK96285_RX_IRQ_OPP_H;
		power_event_bnc_notify(POWER_BNT_WLRX, POWER_NE_WLRX_OPP_H, NULL);
	}
}

void bk96285_rx_mode_irq_handler(struct bk96285_dev_info *di)
{
	int ret;
	u8 rx_ss = 0;

	if (!di)
		return;

	ret = bk96285_rx_get_interrupt(di, &di->irq_val);
	hwlog_info("[mode_irq_handler] irq_val=0x%x\n", di->irq_val);
	if (ret) {
		hwlog_err("irq_handler: read irq failed, clear\n");
		bk96285_rx_clear_irq(di, BK96285_RX_INTR_CLR_ALL);
		bk96285_rx_abnormal_irq_handler(di);
		return;
	}

	bk96285_rx_clear_irq(di, di->irq_val);

	if (di->irq_val & BK96285_RX_IRQ_POWER_ON) {
		di->irq_val &= ~BK96285_RX_IRQ_POWER_ON;
		bk96285_read_byte(di, BK96285_RX_SS_VAL_ADDR, &rx_ss);
		if (rx_ss < di->rx_ss_low_thr) {
			hwlog_err("irq_handler: ss low %d, thr=%d\n", rx_ss, di->rx_ss_low_thr);
			di->irq_val = 0;
		}
		bk96285_rx_power_on_handler(di);
	}
	if (di->irq_val & BK96285_RX_IRQ_READY) {
		di->irq_val &= ~BK96285_RX_IRQ_READY;
		bk96285_rx_ready_handler(di);
	}
	if (di->irq_val & BK96285_RX_IRQ_FSK_PKG_RECV)
		bk96285_rx_data_rcvd_handler(di);
	if (di->irq_val & BK96285_RX_IRQ_FC_SUCC) {
		di->irq_val &= ~BK96285_RX_IRQ_FC_SUCC;
		di->fc_succ = true;
	}
	if (di->irq_val & BK96285_RX_IRQ_LDO_ON) {
		di->irq_val &= ~BK96285_RX_IRQ_LDO_ON;
		di->ldo_on = true;
	}
	if (di->irq_val & BK96285_RX_IRQ_LDO_OFF) {
		di->irq_val &= ~BK96285_RX_IRQ_LDO_OFF;
		di->ldo_on = false;
	}
	bk96285_rx_fault_irq_handler(di);
}

static void bk96285_rx_pmic_vbus_handler(bool vbus_state, void *dev_data)
{
	int ret;
	u32 irq_val = 0;
	struct bk96285_dev_info *di = dev_data;

	if (!di || !vbus_state || !di->g_val.irq_abnormal)
		return;

	if (wlrx_get_wired_channel_state() == WIRED_CHANNEL_ON)
		return;

	if (!bk96285_rx_is_tx_exist(di))
		return;

	ret = bk96285_rx_get_interrupt(di, &irq_val);
	if (ret) {
		hwlog_err("pmic_vbus_handler: read irq failed\n");
		return;
	}
	hwlog_info("[pmic_vbus_handler] irq_val=0x%x\n", irq_val);
	if (irq_val & BK96285_RX_IRQ_READY) {
		bk96285_rx_clear_irq(di, BK96285_RX_INTR_CLR_ALL);
		bk96285_rx_ready_handler(di);
		di->irq_cnt = 0;
		di->g_val.irq_abnormal = false;
		bk96285_enable_irq_wake(di);
		bk96285_enable_irq(di);
	}
}

void bk96285_rx_probe_check_tx_exist(struct bk96285_dev_info *di)
{
	if (!di)
		return;

	if (bk96285_rx_is_tx_exist(di)) {
		bk96285_rx_clear_irq(di, BK96285_RX_INTR_CLR_ALL);
		hwlog_info("[rx_probe_check_tx_exist] rx exist\n");
		bk96285_rx_ready_handler(di);
	} else {
		bk96285_sleep_enable(true, di);
	}
}

void bk96285_rx_shutdown_handler(struct bk96285_dev_info *di)
{
	if (!di || !bk96285_rx_is_tx_exist(di))
		return;

	wlps_control(di->ic_type, WLPS_RX_EXT_PWR, false);
	power_msleep(DT_MSLEEP_50MS, 0, NULL); /* delay 50ms for power off */
	(void)bk96285_rx_set_vfc(WLRX_IC_DFLT_VOUT, di);
	(void)bk96285_rx_set_rx_vout(WLRX_IC_DFLT_VOUT, di);
	(void)bk96285_chip_enable(false, di);
	power_msleep(DT_MSLEEP_200MS, 0, NULL); /* delay 200ms for chip disable */
	(void)bk96285_chip_enable(true, di);
}

int bk96285_get_chip_info_str(char *info_str, int len, void *dev_data)
{
	int ret;
	struct bk96285_chip_info info = { 0 };

	if (!info_str || (len < WLTRX_IC_CHIP_INFO_LEN))
		return -EINVAL;

	ret = bk96285_get_chip_info(dev_data, &info);
	if (ret)
		return -EIO;

	return snprintf_s(info_str, WLTRX_IC_CHIP_INFO_LEN, WLTRX_IC_CHIP_INFO_LEN - 1,
		"chip_id:0x%x mtp_ver=0x%x", info.chip_id, info.mtp_ver);
}

static void bk96285_rx_ext_pwr_post_ctrl(bool flag, void *dev_data)
{
	(void)bk96285_write_dword_mask(dev_data, BK96285_RX_CTRL_ADDR,
		BK96285_RX_CTRL_VDD_EN_MASK, BK96285_RX_CTRL_VDD_EN_SHIFT,
		(flag ? BK96285_RX_CTRL_EN : BK96285_RX_CTRL_DISABLE));
}

static struct wlrx_ic_ops g_bk96285_rx_ic_ops = {
	.get_dev_node           = bk96285_dts_dev_node,
	.fw_update              = bk96285_fw_sram_update,
	.chip_init              = bk96285_rx_chip_init,
	.chip_reset             = bk96285_rx_chip_reset,
	.chip_enable            = bk96285_chip_enable,
	.is_chip_enable         = bk96285_is_chip_enable,
	.sleep_enable           = bk96285_sleep_enable,
	.is_sleep_enable        = bk96285_is_sleep_enable,
	.ext_pwr_post_ctrl      = bk96285_rx_ext_pwr_post_ctrl,
	.get_chip_info          = bk96285_get_chip_info_str,
	.get_vrect              = bk96285_rx_get_vrect,
	.get_vout               = bk96285_rx_get_vout,
	.get_iout               = bk96285_rx_get_iout,
	.get_iavg               = bk96285_rx_get_iavg,
	.get_imax               = bk96285_rx_get_imax,
	.get_vout_reg           = bk96285_rx_get_rx_vout_reg,
	.get_vfc_reg            = bk96285_rx_get_vfc_reg,
	.get_ping_fop           = bk96285_rx_get_ping_fop,
	.get_ping_vrect         = bk96285_rx_get_ping_vrect,
	.set_cm                 = bk96285_rx_set_cm,
	.set_vfc                = bk96285_rx_set_vfc,
	.set_scx_mode           = bk96285_rx_set_scx_mode,
	.set_vout               = bk96285_rx_set_rx_vout,
	.get_vrx_by_vfc_map     = bk96285_rx_get_vmldo_by_vfc_map,
	.get_fop                = bk96285_rx_get_fop,
	.get_cep                = bk96285_rx_get_cep,
	.get_temp               = bk96285_rx_get_temp,
	.get_fod_coef           = bk96285_rx_get_fod,
	.set_fod_coef           = bk96285_rx_set_fod,
	.is_tx_exist            = bk96285_rx_is_tx_exist,
	.kick_watchdog          = bk96285_rx_kick_watchdog,
	.send_ept               = bk96285_rx_send_ept,
	.stop_charging          = bk96285_rx_stop_charging,
	.pmic_vbus_handler      = bk96285_rx_pmic_vbus_handler,
	.enable_ldo_opp_vbst    = bk96285_rx_enable_ldo_opp_vbst,
	.set_turbo_chg_flag     = bk96285_rx_set_turbo_chg_flag,
	.set_cur_pmode_name     = bk96285_rx_set_cur_pmode_name,
	.set_fod_offset         = bk96285_rx_set_fod_offset,
};

static ssize_t bk96285_rx_cm_store(void *dev_data, const char *buf, size_t size)
{
	int tmp[BK96285_RX_CM_LEN] = { 0 };
	struct bk96285_dev_info *di = NULL;

	di = (struct bk96285_dev_info *)dev_data;
	if (!di)
		return -ENODEV;

	/* 0~2 index */
	if (sscanf_s(buf, "%x %x %x", &tmp[0], &tmp[1], &tmp[2]) != BK96285_RX_CM_LEN) {
		hwlog_err("cm_store: unable to parse input %s\n", buf);
		return -EINVAL;
	}
	hwlog_info("[rx_cm_store] input=0x%x 0x%x 0x%x\n", tmp[0], tmp[1], tmp[2]);
	di->rx_mod_cfg.cm.l_volt = (u8)tmp[0];
	di->rx_mod_cfg.cm.h_volt = (u8)tmp[1];
	di->rx_mod_cfg.cm.fac_h_volt = (u8)tmp[2];
	di->rx_mod_cfg.ctrl_by_ap = true;
	return (ssize_t)size;
}

static ssize_t bk96285_rx_cm_show(void *dev_data, char *buf, size_t size)
{
	struct bk96285_dev_info *di = NULL;

	di = (struct bk96285_dev_info *)dev_data;
	if (!di)
		return -ENODEV;

	return snprintf_s(buf, size, size - 1, "l_volt=0x%x h_volt=0x%x fac_h_volt=0x%x\n",
		di->rx_mod_cfg.cm.l_volt, di->rx_mod_cfg.cm.h_volt, di->rx_mod_cfg.cm.fac_h_volt);
}

static ssize_t bk96285_rx_dummy_iload_store(void *dev_data, const char *buf, size_t size)
{
	int ret;
	u32 tmp[BK96285_DUMMY_ILOAD_END] = { 0 };
	struct bk96285_dev_info *di = NULL;

	di = (struct bk96285_dev_info *)dev_data;
	if (!di)
		return -ENODEV;

	/* 0~7 index */
	hwlog_info("[dummy_iload_store] input=%s\n", buf);
	if (sscanf_s(buf, "%u %u %u %u %u %u %u %u %u %u", &tmp[0], &tmp[1], &tmp[2],
		&tmp[3], &tmp[4], &tmp[5], &tmp[6], &tmp[7], &tmp[8], &tmp[9])
		!= BK96285_DUMMY_ILOAD_END) {
		hwlog_err("cm_store: unable to parse input %s\n", buf);
		return -EINVAL;
	}

	ret = memcpy_s(di->dummy_iload, sizeof(di->dummy_iload), tmp, sizeof(tmp));
	if (ret != EOK)
		hwlog_err("[dummy_iload_store] copy failed\n");

	di->dummy_len = BK96285_DUMMY_ILOAD_END;
	return (ssize_t)size;
}

static ssize_t bk96285_rx_dummy_iload_show(void *dev_data, char *buf, size_t size)
{
	struct bk96285_dev_info *di = NULL;

	di = (struct bk96285_dev_info *)dev_data;
	if (!di)
		return -ENODEV;

	return snprintf_s(buf, size, size - 1, "dummy_iload = %u %u %u %u %u %u %u %u %u %u\n",
		di->dummy_iload[BK96285_DUMMY_ILOAD_5V_NO_MOD],
		di->dummy_iload[BK96285_DUMMY_ILOAD_5V_MOD],
		di->dummy_iload[BK96285_DUMMY_ILOAD_9V_NO_MOD],
		di->dummy_iload[BK96285_DUMMY_ILOAD_9V_MOD],
		di->dummy_iload[BK96285_DUMMY_ILOAD_SC_NO_MOD],
		di->dummy_iload[BK96285_DUMMY_ILOAD_SC_MOD],
		di->dummy_iload[BK96285_DUMMY_ILOAD_FODCHK_NO_MOD],
		di->dummy_iload[BK96285_DUMMY_ILOAD_FODCHK_MOD],
		di->dummy_iload[BK96285_DUMMY_ILOAD_12V_NO_MOD],
		di->dummy_iload[BK96285_DUMMY_ILOAD_12V_MOD]);
}

static void bk96285_rx_debug_register(struct bk96285_dev_info *di)
{
	char dir_name[WLTRX_DEBUG_DIR_LEN] = { 0 };

	if (wlic_dbg_rx_get_dirname(di->ic_type, dir_name, WLTRX_DEBUG_DIR_LEN) < 0)
		return;

	power_dbg_ops_register(dir_name, "cm_cfg", di, bk96285_rx_cm_show, bk96285_rx_cm_store);
	power_dbg_ops_register(dir_name, "dummy_iload_cfg", di, bk96285_rx_dummy_iload_show,
		bk96285_rx_dummy_iload_store);
}

int bk96285_rx_ops_register(struct wltrx_ic_ops *ops, struct bk96285_dev_info *di)
{
	if (!ops || !di)
		return -ENODEV;

	ops->rx_ops = kzalloc(sizeof(*(ops->rx_ops)), GFP_KERNEL);
	if (!ops->rx_ops)
		return -ENODEV;

	bk96285_rx_debug_register(di);
	(void)memcpy_s(ops->rx_ops, sizeof(g_bk96285_rx_ic_ops),
		&g_bk96285_rx_ic_ops, sizeof(g_bk96285_rx_ic_ops));
	ops->rx_ops->dev_data = (void *)di;
	di->g_val.vfc_set_complete = true;
	return wlrx_ic_ops_register(ops->rx_ops, di->ic_type);
}
