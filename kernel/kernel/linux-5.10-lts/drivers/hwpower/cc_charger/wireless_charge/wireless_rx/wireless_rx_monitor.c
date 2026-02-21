// SPDX-License-Identifier: GPL-2.0
/*
 * wireless_rx_monitor.c
 *
 * monitor info for wireless charging
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

#include <chipset_common/hwpower/wireless_charge/wireless_rx_monitor.h>
#include <huawei_platform/hwpower/wireless_charge/wireless_rx_platform.h>
#include <chipset_common/hwpower/wireless_charge/wireless_rx_status.h>
#include <chipset_common/hwpower/wireless_charge/wireless_rx_acc.h>
#include <chipset_common/hwpower/wireless_charge/wireless_rx_pmode.h>
#include <chipset_common/hwpower/wireless_charge/wireless_rx_plim.h>
#include <chipset_common/hwpower/wireless_charge/wireless_rx_ic_intf.h>
#include <chipset_common/hwpower/wireless_charge/wireless_rx_alarm.h>
#include <chipset_common/hwpower/wireless_charge/wireless_rx_interfere.h>
#include <huawei_platform/power/wireless/wireless_direct_charger.h>
#include <chipset_common/hwpower/common_module/power_printk.h>
#include <chipset_common/hwpower/common_module/power_dts.h>
#include <huawei_platform/hwpower/common_module/power_platform.h>
#include <chipset_common/hwpower/battery/battery_temp.h>

#define HWLOG_TAG wireless_rx_monitor
HWLOG_REGIST();

/* for vout check */
#define WLRX_VOUT_ERR_CHK_TIME        1000 /* 1s */
#define WLRX_VRECT_VOUT_ERR_CHK_TIME  10000 /* 10s */
#define WLRX_VRECT_VOUT_DIFF_ERR_TH   2500 /* 2500mV */
#define WLRX_VOUT_ERR_DLFT_RATIO      81 /* 81% */

enum wlrx_mon_voltchk_status {
	WLRX_VOLTCHK_ABNORMAL = -1,
	WLRX_VOLTCHK_NORMAL,
	WLRX_VOLTCHK_IGNORABLE,
};

struct wlrx_mon_info {
	/* for rx_ic */
	int vrect;
	int vout;
	int iout;
	int fop;
	int temp;
	int cep;
	int imax;
	int iavg;
	/* for system */
	int soc;
	int tbatt;
	int pid; /* pmode index */
	unsigned int plim_src;
	/* for dc */
	int dc_err;
	int dc_warning;
	int tbatt_lth;
	int tbatt_hth;
	/* for buck */
	int buck_iin_reg;
	/* for acc */
	int palarm;
	/* for interference setting */
	unsigned int intfr_src;
	int intfr_vtx;
	int intfr_vrx;
	int intfr_irx;
	int intfr_fop;
};

struct wlrx_mon_dts {
	int vout_err_ratio;
};

struct wlrx_mon_dev {
	unsigned int drv_type;
	struct wlrx_mon_dts *dts;
	int vout_err_cnt;
	int vrect_vout_diff_err_cnt;
};

static struct wlrx_mon_dev *g_rx_mon_di[WLTRX_DRV_MAX];

static struct wlrx_mon_dev *wlrx_mon_get_di(unsigned int drv_type)
{
	if (!wltrx_is_drv_type_valid(drv_type)) {
		hwlog_err("get_di: drv_type=%u err\n", drv_type);
		return NULL;
	}

	return g_rx_mon_di[drv_type];
}

static unsigned int wlrx_mon_get_ic_type(unsigned int drv_type)
{
	return wltrx_get_dflt_ic_type(drv_type);
}

