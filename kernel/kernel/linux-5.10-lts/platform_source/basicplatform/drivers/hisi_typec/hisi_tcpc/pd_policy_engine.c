/*
 * Copyright (c) 2020 Huawei Technologies Co., Ltd.
 *
 * Copyright (C) 2016 Richtek Technology Corp.
 * Author: TH <tsunghan_tsai@richtek.com>
 *
 * Power Delivery Policy Engine Driver
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

#define LOG_TAG "[PE]"

#include "include/pd_core.h"
#include "include/pd_tcpm.h"
#include "include/pd_dpm_core.h"
#include "include/tcpci.h"
#include "include/pd_process_evt.h"
#include "include/pd_policy_engine.h"

/* ---- Policy Engine State ---- */

static const char * const pe_state_name[] = {
	"PE_SRC_STARTUP",
	"PE_SRC_DISCOVERY",
	"PE_SRC_SEND_CAPABILITIES",
	"PE_SRC_NEGOTIATE_CAPABILITIES",
	"PE_SRC_TRANSITION_SUPPLY",
	"PE_SRC_TRANSITION_SUPPLY2",
	"PE_SRC_READY",
	"PE_SRC_DISABLED",
	"PE_SRC_CAPABILITY_RESPONSE",
	"PE_SRC_HARD_RESET",
	"PE_SRC_HARD_RESET_RECEIVED",
	"PE_SRC_TRANSITION_TO_DEFAULT",
	"PE_SRC_GET_SINK_CAP",
	"PE_SRC_WAIT_NEW_CAPABILITIES",

	"PE_SRC_SEND_SOFT_RESET",
	"PE_SRC_SOFT_RESET",
	"PE_SRC_PING",

#ifdef CONFIG_USB_PD_SRC_STARTUP_DISCOVER_ID
	"PE_SRC_VDM_IDENTITY_REQUEST",
	"PE_SRC_VDM_IDENTITY_ACKED",
	"PE_SRC_VDM_IDENTITY_NAKED",
#endif
#ifdef CONFIG_KIRIN_PD_REV30
	"PE_SRC_SEND_NOT_SUPPORTED",
	"PE_SRC_NOT_SUPPORTED_RECEIVED",
	"PE_SRC_CHUNK_RECEIVED",
#ifdef CONFIG_KIRIN_PD_REV30_ALERT_LOCAL
	"PE_SRC_SEND_SOURCE_ALERT",
#endif
#ifdef CONFIG_KIRIN_PD_REV30_ALERT_REMOTE
	"PE_SRC_SINK_ALERT_RECEIVED",
#endif
#ifdef CONFIG_KIRIN_PD_REV30_SRC_CAP_EXT_LOCAL
	"PE_SRC_GIVE_SOURCE_CAP_EXT",
#endif
#ifdef CONFIG_KIRIN_PD_REV30_STATUS_LOCAL
	"PE_SRC_GIVE_SOURCE_STATUS",
#endif
#ifdef CONFIG_KIRIN_PD_REV30_STATUS_REMOTE
	"PE_SRC_GET_SINK_STATUS",
#endif
#ifdef CONFIG_KIRIN_PD_REV30_PPS_SOURCE
	"PE_SRC_GIVE_PPS_STATUS",
#endif
	"PE_SRC_GET_SINK_CAP_EXT",
#endif

	"PE_SNK_STARTUP",
	"PE_SNK_DISCOVERY",
	"PE_SNK_WAIT_FOR_CAPABILITIES",
	"PE_SNK_EVALUATE_CAPABILITY",
	"PE_SNK_SELECT_CAPABILITY",
	"PE_SNK_TRANSITION_SINK",
	"PE_SNK_READY",
	"PE_SNK_HARD_RESET",
	"PE_SNK_TRANSITION_TO_DEFAULT",
	"PE_SNK_GIVE_SINK_CAP",
	"PE_SNK_GET_SOURCE_CAP",

	"PE_SNK_SEND_SOFT_RESET",
	"PE_SNK_SOFT_RESET",
#ifdef CONFIG_KIRIN_PD_REV30
	"PE_SNK_SEND_NOT_SUPPORTED",
	"PE_SNK_NOT_SUPPORTED_RECEIVED",
	"PE_SNK_CHUNK_RECEIVED",
#ifdef CONFIG_KIRIN_PD_REV30_ALERT_REMOTE
	"PE_SNK_SOURCE_ALERT_RECEIVED",
#endif
#ifdef CONFIG_KIRIN_PD_REV30_ALERT_LOCAL
	"PE_SNK_SEND_SINK_ALERT",
#endif
#ifdef CONFIG_KIRIN_PD_REV30_SRC_CAP_EXT_REMOTE
	"PE_SNK_GET_SOURCE_CAP_EXT",
#endif
#ifdef CONFIG_KIRIN_PD_REV30_STATUS_REMOTE
	"PE_SNK_GET_SOURCE_STATUS",
#endif
#ifdef CONFIG_KIRIN_PD_REV30_STATUS_LOCAL
	"PE_SNK_GIVE_SINK_STATUS",
#endif
#ifdef CONFIG_KIRIN_PD_REV30_PPS_SINK
	"PE_SNK_GET_PPS_STATUS",
#endif
	"PE_SNK_GIVE_SINK_CAP_EXT",
#endif

	"PE_DRS_DFP_UFP_EVALUATE_DR_SWAP",
	"PE_DRS_DFP_UFP_ACCEPT_DR_SWAP",
	"PE_DRS_DFP_UFP_CHANGE_TO_UFP",
	"PE_DRS_DFP_UFP_SEND_DR_SWAP",
	"PE_DRS_DFP_UFP_REJECT_DR_SWAP",

	"PE_DRS_UFP_DFP_EVALUATE_DR_SWAP",
	"PE_DRS_UFP_DFP_ACCEPT_DR_SWAP",
	"PE_DRS_UFP_DFP_CHANGE_TO_DFP",
	"PE_DRS_UFP_DFP_SEND_SWAP",
	"PE_DRS_UFP_DFP_REJECT_DR_SWAP",

	"PE_PRS_SRC_SNK_EVALUATE_PR_SWAP",
	"PE_PRS_SRC_SNK_ACCEPT_PR_SWAP",
	"PE_PRS_SRC_SNK_TRANSITION_TO_OFF",
	"PE_PRS_SRC_SNK_ASSERT_RD",
	"PE_PRS_SRC_SNK_WAIT_SOURCE_ON",
	"PE_PRS_SRC_SNK_SEND_SWAP",
	"PE_PRS_SRC_SNK_REJECT_PR_SWAP",

	"PE_PRS_SNK_SRC_EVALUATE_PR_SWAP",
	"PE_PRS_SNK_SRC_ACCEPT_PR_SWAP",
	"PE_PRS_SNK_SRC_TRANSITION_TO_OFF",
	"PE_PRS_SNK_SRC_ASSERT_RP",
	"PE_PRS_SNK_SRC_SOURCE_ON",
	"PE_PRS_SNK_SRC_SEND_PR_SWAP",
	"PE_PRS_SNK_SRC_REJECT_SWAP",

	"PE_DR_SRC_GET_SOURCE_CAP",
	"PE_DR_SRC_GIVE_SINK_CAP",
	"PE_DR_SNK_GET_SINK_CAP",
	"PE_DR_SNK_GIVE_SOURCE_CAP",

#ifdef CONFIG_KIRIN_PD_REV30
#ifdef CONFIG_KIRIN_PD_REV30_SRC_CAP_EXT_LOCAL
	"PE_DR_SNK_GIVE_SOURCE_CAP_EXT",
#endif
#ifdef CONFIG_KIRIN_PD_REV30_SRC_CAP_EXT_REMOTE
	"PE_DR_SRC_GET_SOURCE_CAP_EXT",
#endif
	"PE_DR_SRC_GIVE_SINK_CAP_EXT",
	"PE_DR_SNK_GET_SINK_CAP_EXT",
#endif

	"PE_VCS_SEND_SWAP",
	"PE_VCS_EVALUATE_SWAP",
	"PE_VCS_ACCEPT_SWAP",
	"PE_VCS_REJECT_SWAP",
	"PE_VCS_WAIT_FOR_VCONN",
	"PE_VCS_TURN_OFF_VCONN",
	"PE_VCS_TURN_ON_VCONN",
	"PE_VCS_SEND_PS_RDY",

	"PE_UFP_VDM_GET_IDENTITY",
	"PE_UFP_VDM_SEND_IDENTITY",
	"PE_UFP_VDM_GET_IDENTITY_NAK",

	"PE_UFP_VDM_GET_SVIDS",
	"PE_UFP_VDM_SEND_SVIDS",
	"PE_UFP_VDM_GET_SVIDS_NAK",

	"PE_UFP_VDM_GET_MODES",
	"PE_UFP_VDM_SEND_MODES",
	"PE_UFP_VDM_GET_MODES_NAK",

	"PE_UFP_VDM_EVALUATE_MODE_ENTRY",
	"PE_UFP_VDM_MODE_ENTRY_ACK",
	"PE_UFP_VDM_MODE_ENTRY_NAK",

	"PE_UFP_VDM_MODE_EXIT",
	"PE_UFP_VDM_MODE_EXIT_ACK",
	"PE_UFP_VDM_MODE_EXIT_NAK",

	"PE_UFP_VDM_ATTENTION_REQUEST",

#ifdef CONFIG_KIRIN_USB_PD_ALT_MODE_SUPPORT
	"PE_UFP_VDM_DP_STATUS_UPDATE",
	"PE_UFP_VDM_DP_CONFIGURE",
#endif

	"PE_DFP_UFP_VDM_IDENTITY_REQUEST",
	"PE_DFP_UFP_VDM_IDENTITY_ACKED",
	"PE_DFP_UFP_VDM_IDENTITY_NAKED",

	"PE_DFP_CBL_VDM_IDENTITY_REQUEST",
	"PE_DFP_CBL_VDM_IDENTITY_ACKED",
	"PE_DFP_CBL_VDM_IDENTITY_NAKED",

	"PE_DFP_VDM_SVIDS_REQUEST",
	"PE_DFP_VDM_SVIDS_ACKED",
	"PE_DFP_VDM_SVIDS_NAKED",

	"PE_DFP_VDM_MODES_REQUEST",
	"PE_DFP_VDM_MODES_ACKED",
	"PE_DFP_VDM_MODES_NAKED",

	"PE_DFP_VDM_MODE_ENTRY_REQUEST",
	"PE_DFP_VDM_MODE_ENTRY_ACKED",
	"PE_DFP_VDM_MODE_ENTRY_NAKED",

	"PE_DFP_VDM_MODE_EXIT_REQUEST",
	"PE_DFP_VDM_MODE_EXIT_ACKED",

	"PE_DFP_VDM_ATTENTION_REQUEST",
	"PE_DFP_VDM_UNKNOWN",

#ifdef CONFIG_PD_DFP_RESET_CABLE
	"PE_DFP_CBL_SEND_SOFT_RESET",
	"PE_DFP_CBL_SEND_CABLE_RESET",
#endif

#ifdef CONFIG_KIRIN_USB_PD_ALT_MODE_DFP_SUPPORT
	"PE_DFP_VDM_DP_STATUS_UPDATE_REQUEST",
	"PE_DFP_VDM_DP_STATUS_UPDATE_ACKED",
	"PE_DFP_VDM_DP_STATUS_UPDATE_NAKED",

	"PE_DFP_VDM_DP_CONFIGURATION_REQUEST",
	"PE_DFP_VDM_DP_CONFIGURATION_ACKED",
	"PE_DFP_VDM_DP_CONFIGURATION_NAKED",
#endif

#ifdef CONFIG_KIRIN_USB_PD_UVDM_SUPPORT
	"PE_UFP_UVDM_RECV",
	"PE_DFP_UVDM_SEND",
	"PE_DFP_UVDM_ACKED",
	"PE_DFP_UVDM_NAKED",
#endif

#ifdef CONFIG_KIRIN_PD_REV30
#ifdef CONFIG_KIRIN_PD_REV30_BAT_CAP_REMOTE
	"PE_GET_BATTERY_CAP",
#endif
#ifdef CONFIG_KIRIN_PD_REV30_BAT_CAP_LOCAL
	"PE_GIVE_BATTERY_CAP",
#endif
#ifdef CONFIG_KIRIN_PD_REV30_BAT_STATUS_REMOTE
	"PE_GET_BATTERY_STATUS",
#endif
#ifdef CONFIG_KIRIN_PD_REV30_BAT_STATUS_LOCAL
	"PE_GIVE_BATTERY_STATUS",
#endif
#ifdef CONFIG_KIRIN_PD_REV30_MFRS_INFO_REMOTE
	"PE_GET_MANUFACTURER_INFO",
#endif
#ifdef CONFIG_KIRIN_PD_REV30_MFRS_INFO_LOCAL
	"PE_GIVE_MANUFACTURER_INFO",
#endif
#ifdef CONFIG_KIRIN_PD_REV30_COUNTRY_CODE_REMOTE
	"PE_GET_COUNTRY_CODES",
#endif
#ifdef CONFIG_KIRIN_PD_REV30_COUNTRY_CODE_LOCAL
	"PE_GIVE_COUNTRY_CODES",
#endif
#ifdef CONFIG_KIRIN_PD_REV30_COUNTRY_INFO_REMOTE
	"PE_GET_COUNTRY_INFO",
#endif
#ifdef CONFIG_KIRIN_PD_REV30_COUNTRY_INFO_LOCAL
	"PE_GIVE_COUNTRY_INFO",
#endif
	"PE_VDM_NOT_SUPPORTED",
	"PE_GET_REVISION",
	"PE_GIVE_REVISION",
#endif

#ifdef CONFIG_KIRIN_USB_PD_CUSTOM_DBGACC_SUPPORT
	"PE_DBG_READY",
#endif

	"PE_ERROR_RECOVERY",

	"PE_BIST_TEST_DATA",
	"PE_BIST_CARRIER_MODE_2",

	"PE_IDLE1",
	"PE_IDLE2",

	"PE_VIRT_HARD_RESET",
	"PE_VIRT_READY",
};

