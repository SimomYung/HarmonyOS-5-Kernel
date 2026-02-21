// SPDX-License-Identifier: GPL-2.0
/*
 * mt5788_rx.c
 *
 * mt5788 rx driver
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
#include <chipset_common/hwpower/hardware_ic/charge_pump.h>
#include <huawei_platform/power/wireless/wireless_direct_charger.h>

#define HWLOG_TAG wireless_mt5788_rx
HWLOG_REGIST();

static const char * const g_mt5788_rx_irq_name[] = {
	/* [n]: n means bit in registers */
	[0]  = "rx_chip_reset",
	[6]  = "rx_ldo_otp_l",
	[7]  = "init_rx_succ",
	[8]  = "rx_power_on",
	[9]  = "rx_ready",
	[10] = "rx_ldo_enable",
	[11] = "rx_ldo_disable",
	[12] = "rx_ppp_success",
	[13] = "rx_ppp_fail",
	[14] = "rx_fsk_recv",
	[15] = "rx_ldo_short",
	[16] = "rx_ldo_opp",
	[17] = "rx_ldo_ocp",
	[18] = "rx_ldo_ovp",
	[19] = "rx_ldo_otp",
	[20] = "rx_vout_ovp",
	[21] = "rx_brg2full",
	[22] = "rx_brg2half",
	[23] = "rx_fc_done",
	[24] = "rx_fc_timeout",
	[25] = "wdg_trig",
	[26] = "mldo_opp_h",
	[27] = "rx_enter_fullsync",
	[28] = "rx_enter_halfsync",
	[29] = "rx_enter_diode",
	[30] = "rx_ac_missing",
};

#define MT5788_VFC_MAP_SIZE      4
struct mt5788_vfc_map {
	int vpa;
	int vmldo;
};

static struct mt5788_vfc_map const rx_vfc_nonscx_map[MT5788_VFC_MAP_SIZE] = {
	{ 5000, 5000 }, { 9000, 9000 }, { 12000, 9000 }, { 15000, 9000 }, /* mV */
};
static struct mt5788_vfc_map const rx_vfc_scx_map[MT5788_VFC_MAP_SIZE] = {
	{ 5000, 5000 }, { 9000, 9000 }, { 12000, 12000 }, { 15000, 15000 }, /* mV */
};

static int mt5788_rx_get_temp(int *temp, void *dev_data)
{
	s16 l_temp = 0;

	if (!temp || mt5788_read_word(dev_data, MT5788_RX_CHIP_TEMP_ADDR, (u16 *)&l_temp))
		return -EINVAL;

	*temp = l_temp;
	return 0;
}

static int mt5788_rx_get_fop(int *fop, void *dev_data)
{
	int ret;

	ret = mt5788_read_word(dev_data, MT5788_RX_FOP_ADDR, (u16 *)fop);
	if (ret)
		return ret;

	*fop = (*fop / 10); /* 10: fop unit */
	return 0;
}

static int mt5788_rx_get_cep(int *cep, void *dev_data)
{
	s8 l_cep = 0;

	if (!cep || mt5788_read_byte(dev_data, MT5788_RX_CEP_VAL_ADDR, (u8 *)&l_cep))
		return -EINVAL;

	*cep = l_cep;
	return 0;
}

static int mt5788_rx_get_vrect(int *vrect, void *dev_data)
{
	return mt5788_read_word(dev_data, MT5788_RX_VRECT_ADDR, (u16 *)vrect);
}

static int mt5788_rx_get_ping_fop(int *fop, void *dev_data)
{
	int ret;

	ret  = mt5788_read_word(dev_data, MT5788_RX_PING_FOP_VAL_ADDR, (u16 *)fop);
	*fop /= MT5788_RX_PING_FOP_VAL_UNIT;
	return ret;
}

static int mt5788_rx_get_ping_vrect(int *vrect, void *dev_data)
{
	return mt5788_read_word(dev_data, MT5788_RX_PING_VRECT_VAL_ADDR, (u16 *)vrect);
}

static int mt5788_rx_get_vout(int *vout, void *dev_data)
{
	return mt5788_read_word(dev_data, MT5788_RX_VOUT_ADDR, (u16 *)vout);
}

static int mt5788_rx_get_iout(int *iout, void *dev_data)
{
	return mt5788_read_word(dev_data, MT5788_RX_IOUT_ADDR, (u16 *)iout);
}

static void mt5788_rx_get_iavg(int *iavg, void *dev_data)
{
	struct mt5788_dev_info *di = dev_data;

	if (!di)
		return;

	wlic_get_rx_iavg(di->ic_type, iavg);
}

static void mt5788_rx_get_imax(int *imax, void *dev_data)
{
	struct mt5788_dev_info *di = dev_data;

	if (!di) {
		if (imax)
			*imax = WLIC_DEFAULT_RX_IMAX;
		return;
	}
	wlic_get_rx_imax(di->ic_type, imax);
}

static int mt5788_rx_get_rx_vout_reg(int *vreg, void *dev_data)
{
	return mt5788_read_word(dev_data, MT5788_RX_VOUT_SET_ADDR, (u16 *)vreg);
}

static int mt5788_rx_get_vfc_reg(int *vfc_reg, void *dev_data)
{
	return mt5788_read_word(dev_data, MT5788_RX_FC_VPA_ADDR, (u16 *)vfc_reg);
}

static void mt5788_rx_show_irq(u32 intr)
{
	u32 i;

	for (i = 0; i < ARRAY_SIZE(g_mt5788_rx_irq_name); i++) {
		if (intr & BIT(i))
			hwlog_info("[rx_show_irq] %s\n", g_mt5788_rx_irq_name[i]);
	}
}

static int mt5788_rx_get_interrupt(struct mt5788_dev_info *di, u32 *intr)
{
	int ret;

	ret = mt5788_read_dword(di, MT5788_RX_IRQ_ADDR, intr);
	if (ret)
		return ret;

	hwlog_info("[get_interrupt] irq=0x%08x\n", *intr);
	mt5788_rx_show_irq(*intr);

	return 0;
}

