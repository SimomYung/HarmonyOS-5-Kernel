/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: debugfs extension
 * Author: Huawei OS Kernel Lab
 * Create: Fri Feb 23 10:05:00 2024
 */
#include <linux/types.h>
#include <linux/init.h>
#include "debugfs_ext_if.h"

static int __init debugfs_ext_init(void)
{
	int ret;
	ret = devhost_register_debugfs_ext();

	return ret;
}
late_initcall(debugfs_ext_init);
