/*
 * pd_process_evt_src.c
 *
 * Power Delivery Process Event For SRC
 *
 * Copyright (c) 2019 Huawei Technologies Co., Ltd.
 *
 * Copyright (C) 2016 Richtek Technology Corp.
 * Author: TH <tsunghan_tsai@richtek.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */


#define LOG_TAG "[evt_src]"

#include "include/pd_core.h"
#include "include/pd_tcpm.h"
#include "include/tcpci_event.h"
#include "include/pd_process_evt.h"
#include "include/pd_dpm_core.h"

/* PD Control MSG reactions */

DECL_PE_STATE_TRANSITION(PD_CTRL_MSG_GOOD_CRC) = {
	{ PE_SRC_TRANSITION_SUPPLY2, PE_SRC_READY },
	{ PE_SRC_SOFT_RESET, PE_SRC_SEND_CAPABILITIES },
	{ PE_DR_SRC_GIVE_SINK_CAP, PE_SRC_READY },
#ifdef CONFIG_KIRIN_PD_REV30
#ifdef CONFIG_KIRIN_PD_REV30_ALERT_LOCAL
	{ PE_SRC_SEND_SOURCE_ALERT, PE_SRC_READY },
#endif
#ifdef CONFIG_KIRIN_PD_REV30_STATUS_LOCAL
	{ PE_SRC_GIVE_SOURCE_STATUS, PE_SRC_READY },
#endif
#endif
	{ PE_SRC_SEND_NOT_SUPPORTED, PE_SRC_READY },
	{ PE_GIVE_MANUFACTURER_INFO, PE_SRC_READY },
};
DECL_PE_STATE_REACTION(PD_CTRL_MSG_GOOD_CRC);

DECL_PE_STATE_TRANSITION(PD_CTRL_MSG_GET_SOURCE_CAP) = {
	{ PE_SRC_READY, PE_SRC_SEND_CAPABILITIES },
};
DECL_PE_STATE_REACTION(PD_CTRL_MSG_GET_SOURCE_CAP);

DECL_PE_STATE_TRANSITION(PD_CTRL_MSG_ACCEPT) = {
	{PE_SRC_SEND_SOFT_RESET, PE_SRC_SEND_CAPABILITIES },
};
DECL_PE_STATE_REACTION(PD_CTRL_MSG_ACCEPT);

DECL_PE_STATE_TRANSITION(PD_CTRL_MSG_REJECT) = {
	{ PE_DR_SRC_GET_SOURCE_CAP, PE_SRC_READY },
};
DECL_PE_STATE_REACTION(PD_CTRL_MSG_REJECT);

/* PD Data MSG reactions */

DECL_PE_STATE_TRANSITION(PD_DATA_MSG_REQUEST) = {
	{ PE_SRC_SEND_CAPABILITIES, PE_SRC_NEGOTIATE_CAPABILITIES },
	{ PE_SRC_READY, PE_SRC_NEGOTIATE_CAPABILITIES },
};
DECL_PE_STATE_REACTION(PD_DATA_MSG_REQUEST);

DECL_PE_STATE_TRANSITION(PD_DATA_MSG_SOURCE_CAP) = {
	{ PE_DR_SRC_GET_SOURCE_CAP, PE_SRC_READY },
};
DECL_PE_STATE_REACTION(PD_DATA_MSG_SOURCE_CAP);

DECL_PE_STATE_TRANSITION(PD_DATA_MSG_SINK_CAP) = {
	{ PE_SRC_GET_SINK_CAP, PE_SRC_READY },
};
DECL_PE_STATE_REACTION(PD_DATA_MSG_SINK_CAP);

/* DPM Event reactions */

DECL_PE_STATE_TRANSITION(PD_DPM_MSG_ACK) = {
	{ PE_SRC_NEGOTIATE_CAPABILITIES, PE_SRC_TRANSITION_SUPPLY },

#ifdef CONFIG_USB_PD_SRC_STARTUP_DISCOVER_ID
	{ PE_SRC_STARTUP, PE_SRC_SEND_CAPABILITIES },
#endif /*  CONFIG_USB_PD_SRC_STARTUP_DISCOVER_ID */
};
DECL_PE_STATE_REACTION(PD_DPM_MSG_ACK);

