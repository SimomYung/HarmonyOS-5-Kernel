/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: PCIe controller core.
 *
 * This software is distributed under the terms of the GNU General
 * Public License ("GPL") as published by the Free Software Foundation,
 * either version 2 of that License or (at your option) any later version.
 */

#include "hi_pcie_core.h"

#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/property.h>
#include <linux/of.h>
#include <securec.h>
#include <linux/iommu/mm_iommu.h>

#include "../hi_pcie_defines.h"
#include "../hi_pcie_external_api.h"
#ifdef CONFIG_PCIE_HCD_LDK
#include "hi_pcie_udk_comm.h"
#endif
#include <platform_include/basicplatform/linux/hck/pcie/hck_pcie.h>

DEFINE_SPINLOCK(g_pcie_core_list_lock);
struct list_head g_pcie_core_list = LIST_HEAD_INIT(g_pcie_core_list);

static void hck_pcie_bypass_pm(struct pci_dev *dev, bool *err)
{
	struct pci_dev *ep_dev = NULL;
	u32 class;

	if (!dev) {
		hi_pcie_pr_e("NULL Param");
		*err = false;
		return;
	}

	ep_dev = dev;
	if (pci_is_root_bus(dev->bus))
		ep_dev = list_first_entry(&dev->subordinate->devices,
					  struct pci_dev, bus_list);

	class = ep_dev->class >> 8;
	hi_pcie_pr_i("EP class version: 0x%x", class);

	if (class == PCI_CLASS_COMMUNICATION_MODEM)
		*err = true;
	else
		*err = false;
}

#ifdef CONFIG_PCIE_HCD_LDK
int hi_pcie_get_ctrlid_by_dev(struct pci_dev *pdev)
{
	struct hi_pcie *pcie = NULL;
	if (pdev == NULL) {
		hi_pcie_pr_e("%s: pdev is NULL", __func__);
		return -EINVAL;
	}
	pcie = pcie_core_get_by_dev(&pdev->dev);
	if (pcie == NULL) {
		hi_pcie_pr_e("%s: pcie get failed", __func__);
		return -EINVAL;
	}

	return pcie->ctrl_id;
}
EXPORT_SYMBOL(hi_pcie_get_ctrlid_by_dev);

static void hck_pcie_need_udk_irq(struct pci_dev *dev, bool *err)
{
	*err = device_property_read_bool(&dev->bus->dev, "need_udk_irq");
}
#endif

static void register_trace_hck_pcie(struct platform_device *pdev)
{
	static bool registered = false;
	bool bypass_pm;
#ifdef CONFIG_PCIE_HCD_LDK	
	static bool udk_hck_registered = false;

	if (udk_hck_registered == false) {
		REGISTER_HCK_VH(kport_pcie_need_udk_irq, hck_pcie_need_udk_irq);
		udk_hck_registered = true;
	}
#endif
	bypass_pm = device_property_read_bool(&pdev->dev, "port_bypass_pm");
	if (bypass_pm) {
		hi_pcie_pr_i("[%s] bypass_pm", __func__);
		return;
	}

	if (registered == false) {
		REGISTER_HCK_VH(kport_pcie_bypass_pm, hck_pcie_bypass_pm);
		registered = true;
	}
}

static struct hi_pcie *pcie_core_new(struct platform_device *pdev)
{
	struct hi_pcie *pcie;
	pcie = devm_kzalloc(&pdev->dev, sizeof(struct hi_pcie), GFP_KERNEL);
	if (!pcie) {
		hi_pcie_pr_e("no memory for allocation of pcie instance");
		return NULL;
	}

	pcie->pdev = pdev;
	pcie->dev = &pdev->dev;
	INIT_LIST_HEAD(&pcie->ports);
	mutex_init(&pcie->port_lock);
	mutex_init(&pcie->power_lock);
	mutex_init(&(pcie->pm_lock));
	return pcie;
}

void pcie_core_add(struct hi_pcie *pcie)
{
	unsigned long flag;

	spin_lock_irqsave(&g_pcie_core_list_lock, flag);

	list_add_tail(&pcie->node, &g_pcie_core_list);

	spin_unlock_irqrestore(&g_pcie_core_list_lock, flag);
}

void pcie_core_remove(u32 id)
{
	struct hi_pcie *pcie = NULL;
	struct hi_pcie *n = NULL;
	unsigned long flag;

	spin_lock_irqsave(&g_pcie_core_list_lock, flag);

	list_for_each_entry_safe(pcie, n, &g_pcie_core_list, node) {
		if (pcie->ctrl_id == id) {
			list_del(&pcie->node);
			break;
		}
	}

	spin_unlock_irqrestore(&g_pcie_core_list_lock, flag);
}

static void pcie_port_events_linkdown_work(struct work_struct *work)
{
	struct hi_pcie_port *port = container_of(work, struct hi_pcie_port, events_work[HI_PCIE_EVENT_LINKDOWN]);
	port->pcie->err_code = PCIE_ERRCODE_LINKDOWN;

	pcie_core_dump_debug_info(port);

	hi_pcie_invoke_event_handler(port->pcie, port->id, HI_PCIE_EVENT_LINKDOWN);
}

