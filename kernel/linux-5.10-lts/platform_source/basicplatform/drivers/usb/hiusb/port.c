/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: usb port driver for HIUSB
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
#include "debugfs.h"
#include "wrapper.h"

#include <linux/dma-mapping.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/slab.h>
#include <linux/platform_drivers/usb/usb_misc_ctrl.h>
#include <linux/platform_drivers/usb/chip_usb_helper.h>
#include <linux/platform_drivers/usb/hiusb_phy.h>
#include <linux/platform_drivers/usb/hiusb.h>
#include <linux/printk.h>
#include <linux/gpio.h>
#include <linux/arm-smccc.h>
#include <securec.h>
#include <platform_include/basicplatform/linux/mfd/pmic_platform.h>
#ifdef CONFIG_SYS_PM_ULSR
#include <platform_include/cee/linux/ulsr.h>
#endif
#ifdef CONFIG_HUAWEI_USB
#include <chipset_common/hwusb/hw_usb.h>
#endif
#ifdef CONFIG_HIUSB_SUPPORT_FFA
#include <platform_include/see/ffa/ffa_plat_drv.h>
#include <platform_include/see/ffa/ffa_msg_id.h>
#else
#include <platform_include/see/bl31_smc.h>
#endif


#undef pr_fmt
#define pr_fmt(fmt) "[HIUSB_PORT]%s: " fmt, __func__

#define to_hiusb_port(ptr) container_of((ptr), struct hiusb_port, dev)

static spinlock_t g_port_class_lock;
static bool g_lock_init = false;

static int hiusb_port_class_iter_fn(struct device *dev, void *data)
{
	hiusb_port_iter_fn_t port_fn = data;

	if (!port_fn)
		return -EINVAL;

	return port_fn(to_hiusb_port(dev));
}

int hiusb_for_each_port(hiusb_port_iter_fn_t fn)
{
	struct class *hiusb_port_class = hiusb_get_port_class();

	pr_info("+\n");

	if (!fn)
		return -EINVAL;

	if (!hiusb_port_class)
		return -ENODEV;

	return class_for_each_device(hiusb_port_class, NULL,
				     (void *)fn, hiusb_port_class_iter_fn);
}

struct hiusb_port *fwnode_hiusb_port_get(struct fwnode_handle *fwnode)
{
	struct device *dev = NULL;
	struct class *hiusb_port_class = hiusb_get_port_class();

	pr_info("+\n");

	if (!hiusb_port_class || !fwnode)
		return NULL;

	dev = class_find_device_by_fwnode(hiusb_port_class, fwnode);
	return dev ? to_hiusb_port(dev) : NULL;
}

static int device_match_portid(struct device *dev, const void *portid)
{
	struct hiusb_port *port = to_hiusb_port(dev);
	return port->portid == *(int*)portid;
}

static struct device *class_find_device_by_portid(struct class *class,
					const int portid)
{
	struct device  *dev = NULL;
	unsigned long  flags;

	if (g_lock_init == false)
		return NULL;

	spin_lock_irqsave(&g_port_class_lock, flags);
	dev = class_find_device(class, NULL, &portid, device_match_portid);
	spin_unlock_irqrestore(&g_port_class_lock, flags);
	return dev;
}

struct hiusb_port *hiusb_get_port_by_id(int portid)
{
	struct device *dev = NULL;
	struct class *hiusb_port_class = hiusb_get_port_class();

	if (!hiusb_port_class )
		return NULL;

	dev = class_find_device_by_portid(hiusb_port_class, portid);
	return dev ? to_hiusb_port(dev) : NULL;
}

bool hiusb_check_is_u2_only_by_portid(u32 portid)
{
	struct hiusb_port *port = NULL;
	u32 lane_num;

	if (portid >= HIUSB_PORT_MAXSIZE)
		return false;

	port = hiusb_get_port_by_id(portid);
	if (port == NULL) {
		pr_info("%s port get failed\n", __func__);
		return false;
	}

	lane_num = port->last_event_state.prop.state.lane_num;
	pr_info("portid %d, lane_num %d +\n", portid, lane_num);
	if (lane_num == HIUSB_PROP_DP_4LANE) {
		pr_info("%s HIUSB_PROP_DP_4LANE\n", __func__);
		return true;
	}

	return false;
}

void hiusb_port_lock(struct hiusb_port *port)
{
	if (!IS_ERR_OR_NULL(port))
		mutex_lock(&port->port_mutex);
}

void hiusb_port_unlock(struct hiusb_port *port)
{
	if (!IS_ERR_OR_NULL(port))
		mutex_unlock(&port->port_mutex);
}

void hiusb_port_get(struct hiusb_port *port)
{
	if (!IS_ERR_OR_NULL(port))
		(void)get_device(&port->dev);
}

void hiusb_port_put(struct hiusb_port *port)
{
	if (!IS_ERR_OR_NULL(port))
		put_device(&port->dev);
}

void hiusb_set_port_power_flag(struct hiusb_port *port, int val)
{
	atomic_set(&port->port_power_on, val);
	pr_info("set_port_power_flag %d\n", val);
}

int hiusb_port_is_powerdown(struct hiusb_port *port)
{
	int power_flag = atomic_read(&port->port_power_on);
	return ((power_flag == HIUSB_POWER_OFF) || (power_flag == HIUSB_POWER_HOLD));
}

static void hiusb_notify_speed_change_if_off(struct hiusb_port *port,
						struct hiusb_port_state *state)
{
	if (hiusb_stop_host_event(state) || hiusb_stop_device_event(state)) {
		pr_debug("notify USB_SPEED_UNKNOWN to app while usb plugout\n");
		port->speed = USB_SPEED_UNKNOWN;
		if (!queue_work(system_power_efficient_wq,
				&port->speed_change_work))
			pr_err("schedule speed_change_work wait:%d\n",
				port->speed);
	}
}

static void hiusb_save_event_time_stamp(struct hiusb_port *port,
						struct hiusb_port_state *state)
{
	if (hiusb_start_device_event(state))
		port->time_stamps.start_device_time_stamp = jiffies;
	else if (hiusb_start_host_event(state))
		port->time_stamps.start_host_time_stamp = jiffies;
	else if (hiusb_stop_host_event(state))
		port->time_stamps.stop_host_time_stamp = jiffies;
	else if (hiusb_hifi_start_event(state) ||
		hiusb_hifi_start_reset_vbus_event(state))
		port->time_stamps.start_hifiusb_time_stamp = jiffies;
	else if (hiusb_hifi_stop_event(state) ||
		hiusb_hifi_stop_reset_vbus_event(state))
		port->time_stamps.stop_hifiusb_time_stamp = jiffies;
	else
		return;
}

