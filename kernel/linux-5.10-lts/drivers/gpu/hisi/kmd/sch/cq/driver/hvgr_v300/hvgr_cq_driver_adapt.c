/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2021-2022. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */

#include "hvgr_cq_driver_adapt.h"

#include <linux/kernel.h>
#include <linux/delay.h>
#include <securec.h>

#include "hvgr_cq_driver_base.h"
#include "hvgr_regmap.h"
#include "hvgr_dm_api.h"
#include "hvgr_log_api.h"
#include "hvgr_datan_api.h"

#include "hvgr_cq_debug_switch.h"
#include "hvgr_cq_schedule.h"
#include "hvgr_softjob.h"
#include "hvgr_cq_defs.h"
#include "hvgr_pm_api.h"
#include "hvgr_hts_api.h"
#include "dm/fcp/hvgr_fcp.h"
#include "hvgr_cq.h"
#include "hvgr_file.h"
#include "mem/hvgr_mem_zone.h"
#include "hvgr_kmd_defs.h"
#include "hvgr_datan_api.h"

#if hvgr_version_lt(HVGR_V500)
#include "hvgr_cq_swctx.h"
#else
#include "hvgr_cq_swctx_divide.h"
#endif
#include "hvgr_sch_dfx.h"

#define CQ_DFX_ORDER_BUF_SIZE            cq_ringbuf_align(1024)
#define CQ_DFX_SYNC_BUF_SIZE             cq_ringbuf_align(1024)
#define CQ_DFX_REPLAY_ORDER_BUF_SIZE     cq_ringbuf_align(1024)

static bool hvgr_cq_alloc_sw_dfxbuffer_memory(struct hvgr_ctx * const ctx,
	struct hvgr_cq_ctx * const cq_ctx);

static void hvgr_cq_free_sw_dfxbuffer_memory(struct hvgr_cq_ctx * const cq_ctx,
	struct cq_sw_writeable_context *sw_wr_ctx);


static struct hvgr_cq_prioriy_cfg cq_pri_tbl[] = {
	{CQ_GLOBAL_PRIORITY_LOW_COMPUTE_L, 1, 0},
	{CQ_GLOBAL_PRIORITY_LOW_COMPUTE_H, 3, 0},
	{CQ_GLOBAL_PRIORITY_MEDIUM_COMPUTE_L, 6, 0},
	{CQ_GLOBAL_PRIORITY_MEDIUM_COMPUTE_H, 9, 0},
	{CQ_GLOBAL_PRIORITY_HIGH_COMPUTE_L, 12, 0},
	{CQ_GLOBAL_PRIORITY_HIGH_COMPUTE_H, 15, 0},
	{CQ_GLOBAL_PRIORITY_REALTIME_COMPUTE_L, 0, 1},
	{CQ_GLOBAL_PRIORITY_REALTIME_COMPUTE_H, 4, 1},
};

void hvgr_get_priority_cfg(uint32_t pri, uint32_t *weight, uint32_t *real)
{
	if (unlikely(pri >= ARRAY_SIZE(cq_pri_tbl)))
		return;
	*weight = cq_pri_tbl[pri].weight;
	*real = cq_pri_tbl[pri].real;
}

static void hvgr_cq_config_priority(struct hvgr_device * const gdev)
{
	uint32_t pri;
	uint32_t weight = 0;
	uint32_t real = 0;

	for (pri = 0; pri < ARRAY_SIZE(cq_pri_tbl); pri++) {
		/* The weight ranges from 0 to 15, four bits are required. */
		weight |= (cq_pri_tbl[pri].weight << (pri << 0x2));
		real |= (cq_pri_tbl[pri].real << pri);
	}
	if (gdev->cq_dev.hw_cfg.cq_queue_weight == 0 && gdev->cq_dev.hw_cfg.cq_queue_real == 0) {
		hvgr_fast_pwr_write_reg(gdev, cq_control_reg(gdev, CQ_ENTRYS_WEIGHT), weight);
		hvgr_fast_pwr_write_reg(gdev, cq_control_reg(gdev, CQ_ENTRYS_REALTIME), real);
	} else {
		hvgr_fast_pwr_write_reg(gdev, cq_control_reg(gdev, CQ_ENTRYS_WEIGHT),
			gdev->cq_dev.hw_cfg.cq_queue_weight);
		hvgr_fast_pwr_write_reg(gdev, cq_control_reg(gdev, CQ_ENTRYS_REALTIME),
			gdev->cq_dev.hw_cfg.cq_queue_real);
	}
}

void hvgr_cq_affinity_config(struct hvgr_device * const gdev, uint32_t channel_id)
{
	int sub_chn;
	uint32_t core_mask = hvgr_pm_get_availiable_cores(gdev);

	hvgr_cq_compute_affinity_init(gdev, channel_id, core_mask);

	for (sub_chn = RENDER_IDX; sub_chn < SUBCHN_MAX_IDX; sub_chn++)
		if (sub_chn != COMPUTE_IDX) {
			hvgr_write_reg(gdev,
				cq_control_reg(gdev,
				cq_bind_affinity_sub_channel(channel_id, sub_chn)), core_mask);
			hvgr_datan_jobtrace(gdev->dm_dev.fcp_data->ctx, JOBTRACE_TYPE_OBJ, KDATAN_TL_WRITE_REG,
				gdev->dm_dev.fcp_data->ctx, datan_cq_control_reg(gdev,
				cq_bind_affinity_sub_channel(channel_id, sub_chn)), core_mask);
		}
}

void hvgr_cq_multi_chain_enable_init(struct hvgr_device * const gdev,
	struct hvgr_cq_dev * const gcqctx, uint32_t channel_id)
{
}

void hvgr_cq_compute_affinity_init(struct hvgr_device * const gdev,
	uint32_t channel_id, uint32_t core_mask)
{
	struct hvgr_cq_dev * const gcqctx = &gdev->cq_dev;
	uint32_t cq_cl_affinity = gcqctx->hw_cfg.cq_cl_affinity;

	/* cq_cl_affinity must be part of core_mask */
	cq_cl_affinity &= core_mask;
	if (cq_cl_affinity == 0) {
		hvgr_err(gdev, HVGR_CQ, "[cq]: %s: core_mask=%d, cq_cl_affinity=%d, not right",
			__func__, core_mask, gcqctx->hw_cfg.cq_cl_affinity);
		/* when config fail, report and recover config. */
		cq_cl_affinity = core_mask;
	}

	hvgr_write_reg(gdev,
		cq_control_reg(gdev, cq_bind_affinity_sub_channel(channel_id, COMPUTE_IDX)),
		cq_cl_affinity);
	hvgr_datan_jobtrace(gdev->dm_dev.fcp_data->ctx, JOBTRACE_TYPE_OBJ, KDATAN_TL_WRITE_REG,
		gdev->dm_dev.fcp_data->ctx, datan_cq_control_reg(gdev,
		cq_bind_affinity_sub_channel(channel_id, COMPUTE_IDX)), cq_cl_affinity);
}

void hvgr_cq_set_affinity(struct hvgr_device * const gdev,
	uint32_t channel_id, uint32_t core_mask)
{
	struct hvgr_cq_dev * const gcqctx = &gdev->cq_dev;
	uint32_t cq_cl_affinity = gcqctx->hw_cfg.cq_cl_affinity;
	uint32_t idx;

	/* cq_cl_affinity must be part of core_mask */
	cq_cl_affinity &= core_mask;
	if (cq_cl_affinity == 0) {
		hvgr_err(gdev, HVGR_CQ, "[cq]: %s: core_mask=%d, cq_cl_affinity=%d, not right",
			__func__, core_mask, gcqctx->hw_cfg.cq_cl_affinity);
		/* when config fail, report and recover config. */
		cq_cl_affinity = core_mask;
	}

	for (idx = RENDER_IDX; idx < SUBCHN_MAX_IDX; ++idx) {
		if (idx == COMPUTE_IDX) {
			hvgr_write_reg(gdev, cq_control_reg(gdev,
				cq_bind_affinity_sub_channel(channel_id, idx)), cq_cl_affinity);
			hvgr_datan_jobtrace(gdev->dm_dev.fcp_data->ctx, JOBTRACE_TYPE_OBJ,
				KDATAN_TL_WRITE_REG, gdev->dm_dev.fcp_data->ctx,
				datan_cq_control_reg(gdev,
				cq_bind_affinity_sub_channel(channel_id, idx)), cq_cl_affinity);
		} else {
			hvgr_write_reg(gdev, cq_control_reg(gdev,
				cq_bind_affinity_sub_channel(channel_id, idx)), core_mask);
			hvgr_datan_jobtrace(gdev->dm_dev.fcp_data->ctx, JOBTRACE_TYPE_OBJ,
				KDATAN_TL_WRITE_REG, gdev->dm_dev.fcp_data->ctx,
				datan_cq_control_reg(gdev,
				cq_bind_affinity_sub_channel(channel_id, idx)), core_mask);
		}
	}
}

