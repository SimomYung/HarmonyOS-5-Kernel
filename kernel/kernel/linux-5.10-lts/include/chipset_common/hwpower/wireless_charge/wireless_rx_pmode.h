/* SPDX-License-Identifier: GPL-2.0 */
/*
 * wireless_rx_pmode.h
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

#ifndef _WIRELESS_RX_PMODE_H_
#define _WIRELESS_RX_PMODE_H_

#include <chipset_common/hwpower/wireless_charge/wireless_trx_intf.h>
#include <linux/slab.h>

#define WLRX_PMODE_CFG_ROW           8
#define WLRX_PMODE_CFG_COL           13

/* attention: mode type should not be modified */
#define WLRX_SUPP_PMODE_BUCK         BIT(0)
#define WLRX_SUPP_PMODE_SC2          BIT(1)
#define WLRX_SUPP_PMODE_SC4          BIT(2)
#define WLRX_SUPP_PMODE_ALL          0xff

enum wlrx_pmode_judge_type {
	WLRX_PMODE_QUICK_JUDGE = 0, /* quick icon display */
	WLRX_PMODE_NORMAL_JUDGE, /* recorecting icon display */
	WLRX_PMODE_FINAL_JUDGE, /* judging power mode */
	WLDC_PMODE_FINAL_JUDGE, /* wireless direct charging */
};

enum {
	WLRX_PMODE_BEGIN = 0,
	WLRX_PMODE_NAME = WLRX_PMODE_BEGIN,
	WLRX_PMODE_VTX_MIN,
	WLRX_PMODE_ITX_MIN,
	WLRX_PMODE_VTX,
	WLRX_PMODE_VRX,
	WLRX_PMODE_IRX,
	WLRX_PMODE_VRECT_TH,
	WLRX_PMODE_TBATT,
	WLRX_PMODE_CABLE,
	WLRX_PMODE_AUTH,
	WLRX_PMODE_ICON,
	WLRX_PMODE_TIMEOUT,
	WLRX_PMODE_EXP_MODE,
	WLRX_PMODE_END,
};

enum wlrx_pmode_type {
	WLRX_PMODE_TYPE_BEGIN = 0,
	WLRX_PMODE_TYPE_5VBUCK = WLRX_PMODE_TYPE_BEGIN,
	WLRX_PMODE_TYPE_9VBUCK,
	WLRX_PMODE_TYPE_12VBUCK,
	WLRX_PMODE_TYPE_5VLVC,
	WLRX_PMODE_TYPE_9VLVC,
	WLRX_PMODE_TYPE_9VSC,
	WLRX_PMODE_TYPE_15VSC,
	WLRX_PMODE_TYPE_END,
};

struct wlrx_pmode {
	const char *name;
	int vtx_min;
	int itx_min;
	int vtx; /* ctrl_para */
	int vrx; /* ctrl_para */
	int irx; /* ctrl_para */
	int vrect_lth;
	int tbatt;
	int cable; /* cable detect type */
	int auth; /* authenticate type */
	unsigned int icon;
	int timeout;
	int expect_mode;
};

struct wlrx_vmode {
	u32 id;
	int vtx;
};

#ifdef CONFIG_WIRELESS_CHARGER
int wlrx_pmode_get_pid_by_name(unsigned int drv_type, const char *mode_name);
int wlrx_pmode_get_curr_int_cfg(unsigned int drv_type, int cfg_type);
bool wlrx_pmode_in_dc_mode(unsigned int drv_type);
int wlrx_pmode_get_curr_pid(unsigned int drv_type);
void wlrx_pmode_set_curr_pid(unsigned int drv_type, int pid);
bool wlrx_pmode_judge(unsigned int drv_type, int pid, unsigned int judge_type);
void wlrx_pmode_select_opt(unsigned int drv_type);
void wlrx_pmode_reset_para(unsigned int drv_type, int rst_type);
int wlrx_pmode_get_supported_max_vrx(unsigned int drv_type);
bool wlrx_pmode_need_auth(unsigned int drv_type);
int wlrx_pmode_set_vtx(unsigned int drv_type, int vtx);
int wlrx_pmode_set_vrx(unsigned int drv_type, int vrx);
int wlrx_pmode_set_vtrx(unsigned int drv_type, int vtx, int vrx);
bool wlrx_pmode_bst_rst_completed(unsigned int drv_type);
void wlrx_pmode_clear_icon_flag(unsigned int drv_type, int pid);
void wlrx_pmode_icon_display(unsigned int drv_type, int judge_type);
void wlrx_pmode_disable_dc_icon(unsigned int drv_type, bool flag);
int wlrx_pmode_get_support_mode(unsigned int drv_type);
void wlrx_pmode_set_mode_type(unsigned int drv_type, unsigned long mode_type, bool enable);
#else
static inline int wlrx_pmode_get_pid_by_name(unsigned int drv_type, const char *mode_name)
{
	return 0;
}

static inline int wlrx_pmode_get_curr_int_cfg(unsigned int drv_type, int cfg_type)
{
	return -EINVAL;
}

static inline bool wlrx_pmode_in_dc_mode(unsigned int drv_type)
{
	return false;
}

static inline int wlrx_pmode_get_curr_pid(unsigned int drv_type)
{
	return 0;
}

static inline void wlrx_pmode_set_curr_pid(unsigned int drv_type, int pid)
{
}

static inline bool wlrx_pmode_judge(unsigned int drv_type, int pid, unsigned int judge_type)
{
	return false;
}

static inline void wlrx_pmode_select_opt(unsigned int drv_type)
{
}

static inline void wlrx_pmode_reset_para(unsigned int drv_type, int rst_type)
{
}

static inline int wlrx_pmode_get_supported_max_vrx(unsigned int drv_type)
{
	return WLTRX_VOLT_5V;
}

static inline bool wlrx_pmode_need_auth(unsigned int drv_type)
{
	return false;
}

static inline int wlrx_pmode_set_vtx(unsigned int drv_type, int vtx)
{
	return -EPERM;
}

static inline int wlrx_pmode_set_vrx(unsigned int drv_type, int vrx)
{
	return -EPERM;
}

static inline int wlrx_pmode_set_vtrx(unsigned int drv_type, int vtx, int vrx)
{
	return -EPERM;
}

static inline bool wlrx_pmode_bst_rst_completed(unsigned int drv_type)
{
	return false;
}

static inline void wlrx_pmode_clear_icon_flag(unsigned int drv_type, int pid)
{
}

static inline void wlrx_pmode_icon_display(unsigned int drv_type, int judge_type)
{
}

static inline void wlrx_pmode_disable_dc_icon(unsigned int drv_type, bool flag)
{
}

static inline int wlrx_pmode_get_support_mode(unsigned int drv_type)
{
	return WLRX_SUPP_PMODE_BUCK;
}

static inline void wlrx_pmode_set_mode_type(unsigned int drv_type, unsigned long mode_type, bool enable)
{
}
#endif /* CONFIG_WIRELESS_CHARGER */

#endif /* _WIRELESS_RX_PMODE_H_ */
