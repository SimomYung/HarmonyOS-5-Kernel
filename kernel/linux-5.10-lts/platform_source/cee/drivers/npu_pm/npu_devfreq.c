/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2017-2020. All rights reserved.
 * Description: npu freq profile ops
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */
#include <linux/version.h>
#include <linux/errno.h>
#include <linux/module.h>
#include <linux/devfreq.h>
#include <linux/math64.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/pm.h>
#include <linux/pm_opp.h>
#include <linux/mutex.h>
#include <linux/list.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/devfreq_cooling.h>
#include <platform_include/cee/linux/npu_pm.h>
#ifdef CONFIG_NPU_PM_SMC
#include "smc/npu_pm_private.h"
#else
#include "npu_pm_private.h"
#endif

#ifdef CONFIG_BIG_MODEL_MONITOR
#include <platform_include/cee/linux/bmm.h>
#endif

#define NPU_PM_DEVFREQ_POLLING_INTERVAL		20 /* ms */
#define NPU_PM_DEVFREQ_DEV_NAME		"npufreq"
#define HZ_PER_KHZ      1000


static struct devfreq_dev_profile npu_pm_devfreq_profile;

int npu_pm_recommended_frequency(struct device *dev,
				   unsigned long *target_freq,
				   u32 flags)
{
	struct dev_pm_opp *opp = NULL;
	unsigned long freq;

	if (target_freq == NULL) {
		dev_err(dev, "%s invalid parm\n", __func__);
		return -EFAULT;
	}

	freq = *target_freq;
	opp = devfreq_recommended_opp(dev, &freq, flags);
	if (IS_ERR_OR_NULL(opp)) {
		dev_err(dev, "failed to get opp (%ld)\n", PTR_ERR(opp));
		return -ESPIPE;
	}

	dev_pm_opp_put(opp);
	*target_freq = freq;

	return 0;
}

static int npu_pm_devfreq_target(struct device *dev,
				   unsigned long *target_freq,
				   u32 flags)
{
	int err;
	struct npu_pm_device *pmdev = dev_get_drvdata(dev);
	struct devfreq *df = pmdev->devfreq;

	if (df == NULL) {
		dev_err(dev, "npu_pm_device devfreq not initialized yet\n");
		return -EPERM;
	}

#if defined(CONFIG_THERMAL_NPU) || defined(CONFIG_THERMAL_NPU_V2)
	struct devfreq_dev_profile *dp = pmdev->devfreq->profile;
	unsigned long limit_freq;
	unsigned int index;
#endif

	err = npu_pm_recommended_frequency(dev, target_freq, flags);
	if (err != 0) {
		dev_err(dev, "failed to get recommended frequency\n");
		return err;
	}

#if defined(CONFIG_THERMAL_NPU) || defined(CONFIG_THERMAL_NPU_V2)
	index = pmdev->devfreq->profile->max_state - pmdev->cur_state - 1;
	if (index >= pmdev->devfreq->profile->max_state)
		index = 0;
	limit_freq = dp->freq_table[index];
	if (limit_freq < *target_freq)
		*target_freq = limit_freq;
#endif

	err = npu_pm_dvfs_hal(*target_freq);
	if (err != 0)
		return err;

#ifdef CONFIG_BIG_MODEL_MONITOR
	bmm_notify_npu_freq(
		((struct npu_pm_device *) dev_get_drvdata(dev))->devfreq, *target_freq);
#endif

	return err;
}

static int npu_pm_devfreq_cur_freq(struct device *dev, unsigned long *freq)
{
	struct npu_pm_device *pmdev = dev_get_drvdata(dev);
	struct devfreq *df = pmdev->devfreq;

	if (df == NULL) {
		struct devfreq_dev_profile *dp = (struct devfreq_dev_profile *)&npu_pm_devfreq_profile;
		*freq = dp->initial_freq;

		dev_err(dev, "npu_pm_device df is not initialized, use the profile initial_freq. freq= %lu\n", *freq);
		return 0;
	}

	*freq = df->previous_freq;

	return 0;
}

static int npu_pm_get_dev_status(struct device *dev,
				   struct devfreq_dev_status *stat)
{
	return 0;
}

static void npu_pm_devfreq_exit(struct device *dev)
{
	struct npu_pm_device *pmdev = dev_get_drvdata(dev);
	struct devfreq_dev_profile *dp = pmdev->devfreq->profile;

	devm_kfree(pmdev->dev, dp->freq_table);
}

static struct devfreq_dev_profile npu_pm_devfreq_profile = {
	/* it would be abnormal to enable devfreq monitor during init */
	.polling_ms     = NPU_PM_DEVFREQ_POLLING_INTERVAL,
	.target         = npu_pm_devfreq_target,
	.get_dev_status = npu_pm_get_dev_status,
	.get_cur_freq   = npu_pm_devfreq_cur_freq,
	.exit           = npu_pm_devfreq_exit,
};

#if defined(CONFIG_THERMAL_NPU) || defined(CONFIG_THERMAL_NPU_V2)
static int npu_cooling_get_max_state(struct thermal_cooling_device *cdev,
				     unsigned long *state)
{
	struct npu_pm_device *pmdev = cdev->devdata;
	unsigned long max_state = pmdev->devfreq->profile->max_state;

	*state = max_state > 0 ? max_state - 1 : 0;

	return 0;
}