static void hvgr_set_cq_bind_config(union reg_cq_bind_config *config,
	struct cq_hw_cfg *hw_cfg, enum hvgr_cq_subchn_index index)
{
	/* Set as user guide render:0x3, bin:0x1, compute:0x4, bvhg:0x8, rt:0x8 */
	uint32_t basic_flush_type[SUBCHN_MAX_IDX] = {0x3, 0x1, 0x4, 0x8, 0x8};
	/* (index == RENDER_IDX) ? 0 : 1 , for now enable barrier on all channel */
	config->st.barrier = CQ_BIND_BARRIER_ENABLE;
	config->st.end_flush_l2 = ((index != BIN_IDX) || hw_cfg->cq_force_flush_l2) ?
		CQ_BIND_ENDFLUSH_CLEAN_AND_INVALIDATE : CQ_BIND_ENDFLUSH_NO;
	config->st.end_flush_shader_type = basic_flush_type[index];
}

static void hvgr_cq_config_bind_cfg(struct hvgr_device * const gdev, uint32_t channel_id)
{
	struct cq_hw_cfg *hw_cfg = &gdev->cq_dev.hw_cfg;
	union reg_cq_bind_config config;
	uint32_t index;

	for (index = RENDER_IDX; index < SUBCHN_MAX_IDX; index++) {
		config.value = hvgr_read_reg(gdev,
			cq_control_reg(gdev, cq_bind_config(channel_id, index)));
		hvgr_set_cq_bind_config(&config, hw_cfg, index);
		hvgr_fast_pwr_write_reg(gdev, cq_control_reg(gdev, cq_bind_config(channel_id, index)),
			config.value);
		hvgr_datan_jobtrace(gdev->dm_dev.fcp_data->ctx, JOBTRACE_TYPE_OBJ,
			KDATAN_TL_WRITE_REG, gdev->dm_dev.fcp_data->ctx,
			datan_cq_control_reg(gdev, cq_bind_config(channel_id, index)), config.value);
	}
}

void hvgr_cq_hwcfg_init(struct hvgr_device * const gdev, uint32_t mode)
{
	struct hvgr_cq_dev * const gcqctx = &gdev->cq_dev;
	uint32_t chnid;

#if hvgr_version_lt(HVGR_V350)
	hvgr_write_reg(gdev, cq_control_reg(gdev, CQ_ENABLE), mode);
#endif
	for (chnid = 0; chnid < CQ_CHANNEL_NUM; chnid++) {
		hvgr_cq_multi_chain_enable_init(gdev, gcqctx, chnid);

		/* Affinity must precede bc_send_mode. */
		hvgr_cq_affinity_config(gdev, chnid);

		hvgr_cq_config_bind_cfg(gdev, chnid);

		hvgr_fast_pwr_write_reg(gdev,
			cq_control_reg(gdev, cq_bind_waitcycle(chnid)),
				gcqctx->hw_cfg.cq_bind_waitcycle);
		hvgr_fast_pwr_write_reg(gdev,
			cq_control_reg(gdev, cq_bind_empty_timeout(chnid)),
				gcqctx->hw_cfg.cq_queue_empty_timeout);
		hvgr_fast_pwr_write_reg(gdev,
			cq_control_reg(gdev, cq_bind_sem_wait_timeout(chnid)),
				gcqctx->hw_cfg.cq_semp_wait_timeout);
	}

	hvgr_fast_pwr_write_reg(gdev, cq_control_reg(gdev, CQ_DBG_EN), gcqctx->hw_cfg.work_mode);
	hvgr_info(gdev, HVGR_CQ, "[cq]: %s: cq_sch_mode=%u, bc_send_mode=%u, work_mode=%u",
		__func__, gcqctx->hw_cfg.cq_sch_mode, gcqctx->hw_cfg.bc_send_mode,
		gcqctx->hw_cfg.work_mode);
	/* Config priority */
	hvgr_cq_config_priority(gdev);
	/* sem_wait_polling_mode: set to 0 temporarily, hardware polling */
	hvgr_fast_pwr_write_reg(gdev, cq_control_reg(gdev, CQ_SEM_WAIT_POLLING_MODE),
		CQ_SEM_WAIT_MODE_POLLING);
	hvgr_fast_pwr_write_reg(gdev, cq_control_reg(gdev, CQ_ENTRYS_OS), gcqctx->hw_cfg.cq_entrys_os);

#if hvgr_version_eq(HVGR_V300)
	hvgr_write_reg(gdev, cq_control_reg(gdev, CQ_SUBQ_ENTRY_OS_PRES_L),
		CQ_SUBQ_ENTRY_OS_PRES_L_H_CFG);
	hvgr_write_reg(gdev, cq_control_reg(gdev, CQ_SUBQ_ENTRY_OS_PRES_H),
		CQ_SUBQ_ENTRY_OS_PRES_L_H_CFG);
#endif

	/* Only for jobtimeline dump */
	hvgr_write_reg(gdev, cq_control_reg(gdev, CQ_DFX_CONFIG), gcqctx->hw_cfg.cq_entry_mode);

	/* config the ram size */
	hvgr_fast_pwr_write_reg(gdev, cq_control_reg(gdev, CQ_BINNING_RAM_SIZE), CQ_BINNING_RAM_SIZE_VALUE);
	hvgr_fast_pwr_write_reg(gdev, cq_control_reg(gdev, CQ_RENDER_RAM_SIZE), CQ_RENDER_RAM_SIZE_VALUE);
	hvgr_fast_pwr_write_reg(gdev, cq_control_reg(gdev, CQ_COMPUTE_RAM_SIZE), CQ_COMPUTER_RAM_SIZE_VALUE);
	hvgr_fast_pwr_write_reg(gdev, cq_control_reg(gdev, CQ_BVH_RAM_SIZE), CQ_BVH_RAM_SIZE_VALUE);
	hvgr_fast_pwr_write_reg(gdev, cq_control_reg(gdev, CQ_RT_RAM_SIZE), CQ_RT_RAM_SIZE_VALUE);

#if hvgr_version_ge(HVGR_V350)
	hvgr_fast_pwr_write_reg(gdev, cq_control_reg(gdev, CQ_MAX_RMU_COUNT), CQ_MAX_RMU_COUNT_VALUE);
	hvgr_fast_pwr_write_reg(gdev, cq_control_reg(gdev, CQ_MAX_RMU_SLICE_COUNT), CQ_MAX_RMU_SLICE_COUNT_VALUE);
	hvgr_fast_pwr_write_reg(gdev, cq_control_reg(gdev, CQ_MAX_VB_SHARP_COUNT), CQ_MAX_VB_SHARP_COUNT_VALUE);
	hvgr_fast_pwr_write_reg(gdev, cq_control_reg(gdev, CQ_RENDER_NODE_STYPE), CQ_RENDER_NODE_STYPE_VALUE);
	hvgr_fast_pwr_write_reg(gdev, cq_control_reg(gdev, CQ_MESH_NODE_STYPE), CQ_MESH_NODE_STYPE_VALUE);
	hvgr_fast_pwr_write_reg(gdev, cq_control_reg(gdev, CQ_INTERNAL_UNIFORM_SIZE), CQ_INTERNAL_UNIFORM_SIZE_VALUE);
#endif
}

bool hvgr_cq_get_empty_flag(struct hvgr_device *gdev, uint32_t channel_id)
{
	union reg_cq_bind_entry_exe_status queue_status;

	queue_status.value =
		hvgr_read_reg(gdev, cq_control_reg(gdev, cq_bind_entry_exe_status(channel_id)));
	return (queue_status.st.entry_exe_empty == 1) && (queue_status.st.queue_entry_empty == 1);
}

