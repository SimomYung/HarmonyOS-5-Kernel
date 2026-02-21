/**
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#include <linux/slab.h>
#include <dpu/dpu_dm.h>
#include "dpp_cmd_data.h"
#include "../opr_format.h"
#include "config/dpu_config_utils.h"
#include "config/dpu_opr_config.h"
#include "cmdlist_interface.h"
#include "dkmd_log.h"

void set_dpp_sel_info(uint32_t dpp_sel, uint32_t dpp_order, struct dpu_dm_dpp_info *dpp_info)
{
	dpp_info->dpp_sel.reg.dpp_sel = dpp_sel;
	dpp_info->dpp_sel.reg.dpp_order0 = dpp_order;
}

void opr_set_dpp_cfg_data(struct opr_cmd_data_base *data)
{
	DPU_DPP_DITHER_IMG_SIZE_UNION dither_size;
	DPU_DPP_DITHER_CTL0_UNION ctrl0;
	DPU_DPP_DITHER_CTL1_UNION ctrl1;
	uint32_t cmdlist_dev_id = dpu_res_get_cmdlist_dev_id();

	ukmd_set_reg(cmdlist_dev_id, (uint32_t)data->scene_id, data->reg_cmdlist_id,
		DPU_DPP_GMP_EN_ADDR(data->module_offset), 0x2);

	dither_size.value = 0;
	dither_size.reg.dpp_dither_img_width = rect_width(&(data->in_common_data.rect)) - 1;
	dither_size.reg.dpp_dither_img_height = rect_height(&(data->in_common_data.rect)) -1;
	ukmd_set_reg(cmdlist_dev_id, (uint32_t)data->scene_id, data->reg_cmdlist_id,
		DPU_DPP_DITHER_IMG_SIZE_ADDR(data->module_offset), dither_size.value);

	ctrl0.value = 0;
	ctrl0.reg.dither_en = 1;
	ctrl0.reg.dither_hifreq_noise_mode = 1;
	ctrl0.reg.dither_rgb_shift_mode = 1;
	ukmd_set_reg(cmdlist_dev_id, (uint32_t)data->scene_id, data->reg_cmdlist_id,
		DPU_DPP_DITHER_CTL0_ADDR(data->module_offset), ctrl0.value);

	ctrl1.value = 0;
	ctrl1.reg.dither_mode = 1;
	ctrl1.reg.dither_sel = 1;
	ukmd_set_reg(cmdlist_dev_id, (uint32_t)data->scene_id, data->reg_cmdlist_id,
		DPU_DPP_DITHER_CTL1_ADDR(data->module_offset), ctrl1.value);

	ukmd_set_reg(cmdlist_dev_id, (uint32_t)data->scene_id, data->reg_cmdlist_id,
		DPU_DPP_ROIGAMA0_EN_ADDR(data->module_offset), 0);
	ukmd_set_reg(cmdlist_dev_id, (uint32_t)data->scene_id, data->reg_cmdlist_id,
		DPU_DPP_ROIGAMA1_EN_ADDR(data->module_offset), 0);
#ifdef DPU_DPP_NLXCC_EN_ADDR
	ukmd_set_reg(cmdlist_dev_id, (uint32_t)data->scene_id, data->reg_cmdlist_id,
		DPU_DPP_NLXCC_EN_ADDR(data->module_offset), 0);
#endif

	ukmd_set_reg(cmdlist_dev_id, (uint32_t)data->scene_id, data->reg_cmdlist_id,
		DPU_DPP_XCC_EN_ADDR(data->module_offset), 0);
	ukmd_set_reg(cmdlist_dev_id, (uint32_t)data->scene_id, data->reg_cmdlist_id,
		DPU_DPP_CLIP_EN_ADDR(data->module_offset), 0);
	ukmd_set_reg(cmdlist_dev_id, (uint32_t)data->scene_id, data->reg_cmdlist_id,
		DPU_DPP_ALSC_DEGAMMA_EN_ADDR(data->module_offset), 0);
	ukmd_set_reg(cmdlist_dev_id, (uint32_t)data->scene_id, data->reg_cmdlist_id,
		DPU_DPP_ALSC_EN_ADDR(data->module_offset), 0);

	ukmd_set_reg(cmdlist_dev_id, (uint32_t)data->scene_id, data->reg_cmdlist_id,
		DPU_DPP_REG_CTRL_ADDR(data->module_offset), (uint32_t)data->scene_id);
	ukmd_set_reg(cmdlist_dev_id, (uint32_t)data->scene_id, data->reg_cmdlist_id,
		DPU_DPP_REG_CTRL_FLUSH_EN_ADDR(data->module_offset), 1);
}