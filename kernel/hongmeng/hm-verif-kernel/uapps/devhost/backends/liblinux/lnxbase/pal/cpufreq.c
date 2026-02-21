/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: Get cpu frequency.
 * Author: Huawei OS Kernel Lab
 * Create: Fri May 31 16:56:22 2024
 */

#include <devhost/pm.h>
#include <devhost/log.h>
#include <liblinux/pal.h>

unsigned int liblinux_pal_cpufreq_quick_get(unsigned int cpuid)
{
	unsigned int freq = 0;
	int err;

	err = devhost_pm_cpufreq_query_cur(cpuid, &freq);
	if (err != 0) {
		dh_error("failed to query current cpufreq err: %d\n", err);
		return 0;
	}

	return freq;
}

unsigned int liblinux_pal_cpufreq_quick_get_max(unsigned int cpuid)
{
	unsigned int freq = 0;
	int err;

	err = devhost_pm_cpufreq_query_max(cpuid, &freq);
	if (err != 0) {
		dh_error("failed to query max cpufreq err: %d\n", err);
		return 0;
	}

	return freq;
}

unsigned int liblinux_pal_cpufreq_quick_get_hw_max(unsigned int cpuid)
{
	unsigned int freq = 0;
	int err;

	err = devhost_pm_cpufreq_query_hw_max(cpuid, &freq);
	if (err != 0) {
		dh_error("failed to query max hw cpufreq err: %d\n", err);
		return 0;
	}

	return freq;
}

unsigned long long liblinux_pal_get_cpu_idle_time(unsigned int cpuid, unsigned long long *wall, int io_busy)
{
	return devhost_pm_get_cpu_idle_time(cpuid, wall, io_busy);
}

int liblinux_pal_set_max_cpufreq(unsigned int cpuid, unsigned int target_freq, enum constraint_reason reason)
{
	return devhost_pm_set_max_cpufreq(cpuid, target_freq, (int)reason);
}
