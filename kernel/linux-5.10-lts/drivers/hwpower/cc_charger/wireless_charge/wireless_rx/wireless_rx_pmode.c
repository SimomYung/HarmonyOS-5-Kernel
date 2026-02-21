// SPDX-License-Identifier: GPL-2.0
/*
 * wireless_rx_pmode.c
 *
 * power mode for wireless charging
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

#include <chipset_common/hwpower/battery/battery_temp.h>
#include <chipset_common/hwpower/common_module/power_algorithm.h>
#include <chipset_common/hwpower/common_module/power_cmdline.h>
#include <chipset_common/hwpower/common_module/power_delay.h>
#include <chipset_common/hwpower/common_module/power_dts.h>
#include <chipset_common/hwpower/common_module/power_printk.h>
#include <chipset_common/hwpower/hardware_channel/power_sw.h>
#include <chipset_common/hwpower/hardware_ic/charge_pump.h>
#include <chipset_common/hwpower/wireless_charge/wireless_dc_check.h>
#include <chipset_common/hwpower/wireless_charge/wireless_dc_common.h>
#include <chipset_common/hwpower/wireless_charge/wireless_rx_acc.h>
#include <chipset_common/hwpower/wireless_charge/wireless_rx_common.h>
#include <chipset_common/hwpower/wireless_charge/wireless_rx_event.h>
#include <chipset_common/hwpower/wireless_charge/wireless_rx_fod.h>
#include <chipset_common/hwpower/wireless_charge/wireless_rx_ic_intf.h>
#include <chipset_common/hwpower/wireless_charge/wireless_rx_module.h>
#include <chipset_common/hwpower/wireless_charge/wireless_rx_pctrl.h>
#include <chipset_common/hwpower/wireless_charge/wireless_rx_pmode.h>
#include <chipset_common/hwpower/wireless_charge/wireless_rx_status.h>
#include <chipset_common/hwpower/wireless_charge/wireless_rx_sysfs.h>
#include <chipset_common/hwpower/wireless_charge/wireless_rx_ui.h>
#include <chipset_common/hwpower/wireless_charge/wireless_test_wp.h>
#include <huawei_platform/hwpower/wireless_charge/wireless_rx_platform.h>

#define HWLOG_TAG wireless_rx_pmode
HWLOG_REGIST();

/* for vmode_para */
#define WLRX_VMODE_CFG_ROW             5
#define WLRX_VMODE_CFG_COL             2

#define WLRX_VREG_OFFSET               50
#define WLRX_BST_ERR_CNT_MAX           5
#define WLRX_RX_BST_DELAY              3000 /* 3s */

struct wlrx_pmode_dts {
	int pcfg_level;
	int vmode_cfg_level;
	struct wlrx_pmode *pcfg;
	struct wlrx_vmode *vmode_cfg;
	u8 *pcfg_mode_type;
};

struct wlrx_pmode_dev {
	unsigned int drv_type;
	int curr_pid;
	int curr_vid;
	int bst_err_cnt;
	bool bst_rst_completed;
	bool dc_icon_disabled;
	unsigned long pmode_timeout;
	unsigned long pmode_icon; /* check icon type by pmode_icon */
	struct wlrx_pmode_dts *dts;
	unsigned long dis_mode_type;
};

static struct wlrx_pmode_dev *g_rx_pmode_di[WLTRX_DRV_MAX];

static void wlrx_pmode_notify_charger_vout(struct wlrx_pmode_dev *di);

static unsigned int wlrx_pmode_get_ic_type(unsigned int drv_type)
{
	return wltrx_get_dflt_ic_type(drv_type);
}

static struct wlrx_pmode_dev *wlrx_pmode_get_di(unsigned int drv_type)
{
	if (!wltrx_is_drv_type_valid(drv_type)) {
		hwlog_err("get_di: drv_type=%u err\n", drv_type);
		return NULL;
	}

	return g_rx_pmode_di[drv_type];
}

static bool wlrx_pmode_is_pid_valid(struct wlrx_pmode_dev *di, int pid)
{
	return (pid >= 0) && (pid < di->dts->pcfg_level);
}

static int wlrx_pmode_get_int_cfg(struct wlrx_pmode_dev *di, int pid, int cfg_type)
{
	switch (cfg_type) {
	case WLRX_PMODE_VRX:
		return di->dts->pcfg[pid].vrx;
	case WLRX_PMODE_IRX:
		return di->dts->pcfg[pid].irx;
	case WLRX_PMODE_VRECT_TH:
		return di->dts->pcfg[pid].vrect_lth;
	case WLRX_PMODE_EXP_MODE:
		return di->dts->pcfg[pid].expect_mode;
	case WLRX_PMODE_VTX:
		return di->dts->pcfg[pid].vtx;
	default:
		hwlog_err("get_int_cfg: invalid input=%d\n", cfg_type);
		return -EINVAL;
	}
}

int wlrx_pmode_get_curr_int_cfg(unsigned int drv_type, int cfg_type)
{
	struct wlrx_pmode_dev *di = wlrx_pmode_get_di(drv_type);

	if (!di)
		return -ENODEV;

	return wlrx_pmode_get_int_cfg(di, di->curr_pid, cfg_type);
}

int wlrx_pmode_get_support_mode(unsigned int drv_type)
{
	return wlrx_sysfs_get_support_mode(drv_type) &
		wlrx_fod_get_support_mode(drv_type);
}