DECL_PE_STATE_TRANSITION(PD_DPM_MSG_NAK) = {
	{ PE_SRC_NEGOTIATE_CAPABILITIES, PE_SRC_CAPABILITY_RESPONSE },
};
DECL_PE_STATE_REACTION(PD_DPM_MSG_NAK);

DECL_PE_STATE_TRANSITION(PD_DPM_MSG_CAP_CHANGED) = {
	{ PE_SRC_READY, PE_SRC_SEND_CAPABILITIES },
	{ PE_SRC_WAIT_NEW_CAPABILITIES, PE_SRC_SEND_CAPABILITIES },
};
DECL_PE_STATE_REACTION(PD_DPM_MSG_CAP_CHANGED);

/* HW Event reactions */

DECL_PE_STATE_TRANSITION(PD_HW_MSG_TX_FAILED) = {
	{ PE_SRC_SOFT_RESET, PE_SRC_HARD_RESET },
	{ PE_SRC_SEND_SOFT_RESET, PE_SRC_HARD_RESET },

#ifdef CONFIG_PD_DFP_RESET_CABLE
	{ PE_DFP_CBL_SEND_SOFT_RESET, PE_SRC_READY },
#endif
};
DECL_PE_STATE_REACTION(PD_HW_MSG_TX_FAILED);

DECL_PE_STATE_TRANSITION(PD_HW_VBUS_STABLE) = {
	{ PE_SRC_TRANSITION_SUPPLY, PE_SRC_TRANSITION_SUPPLY2 },
};
DECL_PE_STATE_REACTION(PD_HW_VBUS_STABLE);

/* PE Event reactions */

DECL_PE_STATE_TRANSITION(PD_PE_MSG_RESET_PRL_COMPLETED) = {
	{ PE_SRC_STARTUP, PE_SRC_SEND_CAPABILITIES },
};
DECL_PE_STATE_REACTION(PD_PE_MSG_RESET_PRL_COMPLETED);

DECL_PE_STATE_TRANSITION(PD_PE_MSG_POWER_ROLE_AT_DEFAULT) = {
	{ PE_SRC_TRANSITION_TO_DEFAULT, PE_SRC_STARTUP },
};
DECL_PE_STATE_REACTION(PD_PE_MSG_POWER_ROLE_AT_DEFAULT);

DECL_PE_STATE_TRANSITION(PD_PE_MSG_IDLE) = {
	{ PE_IDLE1, PE_IDLE2 },
};
DECL_PE_STATE_REACTION(PD_PE_MSG_IDLE);
/* Timer Event reactions */

DECL_PE_STATE_TRANSITION(PD_TIMER_SENDER_RESPONSE) = {
	{ PE_SRC_SEND_CAPABILITIES, PE_SRC_HARD_RESET },
	{ PE_SRC_SEND_SOFT_RESET, PE_SRC_HARD_RESET },

	{ PE_SRC_GET_SINK_CAP, PE_SRC_READY },

#ifdef CONFIG_PD_DFP_RESET_CABLE
	{ PE_DFP_CBL_SEND_SOFT_RESET, PE_SRC_READY },
#endif
	{ PE_DR_SRC_GET_SOURCE_CAP, PE_SRC_READY },
};
DECL_PE_STATE_REACTION(PD_TIMER_SENDER_RESPONSE);

DECL_PE_STATE_TRANSITION(PD_TIMER_PS_HARD_RESET) = {
	{ PE_SRC_HARD_RESET, PE_SRC_TRANSITION_TO_DEFAULT },
	{ PE_SRC_HARD_RESET_RECEIVED, PE_SRC_TRANSITION_TO_DEFAULT },
};
DECL_PE_STATE_REACTION(PD_TIMER_PS_HARD_RESET);

DECL_PE_STATE_TRANSITION(PD_TIMER_BIST_CONT_MODE) = {
	{ PE_BIST_CARRIER_MODE_2, PE_SRC_READY },
};
DECL_PE_STATE_REACTION(PD_TIMER_BIST_CONT_MODE);

DECL_PE_STATE_TRANSITION(PD_TIMER_SOURCE_START) = {
	{ PE_SRC_STARTUP, PE_SRC_SEND_CAPABILITIES },
};
DECL_PE_STATE_REACTION(PD_TIMER_SOURCE_START);

/*
 * [BLOCK] Porcess Ctrl MSG
 */

static bool pd_process_ctrl_msg_good_crc(pd_port_t *pd_port,
		pd_event_t *pd_event)