int hiusb_port_queue_event(struct hiusb_port *port, struct hiusb_event *event)
{
	int ret;
	struct hiusb_port_state *new_state = NULL;

	if (!port || !event)
		return -EINVAL;

	new_state = &event->data.state;

	if (port->eventmask) {
		pr_info("eventmask enabled, mask all events.\n");
		return -EPERM;
	}

	spin_lock(&port->event_manager->event_lock);

	if(!hiusb_event_check(port, &port->last_event_state, new_state)) {
		spin_unlock(&port->event_manager->event_lock);
		pr_err("port %d event check failed, last event:%s,new event:%s\n",
			port->portid,
			hiusb_get_dev_type_string(&port->last_event_state),
			hiusb_get_dev_type_string(new_state));
		return -EINVAL;
	}

	pr_debug("port %d switch to new state: %s\n", port->portid,
		hiusb_get_dev_type_string(new_state));
	ret = hiusb_emanager_queue_event(port->event_manager, event);
	if (ret) {
		pr_err("queue event failed %d\n", ret);
		spin_unlock(&port->event_manager->event_lock);
		return ret;
	}
	if (hiusb_start_device_event(new_state) ||
	    hiusb_stop_device_event(new_state))
		hiusb_port_wake_lock(port);

	port->last_event_state = event->data.state;
	hiusb_save_event_time_stamp(port, new_state);
	hiusb_notify_speed_change_if_off(port, new_state);
	spin_unlock(&port->event_manager->event_lock);

	return ret;
}

static int hiusb_port_assign_match_client(struct hiusb_port *port, void *data,
					  bool (*match)(struct hiusb_client *, void *))
{
	int ret = -ENOENT;
	struct hiusb_client *client = NULL;
	enum hiusb_port_usb_mode mode;
	u8 type = *((u8 *)data);
	pr_info("+\n");
	pr_info("portid %d, type %d\n", port->portid, type);
	if(type == HIUSB_CLIENT_TYPE_DEVICE)
		mode = HIUSB_PORT_USB_MODE_DEVICE;
	else if(type == HIUSB_CLIENT_TYPE_HOST)
		mode = HIUSB_PORT_USB_MODE_HOST;
	else
		mode = HIUSB_PORT_USB_MODE_HIFI;

	list_for_each_entry(client, &port->client_list, list) {
		if (match && match(client, data)) {
			pr_info("port %pK portid %d client %pK\n", port, port->portid, client);
			port->cur_client = client;
			ret = hiusb_client_assign_port(client, mode, &port->dev);
			if (ret) {
				dev_err(&port->dev, "assign port to %s failed %d\n",
					dev_name(&client->dev), ret);
				port->cur_client = NULL;
				continue;
			}
			return 0;
		}
	}
	pr_info("-n");
	return ret;
}

static bool _match_client_by_type(struct hiusb_client *client, void *data)
{
	u8 type = *((u8 *)data);
	pr_info("client->desc->type %d type %d\n", client->desc->type, type);
	return (client->desc->type & type);
}

static int hiusb_port_assign_client_by_type(struct hiusb_port *port, u8 type)
{
	return hiusb_port_assign_match_client(port, &type, _match_client_by_type);
}

static bool _match_client_by_name(struct hiusb_client *client, void *data)
{
	const char *name = data;

	return strcmp(client->desc->name, name) == 0;
}

static int hiusb_port_assign_client_by_name(struct hiusb_port *port, char *name)
{
	return hiusb_port_assign_match_client(port, name, _match_client_by_name);
}

int hiusb_port_start_client(struct hiusb_port *port, u8 type)
{
	int ret = -ENODEV;

	pr_info("+\n");
	if (!port || (hiusb_client_valid_type(type) == 0))
		return -EINVAL;

	if (port->cur_client)
		return -EBUSY;

	ret = hiusb_port_assign_client_by_type(port, type);
	if (ret)
		dev_err(&port->dev, "hiusb_port_start_client failed %d\n", ret);

	pr_info("assign %s type to %d\n", dev_name(&port->dev), type);
	pr_info("-\n");
	return ret;
}

int hiusb_port_init_hw(struct hiusb_port *port)
{
	int ret;

	pr_info("+\n");
	if (!port )
		return -EINVAL;

	ret = hiusb_client_init_hw_port(port->cur_client);

	pr_info("init port hw\n");
	pr_info("-\n");

	return ret;
}

int hiusb_port_hibernate(struct hiusb_port *port)
{
	int ret;

	pr_info("+\n");
	if (!port )
		return -EINVAL;

	ret = hiusb_client_hibernate_port(port->cur_client);
	if (ret)
		dev_err(&port->dev, "hiusb_client_hibernate_port failed %d\n", ret);

	pr_info("-\n");
	return ret;
}

int hiusb_port_wakeup(struct hiusb_port *port)
{
	int ret;

	pr_info("+\n");
	if (!port )
		return -EINVAL;

	ret = hiusb_client_wakeup_port(port->cur_client);
	if (ret)
		dev_err(&port->dev, "hiusb_client_wakeup_port failed %d\n", ret);

	pr_info("-\n");
	return ret;
}

void hiusb_port_set_vbus_power(struct hiusb_port *port, unsigned int is_on)
{
	enum chip_charger_type new;
	pr_info("+\n");

	if (is_on == 0)
		new = CHARGER_TYPE_NONE;
	else
		new = PLEASE_PROVIDE_POWER;

	if (port->charger_type != new) {
		pr_info("set port power %u\n", is_on);
		port->charger_type = new;
		hiusb_port_notify_charger_type(port);
	}

	if (port->fpga_otg_drv_vbus_gpio > 0) {
		gpio_direction_output(port->fpga_otg_drv_vbus_gpio, !!is_on);
		pr_debug("turn %s drvvbus for fpga\n", is_on != 0 ? "on" : "off");
	}

	if (port->vbus_gpio > 0) {
		gpio_direction_output(port->vbus_gpio, !!is_on);
		pr_debug("port%d turn %s vbus\n", is_on != 0 ? "on" : "off", port->portid);
	}

	pr_info("-\n");
}

int hiusb_port_ioctrl(struct hiusb_port *port, enum hiusb_port_client_operation operation_code, void* data)
{
	int ret;

	pr_info("+\n");
	if (!port )
		return -EINVAL;

	ret = hiusb_client_port_ioctrl(port->cur_client, operation_code, data);
	if (ret)
		dev_err(&port->dev, "hiusb_client_port_ioctrl failed %d\n", ret);

	pr_info("-\n");
	return ret;
}

