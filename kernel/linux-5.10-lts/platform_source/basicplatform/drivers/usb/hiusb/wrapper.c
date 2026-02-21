/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: wrapper for HIUSB
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

#include <linux/platform_drivers/usb/hiusb.h>
#include <platform_include/display/linux/dpu_dss_dp.h>
#if defined(CONFIG_HUAWEI_DSM) && defined(CONFIG_HW_DP_SOURCE)
#include <huawei_platform/dp_source/dp_dsm.h>
#endif
#ifdef CONFIG_TCPC_CLASS
#include <huawei_platform/usb/hw_pd_dev.h>
#endif
#include <chipset_common/hwusb/hw_usb.h>
#include <chipset_common/hwpower/common_module/power_event_ne.h>

#undef pr_fmt
#define pr_fmt(fmt) "[HIUSB_WRAPPER]%s: " fmt, __func__

#define HIUSB_DEV_TYPE_OFFSET 5
#define COMBOPHY_EVENT_WAIT_TIMEOUT msecs_to_jiffies(10500)

static char *event_type_string (enum otg_dev_event_type event)
{
	static char *usb_event_strings[] = {
		[CHARGER_CONNECT_EVENT]		= "CHARGER_CONNECT",
		[CHARGER_DISCONNECT_EVENT]	= "CHARGER_DISCONNECT",
		[ID_FALL_EVENT]			= "OTG_CONNECT",
		[ID_RISE_EVENT]			= "OTG_DISCONNECT",
		[DP_IN]				= "DP_IN",
		[DP_OUT]			= "DP_OUT",
		[START_HIFI_USB]		= "START_HIFI_USB",
		[START_HIFI_USB_RESET_VBUS]	= "START_HIFI_USB_RESET_VBUS",
		[STOP_HIFI_USB]			= "STOP_HIFI_USB",
		[STOP_HIFI_USB_RESET_VBUS]	= "STOP_HIFI_USB_RESET_VBUS",
		[HIFI_USB_HIBERNATE]		= "HIFI_USB_HIBERNATE",
		[HIFI_USB_WAKEUP]		= "HIFI_USB_WAKEUP",
		[DISABLE_USB3_PORT]		= "DISABLE_USB3_PORT",
		[SWITCH_CONTROLLER]		= "SWITCH_CONTROLLER",
		[NONE_EVENT]			= "NONE",
	};

	if (event > NONE_EVENT)
		return "illegal event";

	return usb_event_strings[event];
}

static enum tca_irq_type_e hiusb_get_irq_type(struct hiusb_port_state *state)
{
	if (state == NULL)
		return TCA_IRQ_MAX_NUM;

	return (enum tca_irq_type_e)state->prop.state.hpd;
}

enum typec_plug_orien_e hiusb_get_orien(struct hiusb_port_state *state)
{
	if (state == NULL)
		return TYPEC_ORIEN_POSITIVE;

	return (enum typec_plug_orien_e)state->prop.state.orien;
}

static enum otg_dev_event_type hiusb_get_dev_type(struct hiusb_port_state *state)
{
	if (state == NULL)
		return NONE_EVENT;

	if (state->prop.state.usb_role == HIUSB_PROP_USB_DEVICE) {
		if (state->cable.state.usb == HIUSB_CABLE_STATE_IN)
			return CHARGER_CONNECT_EVENT;
		else
			return CHARGER_DISCONNECT_EVENT;
	} else if (state->prop.state.usb_role == HIUSB_PROP_USB_HOST) {
		if (state->prop.state.hifi_state != HIUSB_PROP_HIFI_OFF) {
			return (enum otg_dev_event_type)(state->prop.state.hifi_state +
				HIUSB_DEV_TYPE_OFFSET);
		} else {
			if (state->cable.state.usb == HIUSB_CABLE_STATE_IN)
				return ID_FALL_EVENT;
			else
				return ID_RISE_EVENT;
		}
	} else {
		if (state->cable.state.dp == HIUSB_CABLE_STATE_IN)
			return DP_IN;
		else
			return DP_OUT;
	}
}

enum tcpc_mux_ctrl_type hiusb_get_mode_type(struct hiusb_port_state *state)
{
	if (state == NULL)
		return TCPC_NC;

