// SPDX-License-Identifier: GPL-2.0
#include <linux/cpufreq.h>
#include <linux/types.h>

/* Renamed by hisi_cpufreq_get_maxfreq using wlan_oneimage_rename. */
uint32_t __override hisi_cpufreq_get_maxfreq_15(unsigned int cpu)
{
	return cpufreq_get_hw_max_freq(cpu);
}
