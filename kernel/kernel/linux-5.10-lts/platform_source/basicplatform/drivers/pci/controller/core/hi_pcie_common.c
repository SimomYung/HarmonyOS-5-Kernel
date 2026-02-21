/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: PCIe controller common functions.
 *
 * This software is distributed under the terms of the GNU General
 * Public License ("GPL") as published by the Free Software Foundation,
 * either version 2 of that License or (at your option) any later version.
 */

#include "hi_pcie_common.h"

#include <linux/types.h>
#include <linux/pci_regs.h>

#include "../hi_pcie_defines.h"
#include "hi_pcie_core.h"
#include "../hi_pcie_internal_api.h"

#ifndef PCIE_ECAM_OFFSET
#define pcie_ecam_offset(bus, devfn, where) \
	(((bus) << 20) | ((devfn) << 12) | (where))
#else
#define pcie_ecam_offset(bus, devfn, where) \
	PCIE_ECAM_OFFSET((bus), (devfn), (where))
#endif

int pcie_core_read(void __iomem *addr, int size, u32 *val)
{
	if (!IS_ALIGNED((uintptr_t)addr, size)) {
		*val = 0;
		return -EINVAL;
	}

	if (size == PCIE_SIZE_DWORD) {
		*val = readl(addr);
	} else if (size == PCIE_SIZE_WORD) {
		*val = readw(addr);
	} else if (size == PCIE_SIZE_BYTE) {
		*val = readb(addr);
	} else {
		*val = 0;
		return -EINVAL;
	}

	return 0;
}

int pcie_core_write(void __iomem *addr, int size, u32 val)
{
	if (!IS_ALIGNED((uintptr_t)addr, size))
		return -EINVAL;

	if (size == PCIE_SIZE_DWORD)
		writel(val, addr);
	else if (size == PCIE_SIZE_WORD)
		writew((unsigned short)val, addr);
	else if (size == PCIE_SIZE_BYTE)
		writeb((unsigned char)val, addr);
	else
		return -EINVAL;

	return 0;
}

int pcie_core_generic_read_conf(struct pci_bus *bus,
	u32 devfn, int where, int size, u32 *val)
{
	struct hi_pcie *pcie = bus->sysdata;
	int ret;

	if ((pcie == NULL) || (atomic_read(&(pcie->is_power_on)) == 0))
		return -ENODEV;

	hi_pcie_refclk_host_vote(pcie->ctrl_id, true);

	ret = pci_generic_config_read(bus, devfn, where, size, val);

	hi_pcie_refclk_host_vote(pcie->ctrl_id, false);

	return ret;
}

int pcie_core_generic_write_conf(struct pci_bus *bus,
	u32 devfn, int where, int size, u32 val)
{
	struct hi_pcie *pcie = bus->sysdata;
	int ret;

	if ((pcie == NULL) || (atomic_read(&(pcie->is_power_on)) == 0))
		return -ENODEV;

	hi_pcie_refclk_host_vote(pcie->ctrl_id, true);

	ret = pci_generic_config_write(bus, devfn, where, size, val);

	hi_pcie_refclk_host_vote(pcie->ctrl_id, false);

	return ret;
}

void __iomem *pcie_core_generic_map_bus(struct pci_bus *bus,
	unsigned int devfn, int where)
{
	struct hi_pcie *pcie = bus->sysdata;
	u32 busn = bus->number;
	struct hi_pcie_ecam *ecam;

	if (!pcie)
		return NULL;

	ecam = &pcie->ecam;
	if ((busn < ecam->busr.start) || (busn > ecam->busr.end))
		return NULL;

	busn -= (u32)ecam->busr.start;
	return ecam->base + pcie_ecam_offset(busn, devfn, (u32)where);
}

void pcie_core_config_l0sl1(struct hi_pcie_port *port, u32 aspm)
{
	struct pci_dev *child = NULL;
	struct pci_dev *temp = NULL;

	if (aspm == 0) {
		list_for_each_entry_safe(child, temp, &port->self_dev->subordinate->devices, bus_list)
			pcie_capability_clear_and_set_word(child, PCI_EXP_LNKCTL,
				PCI_EXP_LNKCTL_ASPMC, (u16)aspm);
		pcie_capability_clear_and_set_word(port->self_dev, PCI_EXP_LNKCTL,
			PCI_EXP_LNKCTL_ASPMC, (u16)aspm);
	} else {
		pcie_capability_clear_and_set_word(port->self_dev, PCI_EXP_LNKCTL,
			PCI_EXP_LNKCTL_ASPMC, (u16)aspm);
		list_for_each_entry_safe(child, temp, &port->self_dev->subordinate->devices, bus_list)
			pcie_capability_clear_and_set_word(child, PCI_EXP_LNKCTL,
				PCI_EXP_LNKCTL_ASPMC, (u16)aspm);
	}
}

