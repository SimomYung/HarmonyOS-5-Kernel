/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2021-2022. All rights reserved.
 *
 * Description: This file is used when the vulkan jit enable.
 * Author:
 * Create: 2021-02-03
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */

#include "hvgr_pm_policy.h"

#include "hvgr_log_api.h"
#include "hvgr_dm_api.h"

#include "hvgr_pm_api.h"
#include "hvgr_pm_power.h"

bool hvgr_pm_is_need_power_on(struct hvgr_device *gdev, bool gpu_reset)
{
	struct hvgr_pm_pwr *pm_pwr = &gdev->pm_dev.pm_pwr;

	if (hvgr_pm_is_suspending(gdev)) {
		if (gpu_reset)
			return true;
		else
			return false;
	}

	if (atomic_read(&pm_pwr->user_ref) == 0 &&
		atomic_read(&pm_pwr->cur_policy) == HVGR_PM_JOB_DEMAND)
		return false;
	return true;
}

bool hvgr_pm_is_need_power_off(struct hvgr_device *gdev)
{
	int sr_state;
	struct hvgr_pm_pwr *pm_pwr = &gdev->pm_dev.pm_pwr;

	if (atomic_read(&pm_pwr->user_ref) != 0)
		return false;

	sr_state = atomic_read(&pm_pwr->sr_state);
	if (atomic_read(&pm_pwr->cur_policy) == HVGR_PM_ALWAYS_ON &&
		(sr_state == HVGR_PM_RESUMEING || sr_state == HVGR_PM_ACTIVE))
		return false;
	return true;
}

#if hvgr_version_ge(HVGR_V350)

static void hvgr_pm_policy_always_on_on(struct hvgr_device *gdev, uint32_t new_policy)
{
	struct hvgr_pm_pwr *pm_pwr = &gdev->pm_dev.pm_pwr;

	mutex_lock(&pm_pwr->power_ctrl_lock);
	mutex_lock(&pm_pwr->power_lock);

	if (!hvgr_pm_auto_gpc_is_enable(gdev)) {
		hvgr_info(gdev, HVGR_PM, "auto gpc not enable, direct set");
		goto out;
	}

	if (pm_pwr->cur_status == HVGR_PM_POWER_OFF) {
		hvgr_info(gdev, HVGR_PM, "power off, direct set");
		goto out;
	}

	hvgr_info(gdev, HVGR_PM, "set new_policy always on");
	pm_pwr->cur_policy_tmp = (int)new_policy;
	mutex_unlock(&pm_pwr->power_lock);
	(void)hvgr_pm_reboot_gpu(gdev);

	mutex_unlock(&pm_pwr->power_ctrl_lock);
	return;

out:
	atomic_set(&pm_pwr->cur_policy, (int)new_policy);
	pm_pwr->cur_policy_tmp = (int)new_policy;
	mutex_unlock(&pm_pwr->power_lock);
	(void)hvgr_pm_power_on_sync(gdev, false);
	mutex_unlock(&pm_pwr->power_ctrl_lock);
}

static void hvgr_pm_policy_always_on_off(struct hvgr_device *gdev, uint32_t new_policy)
{
	struct hvgr_pm_pwr *pm_pwr = &gdev->pm_dev.pm_pwr;

	mutex_lock(&pm_pwr->power_ctrl_lock);
	mutex_lock(&pm_pwr->power_lock);
	/*
	 * current policy is always_on, auto gpc is not enable,
	 * so we can directly set cur_policy.
	 */
	pm_pwr->cur_policy_tmp = (int)new_policy;
	atomic_set(&pm_pwr->cur_policy, (int)new_policy);
	hvgr_info(gdev, HVGR_PM, "set new_policy job demand");
	mutex_unlock(&pm_pwr->power_lock);
	(void)hvgr_pm_reboot_gpu(gdev);

	mutex_unlock(&pm_pwr->power_ctrl_lock);
}

static uint32_t hvgr_pm_get_show_policy(struct hvgr_device *gdev)
{
	return gdev->pm_dev.pm_pwr.cur_policy_tmp;
}

#else

static void hvgr_pm_policy_always_on_on(struct hvgr_device *gdev, uint32_t new_policy)
{
	struct hvgr_pm_pwr *pm_pwr = &gdev->pm_dev.pm_pwr;

	atomic_set(&pm_pwr->cur_policy, (int)new_policy);
	(void)hvgr_pm_power_on_sync(gdev, false);
}