static int wlrx_pmode_select_vid(struct wlrx_pmode_dev *di, int vtx)
{
	int id;

	for (id = 0; id < di->dts->vmode_cfg_level; id++) {
		if (vtx == di->dts->vmode_cfg[id].vtx)
			break;
	}

	if (id >= di->dts->vmode_cfg_level) {
		id = 0;
		hwlog_err("select_vid: match vmode_index failed\n");
	}

	return id;
}

static void wlrx_pmode_set_vid(struct wlrx_pmode_dev *di, int vtx)
{
	di->curr_vid = wlrx_pmode_select_vid(di, vtx);
	hwlog_info("[set_vid] id=%d\n", di->curr_vid);
}

static int __wlrx_pmode_set_vtx(struct wlrx_pmode_dev *di, int vtx)
{
	int ret;

	if (wlrx_get_wireless_channel_state() == WIRELESS_CHANNEL_OFF) {
		hwlog_err("set_vtx: not in wireless charging\n");
		return -EPERM;
	}
	if ((vtx > TX_DEFAULT_VOUT) && (wlrx_get_wired_channel_state() == WIRED_CHANNEL_ON)) {
		hwlog_err("set_vtx: wired vbus connect, vtx should be set to %dmV at most\n",
			TX_DEFAULT_VOUT);
		return -EPERM;
	}
	if (wlrx_in_pwroff_reset(di->drv_type) && (vtx > TX_DEFAULT_VOUT)) {
		hwlog_err("set_vtx: in pwroff_reset, vtx should be set to %dmV at most\n",
			 TX_DEFAULT_VOUT);
		return -EPERM;
	}

	hwlog_info("[set_vtx] vtx is set to %dmV\n", vtx);
	ret = wlrx_ic_set_vfc(wlrx_pmode_get_ic_type(di->drv_type), vtx);
	if (!ret)
		wlrx_pmode_set_vid(di, vtx);

	return ret;
}

static int __wlrx_pmode_set_vrx(struct wlrx_pmode_dev *di, int vrx)
{
	if ((wlrx_get_wireless_channel_state() == WIRELESS_CHANNEL_OFF) &&
		(vrx > RX_DEFAULT_VOUT)) {
		hwlog_err("set_vrx: not in wireless charging\n");
		return -EPERM;
	}
	if (wlrx_in_pwroff_reset(di->drv_type) && (vrx > RX_DEFAULT_VOUT)) {
		hwlog_err("set_vrx: in pwroff_reset, vrx should be set to %dmV at most\n",
			RX_DEFAULT_VOUT);
		return -EPERM;
	}

	hwlog_info("[set_vrx] vrx is set to %dmV\n", vrx);
	return wlrx_ic_set_vout(wlrx_pmode_get_ic_type(di->drv_type), vrx);
}

int wlrx_pmode_get_curr_pid(unsigned int drv_type)
{
	struct wlrx_pmode_dev *di = wlrx_pmode_get_di(drv_type);

	if (!di)
		return 0;

	return di->curr_pid;
}

static void __wlrx_pmode_set_curr_pid(struct wlrx_pmode_dev *di, int pid)
{
	if (!wlrx_pmode_is_pid_valid(di, pid)) {
		hwlog_err("set_curr_pid: pid=%d out of range\n", pid);
		return;
	}

	di->curr_pid = pid;
	wlrx_ic_set_cur_pmode_name(wlrx_pmode_get_ic_type(di->drv_type),
		di->dts->pcfg[pid].name);
	hwlog_info("[set_curr_pid] pid=%d\n", pid);
}

void wlrx_pmode_set_curr_pid(unsigned int drv_type, int pid)
{
	struct wlrx_pmode_dev *di = wlrx_pmode_get_di(drv_type);

	if (!di || !wlrx_pmode_is_pid_valid(di, pid))
		return;

	__wlrx_pmode_set_curr_pid(di, pid);
}

int wlrx_pmode_get_pid_by_name(unsigned int drv_type, const char *mode_name)
{
	int pid;
	struct wlrx_pmode_dev *di = wlrx_pmode_get_di(drv_type);

	if (!mode_name || !di)
		return 0;

	for (pid = 0; pid < di->dts->pcfg_level; pid++) {
		if (power_strsame(mode_name, di->dts->pcfg[pid].name))
			return pid;
	}

	return 0;
}

static bool wlrx_pmode_is_dc_mode(struct wlrx_pmode_dev *di, int pid)
{
	unsigned int dc_mode = wldc_get_mode_by_name(di->dts->pcfg[pid].name);

	return (dc_mode >= WLDC_MODE_BEGIN) && (dc_mode < WLDC_MODE_END);
}

bool wlrx_pmode_in_dc_mode(unsigned int drv_type)
{
	struct wlrx_pmode_dev *di = wlrx_pmode_get_di(drv_type);

	if (!di)
		return false;

	return wlrx_pmode_is_dc_mode(di, di->curr_pid);
}

void wlrx_pmode_set_mode_type(unsigned int drv_type, unsigned long mode_type, bool enable)
{
	unsigned long last_dis_mode_type;
	struct wlrx_pmode_dev *di = wlrx_pmode_get_di(drv_type);

	if (!di)
		return;

	last_dis_mode_type = di->dis_mode_type;
	if (enable)
		di->dis_mode_type &= ~mode_type;
	else
		di->dis_mode_type |= mode_type;

	if (di->dis_mode_type != last_dis_mode_type)
		hwlog_info("[wlrx_pmode_set_mode_type] dis_mode_type=0x%x", di->dis_mode_type);
}

