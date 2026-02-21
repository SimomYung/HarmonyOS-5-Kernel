/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2024-2025. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */

#ifndef MALI_KBASE_CONTEXT_SWITCHOVER_H
#define MALI_KBASE_CONTEXT_SWITCHOVER_H

#define KBASE_CTX_SW_TO_FRONT        0
#define KBASE_CTX_SW_TO_BACKGROUND   1
#define KBASE_CTX_SW_INIT_GPU_SIZE   2

#define KBASE_SW_OPER_SUCCESS                  0
#define KBASE_SW_OPER_FAILED                  (-1)
#define KBASE_SW_OPER_FAILED_NEED_CLOSE_CTX   (-2)

#define BATCH_SWAP_SIZE                     (1000)

#include "mali_kbase_defs.h"
#include "mali_kbase_ioctl.h"
struct kbase_mem_context_sw_node {
	pid_t tgid;
	struct list_head node;
	struct kbase_context *ctx;
};
#ifdef CONFIG_MALI_GPU_RECLAIM
long kbase_switch_to_frontground(struct kbase_context * const ctx, pid_t tgid);
long kbase_switch_to_background(struct kbase_context * const ctx,
	struct kbase_ioctl_para_ctx_sw * const para);
void kbase_switch_close_ctx(struct kbase_context * const ctx);
uint32_t kbase_switch_get_ctx_pages(struct kbase_context *ctx);
enum context_swtich_status kbase_switch_get_status(struct kbase_context *ctx);
int kbase_context_switch_init(struct kbase_context *kctx);
void kbase_context_switch_term(struct kbase_context *kctx);
#endif
#endif
