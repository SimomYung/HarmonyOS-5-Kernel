// SPDX-License-Identifier: GPL-2.0
/*
 * bk96285_dts.c
 *
 * bk96285 dts driver
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

#define HWLOG_TAG wireless_bk96285_dts
HWLOG_REGIST();

#define BK96285_CM_CFG_LEN                   3
#define BK96285_POLAR_CFG_LEN                3
#define BK96285_LDO_OPP_VBST_LEN             3

/* for cm_intfr_para */
#define WLRX_CM_INTFR_CFG_ROW                3
#define WLRX_CM_INTFR_CFG_COL                3

static int bk96285_parse_tx_fod_ploss(struct device_node *np, struct bk96285_dev_info *di)
{
	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG), np, "tx_ploss_th0",
		&di->tx_fod.ploss_th0, BK96285_TX_PLOSS_TH0_VAL);
	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG), np, "tx_ploss_th1",
		&di->tx_fod.ploss_th1, BK96285_TX_PLOSS_TH1_VAL);
	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG), np, "tx_ploss_th2",
		&di->tx_fod.ploss_th2, BK96285_TX_PLOSS_TH2_VAL);
	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG), np, "tx_ploss_th3",
		&di->tx_fod.ploss_th3, BK96285_TX_PLOSS_TH3_VAL);

	return 0;
}

static int bk96285_parse_tx_fod(struct device_node *np, struct bk96285_dev_info *di)
{
	int ret, len;

	len = power_dts_read_count_strings(power_dts_tag(HWLOG_TAG), np,
		"tx_fod_cond", WLTRX_FOD_CFG_ROW, WLTX_FOD_COND_END);
	if (len > 0) {
		di->tx_fod_fmt = WLTX_FOD_STATUS_FMT_NEW;
		ret = wltx_parse_fod_cfg(di->ic_type, np, BK96285_TX_FOD_LEN);
	} else {
		di->tx_fod_fmt = WLTX_FOD_STATUS_FMT_OLD;
		ret = bk96285_parse_tx_fod_ploss(np, di);
	}
	if (ret)
		return ret;

	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG), np, "tx_ploss_cnt",
		&di->tx_fod.ploss_cnt, BK96285_TX_PLOSS_CNT_VAL);
	return 0;
}

static int bk96285_parse_ldo_cfg(struct device_node *np, struct bk96285_dev_info *di)
{
	int len;

	len = of_property_count_u8_elems(np, "rx_ldo_cfg_5v");
	if ((len > BK96285_RX_LDO_CFG_LEN) || (len <= 0))
		return -EINVAL;
	if (power_dts_read_u8_array(power_dts_tag(HWLOG_TAG), np,
		"rx_ldo_cfg_5v", di->rx_ldo_cfg.l_volt, len))
		return -EINVAL;

	len = of_property_count_u8_elems(np, "rx_ldo_cfg_9v");
	if ((len > BK96285_RX_LDO_CFG_LEN) || (len <= 0))
		return -EINVAL;
	if (power_dts_read_u8_array(power_dts_tag(HWLOG_TAG), np,
		"rx_ldo_cfg_9v", di->rx_ldo_cfg.m_volt, len))
		return -EINVAL;

	len = of_property_count_u8_elems(np, "rx_ldo_cfg_sc");
	if ((len > BK96285_RX_LDO_CFG_LEN) || (len <= 0))
		return -EINVAL;
	if (power_dts_read_u8_array(power_dts_tag(HWLOG_TAG), np,
		"rx_ldo_cfg_sc", di->rx_ldo_cfg.sc, len))
		return -EINVAL;

	return 0;
}

