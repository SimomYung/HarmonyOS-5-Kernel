/**
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
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
#ifndef LBUF_MGR_V8XX_H
#define LBUF_MGR_V8XX_H

#include "lbuf_mgr.h"
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/uaccess.h>
#include "res_mgr.h"
#include "dkmd_opr_id.h"
#include "dkmd_log.h"

struct used_node_info {
	struct list_head list_node;
	int32_t user_type;
	int32_t scene_id;
	int32_t user_tgid;
	uint32_t used_lb;
};

struct reserved_node_info {
	struct list_head list_node;
	int32_t user_type;
	int32_t scene_id;
	int32_t user_tgid;
	uint32_t reserved_lb; // current lb which has been reserved
	uint32_t reserve_lb; // total lb which need to be reserved
};

struct lbuf_node_info {
	struct semaphore sem;
	uint32_t part_id;
	uint32_t used_lb;
	uint32_t max_lb;
	struct list_head used_node_head;
	struct list_head reserved_node_head;
};

struct dpu_lbuf_mgr {
	uint32_t part_num;
	struct lbuf_node_info *lb_part_info;
	struct semaphore sem;
	bool has_turn_to_client;
};

void *lbuf_mgr_init(struct dpu_res_data *rm_data);
void lbuf_mgr_deinit(void *data);
void lbuf_mgr_reset(void *data);
void lbuf_mgr_release(void *data);
int32_t lbuf_mgr_handle_lb_opt(struct dpu_lbuf_mgr *lbuf_mgr, struct req_lbuf_info *lb_info);
#endif