/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2021-2022. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */
#include "hvgr_sch_dfx.h"
#include "hvgr_log_api.h"
#include "hvgr_cq_schedule.h"
#include "hvgr_pm_api.h"
#include "hvgr_cq_driver_adapt.h"
#include "hvgr_cq_driver_base.h"
#include "hvgr_cq_data.h"
#if hvgr_version_lt(HVGR_V500)
#include "hvgr_cq_swctx.h"
#else
#include "hvgr_cq_swctx_divide.h"
#endif
#include <linux/slab.h>
#include <securec.h>

static void hvgr_cq_print_cq_status(struct hvgr_ctx * const ctx,
	struct hvgr_cq_ctx * const cq_ctx)
{
	struct hvgr_cq_dev *gcqctx = NULL;
	struct hvgr_device *gdev = NULL;
	bool running = false;
	uint32_t bind_status;
	uint32_t queue_status;
	unsigned long flags;

	gdev = cq_ctx->ctx->gdev;
	gcqctx = &gdev->cq_dev;
	spin_lock_irqsave(&gcqctx->schedule_lock, flags);
	running = hvgr_cq_is_queue_running(gcqctx, cq_ctx);
	if (running) {
		bind_status = hvgr_get_bind_status(gdev, cq_ctx->channel_id);
		queue_status = hvgr_cq_get_queue_status(gdev, cq_ctx->channel_id);
	}
	spin_unlock_irqrestore(&gcqctx->schedule_lock, flags);
	if (!running) {
		hvgr_debug(gdev, HVGR_CQ,
			"ctx_%u, queueid_%u is not running, tgid=%d pid=%d cq_status=%d priority=%d.",
			ctx->id, cq_ctx->queue_id, ctx->tgid, ctx->pid, cq_ctx->status, cq_ctx->priority);
	} else {
		hvgr_debug(gdev, HVGR_CQ,
			"ctx_%u, queueid_%u tgid=%d pid=%d chn=%d bind_status=0x%x queue_status=0x%x"
			"priority=%d.",
			cq_ctx->ctx->id, cq_ctx->queue_id, ctx->tgid, ctx->pid, cq_ctx->channel_id, bind_status,
			queue_status, cq_ctx->priority);
	}
}

static void hvgr_cq_print_cq_status_nolock(struct hvgr_ctx * const ctx,
	struct hvgr_cq_ctx * const cq_ctx)
{
	struct hvgr_cq_dev *gcqctx = NULL;
	struct hvgr_device *gdev = NULL;
	bool running = false;
	uint32_t bind_status;
	uint32_t queue_status;

	gdev = cq_ctx->ctx->gdev;
	gcqctx = &gdev->cq_dev;
	running = hvgr_cq_is_queue_running(gcqctx, cq_ctx);
	if (running) {
		bind_status = hvgr_get_bind_status(gdev, cq_ctx->channel_id);
		queue_status = hvgr_cq_get_queue_status(gdev, cq_ctx->channel_id);
		hvgr_debug(gdev, HVGR_CQ,
			"ctx_%u, queueid_%u tgid=%d pid=%d chn=%d bind_status=0x%x queue_status=0x%x"
			"priority=%d.",
			cq_ctx->ctx->id, cq_ctx->queue_id, ctx->tgid, ctx->pid, cq_ctx->channel_id, bind_status,
			queue_status, cq_ctx->priority);
	} else {
		hvgr_debug(gdev, HVGR_CQ,
			"ctx_%u, queueid_%u is not running, tgid=%d pid=%d cq_status=%d priority=%d.",
			ctx->id, cq_ctx->queue_id, ctx->tgid, ctx->pid, cq_ctx->status, cq_ctx->priority);
	}
}

static void hvgr_cq_print_running_cq_status(struct hvgr_ctx * const ctx,
	struct hvgr_cq_ctx * const cq_ctx)
{
	struct hvgr_device *gdev = NULL;
	uint32_t bind_status;
	uint32_t queue_status;

