/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: core driver for HIUSB
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
#include "debugfs.h"

#include <linux/platform_drivers/usb/hiusb.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/pm_runtime.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/platform_drivers/usb/hiusb_phy.h>
#include <linux/platform_drivers/usb/chip_usb_helper.h>
#include <platform_include/basicplatform/linux/mfd/pmic_platform.h>
#include <linux/platform_drivers/usb/usb_reg_cfg.h>
#include <platform_include/cee/linux/lpm_suspend_state_vote.h>
#include <doze.h>
#ifdef CONFIG_SYS_PM_ULSR
#include <platform_include/cee/linux/ulsr.h>
#endif

#undef pr_fmt
#define pr_fmt(fmt) "[HIUSB_CORE]%s: " fmt, __func__

static DEFINE_SPINLOCK(g_port_lock);
static LIST_HEAD(g_port_handle_list);
struct hiusb_core g_hiusb_core;

void hiusb_port_set_hifi_ip_first(struct hiusb_port *port, int flag)
{
	if (!port)
		return;

	port->hifi_ip_first = (unsigned int)flag;
}

enum usb_state hiusb_port_get_usb_state(struct hiusb_port *port)
{
	if (port->is_fpga_flag) {
		pr_debug("this is fpga platform, usb is device mode\n");
		return USB_STATE_DEVICE;
	}

	if (pmic_get_vbus_status() == 0)
		return USB_STATE_OFF;
	else
		return USB_STATE_DEVICE;
}

bool hiusb_port_state_is_host(struct hiusb_port *port)
{
	if (!port)
		return false;

	if (port && (port->usb_state == USB_STATE_HOST))
		return true;
	else
		return false;
}

int hiusb_is_fpga(void)
{
	return !!g_hiusb_core.fpga_flag;
}

unsigned int hiusb_get_soc_spec_efuse(void)
{
	return g_hiusb_core.soc_spec_efuse;
}

static const char * const charger_type_array[] = {
	[CHARGER_TYPE_SDP]     = "sdp",       /* Standard Downstreame Port */
	[CHARGER_TYPE_CDP]     = "cdp",       /* Charging Downstreame Port */
	[CHARGER_TYPE_DCP]     = "dcp",       /* Dedicate Charging Port */
	[CHARGER_TYPE_UNKNOWN] = "unknown",   /* non-standard */
	[CHARGER_TYPE_NONE]    = "none",      /* not connected */
	[PLEASE_PROVIDE_POWER] = "provide"   /* host mode, provide power */
};

const char *hiusb_charger_type_string(enum chip_charger_type type)
{
	if (type >= CHARGER_TYPE_ILLEGAL)
		return "illegal charger";

	return charger_type_array[type];
}

enum chip_charger_type chip_get_charger_type(void)
{
	int portid;
	struct hiusb_port *port = NULL;

	portid = hiusb_get_default_portid();
	if (portid < 0 || portid >= HIUSB_PORT_MAXSIZE) {
		pr_info("%s: invalid default portid %d.\n", __func__, portid);
		return CHARGER_TYPE_NONE;
	}

	port = hiusb_get_port_by_id(portid);
	if (port == NULL) {
		pr_info("%s: get port failed, portid %d.\n", __func__, portid);
		return CHARGER_TYPE_NONE;
	}

	pr_info("%s %d\n", __func__, port->charger_type);
	return port->charger_type;
}
EXPORT_SYMBOL_GPL(chip_get_charger_type);

bool chip_usb_state_is_host(void)
{
	int portid;
	struct hiusb_port *port = NULL;

	portid = hiusb_get_default_portid();
	if (portid < 0 || portid >= HIUSB_PORT_MAXSIZE) {
		pr_info("%s: invalid default portid %d.\n", __func__, portid);
		return false;
	}

	port = hiusb_get_port_by_id(portid);
	if (port == NULL) {
		pr_info("%s: get port failed, portid %d.\n", __func__, portid);
		return false;
	}

	return port->usb_state == USB_STATE_HOST;
}
EXPORT_SYMBOL_GPL(chip_usb_state_is_host);

void chip_usb_set_hifi_ip_first(int flag)
{
	int portid;
	struct hiusb_port *port = NULL;

	portid = hiusb_get_default_portid();
	if (portid < 0 || portid >= HIUSB_PORT_MAXSIZE) {
		pr_info("%s: invalid default portid %d.\n", __func__, portid);
		return;
	}

	port = hiusb_get_port_by_id(portid);
	if (port == NULL) {
		pr_info("%s: get port failed, portid %d.\n", __func__, portid);
		return;
	}

	pr_info("%s flag %d\n", __func__, flag);
	port->hifi_ip_first = (unsigned int)flag;
}
EXPORT_SYMBOL_GPL(chip_usb_set_hifi_ip_first);

void hiusb_set_s3_power_down_flag(bool s3_power_down)
{
	pr_info("%s: set s3 power down flag, s3_power_down:%d->%d\n", __func__, g_hiusb_core.s3_power_down, s3_power_down);
	g_hiusb_core.s3_power_down = s3_power_down;
	return;
}
EXPORT_SYMBOL_GPL(hiusb_set_s3_power_down_flag);

bool hiusb_get_s3_power_down_flag(void)
{
	return g_hiusb_core.s3_power_down;
}
EXPORT_SYMBOL_GPL(hiusb_get_s3_power_down_flag);

int hiusb_usb20_phy_calibrate(struct usb_device *udev)
{
	int ret;
	struct hiusb_port *port = NULL;

	if (udev == NULL) {
		pr_info("%s: udev is NULL.\n", __func__);
		return -ENODEV;
	}

	port = udev_to_port(udev);
	if (port == NULL) {
		pr_info("%s: port is NULL.\n", __func__);
		return -ENODEV;
	}

	ret = hiusb_usb2phy_calibrate(port->usb2_phy);
	if (ret != 0) {
		pr_info("%s: usb2phy calibrate failed %d.\n", __func__, ret);
		return ret;
	}

	return 0;
}
EXPORT_SYMBOL_GPL(hiusb_usb20_phy_calibrate);

