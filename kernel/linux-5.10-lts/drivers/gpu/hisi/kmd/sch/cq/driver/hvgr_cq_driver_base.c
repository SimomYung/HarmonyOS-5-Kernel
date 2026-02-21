/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2021-2022. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */

#include "hvgr_cq_driver_base.h"

#include <linux/kernel.h>
#include <linux/delay.h>

#include "hvgr_regmap.h"
#include "hvgr_dm_api.h"
#include "hvgr_log_api.h"
#include "hvgr_pm_api.h"
#include "hvgr_datan_api.h"
#include "hvgr_hts_api.h"

#include "hvgr_cq.h"
#include "hvgr_cq_schedule.h"
#include "hvgr_cq_debug_switch.h"
#include "hvgr_cq_driver_adapt.h"


void hvgr_cq_int_operate(struct hvgr_device * const gdev, uint32_t irq_mask)
{
	uint32_t chnid;

	for (chnid = 0; chnid < CQ_CHANNEL_NUM; chnid++) {
		hvgr_fast_pwr_write_reg(gdev,
			cq_control_reg(gdev, cq_irq_clear(chnid)), 0xFFFFFFFFU);
		hvgr_fast_pwr_write_reg(gdev,
			cq_control_reg(gdev, cq_irq_mask(chnid)), irq_mask);
	}
}

bool hvgr_is_cl_task(struct hvgr_device *gdev, uint32_t channel_id)
{
	uint32_t subchn_os = hvgr_read_reg(gdev,
		cq_control_reg(gdev, cq_bind_subchn_os(channel_id)));

	hvgr_info(gdev, HVGR_CQ, "%s channel_id(%d) cq_bind_subchn_os(0x%x)\n",
		__func__, channel_id, subchn_os);
	return (subchn_os & (1U << 2)) != 0;
}

void hvgr_cq_bind_physical_chan(struct hvgr_cq_ctx * const cq_ctx, uint32_t channel_id)
{
	struct hvgr_ctx *ctx = cq_ctx->ctx;
	struct hvgr_device * const gdev = ctx->gdev;

	hvgr_hts_set_ssid(ctx, channel_id);
	hvgr_write_reg(gdev, cq_control_reg(gdev, cq_bind_asid(channel_id)), ctx->asid);
	hvgr_write_reg(gdev, cq_control_reg(gdev, cq_bind_addr_hi(channel_id)),
		(cq_ctx->gpu_context_buffer->uva >> 32));
	hvgr_write_reg(gdev, cq_control_reg(gdev, cq_bind_addr_lo(channel_id)),
		(cq_ctx->gpu_context_buffer->uva & 0xFFFFFFFFU));
	hvgr_write_reg(gdev, cq_control_reg(gdev, cq_bind_jd_msb(channel_id)),
		ctx->same_va_4g_base_page);
	hvgr_write_reg(gdev, cq_control_reg(gdev, cq_bind_queue_id(channel_id)),
		cq_ctx->queue_id);
	hvgr_write_reg(gdev, cq_control_reg(gdev, cq_bind_cmd(channel_id)),
		CQ_CMD_START);

	hvgr_info(gdev, HVGR_CQ, "%s ctx_%u channel_id(%d) gpu_context_buffer->uva 0x%lx same_va_4g_base_page 0x%lx\n",
		__func__, ctx->id, channel_id, cq_ctx->gpu_context_buffer->uva, ctx->same_va_4g_base_page);

	hvgr_datan_jobtrace(ctx, JOBTRACE_TYPE_OBJ, KDATAN_TL_WRITE_REG, ctx,
		datan_cq_control_reg(gdev, cq_bind_asid(channel_id)), ctx->asid);
	hvgr_datan_jobtrace(ctx, JOBTRACE_TYPE_OBJ, KDATAN_TL_WRITE_REG, ctx,
		datan_cq_control_reg(gdev, cq_bind_addr_hi(channel_id)),
		(cq_ctx->gpu_context_buffer->uva >> 32));
	hvgr_datan_jobtrace(ctx, JOBTRACE_TYPE_OBJ, KDATAN_TL_WRITE_REG, ctx,
		datan_cq_control_reg(gdev, cq_bind_addr_lo(channel_id)),
		(cq_ctx->gpu_context_buffer->uva & 0xFFFFFFFFU));
	hvgr_datan_jobtrace(ctx, JOBTRACE_TYPE_OBJ, KDATAN_TL_WRITE_REG, ctx,
		datan_cq_control_reg(gdev, cq_bind_jd_msb(channel_id)),
		ctx->same_va_4g_base_page);
	hvgr_datan_jobtrace(ctx, JOBTRACE_TYPE_OBJ, KDATAN_TL_WRITE_REG, ctx,
		datan_cq_control_reg(gdev, cq_bind_queue_id(channel_id)),
		cq_ctx->queue_id);
	hvgr_datan_jobtrace(ctx, JOBTRACE_TYPE_OBJ, KDATAN_TL_WRITE_REG, ctx,
		datan_cq_control_reg(gdev, cq_bind_cmd(channel_id)),
		CQ_CMD_START);
}

