/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: hifi usb driver for HIUSB
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
#include "wrapper.h"

#include <linux/mutex.h>
#include <linux/printk.h>
#include <linux/device.h>
#include <linux/usb.h>
#include <linux/platform_drivers/usb/hiusb_phy.h>
#include <linux/platform_drivers/usb/hiusb.h>
#include <linux/jiffies.h>

#ifdef CONFIG_TCPC_CLASS
#include <huawei_platform/usb/hw_pd_dev.h>
#endif

#undef pr_fmt
#define pr_fmt(fmt) "[HIUSB_HIFI]%s: " fmt, __func__
#define POWER_OFF_DURATION 300

enum hiusb_proxy_state {
	HIUSB_PROXY_NONE,
	HIUSB_PROXY_INITING,
	HIUSB_PROXY_INITDONE,
};

struct hiusb_proxy_manager {
	int	current_portid;
	struct mutex proxy_mutex;
	enum hiusb_proxy_state state;
};

struct hiusb_proxy_manager g_hiusb_proxy_manager;

int hiusb_proxy_hifi_init(void)
{
	mutex_init(&g_hiusb_proxy_manager.proxy_mutex);
	g_hiusb_proxy_manager.current_portid = -ENODEV;
	g_hiusb_proxy_manager.state = HIUSB_PROXY_NONE;
	return 0;
}

static void set_proxy_state(enum hiusb_proxy_state state)
{
	g_hiusb_proxy_manager.state = state;
}

static enum hiusb_proxy_state get_proxy_state(void)
{
	return g_hiusb_proxy_manager.state;
}

static void set_proxy_portid(int portid)
{
	g_hiusb_proxy_manager.current_portid = portid;
}

static int get_proxy_portid(void)
{
	return g_hiusb_proxy_manager.current_portid;
}

void hiusb_hifi_reset_state(void)
{
	set_proxy_state(HIUSB_PROXY_NONE);
	set_proxy_portid(-ENODEV);
}

void hiusb_hifi_lock(void)
{
	mutex_lock(&g_hiusb_proxy_manager.proxy_mutex);
}

void hiusb_hifi_unlock(void)
{
	mutex_unlock(&g_hiusb_proxy_manager.proxy_mutex);
}

#ifdef CONFIG_USB_PROXY_HCD
bool is_hifi_usb_in_used(void)
{
	if (get_proxy_state() == HIUSB_PROXY_INITDONE)
		return true;

	return false;
}
#endif

static int hiusb_port_stop_client_for_hibernate(struct hiusb_port *port)
{
	if (!port)
		return -EINVAL;

	if (!port->cur_client)
		return -ENODEV;

	hiusb_port_ioctrl(port, HIUSB_CLIENT_IOCTRL_STOP_HIFI, NULL);

	port->cur_client = NULL;
	return 0;
}

static int hiusb_port_stop_hifi_usb(struct hiusb_port *port,
					union hiusb_event_data *data, bool reset_power)
{
	int ret = -ESRCH;
	unsigned long start_jiffies;
	unsigned long end_jiffies;
	unsigned int duration;

	pr_info("+\n");

	switch (port->usb_state) {
	case USB_STATE_HIFI_USB:
		if(!port->hiusb_host_wakelock_disable)
			hiusb_port_wake_lock(port);
	#ifdef CONFIG_TCPC_CLASS
		hiusb_pd_dpm_wakelock_ctrl(PD_WAKE_LOCK);
	#endif
		ret = hiusb_port_stop_client(port);
		if (ret)
			pr_err("hiusb_port_stop_client failed\n");

		if (reset_power) {
			start_jiffies = jiffies;
			hiusb_port_set_vbus_power(port, 0);
			hiusb_port_notify_pd_vbus_ctrl(port, CHARGER_TYPE_NONE);
			end_jiffies = jiffies;
			if (time_after(end_jiffies, start_jiffies))
				duration = jiffies_to_msecs(end_jiffies - start_jiffies);
			else
				duration = jiffies_to_msecs(LONG_MAX - start_jiffies + end_jiffies);
			if (duration < POWER_OFF_DURATION)
				msleep(POWER_OFF_DURATION - duration);
		}

		if (port->hifi_ip_first == 0)
			msleep(1500);

		hiusb_usb2phy_setmode(port->usb2_phy, PHY_MODE_USB_HOST);
		hiusb_port_phy_prepare(port, port->last_event_state.cable, port->last_event_state.prop);
		ret = hiusb_port_switch_client_by_type(port, HIUSB_CLIENT_TYPE_HOST);
		if (ret) {
			pr_err("hiusb_port_switch_client_by_type to host failed\n");
			return ret;
		}

		if (reset_power) {
			hiusb_port_set_vbus_power(port, 1);
			hiusb_port_notify_pd_vbus_ctrl(port, PLEASE_PROVIDE_POWER);
		}

		hiusb_set_port_state(port, USB_STATE_HOST);
		set_proxy_state(HIUSB_PROXY_NONE);
		set_proxy_portid(-ENODEV);
		pr_info("port %d usb state: HIFI_USB -> HOST\n", port->portid);
		break;
	case USB_STATE_HIFI_USB_HIBERNATE:
		hiusb_port_wake_lock(port);
	#ifdef CONFIG_TCPC_CLASS
		hiusb_pd_dpm_wakelock_ctrl(PD_WAKE_LOCK);
	#endif
		ret =hiusb_port_stop_client_for_hibernate(port);
		if (ret) {
			pr_err("hiusb_port_stop_client_for_hibernate failed\n");
			return ret;
		}

		hiusb_usb2phy_setmode(port->usb2_phy, PHY_MODE_USB_HOST);
		hiusb_port_phy_prepare(port, port->last_event_state.cable, port->last_event_state.prop);
		ret = hiusb_port_switch_client_by_type(port, HIUSB_CLIENT_TYPE_HOST);
		if (ret) {
			pr_err("hiusb_port_switch_client_by_type failed\n");
			return ret;
		}

		hiusb_set_port_state(port, USB_STATE_HOST);
		set_proxy_state(HIUSB_PROXY_NONE);
		set_proxy_portid(-ENODEV);

		pr_info("port %d usb state: HIFI_USB_HIBERNATE -> HOST\n", port->portid);
		break;
	default:
		pr_info("port %d STOP_HIFI_USB in state %d\n", port->portid, port->usb_state);
		break;
	}
	pr_info("-\n");
	return ret;
}

