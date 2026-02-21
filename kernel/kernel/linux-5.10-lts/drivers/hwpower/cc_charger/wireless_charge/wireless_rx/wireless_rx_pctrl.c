// SPDX-License-Identifier: GPL-2.0
/*
 * wireless_rx_pctrl.c
 *
 * power(vtx,vrx,irx) control of wireless charging
 *
 * Copyright (c) 2020-2021 Huawei Technologies Co., Ltd.
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

#include <chipset_common/hwpower/wireless_charge/wireless_rx_acc.h>
#include <chipset_common/hwpower/wireless_charge/wireless_rx_common.h>
#include <chipset_common/hwpower/wireless_charge/wireless_rx_pctrl.h>
#include <chipset_common/hwpower/wireless_charge/wireless_rx_module.h>
#include <chipset_common/hwpower/wireless_charge/wireless_rx_plim.h>
#include <chipset_common/hwpower/wireless_charge/wireless_rx_ic_intf.h>
#include <chipset_common/hwpower/wireless_charge/wireless_rx_interfere.h>
#include <chipset_common/hwpower/wireless_charge/wireless_rx_pmode.h>
#include <chipset_common/hwpower/common_module/power_algorithm.h>
#include <huawei_platform/hwpower/common_module/power_platform.h>
#include <chipset_common/hwpower/common_module/power_printk.h>
#include <chipset_common/hwpower/common_module/power_dts.h>
#include <chipset_common/hwpower/common_module/power_supply.h>
#include <huawei_platform/hwpower/common_module/power_platform_macro.h>
#include <linux/kernel.h>
#include <linux/of.h>

#define HWLOG_TAG wireless_rx_pctrl
HWLOG_REGIST();

/* for product_para */
#define WLRX_PCTRL_PRODUCT_CFG_LEN           3

/* for segment_para */
#define WLRX_PCTRL_SOC_CFG_ROW               5
#define WLRX_PCTRL_SOC_CFG_COL               5

/* for dcdisconplim */
#define WLRX_PCTRL_DISCONPLIM_ROW            10
#define WLRX_PCTRL_DISCONPLIM_COL            10
#define WLRX_PCTRL_DISCONPLIM_PREV_ACT       0
#define WLRX_PCTRL_DISCONPLIM_POST_ACT       1

/* for vbat_limit */
#define WLRX_PCTRL_VBAT_LIMIT_ROW            8
#define WLRX_PCTRL_VBAT_LIMIT_COL            3

#define WLRX_PCTRL_IRX_MIN_MA                150 /* mA */
#define WLRX_PCTRL_CM_CTRL_WORK_DELAY        30000 /* ms */

#define WLRX_PCTRL_DFLT_ILIM                 5500 /* mA */

struct wlrx_soc_para {
	int soc_min;
	int soc_max;
	int vtx;
	int vrx;
	int irx;
};

struct wlrx_dcdisconplim {
	int cnt;
	int tx_type;
	int adap_type;
	int rvs;
	int discon_delay;
	int vtx;
	int vrx;
	int irx;
	int prev_act;
	int post_act;
};

struct wlrx_pctrl_vbat_limit {
	u32 enable_vbatt;
	u32 disable_vbatt;
	u32 mode_type;
};

struct wlrx_pctrl_dts {
	struct wlrx_pctrl *product_pcfg;
	int soc_pcfg_level;
	struct wlrx_soc_para *soc_pcfg;
	int product_max_pwr;
	int rx_imin;
	int vbat_limit_level;
	struct wlrx_pctrl_vbat_limit *vbat_limit_cfg;
};

struct wlrx_dcdisconplim_para {
	struct wlrx_dcdisconplim *dts;
	int level;
	int cnt;
};

struct wlrx_pctrl_dev {
	unsigned int  drv_type;
	struct wlrx_pctrl_dts *dts;
	struct wlrx_dcdisconplim_para dcdisconplim;
	int rx_ilim;
	u8 cm_ctrl_src;
	struct wlrx_pctrl pctrl;
	struct delayed_work cm_ctrl_work;
};

static struct wlrx_cm_ctrl_para {
	u8 type; /* 1: audio record 2: camera video */
	u8 val; /* 0: exit 1: enter */
} g_cm_ctrl_map[] = {
	{0x1, 0x0}, {0x2, 0x0},
};

static struct wlrx_pctrl_dev *g_rx_pctrl_di[WLTRX_DRV_MAX];

