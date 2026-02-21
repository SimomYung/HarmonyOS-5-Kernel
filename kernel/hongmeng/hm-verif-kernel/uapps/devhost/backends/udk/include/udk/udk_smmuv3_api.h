/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: udk smmuv3 interfaces
 * Author: Kirin Plat Drv Department
 * Create: Wed Mar 13 15:45:12 2024
 */

#ifndef __UDK_SMMUV3_API_H__
#define __UDK_SMMUV3_API_H__

#include <udk/device.h>

int udk_smmuv3_poweron(struct udk_device *dev);
int udk_smmuv3_poweroff(struct udk_device *dev);
#endif /* __UDK_IOMMU_DMA_H__ */
