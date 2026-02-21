/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Device driver for schargerV800 watchdog
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/err.h>
#include <linux/io.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/of_address.h>
#include <linux/of_gpio.h>
#include <linux/delay.h>
#include <linux/regmap.h>
#include <linux/module.h>
#include <linux/platform_device.h>

#include "scharger_v800_watchdog.h"
#include "scharger_v800_regs.h"

#define scharger_err(fmt, args...) pr_err("[schargerV800]" fmt, ## args)
#define scharger_evt(fmt, args...) pr_warn("[schargerV800]" fmt, ## args)
#define scharger_inf(fmt, args...) pr_info("[schargerV800]" fmt, ## args)
#define scharger_dbg(fmt, args...) pr_debug("[schargerV800]" fmt, ## args)

#ifndef unused
#define unused(x) ((void)(x))
#endif
struct scharger_watchdog {
	struct device *dev;
	struct regmap *regmap;
};

static struct scharger_watchdog *g_wdg = NULL;

int scharger_v800_reset_watchdog_timer(void)
{
	struct scharger_watchdog *wdg = g_wdg;

	if (wdg == NULL) {
		scharger_err("%s scharger_watchdog is NULL!\n", __func__);
		return -EINVAL;
	}

	return regmap_update_bits(wdg->regmap, CHG_WDT_SOFT_RST_REG,
				CHG_WDT_SOFT_RST_MSK, 1 << CHG_WDT_SOFT_RST_SHIFT);
}

static int scharger_set_watchdog_enable(struct scharger_watchdog *wdg, int enable)
{
	int ret = 0;

	ret += regmap_update_bits(g_wdg->regmap, CHG_SC_WDT_EN_REG,
			 CHG_SC_WDT_EN_MSK, !!enable << CHG_SC_WDT_EN_SHIFT);

	return ret;
}

int scharger_v800_set_watchdog_timer(int value)
{
	u32 val;
	int ret = 0;
	struct scharger_watchdog *wdg = g_wdg;

	if (wdg == NULL) {
		scharger_err("%s scharger_watchdog is NULL!\n", __func__);
		return -EINVAL;
	}

	scharger_inf("%s val:%d\n", __func__, value);

	if (value == 0) {
		ret = scharger_set_watchdog_enable(wdg, 0);
		return ret;
	}

	if (value >= WATCHDOG_TIMER_120_S)
		val = 7; /* 7: watchdog timer 120s */
	else if (value >= WATCHDOG_TIMER_80_S)
		val = 6; /* 6: watchdog timer 80s */
	else if (value >= WATCHDOG_TIMER_40_S)
		val = 5; /* 5: watchdog timer 40s */
	else if (value >= WATCHDOG_TIMER_10_S)
		val = 4; /* 4: watchdog timer 10s */
	else if (value >= WATCHDOG_TIMER_05_S)
		val = 3; /* 3: watchdog timer 5s */
	else if (value >= WATCHDOG_TIMER_02_S)
		val = 2; /* 2: watchdog timer 2s */
	else
		val = 1; /* 1: watchdog timer 1s */

	scharger_v800_reset_watchdog_timer();

	ret += regmap_update_bits(g_wdg->regmap, CHG_WDT_CTRL_REG, CHG_WDT_CTRL_MSK, val << CHG_WDT_CTRL_SHIFT);
	ret += scharger_set_watchdog_enable(wdg, 1);

	return ret;
}

int scharger_v800_set_watchdog_timer_ms(int ms)
{
	return scharger_v800_set_watchdog_timer(ms / 1000); /* 1000: micro second to second */
}

static int scharger_watchdog_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct scharger_watchdog *wdg = NULL;

	wdg = devm_kzalloc(dev, sizeof(*wdg), GFP_KERNEL);
	if (wdg == NULL) {
		scharger_err("[%s]wdg is null.\n", __func__);
		return -ENOMEM;
	}

	wdg->dev = dev;
	wdg->regmap = dev_get_regmap(dev->parent, NULL);
	if (!wdg->regmap) {
		scharger_err("Parent regmap unavailable.\n");
		return -ENXIO;
	}

	regmap_update_bits(wdg->regmap, CHG_SYS_LOGIC_CFG_REG_4_REG,
		 CHG_WDT_FORCE_MASK_MSK, 1 << CHG_WDT_FORCE_MASK_SHIFT);

	g_wdg = wdg;

	return 0;
}

static void scharger_watchdog_shutdown(struct platform_device *pdev)
{
}

static const struct of_device_id scharger_watchdog_of_match[] = {
	{
		.compatible = "hisilicon,scharger-v800-wdg",
		.data = NULL,
	},
	{},
};

static struct platform_driver scharger_watchdog_driver = {
	.probe = scharger_watchdog_probe,
	.shutdown = scharger_watchdog_shutdown,
	.driver = {
		.name = "scharger_v800_watchdog",
		.owner = THIS_MODULE,
		.of_match_table = scharger_watchdog_of_match,
	},
};

static int __init scharger_watchdog_init(void)
{
	return platform_driver_register(&scharger_watchdog_driver);
}

static void __exit scharger_watchdog_exit(void)
{
	platform_driver_unregister(&scharger_watchdog_driver);
}

fs_initcall(scharger_watchdog_init);
module_exit(scharger_watchdog_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("scharger watchdog driver");