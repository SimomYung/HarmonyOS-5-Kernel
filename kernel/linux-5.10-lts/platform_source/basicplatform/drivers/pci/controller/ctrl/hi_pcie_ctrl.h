/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: HiPCIe controller common logic.
 *
 * This software is distributed under the terms of the GNU General
 * Public License ("GPL") as published by the Free Software Foundation,
 * either version 2 of that License or (at your option) any later version.
 */

#ifndef HI_PCIE_CTRL_H
#define HI_PCIE_CTRL_H

#include <linux/pci.h>
#include "../core/hi_pcie_core.h"
#include "../core/hi_pcie_msi.h"
#include "hi_pcie_ctrl_dma.h"

#define PCIE_VERSION3     3
#define PCIE_VERSION4     4

#define PCIE_CTRL_DMA_QUEUE_MAX 16

enum {
	INT_TYPE_SPI = 0,
	INT_TYPE_ITS,
	INT_TYPE_INTX,
};

enum {
	INT_INDEX_INTA = 0,
	INT_INDEX_INTB,
	INT_INDEX_INTC,
	INT_INDEX_INTD,
	INT_INDEX_IDLE_TIMEOUT,
	INT_INDEX_IDLE_WAKE,
	INT_INDEX_PCS,
	INT_INDEX_LOCAL,
	INT_INDEX_LEVEL2,
};

enum {
	CTRL_VERISON_V300 = 300,
	CTRL_VERISON_V500 = 500,
};

struct hi_pcie_ctrl {
	struct hi_pcie *pcie;
	struct pci_host_bridge *bridge;
	u32 pcie_version;
	u32 ctrl_version;
	u32 ctrl_lane_spec;
	u32 int_type;
	void __iomem *ecam_base;
	u64 ecam_phy_base;
	struct hi_pcie_msi msi;
	u64 its_addr;
	union hi_pcie_re_com *re_com;
	dma_addr_t re_com_phy;
	u32 dma_queue_num;
	struct hi_pcie_dma_queue dma_queues[PCIE_CTRL_DMA_QUEUE_MAX];
	struct pci_epc *epc;
	u32 rx_atu_index;
	u32 tx_atu_index;
	u32 tx_preset;
	u32 recv_lock_time;
	u32 mem_shutdown;
	dma_addr_t dma_addr;
	int local_irq;
};

struct hi_pcie_ctrl_port {
	struct hi_pcie_port *port;
	struct hi_pcie_ctrl *ctrl;
};

struct hi_pcie_ctrl_rc {
	struct hi_pcie_ctrl_port port;
};

struct hi_pcie_ctrl_ep {
	struct hi_pcie_ctrl_port port;
	struct pci_epc *epc;
};

void hi_pcie_ctrl_register(struct hi_pcie *pcie);
#endif