static struct wlrx_pctrl_dev *wlrx_pctrl_get_di(unsigned int drv_type)
{
	if (!wltrx_is_drv_type_valid(drv_type)) {
		hwlog_err("get_di: drv_type=%u err\n", drv_type);
		return NULL;
	}

	return g_rx_pctrl_di[drv_type];
}

static unsigned int wlrx_pctrl_get_ic_type(unsigned int drv_type)
{
	return wltrx_get_dflt_ic_type(drv_type);
}

int wlrx_pctrl_get_para(unsigned int drv_type, unsigned int para_type)
{
	struct wlrx_pctrl_dev *di = wlrx_pctrl_get_di(drv_type);

	if (!di)
		return -ENODEV;

	switch (para_type) {
	case WLRX_PCTRL_VTX:
		return di->pctrl.vtx;
	case WLRX_PCTRL_VRX:
		return di->pctrl.vrx;
	case WLRX_PCTRL_IRX:
		return di->pctrl.irx;
	case WLRX_PCTRL_IRX_LIM:
		return di->rx_ilim;
	default:
		return -EINVAL;
	}
}

static void __wlrx_pctrl_set_para(struct wlrx_pctrl_dev *di, unsigned int para_type, int val)
{
	switch (para_type) {
	case WLRX_PCTRL_INIT:
		di->pctrl.vtx = TX_DEFAULT_VOUT;
		di->pctrl.vrx = RX_DEFAULT_VOUT;
		di->pctrl.irx = di->dts->rx_imin;
		di->rx_ilim = WLRX_PCTRL_DFLT_ILIM;
		break;
	case WLRX_PCTRL_VTX:
		di->pctrl.vtx = power_min_positive(di->pctrl.vtx, val);
		break;
	case WLRX_PCTRL_VRX:
		di->pctrl.vrx = power_min_positive(di->pctrl.vrx, val);
		break;
	case WLRX_PCTRL_IRX:
		di->pctrl.irx = power_min_positive(di->pctrl.irx, val);
		break;
	case WLRX_PCTRL_IRX_MIN:
		di->pctrl.irx = di->dts->rx_imin;
		break;
	default:
		break;
	}
}

void wlrx_pctrl_set_para(unsigned int drv_type, unsigned int para_type, int val)
{
	struct wlrx_pctrl_dev *di = wlrx_pctrl_get_di(drv_type);

	if (!di)
		return;

	__wlrx_pctrl_set_para(di, para_type, val);
}

struct wlrx_pctrl *wlrx_pctrl_get_product_cfg(unsigned int drv_type)
{
	struct wlrx_pctrl_dev *di = wlrx_pctrl_get_di(drv_type);

	if (!di || !di->dts)
		return NULL;

	return di->dts->product_pcfg;
}

u8 wlrx_pctrl_get_cm_ctrl_src(unsigned int drv_type)
{
	struct wlrx_pctrl_dev *di = wlrx_pctrl_get_di(drv_type);

	return  !di ? 0 : di->cm_ctrl_src;
}

static void wlrx_pctrl_cm_ctrl_work(struct work_struct *work)
{
	if (!work)
		return;

	wlrx_plim_clear_src(WLTRX_DRV_MAIN, WLRX_PLIM_SRC_CM_INTFR);
	wlrx_ic_set_cm(WLTRX_IC_MAIN, 0); /* type: 0, set cm */
}

void wlrx_pctrl_handle_cm_ctrl(unsigned int drv_type, u8 type, u8 val)
{
	int i;
	u8 curr_src;
	enum wlrx_scene scn_id = wlrx_get_hall_only_scene();
	struct wlrx_pctrl_dev *di = wlrx_pctrl_get_di(drv_type);

	if (!di || (scn_id < WLRX_SCN_BEGIN) || (scn_id >= WLRX_SCN_END))
		return;

	curr_src = di->cm_ctrl_src;
	for (i = 0; i < ARRAY_SIZE(g_cm_ctrl_map); i++) {
		if (type == g_cm_ctrl_map[i].type)
			g_cm_ctrl_map[i].val = val;
	}

	for (i = 0; i < ARRAY_SIZE(g_cm_ctrl_map); i++) {
		if (g_cm_ctrl_map[i].val > 0)
			curr_src |= BIT(i);
		else
			curr_src &= ~BIT(i);
	}

	if (curr_src == di->cm_ctrl_src)
		return;

	di->cm_ctrl_src = curr_src;
	hwlog_info("[handle_cm_ctrl] curr_src=0x%x\n", di->cm_ctrl_src);
	cancel_delayed_work_sync(&di->cm_ctrl_work);
	if ((curr_src > 0) && (scn_id == WLRX_SCN_HALL_ONLY)) {
		wlrx_plim_set_src(WLTRX_DRV_MAIN, WLRX_PLIM_SRC_CM_INTFR);
		wlrx_ic_set_cm(WLTRX_IC_MAIN, 1); /* type: 1, set cm */
		return;
	}

	schedule_delayed_work(&di->cm_ctrl_work,
		msecs_to_jiffies(WLRX_PCTRL_CM_CTRL_WORK_DELAY));
}

