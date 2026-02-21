/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 *
 * spmi_fer.c
 *
 * This software is licensed under the terms of the GNU General Public
 * either version 2 of that License or (at your option) any later version.
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/module.h>
#include <linux/io.h>
#include <linux/printk.h>
#include <linux/of.h>
#include <linux/of_irq.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/irqdomain.h>
#include <linux/irqdesc.h>
#include <linux/irqnr.h>
#include <linux/hardirq.h>
#include <linux/workqueue.h>
#include <securec.h>
#include <linux/spmi_platform.h>

#ifndef NO_IRQ
#define NO_IRQ 0
#endif

#define reg_set_bits(regval, shift, mask, setval)      \
	((u32)((regval) &                         \
	(~(((1U << (mask)) - 1U) << (shift)))) |   \
	(((setval) & ((1U << (mask)) - 1U)) \
	<< (shift)))

static void __iomem *g_spmi_base;

#define FER_PMU_MAX_NUM 2
#define FER_PMU_INTR_MSK 1
#define FER_PMU_INTR_SFT 18

#define FER_IRQ_NAME_SIZE 20
#define MASK 1
#define UNMASK 0

struct spmi_fer_dev {
	struct device *dev;
	void __iomem *base;
	int irq;
	unsigned int *irqs;
	struct irq_domain *domain;
	int irqnum;
	int fernum;
	char irq_name[FER_IRQ_NAME_SIZE];
	char chip_irq_name[FER_IRQ_NAME_SIZE];
	struct irq_chip irq_chip;
	struct workqueue_struct *irq_wq;
	struct work_struct irq_wk;
};

static int fer_irq_map(
	struct irq_domain *d, unsigned int virq, irq_hw_number_t hw)
{
	struct spmi_fer_dev *spmi_fer = d->host_data;
	int ret;

	if (!spmi_fer)
		return -ENOMEM;

	irq_set_chip_and_handler_name(
		virq, &spmi_fer->irq_chip, handle_simple_irq, "vendor");
	irq_set_chip_data(virq, spmi_fer);
	ret = irq_set_irq_type(virq, IRQ_TYPE_NONE);
	if (ret < 0)
		pr_err("irq set type fail\n");

	return 0;
}

static const struct irq_domain_ops fer_domain_ops = {
	.map = fer_irq_map,
	.xlate = irq_domain_xlate_twocell,
};

static void fer_get_chip_and_irq_name(struct spmi_fer_dev *spmi_fer)
{
	int ret;

	ret = snprintf_s(spmi_fer->irq_name, FER_IRQ_NAME_SIZE,
		FER_IRQ_NAME_SIZE - 1, "%s%u%s",
		"fer-", spmi_fer->fernum, "-irq");
	if (ret < 0)
		pr_err("snprintf error, ret = %d!\n", ret);

	ret = snprintf_s(spmi_fer->chip_irq_name, FER_IRQ_NAME_SIZE,
		FER_IRQ_NAME_SIZE - 1, "%s%u%s",
		"fer-", spmi_fer->fernum, "-chip-irq");
	if (ret < 0)
		pr_err("%s, snprintf error, ret = %d!\n", __func__, ret);
}

static int fer_irq_create_mapping(struct spmi_fer_dev *spmi_fer)
{
	int i;
	unsigned int virq;

	for (i = 0; i < spmi_fer->irqnum; i++) {
		virq = irq_create_mapping(spmi_fer->domain, i);
		if (virq == NO_IRQ) {
			pr_debug("Failed mapping hwirq\n");
			return -ENOSPC;
		}
		spmi_fer->irqs[i] = virq;
		pr_err("[%s]. spmi_fer->irqs[%d] = %u\n", __func__, i,
			spmi_fer->irqs[i]);
	}
	return 0;
}
static void fer_irq_wq_handler(struct work_struct *work)
{
	struct spmi_fer_dev *spmi_fer =
		container_of(work, struct spmi_fer_dev, irq_wk);
	int i;

	for (i = 0; i < spmi_fer->irqnum; i++)
		generic_handle_irq(spmi_fer->irqs[i]);

	enable_irq(spmi_fer->irq);
}

static irqreturn_t fer_irq_handler(int irq, void *data)
{
	struct spmi_fer_dev *spmi_fer = (struct spmi_fer_dev *)data;

	disable_irq_nosync(spmi_fer->irq);

	queue_work(spmi_fer->irq_wq, &spmi_fer->irq_wk);

	return IRQ_HANDLED;
}