int hiusb_entry_device_mode(int portid)
{
	pr_info("+\n");
	mutex_lock(&g_hiusb_core.device_mode_mutex);
	if (g_hiusb_core.device_portid >= 0) {
		pr_info("already device mode,current device portid %d\n", g_hiusb_core.device_portid);
		mutex_unlock(&g_hiusb_core.device_mode_mutex);
		return -EBUSY;
	}

	g_hiusb_core.device_portid = portid;
	mutex_unlock(&g_hiusb_core.device_mode_mutex);
	pr_info("set device portid %d\n", portid);
	pr_info("-\n");
	return 0;
}

void hiusb_exit_device_mode(int portid)
{
	pr_info("set device_portid from %d to %d \n",g_hiusb_core.device_portid, portid);
	mutex_lock(&g_hiusb_core.device_mode_mutex);
	if(g_hiusb_core.device_portid == portid)
		g_hiusb_core.device_portid = -1;
	mutex_unlock(&g_hiusb_core.device_mode_mutex);
}

int hiusb_get_device_id(void)
{
	pr_info("device portid %d\n", g_hiusb_core.device_portid);
	return g_hiusb_core.device_portid;
}

int hiusb_get_default_portid(void)
{
	return g_hiusb_core.default_portid;
}

int hiusb_set_default_portid(int portid)
{
	return g_hiusb_core.default_portid = portid;
}

bool hiusb_is_other_port_enumerate(int portid)
{
	bool ret = true;
	mutex_lock(&g_hiusb_core.device_mode_mutex);
	if(g_hiusb_core.device_portid < 0) {
		ret = false;
		goto unlock;
	}

	if(g_hiusb_core.device_portid == portid)
		ret = false;
unlock:
	mutex_unlock(&g_hiusb_core.device_mode_mutex);
	return ret;
}

static int hisusb_work_off(struct hiusb_port *port)
{
	int    state;

	state = port->usb_state;
	if ((state != USB_STATE_HIFI_USB_HIBERNATE) && (state != USB_STATE_OFF))
		return 1;
	return 0;
}

void hiusb_set_port_state(struct hiusb_port *port, enum usb_state state)
{
	port->usb_state = state;
	if (state == USB_STATE_HIFI_USB_HIBERNATE || state == USB_STATE_OFF) {
		if (hiusb_for_each_port(hisusb_work_off) == 0) {
			pr_info("usb enable doze\n");
			(void)vote_enable_doze(DOZE_CLIENT_USB);
		}
	} else {
		pr_info("usb disable doze\n");
		(void)vote_disable_doze(DOZE_CLIENT_USB);
	}
	return;
}

bool hiusb_port_is_device_mode(struct hiusb_port *port)
{
	return g_hiusb_core.device_portid == port->portid;
}

/* Must be called in g_port_lock */
static int port_handle_bind_port(struct hiusb_port *port)
{
	struct hiusb_port_handle *iter = NULL;
	pr_info("+\n");

	list_for_each_entry(iter, &g_port_handle_list, list) {
		if (iter->port_fwnode == port->fwnode) {
			hiusb_port_get(port);
			pr_info("bind port %pK\n", port);
			iter->data = (void *)port;
		}
	}
	pr_info("-\n");

	return 0;
}

/* Must be called in g_port_lock */
static int port_handle_unbind_port(struct hiusb_port *port)
{
	struct hiusb_port_handle *iter = NULL;

	list_for_each_entry(iter, &g_port_handle_list, list) {
		if (iter->port_fwnode == port->fwnode) {
			hiusb_port_put(port);
			pr_info("unbind port %pK\n", port);
			iter->data = NULL;
		}
	}

	return 0;
}

/* Must be called in g_port_lock */
struct hiusb_port_handle* get_port_handle_by_port(struct hiusb_port *port)
{
	unsigned long flags;
	struct hiusb_port_handle *iter = NULL;

	pr_info("+\n");

	if (port == NULL) {
		pr_err("pointer port is NULL\n");
		return NULL;
	}

	spin_lock_irqsave(&g_port_lock, flags);
	list_for_each_entry(iter, &g_port_handle_list, list) {
		if (iter->port_fwnode == port->fwnode) {
			spin_unlock_irqrestore(&g_port_lock, flags);
			return iter;
		}
	}
	spin_unlock_irqrestore(&g_port_lock, flags);

	pr_info("-\n");
	return NULL;
}

/* It should be a single-port platform, return first port_handle */
struct hiusb_port_handle* get_default_port_handle(void)
{
	unsigned long flags;
	struct hiusb_port_handle *iter = NULL;

	pr_info("+\n");

	spin_lock_irqsave(&g_port_lock, flags);
	list_for_each_entry(iter, &g_port_handle_list, list) {
		spin_unlock_irqrestore(&g_port_lock, flags);
		return iter;
	}
	spin_unlock_irqrestore(&g_port_lock, flags);

	pr_info("-\n");
	return NULL;
}

bool hiusb_check_default_port_state_is_off(void)
{
	struct hiusb_port *port = NULL;

	if (!g_hiusb_core.is_single_port)
		return false;

	port = hiusb_get_port_by_id(HIUSB_SINGLE_PORT_NUM);
	if (port == NULL) {
		pr_info("%s get port 0 failed\n", __func__);
		return false;
	}

	return port->usb_state == USB_STATE_OFF;
}

static int handle_device_plugin_event(struct hiusb_port *port,
			union hiusb_cable cable, union hiusb_prop prop)
{
	int ret = -ESRCH;
	if (port->usb_state == USB_STATE_DEVICE) {
		pr_info("Already in device mode, do nothing\n");
	} else if (port->usb_state == USB_STATE_OFF) {
		hiusb_port_phy_prepare(port, cable, prop);
		ret = hiusb_port_switch_client_by_type(port, HIUSB_CLIENT_TYPE_DEVICE);
		if(ret) {
			pr_err("hiusb_port_switch_client_by_type failed %d\n", ret);
			return ret;
		}

		if(!hiusb_port_enumerate_allowed(port)) {
			pr_info("it need notify USB_CONNECT_DCP while a real charger connected\n");
			port->speed = USB_CONNECT_DCP;
			if (!queue_work(system_power_efficient_wq, &port->speed_change_work))
				pr_err("schedule speed_change_work wait:%d\n", port->speed);
		}
		hiusb_set_port_state(port, USB_STATE_DEVICE);
		if(hiusb_port_sleep_allowed(port)) {
			hiusb_port_wake_unlock(port);
		} else {
			hiusb_port_wake_lock(port);
		}
		pr_info("port %d usb status: OFF -> DEVICE\n", port->portid);
	} else if (port->usb_state == USB_STATE_HOST) {
		pr_info("Charger connect intrrupt in HOST mode\n");
	} else if (port->usb_state == USB_STATE_HIFI_USB) {
		pr_info("vbus power in hifi usb state\n");
	} else {
		pr_info("can not handle charger connect event in mode %s\n",
				chip_usb_state_string(port->usb_state));
	}
	return ret;
}