static void wlrx_pctrl_update_soc_para(struct wlrx_pctrl_dev *di)
{
	int i, soc;

	soc = power_platform_get_battery_ui_capacity();
	for (i = 0; i < di->dts->soc_pcfg_level; i++) {
		if ((soc < di->dts->soc_pcfg[i].soc_min) ||
			(soc > di->dts->soc_pcfg[i].soc_max))
			continue;
		__wlrx_pctrl_set_para(di, WLRX_PCTRL_VTX, di->dts->soc_pcfg[i].vtx);
		__wlrx_pctrl_set_para(di, WLRX_PCTRL_VRX, di->dts->soc_pcfg[i].vrx);
		__wlrx_pctrl_set_para(di, WLRX_PCTRL_IRX, di->dts->soc_pcfg[i].irx);
		break;
	}
}

static int wlrx_pctrl_enable_ldo_opp_vbst(struct wlrx_pctrl_dev *di)
{
	return wlrx_ic_enable_ldo_opp_vbst(true, wlrx_pctrl_get_ic_type(di->drv_type));
}

static struct {
	int act;
	bool executed;
	const char *name;
	int (*handler)(struct wlrx_pctrl_dev *);
} g_dcdisconplim_act[] = {
	{1, false, "ldo_opp_vbst", wlrx_pctrl_enable_ldo_opp_vbst},
};

void wlrx_pctrl_count_dcdiscon(unsigned int drv_type, bool count_flag)
{
	int i;
	struct wlrx_pctrl_dev *di = wlrx_pctrl_get_di(drv_type);

	if (!di)
		return;

	if (!count_flag)
		di->dcdisconplim.cnt = 0;
	else
		di->dcdisconplim.cnt++;
	for (i = 0; i < ARRAY_SIZE(g_dcdisconplim_act); i++)
		g_dcdisconplim_act[i].executed = false;
	hwlog_info("[count_dcdiscon] cnt=%d\n", di->dcdisconplim.cnt);
}

static void wlrx_pctrl_dcdisconplim_act(struct wlrx_pctrl_dev *di, int act_id, int act_type)
{
	int i, ret, act;

	if ((act_id < 0) || (act_id >= di->dcdisconplim.level))
		return;

	if (act_type == WLRX_PCTRL_DISCONPLIM_PREV_ACT)
		act = di->dcdisconplim.dts[act_id].prev_act;
	else if (act_type == WLRX_PCTRL_DISCONPLIM_POST_ACT)
		act = di->dcdisconplim.dts[act_id].post_act;
	else
		return;

	for (i = 0; i < ARRAY_SIZE(g_dcdisconplim_act); i++) {
		if (act == g_dcdisconplim_act[i].act) {
			if (g_dcdisconplim_act[i].executed)
				return;
			ret = g_dcdisconplim_act[i].handler(di);
			if (!ret) {
				g_dcdisconplim_act[i].executed = true;
				hwlog_info("[dcdisconplim_act] act=%s succ\n",
					g_dcdisconplim_act[i].name);
				return;
			}
			hwlog_err("dcdisconplim_act: act=%s failed",
				g_dcdisconplim_act[i].name);
			return;
		}
	}
}