	gdev = cq_ctx->ctx->gdev;
	bind_status = hvgr_get_bind_status(gdev, cq_ctx->channel_id);
	queue_status = hvgr_cq_get_queue_status(gdev, cq_ctx->channel_id);
	hvgr_debug(gdev, HVGR_CQ,
		"ctx_%u, queueid_%u tgid=%d pid=%d chn=%d bind_status=0x%x queue_status=0x%x priority=%d.",
		cq_ctx->ctx->id, cq_ctx->queue_id, ctx->tgid, ctx->pid, cq_ctx->channel_id, bind_status,
		queue_status, cq_ctx->priority);
}

static void hvgr_cq_print_cq_status_by_ctx(struct hvgr_ctx * const ctx)
{
	uint32_t idx;
	struct hvgr_cq_ctx *cq_ctx = NULL;

	for (idx = 0; idx < ARRAY_SIZE(ctx->ctx_cq.cq_ctx); idx++) {
		if (ctx->ctx_cq.cq_ctx[idx].sw_wr_ctx != NULL) {
			cq_ctx = &ctx->ctx_cq.cq_ctx[idx];
			hvgr_cq_print_cq_status(ctx, cq_ctx);
		}
	}
}

static void hvgr_cq_print_cq_status_by_ctx_nolock(struct hvgr_ctx * const ctx)
{
	uint32_t idx;
	struct hvgr_cq_ctx *cq_ctx = NULL;

	for (idx = 0; idx < ARRAY_SIZE(ctx->ctx_cq.cq_ctx); idx++) {
		if (ctx->ctx_cq.cq_ctx[idx].sw_wr_ctx != NULL) {
			cq_ctx = &ctx->ctx_cq.cq_ctx[idx];
			hvgr_cq_print_cq_status_nolock(ctx, cq_ctx);
		}
	}
}

static void hvgr_cq_print_token_callback(struct hvgr_ctx * const ctx)
{
	struct hvgr_cq_ctx_data *ctx_cq = NULL;
	struct token_callback_data *callback = NULL;
	char *buffer = NULL;
	int wrlen = 0;
	unsigned int idx;
	unsigned long flags;
	int ret;

	ctx_cq = &ctx->ctx_cq;
	buffer = kcalloc(CQ_DFX_INFO_MAX_LEN, sizeof(*buffer), GFP_ATOMIC);
	if (unlikely(buffer == NULL)) {
		hvgr_debug(ctx->gdev, HVGR_CQ, "buffer alloc failed!");
		return;
	}

	for (idx = 1; idx < CQ_TOKEN_NUMS; idx++) {
		spin_lock_irqsave(&ctx_cq->callback_data_lock[idx], flags);
		callback = ctx_cq->token_callback[idx];
		if (callback == NULL || callback->ctx == NULL) {
			spin_unlock_irqrestore(&ctx_cq->callback_data_lock[idx], flags);
			continue;
		}

		if (((callback->flags & HVGR_TOKEN_CB_WAIT_FENCE) == 0) &&
			((callback->flags & HVGR_TOKEN_CB_TRIGGER_FENCE) == 0) &&
			(callback->dep_own_nums == 0)) {
			spin_unlock_irqrestore(&ctx_cq->callback_data_lock[idx], flags);
			continue;
		}

		if (callback->fence_data != NULL)
			hvgr_snprintf_s(ret, buffer, CQ_DFX_INFO_MAX_LEN, wrlen,
				"ctx_%u token_%u flag=0x%x seqno=%llu dep_own_nums=%d",
				ctx->id, callback->cq_event.token, callback->flags, callback->fence_data->seqno,
				callback->dep_own_nums);
		else
			hvgr_snprintf_s(ret, buffer, CQ_DFX_INFO_MAX_LEN, wrlen,
				"ctx_%u token_%u flag=0x%x dep_own_nums=%d",
				ctx->id, callback->cq_event.token, callback->flags, callback->dep_own_nums);

		if (callback->dep_own_nums > 0)
			hvgr_snprintf_s(ret, buffer, CQ_DFX_INFO_MAX_LEN, wrlen, " %u",
				callback->dep_own_token[0]);
		spin_unlock_irqrestore(&ctx_cq->callback_data_lock[idx], flags);
		hvgr_debug(ctx->gdev, HVGR_CQ, "%s", buffer);

		memset_s(buffer, CQ_DFX_INFO_MAX_LEN, 0, CQ_DFX_INFO_MAX_LEN);
		wrlen = 0;
	}

	kfree(buffer);
	hvgr_debug(ctx->gdev, HVGR_CQ, "ctx_%u, token_process_step=%d",
		ctx->id, ctx->token_process_step);
}

