/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: event handle for HIUSB
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

#include <linux/atomic.h>
#include <linux/slab.h>

#undef pr_fmt
#define pr_fmt(fmt) "[HIUSB_EVENT]%s: " fmt, __func__

#define work_to_hiusb_event_manager(w_ptr) \
	container_of(w_ptr, struct hiusb_event_manager, event_work)

static bool hiusb_check_hifi_wakeup_event(struct hiusb_port_state *last_state)
{
	pr_info("check hifi wakeup event\n");
	if (hiusb_hifi_hibernate_event(last_state) ||
	    hiusb_hifi_wakeup_event(last_state))
		return true;

	pr_err("check hifi wakeup event failed\n");
	return false;
}

static bool hiusb_check_hifi_hibernate_event(struct hiusb_port_state *last_state)
{
	pr_info("check hifi hibernate event\n");
	if (hiusb_start_host_event(last_state) ||
	    hiusb_hifi_start_event(last_state) ||
	    hiusb_hifi_start_reset_vbus_event(last_state) ||
	    hiusb_hifi_wakeup_event(last_state))
		return true;

	pr_err("check hifi hibernate event failed\n");
	return false;
}

static bool hiusb_check_hifi_stop_event(struct hiusb_port_state *last_state)
{
	pr_info("check hifi stop event\n");
	if (hiusb_start_host_event(last_state) ||
	    hiusb_hifi_start_event(last_state) ||
	    hiusb_hifi_start_reset_vbus_event(last_state) ||
	    hiusb_hifi_hibernate_event(last_state) ||
	    hiusb_hifi_wakeup_event(last_state))
		return true;

	pr_err("check hifi stop event failed\n");
	return false;
}

/* start hifiusb maybe failed, allow retry */
static bool hiusb_check_hifi_start_event(struct hiusb_port_state *last_state)
{
	pr_info("check hifi start event\n");
	if (hiusb_start_host_event(last_state) ||
	    hiusb_hifi_start_event(last_state) ||
	    hiusb_hifi_start_reset_vbus_event(last_state) ||
	    hiusb_hifi_stop_event(last_state) ||
	    hiusb_hifi_stop_reset_vbus_event(last_state))
		return true;

	pr_err("check hifi start event failed\n");
	return false;
}

static bool hiusb_check_stop_device_event(struct hiusb_port_state *last_state)
{
	pr_info("check stop device event\n");
	if (hiusb_start_device_event(last_state))
		return true;

	pr_err("check stop device event failed\n");
	return false;
}

static bool hiusb_check_stop_host_event(struct hiusb_port_state *last_state)
{
	pr_info("check stop host event\n");
	if (hiusb_start_host_event(last_state) ||
	    hiusb_hifi_event(last_state) ||
	    hiusb_dp_out_event(last_state))
		return true;

	pr_err("check stop host event failed\n");
	return false;
}

static bool hiusb_check_start_device_event(struct hiusb_port_state *last_state)
{
	pr_info("check start device event\n");
	if (hiusb_stop_host_event(last_state) || hiusb_stop_device_event(last_state))
		return true;

	pr_err("check start device event failed\n");
	return false;
}

static bool hiusb_check_start_host_event(struct hiusb_port_state *last_state)
{
	pr_info("check start host event\n");
	if (hiusb_stop_host_event(last_state) || hiusb_stop_device_event(last_state))
		return true;

	pr_err("check start host event failed\n");
	return false;
}

static bool hiusb_check_ap_usb_event(struct hiusb_port_state *last_state,
						struct hiusb_port_state *new_state)
{
	pr_info("check ap usb event\n");
	if (new_state->cable.state.usb == HIUSB_CABLE_STATE_IN) {
		if (new_state->prop.state.usb_role == HIUSB_PROP_USB_HOST)
			return hiusb_check_start_host_event(last_state);
		else
			return hiusb_check_start_device_event(last_state);
	} else {
		if (new_state->prop.state.usb_role == HIUSB_PROP_USB_DEVICE)
			return hiusb_check_stop_device_event(last_state);
		else
			return hiusb_check_stop_host_event(last_state);
	}
}

static bool hiusb_check_hifi_event(struct hiusb_port_state *last_state,
					struct hiusb_port_state *new_state)
{
	pr_info("check hifi event\n");
	if (new_state->prop.state.hifi_state == HIUSB_PROP_HIFI_START ||
	    new_state->prop.state.hifi_state == HIUSB_PROP_HIFI_START_RESET_VBUS)
		return hiusb_check_hifi_start_event(last_state);
	else if (new_state->prop.state.hifi_state == HIUSB_PROP_HIFI_STOP ||
	    new_state->prop.state.hifi_state == HIUSB_PROP_HIFI_STOP_RESET_VBUS)
		return hiusb_check_hifi_stop_event(last_state);
	else if (new_state->prop.state.hifi_state == HIUSB_PROP_HIFI_HIBERNATE)
		return hiusb_check_hifi_hibernate_event(last_state);
	else if (new_state->prop.state.hifi_state == HIUSB_PROP_HIFI_WAKEUP)
		return hiusb_check_hifi_wakeup_event(last_state);
	else
		return false;
}

