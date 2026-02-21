/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: HiPCIe legacy api adapter.
 *
 * This software is distributed under the terms of the GNU General
 * Public License ("GPL") as published by the Free Software Foundation,
 * either version 2 of that License or (at your option) any later version.
 */

#include <linux/module.h>

#include "pcie-kport-api.h"
#include "../core/hi_pcie_core.h"
#include "../hi_pcie_external_api.h"
#include "../hi_pcie_internal_api.h"
#include "../ctrl/hi_pcie_ctrl_dfx.h"

static struct hi_pcie_event_handler kport_event_handler[RC_ID_MAX];

static void kport_event_callback(struct hi_pcie_event_handler *event_handler)
{
	struct pcie_kport_register_event *register_event = NULL;

	hi_pcie_pr_i("++%s++", __func__);

	register_event = (struct pcie_kport_register_event *)event_handler->data;
	if ((register_event) && (register_event->callback != NULL) &&
		((event_handler->event == HI_PCIE_EVENT_LINKDOWN) ||(event_handler->event == HI_PCIE_EVENT_AXI_TIMEOUT))) {
		struct pcie_kport_notify *notify = &register_event->notify;

		if (event_handler->event == HI_PCIE_EVENT_LINKDOWN)
			notify->event = PCIE_KPORT_EVENT_LINKDOWN;
		else
			notify->event = PCIE_KPORT_EVENT_CPL_TIMEOUT;

		notify->user = register_event->user;
		hi_pcie_pr_i("Callback for the event : %u", notify->event);
		register_event->callback(notify);
	}
	hi_pcie_pr_i("--%s--", __func__);
}

int pcie_kport_register_event(struct pcie_kport_register_event *reg)
{
	struct pci_dev *dev = NULL;
	struct hi_pcie *pcie = NULL;

	hi_pcie_pr_i("++%s++", __func__);

	if (reg == NULL) {
		hi_pcie_pr_e("reg == NULL");
		return -EINVAL;
	}

	dev = (struct pci_dev *)reg->user;
	pcie = (struct hi_pcie *)(dev->bus->sysdata);

	if (reg->events == PCIE_KPORT_EVENT_LINKDOWN) {
		kport_event_handler[pcie->ctrl_id].event = HI_PCIE_EVENT_LINKDOWN;
	} else if (reg->events == PCIE_KPORT_EVENT_CPL_TIMEOUT) {
		kport_event_handler[pcie->ctrl_id].event = HI_PCIE_EVENT_AXI_TIMEOUT;
	} else {
		hi_pcie_pr_i("EP does not register this event : %u", reg->events);
		return -1;
	}

	kport_event_handler[pcie->ctrl_id].data = reg;
	kport_event_handler[pcie->ctrl_id].fn = kport_event_callback;
	hi_pcie_register_event_handler(pcie->ctrl_id, 0, &kport_event_handler[pcie->ctrl_id]);

	return 0;
}
EXPORT_SYMBOL_GPL(pcie_kport_register_event);

int pcie_kport_deregister_event(struct pcie_kport_register_event *reg)
{
	struct pci_dev *dev = NULL;
	struct hi_pcie *pcie = NULL;
	struct hi_pcie_port *port = NULL;

	hi_pcie_pr_i("++%s++", __func__);

	if (reg == NULL) {
		hi_pcie_pr_e("reg == NULL");
		return -EINVAL;
	}

	dev = (struct pci_dev *)reg->user;
	pcie = (struct hi_pcie *)(dev->bus->sysdata);

	list_for_each_entry(port, &pcie->ports, node) {
		if (port->id == 0) {
			(void)flush_work(&port->events_work[HI_PCIE_EVENT_LINKDOWN]);
			(void)flush_work(&port->events_work[HI_PCIE_EVENT_AXI_TIMEOUT]);
		}
	}

	kport_event_handler[pcie->ctrl_id].data = NULL;
	kport_event_handler[pcie->ctrl_id].fn = NULL;
	hi_pcie_unregister_event_handler(pcie->ctrl_id, 0, &kport_event_handler[pcie->ctrl_id]);

	return 0;
}
EXPORT_SYMBOL_GPL(pcie_kport_deregister_event);

int pcie_kport_pm_control(int power_ops, u32 cdx)
{
	hi_pcie_pr_i("++%s++", __func__);

	return hi_pcie_pm_ctrl(cdx, power_ops);
}
EXPORT_SYMBOL_GPL(pcie_kport_pm_control);

int pcie_kport_ep_off(u32 cdx)
{
	hi_pcie_pr_i("++%s++ no use func", __func__);
	return 0;
}
EXPORT_SYMBOL_GPL(pcie_kport_ep_off);

int pcie_kport_lp_ctrl(u32 cdx, u32 enable)
{
	hi_pcie_pr_i("++%s++", __func__);

	return hi_pcie_lp_ctrl(cdx, enable);
}
EXPORT_SYMBOL_GPL(pcie_kport_lp_ctrl);

int pcie_kport_enumerate(u32 cdx)
{
	hi_pcie_pr_i("++%s++", __func__);
	return hi_pcie_startup(cdx);
}
EXPORT_SYMBOL_GPL(pcie_kport_enumerate);

int pcie_kport_remove_ep(u32 cdx)
{
	struct hi_pcie_port *port = NULL;
	struct hi_pcie *pcie = pcie_core_get(cdx);
	if (!pcie) {
		hi_pcie_pr_e("remove_ep %u failed, pcie is null", cdx);
		return -EINVAL;
	}

	hi_pcie_pr_i("++%s++", __func__);

	mutex_lock(&pcie->port_lock);
	list_for_each_entry(port, &pcie->ports, node) {
		if (port->disable)
			continue;
		pcie_core_remove_device(port);
	}
	mutex_unlock(&pcie->port_lock);

	return 0;
}
EXPORT_SYMBOL_GPL(pcie_kport_remove_ep);

