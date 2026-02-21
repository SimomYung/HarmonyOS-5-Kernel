/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Sat Sep 14 15:32:26 2019
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/irqdomain.h>
#include <devhost/interrupt.h>

#include "devhost.h"

static int kvic_id = -1;
static struct irq_domain *kvic_irqdomain = NULL;

unsigned int devhost_kvic_create_mapping(unsigned int type)
{
	unsigned int virq;
	struct irq_fwspec fwspec;

	if (kvic_id == -1 || kvic_irqdomain == NULL) {
		return -EINVAL;
	}

	fwspec.fwnode = kvic_irqdomain->fwnode;
	fwspec.param_count = 1;
	fwspec.param[0] = type;

	virq = irq_create_fwspec_mapping(&fwspec);
	if (virq == 0) {
		/* need destroy kvic irq mapping */
		pr_err("liblinux create mapping for kvic type %u failed\n", type);
		return -EFAULT;
	}

	return virq;
}
EXPORT_SYMBOL(devhost_kvic_create_mapping);

int devhost_kvic_init(void)
{
	struct fwnode_handle *fwnode = NULL;
	int ret;

	kvic_id = devhost_kvic_default_ctrl();
	if (kvic_id < 0) {
		return kvic_id;
	}
	fwnode = irq_domain_alloc_fwnode((void *)(unsigned long)kvic_id);
	if (fwnode == NULL) {
		return -ENOMEM;
	}
	ret = devhost_virqchip_add(fwnode, kvic_id);
	if (ret < 0) {
		irq_domain_free_fwnode(fwnode);
		return ret;
	}
	kvic_irqdomain = irq_find_matching_fwnode(fwnode, DOMAIN_BUS_ANY);
	BUG_ON(kvic_irqdomain == NULL);

	return 0;
}
