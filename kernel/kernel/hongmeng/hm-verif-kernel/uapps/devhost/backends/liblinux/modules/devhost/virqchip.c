/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Sat Sep 14 16:29:59 2019
 */
#include <linux/of.h>
#include <linux/irq.h>
#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/of_fdt.h>
#include <linux/of_irq.h>
#include <linux/module.h>
#include <linux/version.h>
#include <linux/securec.h>
#include <linux/irqdomain.h>
#include <linux/mod_devicetable.h>
#include <linux/interrupt.h>

#include <devhost/interrupt.h>

#include "devhost.h"

#define DEVHOST_DOMAIN_IRQS_SIZE 4096
#define DEVHOST_IRQ_TYPE_EDGE_MASK 0x02000000
struct virt_irqchip {
	unsigned int irqctrl_id;
	struct irq_domain *irqdomain;
};

static void syscap_irq_disable(struct irq_data *data)
{
	devhost_disable_irq(data->hwirq);
}

static void syscap_irq_enable(struct irq_data *data)
{
	devhost_enable_irq(data->hwirq);
}

static void syscap_irq_eoi(struct irq_data *data)
{
}

static int syscap_set_irq_type(struct irq_data *d, unsigned int type)
{
	unsigned int irq_type;

	if (type & IRQ_TYPE_LEVEL_MASK) {
		irq_type = DEVHOST_IRQ_ATTR_LEVEL_TYPE;
	} else if (type & IRQ_TYPE_EDGE_BOTH) {
		irq_type = DEVHOST_IRQ_ATTR_EDGE_TYPE;
	} else {
		return -EINVAL;
	}

	return devhost_irq_set_type(d->hwirq, irq_type);
}

static int syscap_set_affinity(struct irq_data *d,
				const struct cpumask *mask,
				bool force)
{
	int err;
	struct cpumask cpu_mask = {0};

	cpumask_and(&cpu_mask, mask, cpu_online_mask);

	if (force) {
		pr_err("ivrqchip: the affinity cannot be set by force\n");
		return -EINVAL;
	}

	err = devhost_irq_setaffinity(d->hwirq, cpumask_bits(&cpu_mask)[0]);
	if (err < 0) {
		return err;
	}

#ifdef CONFIG_GENERIC_IRQ_EFFECTIVE_AFF_MASK
	irq_data_update_effective_affinity(d, &cpu_mask);
#endif
	return err;
}

static int syscap_get_irqchip_state(struct irq_data *d,
				    enum irqchip_irq_state which, bool *val)
{
	int ret;
	int _val;

	switch (which) {
	case IRQCHIP_STATE_PENDING:
		ret = devhost_check_irq_state(d->hwirq, DEVHOST_IRQ_PENDING, &_val);
		break;
	case IRQCHIP_STATE_ACTIVE:
		ret = devhost_check_irq_state(d->hwirq, DEVHOST_IRQ_ACTIVE, &_val);
		break;
	case IRQCHIP_STATE_MASKED:
		ret = devhost_check_irq_state(d->hwirq, DEVHOST_IRQ_MASKED, &_val);
		break;
	default:
		ret = -EINVAL;
		break;
	}

	if (ret >= 0) {
		*val = _val > 0 ? true : false;
	}

	return ret;
}

static int syscap_irq_set_wake(struct irq_data *data, unsigned int on)
{
	int can_wakeup = 0;

	if (on) {
		can_wakeup = IRQ_CAN_WAKEUP;
	}

	return devhost_set_wake_irq(data->hwirq, can_wakeup);
}

static struct irq_chip syscap_irqchip = {
	.name             = "syscap-irqctrl",
	.irq_mask         = syscap_irq_disable,
	.irq_unmask       = syscap_irq_enable,
	.irq_eoi	  = syscap_irq_eoi,
	.irq_disable      = syscap_irq_disable,
	.irq_enable       = syscap_irq_enable,
	.irq_set_type     = syscap_set_irq_type,
	.irq_set_affinity = syscap_set_affinity,
	.irq_set_wake     = syscap_irq_set_wake,
	.irq_get_irqchip_state = syscap_get_irqchip_state,
#ifdef CONFIG_ARM_GIC
	.flags            = IRQCHIP_SET_TYPE_MASKED |
			    IRQCHIP_SKIP_SET_WAKE |
			    IRQCHIP_MASK_ON_SUSPEND,
#endif
};

static int __virqchip_irq_handler(int irq, void *data)
{
	struct pt_regs regs;
	int ret;

	ret = memset_s(&regs, sizeof(regs), 0, sizeof(regs));
	BUG_ON(ret != 0);
	local_irq_disable();
	ret = handle_domain_irq((struct irq_domain *)data, irq, &regs);
	if (ret != 0) {
		pr_err("ivrqchip: handle_domain_irq() failed, err=%d\n", ret);
	}
	local_irq_enable();
	return 0;
}

