/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: HiPCIe external api.
 *
 * This software is distributed under the terms of the GNU General
 * Public License ("GPL") as published by the Free Software Foundation,
 * either version 2 of that License or (at your option) any later version.
 */
#include "../hi_pcie_external_api.h"
#include "../core/hi_pcie_core.h"
#include "../hi_pcie_defines.h"
#include "../plat/hi_pcie_idle.h"

int hi_pcie_startup(u32 cdx)
{
	struct hi_pcie *pcie = pcie_core_get(cdx);
	if (!pcie) {
		hi_pcie_pr_e("startup %u failed, pcie is null", cdx);
		return -EINVAL;
	}
	return pcie_core_do_startup(pcie);
}

int hi_pcie_power_ctrl(u32 cdx, u32 power_flag)
{
	struct hi_pcie *pcie = pcie_core_get(cdx);
	if (!pcie) {
		hi_pcie_pr_e("power ctrl %d cdx %u failed, pcie is null", power_flag, cdx);
		return -EINVAL;
	}

	return pcie_core_power_ctrl(pcie, power_flag);
}

int hi_pcie_core_setup(u32 cdx)
{
	struct hi_pcie *pcie = pcie_core_get(cdx);
	if (!pcie) {
		hi_pcie_pr_e("ctrl %u setup failed, pcie is null", cdx);
		return -EINVAL;
	}

	return pcie_core_setup(pcie);
}

int hi_pcie_establish_link(u32 cdx)
{
	struct hi_pcie *pcie = pcie_core_get(cdx);
	if (!pcie) {
		hi_pcie_pr_e("ctrl %u establish link failed, pcie is null", cdx);
		return -EINVAL;
	}

	return pcie_core_establish_link(pcie);
}

void hi_pcie_idle_vote(u32 cdx, u32 pdx, bool pro)
{
	hi_pcie_idle_device_vote(cdx, pdx, pro);
}

int hi_pcie_register_power_callback(u32 cdx, u32 pdx,
	int (*poweron_callback)(void *data),
	int (*poweroff_callback)(void *data), void *data)
{
	struct hi_pcie_port *port = pcie_core_port_get(cdx, pdx);
	if (!port) {
		hi_pcie_pr_e("register (%u-%u) failed, port is null", cdx, pdx);
		return -EINVAL;
	}
	port->poweron_callback = poweron_callback;
	port->poweroff_callback = poweroff_callback;
	port->power_callback_data = data;
	return 0;
}

static int hi_pcie_event_adapter(struct notifier_block *nb, unsigned long action, void *data)
{
	struct hi_pcie_event_handler *handler = container_of(nb, struct hi_pcie_event_handler, notify);
	if (handler->fn != NULL)
		handler->fn(handler);
	return 0;
}

static struct atomic_notifier_head *hi_pcie_get_notifier_head(u32 cdx, u32 pdx, u32 event)
{
	struct hi_pcie *pcie;
	struct hi_pcie_port *port;
	struct atomic_notifier_head *head = NULL;

	if (pdx == 0xFFFFFFFF) {
		pcie = pcie_core_get(cdx);
		if (pcie == NULL) {
			hi_pcie_pr_e("invalid ctrl id %u", cdx);
			return NULL;
		}
		head = &pcie->events[event];
	} else {
		port = pcie_core_port_get(cdx, pdx);
		if (port == NULL) {
			hi_pcie_pr_e("invalid ctrl id %u or port id %u", cdx, pdx);
			return NULL;
		}
		head = &port->events[event];
	}

	return head;
}

int hi_pcie_register_event_handler(u32 cdx, u32 pdx, struct hi_pcie_event_handler *handler)
{
	struct atomic_notifier_head *head;

	if ((handler == NULL) || (handler->event >= HI_PCIE_EVENT_MAX)) {
		hi_pcie_pr_e("invalid event parameter");
		return -1;
	}

	handler->notify.notifier_call = hi_pcie_event_adapter;

	head = hi_pcie_get_notifier_head(cdx, pdx, handler->event);
	if (head == NULL) {
		hi_pcie_pr_e("register event failed, invalid event parameter");
		return -1;
	} else {
		atomic_notifier_chain_register(head, &handler->notify);
		return 0;
	}
}