{
	D("+-\n");
	switch (pd_port->pe_state_curr) {
	case PE_SRC_SEND_SOFT_RESET:
	case PE_SRC_GET_SINK_CAP:
	case PE_DR_SRC_GET_SOURCE_CAP:
		/* 
		 * If other-end device send a get-source-cap,
		 * then must send reqeust,
		 * otherwise, we issue a hardreset
		 */
		pd_enable_timer(pd_port, PD_TIMER_SENDER_RESPONSE);
		return false;

	case PE_SRC_SEND_CAPABILITIES:
		pd_disable_timer(pd_port, PD_TIMER_NO_RESPONSE);
		pd_port->cap_counter = 0;
		pd_port->hard_reset_counter = 0;
		hisi_pd_notify_pe_hard_reset_completed(pd_port);
		pd_enable_timer(pd_port, PD_TIMER_SENDER_RESPONSE);
		return false;

	case PE_SRC_CAPABILITY_RESPONSE:
		if (!pd_port->explicit_contract)
			PE_TRANSIT_STATE(pd_port, PE_SRC_WAIT_NEW_CAPABILITIES);
		else if (pd_port->invalid_contract)
			PE_TRANSIT_STATE(pd_port, PE_SRC_HARD_RESET);
		else
			PE_TRANSIT_STATE(pd_port, PE_SRC_READY);
		return true;

#ifdef CONFIG_KIRIN_PD_REV30_SRC_CAP_EXT_LOCAL
	case PE_SRC_GIVE_SOURCE_CAP_EXT:
		PE_TRANSIT_STATE(pd_port, PE_SRC_READY);
		return true;
#endif
	default:
		return PE_MAKE_STATE_TRANSIT(PD_CTRL_MSG_GOOD_CRC);
	}
}

static bool pd_process_ctrl_msg_get_sink_cap(pd_port_t *pd_port,
		pd_event_t *pd_event, uint8_t next)
{
	D("+\n");
	if (pd_port->pe_state_curr != PE_SRC_READY)
		return false;

	if (pd_port->dpm_caps & DPM_CAP_LOCAL_DR_POWER) {
		PE_TRANSIT_STATE(pd_port, next);
		return true;
	}

	hisi_pd_send_ctrl_msg(pd_port, TCPC_TX_SOP, PD_CTRL_REJECT);
	D("-\n");
	return false;
}

static bool pd_process_ctrl_msg_soft_reset(pd_port_t *pd_port,
		pd_event_t *pd_event)
{
	if (!pd_port->during_swap) {
		PE_TRANSIT_STATE(pd_port, PE_SRC_SOFT_RESET);
		return true;
	}

	return false;
}

static void pd_process_ctrl_msg_port_partner_first(pd_port_t *pd_port,
		pd_event_t *pd_event)
{
#ifdef CONFIG_USB_PD_PARTNER_CTRL_MSG_FIRST
	switch (pd_port->pe_state_curr) {
	case PE_SRC_GET_SINK_CAP:
	case PE_DR_SRC_GET_SOURCE_CAP:
		if (pd_event->msg >= PD_CTRL_GET_SOURCE_CAP &&
				pd_event->msg <= PD_CTRL_VCONN_SWAP) {
			D("Port Partner Request First\n");
			pd_port->pe_state_curr = PE_SRC_READY;
			pd_disable_timer(pd_port, PD_TIMER_SENDER_RESPONSE);
		}
		break;

	default:
		break;
	}
#endif
}

static bool pd_process_ctrl_msg(pd_port_t *pd_port, pd_event_t *pd_event)

