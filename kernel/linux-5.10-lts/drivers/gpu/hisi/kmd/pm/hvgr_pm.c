/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2022-2022. All rights reserved.
 * This file is the dvfs implementation of the PM module.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */

#include <linux/types.h>
#include <linux/errno.h>
#include <linux/pm.h>
#include <securec.h>

#include "hvgr_log_api.h"
#include "hvgr_debugfs_api.h"
#include "hvgr_pm_api.h"
#include "hvgr_dm_api.h"
#include "hvgr_pm_defs.h"
#include "hvgr_pm_policy.h"
#include "hvgr_pm_power.h"
#include "hvgr_pm_gpu_reset.h"
#include "hvgr_pm_core_mask.h"
#include "hvgr_pm_driver.h"
#include "hvgr_pm_dvfs.h"
#include "hvgr_pm_dfx.h"
#include "hvgr_pm_api.h"
#include "hvgr_gpu_id.h"

ssize_t hvgr_pm_set_core_mask(struct hvgr_device *gdev,
	const char *buf, size_t count)
{
	return hvgr_pm_set_core_mask_inner((void *)gdev, buf, count);
}

bool hvgr_pm_retain_power(struct hvgr_device *gdev)
{
	struct hvgr_pm_pwr *pm_pwr = &gdev->pm_dev.pm_pwr;
	bool ret = false;

	mutex_lock(&pm_pwr->power_lock);
	if (pm_pwr->cur_status == HVGR_PM_GPU_READY) {
		hvgr_pm_user_ref_add(gdev);
		ret = true;
	}
	mutex_unlock(&pm_pwr->power_lock);
	return ret;
}

/* Only call this interface when hvgr_pm_retain return true. */
void hvgr_pm_release_power(struct hvgr_device *gdev)
{
	hvgr_pm_release_gpu(gdev);
}

bool hvgr_pm_request_gpu(struct hvgr_device *gdev, bool gpu_reset)
{
	hvgr_pm_user_ref_add(gdev);
	return hvgr_pm_power_on_sync(gdev, gpu_reset);
}

void hvgr_pm_release_gpu(struct hvgr_device *gdev)
{
	hvgr_pm_user_ref_dec(gdev);
#ifdef HVGR_GPU_POWER_ASYNC
	hvgr_pm_power_off_async(gdev);
#else
	hvgr_pm_power_off_sync(gdev);
#endif
}

uint32_t hvgr_pm_get_availiable_cores(const struct hvgr_device *gdev)
{
	return gdev->pm_dev.pm_pwr.core_mask_cur;
}

static int hvgr_pm_soft_init(struct hvgr_device *gdev)
{
	int ret;

	gdev->pm_dev.pm_dir = hvgr_debugfs_create_dir("pm",
		gdev->root_debugfs_dir);
	gdev->pm_dev.gdev = gdev;
	ret = hvgr_pm_power_soft_init(gdev);
	if (ret != 0) {
		hvgr_err(gdev, HVGR_PM, "pm power soft init failed");
		return ret;
	}
	ret = hvgr_pm_gpu_reset_soft_init(gdev);
	if (ret != 0) {
		hvgr_err(gdev, HVGR_PM, "pm gpu reset init failed");
		return ret;
	}
	hvgr_core_mask_init(gdev);
	(void)hvgr_pm_metrics_init(gdev);
	hvgr_pm_dfx_init(gdev);
	return 0;
}

static void hvgr_pm_soft_term(struct hvgr_device *gdev)
{
	hvgr_pm_power_soft_term(gdev);

	if (!IS_ERR_OR_NULL(gdev->pm_dev.pm_reset.reset_workq))
		destroy_workqueue(gdev->pm_dev.pm_reset.reset_workq);

	hvgr_pm_metrics_term(gdev);
	hvgr_core_mask_term(gdev);
}

int hvgr_pm_gpu_access_enable(struct hvgr_device * const gdev)
{
	if (hvgr_pm_soft_init(gdev) != 0) {
		hvgr_err(gdev, HVGR_PM, "hvgr_pm_soft_init failed");
		return -1;
	}

	if (hvgr_pm_driver_hw_access_enable(gdev) != 0) {
		hvgr_pm_soft_term(gdev);
		hvgr_err(gdev, HVGR_PM, "hvgr_pm_driver_hw_access_enable failed");
		return -1;
	}
	return 0;
}

int hvgr_pm_gpu_access_disable(struct hvgr_device * const gdev)
{
	hvgr_pm_driver_hw_access_disable(gdev);
	return 0;
}

void hvgr_pm_gpu_access_term(struct hvgr_device * const gdev)
{
	hvgr_pm_soft_term(gdev);
}


int hvgr_pm_init(struct hvgr_device * const gdev)
{

	hvgr_pm_driver_hw_init(gdev);
	hvgr_pm_policy_init(gdev);

#if hvgr_version_ge(HVGR_V350)
	hvgr_pm_fast_pwr_init(gdev);
#endif
#if hvgr_version_ge(HVGR_V500)
	hvgr_rtu_debugfs_init(gdev);
#endif

	return 0;
}

void hvgr_pm_term(struct hvgr_device *gdev)
{
	/* Set power policy to job demand, so gpu can be power off. */
	atomic_set(&gdev->pm_dev.pm_pwr.cur_policy, HVGR_PM_JOB_DEMAND);
	/* Power off gpu when pm term. */
	hvgr_pm_power_off_sync(gdev);
	hvgr_pm_policy_term(gdev);
}

void hvgr_pm_request_cycle_counter(struct hvgr_device *gdev)
{
	hvgr_pm_driver_request_cycle_counter(gdev);
}

void hvgr_pm_release_cycle_counter(struct hvgr_device *gdev)
{
	hvgr_pm_driver_release_cycle_counter(gdev);
}

void hvgr_pm_try_power_off_gpu(struct hvgr_device *gdev)
{
	hvgr_pm_power_off_sync(gdev);
}