char *pd_pe_state_machine_name(uint8_t state_machine)
{
	char *name[] = {
		"PE_STATE_MACHINE_IDLE = 0",
		"PE_STATE_MACHINE_SINK",
		"PE_STATE_MACHINE_SOURCE",
		"PE_STATE_MACHINE_DR_SWAP",
		"PE_STATE_MACHINE_PR_SWAP",
		"PE_STATE_MACHINE_VCONN_SWAP",
		"PE_STATE_MACHINE_DBGACC",
	};

	return name[state_machine];
}

const char *pd_pe_state_name(uint8_t pe_state)
{
	if (pe_state >= ARRAY_SIZE(pe_state_name)) {
		E("pe_state num: %d\n", pe_state);
		return "illegal";
	} else {
		return pe_state_name[pe_state];
	}
}

/*
 * Policy Engine General State Activity
 */

static void pe_idle_reset_data(pd_port_t *pd_port)
{
	D("\n");
	pd_reset_pe_timer(pd_port);
#ifdef CONFIG_KIRIN_PD_REV30_PPS_SINK
	if (pd_port->request_apdo) {
		pd_port->request_apdo = false;
		hisi_pd_dpm_pps_request_heart_beat(pd_port);
	}
#endif
	hisi_pd_reset_svid_data(pd_port);

	pd_port->pd_prev_connected = false;
	pd_port->dpm_charging_policy = pd_port->dpm_charging_policy_default;
	tcpci_mutex_lock(pd_port->tcpc_dev);
	pd_port->tcpc_dev->pd_wait_pr_swap_complete = false;
	tcpci_mutex_unlock(pd_port->tcpc_dev);
	pd_port->state_machine = PE_STATE_MACHINE_IDLE;

	switch (pd_port->pe_state_curr) {
	case PE_BIST_TEST_DATA:
		hisi_pd_enable_bist_test_mode(pd_port, false);
		break;

	case PE_BIST_CARRIER_MODE_2:
		hisi_pd_disable_bist_mode2(pd_port);
		break;

	default:
		break;
	}

	pd_enable_timer(pd_port, PD_TIMER_PE_IDLE_TOUT);
	hisi_pd_unlock_msg_output(pd_port);
}