static int handle_device_plugout_event(struct hiusb_port *port)
{
	int ret = -ESRCH;
	if (port->usb_state == USB_STATE_OFF) {
		pr_info("Already in off mode, do nothing\n");
	} else if (port->usb_state == USB_STATE_DEVICE) {
		ret = hiusb_port_stop_client(port);
		if (ret) {
			pr_err("hiusb_port_stop_client failed %d\n", ret);
			return ret;
		}
		hiusb_set_port_state(port, USB_STATE_OFF);

		hiusb_port_wake_unlock(port);
		pr_info("port %d usb status: DEVICE -> OFF\n", port->portid);
	} else if (port->usb_state == USB_STATE_HOST) {
		pr_info("Charger disconnect intrrupt in HOST mode\n");
	} else if (port->usb_state == USB_STATE_HIFI_USB) {
		pr_info("vbus disconnect event in hifi usb state\n");
	} else {
		pr_info("can not handle charger disconnect event in mode %s\n",
				chip_usb_state_string(port->usb_state));
	}
	return ret;
}

int handle_start_ap_usb_event(struct hiusb_port *port, union hiusb_cable cable, union hiusb_prop prop)
{
	int ret = -ESRCH;
	switch (port->usb_state) {
	case USB_STATE_OFF:
		hiusb_port_set_vbus_power(port, 1);
		hiusb_port_phy_prepare(port, cable, prop);
		ret = hiusb_port_switch_client_by_type(port, HIUSB_CLIENT_TYPE_HOST);
		if(ret) {
			pr_err("hiusb_port_switch_client_by_type host  failed %d\n", ret);
			hiusb_port_set_vbus_power(port, 0);
		}

		hiusb_set_port_state(port, USB_STATE_HOST);
		if(!port->hiusb_host_wakelock_disable)
			hiusb_port_wake_lock(port);
		pr_info("port %d usb_status: OFF -> HOST\n", port->portid);
		break;
	default:
		pr_info("handle_start_ap_usb_event in state %d\n", port->usb_state);
		break;
	}
	return ret;
}

static int handle_usb_use_hifi_ip_first(struct hiusb_port *port)
{
	return ((port->hifi_ip_first != 0)
				&& (get_hifi_usb_retry_count() == 0)
				&& (get_never_hifi_usb_value() == 0)
				&& (get_usbaudio_nv_is_ready() == 0));
}

static int handle_host_plugin_event(struct hiusb_port *port,
					union hiusb_cable cable, union hiusb_prop prop)
{
	int ret;
	struct hiusb_event event = {0};

	pr_info("+\n");

	if ((handle_usb_use_hifi_ip_first(port)) &&
		(port->last_event_state.prop.state.lane_num == HIUSB_PROP_DP_0LANE)) {
		pr_info("use hifi ip first\n");
		event.data.state.cable = cable;
		event.data.state.prop = prop;
		ret = hiusb_port_process_set_hifi_usb(port, &event);
		port->time_stamps.start_hifiusb_complete_time_stamp = jiffies;
	} else {
		pr_info("use arm ip first\n");
		ret = handle_start_ap_usb_event(port, cable, prop);
		port->time_stamps.start_host_complete_time_stamp = jiffies;
	}

	pr_info("-\n");
	return ret;
}
static int hiusb_port_hifi_stop(struct hiusb_port *port)
{
	int ret;
	hiusb_port_set_vbus_power(port, 0);
	ret = hiusb_port_stop_client(port);
	if (ret)
		pr_err("hiusb_port_stop_client failed %d\n", ret);

	hiusb_set_port_state(port, USB_STATE_OFF);
	hiusb_port_wake_unlock(port);
	return ret;
}
static int handle_host_plugout_event(struct hiusb_port *port)
{
	int ret = -ESRCH;
	pr_info("+\n");

	hiusb_hifi_lock();
	switch (port->usb_state) {
	case USB_STATE_HOST:
		hiusb_port_set_vbus_power(port, 0);
		ret = hiusb_port_stop_client(port);
		if (ret)
			pr_err("hiusb_port_stop_client failed %d\n", ret);

		hiusb_set_port_state(port, USB_STATE_OFF);
		if(!port->hiusb_host_wakelock_disable)
			hiusb_port_wake_unlock(port);
		pr_info("port %d hiusb_status: HOST -> OFF\n", port->portid);
		hiusb_plug_atomic_notifier_call_chain(HIUSB_PROP_HOST_PLUG_OUT, NULL);
		port->time_stamps.stop_host_complete_time_stamp = jiffies;
		break;
	case USB_STATE_HIFI_USB:
		ret = hiusb_port_hifi_stop(port);
		pr_info("port %d usb state: HIFI_USB -> OFF\n", port->portid);
		hiusb_plug_atomic_notifier_call_chain(HIUSB_PROP_HOST_PLUG_OUT, NULL);
		hiusb_hifi_reset_state();
		port->time_stamps.stop_hifiusb_complete_time_stamp = jiffies;
		break;
	case USB_STATE_HIFI_USB_HIBERNATE:
		/* phy was closed in this state */
		ret = hiusb_port_hifi_stop(port);
		pr_info("port %d usb state: HIFI_USB_HIBERNATE -> OFF\n", port->portid);
		hiusb_plug_atomic_notifier_call_chain(HIUSB_PROP_HOST_PLUG_OUT, NULL);
		hiusb_hifi_reset_state();
		break;
	default:
		pr_info("port %d ID_RISE_EVENT in state %d\n", port->portid, port->usb_state);
		break;
	}

	hiusb_hifi_unlock();
	pr_info("-\n");
	return ret;
}

