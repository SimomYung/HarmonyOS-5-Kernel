// SPDX-License-Identifier: GPL-2.0
#include <asm/io.h>
#include <linux/of.h>
#include <linux/slab.h>
#include <linux/cpu.h>
#include <linux/cpumask.h>
#include <linux/platform_device.h>
#include <trace/hooks/liblinux.h>

#include "mips_memlat.h"
#include "memlat_hmdal.h"

struct mips_mem_dev {
	unsigned long dev_freq; /* MHz */
	cpumask_t cpu_mask;
	unsigned int opp_cnt;
	struct dev_info *dev;
	struct opp_item *freq_tab;
	struct mem_freq_map *freq_map;
	unsigned int refill_event;
	unsigned int initialized;
};

static struct mips_mem_dev g_mem_devs[MIPS_MEM_DEV_NUM];
static unsigned int g_dev_index = 0;

static struct dev_info *device_info_of(struct platform_device *pdev,
				       struct mipsmem_cluster_policy *cluster_policy)
{
	struct dev_info *dev = NULL;

	if (strstr(pdev->name, "ddr") != NULL) {
		dev = cluster_policy->ddr_dev;
	} else if (strstr(pdev->name, "l3") != NULL) {
		dev = cluster_policy->l3c_dev;
	} else {
		/* do nothing */
	}

	return dev;
}

/*
 * To support multi-level core-dev freq relationship, and for easier trans to
 * tppmgr, turn the dtsi map into a one-dimensional array,
 * like following:
 *         dev_freq_0  dev_freq_1    .......      dev_freq_n
 *    idx     0         map_len                  n * map_len
 *    idx     1        map_len + 1                   ...
 *    ...
 *    idx  map_len -1  2*map_len -1   ...   (n + 1) * map_len - 1
 */
static struct mem_freq_map *mipsmem_dev_freq_map_init(struct dev_info *dev)
{
	unsigned int i;
	unsigned int map_len, level;
	struct mem_freq_map *freq_map = NULL;

	map_len = dev->map_len;
	level = dev->nlevel;
	freq_map = kmalloc_array(map_len * level, sizeof(struct mem_freq_map), GFP_KERNEL);
	if (freq_map == NULL) {
		pr_err("memory allocation for freq table failure\n");
		return NULL;
	}
	for (i = 0; i < map_len * level; i++) {
		freq_map[i].core_mhz = dev->freq_map[i % map_len].core_mhz;
		freq_map[i].dev_khz = dev->freq_map[i % map_len].target_freq[i / map_len];
	}
	return freq_map;
}

static struct opp_item *mipsmem_dev_freq_tab_init(struct dev_info *dev,
						  unsigned int *freq_tab_len)
{
	unsigned int i, j;
	unsigned int temp, map_len, level, freq_tab_num;
	struct opp_item *freq_tab = NULL;

	map_len = dev->map_len;
	level = dev->nlevel;
	freq_tab_num = map_len * level;
	freq_tab = kmalloc_array(freq_tab_num, sizeof(struct opp_item), GFP_KERNEL);
	if (freq_tab == NULL) {
		pr_err("memory allocation for freq table failure\n");
		return NULL;
	}
	for (i = 0; i < freq_tab_num; i++)
		freq_tab[i].opp_khz = dev->freq_map[i % map_len].target_freq[i / map_len];

	/* bubble sort */
	for (i = 0; i < freq_tab_num - 1; i++) {
		for (j = 0; j < freq_tab_num - i - 1; j++) {
			if (freq_tab[j].opp_khz > freq_tab[j + 1].opp_khz) {
				temp = freq_tab[j].opp_khz;
				freq_tab[j].opp_khz = freq_tab[j + 1].opp_khz;
				freq_tab[j + 1].opp_khz = temp;
			}
		}
	}

	/* remove the duplication */
	for (i = 0; i < freq_tab_num - 1; i++) {
		if (freq_tab[i].opp_khz == freq_tab[i + 1].opp_khz) {
			for (j = i + 1; j < freq_tab_num - 1; j++)
				freq_tab[j] = freq_tab[j + 1];
			i--;
			freq_tab_num--;
		}
	}
	*freq_tab_len = freq_tab_num;

	return freq_tab;
}

static void mipsmem_dev_info_init(struct dev_info *dev,
				  struct mipsmem_cluster_policy *cluster_policy)
{
	unsigned int opp_num = 0;
	struct opp_item *freq_tab = NULL;
	struct mem_freq_map *freq_map = NULL;
	unsigned int dev_id = dev->dev_id;

	freq_tab = mipsmem_dev_freq_tab_init(dev, &opp_num);
	if (freq_tab == NULL) {
		pr_err("memory allocation for freq table failure\n");
		return;
	}
	freq_map = mipsmem_dev_freq_map_init(dev);
	if (freq_tab == NULL) {
		pr_err("memory allocation for freq table failure\n");
		kfree(freq_tab);
		return;
	}
	g_mem_devs[dev_id].dev_freq = freq_tab[0].opp_khz / KHZ_PER_MHZ;
	g_mem_devs[dev_id].opp_cnt = opp_num;
	g_mem_devs[dev_id].freq_tab = freq_tab;
	g_mem_devs[dev_id].freq_map = freq_map;
	g_mem_devs[dev_id].cpu_mask = cluster_policy->cpu_mask;
	g_mem_devs[dev_id].dev = dev;
	g_mem_devs[dev_id].refill_event = dev->refill_event_id;
	g_mem_devs[dev_id].initialized = 1;
}