bool hvgr_cq_get_wait_flag(struct hvgr_device *gdev, uint32_t channel_id)
{
	union reg_cq_bind_entry_exe_status queue_status;

	queue_status.value =
		hvgr_read_reg(gdev, cq_control_reg(gdev, cq_bind_entry_exe_status(channel_id)));
	return (queue_status.st.sem_wait_status) == 1;
}

uint32_t hvgr_cq_get_queue_status(struct hvgr_device *gdev, uint32_t channel_id)
{
	return	hvgr_read_reg(gdev, cq_control_reg(gdev, cq_bind_entry_exe_status(channel_id)));
}

static void hvgr_cq_get_profiling_timestamp(struct hvgr_device *gdev,
	struct token_callback_data *callback)
{
#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0))
	struct timespec ts;

	getrawmonotonic(&ts);
#else
	struct timespec64 ts;

	ktime_get_raw_ts64(&ts);
#endif

	callback->profiling_data.soc_timestamp = hvgr_dm_get_soc_timestamp(gdev);
	callback->profiling_data.os_timestamp =
		(uint64_t)ts.tv_sec * 1000000000 + (uint64_t)ts.tv_nsec;
	hvgr_info(gdev, HVGR_CQ, "soc timestamp is %lu, os timestamp is %lu.",
		callback->profiling_data.soc_timestamp, callback->profiling_data.os_timestamp);
}

static void hvgr_cq_irq_do_notify(struct hvgr_cq_ctx * const cq_ctx, struct hvgr_device *gdev,
	uint32_t channel_id, enum protect_flag *flag)
{
	union reg_cq_bind_status_notify notify;
	struct token_callback_data *callback = NULL;
	struct hvgr_cq_ctx_data *ctx_cq =  &cq_ctx->ctx->ctx_cq;
	unsigned long flags;

	notify.value = hvgr_read_reg(gdev,
		cq_control_reg(gdev, cq_bind_top_notify_info(channel_id)));

	while ((notify.st.entry_info_vld == 1) && (notify.st.entry_id != 0)) {
		if (unlikely(!hvgr_token_is_valid(cq_ctx->ctx, notify.st.entry_id)))
			return;

		hvgr_info(gdev, HVGR_CQ, "%s token=%d, put:get(%d:%d)\n",
			__func__, notify.st.entry_id,
			hvgr_get_queue_put_index(cq_ctx), hvgr_get_queue_get_index(cq_ctx));

		cq_ctx->ctx->token_process_step = TOKEN_PROCESS_IRQ;
		hvgr_cq_record_irq_info(cq_ctx, notify.st.entry_id, HVGR_CQ_IRQ_NOTIFY);

		hvgr_datan_jobtrace(cq_ctx->ctx, JOBTRACE_TYPE_OBJ, KDATAN_TL_NEW_WAIT_IRQ,
			cq_ctx->ctx, cq_ctx->queue_id, channel_id,
			KDATAN_JOBTRACE_IRQ_NOTIFY_STATUS, notify.value);

		spin_lock_irqsave(&ctx_cq->callback_data_lock[notify.st.entry_id], flags);
		callback = ctx_cq->token_callback[notify.st.entry_id];
		if (callback != NULL && callback->ctx != NULL) {
			/* We do not need to check hold_en when the callback->ctx is null. */
			if ((callback->flags & HVGR_TOKEN_HOLD_EN_IN) != 0)
				*flag = CQ_PROTECT_MODE_IN;
			else if ((callback->flags & HVGR_TOKEN_HOLD_EN_OUT) != 0)
				*flag = CQ_PROTECT_MODE_OUT;
			else if ((callback->flags & HVGR_TOKEN_FREQ_HINT) != 0)
				callback->cq_ctx = cq_ctx;
			else
				*flag = CQ_PROTECT_MODE_INVALID;
			if ((callback->flags & HVGR_TOKEN_CL_PROFILING_EXEC) != 0 ||
				(callback->flags & HVGR_TOKEN_CL_PROFILING_COMPLETE) != 0)
				hvgr_cq_get_profiling_timestamp(cq_ctx->ctx->gdev, callback);

			callback->flags |= HVGR_TOKEN_NOTIFY_TOP;
		}
		spin_unlock_irqrestore(&ctx_cq->callback_data_lock[notify.st.entry_id], flags);
		hvgr_info(gdev, HVGR_CQ, "%s hold_en =%d, channel_id=%d, token=%d",
			__func__, *flag, channel_id, notify.st.entry_id);
		hvgr_token_finish(cq_ctx->ctx, notify.st.entry_id);
		notify.value = hvgr_read_reg(gdev,
			cq_control_reg(gdev, cq_bind_top_notify_info(channel_id)));
	}
}

static void hvgr_cq_irq_do_subq(struct hvgr_cq_ctx * const cq_ctx,
	struct hvgr_device *gdev, uint32_t chnid, uint32_t sub_q_mask)
{
	uint32_t idx;
	union reg_cq_bind_status_subq status;
	struct token_callback_data *callback = NULL;
	struct hvgr_cq_ctx_data *ctx_cq = &cq_ctx->ctx->ctx_cq;
	unsigned long flags;

	for (idx = 0; idx < CQ_IRQ_STATUS_SUBQ_BIT_NUMS; idx++) {
		if ((sub_q_mask & (1u << idx)) == 0)
			continue;

		status.value = hvgr_read_reg(gdev,
			cq_control_reg(gdev, cq_bind_status_subq(chnid, idx)));
		while ((status.st.entry_info_vld == 1) && (status.st.entry_id != 0)) {
			hvgr_datan_jobtrace(cq_ctx->ctx, JOBTRACE_TYPE_OBJ, KDATAN_TL_NEW_WAIT_IRQ,
				cq_ctx->ctx, cq_ctx->queue_id, chnid,
				KDATAN_JOBTRACE_IRQ_SUBQ0_STATUS + idx, status.value);
			hvgr_info(gdev, HVGR_SCH,
				"%s token=%d, entry_type=%d, put:get(%d:%d)\n",
				__func__, status.st.entry_id, status.st.entry_type,
				hvgr_get_queue_put_index(cq_ctx),
				hvgr_get_queue_get_index(cq_ctx));

			cq_ctx->ctx->token_process_step = TOKEN_PROCESS_IRQ;
			hvgr_cq_record_irq_info(cq_ctx, status.st.entry_id, HVGR_CQ_IRQ_SUBQ);

			/* If we found error token, break to avoid deadloop. */
			if (unlikely(!hvgr_token_is_valid(cq_ctx->ctx, status.st.entry_id))) {
				hvgr_info(gdev, HVGR_SCH,
					"%s token=%d, idx=%d, channel=%d\n",
					__func__, status.st.entry_id, idx, chnid);
				break;
			}

			spin_lock_irqsave(&ctx_cq->callback_data_lock[status.st.entry_id], flags);
			callback = ctx_cq->token_callback[status.st.entry_id];
			if (callback != NULL) {
				if ((callback->flags & HVGR_TOKEN_FREQ_HINT) != 0)
					callback->cq_ctx = cq_ctx;
				if ((callback->flags & HVGR_TOKEN_CL_PROFILING_EXEC) != 0 ||
					(callback->flags & HVGR_TOKEN_CL_PROFILING_COMPLETE) != 0)
					hvgr_cq_get_profiling_timestamp(gdev, callback);
			}
			spin_unlock_irqrestore(&ctx_cq->callback_data_lock[status.st.entry_id], flags);

			hvgr_token_finish(cq_ctx->ctx, status.st.entry_id);
			status.value = hvgr_read_reg(gdev,
				cq_control_reg(gdev, cq_bind_status_subq(chnid, idx)));
		}
	}
}

static void hvgr_cq_irq_do_entry_boundary(struct hvgr_cq_ctx * const cq_ctx,
	struct hvgr_device *gdev, uint32_t chnid)
{
	union reg_cq_bind_status_boundary status;
	char *job_status[] = {
		"start",
		"done",
		"not availible",
		"stop"
	};
	uint16_t type;