static bool wlrx_pmode_quick_judge(struct wlrx_pmode_dev *di, int pid)
{
	struct wlrx_pmode *pcfg = NULL;
	struct wlprot_acc_cap *acc_cap = wlrx_acc_get_cap(di->drv_type);
	struct wlrx_pctrl *product_pcfg = wlrx_pctrl_get_product_cfg(di->drv_type);

	if (!acc_cap || !product_pcfg)
		return false;

	pcfg = &di->dts->pcfg[pid];
	if ((acc_cap->vmax < pcfg->vtx_min) || (product_pcfg->vtx < pcfg->vtx) ||
		(product_pcfg->vrx < pcfg->vrx) || (product_pcfg->irx < pcfg->irx))
		return false;
	if (acc_cap->vmax * acc_cap->imax < pcfg->vtx_min * pcfg->itx_min)
		return false;

	if (strstr(pcfg->name, WLRX_RVS_MODE_POSTFIX) && !wlrx_is_rvs_tx(di->drv_type))
		return false;

	if (strstr(pcfg->name, WLRX_12V_MODE_POSTFIX) && !wlrx_is_low_vpa_car_tx(di->drv_type))
		return false;

	if (strstr(pcfg->name, WLRX_15V_MODE_POSTFIX) && wlrx_is_low_vpa_car_tx(di->drv_type))
		return false;

	if (strstr(pcfg->name, WLRX_SC_MODE_PREFIX) && wlrx_is_sc_err_tx(di->drv_type))
		return false;

	return true;
}

static bool wlrx_pmode_normal_judge(struct wlrx_pmode_dev *di, int pid)
{
	if ((di->dts->pcfg[pid].auth >= 0) && !wlrx_acc_auth_succ(di->drv_type))
		return false;

	return true;
}

static bool wlrx_pmode_final_judge(struct wlrx_pmode_dev *di, int pid)
{
	int tbatt = 0;
	struct wlrx_pmode *pcfg = NULL;
	struct wlprot_acc_cap *acc_cap = wlrx_acc_get_cap(di->drv_type);

	if (!acc_cap)
		return false;

	if (di->dts->pcfg_mode_type && test_bit(di->dts->pcfg_mode_type[pid], &di->dis_mode_type))
		return false;

	pcfg = &di->dts->pcfg[pid];
	if ((wlrx_pctrl_get_para(di->drv_type, WLRX_PCTRL_VTX) < pcfg->vtx) ||
		(wlrx_pctrl_get_para(di->drv_type, WLRX_PCTRL_VRX) < pcfg->vrx))
		return false;

	if ((pid == di->curr_pid) && (wlrx_get_charge_stage() != WLRX_STAGE_CHARGING)) {
		if ((pcfg->timeout > 0) && time_after(jiffies, di->pmode_timeout))
			return false;
	}
	/* if acc not support 12V, 12V pmode will judge fail */
	if (!acc_cap->support_12v && (pcfg->vtx == WLTRX_VOLT_12V))
		return false;

	bat_temp_get_temperature(BAT_TEMP_MIXED, &tbatt);
	if ((pcfg->tbatt >= 0) && (tbatt >= pcfg->tbatt))
		return false;

	return true;
}

static bool __wlrx_pmode_judge(struct wlrx_pmode_dev *di, int pid, unsigned int judge_type)
{
	switch (judge_type) {
	case WLDC_PMODE_FINAL_JUDGE:
	case WLRX_PMODE_FINAL_JUDGE:
		if (!wlrx_pmode_final_judge(di, pid))
			return false;
		/* fall-through */
	case WLRX_PMODE_NORMAL_JUDGE:
		if (!wlrx_pmode_normal_judge(di, pid))
			return false;
		/* fall-through */
	case WLRX_PMODE_QUICK_JUDGE:
		if (!wlrx_pmode_quick_judge(di, pid))
			return false;
		break;
	default:
		hwlog_err("judge: type=%u err\n", judge_type);
		return false;
	}

	if ((judge_type == WLRX_PMODE_FINAL_JUDGE) &&
		wlrx_pmode_is_dc_mode(di, pid)) {
		if (!wlrx_pmode_dc_judge_crit(di->dts->pcfg[pid].name, pid))
			return false;
	}

	return true;
}

bool wlrx_pmode_judge(unsigned int drv_type, int pid, unsigned int judge_type)
{
	struct wlrx_pmode_dev *di = wlrx_pmode_get_di(drv_type);

	if (!di || !wlrx_pmode_is_pid_valid(di, pid))
		return false;

	return __wlrx_pmode_judge(di, pid, judge_type);
}

static int wlrx_pmode_get_opt_pid(struct wlrx_pmode_dev *di, unsigned int judge_type)
{
	int pid = di->dts->pcfg_level - 1;

	for (; pid >= 0; pid--) {
		if (__wlrx_pmode_judge(di, pid, judge_type))
			break;
	}
	if (pid < 0)
		pid = 0;

	return pid;
}

bool wlrx_pmode_need_auth(unsigned int drv_type)
{
	int pid;
	struct wlrx_pmode_dev *di = wlrx_pmode_get_di(drv_type);
	struct wlprot_acc_cap *cap = wlrx_acc_get_cap(drv_type);

	if (!di || !cap || cap->no_need_cert)
		return false;

	pid = wlrx_pmode_get_opt_pid(di, WLRX_PMODE_QUICK_JUDGE);
	hwlog_info("[need_auth] max_pid=%d\n", pid);

	return di->dts->pcfg[pid].auth > 0;
}