int hiusb_port_deinit_hw(struct hiusb_port *port, unsigned int keep_power)
{
	int ret;

	if (!port)
		return -EINVAL;

	ret = hiusb_client_deinit_hw_port(port->cur_client, keep_power);
	if (ret)
		dev_err(&port->dev, "deinit hw port failed %d\n", ret);

	return ret;
}

int hiusb_port_stop_client(struct hiusb_port *port)
{
	int ret;

	if (!port)
		return -EINVAL;

	if (!port->cur_client)
		return -ENODEV;

	ret = hiusb_client_deassign_port(port->cur_client);
	if (ret) {
		dev_err(&port->dev, "deassign port failed %d\n", ret);
		return ret;
	}
	port->cur_client = NULL;

	return ret;
}

int hiusb_port_suspend_client(struct hiusb_port *port, bool keep_remote_wakeup)
{
	int ret;

	if (!port)
		return -EINVAL;

	if (!port->cur_client)
		return -ENODEV;

	ret = hiusb_client_suspend_port(port->cur_client, keep_remote_wakeup);
	if (ret)
		dev_err(&port->dev, "suspend port failed %d\n", ret);

	return ret;
}

int hiusb_port_resume_client(struct hiusb_port *port, bool keep_remote_wakeup)
{
	int ret;

	if (!port)
		return -EINVAL;

	if (!port->cur_client)
		return -ENODEV;

	ret = hiusb_client_resume_port(port->cur_client, keep_remote_wakeup);
	if (ret)
		dev_err(&port->dev, "resume port failed %d\n", ret);

	return ret;
}

int hiusb_port_switch_client_by_name(struct hiusb_port *port, char *name)
{
	int ret = -ENOENT;

	if ((!port) || (!name))
		return -EINVAL;

	if (port->cur_client) {
		ret = hiusb_client_deassign_port(port->cur_client);
		if (ret) {
			dev_err(&port->dev, "deassign port failed %d\n", ret);
			return ret;
		}
		port->cur_client = NULL;
	}

	ret = hiusb_port_assign_client_by_name(port, name);

	return ret;
}

void hiusb_port_wake_lock(struct hiusb_port *port)
{
	if (port->wake_lock && !port->wake_lock->active) {
		pr_info("hiusb_port_wake_lock %d\n", port->portid);
		__pm_stay_awake(port->wake_lock);
	}
}

void hiusb_port_wake_unlock(struct hiusb_port *port)
{
	if (port->wake_lock && port->wake_lock->active) {
		pr_info("hiusb_port_wake_unlock %d\n", port->portid);
		__pm_relax(port->wake_lock);
	}
}

int hiusb_port_switch_client_by_type(struct hiusb_port *port, u8 type)
{
	int ret = -ENOENT;

	if (!port)
		return -EINVAL;

	if (port->cur_client) {
		ret = hiusb_client_deassign_port(port->cur_client);
		if (ret) {
			dev_err(&port->dev, "deassign port failed %d\n", ret);
			return ret;
		}
		port->cur_client = NULL;
	}

	ret = hiusb_port_assign_client_by_type(port, type);

	return ret;
}

static void hiusb_port_unreset_phy_if_fpga(struct hiusb_port *port)
{
	unsigned int gpio;

	if (port->fpga_phy_reset_gpio < 0)
		return;

	gpio = (unsigned int)port->fpga_phy_reset_gpio;

	gpio_direction_output(gpio, 1);
	mdelay(10); /* mdelay time */

	gpio_direction_output(gpio, 0);
	mdelay(10); /* mdelay time */
}

static unsigned long hiusb_port_tzpc_config(struct hiusb_port *port)
{
#ifdef CONFIG_HIUSB_SUPPORT_FFA
	int ret;
	struct ffa_send_direct_data args = {0};
	args.data0 = FID_USB_TZPC;
	args.data1 = (unsigned int)port->portid;
	args.data2 = 0;
	args.data3 = 0;

	ret = ffa_platdrv_send_msg(&args);
	if (ret != 0)
		pr_err("%s:tzpc:ffa send fail:%d\n", __func__, ret);
	return 0;
#else
	struct arm_smccc_res res;

	arm_smccc_smc((u64)FID_USB_TZPC, port->portid, 0, 0, 0, 0, 0, 0, &res);
	return 0;
#endif
}

int hiusb_port_usb2_init(struct hiusb_port *port)
{
	int ret;
	pr_err("+\n");

	if (port->usb_tzpc_config) {
		if (hiusb_port_tzpc_config(port)) {
			pr_err("tzpc config failed\n");
			return -ESRCH;
		}
	}

	ret = multi_usb_misc_ctrl_init(port->misc_ctrl);
	if (ret)
		return ret;

	if (port->is_fpga_flag) {
		pr_info("fpga unreset u2 phy by reset_gpio\n");
		hiusb_port_unreset_phy_if_fpga(port);
		return ret;
	}

	ret = hiusb_usb2phy_init(port->usb2_phy);
	if (ret) {
#ifdef CONFIG_PG_DEBUG
		pr_err("soc_test name:Usb2phy module_index:16 type:part mask_index:%d result:fail\n", port->portid);
#endif
		multi_usb_misc_ctrl_exit(port->misc_ctrl);
	}

#ifdef CONFIG_PG_DEBUG
	pr_err("soc_test name:Usb2phy module_index:16 type:part mask_index:%d result:success\n", port->portid);
#endif
	pr_err("-\n");
	return ret;
}

int hiusb_port_usb2_exit(struct hiusb_port *port, unsigned int keep_power)
{
	int ret;
	pr_info("+\n");
	if(keep_power) {
		ret = hiusb_usb2phy_poweron(port->usb2_phy);
		if(ret)
			pr_err("usb2 power on fail %d", ret);
	}

	/* Consistent with "hiusb_port_usb2_init" function */
	if (!port->is_fpga_flag) {
		ret = hiusb_usb2phy_exit(port->usb2_phy);
		if(ret) {
			pr_err("usb2 phy exit fail %d", ret);
			(void)hiusb_usb2phy_poweroff(port->usb2_phy);
		}
	}

	multi_usb_misc_ctrl_exit(port->misc_ctrl);
	pr_info("-\n");
	return ret;
}