static void pcie_port_events_axitimeout_work(struct work_struct *work)
{
	struct hi_pcie_port *port = container_of(work, struct hi_pcie_port, events_work[HI_PCIE_EVENT_AXI_TIMEOUT]);

	pcie_core_dump_debug_info(port);

	hi_pcie_invoke_event_handler(port->pcie, port->id, HI_PCIE_EVENT_AXI_TIMEOUT);
}

static void pcie_core_work_init(struct hi_pcie_port *port)
{
	struct hi_pcie *pcie = port->pcie;
	if (pcie == NULL)
		return;

	INIT_WORK(&port->events_work[HI_PCIE_EVENT_LINKDOWN], pcie_port_events_linkdown_work);
	INIT_WORK(&port->events_work[HI_PCIE_EVENT_AXI_TIMEOUT], pcie_port_events_axitimeout_work);

	ATOMIC_INIT_NOTIFIER_HEAD(&(port->events[HI_PCIE_EVENT_LINKDOWN]));
	ATOMIC_INIT_NOTIFIER_HEAD(&(port->events[HI_PCIE_EVENT_AXI_TIMEOUT]));
	ATOMIC_INIT_NOTIFIER_HEAD(&(pcie->events[HI_PCIE_EVENT_LINKDOWN]));
	ATOMIC_INIT_NOTIFIER_HEAD(&(pcie->events[HI_PCIE_EVENT_AXI_TIMEOUT]));
}

void pcie_core_flush_work(struct hi_pcie *pcie)
{
	struct hi_pcie_port *port = NULL;
	struct hi_pcie_port *n = NULL;

	list_for_each_entry_safe(port, n, &pcie->ports, node) {
		flush_work(&port->events_work[HI_PCIE_EVENT_LINKDOWN]);
		flush_work(&port->events_work[HI_PCIE_EVENT_AXI_TIMEOUT]);
	}
}

struct hi_pcie_port *pcie_core_port_new(struct platform_device *pdev,
	struct hi_pcie *pcie, u32 id, void *priv)
{
	struct hi_pcie_port *port = NULL;

	port = devm_kzalloc(&pdev->dev, sizeof(struct hi_pcie_port), GFP_KERNEL);
	if (!port) {
		hi_pcie_pr_e("no memory for allocation of pcie port instance");
		return NULL;
	}

	port->id = id;
	port->priv = priv;
	port->pcie = pcie;

	pcie_core_work_init(port);
	return port;
}

void pcie_core_port_add(struct hi_pcie *pcie, struct hi_pcie_port *port)
{
	mutex_lock(&pcie->port_lock);
	list_add_tail(&port->node, &pcie->ports);
	mutex_unlock(&pcie->port_lock);
}

struct hi_pcie *pcie_core_get(u32 id)
{
	struct hi_pcie *pcie = NULL;
	struct hi_pcie *n = NULL;
	unsigned long flag;

	spin_lock_irqsave(&g_pcie_core_list_lock, flag);

	list_for_each_entry_safe(pcie, n, &g_pcie_core_list, node) {
		if (pcie->ctrl_id == id) {
			spin_unlock_irqrestore(&g_pcie_core_list_lock, flag);
			return pcie;
		}
	}

	spin_unlock_irqrestore(&g_pcie_core_list_lock, flag);

	return NULL;
}

struct hi_pcie *pcie_core_get_by_dev(struct device *dev)
{
	struct hi_pcie *pcie = NULL;
	struct hi_pcie *n = NULL;
	unsigned long flag;
	struct pci_dev *pdev;
	struct pci_host_bridge *host_bridge;

	if (!dev_is_pci(dev))
		return NULL;

	pdev = to_pci_dev(dev);
	host_bridge = pci_find_host_bridge(pdev->bus);

	spin_lock_irqsave(&g_pcie_core_list_lock, flag);

	list_for_each_entry_safe(pcie, n, &g_pcie_core_list, node) {
		if (pcie->host_bridge == host_bridge) {
			spin_unlock_irqrestore(&g_pcie_core_list_lock, flag);
			return pcie;
		}
	}

	spin_unlock_irqrestore(&g_pcie_core_list_lock, flag);

	return NULL;
}

struct hi_pcie_port *pcie_core_port_get(u32 cdx, u32 pdx)
{
	struct hi_pcie_port *port = NULL;
	struct hi_pcie_port *n = NULL;
	struct hi_pcie *pcie = pcie_core_get(cdx);
	if (!pcie) {
		hi_pcie_pr_e("get pcie instance failed by %u", cdx);
		return NULL;
	}
	mutex_lock(&pcie->port_lock);
	list_for_each_entry_safe(port, n, &pcie->ports, node) {
		if (port->id == pdx) {
			mutex_unlock(&pcie->port_lock);
			return port;
		}
	}
	mutex_unlock(&pcie->port_lock);
	return NULL;
}

static int pcie_core_save_port_state(struct hi_pcie_port *port)
{
	int ret = 0;
	struct hi_pcie *pcie = port->pcie;

	if (pcie->ctrl_ops->save_state) {
		ret = pcie->ctrl_ops->save_state(port);
		if (ret != 0)
			return ret;
	}
	ret = pci_save_state(port->self_dev);
	if (ret != 0) {
		hi_pcie_pr_e("save pci state failed");
		return -EINVAL;
	}
	port->saved_state = pci_store_saved_state(port->self_dev);

	return 0;
}

