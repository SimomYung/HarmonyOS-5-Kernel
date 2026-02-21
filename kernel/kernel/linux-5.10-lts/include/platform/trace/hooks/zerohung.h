/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#undef TRACE_SYSTEM
#define TRACE_SYSTEM zerohung

#undef TRACE_INCLUDE_PATH
#define TRACE_INCLUDE_PATH platform/trace/hooks

#if !defined(_TRACE_ZEROHUNG_H) || defined(TRACE_HEADER_MULTI_READ)
#define _TRACE_ZEROHUNG_H

#include <linux/tracepoint.h>
#include <platform_include/basicplatform/linux/hck_vendor_native_hooks.h>
#include <platform_include/basicplatform/linux/hck_vendor_restricted_hooks.h>

DECLARE_HCK_VNH(hung_wp_screen_fake_power_off,
	TP_PROTO(int unused),
	TP_ARGS(unused));

DECLARE_HCK_VNH(hung_wp_screen_getbl,
	TP_PROTO(int *screen_bl_level),
	TP_ARGS(screen_bl_level));

DECLARE_HCK_VNH(hung_wp_screen_setbl,
	TP_PROTO(int level),
	TP_ARGS(level));

#ifdef CONFIG_ARMPC_ECHUB_LID
DECLARE_HCK_VNH(hung_wp_screen_set_lid_status,
	TP_PROTO(int status),
	TP_ARGS(status));
#endif

DECLARE_HCK_VNH(hung_wp_screen_qcom_pkey_press,
	TP_PROTO(int type, int state),
	TP_ARGS(type, state));

DECLARE_HCK_VRH(hung_wp_screen_powerkey_ncb,
	TP_PROTO(unsigned long event),
	TP_ARGS(event), 1);

/*
 * TRACE_EVENT requires at least one argument to be defined.
 * This just throws in a dummy argument.
 */
DECLARE_HCK_VNH(dfx_watchdog_check_hung,
	TP_PROTO(int unused),
	TP_ARGS(unused));

DECLARE_HCK_VNH(dfx_watchdog_lockup_init,
	TP_PROTO(int unused),
	TP_ARGS(unused));

DECLARE_HCK_VNH(dfx_watchdog_lockup_init_work,
	TP_PROTO(int unused),
	TP_ARGS(unused));

#endif /* _TRACE_ZEROHUNG_H */

/* This part must be outside protection */
#include <trace/define_trace.h>