int hi_pcie_unregister_event_handler(u32 cdx, u32 pdx, struct hi_pcie_event_handler *handler)
{
	struct atomic_notifier_head *head;

	if ((handler == NULL) || (handler->event >= HI_PCIE_EVENT_MAX)) {
		hi_pcie_pr_e("invalid event parameter");
		return -1;
	}

	head = hi_pcie_get_notifier_head(cdx, pdx, handler->event);
	if (head == NULL) {
		hi_pcie_pr_e("unregister event failed, invalid event parameter");
		return -1;
	} else {
		atomic_notifier_chain_unregister(head, &handler->notify);
		return 0;
	}
}

void hi_pcie_invoke_event_handler(struct hi_pcie *pcie, u32 pdx, u32 event)
{
	struct atomic_notifier_head *head;
	head = hi_pcie_get_notifier_head(pcie->ctrl_id, pdx, event);
	if (head)
		atomic_notifier_call_chain(head, 0, NULL);
}

int hi_pcie_pm_ctrl(u32 cdx, int power_ops)
{
	int ret = 0;
	struct hi_pcie *pcie = pcie_core_get(cdx);
	if (!pcie) {
		hi_pcie_pr_e("pm_ctrl %u failed, pcie is null", cdx);
		return -EINVAL;
	}

	hi_pcie_pr_i("pm_ctrl %u, power ops %d", cdx, power_ops);

	mutex_lock(&(pcie->pm_lock));

	switch (power_ops) {
	case PCIE_POWER_ON:
	case PCIE_POWER_RESUME:
		ret = pcie_core_usr_resume(pcie, power_ops);
		break;

	case PCIE_POWER_OFF:
	case PCIE_POWER_OFF_NO_PME:
	case PCIE_POWER_SUSPEND:
		ret = pcie_core_usr_suspend(pcie, power_ops);
		break;

	default:
		hi_pcie_pr_e("Invalid power_ops[%d]", power_ops);
		ret = -EINVAL;
		break;
	}

	mutex_unlock(&(pcie->pm_lock));
	return ret;
}

int hi_pcie_lp_ctrl(u32 cdx, bool enable)
{
	struct hi_pcie *pcie = pcie_core_get(cdx);
	if (!pcie) {
		hi_pcie_pr_e("lp_ctrl %u failed, pcie is null", cdx);
		return -EINVAL;
	}

	pcie_core_low_power(pcie, enable);

	return 0;
}

int hi_pcie_cfg_l0sl1(u32 cdx, u32 aspm)
{
	struct hi_pcie_port *port = NULL;
	struct hi_pcie *pcie = pcie_core_get(cdx);
	if (!pcie) {
		hi_pcie_pr_e("cfg_l0sl1 %u failed, pcie is null", cdx);
		return -EINVAL;
	}

	mutex_lock(&pcie->port_lock);
	list_for_each_entry(port, &pcie->ports, node) {
		if (port->disable || atomic_read(&(port->is_link_up)) == 0)
			continue;
		pcie_core_config_l0sl1(port, aspm);
	}
	mutex_unlock(&pcie->port_lock);

	return 0;
}

int hi_pcie_cfg_l1ss(u32 cdx, u32 aspm)
{
	struct hi_pcie_port *port = NULL;
	struct hi_pcie *pcie = pcie_core_get(cdx);
	if (!pcie) {
		hi_pcie_pr_e("cfg_l1ss %u failed, pcie is null", cdx);
		return -EINVAL;
	}

	mutex_lock(&pcie->port_lock);
	list_for_each_entry(port, &pcie->ports, node) {
		if (port->disable || atomic_read(&(port->is_link_up)) == 0)
			continue;
		pcie_core_config_l1ss(port, aspm);
	}
	mutex_unlock(&pcie->port_lock);

	return 0;
}