static void hisi_pe_idle1_entry(pd_port_t *pd_port, pd_event_t *pd_event)
{
	pe_idle_reset_data(pd_port);
	hisi_pd_try_put_pe_idle_event(pd_port);
}

static void hisi_pe_idle2_entry(pd_port_t *pd_port, pd_event_t *pd_event)
{
	hisi_pd_set_rx_enable(pd_port, PD_RX_CAP_PE_IDLE);
	pd_disable_timer(pd_port, PD_TIMER_PE_IDLE_TOUT);
	hisi_pd_notify_pe_idle(pd_port);
}

void hisi_pe_error_recovery_entry(pd_port_t *pd_port, pd_event_t *pd_event)
{
	pe_idle_reset_data(pd_port);
	hisi_pd_set_rx_enable(pd_port, PD_RX_CAP_PE_IDLE);
	hisi_pd_notify_pe_error_recovery(pd_port);
	pd_free_pd_event(pd_port, pd_event);
}


void hisi_pe_bist_test_data_entry(pd_port_t *pd_port, pd_event_t *pd_event)
{
	hisi_pd_enable_bist_test_mode(pd_port, true);
	pd_free_pd_event(pd_port, pd_event);
}

void hisi_pe_bist_test_data_exit(pd_port_t *pd_port, pd_event_t *pd_event)
{
	hisi_pd_enable_bist_test_mode(pd_port, false);
}

void hisi_pe_bist_carrier_mode_2_entry(pd_port_t *pd_port, pd_event_t *pd_event)
{
	hisi_pd_send_bist_mode2(pd_port);
	pd_enable_timer(pd_port, PD_TIMER_BIST_CONT_MODE);
	pd_free_pd_event(pd_port, pd_event);
}

void hisi_pe_bist_carrier_mode_2_exit(pd_port_t *pd_port, pd_event_t *pd_event)
{
	pd_disable_timer(pd_port, PD_TIMER_BIST_CONT_MODE);
	hisi_pd_disable_bist_mode2(pd_port);
}

/*
 * Entry function for sink and source, called after power negotiation.
 */
void hisi_pe_power_ready_entry(pd_port_t *pd_port, pd_event_t *pd_event)
{
	D("\n");
	pd_port->during_swap = false;
	pd_port->explicit_contract = true;

#ifdef CONFIG_JTAG_VDM_DEBUG
	struct tcpc_device *tcpc_dev = pd_port->tcpc_dev;
	tcpci_snk_unattach_by_vbus_bypass(tcpc_dev, false);
#endif

	if (pd_port->data_role == PD_ROLE_UFP)
		hisi_pd_set_rx_enable(pd_port, PD_RX_CAP_PE_READY_UFP);
	else
		hisi_pd_set_rx_enable(pd_port, PD_RX_CAP_PE_READY_DFP);

#ifdef CONFIG_USB_PD_UFP_FLOW_DELAY
	if (pd_port->data_role == PD_ROLE_UFP && (!pd_port->dpm_ufp_flow_delay_done)) {
		D("Delay UFP Flow\n");
		pd_restart_timer(pd_port, PD_TIMER_UFP_FLOW_DELAY);
		pd_free_pd_event(pd_port, pd_event);
		return;
	}
#endif

	hisi_pd_dpm_notify_pe_ready(pd_port, pd_event);
	pd_free_pd_event(pd_port, pd_event);
}

#ifdef CONFIG_KIRIN_PD_REV30
#ifdef CONFIG_KIRIN_PD_REV30_BAT_CAP_REMOTE
static void hisi_pe_get_battery_cap_entry(pd_port_t *pd_port, pd_event_t *pd_event)
{
	struct pd_get_battery_capabilities gbcdb = {0};

	hisi_pd_send_ext_msg(pd_port, TCPC_TX_SOP, PD_EXT_GET_BAT_CAP, false,
		0, PD_GBCDB_SIZE, (uint8_t *)&gbcdb);
}

static void hisi_pe_get_battery_cap_exit(pd_port_t *pd_port, pd_event_t *pd_event)
{
	hisi_pd_dpm_inform_battery_cap(pd_port, pd_event);
}
#endif

#ifdef CONFIG_KIRIN_PD_REV30_BAT_CAP_LOCAL
static void hisi_pe_give_battery_cap_entry(pd_port_t *pd_port, pd_event_t *pd_event)
{
	hisi_pd_dpm_send_battery_cap(pd_port);
}
#endif

#ifdef CONFIG_KIRIN_PD_REV30_BAT_STATUS_REMOTE
static void hisi_pe_get_battery_status_entry(pd_port_t *pd_port, pd_event_t *pd_event)
{
	hisi_pd_dpm_send_battery_status(pd_port);
}

static void hisi_pe_get_battery_status_exit(pd_port_t *pd_port, pd_event_t *pd_event)
{
	hisi_pd_dpm_inform_battery_status(pd_port, pd_event);
	I("Get battery status\n");
}
#endif

#ifdef CONFIG_KIRIN_PD_REV30_BAT_STATUS_LOCAL
static void hisi_pe_give_battery_status_entry(pd_port_t *pd_port, pd_event_t *pd_event)
{
	hisi_pd_dpm_send_battery_status(pd_port);
}
#endif

