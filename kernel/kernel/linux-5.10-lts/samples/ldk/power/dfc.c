// SPDX-License-Identifier: GPL-2.0
#include <devhost/pm.h>
#include <liblinux/pal.h>

void pm_set_min_cpufreq(unsigned int cpuid, unsigned int target_freq)
{
	(void)devhost_pm_set_min_cpufreq(cpuid, target_freq, CONSTRAINT_DRIVER);
}
