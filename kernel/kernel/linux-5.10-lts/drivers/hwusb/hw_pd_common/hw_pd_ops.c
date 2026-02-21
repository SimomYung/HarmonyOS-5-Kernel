/*
 * hw_pd_ops.c
 *
 * Source file of external operation interface of huawei PD driver
 *
 * Copyright (C) 2023 HUAWEI, Inc.
 * Author: HUAWEI, Inc.
 *
 * This package is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/delay.h>

#include <huawei_platform/log/hw_log.h>
#include <huawei_platform/usb/hw_pd/hw_pd_event.h>

#include <chipset_common/hwusb/hw_pd/hw_pd_state.h>
#include <chipset_common/hwusb/hw_pd/hw_pd_core.h>
#include <chipset_common/hwusb/hw_pd/hw_pd_ops.h>
#include <chipset_common/hwusb/hw_pd/hw_pd_interface.h>
#include <chipset_common/hwusb/hw_pd/hw_pd_pps_charger.h>
#include <chipset_common/hwpower/reverse_charge/reverse_charge_pd.h>

#ifndef HWLOG_TAG
#define HWLOG_TAG huawei_pd_ops
HWLOG_REGIST();
#endif

/* External functions for operating to PD/TYPEC chip */
static struct pd_dpm_ops *g_pd_dpm_ops = NULL;
/* Context for external functions above */
static void *g_pd_dpm_client = NULL;

/* External function to check whether water is in typec port */
static struct water_det_ops* g_water_det_ops = NULL;

/* External function to check typec cc pins */
static struct cc_check_ops* g_cc_check_ops = NULL;

#ifdef CONFIG_TYPEC_CAP_CUSTOM_SRC2
/* External function to check cable vdo */
static struct cable_vdo_ops *g_cable_vdo_ops = NULL;
#endif

bool hw_pd_support_host_attach(void)
{
	if (!g_pd_dpm_ops || !g_pd_dpm_ops->pd_dpm_inquire_usb_comm)
		return true;

	if (!g_pd_dpm_ops->pd_dpm_inquire_usb_comm(g_pd_dpm_client) && (pd_state(ctc_cable_flag)))
		return false;
	return true;
}

void hw_pd_start_data_role_swap(void)
{
	if (!g_pd_dpm_ops || !g_pd_dpm_ops->data_role_swap || !g_pd_dpm_client)
		return;

	g_pd_dpm_ops->data_role_swap(g_pd_dpm_client);
}

void hw_pd_emark_detect(void)
{
	hwlog_err("%s\n", __func__);

	if (g_pd_dpm_ops && g_pd_dpm_ops->pd_dpm_detect_emark_cable)
		g_pd_dpm_ops->pd_dpm_detect_emark_cable(g_pd_dpm_client);
}

void hw_pd_emark_finish(void)
{
	if (!g_pd_dpm_ops || !g_pd_dpm_ops->data_role_swap ||
		!g_pd_dpm_client || !g_pd_dpm_ops->pd_dpm_inquire_usb_comm)
		return;

	if (g_pd_dpm_ops->pd_dpm_inquire_usb_comm(g_pd_dpm_client) &&
		!g_pd_dpm_ops->data_role_swap(g_pd_dpm_client)) {
		hwlog_info("emark finish data_role_swap\n");
		msleep(50); /* wait 50ms for swap complete */
	}
}

int hw_pd_power_check(void)
{
	if (!g_pd_dpm_ops) {
		hwlog_err("%s g_pd_dpm_ops is NULL\n", __func__);
		return -EPERM;
	}
	return (g_pd_dpm_ops->pd_dpm_inquire_usb_comm(g_pd_dpm_client));
}

