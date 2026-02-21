/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2021-2022. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */

#include "hvgr_soc.h"

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/of.h>
#include <linux/io.h>
#include <linux/delay.h>
#include <linux/platform_device.h>
#include "hvgr_version.h"
#include "hvgr_defs.h"
#include "hvgr_dm_api.h"
#include "hvgr_pm_api.h"
#include "hvgr_mem_api.h"
#include "hvgr_dm_driver_base.h"
#include "hvgr_regmap.h"
#include "hvgr_platform_api.h"

int hvgr_crg_reset(struct hvgr_device *gdev)
{
	hvgr_platform_reset(gdev);
	return 0;
}

static int hvgr_check_gpc_present(struct hvgr_device *gdev)
{
#ifdef CONFIG_OF
	u32 gpc_mask = 0;

	gdev->pm_dev.pm_pwr.gpc_present_mask = 0;

	/* This value is setted in fastboot. */
	if (of_property_read_u32(gdev->dev->of_node, "gpc-present-mask", &gpc_mask) != 0) {
		dev_warn(gdev->dev, "can't find gpc present mask in dtb\n");
		return 0;
	}

	if (gpc_mask == 0) {
		dev_err(gdev->dev, "gpu is invalid\n");
		return -ENODEV;
	}
	gdev->pm_dev.pm_pwr.gpc_present_mask = gpc_mask;
	dev_warn(gdev->dev, "[HVGR_DEBUG] gpc present mask is 0x%x.\n", gpc_mask);
#else
	gdev->pm_dev.pm_pwr.gpc_present_mask = 0;
#endif

	return 0;
}

static int hvgr_irqs_init(struct platform_device *pdev)
{
	struct hvgr_device *gdev = dev_get_drvdata(&pdev->dev);
	struct resource *r = NULL;
	struct hvgr_irq *irq = NULL;
	int i;

	for (i = 0; i < GPU_IRQ_CNT; i++) {
		irq = &gdev->dm_dev.irqs[i];
		r = platform_get_resource_byname(pdev, IORESOURCE_IRQ, hvgr_get_irq_name(i));
		if (r) {
			irq->irq = (u32)r->start;
			irq->flags = r->flags & IRQF_TRIGGER_MASK;
		} else {
			dev_err(&pdev->dev, "get hvgr irqs %s failed", hvgr_get_irq_name(i));
			return -ENXIO;
		}
	}
	return 0;
}

static int hvgr_map_reg_addr(struct platform_device *pdev)
{
	struct resource *r = NULL;
	struct hvgr_device *gdev = dev_get_drvdata(&pdev->dev);
	struct hvgr_reg_info *reg_info = NULL;
	uint32_t i;

	reg_info = &gdev->dm_dev.dev_reg;

	for (i = 0; i < HVGR_REG_BASE_SIZE; i++) {
		r = platform_get_resource(pdev, IORESOURCE_MEM, i);
		if (unlikely(r == NULL))
			return -1;
#ifdef CONFIG_HVGR_MMU_HM
		if (i == HVGR_REG_MMU_INDEX) {
			reg_info->reg_base[i] = __ioremap(r->start, HVGR_REG_BASE1_LEN, PAGE_KERNEL_RO, MODE_DEVICE);
			if (IS_ERR(reg_info->reg_base[i])) {
				dev_err(&pdev->dev, "ioremap failed for resource");
				return -1;
			}
			continue;
		}
#endif
		/* checks resource, request regions and ioremap resource */
		reg_info->reg_base[i] = devm_ioremap_resource(&pdev->dev, r);
		if (IS_ERR(reg_info->reg_base[i])) {
			dev_err(&pdev->dev, "ioremap failed for resource");
			return -1;
		}
	}

	return hvgr_platform_map_reg_addr(pdev, gdev);
}


static int hvgr_soc_probe(struct platform_device *pdev)
{
	int ret = 0;
	struct hvgr_device *gdev = NULL;

	gdev = devm_kzalloc(&pdev->dev, sizeof(*gdev), GFP_KERNEL);
	if (gdev == NULL) {
		dev_err(&pdev->dev, "Allocate hvgr device failed");
		return -ENOMEM;
	}
	gdev->dev = &pdev->dev;
	gdev->device_type = HVGR_DEVICE_TYPE_SOC;
	dev_set_drvdata(gdev->dev, gdev);

	(void)hvgr_platform_set_mode(gdev);

	ret = hvgr_check_gpc_present(gdev);
	if (ret) {
		dev_err(&pdev->dev, "check hvgr gpc present failed");
		goto failed;
	}

	ret = hvgr_irqs_init(pdev);
	if (ret) {
		dev_err(&pdev->dev, "assign hvgr irqs failed");
		goto failed;
	}

	ret = hvgr_map_reg_addr(pdev);
	if (ret) {
		dev_err(&pdev->dev, "assign hvgr irqs failed");
		goto failed;
	}

	ret = hvgr_device_probe(gdev);
	if (ret) {
		dev_err(&pdev->dev, "hvgr init failed");
		goto failed;
	}


	return 0;
failed:
	dev_set_drvdata(&pdev->dev, NULL);
	return ret;
}

