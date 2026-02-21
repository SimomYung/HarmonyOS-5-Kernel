/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: PCIe controller msi.
 *
 * This software is distributed under the terms of the GNU General
 * Public License ("GPL") as published by the Free Software Foundation,
 * either version 2 of that License or (at your option) any later version.
 */

#include "hi_pcie_msi.h"

#include <linux/types.h>
#include <linux/irqchip/chained_irq.h>
#include <linux/irqdomain.h>
#include <linux/msi.h>
#include <linux/pci-ecam.h>

#include "../hi_pcie_defines.h"
#include "hi_pcie_core.h"

static void hi_pcie_msi_ack_irq(struct irq_data *d)
{
	irq_chip_ack_parent(d);
}

static void hi_pcie_msi_mask_irq(struct irq_data *d)
{
	pci_msi_mask_irq(d);
	irq_chip_mask_parent(d);
}

static void hi_pcie_msi_unmask_irq(struct irq_data *d)
{
	pci_msi_unmask_irq(d);
	irq_chip_unmask_parent(d);
}

static struct irq_chip hi_pcie_msi_irq_chip = {
	.name		= "PCI-MSI",
	.irq_ack	= hi_pcie_msi_ack_irq,
	.irq_mask	= hi_pcie_msi_mask_irq,
	.irq_unmask	= hi_pcie_msi_unmask_irq,
};

static struct msi_domain_info hi_pcie_msi_domain_info = {
	.flags	= (MSI_FLAG_USE_DEF_DOM_OPS | MSI_FLAG_USE_DEF_CHIP_OPS |
			MSI_FLAG_PCI_MSIX | MSI_FLAG_MULTI_PCI_MSI),
	.chip	= &hi_pcie_msi_irq_chip,
};

/* MSI int handler */
irqreturn_t hi_pcie_handle_msi_irq(struct hi_pcie_msi *msi)
{
	int i;
	unsigned long val;
	u32 status, groups, pos, irq;
	irqreturn_t ret = IRQ_NONE;

	groups = msi->num_vectors / msi->vectors_per_group;

	for (i = 0; i < (int)groups; i++) {
		status = msi->get_irq_status(msi, (u32)i);
		if (status == 0)
			continue;

		ret = IRQ_HANDLED;
		val = status;
		pos = 0;
		while ((pos = find_next_bit(&val, msi->vectors_per_group,
						pos)) != msi->vectors_per_group) {
			irq = irq_find_mapping(msi->irq_domain,
						((u32)i * msi->vectors_per_group) + pos);
			generic_handle_irq(irq);
			pos++;
		}
	}

	return ret;
}

void hi_pcie_chained_msi_isr(struct irq_desc *desc)
{
	struct irq_chip *chip = irq_desc_get_chip(desc);
	struct hi_pcie_msi *msi;

	chained_irq_enter(chip, desc);

	msi = irq_desc_get_handler_data(desc);
	hi_pcie_handle_msi_irq(msi);

	chained_irq_exit(chip, desc);
}

static void hi_pcie_msi_setup_msg(struct irq_data *d, struct msi_msg *msg)
{
	struct hi_pcie_msi *msi = irq_data_get_irq_chip_data(d);
	u64 msi_target;

	msi_target = (u64)msi->msi_data;

	msg->address_lo = lower_32_bits(msi_target);
	msg->address_hi = upper_32_bits(msi_target);

	msg->data = d->hwirq;
}

static int hi_pcie_msi_set_affinity(struct irq_data *d,
					const struct cpumask *mask, bool force)
{
	return -EINVAL;
}

static void hi_pcie_msi_bottom_mask(struct irq_data *d)
{
	struct hi_pcie_msi *msi = irq_data_get_irq_chip_data(d);
	unsigned long flags;

	raw_spin_lock_irqsave(&msi->lock, flags);

	msi->mask_irq(msi, d->hwirq);

	raw_spin_unlock_irqrestore(&msi->lock, flags);
}

static void hi_pcie_msi_bottom_unmask(struct irq_data *d)
{
	struct hi_pcie_msi *msi = irq_data_get_irq_chip_data(d);
	unsigned long flags;

	raw_spin_lock_irqsave(&msi->lock, flags);

	msi->unmask_irq(msi, d->hwirq);

	raw_spin_unlock_irqrestore(&msi->lock, flags);
}

