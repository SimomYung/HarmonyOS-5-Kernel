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

#ifndef DPU_COMP_DFR_ACPU_DIMMING_H
#define DPU_COMP_DFR_ACPU_DIMMING_H

#include "dpu_comp_dfr.h"

enum dfr_dimming_index {
	DIMMING_SEQ_120HZ_INDEX,
	DIMMING_SEQ_90HZ_INDEX,
	DIMMING_SEQ_MAX_INDEX
};

enum dfr_dimming_state {
	DIMMING_RESET,
	DIMMING_START,
	DIMMING_FINISH
};

struct dimming_status {
	uint32_t dimming_seq_index;
	uint32_t dimming_repeat_cnt;
	uint32_t dimming_te_cnt;
	uint32_t dimming_state;
	uint32_t debug_dimming_enable;
};

struct dfr_dimming_ctrl {
	ktime_t dimming_timestamp;
	struct dimming_node dimming_seq_list[DIMMING_SEQ_LEN_MAX];
	uint32_t dimming_seq_len;
	struct dimming_status status;
	bool need_reset;
};

void dfr_acpu_dimming_reset(struct dpu_comp_dfr_ctrl *dfr_ctrl);
void dfr_acpu_dimming_enter_dimming_process(struct dpu_comp_dfr_ctrl *dfr_ctrl);

#endif