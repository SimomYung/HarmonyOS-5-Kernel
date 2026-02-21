/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2024-2024. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */

#ifndef HVGR_DM_DFX_H
#define HVGR_DM_DFX_H

#include <linux/types.h>
#include <linux/workqueue.h>
#include <linux/spinlock.h>

#include "hvgr_defs.h"

struct hvgr_job_fault_data {
	uint32_t fault_code;
	uint32_t channel_id;
	uint64_t address;
};

struct hvgr_l2_fault_data {
	uint32_t source_id;
	uint32_t stream_id;
	uint64_t pa;
};

void hvgr_dfx_init(struct hvgr_device * const gdev);
void hvgr_dfx_term(struct hvgr_device * const gdev);
void hvgr_gpu_fault_show_process(struct hvgr_device *gdev,
	struct hvgr_job_fault_data *data, char *process_info, uint32_t info_len);
#endif