static int mt5788_rx_clear_irq(struct mt5788_dev_info *di, u32 intr)
{
	int ret;

	ret = mt5788_write_dword(di, MT5788_RX_IRQ_CLR_ADDR, intr);
	if (ret) {
		hwlog_err("clear_irq: failed\n");
		return ret;
	}

	ret = mt5788_write_dword_mask(di, MT5788_RX_CMD_ADDR,
		MT5788_RX_CMD_CLEAR_INT_MASK, MT5788_RX_CMD_CLEAR_INT_SHIFT,
		MT5788_RX_CMD_VAL);

	return ret;
}

static void mt5788_sleep_enable(bool enable, void *dev_data)
{
	int gpio_val;
	struct mt5788_dev_info *di = dev_data;

	if (!di || di->g_val.irq_abnormal)
		return;

	gpio_set_value(di->gpio_sleep_en,
		enable ? WLTRX_IC_SLEEP_ENABLE : WLTRX_IC_SLEEP_DISABLE);
	gpio_val = gpio_get_value(di->gpio_sleep_en);
	hwlog_info("[sleep_enable] gpio %s now\n", gpio_val ? "high" : "low");
}

static bool mt5788_is_sleep_enable(void *dev_data)
{
	int gpio_val;
	struct mt5788_dev_info *di = dev_data;

	if (!di)
		return false;

	gpio_val = gpio_get_value(di->gpio_sleep_en);
	return gpio_val == WLTRX_IC_SLEEP_ENABLE;
}

static void mt5788_rx_chip_reset(void *dev_data)
{
	int ret;

	ret = mt5788_write_dword_mask(dev_data, MT5788_RX_CMD_ADDR,
		MT5788_RX_CMD_SYS_RST_MASK, MT5788_RX_CMD_SYS_RST_SHIFT,
		MT5788_RX_CMD_VAL);
	if (ret) {
		hwlog_err("chip_reset: set cmd failed\n");
		return;
	}

	hwlog_info("[chip_reset] succ\n");
}

static void mt5788_rx_set_turbo_chg_flag(bool flag, void *dev_data)
{
	struct mt5788_dev_info *di = dev_data;

	if (!di)
		return;

	hwlog_info("[set_turbo_chg_flag] %s\n", flag ? "true" : "false");
	wlic_iout_select_para(di->ic_type, WLICI_FACTOR_TURBO, flag);
}

static void mt5788_rx_set_cur_pmode_name(const char *cur_pmode_name, void *dev_data)
{
	struct mt5788_dev_info *di = dev_data;

	if (!di || !cur_pmode_name)
		return;

	hwlog_info("[set_cur_pmode_name] %s\n", cur_pmode_name);
	wlic_iout_set_cur_pmode_name(di->ic_type, cur_pmode_name);
}

static void mt5788_rx_ext_pwr_post_ctrl(bool flag, void *dev_data)
{
	if (flag)
		(void)mt5788_write_dword_mask(dev_data, MT5788_RX_CTRL_ADDR,
			MT5788_RX_CTRL_EXT_POWER_MASK, MT5788_RX_CTRL_EXT_POWER_SHIFT,
			MT5788_RX_CTRL_EN);
	else
		(void)mt5788_write_dword_mask(dev_data, MT5788_RX_CTRL_ADDR,
			MT5788_RX_CTRL_EXT_POWER_MASK, MT5788_RX_CTRL_EXT_POWER_SHIFT,
			MT5788_RX_CTRL_DIS);
}

static int mt5788_rx_set_rx_vout(int vout, void *dev_data)
{
	int ret;
	struct mt5788_dev_info *di = dev_data;

	if (!di)
		return -EINVAL;

	if (!di->g_val.vfc_set_complete) {
		hwlog_err("set_rx_vout: vfc is not set complete\n");
		return -EINVAL;
	}

	if ((vout < MT5788_RX_VOUT_MIN) || (vout > MT5788_RX_VOUT_MAX)) {
		hwlog_err("set_rx_vout: out of range\n");
		return -EINVAL;
	}

	ret = mt5788_write_word(di, MT5788_RX_VOUT_SET_ADDR, (u16)vout);
	if (ret) {
		hwlog_err("set_rx_vout: write addr failed\n");
		return ret;
	}

	ret = mt5788_write_dword_mask(di, MT5788_RX_CMD_ADDR,
		MT5788_RX_CMD_VOUT_MASK, MT5788_RX_CMD_VOUT_SHIFT, MT5788_RX_CMD_VAL);
	if (ret) {
		hwlog_err("set_rx_vout: write cmd failed\n");
		return ret;
	}

	return 0;
}

static void mt5788_ask_mode_cfg(struct mt5788_dev_info *di, u8 cm, u8 polarity)
{
	int ret;
	u8 askcap;

	askcap = (cm & MT5788_RX_ASK_CAP_CM_MASK) |
		((polarity << MT5788_RX_ASK_CAP_POLARITY_SHIFT) & 0xff); /* 0xff: 8bit */
	hwlog_info("[ask_mode_cfg] cap=0x%x\n", askcap);
	ret = mt5788_write_byte(di, MT5788_RX_ASK_CAP_ADDR, askcap);
	if (ret)
		hwlog_err("ask_mode_cfg: failed\n");
}

static int mt5788_set_intfr_mode_cfg(struct mt5788_dev_info *di)
{
	int i;
	u8 cm_cfg, polar_cfg;

	if ((di->cm_intfr_cfg_level <= 0) || (di->sysfs_cm_type <= 0))
		return -EINVAL;

	for (i = 0; i < di->cm_intfr_cfg_level; i++) {
		if (di->sysfs_cm_type == (int)di->cm_intfr_cfg[i].type) {
			cm_cfg = di->cm_intfr_cfg[i].cm;
			polar_cfg = di->cm_intfr_cfg[i].polar;
			mt5788_ask_mode_cfg(di, cm_cfg, polar_cfg);
			return 0;
		}
	}

	return -EINVAL;
}