static void hvgr_cq_print_sch_info(struct hvgr_ctx * const ctx)
{
	struct hvgr_cq_dev *gcqctx = NULL;
	char *buffer = NULL;
	uint64_t now_time = (uint64_t)ktime_to_ns(ktime_get());
	uint32_t idx;
	int wrlen = 0;
	int ret;

	gcqctx = &ctx->gdev->cq_dev;
	buffer = kcalloc(CQ_DFX_INFO_MAX_LEN, sizeof(*buffer), GFP_ATOMIC);
	if (unlikely(buffer == NULL)) {
		hvgr_debug(ctx->gdev, HVGR_CQ, "buffer alloc failed!");
		return;
	}

	hvgr_snprintf_s(ret, buffer, CQ_DFX_INFO_MAX_LEN, wrlen, "last enqueue pending time:");
	for (idx = 0; idx < CQ_PRIORITY_MAX; idx++) {
		if (gcqctx->last_sch_time[idx] == 0)
			hvgr_snprintf_s(ret, buffer, CQ_DFX_INFO_MAX_LEN, wrlen, " 0");
		else
			hvgr_snprintf_s(ret, buffer, CQ_DFX_INFO_MAX_LEN, wrlen, " %llu",
				(now_time - gcqctx->last_sch_time[idx]));
	}
	hvgr_debug(ctx->gdev, HVGR_CQ, "%s", buffer);
	kfree(buffer);
}

static void hvgr_cq_print_irq_infos(struct hvgr_ctx * const ctx, struct hvgr_cq_ctx * const cq_ctx)
{
	uint32_t idx;
	for (idx = 0; idx < CQ_IRQ_INFO_MAX; idx++)
		hvgr_debug(ctx->gdev, HVGR_CQ, "ctx_%u queueid_%u idx_%u: token=%u seqno=%u type=%d",
			ctx->id, cq_ctx->queue_id, idx, cq_ctx->irq_infos[idx].token,
			cq_ctx->irq_infos[idx].seqno, cq_ctx->irq_infos[idx].type);
}

static void hvgr_cq_print_irq_infos_by_ctx(struct hvgr_ctx * const ctx)
{
	uint32_t idx;
	struct hvgr_cq_ctx *cq_ctx = NULL;

	for (idx = 0; idx < ARRAY_SIZE(ctx->ctx_cq.cq_ctx); idx++) {
		if (ctx->ctx_cq.cq_ctx[idx].sw_wr_ctx != NULL) {
			cq_ctx = &ctx->ctx_cq.cq_ctx[idx];
			hvgr_cq_print_irq_infos(ctx, cq_ctx);
		}
	}
}

static void hvgr_cq_print_running_queue_info(struct hvgr_device *gdev)
{
	unsigned long flags;
	int idx;
	struct hvgr_cq_ctx *cq_ctx = NULL;
	struct hvgr_cq_dev *gcqctx = NULL;

	gcqctx = &gdev->cq_dev;
	hvgr_debug(gdev, HVGR_CQ, "start print running queue info.");
	spin_lock_irqsave(&gcqctx->schedule_lock, flags);
	for (idx = 0; idx < CQ_CHANNEL_NUM; idx++) {
		if (gcqctx->channel_info[idx].running_queue != NULL) {
			cq_ctx = gcqctx->channel_info[idx].running_queue;
			hvgr_cq_print_running_cq_status(cq_ctx->ctx, cq_ctx);
			hvgr_cq_print_token_callback(cq_ctx->ctx);
			hvgr_cq_print_irq_infos(cq_ctx->ctx, cq_ctx);
		}
	}
	spin_unlock_irqrestore(&gcqctx->schedule_lock, flags);
	hvgr_debug(gdev, HVGR_CQ, "end print running queue info.");
}