int wlrx_pmode_get_supported_max_vrx(unsigned int drv_type)
{
	int pid;
	struct wlrx_pmode_dev *di = wlrx_pmode_get_di(drv_type);

	if (!di)
		return WLTRX_VOLT_5V;

	pid = wlrx_pmode_get_opt_pid(di, WLRX_PMODE_QUICK_JUDGE);
	hwlog_info("[get_supported_max_vrx] support_mode=0x%x rx_vmax=%dmV\n",
		wlrx_pmode_get_support_mode(drv_type), di->dts->pcfg[pid].vrx);
	return di->dts->pcfg[pid].vrx;
}

int wlrx_pmode_set_vtx(unsigned int drv_type, int vtx)
{
	struct wlrx_pmode_dev *di = wlrx_pmode_get_di(drv_type);

	if (!di)
		return -ENODEV;

	return __wlrx_pmode_set_vtx(di, vtx);
}

int wlrx_pmode_set_vrx(unsigned int drv_type, int vrx)
{
	struct wlrx_pmode_dev *di = wlrx_pmode_get_di(drv_type);

	if (!di)
		return -ENODEV;

	return __wlrx_pmode_set_vrx(di, vrx);
}

static void wlrx_pmode_revise_vout(struct wlrx_pmode_dev *di)
{
	int vfc_reg = 0;
	int vout_reg = 0;
	struct wlrx_pmode *pcfg = NULL;

	if ((wlrx_get_charge_stage() == WLRX_STAGE_REGULATION_DC) ||
		(wlrx_get_wireless_channel_state() == WIRELESS_CHANNEL_OFF) ||
		wlrx_ic_is_sleep_enable(wlrx_pmode_get_ic_type(di->drv_type)))
		return;

	(void)wlrx_ic_get_vfc_reg(wlrx_pmode_get_ic_type(di->drv_type), &vfc_reg);
	(void)wlrx_ic_get_vout_reg(wlrx_pmode_get_ic_type(di->drv_type), &vout_reg);

	pcfg = &di->dts->pcfg[di->curr_pid];
	if ((vfc_reg <= pcfg->vtx - WLRX_VREG_OFFSET) ||
		(vfc_reg >= pcfg->vtx + WLRX_VREG_OFFSET)) {
		hwlog_err("revise_vout: revise vtx\n");
		if (__wlrx_pmode_set_vtx(di, pcfg->vtx))
			hwlog_err("revise_vout: set vtx failed\n");
		else
			wlrx_pmode_notify_charger_vout(di);
	}

	if ((vout_reg <= pcfg->vrx - WLRX_VREG_OFFSET) ||
		(vout_reg >= pcfg->vrx + WLRX_VREG_OFFSET)) {
		hwlog_err("revise_vout: revise vrx\n");
		if (__wlrx_pmode_set_vrx(di, pcfg->vrx))
			hwlog_err("revise_vout: set vrx failed\n");
	}
}

static void wlrx_pmode_reset_icon_flag(struct wlrx_pmode_dev *di)
{
	int pid;

	for (pid = 0; pid < di->dts->pcfg_level; pid++) {
		if (di->dc_icon_disabled && wlrx_pmode_is_dc_mode(di, pid))
			continue;
		set_bit(pid, &di->pmode_icon);
	}
	hwlog_info("[reset_icon_flag] pmode_icon=0x%x", di->pmode_icon);
}

static void __wlrx_pmode_clear_icon_flag(struct wlrx_pmode_dev *di, int pid)
{
	if (test_bit(pid, &di->pmode_icon)) {
		clear_bit(pid, &di->pmode_icon);
		hwlog_info("[clear_icon_flag] pmode_icon=0x%x", di->pmode_icon);
	}
}

void wlrx_pmode_clear_icon_flag(unsigned int drv_type, int pid)
{
	struct wlrx_pmode_dev *di = wlrx_pmode_get_di(drv_type);

	if (!di || !wlrx_pmode_is_pid_valid(di, pid))
		return;

	__wlrx_pmode_clear_icon_flag(di, pid);
}

static void wlrx_pmode_revise_icon_type(struct wlrx_pmode_dev *di, unsigned int *icon_type)
{
	if (*icon_type != WLRX_UI_SUPER_CHARGE)
		return;

	if (wlrx_is_highpwr_rvs_tx(di->drv_type))
		*icon_type = WLRX_UI_FAST_CHARGE;
}

void wlrx_pmode_icon_display(unsigned int drv_type, int judge_type)
{
	int pid;
	unsigned int icon_type;
	struct wlrx_pmode_dev *di = wlrx_pmode_get_di(drv_type);

	if (!di)
		return;

	for (pid = di->dts->pcfg_level - 1; pid >= 0; pid--) {
		if (test_bit(pid, &di->pmode_icon) &&
			__wlrx_pmode_judge(di, pid, judge_type))
			break;
	}
	if (pid < 0) {
		pid = 0;
		hwlog_err("icon_display: mismatched\n");
	}

	icon_type = di->dts->pcfg[pid].icon;
	wlrx_pmode_revise_icon_type(di, &icon_type);
	if (wlrx_get_wireless_channel_state() == WIRELESS_CHANNEL_OFF) {
		hwlog_err("icon_display: not in wireless charging, ignore\n");
		return;
	}
	wlrx_ui_send_charge_uevent(drv_type, icon_type);
}