#ifdef CONFIG_KIRIN_PD_REV30_MFRS_INFO_REMOTE
static void hisi_pe_get_manufacturer_info_entry(pd_port_t *pd_port, pd_event_t *pd_event)
{
	struct pd_get_manufacturer_info midb;

	midb.info_target = 0x0; // Port
	midb.info_ref = 0x0;
	hisi_pd_send_ext_msg(pd_port, TCPC_TX_SOP, PD_EXT_GET_MFR_INFO, false,
		0, PD_GMIDB_SIZE, (uint8_t *)&midb);
}

static void hisi_pe_get_manufacturer_info_exit(pd_port_t *pd_port, pd_event_t *pd_event)
{
	hisi_pd_dpm_inform_mfrs_info(pd_port, pd_event);
}
#endif

#ifdef CONFIG_KIRIN_PD_REV30_MFRS_INFO_LOCAL
static void hisi_pe_give_manufacturer_info_entry(pd_port_t *pd_port, pd_event_t *pd_event)
{
	hisi_pd_dpm_send_mfrs_info(pd_port);
}
#endif

#ifdef CONFIG_KIRIN_PD_REV30_COUNTRY_CODE_REMOTE
static void hisi_pe_get_country_codes_entry(pd_port_t *pd_port, pd_event_t *pd_event)
{
	hisi_pd_send_ctrl_msg(pd_port, TCPC_TX_SOP, PD_CTRL_GET_COUNTRY_CODE);
}

static void hisi_pe_get_country_codes_exit(pd_port_t *pd_port, pd_event_t *pd_event)
{
	hisi_pd_dpm_inform_country_codes(pd_port, pd_event);
}
#endif

#ifdef CONFIG_KIRIN_PD_REV30_COUNTRY_CODE_LOCAL
static void hisi_pe_give_country_codes_entry(pd_port_t *pd_port, pd_event_t *pd_event)
{
	hisi_pd_send_ctrl_msg(pd_port, TCPC_TX_SOP, PD_CTRL_NOT_SUPPORTED);
}
#endif

#ifdef CONFIG_KIRIN_PD_REV30_COUNTRY_INFO_REMOTE
static void hisi_pe_get_country_info_entry(pd_port_t *pd_port, pd_event_t *pd_event)
{
	uint32_t ccdo = 0x435A;

	hisi_pd_send_data_msg(pd_port, TCPC_TX_SOP,
		PD_DATA_GET_COUNTRY_INFO, PD_CCDO_SIZE, &ccdo);
}

static void hisi_pe_get_country_info_exit(pd_port_t *pd_port, pd_event_t *pd_event)
{
	I("Get country info:\n");
	hisi_pd_dpm_inform_country_info(pd_port, pd_event);
}
#endif

#ifdef CONFIG_KIRIN_PD_REV30_COUNTRY_INFO_LOCAL
static void hisi_pe_give_country_info_entry(pd_port_t *pd_port, pd_event_t *pd_event)
{
	hisi_pd_dpm_send_country_info(pd_port);
}
#endif

static void hisi_pe_vdm_not_supported_entry(pd_port_t *pd_port, pd_event_t *pd_event)
{
	hisi_pd_send_ctrl_msg(pd_port, TCPC_TX_SOP, PD_CTRL_NOT_SUPPORTED);
}

static void hisi_pe_get_revision_entry(pd_port_t *pd_port, pd_event_t *pd_event)
{
	hisi_pd_send_ctrl_msg(pd_port, TCPC_TX_SOP, PD_CTRL_GET_REVISION);
}

static void hisi_pe_get_revision_exit(pd_port_t *pd_port, pd_event_t *pd_event)
{
	hisi_pd_dpm_inform_revision(pd_port, pd_event);
}

static void hisi_pe_give_revision_entry(pd_port_t *pd_port, pd_event_t *pd_event)
{
	hisi_pd_dpm_send_revision(pd_port);
}
#endif

typedef void (*pe_state_action_fcn_t)(pd_port_t *pd_port, pd_event_t *pd_event);

struct pe_state_action_t {
	const pe_state_action_fcn_t entry_action;
};

#define PE_STATE_ACTIONS(state) { .entry_action = hisi_##state##_entry, }

