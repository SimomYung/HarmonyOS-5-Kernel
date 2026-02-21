/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: bc1.2 driver for HIUSB
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
#include <linux/platform_drivers/usb/hiusb_phy.h>

#undef pr_fmt
#define pr_fmt(fmt) "[HIUSB_BC12]%s: " fmt, __func__

bool hiusb_port_enumerate_allowed(struct hiusb_port *port)
{
	if(hiusb_is_other_port_enumerate(port->portid))
		return false;

	if (port->bc_again_delay_time == BC_AGAIN_DELAY_TIME_1)
		return false;

	/* do not start peripheral if real charger connected */
	return ((port->charger_type == CHARGER_TYPE_SDP) ||
		(port->charger_type == CHARGER_TYPE_CDP) ||
		(port->charger_type == CHARGER_TYPE_UNKNOWN));
}

bool hiusb_port_sleep_allowed(const struct hiusb_port *port)
{
	return ((port->charger_type == CHARGER_TYPE_DCP) ||
		(port->charger_type == CHARGER_TYPE_UNKNOWN));
}

bool hiusb_port_bc_again_allowed(const struct hiusb_port *port)
{
	pr_info("hiusb_port_bc_again_allowed type %d\n", port->charger_type);
	if ((port->charger_type == CHARGER_TYPE_SDP) ||
			(port->charger_type == CHARGER_TYPE_CDP))
			return true;

	if (port->bc_unknown_again_flag && (port->charger_type == CHARGER_TYPE_UNKNOWN))
		return true;

	if (port->bc_dcp_again_flag && (port->charger_type == CHARGER_TYPE_DCP))
		return true;

	return false;
}

static void hiusb_port_bc_again(struct hiusb_port *port)
{
	int ret;
	bool isschedule = false;
	unsigned int bc_again_delay_time = 0;

	pr_info("+\n");

	/*
	 * Check usb controller status.
	 */
	if (hiusb_port_is_powerdown(port)) {
		pr_err("usb controller is reset, just return\n");
		return;
	}

	/*
	 * STEP 1
	 * stop peripheral which is started when detected as SDP before
	 */
	if (hiusb_port_enumerate_allowed(port)) {
		ret = hiusb_port_ioctrl(port, HIUSB_CLIENT_IOCTRL_STOP_PERIPHERIAL, NULL);
		if (ret)
			pr_err("stop peripheral error\n");
	}

	/*
	 * STEP 2
	 * if it is CHARGER_TYPE_UNKNOWN, we should pull down d+&d- for 20ms
	 */
	if (port->charger_type == CHARGER_TYPE_UNKNOWN) {
		hiusb_port_set_dpdm_pulldown(port, true);
		msleep(20); /* sleep 20ms */
		hiusb_port_set_dpdm_pulldown(port, false);
	}

	port->charger_type = hiusb_port_detect_charger_type(port);
	hiusb_port_notify_charger_type(port);

	if (port->charger_type == CHARGER_TYPE_UNKNOWN) {
		unsigned long flags;

		spin_lock_irqsave(&port->bc_again_lock, flags);
		if (port->bc_again_delay_time == BC_AGAIN_DELAY_TIME_1) {
			port->bc_again_delay_time = BC_AGAIN_DELAY_TIME_2;
			isschedule = true;
		}

		bc_again_delay_time = port->bc_again_delay_time;
		spin_unlock_irqrestore(&port->bc_again_lock, flags);
	} else {
		port->bc_again_delay_time = 0;
	}

	/*
	 * STEP 3
	 * must recheck enumerate_allowed, because charger_type maybe changed,
	 * and enumerate_allowed according to charger_type
	 */
	if (hiusb_port_enumerate_allowed(port)) {
		/* start peripheral */
		ret = hiusb_port_ioctrl(port, HIUSB_CLIENT_IOCTRL_START_PERIPHERIAL, NULL);
		if (ret) {
			pr_err("start peripheral error\n");
			return;
		}
	} else {
		pr_info("it need notify USB_CONNECT_DCP while a real charger connected\n");
		port->speed = USB_CONNECT_DCP;
		if (!queue_work(system_power_efficient_wq,
						&port->speed_change_work))
			pr_err("schedule speed_change_work wait:%d\n", port->speed);
	}

	/* recheck sleep_allowed for charger_type maybe changed */
	if (hiusb_port_sleep_allowed(port))
		hiusb_port_wake_unlock(port);
	else
		hiusb_port_wake_lock(port);

	if (isschedule) {
		ret = queue_delayed_work(system_power_efficient_wq,
				&port->bc_again_work,
				msecs_to_jiffies(bc_again_delay_time));
		pr_info("schedule ret:%d, run bc_again_work %ums later\n",
			ret, bc_again_delay_time);
	}

	pr_info("-\n");
}