static int spmi_fer_driver_probe(struct platform_device *pdev)
{
	struct spmi_fer_dev *spmi_fer = NULL;
	struct resource *iores = NULL;
	int ret;
	struct device *dev = NULL;
	struct device_node *np = NULL;

	dev = &pdev->dev;
	np = dev->of_node;

	spmi_fer = devm_kzalloc(&pdev->dev,
				sizeof(*spmi_fer), GFP_KERNEL);
	if (!spmi_fer) {
		dev_err(&pdev->dev, "can not allocate spmi_fer_controller data\n");
		return -ENOMEM;
	}
	spmi_fer->dev = &pdev->dev;

	/* NOTE: driver uses the static register mapping */
	iores = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!iores) {
		dev_err(&pdev->dev, "can not get resource!\n");
		return -EINVAL;
	}
	spmi_fer->base = ioremap(iores->start, resource_size(iores));
	if (!spmi_fer->base) {
		dev_err(&pdev->dev, "can not remap base addr!\n");
		return -EADDRNOTAVAIL;
	}
	dev_err(&pdev->dev, "spmi fer base addr= 0x%pK!\n", spmi_fer->base);

	g_spmi_base = spmi_fer->base;

	/* get fer num */
	ret = of_property_read_u32(
		np, "spmi-fer-num", &(spmi_fer->fernum));
	if (ret) {
		pr_err("no spmi-fer-num property set\n");
		goto err_dt_parse;
	}

	/* get pmic irq num */
	ret = of_property_read_u32(
		np, "fer-irq-num", &(spmi_fer->irqnum));
	if (ret) {
		pr_err("no fer-irq-num property set\n");
		goto err_dt_parse;
	}

	spmi_fer->irq = irq_of_parse_and_map(np, 0);
	if (spmi_fer->irq < 0) {
		pr_err("[%s]get fer irq failed\n", __func__);
		goto irq_err;
	}

	fer_get_chip_and_irq_name(spmi_fer);

	spmi_fer->irqs = (unsigned int *)devm_kmalloc(
			dev, spmi_fer->irqnum * sizeof(int), GFP_KERNEL);
	if (!spmi_fer->irqs) {
		dev_err(dev, "[%s] devm_kmalloc fail\n", __func__);
		goto irq_err;
	}

	/* Dynamic obtain struct irq_chip */
	spmi_fer->irq_chip.name = spmi_fer->chip_irq_name;

	spmi_fer->domain = irq_domain_add_simple(
		np, spmi_fer->irqnum, 0, &fer_domain_ops, spmi_fer);

	if (!spmi_fer->domain) {
		dev_err(dev, "%s failed irq domain add simple!\n", __func__);
		ret = -ENODEV;
		goto irq_err;
	}

	ret = fer_irq_create_mapping(spmi_fer);
	if (ret < 0) {
		dev_err(dev, "[%s] fer_irq_create_mapping fail\n", __func__);
		ret = -ENODEV;
		goto irq_err;
	}

	spmi_fer->irq_wq = create_singlethread_workqueue("fer_irq_wk");
	if (spmi_fer->irq_wq == NULL) {
		dev_err(dev, "%s, workqueue create failed!\n", __func__);
		ret = -ENODEV;
		goto irq_err;
	}
	INIT_WORK(&spmi_fer->irq_wk, fer_irq_wq_handler);

	ret = request_threaded_irq(spmi_fer->irq, fer_irq_handler, NULL,
		IRQF_TRIGGER_HIGH | IRQF_NO_SUSPEND, "fer", spmi_fer);
	if (ret < 0) {
		dev_err(dev, "could not claim pmic %d\n", ret);
		goto irq_err;
	}

	return ret;
err_dt_parse:
irq_err:
	iounmap(spmi_fer->base);
	return ret;
}

static int spmi_fer_driver_remove(struct platform_device *pdev)
{
	iounmap(g_spmi_base);
	return 0;
}

static const struct of_device_id spmi_fer_controller_match_table[] = {
	{ .compatible = "spmi-fer-controller" },
	{ }   /* end */
};

static const struct platform_device_id spmi_fer_controller_id[] = {
	{ "spmi-fer-controller", 0 },
	{ }
};
static struct platform_driver spmi_fer_controller_driver = {
	.probe = spmi_fer_driver_probe,
	.remove = spmi_fer_driver_remove,
	.driver = {
		.name = "spmi-fer",
		.owner = THIS_MODULE,
		.of_match_table = spmi_fer_controller_match_table,
	},
	.id_table = spmi_fer_controller_id,
};

static int __init spmi_fer_controller_init(void)
{
	return platform_driver_register(&spmi_fer_controller_driver);
}
postcore_initcall(spmi_fer_controller_init);

static void __exit spmi_fer_controller_exit(void)
{
	platform_driver_unregister(&spmi_fer_controller_driver);
}
module_exit(spmi_fer_controller_exit);

MODULE_LICENSE("GPL v2");