static int pcie_do_config_l1ss(struct pci_dev *pdev, u32 aspm, struct hi_pcie_port *port)
{
	u32 reg_val;
	int l1ss_pm, ltr_pm;

	l1ss_pm = pci_find_ext_capability(pdev, PCI_EXT_CAP_ID_L1SS);
	if (l1ss_pm == 0) {
		hi_pcie_pr_e("Failed to get PCI_EXT_L1SS_CAP_ID");
		return -1;
	}

	if (aspm != 0) {
		/* EP: Power On Value & Scale bit[7~0] */
		if (port->pcie->ep_l1ss_ctrl2) {
			pci_read_config_dword(pdev, l1ss_pm + PCI_L1SS_CTL2, &reg_val);
			reg_val &= ~0xFF;
			reg_val |= port->pcie->ep_l1ss_ctrl2;
			pci_write_config_dword(pdev, l1ss_pm + PCI_L1SS_CTL2, reg_val);
		}

		/* EP: LTR Latency */
		if ((port->target_dev == pdev) && (port->pcie->ep_ltr_latency != 0x0)) {
			ltr_pm = pci_find_ext_capability(pdev, PCI_EXT_CAP_ID_LTR);
			if (ltr_pm)
				pci_write_config_dword(pdev, ltr_pm + PCI_LTR_MAX_SNOOP_LAT,
					port->pcie->ep_ltr_latency);
		}

		pcie_capability_read_dword(pdev, PCI_EXP_DEVCTL2, &reg_val);
		reg_val |= PCI_EXP_DEVCTL2_LTR_EN;
		pcie_capability_write_dword(pdev, PCI_EXP_DEVCTL2, reg_val);

		/* Enable L1.1&L1.2 bit[3~0] */
		if (port->pcie->l1ss_ctrl1 != 0x0)
			reg_val = port->pcie->l1ss_ctrl1;
		else
			pci_read_config_dword(pdev, l1ss_pm + PCI_L1SS_CTL1, &reg_val);
		reg_val &= 0xFFFFFFF0;
		reg_val |= aspm;
		pci_write_config_dword(pdev, l1ss_pm + PCI_L1SS_CTL1, reg_val);
	} else {
		pcie_capability_read_dword(pdev, PCI_EXP_DEVCTL2, &reg_val);
		reg_val &= ~PCI_EXP_DEVCTL2_LTR_EN;
		pcie_capability_write_dword(pdev, PCI_EXP_DEVCTL2, reg_val);

		/* disble L1ss */
		pci_read_config_dword(pdev, l1ss_pm + PCI_L1SS_CTL1, &reg_val);
		reg_val &= 0xFFFFFFF0;
		pci_write_config_dword(pdev, l1ss_pm + PCI_L1SS_CTL1, reg_val);
	}

	return 0;
}

int pcie_core_config_l1ss(struct hi_pcie_port *port, u32 aspm)
{
	int ret;

	ret = pcie_do_config_l1ss(port->target_dev, 0, port);
	if (ret != 0)
		return ret;

	ret = pcie_do_config_l1ss(port->self_dev, 0, port);
	if (ret != 0)
		return ret;

	if (aspm == 0)
		return 0;

	ret = pcie_do_config_l1ss(port->self_dev, aspm, port);
	if (ret != 0)
		return ret;

	return pcie_do_config_l1ss(port->target_dev, aspm, port);
}

int pcie_core_port_mps_adapt(struct hi_pcie_port *port, u32 mps)
{
	int ret;
	u32 mps_cap;
	u32 mps_sup;

	mps_cap = pci_find_capability(port->target_dev, PCI_CAP_ID_EXP);
	pci_read_config_dword(port->target_dev, mps_cap + PCI_EXP_DEVCAP, &mps_sup);

	mps_sup &= PCI_EXP_DEVCAP_PAYLOAD;
	hi_pcie_pr_i("remote mps sup = 0x%x", mps_sup);

	if ((ffs(mps) - 8) > mps_sup || mps > PCI_DEVCTL_SUP_PAYLOAD_512B) {
		hi_pcie_pr_i("mps is invalid, (ffs(mps) - 8) = 0x%x", (ffs(mps) - 8));
		return 0;
	}

	ret = pcie_set_mps(port->self_dev, mps);
	if (ret != 0) {
		hi_pcie_pr_e("set local mps failed %d", ret);
		return ret;
	}

	ret = pcie_set_mps(port->target_dev, mps);
	if (ret != 0) {
		hi_pcie_pr_e("set remote mps failed %d, local mps set default", ret);
		pcie_set_mps(port->self_dev, PCI_DEVCTL_SUP_PAYLOAD_128B);
		return ret;
	}

	return ret;
}