{
	bool ret = false;

	D("+\n");
	pd_process_ctrl_msg_port_partner_first(pd_port, pd_event);

	switch (pd_event->msg) {
	case PD_CTRL_GOOD_CRC:
		return pd_process_ctrl_msg_good_crc(pd_port, pd_event);

	case PD_CTRL_ACCEPT:
		ret = PE_MAKE_STATE_TRANSIT(PD_CTRL_MSG_ACCEPT);
		break;

	case PD_CTRL_REJECT:
		ret = PE_MAKE_STATE_TRANSIT(PD_CTRL_MSG_REJECT);
		break;

	case PD_CTRL_GET_SOURCE_CAP:
		ret = PE_MAKE_STATE_TRANSIT(PD_CTRL_MSG_GET_SOURCE_CAP);
		break;

	case PD_CTRL_GET_SINK_CAP:
		ret = pd_process_ctrl_msg_get_sink_cap(pd_port, pd_event,
			PE_DR_SRC_GIVE_SINK_CAP);
		break;

	/* Swap */
	case PD_CTRL_DR_SWAP:
		ret = hisi_pd_process_ctrl_msg_dr_swap(pd_port, pd_event);
		break;

	case PD_CTRL_PR_SWAP:
		ret = hisi_pd_process_ctrl_msg_pr_swap(pd_port, pd_event);
		break;

	case PD_CTRL_VCONN_SWAP:
		ret = hisi_pd_process_ctrl_msg_vconn_swap(pd_port, pd_event);
		break;

	/* SoftReset */
	case PD_CTRL_SOFT_RESET:
		ret = pd_process_ctrl_msg_soft_reset(pd_port, pd_event);
		break;

	/* Ignore */
	case PD_CTRL_PING:
		hisi_pd_notify_pe_recv_ping_event(pd_port);
		break;

	case PD_CTRL_PS_RDY:
	case PD_CTRL_GOTO_MIN:
	case PD_CTRL_WAIT:
		break;

#ifdef CONFIG_KIRIN_PD_REV30
	case PD_CTRL_FR_SWAP:
		return true;

	case PD_CTRL_NOT_SUPPORTED:
		if (PE_MAKE_STATE_TRANSIT_SINGLE(
			PE_SRC_READY, PE_SRC_NOT_SUPPORTED_RECEIVED))
			return true;
		break;

#ifdef CONFIG_KIRIN_PD_REV30_SRC_CAP_EXT_LOCAL
	case PD_CTRL_GET_SOURCE_CAP_EXT:
		if (PE_MAKE_STATE_TRANSIT_SINGLE(
			PE_SRC_READY, PE_SRC_GIVE_SOURCE_CAP_EXT))
			return true;
		break;
#endif

#ifdef CONFIG_KIRIN_PD_REV30_STATUS_LOCAL
	case PD_CTRL_GET_STATUS:
		if (PE_MAKE_STATE_TRANSIT_SINGLE(
			PE_SRC_READY, PE_SRC_GIVE_SOURCE_STATUS))
			return true;
		break;
#endif

#ifdef CONFIG_KIRIN_PD_REV30_PPS_SOURCE
	case PD_CTRL_GET_PPS_STATUS:
		if (PE_MAKE_STATE_TRANSIT_SINGLE(
			PE_SRC_READY, PE_SRC_GIVE_PPS_STATUS))
			return true;
		break;
#endif
#ifdef CONFIG_KIRIN_PD_REV30_COUNTRY_CODE_LOCAL
	case PD_CTRL_GET_COUNTRY_CODE:
		if (PE_MAKE_STATE_TRANSIT_SINGLE(
			PE_SRC_READY, PE_GIVE_COUNTRY_CODES))
			return true;
		break;
#endif

	case PD_CTRL_GET_REVISION:
		if (PE_MAKE_STATE_TRANSIT_SINGLE(
			PE_SRC_READY, PE_GIVE_REVISION))
			return true;
		break;
	case PD_CTRL_GET_SINK_CAP_EXT:
		if (pd_process_ctrl_msg_get_sink_cap(pd_port, pd_event,
			PE_DR_SRC_GIVE_SINK_CAP_EXT))
			return true;
		break;
#endif
	default:
		break;
	}

	if (ret == false)
		ret = hisi_pd_process_protocol_error(pd_port, pd_event);

	D("-\n");
	return ret;
}

/*
 * [BLOCK] Porcess Data MSG
 */

static bool pd_process_data_msg(pd_port_t *pd_port, pd_event_t *pd_event)

