/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2024. All rights reserved.
 * Author: Huawei Godel Lab
 * Create: Thu Mar 19 19:12:27 CST 2024
 */
#include <linux/of.h>
#include <linux/irq.h>
#include <linux/msi.h>
#include <linux/pci.h>
#include <linux/device.h>
#include <linux/version.h>
#include <linux/securec.h>
#include <linux/irqdomain.h>
#include <asm-generic/msi.h>
#include <devhost/interrupt.h>

#include "devhost.h"

struct vits_chip_data {
	unsigned int irqctrl_id;
	struct irq_domain *irqdomain;
};

static struct vits_chip_data chip_data;

static void vits_mask_pmsi_irq(struct irq_data *data)
{
	devhost_disable_irq(data->hwirq);
}

static void vits_unmask_pmsi_irq(struct irq_data *data)
{
	devhost_enable_irq(data->hwirq);
}

static void vits_compose_pmsi_msg(struct irq_data *d, struct msi_msg *msg)
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
	#define GITS_REG_SIZE		2
	reg = of_get_property(np, "reg", &size);
	if (!reg || size < GITS_REG_SIZE * sizeof(*reg)) {
		of_node_put(np);
		pr_err("get reg data failed\n");
		return;
	}

	#define GITS_TRANSLATER		0x10040
	#define UINT32_BIT_SHIFT	32
	its_base = be32_to_cpup(reg);
	its_base = (its_base << UINT32_BIT_SHIFT) | be32_to_cpup(reg + 1);
	doorbeel = its_base + GITS_TRANSLATER;
	msg->address_lo	= doorbeel & ((1UL << UINT32_BIT_SHIFT) - 1);
	msg->address_hi	= doorbeel >> UINT32_BIT_SHIFT;
	msg->data = d->hwirq;
}

static int vits_set_affinity(struct irq_data *irq_data,
			     const struct cpumask *mask, bool force)
{
	int err;
	struct cpumask cpu_mask = {0};

	UNUSED(force);
	cpumask_and(&cpu_mask, mask, cpu_online_mask);
	err = devhost_irq_setaffinity(irq_data->hwirq, cpumask_bits(&cpu_mask)[0]);
#ifdef CONFIG_GENERIC_IRQ_EFFECTIVE_AFF_MASK
	irq_data_update_effective_affinity(irq_data, &cpu_mask);
#endif
	return err;
}

static struct irq_chip vits_pmsi_irq_chip = {
	.name			= "ITS-pMSI",
	.irq_unmask		= vits_unmask_pmsi_irq,
	.irq_mask		= vits_mask_pmsi_irq,
	.irq_compose_msi_msg	= vits_compose_pmsi_msg,
	.irq_set_affinity 	= vits_set_affinity,
};

static void  __of_pmsi_get_dev_id(struct irq_domain *domain, struct device *dev,
				  u32 *dev_id)
{
	int ret, index = 0;

	/* Suck the DeviceID out of the msi-parent property */
	do {
		struct of_phandle_args args;

		ret = of_parse_phandle_with_args(dev->of_node,
						 "msi-parent", "#msi-cells",
						 index, &args);
		if (args.np == irq_domain_get_of_node(domain)) {
			if (WARN_ON(args.args_count != 1)) {
				return;
			}
			*dev_id = args.args[0];
			break;
		}
		index++;
	} while (ret == 0);
}

static void vits_pmsi_set_desc(msi_alloc_info_t *arg, struct msi_desc *desc)
{
	struct device *dev;
	u32 devid;
	struct irq_domain *domain;
	struct msi_domain_info *info;
	struct vits_chip_data *data;

	arg->desc = desc;
	dev = msi_desc_to_dev(desc);
	domain = dev->msi_domain;
	__of_pmsi_get_dev_id(domain, dev, &devid);

	if (domain) {
		info = domain->host_data;
		data = info->chip_data;
		/* each msi has to create a irq mapping */
		u32 evtid = desc->platform.msi_index;
		/* currently, ignore desc->affinity */
		u32 affinity = 0xFFFFFFFF;

		u32 hwconf[4] = {devid, evtid, 0x2, affinity};
		u32 hwirq = devhost_create_irq_mapping(data->irqctrl_id, hwconf, 4);
		if (hwirq == 0) {
			pr_info("failed to create irq_mapping of irqctrl_id=%u \n", data->irqctrl_id);
		}
		arg->hwirq = hwirq;
	} else {
		pr_err("failed to find device msi domain\n");
	}
}

static int __vits_pmsi_handler(int irq, void *data)
{
	struct pt_regs regs;
	int ret;

	ret = memset_s(&regs, sizeof(regs), 0, sizeof(regs));
	BUG_ON(ret != 0);
	local_irq_disable();
	ret = handle_domain_irq((struct irq_domain *)data, irq, &regs);
	if (ret != 0) {
		pr_err("vitspmsi: handle_domain_irq() failed, err=%d\n", ret);
	}
	local_irq_enable();
	return 0;
}

static int vits_pmsi_init(struct irq_domain *domain,
			 struct msi_domain_info *domain_info,
			 unsigned int virq, irq_hw_number_t hwirq,
			 msi_alloc_info_t *arg)
{
	int ret = 0;
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

	ret = devhost_request_threaded_irq(hwirq, __vits_pmsi_handler,
					   false, "vitschip", domain);
	if (ret < 0) {
		return -EINVAL;
	}

	return 0;
}

static struct msi_domain_ops vits_pmsi_ops = {
	.set_desc	= vits_pmsi_set_desc,
	.msi_init	= vits_pmsi_init,
};

static struct msi_domain_info vits_pmsi_domain_info = {
	.flags	= (MSI_FLAG_USE_DEF_DOM_OPS | MSI_FLAG_USE_DEF_CHIP_OPS),
	.ops	= &vits_pmsi_ops,
	.chip	= &vits_pmsi_irq_chip,
	.chip_data	= &chip_data,
	.handler	= handle_simple_irq,
	.handler_name	= "vits_pmsi_simple",
};

int devhost_vitschip_pmsi_add(struct fwnode_handle *handle, int irqctrl_id)
{
	struct irq_domain *msi_domain;

	chip_data.irqctrl_id = irqctrl_id;
	msi_domain = platform_msi_create_irq_domain(handle, &vits_pmsi_domain_info, NULL);
	if (!msi_domain) {
		pr_err("unable to create pmsi domain\n");
		return -ENOMEM;
	}

	chip_data.irqdomain = msi_domain;

	pr_info("pmsi domain create successfully\n");

	return 0;
}
EXPORT_SYMBOL(devhost_vitschip_pmsi_add);