int hi_pcie_remove_device(u32 cdx, u32 pdx)
{
	struct hi_pcie_port *port = pcie_core_port_get(cdx, pdx);
	if (!port) {
		hi_pcie_pr_e("remove_ep cdx:%u pdx:%u failed, pcie port is null", cdx, pdx);
		return -EINVAL;
	}

	pcie_core_remove_device(port);

	return 0;
}

int hi_pcie_rescan_device(u32 cdx, u32 pdx)
{
	struct hi_pcie_port *port = pcie_core_port_get(cdx, pdx);
	if (!port) {
		hi_pcie_pr_e("rescan_ep cdx:%u pdx:%u failed, pcie port is null", cdx, pdx);
		return -EINVAL;
	}

	pcie_core_rescan_device(port);

	return 0;
}

int hi_pcie_set_speed(u32 cdx, u32 pdx, u16 speed)
{
	struct hi_pcie_port *port = pcie_core_port_get(cdx, pdx);
	if (!port || !port->pcie || !port->pcie->ctrl_ops ||
	    !port->pcie->ctrl_ops->change_speed) {
		hi_pcie_pr_e("set cdx:%u pdx%u speed fail, port param is invalid", cdx, pdx);
		return -EINVAL;
	}

	return port->pcie->ctrl_ops->change_speed(port, speed);
}

int hi_pcie_get_ssid_by_dev(struct device *dev)
{
	struct hi_pcie *pcie = pcie_core_get_by_dev(dev);
	if (!pcie || !pcie->plat_ops || !pcie->plat_ops->get_ssid_by_dev) {
		hi_pcie_pr_e("invalid device");
		return -EINVAL;
	}

	return pcie->plat_ops->get_ssid_by_dev(pcie, to_pci_dev(dev));
}
EXPORT_SYMBOL_GPL(hi_pcie_get_ssid_by_dev);

int hi_pcie_cfg_mps(struct pci_dev *target_dev, u32 cdx, u32 pdx, u32 mps)
{
	int ret;
	u32 mps_cap;
	u32 mps_sup;
	u32 mps_cfg;
	u32 mps_tmp;

	struct hi_pcie_port *port = pcie_core_port_get(cdx, pdx);
	if (port == NULL || port->pcie == NULL || ((mps <= 0) && (port->mps == 0))) {
		hi_pcie_pr_e("set (%u-%u) mps (%u) failed, port is null", cdx, pdx, mps);
		return -1;
	}
 
	if (mps <= 0 && port->mps != 0)
		mps = port->mps;

	mps_cap = pci_find_capability(target_dev, PCI_CAP_ID_EXP);
	pci_read_config_dword(target_dev, mps_cap + PCI_EXP_DEVCAP, &mps_sup);
	mps_sup &= PCI_EXP_DEVCAP_PAYLOAD;
	hi_pcie_pr_i("remote mps sup = 0x%x, mps = 0x%x", mps_sup, mps);

	mps_tmp = ffs(mps) - 8;
	if (mps_tmp > mps_sup || mps > PCI_DEVCTL_SUP_PAYLOAD_512B) {
		hi_pcie_pr_i("mps is invalid, (ffs(mps) - 8) = 0x%x", mps_tmp);
		return 0;
	}

	mps_tmp <<= 5;

	/* set local mps */
	mps_cfg = port->pcie->ctrl_ops->read_conf(port, MPS_CTRL_STATUS, PCIE_SIZE_WORD);
	mps_cfg &= ~PCI_EXP_DEVCTL_PAYLOAD;
	mps_cfg |= mps_tmp;
	port->pcie->ctrl_ops->write_conf(port, MPS_CTRL_STATUS, PCIE_SIZE_DWORD, mps_cfg);

	ret = pcie_set_mps(target_dev, mps);
	if (ret != 0) {
		hi_pcie_pr_e("set remote mps failed %d, local mps set default", ret);
		port->pcie->ctrl_ops->write_conf(port, MPS_CTRL_STATUS, PCIE_SIZE_DWORD, PCI_DEVCTL_SUP_PAYLOAD_128B);
	}

	return ret;
}