	status.value = hvgr_read_reg(gdev,
		cq_control_reg(gdev, cq_bind_entry_boundary_info(chnid)));
	while ((status.st.entry_info_vld == 1) && (status.st.entry_id != 0)) {
		hvgr_info(gdev, HVGR_CQ, "[Chn: %d qid: %u ctx_%u]: entry id: %d entry type: %d %s.\n",
			chnid, cq_ctx->queue_id, cq_ctx->ctx->id, status.st.entry_id,
			status.st.entry_type, job_status[status.st.job_chain_status]);
		hvgr_datan_jobtrace(cq_ctx->ctx, JOBTRACE_TYPE_OBJ, KDATAN_TL_NEW_WAIT_IRQ,
			cq_ctx->ctx, cq_ctx->queue_id, chnid, KDATAN_JOBTRACE_IRQ_BOUNDARY_STATUS, status.value);

		type = status.st.job_chain_status == 0 ? HVGR_TIMELINE_JOB_CHAIN_START :
			HVGR_TIMELINE_JOB_CHAIN_END;
		hvgr_datan_jt_record_entry_boundary(cq_ctx->ctx, type,
			status.st.entry_type, status.st.entry_id, cq_ctx->queue_id);

		hvgr_info(gdev, HVGR_CQ, "boundary irq %u\n", status.value);
		status.value = hvgr_read_reg(gdev,
			cq_control_reg(gdev, cq_bind_entry_boundary_info(chnid)));
	}

	hvgr_info(gdev, HVGR_CQ, "[Chn: %d qid: %u ctx_%u]: entry id: %d entry type: %d %s.\n",
		chnid, cq_ctx->queue_id, cq_ctx->ctx->id, status.st.entry_id, status.st.entry_type,
		job_status[status.st.job_chain_status]);
}

static void hvgr_cq_irq_do_order_buf_full(struct hvgr_cq_ctx * const cq_ctx,
	struct hvgr_device *gdev, uint32_t chnid)
{
}

static void hvgr_cq_irq_do_sync_buf_full(struct hvgr_cq_ctx * const cq_ctx,
	struct hvgr_device *gdev, uint32_t chnid)
{
}

static void hvgr_cq_irq_do_exe_empty(struct hvgr_cq_ctx * const cq_ctx,
	struct hvgr_device *gdev, uint32_t chnid)
{
	struct hvgr_cq_dev * const gcqctx = &gdev->cq_dev;
	union reg_cq_bind_entry_exe_status queue_status;

	if (gcqctx->protect_mode_info.protected_mode_transition ||
		gcqctx->protect_mode_info.protected_mode)
		return;

	queue_status.value = hvgr_read_reg(gdev, cq_control_reg(gdev, cq_bind_entry_exe_status(chnid)));
	if ((queue_status.st.entry_exe_empty == 1) && (queue_status.st.queue_entry_empty == 1) &&
		hvgr_get_queue_get_index(cq_ctx) == hvgr_get_queue_put_index(cq_ctx)) {
		hvgr_info(gdev, HVGR_CQ, "ctx_%u, queueid_%u get_index == put_index",
			cq_ctx->ctx->id, cq_ctx->queue_id);
		hvgr_cq_unbind_channel(cq_ctx, chnid, CQ_NEED_UNBIND_TIMER_FLAG);
		return;
	}
	cq_ctx->wait_timeout = queue_status.value & CQ_WAIT_TIMEOUT_MASK;
	if (cq_ctx->wait_timeout == 0)
		return;

	hvgr_info(gdev, HVGR_CQ, "%s wait entry timeout, type = %u", __func__, cq_ctx->wait_timeout);

#if defined(HVGR_FEATURE_HTS) && HVGR_FEATURE_HTS != 0
	if ((queue_status.value & HTS_WAIT_OVERTIME) != 0) {
		hvgr_cq_unbind_channel(cq_ctx, chnid, CQ_NEED_UNBIND_TIMER_FLAG);
		return;
	}
#endif

#if defined(HVGR_WAITFENCE_LOWPOWER) && HVGR_WAITFENCE_LOWPOWER != 0
	if (cq_ctx->get_index_exe_timeout == hvgr_get_queue_get_index(cq_ctx))
		return;
	cq_ctx->get_index_exe_timeout = hvgr_get_queue_get_index(cq_ctx);

	if ((queue_status.value & CQ_FENCE_WAIT_OVERTIME) != 0) {
		cq_ctx->wait_timeout &= ~CQ_FENCE_WAIT_OVERTIME;
		if (gcqctx->wait_fence_switch_flag) {
			cq_ctx->need_check_wait_entry = true;
			hvgr_info(gdev, HVGR_CQ, "queueid_%u exe_timeout, need check wait entry.",
				cq_ctx->queue_id);
			hvgr_cq_unbind_channel(cq_ctx, chnid, CQ_NEED_UNBIND_TIMER_FLAG);
		}
	}
#endif
}

static inline struct hvgr_cq_ctx *hvgr_cq_get_channel_ctx(struct hvgr_device *gdev,
	uint32_t channel_id)
{
	struct hvgr_cq_dev * const gcqctx = &gdev->cq_dev;

	return gcqctx->channel_info[channel_id].running_queue;
}

void hvgr_cq_schedule_do_channel_irq(struct hvgr_device * const gdev, uint32_t chn)
{
	union reg_cq_irq irq_state;
	bool sche_en = false;
	struct hvgr_cq_ctx *cq_ctx = hvgr_cq_get_channel_ctx(gdev, chn);
	enum protect_flag flag = CQ_PROTECT_MODE_INVALID;

	irq_state.value = hvgr_read_reg(gdev, cq_control_reg(gdev, cq_irq_status(chn)));

	if (unlikely(cq_ctx == NULL)) {
		hvgr_err(gdev, HVGR_CQ, "%s chn(%d) reg value 0x%x not right\n",
			__func__, chn, irq_state.value);
		hvgr_write_reg(gdev, cq_control_reg(gdev, cq_irq_clear(chn)), irq_state.value);
		return;
	}

	if ((irq_state.value & CQ_IRQ_STATUS_SUBQ_BIG_MASK) != 0) {
		hvgr_cq_irq_do_subq(cq_ctx, gdev, chn,
			irq_state.value >> CQ_IRQ_STATUS_SUBQ_BIT_START);
		sche_en = true;
	}

	if (irq_state.st.notify != 0) {
		hvgr_cq_irq_do_notify(cq_ctx, gdev, chn, &flag);
		sche_en = true;
	}

	if (irq_state.st.sync_buf_full != 0)
		hvgr_cq_irq_do_sync_buf_full(cq_ctx, gdev, chn);

	if (irq_state.st.order_buf_full != 0)
		hvgr_cq_irq_do_order_buf_full(cq_ctx, gdev, chn);

	if (irq_state.st.entry_boundary != 0)
		hvgr_cq_irq_do_entry_boundary(cq_ctx, gdev, chn);

	if (irq_state.st.entry_exe_empty != 0)
		hvgr_cq_irq_do_exe_empty(cq_ctx, gdev, chn);

	/* we resolve stop irq at last, to avoid irq left after stop.
	 * when we receive hold_en, we ignore current stop to enter or exit protect mode.
	 */
	if (irq_state.st.stop != 0) {
		hvgr_info(gdev, HVGR_CQ, "%s chn(%d) stop,qid=%u,ctx_%u\n",
			__func__, chn, cq_ctx->queue_id, cq_ctx->ctx->id);
		hvgr_cq_irq_do_stop(cq_ctx, gdev, chn);
		sche_en = true;
	}

	/* clear the entry dump irq when dump completed */
	irq_state.st.sync_buf_full = 0;
	irq_state.st.order_buf_full = 0;
	hvgr_write_reg(gdev, cq_control_reg(gdev, cq_irq_clear(chn)), irq_state.value);
	if (sche_en)
		hvgr_cq_schedule_channel(gdev, chn, flag, false);
}

void hvgr_cq_dump_debug_register(struct hvgr_device * const gdev)
{
	/* dupm register */
	hvgr_debug(gdev, HVGR_CQ, "  CQ_BIND_SUBCHN_OS=0x%08x    0x%08x    0x%08x    0x%08x",
		hvgr_read_reg(gdev, cq_control_reg(gdev, cq_bind_subchn_os(0))),
		hvgr_read_reg(gdev, cq_control_reg(gdev, cq_bind_subchn_os(1))),
		hvgr_read_reg(gdev, cq_control_reg(gdev, cq_bind_subchn_os(2))),
		hvgr_read_reg(gdev, cq_control_reg(gdev, cq_bind_subchn_os(3))));
	hvgr_debug(gdev, HVGR_CQ, "  CQ_DBG_EN=0x%08x    CQ_DFX_CONFIG=0x%08x",
		hvgr_read_reg(gdev, cq_control_reg(gdev, CQ_DBG_EN)),
		hvgr_read_reg(gdev, cq_control_reg(gdev, CQ_DFX_CONFIG)));

	hvgr_datan_dump_chain_state(gdev);
}