int hw_pd_disable_pd_comm(bool disable)
{
	hwlog_info("%s\n", __func__);

	if (!g_pd_dpm_ops) {
		hwlog_err("%s g_pd_dpm_ops is NULL\n", __func__);
		return -EPERM;
	}

	if (!g_pd_dpm_ops->pd_dpm_disable_pd) {
		hwlog_err("%s pd_dpm_disable_pd is NULL\n", __func__);
		return -EPERM;
	}

	return g_pd_dpm_ops->pd_dpm_disable_pd(g_pd_dpm_client, disable);
}

int hw_pd_set_cc_mode(int mode)
{
	static int cur_mode = PD_DPM_CC_MODE_DRP;

	hwlog_info("%s cur=%d, new=%d\n", __func__, cur_mode, mode);
	if (cur_mode == mode)
		return 0;

	if (!g_pd_dpm_ops || !g_pd_dpm_ops->pd_dpm_set_cc_mode) {
		hwlog_err("%s func not set\n", __func__);
		return HW_PD_INVALID_STATE;
	}

	g_pd_dpm_ops->pd_dpm_set_cc_mode(mode);
	cur_mode = mode;
	return 0;
}

/*
 * bugfix: For Hi65xx
 * DRP lost Cable(without adapter) plugin during Wireless.
 */
void hw_pd_set_drp_state(int mode)
{
	if (g_pd_dpm_ops && g_pd_dpm_ops->pd_dpm_enable_drp)
		g_pd_dpm_ops->pd_dpm_enable_drp(mode);
}

void hw_pd_reinit_chip(void)
{
	if (g_pd_dpm_ops && g_pd_dpm_ops->pd_dpm_reinit_chip)
		g_pd_dpm_ops->pd_dpm_reinit_chip(g_pd_dpm_client);
}

bool hw_pd_get_hw_docksvidexist(void)
{
	if (g_pd_dpm_ops && g_pd_dpm_ops->pd_dpm_get_hw_dock_svid_exist)
		return g_pd_dpm_ops->pd_dpm_get_hw_dock_svid_exist(g_pd_dpm_client);

	return false;
}

int hw_pd_set_dc_status(bool dc)
{
	hwlog_err("%s,%d", __func__, __LINE__);
	if (g_pd_dpm_ops && g_pd_dpm_ops->pd_dpm_notify_direct_charge_status) {
		hwlog_err("%s,%d", __func__, __LINE__);
		return g_pd_dpm_ops->pd_dpm_notify_direct_charge_status(g_pd_dpm_client, dc);
	}

	return false;
}

int hw_pd_query_cc_state(unsigned int *cc)
{
	if (!g_pd_dpm_ops || !g_pd_dpm_ops->pd_dpm_get_cc_state || !cc)
		return -1;

	*cc = g_pd_dpm_ops->pd_dpm_get_cc_state();
	return 0;
}

int hw_pd_check_cc_short(bool *check)
{
	if (g_pd_dpm_ops && g_pd_dpm_ops->pd_dpm_check_cc_vbus_short && check) {
		*check = g_pd_dpm_ops->pd_dpm_check_cc_vbus_short();
		hwlog_info("%s ret %d", __func__, (*check));
		return 0;
	}

	return HW_PD_INVALID_STATE;
}

int hw_pd_reg_dpm_ops(struct pd_dpm_ops *ops, void *client)
{
	int ret = 0;

	if (!ops) {
		hwlog_err("pd_dpm ops register fail\n");
		return -EPERM;
	}

	/*
	 * VXXX tcpc dirver lauchs earlier than rt1715 driver(external pd)
	 * and it shall call this function. Overwirte the callback handle
	 * while rt1715 driver calling this
	 */
	if (g_pd_dpm_ops && !pd_state(external_pd_flag)) {
		hwlog_err("pd_dpm ops register fail! g_pd_dpm_ops busy\n");
		return -EBUSY;
	}

	g_pd_dpm_ops = ops;
	g_pd_dpm_client = client;

	return ret;
}