static void mt5788_set_mode_cfg(struct mt5788_dev_info *di, int vfc)
{
	u8 cm_cfg, polar_cfg;

	if (!mt5788_set_intfr_mode_cfg(di))
		return;

	if (vfc <= WLRX_IC_HIGH_VOUT1) {
		cm_cfg = di->rx_mod_cfg.cm.l_volt;
		polar_cfg = di->rx_mod_cfg.polar.l_volt;
	} else {
		if (!power_cmdline_is_factory_mode() &&
			(vfc >= WLRX_IC_HIGH_VOUT2)) {
			cm_cfg = di->rx_mod_cfg.cm.h_volt;
			polar_cfg = di->rx_mod_cfg.polar.h_volt;
		} else {
			cm_cfg = di->rx_mod_cfg.cm.fac_h_volt;
			polar_cfg = di->rx_mod_cfg.polar.fac_h_volt;
		}
	}
	mt5788_ask_mode_cfg(di, cm_cfg, polar_cfg);
}

static int mt5788_rx_enable_boost_mode(struct mt5788_dev_info *di)
{
	return mt5788_write_byte(di, MT5788_RX_SWITCH_BRIDGE_MODE_ADDR, MT5788_RX_HALF_BRIDGE);
}

static int mt5788_rx_disable_boost_mode(struct mt5788_dev_info *di)
{
	return mt5788_write_byte(di, MT5788_RX_SWITCH_BRIDGE_MODE_ADDR, MT5788_RX_FULL_BRIDGE);
}

static int mt5788_rx_set_iout_cali(struct mt5788_dev_info *di)
{
	int ret;

	if (!di->rx_icali)
		return 0;

	ret = mt5788_write_block(di, MT5788_RX_IOUT_CALI_ADDR, di->rx_icali_para,
		MT5788_RX_IOUT_CALI_LEN);
	ret += mt5788_write_dword_mask(di, MT5788_RX_CMD_ADDR,
		MT5788_RX_CMD_SET_CALI_PARAM_MASK, MT5788_RX_CMD_SET_CALI_PARAM_SHIFT,
		MT5788_RX_CMD_VAL);
	return ret;
}

static void mt5788_show_sr_status(struct mt5788_dev_info *di)
{
	u32 sr_status = 0;

	mt5788_read_dword_mask(di, MT5788_RX_SYS_MODE_ADDR, MT5788_RX_SYS_MODE_BRIDGE_MASK,
		MT5788_RX_SYS_MODE_BRIDGE_SHIFT, &sr_status);
	hwlog_info("[sr_status] boost mode %s\n",
		sr_status ? "disabled" : "enable"); /* 1: SR is in half-bridge mode */
}

static int mt5788_rx_set_vpa(struct mt5788_dev_info *di, u16 vpa)
{
	return mt5788_write_word(di, MT5788_RX_FC_VPA_ADDR, vpa);
}

static int mt5788_rx_get_vmldo_by_vfc_map(int vfc, void *dev_data)
{
	int i;
	struct mt5788_vfc_map const *map = NULL;
	struct mt5788_dev_info *di = dev_data;

	if (!di)
		return -ENODEV;

	if (di->is_scx_mode || !di->need_ctrl_brg)
		map = (struct mt5788_vfc_map *)rx_vfc_scx_map;
	else
		map = (struct mt5788_vfc_map *)rx_vfc_nonscx_map;
	for (i = 0; i < MT5788_VFC_MAP_SIZE; i++) {
		if (vfc == map[i].vpa) {
			hwlog_info("[get_vmldo] vmldo=%dmv\n", map[i].vmldo);
			return map[i].vmldo;
		}
	}

	hwlog_err("get_vmldo: mismatch\n");
	return -EINVAL;
}

static int mt5788_rx_set_vmldo(struct mt5788_dev_info *di, u16 vfc)
{
	int vmldo;

	vmldo = mt5788_rx_get_vmldo_by_vfc_map(vfc, di);
	if (vmldo < 0)
		return -EINVAL;

	return mt5788_write_word(di, MT5788_RX_FC_VMLDO_ADDR, vmldo);
}

static int mt5788_rx_send_fc_cmd(struct mt5788_dev_info *di, int vfc)
{
	int ret;

	if (!mt5788_rx_is_tx_exist(di))
		return -EPERM;

	if (vfc == di->last_vfc)
		return 0;

	ret = mt5788_rx_set_vpa(di, vfc);
	if (ret) {
		hwlog_err("send_fc_cmd: set fc_vpa failed\n");
		return ret;
	}

	ret = mt5788_write_dword_mask(di, MT5788_RX_SYS_MODE_ADDR,
		MT5788_RX_SYS_MODE_FC_SUCC_MASK, MT5788_RX_SYS_MODE_FC_SUCC_SHIFT,
		MT5788_RX_SYS_MODE_FC_SUCC_RESET);
	if (ret) {
		hwlog_err("send_fc_cmd: clear fc_status failed\n");
		return ret;
	}

	ret = mt5788_write_dword_mask(di, MT5788_RX_CMD_ADDR,
		MT5788_RX_CMD_SEND_FC_MASK, MT5788_RX_CMD_SEND_FC_SHIFT,
		MT5788_RX_CMD_VAL);
	if (ret) {
		hwlog_err("send_fc_cmd: send fc_cmd failed\n");
		return ret;
	}

	return 0;
}

static bool mt5788_rx_is_fc_succ(struct mt5788_dev_info *di, int vset)
{
	int i;
	int cnt;
	u32 rx_status = 0;

	if (!mt5788_rx_is_tx_exist(di))
		return false;

	if (vset == di->last_vfc)
		return true;

	cnt = MT5788_RX_FC_VOUT_TIMEOUT / MT5788_RX_FC_VOUT_SLEEP_TIME;
	for (i = 0; i < cnt; i++) {
		if ((di->g_val.rx_stop_chrg && (vset > WLRX_IC_DFLT_VOUT)) || !mt5788_rx_is_tx_exist(di))
			return false;
		power_msleep(MT5788_RX_FC_VOUT_SLEEP_TIME, 0, NULL); /* wait for vout stability */

		mt5788_read_dword_mask(di, MT5788_RX_SYS_MODE_ADDR, MT5788_RX_SYS_MODE_FC_SUCC_MASK,
			MT5788_RX_SYS_MODE_FC_SUCC_SHIFT, &rx_status);
		if (!rx_status) {
			hwlog_info("[is_fc_succ] succ, cost_time: %dms\n",
				(i + 1) * MT5788_RX_FC_VOUT_SLEEP_TIME);
			return true;
		}
	}

	hwlog_err("is_fc_succ: timeout\n");
	return false;
}