void hvgr_cq_show_hang_reg_status(struct hvgr_device * const gdev)
{
	hvgr_debug(gdev, HVGR_CQ, "subchn  status [0x%x] [0x%x] [0x%x] [0x%x].\n",
		hvgr_read_reg(gdev, cq_control_reg(gdev, cq_bind_subchn_os(0))),
		hvgr_read_reg(gdev, cq_control_reg(gdev, cq_bind_subchn_os(1))),
		hvgr_read_reg(gdev, cq_control_reg(gdev, cq_bind_subchn_os(2))),
		hvgr_read_reg(gdev, cq_control_reg(gdev, cq_bind_subchn_os(3))));

#if hvgr_version_eq(HVGR_V300)
	hvgr_debug(gdev, HVGR_CQ, "bind status [0x%x] [0x%x] [0x%x] [0x%x].\n",
		hvgr_get_bind_status(gdev, 0), hvgr_get_bind_status(gdev, 1),
		hvgr_get_bind_status(gdev, 2), hvgr_get_bind_status(gdev, 3));
#endif

	hvgr_debug(gdev, HVGR_CQ, "entry status [0x%x] [0x%x] [0x%x] [0x%x].\n",
		hvgr_read_reg(gdev, cq_control_reg(gdev, cq_bind_entry_exe_status(0))),
		hvgr_read_reg(gdev, cq_control_reg(gdev, cq_bind_entry_exe_status(1))),
		hvgr_read_reg(gdev, cq_control_reg(gdev, cq_bind_entry_exe_status(2))),
		hvgr_read_reg(gdev, cq_control_reg(gdev, cq_bind_entry_exe_status(3))));
}

int hvgr_cq_register_irq(struct hvgr_device * const gdev)
{
	if (hvgr_register_irq(gdev, CQ_IRQ, hvgr_cq_irq_handler) != 0) {
		hvgr_err(gdev, HVGR_CQ, "Register cq irq handler failed.");
		return -1;
	}
	return 0;
}

void hvgr_cq_alloc_sw_dfx_memory(struct hvgr_ctx * const ctx,
	struct hvgr_cq_ctx * const cq_ctx)
{
	/* alloc dfx buffer memory */
	if (!hvgr_cq_alloc_sw_dfxbuffer_memory(ctx, cq_ctx))
		hvgr_err(ctx->gdev, HVGR_CQ, "alloc dfx buffer fail ctx[%u].", ctx->id);
}

void hvgr_cq_free_sw_dfx_memory(struct hvgr_ctx * const ctx,
	struct hvgr_cq_ctx * const cq_ctx)
{
	if (unlikely((ctx == NULL) || (cq_ctx == NULL)))
		return;

	hvgr_cq_free_sw_dfxbuffer_memory(cq_ctx, cq_ctx->sw_wr_ctx);
}


static void hvgr_cq_fill_dfx_buffer(struct cq_sw_writeable_context *sw_wr_ctx, uint64_t addr)
{
	if (!sw_wr_ctx)
		return;

	sw_wr_ctx->dfx_buf_info.order_buf_ptr = addr;
	sw_wr_ctx->dfx_buf_info.order_buf_size = CQ_DFX_ORDER_BUF_SIZE / sizeof(uint32_t);
	addr += CQ_DFX_ORDER_BUF_SIZE;

	sw_wr_ctx->dfx_buf_info.sync_buf_ptr = addr;
	sw_wr_ctx->dfx_buf_info.sync_buf_size = CQ_DFX_SYNC_BUF_SIZE / sizeof(uint32_t);
	addr += CQ_DFX_SYNC_BUF_SIZE;

#if hvgr_version_lt(HVGR_V350)
	sw_wr_ctx->dfx_buf_info.replay_order_buf_ptr = addr;
	sw_wr_ctx->dfx_buf_info.replay_order_buf_size =
		CQ_DFX_REPLAY_ORDER_BUF_SIZE / sizeof(uint32_t);
#endif
}

static bool hvgr_cq_alloc_sw_dfxbuffer_memory(struct hvgr_ctx * const ctx,
	struct hvgr_cq_ctx * const cq_ctx)
{
	uint64_t size = hvgr_mem_page_align(CQ_DFX_ORDER_BUF_SIZE + CQ_DFX_SYNC_BUF_SIZE +
		CQ_DFX_REPLAY_ORDER_BUF_SIZE);
	struct hvgr_mem_area *dfx_buffer_area = NULL;
	struct hvgr_mem_alloc_para para;

	para.property = HVGR_MEM_PROPERTY_GPU_MEM;
	para.attribute = HVGR_MEM_ATTR_GPU_RD | HVGR_MEM_ATTR_GPU_WR;
	para.pages = (uint32_t)hvgr_page_nums(size);
	para.max_pages = para.pages;

	hvgr_info(ctx->gdev, HVGR_MEM, "ctx_%u %s gpu memory proterty=0x%lx, max_page=%d start",
		ctx->id, __func__, para.property, para.max_pages);
	dfx_buffer_area = hvgr_mem_allocate(ctx, &para, NULL, false);
	if (!dfx_buffer_area)
		return false;

	if (!hvgr_mem_map_kva(dfx_buffer_area)) {
		if (dfx_buffer_area->zone->opts->free)
			dfx_buffer_area->zone->opts->free(dfx_buffer_area);
		return false;
	}

	atomic_add((int)para.pages, &ctx->mem_ctx.used_kmd_pages);
	(void)memset_s(u64_to_ptr(dfx_buffer_area->kva), size, 0, size);
	cq_ctx->sw_ctx_dfx_area = dfx_buffer_area;

	hvgr_cq_fill_dfx_buffer(cq_ctx->sw_wr_ctx, dfx_buffer_area->uva);

	hvgr_info(ctx->gdev, HVGR_MEM, "ctx_%u %s gpu memory uva=0x%lx, kva=0x%lxend ",
		ctx->id, __func__, dfx_buffer_area->uva, dfx_buffer_area->kva);
	return true;
}

static void hvgr_cq_free_sw_dfxbuffer_memory(struct hvgr_cq_ctx * const cq_ctx,
	struct cq_sw_writeable_context *sw_wr_ctx)
{
	uint64_t size = hvgr_mem_page_align(CQ_DFX_ORDER_BUF_SIZE + CQ_DFX_SYNC_BUF_SIZE +
		CQ_DFX_REPLAY_ORDER_BUF_SIZE);

	if (unlikely((sw_wr_ctx == NULL) || (cq_ctx->ctx == NULL)))
		return;

	if (unlikely(cq_ctx->sw_ctx_dfx_area == NULL))
		return;

	hvgr_mem_unmap_kva(cq_ctx->sw_ctx_dfx_area);
	hvgr_mem_free(cq_ctx->ctx, cq_ctx->sw_ctx_dfx_area->uva);
	atomic_sub((int)hvgr_page_nums(size), &cq_ctx->ctx->mem_ctx.used_kmd_pages);
	cq_ctx->sw_ctx_dfx_area = NULL;
}

void hvgr_cq_config_bc_send_mode(struct hvgr_device *gdev)
{
#if hvgr_version_ge(HVGR_V350)
	uint32_t core_mask = hvgr_pm_get_availiable_cores(gdev);
	struct hvgr_cq_dev * const gcqctx = &gdev->cq_dev;
	uint32_t bc_send_mode = gcqctx->hw_cfg.bc_send_mode;
	uint32_t rt_rbc_send_mode = gcqctx->hw_cfg.rt_rbc_send_mode;

	/* We must set bc_send_mode as interleave when single core. */
	if (hweight32(core_mask) == 1) {
		bc_send_mode = CQ_BC_SEND_INTERLEAVE;
		rt_rbc_send_mode = CQ_BC_SEND_INTERLEAVE;
	}
	hvgr_write_reg(gdev,
		cq_control_reg(gdev, CQ_JOBCHANIN_SEND_MODE), (rt_rbc_send_mode << 1) | bc_send_mode);
#else
	uint32_t core_mask = hvgr_pm_get_availiable_cores(gdev);
	struct hvgr_cq_dev * const gcqctx = &gdev->cq_dev;
	uint32_t bc_send_mode = gcqctx->hw_cfg.bc_send_mode;

	if (hweight32(core_mask) == 1)
		bc_send_mode = CQ_BC_SEND_INTERLEAVE;
	hvgr_write_reg(gdev,
		cq_control_reg(gdev, CQ_BC_SEND_MODE), bc_send_mode);
#endif
}