int hiusb_port_set_dpdm_pulldown(struct hiusb_port *port, bool pulldown)
{
	int ret;
	ret = hiusb_usb2phy_set_dpdm_pulldown(port->usb2_phy, pulldown);
	if(ret)
		pr_err("hiusb_port_set_dpdm_pulldownfail %d", ret);

	return ret;
}

int hiusb_port_set_vdp_src(struct hiusb_port *port, bool enbale)
{
	int ret;
	ret = hiusb_usb2phy_set_vdp_src(port->usb2_phy, enbale);
	if(ret)
		pr_err("hiusb_port_set_vdp_src fail %d", ret);

	return ret;
}

enum chip_charger_type hiusb_port_detect_charger_type(struct hiusb_port *port)
{
	pr_info("+\n");

	/* In FPGA, PC or CDC, usb connect to the standard downstream port,
	 * bc detect is not required, just return CHARGER_TYPE_SDP */
	if (port->disable_bc_detect) {
		pr_info("disable bc12 detect\n");
		return CHARGER_TYPE_SDP;
	}

	if (port->fake_charger_type != CHARGER_TYPE_NONE) {
		pr_info("fake type: %d\n", port->fake_charger_type);
		return port->fake_charger_type;
	}

	return hiusb_usb2phy_detect_charger_type(port->usb2_phy);
}

void hiusb_port_set_charger_type(struct hiusb_port *port, enum chip_charger_type type)
{
	port->charger_type = type;
}

int hiusb_port_usb3_init(struct hiusb_port *port)
{
	int ret;
	struct phy *usb3_phy = NULL;

	ret = multi_usb_misc_ctrl_init(port->misc_ctrl);
	if (ret)
		return ret;

	if (port->usb3_lite_phy)
		usb3_phy = port->usb3_lite_phy;
	else
		usb3_phy = port->usb3_phy;
	ret = hiusb_usb3phy_init(usb3_phy);
	if (ret) {
#ifdef CONFIG_PG_DEBUG
		pr_err("soc_test name:Usb3phy module_index:17 type:part mask_index:%d result:fail\n", port->portid);
#endif
		multi_usb_misc_ctrl_exit(port->misc_ctrl);
	}
#ifdef CONFIG_PG_DEBUG
	pr_err("soc_test name:Usb3phy module_index:17 type:part mask_index:%d result:success\n", port->portid);
#endif

	return ret;
}

int hiusb_port_usb3_exit(struct hiusb_port *port)
{
	int ret;
	struct phy *usb3_phy = NULL;

	if (port->usb3_lite_phy)
		usb3_phy = port->usb3_lite_phy;
	else
		usb3_phy = port->usb3_phy;
	ret = hiusb_usb3phy_exit(usb3_phy);
	if(ret)
		pr_err("usb3 phy exit fail %d", ret);

	multi_usb_misc_ctrl_exit(port->misc_ctrl);

	return ret;
}

static int hiusb_port_get_phys(struct hiusb_port *port)
{
	struct device *dev = &port->dev;
	int ret = 0;

	port->usb2_phy = devm_phy_get(dev, "usb2-phy");
	if (IS_ERR(port->usb2_phy)) {
		ret = PTR_ERR(port->usb2_phy);
		if (ret == -ENOSYS || ret == -ENODEV) {
			port->usb2_phy = NULL;
		} else if (ret == -EPROBE_DEFER) {
			return ret;
		} else {
			dev_err(dev, "no usb2 phy configured\n");
			return ret;
		}
	}

	port->usb3_phy = devm_phy_get(dev, "usb3-phy");
	if (IS_ERR(port->usb3_phy)) {
		ret = PTR_ERR(port->usb3_phy);
		if (ret == -ENOSYS || ret == -ENODEV) {
			port->usb3_phy = NULL;
		} else if (ret == -EPROBE_DEFER) {
			return ret;
		} else {
			dev_err(dev, "no usb3 phy configured\n");
			return ret;
		}
	}

	port->usb3_lite_phy = devm_phy_get(dev, "usb3-lite-phy");
	if (IS_ERR(port->usb3_lite_phy)) {
		ret = PTR_ERR(port->usb3_lite_phy);
		if (ret == -ENOSYS || ret == -ENODEV) {
			port->usb3_lite_phy = NULL;
		} else if (ret == -EPROBE_DEFER) {
			return ret;
		} else {
			dev_err(dev, "no usb3 lite phy configured\n");
			return ret;
		}
	}

	pr_info("usb3_phy 0x%x usb2_phy 0x%x usb3_lite_phy 0x%x\n",
		port->usb3_phy, port->usb2_phy, port->usb3_lite_phy);

	return 0;
}

static void hiusb_port_put_clients(struct hiusb_port *port)
{
	struct hiusb_client *client = NULL;
	struct hiusb_client *next = NULL;
	pr_info("+\n");

	list_for_each_entry_safe(client, next, &port->client_list, list) {
		hiusb_client_put(client);
		hiusb_client_unbind_port(client);
		list_del_init(&client->list);
	}

	pr_info("-\n");
}

struct hiusb_port *udev_to_port(struct usb_device *udev)
{
	if(udev->bus == NULL)
		return NULL;

	if(udev->bus->controller == NULL)
		return NULL;

	if(udev->bus->controller->parent == NULL)
		return NULL;

	if(udev->bus->controller->parent->parent == NULL)
		return NULL;

	return to_hiusb_port(udev->bus->controller->parent->parent);
}

int hiusb_find_portid_by_udev(struct usb_device *udev)
{
	struct hiusb_port *port;

	if (udev == NULL)
		return -ENODEV;

	port = udev_to_port(udev);
	if (port == NULL)
		return -ENODEV;

	return port->portid;
}
EXPORT_SYMBOL_GPL(hiusb_find_portid_by_udev);

static int hiusb_port_get_clients(struct hiusb_port *port)
{
	int nval, i;
	struct hiusb_client *client = NULL;
	pr_info("+");

	nval = fwnode_property_read_string_array(port->fwnode, "client-names", NULL, 0);
	if (nval <= 0)
		return -ENODEV;

	for (i = 0; i < nval; i++) {
		client = hiusb_client_get_by_index(port->fwnode, i);
		if (IS_ERR(client)) {
			pr_err("get client index %d failed\n", i);
			if (PTR_ERR(client) == -EPROBE_DEFER)
				return -EPROBE_DEFER;

			continue;
		}
		hiusb_client_bind_port(client, port);

		list_add_tail(&client->list, &port->client_list);
	}
	pr_info("-");

	return 0;
}

static struct fwnode_handle *hiusb_get_sysctrl_fwnode(struct fwnode_handle *port)
{
	int ret;
	struct fwnode_reference_args ref;