void hvgr_cq_print_cq_info_by_ctx(struct hvgr_ctx * const ctx)
{
	hvgr_cq_print_cq_status_by_ctx(ctx);
	hvgr_cq_print_token_callback(ctx);
	hvgr_cq_print_irq_infos_by_ctx(ctx);
	hvgr_cq_print_sch_info(ctx);
	hvgr_cq_print_running_queue_info(ctx->gdev);
}

void hvgr_cq_fault_print_cq_info(struct hvgr_ctx * const ctx)
{
	hvgr_cq_print_cq_status_by_ctx_nolock(ctx);
	hvgr_cq_print_irq_infos_by_ctx(ctx);
	hvgr_cq_print_sch_info(ctx);
}

void hvgr_cq_record_irq_info(struct hvgr_cq_ctx * const cq_ctx,
	uint32_t token, enum hvgr_cq_irq_type type)
{
	struct hvgr_cq_dev *gcqctx = NULL;
	uint32_t irq_info_index = cq_ctx->irq_info_index;

	if (irq_info_index >= CQ_IRQ_INFO_MAX)
		irq_info_index = 0;
	gcqctx = &cq_ctx->ctx->gdev->cq_dev;
	cq_ctx->irq_infos[irq_info_index].token = token;
	cq_ctx->irq_infos[irq_info_index].seqno = gcqctx->cur_seqno;
	cq_ctx->irq_infos[irq_info_index].type = type;
	cq_ctx->irq_info_index = (irq_info_index + 1) % CQ_IRQ_INFO_MAX;
	gcqctx->cur_seqno += 1;
}

long hvgr_ctx_ioctl_record_init(struct hvgr_ctx * const ctx)
{
	struct hvgr_ioctl_call_record *record = NULL;

	record = vmalloc(sizeof(*record));
	if (record == NULL) {
		hvgr_err(ctx->gdev, HVGR_CQ, "%s vmalloc fail", __func__);
		return -ENOMEM;
	}

	(void)memset_s(record, sizeof(*record), 0, sizeof(*record));
	ctx->ctx_cq.ioctl_record = record;
	return 0;
}

void hvgr_ctx_ioctl_record_term(struct hvgr_ctx * const ctx)
{
	if (ctx->ctx_cq.ioctl_record == NULL)
		return;
	vfree(ctx->ctx_cq.ioctl_record);
	ctx->ctx_cq.ioctl_record = NULL;
}

void hvgr_ctx_ioctl_record(struct hvgr_ctx * const ctx, uint64_t time, uint32_t cmd, uint32_t para)
{
	struct hvgr_ioctl_call_record *record = ctx->ctx_cq.ioctl_record;

	if (record == NULL) {
		hvgr_err(ctx->gdev, HVGR_CQ, "%s record is null", __func__);
		return;
	}

	record->time[record->pos] = time;
	record->cmd[record->pos] = cmd;
	record->para[record->pos] = para;
	record->pos = (++record->pos) == HVGR_IOCTL_CALL_RECORD_MAX ? 0 : record->pos;
}

void hvgr_ctx_ioctl_record_print(struct hvgr_ctx * const ctx)
{
	uint32_t i;
	struct hvgr_ioctl_call_record *record = ctx->ctx_cq.ioctl_record;

	if (record == NULL || record->pos == HVGR_IOCTL_CALL_RECORD_MAX) {
		hvgr_err(ctx->gdev, HVGR_CQ, "%s fail.", __func__);
		return;
	}

	for (i = record->pos; i < HVGR_IOCTL_CALL_RECORD_MAX; i++)
		hvgr_err(ctx->gdev, HVGR_CQ, "%s ctx_%u, pos=%u, time=%u,cmd=%lx,para=%u", __func__,
			ctx->id, i, record->time[i], record->cmd[i], record->para[i]);
	
	for (i = 0; i < record->pos; i++)
		hvgr_err(ctx->gdev, HVGR_CQ, "%s ctx_%u, pos=%u, time=%u,cmd=%lx,para=%u", __func__,
			ctx->id, i, record->time[i], record->cmd[i], record->para[i]);
}
