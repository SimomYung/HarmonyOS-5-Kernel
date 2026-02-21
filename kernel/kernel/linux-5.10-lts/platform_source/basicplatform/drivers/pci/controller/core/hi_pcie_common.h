/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: PCIe controller common functions.
 *
 * This software is distributed under the terms of the GNU General
 * Public License ("GPL") as published by the Free Software Foundation,
 * either version 2 of that License or (at your option) any later version.
 */

#ifndef HI_PCIE_COMMON_H
#define HI_PCIE_COMMON_H

#include <linux/types.h>
#include <linux/pci.h>
#include "hi_pcie_dsm.h"

struct hi_pcie_ecam {
	void __iomem *base;
	u64 base_phy;
	struct resource busr;
};

int pcie_core_read(void __iomem *addr, int size, u32 *val);
int pcie_core_write(void __iomem *addr, int size, u32 val);
int pcie_core_generic_read_conf(struct pci_bus *bus,
	u32 devfn, int where, int size, u32 *val);
int pcie_core_generic_write_conf(struct pci_bus *bus,
	u32 devfn, int where, int size, u32 val);
void __iomem *pcie_core_generic_map_bus(struct pci_bus *bus,
	unsigned int devfn, int where);
#endif