	if (!port)
		return NULL;

	ret = fwnode_property_get_reference_args(port, "hiusb-sysctrl", NULL,
						 0, 0, &ref);
	if (ret)
		return NULL;

	return ref.fwnode;
}

static void hiusb_port_get_resource_for_fpga(struct hiusb_port *port)
{
	struct device *dev = &port->dev;

	port->fpga_usb_mode_gpio = -1;
	port->fpga_otg_drv_vbus_gpio = -1;
	port->fpga_phy_reset_gpio = -1;
	port->fpga_phy_switch_gpio = -1;

	if (of_property_read_u32(dev->of_node, "fpga_flag",
			    &(port->is_fpga_flag)))
		port->is_fpga_flag = 0;

	if (port->is_fpga_flag == 0)
		return;

	pr_err("this is fpga platform\n");

	port->fpga_usb_mode_gpio = of_get_named_gpio(dev->of_node,
			"fpga_usb_mode_gpio", 0);
	port->fpga_otg_drv_vbus_gpio = of_get_named_gpio(dev->of_node,
			"fpga_otg_drv_vbus_gpio", 0);
	port->fpga_phy_reset_gpio = of_get_named_gpio(dev->of_node,
			"fpga_phy_reset_gpio", 0);
	port->fpga_phy_switch_gpio = of_get_named_gpio(dev->of_node,
			"fpga_phy_switch_gpio", 0);

	pr_err("fpga usb gpio info:usb_mode=%d, dr_vbus=%d, phy_reset=%d, phy_switch=%d\n",
			port->fpga_usb_mode_gpio, port->fpga_otg_drv_vbus_gpio,
			port->fpga_phy_reset_gpio, port->fpga_phy_switch_gpio);
}

static void hiusb_port_get_init_state(struct hiusb_port *port)
{
	const char *init_state = NULL;

	if (of_property_read_string(port->dev.of_node, "init-state",
				&init_state) == 0) {
		if (strcmp(init_state, "usb_host") == 0)
			port->init_state = USB_STATE_HOST;
		else if (strcmp(init_state, "usb_device") == 0)
			port->init_state = USB_STATE_DEVICE;
		else
			port->init_state = USB_STATE_OFF;
	} else {
		pr_info("init-state property is not found\n");
		port->init_state = USB_STATE_OFF;
	}
}

static int hiusb_port_get_res_form_fwnode(struct hiusb_port *port)
{
	int ret;
	bool is_default_port;

	port->misc_ctrl = hiusb_get_sysctrl_fwnode(port->fwnode);
	if(!port->misc_ctrl)
		return -ENODEV;

	if (of_property_read_u32(port->dev.of_node, "hifi_ip_first",
				&(port->hifi_ip_first)))
		port->hifi_ip_first = 0;

	port->support_remote_wakeup = of_property_read_bool(port->dev.of_node, "support_remote_wakeup") ? 1 : 0;
	port->support_sleep_on_low_power = of_property_read_bool(port->dev.of_node, "support_sleep_on_low_power") ? 1 : 0;
	port->hiusb_host_wakelock_disable = of_property_read_bool(port->dev.of_node, "hiusb_host_wakelock_disable") ? 1 : 0;

	if (of_property_read_u32(port->dev.of_node, "usb_connect_suspend",
				&(port->usb_connect_suspend)))
		port->usb_connect_suspend = 0;

	port->support_device_mode = of_property_read_bool(port->dev.of_node,
				"support_device_mode");
	port->quirk_keep_u2_power_suspend = of_property_read_bool(port->dev.of_node,
					"quirk-keep-u2-power-suspend");

	port->notify_state = of_property_read_bool(port->dev.of_node,
						"notify_state");
	port->usb_tzpc_config = of_property_read_bool(port->dev.of_node, "usb_tzpc_config");
	pr_info("port id %d, usb_tzpc_config %d\n", port->portid, port->usb_tzpc_config);
	is_default_port = of_property_read_bool(port->dev.of_node,
						"default_port");
	if (is_default_port) {
		pr_info("default portid %d\n", port->portid);
		hiusb_set_default_portid(port->portid);
	}

	port->no_need_vbus_gpio = of_property_read_bool(port->dev.of_node, "no_need_vbus_gpio");
	pr_info("port id %d, no_need_vbus_gpio %d\n", port->portid, port->no_need_vbus_gpio);
	if (!port->no_need_vbus_gpio) {
		port->vbus_gpio = of_get_named_gpio(port->dev.of_node, "vbus_gpio", 0);
		if (port->vbus_gpio < 0)
			pr_info("port%d vbus gpio not found\n", port->portid);
		else
			gpio_direction_output(port->vbus_gpio, 0);
	}

	hiusb_port_get_init_state(port);

	hiusb_port_get_resource_for_fpga(port);

	if(!multi_usb_misc_ctrl_is_ready(port->misc_ctrl)){
		pr_err("sysctrl not ready\n");
		ret = -EPROBE_DEFER;
		goto put_sysctrl;
	}

	ret = hiusb_port_get_phys(port);
	if (ret) {
		pr_err("get phy failed %d\n", ret);
		goto put_sysctrl;
	}

	ret = hiusb_port_get_clients(port);
	if (ret) {
		pr_err("get controller failed %d\n", ret);
		goto put_sysctrl;
	}

	return 0;
put_sysctrl:
	fwnode_handle_put(port->misc_ctrl);
	return ret;
}

static void hiusb_port_put_res_form_fwnode(struct hiusb_port *port)
{
	if (port->misc_ctrl)
		fwnode_handle_put(port->misc_ctrl);

	hiusb_port_put_clients(port);
}

static void hiusb_port_dev_release(struct device *dev)
{
	struct hiusb_port *port = to_hiusb_port(dev);
	pr_info("+\n");

	hiusb_port_remove_attr_file(port);

	hiusb_bc_exit(port);
	wakeup_source_unregister(port->wake_lock);
	port->wake_lock = NULL;

	hiusb_event_manager_destroy(port->event_manager);
	hiusb_port_put_res_form_fwnode(port);
	mutex_destroy(&port->port_mutex);
	kfree(port);
	pr_info("-\n");
}