int pcie_core_save_state(struct hi_pcie *pcie)
{
	struct hi_pcie_port *port = NULL;
	mutex_lock(&pcie->port_lock);
	list_for_each_entry(port, &pcie->ports, node) {
		if (port->disable || atomic_read(&(port->is_link_up)) == 0)
			continue;
		pcie_core_save_port_state(port);
	}
	mutex_unlock(&pcie->port_lock);
	return 0;
}

void pcie_core_dump_debug_info(struct hi_pcie_port *port)
{
	if (port->pcie->dfx_ops != NULL) {
		if (port->pcie->dfx_ops->dump_ltssm_trace != NULL)
			port->pcie->dfx_ops->dump_ltssm_trace(port, 0);

		if (port->pcie->dfx_ops->dump_tlpm_trace != NULL)
			port->pcie->dfx_ops->dump_tlpm_trace(port);

		if (port->pcie->dfx_ops->dump_port_info != NULL)
			port->pcie->dfx_ops->dump_port_info(port, 0);

		if (port->pcie->dfx_ops->dump_ctrl_info != NULL)
			port->pcie->dfx_ops->dump_ctrl_info(port->pcie, 0);
	}

	if ((port->pcie->phy_dfx_ops != NULL) && (port->pcie->idle == NULL)) {
		if (port->pcie->phy_dfx_ops->phy_param_dump != NULL)
			port->pcie->phy_dfx_ops->phy_param_dump(&port->pcie->phy);
	}
}

static int pcie_core_restore_port_state(struct hi_pcie_port *port)
{
	int ret = 0;
	struct hi_pcie *pcie = port->pcie;

	if (pcie->ctrl_ops->restore_state) {
		ret = pcie->ctrl_ops->restore_state(port);
		if (ret != 0)
			return ret;
	}
	pci_load_saved_state(port->self_dev, port->saved_state);
	pci_restore_state(port->self_dev);
	pci_load_saved_state(port->self_dev, port->saved_state);

	return 0;
}

static int pcie_core_restore_state(struct hi_pcie *pcie)
{
	struct hi_pcie_port *port = NULL;
	list_for_each_entry(port, &pcie->ports, node) {
		if (port->disable || atomic_read(&(port->is_link_up)) == 0)
			continue;
		pcie_core_restore_port_state(port);
	}
	return 0;
}

int pcie_core_establish_link(struct hi_pcie *pcie)
{
	int ret;

	if (!pcie->plat_ops->start_link || !pcie->ctrl_ops ||
		!pcie->ctrl_ops->ltssm_enable)
		return 0;

	ret = pcie->plat_ops->start_link(pcie);
	if (ret)
		return ret;

	return pcie->ctrl_ops->ltssm_enable(pcie);
}

int pcie_core_low_power(struct hi_pcie *pcie, bool enable)
{
	struct hi_pcie_port *port = NULL;
	if (!pcie->ctrl_ops->low_power)
		return 0;

	mutex_lock(&pcie->port_lock);
	list_for_each_entry(port, &pcie->ports, node) {
		if (port->disable || atomic_read(&(port->is_link_up)) == 0)
			continue;
		pcie->ctrl_ops->low_power(port, enable);
	}
	mutex_unlock(&pcie->port_lock);

	return 0;
}

int pcie_core_fixup(struct hi_pcie *pcie)
{
	if (!pcie->plat_ops || !pcie->plat_ops->fixup_cfg)
		return 0;

	return pcie->plat_ops->fixup_cfg(pcie);
}

static int pcie_core_shutdown_prepare(struct hi_pcie *pcie)
{
	struct hi_pcie_port *port = NULL;
	if (!pcie->ctrl_ops->shutdown_prepare)
		return 0;
	hi_pcie_pr_i("hi_pcie shut down prepare");

	mutex_lock(&pcie->port_lock);
	list_for_each_entry(port, &pcie->ports, node) {
		if (port->disable || atomic_read(&(port->is_link_up)) == 0)
			continue;
		pcie->ctrl_ops->shutdown_prepare(port);
	}
	mutex_unlock(&pcie->port_lock);

	return 0;
}

static int pcie_core_get_pci_dev(struct hi_pcie *pcie)
{
	struct hi_pcie_port *port = NULL;
	u16 dev_id = 0;
	u16 vendor_id = 0;
	struct pci_dev *dev = NULL;

	mutex_lock(&pcie->port_lock);
	list_for_each_entry(port, &pcie->ports, node) {
		if (port->disable || atomic_read(&(port->is_link_up)) == 0)
			continue;
		vendor_id = pcie->ctrl_ops->read_conf(port, PCI_VENDOR_ID, PCIE_SIZE_WORD);
		dev_id = pcie->ctrl_ops->read_conf(port, PCI_DEVICE_ID, PCIE_SIZE_WORD);
		hi_pcie_pr_e("port %u : dev id %x, vendor id %x", port->port_id, dev_id, vendor_id);
		port->self_dev = pci_get_device(vendor_id, dev_id, NULL);
		if (!port->self_dev) {
			hi_pcie_pr_e("get pci dev failed, vendor = %u, dev = %u", vendor_id, dev_id);
			mutex_unlock(&pcie->port_lock);
			return -1;
		}
		list_for_each_entry(dev, &port->self_dev->subordinate->devices, bus_list) {
			port->target_dev = dev;
			if(port->target_dev != NULL)
				break;
		}
		if(!port->target_dev) {
			hi_pcie_pr_e("get pci target dev failed");
			mutex_unlock(&pcie->port_lock);
			return -1;
		}
	}

	mutex_unlock(&pcie->port_lock);

	return 0;
}

