// SPDX-License-Identifier: GPL-2.0
/*
 * soc_freq_set.c
 *
 * set soc freq directly
 *
 * Copyright (c) 2012-2024 Huawei Technologies Co., Ltd.
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

#include <linux/debugfs.h>
#include <linux/interrupt.h>
#include <linux/kernel.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/seq_file.h>
#include <platform_include/cee/linux/hw_vote.h>
#include <securec.h>
#include <trace/events/power.h>

#include "ipa_thermal.h"

#define MODULE_NAME	"ipafreq_set"
#define MAX_NAME_LEN 30
struct ipafreq_set {
	char name[MAX_NAME_LEN];
	struct hvdev *freqset_hvdev;
};

unsigned int g_freqset_dev_num;
struct ipafreq_set *g_freqset_dev;

int hv_set_ipafreq(enum freqset_table id, unsigned int freq)
{
	int ret = -1;

	if (id < IPA_NUM_MAX) {
		ret = hv_set_freq(g_freqset_dev[id].freqset_hvdev, freq);
		if (ret != 0)
			pr_err("%s set hvdev freq fail\n", g_freqset_dev[id].name);

#ifdef trace_perf
		trace_perf(clock_set_rate, g_freqset_dev[id].name, freq, 0);
#endif
	} else {
		pr_err("ipafreq set id %d is invalid\n", id);
	}

	return ret;
}

static void freqset_hvdev_exit(void)
{
	unsigned int i;

	for (i = 0; i < g_freqset_dev_num; i++) {
		if (g_freqset_dev[i].freqset_hvdev != NULL)
			hvdev_remove(g_freqset_dev[i].freqset_hvdev);
	}
}

static struct hvdev *freqset_hvdev_init(struct device *dev, struct device_node *np)
{
	int ret;
	struct hvdev *hvdev = NULL;
	const char *ch_name = NULL;
	const char *vsrc = NULL;

	ret = of_property_read_string_index(np, "freq-vote-channel", 0,
					    &ch_name);
	if (ret != 0) {
		dev_err(dev, "parse freq-vote-channel fail!\n");
		goto err_out;
	}

	ret = of_property_read_string_index(np, "freq-vote-channel", 1,
					    &vsrc);
	if (ret != 0) {
		dev_err(dev, "parse vote src fail!\n");
		goto err_out;
	}

	hvdev = hvdev_register(dev, ch_name, vsrc);
	if (IS_ERR_OR_NULL(hvdev))
		dev_err(dev, "hvdev register fail!\n");

err_out:
	return hvdev;
}

static int ipafreq_set_dev_init(struct device *dev)
{
	unsigned int dev_num;
	int index = -1;
	struct device_node *np = dev->of_node;
	struct device_node *child = NULL;
	struct ipafreq_set *freqset = NULL;
	const char *trace_name = NULL;
	int ret;

	dev_num = device_get_child_node_count(dev);
	if (dev_num == 0) {
		dev_err(dev, "no child node\n");
		return -EINVAL;
	}
	g_freqset_dev_num = dev_num;
	g_freqset_dev = devm_kzalloc(dev, sizeof(struct ipafreq_set) * dev_num,
				   GFP_KERNEL);
	if (g_freqset_dev == NULL)
		return -ENOMEM;
	for_each_child_of_node(np, child) {
		index++;
		freqset = &g_freqset_dev[index];
		ret = of_property_read_string(child, "ipafreq-name", &trace_name);
		if (ret != 0)
			continue;
		ret = snprintf_s(freqset->name, MAX_NAME_LEN,
				 MAX_NAME_LEN - 1, "ipafreq_%s",
				 trace_name);
		if (ret < 0)
			continue;
		freqset->freqset_hvdev = freqset_hvdev_init(dev, child);
		if (freqset->freqset_hvdev == NULL)
			continue;
	}
 
	return 0;
}

static int ipafreq_set_probe(struct platform_device *pdev)
{
	int ret;
	struct device *dev = &pdev->dev;

	ret = ipafreq_set_dev_init(dev);
	if (ret != 0) {
		dev_err(dev, "ipafreq_set thermal dev init fail%d\n", ret);
		return ret;
	}

	return 0;
}

static const struct of_device_id ipafreq_match[] = {
	{ .compatible = "hisilicon,ipafreq" },
	{}
};

static struct platform_driver ipafreq_driver = {
	.probe  = ipafreq_set_probe,
	.driver = {
		.name = MODULE_NAME,
		.of_match_table = of_match_ptr(ipafreq_match),
	},
};

static int __init ipafreq_set_init(void)
{
	int ret;
 
	ret = platform_driver_register(&ipafreq_driver);
	if (ret != 0)
		pr_err("fail to register ipafreq_set thermal\n");
	return ret;
}

static void __exit ipafreq_set_exit(void)
{
	freqset_hvdev_exit();
	platform_driver_unregister(&ipafreq_driver);
}

module_init(ipafreq_set_init);
module_exit(ipafreq_set_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("ipafreq set driver");
MODULE_AUTHOR("Huawei Technologies Co., Ltd.");