void wlrx_pmode_disable_dc_icon(unsigned int drv_type, bool flag)
{
	int pid;
	struct wlrx_pmode_dev *di = wlrx_pmode_get_di(drv_type);

	if (!di)
		return;

	if (!flag) {
		di->dc_icon_disabled = false;
		return;
	}

	for (pid = di->dts->pcfg_level - 1; pid >= 0; pid--) {
		if (wlrx_pmode_is_dc_mode(di, pid))
			__wlrx_pmode_clear_icon_flag(di, pid);
	}
	di->dc_icon_disabled = true;
	wlrx_pmode_icon_display(di->drv_type, WLRX_PMODE_NORMAL_JUDGE);
}

static void wlrx_pmode_report_bst_fail_dmd(struct wlrx_pmode_dev *di)
{
	static bool dsm_report_flag = false;
	char dsm_buff[POWER_DSM_BUF_SIZE_0512] = { 0 };

	if (++di->bst_err_cnt < WLRX_BST_ERR_CNT_MAX) {
		dsm_report_flag = false;
		return;
	}

	di->bst_err_cnt = WLRX_BST_ERR_CNT_MAX;
	if (dsm_report_flag)
		return;

	/* dmd caused by misalignment not reported */
	if (!wlrx_evt_is_pwr_good(di->drv_type)) {
		hwlog_err("report_bst_fail_dmd: misaligned\n");
		return;
	}
	wlrx_dsm_report(di->drv_type, POWER_DSM_ERROR_WIRELESS_BOOSTING_FAIL,
		dsm_buff, sizeof(dsm_buff));
	dsm_report_flag = true;
}

static void wlrx_pmode_set_buck_ovp(struct wlrx_pmode_dev *di, int vtx, bool before_vfc)
{
	int vout = vtx;

	if (!before_vfc)
		vout = wlrx_ic_get_vrx_by_vfc_map(wlrx_pmode_get_ic_type(di->drv_type), vtx);
	if (vout >= RX_HIGH_VOUT2)
		power_sw_set_output_by_label(POWER_SW_CHARGER_OVP, POWER_SW_OFF, true);
	else if (vout > 0)
		power_sw_set_output_by_label(POWER_SW_CHARGER_OVP, POWER_SW_ON, false);
}

static void wlrx_pmode_notify_charger_vout(struct wlrx_pmode_dev *di)
{
	int vtx;
	int vrx;
	int cp_vout;
	int cp_ratio;

	vtx = di->dts->vmode_cfg[di->curr_vid].vtx;
	vrx = wlrx_ic_get_vrx_by_vfc_map(wlrx_pmode_get_ic_type(di->drv_type), vtx);
	cp_ratio = charge_pump_get_cp_ratio(CP_TYPE_MAIN);
	if ((cp_ratio <= 0) || (vrx <= 0)) {
		hwlog_err("notify_charger_vout: cp_ratio/vrx err\n");
		return;
	}
	hwlog_info("[notify_charger_vout] cp_ratio=%d\n", cp_ratio);
	cp_vout = vrx / cp_ratio;
	power_event_bnc_notify(POWER_BNT_WLC, POWER_NE_WLC_CHARGER_VBUS, &cp_vout);
}

bool wlrx_pmode_bst_rst_completed(unsigned int drv_type)
{
	struct wlrx_pmode_dev *di = wlrx_pmode_get_di(drv_type);

	if (!di)
		return false;

	return di->bst_rst_completed;
}

static int wlrx_pmode_boost_vout(struct wlrx_pmode_dev *di, int cur_vid, int target_vid)
{	
	int ret;
	int vid;
	int vtx;
	int bst_delay = WLRX_RX_BST_DELAY;
	int vfc_reg = 0;

	if (di->bst_err_cnt >= WLRX_BST_ERR_CNT_MAX)
		return -EPERM;

	wlrx_set_iout_min();
	(void)power_msleep(WLRX_ILIM_DELAY, DT_MSLEEP_25MS, wlrx_msleep_exit);
	di->bst_rst_completed = false;
	wlrx_ic_get_bst_delay_time(wlrx_pmode_get_ic_type(di->drv_type), &bst_delay);

	for (vid = cur_vid + 1; vid <= target_vid; vid++) {
		(void)wlrx_ic_get_vfc_reg(wlrx_pmode_get_ic_type(di->drv_type), &vfc_reg);
		vtx = di->dts->vmode_cfg[vid].vtx;
		wlrx_pmode_set_buck_ovp(di, vtx, true);
		ret = __wlrx_pmode_set_vtx(di, vtx);
		if (ret) {
			hwlog_err("boost_vout: boost failed\n");
			wlrx_pmode_report_bst_fail_dmd(di);
			(void)__wlrx_pmode_set_vtx(di, vfc_reg);
			di->bst_rst_completed = true;
			return ret;
		}
		wlrx_pmode_notify_charger_vout(di);
		wlrx_set_iout_min();
		if (vid == target_vid)
			continue;
		(void)power_msleep(bst_delay, DT_MSLEEP_25MS, wlrx_msleep_exit);
		wlrx_pmode_set_buck_ovp(di, vtx, false);
	}

	di->bst_rst_completed = true;
	di->bst_err_cnt = 0;
	power_event_bnc_notify(POWER_BNT_WLC, POWER_NE_WLC_TX_VSET, &vtx);

	return 0;
}