int pcie_core_probe_devices(struct hi_pcie *pcie)
{
	int ret = 0;

	if (pcie->ctrl_ops->probe) {
		ret = pcie->ctrl_ops->probe(pcie);
		if (ret != 0)
			return ret;
	}

	return pcie_core_get_pci_dev(pcie);
}

static void pcie_core_update_port_devid(struct hi_pcie_port *port)
{
	struct hi_pcie *pcie = port->pcie;
	if (pcie->mode == PCIE_MODE_RC)
		pcie->ctrl_ops->write_conf(port, PCI_VENDOR_ID, PCIE_SIZE_DWORD, port->dev_ven_id);
	else
		pcie->ctrl_ops->write_conf(port, PCI_VENDOR_ID, PCIE_SIZE_DWORD, PCIE_DEFAULT_EP_ID);

	hi_pcie_pr_i("ctrl_id %u : vendor device id = %x",
		port->pcie->ctrl_id, pcie->ctrl_ops->read_conf(port, PCI_VENDOR_ID, PCIE_SIZE_DWORD));
}

static void pcie_core_update_devid(struct hi_pcie *pcie)
{
	struct hi_pcie_port *port = NULL;

	mutex_lock(&pcie->port_lock);

	list_for_each_entry(port, &pcie->ports, node)
		pcie_core_update_port_devid(port);

	mutex_unlock(&pcie->port_lock);
}

int pcie_core_power_ctrl(struct hi_pcie *pcie, u32 power_flag)
{
	int ret = 0;

	mutex_lock(&pcie->power_lock);

	if (power_flag == PCIE_POWER_ON || power_flag == PCIE_POWER_RESUME)
		ret = pcie->plat_ops->plat_on(pcie);
	else if (power_flag == PCIE_POWER_OFF || power_flag == PCIE_POWER_SUSPEND ||
		power_flag == PCIE_POWER_OFF_NO_PME || power_flag == PCIE_POWER_OFF_NO_LINK)
		ret = pcie->plat_ops->plat_off(pcie);

	mutex_unlock(&pcie->power_lock);

	return ret;
}

int pcie_core_setup(struct hi_pcie *pcie)
{
	int ret = 0;
	if (pcie->ctrl_ops->setup) {
		ret = pcie->ctrl_ops->setup(pcie);
		if (ret == 0)
			pcie_core_update_devid(pcie);
	}
	return ret;
}

int pcie_core_do_startup(struct hi_pcie *pcie)
{
	int ret = 0;

	hi_pcie_pr_i("++%s++", __func__);

	if (atomic_read(&(pcie->is_enumerated))) {
		hi_pcie_pr_e("Enumeration was done successed before");
		return 0;
	}

#ifdef CONFIG_FMEA_FAULT_INJECTION
#ifdef CONFIG_HIPCIE_TEST
	ret = stub_pcie_fault_injection();
	if (ret != 0) {
		hi_pcie_pr_e("pcie stub pwron failed");
		return ret;
	}
#endif
#endif

	ret = pcie_core_power_ctrl(pcie, PCIE_POWER_ON);
	if (ret != 0)
		return ret;
	ret = pcie_core_setup(pcie);
	if (ret != 0)
		goto POWER_OFF;

	ret = pcie_core_establish_link(pcie);
	if (pcie->mode != PCIE_MODE_RC || pcie->loopback_type != PCIE_LOOPBACK_NONE) {
		hi_pcie_pr_e("ep or loopback mode, no need probe, success");
		return 0;
	}

	if (ret != 0) {
		hi_pcie_pr_i("establish link failed");
		goto POWER_OFF;
	}

	hi_pcie_pr_i("establish link success");

	ret = pcie_core_probe_devices(pcie);
	if (ret != 0)
		goto POWER_OFF;

	ret = pcie_core_fixup(pcie);
	if (ret != 0)
		hi_pcie_pr_e("pcie fixup cfg fail");

	ret = pcie_core_save_state(pcie);
	if (ret != 0)
		goto POWER_OFF;

	ret = pcie_core_low_power(pcie, !pcie->aspm_dis);
	if (ret != 0)
		hi_pcie_pr_e("pcie lowpower cfg failed");

	pcie->err_code = PCIE_ERRCODE_OK;

	atomic_set(&(pcie->is_enumerated), 1);
	hi_pcie_pr_i("--%s--", __func__);
	return 0;

POWER_OFF:
	if (pcie_core_power_ctrl(pcie, PCIE_POWER_OFF))
		hi_pcie_pr_e("failed to poweroff");

	return ret;
}

void pcie_core_remove_device(struct hi_pcie_port *port)
{
	struct pci_dev *dev = NULL;
	struct pci_dev *temp = NULL;

	pci_lock_rescan_remove();

	list_for_each_entry_safe(dev, temp, &port->self_dev->subordinate->devices, bus_list)
		pci_stop_and_remove_bus_device(dev);

	pci_unlock_rescan_remove();

	port->target_dev = NULL;
}

