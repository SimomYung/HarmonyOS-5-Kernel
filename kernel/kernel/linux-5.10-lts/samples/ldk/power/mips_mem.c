// SPDX-License-Identifier: GPL-2.0
/*
 *
 * mips.c
 *
 * for hisilicon efficinecy control algorithm mips load tracking.
 *
 * Copyright (c) 2023-2024 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/topology.h>
#include <linux/cpu.h>
#include <linux/of.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/of_address.h>
#include <trace/events/power.h>
#include <linux/sched/cpufreq.h>
#include <linux/cpu_pm.h>

#ifdef CONFIG_LIBLINUX
#include <trace/hooks/liblinux.h>
#endif

#include "mips_memlat.h"

struct mipsmem_cpu_info {
	int cpu;
	bool enable;
	struct mipsmem_cluster_policy *policy;
	struct notifier_block mips_cpu_idle_nb;
};

static DEFINE_PER_CPU(struct mipsmem_cpu_info, cpu_info);

static int get_mask_from_dev_handle(struct platform_device *pdev,
				    cpumask_t *mask)
{
	struct device_node *np = pdev->dev.of_node;
	struct device_node *cpu_np = NULL;
	struct device_node *mon_np = NULL;
	int i, cpu;

	cpumask_clear(mask);

	for_each_online_cpu(cpu) {
		cpu_np = of_get_cpu_node(cpu, NULL);
		if (!cpu_np) {
			pr_err("%s: failed to get cpu%d node\n",
			       __func__, cpu);
			return -ENOENT;
		}
		/* Get mipsmem monitor descriptor node */
		for (i = 0; ; i++) {
			mon_np = of_parse_phandle(cpu_np, "mips-mem-monitors", i);
			if (!mon_np)
				break;

			/* CPUs are sharing mipsmem monitor node */
			if (np == mon_np) {
				cpumask_set_cpu(cpu, mask);
				of_node_put(mon_np);
				break;
			}
			of_node_put(mon_np);
		}

		of_node_put(cpu_np);
	}

	if (cpumask_empty(mask))
		return -ENOENT;

	return 0;
}

static unsigned int get_max_cluster_id(void)
{
	static unsigned int max_cluster_id = 0;
	int cpu;

	if (max_cluster_id != 0)
		return max_cluster_id;
	for_each_possible_cpu(cpu) {
		max_cluster_id = max(max_cluster_id,
				     topology_physical_package_id(cpu));
	}

	return max_cluster_id;
}

static struct mipsmem_cluster_policy *mipsmem_cluster_alloc_and_init(struct platform_device *pdev,
								     cpumask_t *cpus)
{
	struct mipsmem_cluster_policy *policy = NULL;
	int ret = 0;
	struct device *dev = &pdev->dev;
	struct mipsmem_cpu_info *mem_cpu = &per_cpu(cpu_info, cpumask_first(cpus));

	policy = mem_cpu->policy;
	if (policy)
		return policy;
	policy = devm_kzalloc(dev, sizeof(*policy), GFP_KERNEL);
	if (IS_ERR_OR_NULL(policy)) {
		dev_err(dev, "mips mem alloc cluster err\n");
		return NULL;
	}
	cpumask_copy(&policy->cpu_mask, cpus);

	return policy;
}

#define DEFAULT_LEVEL	1
static void init_table_level(struct device *dev, unsigned int *level)
{
	int ret;

	ret = of_property_read_u32(dev->of_node, "nr-level", level);
	if (ret != 0)
		*level = DEFAULT_LEVEL;
	dev_err(dev, "mips_mem:%s table_level = %d\n", dev->of_node->name, *level);
}

static struct core_dev_map *init_core_dev_map(struct device *dev,
					      const char *prop_name, int *map_len,
					      unsigned int col)
{
	int len, nf, i, j;
	u32 data;
	struct core_dev_map *tbl = NULL;
	int ret;

	if (of_find_property(dev->of_node, prop_name, &len) == NULL)
		return NULL;

	len /= sizeof(data);
	if ((len % col) != 0 || len == 0 || col <= 1)
		return NULL;

	nf = len / col;

	tbl = devm_kzalloc(dev, (nf + 1) * sizeof(struct core_dev_map),
			   GFP_KERNEL);
	if (tbl == NULL)
		return NULL;

	for (i = 0; i < nf; i++) {
		ret = of_property_read_u32_index(dev->of_node, prop_name, i * col,
						 &data);
		if (ret != 0)
			return NULL;

		tbl[i].core_mhz = data / KHZ_PER_MHZ;
		tbl[i].target_freq = (unsigned int *)devm_kzalloc(dev, sizeof(unsigned int) * (col - 1),
								  GFP_KERNEL);
		if (tbl[i].target_freq == NULL)
			return NULL;
		for (j = 0; j < col - 1; j++) {
			ret = of_property_read_u32_index(dev->of_node, prop_name, i * col + j + 1,
							 &data);
			if (ret != 0)
				return NULL;
			tbl[i].target_freq[j] = data * KHZ_PER_MHZ;
			pr_debug("Entry%d CPU:%u, Dev:%lu\n", i, tbl[i].core_mhz,
				 tbl[i].target_freq[j]);
		}
	}
	tbl[i].core_mhz = 0;
	*map_len = nf;

	return tbl;
}

