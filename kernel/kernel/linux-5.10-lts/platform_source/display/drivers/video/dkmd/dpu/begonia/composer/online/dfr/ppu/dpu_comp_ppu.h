/**
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
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

#ifndef DPU_COMP_PPU_H
#define DPU_COMP_PPU_H

#include "dkmd_dpu.h"

struct dpu_comp_dfr_ctrl;
struct dpu_comp_ppu_ctrl {
	struct dpu_comp_dfr_ctrl *dfr_ctrl;
	struct panel_refresh_ctrl panel_refresh_ctrl;
	struct dkmd_rect panel_rect;
	uint32_t ppu_ctrl_mode;
	uint32_t panel_refresh_mode;
};

void dpu_ppu_setup_priv_data(struct dpu_comp_ppu_ctrl *ppu_ctrl, struct dpu_comp_dfr_ctrl *dfr_ctrl);
void dpu_ppu_release_priv_data(struct dpu_comp_ppu_ctrl *ppu_ctrl);
void dpu_ppu_process(struct dpu_comp_ppu_ctrl *ppu_ctrl);
uint32_t dpu_ppu_get_te_rate(struct dpu_comp_ppu_ctrl *ppu_ctrl);
uint32_t dpu_ppu_get_te_mask_num(struct dpu_comp_ppu_ctrl *ppu_ctrl);

#endif
