/**
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
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

#ifndef COMP_ABNORMAL_HANDLE_H
#define COMP_ABNORMAL_HANDLE_H

#include <linux/types.h>
#include "dkmd_isr.h"
#include "dpu_comp_online.h"

#define INPUT_LAYER_OFFSET          0x80
#define INPUT_LAYER_CNT             12
#define DM_LAYER_HEIGHT_OFFSET      0x1A0

#define COMP_VACTIVE_TIMEOUT_RET_VALUE (-2)
struct dpu_composer;

enum {
	ABNORMAL_UNDERFLOW = BIT(0),
	ABNORMAL_WAIT_VACTIVE_TIMEOUT = BIT(1),
	ABNORMAL_CHECK_LDI_FAILED = BIT(2),
	ABNORMAL_TE_IRQ_REGISTER_FAILED = BIT(3),
};

struct dpu_dsm_dm_dump {
	uint32_t start_addr;
	uint32_t end_addr;
};

void dpu_comp_abnormal_handle_init(struct ukmd_isr *isr, struct dpu_composer *dpu_comp, uint32_t listening_bit);
void dpu_comp_abnormal_handle_deinit(struct ukmd_isr *isr, uint32_t listening_bit);
void dpu_comp_abnormal_debug_dump(struct dpu_composer *dpu_comp, uint32_t scene_id);
void dpu_comp_abnormal_dump_lbuf(char __iomem *dpu_base);
void dpu_comp_abnormal_dump_dbuf(char __iomem *dpu_base, uint32_t scene_id);
void dpu_comp_abnormal_dump_pll(struct dpu_composer *dpu_comp);
void dpu_comp_abnormal_dump_dacc(char __iomem *dpu_base, uint32_t scene_id);
void dpu_comp_abnormal_dump_reg_dm(char __iomem *dpu_base, uint32_t scene_id);
void dpu_comp_abnormal_dvfs(uint32_t comp_index);
void dpu_comp_reset_hardware_manually(struct dpu_composer *dpu_comp, struct comp_online_present *present);
void dpu_abnormal_dump_hardware_cursor(struct dpu_composer *dpu_comp, struct comp_online_present *present);
void dpu_abnormal_dump_tunnel_present_dm(struct dpu_composer *dpu_comp);
void dpu_comp_abnormal_dsm_dump_reg_dm(struct dpu_composer *dpu_comp, uint32_t scene_id);
void dpu_comp_dsm_notify(struct dpu_composer *dpu_comp, uint32_t scene_id);
void dpu_comp_abnormal_dump_sdma_info(char __iomem *module_base);
void dpu_comp_abnormal_dump_ov_info(char __iomem *dpu_base);
void dpu_comp_abnormal_dump_offline(char __iomem *dpu_base);
void dpu_offline_debug_dump(struct dpu_composer *dpu_comp);
void dpu_check_tunnel_state(struct dpu_composer *dpu_comp);
void dpu_check_hardware_cursor_state(struct dpu_composer *dpu_comp);
void dpu_self_healing_init(struct dpu_self_healing_ctrl *self_healing_ctrl);
void dpu_comp_self_healing_process(struct dpu_composer *dpu_comp);
#endif