{
	bool ret = false;

	D("+\n");
	switch (pd_event->msg) {
	case PD_DATA_SOURCE_CAP:
		ret = PE_MAKE_STATE_TRANSIT(PD_DATA_MSG_SOURCE_CAP);
		break;

	case PD_DATA_SINK_CAP:
		ret = PE_MAKE_STATE_TRANSIT(PD_DATA_MSG_SINK_CAP);
		break;

	case PD_DATA_BIST:
		ret = hisi_pd_process_data_msg_bist(pd_port, pd_event);
		break;

	case PD_DATA_REQUEST:
		ret = PE_MAKE_STATE_TRANSIT(PD_DATA_MSG_REQUEST);
		break;

#ifdef CONFIG_KIRIN_PD_REV30
#ifdef CONFIG_KIRIN_PD_REV30_ALERT_REMOTE
	case PD_DATA_ALERT:
		if (PE_MAKE_STATE_TRANSIT_SINGLE(
			PE_SRC_READY, PE_SRC_SINK_ALERT_RECEIVED))
			return true;
		break;
#endif

#ifdef CONFIG_KIRIN_PD_REV30_MFRS_INFO_LOCAL
	case PD_DATA_GET_COUNTRY_INFO:
		if (PE_MAKE_STATE_TRANSIT_SINGLE(
			PE_SRC_READY, PE_GIVE_COUNTRY_INFO))
			return true;
		break;
#endif

	case PD_DATA_REVISION:
		ret = PE_MAKE_STATE_TRANSIT_SINGLE(
			PE_GET_REVISION, PE_SRC_READY);
		break;
#ifdef CONFIG_KIRIN_PD_REV30_BAT_STATUS_REMOTE
	case PD_DATA_BAT_STATUS:
		ret = PE_MAKE_STATE_TRANSIT_SINGLE(
			PE_GET_BATTERY_STATUS, PE_SRC_READY);
		break;
#endif
#endif

	case PD_DATA_VENDOR_DEF:
		return false;

	default:
		break;
	}

	if (!ret)
		ret = hisi_pd_process_protocol_error(pd_port, pd_event);

	D("-\n");
	return ret;
}

/*
 * [BLOCK] Porcess Extend MSG
 */
#ifdef CONFIG_KIRIN_PD_REV30
static inline bool pd_process_ext_msg(
		pd_port_t *pd_port, pd_event_t *pd_event)
{
	switch (pd_event->msg) {

#ifdef CONFIG_KIRIN_PD_REV30_SRC_CAP_EXT_REMOTE
	case PD_EXT_SOURCE_CAP_EXT:
		if (PE_MAKE_STATE_TRANSIT_SINGLE(
			PE_DR_SRC_GET_SOURCE_CAP_EXT, PE_SRC_READY))
			return true;
		break;
#endif

#ifdef CONFIG_KIRIN_PD_REV30_STATUS_REMOTE
	case PD_EXT_STATUS:
		if (PE_MAKE_STATE_TRANSIT_SINGLE(
			PE_SRC_GET_SINK_STATUS, PE_SRC_READY))
			return true;
		break;
#endif

	case PD_EXT_SINK_CAP_EXT:
		if (PE_MAKE_STATE_TRANSIT_SINGLE(
			PE_SRC_GET_SINK_CAP_EXT, PE_SRC_READY))
			return true;
		break;

#ifdef CONFIG_KIRIN_PD_REV30_BAT_CAP_LOCAL
	case PD_EXT_GET_BAT_CAP:
		if (PE_MAKE_STATE_TRANSIT_SINGLE(
			PE_SRC_READY, PE_GIVE_BATTERY_CAP))
			return true;
		break;
#endif

#ifdef CONFIG_KIRIN_PD_REV30_BAT_STATUS_LOCAL
	case PD_EXT_GET_BAT_STATUS:
		if (PE_MAKE_STATE_TRANSIT_SINGLE(
			PE_SRC_READY, PE_GIVE_BATTERY_STATUS))
			return true;
		break;
#endif

#ifdef CONFIG_KIRIN_PD_REV30_BAT_CAP_REMOTE
	case PD_EXT_BAT_CAP:
		if (PE_MAKE_STATE_TRANSIT_SINGLE(
			PE_GET_BATTERY_CAP, PE_SRC_READY))
			return true;
		break;
#endif

#ifdef CONFIG_KIRIN_PD_REV30_MFRS_INFO_LOCAL
	case PD_EXT_GET_MFR_INFO:
		if (PE_MAKE_STATE_TRANSIT_SINGLE(
			PE_SRC_READY, PE_GIVE_MANUFACTURER_INFO))
			return true;
		break;
#endif

#ifdef CONFIG_KIRIN_PD_REV30_MFRS_INFO_REMOTE
	case PD_EXT_MFR_INFO:
		if (PE_MAKE_STATE_TRANSIT_SINGLE(
			PE_GET_MANUFACTURER_INFO, PE_SRC_READY))
			return true;
		break;
#endif

#ifdef CONFIG_KIRIN_PD_REV30_COUNTRY_INFO_REMOTE
	case PD_EXT_COUNTRY_INFO:
		if (PE_MAKE_STATE_TRANSIT_SINGLE(
			PE_GET_COUNTRY_INFO, PE_SRC_READY))
			return true;
		break;
#endif

#ifdef CONFIG_KIRIN_PD_REV30_COUNTRY_CODE_REMOTE
	case PD_EXT_COUNTRY_CODES:
		if (PE_MAKE_STATE_TRANSIT_SINGLE(
			PE_GET_COUNTRY_CODES, PE_SRC_READY))
			return true;
		break;
#endif
	case PD_EXT_SEC_REQUEST:
	case PD_EXT_SEC_RESPONSE:
	case PD_EXT_FW_UPDATE_REQUEST:
	case PD_EXT_FW_UPDATE_RESPONSE:
		if (PE_MAKE_STATE_TRANSIT_SINGLE(
			PE_SRC_READY, PE_SRC_SEND_NOT_SUPPORTED))
			return true;
		break;
	default:
		I("unknow ext msg: %u\n", pd_event->msg);
		break;
	}

	return hisi_pd_process_protocol_error(pd_port, pd_event);
}