void pcie_core_rescan_device(struct hi_pcie_port *port)
{
	u32 size;
	struct pci_dev *dev = NULL;
	struct pci_dev *temp = NULL;

	pci_load_saved_state(port->self_dev, port->saved_state);

	pci_restore_state(port->self_dev);

	pcie_core_update_port_devid(port);

	pci_lock_rescan_remove();

	size = pci_rescan_bus_bridge_resize(port->self_dev);
	hi_pcie_pr_i("scaned %d pcie devices", size);

	pci_unlock_rescan_remove();

	list_for_each_entry_safe(dev, temp, &port->self_dev->subordinate->devices, bus_list) {
		port->target_dev = dev;
		if(port->target_dev != NULL)
			break;
	}
}

static void pcie_core_parse_aspm_config(struct platform_device *pdev, struct hi_pcie *pcie)
{
	int ret;

	ret = device_property_read_u32(&pdev->dev, "ep_l1ss_ctrl2", &pcie->ep_l1ss_ctrl2);
	if (ret != 0) {
		pcie->ep_l1ss_ctrl2 = 0x0;
		hi_pcie_pr_e("get ep_l1ss_ctrl2 config failed, set default[0x%x]", pcie->ep_l1ss_ctrl2);
	}
	hi_pcie_pr_i("ep_l1ss_ctrl2:0x%x ", pcie->ep_l1ss_ctrl2);

	ret = device_property_read_u32(&pdev->dev, "ep_ltr_latency", &pcie->ep_ltr_latency);
	if (ret != 0) {
		pcie->ep_ltr_latency = 0x0;
		hi_pcie_pr_e("get ep_ltr_latency config failed, set default[0x%x]", pcie->ep_ltr_latency);
	}
	hi_pcie_pr_i("ep_ltr_latency:0x%x ", pcie->ep_ltr_latency);

	ret = device_property_read_u32(&pdev->dev, "l1ss_ctrl1", &pcie->l1ss_ctrl1);
	if (ret != 0) {
		pcie->l1ss_ctrl1 = 0x0;
		hi_pcie_pr_e("get l1ss_ctrl1 config failed, set default[0x%x]", pcie->l1ss_ctrl1);
	}
	hi_pcie_pr_i("l1ss_ctrl1:0x%x ", pcie->l1ss_ctrl1);
}

static int pcie_core_parse_config(struct platform_device *pdev, struct hi_pcie *pcie)
{
	int ret;

	ret = device_property_read_u32(&pdev->dev, "ctrl_id", &pcie->ctrl_id);
	if (ret != 0) {
		hi_pcie_pr_e("get id config failed");
		return ret;
	}

	ret = device_property_read_u32(&pdev->dev, "mode", &pcie->mode);
	if (ret != 0) {
		hi_pcie_pr_e("get mode config failed");
		return ret;
	}
	ret = device_property_read_u32(&pdev->dev, "port_num", &pcie->port_num);
	if (ret != 0) {
		hi_pcie_pr_e("get port_num config failed");
		return ret;
	}
	ret = device_property_read_u32(&pdev->dev, "chip_type", &pcie->chip_type);
	if (ret != 0) {
		hi_pcie_pr_e("get chip_type config failed, set default FPGA");
		pcie->chip_type = BOARD_FPGA;
	}
	hi_pcie_pr_i("ctrl_id:%d, chip_type:%d ", pcie->ctrl_id, pcie->chip_type);
	ret = device_property_read_u32(&pdev->dev, "mult_ctrler", &pcie->mult_ctrler);
	if (ret != 0) {
		hi_pcie_pr_e("get mult_ctrler config failed, set default[0x0]");
		pcie->mult_ctrler = 0;
	}
	pcie->bypass_smmu = device_property_read_bool(&pdev->dev, "bypass_smmu");
#ifdef CONFIG_PCIE_HCD_LDK
	pcie->udk_local_irq = device_property_read_bool(&pdev->dev, "need_udk_irq");
#endif
	pcie->auto_enumerate = device_property_read_bool(&pdev->dev, "auto_enumerate");

	pcie->aspm_dis = device_property_read_bool(&pdev->dev, "aspm_dis");
	hi_pcie_pr_i("aspm_dis:%d ", pcie->aspm_dis);

	ret = device_property_read_u32(&pdev->dev, "fnpll_fsm_ctrl", &pcie->fnpll_fsm_ctrl);
	if (ret != 0) {
		hi_pcie_pr_e("get fnpll_fsm_ctrl config failed, set default[PCIE_PLL_FSM_CLOSE]");
		pcie->fnpll_fsm_ctrl = PCIE_PLL_FSM_CLOSE;
	}
	hi_pcie_pr_i("fnpll_fsm_ctrl:%d ", pcie->fnpll_fsm_ctrl);

	pcie_core_parse_aspm_config(pdev, pcie);

	pcie->enable_async_suspend = device_property_read_bool(&pdev->dev, "enable_async_suspend");
	return 0;
}