static void wlrx_mon_update_sys_info(struct wlrx_mon_dev *di, struct wlrx_mon_info *info)
{
	(void)bat_temp_get_temperature(BAT_TEMP_MIXED, &info->tbatt);
	info->soc = power_platform_get_battery_ui_capacity();
	info->pid = wlrx_pmode_get_curr_pid(di->drv_type);
	info->plim_src = wlrx_plim_get_src(di->drv_type);
}

static void wlrx_mon_update_ic_info(struct wlrx_mon_dev *di, struct wlrx_mon_info *info)
{
	unsigned int ic_type = wlrx_mon_get_ic_type(di->drv_type);

	wlrx_ic_get_imax(ic_type, &info->imax);
	wlrx_ic_get_iavg(ic_type, &info->iavg);
	(void)wlrx_ic_get_vrect(ic_type, &info->vrect);
	(void)wlrx_ic_get_vout(ic_type, &info->vout);
	(void)wlrx_ic_get_iout(ic_type, &info->iout);
	(void)wlrx_ic_get_fop(ic_type, &info->fop);
	(void)wlrx_ic_get_temp(ic_type, &info->temp);
	(void)wlrx_ic_get_cep(ic_type, &info->cep);
}

static void wlrx_mon_update_acc_info(struct wlrx_mon_dev *di, struct wlrx_mon_info *info)
{
	info->palarm = wlrx_get_alarm_plim();
}

static void wlrx_mon_update_buck_info(struct wlrx_mon_dev *di, struct wlrx_mon_info *info)
{
	info->buck_iin_reg = wlrx_buck_get_iin_regval(di->drv_type);
}

static void wlrx_mon_update_dc_info(struct wlrx_mon_dev *di, struct wlrx_mon_info *info)
{
	info->dc_err = wldc_get_error_cnt();
	info->dc_warning = wldc_get_warning_cnt();
	info->tbatt_lth = wldc_get_tbatt_lth();
	info->tbatt_hth = wldc_get_tbatt_hth();
}

static void wlrx_mon_update_sysfs_info(struct wlrx_mon_dev *di, struct wlrx_mon_info *info)
{
	info->intfr_src = wlrx_intfr_get_src(di->drv_type);
	info->intfr_vtx = wlrx_intfr_get_vtx(di->drv_type);
	info->intfr_vrx = wlrx_intfr_get_vrx(di->drv_type);
	info->intfr_irx = wlrx_intfr_get_irx(di->drv_type);
	info->intfr_fop = wlrx_intfr_get_fixed_fop(di->drv_type);
}

static void wlrx_mon_print_info(struct wlrx_mon_info *info)
{
	hwlog_info("[monitor_info] [sys]soc:%-3d tbatt:%d pid:%d plim_src:0x%02x\t"
		"[dc] warn:%d err:%d tbatt_lth:%d tbatt_hth:%d [tx] palarm:%d\t"
		"[rx] vrect:%-5d vout:%-5d iout:%-4d fop:%-3d cep:%-3d imax:%-4d iavg:%-4d temp:%-3d\t"
		"[buck] iin_reg:%-4d [intfr] src:0x%02x vtx:%-5d vrx:%-5d irx:%-4d fop:%-3d\n",
		info->soc, info->tbatt, info->pid, info->plim_src,
		info->dc_warning, info->dc_err, info->tbatt_lth, info->tbatt_hth, info->palarm,
		info->vrect, info->vout, info->iout, info->fop, info->cep,
		info->imax, info->iavg, info->temp, info->buck_iin_reg,
		info->intfr_src, info->intfr_vtx, info->intfr_vrx, info->intfr_irx, info->intfr_fop);
}

static bool wlrx_mon_need_show_info(struct wlrx_mon_dev *di)
{
	int buck_ireg;
	static int buck_ireg_last;
	static int cnt;

	if (wlrx_is_fac_tx(di->drv_type))
		return true;
	if (wlrx_get_charge_stage() < WLRX_STAGE_CHARGING)
		return true;
	if (!wlrx_pmode_bst_rst_completed(di->drv_type))
		return true;

	if (++cnt == WLRX_MON_LOG_INTERVAL / WLRX_MON_WORK_INTERVAL) {
		cnt = 0;
		return true;
	}

	buck_ireg = wlrx_buck_get_iin_regval(di->drv_type);
	if (buck_ireg_last != buck_ireg) {
		buck_ireg_last = buck_ireg;
		return true;
	}

	return false;
}