static const struct pe_state_action_t pe_state_actions[] = {
	/* src activity */
	PE_STATE_ACTIONS(pe_src_startup),
	PE_STATE_ACTIONS(pe_src_discovery),
	PE_STATE_ACTIONS(pe_src_send_capabilities),
	PE_STATE_ACTIONS(pe_src_negotiate_capabilities),
	PE_STATE_ACTIONS(pe_src_transition_supply),
	PE_STATE_ACTIONS(pe_src_transition_supply2),
	PE_STATE_ACTIONS(pe_src_ready),
	PE_STATE_ACTIONS(pe_src_disabled),
	PE_STATE_ACTIONS(pe_src_capability_response),
	PE_STATE_ACTIONS(pe_src_hard_reset),
	PE_STATE_ACTIONS(pe_src_hard_reset_received),
	PE_STATE_ACTIONS(pe_src_transition_to_default),
	PE_STATE_ACTIONS(pe_src_get_sink_cap),
	PE_STATE_ACTIONS(pe_src_wait_new_capabilities),

	PE_STATE_ACTIONS(pe_src_send_soft_reset),
	PE_STATE_ACTIONS(pe_src_soft_reset),
	PE_STATE_ACTIONS(pe_src_ping),

#ifdef CONFIG_USB_PD_SRC_STARTUP_DISCOVER_ID
	PE_STATE_ACTIONS(pe_src_vdm_identity_request),
	PE_STATE_ACTIONS(pe_src_vdm_identity_acked),
	PE_STATE_ACTIONS(pe_src_vdm_identity_naked),
#endif
#ifdef CONFIG_KIRIN_PD_REV30
	PE_STATE_ACTIONS(pe_src_send_not_supported),
	PE_STATE_ACTIONS(pe_src_not_supported_received),
	PE_STATE_ACTIONS(pe_src_chunk_received),
#ifdef CONFIG_KIRIN_PD_REV30_ALERT_LOCAL
	PE_STATE_ACTIONS(pe_src_send_source_alert),
#endif
#ifdef CONFIG_KIRIN_PD_REV30_ALERT_REMOTE
	PE_STATE_ACTIONS(pe_src_sink_alert_received),
#endif
#ifdef CONFIG_KIRIN_PD_REV30_SRC_CAP_EXT_LOCAL
	PE_STATE_ACTIONS(pe_src_give_source_cap_ext),
#endif
#ifdef CONFIG_KIRIN_PD_REV30_STATUS_LOCAL
	PE_STATE_ACTIONS(pe_src_give_source_status),
#endif
#ifdef CONFIG_KIRIN_PD_REV30_STATUS_REMOTE
	PE_STATE_ACTIONS(pe_src_get_sink_status),
#endif
#ifdef CONFIG_KIRIN_PD_REV30_PPS_SOURCE
	PE_STATE_ACTIONS(pe_src_give_pps_status),
#endif
	PE_STATE_ACTIONS(pe_src_get_sink_cap_ext),
#endif /* CONFIG_KIRIN_PD_REV30 */

	/* snk activity */
	PE_STATE_ACTIONS(pe_snk_startup),
	PE_STATE_ACTIONS(pe_snk_discovery),
	PE_STATE_ACTIONS(pe_snk_wait_for_capabilities),
	PE_STATE_ACTIONS(pe_snk_evaluate_capability),
	PE_STATE_ACTIONS(pe_snk_select_capability),
	PE_STATE_ACTIONS(pe_snk_transition_sink),
	PE_STATE_ACTIONS(pe_snk_ready),
	PE_STATE_ACTIONS(pe_snk_hard_reset),
	PE_STATE_ACTIONS(pe_snk_transition_to_default),
	PE_STATE_ACTIONS(pe_snk_give_sink_cap),
	PE_STATE_ACTIONS(pe_snk_get_source_cap),

	PE_STATE_ACTIONS(pe_snk_send_soft_reset),
	PE_STATE_ACTIONS(pe_snk_soft_reset),
#ifdef CONFIG_KIRIN_PD_REV30
	PE_STATE_ACTIONS(pe_snk_send_not_supported),
	PE_STATE_ACTIONS(pe_snk_not_supported_received),
	PE_STATE_ACTIONS(pe_snk_chunk_received),
#ifdef CONFIG_KIRIN_PD_REV30_ALERT_REMOTE
	PE_STATE_ACTIONS(pe_snk_source_alert_received),
#endif
#ifdef CONFIG_KIRIN_PD_REV30_ALERT_LOCAL
	PE_STATE_ACTIONS(pe_snk_send_sink_alert),
#endif
#ifdef CONFIG_KIRIN_PD_REV30_SRC_CAP_EXT_REMOTE
	PE_STATE_ACTIONS(pe_snk_get_source_cap_ext),
#endif
#ifdef CONFIG_KIRIN_PD_REV30_STATUS_REMOTE
	PE_STATE_ACTIONS(pe_snk_get_source_status),
#endif
#ifdef CONFIG_KIRIN_PD_REV30_STATUS_LOCAL
	PE_STATE_ACTIONS(pe_snk_give_sink_status),
#endif
#ifdef CONFIG_KIRIN_PD_REV30_PPS_SINK
	PE_STATE_ACTIONS(pe_snk_get_pps_status),
#endif
	PE_STATE_ACTIONS(pe_snk_give_sink_cap_ext),
#endif /* CONFIG_KIRIN_PD_REV30 */

	/* drs dfp activity */
	PE_STATE_ACTIONS(pe_drs_dfp_ufp_evaluate_dr_swap),
	PE_STATE_ACTIONS(pe_drs_dfp_ufp_accept_dr_swap),
	PE_STATE_ACTIONS(pe_drs_dfp_ufp_change_to_ufp),
	PE_STATE_ACTIONS(pe_drs_dfp_ufp_send_dr_swap),
	PE_STATE_ACTIONS(pe_drs_dfp_ufp_reject_dr_swap),

	/* drs ufp activity */
	PE_STATE_ACTIONS(pe_drs_ufp_dfp_evaluate_dr_swap),
	PE_STATE_ACTIONS(pe_drs_ufp_dfp_accept_dr_swap),
	PE_STATE_ACTIONS(pe_drs_ufp_dfp_change_to_dfp),
	PE_STATE_ACTIONS(pe_drs_ufp_dfp_send_dr_swap),
	PE_STATE_ACTIONS(pe_drs_ufp_dfp_reject_dr_swap),

	/* prs src activity */
	PE_STATE_ACTIONS(pe_prs_src_snk_evaluate_pr_swap),
	PE_STATE_ACTIONS(pe_prs_src_snk_accept_pr_swap),
	PE_STATE_ACTIONS(pe_prs_src_snk_transition_to_off),
	PE_STATE_ACTIONS(pe_prs_src_snk_assert_rd),
	PE_STATE_ACTIONS(pe_prs_src_snk_wait_source_on),
	PE_STATE_ACTIONS(pe_prs_src_snk_send_swap),
	PE_STATE_ACTIONS(pe_prs_src_snk_reject_pr_swap),

	/* prs snk activity */
	PE_STATE_ACTIONS(pe_prs_snk_src_evaluate_pr_swap),
	PE_STATE_ACTIONS(pe_prs_snk_src_accept_pr_swap),
	PE_STATE_ACTIONS(pe_prs_snk_src_transition_to_off),
	PE_STATE_ACTIONS(pe_prs_snk_src_assert_rp), /* hisi_pe_prs_snk_src_assert_rp_entry */
	PE_STATE_ACTIONS(pe_prs_snk_src_source_on),
	PE_STATE_ACTIONS(pe_prs_snk_src_send_swap),
	PE_STATE_ACTIONS(pe_prs_snk_src_reject_swap),

	/* dr src activity */
	PE_STATE_ACTIONS(pe_dr_src_get_source_cap),
	PE_STATE_ACTIONS(pe_dr_src_give_sink_cap),

	/* dr snk activity */
	PE_STATE_ACTIONS(pe_dr_snk_get_sink_cap),
	PE_STATE_ACTIONS(pe_dr_snk_give_source_cap),

#ifdef CONFIG_KIRIN_PD_REV30
#ifdef CONFIG_KIRIN_PD_REV30_SRC_CAP_EXT_LOCAL
	PE_STATE_ACTIONS(pe_dr_snk_give_source_cap_ext),
#endif
#ifdef CONFIG_KIRIN_PD_REV30_SRC_CAP_EXT_REMOTE
	PE_STATE_ACTIONS(pe_dr_src_get_source_cap_ext),
#endif
	PE_STATE_ACTIONS(pe_dr_src_give_sink_cap_ext),
	PE_STATE_ACTIONS(pe_dr_snk_get_sink_cap_ext),
#endif	

	/* vcs activity */
	PE_STATE_ACTIONS(pe_vcs_send_swap),
	PE_STATE_ACTIONS(pe_vcs_evaluate_swap),
	PE_STATE_ACTIONS(pe_vcs_accept_swap),
	PE_STATE_ACTIONS(pe_vcs_reject_vconn_swap),
	PE_STATE_ACTIONS(pe_vcs_wait_for_vconn),
	PE_STATE_ACTIONS(pe_vcs_turn_off_vconn),
	PE_STATE_ACTIONS(pe_vcs_turn_on_vconn),
	PE_STATE_ACTIONS(pe_vcs_send_ps_rdy),

	/* ufp structured vdm activity */
	PE_STATE_ACTIONS(pe_ufp_vdm_get_identity),
	PE_STATE_ACTIONS(pe_ufp_vdm_send_identity),
	PE_STATE_ACTIONS(pe_ufp_vdm_get_identity_nak),

	PE_STATE_ACTIONS(pe_ufp_vdm_get_svids),
	PE_STATE_ACTIONS(pe_ufp_vdm_send_svids),
	PE_STATE_ACTIONS(pe_ufp_vdm_get_svids_nak),

	PE_STATE_ACTIONS(pe_ufp_vdm_get_modes),
	PE_STATE_ACTIONS(pe_ufp_vdm_send_modes),
	PE_STATE_ACTIONS(pe_ufp_vdm_get_modes_nak),

	PE_STATE_ACTIONS(pe_ufp_vdm_evaluate_mode_entry),
	PE_STATE_ACTIONS(pe_ufp_vdm_mode_entry_ack),
	PE_STATE_ACTIONS(pe_ufp_vdm_mode_entry_nak),

	PE_STATE_ACTIONS(pe_ufp_vdm_mode_exit),
	PE_STATE_ACTIONS(pe_ufp_vdm_mode_exit_ack),
	PE_STATE_ACTIONS(pe_ufp_vdm_mode_exit_nak),

	PE_STATE_ACTIONS(pe_ufp_vdm_attention_request),

#ifdef CONFIG_KIRIN_USB_PD_ALT_MODE_SUPPORT
	PE_STATE_ACTIONS(pe_ufp_vdm_dp_status_update),
	PE_STATE_ACTIONS(pe_ufp_vdm_dp_configure),
#endif

	/* dfp structured vdm */
	PE_STATE_ACTIONS(pe_dfp_ufp_vdm_identity_request),
	PE_STATE_ACTIONS(pe_dfp_ufp_vdm_identity_acked),
	PE_STATE_ACTIONS(pe_dfp_ufp_vdm_identity_naked),

	PE_STATE_ACTIONS(pe_dfp_cbl_vdm_identity_request),
	PE_STATE_ACTIONS(pe_dfp_cbl_vdm_identity_acked),
	PE_STATE_ACTIONS(pe_dfp_cbl_vdm_identity_naked),

	PE_STATE_ACTIONS(pe_dfp_vdm_svids_request),
	PE_STATE_ACTIONS(pe_dfp_vdm_svids_acked),
	PE_STATE_ACTIONS(pe_dfp_vdm_svids_naked),

	PE_STATE_ACTIONS(pe_dfp_vdm_modes_request),
	PE_STATE_ACTIONS(pe_dfp_vdm_modes_acked),
	PE_STATE_ACTIONS(pe_dfp_vdm_modes_naked),

	PE_STATE_ACTIONS(pe_dfp_vdm_mode_entry_request),
	PE_STATE_ACTIONS(pe_dfp_vdm_mode_entry_acked),
	PE_STATE_ACTIONS(pe_dfp_vdm_mode_entry_naked),

	PE_STATE_ACTIONS(pe_dfp_vdm_mode_exit_request),
	PE_STATE_ACTIONS(pe_dfp_vdm_mode_exit_acked),

	PE_STATE_ACTIONS(pe_dfp_vdm_attention_request),
	PE_STATE_ACTIONS(pe_dfp_vdm_unknown),

#ifdef CONFIG_PD_DFP_RESET_CABLE
	PE_STATE_ACTIONS(pe_dfp_cbl_send_soft_reset),
	PE_STATE_ACTIONS(pe_dfp_cbl_send_cable_reset),
#endif

#ifdef CONFIG_KIRIN_USB_PD_ALT_MODE_DFP_SUPPORT
	PE_STATE_ACTIONS(pe_dfp_vdm_dp_status_update_request),
	PE_STATE_ACTIONS(pe_dfp_vdm_dp_status_update_acked),
	PE_STATE_ACTIONS(pe_dfp_vdm_dp_status_update_naked),

	PE_STATE_ACTIONS(pe_dfp_vdm_dp_configuration_request),
	PE_STATE_ACTIONS(pe_dfp_vdm_dp_configuration_acked),
	PE_STATE_ACTIONS(pe_dfp_vdm_dp_configuration_naked),
#endif

#ifdef CONFIG_KIRIN_USB_PD_UVDM_SUPPORT
	PE_STATE_ACTIONS(pe_ufp_uvdm_recv),
	PE_STATE_ACTIONS(pe_dfp_uvdm_send),
	PE_STATE_ACTIONS(pe_dfp_uvdm_acked),
	PE_STATE_ACTIONS(pe_dfp_uvdm_naked),
#endif

#ifdef CONFIG_KIRIN_PD_REV30
#ifdef CONFIG_KIRIN_PD_REV30_BAT_CAP_REMOTE
	PE_STATE_ACTIONS(pe_get_battery_cap),
#endif
#ifdef CONFIG_KIRIN_PD_REV30_BAT_CAP_LOCAL
	PE_STATE_ACTIONS(pe_give_battery_cap),
#endif
#ifdef CONFIG_KIRIN_PD_REV30_BAT_STATUS_REMOTE
	PE_STATE_ACTIONS(pe_get_battery_status),
#endif
#ifdef CONFIG_KIRIN_PD_REV30_BAT_STATUS_LOCAL
	PE_STATE_ACTIONS(pe_give_battery_status),
#endif
#ifdef CONFIG_KIRIN_PD_REV30_MFRS_INFO_REMOTE
	PE_STATE_ACTIONS(pe_get_manufacturer_info),
#endif
#ifdef CONFIG_KIRIN_PD_REV30_MFRS_INFO_LOCAL
	PE_STATE_ACTIONS(pe_give_manufacturer_info),
#endif
#ifdef CONFIG_KIRIN_PD_REV30_COUNTRY_CODE_REMOTE
	PE_STATE_ACTIONS(pe_get_country_codes),
#endif
#ifdef CONFIG_KIRIN_PD_REV30_COUNTRY_CODE_LOCAL
	PE_STATE_ACTIONS(pe_give_country_codes),
#endif
#ifdef CONFIG_KIRIN_PD_REV30_COUNTRY_INFO_REMOTE
	PE_STATE_ACTIONS(pe_get_country_info),
#endif
#ifdef CONFIG_KIRIN_PD_REV30_COUNTRY_INFO_LOCAL
	PE_STATE_ACTIONS(pe_give_country_info),
#endif
	PE_STATE_ACTIONS(pe_vdm_not_supported),
	PE_STATE_ACTIONS(pe_get_revision),
	PE_STATE_ACTIONS(pe_give_revision),
#endif

	/* general activity */
#ifdef CONFIG_KIRIN_USB_PD_CUSTOM_DBGACC_SUPPORT
	PE_STATE_ACTIONS(pe_dbg_ready),
#endif

	PE_STATE_ACTIONS(pe_error_recovery),

	PE_STATE_ACTIONS(pe_bist_test_data),
	PE_STATE_ACTIONS(pe_bist_carrier_mode_2),

	PE_STATE_ACTIONS(pe_idle1),
	PE_STATE_ACTIONS(pe_idle2),
};

