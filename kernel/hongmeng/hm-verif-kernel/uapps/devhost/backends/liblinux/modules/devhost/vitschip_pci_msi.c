/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Author: Huawei Godel Lab
 * Create: Fri Dec 13 21:10:37 CST 2019
 */
#include <linux/of.h>
#include <linux/irq.h>
#include <linux/msi.h>
#include <linux/pci.h>
#include <linux/device.h>
#include <linux/version.h>
#include <linux/irqdomain.h>
#include <asm-generic/msi.h>
#include <devhost/interrupt.h>

#include "devhost.h"

struct vits_chip_data {
	unsigned int irqctrl_id;
	struct irq_domain *irqdomain;
};

static struct vits_chip_data chip_data;

static void vits_mask_msi_irq(struct irq_data *data)
{
	pci_msi_mask_irq(data);
	devhost_disable_irq(data->hwirq);
}

static void vits_unmask_msi_irq(struct irq_data *data)
{
	devhost_enable_irq(data->hwirq);
	pci_msi_unmask_irq(data);
}

static void vits_compose_msi_msg(struct irq_data *d, struct msi_msg *msg)
{
	struct device_node *np = NULL;
	unsigned int size;
	const __be32 *reg;
	unsigned long its_base;
	unsigned long doorbeel;

	np = of_find_compatible_node(NULL, NULL, "arm,gic-v3-its");
	if (!np) {
		pr_err("find its node failed\n");
		return;
	}
	reg = of_get_property(np, "reg", &size);
	if (!reg || size < 2 * sizeof(*reg)) {
		of_node_put(np);
		pr_err("get reg data failed\n");
		return;
	}

	/*
	 * The reg is used to concatenate the high and low bits of msg data,
	 * and the irq occupies a maximum of 11 bits.
	 */
	its_base = be32_to_cpup(reg);
	its_base = (its_base << 32) | be32_to_cpup(reg + 1);
	#define GITS_TRANSLATER 0x10040
	doorbeel = its_base + GITS_TRANSLATER;

	msg->address_lo	= doorbeel & ((1UL << 32) - 1);
	msg->address_hi	= doorbeel >> 32;
	msg->data = d->hwirq & ((1UL << 11) - 1);
}

static int vits_set_affinity(struct irq_data *irq_data,
			     const struct cpumask *mask, bool force)
{
	return 0;
}

static struct irq_chip vits_chip = {
	.name			= "VITS",
	.irq_unmask		= vits_unmask_msi_irq,
	.irq_mask		= vits_mask_msi_irq,
	.irq_compose_msi_msg	= vits_compose_msi_msg,
	.irq_set_affinity 	= vits_set_affinity,
};

static int __vits_pci_msi_handler(int irq, void *data)
{
	int ret;

	ret = handle_domain_irq((struct irq_domain *)data, irq, NULL);
	if (ret != 0) {
		pr_err("ivrqchip: handle_domain_irq() failed, err=%d\n", ret);
	}

	return 0;
}

static void vits_pci_msi_domain_set_desc(msi_alloc_info_t *arg, struct msi_desc *desc)
{
	struct pci_dev *dev;
	u32 devid;
	struct irq_domain *domain;
	struct msi_domain_info *info;
	struct vits_chip_data *data;
	int i;

	arg->desc = desc;
	dev = msi_desc_to_pci_dev(desc);
	devid = PCI_DEVID(dev->bus->number, dev->devfn);
#if LINUX_VERSION_CODE < KERNEL_VERSION(4,4,206)
	domain = pci_msi_get_domain(dev);
#else
	domain = dev->dev.msi_domain;
#endif

	if (domain) {
		info = domain->host_data;
		data = info->chip_data;
		/* each msi has to create a irq mapping */
		for (i = 0; i < desc->nvec_used; i++) {
			u32 evtid = desc->msi_attrib.entry_nr + i;
			/* currently, ignore desc->affinity */
			u32 affinity = 0xFFFFFFFF;

			u32 hwconf[4] = {devid, evtid, 0x2, affinity};
			u32 hwirq = devhost_create_irq_mapping(data->irqctrl_id, hwconf, 4);
			if (hwirq == 0) {
				pr_info("failed to create irq_mapping of irqctrl_id=%u \n", data->irqctrl_id);
			}

			if (i == 0) {
				arg->hwirq = hwirq;
			}
		}
	} else {
		pr_err("failed to find device msi domain\n");
	}
}

static int vits_pci_msi_domain_init(struct irq_domain *domain,
				    struct msi_domain_info *domain_info,
				    unsigned int virq, irq_hw_number_t hwirq,
				    msi_alloc_info_t *arg)
{
	irq_domain_set_hwirq_and_chip(domain, virq, hwirq, domain_info->chip,
				      domain_info->chip_data);

	if ((domain_info->handler_name != NULL) &&
	    (domain_info->handler != NULL)) {
		__irq_set_handler(virq, domain_info->handler, 0,
				  domain_info->handler_name);
		if (domain_info->handler_data != NULL) {
			irq_set_handler_data(virq, domain_info->handler_data);
		}
	}

	devhost_request_threaded_irq(hwirq, __vits_pci_msi_handler,
				     false, "vitschip", domain);

	return 0;
}

static struct msi_domain_ops vits_pci_msi_domain_ops __read_mostly = {
	.set_desc	= vits_pci_msi_domain_set_desc,
	.msi_init	= vits_pci_msi_domain_init,
};

static struct msi_domain_info vits_pci_msi_domain_info __read_mostly = {
	.flags		= (MSI_FLAG_USE_DEF_DOM_OPS | MSI_FLAG_USE_DEF_CHIP_OPS |
				MSI_FLAG_MULTI_PCI_MSI | MSI_FLAG_PCI_MSIX),
	.ops		= &vits_pci_msi_domain_ops,
	.chip		= &vits_chip,
	.chip_data	= &chip_data,
	.handler	= handle_simple_irq,
	.handler_name	= "vits_simple",
};

int devhost_vitschip_add(struct fwnode_handle *handle, int irqctrl_id)
{
	struct irq_domain *msi_domain;

	chip_data.irqctrl_id = irqctrl_id;
	msi_domain = pci_msi_create_irq_domain(handle, &vits_pci_msi_domain_info, NULL);
	if (!msi_domain) {
		pr_err("unable to create PCI domain\n");
		return -ENOMEM;
	}

	chip_data.irqdomain = msi_domain;

	pr_info("pci msi domain create successfully\n");

	return 0;
}
EXPORT_SYMBOL(devhost_vitschip_add);
