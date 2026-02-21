/*
 * thermal_ctrl.c
 *
 * interfaces defined for thermal_ctrl
 *
 * Copyright (c) 2020-2024 Huawei Technologies Co., Ltd.
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

#include <linux/compat.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/platform_device.h>
#include <linux/uaccess.h>
#include <linux/of.h>
#include <platform_include/cee/linux/lpm_thermal.h>
#include <platform_include/cee/linux/thermal_perf_ctrl.h>

const char *const g_tz_type_list[TZ_ID_MAX] = {
	"soc",
	"cluster0",
	"cluster1",
	"cluster2",
	"gpu",
	"npu",
	"modem",
};

static int get_soc_ip_temp(const char *name)
{
	int sensor_id, temp, ret;

	sensor_id = ipa_get_tsensor_id(name);
	ret = ipa_get_sensor_value(sensor_id, &temp);
	if (ret != 0) {
		pr_err("%s: fail, ret=%d\n", __func__, ret);
		temp = 0;
	}

	return temp;
}

static int perf_ctrl_get_tz_temp(void __user *uarg, enum thermal_tz_id id)
{
	int i, temp, max_temp = INT_MIN;
	int ret = 0;
	int soc_temp[GPU_TZ_ID] = {0};

	if (uarg == NULL) {
		pr_err("uarg is NULL\n");
		return -EINVAL;
	}

	if (id < SOC_TZ_ID || id >= TZ_ID_MAX) {
		pr_err("id is error. id=%d\n", id);
		return -EINVAL;
	}

	if (copy_from_user(&temp, uarg, sizeof(int))) {
		pr_err("%s: copy_from_user fail\n", __func__);
		return -EFAULT;
	}

	if (id != SOC_TZ_ID) {
		temp = get_soc_ip_temp(g_tz_type_list[id]);
	} else {
		for (i = SOC_TZ_ID + 1; i <= GPU_TZ_ID; i++) {
			soc_temp[i] = get_soc_ip_temp(g_tz_type_list[i]);
			if (soc_temp[i] > max_temp)
				max_temp = soc_temp[i];
		}
		temp = max_temp;
	}

	if (copy_to_user(uarg, &temp, sizeof(int))) {
		pr_err("%s: copy_to_user fail\n", __func__);
		ret = -EFAULT;
	}

	return ret;
}

static int perf_ctrl_get_all_tz_temp(void __user *uarg)
{
	struct tz_temp tz_temp_val;
	int i, max_temp = INT_MIN;
	int ret = 0;
	int soc_temp[TZ_ID_MAX] = {0};

	if (uarg == NULL)
		return -EINVAL;

	if (copy_from_user(&tz_temp_val, uarg, sizeof(struct tz_temp))) {
		pr_err("%s: copy_from_user fail\n", __func__);
		return -EFAULT;
	}

	for (i = SOC_TZ_ID + 1; i < TZ_ID_MAX; i++) {
		soc_temp[i] = get_soc_ip_temp(g_tz_type_list[i]);
		if (i <= GPU_TZ_ID && soc_temp[i] > max_temp)
			max_temp = soc_temp[i];
	}
	soc_temp[SOC_TZ_ID] = max_temp;

	tz_temp_val.soc_temp = soc_temp[SOC_TZ_ID];
	tz_temp_val.cluster0_temp = soc_temp[CLUSTER_0_TZ_ID];
	tz_temp_val.cluster1_temp = soc_temp[CLUSTER_1_TZ_ID];
	tz_temp_val.cluster2_temp = soc_temp[CLUSTER_2_TZ_ID];
	tz_temp_val.gpu_temp = soc_temp[GPU_TZ_ID];
	tz_temp_val.npu_temp = soc_temp[NPU_TZ_ID];
	tz_temp_val.modem_temp = soc_temp[MODEM_TZ_ID];

	if (copy_to_user(uarg, &tz_temp_val, sizeof(struct tz_temp))) {
		pr_err("%s: copy_to_user fail\n", __func__);
		ret = -EFAULT;
	}

	return ret;
}

static long thermal_ctrl_ioctl(struct file *file, unsigned int cmd,
			       unsigned long arg)
{
	void __user *uarg = (void __user *)(uintptr_t)arg;
	int ret = 0;

	if (cmd >= THERMAL_CMD_MAX) {
		pr_err("thermal temp cmd:0x%x not right!\n", cmd);
		return -EINVAL;
	}
	pr_info("thermal ioctl cmd:0x%x\n", cmd);
	switch (cmd) {
	case GET_SOC_TEMP:
		ret = perf_ctrl_get_tz_temp(uarg, SOC_TZ_ID);
		break;

	case GET_CLUSTER0_TEMP:
		ret = perf_ctrl_get_tz_temp(uarg, CLUSTER_0_TZ_ID);
		break;

	case GET_CLUSTER1_TEMP:
		ret = perf_ctrl_get_tz_temp(uarg, CLUSTER_1_TZ_ID);
		break;

	case GET_CLUSTER2_TEMP:
		ret = perf_ctrl_get_tz_temp(uarg, CLUSTER_2_TZ_ID);
		break;

	case GET_GPU_TEMP:
		ret = perf_ctrl_get_tz_temp(uarg, GPU_TZ_ID);
		break;

	case GET_NPU_TEMP:
		ret = perf_ctrl_get_tz_temp(uarg, NPU_TZ_ID);
		break;

	case GET_MODEM_TEMP:
		ret = perf_ctrl_get_tz_temp(uarg, MODEM_TZ_ID);
		break;

	case GET_THERMAL_TEMP:
		ret = perf_ctrl_get_all_tz_temp(uarg);
		break;

	default:
		pr_err("thermal temp cmd:0x%x not right!\n", cmd);
		ret = -EINVAL;
	}
	if (ret)
		pr_err("thermal_ctrl_ioctl failed. ret:%d\n", ret);

	return ret;
}

#ifdef CONFIG_COMPAT
static long thermal_ctrl_compact_ioctl(struct file *file, unsigned int cmd,
				       unsigned long arg)
{
	return thermal_ctrl_ioctl(file, cmd, (unsigned long)(compat_ptr(arg)));
}
#endif

static int thermal_ctrl_open(struct inode *inode, struct file *file)
{
	return 0;
}

static int thermal_ctrl_release(struct inode *inode, struct file *file)
{
	return 0;
}

static const struct file_operations thermal_ctrl_fops = {
	.owner = THIS_MODULE,
#ifdef CONFIG_COMPAT
	.compat_ioctl = thermal_ctrl_compact_ioctl,
#endif
	.unlocked_ioctl = thermal_ctrl_ioctl,
	.open = thermal_ctrl_open,
	.release = thermal_ctrl_release,
};

static struct miscdevice thermal_ctrl_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "hisi_thermal_ctrl",
	.fops = &thermal_ctrl_fops,
};

static int __init thermal_ctrl_dev_init(void)
{
	int err;
	
	pr_info("thermal_ctrl_dev_init\n");
	err = misc_register(&thermal_ctrl_device);
	if (err != 0)
		return err;

	return 0;
}

static void __exit thermal_ctrl_dev_exit(void)
{
	misc_deregister(&thermal_ctrl_device);
}

module_init(thermal_ctrl_dev_init);
module_exit(thermal_ctrl_dev_exit);