/**
 * Common exit function for several state.
 */
static inline void pe_exit_action_disable_sender_response(
		pd_port_t *pd_port, pd_event_t *pd_event)
{
	D("\n");
	pd_disable_timer(pd_port, PD_TIMER_SENDER_RESPONSE);
}

static pe_state_action_fcn_t pe_state_exit_actions[PE_STATE_MAX] = {
	/* Source */
	[PE_SRC_SEND_CAPABILITIES] = hisi_pe_src_send_capabilities_exit,
	[PE_SRC_TRANSITION_SUPPLY] = hisi_pe_src_transition_supply_exit,
	[PE_SRC_TRANSITION_TO_DEFAULT] = hisi_pe_src_transition_to_default_exit,
	[PE_SRC_GET_SINK_CAP] = hisi_pe_src_get_sink_cap_exit,
#ifdef CONFIG_KIRIN_PD_REV30
#ifdef CONFIG_KIRIN_PD_REV30_STATUS_REMOTE
	[PE_SRC_GET_SINK_STATUS] = hisi_pe_src_get_sink_status_exit,
#endif
	[PE_SRC_GET_SINK_CAP_EXT] = hisi_pe_src_get_sink_cap_ext_exit,
#endif

	/* Sink */
	[PE_SNK_WAIT_FOR_CAPABILITIES] = hisi_pe_snk_wait_for_capabilities_exit,
	[PE_SNK_SELECT_CAPABILITY] = hisi_pe_snk_select_capability_exit,
#ifdef CONFIG_KIRIN_PD_REV30
#ifdef CONFIG_KIRIN_PD_REV30_SRC_CAP_EXT_REMOTE
	[PE_SNK_GET_SOURCE_CAP_EXT] = hisi_pe_snk_get_source_cap_ext_exit,
#endif

#ifdef CONFIG_KIRIN_PD_REV30_STATUS_REMOTE
	[PE_SNK_GET_SOURCE_STATUS] = hisi_pe_snk_get_source_status_exit,
#endif
#ifdef CONFIG_KIRIN_PD_REV30_PPS_SINK
	[PE_SNK_GET_PPS_STATUS] = hisi_pe_snk_get_pps_status_exit,
#endif
#endif
	[PE_SNK_TRANSITION_SINK] = hisi_pe_snk_transition_sink_exit,
	[PE_SNK_TRANSITION_TO_DEFAULT] = hisi_pe_snk_transition_to_default_exit,
	[PE_DR_SRC_GET_SOURCE_CAP] = hisi_pe_dr_src_get_source_cap_exit,
	[PE_DR_SNK_GET_SINK_CAP] = hisi_pe_dr_snk_get_sink_cap_exit,
#ifdef CONFIG_KIRIN_PD_REV30_SRC_CAP_EXT_REMOTE
	[PE_DR_SRC_GET_SOURCE_CAP_EXT] = hisi_pe_dr_src_get_source_cap_ext_exit,
#endif

#ifdef CONFIG_KIRIN_PD_REV30
#ifdef CONFIG_KIRIN_PD_REV30_BAT_CAP_REMOTE
	[PE_GET_BATTERY_CAP] = hisi_pe_get_battery_cap_exit,
#endif

#ifdef CONFIG_KIRIN_PD_REV30_BAT_STATUS_REMOTE
	[PE_GET_BATTERY_STATUS] = hisi_pe_get_battery_status_exit,
#endif

#ifdef CONFIG_KIRIN_PD_REV30_MFRS_INFO_REMOTE
	[PE_GET_MANUFACTURER_INFO] = hisi_pe_get_manufacturer_info_exit,
#endif

#ifdef CONFIG_KIRIN_PD_REV30_COUNTRY_CODE_REMOTE
	[PE_GET_COUNTRY_CODES] = hisi_pe_get_country_codes_exit,
#endif

#ifdef CONFIG_KIRIN_PD_REV30_COUNTRY_INFO_REMOTE
	[PE_GET_COUNTRY_INFO] = hisi_pe_get_country_info_exit,
#endif

	[PE_GET_REVISION] = hisi_pe_get_revision_exit,
#endif

	[PE_BIST_TEST_DATA] = hisi_pe_bist_test_data_exit,
	[PE_BIST_CARRIER_MODE_2] = hisi_pe_bist_carrier_mode_2_exit,
	[PE_VCS_SEND_SWAP] = pe_exit_action_disable_sender_response,
	[PE_PRS_SRC_SNK_SEND_SWAP] = pe_exit_action_disable_sender_response,
	[PE_PRS_SNK_SRC_SEND_SWAP] = pe_exit_action_disable_sender_response,
	[PE_DRS_DFP_UFP_SEND_DR_SWAP] = pe_exit_action_disable_sender_response,
	[PE_DRS_UFP_DFP_SEND_DR_SWAP] = pe_exit_action_disable_sender_response,
	[PE_PRS_SRC_SNK_WAIT_SOURCE_ON] = hisi_pe_prs_src_snk_wait_source_on_exit,
	[PE_PRS_SNK_SRC_SOURCE_ON] = hisi_pe_prs_snk_src_source_on_exit,
	[PE_PRS_SNK_SRC_TRANSITION_TO_OFF] = hisi_pe_prs_snk_src_transition_to_off_exit,
	[PE_VCS_WAIT_FOR_VCONN] = hisi_pe_vcs_wait_for_vconn_exit,
};