static void wlrx_pctrl_update_dcdisconplim_para(struct wlrx_pctrl_dev *di, struct wlrx_pctrl *p_new)
{
	int i;
	int prev_act_id = -1;
	int post_act_id = -1;
	int discon_delay = 0;
	int tx_type = wlrx_acc_get_tx_type(di->drv_type);
	struct wlrx_dcdisconplim *para;
	struct wlprot_acc_cap *cap = wlrx_acc_get_cap(di->drv_type);

	if (!cap || (di->dcdisconplim.level <= 0))
		return;

	for (i = 0; i < di->dcdisconplim.level; i++) {
		para = &di->dcdisconplim.dts[i];
		if (!para || (para->cnt < 0))
			continue;
		if ((para->adap_type > 0) && (cap->adp_type != para->adap_type))
			continue;
		if ((para->tx_type > 0) && (tx_type != para->tx_type))
			continue;
		if (di->dcdisconplim.cnt < para->cnt) {
			prev_act_id = i;
			discon_delay = para->discon_delay;
		} else {
			post_act_id = i;
			p_new->vtx = power_min_positive(para->vtx, p_new->vtx);
			p_new->vrx = power_min_positive(para->vrx, p_new->vrx);
			p_new->irx = power_min_positive(para->irx, p_new->irx);
			break;
		}
	}
	wlrx_set_discon_delay(WLTRX_DRV_MAIN, discon_delay);
	wlrx_pctrl_dcdisconplim_act(di, prev_act_id, WLRX_PCTRL_DISCONPLIM_PREV_ACT);
	wlrx_pctrl_dcdisconplim_act(di, post_act_id, WLRX_PCTRL_DISCONPLIM_POST_ACT);
}

static void wlrx_pctrl_update_product_para(struct wlrx_pctrl_dev *di)
{
	di->pctrl.vtx = di->dts->product_pcfg->vtx;
	di->pctrl.vrx = di->dts->product_pcfg->vrx;
	di->pctrl.irx = di->dts->product_pcfg->irx;
}

static void wlrx_pctrl_update_plim_para(struct wlrx_pctrl_dev *di, struct wlrx_pctrl *p_new)
{
	wlrx_plim_update_pctrl(di->drv_type, p_new);
}

static void wlrx_pctrl_update_trx_plim_para(struct wlrx_pctrl_dev *di, struct wlrx_pctrl *p_new)
{
	wlrx_acc_update_trx_plim_para(di->drv_type, p_new);
}

static void wlrx_pctrl_update_para_new(struct wlrx_pctrl_dev *di)
{
	struct wlrx_pctrl pctrl_new = { 0 };

	wlrx_pctrl_update_plim_para(di, &pctrl_new);
	wlrx_pctrl_update_dcdisconplim_para(di, &pctrl_new);
	wlrx_pctrl_update_trx_plim_para(di, &pctrl_new);
	/* add new case here, named as wlrx_pctrl_update_xxx_para */
	di->rx_ilim = pctrl_new.irx;
	__wlrx_pctrl_set_para(di, WLRX_PCTRL_VTX, pctrl_new.vtx);
	__wlrx_pctrl_set_para(di, WLRX_PCTRL_VRX, pctrl_new.vrx);
	__wlrx_pctrl_set_para(di, WLRX_PCTRL_IRX, pctrl_new.irx);
}

static void wlrx_pctrl_vbat_limit_para(struct wlrx_pctrl_dev *di)
{
	int i;
	int vbat = 0;

	if (!di->dts->vbat_limit_cfg || (di->dts->vbat_limit_level == 0))
		return;

	power_supply_get_int_prop(POWER_PLATFORM_BAT_PSY_NAME, POWER_SUPPLY_PROP_VOLTAGE_NOW, &vbat,
		POWER_SUPPLY_DEFAULT_VOLTAGE_NOW, POWER_UV_PER_MV);
	for (i = 0; i < di->dts->vbat_limit_level; i++) {
		if (vbat >= di->dts->vbat_limit_cfg[i].enable_vbatt)
			wlrx_pmode_set_mode_type(di->drv_type, di->dts->vbat_limit_cfg[i].mode_type, true);
		else if (vbat <= di->dts->vbat_limit_cfg[i].disable_vbatt)
			wlrx_pmode_set_mode_type(di->drv_type, di->dts->vbat_limit_cfg[i].mode_type, false);
	}
}

void wlrx_pctrl_update_para(unsigned int drv_type)
{
	struct wlrx_pctrl_dev *di = wlrx_pctrl_get_di(drv_type);

	if (!di)
		return;

	wlrx_pctrl_update_product_para(di);
	wlrx_pctrl_update_soc_para(di);
	wlrx_intfr_update_pctrl(drv_type, &di->pctrl);
	wlrx_pctrl_update_para_new(di);
	wlrx_pctrl_vbat_limit_para(di);
}

