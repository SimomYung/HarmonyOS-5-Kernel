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

#ifndef PARTIAL_UPDATE_BIGDATA_H
#define PARTIAL_UPDATE_BIGDATA_H

#include "dkmd_object.h"
#include "dpu_comp_mgr.h"

#define AREA_INTERVAL_SIZE 25
#define FORBID_PARTIAL_REASON_SIZE 32
#define FORBID_PARTIAL_DISENABLE_INDEX 16

struct partial_update_bigdata {
	uint64_t frame_count;
	struct dkmd_rect dirty_rect;
	uint32_t area_count[AREA_INTERVAL_SIZE];
	uint32_t forbid_reasons[FORBID_PARTIAL_REASON_SIZE];
	struct mutex lock;
};

int32_t partial_update_bigdata_update(struct dpu_composer *dpu_comp, struct disp_frame *frame);
void partial_update_bigdata_setup(struct dpu_composer *dpu_comp);

#endif