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

#ifndef SDMA_CMD_DATA_H
#define SDMA_CMD_DATA_H

#include <linux/types.h>
#include "dkmd_opr_id.h"
#include "dkmd_user_panel_info.h"

struct dkmd_base_layer;
struct opr_cmd_data;

struct opr_cmd_data *init_sdma_cmd_data(union dkmd_opr_id id);
int32_t opr_set_sdma_data(struct opr_cmd_data *cmd_data, const struct dkmd_base_layer *base_layer,
	const struct opr_cmd_data *pre_cmd_data, const struct opr_cmd_data **next_cmd_datas, uint32_t next_oprs_num);
void opr_set_sdma_layer_fmt(uint32_t fmt, uint32_t compress_type, uint32_t dma_sel,
	struct dpu_dm_layer_info *layer_info);
int32_t get_rect_align_info(struct dkmd_rect_coord *rect, const struct dkmd_base_layer *layer);
void set_layer_clip_info(struct dpu_dm_layer_info *layer_info,
	const struct dkmd_rect_coord *src_rect, const struct dkmd_rect_coord *align_rect);
void opr_set_sdma_config(struct opr_cmd_data *cmd_data);
#endif