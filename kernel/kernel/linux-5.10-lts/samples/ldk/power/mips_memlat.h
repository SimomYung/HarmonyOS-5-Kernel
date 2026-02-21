/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LINUX_MIPS_MEM_H__
#define __LINUX_MIPS_MEM_H__

#define KHZ_PER_MHZ 1000
#define HZ_PER_MHZ 1000000

#ifdef CONFIG_LIBLINUX_MIPS_MEM_DEV_NUM
#define MIPS_MEM_DEV_NUM	CONFIG_LIBLINUX_MIPS_MEM_DEV_NUM
#else
#define MIPS_MEM_DEV_NUM	6
#endif

struct core_dev_map {
	unsigned int core_mhz;
	unsigned int *target_freq;
};

struct vote_reg {
	void __iomem *reg; /* dev frequency vote register */
	u32 mask;
	u8 bit;
};

struct dev_info {
	unsigned int nlevel;
	struct vote_reg *hw_vote;
	struct core_dev_map *freq_map;
	unsigned int map_len;
	unsigned int refill_event_id;
	unsigned int dev_id;
};

struct mipsmem_cluster_policy {
	cpumask_t cpu_mask;
	struct dev_info *ddr_dev;
	struct dev_info *l3c_dev;
};

#endif /* __LINUX_MIPS_MEM_H__ */
