/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022.
 * Description: Implement of csi2if common part
 *
 * This file is released under the GPLv2.
 */
#include "i2a_csi2if.h"
#include <linux/interrupt.h>
#include <linux/of_address.h>
#include <linux/of_irq.h>
#include <linux/ioport.h>
#include <linux/device.h>
#include "plat_csi2if.h"
#include "i2a_helper.h"

struct name_address {
	const char *name;
	uint32_t *address;
};

static irqreturn_t csi_irq_handler(int irq, void *dev_id)
{
	(void)irq;
	plat_csi2if_irq_handler(irq, dev_id);
	return IRQ_HANDLED;
}

int i2a_parse_csi_info(struct platform_device *pdev, struct csi_phy_info *info)
{
	if (!pdev || !info) {
		pr_err("invalid to parse csi phy info");
		return -EINVAL;
	}

	int i = 0;
	struct device_node *np;
	struct name_address nas[] = {
		{ "csi_index", &info->csi_index },
		{ "phy_id", &info->phy_id },
		{ "phy_mode", &info->phy_mode },
		{ "phy_workmode", &info->phy_workmode },
		{ "nr_lanes", &info->nr_lanes },
	};

	np = pdev->dev.of_node;
	if (!np) {
		dev_err(&pdev->dev, "devices node is null");
		return -EINVAL;
	}

	for (i = 0; i < ARRAY_SIZE(nas); ++i) {
		if (of_property_read_u32(np, nas[i].name, nas[i].address)) {
			dev_err(&pdev->dev, "%pOF don't contain %s property", np, nas[i].name);
			return -ENOENT;
		}
	}

	info->wrapper_base = devm_ioremap_named_iomem(pdev, "csi-wrapper");
	info->csi2if_base = devm_ioremap_named_iomem(pdev, "csi2if");
	if (!info->wrapper_base || !info->csi2if_base) {
		dev_err(&pdev->dev, "%pOF wrapper or csi2if address error", np);
		return -ENOMEM;
	}

	info->csi_irq_no = platform_get_irq_byname(pdev, "csi");
	if (info->csi_irq_no <= 0) {
		dev_err(&pdev->dev, "%pOF get irq failed", np);
		return -ENOENT;
	}
	dev_info(&pdev->dev, "csi irq no: %d", info->csi_irq_no);

	return 0;
}

int i2a_csi_enable(struct device *dev, struct csi_phy_info *info)
{
	if (!dev || !info)
		return -1;

	int ret = devm_request_irq(dev, info->csi_irq_no, csi_irq_handler, 0, dev_name(dev), info);
	if (ret) {
		dev_err(dev, "request csi irq failed");
		return -EFAULT;
	}

	return plat_csi2if_enable(info);
}

int i2a_csi_disable(struct device *dev, struct csi_phy_info *info)
{
	if (!dev || !info)
		return -1;

	int ret;
	devm_free_irq(dev, info->csi_irq_no, info);
	ret = plat_csi2if_disable(info);
	return ret;
}
