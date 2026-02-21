/*
 * Copyright (c) 2020 Huawei Technologies Co., Ltd.
 *
 * Copyright (C) 2016 Richtek Technology Corp.
 * Author: TH <tsunghan_tsai@richtek.com>
 *
 * Power Delivery Policy Engine for DR
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

#define LOG_TAG "[PE_DR]"

#include "include/pd_core.h"
#include "include/pd_tcpm.h"
#include "include/pd_dpm_core.h"
#include "include/tcpci.h"
#include "include/pd_policy_engine.h"

/*
 * [PD2.0]
 * Figure 8-53 Dual-Role (Source) Get Source Capabilities diagram
 * Figure 8-54 Dual-Role (Source) Give Sink Capabilities diagram
 * Figure 8-55 Dual-Role (Sink) Get Sink Capabilities State Diagram
 * Figure 8-56 Dual-Role (Sink) Give Source Capabilities State Diagram
 */

#define PD_CTRL_GET_SINK_CAP_CNT_MAX 3
#define PD_CTRL_TX_SINK_CAP_MAX_DELAY_TIM_US 800

void hisi_pe_dr_src_get_source_cap_entry(pd_port_t *pd_port, pd_event_t *pd_event)
{
	hisi_pd_send_ctrl_msg(pd_port, TCPC_TX_SOP, PD_CTRL_GET_SOURCE_CAP);
}

void hisi_pe_dr_src_get_source_cap_exit(pd_port_t *pd_port, pd_event_t *pd_event)
{
	D("\n");
	pd_disable_timer(pd_port, PD_TIMER_SENDER_RESPONSE);
	hisi_pd_dpm_dr_inform_source_cap(pd_port, pd_event);
}

void hisi_pe_dr_src_give_sink_cap_entry(pd_port_t *pd_port, pd_event_t *pd_event)
{
	D("\n");
	hisi_pd_dpm_send_sink_caps(pd_port);
	if (pd_port->vdf_flags & VDF_FLAGS_PD_31_CERTIFICATION_MASK != VDF_FLAGS_PD_31_CERTIFICATION_MASK) {
		pd_free_pd_event(pd_port, pd_event);
		return;
    }
	if (pd_port->pd_source_get_sink_cap_cnt == PD_CTRL_GET_SINK_CAP_CNT_MAX) {
		udelay(PD_CTRL_TX_SINK_CAP_MAX_DELAY_TIM_US);
		tcpci_reset_only_pd_fsm(pd_port->tcpc_dev);
		pd_port->pd_source_get_sink_cap_cnt = 0;
		D("delay 800us\n");
	}
	pd_free_pd_event(pd_port, pd_event);
}

void hisi_pe_dr_snk_get_sink_cap_entry(pd_port_t *pd_port, pd_event_t *pd_event)
{
	hisi_pd_send_ctrl_msg(pd_port, TCPC_TX_SOP, PD_CTRL_GET_SINK_CAP);
}

void hisi_pe_dr_snk_get_sink_cap_exit(pd_port_t *pd_port, pd_event_t *pd_event)
{
	D("\n");
	pd_disable_timer(pd_port, PD_TIMER_SENDER_RESPONSE);
	hisi_pd_dpm_dr_inform_sink_cap(pd_port, pd_event);
}

void hisi_pe_dr_snk_give_source_cap_entry(pd_port_t *pd_port, pd_event_t *pd_event)
{
	D("\n");
	hisi_pd_dpm_send_source_caps(pd_port);
	pd_free_pd_event(pd_port, pd_event);
}

#ifdef CONFIG_KIRIN_PD_REV30
#ifdef CONFIG_KIRIN_PD_REV30_SRC_CAP_EXT_LOCAL
void hisi_pe_dr_snk_give_source_cap_ext_entry(pd_port_t *pd_port, pd_event_t *pd_event)
{
	hisi_pd_dpm_send_source_cap_ext(pd_port);
}
#endif

#ifdef CONFIG_KIRIN_PD_REV30_SRC_CAP_EXT_REMOTE
void hisi_pe_dr_src_get_source_cap_ext_entry(pd_port_t *pd_port, pd_event_t *pd_event)
{
	hisi_pd_send_ctrl_msg(pd_port, TCPC_TX_SOP, PD_CTRL_GET_SOURCE_CAP_EXT);
}

void hisi_pe_dr_src_get_source_cap_ext_exit(pd_port_t *pd_port, pd_event_t *pd_event)
{
	hisi_pd_dpm_inform_source_cap_ext(pd_port, pd_event);
}
#endif

void hisi_pe_dr_src_give_sink_cap_ext_entry(pd_port_t *pd_port, pd_event_t *pd_event)
{
	hisi_pd_dpm_send_sink_cap_ext(pd_port);
}

void hisi_pe_dr_snk_get_sink_cap_ext_entry(pd_port_t *pd_port, pd_event_t *pd_event)
{
	hisi_pd_send_ctrl_msg(pd_port, TCPC_TX_SOP, PD_CTRL_GET_SINK_CAP_EXT);
}

void hisi_pe_dr_snk_get_sink_cap_ext_exit(pd_port_t *pd_port, pd_event_t *pd_event)
{
	hisi_pd_dpm_inform_sink_cap_ext(pd_port, pd_event);
}
#endif

