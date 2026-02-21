/*
 * hw_pd_dsm.c
 *
 * Source file of dsm for huawei PD driver
 *
 * Copyright (C) 2024 HUAWEI, Inc.
 * Author: HUAWEI, Inc.
 *
 * This package is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <securec.h>
#include <linux/device.h>
#include <linux/init.h>
#include <linux/of.h>
#include <linux/module.h>
#include <linux/time.h>

#include <chipset_common/hwusb/hw_pd/hw_pd_core.h>
#include <huawei_platform/log/hw_log.h>
#include <huawei_platform/power/huawei_charger.h>
#include <chipset_common/hwusb/hw_pd/hw_pd_dsm.h>

#ifndef HWLOG_TAG
#define HWLOG_TAG huawei_pd_dsm
HWLOG_REGIST();
#endif

#ifdef CONFIG_HUAWEI_OHOS_DSM
#include <chipset_common/hwpower/common_module/power_dsm.h>
#include <chipset_common/hwpower/common_module/power_supply.h>
#include <chipset_common/hwpower/common_module/power_supply_interface.h>
#include <huawei_platform/hwpower/common_module/power_platform_macro.h>

#define DSM_USB_BUF_SIZE_0016         16
#define DSM_VBUS_ONLY_INTERVAL        (24 * 60 * 60) /* s */

enum dsm_vbus_only_param_index {
	DSM_VBUS_ONLY_PARAM_VBAT = 0,
	DSM_VBUS_ONLY_PARAM_VUSB,
	DSM_VBUS_ONLY_PARAM_IBAT,
	DSM_VBUS_ONLY_PARAM_MAX,
};

static struct timespec64 dsm_vbus_only_ts64_dmd_last = { 0 };

