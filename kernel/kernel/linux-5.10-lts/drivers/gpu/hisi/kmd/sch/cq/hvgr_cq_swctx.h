/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2021-2022. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */

#ifndef HVGR_CQ_SWCTX_H
#define HVGR_CQ_SWCTX_H

#include "hvgr_defs.h"
#include "hvgr_cq_data.h"

static inline uint32_t hvgr_get_queue_put_index(struct hvgr_cq_ctx *cq_ctx)
{
	return cq_ctx->sw_wr_ctx->queue_put_index;
}

static inline uint32_t hvgr_get_queue_get_index(struct hvgr_cq_ctx *cq_ctx)
{
	return cq_ctx->sw_wr_ctx->queue_get_index;
}

static inline void hvgr_set_queue_get_index(struct hvgr_cq_ctx *cq_ctx, uint32_t get_index)
{
	cq_ctx->sw_wr_ctx->queue_get_index = get_index;
}

#if hvgr_version_lt(300)
#else
static inline uint32_t hvgr_get_entry_done(struct hvgr_cq_ctx *cq_ctx, uint32_t index)
{
	return cq_ctx->sw_wr_ctx->entry_done[index];
}

static inline void hvgr_set_entry_done(struct hvgr_cq_ctx *cq_ctx, uint32_t index, uint32_t val)
{
	cq_ctx->sw_wr_ctx->entry_done[index] = val;
}
#endif
#endif
