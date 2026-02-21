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
#include <dpu_format.h>
#include "sdma_cmd_data.h"
#include "../opr_cmd_data.h"
#include "../opr_format.h"
#include "dkmd_log.h"
#include "cmdlist_interface.h"

#define HEBC_YUV_Y10_PER_BLOCK_WIDTH 32
#define HEBC_YUV_Y10_PER_BLOCK_HEIGHT 8
#define HEBC_YUV_Y8_PER_BLOCK_WIDTH 64
#define HEBC_YUV_Y8_PER_BLOCK_HEIGHT 8
#define HEBC_RGB_PER_BLOCK_WIDTH_ROTATE 16
#define HEBC_RGB_PER_BLOCK_HEIGHT_ROTATE 8
#define HEBC_RGB_PER_BLOCK_WIDTH 32
#define HEBC_RGB_PER_BLOCK_HEIGHT 4

#ifndef align_up
#define align_up(val, al) ((((val) + ((al) - 1)) / (al)) * (al))
#endif

#ifndef align_down
#define align_down(val, al)  (((val) / (al)) * (al))
#endif

void opr_set_sdma_layer_fmt(uint32_t fmt, uint32_t compress_type, uint32_t dma_sel,
	struct dpu_dm_layer_info *layer_info)
{
	layer_info->layer_dma_sel.reg.layer_dma_fmt = (uint32_t)dpu_fmt_to_sdma((int32_t)fmt);
	layer_info->layer_dma_sel.reg.layer_fbc_type = compress_type;

	layer_info->layer_ov_dfc_cfg.reg.layer_ov_fmt = (uint32_t)dpu_fmt_2_dfc_pixel_fmt((int32_t)fmt);
	dpu_pr_info("format=%d compress_type=%d", fmt, compress_type);
}

static void calc_rect_align(struct dkmd_rect_coord *rect,
	uint32_t pixel_width_align, uint32_t pixel_height_align)
{
	rect->left = align_down(rect->left, pixel_width_align);
	rect->right = align_up(rect->right, pixel_width_align);
	rect->top = align_down(rect->top, pixel_height_align);
	rect->bottom = align_up(rect->bottom, pixel_height_align);
}

static int32_t get_hebc_align_info(const struct dkmd_base_layer *layer, uint32_t *pixel_width_align,
	uint32_t *pixel_height_align, uint32_t *block_width, uint32_t *block_height)
{
	int32_t format = layer->format;
	if (is_yuv_semiplanar(format)) {
		if (format == DPU_FMT_YCRCB420_SP_10BIT || format == DPU_FMT_YCBCR420_SP_10BIT) {
			*block_width = HEBC_YUV_Y10_PER_BLOCK_WIDTH;
			*block_height = HEBC_YUV_Y10_PER_BLOCK_HEIGHT;
			*pixel_width_align = *block_width * 2;
			*pixel_height_align = *block_height * 2;
		} else if (format == DPU_FMT_YCBCR_420_SP || format == DPU_FMT_YCRCB_420_SP) {
			*block_width = HEBC_YUV_Y8_PER_BLOCK_WIDTH;
			*block_height = HEBC_YUV_Y8_PER_BLOCK_HEIGHT;
			*pixel_width_align = *block_width;
			*pixel_height_align = *block_height * 2;
		} else {
			dpu_pr_err("Not supported dpu format %d!", format);
			return -1;
		}

		if ((layer->transform & TRANSFORM_ROT_90) != 0) {
			*pixel_width_align = *block_width;
			*pixel_height_align = 64;
			dpu_pr_info("rotation yuv sp height align 64");
		}

		dpu_pr_info("YUV SP Hebcd Align Info: block_width=%u block_height=%u", *block_width, *block_height);
		return 0;
	}

	if ((layer->transform & TRANSFORM_ROT_90) != 0) {
		*block_width = HEBC_RGB_PER_BLOCK_WIDTH_ROTATE;
		*block_height = HEBC_RGB_PER_BLOCK_HEIGHT_ROTATE;
		*pixel_height_align = (is_16bpp_rgb(format)) ? (*block_height * 4) : (*block_height * 2);
	} else {
		*block_width = HEBC_RGB_PER_BLOCK_WIDTH;
		*block_height = HEBC_RGB_PER_BLOCK_HEIGHT;
		*pixel_height_align = *block_height;
	}

	*pixel_width_align = *block_width;

	dpu_pr_info("Hebcd Align Info: block_width=%u block_height=%u", *block_width, *block_height);
	return 0;
}

