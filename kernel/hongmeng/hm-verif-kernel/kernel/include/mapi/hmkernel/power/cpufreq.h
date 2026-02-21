/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Export header file of cpufreq to kernel modules
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jan 24 23:24:56 2023
 */

#ifndef MAPI_HMKERNEL_POWER_CPUFREQ_H
#define MAPI_HMKERNEL_POWER_CPUFREQ_H

#include <hmkernel/types.h>
#include <hmkernel/errno.h>
#include <uapi/hmkernel/power/cpufreq.h>

typedef void (*cpufreq_upd_hook_fn)(unsigned int cpu_id, unsigned int freq_idx,
				    unsigned int freq_ratio);

#ifdef CONFIG_PM_CPUFREQ

int cpufreq_register_upd_hook(unsigned int type, cpufreq_upd_hook_fn fn);

#else

static inline
int cpufreq_register_upd_hook(unsigned int type, cpufreq_upd_hook_fn fn)
{
	return E_HM_OK;
}

#endif /* CONFIG_PM_CPUFREQ */

#endif