bool hvgr_cq_entry_wait_inqueue(struct hvgr_cq_ctx * const cq_ctx)
{
	bool ret;

	if (cq_ctx->wait_timeout == 0)
		return false;

	if ((cq_ctx->wait_timeout & HTS_WAIT_OVERTIME) != 0) {
		cq_ctx->wait_timeout &= ~HTS_WAIT_OVERTIME;
		ret = hvgr_hts_wait_entry_enqueue(cq_ctx);
		if (ret)
			hvgr_cq_mod_status_macro(cq_ctx, CQ_STATUS_WAITING);
		return ret;
	}
	return false;
}

#if defined(HVGR_WAITFENCE_LOWPOWER) && HVGR_WAITFENCE_LOWPOWER != 0
static bool hvgr_cq_is_wait_semaphore_set(struct hvgr_cq_ctx * const cq_ctx, int index,
	uint64_t *sem_addr_ptr, uint64_t *sem_value_ptr)
{
	struct cq_sw_writeable_context *sw_wr_ctx = cq_ctx->sw_wr_ctx;
	struct hvgr_cq_semp_info *info = NULL;
	struct hvgr_cq_ctx_data *ctx_cq = &cq_ctx->ctx->ctx_cq;
	uint32_t sem_addr_low = sw_wr_ctx->wait_entry_info[1 + index * 8];
	uint32_t base_page = cq_ctx->ctx->same_va_4g_base_page;
	uint64_t sem_addr = (((uint64_t)base_page << 32) | ((uint64_t)sem_addr_low));
	uint32_t sem_value_low = sw_wr_ctx->wait_entry_info[2 + index * 8];
	uint32_t sem_value_high = sw_wr_ctx->wait_entry_info[3 + index * 8];
	uint64_t sem_value = (((uint64_t)sem_value_high << 32) | ((uint64_t)sem_value_low));
	uint64_t addr_value;
	unsigned long flags;
	volatile uint64_t *addr = NULL;

	spin_lock_irqsave(&ctx_cq->semp_lock, flags);
	info = &ctx_cq->semp_info;
	if (unlikely(!ctx_cq->semp_res_flag || info->semp_buffer->kva == 0 ||
		info->semp_buffer->uva == 0 || info->size == 0)) {
		spin_unlock_irqrestore(&ctx_cq->semp_lock, flags);
		return true;
	}

	sem_addr = info->semp_buffer->kva + (sem_addr - info->semp_buffer->uva);
	addr = (volatile uint64_t *)u64_to_ptr(sem_addr);
	addr_value = (*((volatile uint64_t *)addr));
	hvgr_info(cq_ctx->ctx->gdev, HVGR_SCH,
		"ctx_%u, queueid_%u addr_0x%llx value_0x%llx sem_value_0x%llx",
		cq_ctx->ctx->id, cq_ctx->queue_id, sem_addr, addr_value, sem_value);
	if (addr_value == sem_value) {
		spin_unlock_irqrestore(&ctx_cq->semp_lock, flags);
		return true;
	}

	spin_unlock_irqrestore(&ctx_cq->semp_lock, flags);
	*sem_addr_ptr = sem_addr;
	*sem_value_ptr = sem_value;
	return false;
}

static int hvgr_cq_get_wait_fence_entry(struct hvgr_cq_ctx * const cq_ctx, uint32_t fence_entry_id)
{
	struct cq_sw_writeable_context *sw_wr_ctx = cq_ctx->sw_wr_ctx;
	int entry_info_id;
	int index = -1;
	uint32_t entry_id;

	for (entry_info_id = 0; entry_info_id < CQ_WAIT_ENTRY_INFO_NUM; entry_info_id++) {
		entry_id = sw_wr_ctx->wait_entry_info[5 + entry_info_id * 8];
		if (entry_id == fence_entry_id) {
			index = entry_info_id;
			break;
		}
	}

	return index;
}

static bool hvgr_cq_is_wait_fence(struct hvgr_cq_ctx * const cq_ctx, uint32_t *fence_entry_id)
{
	uint64_t *top_entry_status_addr;
	uint64_t *top_idx_addr;
	uint64_t top_idx_value;
	uint64_t top_pop_idx;
	uint64_t top_pop_entry_status;
	uint64_t top_pop_entry_type;
	uint32_t top_pop_entry_entryid;

	top_entry_status_addr = cq_ctx->hw_wr_ctx + 16;
	top_idx_addr = top_entry_status_addr + 256;
	top_idx_value = (*((volatile uint64_t *)top_idx_addr));
	top_pop_idx = (top_idx_value & 0xFFFFFFFF) & 0xFF;
	top_pop_entry_status = (*((volatile uint64_t *)(top_entry_status_addr + top_pop_idx)));
	top_pop_entry_type = (top_pop_entry_status >> 9) & 0xF;
	top_pop_entry_entryid = (uint32_t)((top_pop_entry_status >> 17) & 0x3FFF);

	hvgr_info(cq_ctx->ctx->gdev, HVGR_SCH,
		"ctx_%u, queueid_%u top_pop_idx_0x%llx entry_status_0x%llx.",
		cq_ctx->ctx->id, cq_ctx->queue_id, top_idx_value, top_pop_entry_status);

	if ((top_pop_entry_type == 1 || top_pop_entry_type == 4 || top_pop_entry_type == 6) &&
		((top_pop_entry_entryid & (1U << HVGR_WAIT_ENTRY_FENCE_FLAG)) != 0)) {
		hvgr_info(cq_ctx->ctx->gdev, HVGR_SCH, "ctx_%u, queueid_%u entry_type_%u entryid_%u.",
			cq_ctx->ctx->id, cq_ctx->queue_id, top_pop_entry_type, top_pop_entry_entryid);
		*fence_entry_id = top_pop_entry_entryid;
		return true;
	}

	return false;
}

void hvgr_cq_check_wait_entry(struct hvgr_cq_ctx * const cq_ctx, struct hvgr_device *gdev)
{
	uint64_t sem_addr = 0U;
	uint64_t sem_value = 0U;
	int index = 0;
	uint32_t fence_entry_id;

	cq_ctx->need_check_wait_entry = false;

	if (!hvgr_cq_is_wait_fence(cq_ctx, &fence_entry_id)) {
		hvgr_info(gdev, HVGR_SCH, "ctx_%u, queueid_%u is not wait fence.",
			cq_ctx->ctx->id, cq_ctx->queue_id);
		return;
	}

	index = hvgr_cq_get_wait_fence_entry(cq_ctx, fence_entry_id);
	if (index < 0) {
		hvgr_info(gdev, HVGR_SCH, "ctx_%u, queueid_%u not find wait fence.",
			cq_ctx->ctx->id, cq_ctx->queue_id);
		return;
	}

	if (hvgr_cq_is_wait_semaphore_set(cq_ctx, index, &sem_addr, &sem_value)) {
		hvgr_info(gdev, HVGR_SCH, "queueid_%u fence sem is set.", cq_ctx->queue_id);
		return;
	}

	cq_ctx->need_waiting_fence = true;
	cq_ctx->wait_fence_sem_addr = sem_addr;
	cq_ctx->wait_fence_sem_value = sem_value;
	hvgr_info(gdev, HVGR_SCH, "ctx_%u, queueid_%u need enqueue fence_waiting_list.",
		cq_ctx->ctx->id, cq_ctx->queue_id);
}
#endif

#if hvgr_version_ge(HVGR_V500)
static bool hvgr_cq_alloc_umd_writeable_memory(struct hvgr_ctx * const ctx,
	struct hvgr_cq_ctx * const cq_ctx)
{
	uint64_t size = 0;
	struct hvgr_mem_area *umd_writeable_area = NULL;
	struct hvgr_mem_alloc_para para;

	size = hvgr_mem_page_align(cq_ringbuf_align(sizeof(struct cq_umd_sw_writeable_context)));

	para.property = HVGR_MEM_PROPERTY_4G_VA;
	para.attribute = HVGR_MEM_ATTR_CPU_RD | HVGR_MEM_ATTR_CPU_WR |
		HVGR_MEM_ATTR_GPU_RD | HVGR_MEM_ATTR_GPU_WR | HVGR_MEM_MASK_KMD_ACCESS;
	para.pages = hvgr_page_nums(size);
	para.max_pages = para.pages;