static pe_state_action_fcn_t hisi_pe_get_exit_action(uint8_t pe_state)
{
	I("exit pe_state: %u\n", pe_state);
	if (pe_state < ARRAY_SIZE(pe_state_exit_actions))
		return pe_state_exit_actions[pe_state];

	E("illegal pe state: %d\n", pe_state);
	return NULL;
}

static void print_state(pd_port_t *pd_port, bool vdm_evt, uint8_t state)
{
	I("%s(%u)-> %s\n", vdm_evt ? "VDM" : "PD", state, pd_pe_state_name(state));
	D("pe state: %s(%u)-> %s; " "pr: %s; " "dr: %s; " "vconn_source %d\n",
		vdm_evt ? "VDM" : "PD", state, pd_pe_state_name(state),
		pd_port->power_role ? "Source" : "Sink",
		pd_port->data_role ? "DFP" : "UFP",
		pd_port->vconn_source);
}

static void pd_pe_state_change(pd_port_t *pd_port, pd_event_t *pd_event, bool vdm_evt)
{
	pe_state_action_fcn_t prev_exit_action = NULL;
	pe_state_action_fcn_t next_entry_action = NULL;

	uint8_t old_state = pd_port->pe_state_curr;
	uint8_t new_state = pd_port->pe_state_next;

	I("pe_state_change: %s(%u) ----> %s(%u)\n", pd_pe_state_name(old_state),
		old_state, pd_pe_state_name(new_state), new_state);
	if ((old_state >= PD_NR_PE_STATES) || (new_state >= ARRAY_SIZE(pe_state_actions))) {
		pr_err("state error\n");
		return;
	}

	if ((new_state == PE_IDLE1) || (new_state == PE_IDLE2))
		prev_exit_action = NULL;
	else
		prev_exit_action = hisi_pe_get_exit_action(old_state);

	next_entry_action = pe_state_actions[new_state].entry_action;

	print_state(pd_port, vdm_evt, new_state);

	if (prev_exit_action)
		prev_exit_action(pd_port, pd_event);

	if (next_entry_action)
		next_entry_action(pd_port, pd_event);

	if (vdm_evt)
		pd_port->pe_vdm_state = new_state;
	else
		pd_port->pe_pd_state = new_state; 
}

static int pd_handle_event(pd_port_t *pd_port, pd_event_t *pd_event, bool vdm_evt)
{
	bool ret = false;
 
	if (vdm_evt) {
		if (pd_port->reset_vdm_state && (!pd_port->dpm_ack_immediately)) {
			pd_port->reset_vdm_state = false;
			pd_port->pe_vdm_state = pd_port->pe_pd_state;
			D("set pe_vdm_state %u\n", pd_port->pe_vdm_state);
		}

		pd_port->pe_state_curr = pd_port->pe_vdm_state;
		D("pe_state_curr = pe_vdm_state, %s\n",  pd_pe_state_name(pd_port->pe_vdm_state));
	} else {
		pd_port->pe_state_curr = pd_port->pe_pd_state;
		D("pe_state_curr = pe_pd_state, %s\n",  pd_pe_state_name(pd_port->pe_pd_state));
	}

	ret = hisi_pd_process_event(pd_port, pd_event, vdm_evt);
	if (ret)
		pd_pe_state_change(pd_port, pd_event, vdm_evt);
	else
		pd_free_pd_event(pd_port, pd_event);

	return 1;
}