void chip_usb_otg_bc_again(void)
{
	pr_debug("+\n");
}
EXPORT_SYMBOL_GPL(chip_usb_otg_bc_again);

static void hiusb_port_bc_again_work(struct work_struct *work)
{
	struct hiusb_port *port = container_of(work,
			struct hiusb_port, bc_again_work.work);

	pr_info("+\n");
	hiusb_port_lock(port);

	/* we are here because it's detected as SDP before */
	if (hiusb_port_bc_again_allowed(port)) {
		pr_info("current charger_type %d, start bc_again\n", port->charger_type);
		hiusb_port_bc_again(port);
	}

	hiusb_port_unlock(port);
	pr_info("-\n");
}

/*
 * In some cases, DCP is detected as SDP wrongly. To avoid this,
 * start bc_again delay work to detect charger type once more.
 * If later the enum process is executed, then it's a real SDP, so
 * the work will be canceled.
 */
void hiusb_port_schedule_bc_again(struct hiusb_port *port)
{
	int ret;
	unsigned long flags;
	unsigned int bc_again_delay_time;

	pr_info("+\n");

	if (!port->bc_again_flag)
		return;

	spin_lock_irqsave(&port->bc_again_lock, flags);
	if ((port->charger_type == CHARGER_TYPE_UNKNOWN)
		&& (port->bc_unknown_again_flag == BC_AGAIN_TWICE))
		port->bc_again_delay_time = BC_AGAIN_DELAY_TIME_1;
	else if ((port->charger_type == CHARGER_TYPE_DCP) &&
			port->bc_unknown_again_flag != 0)
		port->bc_again_delay_time = BC_AGAIN_DELAY_TIME_1;
	else
		port->bc_again_delay_time = BC_AGAIN_DELAY_TIME_2;

	bc_again_delay_time = port->bc_again_delay_time;
	spin_unlock_irqrestore(&port->bc_again_lock, flags);

	ret = queue_delayed_work(system_power_efficient_wq,
			&port->bc_again_work,
			msecs_to_jiffies(bc_again_delay_time));
	pr_info("schedule ret:%d, run bc_again_work %ums later\n",
		ret, bc_again_delay_time);

	pr_info("-\n");
}

void hiusb_cancel_bc_again(int sync)
{
	int portid;
	struct hiusb_port *port = NULL;
	pr_info("+\n");

	portid = hiusb_get_device_id();
	if (portid < 0)
		return;

	port = hiusb_get_port_by_id(portid);
	if (port == NULL)
		return;

	hiusb_port_cancel_bc_again(port, sync);
	pr_info("-\n");
}

void hiusb_port_cancel_bc_again(struct hiusb_port *port, int sync)
{
	int ret;
	pr_info("+\n");
	if (port->bc_again_flag) {
		if (sync)
			ret = cancel_delayed_work_sync(&port->bc_again_work);
		else
			ret = cancel_delayed_work(&port->bc_again_work);
		pr_info("cancel_delayed_work(result:%d)\n", ret);
		port->bc_again_delay_time = 0;
	}
	pr_info("-\n");
}

int hiusb_bc_init(struct hiusb_port *port)
{
	struct device *dev = &port->dev;

	pr_info("+\n");

	spin_lock_init(&port->bc_again_lock);

	hiusb_set_port_power_flag(port, HIUSB_POWER_OFF);

	if (of_property_read_u32(dev->of_node, "bc_again_flag",
			    &(port->bc_again_flag)))
		port->bc_again_flag = 0;

	if (of_property_read_bool(dev->of_node, "disable_bc_detect"))
		port->disable_bc_detect = 1;
	else
		port->disable_bc_detect = 0;

	if (port->bc_again_flag) {
		INIT_DELAYED_WORK(&port->bc_again_work, hiusb_port_bc_again_work);
		if (of_property_read_u32(dev->of_node, "bc_unknown_again_flag",
			    &(port->bc_unknown_again_flag)))
			port->bc_unknown_again_flag = 0;

		if (of_property_read_u32(dev->of_node, "bc_dcp_again_flag",
			    &(port->bc_dcp_again_flag)))
			port->bc_dcp_again_flag = 0;
	} else {
		port->bc_unknown_again_flag = 0;
		port->bc_dcp_again_flag = 0;
	}
	pr_info("bc_again_flag%d bc_unknown_again_flag %d\n",
					port->bc_again_flag, port->bc_unknown_again_flag);

	pr_info("-\n");
	return 0;
}

void hiusb_bc_exit(struct hiusb_port *port)
{
	pr_info("+\n");

	pr_info("-\n");
}