	if (state->prop.state.usb_role != HIUSB_PROP_USB_OFF) {
		if (state->cable.state.usb == HIUSB_CABLE_STATE_IN) {
			if (state->prop.state.lane_num == HIUSB_PROP_DP_0LANE)
				return TCPC_USB31_CONNECTED;
			else if (state->prop.state.lane_num == HIUSB_PROP_DP_2LANE)
				return TCPC_USB31_AND_DP_2LINE;
			else
				return TCPC_DP;
		} else {
			return TCPC_NC;
		}
	} else {
		if (state->prop.state.lane_num == HIUSB_PROP_DP_4LANE)
			return TCPC_DP;
		else
			return TCPC_USB31_AND_DP_2LINE;
	}
}

bool hiusb_port_is_dp_mode(struct hiusb_port_state *state)
{
	if (state == NULL)
		return false;

	if (hiusb_get_mode_type(state) == TCPC_DP)
		return true;

	return false;
}

char *hiusb_get_dev_type_string(struct hiusb_port_state *state)
{
	if (state == NULL)
		return "invalid dev type";

	return event_type_string(hiusb_get_dev_type(state));
}

void hiusb_port_notify_dp_done(struct hiusb_port *port, enum tcpc_mux_ctrl_type mode_type, int ret)
{
	/* There is no PD on the FPGA */
	if (port && (port->is_fpga_flag != 0))
		return;

#if defined(CONFIG_TCPC_CLASS) && defined(CONFIG_CONTEXTHUB_PD)
	return dp_dfp_u_notify_dp_configuration_done(mode_type, ret);
#endif
}

void hiusb_combophy_event_watchdog(struct timer_list *t)
{
#if IS_ENABLED(CONFIG_TCPC_CLASS) && IS_ENABLED(CONFIG_CONTEXTHUB_PD)
	struct hiusb_event_manager *manager = from_timer(manager, t, event_timer);
	hiusb_port_notify_dp_done(manager->port, hiusb_get_mode_type(&manager->port->last_event_state), -ETIMEDOUT);
#endif
	pr_err("timeout for handle event\n");
}

static void hiusb_get_port_state_when_hpd_in(struct hiusb_port_state *state,
					enum tca_irq_type_e irq_type,
					enum tcpc_mux_ctrl_type mode_type,
					enum tca_device_type_e dev_type)
{
	state->cable.state.vbus = HIUSB_CABLE_STATE_IN;
	if (mode_type == TCPC_DP) {
		/* before dp_in event,we will receive otg_connect(TCPC_DP) event */
		if (dev_type == TCA_ID_FALL_EVENT) {
			state->cable.state.dp = HIUSB_CABLE_STATE_OUT;
			state->cable.state.usb = HIUSB_CABLE_STATE_IN;
		} else {
			state->cable.state.dp = HIUSB_CABLE_STATE_IN;
			state->cable.state.usb = HIUSB_CABLE_STATE_OUT;
		}
		state->prop.state.lane_num = HIUSB_PROP_DP_4LANE;
	} else if (mode_type == TCPC_USB31_AND_DP_2LINE) {
		if (dev_type == TCA_CHARGER_CONNECT_EVENT ||
			dev_type == TCA_ID_FALL_EVENT) {
			state->cable.state.dp = HIUSB_CABLE_STATE_OUT;
			state->cable.state.usb = HIUSB_CABLE_STATE_IN;
		} else if (dev_type == TCA_DP_IN) {
			state->cable.state.dp = HIUSB_CABLE_STATE_IN;
			state->cable.state.usb = HIUSB_CABLE_STATE_OUT;
		} else {
			pr_err("invalid dev type %d\n", dev_type);
		}
		state->prop.state.lane_num = HIUSB_PROP_DP_2LANE;
	} else if (mode_type == TCPC_USB31_CONNECTED) {
		state->cable.state.dp = HIUSB_CABLE_STATE_OUT;
		state->cable.state.usb = HIUSB_CABLE_STATE_IN;
		state->prop.state.lane_num = HIUSB_PROP_DP_0LANE;
	} else {
		pr_err("invalid mode type %d\n", mode_type);
	}
}