static int pd_put_dpm_ack_immediately(pd_port_t *pd_port, bool vdm_evt)
{
	pd_event_t pd_event = {
		.event_type = PD_EVT_DPM_MSG,
		.msg = PD_DPM_ACK,
		.pd_msg = NULL,
	};

	pd_handle_event(pd_port, &pd_event, vdm_evt);
	pd_port->dpm_ack_immediately = false;

	return 1;
}

static int hisi_pe_get_vdm_event(struct tcpc_device *tcpc_dev,
		pd_event_t *pd_event)
{
	pd_port_t *pd_port = &tcpc_dev->pd_port;
	bool vdm_evt = false;

	switch (pd_port->pe_pd_state) {
	case PE_SNK_READY:
	case PE_SRC_READY:
	case PE_SRC_STARTUP:
	case PE_SRC_DISCOVERY:
#ifdef CONFIG_KIRIN_USB_PD_CUSTOM_DBGACC_SUPPORT
	case PE_DBG_READY:
#endif
		vdm_evt = hisi_pd_get_vdm_event(tcpc_dev, pd_event);
		break;

	default:
		break;
	}

	if (!vdm_evt)
		return 0;

	return 1;
}

#ifdef CONFIG_KIRIN_PD_REV30

static inline bool pd_check_sink_tx_ok(pd_port_t *pd_port)
{
	I("\n");
#ifdef CONFIG_KIRIN_PD_REV30_COLLISION_AVOID
	if (pd_check_rev30(pd_port) &&
		(pd_port->pd_traffic_control != PD_SINK_TX_OK))
		return false;
#endif	/* CONFIG_KIRIN_PD_REV30_COLLISION_AVOID */

	return true;
}

static inline bool pd_check_source_tx_ok(pd_port_t *pd_port)
{
	I("\n");
#ifdef CONFIG_KIRIN_PD_REV30_COLLISION_AVOID
	if (!pd_check_rev30(pd_port))
		return true;

	if (pd_port->pd_traffic_control == PD_SOURCE_TX_OK)
		return true;

	if (!pd_port->pd_traffic_idle)
		hisi_pd_set_sink_tx(pd_port, PD30_SINK_TX_NG);

	return false;
#else
	return true;
#endif	/* CONFIG_KIRIN_PD_REV30_COLLISION_AVOID */
}

static inline bool pd_check_pd30_tx_ready(pd_port_t *pd_port)
{
	I("\n");
#ifdef CONFIG_USB_PD_PE_SINK
	if (pd_port->pe_pd_state == PE_SNK_READY)
		return pd_check_sink_tx_ok(pd_port);
#endif	/* CONFIG_USB_PD_PE_SINK */

#ifdef CONFIG_USB_PD_PE_SOURCE
	if (pd_port->pe_pd_state == PE_SRC_READY)
		return pd_check_source_tx_ok(pd_port);
#endif	/* CONFIG_USB_PD_PE_SOURCE */

#ifdef CONFIG_USB_PD_CUSTOM_DBGACC
	if (pd_port->pe_pd_state == PE_DBG_READY)
		return true;
#endif	/* CONFIG_USB_PD_CUSTOM_DBGACC */

	return false;
}
#else
static inline bool pd_check_pd20_tx_ready(pd_port_t *pd_port)
{
	I("\n");
	switch (pd_port->pe_pd_state) {
#ifdef CONFIG_USB_PD_PE_SINK
	case PE_SNK_READY:
		return true;
#endif	/* CONFIG_USB_PD_PE_SINK */

#ifdef CONFIG_USB_PD_PE_SOURCE
	case PE_SRC_READY:
		return true;
#endif	/* CONFIG_USB_PD_PE_SOURCE */

#ifdef CONFIG_USB_PD_CUSTOM_DBGACC
	case PE_DBG_READY:
		return true;
#endif	/* CONFIG_USB_PD_CUSTOM_DBGACC */

	default:
		return false;
	}
}
#endif	/* ndef CONFIG_KIRIN_PD_REV30 */


/**
 *
 * Check PE is ready to initiate an active event (AMS).
 *
 * For revision 2, checking the PE state is in Ready.
 * For revision 3, checking SinkTx besides above description.
 *
 * If PR=SRC and CC=SinkTxOK, change CC to SinkTxNG;
 *
 * Returns a boolean value to present PE is available or not.
 */

static inline bool pd_check_tx_ready(pd_port_t *pd_port)
{
	I("\n");
#ifdef CONFIG_KIRIN_PD_REV30
	return pd_check_pd30_tx_ready(pd_port);
#else
	return pd_check_pd20_tx_ready(pd_port);
#endif	/* CONFIG_KIRIN_PD_REV30 */
}

/**
 *
 * Get a pending TCPM event from the event queue
 *
 * Returns TCP_DPM_EVT_ID if succeeded,
 * otherwise returns DPM_READY_REACTION_BUSY.
 */

static inline uint8_t pd_try_get_deferred_tcp_event(pd_port_t *pd_port)
{
	I("\n");
	if (!hisi_pd_get_deferred_tcp_event_ex(
		pd_port->tcpc_dev, &pd_port->tcp_event))
		return DPM_READY_REACTION_BUSY;

#ifdef CONFIG_USB_PD_TCPM_CB_2ND
	pd_port->tcp_event_drop_reset_once = true;
#endif	/* CONFIG_USB_PD_TCPM_CB_2ND */

	if (!pd_check_tx_ready(pd_port))
		return false;

	pd_port->tcp_event_id_1st = pd_port->tcp_event.event_id;
	return pd_port->tcp_event_id_1st;
}

static int pd_try_get_active_event(struct tcpc_device *tcpc_dev,
		pd_event_t *pd_event)
{
	bool ret = false;
	uint8_t from_pe = PD_TCP_FROM_PE;
	pd_port_t *pd_port = &tcpc_dev->pd_port;
	I("\n");

	ret = pd_get_deferred_tcp_event(tcpc_dev, &pd_port->tcp_event);
	pd_port->tcp_event_id_1st = pd_port->tcp_event.event_id;

	if (ret) {
		pd_event->event_type = PD_EVT_TCP_MSG;
		pd_event->msg = pd_port->tcp_event.event_id;
		pd_event->msg_sec = from_pe;
		pd_event->pd_msg = NULL;
	}

	return ret;
}

int hisi_pd_policy_engine_run(struct tcpc_device *tcpc_dev)
{
	pd_event_t pd_event = {0};
	pd_port_t *pd_port = &tcpc_dev->pd_port;
	bool vdm_evt = false;

	if (hisi_pd_get_event(tcpc_dev, &pd_event)) {
		D("pd event\n");
	} else if (hisi_pe_get_vdm_event(tcpc_dev, &pd_event)){
		vdm_evt = true;
		D("vdm event\n");
	} else if (pd_try_get_active_event(tcpc_dev, &pd_event)){
		D("tcp event\n");
	} else {
		D("no event\n");
		return 0;
	}

	mutex_lock(&pd_port->pd_lock);

	pd_handle_event(pd_port, &pd_event, vdm_evt);

	if (pd_port->dpm_ack_immediately)
		pd_put_dpm_ack_immediately(pd_port, vdm_evt);

	mutex_unlock(&pd_port->pd_lock);

	return 1;
}