static void wlrx_mon_show_info(struct wlrx_mon_dev *di)
{
	struct wlrx_mon_info info;

	if (!wlrx_mon_need_show_info(di))
		return;

	memset(&info, 0, sizeof(info));
	wlrx_mon_update_sys_info(di, &info);
	wlrx_mon_update_ic_info(di, &info);
	wlrx_mon_update_acc_info(di, &info);
	wlrx_mon_update_buck_info(di, &info);
	wlrx_mon_update_dc_info(di, &info);
	wlrx_mon_update_sysfs_info(di, &info);
	wlrx_mon_print_info(&info);
}

static enum wlrx_mon_voltchk_status wlrx_mon_volt_check_status(struct wlrx_mon_dev *di)
{
	unsigned int ic_type;
	int vout = 0;
	int vbus = 0;
	int iavg = 0;
	int vout_reg = 0;

	ic_type = wlrx_mon_get_ic_type(di->drv_type);
	(void)wlrx_ic_get_vout(ic_type, &vout);
	if ((vout <= 0) || !wlrx_pmode_bst_rst_completed(di->drv_type) ||
		(wlrx_get_charge_stage() < WLRX_STAGE_HANDSHAKE))
		return WLRX_VOLTCHK_IGNORABLE;

	(void)charge_get_vbus(&vbus);
	vout = (vout >= vbus) ? vout : vbus;
	(void)wlrx_ic_get_vout_reg(ic_type, &vout_reg);
	if (vout >= vout_reg * di->dts->vout_err_ratio / POWER_PERCENT)
		return WLRX_VOLTCHK_NORMAL;

	wlrx_ic_get_iavg(ic_type, &iavg);
	if (iavg >= RX_EPT_IGNORE_IOUT)
		return WLRX_VOLTCHK_IGNORABLE;

	hwlog_err("volt_check_status: vout=%dmV lower than %d*%d%%mV\n",
		vout, vout_reg, di->dts->vout_err_ratio);
	return WLRX_VOLTCHK_ABNORMAL;
}

static void wlrx_mon_abnormal_volt_act(struct wlrx_mon_dev *di)
{
	unsigned int ic_type;
	int vfc_reg = 0;

	ic_type = wlrx_mon_get_ic_type(di->drv_type);
	(void)wlrx_ic_get_vfc_reg(ic_type, &vfc_reg);
	if (!wlrx_ic_is_sleep_enable(ic_type) && (vfc_reg >= WLTRX_VOLT_12V)) {
		wlrx_plim_set_src(di->drv_type, WLRX_PLIM_SRC_VOUT_ERR);
		hwlog_err("abnormal_volt_act: high vout err\n");
		return;
	}

	if (wlrx_get_wireless_channel_state() == WIRELESS_CHANNEL_ON) {
		wlrx_ic_send_ept(ic_type, WIRELESS_EPT_ERR_VOUT);
		hwlog_info("[abnormal_volt_act] vout lower than vout_reg*%d%% for %dms, send EPT\n",
			di->dts->vout_err_ratio, WLRX_VOUT_ERR_CHK_TIME);
	}
}

static void wlrx_mon_check_volt(struct wlrx_mon_dev *di)
{
	enum wlrx_mon_voltchk_status ret;
	int cnt_max = WLRX_VOUT_ERR_CHK_TIME / WLRX_MON_WORK_INTERVAL;

	ret = wlrx_mon_volt_check_status(di);
	if (ret == WLRX_VOLTCHK_IGNORABLE)
		return;
	if (ret == WLRX_VOLTCHK_NORMAL) {
		di->vout_err_cnt = 0;
		return;
	}

	if (++di->vout_err_cnt < cnt_max)
		return;
	di->vout_err_cnt = cnt_max;

	wlrx_mon_abnormal_volt_act(di);
}

