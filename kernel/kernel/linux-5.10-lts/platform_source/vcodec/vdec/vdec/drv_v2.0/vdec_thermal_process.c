/*
 * vdec_thermal_process.c
 *
 * This is for vdec temperature process
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

#include <linux/module.h>
#include "platform_include/cee/linux/thermal_interact_lpmcu.h"
#include "vcodec_vdec_plat.h"
#include "vcodec_vdec.h"
#include "vcodec_vdec_regulator.h"
#include "dbg.h"

static void vdec_lowtemp_work(struct work_struct *work __attribute__((__unused__)))
{
	clk_rate_e dst_clk_rate = VDEC_CLK_RATE_LOWER;
	vdec_entry *vdec = vdec_get_entry();

	vdec_mutex_lock(&vdec->vdec_mutex);
	if (vdec->power_state == false) {
		vdec_mutex_unlock(&vdec->vdec_mutex);
		return;
	}
	vdec_plat_get_static_clk_rate(&dst_clk_rate);
	vdec_plat_regulator_set_clk_rate(dst_clk_rate);
	vdec_mutex_unlock(&vdec->vdec_mutex);
}

static DECLARE_WORK(g_vdec_lowtemp_work, vdec_lowtemp_work);

static int vdec_lowtemp_handler(unsigned int level)
{
	dprint(PRN_DBG, "device %s low temperature\n", level ? "into" : "out of");
	vdec_plat *plt = vdec_plat_get_entry();

	plt->clk_ctrl.low_temp_flag = 0;
	if (level != 0)
		plt->clk_ctrl.low_temp_flag = 1;

	if (!schedule_work(&g_vdec_lowtemp_work))
		dprint(PRN_ERROR, "schedule work failed\n");

	return 0;
}

static struct thermal_lpmcu_ops g_vdec_thermal_ops = {
	.name = "vdec_lowtemp",
	.lowtemp_thermal_handler = vdec_lowtemp_handler,
};

static int32_t __init vdec_thermal_init(void)
{
	if (!thermal_lpmcu_ops_register(&g_vdec_thermal_ops))
		dprint(PRN_ALWS, "register vdec thermal ops\n");

	return 0;
}

static void __exit vdec_thermal_deinit(void)
{
	thermal_lpmcu_ops_unregister(&g_vdec_thermal_ops);
	dprint(PRN_ALWS, "unregister vdec thermal ops\n");
}

module_init(vdec_thermal_init);
module_exit(vdec_thermal_deinit);