int wlrx_pctrl_get_product_pmax(unsigned int drv_type)
{
	struct wlrx_pctrl_dev *di = wlrx_pctrl_get_di(drv_type);

	if (!di || !di->dts)
		return 0;

	return di->dts->product_max_pwr;
}

int wlrx_pctrl_get_fix_fop(unsigned int drv_type)
{
	int fop;

	fop = wlrx_intfr_get_fixed_fop(WLTRX_DRV_MAIN);
	if (fop > 0)
		goto out;

	fop = wlrx_acc_get_tx_fixed_fop(WLTRX_DRV_MAIN);
out:
	return fop;
}

static void wlrx_pctrl_parse_soc_cfg(const struct device_node *np, struct wlrx_pctrl_dts *dts)
{
	int i, len;

	len = power_dts_read_u32_count(power_dts_tag(HWLOG_TAG), np,
		"segment_para", WLRX_PCTRL_SOC_CFG_ROW, WLRX_PCTRL_SOC_CFG_COL);
	if (len <= 0)
		return;

	dts->soc_pcfg = kcalloc(len / WLRX_PCTRL_SOC_CFG_COL, sizeof(*dts->soc_pcfg), GFP_KERNEL);
	if (!dts->soc_pcfg)
		return;

	if (power_dts_read_u32_array(power_dts_tag(HWLOG_TAG), np,
		"segment_para", (u32 *)dts->soc_pcfg, len)) {
		kfree(dts->soc_pcfg);
		return;
	}

	dts->soc_pcfg_level = len / WLRX_PCTRL_SOC_CFG_COL;
	for (i = 0; i < dts->soc_pcfg_level; i++)
		hwlog_info("[soc_pcfg][%d] soc_min:%-3d soc_max:%-3d vtx:%-5d vrx:%-5d irx:%-4d\n",
			i, dts->soc_pcfg[i].soc_min, dts->soc_pcfg[i].soc_max,
			dts->soc_pcfg[i].vtx, dts->soc_pcfg[i].vrx, dts->soc_pcfg[i].irx);
}

static void wlrx_pctrl_parse_dcdisconplim(const struct device_node *np, struct wlrx_pctrl_dev *di)
{
	int i, len, size;
	struct wlrx_dcdisconplim *dts;

	len = power_dts_read_count_strings(power_dts_tag(HWLOG_TAG), np,
		"dcdisconplim", WLRX_PCTRL_DISCONPLIM_ROW, WLRX_PCTRL_DISCONPLIM_COL);
	if (len <= 0)
		return;

	size = len / WLRX_PCTRL_DISCONPLIM_COL;
	di->dcdisconplim.dts = kcalloc(size, sizeof(*di->dcdisconplim.dts), GFP_KERNEL);
	if (!di->dcdisconplim.dts)
		return;

	len = power_dts_read_string_array(power_dts_tag(HWLOG_TAG), np,
		"dcdisconplim", (int *)di->dcdisconplim.dts, size, WLRX_PCTRL_DISCONPLIM_COL);
	if (len <= 0) {
		kfree(di->dcdisconplim.dts);
		return;
	}

	di->dcdisconplim.level = size;
	for (i = 0; i < di->dcdisconplim.level; i++) {
		dts = &di->dcdisconplim.dts[i];
		hwlog_info("[dcdisconplim][%d] cnt:%d tx_type:%-2d adap_type:%-2d\t"
			"vtx:%-5d vrx:%-5d irx:%-4d prev_act:%d post_act:%d\n",
			i, dts->cnt, dts->tx_type, dts->adap_type,
			dts->vtx, dts->vrx, dts->irx, dts->prev_act, dts->post_act);
	}
}

static int wlrx_pctrl_parse_product_cfg(const struct device_node *np, struct wlrx_pctrl_dts *dts)
{
	dts->product_pcfg = kzalloc(sizeof(*dts->product_pcfg), GFP_KERNEL);
	if (!dts->product_pcfg)
		return -ENOMEM;

	if (power_dts_read_u32_array(power_dts_tag(HWLOG_TAG), np,
		"product_para", (u32 *)dts->product_pcfg, WLRX_PCTRL_PRODUCT_CFG_LEN))
		return -EINVAL;

	hwlog_info("[product_pcfg] vtx_max:%dmV vrx_max:%dmV irx_max:%dmA\n",
		dts->product_pcfg->vtx, dts->product_pcfg->vrx, dts->product_pcfg->irx);
	return 0;
}