static int _process_usb_state(struct hiusb_port *port,
			      union hiusb_cable cable, union hiusb_prop prop)
{
	int ret;
	pr_info("+\n");
	pr_info("usb_role %d usb %d\n", prop.state.usb_role, cable.state.usb);

	if (cable.state.usb == HIUSB_CABLE_STATE_IN) {
		if (prop.state.usb_role == HIUSB_PROP_USB_DEVICE) {
			ret = handle_device_plugin_event(port, cable, prop);
			port->time_stamps.start_device_complete_time_stamp = jiffies;
		} else {
			ret = handle_host_plugin_event(port, cable, prop);
		}
	} else {
		if (prop.state.usb_role == HIUSB_PROP_USB_DEVICE) {
			ret = handle_device_plugout_event(port);
		} else {
			ret = handle_host_plugout_event(port);
		}
	}

	if (ret)
		pr_err("process usb state failed %d\n", ret);

	chip_usb_state_notify(port->usb_state, port);
	pr_info("-\n");
	return ret;
}

static int _process_dp_state(struct hiusb_port *port,
				struct hiusb_event *event)
{
	int ret;
	union hiusb_cable cable = event->data.state.cable;
	union hiusb_prop prop = event->data.state.prop;

	if (cable.state.dp == HIUSB_CABLE_STATE_IN)
		ret = hiusb_port_set_dp_hpd(port, prop.state.hpd == HIUSB_PROP_DP_HPD_IN);
	else
		ret = hiusb_port_set_dp_hpd(port, false);

	if (ret)
		pr_err("set dp hpd failed %d\n", ret);

	return hiusb_port_dp_trigger(port, &event->data.state);
}

void hiusb_port_phy_prepare(struct hiusb_port *port, union hiusb_cable cable, union hiusb_prop prop)
{
	if (prop.state.orien)
		hiusb_usb3phy_set_orien(port->usb3_phy, HIUSB3_PHY_ORIEN_FLIP);
	else
		hiusb_usb3phy_set_orien(port->usb3_phy, HIUSB3_PHY_ORIEN_NORMAL);

	/* set phy mode before switch port mode */
	if (prop.state.lane_num == HIUSB_PROP_DP_0LANE)
		hiusb_usb3phy_set_dpmode(port->usb3_phy, HIUSB3_PHY_DP_0LANE, prop.state.usb_role);
	else if (prop.state.lane_num == HIUSB_PROP_DP_2LANE)
		hiusb_usb3phy_set_dpmode(port->usb3_phy, HIUSB3_PHY_DP_2LANE, prop.state.usb_role);
	else
		hiusb_usb3phy_set_dpmode(port->usb3_phy, HIUSB3_PHY_DP_4LANE, prop.state.usb_role);

	if (prop.state.usb_role == HIUSB_PROP_USB_HOST)
		hiusb_usb2phy_setmode(port->usb2_phy, PHY_MODE_USB_HOST);
	else if (prop.state.usb_role == HIUSB_PROP_USB_DEVICE)
		hiusb_usb2phy_setmode(port->usb2_phy, PHY_MODE_USB_DEVICE);
	else
		pr_err("invalid usb role\n");
}

static int hiusb_port_process_set_state(struct hiusb_port *port,
					struct hiusb_event *event)
{
	union hiusb_cable cable;
	union hiusb_prop prop;
	int ret = -EFAULT;

	pr_info("+\n");

	if (IS_ERR_OR_NULL(port) || !event)
		return -EINVAL;

	cable = event->data.state.cable;
	prop = event->data.state.prop;

	if (prop.state.usb_role != HIUSB_PROP_USB_OFF) {
		ret = _process_usb_state(port, cable, prop);
		if (ret) {
			pr_err("process usb state fail %d\n",ret);
			goto out;
		}
	} else {
		ret = _process_dp_state(port, event);
		if (ret) {
			pr_err("process dp state fail %d\n",ret);
			goto out;
		}
	}
out:
	hiusb_port_stop_event_timer(port);
	hiusb_port_notify_dp_done(port, hiusb_get_mode_type(&event->data.state), ret);

	pr_info("-\n");
	return ret;
}

static int hiusb_queue_event(struct hiusb_port *port,
				union hiusb_event_data *data,
				hiusb_process_event_t process_event,
				enum hiusb_event_data_type data_type)
{
	struct hiusb_event event = {0};
	unsigned long flags;
	int ret;

	pr_info("+\n");
	spin_lock_irqsave(&g_port_lock, flags);
	hiusb_port_get(port);
	spin_unlock_irqrestore(&g_port_lock, flags);

	event.data = *data;
	event.process_event = process_event;
	ret = hiusb_port_queue_event(port, &event);
	if(ret != 0)
		hiusb_port_put(port);

	pr_info("-\n");
	return ret;
}

int hiusb_start_hifi_hibernate(int portid)
{
	int ret;
	struct hiusb_port *port = NULL;
	struct hiusb_event event = {0};
	struct hiusb_port_state state = {0};
	pr_info("+\n");

	port = hiusb_get_port_by_id(portid);
	if(IS_ERR_OR_NULL(port))
		return -ENODEV;

	state = port->last_event_state;
	state.cable.state.usb = HIUSB_CABLE_STATE_IN;
	state.prop.state.usb_role = HIUSB_PROP_USB_HOST;
	state.prop.state.hifi_state = HIUSB_PROP_HIFI_HIBERNATE;

	event.process_event = hiusb_port_process_set_hifi_hibernate;
	event.data.state = state;
	ret = hiusb_port_queue_event(port, &event);
	if(ret != 0)
		hiusb_port_put(port);

	pr_info("-\n");
	return ret;
}

int hiusb_start_hifi_wakeup(int portid)
{
	int ret;
	struct hiusb_port *port = NULL;
	struct hiusb_event event = {0};
	struct hiusb_port_state state = {0};
	pr_info("+\n");

	port = hiusb_get_port_by_id(portid);
	if(IS_ERR_OR_NULL(port))
		return -ENODEV;

	if (port->usb_state != USB_STATE_HIFI_USB_HIBERNATE)
		return -EINVAL;

	state = port->last_event_state;
	state.cable.state.usb = HIUSB_CABLE_STATE_IN;
	state.prop.state.usb_role = HIUSB_PROP_USB_HOST;
	state.prop.state.hifi_state = HIUSB_PROP_HIFI_WAKEUP;

	event.process_event = hiusb_port_process_set_hifi_wakeup;
	event.data.state = state;
	ret = hiusb_port_queue_event(port, &event);
	if(ret != 0)
		hiusb_port_put(port);

	pr_info("-\n");
	return ret;
}

