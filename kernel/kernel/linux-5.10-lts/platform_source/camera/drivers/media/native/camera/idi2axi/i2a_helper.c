/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022.
 * Description: idi2axi common helper functions
 *
 * This file is released under the GPLv2.
 */
#include "i2a_helper.h"
#include <linux/io.h>

void __iomem *devm_ioremap_named_iomem(struct platform_device *pdev, const char *name)
{
	if (!pdev)
		return NULL;
	void __iomem *ka;
	struct resource *iores;

	iores = platform_get_resource_byname(pdev, IORESOURCE_MEM, name);
	if (!iores) {
		dev_err(&pdev->dev, "%pOF failed to get %s reg", pdev->dev.of_node, name);
		return NULL;
	}

	ka = devm_ioremap(&pdev->dev, iores->start, resource_size(iores));
	if (!ka) {
		dev_err(&pdev->dev, "%pOF failed to map %s reg", pdev->dev.of_node, name);
		return NULL;
	}

	dev_dbg(&pdev->dev, "%pOF %s reg:%#x iomaped to:%pK",
		pdev->dev.of_node, name, iores->start, ka);
	return ka;
}