static void wlrx_mon_vrect_vout_diff_check(struct wlrx_mon_dev *di)
{
	unsigned int ic_type;
	int vrect = 0;
 	int vout = 0;
	int cnt_max = WLRX_VRECT_VOUT_ERR_CHK_TIME / WLRX_MON_WORK_INTERVAL;

	(void)wlrx_ic_get_vrect(WLTRX_IC_MAIN, &vrect);
 	(void)wlrx_ic_get_vout(WLTRX_IC_MAIN, &vout);
	if (vrect - vout < WLRX_VRECT_VOUT_DIFF_ERR_TH) {
		di->vrect_vout_diff_err_cnt = 0;
		return;
	}
	if (++di->vrect_vout_diff_err_cnt < cnt_max)
		return;

	di->vrect_vout_diff_err_cnt = 0;
	ic_type = wlrx_mon_get_ic_type(di->drv_type);
	wlrx_ic_send_ept(ic_type, WIRELESS_EPT_ERR_VOUT);
	hwlog_info("%s vout lower than vrect %dmV for %dms, send EPT\n",
		__func__, WLRX_VRECT_VOUT_DIFF_ERR_TH, WLRX_VRECT_VOUT_ERR_CHK_TIME);
}

void wlrx_mon_handler(unsigned int drv_type)
{
	struct wlrx_mon_dev *di = wlrx_mon_get_di(drv_type);

	if (!di)
		return;

	wlrx_mon_check_volt(di);
	wlrx_mon_vrect_vout_diff_check(di);
	wlrx_mon_show_info(di);
}

void wlrx_mon_reset_para(unsigned int drv_type, int rst_type)
{
	struct wlrx_mon_dev *di = wlrx_mon_get_di(drv_type);

	if (!di)
		return;

	switch (rst_type) {
	case WLTRX_RST_IN_INIT:
		di->vout_err_cnt = 0;
		di->vrect_vout_diff_err_cnt = 0;
		break;
	default:
		break;
	}
}

static int wlrx_mon_parse_dts(const struct device_node *np, struct wlrx_mon_dev *di)
{
	di->dts = kzalloc(sizeof(*di->dts), GFP_KERNEL);
	if (!di->dts)
		return -ENOMEM;

	(void)power_dts_read_u32(power_dts_tag(HWLOG_TAG), np,
		"rx_vout_err_ratio", (u32 *)&di->dts->vout_err_ratio, WLRX_VOUT_ERR_DLFT_RATIO);

	return 0;
}

static void wlrx_mon_kfree_dev(struct wlrx_mon_dev *di)
{
	if (!di)
		return;

	if (di->dts)
		kfree(di->dts);

	kfree(di);
}

int wlrx_mon_init(unsigned int drv_type, struct device *dev)
{
	int ret;
	struct wlrx_mon_dev *di = NULL;

	if (!dev || !wltrx_is_drv_type_valid(drv_type))
		return -ENODEV;

	di = kzalloc(sizeof(*di), GFP_KERNEL);
	if (!di)
		return -ENOMEM;
	ret = wlrx_mon_parse_dts(dev->of_node, di);
	if (ret)
		goto exit;

	di->drv_type = drv_type;
	g_rx_mon_di[drv_type] = di;
	return 0;

exit:
	wlrx_mon_kfree_dev(di);
	return ret;
}

void wlrx_mon_deinit(unsigned int drv_type)
{
	if (!wltrx_is_drv_type_valid(drv_type))
		return;

	wlrx_mon_kfree_dev(g_rx_mon_di[drv_type]);
	g_rx_mon_di[drv_type] = NULL;
}