static ssize_t clients_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	int len;
	int i = 0;
	struct hiusb_port *port = dev_get_drvdata(dev);
	struct hiusb_client *client = NULL;

	len = snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "%s\n", "Support clients:");
	mutex_lock(&port->port_mutex);
	list_for_each_entry(client, &port->client_list, list) {
		i++;
		if (len < PAGE_SIZE)
			len += snprintf_s(buf + len, PAGE_SIZE - len, PAGE_SIZE - len -1,
					"%d. %s\n", i, client->desc->name);
	}
	mutex_unlock(&port->port_mutex);

	if (len < PAGE_SIZE)
		len += snprintf_s(buf + len, PAGE_SIZE - len, PAGE_SIZE - len-1, "%s\n",
				"Write client name to improve its prio");
	return len;
}

static ssize_t clients_store(struct device *dev, struct device_attribute *attr,
			     const char *buf, size_t size)
{
	return (ssize_t)size;
}
static DEVICE_ATTR_RW(clients);

static struct attribute *g_hiusb_port_attrs[] = {
	&dev_attr_clients.attr,
	NULL,
};

static const struct attribute_group g_hiusb_port_group = {
	.attrs = g_hiusb_port_attrs,
};

static const struct attribute_group *g_hiusb_port_groups[] = {
	&g_hiusb_port_group,
	NULL,
};

#ifdef CONFIG_PM_SLEEP
static int hiusb_port_prepare(struct device *dev)
{
	struct hiusb_port *port = to_hiusb_port(dev);
	int ret = 0;

	if (!port) {
		pr_err("port NULL\n");
		return -ENODEV;
	}
	
	pr_info("+ portid:%d\n", port->portid);

	mutex_lock(&port->port_mutex);

	switch (port->usb_state) {
	case USB_STATE_OFF:
		pr_info("off state.\n");
		break;
	case USB_STATE_DEVICE:
		pr_info("device state.\n");
		if (!hiusb_port_enumerate_allowed(port)) {
			pr_info("connected is a real charger\n");
			hiusb_port_set_vdp_src(port, false);
		}

		break;
	case USB_STATE_HOST:
		pr_info("host mode.\n");
		break;
	case USB_STATE_HIFI_USB:
	case USB_STATE_HIFI_USB_HIBERNATE:
		break;
	default:
		pr_err("illegal state!\n");
		ret = -EFAULT;
		goto error;
	}

	pr_info("- portid:%d\n", port->portid);
	return ret;
error:
	mutex_unlock(&port->port_mutex);
	return ret;
}

static void hiusb_port_complete(struct device *dev)
{
	struct hiusb_port *port = to_hiusb_port(dev);

	if (!port) {
		pr_err("port NULL !\n");
		return;
	}

	pr_info("+ portid:%d\n", port->portid);

	switch (port->usb_state) {
	case USB_STATE_OFF:
		pr_info("off state.\n");
		break;
	case USB_STATE_DEVICE:
		pr_info("device state.charger_type[%d]\n", port->charger_type);

		if(hiusb_port_sleep_allowed(port)) {
			hiusb_port_wake_unlock(port);
		} else {
			hiusb_port_wake_lock(port);
		}

		/* do not start peripheral if real charger connected */
		if (!hiusb_port_enumerate_allowed(port))
			pr_info("a real charger connected\n");

		break;
	case USB_STATE_HOST:
		pr_info("host mode.\n");
		break;
	case USB_STATE_HIFI_USB:
	case USB_STATE_HIFI_USB_HIBERNATE:
		/* keep audio usb power on */
		break;
	default:
		pr_err("illegal state!\n");
		break;
	}

	mutex_unlock(&port->port_mutex);
	pr_info("- portid:%d\n", port->portid);
}


static int hiusb_port_suspend_process(struct hiusb_port *port,
		bool host_mode, bool is_freeze)
{
	int ret;
	bool keep_power = false;
	bool keep_remote_wakeup = false;
	bool power_down_flag_is_setted = hiusb_get_s3_power_down_flag();
	pr_info("+ portid:%d portState(%s)\n", 
		port->portid, chip_usb_state_string(port->usb_state));

#if defined(CONFIG_PM_SLEEP) && defined(CONFIG_PRODUCT_ARMPC)
	hiusb_port_dp_trigger_in_suspend_resume(port, true);
#endif

	if (!host_mode && port->quirk_keep_u2_power_suspend != 0)
		keep_power = true;

	if (port->support_remote_wakeup) {
		keep_remote_wakeup = true;

		if (is_freeze)
			keep_remote_wakeup = false;

#ifdef CONFIG_SYS_PM_ULSR
		pr_info("%s: is_ulsr:%d\n", __func__, is_ulsr());
		if (is_ulsr())
			keep_remote_wakeup = false;
#endif
	}

	pr_info("keep_remote_wakeup:%d s3_power_down_flag:%d\n", keep_remote_wakeup, power_down_flag_is_setted);

	ret = hiusb_port_suspend_client(port, keep_remote_wakeup);
	if (ret)
		pr_err("hiusb_port_suspend_client failed %d\n", ret);

	/* usbc host support remote wakeup */
	if (keep_remote_wakeup) {
		pr_info("suspend with perifix power on, may support usb remote wakeup\n");
		ret = hiusb_usb2phy_suspend(port->usb2_phy);
		if (ret)
			pr_err("usb2_phy_suspend failed %d\n", ret);
		pr_info("- portid:%d ret:%d\n", port->portid, ret);
		return ret;
	}

	pr_info("suspend with perifix power down, can not support usb remote wakeup\n");
	ret = hiusb_port_usb2_exit(port, keep_power);
	if (ret)
		pr_err("hiusb_port_usb2_exit failed %d\n", ret);

	if (port->usb_state != USB_STATE_HIFI_USB) {
		ret = hiusb_port_usb3_exit(port);
		if (ret) {
			pr_err("hiusb_port_usb3_exit failed %d\n", ret);
			goto err_u2_power_off;
		}
	}

	pr_info("- portid:%d ret:%d\n", port->portid, ret);
	return ret;

err_u2_power_off:
	if (keep_power) {
		if (hiusb_usb2phy_poweroff(port->usb2_phy))
			pr_err("usb2 phy poweroff failed\n");
	}
	pr_info("- portid:%d ret:%d\n", port->portid, ret);
	return ret;
}

static int hiusb_port_resume_process(struct device *dev,
			struct hiusb_port *port, bool is_freeze)
{
	int ret;
	bool keep_remote_wakeup = false;
	bool power_down_flag_is_setted = hiusb_get_s3_power_down_flag();

	pr_info("+ portid:%d portState(%s)\n", 
		port->portid, chip_usb_state_string(port->usb_state));

	hiusb_port_phy_prepare(port, port->last_event_state.cable, port->last_event_state.prop);

