/*
 * hw_pd_reverse_sc.c
 *
 * Source file of reverse supercharge of huawei PD driver
 *
 * Copyright (C) 2024 HUAWEI, Inc.
 * Author: HUAWEI, Inc.
 *
 * This package is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/delay.h>
#include <linux/kernel.h>
#include <linux/notifier.h>
#include <linux/usb.h>

#include <chipset_common/hwusb/hw_pd/hw_pd_ops.h>
#include <chipset_common/hwusb/hw_pd/hw_pd_state.h>
#include <chipset_common/hwusb/hw_pd/hw_pd_reverse_sc.h>
#include <chipset_common/hwpower/reverse_charge/reverse_charge_protocol.h>
#include <chipset_common/hwpower/common_module/power_common_macro.h>
#include <huawei_platform/log/hw_log.h>

#ifndef HWLOG_TAG
#define HWLOG_TAG hw_pd_reverse_sc
HWLOG_REGIST();
#endif /* HWLOG_TAG */

static struct hw_pd_reverse_sc_info reverse_sc_info;

static void reverse_sc_attach_work(struct work_struct *work)
{
	int i;

	hwlog_info("%s start\n", __func__);
	reverse_sc_info.reverse_sc_exit = false;
	for (i = 0; i < REVERSE_SC_DETECTED_TIMES; i++) {
		if (reverse_sc_info.reverse_sc_exit) {
			hwlog_info("%s:detect exit\r\n", __func__);
			reverse_sc_info.reverse_sc_detect_result = 0;
			return;
		}

		if (hw_pd_get_rev_sc_dev_flag()) {
			hwlog_info("%s:pd dev detect success\n", __func__);
			reverse_sc_info.reverse_sc_detect_result |= REVERSE_SC_PD_DEV_DETECTED;
		}

		/* 1: bc1.2 detect succ */
		if (rprot_get_dev_det_result() == 1) {
			hwlog_info("%s: bc1.2 detect success\n", __func__);
			reverse_sc_info.reverse_sc_detect_result |= REVERSE_SC_BC12_DETECTED;
		}

		if ((reverse_sc_info.reverse_sc_detect_result & REVERSE_SC_PD_DEV_DETECTED) &&
			(reverse_sc_info.reverse_sc_detect_result & REVERSE_SC_BC12_DETECTED))
				break;

		msleep(50); /* wait 50 ms for reverse sc dev detect */
	}
	rprot_enable_sleep(1); /* 1: enable mcu sleep */
	hwlog_info("%s end\n", __func__);
}

static bool is_usb_hub_device(struct usb_device *udev)
{
	u8 nintf;
	struct usb_interface_cache *intfc = NULL;
	struct usb_host_interface *alt = NULL;
	struct usb_host_config *config = NULL;

	hwlog_info("%s\n", __func__);
	if (udev == NULL || udev->descriptor.bNumConfigurations != 1)
		return false;

	config = &udev->config[0];
	if (!config)
		return false;
	nintf = config->desc.bNumInterfaces;
	if (nintf != 1)
		return false;
	intfc = config->intf_cache[0];
	if (!intfc)
		return false;
	alt = &intfc->altsetting[0];
	if (!alt)
		return false;
	return alt->desc.bInterfaceClass == USB_CLASS_HUB;
}