#endif

/*
 * [BLOCK] Porcess DPM MSG
 */

static bool pd_process_dpm_msg(pd_port_t *pd_port, pd_event_t *pd_event)
{
	bool ret = false;

	switch (pd_event->msg) {
	case PD_DPM_ACK:
		ret = PE_MAKE_STATE_TRANSIT(PD_DPM_MSG_ACK);
		break;

	case PD_DPM_NAK:
		ret = PE_MAKE_STATE_TRANSIT(PD_DPM_MSG_NAK);
		break;

	case PD_DPM_CAP_CHANGED:
		ret = PE_MAKE_STATE_TRANSIT(PD_DPM_MSG_CAP_CHANGED);
		break;

	case PD_DPM_ERROR_RECOVERY:
		PE_TRANSIT_STATE(pd_port, PE_ERROR_RECOVERY);
		return true;

#ifdef CONFIG_KIRIN_PD_REV30
	case PD_DPM_NOT_SUPPORT:
		if (pd_check_rev30(pd_port)) {
			PE_TRANSIT_STATE(pd_port, PE_VDM_NOT_SUPPORTED);
			return true;
		}
		break;
#endif
	default:
		break;
	}

	return ret;
}

/*
 * [BLOCK] Porcess HW MSG
 */

static bool pd_process_hw_msg_vbus_present(
		pd_port_t *pd_port, pd_event_t *pd_event)
{
	D("+\n");
	switch (pd_port->pe_state_curr) {
	case PE_SRC_STARTUP:
		pd_enable_timer(pd_port, PD_TIMER_SOURCE_START);
		break;

	case PE_SRC_TRANSITION_TO_DEFAULT:
		pd_put_pe_event(pd_port, PD_PE_POWER_ROLE_AT_DEFAULT);
		break;

	default:
		break;
	}

	D("-\n");
	return false;
}

static bool pd_process_hw_msg_tx_failed(
		pd_port_t *pd_port, pd_event_t *pd_event)
{
	D("+-\n");
	if (pd_port->pe_state_curr == PE_SRC_SEND_CAPABILITIES) {
		if (pd_port->pd_connected) {
			D("PR_SWAP NoResp\n");
			return false;
		}

		PE_TRANSIT_STATE(pd_port, PE_SRC_DISCOVERY);
		return true;
	}

	return PE_MAKE_STATE_TRANSIT_FORCE(PD_HW_MSG_TX_FAILED,
			PE_SRC_SEND_SOFT_RESET);
}


static bool pd_process_hw_msg(
		pd_port_t *pd_port, pd_event_t *pd_event)
{
	bool ret = false;

	D("+\n");
	switch (pd_event->msg) {
	case PD_HW_CC_DETACHED:
		PE_TRANSIT_STATE(pd_port, PE_IDLE1);
		return true;

	case PD_HW_CC_ATTACHED:
		PE_TRANSIT_STATE(pd_port, PE_SRC_STARTUP);
		return true;

	case PD_HW_RECV_HARD_RESET:
		ret = hisi_pd_process_recv_hard_reset(
				pd_port, pd_event, PE_SRC_HARD_RESET_RECEIVED);
		break;

	case PD_HW_VBUS_PRESENT:
		ret = pd_process_hw_msg_vbus_present(pd_port, pd_event);
		break;

	case PD_HW_VBUS_SAFE0V:
		pd_enable_timer(pd_port, PD_TIMER_SRC_RECOVER);
		break;

	case PD_HW_VBUS_STABLE:
		ret = PE_MAKE_STATE_TRANSIT(PD_HW_VBUS_STABLE);
		break;

	case PD_HW_TX_FAILED:
		ret = pd_process_hw_msg_tx_failed(pd_port, pd_event);
		break;

	case PD_HW_VBUS_ABSENT:
		break;

	default:
		break;
	};

	D("-\n");
	return ret;
}

