/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Add pm cpu down func.
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jun 29 10:43:28 2021
 */
#ifndef MAPI_HMKERNEL_PM_CPU_H
#define MAPI_HMKERNEL_PM_CPU_H

#include <hmkernel/errno.h>
#include <lib/dlist.h>
#include <uapi/hmkernel/drivers/pm_cpu.h>

#ifdef CONFIG_CPU_HOTPLUG
int pm_cpu_down(unsigned int target_cpu, unsigned int flag);
#else
static inline int pm_cpu_down(unsigned int target_cpu, unsigned int flag)
{
	return E_HM_NOSYS;
}
#endif

#endif
