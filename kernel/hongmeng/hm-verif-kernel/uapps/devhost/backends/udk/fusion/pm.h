/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: UDK fusion pm headers
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jul 15 15:50:54 2023
 */

#ifndef __UDK_FUSION_PM_H__
#define __UDK_FUSION_PM_H__

#include <devhost/pm.h>

int udk_fusion_pm_init(const struct devhost_plat_pm_ops *plat_pm_ops);
void udk_fusion_pm_rdlock(void);
void udk_fusion_pm_unlock(void);

#endif
