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

#include <chipset_common/hwusb/hw_pd/hw_pd_core.h>
#include <huawei_platform/log/hw_log.h>
#include <huawei_platform/power/huawei_charger.h>

#ifdef CONFIG_HUAWEI_OHOS_DSM
#include <chipset_common/hwpower/common_module/power_dsm.h>

struct hisysevent_error_no_map usb_event_map[] = {
	{ 920004100, POWER_DSM_LEVEL_FAULT, "DSM_USB",  "USB_XHCI_CMD_TIMEOUT", "CONTENT" },
	{ 920004101, POWER_DSM_LEVEL_STATISTIC, "DSM_USB",  "USB_RECORD_ABNORMAL_DEVICE", "CONTENT" },
	{ 920004102, POWER_DSM_LEVEL_FAULT, "DSM_USB",  "USB_AP_USE_HIFIUSB_ENUM_TIMEOUT", "CONTENT" },
	{ 920004104, POWER_DSM_LEVEL_FAULT, "DSM_USB",  "USB_RETRIGGER_APSD", "CONTENT" },
	{ 920004200, POWER_DSM_LEVEL_STATISTIC, "DSM_USB",  "DSM_PD_TYPEC_VBUS_VALID", "CONTENT" },
	{ 920004201, POWER_DSM_LEVEL_FAULT, "DSM_USB",  "DSM_PD_TYPEC_VBUS_STILL_VALID", "CONTENT" },
	{ 920014000, POWER_DSM_LEVEL_STATISTIC, "DSM_USB",  "ERROR_NO_WATER_CHECK_IN_USB", "CONTENT" },
	{ 928021000, POWER_DSM_LEVEL_FAULT, "DSM_USB",  "HUB_POWER_ON_ERROR", "CONTENT" },
	{ 928021001, POWER_DSM_LEVEL_FAULT, "DSM_USB",  "HUB_CREATE_NODE_ERROR", "CONTENT" },
	{ 928021002, POWER_DSM_LEVEL_FAULT, "DSM_USB",  "HUB_SWITCH_MODE_ERROR", "CONTENT" },
	{ 928021003, POWER_DSM_LEVEL_FAULT, "DSM_USB",  "HUB_STANDBY_MODE_ERROR", "CONTENT" },
	{ 928021004, POWER_DSM_LEVEL_FAULT, "DSM_USB",  "HUB_I2C_ERROR", "CONTENT" },
	{ 928021005, POWER_DSM_LEVEL_FAULT, "DSM_USB",  "HUB_ENABLE_ERROR", "CONTENT" },
	{ 928021006, POWER_DSM_LEVEL_FAULT, "DSM_USB",  "TBOX_REG_INFO", "CONTENT" },
	{ 928021007, POWER_DSM_LEVEL_FAULT, "DSM_USB",  "TBOX_UNREG_INFO", "CONTENT" },
	{ 928021008, POWER_DSM_LEVEL_FAULT, "DSM_USB",  "APA_REG_INFO", "CONTENT" },
	{ 928021009, POWER_DSM_LEVEL_FAULT, "DSM_USB",  "APA_UNREG_INFO", "CONTENT" },
	{ 928021010, POWER_DSM_LEVEL_FAULT, "DSM_USB",  "C385_TBOX_REG_INFO", "CONTENT" },
	{ 928021011, POWER_DSM_LEVEL_FAULT, "DSM_USB",  "C385_TBOX_UNREG_INFO", "CONTENT" },
	{ 928021012, POWER_DSM_LEVEL_FAULT, "DSM_USB",  "SWITCH_TO_HOST_MODE_INFO", "CONTENT" },
	{ 928021013, POWER_DSM_LEVEL_FAULT, "DSM_USB",  "SWITCH_TO_DEVICE_MODE_INFO", "CONTENT" },
	{ 936000102, POWER_DSM_LEVEL_STATISTIC, "DSM_USB",  "DP_IMONITOR_BASIC_INFO_NO", "CONTENT" },
	{ 936000103, POWER_DSM_LEVEL_STATISTIC, "DSM_USB",  "DP_IMONITOR_TIME_INFO_NO", "CONTENT" },
	{ 936000104, POWER_DSM_LEVEL_STATISTIC, "DSM_USB",  "DP_IMONITOR_EXTEND_INFO_NO", "CONTENT" },
	{ 936000105, POWER_DSM_LEVEL_STATISTIC, "DSM_USB",  "DP_IMONITOR_HPD_NO", "CONTENT" },
	{ 936000106, POWER_DSM_LEVEL_STATISTIC, "DSM_USB",  "DP_IMONITOR_LINK_TRAINING_NO", "CONTENT" },
	{ 936000107, POWER_DSM_LEVEL_STATISTIC, "DSM_USB",  "DP_IMONITOR_HOTPLUG_NO", "CONTENT" },
	{ 936000108, POWER_DSM_LEVEL_STATISTIC, "DSM_USB",  "DP_IMONITOR_HDCP_NO", "CONTENT" },
	{ 920004200, POWER_DSM_LEVEL_STATISTIC, "DSM_USB",  "DSM_PD_TYPEC_VBUS_VALID", "CONTENT" },
	{ 920004201, POWER_DSM_LEVEL_FAULT, "DSM_USB",  "DSM_PD_TYPEC_VBUS_STILL_VALID", "CONTENT" },
	{ 920004105, POWER_DSM_LEVEL_STATISTIC, "DSM_USB",  "CONTINUOUS_STOP_CHARGING", "CONTENT" },
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
#endif /* CONFIG_HUAWEI_OHOS_DSM */

