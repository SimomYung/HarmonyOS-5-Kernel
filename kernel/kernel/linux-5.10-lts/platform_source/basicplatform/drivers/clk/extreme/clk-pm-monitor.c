/*
 * Copyright (c) 2016-2019 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/mutex.h>
#include <linux/notifier.h>
#include <linux/suspend.h>
#include <linux/list.h>
#include <linux/io.h>
#include <linux/of.h>
#include <linux/clk.h>
#include <linux/of_address.h>
#include <linux/platform_device.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/clk-provider.h>
#include <linux/clkdev.h>

static HLIST_HEAD(clocks_pm);
static HLIST_HEAD(clocks_cleanup_list);
static DEFINE_MUTEX(clocks_pm_lock);
static unsigned int g_cleanup_flag = 0;

struct pmclk_node {
	const char *name;
	struct hlist_node next_node;
};

static void pm_clocks_cleanup(struct clk *clk)
{
	struct pmclk_node *pmclk = NULL;

	if (g_cleanup_flag == 0)
		return;

	hlist_for_each_entry(pmclk, &clocks_cleanup_list, next_node) {
		if (strcmp(pmclk->name, __clk_get_name(clk)) == 0) {
			while (__clk_is_enabled(clk))
				clk_disable_unprepare(clk);
			pr_err("[%s][%s]:cnt = %u !\n", __func__, __clk_get_name(clk),
				__clk_get_enable_count(clk));
			break;
		}
	}
}

void pmclk_monitor_enable(void)
{
	struct pmclk_node *pmclk = NULL;
	struct clk *clk_node = NULL;

	hlist_for_each_entry(pmclk, &clocks_pm, next_node) {
		clk_node = __clk_lookup(pmclk->name);
		if (IS_ERR_OR_NULL(clk_node)) {
			pr_err("%s get failed!\n", pmclk->name);
			return;
		}
		if (__clk_get_enable_count(clk_node) > 0) {
			pr_err("[%s]:cnt = %u !\n", __clk_get_name(clk_node),
				__clk_get_enable_count(clk_node));
			pm_clocks_cleanup(clk_node);
		}
	}
}

static void pmclk_add(struct pmclk_node *clk, struct hlist_head *clocks_list)
{
	mutex_lock(&clocks_pm_lock);

	hlist_add_head(&clk->next_node, clocks_list);

	mutex_unlock(&clocks_pm_lock);
}

static int get_monitor_node(struct device_node *np, struct hlist_head *clocks_list)
{
	struct pmclk_node *pm_clk = NULL;
	int ret;
	unsigned int num, i;

	ret = of_property_read_u32(np, "pmclk-num", &num);
	if (ret) {
		pr_err("cound not find pmclk-num property!\n");
		return -EINVAL;
	}

	for (i = 0; i < num; i++) {
		pm_clk = kzalloc(sizeof(*pm_clk), GFP_KERNEL);
		if (pm_clk == NULL) {
			pr_err("[%s] fail to alloc pm_clk!\n", __func__);
			goto out;
		}

		ret = of_property_read_string_index(np, "clock-names", i, &(pm_clk->name));
		if (ret) {
			pr_err("%s:Failed to get clk-names\n", __func__);
			kfree(pm_clk);
			goto out;
		}

		pmclk_add(pm_clk, clocks_list);
		pm_clk = NULL;
	}

out:
	pr_err("pm clk monitor setup!\n");
	return 0;
}

static int pmclk_monitor_probe(struct platform_device *pdev)
{
	struct device_node *np = pdev->dev.of_node;
	struct device_node *clk_cleanup_np = NULL;
	int ret;

	ret = get_monitor_node(np, &clocks_pm);
	if (ret) {
		pr_err("pm-clk-monitor probe error!\n");
		return -EINVAL;
	}

	clk_cleanup_np = of_find_compatible_node(np, NULL, "pmclk-cleanup");
	if (clk_cleanup_np == NULL) {
		pr_info("%s:Failed to get ppll-disable\n", __func__);
		return 0;
    }

	ret = get_monitor_node(clk_cleanup_np, &clocks_cleanup_list);
	if (ret) {
		pr_err("pmclk disable probe error!\n");
		return -EINVAL;
	}

	g_cleanup_flag = 1;
	pr_info("pm clk monitor setup!\n");
	return 0;
}

static int pmclk_monitor_remove(struct platform_device *pdev)
{
	return 0;
}

static const struct of_device_id pmclk_of_match[] = {
	{ .compatible = "hisilicon,pm-clk-monitor" },
	{},
};
MODULE_DEVICE_TABLE(of, pmclk_of_match);

static struct platform_driver pmclk_monitor_driver = {
	.probe          = pmclk_monitor_probe,
	.remove         = pmclk_monitor_remove,
	.driver         = {
		.name   = "pmclk",
		.owner  = THIS_MODULE,
		.of_match_table = of_match_ptr(pmclk_of_match),
	},
};

static int __init pmclk_monitor_init(void)
{
	return platform_driver_register(&pmclk_monitor_driver);
}

fs_initcall(pmclk_monitor_init);
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("clock driver");
