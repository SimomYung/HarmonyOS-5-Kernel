/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2022-2022. All rights reserved.
 */

#include "hvgr_platform_api.h"

#include <linux/delay.h>

#include "hvgr_platform_ffa.h"

void hvgr_platform_reset(struct hvgr_device *gdev)
{
	if (unlikely(hvgr_platform_crg_reset(gdev) != 0))
		dev_err(gdev->dev, "hvgr platform reset fail");

}

void hvgr_platform_enable_icg_clk(struct hvgr_device *gdev)
{
	return;
}