#define VOTE_REG_NUM	2
#define MBITS_LEFT_SHIFT	16
static struct vote_reg *init_hw_vote(struct device *dev)
{
	void __iomem *reg_base = NULL;
	struct vote_reg *hw_vote = NULL;
	u32 temp[VOTE_REG_NUM] = {0};
	u32 reg_mask = 0;
	u32 reg_bit;

	reg_base = of_iomap(dev->of_node, 0);
	if (reg_base == NULL) {
		dev_err(dev, "fail to map io!\n");
		return NULL;
	}
	hw_vote = devm_kzalloc(dev, sizeof(*hw_vote), GFP_KERNEL);
	if (hw_vote == NULL) {
		dev_err(dev, "[%s] node %s doesn't have hw_vote alloc fail!\n",
			__func__, dev->of_node->name);
		goto numap_base;
	}
	if (of_property_read_u32_array(dev->of_node, "vote-reg",
				       &temp[0], VOTE_REG_NUM) != 0) {
		dev_err(dev, "[%s] node %s doesn't have vote-reg property!\n",
			__func__, dev->of_node->name);
		goto free_hw;
	}
	hw_vote->reg = reg_base + temp[0];

	if (of_property_read_u32(dev->of_node, "vote-reg-mask",
				 &reg_mask) != 0) {
		dev_err(dev, "[%s] node %s doesn't have vote-reg mask property!\n",
			__func__, dev->of_node->name);
		goto free_hw;
	}
	hw_vote->mask = reg_mask;

	if (of_property_read_u32(dev->of_node, "vote-reg-bit", &reg_bit) != 0) {
		dev_err(dev, "[%s] node %s doesn't have vote-reg bit property!\n",
			__func__, dev->of_node->name);
		goto free_hw;
	}
	hw_vote->bit = reg_bit;
	return hw_vote;
free_hw:
	devm_kfree(dev, hw_vote);
numap_base:
	iounmap(reg_base);
	return NULL;
}

static int init_refill_event_id(struct device *dev, unsigned int *event_id)
{
	int ret;

	ret = of_property_read_u32(dev->of_node, "refill-event-id", event_id);
	if (ret != 0)
		dev_err(dev, "mips_mem: %s doesn't have refill-event-id property!\n",
			dev->of_node->name);
	dev_info(dev, "mips_mem:%s eventid = %d\n", dev->of_node->name, *event_id);
	return ret;
}

static int mipsmem_dev_alloc_and_init(struct platform_device *pdev, struct mipsmem_cluster_policy *policy)
{
	struct dev_info *dev_info = NULL;
	struct device *dev = &pdev->dev;
	int ret;
	unsigned int cluster_id;

	dev_info = devm_kzalloc(dev, sizeof(*dev_info), GFP_KERNEL);
	if (dev_info == NULL) {
		dev_err(dev, "[%s] node %s doesn't have dev_info alloc fail!\n",
			__func__, dev->of_node->name);
		return -ENOMEM;
	}
	cluster_id = topology_physical_package_id(cpumask_first(&policy->cpu_mask));
	if (strstr(pdev->name, "l3")) {
		policy->l3c_dev = dev_info;
		dev_info->dev_id = cluster_id + get_max_cluster_id() + 1;
		dev_info(dev, "mips: l3 name: %s, id: %d\n", pdev->name, dev_info->dev_id);
	} else {
		policy->ddr_dev = dev_info;
		dev_info->dev_id = cluster_id;
		dev_info(dev, "mips: ddr name: %s, id: %d\n", pdev->name, dev_info->dev_id);
	}

	init_table_level(dev, &dev_info->nlevel);
	dev_info->freq_map = init_core_dev_map(dev, "core-dev-table",
					       &dev_info->map_len, dev_info->nlevel + 1);
	if (dev_info->freq_map == NULL) {
		dev_err(dev, "Couldn't find the freq map table!\n");
		ret = -EINVAL;
		goto error;
	}

	ret = init_refill_event_id(dev, &dev_info->refill_event_id);
	if (ret)
		goto error;

	dev_info->hw_vote = init_hw_vote(dev);
	if (dev_info->hw_vote == NULL) {
		dev_err(dev, "Couldn't find the hw_vote!\n");
		ret = -EINVAL;
		goto error;
	}

	return 0;
error:
	devm_kfree(dev, dev_info);
	return ret;
}

static int mips_driver_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	cpumask_t cpus;
	int ret;
	struct mipsmem_cluster_policy *policy = NULL;

	if (get_mask_from_dev_handle(pdev, &cpus) != 0) {
		dev_err(dev, "No CPU use this mon\n");
		return -ENODEV;
	}

	policy = mipsmem_cluster_alloc_and_init(pdev, &cpus);
	if (!policy) {
		dev_err(dev, "init cluster fail\n");
		return -ENOMEM;
	}

	ret = mipsmem_dev_alloc_and_init(pdev, policy);
	if (ret) {
		dev_err(dev, "init dev fail\n");
		devm_kfree(dev, policy);
	}
#ifdef CONFIG_LIBLINUX
	trace_ldk_vh_mips_mem_info(pdev, policy);
#endif
	return ret;
}

static const struct of_device_id match_table[] = {
	{ .compatible = "mips-mem" },
	{}
};

static struct platform_driver mips_mem_mon_driver = {
	.probe = mips_driver_probe,
	.driver = {
		.name = "mips-mem",
		.of_match_table = match_table,
		.owner = THIS_MODULE,
	},
};

static int __init mips_mem_init(void)
{
	return platform_driver_register(&mips_mem_mon_driver);
}
module_init(mips_mem_init);