/* include ap_usb event and hifi_usb event */
static bool hiusb_check_usb_event(struct hiusb_port_state *last_state,
					struct hiusb_port_state *new_state)
{
	pr_info("check usb event\n");
	if (hiusb_hifi_event(new_state))
		return hiusb_check_hifi_event(last_state, new_state);
	else
		return hiusb_check_ap_usb_event(last_state, new_state);
}

static bool hiusb_check_dp_event(struct hiusb_port_state *last_state,
					struct hiusb_port_state *new_state)
{
	pr_info("check dp event\n");
	if (hiusb_start_host_event(last_state) ||
	    hiusb_hifi_stop_event(last_state) ||
	    hiusb_hifi_stop_reset_vbus_event(last_state) ||
	    hiusb_dp_in_event(last_state) ||
	    hiusb_dp_out_event(last_state))
		return true;

	return false;
}

static void hiusb_print_port_state(struct hiusb_port_state *state,
					bool flag)
{
	pr_info("#%s port cable#\n"
		"[dp]:   %d\n"
		"[usb]:  %d\n"
		"[vbus]: %d\n",
		flag ? "last" : "new",
		state->cable.state.dp,
		state->cable.state.usb,
		state->cable.state.vbus);
	pr_info("#%s port prop#\n"
		"[power_role]: %d\n"
		"[orien]:      %d\n"
		"[usb_role]:   %d\n"
		"[lane_num]:   %d\n"
		"[hpd]:        %d\n"
		"[hifi_state]: %d\n",
		flag ? "last" : "new",
		state->prop.state.power_role,
		state->prop.state.orien,
		state->prop.state.usb_role,
		state->prop.state.lane_num,
		state->prop.state.hpd,
		state->prop.state.hifi_state);
}

bool hiusb_event_check(struct hiusb_port *port,
			    struct hiusb_port_state *last_state,
			    struct hiusb_port_state *new_state)
{
	pr_info("port %d event check start, last event %s, new event %s\n",
		port->portid,
		hiusb_get_dev_type_string(last_state),
		hiusb_get_dev_type_string(new_state));
	hiusb_print_port_state(last_state, true);
	hiusb_print_port_state(new_state, false);

	if (new_state->prop.state.usb_role != HIUSB_PROP_USB_OFF)
		return hiusb_check_usb_event(last_state, new_state);
	else
		return hiusb_check_dp_event(last_state, new_state);
}

void hiusb_event_manager_destroy(struct hiusb_event_manager *emanager)
{
	const char *name = NULL;

	if (!emanager)
		return;

	name = dev_name(&emanager->port->dev);
	pr_info("%s destroy\n", name ? name : "Unknown port");

	cancel_work_sync(&emanager->event_work);
	del_timer_sync(&emanager->event_timer);
	kfree(emanager);
}

static int event_enqueue(struct hiusb_event_manager *emanager,
				struct hiusb_event *event)
{
	struct hiusb_port_state *state = &event->data.state;

	/*
	 * check if there is noly 1 empty space to save event
	 * drop event if it is not OTG_DISCONNECT or CHARGER_DISCONNECT
	*/
	if (kfifo_avail(&emanager->event_fifo) == 1 &&
		!hiusb_stop_host_event(state) &&
		!hiusb_stop_device_event(state)) {
		pr_err("drop event %s except disconnect event\n",
			hiusb_get_dev_type_string(state));
		return -ENOSPC;
	}

	if (kfifo_in(&emanager->event_fifo, event, 1) == 0) {
		pr_err("drop event %s.\n", hiusb_get_dev_type_string(state));
		return -ENOSPC;
	}

	return 0;
}

static int event_dequeue(struct hiusb_event_manager *emanager,
			 struct hiusb_event *event)
{
	return kfifo_out_spinlocked(&emanager->event_fifo, event,
						1, &emanager->event_lock);
}

static void _event_work(struct work_struct *work)
{
	struct hiusb_event_manager *emanager = work_to_hiusb_event_manager(work);
	struct hiusb_event event;
	int ret;

	while (event_dequeue(emanager, &event)) {
		if (!event.process_event)
			continue;
		hiusb_port_lock(emanager->port);
		ret = event.process_event(emanager->port, &event);
		if (ret)
			pr_err("handle event error\n");
		hiusb_port_unlock(emanager->port);
		hiusb_port_put(emanager->port);
	}
}

struct hiusb_event_manager *hiusb_event_manager_create(struct hiusb_port *port)
{
	struct hiusb_event_manager *emanager = NULL;

	if (!port)
		return ERR_PTR(-EINVAL);

	emanager = kzalloc(sizeof(*emanager), GFP_KERNEL);
	if (!emanager)
		return ERR_PTR(-ENOMEM);

	emanager->port = port;
	INIT_KFIFO(emanager->event_fifo);
	spin_lock_init(&emanager->event_lock);
	INIT_WORK(&emanager->event_work, _event_work);

	timer_setup(&emanager->event_timer, hiusb_combophy_event_watchdog, 0);

	return emanager;
}

int hiusb_emanager_queue_event(struct hiusb_event_manager *emanager,
					struct hiusb_event *event)
{
	if (IS_ERR_OR_NULL(emanager) || !event)
		return -EINVAL;

	if (event_enqueue(emanager, event) == 0) {
		if (!queue_work(system_power_efficient_wq, &emanager->event_work))
			pr_err("schedule event_work wait\n");
	} else {
		pr_err("enqueue event failed\n");
		return -EBUSY;
	}

	return 0;
}