static int usb_reverse_notifier_call(struct notifier_block *nb,
	unsigned long action, void *data)
{
	struct usb_device *udev = data;
	bool is_hub = false;

	hwlog_info("%s: rchg super device detect\n", __func__);
	if (!udev || (action != USB_DEVICE_ADD) || (reverse_sc_info.reverse_sc_dev_vid != 0)
		|| !hw_pd_is_rev_sc_plugin())
		return NOTIFY_DONE;
	if ((udev->parent != NULL) && (udev->parent->parent == NULL))
		is_hub = is_usb_hub_device(udev);

	if (is_hub) {
		reverse_sc_info.reverse_sc_dev_vid = REVERSE_SC_ERR_VID;
	} else {
		reverse_sc_info.reverse_sc_dev_vid = le16_to_cpu(udev->descriptor.idVendor);
		reverse_sc_info.reverse_sc_dev_pid = le16_to_cpu(udev->descriptor.idProduct);
	}
	hwlog_info("%s vid:%x pid:%x\n", __func__, reverse_sc_info.reverse_sc_dev_vid,
		reverse_sc_info.reverse_sc_dev_pid);

	if ((reverse_sc_info.reverse_sc_detect_result & REVERSE_SC_PD_DEV_DETECTED) &&
		reverse_sc_info.reverse_sc_dev_vid == REVERSE_SC_HUAWEI_DEV_VID) {
		if (reverse_sc_info.reverse_sc_dev_pid == REVERSE_SC_HUAWEI_QCOM_DEV_PID ||
			(reverse_sc_info.reverse_sc_dev_pid == REVERSE_SC_HUAWEI_HISI_DEV_PID &&
			reverse_sc_info.reverse_sc_detect_result & REVERSE_SC_BC12_DETECTED)) {
			reverse_sc_info.reverse_sc_mode = REVERSE_SC_MODE_SUPER;
			hw_pd_set_rev_sc_switch_flag(REVERSE_SC_MODE_SUPER);
		}
	}

	return NOTIFY_OK;
}

struct notifier_block usb_reverse_nb = {
	.notifier_call = usb_reverse_notifier_call,
};

void hw_pd_reverse_sc_init(void)
{
	INIT_DELAYED_WORK(&reverse_sc_info.reverse_sc_attach_work, reverse_sc_attach_work);
	usb_register_notify(&usb_reverse_nb);
}

void hw_pd_reverse_sc_state_clear(void)
{
	hwlog_info("%s set reverse sc default\n", __func__);
	hw_pd_set_rev_sc_dev_flag(false);
	if (hw_pd_is_rev_sc_plugout()) {
		reverse_sc_info.reverse_sc_detect_result = 0;
		reverse_sc_info.reverse_sc_dev_vid = 0;
		reverse_sc_info.reverse_sc_dev_pid = 0;
	}
	reverse_sc_info.reverse_sc_exit = true;
	cancel_delayed_work_sync(&reverse_sc_info.reverse_sc_attach_work);
}

ssize_t reverse_sc_mode_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	if (!pd_state(support_reverse_sc))
		return -EINVAL;

	hwlog_info("%s = %d\n", __func__, reverse_sc_info.reverse_sc_mode);
	return scnprintf(buf, PAGE_SIZE, "%d\n", reverse_sc_info.reverse_sc_mode);
}

ssize_t reverse_sc_mode_store(struct device *dev, struct device_attribute *attr,
	const char *buf, size_t count)
{
	long val = 0;

	if (!pd_state(support_reverse_sc))
		return -EINVAL;

	hwlog_info("%s:%s", __func__, buf);
	if (kstrtol(buf, POWER_BASE_DEC, &val) < 0) {
		hwlog_err("reverse sc mode val invalid\n");
		return -EINVAL;
	}

	reverse_sc_info.reverse_sc_mode = (int)val;
	hw_pd_set_rev_sc_switch_flag(reverse_sc_info.reverse_sc_mode);
	return count;
}

int reverse_sc_device_detect(void)
{
	hwlog_info("%s\n", __func__);

	if (hw_pd_get_reverse_charge_state())
		// 1: skip usb enumeration
		return 1;

	if (hw_pd_is_rev_sc_plugin())
		schedule_delayed_work(&reverse_sc_info.reverse_sc_attach_work,
			msecs_to_jiffies(REVERSE_SC_DETECTED_TIME));
	else
		rprot_enable_sleep(1); /* 1: enable mcu sleep */

	return 0;
}