static int32_t get_linear_align_info(const struct dkmd_base_layer *layer, uint32_t *pixel_width_align,
	uint32_t *pixel_height_align, uint32_t *block_width, uint32_t *block_height)
{
	int32_t format = layer->format;

	*block_width = 1;
	*block_height = 1;
	*pixel_height_align = (is_yuv420_planar(format) || is_yuv420_semiplanar(format)) ? 2 : 1;

	int bpp = get_bpp_by_dpu_format(format);
	if (bpp <= 0) {
		dpu_pr_err("layer format=%d get bpp fail", format);
		return -1;
	}

	// dma requires 128 bits read align
	*pixel_width_align = DMA_ALIGN_BYTES / (uint32_t)(bpp);
	if (is_10bit_yuv_planar(format)) {
		*pixel_width_align *= 4;
	} else if (is_yuv420_planar(format) || is_yuv422_planar(format) ||
			   is_10bit_yuv_semiplanar(format) || is_10bit_yuv422_package(format)) {
		*pixel_width_align *= 2;
	}
	return 0;
}

int32_t get_rect_align_info(struct dkmd_rect_coord *rect, const struct dkmd_base_layer *layer)
{
	uint32_t pixel_width_align = 0;
	uint32_t pixel_height_align = 0;
	uint32_t block_width = 0;
	uint32_t block_height = 0;
	int32_t ret = 0;

	if (layer->compress_type != 0) {
		ret = get_hebc_align_info(layer, &pixel_width_align, &pixel_height_align,
			&block_width, &block_height);
	} else {
		ret = get_linear_align_info(layer, &pixel_width_align, &pixel_height_align,
			&block_width, &block_height);
	}

	if (ret != 0) {
		dpu_pr_err("get align info fail");
		return -1;
	}

	calc_rect_align(rect, pixel_width_align, pixel_height_align);

	return 0;
}

void set_layer_clip_info(struct dpu_dm_layer_info *layer_info,
	const struct dkmd_rect_coord *src_rect, const struct dkmd_rect_coord *align_rect)
{
	struct dkmd_rect_coord clip_rect;
	uint32_t layer_width = rect_width(src_rect);
	uint32_t layer_height = rect_height(src_rect);
	uint32_t align_width = rect_width(align_rect);
	uint32_t align_height = rect_height(align_rect);

	clip_rect.left = src_rect->left - align_rect->left;
	clip_rect.top = src_rect->top - align_rect->top;

	clip_rect.right = (align_width - layer_width) - clip_rect.left;
	clip_rect.bottom = (align_height - layer_height) - clip_rect.top;

	layer_info->layer_bot_crop.reg.layer_top_crop = clip_rect.top;
	layer_info->layer_bot_crop.reg.layer_bot_crop = clip_rect.bottom;
	layer_info->layer_bot_crop.reg.layer_clip_right = clip_rect.right;

	layer_info->layer_clip_left.reg.layer_clip_left = clip_rect.left;
}

void opr_set_sdma_config(struct opr_cmd_data *cmd_data)
{
	uint32_t cmdlist_dev_id = dpu_res_get_cmdlist_dev_id();
	struct opr_cmd_data_base *data_base = cmd_data->data;
 
	dpu_assert(!data_base);
	ukmd_set_reg(cmdlist_dev_id, (uint32_t)data_base->scene_id, data_base->reg_cmdlist_id,
		DPU_GLB_SDMA_CTRL0_ADDR(DPU_GLB0_OFFSET, 0), 0x1371);
	ukmd_set_reg(cmdlist_dev_id, (uint32_t)data_base->scene_id, data_base->reg_cmdlist_id,
		DPU_GLB_SDMA_CTRL0_ADDR(DPU_GLB0_OFFSET, 1), 0x1371);
	ukmd_set_reg(cmdlist_dev_id, (uint32_t)data_base->scene_id, data_base->reg_cmdlist_id,
		DPU_DBCU_MIF_CTRL_SMARTDMA_0_ADDR(DPU_DBCU_OFFSET), 0x1);
	ukmd_set_reg(cmdlist_dev_id, (uint32_t)data_base->scene_id, data_base->reg_cmdlist_id,
		DPU_DBCU_MIF_CTRL_SMARTDMA_0_ADDR(DPU_DBCU_OFFSET) + 1 * 0x10, 0x1);
 
	ukmd_set_reg(cmdlist_dev_id, (uint32_t)data_base->scene_id, data_base->reg_cmdlist_id,
		DPU_DBCU_SMARTDMA_0_AXI_SEL_ADDR(DPU_DBCU_OFFSET), 0);
	ukmd_set_reg(cmdlist_dev_id, (uint32_t)data_base->scene_id, data_base->reg_cmdlist_id,
		DPU_DBCU_SMARTDMA_0_AXI_SEL_ADDR(DPU_DBCU_OFFSET) + 1 * 0x4, 2);
}