static int npu_cooling_get_cur_state(struct thermal_cooling_device *cdev,
				     unsigned long *state)
{
	struct npu_pm_device *pmdev = cdev->devdata;

	*state = pmdev->cur_state;

	return 0;
}

static int npu_cooling_set_cur_state(struct thermal_cooling_device *cdev,
				     unsigned long state)
{
	struct npu_pm_device *pmdev = cdev->devdata;
	unsigned long max_state = pmdev->devfreq->profile->max_state;
#ifdef CONFIG_THERMAL_NPU_V2
	unsigned long freq;
#endif

	if (state == pmdev->cur_state || max_state == 0)
		return 0;

	if (state > max_state - 1) {
		dev_err(pmdev->dev, "set cur state error:%lu!\n", state);
		return -EINVAL;
	}
	pmdev->cur_state = state;
#ifdef CONFIG_THERMAL_NPU_V2
	freq = pmdev->devfreq->profile->freq_table[max_state - state - 1];
	dev_pm_qos_update_request(&pmdev->req_max_freq,
				DIV_ROUND_UP(freq, HZ_PER_KHZ));
#endif

	return 0;
}

static struct thermal_cooling_device_ops npu_cooling_dev_ops = {
	.get_max_state = npu_cooling_get_max_state,
	.get_cur_state = npu_cooling_get_cur_state,
	.set_cur_state = npu_cooling_set_cur_state,
};
#endif

int npu_pm_devfreq_init(struct npu_pm_device *pmdev)
{
	struct devfreq_dev_profile *dp = &npu_pm_devfreq_profile;
	struct device *dev = pmdev->dev;
	struct device_node *np = dev->of_node;
	u32 init_freq = 0;
	struct dev_pm_opp *opp = NULL;
	int ret;

	if (np == NULL) {
		dev_err(dev, "npu dts not found\n");
		return -ENODEV;
	}

	if (dev_pm_opp_of_add_table(dev) != 0)
		return -EFAULT;

	ret = of_property_read_u32(np, "initial_freq", &init_freq);
	if (ret != 0) {
		dev_err(dev, "parse npu initial frequency fail %d\n", ret);
		return -EINVAL;
	}
	dp->initial_freq = (unsigned long)init_freq * KHZ;

	/* avoid that the initial_freq from dts is not exist */
	opp = dev_pm_opp_find_freq_ceil(dev, &dp->initial_freq);
	if (IS_ERR(opp)) {
		dev_warn(dev, "get initial freq from freq table fail\n");
		return -EFAULT;
	}

	dev_pm_opp_put(opp);

	/*
	 * devfreq_add_device only copies a few of npu_pm_dev's fields,
	 * so set drvdata explicitly so other models can access pmdev.
	 */
	dev_set_drvdata(dev, pmdev);
	if (strncmp(NPU_PM_DEVFREQ_DEV_NAME, dev_name(dev), strlen(NPU_PM_DEVFREQ_DEV_NAME)) != 0) {
		dev_err(dev, "devname: %s rename to: %s\n", dev_name(dev), NPU_PM_DEVFREQ_DEV_NAME);
		dev_set_name(dev, NPU_PM_DEVFREQ_DEV_NAME);
	}
	pmdev->devfreq = devfreq_add_device(dev, dp, "npu_pm_qos", NULL);
	if (IS_ERR(pmdev->devfreq))
		return PTR_ERR(pmdev->devfreq);
	/* npu default status: power off */
	ret = devfreq_suspend_device(pmdev->devfreq);
	if (ret != 0)
		dev_err(dev, "devfreq default status set failed\n");

#if defined(CONFIG_THERMAL_NPU) || defined(CONFIG_THERMAL_NPU_V2)
	pmdev->cur_state = pmdev->devfreq->profile->max_state;
	pmdev->devfreq_cooling = thermal_cooling_device_register("npu_pm",
								 pmdev,
								 &npu_cooling_dev_ops);
	if (IS_ERR_OR_NULL(pmdev->devfreq_cooling)) {
		ret = PTR_ERR(pmdev->devfreq_cooling);
		dev_err(pmdev->dev,
			"Failed to register cooling device %d\n",
			ret);
		return ret;
	}
#ifdef CONFIG_THERMAL_NPU_V2
	ret = dev_pm_qos_add_request(pmdev->dev, &pmdev->req_max_freq,
				     DEV_PM_QOS_MAX_FREQUENCY,
				     PM_QOS_MAX_FREQUENCY_DEFAULT_VALUE);
	if (ret != 0) {
		dev_err(pmdev->dev, "failed to register qos %d\n", ret);
		return ret;
	}
#endif
#endif

	return 0;
}

void npu_pm_devfreq_term(struct npu_pm_device *pmdev)
{
	struct device *dev = pmdev->dev;
	int err;

	err = devfreq_remove_device(pmdev->devfreq);
	if (err != 0)
		dev_err(dev, "Failed to terminate devfreq %d\n", err);
	else
		pmdev->devfreq = NULL;
}

int npu_pm_devfreq_suspend(struct devfreq *devfreq)
{
	return devfreq_suspend_device(devfreq);
}

int npu_pm_devfreq_resume(struct devfreq *devfreq)
{
	return devfreq_resume_device(devfreq);
}