static void pcie_core_parse_time_config(struct device_node *node, struct hi_pcie_port *port)
{
	int ret;
	u32 size = 2;
	ret = of_property_read_u32_array(node, "t_ref2perst", port->t_ref2perst, size);
	if (ret != 0) {
		port->t_ref2perst[0] = PCIE_REF2PERST1_DELAY_MIN;
		port->t_ref2perst[1] = PCIE_REF2PERST2_DELAY_MAX;
		hi_pcie_pr_i("get t_ref2perst failed, set default");
	}

	ret = of_property_read_u32_array(node, "t_perst2access", port->t_perst2access, size);
	if (ret != 0) {
		port->t_perst2access[0] = PCIE_PERST2ACCESS1_DELAY_MIN;
		port->t_perst2access[1] = PCIE_PERST2ACCESS1_DELAY_MIN;
		hi_pcie_pr_i("get t_perst2access failed, set default");
	}
}

int pcie_core_port_parse_config(struct hi_pcie_port *port)
{
	int ret;
	char node_name[32] = {0};
	struct device_node *node;

	(void)snprintf_s(node_name, sizeof(node_name), sizeof(node_name) - 1,
		"hi_pcie,port%u%u", port->pcie->ctrl_id, port->id);

	node = of_find_compatible_node(port->pcie->dev->of_node, NULL, node_name);
	if (node == NULL) {
		hi_pcie_pr_e("get port(%s) node failed", node_name);
		return -EINVAL;
	}
	ret = of_property_read_u32(node, "port_id", &port->port_id);
	if (ret != 0) {
		hi_pcie_pr_e("get port_id config failed");
		return ret;
	}
	ret = of_property_read_u32(node, "num-lanes", &port->num_lanes);
	if (ret != 0) {
		hi_pcie_pr_e("get num-lanes config failed");
		return ret;
	}
	ret = of_property_read_u16(node, "limit_speed", &port->limit_speed);
	if (ret != 0) {
		hi_pcie_pr_e("get speed config failed, set default speed[0x0]");
		port->limit_speed = PCIE_LINK_SPEED_LIMIT;
	}

	ret = of_property_read_u32(node, "pf", &port->pf);
	if (ret != 0) {
		hi_pcie_pr_e("pf default 0");
		port->pf = 0;
	}

	ret = of_property_read_u32(node, "mps", &port->mps);
	if (ret != 0) {
		hi_pcie_pr_i("mps default 0, need not adapt ret %d", ret);
		port->mps = 0;
	}

	hi_pcie_pr_i("get mps val %u", port->mps);

	ret = of_property_read_u32(node, "ep_device_type", &port->ep_device_type);
	if (ret != 0) {
		hi_pcie_pr_e("get ep_device_type failed, set default type");
		port->ep_device_type = PCIE_DEVICE_DEFAULT;
	}

	ret = of_property_read_u32(node, "dev_ven_id", &port->dev_ven_id);
	if (ret != 0) {
		hi_pcie_pr_e("get dev_ven_id failed, set default type");
		port->dev_ven_id = PCIE_DEFAULT_DEVICE_ID << 16 | PCIE_DEFAULT_VENDOR_ID;
	}

	port->disable = of_property_read_bool(node, "disable");
	if (port->disable == false)
		atomic_set(&(port->is_link_up), 0);

	port->disable_l0s = of_property_read_bool(node, "disable_l0s");
	if (port->disable_l0s)
		hi_pcie_pr_i("port doesn't support l0s");

	ret = of_property_read_u32(node, "loopback_partner", &port->loopback_partner);
	if (ret != 0) {
		hi_pcie_pr_e("get loopback_partner failed");
		port->loopback_partner = PCIE_ERR;
	}

	pcie_core_parse_time_config(node, port);

	if (port->pcie->mode == PCIE_MODE_RC) {
		ret = of_property_read_u32(node, "device_type", &port->device_type);
		if (ret != 0) {
			hi_pcie_pr_e("get device_type config failed");
			return ret;
		}
		ret = of_property_read_u32(node, "reset_gpio", &port->reset_gpio);
		if (ret != 0) {
			hi_pcie_pr_e("get gpio config failed");
			return ret;
		}
	} else {
		ret = of_property_read_u32(node, "functions", &port->functions);
		if (ret != 0) {
			hi_pcie_pr_e("get functions config failed");
			return ret;
		}
	}
	return 0;
}

int pcie_core_usr_resume(struct hi_pcie *pcie, int power_ops)
{
	hi_pcie_pr_d("+[%s] RC[%u]+", __func__, pcie->ctrl_id);

	atomic_set(&(pcie->pwroff_by_usr), 0);

	if (pcie_core_power_ctrl(pcie, (u32)power_ops)) {
		hi_pcie_pr_e("Failed to power on");
		goto FAIL;
	}

	if (pcie_core_setup(pcie)) {
		hi_pcie_pr_e("Failed to setup pcie ctrl");
		goto FAIL;
	}

	if (pcie_core_establish_link(pcie)) {
		hi_pcie_pr_e("Failed to link up");
		if (pcie->plat_ops->plat_off(pcie))
			hi_pcie_pr_e("Failed pwroff rc");
		goto FAIL;
	}

	if (pcie_core_restore_state(pcie)) {
		hi_pcie_pr_e("Failed to restore rc cfg");
		goto FAIL;
	}

	pcie_core_low_power(pcie, !pcie->aspm_dis);

	pcie->err_code = PCIE_ERRCODE_OK;

	return 0;
FAIL:
	atomic_set(&(pcie->pwroff_by_usr), 1);
	return -EINVAL;
}