static void hi_pcie_msi_bottom_ack(struct irq_data *d)
{
	struct hi_pcie_msi *msi = irq_data_get_irq_chip_data(d);

	msi->ack_irq(msi, d->hwirq);
}

static struct irq_chip hi_pcie_msi_bottom_irq_chip = {
	.name				 = "HI-PCIE-MSI",
	.irq_ack			 = hi_pcie_msi_bottom_ack,
	.irq_compose_msi_msg		 = hi_pcie_msi_setup_msg,
	.irq_set_affinity		 = hi_pcie_msi_set_affinity,
	.irq_mask			 = hi_pcie_msi_bottom_mask,
	.irq_unmask			 = hi_pcie_msi_bottom_unmask,
};

static int hi_pcie_irq_domain_alloc(struct irq_domain *domain,
			unsigned int virq, unsigned int nr_irqs, void *args)
{
	struct hi_pcie_msi *msi = domain->host_data;
	unsigned long flags;
	u32 i;
	int bit;

	raw_spin_lock_irqsave(&msi->lock, flags);

	bit = bitmap_find_free_region(msi->msi_irq_bitmap,
				msi->num_vectors, (int)order_base_2(nr_irqs));

	raw_spin_unlock_irqrestore(&msi->lock, flags);

	if (bit < 0)
		return -ENOSPC;

	for (i = 0; i < nr_irqs; i++)
		irq_domain_set_info(domain, virq + i, bit + i,
				&hi_pcie_msi_bottom_irq_chip, msi,
				handle_edge_irq, NULL, NULL);

	return 0;
}

static void hi_pcie_irq_domain_free(struct irq_domain *domain,
				    unsigned int virq, unsigned int nr_irqs)
{
	struct irq_data *d = irq_domain_get_irq_data(domain, virq);
	struct hi_pcie_msi *msi = domain->host_data;
	unsigned long flags;

	raw_spin_lock_irqsave(&msi->lock, flags);

	if(d != NULL)
		bitmap_release_region(msi->msi_irq_bitmap, d->hwirq,
				      (int)order_base_2(nr_irqs));

	raw_spin_unlock_irqrestore(&msi->lock, flags);
}

static const struct irq_domain_ops hi_pcie_msi_domain_ops = {
	.alloc	= hi_pcie_irq_domain_alloc,
	.free	= hi_pcie_irq_domain_free,
};

int hi_pcie_init_msi(struct hi_pcie_msi *msi, struct fwnode_handle *fwnode)
{
	int ret;

	raw_spin_lock_init(&msi->lock);

	msi->irq_domain = irq_domain_create_linear(fwnode, msi->num_vectors,
							&hi_pcie_msi_domain_ops, msi);
	if (!msi->irq_domain) {
		hi_pcie_pr_e("create irq domain failed");
		return -ENOMEM;
	}

	irq_domain_update_bus_token(msi->irq_domain, DOMAIN_BUS_NEXUS);

	msi->msi_domain = pci_msi_create_irq_domain(fwnode,
				&hi_pcie_msi_domain_info, msi->irq_domain);
	if (!msi->msi_domain) {
		hi_pcie_pr_e("create msi irq domain failed");
		irq_domain_remove(msi->irq_domain);
		return -ENOMEM;
	}

	if (msi->msi_data == 0) {
		ret = dma_set_mask(msi->dev, DMA_BIT_MASK(32));
		if (ret)
			hi_pcie_pr_e("set mask failed");
		msi->msi_data = dma_map_single_attrs(msi->dev, &msi->msi_msg,
							sizeof(msi->msi_msg), DMA_FROM_DEVICE,
							DMA_ATTR_SKIP_CPU_SYNC);
		ret = dma_mapping_error(msi->dev, msi->msi_data);
		if (ret) {
			hi_pcie_pr_e("map msi msg failed");
			hi_pcie_free_msi(msi);
			return ret;
		}
		msi->msi_msg_used = true;
	}

	return 0;
}

void hi_pcie_free_msi(struct hi_pcie_msi *msi)
{
	if (msi->msi_irq)
		irq_set_chained_handler_and_data((unsigned int)msi->msi_irq, NULL, NULL);

	irq_domain_remove(msi->msi_domain);
	irq_domain_remove(msi->irq_domain);

	if (msi->msi_msg_used)
		dma_unmap_single_attrs(msi->dev, msi->msi_data, sizeof(msi->msi_msg),
						DMA_FROM_DEVICE, DMA_ATTR_SKIP_CPU_SYNC);
}