/* platform_source/cee/drivers/mips_mem.c */
static void hook_mips_mem_info(void *args, struct platform_device *pdev,
			       struct mipsmem_cluster_policy *cluster_policy)
{
	struct dev_info *dev = NULL;

	dev = device_info_of(pdev, cluster_policy);
	if (dev == NULL) {
		pr_err("invalid platform device: %s\n", pdev->name);
		return;
	}
	mipsmem_dev_info_init(dev, cluster_policy);
	g_dev_index++;
}
INIT_VENDOR_HOOK(ldk_vh_mips_mem_info, hook_mips_mem_info);

static struct mips_mem_dev *mem_device_of(unsigned int id)
{
	if (id >= MIPS_MEM_DEV_NUM) {
		pr_err("invalid memlat device id: %u\n", id);
		return NULL;
	}
	if (g_mem_devs[id].initialized == 0) {
		return NULL;
	}

	return &g_mem_devs[id];
}

int memlat_hmdal_get_nlevel(unsigned int id)
{
	struct mips_mem_dev *mem_device = NULL;

	mem_device = mem_device_of(id);
	if (mem_device == NULL) {
		pr_err("invalid memlet device id %u\n", id);
		return -EINVAL;
	}
	return mem_device->dev->nlevel;
}
EXPORT_SYMBOL(memlat_hmdal_get_nlevel);

int memlat_hmdal_get_refill_event(unsigned int id)
{
	struct mips_mem_dev *mem_device = NULL;

	mem_device = mem_device_of(id);
	if (mem_device == NULL) {
		pr_err("invalid memlet device id %u\n", id);
		return -EINVAL;
	}
	return mem_device->refill_event;
}
EXPORT_SYMBOL(memlat_hmdal_get_refill_event);

int memlat_hmdal_get_cpuset(unsigned int id, unsigned long *bits)
{
	struct mips_mem_dev *mem_device = NULL;

	if (bits == NULL) {
		pr_err("get memlat device cpuset failed\n");
		return -EINVAL;
	}
	mem_device = mem_device_of(id);
	if (mem_device == NULL) {
		pr_err("invalid memlet device id %u\n", id);
		return -EINVAL;
	}
	*bits = mem_device->cpu_mask.bits[0];

	return 0;
}
EXPORT_SYMBOL(memlat_hmdal_get_cpuset);

int memlat_hmdal_get_opp_table(unsigned int id,
			       const struct opp_item **freq_table)
{
	unsigned int opp_cnt = 0;
	struct mips_mem_dev *mem_device = NULL;

	mem_device = mem_device_of(id);
	if (mem_device == NULL) {
		pr_err("invalid memlet device id %u\n", id);
		return -EINVAL;
	}
	opp_cnt = mem_device->opp_cnt;
	if (freq_table != NULL) {
		*freq_table = mem_device->freq_tab;
	}

	return opp_cnt;
}
EXPORT_SYMBOL(memlat_hmdal_get_opp_table);

int memlat_hmdal_get_freq_map(unsigned int id,
			      const struct mem_freq_map **freq_map)
{
	unsigned int map_len;
	struct mem_freq_map *map = NULL;
	struct mips_mem_dev *mem_device = NULL;

	mem_device = mem_device_of(id);
	if (mem_device == NULL) {
		pr_err("invalid memlet device id %u\n", id);
		return -EINVAL;
	}
	map = (struct mem_freq_map *)(mem_device->freq_map);
	map_len = mem_device->dev->map_len * mem_device->dev->nlevel;
	if (map == NULL) {
		pr_err("freq map is not initialised\n");
		return -EINVAL;
	}
	if (freq_map != NULL) {
		*freq_map = map;
	}

	return map_len;
}
EXPORT_SYMBOL(memlat_hmdal_get_freq_map);

int memlat_hmdal_get_freq(unsigned int id, unsigned long *freq)
{
	struct mips_mem_dev *mem_device = NULL;

	if (freq == NULL) {
		pr_err("get memlat device freq failed\n");
		return -EINVAL;
	}
	mem_device = mem_device_of(id);
	if (mem_device == NULL) {
		pr_err("invalid memlet device id %u\n", id);
		return -EINVAL;
	}
	*freq = mem_device->dev_freq * HZ_PER_MHZ;

	return 0;
}
EXPORT_SYMBOL(memlat_hmdal_get_freq);

int memlat_hmdal_set_freq(unsigned int id, unsigned long freq)
{
	u32 value;
	unsigned long dev_freq;
	struct mips_mem_dev *mem_device = NULL;

	mem_device = mem_device_of(id);
	if (mem_device == NULL) {
		pr_err("invalid memlet device id %u\n", id);
		return -EINVAL;
	}
	dev_freq = freq / HZ_PER_MHZ;
	value = (dev_freq << mem_device->dev->hw_vote->bit) | mem_device->dev->hw_vote->mask;
	writel(value, mem_device->dev->hw_vote->reg);
	mem_device->dev_freq = dev_freq;

	return 0;
}
EXPORT_SYMBOL(memlat_hmdal_set_freq);