int pcie_core_usr_suspend(struct hi_pcie *pcie, int power_ops)
{
	hi_pcie_pr_d("+[%s] RC[%d]+", __func__, pcie->ctrl_id);

	if ((atomic_read(&(pcie->pwroff_by_usr)) != 0) ||
		(atomic_read(&(pcie->is_power_on)) == 0)) {
		hi_pcie_pr_e("RC[%u] is already suspend by EP", pcie->ctrl_id);
		return -EINVAL;
	}

	if (power_ops != PCIE_POWER_OFF_NO_PME) {
		pcie_core_low_power(pcie, false);
		pcie_core_shutdown_prepare(pcie);
	}

	if (pcie_core_power_ctrl(pcie, (u32)power_ops)) {
		hi_pcie_pr_e("[%s] RC[%u] failed to pwr off", __func__, pcie->ctrl_id);
		return -EINVAL;
	}

	atomic_set(&(pcie->pwroff_by_usr), 1);
	return 0;
}

#ifdef CONFIG_PM
int pcie_core_pm_up(struct hi_pcie *pcie)
{
	mutex_lock(&(pcie->pm_lock));
	hi_pcie_pr_i("++%s++", __func__);

	if (atomic_read(&(pcie->is_enumerated)) != 1) {
		hi_pcie_pr_e("pcie ctrl id %u need not resume", pcie->ctrl_id);
		mutex_unlock(&(pcie->pm_lock));
		return 0;
	}

	if (atomic_read(&(pcie->pwroff_by_usr)) != 0) {
		hi_pcie_pr_e("RC[%u] is pwroff by EP, need not resume", pcie->ctrl_id);
		mutex_unlock(&(pcie->pm_lock));
		return 0;
	}

	if (pcie_core_power_ctrl(pcie, PCIE_POWER_RESUME)) {
		hi_pcie_pr_e("Failed to power on");
		goto FAIL;
	}

	if (pcie_core_setup(pcie)) {
		hi_pcie_pr_e("Failed to setup pcie ctrl");
		goto FAIL;
	}

	if (pcie_core_establish_link(pcie)) {
		hi_pcie_pr_e("Failed to link up");
		goto FAIL;
	}

	if (pcie->mode == PCIE_MODE_RC) {
		if (pcie_core_restore_state(pcie)) {
			hi_pcie_pr_e("Failed to restore rc cfg");
			goto FAIL;
		}

		pcie_core_low_power(pcie, !pcie->aspm_dis);
	}
	mutex_unlock(&(pcie->pm_lock));

	return 0;

FAIL:
	mutex_unlock(&(pcie->pm_lock));
	return -EINVAL;
}

int pcie_core_pm_down(struct hi_pcie *pcie)
{
	int ret = 0;
	hi_pcie_pr_i("++%s++", __func__);

	mutex_lock(&(pcie->pm_lock));
	if ((atomic_read(&(pcie->is_enumerated)) != 0) && (atomic_read(&(pcie->is_power_on)) != 0)) {
#ifdef CONFIG_HIPCIE_OCT
		if (pcie->ctrl_id == RC_ID2) {
			ret = pcie_core_save_state(pcie);
			if (ret != 0)
				hi_pcie_pr_e("[%s] RC[%u] save state failed", __func__, pcie->ctrl_id);
		}
#endif
		if (atomic_read(&(pcie->pwroff_by_usr)) == 0) {
			pcie_core_low_power(pcie, false);
			pcie_core_shutdown_prepare(pcie);
		}

		if (pcie_core_power_ctrl(pcie, PCIE_POWER_SUSPEND)) {
			hi_pcie_pr_e("[%s] RC[%u] failed", __func__, pcie->ctrl_id);
			ret = -EINVAL;
		}
	}
	mutex_unlock(&(pcie->pm_lock));
	return ret;
}

static int pcie_core_resume_noirq(struct device *dev)
{
	struct hi_pcie *pcie = dev_get_drvdata(dev);
	if (!pcie) {
		hi_pcie_pr_e("resume failed, pcie == NULL");
		return -EINVAL;
	}

	hi_pcie_pr_i("+[%s] RC[%u]+", __func__, pcie->ctrl_id);

	if (pcie_core_pm_up(pcie)) {
		hi_pcie_pr_e("PCIe[%u] resume_noirq failed", pcie->ctrl_id);
		return -EINVAL;
	}

	return 0;
}

static int pcie_core_suspend_noirq(struct device *dev)
{
	struct hi_pcie *pcie = dev_get_drvdata(dev);
	if (!pcie) {
		hi_pcie_pr_e("suspend failed, pcie == NULL");
		return -EINVAL;
	}

	hi_pcie_pr_i("+[%s] RC[%u]+", __func__, pcie->ctrl_id);

	if (pcie_core_pm_down(pcie))
		hi_pcie_pr_e("PCIe[%u] suspend_noirq failed", pcie->ctrl_id);

	return 0;
}

static int pcie_core_restore_noirq(struct device *dev)
{
	struct hi_pcie *pcie = dev_get_drvdata(dev);
	if (!pcie) {
		hi_pcie_pr_e("restore failed, pcie == NULL");
		return -EINVAL;
	}

	hi_pcie_pr_i("+[%s] RC[%u]+", __func__, pcie->ctrl_id);

	if (pcie_core_pm_up(pcie)) {
		hi_pcie_pr_e("PCIe[%u] restore_noirq failed", pcie->ctrl_id);
		return -EINVAL;
	}

	return 0;
}

