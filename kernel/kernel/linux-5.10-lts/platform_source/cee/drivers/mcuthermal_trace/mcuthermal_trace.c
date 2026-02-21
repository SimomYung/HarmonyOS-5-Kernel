// SPDX-License-Identifier: GPL-2.0
/*
 * mcuthermal_trace.c
 *
 * show mcuthermal freq limit in systrace
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

#define MODULE_NAME	"mcuthermal_trace"
#define MAX_NAME_LEN 30
struct mcuthermal_trace {
	char name[MAX_NAME_LEN];
	struct hvdev *thermal_hvdev;
	bool valid;
};

struct mcuthermal_trace_info {
	int irq;
	unsigned int dev_num;
};

struct mcuthermal_trace_info g_trace_info = {
	.irq = -1,
	.dev_num = 0,
};
struct mcuthermal_trace *g_trace_dev;

static irqreturn_t mcuthermal_irq_trace_handler(int irq __maybe_unused,
						void *drvdata __maybe_unused)
{
	unsigned int i;
	unsigned int freq_khz = 0;
	int ret;

	if (g_trace_info.irq < 0)
		return IRQ_NONE;

	for (i = 0; i < g_trace_info.dev_num; i++) {
		if (!g_trace_dev[i].valid)
			continue;
		ret = hv_get_freq(g_trace_dev[i].thermal_hvdev, &freq_khz);
		if (ret != 0) {
			pr_err("%s get hvdev freq fail\n", g_trace_dev[i].name);
			continue;
		}
#ifdef trace_perf
		trace_perf(clock_set_rate, g_trace_dev[i].name, freq_khz, 0);
#endif
	}

	return IRQ_HANDLED;
}

static void trace_hvdev_exit(void)
{
	unsigned int i;

	for (i = 0; i < g_trace_info.dev_num; i++) {
		if (g_trace_dev[i].thermal_hvdev != NULL)
			hvdev_remove(g_trace_dev[i].thermal_hvdev);
	}
}

static struct hvdev *trace_hvdev_init(struct device *dev, struct device_node *np)
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

static int mcuthermal_trace_dev_init(struct device *dev)
{
	unsigned int dev_num;
	int index = -1;
	struct device_node *np = dev->of_node;
	struct device_node *child = NULL;
	struct mcuthermal_trace *trace = NULL;
	const char *trace_name = NULL;
	int ret;

	dev_num = device_get_child_node_count(dev);
	if (dev_num == 0) {
		dev_err(dev, "no child node\n");
		return -EINVAL;
	}
	g_trace_info.dev_num = dev_num;
	g_trace_dev = devm_kzalloc(dev, sizeof(struct mcuthermal_trace) * dev_num,
				   GFP_KERNEL);
	if (g_trace_dev == NULL)
		return -ENOMEM;
	for_each_child_of_node(np, child) {
		index++;
		trace = &g_trace_dev[index];
		ret = of_property_read_string(child, "trace-name", &trace_name);
		if (ret != 0)
			continue;
		ret = snprintf_s(trace->name, MAX_NAME_LEN,
				 MAX_NAME_LEN - 1, "mcuthermal_%s",
				 trace_name);
		if (ret < 0)
			continue;
		trace->thermal_hvdev = trace_hvdev_init(dev, child);
		if (trace->thermal_hvdev == NULL)
			continue;
		trace->valid = true;
	}

	return 0;
}

static int mcuthermal_trace_probe(struct platform_device *pdev)
{
	int ret;
	struct device *dev = &pdev->dev;

	ret = mcuthermal_trace_dev_init(dev);
	if (ret != 0) {
		dev_err(dev, "lpmcu thermal trace dev init fail%d\n", ret);
		return ret;
	}

	g_trace_info.irq = platform_get_irq(pdev, 0);
	if (g_trace_info.irq < 0) {
		ret = -ENXIO;
		dev_err(dev, "fail to get irq!\n");
		return ret;
	}

	ret = devm_request_irq(dev, g_trace_info.irq,
			       mcuthermal_irq_trace_handler,
			       IRQF_TRIGGER_RISING, MODULE_NAME, dev);
	if (ret != 0) {
		dev_err(dev, "failed to request irq\n");
		return ret;
	}

	return 0;
}

static const struct of_device_id trace_match[] = {
	{ .compatible = "hisilicon,mcuthermal" },
	{}
};

static struct platform_driver trace_driver = {
	.probe  = mcuthermal_trace_probe,
	.driver = {
		.name = MODULE_NAME,
		.of_match_table = of_match_ptr(trace_match),
	},
};

static int __init mcuthermal_trace_init(void)
{
	int ret;

	ret = platform_driver_register(&trace_driver);
	if (ret != 0)
		pr_err("fail to register lpmcu thermal trace\n");
	return ret;
}

static void __exit mcuthermal_trace_exit(void)
{
	trace_hvdev_exit();
	platform_driver_unregister(&trace_driver);
}

module_init(mcuthermal_trace_init);
module_exit(mcuthermal_trace_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("mcuthermal trace driver");
MODULE_AUTHOR("Huawei Technologies Co., Ltd.");
