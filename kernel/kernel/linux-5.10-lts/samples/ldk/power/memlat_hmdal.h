/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LINUX_MEMLAT_HMDAL_H__
#define __LINUX_MEMLAT_HMDAL_H__

#define HZ_PER_KHZ	1000

struct opp_item {
	unsigned int opp_khz;
};

struct mem_freq_map {
	unsigned int core_mhz;
	unsigned int dev_khz;
};

int memlat_hmdal_get_cpuset(unsigned int id, unsigned long *bits);
int memlat_hmdal_get_opp_table(unsigned int id,
			       const struct opp_item **freq_table);
int memlat_hmdal_get_freq_map(unsigned int id,
			      const struct mem_freq_map **freq_map);
int memlat_hmdal_get_freq(unsigned int id, unsigned long *freq);
int memlat_hmdal_set_freq(unsigned int id, unsigned long freq);

#endif	/* __LINUX_MEMLAT_HMDAL_H__ */
