/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: notify for HIUSB
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2  of
 * the License as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
#include "core.h"
#include <linux/types.h>
#include <linux/usb/ch9.h>
#include <linux/usb.h>
#include <linux/phy/phy.h>
#include <linux/platform_drivers/usb/hiusb_phy.h>
#include <linux/platform_drivers/usb/chip_usb_debug_framework.h>
#include <linux/platform_drivers/usb/hisi_tcpm.h>
#include <linux/hid.h>
#ifdef CONFIG_HUAWEI_EXTERNAL_CAMERA
#include <huawei_platform/camera/external_camera.h>
#endif

#undef pr_fmt
#define pr_fmt(fmt) "[HIUSB_NOTIFY]%s: " fmt, __func__

ATOMIC_NOTIFIER_HEAD(hiusb_device_event_nh);
ATOMIC_NOTIFIER_HEAD(hiusb_plug_event_nh);
static BLOCKING_NOTIFIER_HEAD(hiusb_charger_type_notifier);
static BLOCKING_NOTIFIER_HEAD(usb_state_notifier);

int chip_usb_state_notifier_register(struct notifier_block *nb)
{
	if (!nb)
		return -EINVAL;

	return blocking_notifier_chain_register(&usb_state_notifier, nb);
}

int chip_usb_state_notifier_unregister(struct notifier_block *nb)
{
	if (!nb)
		return -EINVAL;

	return blocking_notifier_chain_unregister(&usb_state_notifier, nb);
}

void chip_usb_state_notify(enum usb_state state, struct hiusb_port *port)
{
	int ret;

	if (!port->notify_state)
		return;

	ret = blocking_notifier_call_chain(&usb_state_notifier, state,
					&(port->portid));
	if (ret)
		pr_err("cb return %d\n", ret);
}

int chip_charger_type_notifier_register(struct notifier_block *nb)
{
	if (!nb)
		return -EINVAL;
	return blocking_notifier_chain_register(&hiusb_charger_type_notifier, nb);
}

int chip_charger_type_notifier_unregister(struct notifier_block *nb)
{
	if (!nb)
		return -EINVAL;
	return blocking_notifier_chain_unregister(&hiusb_charger_type_notifier, nb);
}

static void hiusb_charger_type_notify(enum chip_charger_type charger_type, void *data)
{
	int ret;
	pr_info("%s: charger_type %d\n", __func__, charger_type);
	ret = blocking_notifier_call_chain(&hiusb_charger_type_notifier, charger_type,
			data);
	if (ret)
		pr_err("cb return %d\n", ret);
}

void hiusb_port_notify_charger_type(struct hiusb_port *port)
{
	int portid;
	int i;

	pr_info("%s: type %d\n", __func__, port->charger_type);

	portid = hiusb_get_default_portid();
	if (portid < 0 || portid >= HIUSB_PORT_MAXSIZE) {
		pr_info("%s: no need to notify\n", __func__);
		return;
	}

	if (portid == port->portid) {
		for (i = 0; i < 50; i++) {
			if (!hisi_usb_typec_during_hardreset())
				break;
			msleep(20);
		}

		hiusb_charger_type_notify(port->charger_type, NULL);
	}
}

int hiusb_plug_event_notifier_register(struct notifier_block *nb)
{
	return atomic_notifier_chain_register(&hiusb_plug_event_nh, nb);
}

int hiusb_plug_event_notifier_unregister(struct notifier_block *nb)
{
	return atomic_notifier_chain_unregister(&hiusb_plug_event_nh, nb);
}

int hiusb_plug_atomic_notifier_call_chain(unsigned long val, void *v)
{
	return atomic_notifier_call_chain(&hiusb_plug_event_nh, val, v);
}

int hiusb_device_event_notifier_register(struct notifier_block *nb)
{
	return atomic_notifier_chain_register(&hiusb_device_event_nh, nb);
}

static int hiusb_device_event_notifier_unregister(struct notifier_block *nb)
{
	return atomic_notifier_chain_unregister(&hiusb_device_event_nh, nb);
}

int hiusb_atomic_notifier_call_chain(unsigned long val, void *v)
{
	return atomic_notifier_call_chain(&hiusb_device_event_nh, val, v);
}

static int hiusb_device_event_notifier_fn(struct notifier_block *nb,
					unsigned long event, void *para)
{
	enum usb_device_speed speed;
	unsigned long flags;
	struct hiusb_port *port = NULL;
	struct hiusb_core *hiusb_core = container_of(nb, struct hiusb_core, event_nb);

	pr_info("+, event %lu\n", event);

	port = hiusb_get_port_by_id(hiusb_core->device_portid);
	if (port == NULL)
		return -ENODEV;

	pr_info("portid %d\n", hiusb_core->device_portid);
	switch (event) {
	case DEVICE_EVENT_CONNECT_DONE:
		speed = *(enum usb_device_speed *)para;

		/*
		 * Keep VDP_SRC if speed is USB_SPEED_SUPER
		 * and charger_type is CHARGER_TYPE_CDP.
		 */
		if (port->charger_type == CHARGER_TYPE_CDP &&
				speed == USB_SPEED_SUPER)
			(void)hiusb_port_set_vdp_src(port, true);
		break;
	case DEVICE_EVENT_PULLUP:
		/* Disable VDP_SRC for communicaton on D+ */
		(void)hiusb_port_set_vdp_src(port, false);
		break;
	case DEVICE_EVENT_SETCONFIG:
		speed = *(enum usb_device_speed  *)para;
		spin_lock_irqsave(&port->bc_again_lock, flags);
		if (port->charger_type == CHARGER_TYPE_UNKNOWN)
			port->charger_type = CHARGER_TYPE_SDP;

		port->speed = speed;
		if (!queue_work(system_power_efficient_wq,
				&port->speed_change_work))
			pr_err("schedule speed_change_work wait:%d\n", port->speed);

		spin_unlock_irqrestore(&port->bc_again_lock, flags);
		break;
	default:
		break;
	}

	pr_info("-\n");
	return 0;
}


