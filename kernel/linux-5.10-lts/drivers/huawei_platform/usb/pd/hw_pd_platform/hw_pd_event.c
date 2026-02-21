/*
 * hw_pd_event.c
 *
 * Source file of platform event reporting for huawei PD driver
 *
 * Copyright (C) 2023 HUAWEI, Inc.
 * Author: HUAWEI, Inc.
 *
 * This package is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/fb.h>

#include <platform_include/basicplatform/linux/power/platform/coul/coul_drv.h>
#include <platform_include/basicplatform/linux/power/platform/bci_battery.h>
#include <platform_include/basicplatform/linux/mfd/pmic_mntn.h>
#include <chipset_common/hwpower/common_module/power_event_ne.h>
#include <huawei_platform/log/hw_log.h>
#include <huawei_platform/usb/hw_pd/hw_pd_event.h>
#include <linux/hisi/usb/chip_usb.h>

#include <securec.h>
#ifdef CONFIG_DKMD_DKSM
#include <platform_include/display/dkmd/dkmd_notify.h>
#include <chipset_common/hwusb/hw_pd/hw_pd_state.h>
#endif

#ifndef HWLOG_TAG
#define HWLOG_TAG huawei_pd_event
HWLOG_REGIST();
#endif /* HWLOG_TAG */

#define LDO_NAME_SIZE 16
#define LDO_NAME_NUM  32

static struct notifier_block ocp_nb;
static struct notifier_block usb_nb;
static struct notifier_block wired_rchg_nb;

static char g_ldo_name[LDO_NAME_NUM] = {0};
static hw_pd_event_cb g_ocp_cb = NULL;
static hw_pd_event_cb g_fb_cb = NULL;
static hw_pd_event_cb g_bc12_cb = NULL;
static hw_pd_event_cb g_wired_rchg_cb = NULL;

#ifdef CONFIG_DKMD_DKSM
static int hw_pd_handle_dkmd_event(struct notifier_block *self,
	unsigned long event, void *data)
{
	struct dkmd_event *dkmd_event = (struct dkmd_event *)data;
	int blank;
 
	hwlog_info("%s dkmd event %u\n", __func__, event);
	if (!g_fb_cb || !dkmd_event) {
		return NOTIFY_DONE;
	}
 
	blank = dkmd_event->value;
 
	hwlog_err("%s: event=%lu, blank=%d\n", __func__, event, blank);
	if ((event == DKMD_EVENT_BLANK) && (blank == FB_BLANK_UNBLANK))
		g_fb_cb(HW_PD_EVENT_TYPE_FB, (char *)(&blank));
 
	return NOTIFY_DONE;
}
#endif

static int hw_pd_handle_fb_event(struct notifier_block *self,
	unsigned long event, void *data)
{
	struct fb_event *fb_event = data;
	int *blank = NULL;

	hwlog_info("%s fb event %u\n", __func__, event);

	if (!g_fb_cb || !fb_event)
		return NOTIFY_DONE;

	blank = fb_event->data;
	if (!blank)
		return NOTIFY_DONE;

	if ((event == FB_EVENT_BLANK) && (*blank == FB_BLANK_UNBLANK))
		g_fb_cb(HW_PD_EVENT_TYPE_FB, (char *)blank);

	return NOTIFY_DONE;
}

#ifdef CONFIG_DKMD_DKSM
static struct notifier_block hw_pd_handle_dkmd_notifier = {
	.notifier_call = hw_pd_handle_dkmd_event,
};
#endif
static struct notifier_block hw_pd_handle_fb_notifier = {
	.notifier_call = hw_pd_handle_fb_event,
};

/* response pd_ocp event */
static int hw_pd_ocp_nb_call(struct notifier_block *ocp_nb, unsigned long event, void *data)
{
	hwlog_info("%s ocp event %s\n", __func__, (data ? (char *)data : "NA"));

	if (!data)
		return 0;

	if (strlen(g_ldo_name) && strncmp(g_ldo_name, (char *)data, (LDO_NAME_SIZE - 1)))
		return 0;

	if (g_ocp_cb)
		g_ocp_cb(HW_PD_EVENT_TYPE_OCP, NULL);

	return 0;
}

/* response bc1.2 event */
static int hw_pd_report_bc12_cb(struct notifier_block *usb_nb, unsigned long event, void *data)
{
	struct bc12_event be = {event, data};

	hwlog_info("%s bc12 event %u\n", __func__, event);
	g_bc12_cb(HW_PD_EVENT_TYPE_BC12, (char *)&be);
	return 0;
}

/* response wired_rchg event */
static int hw_pd_wired_rchg_nb_call(struct notifier_block *wired_rchg_nb, unsigned long event, void *data)
{
	struct wired_rchg_event rchge = {event, data};

	hwlog_info("%s wired_rchg event %u\n", __func__, event);
	g_wired_rchg_cb(HW_PD_EVENT_TYPE_RCHG, (char *)&rchge);

	return 0;
}

int hw_pd_event_reg(unsigned int type, const char *context, hw_pd_event_cb cb)
{
	int ret = -EIO;

	hwlog_info("%s reg type %d\n", __func__, type);

	if (!cb) {
		hwlog_info("%s invalid arg\n", __func__);
		return -EIO;
	}

	switch (type) {
	case HW_PD_EVENT_TYPE_OCP :
		if (context) {
			ret = strcpy_s(g_ldo_name, sizeof(g_ldo_name), context);
			if (ret != EVENT_OK)
				hwlog_err("%s copy err\n", __func__);
			hwlog_info("%s reg ldo %s for ocp\n", __func__, g_ldo_name);
		}
		g_ocp_cb = cb;
		ocp_nb.notifier_call = hw_pd_ocp_nb_call;
		pmic_mntn_register_notifier(&ocp_nb);
		ret = 0;
		break;
	case HW_PD_EVENT_TYPE_FB :
		g_fb_cb = cb;
#ifdef CONFIG_DKMD_DKSM
		if (pd_state(support_fold_screen) != 0)
			dkmd_register_client(&hw_pd_handle_dkmd_notifier);
		else
			fb_register_client(&hw_pd_handle_fb_notifier);
#else
		fb_register_client(&hw_pd_handle_fb_notifier);
#endif
		break;
	case HW_PD_EVENT_TYPE_BC12 :
		g_bc12_cb = cb;
		usb_nb.notifier_call = hw_pd_report_bc12_cb;
		ret = chip_charger_type_notifier_register(&usb_nb);
		if (ret < 0)
			hwlog_err("chip_charger_type_notifier_register failed\n");
		break;
	case HW_PD_EVENT_TYPE_RCHG :
		g_wired_rchg_cb = cb;
		wired_rchg_nb.notifier_call = hw_pd_wired_rchg_nb_call;
		ret = power_event_bnc_register(POWER_BNT_REVERSE_CHARGE, &wired_rchg_nb);
		if (ret)
			hwlog_err("%s register POWER_BNT_REVERSE_CHARGE failed\n", __func__);
	default :
		hwlog_err("%s invalid type\n", __func__);
		break;
	}

	return ret;
}