static int virqchip_domain_map(struct irq_domain *d,
			       unsigned int irq, irq_hw_number_t hwirq)
{
	struct virt_irqchip *chip = d->host_data;
	int ret;
	irq_domain_set_info(d, irq, hwirq, &syscap_irqchip, chip, handle_fasteoi_irq, NULL, NULL);
	irq_set_probe(irq);

#if LINUX_VERSION_CODE < KERNEL_VERSION(4,4,206)
	ret = devhost_request_threaded_irq(hwirq, __virqchip_irq_handler,
				     (d->fwnode->type == FWNODE_IRQCHIP), /* is_kvic */
				     "virqchip", d);
#else
	ret = devhost_request_threaded_irq(hwirq, __virqchip_irq_handler,
				     (d->fwnode->ops == &irqchip_fwnode_ops), "virqchip", d);
#endif

	return ret;
}

static u32 *virqchip_param_convert(struct irq_fwspec *fwspec, u32 *fixed_intspec,
				   unsigned int *type)
{
	if (WARN_ON(fwspec->param_count < 1)) {
		return NULL;
	}

	if (is_of_node(fwspec->fwnode)) {
		if (fwspec->param_count < 3) {
			return NULL;
		}

		/* Make sure the number of arguments is 3 and
		 * convert linux gic SPI intspec to hmos gic intspec format */
		if ((fwspec->param[0] == 0) && (fwspec->param_count == 3)) {
			fixed_intspec[0] = fwspec->param[1] + 32;
			fixed_intspec[1] = 0xE0;
			if (fwspec->param[2] & IRQ_TYPE_EDGE_BOTH) {
				fixed_intspec[1] |= DEVHOST_IRQ_TYPE_EDGE_MASK;
			}
			fixed_intspec[2] = 0xFFFFFFFF;
			*type = fwspec->param[2] & IRQ_TYPE_SENSE_MASK;

			return fixed_intspec;
		} else if (fwspec->param[0] < 32) {
			/*
			 * PPI: ==> 'num < 32'
			 * linux format, param[0] = 1
			 * hmos format, param[0] = [16, 31]
			 */
			return NULL;
		} else {
			/* LDK only supports SPI interrupts.
			* Other types and hm interrupts processing through this branch. */
			*type = (fwspec->param[1] & DEVHOST_IRQ_TYPE_EDGE_MASK) ? IRQ_TYPE_EDGE_BOTH :
										  IRQ_TYPE_LEVEL_MASK;
		}
	}

	return fwspec->param;
}

static int virqchip_domain_translate(struct irq_domain *d,
				     struct irq_fwspec *fwspec,
				     unsigned long *hwirq,
				     unsigned int *type)
{
	u32 *args = NULL;
	unsigned long hwirq_dh;
	u32 fixed_intspec[3] = {0};
	unsigned int irq_type = IRQ_TYPE_NONE;
	struct virt_irqchip *chip = d->host_data;

	args = virqchip_param_convert(fwspec, fixed_intspec, &irq_type);
	if (args == NULL) {
		pr_debug("ivrqchip: translate convert[%u] returns NULL\n",
			 fwspec->param_count);
		return -EINVAL;
	}
	hwirq_dh = devhost_irq_translate(chip->irqctrl_id, args,
					 fwspec->param_count);
	if (hwirq_dh == 0) {
		return -EINVAL;
	}

	*hwirq = hwirq_dh;
	*type = irq_type;
	return 0;
}

static int virqchip_domain_alloc(struct irq_domain *domain, unsigned int virq,
				 unsigned int nr_irqs, void *arg)
{
	unsigned int i, j;
	u32 *map_args = NULL;
	irq_hw_number_t hwirq;
	u32 fixed_intspec[3] = {0};
	struct irq_fwspec *fwspec = arg;
	unsigned int irq_type = IRQ_TYPE_NONE;
	struct virt_irqchip *chip = domain->host_data;
	int ret;

	map_args = virqchip_param_convert(arg, fixed_intspec, &irq_type);
	if (map_args == NULL) {
		pr_debug("ivrqchip: alloc convert[%u] returns NULL\n",
			 fwspec->param_count);
		return -EINVAL;
	}
	hwirq = devhost_create_irq_mapping(chip->irqctrl_id,
					   map_args, fwspec->param_count);
	if (hwirq == 0) {
		return -EINVAL;
	}

	for (i = 0; i < nr_irqs; i++) {
		ret = virqchip_domain_map(domain, virq + i, hwirq + i);
		if (ret) {
			for (j = 0; j < i; j++) {
				devhost_free_irq(hwirq + j);
			}
			return -ENOMEM;
		}
	}

	return 0;
}

static void virqchip_domain_free(struct irq_domain *domain,
				 unsigned int virq,
				 unsigned int nr_irqs)
{
	struct irq_data *irq_data = NULL;
	unsigned int last_virq = virq + nr_irqs;
	unsigned int current_virq = virq;

	while (current_virq < last_virq) {
		irq_set_handler(current_virq, NULL);
		irq_data = irq_domain_get_irq_data(domain, current_virq);
		if (irq_data != NULL) {
			irq_domain_reset_irq_data(irq_data);
		}
		current_virq++;
	}
}

static struct irq_domain_ops virqchip_domain_ops __read_mostly = {
	.translate = virqchip_domain_translate,
	.alloc = virqchip_domain_alloc,
	.free = virqchip_domain_free,
};

