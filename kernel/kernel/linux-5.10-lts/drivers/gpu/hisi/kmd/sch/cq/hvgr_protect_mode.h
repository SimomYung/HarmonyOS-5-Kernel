/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2021-2022. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */

#ifndef HVGR_PROTECT_MODE_H
#define HVGR_PROTECT_MODE_H

#include <linux/types.h>
#include "hvgr_defs.h"
#include "sch/cq/hvgr_cq_data.h"
#include "sch/cq/driver/hvgr_cq_driver_base.h"
#include "sch/cq/sched/hvgr_cq_schedule.h"
#if hvgr_version_lt(500)
#include "hvgr_cq_swctx.h"
#else
#include "hvgr_cq_swctx_divide.h"
#endif

#if defined(HVGR_FEATURE_PROTECT_MODE)
bool hvgr_get_prot_enhance(struct hvgr_device * const gdev);

#if hvgr_version_lt(300)
bool hvgr_mem_protect_jcd_pool_init(struct hvgr_device * const gdev);

bool hvgr_mem_protect_jcd_pool_term(struct hvgr_device * const gdev);

int hvgr_mem_protect_jcd_init(struct hvgr_ctx * const ctx);

void hvgr_mem_protect_jcd_term(struct hvgr_ctx * const ctx);

irqreturn_t hvgr_job_irq_handler(int irq, void *data);

void hvgr_protect_mode_disable_interrupts(struct hvgr_device *gdev);
#endif

void hvgr_cq_protect_mode_check_running(struct hvgr_cq_ctx * const cq_ctx,
	struct hvgr_device *gdev, uint32_t channel_id, enum protect_flag flag);

void hvgr_cq_schedule_channel_running_proc(struct hvgr_device * const gdev,
	struct hvgr_cq_ctx * const cq_ctx, uint32_t channel_id, enum protect_flag flag,
	bool need_stop);

void hvgr_cq_protect_mode_check_stopped(struct hvgr_cq_ctx * const cq_ctx,
	struct hvgr_device *gdev, uint32_t channel_id);

void hvgr_cq_protect_mode_enter(struct hvgr_cq_ctx * const cq_ctx, struct hvgr_device *gdev,
	uint32_t channel_id);

void hvgr_cq_protect_mode_exit(struct hvgr_cq_ctx * const cq_ctx, struct hvgr_device *gdev,
	uint32_t channel_id);
#else
static inline bool hvgr_get_prot_enhance(struct hvgr_device * const gdev)
{
	return false;
}

static inline bool hvgr_mem_protect_jcd_pool_init(struct hvgr_device * const gdev)
{
	return true;
}

static inline bool hvgr_mem_protect_jcd_pool_term(struct hvgr_device * const gdev)
{
	return true;
}

static inline int hvgr_mem_protect_jcd_init(struct hvgr_ctx * const ctx)
{
	return 0;
}

static inline void hvgr_mem_protect_jcd_term(struct hvgr_ctx * const ctx) {}

static inline irqreturn_t hvgr_job_irq_handler(int irq, void *data)
{
	return IRQ_HANDLED;
}

static inline void hvgr_protect_mode_disable_interrupts(struct hvgr_device *gdev) {}

static inline void hvgr_cq_protect_mode_check_running(struct hvgr_cq_ctx * const cq_ctx,
	struct hvgr_device *gdev, uint32_t channel_id, enum protect_flag flag) {}

static inline void hvgr_cq_schedule_channel_running_proc(struct hvgr_device * const gdev,
	struct hvgr_cq_ctx * const cq_ctx, uint32_t channel_id, enum protect_flag flag,
	bool need_stop)
{
	bool empty_flag = hvgr_cq_get_empty_flag(gdev, channel_id);

	hvgr_info(cq_ctx->ctx->gdev, HVGR_CQ,
		"%s ctx_%u chn=%d, stop=%d getidx=%d,put=%d,empty=%d.\n", __func__,
		cq_ctx->ctx->id, channel_id, need_stop, hvgr_get_queue_get_index(cq_ctx),
		hvgr_get_queue_put_index(cq_ctx), empty_flag);
	if (hvgr_cq_is_need_unbind(empty_flag, cq_ctx) || need_stop)
		hvgr_cq_unbind_channel(cq_ctx, channel_id, CQ_NEED_UNBIND_TIMER_FLAG);
}

static inline void hvgr_cq_protect_mode_check_stopped(struct hvgr_cq_ctx * const cq_ctx,
	struct hvgr_device *gdev, uint32_t channel_id) {}

static inline void hvgr_cq_protect_mode_enter(struct hvgr_cq_ctx * const cq_ctx,
	struct hvgr_device *gdev, uint32_t channel_id) {}

static inline void hvgr_cq_protect_mode_exit(struct hvgr_cq_ctx * const cq_ctx,
	struct hvgr_device *gdev, uint32_t channel_id) {}
#endif
#endif