static bool mt5788_rx_is_vmldo_set_succ(struct mt5788_dev_info *di, int vfc)
{
	int i;
	int cnt;
	int vout = 0;
	int vmldo;

	vmldo = mt5788_rx_get_vmldo_by_vfc_map(vfc, di);
	if (vmldo < 0)
		return false;

	cnt = MT5788_RX_SET_VMLDO_SLEEP_TIMEOUT / MT5788_RX_SET_VMLDO_SLEEP_TIME;
	for (i = 0; i < cnt; i++) {
		if ((di->g_val.rx_stop_chrg && (vfc > WLRX_IC_DFLT_VOUT)) ||
			!mt5788_rx_is_tx_exist(di) || mt5788_is_sleep_enable(di))
			return false;
		power_msleep(MT5788_RX_SET_VMLDO_SLEEP_TIME, 0, NULL); /* wait for vout stability */
		(void)mt5788_rx_get_vout(&vout, di);
		if (vout >= (vmldo - 200)) { /* 200: maximum deviation */
			hwlog_info("[is_vmldo_set_succ] succ, cost_time: %dms\n",
				(i + 1) * MT5788_RX_SET_VMLDO_SLEEP_TIME);
			return true;
		}
	}

	hwlog_err("is_vmldo_set_succ: timeout\n");
	return false;
}

static int mt5788_rx_set_cm(int cm_type, void *dev_data)
{
	int vfc_reg = 0;
	struct mt5788_dev_info *di = dev_data;

	if (!di || (di->cm_intfr_cfg_level <= 0))
		return -EINVAL;

	hwlog_info("[rx_set_cm] cm_type: %d\n", cm_type);
	di->sysfs_cm_type = cm_type;
	(void)mt5788_rx_get_vfc_reg(&vfc_reg, di);
	mt5788_set_mode_cfg(di, vfc_reg);
	return 0;
}

static bool mt5788_rx_is_cp_open(struct mt5788_dev_info *di)
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
	(void)mt5788_rx_get_rx_vout_reg(&rx_vset, di);
	(void)mt5788_rx_get_vout(&rx_vout, di);
	cp_vout = charge_pump_get_cp_vout(CP_TYPE_MAIN);
	cp_vout = (cp_vout > 0) ? cp_vout : wldc_get_ls_vbus();

	hwlog_info("[is_cp_open] [rx] ratio:%d vset:%d vout:%d [cp] vout:%d\n",
		rx_ratio, rx_vset, rx_vout, cp_vout);
	if ((cp_vout * rx_ratio) < (rx_vout - MT5788_RX_FC_VOUT_ERR_LTH))
		return false;
	if ((cp_vout * rx_ratio) > (rx_vout + MT5788_RX_FC_VOUT_ERR_UTH))
		return false;

	return true;
}