static void bk96285_parse_cm_intfr_cfg(struct device_node *np, struct bk96285_dev_info *di)
{
	int i, len, size;

	len = power_dts_read_u8_count(power_dts_tag(HWLOG_TAG), np,
		"rx_cm_intfr_cfg", WLRX_CM_INTFR_CFG_ROW, WLRX_CM_INTFR_CFG_COL);
	if (len <= 0)
		return;

	size = sizeof(*di->cm_intfr_cfg) * (len / WLRX_CM_INTFR_CFG_COL);
	di->cm_intfr_cfg = kzalloc(size, GFP_KERNEL);
	if (!di->cm_intfr_cfg)
		return;

	if (power_dts_read_u8_array(power_dts_tag(HWLOG_TAG), np,
		"rx_cm_intfr_cfg", (u8 *)di->cm_intfr_cfg, len))
		return;

	di->cm_intfr_cfg_level = len / WLRX_CM_INTFR_CFG_COL;
	for (i = 0; i < di->cm_intfr_cfg_level; i++)
		hwlog_info("rx_cm_intfr[%d], type: %u cm: %u polar: %u\n",
			i, di->cm_intfr_cfg[i].type, di->cm_intfr_cfg[i].cm, di->cm_intfr_cfg[i].polar);
}

static void bk96285_parse_rx_ask_mod_cfg(struct device_node *np,
	struct bk96285_dev_info *di)
{
	if (power_dts_read_u8_array(power_dts_tag(HWLOG_TAG), np,
		"rx_mod_cm_cfg", (u8 *)&di->rx_mod_cfg.cm, BK96285_CM_CFG_LEN))
		return;

	if (power_dts_read_u8_array(power_dts_tag(HWLOG_TAG), np,
		"rx_mod_polar_cfg", (u8 *)&di->rx_mod_cfg.polar, BK96285_POLAR_CFG_LEN))
		return;

	di->rx_mod_cfg.ctrl_by_ap = true;
	hwlog_info("[parse_rx_ask_mod_cfg] [cm] l_volt=0x%x h_volt=0x%x fac_h_volt=0x%x\n",
		di->rx_mod_cfg.cm.l_volt, di->rx_mod_cfg.cm.h_volt,
		di->rx_mod_cfg.cm.fac_h_volt);
	hwlog_info("[parse_rx_ask_mod_cfg] [polar] l_volt=0x%x h_volt=0x%x fac_h_volt=0x%x\n",
		di->rx_mod_cfg.polar.l_volt, di->rx_mod_cfg.polar.h_volt,
		di->rx_mod_cfg.polar.fac_h_volt);
}

static void bk96285_parse_ldo_opp_vbst(struct device_node *np,
	struct bk96285_dev_info *di)
{
	if (power_dts_read_u32_array(power_dts_tag(HWLOG_TAG), np,
		"ldo_opp_vbst", (u32 *)&di->ldo_opp_vbst, BK96285_LDO_OPP_VBST_LEN)) {
		di->ldo_opp_vbst.vbst = BK96285_RX_LDO_OPP_VBST_DFLT;
		di->ldo_opp_vbst.iout_lth = BK96285_RX_LDO_OPP_ITHL_DFLT;
		di->ldo_opp_vbst.iout_hth = BK96285_RX_LDO_OPP_ITHH_DFLT;
	}

	hwlog_info("[parse_ldo_opp_vbst] vbst=%u iout_lth=%u iout_hth=%u\n",
		di->ldo_opp_vbst.vbst, di->ldo_opp_vbst.iout_lth, di->ldo_opp_vbst.iout_hth);
}

static void bk96285_parse_dummy_iload(const struct device_node *np,
	struct bk96285_dev_info *di)
{
	int len;

	di->dummy_len = 0;

	len = of_property_count_u32_elems(np, "dummy_iload_new");
	if ((len > BK96285_DUMMY_ILOAD_END) || (len <= 0)) {
		hwlog_err("parse_dummy_iload: len out range len =%d\n", len);
		return;
	}

