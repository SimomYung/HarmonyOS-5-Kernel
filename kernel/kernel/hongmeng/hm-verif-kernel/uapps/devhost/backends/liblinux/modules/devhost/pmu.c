/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Fri Sep 09 10:45:12 2019
 */
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/clockchips.h>
#include <linux/clocksource.h>
#include <devhost/backend.h>

#include "devhost.h"

int pmu_enable(void)
{
	return devhost_pmu_enable();
}
EXPORT_SYMBOL(pmu_enable);

int pmu_disable(void)
{
	return devhost_pmu_disable();
}
EXPORT_SYMBOL(pmu_disable);