static int mt5788_rx_check_cp_mode(struct mt5788_dev_info *di)
{
	int i;
	int cnt;
	int ret;

	ret = charge_pump_set_cp_mode(CP_TYPE_MAIN);
	if (ret) {
		hwlog_err("check_cp_mode: set cp_mode failed\n");
		return ret;
	}
	cnt = MT5788_RX_BPCP_TIMEOUT / MT5788_RX_BPCP_SLEEP_TIME;
	for (i = 0; i < cnt; i++) {
		msleep(MT5788_RX_BPCP_SLEEP_TIME);
		if (mt5788_rx_is_cp_open(di)) {
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

static int mt5788_rx_set_boost_mode(struct mt5788_dev_info *di, int vfc)
{
	if (!di->need_ctrl_brg)
		return 0;

	if (power_cmdline_is_factory_mode() && di->is_scx_mode)
		return mt5788_rx_enable_boost_mode(di);

	if (vfc >= WLRX_IC_HIGH_VOUT2)
		return mt5788_rx_disable_boost_mode(di);

	return mt5788_rx_enable_boost_mode(di);
}

static int mt5788_rx_set_vtx(int vfc, struct mt5788_dev_info *di)
{
	int i;
	int ret;

	ret = mt5788_rx_set_vmldo(di, vfc);
	if (ret) {
		hwlog_err("[set_vfc] send fc_vmldo failed\n");
		return ret;
	}

	(void)mt5788_rx_get_vfc_reg(&di->last_vfc, di);

	for (i = 0; i < MT5788_RX_FC_VPA_RETRY_CNT; i++) {
		if ((di->g_val.rx_stop_chrg && (vfc > WLRX_IC_DFLT_VOUT)) || !mt5788_rx_is_tx_exist(di))
			return -EPERM;

		ret = mt5788_rx_send_fc_cmd(di, vfc);
		if (ret) {
			hwlog_err("set_vfc: send fc_cmd failed\n");
			continue;
		}
		hwlog_info("[set_vfc] send fc_cmd sucess, cnt: %d\n", i);

		if (mt5788_rx_is_fc_succ(di, vfc)) {
			if (!mt5788_rx_is_vmldo_set_succ(di, vfc))
				continue;
			if ((vfc < RX_HIGH_VOUT2) && wldc_need_cp_div2()) {
				ret = mt5788_rx_check_cp_mode(di);
				msleep(100);
			} else if (vfc < RX_HIGH_VOUT2) {
				(void)charge_pump_set_bp_mode(CP_TYPE_MAIN);
			}
			if (ret)
				return ret;
			mt5788_set_mode_cfg(di, vfc);
			hwlog_info("[set_vfc] succ\n");
			di->last_vfc = vfc;
			return 0;
		}
	}

	return -EIO;
}

static int mt5788_rx_set_vfc(int vfc, void *dev_data)
{
	int ret;
	struct mt5788_dev_info *di = dev_data;

	if (!di)
		return -ENODEV;

	di->g_val.vfc_set_complete = false;
	hwlog_info("[set_vfc] vfc=%d\n", vfc);

	if ((vfc >= RX_HIGH_VOUT2) && wldc_need_cp_div2()) {
		ret = mt5788_rx_check_cp_mode(di);
		if (ret)
			goto out;
		msleep(100); /* wait for vout stability */
	}

	ret = mt5788_rx_set_boost_mode(di, vfc);
	if (ret)
		goto out;

	ret = mt5788_rx_set_vtx(vfc, di);
	if (ret)
		hwlog_err("set_vfc: failed\n");
out:
	di->g_val.vfc_set_complete = true;
	return ret;
}

static void mt5788_rx_set_scx_mode(bool scx_mode, void *dev_data)
{
	struct mt5788_dev_info *di = dev_data;

	if (!di)
		return;

	di->is_scx_mode = scx_mode;
}

static void mt5788_rx_send_ept(int ept_type, void *dev_data)
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
	ret = mt5788_write_byte(dev_data, MT5788_RX_EPT_MSG_ADDR, (u8)ept_type);
	ret += mt5788_write_dword_mask(dev_data, MT5788_RX_CMD_ADDR,
		MT5788_RX_CMD_SEND_EPT_MASK, MT5788_RX_CMD_SEND_EPT_SHIFT,
		MT5788_RX_CMD_VAL);
	if (ret)
		hwlog_err("send_ept: failed, ept=0x%x\n", ept_type);
}

bool mt5788_rx_is_tx_exist(void *dev_data)
{
	int ret;
	u32 mode = 0;

	ret = mt5788_get_mode(dev_data, &mode);
	if (ret) {
		hwlog_err("is_tx_exist: get mode failed\n");
		return false;
	}

	if (mode & MT5788_RX_SYS_MODE_RX)
		return true;

	return false;
}

static int mt5788_rx_kick_watchdog(void *dev_data)
{
	return mt5788_write_word(dev_data, MT5788_RX_WDT_ADDR, MT5788_RX_KICK_WDT_VAL);
}

static int mt5788_rx_get_fod(char *fod_str, int len, void *dev_data)
{
	int i;
	int ret;
	char tmp[MT5788_RX_FOD_TMP_STR_LEN] = { 0 };
	u8 fod_arr[MT5788_RX_FOD_LEN] = { 0 };

	if (!fod_str || (len < WLRX_IC_FOD_COEF_LEN))
		return -EINVAL;

	ret = mt5788_read_block(dev_data, MT5788_RX_FOD_ADDR, fod_arr, MT5788_RX_FOD_LEN);
	if (ret)
		return ret;

	for (i = 0; i < MT5788_RX_FOD_LEN; i++) {
		(void)snprintf_s(tmp, MT5788_RX_FOD_TMP_STR_LEN, MT5788_RX_FOD_TMP_STR_LEN,
			"%x ", fod_arr[i]);
		if (strncat_s(fod_str, len - 1, tmp, strlen(tmp)))
			return -EINVAL;
	}

	return strlen(fod_str);
}

static int mt5788_rx_set_fod(const char *fod_str, void *dev_data)
{
	int ret;
	char *cur = (char *)fod_str;
	char *token = NULL;
	int i;
	u8 val = 0;
	const char *sep = " ,";
	u8 fod_arr[MT5788_RX_FOD_LEN] = { 0 };

	if (!fod_str) {
		hwlog_err("set_fod: input fod_str err\n");
		return -EINVAL;
	}

	for (i = 0; i < MT5788_RX_FOD_LEN; i++) {
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

	ret = mt5788_write_block(dev_data, MT5788_RX_FOD_ADDR, fod_arr,
		MT5788_RX_FOD_LEN);
	ret += mt5788_write_dword_mask(dev_data, MT5788_RX_CMD_ADDR,
		MT5788_RX_CMD_VOUT_MASK, MT5788_RX_CMD_VOUT_SHIFT, MT5788_RX_CMD_VAL);
	return ret;
}

static int mt5788_rx_enable_ldo_opp_vbst(bool vbst_en, void *dev_data)
{
	int ret;
	struct mt5788_dev_info *di = dev_data;

	if (!di)
		return -ENODEV;

	if (vbst_en) {
		ret = mt5788_write_word(di, MT5788_RX_LDO_OPP_VBST_ADDR,
			di->ldo_opp_vbst.vbst);
		ret += mt5788_write_word(di, MT5788_RX_LDO_OPP_ITHL_ADDR,
			di->ldo_opp_vbst.iout_lth);
		ret += mt5788_write_word(di, MT5788_RX_LDO_OPP_ITHH_ADDR,
			di->ldo_opp_vbst.iout_hth);
	} else {
		ret = mt5788_write_word(di, MT5788_RX_LDO_OPP_VBST_ADDR, 0);
	}

	return ret;
}

static int mt5788_rx_5vbuck_chip_init(struct mt5788_dev_info *di)
{
	int ret;

	hwlog_info("[5vbuck_chip_init] chip init\n");
	ret = mt5788_write_block(di, MT5788_RX_LDO_CFG_ADDR,
		di->rx_ldo_cfg.l_volt, MT5788_RX_LDO_CFG_LEN);
	ret += mt5788_write_word(di, MT5788_RX_ILOAD_ADDR,
		di->dummy_iload[MT5788_DUMMY_ILOAD_5V_NO_MOD]);
	ret += mt5788_write_word(di, MT5788_RX_ILOAD_ASK_ADDR,
		di->dummy_iload[MT5788_DUMMY_ILOAD_5V_MOD]);
	return ret;
}

static int mt5788_rx_init_fod_coef(struct mt5788_dev_info *di, unsigned int tx_type)
{
	int vfc;
	int vrx;
	int ret;
	int vfc_reg = 0;
	u8 *rx_fod = NULL;

	(void)mt5788_rx_get_vfc_reg(&vfc_reg, di);
	hwlog_info("[init_fod_coef] vfc_reg: %dmV\n", vfc_reg);
	vrx = mt5788_rx_get_vmldo_by_vfc_map(vfc_reg, di);
	if (vrx < 9000) /* (0, 9)V, set 5v fod */
		vfc = 5000;
	else if (vrx < 15000) /* [9, 15)V, set 9V fod */
		vfc = 9000;
	else if (vrx < 18000) /* [15, 18)V, set 15V fod */
		vfc = 15000;
	else
		return -EINVAL;

	rx_fod = wlrx_get_fod_ploss_th(di->ic_type, vfc, tx_type, MT5788_RX_FOD_LEN);
	if (!rx_fod)
		return -EINVAL;

	ret = mt5788_write_block(di, MT5788_RX_FOD_ADDR, rx_fod, MT5788_RX_FOD_LEN);
	ret += mt5788_write_dword_mask(di, MT5788_RX_CMD_ADDR, MT5788_RX_CMD_FOD_UPDATE_MASK,
		MT5788_RX_CMD_FOD_UPDATE_SHIFT, MT5788_RX_CMD_VAL);
	return ret;
}

static int mt5788_rx_fodchk_chip_init(struct mt5788_dev_info *di)
{
	int ret;

	hwlog_info("fodchk_chip_init\n");
	ret = mt5788_write_word(di, MT5788_RX_ILOAD_ADDR,
		di->dummy_iload[MT5788_DUMMY_ILOAD_FODCHK_NO_MOD]);
	ret += mt5788_write_word(di, MT5788_RX_ILOAD_ASK_ADDR,
		di->dummy_iload[MT5788_DUMMY_ILOAD_FODCHK_MOD]);
	ret += mt5788_write_word(di, MT5788_RX_LDO_CFG_V2_ADDR, 0);
	ret += mt5788_write_word(di, MT5788_RX_LDO_CFG_V1_ADDR, 0);
	ret += mt5788_write_byte(di, MT5788_RX_ASK_CAP_ADDR, di->fodchk_ask_cap);
	ret += mt5788_write_dword(di, MT5788_RX_FOD_OFFSET0_ADDR, 0);
	ret += mt5788_write_dword_mask(di, MT5788_RX_CMD_ADDR, MT5788_RX_CMD_FOD_UPDATE_MASK,
		MT5788_RX_CMD_FOD_UPDATE_SHIFT, MT5788_RX_CMD_VAL);
	ret += mt5788_write_byte(di, MT5788_RX_RPP_INTV_ADDR, 1);
	if (ret) {
		hwlog_err("fodchk_chip_init: write fail\n");
		return -EIO;
	}

	return 0;
}

static int mt5788_rx_dflt_chip_init(struct mt5788_dev_info *di, unsigned int tx_type)
{
	int ret;

	hwlog_info("dflt_chip_init\n");
	di->g_val.rx_stop_chrg = false;
	ret = mt5788_write_word(di, MT5788_RX_WDT_TIMEOUT_ADDR, di->rx_wdt_timeout);
	ret += mt5788_write_word(di, MT5788_RX_VFC_DIFF_ADDR, di->rx_vfc_diff);
	ret += mt5788_rx_kick_watchdog(di);
	ret += mt5788_write_byte(di, MT5788_RX_RPP_INTV_ADDR, di->rpp_intv_cnt);
	ret += mt5788_write_word(di, MT5788_RX_FOD_OFFSET_ADDR,
		di->rx_fod_offset);
	if (ret) {
		hwlog_err("dflt_chip_init: write fail\n");
		return -EIO;
	}

	return 0;
}

static int mt5788_rx_chip_init(unsigned int init_type, unsigned int tx_type, void *dev_data)
{
	int ret = 0;
	struct mt5788_dev_info *di = dev_data;

	if (!di)
		return -ENODEV;

	(void)mt5788_write_word(di, MT5788_RX_OTP_ADDR,
		di->otp_th);
	(void)mt5788_write_byte(di, MT5788_RX_LDO_OPP_OFF_ADDR,
		MT5788_RX_LDO_OFF_OFF_EN);

	switch (init_type) {
	case WLRX_IC_DFLT_CHIP_INIT:
		ret += mt5788_rx_dflt_chip_init(di, tx_type);
		/* fall through */
	case WLRX_IC_5VBUCK_CHIP_INIT:
		ret += mt5788_rx_5vbuck_chip_init(di);
		ret += mt5788_rx_init_fod_coef(di, tx_type);
		break;
	case WLRX_IC_9VBUCK_CHIP_INIT:
		hwlog_info("[chip_init] 9v chip init\n");
		mt5788_write_block(di, MT5788_RX_LDO_CFG_ADDR,
			di->rx_ldo_cfg.m_volt, MT5788_RX_LDO_CFG_LEN);
		ret += mt5788_write_word(di, MT5788_RX_ILOAD_ADDR,
			di->dummy_iload[MT5788_DUMMY_ILOAD_9V_NO_MOD]);
		ret += mt5788_write_word(di, MT5788_RX_ILOAD_ASK_ADDR,
			di->dummy_iload[MT5788_DUMMY_ILOAD_9V_MOD]);
		ret += mt5788_rx_init_fod_coef(di, tx_type);
		break;
	case WLRX_IC_SC_CHIP_INIT:
		hwlog_info("[chip_init] sc chip init\n");
		mt5788_write_block(di, MT5788_RX_LDO_CFG_ADDR,
			di->rx_ldo_cfg.sc, MT5788_RX_LDO_CFG_LEN);
		ret += mt5788_write_word(di, MT5788_RX_ILOAD_ADDR,
			di->dummy_iload[MT5788_DUMMY_ILOAD_SC_NO_MOD]);
		ret += mt5788_write_word(di, MT5788_RX_ILOAD_ASK_ADDR,
			di->dummy_iload[MT5788_DUMMY_ILOAD_SC_MOD]);
		ret += mt5788_rx_init_fod_coef(di, tx_type);
		break;
	case WIRELESS_FODCHK_CHIP_INIT:
		ret += mt5788_rx_fodchk_chip_init(di);
		break;
	default:
		hwlog_err("chip_init: input para invalid\n");
		break;
	}

	return ret;
}

static void mt5788_rx_stop_charging(void *dev_data)
{
	struct mt5788_dev_info *di = dev_data;

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
		mt5788_enable_irq_wake(di);
		mt5788_enable_irq(di);
		hwlog_info("[stop_charging] wired channel on, enable irq\n");
	}
}

static int mt5788_rx_data_rcvd_handler(struct mt5788_dev_info *di)
{
	int ret;
	int i;
	u8 cmd;
	u8 buff[HWQI_PKT_LEN] = { 0 };

	ret = mt5788_read_block(di, MT5788_RX_FSK_HEADER_ADDR,
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
	case HWQI_CMD_ACK_BST_ERR:
	case HWQI_CMD_GET_TX_VERSION:
	case HWQI_CMD_GET_TX_SN:
		di->irq_val &= ~MT5788_RX_IRQ_FSK_PKT;
		if (di->g_val.qi_hdl &&
			di->g_val.qi_hdl->hdl_non_qi_fsk_pkt)
			di->g_val.qi_hdl->hdl_non_qi_fsk_pkt(buff, HWQI_PKT_LEN, di);
		break;
	default:
		break;
	}
	return 0;
}

void mt5788_rx_abnormal_irq_handler(struct mt5788_dev_info *di)
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
	mt5788_disable_irq_wake(di);
	mt5788_disable_irq_nosync(di);
	gpio_set_value(di->gpio_sleep_en, RX_SLEEP_EN_DISABLE);
	hwlog_err("handle_abnormal_irq: more than %d irq in %ds, disable irq\n",
		WIRELESS_INT_CNT_TH, WIRELESS_INT_TIMEOUT_TH / POWER_MS_PER_S);
}

static void mt5788_rx_ready_handler(struct mt5788_dev_info *di)
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

static void mt5788_rx_power_on_handler(struct mt5788_dev_info *di)
{
	u8 rx_ss = 0;
	int vrect = 0;
	int pwr_flag = WLRX_IC_PWR_ON_NOT_GOOD;

	if (wlrx_get_wired_channel_state() == WIRED_CHANNEL_ON) {
		hwlog_err("rx_power_on_handler: wired channel on, ignore\n");
		return;
	}

	(void)mt5788_rx_set_iout_cali(di);
	(void)mt5788_rx_5vbuck_chip_init(di);
	mt5788_rx_abnormal_irq_handler(di);
	mt5788_read_byte(di, MT5788_RX_SS_ADDR, &rx_ss);
	mt5788_rx_get_vrect(&vrect, di);
	hwlog_info("[rx_power_on_handler] ss=%u vrect=%d\n", rx_ss, vrect);
	if ((rx_ss > di->rx_ss_good_lth) && (rx_ss <= MT5788_RX_SS_MAX))
		pwr_flag = WLRX_IC_PWR_ON_GOOD;

	mt5788_rx_get_vfc_reg(&di->last_vfc, di);
	hwlog_info("[rx_power_on_handler] last_vfc=%d\n", di->last_vfc);
	mt5788_show_sr_status(di);
	power_event_bnc_notify(POWER_BNT_WLRX, POWER_NE_WLRX_PWR_ON, &pwr_flag);
}

static void mt5788_rx_mode_irq_recheck(struct mt5788_dev_info *di)
{
	int ret;
	u32 irq_val = 0;

	if (gpio_get_value(di->gpio_int))
		return;

	hwlog_info("[rx_mode_irq_recheck] gpio_int low, re-check irq\n");
	ret = mt5788_rx_get_interrupt(di, &irq_val);
	if (ret)
		return;

	if (irq_val & MT5788_RX_IRQ_READY)
		mt5788_rx_ready_handler(di);

	mt5788_rx_clear_irq(di, MT5788_RX_IRQ_CLR_ALL);
}

static void mt5788_rx_fault_irq_handler(struct mt5788_dev_info *di)
{
	if (di->irq_val & MT5788_RX_IRQ_OCP) {
		di->irq_val &= ~MT5788_RX_IRQ_OCP;
		power_event_bnc_notify(POWER_BNT_WLRX, POWER_NE_WLRX_OCP, NULL);
	}

	if (di->irq_val & MT5788_RX_IRQ_OVP) {
		di->irq_val &= ~MT5788_RX_IRQ_OVP;
		power_event_bnc_notify(POWER_BNT_WLRX, POWER_NE_WLRX_OVP, NULL);
	}

	if (di->irq_val & MT5788_RX_IRQ_OTP) {
		di->irq_val &= ~MT5788_RX_IRQ_OTP;
		power_event_bnc_notify(POWER_BNT_WLRX, POWER_NE_WLRX_OTP, NULL);
	}

	if (di->irq_val & MT5788_RX_IRQ_OPP_H) {
                di->irq_val &= ~MT5788_RX_IRQ_OPP_H;
                power_event_bnc_notify(POWER_BNT_WLRX, POWER_NE_WLRX_OPP_H, NULL);
        }
}

void mt5788_rx_mode_irq_handler(struct mt5788_dev_info *di)
{
	int ret;

	if (!di)
		return;

	ret = mt5788_rx_get_interrupt(di, &di->irq_val);
	hwlog_info("[mode_irq_handler] irq_val=0x%x\n", di->irq_val);
	if (ret) {
		hwlog_err("irq_handler: read irq failed, clear\n");
		mt5788_rx_clear_irq(di, MT5788_RX_IRQ_CLR_ALL);
		mt5788_rx_abnormal_irq_handler(di);
		goto rechk_irq;
	}

	mt5788_rx_clear_irq(di, di->irq_val);

	if (di->irq_val & MT5788_RX_IRQ_POWER_ON) {
		di->irq_val &= ~MT5788_RX_IRQ_POWER_ON;
		mt5788_rx_power_on_handler(di);
	}
	if (di->irq_val & MT5788_RX_IRQ_READY) {
		di->irq_val &= ~MT5788_RX_IRQ_READY;
		mt5788_rx_ready_handler(di);
	}
	if (di->irq_val & MT5788_RX_IRQ_FSK_PKT)
		mt5788_rx_data_rcvd_handler(di);

	mt5788_rx_fault_irq_handler(di);

rechk_irq:
	mt5788_rx_mode_irq_recheck(di);
}

static void mt5788_rx_pmic_vbus_handler(bool vbus_state, void *dev_data)
{
	int ret;
	u32 irq_val = 0;
	struct mt5788_dev_info *di = dev_data;

	if (!di || !vbus_state || !di->g_val.irq_abnormal)
		return;

	if (wlrx_get_wired_channel_state() == WIRED_CHANNEL_ON)
		return;

	if (!mt5788_rx_is_tx_exist(di))
		return;

	ret = mt5788_rx_get_interrupt(di, &irq_val);
	if (ret) {
		hwlog_err("pmic_vbus_handler: read irq failed\n");
		return;
	}
	hwlog_info("[pmic_vbus_handler] irq_val=0x%x\n", irq_val);
	if (irq_val & MT5788_RX_IRQ_READY) {
		mt5788_rx_clear_irq(di, MT5788_RX_IRQ_CLR_ALL);
		mt5788_rx_ready_handler(di);
		di->irq_cnt = 0;
		di->g_val.irq_abnormal = false;
		mt5788_enable_irq_wake(di);
		mt5788_enable_irq(di);
	}
}

void mt5788_rx_probe_check_tx_exist(struct mt5788_dev_info *di)
{
	if (!di)
		return;

	if (mt5788_rx_is_tx_exist(di)) {
		mt5788_rx_clear_irq(di, MT5788_RX_IRQ_CLR_ALL);
		hwlog_info("[rx_probe_check_tx_exist] rx exist\n");
		mt5788_rx_ready_handler(di);
	} else {
		mt5788_sleep_enable(true, di);
	}
}

void mt5788_rx_shutdown_handler(struct mt5788_dev_info *di)
{
	if (!di || !mt5788_rx_is_tx_exist(di))
		return;

	wlps_control(di->ic_type, WLPS_RX_EXT_PWR, false);
	power_msleep(DT_MSLEEP_50MS, 0, NULL); /* delay 50ms for power off */
	(void)mt5788_rx_set_vfc(WLRX_IC_DFLT_VOUT, di);
	(void)mt5788_rx_set_rx_vout(WLRX_IC_DFLT_VOUT, di);
	(void)mt5788_chip_enable(false, di);
	power_msleep(DT_MSLEEP_200MS, 0, NULL); /* delay 200ms for chip disable */
	(void)mt5788_chip_enable(true, di);
}

static struct wlrx_ic_ops g_mt5788_rx_ic_ops = {
	.get_dev_node           = mt5788_dts_dev_node,
	.fw_update              = mt5788_fw_sram_update,
	.chip_init              = mt5788_rx_chip_init,
	.chip_reset             = mt5788_rx_chip_reset,
	.chip_enable            = mt5788_chip_enable,
	.is_chip_enable         = mt5788_is_chip_enable,
	.sleep_enable           = mt5788_sleep_enable,
	.is_sleep_enable        = mt5788_is_sleep_enable,
	.ext_pwr_post_ctrl      = mt5788_rx_ext_pwr_post_ctrl,
	.get_chip_info          = mt5788_get_chip_info_str,
	.get_vrect              = mt5788_rx_get_vrect,
	.get_vout               = mt5788_rx_get_vout,
	.get_iout               = mt5788_rx_get_iout,
	.get_iavg               = mt5788_rx_get_iavg,
	.get_imax               = mt5788_rx_get_imax,
	.get_vout_reg           = mt5788_rx_get_rx_vout_reg,
	.get_vfc_reg            = mt5788_rx_get_vfc_reg,
	.get_ping_fop           = mt5788_rx_get_ping_fop,
	.get_ping_vrect         = mt5788_rx_get_ping_vrect,
	.set_cm                 = mt5788_rx_set_cm,
	.set_vfc                = mt5788_rx_set_vfc,
	.set_scx_mode           = mt5788_rx_set_scx_mode,
	.set_vout               = mt5788_rx_set_rx_vout,
	.get_vrx_by_vfc_map     = mt5788_rx_get_vmldo_by_vfc_map,
	.get_fop                = mt5788_rx_get_fop,
	.get_cep                = mt5788_rx_get_cep,
	.get_temp               = mt5788_rx_get_temp,
	.get_fod_coef           = mt5788_rx_get_fod,
	.set_fod_coef           = mt5788_rx_set_fod,
	.is_tx_exist            = mt5788_rx_is_tx_exist,
	.kick_watchdog          = mt5788_rx_kick_watchdog,
	.send_ept               = mt5788_rx_send_ept,
	.stop_charging          = mt5788_rx_stop_charging,
	.pmic_vbus_handler      = mt5788_rx_pmic_vbus_handler,
	.enable_ldo_opp_vbst    = mt5788_rx_enable_ldo_opp_vbst,
	.set_turbo_chg_flag     = mt5788_rx_set_turbo_chg_flag,
	.set_cur_pmode_name     = mt5788_rx_set_cur_pmode_name,
};

int mt5788_rx_ops_register(struct wltrx_ic_ops *ops, struct mt5788_dev_info *di)
{
	if (!ops || !di)
		return -ENODEV;

	ops->rx_ops = kzalloc(sizeof(*(ops->rx_ops)), GFP_KERNEL);
	if (!ops->rx_ops)
		return -ENODEV;

	(void)memcpy_s(ops->rx_ops, sizeof(g_mt5788_rx_ic_ops), &g_mt5788_rx_ic_ops, sizeof(g_mt5788_rx_ic_ops));
	ops->rx_ops->dev_data = (void *)di;
	di->g_val.vfc_set_complete = true;
	return wlrx_ic_ops_register(ops->rx_ops, di->ic_type);
}
