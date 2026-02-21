/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Override pm interfaces
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jul 10 15:50:54 2023
 */

#include <libstrict/strict.h>

#include <devhost/pm.h>

#include "pm.h"

int devhost_pm_init(const struct devhost_plat_pm_ops *plat_pm_ops)
{
	return udk_fusion_pm_init(plat_pm_ops);
}
