/*
 * ddr_scene.c
 *
 * func of different scene for ddr
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
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <securec.h>
#include <linux/uaccess.h>
#include <linux/seq_file.h>
#include <linux/arm_ffa.h>
#include <platform_include/see/ffa/ffa_msg_id.h>
#include <platform_include/see/ffa/ffa_plat_drv.h>
#include <platform_include/cee/linux/ddr_perf_ctrl.h>
#include "ddr_perf_ctrl_common.h"

#define DEVICE_NAME "ddr_scene"
#define SUCCESS 0
#define ERR                  (-1)

static unsigned int g_cur_scene = 0;
#define SCENE_MAGIC		0x77f5a68


static void set_ddr_scene(unsigned long long scene)
{
#ifdef CONFIG_PLATDRV_SUPPORT_FFA
	int ret;
	if (scene == g_cur_scene)
		return;

	struct ffa_send_direct_data args = {
		.data0 = FID_DDR_SCENE_SET,
		.data1 = SCENE_MAGIC,
		.data2 = (unsigned long)scene,
		.data3 = 0,
		.data4 = 0,
	};

	ret = ffa_platdrv_send_msg(&args);
	if (ret != 0) {
		printk(KERN_ERR "failed to send_msg!\n");
		return;
	}
	g_cur_scene = scene;
#endif
}

int perf_ctrl_set_ddr_scene(void __user *uarg)
{
	struct ddr_scene scene = {0};

	if (uarg == NULL)
		return -EINVAL;

	if (copy_from_user(&scene, uarg, sizeof(struct ddr_scene))) {
		pr_err("%s: copy_from_user fail\n", __func__);
		return -EFAULT;
	}

	set_ddr_scene(scene.app_scene);
	return 0;
}