static int pcie_core_freeze_noirq(struct device *dev)
{
	struct hi_pcie *pcie = dev_get_drvdata(dev);
	if (!pcie) {
		hi_pcie_pr_e("freeze failed, pcie == NULL");
		return -EINVAL;
	}

	hi_pcie_pr_i("+[%s] RC[%u]+", __func__, pcie->ctrl_id);

	if (pcie_core_pm_down(pcie))
		hi_pcie_pr_e("PCIe[%u] freeze_noirq failed", pcie->ctrl_id);

	return 0;
}

static int pcie_core_thaw_noirq(struct device *dev)
{
	struct hi_pcie *pcie = dev_get_drvdata(dev);
	if (!pcie) {
		hi_pcie_pr_e("thaw failed, pcie == NULL");
		return -EINVAL;
	}

	hi_pcie_pr_i("+[%s] RC[%u]+", __func__, pcie->ctrl_id);

	if (pcie_core_pm_up(pcie)) {
		hi_pcie_pr_e("PCIe[%u] thaw_noirq failed", pcie->ctrl_id);
		return -EINVAL;
	}

	return 0;
}
#endif

static int pcie_core_init(struct platform_device *pdev, struct hi_pcie *pcie)
{
	if (pcie->ctrl_ops->init)
		return pcie->ctrl_ops->init(pdev, pcie);
	return 0;
}

static void pcie_core_shutdown(struct platform_device *pdev)
{
	int ret = 0;
	struct hi_pcie *pcie = dev_get_drvdata(&pdev->dev);

	hi_pcie_pr_i("++%s++", __func__);

	if (!pcie) {
		hi_pcie_pr_e("shutdown failed, pcie == NULL");
		return;
	}

	ret = pcie_core_power_ctrl(pcie, PCIE_POWER_OFF);
	if (ret != 0) {
		hi_pcie_pr_e("shutdown failed, poweroff return %d", ret);
		return;
	}
}

static int pcie_core_plat_init(struct platform_device *pdev, struct hi_pcie *pcie)
{
	if (pcie->plat_ops->plat_init)
		return pcie->plat_ops->plat_init(pdev, pcie);
	return 0;
}

static int pcie_core_probe(struct platform_device *pdev)
{
	int ret = 0;
	struct hi_pcie *pcie = NULL;

	hi_pcie_pr_i("++%s++", __func__);

	pcie = pcie_core_new(pdev);
	if (!pcie)
		return -ENOMEM;

	ret = pcie_core_parse_config(pdev, pcie);
	if (ret != 0) {
		hi_pcie_pr_e("parse core config failed");
		return ret;
	}

	ret = hi_pcie_register_ops(pdev, pcie);
	if (ret != 0) {
		hi_pcie_pr_e("register ops failed");
		return ret;
	}

	ret = pcie_core_init(pdev, pcie);
	if (ret != 0) {
		hi_pcie_pr_e("ctrl init failed");
		return ret;
	}

	ret = pcie_core_plat_init(pdev, pcie);
	if (ret != 0) {
		hi_pcie_pr_e("platform init failed");
		return ret;
	}

	platform_set_drvdata(pdev, pcie);

	pcie_core_add(pcie);

	(void)hi_pcie_idle_hook_register(pdev);

	register_trace_hck_pcie(pdev);
	pcie_register_dsm_client();
	if (pcie->enable_async_suspend) {
		device_enable_async_suspend(&pdev->dev);
	}
#ifdef CONFIG_PCIE_HCD_LDK
	if (pcie->udk_local_irq) {
		pcie->auto_enumerate = false;
		ret = pcie_ldk_udk_interactive_init(pcie->ctrl);
		if (ret != 0) {
			hi_pcie_pr_e("pcie_ldk_udk_interactive_init failed, ret = %d", ret);
			return ret;
		}
	}
#endif
	if (pcie->auto_enumerate) {
		ret = pcie_core_do_startup(pcie);
		if (ret != 0)
			hi_pcie_pr_e("pcie startup failed");
	}

	return 0;
}

const struct dev_pm_ops pcie_dev_pm_ops = {
#ifdef CONFIG_PM
	.suspend_noirq	= pcie_core_suspend_noirq,
	.resume_noirq	= pcie_core_resume_noirq,
	.restore_noirq	= pcie_core_restore_noirq,
	.freeze_noirq	= pcie_core_freeze_noirq,
	.thaw_noirq	= pcie_core_thaw_noirq,
#endif
};

const struct of_device_id pcie_match_table[] = {
	{
		.compatible = "hi_pcie",
		.data 		= NULL,
	},
	{ }, /* end */
};

struct acpi_device_id acpi_pcie_match_table[] = {
	{ "PCIE0000", 0},
	{ "PCIE0001", 0},
	{ }, /* end */
};

struct platform_driver pcie_driver = {
	.probe		= pcie_core_probe,
	.shutdown	= pcie_core_shutdown,
	.driver		= {
		.name				 = "hi_pcie",
		.pm					 = &pcie_dev_pm_ops,
		.of_match_table		 = pcie_match_table,
		.suppress_bind_attrs = true
	},
};

module_platform_driver(pcie_driver);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("HiPCIe driver");