struct hisysevent_error_no_map usb_event_map[] = {
	{ 920004100, POWER_DSM_LEVEL_FAULT, "DSM_USB",  "USB_XHCI_CMD_TIMEOUT", "CONTENT", 0 },
	{ 920004101, POWER_DSM_LEVEL_STATISTIC, "DSM_USB",  "USB_RECORD_ABNORMAL_DEVICE", "CONTENT", 0 },
	{ 920004102, POWER_DSM_LEVEL_FAULT, "DSM_USB",  "USB_AP_USE_HIFIUSB_ENUM_TIMEOUT", "CONTENT", 0 },
	{ 920004104, POWER_DSM_LEVEL_FAULT, "DSM_USB",  "USB_RETRIGGER_APSD", "CONTENT", 0 },
	{ 920004200, POWER_DSM_LEVEL_STATISTIC, "DSM_USB",  "DSM_PD_TYPEC_VBUS_VALID", "CONTENT", 0 },
	{ 920004201, POWER_DSM_LEVEL_FAULT, "DSM_USB",  "DSM_PD_TYPEC_VBUS_STILL_VALID", "CONTENT", 0 },
	{ 920014000, POWER_DSM_LEVEL_STATISTIC, "DSM_USB",  "ERROR_NO_WATER_CHECK_IN_USB", "CONTENT", 0 },
	{ 928021000, POWER_DSM_LEVEL_FAULT, "DSM_USB",  "HUB_POWER_ON_ERROR", "CONTENT", 0 },
	{ 928021001, POWER_DSM_LEVEL_FAULT, "DSM_USB",  "HUB_CREATE_NODE_ERROR", "CONTENT", 0 },
	{ 928021002, POWER_DSM_LEVEL_FAULT, "DSM_USB",  "HUB_SWITCH_MODE_ERROR", "CONTENT", 0 },
	{ 928021003, POWER_DSM_LEVEL_FAULT, "DSM_USB",  "HUB_STANDBY_MODE_ERROR", "CONTENT", 0 },
	{ 928021004, POWER_DSM_LEVEL_FAULT, "DSM_USB",  "HUB_I2C_ERROR", "CONTENT", 0 },
	{ 928021005, POWER_DSM_LEVEL_FAULT, "DSM_USB",  "HUB_ENABLE_ERROR", "CONTENT", 0 },
	{ 928021006, POWER_DSM_LEVEL_FAULT, "DSM_USB",  "TBOX_REG_INFO", "CONTENT", 0 },
	{ 928021007, POWER_DSM_LEVEL_FAULT, "DSM_USB",  "TBOX_UNREG_INFO", "CONTENT", 0 },
	{ 928021008, POWER_DSM_LEVEL_FAULT, "DSM_USB",  "APA_REG_INFO", "CONTENT", 0 },
	{ 928021009, POWER_DSM_LEVEL_FAULT, "DSM_USB",  "APA_UNREG_INFO", "CONTENT", 0 },
	{ 928021010, POWER_DSM_LEVEL_FAULT, "DSM_USB",  "C385_TBOX_REG_INFO", "CONTENT", 0 },
	{ 928021011, POWER_DSM_LEVEL_FAULT, "DSM_USB",  "C385_TBOX_UNREG_INFO", "CONTENT", 0 },
	{ 928021012, POWER_DSM_LEVEL_FAULT, "DSM_USB",  "SWITCH_TO_HOST_MODE_INFO", "CONTENT", 0 },
	{ 928021013, POWER_DSM_LEVEL_FAULT, "DSM_USB",  "SWITCH_TO_DEVICE_MODE_INFO", "CONTENT", 0 },
	{ 936000102, POWER_DSM_LEVEL_STATISTIC, "DSM_USB",  "DP_IMONITOR_BASIC_INFO_NO", "CONTENT", 0 },
	{ 936000103, POWER_DSM_LEVEL_STATISTIC, "DSM_USB",  "DP_IMONITOR_TIME_INFO_NO", "CONTENT", 0 },
	{ 936000104, POWER_DSM_LEVEL_STATISTIC, "DSM_USB",  "DP_IMONITOR_EXTEND_INFO_NO", "CONTENT", 0 },
	{ 936000105, POWER_DSM_LEVEL_STATISTIC, "DSM_USB",  "DP_IMONITOR_HPD_NO", "CONTENT", 0 },
	{ 936000106, POWER_DSM_LEVEL_STATISTIC, "DSM_USB",  "DP_IMONITOR_LINK_TRAINING_NO", "CONTENT", 0 },
	{ 936000107, POWER_DSM_LEVEL_STATISTIC, "DSM_USB",  "DP_IMONITOR_HOTPLUG_NO", "CONTENT", 0 },
	{ 936000108, POWER_DSM_LEVEL_STATISTIC, "DSM_USB",  "DP_IMONITOR_HDCP_NO", "CONTENT", 0 },
	{ 920004200, POWER_DSM_LEVEL_STATISTIC, "DSM_USB",  "DSM_PD_TYPEC_VBUS_VALID", "CONTENT", 0 },
	{ 920004201, POWER_DSM_LEVEL_FAULT, "DSM_USB",  "DSM_PD_TYPEC_VBUS_STILL_VALID", "CONTENT", 0 },
	{ 920004105, POWER_DSM_LEVEL_STATISTIC, "DSM_USB",  "CONTINUOUS_STOP_CHARGING", "CONTENT", 0 },
	{ 920002024, POWER_DSM_LEVEL_STATISTIC, "DSM_USB",  "PORT_ELECTRIFIED", "CONTENT", 0 },
};

void pd_power_dsm_init(void)
{
	struct power_dsm_err_map *usb_dsm_map = NULL;
	int i;
	usb_dsm_map = power_dsm_single_map_register("DSM_USB");
	if (usb_dsm_map) {
		usb_dsm_map->event_num = sizeof(usb_event_map) / sizeof(usb_event_map[0]);
		for (i = 0; i < usb_dsm_map->event_num; i++) {
			usb_dsm_map->map[i].error_no = usb_event_map[i].error_no;
			usb_dsm_map->map[i].level = usb_event_map[i].level;
			(void)strncpy_s(usb_dsm_map->map[i].domain, POWER_DSM_BUF_SIZE_0016, usb_event_map[i].domain, strlen(usb_event_map[i].domain));
			(void)strncpy_s(usb_dsm_map->map[i].name, POWER_DSM_BUF_SIZE_0032, usb_event_map[i].name, strlen(usb_event_map[i].name));
			(void)strncpy_s(usb_dsm_map->map[i].key, POWER_DSM_BUF_SIZE_0048, usb_event_map[i].key, strlen(usb_event_map[i].key));
		}
	}
	return;
}