static void hvgr_pm_policy_always_on_off(struct hvgr_device *gdev, uint32_t new_policy)
{
	struct hvgr_pm_pwr *pm_pwr = &gdev->pm_dev.pm_pwr;

	atomic_set(&pm_pwr->cur_policy, (int)new_policy);
#ifdef HVGR_GPU_POWER_ASYNC
	hvgr_pm_power_off_async(gdev);
#else
	hvgr_pm_power_off_sync(gdev);
#endif
}

static uint32_t hvgr_pm_get_show_policy(struct hvgr_device *gdev)
{
	struct hvgr_pm_pwr *pm_pwr = &gdev->pm_dev.pm_pwr;

	return (uint32_t)atomic_read(&pm_pwr->cur_policy);
}

#endif

ssize_t hvgr_pm_set_policy(struct device *dev, struct device_attribute *attr,
	const char *buf, size_t count)
{
	uint32_t new_policy;
	struct hvgr_device *gdev = dev_get_drvdata(dev);
	struct hvgr_pm_pwr *pm_pwr = NULL;
	const char *policy_name[HVGR_PM_POLICY_MAX] = {
		"always_on",
		"job_demand",
	};

	if (gdev == NULL)
		return -ENODEV;

	pm_pwr = &gdev->pm_dev.pm_pwr;

	for (new_policy = 0; new_policy < HVGR_PM_POLICY_MAX; new_policy++) {
		if (sysfs_streq(policy_name[new_policy], buf))
			break;
	}

	if (unlikely(new_policy == HVGR_PM_POLICY_MAX)) {
		hvgr_err(gdev, HVGR_PM, "power_policy: policy not found\n");
		return -EINVAL;
	}

	if ((uint32_t)atomic_read(&pm_pwr->cur_policy) == new_policy)
		return (ssize_t)count;

	if (new_policy == HVGR_PM_ALWAYS_ON)
		hvgr_pm_policy_always_on_on(gdev, new_policy);
	else
		hvgr_pm_policy_always_on_off(gdev, new_policy);

	return (ssize_t)count;
}

ssize_t hvgr_pm_show_policy(struct device *dev, struct device_attribute *attr, char *const buf)
{
	uint32_t id;
	uint32_t policy;
	ssize_t ret = 0;
	struct hvgr_pm_pwr *pm_pwr = NULL;
	struct hvgr_device *gdev = dev_get_drvdata(dev);
	const char *policy_name[HVGR_PM_POLICY_MAX] = {
		"always_on",
		"job_demand",
	};

	if (gdev == NULL)
		return -ENODEV;

	pm_pwr = &gdev->pm_dev.pm_pwr;

	mutex_lock(&pm_pwr->power_lock);
	for (id = 0; id < HVGR_PM_POLICY_MAX; id++) {
		policy = hvgr_pm_get_show_policy(gdev);
		if (policy == id)
			ret += scnprintf(buf + ret,
				PAGE_SIZE - ret, "[%s] ", policy_name[id]);
		else
			ret += scnprintf(buf + ret,
				PAGE_SIZE - ret, "%s ", policy_name[id]);
	}
	mutex_unlock(&pm_pwr->power_lock);

	ret += scnprintf(buf + ret, PAGE_SIZE - ret, "\n");
	return ret;
}

static DEVICE_ATTR(power_policy, S_IRUGO | S_IWUSR, hvgr_pm_show_policy,
	hvgr_pm_set_policy);

#ifdef HVGR_GPU_POWER_ASYNC
#endif

void hvgr_pm_policy_init(struct hvgr_device *gdev)
{
	struct hvgr_pm_pwr *pm_pwr = &gdev->pm_dev.pm_pwr;

	if (hvgr_is_fpga(gdev) || gdev->subsys_ops->pm_is_always_on())
		atomic_set(&pm_pwr->cur_policy, HVGR_PM_ALWAYS_ON);
	else
		atomic_set(&pm_pwr->cur_policy, HVGR_PM_JOB_DEMAND);

	pm_pwr->cur_policy_tmp = atomic_read(&pm_pwr->cur_policy);

	if (sysfs_create_file(&gdev->dev->kobj, &dev_attr_power_policy.attr))
		hvgr_err(gdev, HVGR_PM, "Create power_policy failed.");
#ifdef HVGR_GPU_POWER_ASYNC
#endif
}

void hvgr_pm_policy_term(struct hvgr_device *gdev)
{
	sysfs_remove_file(&gdev->dev->kobj, &dev_attr_power_policy.attr);
#ifdef HVGR_GPU_POWER_ASYNC
#endif
}

bool hvgr_is_always_on(struct hvgr_device *gdev) {
	if (atomic_read(&gdev->pm_dev.pm_pwr.cur_policy) == HVGR_PM_ALWAYS_ON)
		return true;
	return false;
}
