/*
 * vcodec_vdec_plat_v750.c
 *
 * This is for vdec platform
 *
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
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
#include <linux/of.h>
#include "vcodec_vdec_plat.h"
#include "vcodec_vdec_regulator.h"

static bool bypass_vdec_by_soc_spec(void)
{
	return false;
}

static bool need_cfg_tbu_max_tok_trans(void)
{
	vdec_dts *dts_info = &(vdec_plat_get_entry()->dts_info);
	return dts_info->is_es_plat;
}

static bool need_enable_tbu_ecc(void)
{
	return false;
}

static vdec_plat_ops g_vdec_plat_v750_ops = {
	.vcodec_vdec_device_bypass = bypass_vdec_by_soc_spec,
	.smmu_need_cfg_max_tok_trans = need_cfg_tbu_max_tok_trans,
	.smmu_need_enable_tbu_ecc = need_enable_tbu_ecc,
	.vdec_init_ecc = NULL,
	.vdec_deinit_ecc = NULL,
};

vdec_plat_ops *get_vdec_plat_ops(void)
{
	return &g_vdec_plat_v750_ops;
}