int hw_pd_check_wather_detect(void)
{
	if (!g_water_det_ops || !g_water_det_ops->is_water_detected) {
		hwlog_info("%s no check ops\n", __func__);
		return HW_PD_INVALID_STATE;
	}

	/* If water is not detected, ignore current event */
	return g_water_det_ops->is_water_detected();
}

int hw_pd_reg_water_ops(struct water_det_ops* ops)
{
	int ret = 0;

	if (g_water_det_ops) {
		hwlog_err("water_det_ops register fail! g_water_det_ops busy\n");
		return -EBUSY;
	}

	if (ops) {
		g_water_det_ops = ops;
	} else {
		hwlog_err("water_det_ops register fail\n");
		ret = -EPERM;
	}
	return ret;
}

int hw_pd_dc_cable_detect(void)
{
	int ret;

	if (!g_cc_check_ops)
		return HW_PD_INVALID_STATE;
	ret = g_cc_check_ops->is_cable_for_direct_charge();
	if (ret) {
		hwlog_info("%s:cc_check fail\n", __func__);
		return HW_PD_INVALID_STATE;
	}
	return 0;
}

int hw_pd_reg_cc_ops(struct cc_check_ops* ops)
{
	int ret = 0;

	if (g_cc_check_ops) {
		hwlog_err("cc_check ops register fail! g_cc_check_ops busy\n");
		return -EBUSY;
	}

	if (ops) {
		g_cc_check_ops = ops;
	} else {
		hwlog_err("cc_check ops register fail\n");
		ret = -EPERM;
	}
	return ret;
}

#ifdef CONFIG_TYPEC_CAP_CUSTOM_SRC2
bool hw_pd_is_custsrc2_cable(void)
{
	int ret;
	if (g_cable_vdo_ops && g_cable_vdo_ops->is_cust_src2_cable) {
		ret = g_cable_vdo_ops->is_cust_src2_cable();
		hwlog_info("%s: return %d\n", __func__, ret);
		return (ret != 0);
	}
	return false;
}

int hw_pd_reg_cablevdo_ops(struct cable_vdo_ops *ops)
{
	if (!ops) {
		hwlog_err("cable_vdo_ops ops register fail\n");
		return -EPERM;
	}

	g_cable_vdo_ops = ops;
	return 0;
}
#endif /* CONFIG_TYPEC_CAP_CUSTOM_SRC2 */

int hw_pd_pps_get_apdo_caps(struct pps_apdo_cap *caps, int num, struct pps_pdo_raw *raw)
{
	int ret = -EACCES;

	if (g_pd_dpm_ops && g_pd_dpm_ops->pd_dpm_pps_get_apdo_caps)
		ret = g_pd_dpm_ops->pd_dpm_pps_get_apdo_caps(g_pd_dpm_client, caps, num, raw);
	hwlog_info("%s ret %d\n", __func__, ret);
	return ret;
}

int hw_pd_pps_set_state(enum pps_set_state state, int mv, int ma)
{
	int ret = -EACCES;

	if (g_pd_dpm_ops && g_pd_dpm_ops->pd_dpm_pps_set_state)
		ret = g_pd_dpm_ops->pd_dpm_pps_set_state(g_pd_dpm_client, state, mv, ma);
	hwlog_info("%s: state=%d, mv=%d, ma=%d ret=%d\n",
		__func__, (int)state, mv, ma, ret);
	return ret;
}

int hw_pd_pps_get_status(int *mv, int *ma)
{
	int ret = -EACCES;

	if (g_pd_dpm_ops && g_pd_dpm_ops->pd_dpm_pps_get_status)
		ret = g_pd_dpm_ops->pd_dpm_pps_get_status(g_pd_dpm_client, mv, ma);
	hwlog_info("%s: mv=%d, ma=%d ret=%d\n", __func__, *mv, *ma, ret);
	return ret;
}

