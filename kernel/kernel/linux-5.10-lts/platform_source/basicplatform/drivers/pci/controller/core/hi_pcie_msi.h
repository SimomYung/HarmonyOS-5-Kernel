/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: PCIe controller msi.
 *
 * This software is distributed under the terms of the GNU General
 * Public License ("GPL") as published by the Free Software Foundation,
 * either version 2 of that License or (at your option) any later version.
 */

#ifndef HI_PCIE_MSI_H
#define HI_PCIE_MSI_H

#include <linux/types.h>
#include <linux/bitfield.h>
#include <linux/dma-mapping.h>
#include <linux/irq.h>
#include <linux/msi.h>

#include "../core/hi_pcie_core.h"

#define MAX_MSI_IRQS 256

struct hi_pcie_msi {
	void *priv;
	int msi_irq;
	struct irq_domain *irq_domain;
	struct irq_domain *msi_domain;
	u16 msi_msg;
	bool msi_msg_used;
	dma_addr_t msi_data;
	u32 num_vectors;
	u32 vectors_per_group;
	struct device *dev;
	raw_spinlock_t lock;
	u32 irq_mask;
	DECLARE_BITMAP(msi_irq_bitmap, MAX_MSI_IRQS);

	void (*mask_irq)(struct hi_pcie_msi *msi, u32 irq);
	void (*unmask_irq)(struct hi_pcie_msi *msi, u32 irq);
	void (*ack_irq)(struct hi_pcie_msi *msi, u32 irq);
	u32 (*get_irq_status)(struct hi_pcie_msi *msi, u32 group);
};

int hi_pcie_init_msi(struct hi_pcie_msi *msi, struct fwnode_handle *fwnode);
void hi_pcie_free_msi(struct hi_pcie_msi *msi);
irqreturn_t hi_pcie_handle_msi_irq(struct hi_pcie_msi *msi);
void hi_pcie_chained_msi_isr(struct irq_desc *desc);
#endif
