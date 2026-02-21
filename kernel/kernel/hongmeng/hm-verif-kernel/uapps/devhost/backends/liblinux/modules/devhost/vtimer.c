/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan 11 10:45:12 2019
 */
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/clockchips.h>
#include <linux/clocksource.h>
#include <linux/irq.h>

#include <lnxbase/lnxbase.h>

#include "devhost.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Weilong Chen");
MODULE_AUTHOR("Jiuyue Ma");
MODULE_VERSION("0.2");

static struct clocksource clocksource = {
	.name           = "lnxbase_cs",
	.rating         = 200,
	.mask           = CLOCKSOURCE_MASK(64),
	.flags          = CLOCK_SOURCE_IS_CONTINUOUS | CLOCK_SOURCE_SUSPEND_NONSTOP,
};

const static struct lnxbase_clocksource *ext_cs = NULL;

static u64 clocksource_read_wrapper(struct clocksource *cs)
{
	(void)cs;
	return ext_cs->read();
}

int __init vtimer_init(void)
{
	int ret;

	/* try to init lnxbase clock source */
	ext_cs = lnxbase_default_clocksource();
	if (ext_cs != NULL) {
		clocksource.read = clocksource_read_wrapper;
		clocksource.mult = ext_cs->mult;
		clocksource.shift = ext_cs->shift;
		ret = __clocksource_register(&clocksource);
		if (ret < 0) {
			pr_warn("vtimer: skip register failed clocksource %s, ret=%d\n",
				clocksource.name, ret);
			return ret;
		}
	}

	/* init clock event device */
	ret = vtimer_kevt_init();
	if (ret < 0) {
		pr_err("vtimer: kdevt init failed, ret=%d\n", ret);
		return ret;
	}

	return 0;
}