int pcie_kport_rescan_ep(u32 cdx)
{
	struct hi_pcie_port *port = NULL;
	struct hi_pcie *pcie = pcie_core_get(cdx);
	if (!pcie) {
		hi_pcie_pr_e("rescan_ep %u failed, pcie is null", cdx);
		return -EINVAL;
	}

	hi_pcie_pr_i("++%s++", __func__);

	mutex_lock(&pcie->port_lock);
	list_for_each_entry(port, &pcie->ports, node) {
		if (port->disable)
			continue;
		pcie_core_rescan_device(port);
	}
	mutex_unlock(&pcie->port_lock);

	return 0;
}
EXPORT_SYMBOL_GPL(pcie_kport_rescan_ep);

int pcie_kport_ep_link_ltssm_notify(u32 cdx, u32 link_status)
{
	hi_pcie_pr_i("++%s++", __func__);
	return 0;
}
EXPORT_SYMBOL_GPL(pcie_kport_ep_link_ltssm_notify);

int pcie_kport_power_notifiy_register(u32 cdx, int (*poweron)(void *data),
				int (*poweroff)(void *data), void *data)
{
	hi_pcie_pr_i("++%s++", __func__);

	return hi_pcie_register_power_callback(cdx, 0, poweron, poweroff, data);
}
EXPORT_SYMBOL_GPL(pcie_kport_power_notifiy_register);

void pcie_kport_key_info_dump(void)
{
	struct hi_pcie_port *port;

	hi_pcie_pr_i("++%s++", __func__);

	port = pcie_core_port_get(0, 0);
	if (!port || !port->pcie || !port->pcie->dfx_ops ||
		!port->pcie->dfx_ops->dump_port_info) {
		hi_pcie_pr_e("dump key-info pcie 0 failed, port param is null");
		return;
	}

	port->pcie->dfx_ops->dump_port_info(port, 0);
}
EXPORT_SYMBOL_GPL(pcie_kport_key_info_dump);

void pcie_kport_outbound_atu(u32 cdx, int index, int type, u64 cpu_addr,
			     u64 pci_addr, u32 size)
{
	hi_pcie_pr_i("++%s++", __func__);
	hi_pcie_setup_outbound(cdx, (u32)index, (u32)cpu_addr, (u32)pci_addr, size);
}
EXPORT_SYMBOL_GPL(pcie_kport_outbound_atu);

int pcie_kport_set_host_speed(u32 cdx, enum link_speed speed)
{
	hi_pcie_pr_i("++%s++", __func__);

	return hi_pcie_set_speed(cdx, 0, speed);
}
EXPORT_SYMBOL_GPL(pcie_kport_set_host_speed);

void pcie_kport_refclk_device_vote(u32 ep_type, u32 cdx, u32 vote)
{
	hi_pcie_pr_i("++%s++", __func__);
	hi_pcie_idle_vote(cdx, 0, vote);
	return;
}
EXPORT_SYMBOL_GPL(pcie_kport_refclk_device_vote);

enum dw_link_status {
	LTSSM_PWROFF = 0x0,
	LTSSM_CPLC = 0x3,
	LTSSM_L0 = 0x11,
	LTSSM_L0S = 0x12,
	LTSSM_L1 = 0x14,
	LTSSM_L1_1 = 0x4000,
	LTSSM_L1_2 = 0xC000,
	LTSSM_OTHERS = 0xFFFF,
};

int show_link_state(u32 cdx)
{
	u32 step;
	u32 state = 0;
	static struct {
		u32 dw_link_status;
		u32 hi_ctrl_status;
	} link_status_map[] = {
		{ LTSSM_PWROFF, PCIE_LINKSTATE_UNKNOWN },
		{ LTSSM_CPLC, PCIE_LINKSTATE_POLLING },
		{ LTSSM_L0, PCIE_LINKSTATE_L0 },
		{ LTSSM_L0S, PCIE_LINKSTATE_L0S },
		{ LTSSM_L1, PCIE_LINKSTATE_L1 },
		{ LTSSM_L1_1, PCIE_LINKSTATE_L1D1 },
		{ LTSSM_L1_2, PCIE_LINKSTATE_L1D2 },
		{ LTSSM_OTHERS, PCIE_LINKSTATE_UNKNOWN },
	};

	struct hi_pcie_port *port = pcie_core_port_get(cdx, 0);
	if (!port || !port->pcie || !port->pcie->dfx_ops
		|| !port->pcie->dfx_ops->get_link_state) {
		hi_pcie_pr_e("show_link_state %u failed, pcie is null", cdx);
		return -EINVAL;
	}
	state = port->pcie->dfx_ops->get_link_state(port);
	for (step = 0; step < ARRAY_SIZE(link_status_map); step++) {
		if (state == link_status_map[step].hi_ctrl_status)
			return (int)link_status_map[step].dw_link_status;
	}

	hi_pcie_pr_e("pcie %u ctrl_state: 0x%x is out of map", cdx ,state);

	return -EINVAL;
}
EXPORT_SYMBOL_GPL(show_link_state);

#ifdef CONFIG_HIPCIE_SLT
int pcie_slt_hook_register(u32 cdx, u32 device_type, int (*init)(void *),
			   int (*prepare)(void *), int (*shutdown)(void *),
			   int (*startup)(void *),
			   int (*data_transfer)(void *, u32, u32))
{
	return hi_pcie_slt_hook_register(cdx, 0, device_type,
					init, prepare, shutdown, startup, data_transfer);
}
EXPORT_SYMBOL(pcie_slt_hook_register);
#endif
