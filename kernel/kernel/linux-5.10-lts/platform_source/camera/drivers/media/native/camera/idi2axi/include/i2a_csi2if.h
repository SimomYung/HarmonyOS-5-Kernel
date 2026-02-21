/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022.
 * Description: csi2if interfaces
 *
 * This file is released under the GPLv2.
 */
#ifndef I2A_CSI2IF_H
#define I2A_CSI2IF_H
#include <linux/types.h>
#include <linux/platform_device.h>

struct csi_phy_info {
	uint32_t csi_index;
	uint32_t phy_id;
	uint32_t phy_mode;
	uint32_t phy_workmode;
	uint32_t nr_lanes;

	int csi_irq_no;

	void __iomem *wrapper_base;
	void __iomem *csi2if_base;
};

int i2a_parse_csi_info(struct platform_device *pdev, struct csi_phy_info *info);
int i2a_csi_enable(struct device *dev, struct csi_phy_info *info);
int i2a_csi_disable(struct device *dev, struct csi_phy_info *info);

#endif /* end of include guard: I2A_CSI2IF_H */