static int wlrx_pmode_reset_vout(struct wlrx_pmode_dev *di, int cur_vid, int target_vid)
{
	int ret;
	int vid;
	int vtx;
	int last_vfc_reg = 0;

	wlrx_set_iout_min();
	(void)power_msleep(WLRX_ILIM_DELAY, DT_MSLEEP_25MS, wlrx_msleep_exit);
	di->bst_rst_completed = false;

	for (vid = cur_vid - 1; vid >= target_vid; vid--) {
		(void)wlrx_ic_get_vfc_reg(wlrx_pmode_get_ic_type(di->drv_type), &last_vfc_reg);
		vtx = di->dts->vmode_cfg[vid].vtx;
		wlrx_wait_fop_fix_to_default(vtx);
		ret = __wlrx_pmode_set_vtx(di, vtx);
		if (ret) {
			hwlog_err("reset_vout: reset failed\n");
			(void)__wlrx_pmode_set_vtx(di, last_vfc_reg);
			di->bst_rst_completed = true;
			return ret;
		}
		wlrx_pmode_set_buck_ovp(di, vtx, false);
		wlrx_pmode_notify_charger_vout(di);
		wlrx_set_iout_min();
	}

	di->bst_rst_completed = true;
	return 0;
}

static int wlrx_pmode_set_vout(struct wlrx_pmode_dev *di, int cur_vid, int target_vid)
{
	int ret;
	int vtx;

	if (target_vid > cur_vid) {
		ret = wlrx_pmode_boost_vout(di, cur_vid, target_vid);
	} else if (target_vid < cur_vid) {
		ret = wlrx_pmode_reset_vout(di, cur_vid, target_vid);
	} else {
		vtx = di->dts->vmode_cfg[target_vid].vtx;
		ret = __wlrx_pmode_set_vtx(di, vtx);
		if (ret) {
			hwlog_err("set_vout: set vtx failed\n");
			return ret;
		}
		return __wlrx_pmode_set_vrx(di, vtx);
	}

	if (wlrx_get_wired_channel_state() == WIRED_CHANNEL_ON) {
		hwlog_err("set_vout: wired vbus connect\n");
		return -EPERM;
	}
	if (wlrx_get_wireless_channel_state() == WIRELESS_CHANNEL_OFF) {
		hwlog_err("set_vout: wireless vbus disconnect\n");
		return -EPERM;
	}
	if (di->curr_vid == cur_vid)
		return ret;

	vtx = di->dts->vmode_cfg[di->curr_vid].vtx;
	wlrx_chip_init(di->drv_type, vtx);
	wlrx_pmode_notify_charger_vout(di);

	return ret;
}

int wlrx_pmode_set_vtrx(unsigned int drv_type, int vtx, int vrx)
{
	int ret;
	int curr_vid;
	int target_vid;
	int vfc_reg = 0;
	struct wlrx_pmode_dev *di = wlrx_pmode_get_di(drv_type);
	
	if (!di)
		return -ENODEV;

	if (wlrx_ic_is_sleep_enable(wlrx_pmode_get_ic_type(di->drv_type))) {
		hwlog_info("set_vtrx: sleep_en eanble, ignore\n");
		return -ENXIO;
	}

	(void)wlrx_ic_get_vfc_reg(wlrx_pmode_get_ic_type(di->drv_type), &vfc_reg);
	curr_vid = wlrx_pmode_select_vid(di, vfc_reg);
	target_vid = wlrx_pmode_select_vid(di, vtx);

	ret = wlrx_pmode_set_vout(di, curr_vid, target_vid);
	if (ret) {
		hwlog_err("set_vtrx: set vtrx failed\n");
		return ret;
	}

	return __wlrx_pmode_set_vrx(di, vrx);
}

static int wlrx_pmode_vout_ctrl(struct wlrx_pmode_dev *di, int target_pid)
{
	int ret;
	int vtx;
	int target_vout;
	int target_vid;
	int vfc_reg = 0;
	int last_vid;

	if (wlrx_pmode_is_dc_mode(di, target_pid))
		return 0;
	if (wlrx_get_wireless_channel_state() != WIRELESS_CHANNEL_ON)
		return -EPERM;
	if (wlrx_ic_is_sleep_enable(wlrx_pmode_get_ic_type(di->drv_type))) {
		hwlog_info("vout_ctrl: sleep_en eanble, ignore\n");
		return -ENXIO;
	}

	last_vid = di->curr_vid;
	(void)wlrx_ic_get_vfc_reg(wlrx_pmode_get_ic_type(di->drv_type), &vfc_reg);
	vtx = di->dts->vmode_cfg[di->curr_vid].vtx;
	if (vfc_reg != vtx) {
		hwlog_err("vout_ctrl: vfc_reg %dmV != cur_mode_vtx %dmV\n", vfc_reg, vtx);
		ret = __wlrx_pmode_set_vtx(di, vtx);
		if (ret)
			hwlog_err("vout_ctrl: set vtx failed\n");
	}

	target_vout = di->dts->pcfg[target_pid].vtx;
	target_vid = wlrx_pmode_select_vid(di, target_vout);
	wlrx_pctrl_set_para(di->drv_type, WLRX_PCTRL_VTX, di->dts->pcfg[target_pid].vtx);
	wlrx_pctrl_set_para(di->drv_type, WLRX_PCTRL_VRX, di->dts->pcfg[target_pid].vrx);
	ret = wlrx_pmode_set_vout(di, di->curr_vid, target_vid);
	if (ret)
		return ret;
	if (last_vid != di->curr_vid)
		return 0;
	vtx = di->dts->vmode_cfg[di->curr_vid].vtx;
	wlrx_chip_init(di->drv_type, vtx);
	wlrx_pmode_notify_charger_vout(di);

	return 0;
}