void hiusb_stop_hifi_usb(int portid, bool reset_power)
{
	int ret;
	struct hiusb_port *port = NULL;
	struct hiusb_event event = {0};
	struct hiusb_port_state state = {0};
	pr_info("+\n");

	port = hiusb_get_port_by_id(portid);
	if(IS_ERR_OR_NULL(port)) {
		pr_err("port %d find port id fail", portid);
		return;
	}

	state = port->last_event_state;
	state.cable.state.usb = HIUSB_CABLE_STATE_OUT;
	state.prop.state.usb_role = HIUSB_PROP_USB_HOST;

	if(reset_power) {
		event.process_event = hiusb_port_process_set_hifi_usb_reset_power;
		state.prop.state.hifi_state = HIUSB_PROP_HIFI_STOP_RESET_VBUS;
	}
	else {
		event.process_event = hiusb_port_process_set_hifi_usb;
		state.prop.state.hifi_state = HIUSB_PROP_HIFI_STOP;
	}

	event.data.state = state;
	ret = hiusb_port_queue_event(port, &event);
	if(ret != 0)
		hiusb_port_put(port);

	pr_info("-\n");
	return;
}

int hiusb_start_hifi_usb(int portid, bool reset_power)
{
	int ret;
	struct hiusb_port *port = NULL;
	struct hiusb_event event = {0};
	struct hiusb_port_state state = {0};
	pr_info("+\n");

	port = hiusb_get_port_by_id(portid);
	if(IS_ERR_OR_NULL(port))
		return -ENODEV;

	state = port->last_event_state;
	state.cable.state.usb = HIUSB_CABLE_STATE_IN;
	state.prop.state.usb_role = HIUSB_PROP_USB_HOST;

	if(reset_power) {
		event.process_event = hiusb_port_process_set_hifi_usb_reset_power;
		state.prop.state.hifi_state = HIUSB_PROP_HIFI_START_RESET_VBUS;
	}
	else {
		event.process_event = hiusb_port_process_set_hifi_usb;
		state.prop.state.hifi_state = HIUSB_PROP_HIFI_START;
	}

	event.data.state = state;
	ret = hiusb_port_queue_event(port, &event);
	if(ret != 0)
		hiusb_port_put(port);

	pr_info("-\n");
	return ret;
}

int hiusb_set_state(struct hiusb_port *port, struct hiusb_port_state state)
{
	union hiusb_event_data data;

	if (!port)
		return -EINVAL;

	data.state = state;
	return hiusb_queue_event(port, &data, hiusb_port_process_set_state, EVENT_DATA_PORT_STATE);
}

enum usb_state hiusb_get_state(const struct hiusb_port_handle *handle)
{
	unsigned long flags;
	struct hiusb_port *port = NULL;
	pr_info("+\n");

	spin_lock_irqsave(&g_port_lock, flags);
	if (!handle->data) {
		spin_unlock_irqrestore(&g_port_lock, flags);
		return USB_STATE_UNKNOWN;
	}
	port = handle->data;
	spin_unlock_irqrestore(&g_port_lock, flags);

	pr_info("-\n");
	return port->usb_state;
}

int hiusb_host_start(struct hiusb_port *port)
{
	int ret;
	struct hiusb_port_state state = {0};

	state.cable.state.dp = HIUSB_CABLE_STATE_OUT;
	state.cable.state.usb = HIUSB_CABLE_STATE_IN;
	state.cable.state.vbus = HIUSB_CABLE_STATE_IN;
	state.prop.state.usb_role = HIUSB_PROP_USB_HOST;
	state.prop.state.power_role = HIUSB_PROP_FEILD_VBUS_INPUT;
	state.prop.state.orien = 0;
	state.prop.state.lane_num = HIUSB_PROP_DP_2LANE;
	state.prop.state.hpd = HIUSB_PROP_DP_HPD_IN;
	ret = hiusb_set_state(port, state);
	pr_err("ret %d\n", ret);
	return ret;
}

int hiusb_host_stop(struct hiusb_port *port)
{
	struct hiusb_port_state state = {0};

	state.cable.state.dp = HIUSB_CABLE_STATE_OUT;
	state.cable.state.usb = HIUSB_CABLE_STATE_OUT;
	state.cable.state.vbus = HIUSB_CABLE_STATE_OUT;
	state.prop.state.usb_role = HIUSB_PROP_USB_HOST;
	state.prop.state.power_role = HIUSB_PROP_FEILD_VBUS_INPUT;
	state.prop.state.orien = 0;
	state.prop.state.lane_num = HIUSB_PROP_DP_2LANE;
	state.prop.state.hpd = HIUSB_PROP_DP_HPD_OUT;
	return hiusb_set_state(port, state);
}

int hiusb_device_start(struct hiusb_port *port)
{
	int ret;
	struct hiusb_port_state state = {0};
	state.cable.state.dp = HIUSB_CABLE_STATE_OUT;
	state.cable.state.usb = HIUSB_CABLE_STATE_IN;
	state.cable.state.vbus = HIUSB_CABLE_STATE_IN;
	state.prop.state.usb_role = HIUSB_PROP_USB_DEVICE;
	state.prop.state.power_role = HIUSB_PROP_FEILD_VBUS_INPUT;
	state.prop.state.orien = 0;
	state.prop.state.lane_num = HIUSB_PROP_DP_2LANE;
	state.prop.state.hpd = HIUSB_PROP_DP_HPD_IN;
	ret = hiusb_set_state(port, state);
	pr_err("ret %d\n", ret);
	return ret;
}

