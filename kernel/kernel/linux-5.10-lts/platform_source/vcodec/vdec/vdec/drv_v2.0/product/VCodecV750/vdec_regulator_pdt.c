/*
 * vdec_regulator_pdt.c
 *
 * This is for vdec platform
 *
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
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

#include "vcodec_vdec_regulator.h"
#include "dbg.h"

static int32_t vdec_pdt_get_regulator_clk(struct device *dev, struct vdec_regulator *regulator)
{
	regulator->clk_vdec = devm_clk_get(dev, VDEC_CLOCK_NAME);
	if (IS_ERR_OR_NULL(regulator->clk_vdec)) {
		dprint(PRN_FATAL, "get clk failed\n");
		return VCODEC_FAILURE;
	}
	return VCODEC_SUCCESS;
}

static int32_t vdec_pdt_regulator_open_clk(void)
{
	vdec_plat *plt = vdec_plat_get_entry();
	int32_t ret = clk_prepare_enable(plt->regulator_info.clk_vdec);
	if (ret) {
		dprint(PRN_FATAL, "clk enable failed\n");
		return VCODEC_FAILURE;
	}
	return VCODEC_SUCCESS;
}

static void vdec_pdt_regulator_close_clk(void)
{
	vdec_plat *plt = vdec_plat_get_entry();
	clk_disable_unprepare(plt->regulator_info.clk_vdec);
}

static bool vdec_pdt_transition_clk_enable(void)
{
	return false;
}

static struct vdec_regulator_pdt_ops g_vdec_regulator_ops = {
	.get_clk = vdec_pdt_get_regulator_clk,
	.open_clk = vdec_pdt_regulator_open_clk,
	.close_clk = vdec_pdt_regulator_close_clk,
	.transition_clk_enable = vdec_pdt_transition_clk_enable,
	.map_reg_info = VCODEC_NULL,
	.unmap_reg_info = VCODEC_NULL,
	.vdec_global_reset = VCODEC_NULL,
};

struct vdec_regulator_pdt_ops *get_vdec_regulator_plat_ops(void)
{
	return &g_vdec_regulator_ops;
}