static int wlrx_pmode_set(struct wlrx_pmode_dev *di, int pid)
{
	int ret;

	ret = wlrx_pmode_vout_ctrl(di, pid);
	if (ret)
		return ret;

	if (pid == di->curr_pid)
		return 0;

	if (di->dts->pcfg[pid].timeout > 0)
		di->pmode_timeout = jiffies + msecs_to_jiffies(
			di->dts->pcfg[pid].timeout * POWER_MS_PER_S);
	__wlrx_pmode_set_curr_pid(di, pid);
	if (__wlrx_pmode_set_vrx(di, di->dts->pcfg[pid].vrx))
		hwlog_err("set: set vrx failed\n");

	return 0;
}

static void wlrx_pmode_switch(struct wlrx_pmode_dev *di, int start_pid, int end_pid)
{
	int pid;

	if (!wlrx_pmode_is_pid_valid(di, start_pid) ||
		!wlrx_pmode_is_pid_valid(di, end_pid))
		return;

	/* start sample wp test of factory version, don't switch pmode */
	if (power_cmdline_is_factory_mode() &&
		(wlrx_get_charge_stage() != WLRX_STAGE_CHARGING) &&
		(wp_get_sample_type() != WLRX_SAMPLE_WORK_IDLE))
		return;

	for (pid = start_pid; pid >= end_pid; pid--) {
		if (!__wlrx_pmode_judge(di, pid, WLRX_PMODE_FINAL_JUDGE))
			continue;
		if (wlrx_pmode_is_dc_mode(di, pid))
			return;
		wlrx_ic_set_scx_mode(di->drv_type, false);
		if (!wlrx_pmode_set(di, pid))
			break;
	}
	if (pid < 0) {
		__wlrx_pmode_set_curr_pid(di, 0);
		wlrx_pmode_set(di, 0);
	}
}

void wlrx_pmode_select_opt(unsigned int drv_type)
{
	int expect_mode;
	bool curr_pmode_ok = false;
	struct wlrx_pmode_dev *di = wlrx_pmode_get_di(drv_type);

	if (!di)
		return;

	if (wlrx_get_charge_stage() == WLRX_STAGE_CHARGING) {
		wlrx_pmode_switch(di, di->dts->pcfg_level - 1, 0);
	} else {
		expect_mode = di->dts->pcfg[di->curr_pid].expect_mode;
		curr_pmode_ok = __wlrx_pmode_judge(di, di->curr_pid, WLRX_PMODE_FINAL_JUDGE);
		if (!curr_pmode_ok)
			wlrx_pmode_switch(di, di->curr_pid - 1, 0);
		else
			wlrx_pmode_switch(di, expect_mode, di->curr_pid + 1);
	}

	wlrx_pmode_revise_vout(di);
}

static void wlrx_pmode_reset_para_in_init(struct wlrx_pmode_dev *di)
{
	di->bst_err_cnt = 0;
	di->curr_pid = 0;
	wlrx_pmode_set_vid(di, TX_DEFAULT_VOUT);
	di->pmode_timeout = 0;
	wlrx_pmode_reset_icon_flag(di);
	di->dis_mode_type = 0;
}

static void wlrx_pmode_reset_para_in_stop(struct wlrx_pmode_dev *di)
{
	di->curr_pid = 0;
	wlrx_pmode_set_buck_ovp(di, RX_DEFAULT_VOUT, false);
}

static void wlrx_pmode_reset_para_in_proc(struct wlrx_pmode_dev *di)
{
	wlrx_pmode_set_vid(di, TX_DEFAULT_VOUT);
	di->curr_pid = 0;
	wlrx_pctrl_set_para(di->drv_type, WLRX_PCTRL_INIT, 0);
	if (__wlrx_pmode_set_vrx(di, RX_DEFAULT_VOUT))
		hwlog_err("reset_para_in_proc: set vrx failed\n");
}

void wlrx_pmode_reset_para(unsigned int drv_type, int rst_type)
{
	struct wlrx_pmode_dev *di = wlrx_pmode_get_di(drv_type);

	if (!di)
		return;

	switch (rst_type) {
	case WLTRX_RST_IN_INIT:
		wlrx_pmode_reset_para_in_init(di);
		break;
	case WLTRX_RST_IN_STOP:
		wlrx_pmode_reset_para_in_stop(di);
		break;
	case WLTRX_RST_IN_PROC:
		wlrx_pmode_reset_para_in_proc(di);
		break;
	default:
		hwlog_err("reset_para: input=%d invalid\n", rst_type);
		break;
	}
}

static int wlrx_pmode_cfg_str2int(const char *str, int *pcfg, int i)
{
	if (kstrtoint(str, 0, &pcfg[(i - 1) % WLRX_PMODE_CFG_COL]))
		return -EINVAL;

	return 0;
}

static int wlrx_pmode_parse_vmode_cfg(const struct device_node *np,
	struct wlrx_pmode_dts *dts)
{
	int i, len, size;

	len = power_dts_read_u32_count(power_dts_tag(HWLOG_TAG), np,
		"volt_mode", WLRX_VMODE_CFG_ROW, WLRX_VMODE_CFG_COL);
	if (len <= 0)
		return -EINVAL;

	size = sizeof(*dts->vmode_cfg) * (len / WLRX_VMODE_CFG_COL);
	dts->vmode_cfg = kzalloc(size, GFP_KERNEL);
	if (!dts->vmode_cfg)
		return -ENOMEM;