/*
 * [BLOCK] Porcess PE MSG
 */

static bool pd_process_pe_msg(pd_port_t *pd_port, pd_event_t *pd_event)
{
	bool ret = false;

	D("+\n");
	switch (pd_event->msg) {
	case PD_PE_RESET_PRL_COMPLETED:
		ret = PE_MAKE_STATE_TRANSIT(PD_PE_MSG_RESET_PRL_COMPLETED);
		break;

	case PD_PE_POWER_ROLE_AT_DEFAULT:
		ret = PE_MAKE_STATE_TRANSIT(PD_PE_MSG_POWER_ROLE_AT_DEFAULT);
		break;

	case PD_PE_IDLE:
		ret = PE_MAKE_STATE_TRANSIT(PD_PE_MSG_IDLE);
		break;

	default:
		break;
	}

	D("-\n");
	return ret;
}

/*
 * [BLOCK] Porcess Timer MSG
 */
static bool pd_process_timer_msg_source_start(
		pd_port_t *pd_port, pd_event_t *pd_event)
{
	D("+-\n");
#ifdef CONFIG_USB_PD_SRC_STARTUP_DISCOVER_ID
	/* dpm_flags decide discover cable id or not */
	if (pd_is_auto_discover_cable_id(pd_port)) {
		if (vdm_put_dpm_discover_cable_event(pd_port))
			/*
			 * send vdm, return true means sucess
			 * and waiting for dpm_ack event
			 */
			return false;
	}
#endif

	return PE_MAKE_STATE_TRANSIT(PD_TIMER_SOURCE_START);
}

static bool pd_process_timer_msg_source_cap(
		pd_port_t *pd_port, pd_event_t *pd_event)
{
	D("+\n");
	if (pd_port->pe_state_curr != PE_SRC_DISCOVERY)
		return false;

	if (pd_port->cap_counter <= PD_CAPS_COUNT)
		PE_TRANSIT_STATE(pd_port, PE_SRC_SEND_CAPABILITIES);
	else /* in this state, PD always not connected */
		PE_TRANSIT_STATE(pd_port, PE_SRC_DISABLED);

	D("-\n");
	return true;
}

static bool pd_process_timer_msg_no_response(
		pd_port_t *pd_port, pd_event_t *pd_event)
{
	D("+\n");
	if (pd_port->hard_reset_counter <= PD_HARD_RESET_COUNT)
		PE_TRANSIT_STATE(pd_port, PE_SRC_HARD_RESET);
	else if (pd_port->pd_prev_connected)
		PE_TRANSIT_STATE(pd_port, PE_ERROR_RECOVERY);
	else
		PE_TRANSIT_STATE(pd_port, PE_SRC_DISABLED);

	D("-\n");
	return true;
}

static inline void pd_process_timer_msg_dfp_flow_delay(pd_port_t *pd_port,
		pd_event_t *pd_event)
{
	if (pd_port->pe_state_curr == PE_SRC_READY)
		hisi_pd_dpm_notify_dfp_delay_done(pd_port, pd_event);
}

static inline void pd_process_timer_msg_ufp_flow_delay(pd_port_t *pd_port,
		pd_event_t *pd_event)
{
	if (pd_port->pe_state_curr == PE_SRC_READY)
		hisi_pd_dpm_notify_ufp_delay_done(pd_port, pd_event);
}