static int hvgr_soc_remove(struct platform_device *pdev)
{
	struct hvgr_device *gdev = dev_get_drvdata(&pdev->dev);

	if (unlikely(gdev == NULL))
		return -EINVAL;

	(void)hvgr_device_remove(gdev);

	dev_set_drvdata(&pdev->dev, NULL);
	return 0;
}

/*
 * hvgr_pm_suspend_ops - Suspend callback from the OS.
 *
 * This is called by Linux when the device should suspend.
 *
 * @dev:  The device to suspend
 *
 * Return: A standard Linux error code
 */
static int hvgr_pm_suspend_ops(struct device *dev)
{
	struct hvgr_device *gdev = dev_get_drvdata(dev);

	if (unlikely(gdev == NULL))
		return -ENODEV;

	return hvgr_pm_suspend(gdev);
}

/*
 * hvgr_pm_resume_ops - Resume callback from the OS.
 *
 * This is called by Linux when the device should resume from suspension.
 *
 * @dev:  The device to resume
 *
 * Return: A standard Linux error code
 */
static int hvgr_pm_resume_ops(struct device *dev)
{
	struct hvgr_device *gdev = dev_get_drvdata(dev);

	if (unlikely(gdev == NULL))
		return -ENODEV;

	(void)hvgr_platform_set_mode(gdev);
	if (unlikely(hvgr_platform_smmu_set_sid(gdev) != 0))
		return -EAGAIN;

	hvgr_pm_resume(gdev);

	return 0;
}

/*
 * hvgr_pm_runtime_suspend_ops - Runtime suspend callback from the OS.
 *
 * This is called by Linux when the device should prepare for a condition in
 * which it will not be able to communicate with the CPU(s) and RAM due to
 * power management.
 *
 * @dev:  The device to suspend
 *
 * Return: A standard Linux error code
 */

static int hvgr_pm_suspend_ops_freeze(struct device *dev)
{
	dev_err(dev, "hvgr_pm_suspend_ops_freeze!");
	return hvgr_pm_suspend_ops(dev);
}

static int hvgr_pm_resume_ops_thaw(struct device *dev)
{
	dev_err(dev, "hvgr_pm_resume_ops_thaw!");
	return hvgr_pm_resume_ops(dev);
}

static int hvgr_pm_resume_ops_restore(struct device *dev)
{
	dev_err(dev, "hvgr_pm_resume_ops_restore!");
	return hvgr_pm_resume_ops(dev);
}

static const struct dev_pm_ops hvgr_pm_ops = {
	.suspend = hvgr_pm_suspend_ops,
	.resume = hvgr_pm_resume_ops,
	.freeze = hvgr_pm_suspend_ops_freeze,
	.thaw = hvgr_pm_resume_ops_thaw,
	.restore = hvgr_pm_resume_ops_restore,
};

static const struct of_device_id hvgr_dt_ids[] = {
	{ .compatible = "hisi,hvgr" },
	{ .compatible = "hisi,hvgr-tv100" },
	{ .compatible = "hisi,hvgr-tv200" },
	{}, /* sentinel */
};
MODULE_DEVICE_TABLE(of, hvgr_dt_ids);

static struct platform_driver hvgr_driver = {
	.probe = hvgr_soc_probe,
	.remove = hvgr_soc_remove,
	.driver = {
		.name = HVGR_DIRVER_NAME,
		.owner = THIS_MODULE,
		.pm = &hvgr_pm_ops,
		.of_match_table = of_match_ptr(hvgr_dt_ids),
	},
};

int __init hvgr_driver_init(void)
{
	return platform_driver_register(&hvgr_driver);
}

void __exit hvgr_driver_exit(void)
{
	platform_driver_unregister(&hvgr_driver);
}
module_init(hvgr_driver_init);
module_exit(hvgr_driver_exit);
MODULE_DESCRIPTION("driver for hvgr soc");
MODULE_LICENSE("GPL");