static void hiusb_get_port_state_from_event(struct hiusb_port_state *state,
					enum tca_irq_type_e irq_type,
					enum tcpc_mux_ctrl_type mode_type,
					enum tca_device_type_e dev_type,
					enum typec_plug_orien_e typec_orien)
{
	if (irq_type == TCA_IRQ_HPD_OUT) {
		state->cable.state.dp = HIUSB_CABLE_STATE_OUT;
		state->cable.state.usb = HIUSB_CABLE_STATE_OUT;
		state->cable.state.vbus = HIUSB_CABLE_STATE_OUT;
		if (mode_type == TCPC_DP)
			state->prop.state.lane_num = HIUSB_PROP_DP_4LANE;
		else
			state->prop.state.lane_num = HIUSB_PROP_DP_2LANE;
	} else if (irq_type == TCA_IRQ_HPD_IN) {
		hiusb_get_port_state_when_hpd_in(state, irq_type, mode_type, dev_type);
	} else if (irq_type == TCA_IRQ_SHORT) {
		state->cable.state.dp = HIUSB_CABLE_STATE_IN;
		state->cable.state.usb = HIUSB_CABLE_STATE_OUT;
		state->cable.state.vbus = HIUSB_CABLE_STATE_IN;
		if (mode_type == TCPC_USB31_AND_DP_2LINE)
			state->prop.state.lane_num = HIUSB_PROP_DP_2LANE;
		else
			state->prop.state.lane_num = HIUSB_PROP_DP_4LANE;
	} else {
		pr_err("invalid irq_type %d\n", irq_type);
	}

	if (dev_type == TCA_CHARGER_CONNECT_EVENT ||
		dev_type == TCA_CHARGER_DISCONNECT_EVENT) {
		state->prop.state.power_role = HIUSB_PROP_FEILD_VBUS_INPUT;
		state->prop.state.usb_role = HIUSB_PROP_USB_DEVICE;
	} else if (dev_type == TCA_ID_FALL_EVENT ||
		dev_type == TCA_ID_RISE_EVENT) {
		state->prop.state.power_role = HIUSB_PROP_FEILD_VBUS_OUTPUT;
		state->prop.state.usb_role = HIUSB_PROP_USB_HOST;
	} else {
		state->prop.state.power_role = HIUSB_PROP_FEILD_VBUS_OUTPUT;
		state->prop.state.usb_role = HIUSB_PROP_USB_OFF;
	}

	state->prop.state.orien = typec_orien;
	state->prop.state.hpd = irq_type;
}

static const char *hiusb_irq_type_string(enum tca_irq_type_e irq_type)
{
	static const char * const irq_type_strings[] = {
		[TCA_IRQ_HPD_OUT] = "TCA_IRQ_HPD_OUT",
		[TCA_IRQ_HPD_IN] = "TCA_IRQ_HPD_IN",
		[TCA_IRQ_SHORT] = "TCA_IRQ_SHORT",
	};

	if (irq_type > TCA_IRQ_SHORT)
		return "illegal irq_type";

	return irq_type_strings[irq_type];
}

static const char *hiusb_mode_type_string(enum tcpc_mux_ctrl_type mode_type)
{
	static const char * const mode_type_strings[] = {
		[TCPC_NC] = "TCPC_NC",
		[TCPC_USB31_CONNECTED] = "TCPC_USB31_CONNECTED",
		[TCPC_DP] = "TCPC_DP",
		[TCPC_USB31_AND_DP_2LINE] = "TCPC_USB31_AND_DP_2LINE",
	};

	if (mode_type > TCPC_USB31_AND_DP_2LINE)
		return "illegal mode_type";

	return mode_type_strings[mode_type];
}

static const char *hiusb_dev_type_string(enum tca_device_type_e dev_type)
{
	static const char * const dev_type_strings[] = {
		[TCA_CHARGER_CONNECT_EVENT] = "TCA_CHARGER_CONNECT_EVENT",
		[TCA_CHARGER_DISCONNECT_EVENT] = "TCA_CHARGER_DISCONNECT_EVENT",
		[TCA_ID_FALL_EVENT] = "TCA_ID_FALL_EVENT",
		[TCA_ID_RISE_EVENT] = "TCA_ID_RISE_EVENT",
		[TCA_DP_OUT] = "TCA_DP_OUT",
		[TCA_DP_IN] = "TCA_DP_IN",
	};

	if (dev_type > TCA_DP_IN)
		return "illegal dev_type";

	return dev_type_strings[dev_type];
}

