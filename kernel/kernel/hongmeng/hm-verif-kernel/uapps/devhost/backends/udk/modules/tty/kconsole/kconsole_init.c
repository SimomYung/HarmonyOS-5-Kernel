/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Kconsole backend init
 * Author: Huawei OS Kernel Lab
 * Create: Thu Nov 05 10:43:37 2020
 */

#include "kconsole_core.h"
#include "../tty_wrap.h"

static int kconsole_init(void)
{
	/* To reusing common code, put the main part into kconsole core */
	int ret;
	ret = do_kconsole_init();
	if (ret < 0) {
		tty_warn("tty: kconsole init failed, ret=%d\n", ret);
	}
	return ret;
}

static int kconsole_exit(void)
{
	int ret;
	ret = do_kconsole_exit();
	if (ret < 0) {
		tty_warn("tty: kconsole exit failed, ret=%d\n", ret);
	}
	return ret;
}

udk_module_init(kconsole_init);
udk_module_exit(kconsole_exit);
