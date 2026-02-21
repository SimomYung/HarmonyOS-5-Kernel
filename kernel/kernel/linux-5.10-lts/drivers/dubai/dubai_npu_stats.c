/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: dubai npu stats
 */

#include "dubai_npu_stats.h"

#include <securec.h>
#include <linux/slab.h>
#include <chipset_common/dubai/dubai_ioctl.h>
#include "utils/dubai_utils.h"

#define IOC_NPU_ENABLE DUBAI_NPU_DIR_IOC(W, 1, bool)
#define IOC_NPU_INFO_GET DUBAI_NPU_DIR_IOC(R, 2, struct dubai_npu_stats)

static DEFINE_MUTEX(dubai_npu_lock);
static uint64_t g_npu_energy = 0; /* unit: mAS */
static struct dubai_npu_stats g_delta_stats;
static struct dubai_npu_freq_info *g_npu_freq_stats = NULL;
static struct dubai_npu_stats_ops *g_npu_stats_op = NULL;

static int dubai_npu_enable(void __user *argp);
static int dubai_npu_info_parse(void __user *argp);

long dubai_ioctl_npu(unsigned int cmd, void __user *argp)
{
	int rc = 0;

	if (argp == NULL) {
		dubai_err("invalid argp");
		return -EINVAL;
	}
	switch (cmd) {
	case IOC_NPU_ENABLE:
		rc = dubai_npu_enable(argp);
		break;
	case IOC_NPU_INFO_GET:
		rc = dubai_npu_info_parse(argp);
		break;
	default:
		rc = -EINVAL;
		break;
	}

	return rc;
}

static void dubai_npu_stats_reset(void)
{
	mutex_lock(&dubai_npu_lock);
	if (g_npu_freq_stats) {
		kfree(g_npu_freq_stats);
		g_npu_freq_stats = NULL;
	}
	(void)memset_s(&g_delta_stats, sizeof(struct dubai_npu_stats),
		       0, sizeof(struct dubai_npu_stats));
	mutex_unlock(&dubai_npu_lock);
}

static int dubai_npu_info_init(void)
{
	int ret;
	unsigned int freq_num;

	freq_num = g_npu_stats_op->get_num();
	if (freq_num == 0) {
		dubai_err("invalid NPU freq num");
		return -EINVAL;
	}
	mutex_lock(&dubai_npu_lock);
	g_npu_freq_stats = kzalloc(freq_num * sizeof(struct dubai_npu_freq_info), GFP_KERNEL);
	if (!g_npu_freq_stats) {
		dubai_err("Failed to alloc mem for NPU freq stats");
		mutex_unlock(&dubai_npu_lock);
		return -ENOMEM;
	}
	ret = memset_s(&g_delta_stats, sizeof(struct dubai_npu_stats),
		       0, sizeof(struct dubai_npu_stats));
	if (ret != 0) {
		dubai_err("NPU info memset failed");
		kfree(g_npu_freq_stats);
		g_npu_freq_stats = NULL;
		mutex_unlock(&dubai_npu_lock);
		return -EFAULT;
	}
	g_delta_stats.num = freq_num;
	g_npu_energy = 0;
	mutex_unlock(&dubai_npu_lock);

	return 0;
}

static int dubai_npu_enable(void __user *argp)
{
	int err;
	bool enable = false;

	if (!g_npu_stats_op || !g_npu_stats_op->enable || !g_npu_stats_op->get_num) {
		dubai_info("NPU stats not supported");
		return -EOPNOTSUPP;
	}
	if (get_enable_value(argp, &enable)) {
		dubai_err("Failed to get value from user");
		return -EFAULT;
	}
	err = g_npu_stats_op->enable(enable);
	if (err != 0) {
		if (err == -EEXIST) {
			dubai_info("NPU stats is already %s", enable ? "enable" : "disable");
			return 0;
		} else {
			dubai_err("Failed to enable stats");
			return -EFAULT;
		}
	}

	if (enable) {
		err = dubai_npu_info_init();
		if (err != 0) {
			dubai_err("Failed to init npu dubai info");
			(void)g_npu_stats_op->enable(false);
			return -EFAULT;
		}
	} else {
		dubai_npu_stats_reset();
	}

	return 0;
}

static int dubai_npu_info_parse(void __user *argp)
{
	int ret;
	unsigned int i;
	uint64_t npu_energy;
	struct dubai_npu_freq_info *freq_stats = NULL;

	if (!g_npu_stats_op || !g_npu_stats_op->get_num || !g_npu_stats_op->get_stats) {
		dubai_err("NPU stats not supported");
		return -EOPNOTSUPP;
	}

	freq_stats = kzalloc(g_delta_stats.num * sizeof(struct dubai_npu_freq_info), GFP_KERNEL);
	if (!freq_stats) {
		dubai_err("Failed to alloc npu_freq_stats");
		return -ENOMEM;
	}

	if (g_npu_stats_op->get_stats(freq_stats, &npu_energy, g_delta_stats.num)) {
		dubai_err("Failed to get stats of NPU");
		kfree(freq_stats);
		return -EFAULT;
	}

	mutex_lock(&dubai_npu_lock);
	if (unlikely(!g_npu_freq_stats)) {
		mutex_unlock(&dubai_npu_lock);
		dubai_err("invalid npu stats");
		return -EFAULT;
	}
	for (i = 0; i < g_delta_stats.num; i++) {
		g_delta_stats.npu_info[i].freq = freq_stats[i].freq;
		g_delta_stats.npu_info[i].run_time = (freq_stats[i].run_time < g_npu_freq_stats[i].run_time) ?
						      0 : freq_stats[i].run_time - g_npu_freq_stats[i].run_time;
	}
	g_delta_stats.energy = (npu_energy < g_npu_energy) ? 0 : npu_energy - g_npu_energy;
	g_npu_energy = npu_energy;
	ret = memcpy_s(g_npu_freq_stats, g_delta_stats.num * sizeof(struct dubai_npu_freq_info),
		       freq_stats, g_delta_stats.num * sizeof(struct dubai_npu_freq_info));
	if (ret != 0) {
		dubai_err("NPU info copy to freq stats failed");
		mutex_unlock(&dubai_npu_lock);
		kfree(freq_stats);
		return -EFAULT;
	}
	mutex_unlock(&dubai_npu_lock);
	kfree(freq_stats);

	if (g_delta_stats.num > 0) {
		if (unlikely(copy_to_user(argp, &g_delta_stats, sizeof(struct dubai_npu_stats)))) {
			return -EFAULT;
		}
	}

	return 0;
}

int dubai_npu_register_ops(struct dubai_npu_stats_ops *op)
{
	if (!g_npu_stats_op) {
		g_npu_stats_op = op;
	}

	return 0;
}

int dubai_npu_unregister_ops(void)
{
	if (g_npu_stats_op) {
		g_npu_stats_op = NULL;
	}
	return 0;
}

void dubai_npu_stats_init(void)
{
}

void dubai_npu_stats_exit(void)
{
	dubai_npu_stats_reset();
}
