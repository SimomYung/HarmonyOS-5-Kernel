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
enum __cpufreq_qos_type {
	CPUFREQ_QOS_USER, /* sysfs */
	CPUFREQ_QOS_DRG,
	NR_CPUFREQ_QOS_TYPE,
};

#ifdef CONFIG_PM_CPUFREQ

int cpufreq_register_upd_hook(unsigned int type, cpufreq_upd_hook_fn fn);

void cpufreq_upd_curr_frequency(unsigned int cpu_id, unsigned int freq_idx,
				unsigned int freq_ratio);

void cpufreq_upd_limit_frequency(unsigned int type, unsigned int cpu_id,
				 unsigned int freq_idx, unsigned int freq_ratio, unsigned int qos);
void cpufreq_limit_next_freq(unsigned int cpu_id, unsigned long *next_freq);
void cpufreq_update_frequency(unsigned int cpu_id, unsigned int freq_idx,
			      unsigned int freq_ratio);
#else

static inline
int cpufreq_register_upd_hook(unsigned int type, cpufreq_upd_hook_fn fn)
{
	return E_HM_OK;
}

static inline
void cpufreq_upd_curr_frequency(unsigned int cpu_id, unsigned int freq_idx,
				unsigned int freq_ratio)
{
	UNUSED(cpu_id, freq_idx, freq_ratio);
}

static inline
void cpufreq_upd_limit_frequency(unsigned int type, unsigned int cpu_id,
				 unsigned int freq_idx, unsigned int freq_ratio, unsigned int qos)
{
	UNUSED(type, cpu_id, freq_idx, freq_ratio, qos);
}

static inline
void cpufreq_limit_next_freq(unsigned int cpu_id, unsigned long *next_freq)
{
	UNUSED(cpu_id, next_freq);
}

static inline
void cpufreq_update_frequency(unsigned int cpu_id, unsigned int freq_idx,
			      unsigned int freq_ratio)
{
	UNUSED(cpu_id, freq_idx, freq_ratio);
}
#endif /* CONFIG_PM_CPUFREQ */

#endif