void hw_pd_dsm_vbus_only(void)
{
	int vbat = 0;
	int vusb;
	int ibat = 0;
	int ret;
	int i;
	struct hiview_info info[DSM_VBUS_ONLY_PARAM_MAX] = { 0 };
	char hiview_str[DSM_VBUS_ONLY_PARAM_MAX][DSM_USB_BUF_SIZE_0016] = { 0 };
	int hiview_value[DSM_VBUS_ONLY_PARAM_MAX] = { 0 };

	if (hw_pd_time_interval(DSM_VBUS_ONLY_INTERVAL, &dsm_vbus_only_ts64_dmd_last)) {
		power_supply_get_int_prop(POWER_PLATFORM_BAT_PSY_NAME,
				POWER_SUPPLY_PROP_VOLTAGE_NOW, &vbat,
				POWER_SUPPLY_DEFAULT_VOLTAGE_NOW, POWER_UV_PER_MV);
		vusb = charge_get_vusb();
		power_supply_get_int_prop(POWER_PLATFORM_BAT_PSY_NAME,
				POWER_SUPPLY_PROP_CURRENT_NOW, &ibat,
				POWER_SUPPLY_DEFAULT_CURRENT_NOW, 1);

		hwlog_info("%s INT--VBAT:%d VUSB:%d IBAT:%d\n", __func__, vbat, vusb, ibat);
		info[DSM_VBUS_ONLY_PARAM_VBAT].key = "VBAT";
		hiview_value[DSM_VBUS_ONLY_PARAM_VBAT] = vbat;
		info[DSM_VBUS_ONLY_PARAM_VUSB].key = "VUSB";
		hiview_value[DSM_VBUS_ONLY_PARAM_VUSB] = vusb;
		info[DSM_VBUS_ONLY_PARAM_IBAT].key = "IBAT";
		hiview_value[DSM_VBUS_ONLY_PARAM_IBAT] = ibat;

		for (i = 0; i < DSM_VBUS_ONLY_PARAM_MAX; i++) {
			ret = snprintf_s(hiview_str[i], DSM_USB_BUF_SIZE_0016,
				DSM_USB_BUF_SIZE_0016 - 1, "%d", hiview_value[i]);
			if (ret < 0) {
				hwlog_err("snprintf_s failed\n");
				return;
			}
			info[i].value = hiview_str[i];
		}
		ret = power_dsm_report_multi_hiview(DSM_USB_PORT_ELECTRIFIED, info, DSM_VBUS_ONLY_PARAM_MAX);
		hwlog_info("%s :power_dsm_report_multi_hiview return %d\n", __func__, ret);
	}
}
#endif /* CONFIG_HUAWEI_OHOS_DSM */

bool hw_pd_time_interval(int time_interval, struct timespec64 *last_time) {
	struct timespec64 ts64_now = { 0 };
	struct timespec64 ts64_sum = { 0 };
	struct timespec64 ts64_interval = { 0 };

	if (!last_time) {
		hwlog_err("last_time param error\n");
		return false;
	}

	ts64_interval.tv_sec = time_interval;
	ts64_interval.tv_nsec = 0;

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5, 10, 0))
	ktime_get_coarse_real_ts64(&ts64_now);
#else
	ts64_now = current_kernel_time64();
#endif
	ts64_sum = timespec64_add_safe(*last_time, ts64_interval);
	if (ts64_sum.tv_sec == TIME_T_MAX) {
		hwlog_err("%s time overflow happend when add time_interval\n", __func__);
	} else if (timespec64_compare(&ts64_sum, &ts64_now) < 0) {
		*last_time = ts64_now;
		return true;
	} else {
		hwlog_info("happend within time_interval, do not report\n");
	}
	return false;
}
