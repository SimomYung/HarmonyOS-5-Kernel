/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2022-2022. All rights reserved.
 * This file is the dvfs implementation of the PM module.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */

#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/types.h>
#include "hvgr_defs.h"

int hvgr_debugfs_init(struct hvgr_device * const gdev)
{
	return 0;
}

void hvgr_debugfs_term(struct hvgr_device * const gdev)
{
	return;
}