int hw_pd_get_dp_state(void)
{
	int ret = -EINVAL;

	if (g_pd_dpm_ops && g_pd_dpm_ops->pd_dpm_get_dp_state)
		ret = g_pd_dpm_ops->pd_dpm_get_dp_state(g_pd_dpm_client);

	hwlog_info("%s: ret=%d\n", __func__, ret);
	return (ret > 0);
}

#ifdef CONFIG_REVERSE_CHARGE
int hw_pd_get_reverse_charge_state(void)
{
	if (g_pd_dpm_ops && g_pd_dpm_ops->pd_dpm_is_reverse_charge_enable)
		return g_pd_dpm_ops->pd_dpm_is_reverse_charge_enable();

	return 0;
}

bool hw_pd_get_rev_sc_dev_flag(void)
{
	if (g_pd_dpm_ops && g_pd_dpm_ops->pd_dpm_get_rev_sc_dev_flag)
		return g_pd_dpm_ops->pd_dpm_get_rev_sc_dev_flag();

	return false;
}

void hw_pd_set_rev_sc_dev_flag(bool flag)
{
	if (g_pd_dpm_ops && g_pd_dpm_ops->pd_dpm_set_rev_sc_dev_flag)
		g_pd_dpm_ops->pd_dpm_set_rev_sc_dev_flag(flag);
}

void hw_pd_set_rev_sc_switch_flag(int rever_point)
{
	if (g_pd_dpm_ops && g_pd_dpm_ops->pd_dpm_set_rev_sc_switch_flag)
		g_pd_dpm_ops->pd_dpm_set_rev_sc_switch_flag(rever_point);
}

bool hw_pd_is_rev_sc_plugin(void)
{
	if (g_pd_dpm_ops && g_pd_dpm_ops->pd_dpm_is_rev_sc_plugin)
		return g_pd_dpm_ops->pd_dpm_is_rev_sc_plugin();

	return false;
}

bool hw_pd_is_rev_sc_plugout(void)
{
	if (g_pd_dpm_ops && g_pd_dpm_ops->pd_dpm_is_rev_sc_plugout)
		return g_pd_dpm_ops->pd_dpm_is_rev_sc_plugout();

	return false;
}
#endif /* CONFIG_REVERSE_CHARGE */

void hw_pd_ops_set_src_psrdelay(int delay)
{
	if (!g_pd_dpm_ops || !g_pd_dpm_ops->pd_dpm_set_src_psrdelay)
		return;

	hwlog_info("%s: set delay %d\n", __func__, delay);
	g_pd_dpm_ops->pd_dpm_set_src_psrdelay(g_pd_dpm_client, delay);
}

bool hw_pd_ops_check_src_cap(void)
{
	int ret = -EINVAL;

	if (g_pd_dpm_ops && g_pd_dpm_ops->pd_dpm_get_src_extcap)
		ret = g_pd_dpm_ops->pd_dpm_get_src_extcap(g_pd_dpm_client);

	hwlog_info("%s: ret=%d\n", __func__, ret);
	return (ret > 0);
}

int hw_pd_ops_update_src_cap(u32 otg_vol, u32 otg_cur, int send)
{
	struct otg_cap allow_ext = {0};

	allow_ext.otg_vol = otg_vol;
	allow_ext.otg_cur = otg_cur;

	if (!g_pd_dpm_ops || !g_pd_dpm_ops->pd_dpm_set_src_cap)
		return -EACCES;

	hwlog_info("%s: max_otg_vol=%d, max_otg_cur=%d, send=%d\n",
		__func__, allow_ext.otg_vol, allow_ext.otg_cur, send);
	g_pd_dpm_ops->pd_dpm_set_src_cap(g_pd_dpm_client, &allow_ext, (send ? 1 : 0));

	return 0;
}

int hw_pd_get_src_cap(void)
{
	if (!g_pd_dpm_ops || !g_pd_dpm_ops->pd_dpm_get_src_cap)
		return false;
 
	return g_pd_dpm_ops->pd_dpm_get_src_cap(g_pd_dpm_client);
}