	umd_writeable_area = hvgr_mem_allocate(ctx, &para, NULL, false);
	if (umd_writeable_area == NULL) {
		hvgr_err(ctx->gdev, HVGR_MEM, "%s alloc umd_writeable_area failed.", __func__);
		return false;
	}

	cq_ctx->sw_ctx_umd_area = umd_writeable_area;

	cq_ctx->sw_ctx_addr = umd_writeable_area->uva;
	cq_ctx->sw_umd_wr_ctx = u64_to_ptr(umd_writeable_area->kva);
	cq_ctx->sw_wr_ctx->umd_sw_context_address_l = umd_writeable_area->uva & 0xFFFFFFFFU;
	cq_ctx->sw_wr_ctx->umd_sw_context_address_h = umd_writeable_area->uva >> 32;

	hvgr_info(ctx->gdev, HVGR_MEM, "ctx_%u %s gpu memory uva=0x%lx, kva=0x%lxend ",
		ctx->id, __func__, umd_writeable_area->uva, umd_writeable_area->kva);
	return true;
}

bool hvgr_cq_alloc_umd_memory(struct hvgr_ctx * const ctx,
	struct hvgr_cq_ctx * const cq_ctx)
{
	/* alloc umd sw writeable memory */
	if (!hvgr_cq_alloc_umd_writeable_memory(ctx, cq_ctx)) {
		hvgr_err(ctx->gdev, HVGR_CQ, "alloc umd writeable fail ctx[%u].", ctx->id);
		return false;
	}

	return true;
}

void hvgr_cq_free_umd_memory(struct hvgr_ctx * const ctx,
	struct hvgr_cq_ctx * const cq_ctx)
{
	if (unlikely((ctx == NULL) || (cq_ctx == NULL)))
		return;

	hvgr_mem_free(ctx, cq_ctx->sw_ctx_addr);
	cq_ctx->sw_ctx_addr = 0;
	cq_ctx->sw_ctx_umd_area = NULL;
}

static bool hvgr_cq_alloc_hw_writeable_memory(struct hvgr_ctx * const ctx,
	struct hvgr_cq_ctx * const cq_ctx)
{
	uint64_t size = CQ_HW_WRITEABLE_CONTEXT_SIZE;
	struct hvgr_mem_area *hw_writeable_area = NULL;
	struct hvgr_mem_alloc_para para;

	para.property = HVGR_MEM_PROPERTY_GPU_MEM;
	para.attribute = HVGR_MEM_ATTR_GPU_RD | HVGR_MEM_ATTR_GPU_WR;
	para.pages = hvgr_page_nums(size);
	para.max_pages = para.pages;

	hw_writeable_area = hvgr_mem_allocate(ctx, &para, NULL, false);
	if (hw_writeable_area == NULL) {
		hvgr_err(ctx->gdev, HVGR_MEM, "%s alloc hw_writeable_area failed.", __func__);
		return false;
	}

	if (!hvgr_mem_map_kva(hw_writeable_area)) {
		if (hw_writeable_area->zone->opts->free)
			hw_writeable_area->zone->opts->free(hw_writeable_area);
		return false;
	}
	atomic_add((int)para.pages, &ctx->mem_ctx.used_kmd_pages);
	(void)memset_s(u64_to_ptr(hw_writeable_area->kva), size, 0, size);
	cq_ctx->sw_ctx_hw_area = hw_writeable_area;

	cq_ctx->sw_wr_ctx->hw_context_address = hw_writeable_area->uva;

	hvgr_info(ctx->gdev, HVGR_MEM, "ctx_%u %s gpu memory uva=0x%lx, kva=0x%lxend ",
		ctx->id, __func__, hw_writeable_area->uva, hw_writeable_area->kva);
	return true;
}

bool hvgr_cq_alloc_hw_memory(struct hvgr_ctx * const ctx,
	struct hvgr_cq_ctx * const cq_ctx)
{
	/* alloc hw writeable memory */
	if (!hvgr_cq_alloc_hw_writeable_memory(ctx, cq_ctx)) {
		hvgr_err(ctx->gdev, HVGR_CQ, "alloc hw writeable fail ctx[%u].", ctx->id);
		return false;
	}

	return true;
}

static void hvgr_cq_free_hw_writeable_memory(struct hvgr_cq_ctx * const cq_ctx,
	struct cq_sw_writeable_context *sw_wr_ctx)
{
	uint64_t size = CQ_HW_WRITEABLE_CONTEXT_SIZE;

	if (unlikely(sw_wr_ctx == NULL))
		return;

	if (likely(cq_ctx->sw_ctx_hw_area != NULL)) {
		hvgr_mem_unmap_kva(cq_ctx->sw_ctx_hw_area);
		cq_ctx->sw_ctx_hw_area = NULL;
	}

	if (unlikely(cq_ctx->ctx == NULL))
		return;

	hvgr_mem_free(cq_ctx->ctx, sw_wr_ctx->hw_context_address);
	sw_wr_ctx->hw_context_address = 0;
	atomic_sub((int)hvgr_page_nums(size), &cq_ctx->ctx->mem_ctx.used_kmd_pages);
}

void hvgr_cq_free_hw_memory(struct hvgr_ctx * const ctx,
	struct hvgr_cq_ctx * const cq_ctx)
{
	if (unlikely((ctx == NULL) || (cq_ctx == NULL)))
		return;

	hvgr_cq_free_hw_writeable_memory(cq_ctx, cq_ctx->sw_wr_ctx);
}

static void hvgr_cq_fill_ring_buffer(struct cq_sw_writeable_context *sw_wr_ctx, uint64_t addr)
{
	if (sw_wr_ctx == NULL)
		return;

	sw_wr_ctx->ring_buffer.rend_ring_buf_struct_ptr = addr;
	sw_wr_ctx->ring_buffer.rend_ring_buf_struct_size = CQ_REND_RING_BUF_SIZE / sizeof(uint32_t);
	addr += CQ_REND_RING_BUF_SIZE;

	sw_wr_ctx->ring_buffer.bin_ring_buf_struct_ptr = addr;
	sw_wr_ctx->ring_buffer.bin_ring_buf_struct_size = CQ_BIN_RING_BUF_SIZE / sizeof(uint32_t);
	addr += CQ_BIN_RING_BUF_SIZE;

	sw_wr_ctx->ring_buffer.comp_ring_buf_struct_ptr = addr;
	sw_wr_ctx->ring_buffer.comp_ring_buf_struct_size = CQ_COMPUTER_RING_BUF_SIZE / sizeof(uint32_t);
	addr += CQ_COMPUTER_RING_BUF_SIZE;

	sw_wr_ctx->ring_buffer.bvhg_ring_buf_struct_ptr = addr;
	sw_wr_ctx->ring_buffer.bvhg_ring_buf_struct_size = CQ_BVHG_RING_BUF_SIZE / sizeof(uint32_t);
	addr += CQ_BVHG_RING_BUF_SIZE;

	sw_wr_ctx->ring_buffer.rt_ring_buf_struct_ptr = addr;
	sw_wr_ctx->ring_buffer.rt_ring_buf_struct_size = CQ_RT_RING_BUF_SIZE / sizeof(uint32_t);
}

static bool hvgr_cq_alloc_hw_ringbuffer_memory(struct hvgr_ctx * const ctx,
	struct hvgr_cq_ctx * const cq_ctx)
{
	uint64_t size = CQ_HW_RINGBUF_SIZE;
	struct hvgr_mem_area *hw_ringbuffer_area = NULL;
	struct hvgr_mem_alloc_para para;

	para.property = HVGR_MEM_PROPERTY_GPU_MEM;
	para.attribute = HVGR_MEM_ATTR_GPU_RD | HVGR_MEM_ATTR_GPU_WR;
	para.pages = hvgr_page_nums(size);
	para.max_pages = para.pages;

	hw_ringbuffer_area = hvgr_mem_allocate(ctx, &para, NULL, false);
	if (hw_ringbuffer_area == NULL) {
		hvgr_err(ctx->gdev, HVGR_MEM, "%s alloc hw_ringbuffer_area failed.", __func__);
		return false;
	}