int hiusb_device_stop(struct hiusb_port *port)
{
	struct hiusb_port_state state = {0};
	state.cable.state.dp = HIUSB_CABLE_STATE_OUT;
	state.cable.state.usb = HIUSB_CABLE_STATE_OUT;
	state.cable.state.vbus = HIUSB_CABLE_STATE_OUT;
	state.prop.state.usb_role = HIUSB_PROP_USB_DEVICE;
	state.prop.state.power_role = HIUSB_PROP_FEILD_VBUS_INPUT;
	state.prop.state.orien = 0;
	state.prop.state.lane_num = HIUSB_PROP_DP_2LANE;
	state.prop.state.hpd = HIUSB_PROP_DP_HPD_OUT;
	return hiusb_set_state(port, state);
}

int hiusb_dp_in(struct hiusb_port *port)
{
	struct hiusb_port_state state = {0};
	state.cable.state.dp = HIUSB_CABLE_STATE_IN;
	state.cable.state.usb = HIUSB_CABLE_STATE_OUT;
	state.cable.state.vbus = HIUSB_CABLE_STATE_OUT;
	state.prop.state.usb_role = HIUSB_PROP_USB_OFF;
	state.prop.state.power_role = HIUSB_PROP_FEILD_VBUS_INPUT;
	state.prop.state.orien = 0;
	state.prop.state.lane_num = HIUSB_PROP_DP_2LANE;
	state.prop.state.hpd = HIUSB_PROP_DP_HPD_IN;
	return hiusb_set_state(port, state);
}

int hiusb_dp_out(struct hiusb_port *port)
{
	struct hiusb_port_state state = {0};
	state.cable.state.dp = HIUSB_CABLE_STATE_OUT;
	state.cable.state.usb = HIUSB_CABLE_STATE_OUT;
	state.cable.state.vbus = HIUSB_CABLE_STATE_OUT;
	state.prop.state.usb_role = HIUSB_PROP_USB_OFF;
	state.prop.state.power_role = HIUSB_PROP_FEILD_VBUS_INPUT;
	state.prop.state.orien = 0;
	state.prop.state.lane_num = HIUSB_PROP_DP_2LANE;
	state.prop.state.hpd = HIUSB_PROP_DP_HPD_OUT;
	return hiusb_set_state(port, state);
}

int hiusb_runtime_enable(struct device *dev)
{
	int ret;
	pm_runtime_set_active(dev);
	pm_runtime_enable(dev);
	pm_runtime_no_callbacks(dev);
	ret = pm_runtime_get_sync(dev);
	if (ret < 0) {
		pr_err("pm_runtime_get_sync failed %d\n", ret);
		return ret;
	}

	pm_runtime_forbid(dev);
	return 0;
}

static int hiusb_port_process_set_prop(struct hiusb_port *port,
				       struct hiusb_event *event)
{
	int ret;

	if (IS_ERR_OR_NULL(port) || !event)
		return -EINVAL;

	ret = _process_dp_state(port, event);
	if (ret)
		return ret;

	return 0;
}

int hiusb_set_prop(const struct hiusb_port_handle *handle,
		   struct hiusb_port_state state)
{
	union hiusb_event_data data;
	if (!handle)
		return -EINVAL;

	data.state = state;
	return hiusb_queue_event(handle->data, &data, hiusb_port_process_set_prop, EVENT_DATA_PORT_STATE);
}

static int process_switch_client_by_type(struct hiusb_port *port,
						struct hiusb_event *event)
{
	if (IS_ERR_OR_NULL(port) || !event)
		return -EINVAL;

	return hiusb_port_switch_client_by_type(port, event->data.type);
}

int hiusb_switch_client_by_type(const struct hiusb_port_handle *handle, u8 type)
{
	union hiusb_event_data data;
	if (!handle)
		return -EINVAL;

	data.type = type;
	return hiusb_queue_event(handle->data, &data, process_switch_client_by_type, EVENT_DATA_PORT_TYPE);
}

static int process_switch_client_by_name(struct hiusb_port *port,
						struct hiusb_event *event)
{
	if (IS_ERR_OR_NULL(port) || !event)
		return -EINVAL;

	return hiusb_port_switch_client_by_name(port, event->data.name);
}

int hiusb_switch_client_by_name(const struct hiusb_port_handle *handle, char *name)
{
	union hiusb_event_data data;
	if (!handle)
		return -EINVAL;

	data.name = name;
	return hiusb_queue_event(handle->data, &data, process_switch_client_by_name, EVENT_DATA_PORT_NAME);
}

/**
 * hiusb_port_handle_create_by_dev - Create hiusb_port_handle by fwnode of device.
 *
 * @device:	device form which the hiusb_port associated.
 *
 * Return NULL or ERR_PTR on failure.
 */
struct hiusb_port_handle *
hiusb_port_handle_create_by_dev(struct device *dev)
{
	struct fwnode_handle *ep_node = NULL;
	struct fwnode_handle *port = NULL;
	struct hiusb_port_handle *handle = NULL;
	unsigned long flags;
	pr_info("+\n");

	if (!dev || !dev_fwnode(dev))
		return ERR_PTR(-EINVAL);

	fwnode_graph_for_each_endpoint(dev_fwnode(dev), ep_node) {
		port = fwnode_graph_get_remote_port(ep_node);
		if (port)
			pr_err("port %d\n", fwnode_property_present(port, "hiusb-port"));

		if (!port || !fwnode_property_present(port, "hiusb-port"))
				continue;

		handle = kzalloc(sizeof(*handle), GFP_KERNEL);
		if (!handle)
			return ERR_PTR(-ENOMEM);

		INIT_LIST_HEAD(&handle->list);
		handle->port_fwnode = port;
		pr_info("handle->port_fwnode %pK\n", handle->port_fwnode);

		spin_lock_irqsave(&g_port_lock, flags);
		handle->data = (void *)fwnode_hiusb_port_get(handle->port_fwnode);
		pr_info("handle->data %pK\n", handle->data);
		list_add(&handle->list, &g_port_handle_list);
		spin_unlock_irqrestore(&g_port_lock, flags);

		fwnode_handle_put(port);

		pr_info("-\n");
		return handle;
	}

	return NULL;
}