	if (power_dts_read_u32_array(power_dts_tag(HWLOG_TAG), np,
		"dummy_iload_new", (u32 *)&di->dummy_iload, len)) {
		hwlog_err("parse_dummy_iload: power_dts_read_u32_array failed\n");
		return;
	}
	di->dummy_len = len;
	hwlog_info("[parse_dummy_iload] dummy_iload_new=<%u %u %u %u %u %u %u %u %u %u>\n",
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

static int bk96285_parse_basic_rx_para(struct device_node *np, struct bk96285_dev_info *di)
{
	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"rx_ss_good_lth", (u32 *)&di->rx_ss_good_lth, BK96285_RX_SS_MAX);
	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"mtp_check_delay_user", &di->mtp_check_delay.user,
		WIRELESS_FW_WORK_DELAYED_TIME);
	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"mtp_check_delay_fac", &di->mtp_check_delay.fac,
		WIRELESS_FW_WORK_DELAYED_TIME);
	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"gpio_en_valid_val", (u32 *)&di->gpio_en_valid_val,
		WLTRX_IC_EN_ENABLE);
	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"rx_vfc_diff", &di->rx_vfc_diff,
		BK96285_RX_FC_SUCC_THR_DEFAULT);
	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"rx_fod_offset", &di->rx_fod_offset,
		BK96285_RX_FOD_OFFSET_DEFAULT);
	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"fodchk_ask_cap", &di->fodchk_ask_cap, BK96285_RX_CMA_EN_VAL);
	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"need_ctrl_brg", (u32 *)&di->need_ctrl_brg,
		BK96285_RX_NEED_CTRL_BRG);
	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"prevfod_gain_unit", &di->prevfod_gain_unit,
		BK96285_RX_GAIN_UNIT_DEFAULT);
	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"rx_ldo_opp_en", &di->rx_ldo_opp_en, 0);
	return 0;
}

static int bk96285_parse_basic_tx_para(struct device_node *np, struct bk96285_dev_info *di)
{
	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"tx_ping_freq", &di->tx_ping_freq, BK96285_TX_PING_FREQ);
	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"tx_ping_duty", &di->tx_ping_duty, BK96285_TX_PING_DUTY);
	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"ping_ocp_th", (u32 *)&di->ping_ocp_th,
		BK96285_TX_PING_OCP_TH_DEFAULT);
	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"tx_pt_ocp_th", &di->tx_pt_ocp_th, BK96285_TX_PT_OCP_TH);
	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"duty_min", (u32 *)&di->duty_min, BK96285_TX_PT_DUTY_MIN_DEFAULT);
	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"duty_max", (u32 *)&di->duty_max, BK96285_TX_PT_DUTY_MAX_DEFAULT);
	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"tx_fodchk_duty_min", (u32 *)&di->tx_fodchk_duty_min, di->duty_min);
	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"tx_fodchk_duty_max", (u32 *)&di->tx_fodchk_duty_max, di->duty_max);
	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"full_bridge_ith", (u32 *)&di->full_bridge_ith,
		BK96285_TX_FULL_BRI_ITH);
	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"coil_test_ping_freq", &di->coil_test_ping_freq,
		BK96285_COIL_TEST_PING_FREQ);
	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"coil_test_ping_duty", &di->coil_test_ping_duty, BK96285_TX_PING_DUTY);
	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"rx_ss_low_thr", &di->rx_ss_low_thr, 0);
	return 0;
}

int bk96285_parse_dts(struct device_node *np, struct bk96285_dev_info *di)
{
	int ret;

	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"product_type", &di->product_type, BK96285_PRODUCT_TYPE_DFLT);

	ret = bk96285_parse_basic_rx_para(np, di);
	if (ret) {
		hwlog_err("parse_dts: parse rx basic para failed\n");
		return ret;
	}

	ret = bk96285_parse_basic_tx_para(np, di);
	if (ret) {
		hwlog_err("parse_dts: parse tx basic para failed\n");
		return ret;
	}

	ret = bk96285_parse_tx_fod(np, di);
	if (ret) {
		hwlog_err("parse_dts: parse tx_fod para failed\n");
		return ret;
	}

	ret = wlrx_parse_fod_cfg(di->ic_type, np, BK96285_RX_FOD_LEN);
	if (ret) {
		hwlog_err("parse_dts: parse rx_fod para failed\n");
		return ret;
	}

	ret = bk96285_parse_ldo_cfg(np, di);
	if (ret) {
		hwlog_err("parse_dts: parse ldo cfg failed\n");
		return ret;
	}

	bk96285_parse_ldo_opp_vbst(np, di);
	bk96285_parse_cm_intfr_cfg(np, di);
	bk96285_parse_rx_ask_mod_cfg(np, di);
	bk96285_parse_dummy_iload(np, di);
	return 0;
}
