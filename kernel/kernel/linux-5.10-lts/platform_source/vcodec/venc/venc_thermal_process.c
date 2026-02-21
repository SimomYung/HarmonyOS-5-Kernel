/*
 * venc_thermal_process.c
 *
 * This is for venc temperature process
 *
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include "platform_include/cee/linux/thermal_interact_lpmcu.h"
#include "venc_regulator.h" 
#include "venc_thermal_process.h"

static int venc_lowtemp_process(unsigned int level)
{
	VCODEC_INFO_VENC("device %s low temperature\n", level ? "into" : "out of");
	venc_set_low_temp_flag(level);

	return 0;
}

static struct thermal_lpmcu_ops g_venc_thermal_ops = {
	.name = "venc_lowtemp",
	.lowtemp_thermal_handler = venc_lowtemp_process,
};

void venc_thermal_init(void)
{
    if (thermal_lpmcu_ops_register(&g_venc_thermal_ops))
		VCODEC_ERR_VENC("register thermal lpm failed\n");
	VCODEC_INFO_VENC("register venc thermal lpm finish\n");
}

void venc_thermal_deinit(void)
{
	thermal_lpmcu_ops_unregister(&g_venc_thermal_ops);
	VCODEC_INFO_VENC("unregister venc thermal lpm finish\n");
}