static int hiusb_xhci_notifier_fn(struct notifier_block *nb,
			unsigned long action, void *data)
{
	struct usb_device *udev = (struct usb_device *)data;
	struct hiusb_port *port = NULL;

	pr_info("+\n");

	if (!udev) {
		pr_info("udev is null,just return\n");
		return 0;
	}

	if ((action == USB_DEVICE_ADD) && (udev->descriptor.bDeviceClass == USB_CLASS_HUB)) {
		pr_info("usb hub don't notify\n");
		return 0;
	}

	if (((action == USB_DEVICE_ADD) || (action == USB_DEVICE_REMOVE))
		&& ((udev->parent != NULL) && (udev->parent->parent == NULL))) {
#ifdef CONFIG_HUAWEI_EXTERNAL_CAMERA
        if (is_renesas_controller(udev)) {
            pr_info("is renesas pcie2usb don't notify\n");
            return 0;
        }
#endif
		pr_info("xhci device speed is %d action %s\n", udev->speed,
			(action == USB_DEVICE_ADD) ? "USB_DEVICE_ADD" : "USB_DEVICE_REMOVE");

		port = udev_to_port(udev);
		if (port == NULL)
			return 0;

		/* only device plug out while phone is host mode,not the usb cable */
		if (action == USB_DEVICE_REMOVE)
			port->speed = USB_CONNECT_HOST;
		else
			port->speed = udev->speed;

		if (action == USB_DEVICE_ADD)
			port->time_stamps.device_add_time_stamp = jiffies;

		pr_info("port%d speed change work ontify speed %d\n", port->portid, port->speed);
		if (!queue_work(system_power_efficient_wq,
				&port->speed_change_work))
			pr_err("schedule speed_change_work wait:%d\n", port->speed);
	}

	pr_info("-\n");
	return 0;
}

static void hiusb_usb3_phy_dump_info(struct hiusb_port *port)
{
	struct hiusb_usb3phy *combophy = NULL;

	pr_info("+\n");

	if (!port->usb3_phy)
		return;

	combophy = phy_get_drvdata(port->usb3_phy);
	if (combophy && combophy->regdump)
		combophy->regdump(combophy);

	pr_info("-\n");
}

static int hiusb_device_event_nb(struct notifier_block *nb,
			unsigned long action, void *data)
{
	struct hiusb_port *port = NULL;
	struct usb_device *udev = (struct usb_device *)data;

	pr_info("+\n");

	if (udev == NULL) {
		pr_info("udev is null,just return\n");
		return 0;
	}

	port = udev_to_port(udev);
	if (port == NULL) {
		pr_info("port is null,just return\n");
		return 0;
	}

	if (action == USB_GADGET_DEVICE_RESET) {
		pr_err("usb device reset\n");
		hiusb_usb3_phy_dump_info(port);
	}

	pr_info("-\n");
	return NOTIFY_OK;
}

int hiusb_device_register_notifier(struct hiusb_core *hiusb_core)
{
	int ret;

	hiusb_core->xhci_nb.notifier_call = hiusb_xhci_notifier_fn;
	usb_register_notify(&hiusb_core->xhci_nb);

	hiusb_core->event_nb.notifier_call = hiusb_device_event_notifier_fn;
	ret = hiusb_device_event_notifier_register(&hiusb_core->event_nb);
	if (ret) {
		usb_unregister_notify(&hiusb_core->xhci_nb);
		hiusb_core->xhci_nb.notifier_call = NULL;
		hiusb_core->event_nb.notifier_call = NULL;
		pr_err("hiusb device event notifier register failed\n");
		return ret;
	}

	hiusb_core->device_event_nb.notifier_call = hiusb_device_event_nb;
	ret = usb_blockerr_register_notify(&hiusb_core->device_event_nb);
	if (ret) {
		usb_unregister_notify(&hiusb_core->xhci_nb);
		hiusb_device_event_notifier_unregister(&hiusb_core->event_nb);
		hiusb_core->xhci_nb.notifier_call = NULL;
		hiusb_core->event_nb.notifier_call = NULL;
		hiusb_core->device_event_nb.notifier_call = NULL;
		pr_err("usb debug event register failed\n");
	}

	return 0;
}

void hiusb_device_unregister_notifier(struct hiusb_core *hiusb_core)
{
	if (hiusb_core->event_nb.notifier_call) {
		hiusb_device_event_notifier_unregister(&hiusb_core->event_nb);
		hiusb_core->event_nb.notifier_call = NULL;
	}

	if (hiusb_core->xhci_nb.notifier_call) {
		usb_unregister_notify(&hiusb_core->xhci_nb);
		hiusb_core->xhci_nb.notifier_call = NULL;
	}

	if (hiusb_core->device_event_nb.notifier_call) {
		usb_unregister_notify(&hiusb_core->device_event_nb);
		hiusb_core->device_event_nb.notifier_call = NULL;
	}
}