static int hiusb_port_start_hifi_usb(struct hiusb_port *port,
					union hiusb_event_data *data, bool reset_power)
{
	int ret = -ESRCH;

	pr_info("+\n");

	if(HIUSB_PROXY_NONE != get_proxy_state()) {
		pr_info("hifi port is %d,cur port state is %d\n", get_proxy_portid(), port->usb_state);
		return handle_start_ap_usb_event(port, data->state.cable, data->state.prop);
	}

	switch (port->usb_state) {
	case USB_STATE_OFF:
		hiusb_port_set_vbus_power(port, 1);
		hiusb_usb2phy_setmode(port->usb2_phy, PHY_MODE_USB_HOST);
		ret = hiusb_port_switch_client_by_type(port, HIUSB_CLIENT_TYPE_HIFI);
		if (ret) {
			pr_err("hiusb_start_hifiusb failed %d\n", ret);
			return handle_start_ap_usb_event(port, data->state.cable, data->state.prop);
		}

		set_proxy_state(HIUSB_PROXY_INITDONE);
		set_proxy_portid(port ->portid);

		hiusb_set_port_state(port, USB_STATE_HIFI_USB);
		if(!port->hiusb_host_wakelock_disable)
			hiusb_port_wake_unlock(port);
	#ifdef CONFIG_TCPC_CLASS
		hiusb_pd_dpm_wakelock_ctrl(PD_WAKE_UNLOCK);
	#endif
		pr_info("port %d usb state: OFF -> HIFI_USB\n", port->portid);
		break;

	case USB_STATE_HOST:
		if (reset_power) {
			hiusb_port_set_vbus_power(port, 0);
			hiusb_port_notify_pd_vbus_ctrl(port, CHARGER_TYPE_NONE);
		}

		ret = hiusb_port_stop_client(port);
		if (ret) {
			pr_err("hiusb_port_stop_client failed %d\n", ret);
			return ret;
		}

		if (reset_power) {
			hiusb_port_set_vbus_power(port, 1);
			hiusb_port_notify_pd_vbus_ctrl(port, PLEASE_PROVIDE_POWER);
		}

		hiusb_usb2phy_setmode(port->usb2_phy, PHY_MODE_USB_HOST);
		ret = hiusb_port_start_client(port, HIUSB_CLIENT_TYPE_HIFI);
		if (ret) {
			pr_err("hiusb_start_hifiusb failed %d\n", ret);
			hiusb_port_phy_prepare(port, data->state.cable, data->state.prop);
			ret = hiusb_port_start_client(port, HIUSB_CLIENT_TYPE_HOST);
			if (ret) {
				pr_err("hiusb_start_host failed %d\n", ret);
			} else {
				hiusb_set_port_state(port, USB_STATE_HOST);
				if(!port->hiusb_host_wakelock_disable)
					hiusb_port_wake_lock(port);
				pr_info("port %d usb_status: HOST -> HOST\n", port->portid);
			}
			return ret;
		}

		set_proxy_state(HIUSB_PROXY_INITDONE);
		set_proxy_portid(port ->portid);

		hiusb_set_port_state(port, USB_STATE_HIFI_USB);
		if(!port->hiusb_host_wakelock_disable)
			hiusb_port_wake_unlock(port);
	#ifdef CONFIG_TCPC_CLASS
		hiusb_pd_dpm_wakelock_ctrl(PD_WAKE_UNLOCK);
	#endif
		pr_info("port %d usb state: HOST -> HIFI_USB\n", port->portid);
		break;
	default:
		pr_info("port %d START_HIFI_USB in state %d\n", port->portid, port->usb_state);
		break;
	}

	pr_info("-\n");
	return ret;
}

static int hiusb_port_set_hifi_usb(struct hiusb_port *port,
					union hiusb_event_data *data, bool reset_power)
{
	int ret;
	struct hiusb_port_state state;