int devhost_virqchip_add(struct fwnode_handle *fwnode, int irqctrl_id)
{
	struct virt_irqchip *virqchip = NULL;
	int ret;

	if (fwnode == NULL) {
		return -EINVAL;
	}

#if LINUX_VERSION_CODE < KERNEL_VERSION(4,4,206)
	if (!is_of_node(fwnode) && (fwnode->type != FWNODE_IRQCHIP)) {
		return -EINVAL;
	}
#endif

	virqchip = (struct virt_irqchip *)kmalloc(sizeof(*virqchip), GFP_KERNEL);
	if (virqchip == NULL) {
		ret = -ENOMEM;
		goto err_virqchip_alloc;
	}
	virqchip->irqctrl_id = (unsigned int)irqctrl_id;
	virqchip->irqdomain = irq_domain_create_linear(fwnode, DEVHOST_DOMAIN_IRQS_SIZE,
						       &virqchip_domain_ops,
						       virqchip);
	if (virqchip->irqdomain == NULL) {
		ret = -EFAULT;
		goto err_irq_domain_add;
	}

	return 0;

err_irq_domain_add:
	kfree(virqchip);
err_virqchip_alloc:
	return ret;
}
EXPORT_SYMBOL(devhost_virqchip_add);

#ifdef CONFIG_DEVHOST_VITS_PCI_MSI
static int __of_vitschip_pci_msi_init(struct device_node *node, int irqctrl_id)
{
	int ret = 0;
	struct device_node *child = NULL;

	for_each_child_of_node(node, child) {
		if (of_find_property(child, "msi-controller", NULL) != NULL) {
			ret = devhost_vitschip_add(of_node_to_fwnode(child), irqctrl_id);
			if (ret < 0) {
				pr_err("of: failed to add virt itschip \"%s\", err=%d\n",
					child->name, ret);
			}
		}
	}
	return ret;
}
#endif

static int __of_vitschip_pmsi_init(struct device_node *nod, int irqctrl_id)
{
	int ret = 0;
	struct device_node *child = NULL;

	for_each_child_of_node(nod, child) {
		if (of_find_property(child, "msi-controller", NULL) != NULL) {
			ret = devhost_vitschip_pmsi_add(of_node_to_fwnode(child), irqctrl_id);
			if (ret < 0) {
				pr_err("of: failed to add virt itschip \"%s\", err=%d\n",
					child->name, ret);
			}
		}
	}

	return ret;
}

static int __of_virqchip_init(struct device_node *nod, struct device_node *parent)
{
	int irqctrl_id = 0;
	int ret;

	ret = of_property_read_s32(nod, "irqctrl_id", &irqctrl_id);
	if (ret < 0) {
		return ret;
	}

	ret = devhost_virqchip_add(of_node_to_fwnode(nod), irqctrl_id);
	if (ret < 0) {
		pr_err("devhost: failed to add virt irqchip \"%s\", err=%d\n",
		       nod->name, ret);
		return ret;
	}

	ret = __of_vitschip_pmsi_init(nod, irqctrl_id);
	if (ret < 0) {
		pr_err("devhost: failed to init virt itschip, err=%d\n", ret);
	}

#ifdef CONFIG_DEVHOST_VITS_PCI_MSI
	ret = __of_vitschip_pci_msi_init(nod, irqctrl_id);
	if (ret < 0) {
		pr_err("devhost: failed to init pci itschip, err=%d\n", ret);
	}
#endif
	return ret;
}

#ifndef CONFIG_LIBLINUX_BOOTFDT
struct virqchip_earlyfdt_notifier {
	struct notifier_block nb;
	const struct of_device_id *tbl;
};

static
int __virqchip_earlyfdt_notifier(struct notifier_block *nb,
				 unsigned long action, void *data)
{
	struct virqchip_earlyfdt_notifier *notifer =
		container_of(nb, struct virqchip_earlyfdt_notifier, nb);
	struct device_node *nod = NULL;
	of_irq_init_cb_t init_fn;
	const struct of_device_id *match;
	int ret;

	for_each_matching_node_and_match(nod, notifer->tbl, &match) {
		init_fn = (of_irq_init_cb_t)match->data;
		ret = init_fn(nod, NULL);
		if (ret) {
			pr_err("irqchip `%s` init failed\n", nod->name);
			return notifier_from_errno(ret);
		}
	}

	return NOTIFY_OK;
}

static struct virqchip_earlyfdt_notifier vchip_notifier = {
	.nb = {
		.notifier_call = __virqchip_earlyfdt_notifier,
	},
};

static int liblinux_register_irqchip_table(const struct of_device_id *table)
{
	vchip_notifier.tbl = table;
	return of_register_earlyfdt_notifier(&vchip_notifier.nb);
}
#endif

int devhost_virqchip_init(void)
{
	static struct of_device_id virqchip_table[] = {
		{
			.compatible = "hongmeng,virqchip",
			.data = __of_virqchip_init,
		},
		{}
	};

	return liblinux_register_irqchip_table(virqchip_table);
}
