/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Init of tty kconsole backend
 * Author: Huawei OS Kernel Lab
 * Create: Mon Nov 09 07:13:52 2020
 */

#include "kconsole_core.h"
#include <linux/init.h>
#include <linux/module.h>
#include "../tty_wrap.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Peitao Shi");
MODULE_DESCRIPTION("simple kconsole backend for tty");
MODULE_VERSION("0.01");

static int __init kconsole_init(void)
{
	/* To reusing common code, put the main part into kconsole core */
	int ret;
	ret = do_kconsole_init();
	if (ret < 0) {
		tty_warn("tty: kconsole exit failed, ret=%d\n", ret);
	}
	return ret;
}

module_init(kconsole_init);