	if (port->support_remote_wakeup) {
		keep_remote_wakeup = true;

		if (is_freeze)
			keep_remote_wakeup = false;

#ifdef CONFIG_SYS_PM_ULSR
		pr_info("%s: is_ulsr:%d\n", __func__, is_ulsr());
		if (is_ulsr())
			keep_remote_wakeup = false;
#endif

	pr_info("keep_remote_wakeup:%d power_down_flag:%d\n", keep_remote_wakeup, power_down_flag_is_setted);
	}

	if (keep_remote_wakeup) {
		pr_info("resume with perifix power on\n");
		ret = hiusb_usb2phy_resume(port->usb2_phy);
		if (ret) {
			pr_err("usb2_phy_resume failed %d\n", ret);
			pr_err("- portid:%d ret:%d\n", port->portid, ret);
			return ret;
		}

		ret = hiusb_port_resume_client(port, keep_remote_wakeup);
		if (ret)
			pr_err("hiusb_port_resume_client failed %d\n", ret);
	} else {
		pr_info("resume with perifix power down, will reinit usb\n");

		ret = hiusb_port_usb2_init(port);
		if (ret) {
			pr_err("client init usb2 fail %d\n", ret);
			pr_err("- portid:%d ret:%d\n", port->portid, ret);
			return ret;
		}

		if (port->usb_state != USB_STATE_HIFI_USB) {
			ret = hiusb_port_usb3_init(port);
			if (ret){
				pr_err("client init usb3 fail %d\n", ret);
				goto init_usb3_fail;
			}
		}

		ret = hiusb_port_resume_client(port, keep_remote_wakeup);
		if (ret) {
			pr_err("hiusb_port_resume_client failed %d\n", ret);
			goto resume_client_fail;
		}
	}

#if defined(CONFIG_PM_SLEEP) && defined(CONFIG_PRODUCT_ARMPC)
	hiusb_port_dp_trigger_in_suspend_resume(port, false);
#endif

	pm_runtime_disable(dev);
	pm_runtime_set_active(dev);
	pm_runtime_enable(dev);

	pr_info("- portid:%d ret:%d\n", port->portid, ret);
	return ret;

resume_client_fail:
	if (port->usb_state != USB_STATE_HIFI_USB) {
		hiusb_port_usb3_exit(port);
	}
init_usb3_fail:
	hiusb_port_usb2_exit(port, 0);

	pr_info("- portid:%d ret:%d\n", port->portid, ret);
	return ret;
}

static int hiusb_port_common_suspend(struct device *dev, bool is_freeze)
{
	struct hiusb_port *port = to_hiusb_port(dev);
	int ret = 0;

	if (!port) {
		pr_err("port NULL\n");
		return -EBUSY;
	}

	pr_info("+ portid:%d\n", port->portid);

	if (port->usb_state == USB_STATE_DEVICE) {
		if (!hiusb_port_sleep_allowed(port)) {
			pr_err("not sleep allowed\n");
			return -EBUSY;
		}

		ret = hiusb_port_suspend_process(port, false, is_freeze);
		if (ret)
			pr_err("hiusb_port_suspend_process failed in device state\n");
	} else if (port->usb_state == USB_STATE_HOST || port->usb_state == USB_STATE_HIFI_USB) {
		if (port->usb_connect_suspend == 1) {
			pr_err("port state %d suspend with power on\n", port->usb_state);
			return 0;
		}
		if (port->support_sleep_on_low_power == 0) {
			pr_err("port state %d not sleep allowed\n", port->usb_state);
			return -EBUSY;
		}
		ret = hiusb_port_suspend_process(port, true, is_freeze);
		if (ret)
			pr_err("hiusb_port_suspend_process failed in host state\n");
	} else {
		pr_info("port in state %s\n",
			chip_usb_state_string(port->usb_state));
	}

	pr_info("- portid:%d\n", port->portid);
	return ret;
}

static int hiusb_port_common_resume(struct device *dev, bool is_freeze)
{
	struct hiusb_port *port = to_hiusb_port(dev);
	int ret = 0;

	if (!port) {
		pr_err("port NULL\n");
		return -EBUSY;
	}

	pr_info("+ portid:%d\n", port->portid);

	if (port->usb_state == USB_STATE_DEVICE) {
		ret = hiusb_port_resume_process(dev, port, is_freeze);
		if (ret)
			pr_err("hiusb_port_resume_process failed in device state\n");
	} else if (port->usb_state == USB_STATE_HOST  || port->usb_state == USB_STATE_HIFI_USB) {
		if (port->usb_connect_suspend == 1) {
			pr_err("port state %d resume with power on\n", port->usb_state);
			return 0;
		}

		ret = hiusb_port_resume_process(dev, port, is_freeze);
		if (ret)
			pr_err("hiusb_port_resume_process failed in host state\n");
	} else {
		pr_info("port in state %s\n",
			chip_usb_state_string(port->usb_state));
	}

	pr_info("- portid:%d\n", port->portid);
	return ret;
}

static int hiusb_port_suspend(struct device *dev)
{
	int ret;

	pr_info("+\n");
	ret = hiusb_port_common_suspend(dev, false);
	pr_info("- ret:%d\n", ret);

	return ret;
}

static int hiusb_port_resume(struct device *dev)
{
	int ret;

	pr_info("+\n");
	ret = hiusb_port_common_resume(dev, false);
	pr_info("- ret:%d\n", ret);

	return ret;
}

static int hiusb_port_freeze(struct device *dev)
{
	int ret;

	pr_info("+\n");
	ret = hiusb_port_common_suspend(dev, true);
	pr_info("- ret:%d\n", ret);

	return ret;
}

static int hiusb_port_thaw(struct device *dev)
{
	int ret;

	pr_info("+\n");
	ret = hiusb_port_common_resume(dev, true);
	pr_info("- ret:%d\n", ret);

	return ret;
}
#endif

const struct dev_pm_ops hiusb_port_dev_pm_ops = {
#ifdef CONFIG_PM_SLEEP
	.prepare = hiusb_port_prepare,
	.complete = hiusb_port_complete,
	.suspend = hiusb_port_suspend,
	.resume = hiusb_port_resume,
	.freeze = hiusb_port_freeze,
	.thaw = hiusb_port_thaw,
	.poweroff = hiusb_port_freeze,
	.restore = hiusb_port_thaw,
#endif
};

static const struct device_type g_hiusb_port_dev_type = {
	.name = "hiusb-port-dev",
	.groups = g_hiusb_port_groups,
	.release = hiusb_port_dev_release,
	.pm = &hiusb_port_dev_pm_ops,
};

