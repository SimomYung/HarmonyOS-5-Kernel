/*
 * heca_v2_lts.c
 *
 * hisilicon efficinecy control algorithm lts.
 *
 * Copyright (c) 2024-2024 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */

#include <linux/module.h>
#include <linux/notifier.h>
#include <linux/suspend.h>
#include <linux/miscdevice.h>
#include "heca_worker.h"
#include "heca_ipc.h"
#include "heca_map_ddr_v2.h"
#include "heca_gpu_lts.h"
#include <securec.h>
#include <platform_include/cee/linux/heca_v2_lts.h>

#define HECA_MISC_DEV_NAME "heca_misc_dev"

#define FREQ_CTRL_IOCTL_SET_HECA \
	_IOR('F', 0x01, struct freq_ctrl_heca)

#define FREQ_CTRL_IOCTL_GPU_HECA_SET \
	_IOR('F', 0x02, struct freq_ctrl_heca)

#define FREQ_CTRL_IOCTL_GPU_HECA_CONFIG \
	_IOR('F', 0x03, struct heca_gpu_conf)

static bool g_heca_enable = false;

static DEFINE_SPINLOCK(state_lock);

void heca_start_timer(enum heca_ipc_id type)
{
	unsigned int msg = 0;
	int ret = heca_ipc_send_data(type, &msg, sizeof(msg), HECA_IPC_ASYNC);

	pr_info("heca send start %u ret %d\n", type, ret);
}

void heca_stop_timer(enum heca_ipc_id type)
{
	unsigned int msg = 0;
	int ret = heca_ipc_send_data(type, &msg, sizeof(msg), HECA_IPC_ASYNC);

	pr_info("heca send stop %u ret %d\n", type, ret);
}

void set_heca_state(bool enabled)
{
	spin_lock(&state_lock);
	if (g_heca_enable == enabled)
		goto unlock;

	g_heca_enable = enabled;

	if (g_heca_enable)
		heca_start_timer(HECA_START);
	else
		heca_stop_timer(HECA_STOP);
unlock:
	spin_unlock(&state_lock);
}
EXPORT_SYMBOL(set_heca_state);

static void set_cpu_heca_state(unsigned long arg)
{
	struct freq_ctrl_heca heca_data;

	if (copy_from_user(&heca_data, (void __user *)arg, sizeof(heca_data)) != 0) {
		pr_err("copy from user failed\n");
		return;
	}

	set_heca_state(!!heca_data.enabled);
}

static int heca_pm_notify(struct notifier_block *nb, unsigned long action, void *ptr)
{
	if (action != PM_POST_SUSPEND)
		return NOTIFY_OK;

	spin_lock(&state_lock);
	if (g_heca_enable)
		heca_start_timer(HECA_START);
	spin_unlock(&state_lock);

	return NOTIFY_OK;
}

static struct notifier_block g_heca_pm_notify = {
	.notifier_call = heca_pm_notify,
};

static void heca_pm_init(void)
{
	register_pm_notifier(&g_heca_pm_notify);
}

static long heca_ctrl_ioctl(struct file *file, unsigned int cmd,
			    unsigned long arg)
{
	int err;

	if (arg == 0) {
                pr_err("heca ctrl ioctl: Bad input argument address\n");
		return -EINVAL;
	}

	switch (cmd) {
	case FREQ_CTRL_IOCTL_SET_HECA:
		set_cpu_heca_state(arg);
		break;
	case FREQ_CTRL_IOCTL_GPU_HECA_SET:
		set_gpu_heca_state(arg);
		break;
	case FREQ_CTRL_IOCTL_GPU_HECA_CONFIG:
		set_gpu_heca_conf(arg);
		break;
	default:
		pr_err("heca ioctl cmd not support: %u\n", cmd);
		err = -EINVAL;
		break;
	}

	return err;
}

static int heca_ctrl_open(struct inode *inode, struct file *filp)
{
	return 0;
}

static int heca_ctrl_release(struct inode *inode, struct file *filp)
{
	return 0;
}

static const struct file_operations heca_misc_fops = {
	.owner = THIS_MODULE,
	.open = heca_ctrl_open,
	.release = heca_ctrl_release,
	.unlocked_ioctl = heca_ctrl_ioctl,
};

static struct miscdevice heca_misc_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = HECA_MISC_DEV_NAME,
	.fops = &heca_misc_fops,
};

static int __init heca_init(void)
{
	int err;

	heca_worker_init();
	heca_pm_init();
	heca_gpu_init();

	err = misc_register(&heca_misc_dev);
	if (err < 0)
		pr_err("misc device %s register failed: %d\n",
		       HECA_MISC_DEV_NAME, err);	

	return 0;
}
module_init(heca_init);
