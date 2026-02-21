/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: HiPCIe internal api.
 *
 * This software is distributed under the terms of the GNU General
 * Public License ("GPL") as published by the Free Software Foundation,
 * either version 2 of that License or (at your option) any later version.
 */

#include "../hi_pcie_internal_api.h"

#include "../hi_pcie_defines.h"
#include "../core/hi_pcie_common.h"
#include "../core/hi_pcie_core.h"

int hi_pcie_config_l0sl1(u32 cdx, u32 pdx, u32 aspm)
{
	struct hi_pcie_port *port = pcie_core_port_get(cdx, pdx);
	if (!port) {
		hi_pcie_pr_e("config port (%u-%u) l0sl1 failed, port is null", cdx, pdx);
		return -1;
	}

	pcie_core_config_l0sl1(port, aspm);
	return 0;
}

int hi_pcie_config_l1ss(u32 cdx, u32 pdx, u32 aspm)
{
	struct hi_pcie_port *port = pcie_core_port_get(cdx, pdx);
	if (!port) {
		hi_pcie_pr_e("config port (%u-%u) l0sl1 failed, port is null", cdx, pdx);
		return -1;
	}

	return pcie_core_config_l1ss(port, aspm);
}

int hi_pcie_set_power_state(u32 cdx, u32 pdx, u32 state)
{
	int ret;
	struct hi_pcie_port *port = pcie_core_port_get(cdx, pdx);
	if (!port) {
		hi_pcie_pr_e("config port (%u-%u) l0sl1 failed, port is null", cdx, pdx);
		return -1;
	}

	ret = pci_set_power_state(port->target_dev, (pci_power_t)state);
	if (ret != 0) {
		hi_pcie_pr_e("failed to set power to state %u", state);
		return ret;
	}

	return 0;
}

int hi_pcie_setup_outbound(u32 cdx, u32 index, u32 base, u32 target, u32 size)
{
	struct hi_pcie *pcie = pcie_core_get(cdx);
	if (!pcie) {
		hi_pcie_pr_e("get pcie instance failed by %u", cdx);
		return -EINVAL;
	}

	if (pcie->transfer_ops && pcie->transfer_ops->setup_inbound)
		pcie->transfer_ops->setup_outbound(pcie, index, base, target, size);

	return 0;
}

int hi_pcie_setup_inbound(u32 cdx, u32 index, u32 base, u32 target, u32 size)
{
	struct hi_pcie *pcie = pcie_core_get(cdx);
	if (!pcie) {
		hi_pcie_pr_e("get pcie instance failed by %u", cdx);
		return -EINVAL;
	}

	if (pcie->transfer_ops && pcie->transfer_ops->setup_inbound)
		pcie->transfer_ops->setup_inbound(pcie, index, base, target, size);

	return 0;
}

int hi_pcie_dma_read(u32 cdx, u32 pdx, u32 queue, struct hi_pcie_dma_para *para)
{
	struct hi_pcie_port *port = pcie_core_port_get(cdx, pdx);
	if (!port) {
		hi_pcie_pr_e("(%u-%u) dma read failed, port is null", cdx, pdx);
		return -1;
	}

	if (port->pcie && port->pcie->transfer_ops &&
		port->pcie->transfer_ops->read_by_dma)
		return port->pcie->transfer_ops->read_by_dma(port, queue, para);

	return -1;
}

int hi_pcie_dma_write(u32 cdx, u32 pdx, u32 queue, struct hi_pcie_dma_para *para)
{
	struct hi_pcie_port *port = pcie_core_port_get(cdx, pdx);
	if (!port) {
		hi_pcie_pr_e("(%u-%u) dma write failed, port is null", cdx, pdx);
		return -1;
	}

	if (port->pcie && port->pcie->transfer_ops &&
		port->pcie->transfer_ops->write_by_dma)
		return port->pcie->transfer_ops->write_by_dma(port, queue, para);

	return -1;
}

void hi_pcie_refclk_host_vote(u32 cdx, bool vote)
{
	struct hi_pcie *pcie = pcie_core_get(cdx);
	if (!pcie) {
		hi_pcie_pr_e("get pcie instance failed by %u", cdx);
		return;
	}

	if (pcie->idle)
		hi_pcie_idle_host_vote(pcie->idle, cdx, vote);
}
