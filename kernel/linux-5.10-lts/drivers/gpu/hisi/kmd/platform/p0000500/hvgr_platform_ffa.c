/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2021-2021.
 * All rights reserved.
 *
 * Description: This file is for the gpu smc process.
 * Author:
 * Create: 2021-02-03
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */
#include "hvgr_platform_ffa.h"

#include <linux/delay.h>

#include "hvgr_platform_api.h"
#include "hvgr_ffa.h"


uint64_t hvgr_platform_protected_cfg(struct hvgr_device *gdev, u32 mode)
{
	return 0;
}

int hvgr_platform_smmu_tcu_on(struct hvgr_device *gdev)
{
	return 0;
}

int hvgr_platform_smmu_tcu_off(struct hvgr_device *gdev)
{
	return 0;
}

uint64_t hvgr_platform_smmu_set_sid(struct hvgr_device *gdev)
{
	return 0;
}

uint64_t hvgr_platform_smmu_tbu_connect(struct hvgr_device *gdev)
{
	return 0;
}

uint64_t hvgr_platform_smmu_tbu_disconnect(struct hvgr_device *gdev)
{
	return 0;
}

uint64_t hvgr_platform_smmu_bypass(struct hvgr_device *gdev, u32 partition)
{
	return 0;
}

uint64_t hvgr_platform_sec_cfg(struct hvgr_device *gdev, u32 partition)
{
	return 0;
}

uint64_t hvgr_platform_set_mode(struct hvgr_device *gdev)
{
	return 0;
}
uint64_t hvgr_platform_crg_reset(struct hvgr_device *gdev)
{
	uint64_t ret;
	uint32_t partition = 0;

	ret = hvgr_ffa_call(gdev, HVGR_FFA_CRG_RESET, partition, 0);
	if (ret != 0)
		dev_err(gdev->dev, "partition crg reset failed!");

	return ret;
}

uint64_t hvgr_platform_set_qos(struct hvgr_device *gdev, u32 partition)
{
	uint64_t ret;

	ret = hvgr_ffa_call(gdev, HVGR_FFA_SET_QOS, partition, 0);
	if (ret != 0)
		dev_err(gdev->dev, "[HVGR_ERROR] partition %u set qos failed!", partition);

	return ret;
}

uint64_t hvgr_platform_sh_runtime(struct hvgr_device *gdev, unsigned char *buf, unsigned long size)
{
	uint64_t ret;
 
	ret = hvgr_ffa_call_with_shmem(gdev, HVGR_FFA_SH_RUNTIME, buf, size);
	if (ret != 0)
		dev_err(gdev->dev, "send sh runtime fail ret %lu", ret);
 
	return ret;
}
