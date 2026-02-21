/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Define interface of power
 * Author: Huawei OS Kernel Lab
 * Create: Sat Mar 11 10:21:19 UTC 2023
 */

#include <lnxbase/lnxbase.h>

#include <devhost/pm.h>
#include <hongmeng/errno.h>
#include <libhmpm/wl_stat.h>

int lnxbase_wakelock_stat_enqueue(const struct wl_stat *stat, void *args)
{
	int ret;

	ret = devhost_wakelock_stat_enqueue(stat, args);

	return -hmerrno2posix(ret);
}
