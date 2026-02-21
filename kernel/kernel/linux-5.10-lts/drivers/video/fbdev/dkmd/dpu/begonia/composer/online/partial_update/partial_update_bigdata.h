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
#define MULTI_DIRTY_COMBINATION_SIZE 15
#define MULTI_DIRTY_AREA_BUF_SIZE 320

struct partial_update_bigdata {
	uint64_t frame_count;
	struct dkmd_rect dirty_rect;
	uint32_t area_count[AREA_INTERVAL_SIZE];
	uint32_t forbid_reasons[FORBID_PARTIAL_REASON_SIZE];
	struct mutex lock;
};

struct multi_dirty_common_data {
	uint32_t xres;
	uint32_t yres;
	int32_t panel_id;
	int32_t dirty_size;
};

struct partial_update_multi_dirty_bigdata {
	uint64_t frame_count;
	uint32_t area_count[MULTI_DIRTY_COMBINATION_SIZE][AREA_INTERVAL_SIZE];
    char area_buf[MULTI_DIRTY_COMBINATION_SIZE][MULTI_DIRTY_AREA_BUF_SIZE];
	struct mutex lock;
};

struct partial_update_bigdata_work {
	struct kthread_work kwork;
	struct multi_dirty_common_data common_data;
	uint8_t dirty_region_updt_support;
	uint64_t forbid_partial_update_reason;
	struct dkmd_rect multi_dirty_rects[DISP_RECT_MAX_DIRTY_NUM];
	bool multi_dirty_is_init;
	struct mutex lock;
	struct kthread_worker bigdata_worker_queue;
	struct task_struct *bigdata_task_thread;
};

struct partial_update_rect_distance {
	int32_t seq_id;
	int32_t distance;
};

struct multi_dirty_cal_param {
	struct multi_dirty_common_data common_data;
	int32_t div_num;
	uint32_t fullscreen_area;
	int32_t support_dirty_count;
	int32_t div;
    struct dkmd_rect need_cal_dirty_rects[DISP_RECT_MAX_DIRTY_NUM];
};

int32_t partial_update_bigdata_update(struct dpu_composer *dpu_comp, struct disp_frame *frame);
void partial_update_bigdata_setup(struct dpu_composer *dpu_comp);
void partial_update_bigdata_func(struct kthread_work *work);
void partial_update_bigdata_thread_init(struct dpu_composer *dpu_comp);
void partial_update_bigdata_thread_deinit(struct dpu_composer *dpu_comp);
void setup_multi_dirty_bigdata_update(struct dpu_composer *dpu_comp, struct disp_frame *frame);

#endif