static int hiusb_port_class_create_device(struct hiusb_port *port, struct device *parent)
{
	int ret;
	int protid;
	struct class *hiusb_port_class = hiusb_get_port_class();

	if (hiusb_port_class == NULL) {
		pr_err("hiusb_port_class has not been initialized\n");
		return -ENODEV;
	}

	device_initialize(&port->dev);
	port->dev.parent = parent;
	port->dev.fwnode = port->fwnode;
	port->dev.of_node = to_of_node(port->fwnode);

	if (of_property_read_u32(port->dev.of_node, "portid", &protid)){
		pr_info("get port id fail\n");
		return -ENODEV;
	}

	port->portid = protid;
	port->dev.class = hiusb_port_class;
	port->dev.type = &g_hiusb_port_dev_type;

	dev_set_name(&port->dev, "%s-port%d", dev_name(parent), protid);

	pr_info("%s do device register\n", dev_name(&port->dev));

	port->dev.dma_ops = parent->dma_ops;

	ret = device_add(&port->dev);
	if (ret) {
		pr_err("device register failed %d\n", ret);
		put_device(&port->dev);
		return ret;
	}
	dev_set_drvdata(&port->dev, port);

	port->dev.coherent_dma_mask = DMA_BIT_MASK(64);
	port->dev.dma_mask = &port->dev.coherent_dma_mask;

	ret = hiusb_runtime_enable(&port->dev);
	if (ret < 0) {
		put_device(&port->dev);
		pr_err("hiusb_runtime_enable failed %d\n", ret);
	}
	return ret;
}

static void hiusb_port_class_destroy_device(struct hiusb_port *port)
{
	if (!IS_ERR_OR_NULL(port)) {
		pm_runtime_put_sync(&port->dev);
		pm_runtime_disable(&port->dev);

		device_unregister(&port->dev);
	}
}

static void hiusb_port_data_initialize(struct hiusb_port *port)
{
	hiusb_set_port_state(port, USB_STATE_OFF);
	port->charger_type = CHARGER_TYPE_SDP;
	port->fake_charger_type = CHARGER_TYPE_NONE;
	port->eventmask = 0;
}

/* Initialize to device mode by default on fpga platform */
static void hiusb_port_state_init_in_fpga(struct hiusb_port *port)
{
	int ret;

	pr_info("start to set port%d type to device in fpga\n", port->portid);
	/* if vbus is on, detect charger type */
	if (pmic_get_vbus_status()) {
		port->charger_type = hiusb_port_detect_charger_type(port);
		hiusb_port_notify_charger_type(port);
	}

	if (hiusb_port_sleep_allowed(port))
		hiusb_port_wake_unlock(port);
	else
		hiusb_port_wake_lock(port);

	hiusb_usb2phy_setmode(port->usb2_phy, PHY_MODE_USB_DEVICE);
	ret = hiusb_port_start_client(port, HIUSB_CLIENT_TYPE_DEVICE);
	if (ret) {
		hiusb_port_wake_unlock(port);
		pr_err("hiusb start device client failed %d\n", ret);
		return;
	}

	if (hiusb_port_enumerate_allowed(port)) {
		/* start peripheral */
		ret = hiusb_port_ioctrl(port, HIUSB_CLIENT_IOCTRL_START_PERIPHERIAL, NULL);
		if (ret) {
			hiusb_port_wake_unlock(port);
			pr_err("start peripheral error\n");
			return;
		}
	}

	hiusb_set_port_state(port, USB_STATE_DEVICE);
	hiusb_set_device_in_state(&port->last_event_state);
	pr_info("set port%d to device in fpga\n", port->portid);
}

static void hiusb_port_state_init(struct hiusb_port *port)
{
	if (port->is_fpga_flag) {
		hiusb_port_state_init_in_fpga(port);
		return;
	}

	if (pmic_get_vbus_status() == 0) {
		port->charger_type = CHARGER_TYPE_NONE;
		/* vdp src may enable in fastboot/uefi, force disable */
		port->vdp_src_enable = 1;
		hiusb_port_set_vdp_src(port, false);
	}
	hiusb_set_device_out_state(&port->last_event_state);
}

struct hiusb_port *hiusb_port_create(struct device *parent, struct fwnode_handle *fwnode)
{
	int ret;
	struct hiusb_port *port = NULL;
	struct class *hiusb_port_class = hiusb_get_port_class();

	pr_info("+\n");

	if (!hiusb_port_class) {
		pr_err("hiusb_port_class has not been initialized\n");
		return ERR_PTR(-ENODEV);
	}

	if (!parent || !fwnode)
		return ERR_PTR(-EINVAL);

	port = kzalloc(sizeof(*port), GFP_KERNEL);
	if (!port)
		return ERR_PTR(-ENOMEM);

	mutex_init(&port->port_mutex);
	if (!g_lock_init) {
		spin_lock_init(&g_port_class_lock);
		g_lock_init = true;
	}
	port->fwnode = fwnode;

	INIT_LIST_HEAD(&port->client_list);

	ret = hiusb_port_class_create_device(port, parent);
	if (ret) {
		kfree(port);
		return ERR_PTR(ret);
	}

	ret = hiusb_port_get_res_form_fwnode(port);
	if (ret)
		goto free_port;

	port->event_manager = hiusb_event_manager_create(port);
	if (IS_ERR(port->event_manager)) {
		ret = PTR_ERR(port->event_manager);
		goto free_port;
	}

	port->wake_lock = wakeup_source_register(NULL, "usb_wake_lock");
	if (!port->wake_lock)
		pr_err("hiusb wakeup source register failed.\n");

	INIT_WORK(&port->speed_change_work, hiusb_port_speed_change_work);

	hiusb_bc_init(port);

	hiusb_port_data_initialize(port);

	ret = hiusb_port_create_attr_file(port);
	if (ret)
		goto free_port;

	hiusb_port_state_init(port);
	port->port_ready = true;

	pr_info("-\n");
	return port;

free_port:
	pm_runtime_put_sync(&port->dev);
	pm_runtime_disable(&port->dev);

	device_unregister(&port->dev);
	return ERR_PTR(ret);
}

void hiusb_port_destroy(struct hiusb_port *port)
{
	hiusb_port_class_destroy_device(port);
}

void hiusb_config_tzpc_by_portid(int portid)
{
	struct hiusb_port *port;

	port = hiusb_get_port_by_id(portid);
	if (!port) {
		pr_err("config tzpc fail, port id:%d\n", portid);
		return;
	}

	hiusb_port_tzpc_config(port);
}