void hiusb_port_handle_destroy(struct hiusb_port_handle *handle)
{
	unsigned long flags;
	struct hiusb_port *port = NULL;
	pr_info("+\n");

	if (!handle)
		return;

	port = handle->data;
	spin_lock_irqsave(&g_port_lock, flags);
	list_del(&handle->list);
	hiusb_port_put(port);
	spin_unlock_irqrestore(&g_port_lock, flags);
	kfree(handle);
	pr_info("-\n");
}

static int hiusb_destroy_port_fn(struct hiusb_port *port)
{
	hiusb_port_destroy(port);
	return 0;
}

static void hiusb_destroy_ports(void)
{
	unsigned long flags;
	int ret;

	spin_lock_irqsave(&g_port_lock, flags);
	ret = hiusb_for_each_port(port_handle_unbind_port);
	spin_unlock_irqrestore(&g_port_lock, flags);
	if (ret)
		pr_err("port_handle unbind port failed\n");

	ret = hiusb_for_each_port(hiusb_destroy_port_fn);
	if (ret)
		pr_err("port destroy failed\n");
}

static int hiusb_create_ports(struct device *dev)
{
	struct fwnode_handle *port_node = NULL;
	struct fwnode_handle *ports_node = NULL;
	struct device_node *port_devicenode = NULL;
	struct hiusb_port *port = NULL;
	unsigned long flags;
	int ret;
	pr_info("+\n");

	if (!dev_fwnode(dev))
		return -ENODEV;

	ports_node = fwnode_get_named_child_node(dev_fwnode(dev), "ports");
	if(!ports_node)
		return -ENODEV;

	port_node = fwnode_get_named_child_node(ports_node, "port");
	of_node_put(to_of_node(ports_node));
	if(!port_node)
		return -ENODEV;

	while (port_node != NULL) {
		port = hiusb_port_create(dev, port_node);
		if (IS_ERR(port)) {
			pr_err("create port failed %ld\n", PTR_ERR(port));
			goto destroy_ports;
		}

		do {
			port_node = fwnode_get_next_child_node(ports_node, port_node);
			if (!port_node)
				break;
			port_devicenode = to_of_node(port_node);
		} while (of_node_cmp(port_devicenode->name, "port"));
	}

	spin_lock_irqsave(&g_port_lock, flags);
	ret = hiusb_for_each_port(port_handle_bind_port);
	spin_unlock_irqrestore(&g_port_lock, flags);
	if (ret) {
		pr_err("port_handle bind port failed\n");
		goto destroy_ports;
	}
	pr_info("-\n");
	return ret;

destroy_ports:
	hiusb_destroy_ports();
	return PTR_ERR(port);
}

static int hiusb_get_clk_resource(struct device *dev)
{
	if (g_hiusb_core.stub_clk || g_hiusb_core.dis_serdes_clk)
		return 0;

	return devm_chip_usb_get_clks(dev, &g_hiusb_core.clks, &g_hiusb_core.num_clocks);
}

static int hiusb_get_resource(struct device *dev)
{
	g_hiusb_core.is_single_port = of_property_read_bool(dev->of_node, "single_port");
	g_hiusb_core.dis_serdes_clk = of_property_read_bool(dev->of_node, "dis_serdes_clk");
	g_hiusb_core.stub_clk = of_property_read_bool(dev->of_node, "stub-clk");
	g_hiusb_core.usb_s3_lpm3_volt = of_property_read_bool(dev->of_node, "usb_s3_lpm3_volt");
	(void)hiusb_get_clk_resource(dev);

	pr_info("hiusb num_clocks %d\n", g_hiusb_core.num_clocks);

	g_hiusb_core.apb_bridge_reset = of_get_chip_usb_reg_cfg(dev->of_node, "apb_bridge_reset");
	if (!g_hiusb_core.apb_bridge_reset)
		pr_info("platform no need apb_bridge_reset\n");

	g_hiusb_core.apb_bridge_unreset = of_get_chip_usb_reg_cfg(dev->of_node, "apb_bridge_unreset");
	if (!g_hiusb_core.apb_bridge_unreset)
		pr_info("platform no need apb_bridge_unreset\n");

	if (of_property_read_u32(dev->of_node, "soc_spec_efuse", &g_hiusb_core.soc_spec_efuse))
		pr_info("platform no soc_spec_efuse\n");

	return 0;
}

static void hiusb_disable_serdes_clk(struct device *dev)
{
	int ret;
	struct clk *serdes_clk = NULL;

	if (!g_hiusb_core.dis_serdes_clk)
		return;

	serdes_clk = devm_clk_get(dev, "clk_gate_abb_192");
	if (IS_ERR_OR_NULL(serdes_clk)) {
		pr_err("failed to get clk_gate_abb_192\n");
		return;
	}

	ret = clk_prepare_enable(serdes_clk);
	if (ret) {
		pr_err("failed to enable clk_gate_abb_192 ret %d\n", ret);
		return;
	}

	clk_disable_unprepare(serdes_clk);
	pr_info("hiusb disable serdes clk finish.\n");
}

static int hiusb_apb_bridge_config(void)
{
	int ret;

	if (!g_hiusb_core.stub_clk && g_hiusb_core.num_clocks) {
		ret = chip_usb_init_clks(g_hiusb_core.clks, g_hiusb_core.num_clocks);
		if (ret) {
			pr_err("config clk failed\n");
			return ret;
		}
	}

	if (g_hiusb_core.apb_bridge_unreset) {
		pr_info("config apb bridge unreset\n");
		ret = chip_usb_reg_write(g_hiusb_core.apb_bridge_unreset);
		if (ret) {
			pr_err("config apb bridge unreset failed\n");
			goto clk_shutdown;
		}
	}

	return 0;

clk_shutdown:
	chip_usb_shutdown_clks(g_hiusb_core.clks, g_hiusb_core.num_clocks);
	return ret;
}

static int hiusb_apb_bridge_reset(void)
{
	int ret = 0;

	if (g_hiusb_core.apb_bridge_reset) {
		pr_info("config apb bridge reset\n");
		ret = chip_usb_reg_write(g_hiusb_core.apb_bridge_reset);
		if (ret)
			pr_err("config apb bridge reset failed\n");
	}

	chip_usb_shutdown_clks(g_hiusb_core.clks, g_hiusb_core.num_clocks);
	return ret;
}