void hiusb_port_start_event_timer(struct hiusb_port *port)
{
	struct timer_list *event_timer = NULL;

	if (!port->event_manager) {
		pr_err("%s: event_manager is null\n", __func__);
		return;
	}

	event_timer = &port->event_manager->event_timer;
	pr_info("port %d start a event_timer 0x%pK\n", port->portid, event_timer);

	event_timer->expires = jiffies + COMBOPHY_EVENT_WAIT_TIMEOUT;
	(void)mod_timer(event_timer, event_timer->expires);
	port->event_manager->is_timer_start = true;
}

void hiusb_port_stop_event_timer(struct hiusb_port *port)
{
	struct timer_list *event_timer = &port->event_manager->event_timer;

	if (port->event_manager->is_timer_start) {
		pr_info("port %d stop a event_timer 0x%pK\n", port->portid, event_timer);

		(void)del_timer_sync(event_timer);
		port->event_manager->is_timer_start = false;
	}
}

int pd_event_notify_by_portid(int portid,
			enum tca_irq_type_e irq_type,
			enum tcpc_mux_ctrl_type mode_type,
			enum tca_device_type_e dev_type,
			enum typec_plug_orien_e typec_orien)
{
	int ret;
	enum tcpc_mux_ctrl_type cur_mode;
	struct hiusb_port_state state = {0};
	struct hiusb_port *port = NULL;

	port = hiusb_get_port_by_id(portid);
	if (port == NULL) {
		pr_err("invalid portid %d\n", portid);
		ret = -EINVAL;
		goto err_out;
	}

	if (!port->port_ready) {
		pr_err("%s: port not ready, portid %d.\n", __func__, portid);
		ret = -EINVAL;
		goto err_out;
	}

	if (irq_type >= TCA_IRQ_MAX_NUM || mode_type >= TCPC_MUX_MODE_MAX ||
		dev_type >= TCA_DEV_MAX || typec_orien >= TYPEC_ORIEN_MAX) {
		pr_err("%s: invalid event\n", __func__);
		ret = -EINVAL;
		goto err_out;
	}

	hiusb_get_port_state_from_event(&state, irq_type, mode_type,
					dev_type, typec_orien);
	cur_mode = hiusb_get_mode_type(&port->last_event_state);
	pr_info("Port[%d]IRQ[%s]MODEcur[%s]new[%s]DEV[%s]ORIEN[%d]\n",
		port->portid,
		hiusb_irq_type_string(irq_type),
		hiusb_mode_type_string(cur_mode),
		hiusb_mode_type_string(mode_type),
		hiusb_dev_type_string(dev_type),
		typec_orien);

#if defined(CONFIG_HUAWEI_DSM) && defined(CONFIG_HW_DP_SOURCE)
	dp_imonitor_set_pd_event(irq_type, cur_mode, mode_type,
				dev_type, typec_orien);
#endif

	/* start pd event timer */
	hiusb_port_start_event_timer(port);
	ret = hiusb_set_state(port, state);
	if (ret) {
		hiusb_port_stop_event_timer(port);
		goto err_out;
	}

	return 0;
err_out:
	pr_err("%s: return err %d\n", __func__, ret);
	hiusb_port_notify_dp_done(port, mode_type, ret);
	return ret;
}

int pd_event_notify(enum tca_irq_type_e irq_type,
			enum tcpc_mux_ctrl_type mode_type,
			enum tca_device_type_e dev_type,
			enum typec_plug_orien_e typec_orien)
{
	int ret;
	int portid;
	enum tcpc_mux_ctrl_type cur_mode;
	struct hiusb_port_state state = {0};
	struct hiusb_port *port = NULL;

	if (irq_type >= TCA_IRQ_MAX_NUM || mode_type >= TCPC_MUX_MODE_MAX ||
		dev_type >= TCA_DEV_MAX || typec_orien >= TYPEC_ORIEN_MAX) {
		pr_err("%s: invalid event\n", __func__);
		ret = -EINVAL;
		goto err_out;
	}