bool hvgr_cq_is_append_exception_flag(struct hvgr_cq_ctx *cq_ctx)
{
	return (cq_ctx->flags & (~CQ_KMD_CTX_START_SUBMIT)) != 0;
}

static void hvgr_cq_check_large_tile(struct hvgr_cq_ctx * const cq_ctx)
{
	ktime_t cur_time = ktime_get();
	if ((uint64_t)ktime_to_ns(cur_time - cq_ctx->unbind_time) > HVGR_UNBIND_THRESHOLD &&
		(uint64_t)ktime_to_ns(cur_time - cq_ctx->last_log_time) > HVGR_UNBIND_LOG_THRESHOLD) {
		hvgr_debug(cq_ctx->ctx->gdev, HVGR_CQ,
			"%s ctx_%u(tgid:%u,task:%s,group:%s),entry(%u) unbind %llu ns.",
			__func__, cq_ctx->ctx->id, cq_ctx->ctx->tgid, cq_ctx->ctx->process_name,
			cq_ctx->ctx->group_name, cq_ctx->subchn_os,
			(uint64_t)ktime_to_ns(cur_time - cq_ctx->unbind_time));
		cq_ctx->last_log_time = cur_time;
	}
}

void hvgr_cq_irq_do_stop(struct hvgr_cq_ctx * const cq_ctx, struct hvgr_device *gdev,
	uint32_t channel_id)
{
	if (unlikely(hvgr_cq_is_append_exception_flag(cq_ctx))) {
		hvgr_debug(cq_ctx->ctx->gdev, HVGR_CQ,
			"%s clear error status,ctx_%u", __func__, cq_ctx->ctx->id);
		cq_ctx->flags &= CQ_KMD_CTX_START_SUBMIT;
	}

	if (cq_ctx->status != CQ_STATUS_STOPPING)
		hvgr_err(cq_ctx->ctx->gdev, HVGR_CQ,
			"ctx_%u qid=%u Current status is not expected to come in. %u",
			cq_ctx->ctx->id, cq_ctx->queue_id, (uint32_t)cq_ctx->status);
	hvgr_cq_mod_status_macro(cq_ctx, CQ_STATUS_STOPPED);
	hvgr_cq_check_large_tile(cq_ctx);
	hvgr_datan_jobtrace(cq_ctx->ctx, JOBTRACE_TYPE_OBJ, KDATAN_TL_NEW_WAIT_IRQ, cq_ctx->ctx,
		cq_ctx->queue_id, channel_id, KDATAN_JOBTRACE_IRQ_STOP_STATUS, 0);
	hvgr_cq_sched_update_dvfs_status(cq_ctx, HVGR_THRO_IDLE, HVGR_THRO_ACTION_DO_STOP);

	hvgr_update_freq_hint(cq_ctx->ctx->gdev);
	del_timer(&cq_ctx->backup_timer);

#if defined(HVGR_WAITFENCE_LOWPOWER) && HVGR_WAITFENCE_LOWPOWER != 0
	if (cq_ctx->need_check_wait_entry)
		hvgr_cq_check_wait_entry(cq_ctx, gdev);
#endif
}