static void wlrx_pctrl_parse_vbat_limit_cfg(const struct device_node *np, struct wlrx_pctrl_dts *dts)
{
	int i, len, row;

	len = power_dts_read_u32_count(power_dts_tag(HWLOG_TAG), np,
		"vbat_limit_para", WLRX_PCTRL_VBAT_LIMIT_ROW, WLRX_PCTRL_VBAT_LIMIT_COL);
	if (len <= 0)
		return;

	row = len / WLRX_PCTRL_VBAT_LIMIT_COL;
	dts->vbat_limit_cfg = kzalloc(sizeof(*dts->vbat_limit_cfg) * row, GFP_KERNEL);
	if (!dts->vbat_limit_cfg)
		return;

	if (power_dts_read_u32_array(power_dts_tag(HWLOG_TAG), np,
		"vbat_limit_para", (u32 *)dts->vbat_limit_cfg, len)) {
		kfree(dts->vbat_limit_cfg);
		return;
	}

	dts->vbat_limit_level = row;
	for (i = 0; i < dts->vbat_limit_level; i++)
		hwlog_info("vbat_limit_para[%d] mode_type:%-4d enable_vbatt:%-4d disable_vbatt:%-4d\n",
			i, dts->vbat_limit_cfg->mode_type, dts->vbat_limit_cfg->enable_vbatt,
			dts->vbat_limit_cfg->disable_vbatt);
}

static int wlrx_pctrl_parse_dts(const struct device_node *np, struct wlrx_pctrl_dev *di)
{
	di->dts = kzalloc(sizeof(*di->dts), GFP_KERNEL);
	if (!di->dts)
		return -ENOMEM;

	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"rx_iout_min", (u32 *)&di->dts->rx_imin, WLRX_PCTRL_IRX_MIN_MA);
	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"product_max_pwr", (u32 *)&di->dts->product_max_pwr, 0);
	wlrx_pctrl_parse_soc_cfg(np, di->dts);
	wlrx_pctrl_parse_dcdisconplim(np, di);
	wlrx_pctrl_parse_vbat_limit_cfg(np, di->dts);

	return wlrx_pctrl_parse_product_cfg(np, di->dts);
}

static void wlrx_pctrl_kfree_dev(struct wlrx_pctrl_dev *di)
{
	if (!di)
		return;

	if (di->dts) {
		kfree(di->dts->product_pcfg);
		kfree(di->dts->soc_pcfg);
		if (di->dts->vbat_limit_cfg)
			kfree(di->dts->vbat_limit_cfg);
		kfree(di->dts);
	}

	kfree(di);
}

static int wlrx_pctrl_submodule_init(unsigned int drv_type, struct device *dev)
{
	return wlrx_plim_init(drv_type, dev) || wlrx_intfr_init(drv_type, dev);
}

static void wlrx_pctrl_submodule_deinit(unsigned int drv_type)
{
	wlrx_plim_deinit(drv_type);
	wlrx_intfr_deinit(drv_type);
}

int wlrx_pctrl_init(unsigned int drv_type, struct device *dev)
{
	int ret;
	struct wlrx_pctrl_dev *di = NULL;

	if (!dev || !wltrx_is_drv_type_valid(drv_type))
		return -ENODEV;

	di = kzalloc(sizeof(*di), GFP_KERNEL);
	if (!di)
		return -ENOMEM;

	ret = wlrx_pctrl_parse_dts(dev->of_node, di);
	if (ret)
		goto exit;
	ret = wlrx_pctrl_submodule_init(drv_type, dev);
	if (ret)
		goto exit;

	INIT_DELAYED_WORK(&di->cm_ctrl_work, wlrx_pctrl_cm_ctrl_work);
	di->drv_type = drv_type;
	g_rx_pctrl_di[drv_type] = di;
	return 0;

exit:
	wlrx_pctrl_kfree_dev(di);
	return ret;
}

void wlrx_pctrl_deinit(unsigned int drv_type)
{
	if (!wltrx_is_drv_type_valid(drv_type))
		return;

	wlrx_pctrl_submodule_deinit(drv_type);
	wlrx_pctrl_kfree_dev(g_rx_pctrl_di[drv_type]);
	g_rx_pctrl_di[drv_type] = NULL;
}