static void hiusb_put_resource(struct device *dev)
{
	if (g_hiusb_core.apb_bridge_unreset)
		of_remove_chip_usb_reg_cfg(g_hiusb_core.apb_bridge_unreset);

	if (g_hiusb_core.apb_bridge_reset)
		of_remove_chip_usb_reg_cfg(g_hiusb_core.apb_bridge_reset);
}

static int hiusb_port_mode_init_fn(struct hiusb_port *port)
{
	int ret;

	if (port->init_state == USB_STATE_DEVICE) {
		ret = hiusb_device_start(port);
		if (ret) {
			pr_err("set port%d to device failed %d\n", port->portid, ret);
			return ret;
		}
	} else if (port->init_state == USB_STATE_HOST) {
		ret = hiusb_host_start(port);
		if (ret) {
			pr_err("set port%d to host failed %d\n", port->portid, ret);
			return ret;
		}
	}

	return 0;
}

static int hiusb_probe(struct platform_device *pdev)
{
	int ret;
	struct device *dev = &pdev->dev;
	pr_info("+\n");

	BUILD_BUG_ON(sizeof(struct hiusb_event) != HIUSB_EVENT_SIZE);
	g_hiusb_core.device_portid = -1;
	g_hiusb_core.default_portid = -1;
	g_hiusb_core.s3_power_down = false;
	g_hiusb_core.is_single_port = false;
	g_hiusb_core.s3_perifixup_count = 0;
	mutex_init(&g_hiusb_core.device_mode_mutex);

	ret = hiusb_get_resource(dev);
	if (ret) {
		pr_err("hiusb get resource failed %d\n", ret);
		return ret;
	}

	ret = hiusb_apb_bridge_config();
	if (ret) {
		pr_err("hiusb apb brdge config failed %d\n", ret);
		goto put_resource;
	}

	hiusb_disable_serdes_clk(dev);

	ret = hiusb_create_ports(dev);
	if (ret) {
		pr_err("ports create failed %d\n", ret);
		goto apb_bridge_reset;
	}

	ret = hiusb_runtime_enable(dev);
	if (ret < 0) {
		pr_err("hiusb_runtime_enable failed %d\n", ret);
		goto destroy_ports;
	}

	ret = hiusb_proxy_hifi_init();
	if (ret){
		pr_err("hiusb_proxy_hifi_init failed %d\n", ret);
		goto err_pm_put;
	}

	ret = hiusb_core_debugfs_init(dev);
	if (ret)
		goto err_pm_put;

	ret = hiusb_device_register_notifier(&g_hiusb_core);
	if (ret) {
		pr_err("hiusb register notifier failed %d\n", ret);
		goto debugfs_exit;
	}

	hiusb_for_each_port(hiusb_port_mode_init_fn);
	pr_info("-\n");
	return 0;

debugfs_exit:
	hiusb_core_debugfs_exit();
err_pm_put:
	pm_runtime_put_sync(dev);
	pm_runtime_disable(dev);
destroy_ports:
	hiusb_destroy_ports();
apb_bridge_reset:
	hiusb_apb_bridge_reset();
put_resource:
	hiusb_put_resource(dev);

	return ret;
}

static int hiusb_remove(struct platform_device *pdev)
{
	hiusb_device_unregister_notifier(&g_hiusb_core);
	hiusb_destroy_ports();
	mutex_destroy(&g_hiusb_core.device_mode_mutex);
	hiusb_core_debugfs_exit();

	hiusb_put_resource(&pdev->dev);
	pm_runtime_put_sync(&pdev->dev);
	pm_runtime_disable(&pdev->dev);
	return 0;
}

#ifdef CONFIG_OF
static const struct of_device_id of_hiusb_match[] = {
	{
		.compatible = "hisilicon,hiusb"
	},
	{ },
};
MODULE_DEVICE_TABLE(of, of_hiusb_match);
#endif

#ifdef CONFIG_PM_SLEEP
static int hisusb_portstate_count(struct hiusb_port *port)
{
#ifdef CONFIG_SYS_PM_ULSR
	if (is_ulsr())
		return 0;
#endif

	if (port == NULL)
		return 0;

	if (port->usb_state == USB_STATE_HOST || port->usb_state == USB_STATE_HIFI_USB)
		g_hiusb_core.s3_perifixup_count ++;

	return 0;
}

static int hiusb_suspend(struct device *dev)
{
	/* Set Perifix Keep Up */
	if (g_hiusb_core.usb_s3_lpm3_volt) {
		hiusb_for_each_port(hisusb_portstate_count);

		pr_info("s3_perifixup_count 0x%x\n", g_hiusb_core.s3_perifixup_count);
		if (g_hiusb_core.s3_perifixup_count > 0)
			lpm_suspend_state_vote(ID_USB, 1);
		else
			lpm_suspend_state_vote(ID_USB, 0);
	}
	hiusb_apb_bridge_reset();
	return 0;
}

static int hiusb_resume(struct device *dev)
{
	if (g_hiusb_core.usb_s3_lpm3_volt) {
		pr_info("s3_perifixup_count 0x%x\n", g_hiusb_core.s3_perifixup_count);
		g_hiusb_core.s3_perifixup_count = 0;
	}
	hiusb_apb_bridge_config();
	return 0;
}

static int hiusb_freeze(struct device *dev)
{
	hiusb_apb_bridge_reset();
	return 0;
}

static int hiusb_restore(struct device *dev)
{
	hiusb_apb_bridge_config();
	return 0;
}

#endif

const struct dev_pm_ops hiusb_core_dev_pm_ops = {
#ifdef CONFIG_PM_SLEEP
	.suspend = hiusb_suspend,
	.resume = hiusb_resume,
	.freeze = hiusb_freeze,
	.thaw = hiusb_restore,
	.poweroff = hiusb_freeze,
	.restore = hiusb_restore,
#endif
};

static struct platform_driver hiusb_driver = {
	.probe		= hiusb_probe,
	.remove		= hiusb_remove,
	.driver		= {
		.name = "hiusb",
		.of_match_table = of_match_ptr(of_hiusb_match),
		.pm = &hiusb_core_dev_pm_ops,
	},
};

module_platform_driver(hiusb_driver);

MODULE_DESCRIPTION("HIUSB Driver");
MODULE_LICENSE("GPL v2");