	portid = hiusb_get_default_portid();
	if (portid < 0 || portid >= HIUSB_PORT_MAXSIZE) {
		pr_info("%s: invalid default portid %d.\n", __func__, portid);
		ret = -EINVAL;
		goto err_out;
	}

	port = hiusb_get_port_by_id(portid);
	if (port == NULL) {
		pr_info("%s: get port failed, portid %d.\n", __func__, portid);
		ret = -EINVAL;
		goto err_out;
	}

	if (!port->port_ready) {
		pr_err("%s: port not ready, portid %d.\n", __func__, portid);
		ret = -EINVAL;
		goto err_out;
	}

	cur_mode = hiusb_get_mode_type(&port->last_event_state);
	pr_info("IRQ[%s]MODEcur[%s]new[%s]DEV[%s]ORIEN[%d]\n",
		hiusb_irq_type_string(irq_type),
		hiusb_mode_type_string(cur_mode),
		hiusb_mode_type_string(mode_type),
		hiusb_dev_type_string(dev_type),
		typec_orien);

#if defined(CONFIG_HUAWEI_DSM) && defined(CONFIG_HW_DP_SOURCE)
	dp_imonitor_set_pd_event(irq_type, cur_mode, mode_type,
				dev_type, typec_orien);
#endif
	hiusb_get_port_state_from_event(&state, irq_type, mode_type, dev_type, typec_orien);

	/* start pd event timer */
	hiusb_port_start_event_timer(port);
	ret = hiusb_set_state(port, state);
	if (ret) {
		hiusb_port_stop_event_timer(port);
		goto err_out;
	}

	return 0;
err_out:
	pr_err("%s: return err %d\n", __func__, ret);
	hiusb_port_notify_dp_done(port, mode_type, ret);
	return ret;
}

/*
	notify dp in suspend and resume process.
	port:          pointer to hiusb_port
	suspend_flag:  flag for suspend/resume
			true  -- suspend
			false -- resume
*/
int hiusb_port_dp_trigger_in_suspend_resume(struct hiusb_port *port, bool suspend_flag)
{
	int ret;
	enum tcpc_mux_ctrl_type curr_mode;
	enum typec_plug_orien_e curr_orien;
	enum tca_irq_type_e irq_type;

	pr_info("+\n");
	curr_mode = hiusb_get_mode_type(&port->last_event_state);
	pr_info("hiusb port dp trigger, cur_mode:%u\n", curr_mode);
	if (curr_mode == TCPC_DP || curr_mode == TCPC_USB31_AND_DP_2LINE) {
		if (suspend_flag == true)
			irq_type = TCA_IRQ_HPD_OUT;
		else
			irq_type = TCA_IRQ_HPD_IN;

		curr_orien = hiusb_get_orien(&port->last_event_state);

		ret = dpu_dptx_hpd_trigger(irq_type, curr_mode, curr_orien, port->portid);
		if (ret)
			pr_err("dptx_hpd_trigger failed, [%d].\n", ret);
	}

	pr_info("-\n");
	return 0;
}

int hiusb_port_dp_trigger(struct hiusb_port *port, struct hiusb_port_state *state)
{
	int ret;
	enum tca_irq_type_e irq_type;
	enum tcpc_mux_ctrl_type curr_mode;
	enum typec_plug_orien_e curr_orien;

	pr_info("+\n");
	curr_mode = hiusb_get_mode_type(state);
	pr_info("curr_mode 0x%x\n", curr_mode);
	if (curr_mode == TCPC_DP || curr_mode == TCPC_USB31_AND_DP_2LINE) {
		irq_type = hiusb_get_irq_type(state);
		curr_orien = hiusb_get_orien(state);

		pr_info("%s portid %d irq_type %d curr_orien %d curr_mode %d\n",
			__func__, port->portid, irq_type, curr_orien, curr_mode);
		ret = dpu_dptx_hpd_trigger(irq_type, curr_mode, curr_orien, port->portid);
		if (ret)
			pr_err("dptx_hpd_trigger failed, [%d].\n", ret);
	}

	pr_info("-\n");
	return 0;
}