	pr_info("+\n");
	state = *(struct hiusb_port_state *)data;

	mutex_lock(&g_hiusb_proxy_manager.proxy_mutex);

	if (state.cable.state.usb == HIUSB_CABLE_STATE_IN) {
		ret = hiusb_port_start_hifi_usb(port, data, reset_power);
		if(ret)
			pr_err("hiusb_port_start_hifi_usb failed %d\n", ret);
	} else {
		if(HIUSB_PROXY_INITDONE != get_proxy_state()){
			pr_err("hifi port is %d,cur port state is %d\n", get_proxy_portid(), port->usb_state);
			ret = -EINVAL;
			goto unlock;
		}

		if(port->portid != get_proxy_portid())
		{
			pr_err("port portid %d != proxy portid %d\n", port->portid , get_proxy_portid());
			ret = -EINVAL;
			goto unlock;
		}

		ret = hiusb_port_stop_hifi_usb(port, data, reset_power);
		if (ret)
			pr_err("hiusb_port_stop_hifi_usb failed %d\n", ret);
	}

unlock:
	mutex_unlock(&g_hiusb_proxy_manager.proxy_mutex);
	pr_info("-\n");
	return ret;
}

int hiusb_port_process_set_hifi_usb_reset_power(struct hiusb_port *port,
					struct hiusb_event *event)
{
	return hiusb_port_set_hifi_usb(port, &event->data, true);
}

int hiusb_port_process_set_hifi_usb(struct hiusb_port *port,
					struct hiusb_event *event)
{
	return hiusb_port_set_hifi_usb(port, &event->data, false);
}

int hiusb_port_process_set_hifi_hibernate(struct hiusb_port *port,
					struct hiusb_event *event)
{
	int ret = -ESRCH;
	mutex_lock(&g_hiusb_proxy_manager.proxy_mutex);
	switch (port->usb_state) {
	case USB_STATE_HIFI_USB:
		ret = hiusb_port_hibernate(port);
		if(ret) {
			pr_err("hiusb_port_client_hibernate failed %d\n", ret);
			goto unlock;
		}
		hiusb_set_port_state(port, USB_STATE_HIFI_USB_HIBERNATE);
		pr_info("port %d usb state: HIFI_USB -> HIFI_USB_HIBERNATE\n", port->portid);
		msleep(50); /* debounce of suspend state */
		break;
	default:
		pr_info("HIFI_USB_HIBERNATE in state %d\n", port->usb_state);
		break;
	}
unlock:
	mutex_unlock(&g_hiusb_proxy_manager.proxy_mutex);
	return ret;
}

int hiusb_port_process_set_hifi_wakeup(struct hiusb_port *port,
					struct hiusb_event *event)
{
	int ret = -ESRCH;

	mutex_lock(&g_hiusb_proxy_manager.proxy_mutex);
	switch (port->usb_state) {
	case USB_STATE_HIFI_USB_HIBERNATE:
		hiusb_port_wake_lock(port);
		ret = hiusb_port_wakeup(port);
		if(ret) {
			pr_err("hiusb_port_client_hibernate failed %d\n", ret);
			goto unlock;
		}

		hiusb_set_port_state(port, USB_STATE_HIFI_USB);
		hiusb_port_wake_unlock(port);
		pr_info("port %d usb state: HIFI_USB_HIBERNATE -> HIFI_USB\n", port->portid);
		break;
	default:
		pr_info("HIFI_USB_WAKEUP in state %d\n", port->usb_state);
		hiusb_plug_atomic_notifier_call_chain(HIUSB_PROP_WAKEUP_CALLBACK, NULL);
		break;
	}
unlock:
	mutex_unlock(&g_hiusb_proxy_manager.proxy_mutex);
	return ret;
}

/*
 * Caution: this function must be called in lock
 * Currently, this function called only by usb_resume
 */
int hiusb_wakeup_hifi_usb(int portid)
{
	int ret = 0;
	struct hiusb_port *port = NULL;

	mutex_lock(&g_hiusb_proxy_manager.proxy_mutex);

	if(portid != get_proxy_portid())
	{
		pr_err("portid %d %d\n", portid, get_proxy_portid());
		ret = -EINVAL;
		goto unlock;
	}

	port = hiusb_get_port_by_id(portid);
	if (port == NULL) {
		pr_err("hiusb get port failed, portid %d\n", portid);
		ret = -ENODEV;
		goto unlock;
	}

	switch (port->usb_state) {
	case USB_STATE_HIFI_USB_HIBERNATE:
		ret = hiusb_port_wakeup(port);
		if(ret) {
			pr_err("hiusb_port_client_hibernate failed %d\n", ret);
			goto unlock;
		}

		hiusb_set_port_state(port, USB_STATE_HIFI_USB);
		pr_info("usb state: HIFI_USB_HIBERNATE -> HIFI_USB\n");
		break;
	default:
		pr_info("HIFI_USB_WAKEUP in state %d\n", port->usb_state);
		break;
	}
unlock:
	mutex_unlock(&g_hiusb_proxy_manager.proxy_mutex);
	return ret;
}