static bool pd_process_timer_msg(pd_port_t *pd_port, pd_event_t *pd_event)
{
	D("+\n");
	switch (pd_event->msg) {
	case PD_TIMER_BIST_CONT_MODE:
		return PE_MAKE_STATE_TRANSIT(PD_TIMER_BIST_CONT_MODE);

	case PD_TIMER_SOURCE_CAPABILITY:
		return pd_process_timer_msg_source_cap(pd_port, pd_event);

#ifndef CONFIG_USB_PD_DBG_IGRONE_TIMEOUT
	case PD_TIMER_SENDER_RESPONSE:
		return PE_MAKE_STATE_TRANSIT(PD_TIMER_SENDER_RESPONSE);
#endif

	case PD_TIMER_PS_HARD_RESET:
		return PE_MAKE_STATE_TRANSIT(PD_TIMER_PS_HARD_RESET);

	case PD_TIMER_SOURCE_START:
		return pd_process_timer_msg_source_start(pd_port, pd_event);

#ifndef CONFIG_USB_PD_DBG_IGRONE_TIMEOUT
	case PD_TIMER_NO_RESPONSE:
		return pd_process_timer_msg_no_response(pd_port, pd_event);
#endif

	case PD_TIMER_SOURCE_TRANSITION:
		if (pd_port->state_machine != PE_STATE_MACHINE_PR_SWAP)
			hisi_pd_dpm_src_transition_power(pd_port, pd_event);
		break;

#ifdef CONFIG_PD_DISCOVER_CABLE_ID
	case PD_TIMER_DISCOVER_ID:
		vdm_put_dpm_discover_cable_event(pd_port);
		break;
#endif /* CONFIG_PD_DISCOVER_CABLE_ID */

	case PD_TIMER_SRC_RECOVER:
		pd_dpm_source_vbus(pd_port, true);
		hisi_pd_enable_vbus_valid_detection(pd_port, true);
		break;

#ifdef CONFIG_USB_PD_DFP_FLOW_DELAY
	case PD_TIMER_DFP_FLOW_DELAY:
		pd_process_timer_msg_dfp_flow_delay(pd_port, pd_event);
		break;
#endif /* CONFIG_USB_PD_DFP_FLOW_DELAY */

#ifdef CONFIG_USB_PD_UFP_FLOW_DELAY
	case PD_TIMER_UFP_FLOW_DELAY:
		pd_process_timer_msg_ufp_flow_delay(pd_port, pd_event);
		break;
#endif /* CONFIG_USB_PD_UFP_FLOW_DELAY */

#ifdef CONFIG_KIRIN_PD_REV30_COLLISION_AVOID
	case PD_TIMER_SINK_TX:
		if (pd_port->pd_traffic_control == PD_SINK_TX_NG)
			pd_port->pd_traffic_control = PD_SOURCE_TX_OK;

#ifdef CONFIG_KIRIN_PD_REV30_SRC_FLOW_DELAY_STARTUP
		if (pd_port->pd_traffic_control == PD_SOURCE_TX_START)
			pd_port->pd_traffic_control = PD_SINK_TX_OK;
#endif
		break;
#endif

#ifdef CONFIG_KIRIN_PD_REV30
	case PD_TIMER_CK_NOT_SUPPORTED:
		I("Timer: Not Supported\n");
		return PE_MAKE_STATE_TRANSIT_SINGLE(
			PE_SRC_CHUNK_RECEIVED, PE_SRC_SEND_NOT_SUPPORTED);
#endif

#ifdef CONFIG_KIRIN_PD_REV30_COLLISION_AVOID
	case PD_TIMER_DEFERRED_EVT:
		I("timer deferred evt\n");
		break;

#endif
	default:
		break;
	}

	D("-\n");
	return false;
}

/*
 * [BLOCK] Process Policy Engine's SRC Message
 */

bool hisi_pd_process_event_src(pd_port_t *pd_port, pd_event_t *pd_event)
{
	D("\n");
	switch (pd_event->event_type) {
	case PD_EVT_CTRL_MSG:
		return pd_process_ctrl_msg(pd_port, pd_event);

	case PD_EVT_DATA_MSG:
		return pd_process_data_msg(pd_port, pd_event);

#ifdef CONFIG_KIRIN_PD_REV30
	case PD_EVT_EXT_MSG:
		return pd_process_ext_msg(pd_port, pd_event);
#endif

	case PD_EVT_DPM_MSG:
		return pd_process_dpm_msg(pd_port, pd_event);

	case PD_EVT_HW_MSG:
		return pd_process_hw_msg(pd_port, pd_event);

	case PD_EVT_PE_MSG:
		return pd_process_pe_msg(pd_port, pd_event);

	case PD_EVT_TIMER_MSG:
		return pd_process_timer_msg(pd_port, pd_event);

	default:
		return false;
	}
	D("-\n");
}