void hiusb_port_speed_change_work(struct work_struct *work)
{
	struct hiusb_port *port = container_of(work,
				    struct hiusb_port, speed_change_work);
	pr_info("+\n");
	pr_info("device speed is %d\n", port->speed);

#ifdef CONFIG_HUAWEI_USB
	if ((port->speed != USB_CONNECT_HOST) &&
			(port->speed != USB_CONNECT_DCP))
		power_event_bnc_notify(POWER_BNT_HW_USB, POWER_NE_HW_USB_SPEED, &port->speed);
#endif
	pr_info("-\n");
}

void hiusb_port_notify_pd_vbus_ctrl(struct hiusb_port *port,
					enum chip_charger_type charger_type)
{
#ifdef CONFIG_TCPC_CLASS
	int portid;

	portid = hiusb_get_default_portid();
	if (portid < 0 || portid >= HIUSB_PORT_MAXSIZE) {
		pr_info("%s: invalid default portid %d.\n", __func__, portid);
		return;
	}

	if (portid == port->portid)
		pd_dpm_vbus_ctrl(charger_type);
#endif
}

int chip_usb_otg_event(enum otg_dev_event_type event_type)
{
	int portid;
	struct hiusb_port *port = NULL;

	if (event_type < CHARGER_CONNECT_EVENT || event_type > DP_IN) {
		pr_err("%s: invalid event_type %d.\n", __func__, event_type);
		return -EINVAL;
	}

	portid = hiusb_get_default_portid();
	if (portid < 0 || portid >= HIUSB_PORT_MAXSIZE) {
		pr_err("%s: invalid default portid %d.\n", __func__, portid);
		return -EINVAL;
	}

	port = hiusb_get_port_by_id(portid);
	if (port == NULL) {
		pr_err("%s: get port failed, portid %d.\n", __func__, portid);
		return -EINVAL;
	}

	switch (event_type) {
	case CHARGER_CONNECT_EVENT:
		return hiusb_device_start(port);
	case CHARGER_DISCONNECT_EVENT:
		return hiusb_device_stop(port);
	case ID_FALL_EVENT:
		return hiusb_host_start(port);
	case ID_RISE_EVENT:
		return hiusb_host_stop(port);
	case DP_OUT:
		return hiusb_dp_out(port);
	case DP_IN:
		return hiusb_dp_in(port);
	default:
		return -EINVAL;
	}
}
EXPORT_SYMBOL_GPL(chip_usb_otg_event);

static int hiusb_ports_start_fn(struct hiusb_port *port)
{
	int default_portid;

	if (!port) {
		pr_info("port is null\n");
		return -EINVAL;
	}

	pr_info("+:usb_state is 0x%x\n", port->usb_state);
	default_portid = hiusb_get_default_portid();
	/* if port is default mode, just return */
	if (port->portid == default_portid) {
		pr_info("default port no need switch mode\n");
		return 0;
	}

	/* set port mode to off */
	if (port->usb_state == USB_STATE_OFF)
		return hiusb_host_start(port);

	pr_info("-\n");
	return 0;
}

static int hiusb_ports_stop_fn(struct hiusb_port *port)
{
	int default_portid;

	if (!port){
		pr_info("port is null\n");
		return -EINVAL;
	}

	pr_info("+:usb_state is 0x%x\n", port->usb_state);
	default_portid = hiusb_get_default_portid();
	/* if port is default mode, just return */
	if (port->portid == default_portid) {
		pr_info("default port no need switch mode\n");
		return 0;
	}

	/* set port mode to off */
	if (port->usb_state == USB_STATE_HOST || port->usb_state == USB_STATE_HIFI_USB)
		return hiusb_host_stop(port);

	pr_info("-\n");
	return 0;
}

int usb_ports_mode_switch(u8 is_on)
{
	pr_info("+:is_on 0x%x\n", is_on);
	if (is_on)
		return hiusb_for_each_port(hiusb_ports_start_fn);
	else
		return hiusb_for_each_port(hiusb_ports_stop_fn);
}

void hiusb_pd_dpm_wakelock_ctrl(unsigned long event)
{
#ifdef CONFIG_TCPC_CLASS
	pd_dpm_wakelock_ctrl(event);
#endif
}