	if (power_dts_read_u32_array(power_dts_tag(HWLOG_TAG), np,
		"volt_mode", (u32 *)dts->vmode_cfg, len))
		return -EINVAL;

	dts->vmode_cfg_level = len / WLRX_VMODE_CFG_COL;
	for (i = 0; i < dts->vmode_cfg_level; i++)
		hwlog_info("vmode[%d], id: %u vtx: %-5d\n",
			i, dts->vmode_cfg[i].id, dts->vmode_cfg[i].vtx);

	return 0;
}

static int wlrx_pmode_parse_pcfg(const struct device_node *np, struct wlrx_pmode_dts *dts)
{
	int i, len, size;
	const char *tmp_str = NULL;
	struct wlrx_pmode *pcfg = NULL;

	len = power_dts_read_count_strings(power_dts_tag(HWLOG_TAG), np,
		"rx_mode_para", WLRX_PMODE_CFG_ROW, WLRX_PMODE_CFG_COL);
	if (len <= 0)
		return -EINVAL;

	size = sizeof(*dts->pcfg) * (len / WLRX_PMODE_CFG_COL);
	dts->pcfg = kzalloc(size, GFP_KERNEL);
	if (!dts->pcfg)
		return -ENOMEM;

	dts->pcfg_level = len / WLRX_PMODE_CFG_COL;
	for (i = 0; i < len; i++) {
		if (power_dts_read_string_index(power_dts_tag(HWLOG_TAG), np,
			"rx_mode_para", i, &tmp_str))
			return -EINVAL;
		if ((i % WLRX_PMODE_CFG_COL) == 0) { /* 0: pmode name */
			dts->pcfg[i / WLRX_PMODE_CFG_COL].name = tmp_str;
			continue;
		}
		if (wlrx_pmode_cfg_str2int(tmp_str,
			(int *)&dts->pcfg[i / WLRX_PMODE_CFG_COL].vtx_min, i))
			return -EINVAL;
	}
	for (i = 0; i < dts->pcfg_level; i++) {
		pcfg = &dts->pcfg[i];
		hwlog_info("pmode[%d] name:%-4s vtx_min:%-5d itx_min:%-4d\t"
			"vtx:%-5d vrx:%-5d irx:%-4d vrect_lth:%-5d tbatt:%-3d\t"
			"cable:%-2d auth:%-2d icon:%u timeout:%-4d expect_mode:%-2d\n",
			i, pcfg->name, pcfg->vtx_min, pcfg->itx_min,
			pcfg->vtx, pcfg->vrx, pcfg->irx, pcfg->vrect_lth,
			pcfg->tbatt, pcfg->cable, pcfg->auth, pcfg->icon,
			pcfg->timeout, pcfg->expect_mode);
	}
	return 0;
}

static int wlrx_pmode_parse_rx_mode_type(const struct device_node *np, struct wlrx_pmode_dts *dts)
{
	int len;

	len = of_property_count_u8_elems(np, "rx_mode_type_para");
	if (len <= 0)
		return 0;

	if (len != dts->pcfg_level)
		return -EINVAL;

	dts->pcfg_mode_type = kzalloc(len * sizeof(*(dts->pcfg_mode_type)), GFP_KERNEL);
	if (!dts->pcfg_mode_type)
		return -EINVAL;

	if (power_dts_read_u8_array(power_dts_tag(HWLOG_TAG), np,
		"rx_mode_type_para", dts->pcfg_mode_type, len)) {
		kfree(dts->pcfg_mode_type);
		return -EINVAL;
	}

	return 0;
}

static int wlrx_pmode_parse_dts(const struct device_node *np, struct wlrx_pmode_dts **dts)
{
	int ret;

	*dts = kzalloc(sizeof(**dts), GFP_KERNEL);
	if (!*dts)
		return -ENOMEM;

	ret = wlrx_pmode_parse_pcfg(np, *dts);
	if (ret)
		return ret;

	ret = wlrx_pmode_parse_vmode_cfg(np, *dts);
	if (ret)
		return ret;

	ret = wlrx_pmode_parse_rx_mode_type(np, *dts);
	if (ret)
		return ret;

	return 0;
}

static void wlrx_pmode_kfree_dev(struct wlrx_pmode_dev *di)
{
	if (!di)
		return;

	if (di->dts) {
		kfree(di->dts->vmode_cfg);
		kfree(di->dts->pcfg);
		if (di->dts->pcfg_mode_type)
			kfree(di->dts->pcfg_mode_type);
		kfree(di->dts);
	}

	kfree(di);
}

int wlrx_pmode_init(unsigned int drv_type, struct device *dev)
{
	int ret;
	struct wlrx_pmode_dev *di = NULL;

	if (!dev || !wltrx_is_drv_type_valid(drv_type))
		return -EINVAL;

	di = kzalloc(sizeof(*di), GFP_KERNEL);
	if (!di)
		return -ENOMEM;

	ret = wlrx_pmode_parse_dts(dev->of_node, &di->dts);
	if (ret)
		goto exit;

	di->bst_rst_completed = true;
	di->drv_type = drv_type;
	wlrx_pmode_reset_icon_flag(di);
	g_rx_pmode_di[drv_type] = di;
	return 0;

exit:
	wlrx_pmode_kfree_dev(di);
	return ret;
}

void wlrx_pmode_deinit(unsigned int drv_type)
{
	if (!wltrx_is_drv_type_valid(drv_type))
		return;

	wlrx_pmode_kfree_dev(g_rx_pmode_di[drv_type]);
	g_rx_pmode_di[drv_type] = NULL;
}
