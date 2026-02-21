/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2021-2022. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */

#ifndef HVGR_MEM_CONTEXT_SWITCHOVER_H
#define HVGR_MEM_CONTEXT_SWITCHOVER_H

#define HVGR_CTX_SW_TO_FRONT        0
#define HVGR_CTX_SW_TO_BACKGROUND   1
#define HVGR_CTX_SW_INIT_GPU_SIZE   2
#define HVGR_CTX_SW_CLEAR_POOLS     3

#define HVGR_SW_OPER_SUCCESS                  0
#define HVGR_SW_OPER_FAILED                  (-1)
#define HVGR_SW_OPER_FAILED_NEED_CLOSE_CTX   (-2)


#define BATCH_SWAP_SIZE                     (1000U)

#include "hvgr_defs.h"

struct hvgr_mem_context_sw_node {
	pid_t tgid;
	struct list_head node;
	struct hvgr_ctx *ctx;
};

#ifdef CONFIG_LIBLINUX
long hvgr_switch_to_frontground(struct hvgr_ctx * const ctx, pid_t tgid);
long hvgr_switch_to_background(struct hvgr_ctx * const ctx,
	hvgr_ioctl_para_ctx_sw * const para);
void hvgr_switch_close_ctx(struct hvgr_ctx * const sw_ctx);
uint32_t hvgr_switch_get_ctx_pages(struct hvgr_ctx *ctx);
void hvgr_switch_free_swapped_area(struct hvgr_mem_area * const area);
enum context_swtich_status hvgr_switch_get_status(struct hvgr_ctx *ctx);
long hvgr_switch_clear_pools(struct hvgr_ctx * const ctx, pid_t tgid);
#endif
#endif