	if (!hvgr_mem_map_kva(hw_ringbuffer_area)) {
		if (hw_ringbuffer_area->zone->opts->free)
			hw_ringbuffer_area->zone->opts->free(hw_ringbuffer_area);
		return false;
	}
	atomic_add((int)para.pages, &ctx->mem_ctx.used_kmd_pages);
	(void)memset_s(u64_to_ptr(hw_ringbuffer_area->kva), size, 0, size);
	cq_ctx->sw_ctx_hw_ring_area = hw_ringbuffer_area;

	hvgr_cq_fill_ring_buffer(cq_ctx->sw_wr_ctx, hw_ringbuffer_area->uva);

	hvgr_info(ctx->gdev, HVGR_MEM, "ctx_%u %s gpu memory uva=0x%lx, kva=0x%lxend ",
		ctx->id, __func__, hw_ringbuffer_area->uva, hw_ringbuffer_area->kva);
	return true;
}

bool hvgr_cq_alloc_hw_ring_memory(struct hvgr_ctx * const ctx,
	struct hvgr_cq_ctx * const cq_ctx)
{
	/* alloc hw ringbuffer memory */
	if (!hvgr_cq_alloc_hw_ringbuffer_memory(ctx, cq_ctx)) {
		hvgr_err(ctx->gdev, HVGR_CQ, "alloc hw ringbuffer fail ctx[%u].", ctx->id);
		return false;
	}

	return true;
}

static void hvgr_cq_free_hw_ringbuffer_memory(struct hvgr_cq_ctx * const cq_ctx,
	struct cq_sw_writeable_context *sw_wr_ctx)
{
	uint64_t size = CQ_HW_RINGBUF_SIZE;

	if (unlikely(sw_wr_ctx == NULL))
		return;

	if (likely(cq_ctx->sw_ctx_hw_ring_area != NULL)) {
		hvgr_mem_unmap_kva(cq_ctx->sw_ctx_hw_ring_area);
		cq_ctx->sw_ctx_hw_ring_area = NULL;
	}

	if (unlikely(cq_ctx->ctx == NULL))
		return;

	hvgr_mem_free(cq_ctx->ctx, sw_wr_ctx->ring_buffer.rend_ring_buf_struct_ptr);
	atomic_sub((int)hvgr_page_nums(size), &cq_ctx->ctx->mem_ctx.used_kmd_pages);
}

void hvgr_cq_free_hw_ring_memory(struct hvgr_ctx * const ctx,
	struct hvgr_cq_ctx * const cq_ctx)
{
	if (unlikely((ctx == NULL) || (cq_ctx == NULL)))
		return;

	hvgr_cq_free_hw_ringbuffer_memory(cq_ctx, cq_ctx->sw_wr_ctx);
}

static void hvgr_cq_fill_memfree_ring_buffer(struct cq_sw_writeable_context *sw_wr_ctx, uint64_t addr)
{
	if (sw_wr_ctx == NULL)
		return;

	sw_wr_ctx->memfree_ring_buffer.rend_ring_buf_struct_ptr = addr;
	sw_wr_ctx->memfree_ring_buffer.rend_ring_buf_struct_size = CQ_REND_MEMFREE_RING_BUF_SIZE / sizeof(uint32_t);
	addr += CQ_REND_MEMFREE_RING_BUF_SIZE;

	sw_wr_ctx->memfree_ring_buffer.bin_ring_buf_struct_ptr = addr;
	sw_wr_ctx->memfree_ring_buffer.bin_ring_buf_struct_size = CQ_BIN_MEMFREE_RING_BUF_SIZE / sizeof(uint32_t);
	addr += CQ_BIN_MEMFREE_RING_BUF_SIZE;

	sw_wr_ctx->memfree_ring_buffer.comp_ring_buf_struct_ptr = addr;
	sw_wr_ctx->memfree_ring_buffer.comp_ring_buf_struct_size = CQ_COMPUTER_MEMFREE_RING_BUF_SIZE / sizeof(uint32_t);
	addr += CQ_COMPUTER_MEMFREE_RING_BUF_SIZE;

	sw_wr_ctx->memfree_ring_buffer.bvhg_ring_buf_struct_ptr = addr;
	sw_wr_ctx->memfree_ring_buffer.bvhg_ring_buf_struct_size = CQ_BVHG_MEMFREE_RING_BUF_SIZE / sizeof(uint32_t);
	addr += CQ_BVHG_MEMFREE_RING_BUF_SIZE;

	sw_wr_ctx->memfree_ring_buffer.rt_ring_buf_struct_ptr = addr;
	sw_wr_ctx->memfree_ring_buffer.rt_ring_buf_struct_size = CQ_RT_MEMFREE_RING_BUF_SIZE / sizeof(uint32_t);
}

static bool hvgr_cq_alloc_memfree_ringbuffer_memory(struct hvgr_ctx * const ctx,
	struct hvgr_cq_ctx * const cq_ctx)
{
	uint64_t size = CQ_HW_MEMFREE_RINGBUF_SIZE;
	struct hvgr_mem_area *memfree_ringbuffer_area = NULL;
	struct hvgr_mem_alloc_para para;

	para.property = HVGR_MEM_PROPERTY_GPU_MEM;
	para.attribute = HVGR_MEM_ATTR_GPU_RD | HVGR_MEM_ATTR_GPU_WR;
	para.pages = hvgr_page_nums(size);
	para.max_pages = para.pages;

	memfree_ringbuffer_area = hvgr_mem_allocate(ctx, &para, NULL, false);
	if (memfree_ringbuffer_area == NULL) {
		hvgr_err(ctx->gdev, HVGR_MEM, "%s alloc memfree_ringbuffer_area failed.", __func__);
		return false;
	}

	if (!hvgr_mem_map_kva(memfree_ringbuffer_area)) {
		if (memfree_ringbuffer_area->zone->opts->free)
			memfree_ringbuffer_area->zone->opts->free(memfree_ringbuffer_area);
		return false;
	}
	atomic_add((int)para.pages, &ctx->mem_ctx.used_kmd_pages);
	(void)memset_s(u64_to_ptr(memfree_ringbuffer_area->kva), size, 0, size);
	cq_ctx->sw_ctx_memfree_ring_area = memfree_ringbuffer_area;

	hvgr_cq_fill_memfree_ring_buffer(cq_ctx->sw_wr_ctx, memfree_ringbuffer_area->uva);

	hvgr_info(ctx->gdev, HVGR_MEM, "ctx_%u %s gpu memory uva=0x%lx, kva=0x%lxend ",
		ctx->id, __func__, memfree_ringbuffer_area->uva, memfree_ringbuffer_area->kva);
	return true;
}

bool hvgr_cq_alloc_memfree_ring_memory(struct hvgr_ctx * const ctx,
	struct hvgr_cq_ctx * const cq_ctx)
{
	if (!hvgr_cq_alloc_memfree_ringbuffer_memory(ctx, cq_ctx)) {
		hvgr_err(ctx->gdev, HVGR_CQ, "alloc hw memfree ringbuffer fail ctx[%u].", ctx->id);
		return false;
	}

	return true;
}

static void hvgr_cq_free_memfree_ringbuffer_memory(struct hvgr_cq_ctx * const cq_ctx,
	struct cq_sw_writeable_context *sw_wr_ctx)
{
	uint64_t size = CQ_HW_MEMFREE_RINGBUF_SIZE;

	if (unlikely(sw_wr_ctx == NULL))
		return;

	if (likely(cq_ctx->sw_ctx_memfree_ring_area != NULL)) {
		hvgr_mem_unmap_kva(cq_ctx->sw_ctx_memfree_ring_area);
		cq_ctx->sw_ctx_memfree_ring_area = NULL;
	}

	if (unlikely(cq_ctx->ctx == NULL))
		return;

	hvgr_mem_free(cq_ctx->ctx, sw_wr_ctx->memfree_ring_buffer.rend_ring_buf_struct_ptr);
	atomic_sub((int)hvgr_page_nums(size), &cq_ctx->ctx->mem_ctx.used_kmd_pages);
}

void hvgr_cq_free_memfree_ring_memory(struct hvgr_ctx * const ctx,
	struct hvgr_cq_ctx * const cq_ctx)
{
	if (unlikely((ctx == NULL) || (cq_ctx == NULL)))
		return;

	hvgr_cq_free_memfree_ringbuffer_memory(cq_ctx, cq_ctx->sw_wr_ctx);
}

#endif
