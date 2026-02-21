/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2021-2022. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */

#include "hvgr_cq_schedule.h"

#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/list.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/moduleparam.h>
#include <linux/completion.h>
#include <linux/ktime.h>
#include <securec.h>
#include <linux/bitmap.h>

#include "hvgr_regmap.h"

#include "apicmd/hvgr_kmd_defs.h"
#include "hvgr_dm_api.h"
#include "hvgr_mmu_api.h"
#include "mem/hvgr_as.h"
#include "hvgr_mem_api.h"
#include "hvgr_asid_api.h"
#include "hvgr_log_api.h"
#include "hvgr_pm_api.h"
#include "hvgr_datan_api.h"
#include "hvgr_platform_api.h"

#include "hvgr_cq_driver_base.h"
#include "hvgr_wq.h"
#include "hvgr_msync.h"
#include "hvgr_softjob.h"
#include "hvgr_cq_queue.h"
#include "hvgr_cq_debug_switch.h"
#include "hvgr_cq.h"
#include "hvgr_assert.h"
#include "hvgr_sch_policy_manager.h"
#include "hvgr_sch_dfx.h"
#include "hvgr_cq_driver_adapt.h"
#include "hvgr_protect_mode.h"
#include "hvgr_hts_api.h"
#include "hvgr_cq_schedule_submit.h"
#include "hvgr_mem_context_switchover.h"
#if defined(HVGR_STATE_BUNDLE2) && HVGR_STATE_BUNDLE2 > 0
#include "hvgr_mcu_common.h"
#endif
#if hvgr_version_lt(HVGR_V500)
#include "hvgr_cq_swctx.h"
#else
#include "hvgr_cq_swctx_divide.h"
#endif

#define HVGR_HAS_SUBMIT       1
#define HVGR_NO_SUBMIT        0

void hvgr_cq_schedule_channel_start_no_check(struct hvgr_device * const gdev, uint32_t channel_id)
{
	struct hvgr_cq_ctx *next_queue = NULL;

	next_queue = hvgr_cq_get_available_queue(gdev);
	if (next_queue != NULL) {
#if defined(HVGR_SUPPORT_SPEC_LOW) && HVGR_SUPPORT_SPEC_LOW != 0
		if (next_queue->priority == CQ_PRIORITY_SPECIAL_LOW) {
			hvgr_info(gdev, HVGR_SPEC_LOW, "%s gpu_sch_sts=%u, ctx_%u,qid=%u,priv=%u\n",
				__func__, gdev->cq_dev.gpu_sch_sts, next_queue->ctx->id,
				next_queue->queue_id, next_queue->priority);
			if (gdev->cq_dev.gpu_sch_sts == HVGR_GPU_IDLE)
				hvgr_cq_schedule_cq_in(next_queue, 0);
			else
				hvgr_cq_schedule_enqueue(&gdev->cq_dev, next_queue);
			return;
		}
#endif
		hvgr_cq_schedule_cq_in(next_queue, channel_id);
	}
}

void hvgr_cq_schedule_channel_start(struct hvgr_device * const gdev, uint32_t channel_id)
{
	struct hvgr_cq_dev * const gcqctx = &gdev->cq_dev;
	struct hvgr_cq_ctx *cq_ctx = NULL;

	if (unlikely((!gcqctx->submit_allow) || (!gcqctx->channel_info[channel_id].available)) ||
		unlikely(hvgr_pm_get_gpu_status(gdev) != HVGR_PM_GPU_READY))
		return;

	cq_ctx = gcqctx->channel_info[channel_id].running_queue;
	if (cq_ctx != NULL) {
		hvgr_err(gdev, HVGR_CQ, "%s cqctx->flags=0x%x\n", __func__, cq_ctx->flags);
		return;
	}

	hvgr_cq_schedule_channel_start_no_check(gdev, channel_id);
}

void hvgr_cq_schedule_channel_start_all(struct hvgr_device * const gdev)
{
	uint32_t chn;
	for (chn = 0; chn < CQ_CHANNEL_NUM; chn++) {
		hvgr_cq_schedule_channel_start(gdev, chn);
#if defined(HVGR_SUPPORT_SPEC_LOW) && HVGR_SUPPORT_SPEC_LOW != 0
		if (gdev->cq_dev.cq_spec_low != NULL)
			return;
#endif
	}
}

#if defined(HVGR_SUPPORT_SPEC_LOW) && HVGR_SUPPORT_SPEC_LOW != 0
static void hvgr_cq_schedule_channel_start_all_no_check(struct hvgr_device * const gdev)
{
	uint32_t chn;
	for (chn = 0; chn < CQ_CHANNEL_NUM; chn++) {
		hvgr_cq_schedule_channel_start_no_check(gdev, chn);
		if (gdev->cq_dev.cq_spec_low != NULL)
			return;
	}
}

static bool hvgr_cq_schedule_queue_is_empty_expect_spec_low(struct hvgr_cq_dev * const gcqctx)
{
	struct cq_pending *pending_queue = NULL;
	int idx;
#if hvgr_version_lt(HVGR_V300)
	int pri_low_idx = CQ_PRIORITY_LOW;
#else
	int pri_low_idx = CQ_PRIORITY_SPECIAL_LOW + 1;
#endif

	for (idx = CQ_PRIORITY_HIGH; idx >= pri_low_idx; idx--) {
		pending_queue = &gcqctx->pending_queue[idx];
		if (!(list_empty(&pending_queue->pending_list)))
			return false;
	}

	return true;
}
#endif

bool hvgr_cq_schedule_queue_is_empty(struct hvgr_cq_dev * const gcqctx)
{
	struct cq_pending *pending_queue = NULL;
	uint32_t idx;

	for (idx = CQ_PRIORITY_MAX; idx > 0; idx--) {
		pending_queue = &gcqctx->pending_queue[idx - 1];
		if (!(list_empty(&pending_queue->pending_list)))
			return false;
	}

	return true;
}

static inline struct hvgr_cq_ctx *hvgr_cq_get_channel_ctx(struct hvgr_device *gdev,
	uint32_t channel_id)
{
	struct hvgr_cq_dev * const gcqctx = &gdev->cq_dev;

	return gcqctx->channel_info[channel_id].running_queue;
}

#ifdef CONFIG_HVGR_DFX_SH
static void hvgr_cq_set_dynamic_affinity(struct hvgr_cq_ctx * const cq_ctx,
	uint32_t channel_id)
{
	struct hvgr_ctx *ctx = cq_ctx->ctx;
	struct hvgr_device * const gdev = ctx->gdev;
	uint32_t org_mask = hvgr_pm_get_availiable_cores(gdev);
	struct gpu_dmd_msg *dmd_msg = gdev->dm_dev.dmd_msg;
	uint32_t dmd_gaf = (uint32_t)atomic_read(&dmd_msg->dmd_gaf);
	uint32_t core_mask;

	if (!dmd_msg->enable_sh_act || !dmd_msg->enable_sh_gaf)
		return;

	if (likely((ctx->gaf_flag == 0) || (dmd_gaf == 0)))
		goto end;

	core_mask = org_mask & (~dmd_gaf);
	if (core_mask != 0) {
		hvgr_cq_set_affinity(gdev, channel_id, core_mask);
		return;
	}

end:
	hvgr_cq_set_affinity(gdev, channel_id, org_mask);
}
#endif

static void hvgr_cq_schedule_update_gpu_status(struct hvgr_device *gdev)
{
	bool gpu_busy = false;
	struct hvgr_cq_dev * const gcqctx = &gdev->cq_dev;
	ktime_t now;

	now = ktime_get();
	if (gcqctx->gpu_sch_sts != HVGR_GPU_IDLE)
		gpu_busy = true;
	hvgr_pm_change_gpu_active_status(gdev, now, gpu_busy);
}

static void hvgr_cq_bind_channel(struct hvgr_cq_ctx * const cq_ctx, uint32_t channel_id)
{
	cq_ctx->running_counter = CQ_SCH_TIMEOUT_ROBIN;
	struct hvgr_cq_dev * const gcqctx = &cq_ctx->ctx->gdev->cq_dev;

#ifdef CONFIG_HVGR_DFX_SH
	hvgr_cq_set_dynamic_affinity(cq_ctx, channel_id);
#endif
	hvgr_cq_bind_physical_chan(cq_ctx, channel_id);


	hvgr_datan_jt_record_phychn_status(cq_ctx->ctx, channel_id, HVGR_TIMELINE_PHY_CHN_BIND,
		cq_ctx->queue_id);
	hvgr_cq_mod_status_macro(cq_ctx, CQ_STATUS_RUNNING);
	gcqctx->gpu_sch_sts = HVGR_GPU_RUNNING;
	cq_ctx->ctx->gdev->time_record.gpu_running = (uint64_t)ktime_to_ns(ktime_get());
	hvgr_cq_schedule_update_gpu_status(cq_ctx->ctx->gdev);
	gcqctx->channel_info[channel_id].timer_should_run = true;
	gcqctx->channel_info[channel_id].timer_running_flag = true;
	hrtimer_start(&gcqctx->channel_info[channel_id].schedule_timer,
		ns_to_ktime(((u64)(gcqctx->hw_cfg.cq_schedule_unbind_time)) * 1000U),
		HRTIMER_MODE_REL);
	hvgr_cq_sched_update_dvfs_status(cq_ctx, HVGR_THRO_NORMAL, HVGR_THRO_ACTION_BIND);
	hvgr_update_freq_hint(cq_ctx->ctx->gdev);
}

void hvgr_cq_unbind_channel(struct hvgr_cq_ctx * const cq_ctx,
	uint32_t channel_id, uint32_t sche_timer_flag)
{
	struct hrtimer *schedule_timer = NULL;
	struct hvgr_ctx *ctx = cq_ctx->ctx;
	struct hvgr_cq_dev * const gcqctx = &ctx->gdev->cq_dev;
	struct cq_channel *cq_chan = &gcqctx->channel_info[channel_id];
	int ret;

	hvgr_cq_mod_status_macro(cq_ctx, CQ_STATUS_STOPPING);
	cq_ctx->cl_tried = false;

	cq_ctx->put_index_unbind = hvgr_get_queue_put_index(cq_ctx);
	cq_ctx->get_index_unbind = hvgr_get_queue_get_index(cq_ctx);
	cq_ctx->empty_flag = hvgr_cq_get_empty_flag(ctx->gdev, channel_id);
	cq_ctx->unbind_time = ktime_get();
	cq_ctx->subchn_os = hvgr_read_reg(ctx->gdev,
		cq_control_reg(ctx->gdev, cq_bind_subchn_os(channel_id)));
	hvgr_info(ctx->gdev, HVGR_CQ, "%s ctx_%u qid=%u chn=%u, priv=%u\n",
		__func__, ctx->id, cq_ctx->queue_id, channel_id, cq_ctx->priority);
	hvgr_datan_jobtrace(ctx, JOBTRACE_TYPE_OBJ, KDATAN_TL_WRITE_REG, ctx,
		datan_cq_control_reg(ctx->gdev, cq_bind_cmd(channel_id)), CQ_CMD_STOP);
	hvgr_write_reg(ctx->gdev, cq_control_reg(ctx->gdev, cq_bind_cmd(channel_id)), CQ_CMD_STOP);

	hvgr_datan_jt_record_phychn_status(cq_ctx->ctx, channel_id, HVGR_TIMELINE_PHY_CHN_UNBIND,
		cq_ctx->queue_id);
	if (likely((sche_timer_flag & CQ_NEED_UNBIND_TIMER_FLAG) != 0))
		(void)mod_timer(&cq_ctx->backup_timer,
			jiffies + gcqctx->reset_timeout_normal * HZ / CQ_TIME_S2MS);

	schedule_timer = &cq_chan->schedule_timer;
	ret = hrtimer_try_to_cancel(schedule_timer);
	if (ret >= 0)
		cq_chan->timer_running_flag = false;
	cq_chan->timer_should_run = false;
}

void hvgr_cq_unbind_all_channel(struct hvgr_device *gdev)
{
	uint32_t chn;

	hvgr_info(gdev, HVGR_CQ, "%s\n", __func__);
	for (chn = 0; chn < CQ_CHANNEL_NUM; chn++) {
		struct hvgr_cq_ctx *cq_ctx = hvgr_cq_get_channel_ctx(gdev, chn);
		struct hvgr_ctx *ctx = NULL;

		if (cq_ctx == NULL)
			continue;

		ctx = cq_ctx->ctx;
		hvgr_info(gdev, HVGR_CQ,
			"%s channel(%d) qid=%u,ctx_%u, kcts->tpid=%d, kcts->pid=%d\n",
			__func__, chn, cq_ctx->queue_id, cq_ctx->ctx->id, cq_ctx->ctx->tgid,
			cq_ctx->ctx->pid);

		hvgr_info(gdev, HVGR_CQ, "%s ctx_%u, cqctx->status=%d\n",
			__func__, ctx->id, cq_ctx->status);
		switch (cq_ctx->status) {
		case CQ_STATUS_RUNNING:
			hvgr_info(gdev, HVGR_CQ, "%s ctx_%u, cqctx->flags=0x%x\n",
				__func__, ctx->id, cq_ctx->flags);
			hvgr_cq_unbind_channel(cq_ctx, cq_ctx->channel_id,
				CQ_NEED_UNBIND_TIMER_FLAG);
			break;
		case CQ_STATUS_PENDING:
		case CQ_STATUS_WAITING:
		case CQ_STATUS_DIVE:
		case CQ_STATUS_IDLE:
		case CQ_STATUS_TIMEOUT:
		case CQ_STATUS_ERROR:
			/* not possible. */
			hvgr_err(gdev, HVGR_CQ, "%s ctx_%u, cqctx->status=%d, not right.\n",
				__func__, ctx->id, cq_ctx->status);
			break;
		case CQ_STATUS_STOPPING:
		case CQ_STATUS_STOPPED:
			/* if queue just in stopping, we just wait for irq or clear. */
			break;
		}
	}
}

static bool hvgr_cq_check_entry_timeout(struct hvgr_cq_ctx *cq_ctx, struct hvgr_cq_dev *gcqctx)
{
	if (!gcqctx->entry_timeout_switch)
		return false;
	if(hvgr_is_cl_task(cq_ctx->ctx->gdev, cq_ctx->channel_id)) {
		if ((cq_ctx->semp_timeout_counter) >=
			((gcqctx->reset_timeout_cl / CQ_TIME_MS2TICK) +
			(gcqctx->cq_entry_timeout_cl / CQ_TIME_MS2TICK))) {
			hvgr_debug(cq_ctx->ctx->gdev, HVGR_CQ, "task is cl task");
			hvgr_debug(cq_ctx->ctx->gdev, HVGR_CQ,
				"detection time: %u ms,[%ums][%ums]\n",
				gcqctx->reset_timeout_cl + gcqctx->cq_entry_timeout_cl,
				gcqctx->reset_timeout_cl, gcqctx->cq_entry_timeout_cl);
			return true;
		}
		return false;
	}
	if ((cq_ctx->semp_timeout_counter) >=
		((gcqctx->reset_timeout_normal / CQ_TIME_MS2TICK) +
		(gcqctx->cq_entry_timeout / CQ_TIME_MS2TICK))) {
		hvgr_debug(cq_ctx->ctx->gdev, HVGR_CQ, "task is not cl task");
		hvgr_debug(cq_ctx->ctx->gdev, HVGR_CQ,
			"detection time: %u ms,[%ums][%ums]\n",
			gcqctx->reset_timeout_normal + gcqctx->cq_entry_timeout,
			gcqctx->reset_timeout_normal, gcqctx->cq_entry_timeout);
		return true;
	}
	return false;
}

static int hvgr_cq_collect_count(struct hvgr_cq_ctx *cq_ctx, struct hvgr_cq_dev *gcqctx)
{
	struct hvgr_device *gdev = NULL;

	gdev = container_of(gcqctx, struct hvgr_device, cq_dev);
	if (cq_ctx->running_counter > 0) {
		if (cq_ctx->get_index == hvgr_get_queue_get_index(cq_ctx)) {
			cq_ctx->running_counter--;
			cq_ctx->semp_timeout_counter++;
		} else {
			cq_ctx->get_index = hvgr_get_queue_get_index(cq_ctx);
			cq_ctx->running_counter = CQ_SCH_TIMEOUT_ROBIN;
			cq_ctx->semp_timeout_counter = 0;
		}
	}

	if (hvgr_cq_check_entry_timeout(cq_ctx, gcqctx)) {
		cq_ctx->semp_timeout_counter = 0;
		hvgr_err(gdev, HVGR_CQ,
			"CQ entry timeout tgid: %d, pid: %d, qid: %u, process_name: %s,"
			"group_name: %s\n, b_status: 0x%x, q_status: 0x%x\n",
			cq_ctx->ctx->tgid, cq_ctx->ctx->pid, cq_ctx->queue_id,
			cq_ctx->ctx->process_name, cq_ctx->ctx->group_name,
			hvgr_get_bind_status(gdev, cq_ctx->channel_id),
			hvgr_cq_get_queue_status(gdev, cq_ctx->channel_id));
		if (cq_ctx->entry_timeout_get_index != hvgr_get_queue_get_index(cq_ctx) ||
			cq_ctx->entry_timeout_put_index != hvgr_get_queue_put_index(cq_ctx)) {
			cq_ctx->entry_timeout_get_index = hvgr_get_queue_get_index(cq_ctx);
			cq_ctx->entry_timeout_put_index = hvgr_get_queue_put_index(cq_ctx);
			hvgr_err(gdev, HVGR_CQ, "put idx : %d, get idx : %d",
				hvgr_get_queue_get_index(cq_ctx),
				hvgr_get_queue_put_index(cq_ctx));
		}
		hvgr_cq_fault_print_cq_info(cq_ctx->ctx);
		if (gcqctx->semp_timeout_enable || gcqctx->protect_mode_info.protected_mode) {
			cq_ctx->flags |= CQ_KMD_CTX_ENTRY_TIMEOUT;
			hvgr_datan_gpu_fault_type_set(gdev, DMD_ENTRY_TIMEOUT, "Cq entry timeout");
			hvgr_pm_gpu_reset(gdev, GPU_RESET_TYPE_NORMAL);
			return -1;
		}
	}
	return 0;
}

static void hvgr_cq_check_running_counter(struct hvgr_cq_ctx *cq_ctx, struct hvgr_cq_dev *gcqctx,
	bool empty, uint32_t chn, struct hrtimer *timer)
{
	struct hvgr_device *gdev = NULL;
	uint32_t value;

	gdev = container_of(gcqctx, struct hvgr_device, cq_dev);
	if ((cq_ctx->running_counter == 0) || !empty) {
		hvgr_info(gdev, HVGR_SCH,
			"[cq]: %s empty(%d), ctx_%u(%d), (%d:%d), s(%d), run_cnt(%d).",
			__func__, empty, cq_ctx->ctx->id, cq_ctx->queue_id,
			hvgr_get_queue_put_index(cq_ctx), hvgr_get_queue_get_index(cq_ctx),
			cq_ctx->status, cq_ctx->running_counter);

		if (unlikely(gcqctx->gpu_sch_sts == HVGR_GPU_IDLE))
			hvgr_err(gdev, HVGR_SCH, "%s chn(%d) gpu idle, not right\n", __func__, chn);
		value = hvgr_get_bind_status(gdev, chn);
		hvgr_info(gdev, HVGR_SCH, "%s chn(%d) cq_bind_status_0(0x%x)\n",
			__func__, chn, value);
		hvgr_cq_unbind_channel(cq_ctx, chn, CQ_NEED_UNBIND_TIMER_FLAG);
		gcqctx->channel_info[chn].timer_running_flag = false;
		/* if hardware not response, here must prepare reset. */
	} else {
		gcqctx->channel_info[chn].timer_should_run = true;
		hrtimer_start(timer,
			ns_to_ktime(((u64)(gcqctx->hw_cfg.cq_schedule_unbind_time)) * 1000U),
			HRTIMER_MODE_REL);
	}
}

static enum hrtimer_restart kmd_cq_schedule_timer(struct hrtimer *timer)
{
	unsigned long flags;
	struct hvgr_device *gdev = NULL;
	struct hvgr_cq_dev *gcqctx = NULL;
	uint32_t chn;
	struct cq_channel *channel_info = NULL;
	struct hvgr_cq_ctx *cq_ctx = NULL;
	bool empty = false;

	channel_info = container_of(timer, struct cq_channel, schedule_timer);
	gcqctx = channel_info->global_cq_data;
	gdev = container_of(gcqctx, struct hvgr_device, cq_dev);
	chn = channel_info->channel_id;

	spin_lock_irqsave(&gdev->cq_dev.schedule_lock, flags);
	if (!gcqctx->channel_info[chn].timer_should_run) {
		hvgr_info(gdev, HVGR_CQ, "%s chn(%d) timer should stop\n", __func__, chn);
		gcqctx->channel_info[chn].timer_running_flag = false;
		spin_unlock_irqrestore(&gdev->cq_dev.schedule_lock, flags);
		return HRTIMER_NORESTART;
	}

	cq_ctx = hvgr_cq_get_channel_ctx(gdev, chn);
	if ((cq_ctx == NULL) ||
		(cq_ctx->ctx == NULL) ||
		(cq_ctx->ctx->gdev == NULL)) {
		hvgr_err(gdev, HVGR_CQ, "%s null param\n", __func__);
		gcqctx->channel_info[chn].timer_running_flag = false;
		spin_unlock_irqrestore(&gdev->cq_dev.schedule_lock, flags);
		return HRTIMER_NORESTART;
	}

	if (hvgr_cq_collect_count(cq_ctx, gcqctx) != 0) {
		spin_unlock_irqrestore(&gdev->cq_dev.schedule_lock, flags);
		return HRTIMER_NORESTART;
	}

#if defined(HVGR_SUPPORT_SPEC_LOW) && HVGR_SUPPORT_SPEC_LOW != 0
	empty = hvgr_cq_schedule_queue_is_empty_expect_spec_low(&gdev->cq_dev);
#else
	empty = hvgr_cq_schedule_queue_is_empty(&gdev->cq_dev);
#endif
	/* We change the suspend schedule timer longer when in protect mode. */
	if (gcqctx->protect_mode_info.protected_mode)
		empty = true;

	hvgr_cq_check_running_counter(cq_ctx, gcqctx, empty, chn, timer);
	spin_unlock_irqrestore(&gdev->cq_dev.schedule_lock, flags);

	return HRTIMER_NORESTART;
}

bool hvgr_cq_schedule_find_queue(struct hvgr_cq_dev * const gcqctx,
	struct hvgr_cq_ctx * const cq_ctx)
{
	enum cq_priority priority = cq_ctx->priority;
	struct cq_pending *pending_queue = NULL;
	struct list_head *iterator = NULL;

	if (priority >= CQ_PRIORITY_MAX) {
		hvgr_err(cq_ctx->ctx->gdev, HVGR_CQ, "priority=%d\n", priority);
		return false;
	}
	pending_queue = &gcqctx->pending_queue[priority];

	iterator = &pending_queue->pending_list;
	for (iterator = iterator->next;
		iterator != &pending_queue->pending_list;
		iterator = iterator->next) {
		if (iterator == &cq_ctx->pending_item)
			return true;
	}
	return false;
}

long hvgr_cq_schedule_unqueue(struct hvgr_cq_dev * const gcqctx,
	struct hvgr_cq_ctx * const cq_ctx)
{
	bool exist = false;

	exist = hvgr_cq_schedule_find_queue(gcqctx, cq_ctx);
	if (exist) {
		list_del(&cq_ctx->pending_item);
		gcqctx->last_sch_time[cq_ctx->priority] = 0;
	}
	hvgr_cq_mod_status_macro(cq_ctx, CQ_STATUS_IDLE);
	return 0;
}

long hvgr_cq_schedule_enqueue(struct hvgr_cq_dev * const gcqctx,
	struct hvgr_cq_ctx * const cq_ctx)
{
	enum cq_priority priority = cq_ctx->priority;
	struct cq_pending *pending_queue = NULL;
	bool exist = false;

	if (unlikely(priority >= CQ_PRIORITY_MAX)) {
		hvgr_err(cq_ctx->ctx->gdev, HVGR_CQ, "priority=%d\n", priority);
		return -1;
	}

	if (unlikely((cq_ctx->flags & CQ_KMD_CTX_START_SUBMIT) == 0)) {
		hvgr_err(cq_ctx->ctx->gdev, HVGR_CQ, "qid(%d) flags=0x%x\n",
			cq_ctx->queue_id, cq_ctx->flags);
		return -1;
	}

	exist = hvgr_cq_schedule_find_queue(gcqctx, cq_ctx);
	if (!exist) {
		hvgr_info(cq_ctx->ctx->gdev, HVGR_SCH, "[cq]: %s. ctx_%u.%d enqueue chn : %d",
			__func__, cq_ctx->ctx->id, cq_ctx->queue_id, priority);
		pending_queue = &gcqctx->pending_queue[priority];
		list_add_tail(&cq_ctx->pending_item,
			&pending_queue->pending_list);
		gcqctx->last_sch_time[priority] = (uint64_t)ktime_to_ns(ktime_get());
	}
	hvgr_cq_mod_status_macro(cq_ctx, CQ_STATUS_PENDING);
	return 0;
}

bool hvgr_cq_is_queue_error(struct hvgr_cq_ctx const *cq_ctx)
{
	/* true is error */
	if (unlikely(cq_ctx->cq_status == NULL))
		return false;
	if (unlikely(cq_ctx->err_flags == CQ_ERROR))
		hvgr_debug(cq_ctx->ctx->gdev, HVGR_CQ, "%s ctx_%u, qid=%u, err_flags=%d\n", __func__,
			cq_ctx->ctx->id, cq_ctx->queue_id, cq_ctx->err_flags);
	return (cq_ctx->err_flags == CQ_ERROR);
}

bool hvgr_cq_has_queue_error(struct hvgr_ctx const *ctx)
{
	uint32_t idx;
	bool ret = false;

	for (idx = 0; idx < ARRAY_SIZE(ctx->ctx_cq.cq_ctx); idx++) {
		ret = hvgr_cq_is_queue_error(&ctx->ctx_cq.cq_ctx[idx]);
		if (ret)
			return true;
	}
	return ret;
}

static inline bool hvgr_cq_check_ctx_in_gpu(struct hvgr_ctx const * const ctx)
{
	uint32_t idx;

	for (idx = 0; idx < ARRAY_SIZE(ctx->ctx_cq.cq_ctx); idx++) {
		struct hvgr_cq_ctx const *cq_ctx = &ctx->ctx_cq.cq_ctx[idx];
		if (((cq_ctx->status == CQ_STATUS_RUNNING) ||
			(cq_ctx->status == CQ_STATUS_STOPPING)) &&
			(cq_ctx->sw_wr_ctx != NULL) &&
			(cq_ctx->channel_id != CQ_CHANNEL_ID_INVALID))
			return true;
	}

	return false;
}

void hvgr_cq_schedule_cq_in(struct hvgr_cq_ctx * const cq_ctx, uint32_t channel_id)
{
	struct hvgr_ctx * const ctx = cq_ctx->ctx;
	struct hvgr_cq_dev * const gcqctx = &ctx->gdev->cq_dev;
	struct hvgr_cq_ctx * running_cq = NULL;

#if defined(CONFIG_LIBLINUX) && !defined(CONFIG_LIBLINUX_CDC)
	if (hvgr_switch_get_status(ctx) != CTX_IS_FORE_GROUND)
		hvgr_debug(ctx->gdev, HVGR_CQ,
			"ctx_%u, queueid_%u sch but not fground.tgid=%d pid=%d pname=%s gname=%s.",
			ctx->id, cq_ctx->queue_id, ctx->tgid, ctx->pid, ctx->process_name, ctx->group_name);
#endif
	/* re-check running_cq to prevent duplicate submit */
	if (unlikely(gcqctx->channel_info[channel_id].running_queue != NULL)) {
		if (gcqctx->gpu_sch_sts == HVGR_GPU_IDLE) {
			hvgr_err(ctx->gdev, HVGR_SCH, "%s current gpu status is wrong.", __func__);
			gcqctx->gpu_sch_sts = HVGR_GPU_RUNNING;
		}
		running_cq = gcqctx->channel_info[channel_id].running_queue;
		hvgr_err(ctx->gdev, HVGR_SCH, "%s channel=%d is already binded empty_flag=%u\n"
			"running_cq(ctx_%u,qid=%u,pri=%u),cq_currernt(ctx_%u,qid=%u,pri=%u)",
			__func__, channel_id, hvgr_cq_get_empty_flag(ctx->gdev, channel_id),
			running_cq->ctx->id, running_cq->queue_id, running_cq->priority,
			ctx->id, cq_ctx->queue_id, cq_ctx->priority);
		(void)hvgr_cq_schedule_enqueue(gcqctx, cq_ctx);
		return;
	}
	/* If bind status is not null, enqueue */
#if hvgr_version_eq(HVGR_V200)
	if (unlikely(hvgr_read_reg(ctx->gdev, cq_control_reg(ctx->gdev, cq_bind_status_0(channel_id))) != 0)) {
		hvgr_err(ctx->gdev, HVGR_SCH, "%s chn_%u is not NULL, status=%u.", __func__, channel_id,
			hvgr_read_reg(ctx->gdev, cq_control_reg(ctx->gdev, cq_bind_status_0(channel_id))));
		(void)hvgr_cq_schedule_enqueue(gcqctx, cq_ctx);
		return;
	}
#endif
	/* find free ASID */
	if (!hvgr_cq_check_ctx_in_gpu(ctx)) {
		if (!hvgr_ctx_sched_in(ctx))
			return;
	}

	if (unlikely(!hvgr_ctx_is_asid_valid(ctx->asid))) {
		hvgr_err(ctx->gdev, HVGR_SCH, "sch in asid %u is invalid", ctx->asid);
		return;
	}

	cq_ctx->channel_id = channel_id;
	gcqctx->channel_info[channel_id].running_queue = cq_ctx;

#if hvgr_version_ge(HVGR_V500)
	hvgr_pm_rtu_power_on(ctx->gdev, cq_ctx);
#endif

#if defined(HVGR_SUPPORT_SPEC_LOW) && HVGR_SUPPORT_SPEC_LOW != 0
	if (cq_ctx->priority == CQ_PRIORITY_SPECIAL_LOW)
		gcqctx->cq_spec_low = cq_ctx;
#endif
	hvgr_info(ctx->gdev, HVGR_SCH, "%s ctx_%u(%u) (%u:%u), ASID(%u), chn(%u), priv(%u)",
		__func__, cq_ctx->ctx->id, cq_ctx->queue_id, hvgr_get_queue_put_index(cq_ctx),
		hvgr_get_queue_get_index(cq_ctx), ctx->asid, channel_id, cq_ctx->priority);

	hvgr_datan_jobtrace(ctx, JOBTRACE_TYPE_OBJ, KDATAN_TL_RET_QUEUE_CHANNEL, cq_ctx->ctx,
		cq_ctx->queue_id, channel_id);
	hvgr_cq_bind_channel(cq_ctx, channel_id);

	(void)hvgr_wq_start_work(cq_ctx);
}

bool hvgr_cq_is_chn_running(struct hvgr_cq_dev *gcqctx)
{
	int idx;

	for (idx = 0; idx < CQ_CHANNEL_NUM; idx++) {
		if (gcqctx->channel_info[idx].running_queue != NULL)
			return true;
	}
	return false;
}

bool hvgr_cq_is_queue_running(struct hvgr_cq_dev *gcqctx, struct hvgr_cq_ctx * const cq_ctx)
{
	int idx;

	for (idx = 0; idx < CQ_CHANNEL_NUM; idx++) {
		if ((gcqctx->channel_info[idx].running_queue != NULL) &&
			(gcqctx->channel_info[idx].running_queue->queue_id == cq_ctx->queue_id) &&
			(cq_ctx->queue_id != CQ_CHANNEL_ID_INVALID))
			return true;
	}
	return false;
}

bool hvgr_cq_is_need_into_pending(bool enqueue_flag, struct hvgr_cq_ctx * const cq_ctx)
{
	if (enqueue_flag && ((!cq_ctx->empty_flag) ||
		(hvgr_get_queue_get_index(cq_ctx) != hvgr_get_queue_put_index(cq_ctx)) ||
		(cq_ctx->put_index_unbind != hvgr_get_queue_put_index(cq_ctx)) ||
		(cq_ctx->get_index_unbind != hvgr_get_queue_get_index(cq_ctx))))
		return true;

	return false;
}

#if defined(HVGR_WAITFENCE_LOWPOWER) && HVGR_WAITFENCE_LOWPOWER != 0
bool hvgr_cq_is_deal_wait_fence(struct hvgr_cq_ctx * const cq_ctx)
{
	struct hvgr_cq_ctx_data *ctx_cq = &cq_ctx->ctx->ctx_cq;
	unsigned long flags;
	bool ret = false;

	if (cq_ctx->need_check_wait_entry)
		return true;

	spin_lock_irqsave(&ctx_cq->wait_fence_lock, flags);
	if (hvgr_cq_has_in_fence_waiting_list(cq_ctx))
		ret = true;
	spin_unlock_irqrestore(&ctx_cq->wait_fence_lock, flags);

	return ret;
}

static void hvgr_cq_enqueue_fence_waiting_list(struct hvgr_cq_ctx * const cq_ctx)
{
	struct hvgr_cq_ctx_data *ctx_cq = &cq_ctx->ctx->ctx_cq;
	struct hvgr_cq_dev * const gcqctx = &cq_ctx->ctx->gdev->cq_dev;
	unsigned long flags;

	cq_ctx->need_waiting_fence = false;

	spin_lock_irqsave(&ctx_cq->wait_fence_lock, flags);
	list_add_tail(&cq_ctx->pending_item, &ctx_cq->fence_waiting_list);
	hvgr_cq_mod_status_macro(cq_ctx, CQ_STATUS_WAITING);
	atomic_inc(&gcqctx->wait_fence_num);
	(void)mod_timer(&cq_ctx->in_fence_waiting_timer,
		jiffies + (uint32_t)CQ_WAIT_FENCE_TIMEOUT * HZ / CQ_TIME_S2MS);
	hvgr_info(cq_ctx->ctx->gdev, HVGR_SCH, "ctx_%u, queueid_%u enqueue fence waiting list.",
		cq_ctx->ctx->id, cq_ctx->queue_id);
	spin_unlock_irqrestore(&ctx_cq->wait_fence_lock, flags);
}
#endif

void hvgr_cq_schedule_cq_out(struct hvgr_cq_ctx * const cq_ctx, bool enqueue_flag)
{
	struct hvgr_ctx * const ctx = cq_ctx->ctx;
	struct hvgr_cq_dev * const gcqctx = &ctx->gdev->cq_dev;
	uint32_t channel_id = cq_ctx->channel_id;

	hvgr_info(ctx->gdev, HVGR_SCH,
		"%s, ctx_%u(%u), (%u:%u : %u:%u).%u, f(%x), s(%x) ASID(%u), chn(%u), priv(%u)",
		__func__, ctx->id, cq_ctx->queue_id,
		hvgr_get_queue_put_index(cq_ctx), hvgr_get_queue_get_index(cq_ctx),
		cq_ctx->put_index_unbind, cq_ctx->get_index_unbind, cq_ctx->empty_flag,
		cq_ctx->flags, cq_ctx->status, ctx->asid, cq_ctx->channel_id, cq_ctx->priority);
	if (channel_id != CQ_CHANNEL_ID_INVALID)
		gcqctx->channel_info[channel_id].running_queue = NULL;
	cq_ctx->channel_id = CQ_CHANNEL_ID_INVALID;

#if defined(HVGR_SUPPORT_SPEC_LOW) && HVGR_SUPPORT_SPEC_LOW != 0
	if (cq_ctx->priority == CQ_PRIORITY_SPECIAL_LOW)
		gcqctx->cq_spec_low = NULL;
#endif
	/* after gpu reset, it will clear all asid. */
	if (!hvgr_cq_check_ctx_in_gpu(ctx)) {
#if hvgr_version_ge(HVGR_V350)
		hvgr_mmu_clear_mmu_as_info(ctx);
#endif
		hvgr_ctx_sched_out(ctx);
	}

	if (!hvgr_cq_is_chn_running(gcqctx)) {
		hvgr_info(ctx->gdev, HVGR_SCH, "%s gpu_sch_sts=%d.\n",
			__func__, gcqctx->gpu_sch_sts);
		gcqctx->gpu_sch_sts = HVGR_GPU_IDLE;
		ctx->gdev->time_record.gpu_idle = (uint64_t)ktime_to_ns(ktime_get());
		hvgr_cq_schedule_update_gpu_status(ctx->gdev);
	}

	if ((cq_ctx->flags & CQ_KMD_CTX_START_SUBMIT) == 0) {
		hvgr_cq_mod_status_macro(cq_ctx, CQ_STATUS_IDLE);
		wake_up(&cq_ctx->queue_done_wait);
		goto exit;
	}
	hvgr_info(ctx->gdev, HVGR_SCH, "%s cqctx->flags=0x%x\n", __func__, cq_ctx->flags);
	/* if there are some error, will not in queue */
	if (hvgr_cq_is_append_exception_flag(cq_ctx)) {
		hvgr_cq_mod_status_macro(cq_ctx, CQ_STATUS_ERROR);
		hvgr_debug(ctx->gdev, HVGR_SCH, "%s, ctx_%u(%u), (%u:%u).%u,f(%x),s(%x).",
			__func__, ctx->id, cq_ctx->queue_id,
			hvgr_get_queue_put_index(cq_ctx), hvgr_get_queue_get_index(cq_ctx),
			cq_ctx->empty_flag, cq_ctx->flags, cq_ctx->status);
		goto exit;
	}

#if defined(HVGR_WAITFENCE_LOWPOWER) && HVGR_WAITFENCE_LOWPOWER != 0
	if (cq_ctx->need_waiting_fence) {
		hvgr_cq_enqueue_fence_waiting_list(cq_ctx);
		goto exit;
	}
#endif

	hvgr_info(ctx->gdev, HVGR_SCH, "%s enqueue_flag=%d.\n", __func__, enqueue_flag);
	/* If CQ not empty, enqueue it to pending queue. */

	if (hvgr_cq_entry_wait_inqueue(cq_ctx))
		goto exit;

#if hvgr_version_ge(HVGR_V500)
	hvgr_pm_rtu_power_off(ctx->gdev, cq_ctx, RTU_DEC_CNT_ENABLE);
#endif
	if (hvgr_cq_is_need_into_pending(enqueue_flag, cq_ctx))
		(void)hvgr_cq_schedule_enqueue(gcqctx, cq_ctx);
	else
		hvgr_cq_mod_status_macro(cq_ctx, CQ_STATUS_IDLE);

exit:
	if (gcqctx->gpu_sch_sts == HVGR_GPU_IDLE)
		wake_up(&gcqctx->stop_queue_done_wait);

	(void)hvgr_wq_stop_work(cq_ctx);
}

void hvgr_queue_pm_release_work(struct hvgr_cq_dev * const gcqctx)
{
#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0))
	(void)queue_work(gcqctx->sched_pm_release_wq, &gcqctx->sched_pm_release_work);
#else
	(void)kthread_queue_work(gcqctx->sched_pm_release_wq, &gcqctx->sched_pm_release_work);
#endif
}

void hvgr_flush_pm_release_work(struct hvgr_cq_dev * const gcqctx)
{
#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0))
	flush_workqueue(gcqctx->sched_pm_release_wq);
#else
	kthread_flush_work(&gcqctx->sched_pm_release_work);
#endif
}

static void hvgr_cq_power_off_after_gpu_idle(struct hvgr_ctx *ctx)
{
	struct hvgr_cq_dev * const gcqctx = &ctx->gdev->cq_dev;

	if (atomic_cmpxchg(&gcqctx->pm_pwr_flag, 1, 0) != 1)
		hvgr_err(ctx->gdev, HVGR_SCH, "%s start power off.ctx_%u, refcount=%d not right.\n",
			__func__, ctx->id, atomic_read(&gcqctx->pm_pwr_flag));

	hvgr_queue_pm_release_work(gcqctx);
}

bool hvgr_cq_is_need_unbind(bool empty_flag, struct hvgr_cq_ctx * const cq_ctx)
{
	if (empty_flag &&
		(hvgr_get_queue_get_index(cq_ctx) == hvgr_get_queue_put_index(cq_ctx)))
		return true;
	return false;
}

bool __attribute__((weak)) hvgr_cq_is_need_run_sched(void)
{
	return true;
}

#if defined(HVGR_SUPPORT_SPEC_LOW) && HVGR_SUPPORT_SPEC_LOW != 0
static int hvgr_cq_schedule_special_low_stopped(struct hvgr_cq_ctx * const cq_ctx)
{
	struct hvgr_ctx *ctx = cq_ctx->ctx;
	if (cq_ctx->priority != CQ_PRIORITY_SPECIAL_LOW)
		return HVGR_NO_SUBMIT;

	hvgr_info(ctx->gdev, HVGR_SPEC_LOW, "%s ctx_%u, qid(%d) try to start all channel.\n",
		__func__, cq_ctx->ctx->id, cq_ctx->queue_id);
	hvgr_cq_schedule_channel_start_all_no_check(cq_ctx->ctx->gdev);
	return HVGR_HAS_SUBMIT;
}
#endif

void hvgr_cq_schedule_channel(struct hvgr_device * const gdev,
	uint32_t channel_id, enum protect_flag flag, bool need_stop)
{
	struct hvgr_cq_dev * const gcqctx = &gdev->cq_dev;
	struct hvgr_cq_ctx * const cq_ctx = hvgr_cq_get_channel_ctx(gdev, channel_id);
	struct hvgr_ctx *ctx = NULL;

	if (cq_ctx == NULL)
		return;

	ctx = cq_ctx->ctx;
	gcqctx->channel_sche_times++;

	switch (cq_ctx->status) {
	case CQ_STATUS_RUNNING:
	{
		if (unlikely(!hvgr_cq_is_need_run_sched()))
			break;

		hvgr_cq_schedule_channel_running_proc(gdev, cq_ctx, channel_id, flag, need_stop);
		break;
	}
	case CQ_STATUS_STOPPING:
		hvgr_cq_protect_mode_check_running(cq_ctx, gdev, channel_id, flag);
		break;
	case CQ_STATUS_STOPPED:
		hvgr_cq_protect_mode_check_running(cq_ctx, gdev, channel_id, flag);

		if (gcqctx->protect_mode_info.protected_mode_transition) {
			hvgr_cq_protect_mode_check_stopped(cq_ctx, gdev, channel_id);
			break;
		} else if (gcqctx->protect_mode_info.protected_mode) {
			/* resubmit it */
			hvgr_cq_bind_channel(cq_ctx, channel_id);
			break;
		}

		/* else normal job, */
		hvgr_cq_schedule_cq_out(cq_ctx, true);
		hvgr_hts_wait_list_in_schedule(gdev, HVGR_SCH_ONLY_INQUQUE);
		if (likely(gcqctx->submit_allow &&
			gcqctx->channel_info[channel_id].available)) {
			int ret = HVGR_NO_SUBMIT;

#if defined(HVGR_SUPPORT_SPEC_LOW) && HVGR_SUPPORT_SPEC_LOW != 0
			ret = hvgr_cq_schedule_special_low_stopped(cq_ctx);
#endif
			if (ret == HVGR_NO_SUBMIT)
				hvgr_cq_schedule_channel_start_no_check(gdev, channel_id);
		}

		if (gcqctx->gpu_sch_sts == HVGR_GPU_IDLE)
			hvgr_cq_power_off_after_gpu_idle(ctx);
#if hvgr_version_ge(HVGR_V500)
		else
			hvgr_pm_rtu_power_off(gdev, cq_ctx, RTU_POWER_OFF_ENABLE);
#endif
		break;
	case CQ_STATUS_TIMEOUT:
		hvgr_cq_unbind_channel(cq_ctx, channel_id, CQ_NEED_UNBIND_TIMER_FLAG);
	/* fall through */
	case CQ_STATUS_PENDING:
	/* fall through */
	case CQ_STATUS_WAITING:
	/* fall through */
	case CQ_STATUS_DIVE:
	/* fall through */
	case CQ_STATUS_IDLE:
	/* fall through */
	case CQ_STATUS_ERROR:
		/* not possible. */
		hvgr_err(gdev, HVGR_CQ, "%s ctx_%u, cqctx->status=%d, not right.\n",
			__func__, ctx->id, cq_ctx->status);
		break;
	}
}

irqreturn_t hvgr_cq_irq_handler(int irq, void *data)
{
	struct hvgr_device * const gdev = (struct hvgr_device * const)data;
	irqreturn_t ret = IRQ_NONE;
	unsigned long flags;

#if defined(HVGR_STATE_BUNDLE2) && HVGR_STATE_BUNDLE2 > 0
	if (hvgr_pm_get_gpu_status(gdev) == HVGR_PM_GPU_READY) {
		hvgr_lowpa_irq(gdev);
		ret = IRQ_HANDLED;
	} else {
		hvgr_err(gdev, HVGR_CQ, "%s gpu_pm_sts=%d, not excepted, ignore lowpa irq",
			__func__, hvgr_pm_get_gpu_status(gdev));
	}
#endif

#if defined(HVGR_FEATURE_HTS) && HVGR_FEATURE_HTS != 0
	if (hvgr_pm_get_gpu_status(gdev) == HVGR_PM_GPU_READY ||
		hvgr_pm_get_gpu_status(gdev) == HVGR_PM_GPC_POWERING_OFF) {
		hvgr_hts_no_consumer_irq(gdev);
		ret = IRQ_HANDLED;
	} else {
		hvgr_err(gdev, HVGR_CQ, "%s gpu_pm_sts=%d, not excepted, ignore hts irq.",
			__func__, hvgr_pm_get_gpu_status(gdev));
	}

#endif
	if (hvgr_pm_get_gpu_status(gdev) == HVGR_PM_GPU_READY) {
		spin_lock_irqsave(&gdev->cq_dev.schedule_lock, flags);
		hvgr_cq_schedule_irq(gdev);
		spin_unlock_irqrestore(&gdev->cq_dev.schedule_lock, flags);
		ret = IRQ_HANDLED;
	} else {
		hvgr_err(gdev, HVGR_CQ, "%s gpu_pm_sts=%d, not excepted, ignore cq irq.",
			__func__, hvgr_pm_get_gpu_status(gdev));
	}
	return ret;
}

void hvgr_cq_start_and_schedule(struct hvgr_device * const gdev)
{
	hvgr_cq_schedule_channel_all(gdev, true);
}

void hvgr_cq_schedule_submit_change_to_hw(struct hvgr_cq_ctx * const cq_ctx)
{
	struct hvgr_device *gdev = NULL;

	gdev = cq_ctx->ctx->gdev;
	if (cq_ctx->status == CQ_STATUS_RUNNING) {
		hvgr_datan_jobtrace(cq_ctx->ctx, JOBTRACE_TYPE_OBJ, KDATAN_TL_WRITE_REG,
			cq_ctx->ctx, datan_cq_control_reg(gdev, CQ_SUBMIT),
			cq_ctx->queue_id);
		hvgr_write_reg(cq_ctx->ctx->gdev, cq_control_reg(gdev, CQ_SUBMIT),
			cq_ctx->queue_id);
		hvgr_info(cq_ctx->ctx->gdev, HVGR_CQ,
			"%s in, qid=%u, ctx_%u, write submit reg.\n",
			__func__, cq_ctx->queue_id, cq_ctx->ctx->id, cq_ctx->flags);
	}
}

#if defined(HVGR_SUPPORT_SPEC_LOW) && HVGR_SUPPORT_SPEC_LOW != 0
static int hvgr_cq_schedule_special_low_submit(struct hvgr_ctx * const ctx,
	struct hvgr_cq_dev * const gcqctx, struct hvgr_cq_ctx * const cq_ctx)
{
	if (cq_ctx->priority == CQ_PRIORITY_SPECIAL_LOW) {
		hvgr_info(ctx->gdev, HVGR_SPEC_LOW, "%s qid=%u, ctx_%u, gpu_sch_sts=%d.\n",
			__func__, cq_ctx->queue_id, cq_ctx->ctx->id, gcqctx->gpu_sch_sts);
		if (gcqctx->gpu_sch_sts == HVGR_GPU_IDLE)
			hvgr_cq_schedule_cq_in(cq_ctx, 0);
		else
			hvgr_cq_schedule_enqueue(gcqctx, cq_ctx);
		return HVGR_HAS_SUBMIT;
	}

	if (gcqctx->cq_spec_low == NULL)
		return HVGR_NO_SUBMIT;

	if (gcqctx->cq_spec_low->status == CQ_STATUS_STOPPING) {
		hvgr_cq_schedule_enqueue(gcqctx, cq_ctx);
		return HVGR_HAS_SUBMIT;
	}

	hvgr_info(ctx->gdev, HVGR_SPEC_LOW, "%s will unbind special low queue ctx_%u, qid=%u.\n",
		__func__, gcqctx->cq_spec_low->ctx->id, gcqctx->cq_spec_low->queue_id);
	hvgr_cq_unbind_channel(gcqctx->cq_spec_low,
		gcqctx->cq_spec_low->channel_id, CQ_NEED_UNBIND_TIMER_FLAG);
	hvgr_cq_schedule_enqueue(gcqctx, cq_ctx);
	return HVGR_HAS_SUBMIT;
}
#endif

void hvgr_cq_schedule_start(struct hvgr_ctx * const ctx,
	struct hvgr_cq_dev * const gcqctx, struct hvgr_cq_ctx * const cq_ctx)
{
	uint32_t channel;
	struct hvgr_cq_ctx *running_cq = NULL;

	hvgr_info(ctx->gdev, HVGR_SCH, "[cq]: %s : ctx_%u.%u (%u : %u): state = %d",
		__func__, ctx->id, cq_ctx->queue_id, hvgr_get_queue_put_index(cq_ctx),
		hvgr_get_queue_get_index(cq_ctx), cq_ctx->status);
	hvgr_cq_schedule_submit_change_to_hw(cq_ctx);

	/* If CQ status is running but not in running queue, change status to idle and submit */
	if ((cq_ctx->status == CQ_STATUS_RUNNING) && !hvgr_cq_is_queue_running(gcqctx, cq_ctx)) {
		hvgr_err(ctx->gdev, HVGR_SCH, "%s ctx_%u qid=%u priority=%u is fake running in chn_%u",
			__func__, cq_ctx->ctx->id, cq_ctx->queue_id, cq_ctx->priority, cq_ctx->channel_id);
		hvgr_cq_mod_status_macro(cq_ctx, CQ_STATUS_IDLE);
	}
	/* If CQ status is not CQ_STATUS_IDLE, the schedule will deal it. */
	if (cq_ctx->status != CQ_STATUS_IDLE)
		return;

	if (!gcqctx->submit_allow || hvgr_pm_get_gpu_status(ctx->gdev) != HVGR_PM_GPU_READY) {
		(void)hvgr_cq_schedule_enqueue(gcqctx, cq_ctx);
		return;
	}

#if defined(HVGR_SUPPORT_SPEC_LOW) && HVGR_SUPPORT_SPEC_LOW != 0
	if (hvgr_cq_schedule_special_low_submit(ctx, gcqctx, cq_ctx) == HVGR_HAS_SUBMIT)
		return;
#endif

	channel = hvgr_cq_get_channel(gcqctx, cq_ctx);
	/* No channel is invalid, add to pending queue */
	if (channel == CQ_CHANNEL_ID_INVALID) {
		(void)hvgr_cq_schedule_enqueue(gcqctx, cq_ctx);
		return;
	}

	running_cq = gcqctx->channel_info[channel].running_queue;
	/* Channel has queue, unbind the running quque. */
	if (running_cq != NULL) {
		hvgr_cq_unbind_channel(running_cq, running_cq->channel_id,
			CQ_NEED_UNBIND_TIMER_FLAG);
		(void)hvgr_cq_schedule_enqueue(gcqctx, cq_ctx);
		return;
	}

	/* Channel is free, bind the new queue. */
	hvgr_cq_schedule_cq_in(cq_ctx, channel);
}

bool hvgr_is_cqctx_in_protect_running_queue(struct hvgr_cq_ctx *cq_ctx)
{
	uint32_t i;
	struct hvgr_cq_dev * const gcqctx = &cq_ctx->ctx->gdev->cq_dev;

	/* Judge whether the cqctx is in protected mode running queue or bak_running queue */
	if (cq_ctx == gcqctx->protect_mode_info.running_queue)
		return true;

	for (i = 0; i < ARRAY_SIZE(gcqctx->protect_mode_info.bak_running_queue); i++)
		if (cq_ctx == gcqctx->protect_mode_info.bak_running_queue[i])
			return true;

	return false;
}

void hvgr_cq_hts_schedule(struct hvgr_device * const gdev, struct hvgr_cq_ctx *cq_ctx,
	enum sch_type type)
{
	switch (type) {
	case HVGR_SCH_SCHEDULE_ALL:
		hvgr_cq_schedule_channel_all(gdev, false);
		break;
	case HVGR_SCH_ONLY_INQUQUE:
		(void)hvgr_cq_schedule_enqueue(&gdev->cq_dev, cq_ctx);
		break;
	case HVGR_SCH_SUBMIT_ALL:
		if (cq_ctx->status == CQ_STATUS_WAITING)
			hvgr_cq_mod_status_macro(cq_ctx, CQ_STATUS_IDLE);
		hvgr_cq_schedule_start(cq_ctx->ctx, &gdev->cq_dev, cq_ctx);
		break;
	default:
		break;
	}
}

long hvgr_cq_clear_err(struct hvgr_ctx * const ctx, uint32_t queue_id)
{
	unsigned long flags;
	struct hvgr_cq_dev *gcqctx = &ctx->gdev->cq_dev;
	struct hvgr_cq_ctx *cq_ctx = NULL;

	hvgr_debug(ctx->gdev, HVGR_CQ, "%s ctx_%u, queue_id=%d\n", __func__, ctx->id, queue_id);
	/* Make sure the cq is legal. */
	mutex_lock(&ctx->ctx_lock);
	cq_ctx = hvgr_cq_find_cqctx(ctx, queue_id);
	if (cq_ctx == NULL) {
		mutex_unlock(&ctx->ctx_lock);
		return -EINVAL;
	}

	if (unlikely(cq_ctx->cq_status == NULL)) {
		mutex_unlock(&ctx->ctx_lock);
		return -EINVAL;
	}

	/* clear all error by user excepet submit flag. */
	spin_lock_irqsave(&gcqctx->schedule_lock, flags);
	if ((cq_ctx->err_flags == CQ_ERROR) &&
		((cq_ctx->flags & CQ_KMD_CTX_ERROR_FLAG) != 0)) {
		cq_ctx->flags &= CQ_KMD_CTX_START_SUBMIT;
		hvgr_cq_mod_status_macro(cq_ctx, CQ_STATUS_IDLE);
		cq_ctx->cq_status->err_flags = CQ_ERROR_NONE;
		cq_ctx->err_flags = CQ_ERROR_NONE;
		cq_ctx->ctx->ctx_cq.err_disp = false;
	}
	spin_unlock_irqrestore(&gcqctx->schedule_lock, flags);
	hvgr_debug(ctx->gdev, HVGR_CQ,
		"%s ctx_%u, queue_id=%d, cq_ctx->status=%d,err_flags=%d,flags=0x%x\n", __func__,
		ctx->id, queue_id, cq_ctx->status, cq_ctx->err_flags, cq_ctx->flags);

#if hvgr_version_ge(HVGR_V500)
	(void)memset_s(u64_to_ptr(cq_ctx->sw_ctx_hw_area->kva), CQ_HW_WRITEABLE_CONTEXT_SIZE, 0, CQ_HW_WRITEABLE_CONTEXT_SIZE);
	(void)memset_s(u64_to_ptr(cq_ctx->sw_ctx_hw_ring_area->kva), CQ_HW_RINGBUF_SIZE, 0, CQ_HW_RINGBUF_SIZE);
#endif

	mutex_unlock(&ctx->ctx_lock);
	return 0;
}

static bool hvgr_cq_schedule_check_end(struct hvgr_cq_ctx *cq_ctx)
{
	bool is_end = false;

	/* If the queue is in protect mode, we need to wait it exit protect mode. */
	mutex_lock(&cq_ctx->dump_mem_lock);
	is_end =
		((cq_ctx->status == CQ_STATUS_IDLE) || (cq_ctx->status == CQ_STATUS_ERROR)) &&
		(!cq_ctx->dump_mem_flag) &&
		(!hvgr_is_cqctx_in_protect_running_queue(cq_ctx));
	mutex_unlock(&cq_ctx->dump_mem_lock);
	return is_end;
}

void hvgr_cq_schedule_unsubmit(struct hvgr_ctx * const ctx, struct hvgr_cq_ctx *cq_ctx)
{
	unsigned long flags;
	struct hvgr_cq_dev *gcqctx = NULL;

	gcqctx = &ctx->gdev->cq_dev;
	spin_lock_irqsave(&gcqctx->schedule_lock, flags);
	/* unmask submit flag */
	cq_ctx->flags &= (~CQ_KMD_CTX_START_SUBMIT);

	hvgr_info(ctx->gdev, HVGR_CQ, "%s ctx_%u, cq_ctx->status=%d\n",
		__func__, ctx->id, cq_ctx->status);
	switch (cq_ctx->status) {
	case CQ_STATUS_RUNNING:
		/* if we are in not allow submit status, gpu is resetting,
		 * running job will clear after reset, just wait.
		 * after clear, the status will be pending or idle, will not come in here by lock.
		 * if come here first, submit flag will be clear, and complete will be set.
		 */
		hvgr_info(ctx->gdev, HVGR_CQ,
			"%s ctx_%u, submit_allow=%d, cq_ctx->flags=0x%x channel_id = 0x%x.\n",
			__func__, ctx->id, gcqctx->submit_allow, cq_ctx->flags, cq_ctx->channel_id);

		/* If cq is not in running queue, change status to idle for process exit */
		if (!hvgr_cq_is_queue_running(gcqctx, cq_ctx)) {
			hvgr_cq_show_status_mod_record(cq_ctx);
			hvgr_err(ctx->gdev, HVGR_CQ,
			"%s ctx_%u, submit_allow=%d, cq_ctx->flags=0x%x channel_id = 0x%x.\n"
			"last time: buck_off=%llu,buck_on=%llu,gpu_idle=%llu,gpu_running=%llu \n"
			"crg_reset=%llu,user_ref=%d,pm_status=%d,cur_time=%llu\n",
			__func__, ctx->id, gcqctx->submit_allow, cq_ctx->flags, cq_ctx->channel_id,
			ctx->gdev->time_record.buck_off, ctx->gdev->time_record.buck_on,
			ctx->gdev->time_record.gpu_idle, ctx->gdev->time_record.gpu_running,
			ctx->gdev->time_record.crg_reset,
			atomic_read(&ctx->gdev->pm_dev.pm_pwr.user_ref),
			hvgr_pm_get_gpu_status(ctx->gdev),
			(uint64_t)ktime_to_ns(ktime_get()));
			hvgr_cq_mod_status_macro(cq_ctx, CQ_STATUS_IDLE);
			break;
		}

		if (gcqctx->submit_allow) {
			/* if exception, clear it directly */
			if (!hvgr_cq_is_queue_error(cq_ctx))
				hvgr_cq_unbind_channel(cq_ctx, cq_ctx->channel_id,
					CQ_NEED_UNBIND_TIMER_FLAG);
		}
		break;
	/* fall through */
	case CQ_STATUS_PENDING:
		(void)hvgr_cq_schedule_unqueue(gcqctx, cq_ctx);
	/* fall through */
	case CQ_STATUS_IDLE:
		break;
	/* fall through */
	case CQ_STATUS_WAITING:
#if defined(HVGR_WAITFENCE_LOWPOWER) && HVGR_WAITFENCE_LOWPOWER != 0
		hvgr_cq_wait_fence_data_term(cq_ctx, false);
#endif
/* if cqctx in hts_timeout_cq_ctx means it will gpu reset, just wait */
#if defined(HVGR_FEATURE_HTS) && HVGR_FEATURE_HTS != 0
		if (cq_ctx == ctx->gdev->hts_dev.hts_timeout_cq_ctx)
			break;
#endif
		hvgr_cq_mod_status_macro(cq_ctx, CQ_STATUS_IDLE);
	/* fall through */
	case CQ_STATUS_DIVE:
	/* fall through */
	case CQ_STATUS_TIMEOUT:
		/* not possible. */
	/* fall through */
	case CQ_STATUS_STOPPED:
	/* fall through */
		/* not possible, stopped will change to idle or pending. */
	case CQ_STATUS_ERROR:
	/* fall through */
		/* just wait for errflow clear. */
	case CQ_STATUS_STOPPING:
		/* just wait for irq or clear. */
		break;
	}

	spin_unlock_irqrestore(&gcqctx->schedule_lock, flags);
#if defined(HVGR_FEATURE_HTS) && HVGR_FEATURE_HTS != 0
	del_timer_sync(&cq_ctx->hts_timer);
#endif
#if defined(HVGR_WAITFENCE_LOWPOWER) && HVGR_WAITFENCE_LOWPOWER != 0
	del_timer_sync(&cq_ctx->in_fence_waiting_timer);
#endif
	wait_event(cq_ctx->queue_done_wait, hvgr_cq_schedule_check_end(cq_ctx));

	hvgr_cq_sched_update_dvfs_status(cq_ctx, HVGR_THRO_IDLE, HVGR_THRO_ACTION_RESTORE_INITAL);
	hvgr_info(ctx->gdev, HVGR_CQ, "%s cq_ctx->status=%d.dump_mem_flag=%d\n", __func__,
		(unsigned int)cq_ctx->status, cq_ctx->dump_mem_flag);
}

static void hvgr_cq_schedule_data_init(struct hvgr_device * const gdev)
{
	struct hvgr_cq_dev * const gcqctx = &gdev->cq_dev;
#if hvgr_version_eq(HVGR_V300)
	gcqctx->hw_cfg.cq_subq_entry_os_l = CQ_SUBQ_ENTRY_OS_PRES_L_H_CFG;
	gcqctx->hw_cfg.cq_subq_entry_os_h = CQ_SUBQ_ENTRY_OS_PRES_L_H_CFG;
#endif

#if hvgr_version_ge(HVGR_V300)
	gcqctx->hw_cfg.cq_entry_mode = CQ_ENTRY_REORDER;
	gcqctx->hw_cfg.cq_entrys_os = CQ_ENTRYS_OS_CFG;
	gcqctx->hw_cfg.cq_queue_weight = 0;
	gcqctx->hw_cfg.cq_queue_real = 0;
#endif

	gcqctx->reset_timeout_normal = hvgr_platform_get_sch_unbind_timeout();
	gcqctx->reset_timeout_cl = hvgr_platform_get_sch_cl_unbind_timeout();
	gcqctx->cq_entry_timeout_cl = CQ_ENTRY_TIME_CL;
	gcqctx->cq_entry_timeout = CQ_ENTRY_TIME;
	gcqctx->hw_cfg.work_mode = CQ_WORK_MODE_NORMAL;
	gcqctx->hw_cfg.cq_sch_mode = CQ_SCH_MODE_PARALLEL;
	hvgr_set_send_mode(gdev);
	gcqctx->hw_cfg.cq_semp_wait_timeout_switch = CQ_DISABLE_TIMEOUT_SWITCH;
	gcqctx->hw_cfg.cq_schedule_unbind_time = CQ_SCH_TIME_ROBIN_MS;
	gcqctx->hw_cfg.cq_serial_timeslice = CQ_TIMESLICE_VALUE;
	/* multi render job chain open--->bind barrier close-->register is 0
	 * multi render job chain close--->bind barrier open -->register is 1
	 */
	gcqctx->hw_cfg.cq_multi_chain_enable = CQ_DISABLE_MULTI_CHAIN;

	gcqctx->hw_cfg.cq_bind_waitcycle = CQ_BIND_WAITCYCLE_DEFAULT;

	gcqctx->hw_cfg.cq_semp_wait_timeout = CQ_BIND_TIME_OUT_VALUE;

	if (hvgr_is_fpga(gdev))
		gcqctx->hw_cfg.cq_queue_empty_timeout = CQ_EMPTY_TIMEOUT_IN_FPGA;
	else
		gcqctx->hw_cfg.cq_queue_empty_timeout = CQ_EMPTY_TIMEOUT_IN_CHIP;

	hvgr_info(gdev, HVGR_CQ, "%s cq_queue_empty_timeout=%d\n", __func__,
		gcqctx->hw_cfg.cq_queue_empty_timeout);

	gcqctx->hw_cfg.cq_cl_affinity = hvgr_pm_get_availiable_cores(gdev);

	gcqctx->hw_cfg.cq_entry_boundary = HVGR_ENABLE;
	gcqctx->hw_cfg.cq_mmu_invalid_parellel = HVGR_ENABLE;

	gcqctx->hw_cfg.cq_order_dump = HVGR_DISABLE;
	gcqctx->hw_cfg.cq_sync_dump = HVGR_DISABLE;

	gcqctx->hw_cfg.cq_force_flush_l2 = false;
}

#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0))
void hvgr_schedule_pm_release_work(struct work_struct *data)
#else
void hvgr_schedule_pm_release_work(struct kthread_work *data)
#endif
{
	struct hvgr_device *gdev = NULL;
	struct hvgr_cq_dev *gcqctx = NULL;

	gcqctx = container_of(data, struct hvgr_cq_dev, sched_pm_release_work);
	gdev = container_of(gcqctx, struct hvgr_device, cq_dev);

	hvgr_pm_release_gpu(gdev);
	return;
}

#if hvgr_version_ge(HVGR_V350)
static void hvgr_cq_asid_and_ssid_init(struct hvgr_device * const gdev)
{
	bitmap_zero(gdev->asid_bitmap, HVGR_MAX_NR_AS);
	bitmap_zero(gdev->cq_dev.ssid_bitmap, HVGR_ALL_SSID_NUM);
	/* 0 is occupied by common_ctx. */
	bitmap_set(gdev->asid_bitmap, 0, 1);
	bitmap_set(gdev->cq_dev.ssid_bitmap, 0, 1);
}
#endif

long hvgr_cq_schedule_init(struct hvgr_device * const gdev)
{
	struct hvgr_cq_dev * const gcqctx = &gdev->cq_dev;
	uint32_t idx;

#if hvgr_version_ge(HVGR_V350)
	hvgr_cq_asid_and_ssid_init(gdev);
#endif
	gdev->as_free = 0xff;
	for (idx = 0; idx < ARRAY_SIZE(gcqctx->pending_queue); idx++)
		INIT_LIST_HEAD(&gcqctx->pending_queue[idx].pending_list);

	for (idx = 0; idx < CQ_CHANNEL_NUM; idx++) {
		gcqctx->channel_info[idx].global_cq_data = gcqctx;
		gcqctx->channel_info[idx].channel_id = idx;
		gcqctx->channel_info[idx].available = true;

		hrtimer_init(&gcqctx->channel_info[idx].schedule_timer,
			CLOCK_MONOTONIC, HRTIMER_MODE_REL);
		gcqctx->channel_info[idx].schedule_timer.function =
			kmd_cq_schedule_timer;
		gcqctx->channel_info[idx].timer_running_flag = false;
		gcqctx->channel_info[idx].timer_should_run = false;
	}

	spin_lock_init(&gcqctx->schedule_lock);
	spin_lock_init(&gcqctx->cq_mmu_lock);
#if defined(HVGR_STATE_BUNDLE2) && HVGR_STATE_BUNDLE2 > 0
	spin_lock_init(&gcqctx->cq_irq_lock);
#endif
	spin_lock_init(&gcqctx->protect_mode_switch);
	mutex_init(&gcqctx->submit_lock);
	mutex_init(&gcqctx->err_recover_lock);
#if defined(HVGR_WAITFENCE_LOWPOWER) && HVGR_WAITFENCE_LOWPOWER != 0
	atomic_set(&gcqctx->wait_fence_num, 0);
	gcqctx->wait_fence_switch_flag = true;
#endif
	gcqctx->cur_seqno = 0;

	hvgr_cq_schedule_data_init(gdev);
	hvgr_sch_policy_manager_init(gcqctx);

	if (hvgr_cq_pm_wq_init(gdev) != 0)
		return -1;

#if defined(HVGR_WAITFENCE_LOWPOWER) && HVGR_WAITFENCE_LOWPOWER != 0
	if (hvgr_cq_wait_fence_wq_init(gdev) != 0) {
		hvgr_cq_pm_wq_term(gdev);
		return -1;
	}
#endif
	return 0;
}

void hvgr_cq_schedule_term(struct hvgr_device * const gdev)
{
	struct hvgr_cq_dev * const gcqctx = &gdev->cq_dev;
	uint32_t chn;
	struct cq_channel *cq_chn = NULL;

	hvgr_cq_pm_wq_term(gdev);
#if defined(HVGR_WAITFENCE_LOWPOWER) && HVGR_WAITFENCE_LOWPOWER != 0
	hvgr_cq_wait_fence_wq_term(gdev);
#endif
	for (chn = 0; chn < CQ_CHANNEL_NUM; chn++) {
		cq_chn = &gcqctx->channel_info[chn];
		if (cq_chn->timer_running_flag) {
			(void)hrtimer_cancel(&cq_chn->schedule_timer);
			cq_chn->timer_running_flag = false;
		}
	}
}

void hvgr_cq_schedule_stop_submit(struct hvgr_device * const gdev)
{
	unsigned long flags;

	spin_lock_irqsave(&gdev->cq_dev.schedule_lock, flags);
	/* don't allow submit new queue. */
	hvgr_info(gdev, HVGR_CQ, "%s suspend schedule.\n", __func__);
	hvgr_sch_switch_off_request(gdev);
	spin_unlock_irqrestore(&gdev->cq_dev.schedule_lock, flags);
}

void hvgr_cq_clear_status_before_reset(struct hvgr_cq_ctx * const cq_ctx, uint32_t channel_id)
{
	struct hrtimer *schedule_timer = NULL;
	struct hvgr_ctx *ctx = cq_ctx->ctx;
	struct hvgr_cq_dev * const gcqctx = &ctx->gdev->cq_dev;
	struct cq_channel *cq_chan = &gcqctx->channel_info[channel_id];
	int ret;

	hvgr_debug(ctx->gdev, HVGR_CQ, "%s ctx_%u,pid=%d,tgid=%d,qid=%u,channel_id=%d\n",
		__func__, ctx->id, ctx->pid, ctx->tgid, cq_ctx->queue_id, channel_id);
	if (cq_chan->timer_running_flag) {
		schedule_timer = &cq_chan->schedule_timer;
		ret = hrtimer_try_to_cancel(schedule_timer);
		if (ret >= 0)
			cq_chan->timer_running_flag = false;
		cq_chan->timer_should_run = false;
	}

	/* avoid backup timer will cause reset again. */
	del_timer(&cq_ctx->backup_timer);
}

void hvgr_cq_schedule_soft_clr(struct hvgr_device * const gdev)
{
	struct hvgr_ctx *ctx = NULL;
	uint32_t chn;
	unsigned long flags;

	spin_lock_irqsave(&gdev->cq_dev.schedule_lock, flags);
	for (chn = 0; chn < CQ_CHANNEL_NUM; chn++) {
		struct hvgr_cq_ctx *cq_ctx = hvgr_cq_get_channel_ctx(gdev, chn);

		if (cq_ctx == NULL)
			continue;
		ctx = cq_ctx->ctx;

		hvgr_info(gdev, HVGR_CQ,
			"%s channel(%d) qid=%u,ctx_%u, kcts->tpid=%d, kcts->pid=%d\n",
			__func__, chn, cq_ctx->queue_id, ctx->id, cq_ctx->ctx->tgid,
			cq_ctx->ctx->pid);

		hvgr_cq_clear_status_before_reset(cq_ctx, chn);
	}
	spin_unlock_irqrestore(&gdev->cq_dev.schedule_lock, flags);
}

void hvgr_cq_schedule_softstop(struct hvgr_device * const gdev)
{
	struct hvgr_ctx *ctx = NULL;
	uint32_t chn;
	unsigned long flags;

	spin_lock_irqsave(&gdev->cq_dev.schedule_lock, flags);
	for (chn = 0; chn < CQ_CHANNEL_NUM; chn++) {
		struct hvgr_cq_ctx *cq_ctx = hvgr_cq_get_channel_ctx(gdev, chn);

		if (cq_ctx == NULL)
			continue;
		ctx = cq_ctx->ctx;

		if ((cq_ctx->flags & CQ_KMD_CTX_ENTRY_TIMEOUT) != 0) {
			cq_ctx->flags &= (~CQ_KMD_CTX_ENTRY_TIMEOUT);
			hvgr_debug(gdev, HVGR_CQ,
				"%s channel(%d) qid=%u,ctx_%u, kcts->tpid=%d, kcts->pid=%d"
				" entry timeout not unbind\n",
				__func__, chn, cq_ctx->queue_id, ctx->id, cq_ctx->ctx->tgid,
				cq_ctx->ctx->pid);
			continue;
		}

		hvgr_info(gdev, HVGR_CQ,
			"%s channel(%d) qid=%u,ctx_%u, kcts->tpid=%d, kcts->pid=%d\n",
			__func__, chn, cq_ctx->queue_id, ctx->id, cq_ctx->ctx->tgid,
			cq_ctx->ctx->pid);

		hvgr_cq_unbind_channel(cq_ctx, chn, 0); /* try but not wait. */
	}
	spin_unlock_irqrestore(&gdev->cq_dev.schedule_lock, flags);
}

static bool hvgr_cq_suspend_wait_condition(struct hvgr_device *gdev)
{
	struct hvgr_cq_dev * const gcqctx = &gdev->cq_dev;
	struct hvgr_pm_reset *pm_reset = &gdev->pm_dev.pm_reset;
	int reset_status;

	reset_status = atomic_read(&pm_reset->reset_status);
	/* Only exit protect mode and GPU IDLE at the same time can start suspend. */
	if (!gcqctx->protect_mode_info.protected_mode &&
		!gcqctx->protect_mode_info.protected_mode_transition &&
		gcqctx->gpu_sch_sts == HVGR_GPU_IDLE && reset_status == GPU_RESET_END)
			return true;

	return false;
}

int hvgr_cq_suspend(struct hvgr_device *gdev)
{
	struct hvgr_cq_dev * const gcqctx = &gdev->cq_dev;
	int ret;
	unsigned long flags;

	mutex_lock(&gcqctx->submit_lock);
	spin_lock_irqsave(&gdev->cq_dev.schedule_lock, flags);
	hvgr_info(gdev, HVGR_CQ, "%s begin\n", __func__);
	/* don't allow submit new queue. */
	hvgr_sch_switch_off_request(gdev);
	gcqctx->cq_suspend = true;

	if (gcqctx->protect_mode_info.protected_mode ||
		gcqctx->protect_mode_info.protected_mode_transition) {
		hvgr_debug(gdev, HVGR_CQ, "Just wait when in protect mode or in/out protect mode.");
		goto start_wait;
	}

	hvgr_cq_unbind_all_channel(gdev);

start_wait:
	spin_unlock_irqrestore(&gdev->cq_dev.schedule_lock, flags);

	ret = (int)wait_event_timeout(gdev->cq_dev.stop_queue_done_wait,
		hvgr_cq_suspend_wait_condition(gdev), CQ_SCH_TIMEOUT_UNBIND);
	if (ret == 0) {
		hvgr_err(gdev, HVGR_CQ, "suspend_test:Wait GPU idle timeout.");
		hvgr_sch_switch_on_request(gdev);
		gcqctx->cq_suspend = false;
		mutex_unlock(&gcqctx->submit_lock);
		return -1;
	}
	mutex_unlock(&gcqctx->submit_lock);
	hvgr_info(gdev, HVGR_CQ, "%s end\n", __func__);
	return 0;
}

void hvgr_cq_resume(struct hvgr_device *gdev)
{
	uint32_t chn;

	gdev->cq_dev.cq_suspend = false;
	/* We must config reg when core mask. */
	(void)hvgr_pm_request_gpu(gdev, false);
	hvgr_cq_config_bc_send_mode(gdev);
	for (chn = 0; chn < CQ_CHANNEL_NUM; chn++)
		hvgr_cq_affinity_config(gdev, chn);
	hvgr_pm_release_gpu(gdev);

	hvgr_cq_schedule_channel_all(gdev, true);
}

void hvgr_cq_dispatch_err_info(struct hvgr_device *gdev, struct hvgr_cq_ctx *cqctx)
{
	unsigned long flags;
	struct hvgr_cq_dev * const gcqctx = &gdev->cq_dev;

	hvgr_info(gdev, HVGR_CQ, "%s in", __func__);

	spin_lock_irqsave(&gcqctx->schedule_lock, flags);
	cqctx->dump_mem_flag = true;
	cqctx->empty_flag = 1;
	hvgr_cq_sched_update_dvfs_status(cqctx, HVGR_THRO_IDLE, HVGR_THRO_ACTION_RESTORE_INITAL);
	hvgr_cq_schedule_cq_out(cqctx, true);
	hvgr_cq_show_status_mod_record(cqctx);

	if (gcqctx->protect_mode_info.protected_mode ||
		gcqctx->protect_mode_info.protected_mode_transition) {
		if (cqctx == gcqctx->protect_mode_info.running_queue)
			gcqctx->protect_mode_info.running_queue = NULL;
	}
	spin_unlock_irqrestore(&gcqctx->schedule_lock, flags);

	/* gpu hang ,then come here. when err, data set CQ_ERR_FLOW_TOKEN_VALUE */
	if (!cqctx->ctx->ctx_cq.err_disp)
		hvgr_token_finish(cqctx->ctx, CQ_ERR_FLOW_TOKEN_VALUE);
	mutex_lock(&cqctx->dump_mem_lock);
	cqctx->dump_mem_flag = false;
	cqctx->ctx->ctx_cq.err_disp = true;
	wake_up(&cqctx->queue_done_wait);
	mutex_unlock(&cqctx->dump_mem_lock);
	hvgr_info(gdev, HVGR_CQ, "%s out", __func__);
}

static void hvgr_cq_schedule_stop_wq_byctx(struct hvgr_device *gdev, struct hvgr_ctx *ctx)
{
	struct hvgr_cq_ctx *cqctx = NULL;
	uint32_t idx;

	for (idx = 0; idx < ARRAY_SIZE(ctx->ctx_cq.cq_ctx); idx++) {
		cqctx = &ctx->ctx_cq.cq_ctx[idx];

		if (cqctx->sw_wr_ctx == NULL)
			continue;

		if (cqctx->wq_kwq != NULL) {
			(void)hvgr_wq_stop_work(cqctx);
#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0))
			flush_workqueue(cqctx->wq_kwq);
#else
			kthread_flush_work(&cqctx->wq_kwq_handle);
#endif
		}
	}
}

static void hvgr_cq_schedule_recover_wq_byctx(struct hvgr_device *gdev, struct hvgr_ctx *ctx)
{
	struct hvgr_cq_ctx *cqctx = NULL;
	uint32_t idx;

	for (idx = 0; idx < ARRAY_SIZE(ctx->ctx_cq.cq_ctx); idx++) {
		cqctx = &ctx->ctx_cq.cq_ctx[idx];

		if (cqctx->sw_wr_ctx == NULL)
			continue;
		hvgr_wq_recover(ctx, cqctx);
	}
}

#if defined(HVGR_WAITFENCE_LOWPOWER) && HVGR_WAITFENCE_LOWPOWER != 0
static void hvgr_cq_schedule_fence_clean_byctx(struct hvgr_device *gdev, struct hvgr_ctx *ctx)
{
	struct hvgr_cq_ctx *cqctx = NULL;
	uint32_t idx;

	for (idx = 0; idx < ARRAY_SIZE(ctx->ctx_cq.cq_ctx); idx++) {
		cqctx = &ctx->ctx_cq.cq_ctx[idx];

		if (cqctx->sw_wr_ctx == NULL)
			continue;
		hvgr_cq_wait_fence_data_term(cqctx, true);
	}
}
#endif

void hvgr_cq_clean_kernel_resource_byctx(struct hvgr_device *gdev, struct hvgr_ctx *ctx)
{
	hvgr_info(gdev, HVGR_CQ, "%s ctx_%u in", __func__, ctx->id);
	/* 2. flush softq workqueue. */
	hvgr_cq_schedule_stop_wq_byctx(gdev, ctx);

	/* 3. flush token list, need clear wait. */
	hvgr_token_clean_all(ctx, false);

#if defined(HVGR_WAITFENCE_LOWPOWER) && HVGR_WAITFENCE_LOWPOWER != 0
	/* 4. clean wait fence data. */
	hvgr_cq_schedule_fence_clean_byctx(gdev, ctx);
#endif

	/* 5. clear softq getindex and recover wq_entry_list. */
	hvgr_cq_schedule_recover_wq_byctx(gdev, ctx);

	/* 6. free all jit memory and sparse mem. */
	hvgr_mem_unbind_all_when_gpu_fault(ctx, HVGR_MEM_PROPERTY_SPARSE);
	hvgr_mem_free_area_when_gpu_fault(ctx, HVGR_MEM_PROPERTY_JIT);
	/* 7. end */
	hvgr_info(gdev, HVGR_CQ, "%s ctx_%u out", __func__, ctx->id);
}

static void hvgr_cq_clean_running_queue(struct hvgr_device *gdev, struct hvgr_cq_ctx *cqctx)
{
	if (cqctx == NULL)
		return;

	hvgr_cq_append_all_queue_with_ctx(gdev, cqctx->ctx, CQ_KMD_CTX_ERROR_FLAG);

	/* There are only one queue run when in protect mode. so just compare to ctx_old. */
	if (!cqctx->ctx->ctx_cq.err_disp)
		hvgr_cq_clean_kernel_resource_byctx(gdev, cqctx->ctx);
	hvgr_cq_dispatch_err_info(gdev, cqctx);
}

void hvgr_cq_clean_kernel_resource(struct hvgr_device *gdev)
{
	struct hvgr_cq_dev * const gcqctx = &gdev->cq_dev;
	unsigned long flags;
	uint32_t chn;
	struct hvgr_cq_ctx *cqctx = NULL;

	spin_lock_irqsave(&gcqctx->schedule_lock, flags);
	/* 1. set fault flag. */
	hvgr_cq_append_flag_all_queue(gdev, CQ_KMD_CTX_ERROR_FLAG, true);
	spin_unlock_irqrestore(&gcqctx->schedule_lock, flags);

	for (chn = 0; chn < CQ_CHANNEL_NUM; chn++) {
		cqctx = hvgr_cq_get_channel_ctx(gdev, chn);
		if (cqctx == NULL)
			continue;

		hvgr_debug(gdev, HVGR_CQ, "%s chn(%d) qid=%u,ctx_%u,get_index=%d, flags=0x%x\n",
			__func__, chn, cqctx->queue_id, cqctx->ctx->id,
			hvgr_get_queue_get_index(cqctx), cqctx->flags);

		if (!cqctx->ctx->ctx_cq.err_disp)
			hvgr_cq_clean_kernel_resource_byctx(gdev, cqctx->ctx);

		hvgr_cq_dispatch_err_info(gdev, cqctx);
#if defined(HVGR_FEATURE_HTS) && HVGR_FEATURE_HTS != 0
		hvgr_hts_block_proc(cqctx->ctx);
#endif
	}

	/* Only clear protect mode running queue, bak running queue will submit again. */
	if (gcqctx->protect_mode_info.running_queue != NULL) {
		cqctx = gcqctx->protect_mode_info.running_queue;

		hvgr_debug(gdev, HVGR_CQ, "%s clear running_qid=%u, ctx_%u", __func__,
			cqctx->queue_id, cqctx->ctx->id);

		hvgr_cq_clean_running_queue(gdev, cqctx);
	}

#if defined(HVGR_FEATURE_HTS) && HVGR_FEATURE_HTS != 0
	/* clear hts timeout cqctx */
	if (gdev->hts_dev.hts_timeout_cq_ctx != NULL) {
		cqctx = gdev->hts_dev.hts_timeout_cq_ctx;
		hvgr_debug(gdev, HVGR_HTS, "%s clear hts timeout qid=%u, ctx_%u", __func__,
			cqctx->queue_id, cqctx->ctx->id);
		hvgr_hts_block_proc(cqctx->ctx);
		hvgr_cq_clean_running_queue(gdev, cqctx);
		gdev->hts_dev.hts_timeout_cq_ctx = NULL;
		atomic_set(&gdev->hts_dev.hts_timeout_flag, 0);
	}
#endif
}

void hvgr_cq_schedule_reset_clear_task(struct hvgr_device *gdev)
{
	struct hvgr_cq_dev * const gcqctx = &gdev->cq_dev;

	mutex_lock(&gcqctx->err_recover_lock);
	hvgr_cq_clean_kernel_resource(gdev);
	mutex_unlock(&gcqctx->err_recover_lock);

	if (atomic_cmpxchg(&gcqctx->pm_pwr_flag, 1, 0) == 1)
		hvgr_queue_pm_release_work(gcqctx);
	else
		hvgr_debug(gdev, HVGR_CQ, "%s start power off. but refcount not right=%d.\n",
			__func__, atomic_read(&gcqctx->pm_pwr_flag));
}

void hvgr_update_freq_hint(struct hvgr_device *gdev)
{
	ktime_t now;
	uint32_t hint, queue_hint, i;
	bool cl_flag = false;
	bool cl_low_flag[CQ_CHANNEL_NUM] = {false};
	struct hvgr_cq_ctx *cqctx = NULL;
#ifdef CONFIG_HVGR_GPU_THROTTLE_DEVFREQ
	unsigned long freq;
#endif

	hint = HVGR_THRO_NORMAL;
	for (i = 0; i < CQ_CHANNEL_NUM; i++) {
		cqctx = hvgr_cq_get_channel_ctx(gdev, i);
		if (cqctx == NULL) {
			cl_low_flag[i] = true;
			continue;
		}
		if (cqctx->cur_dvfs_status < HVGR_THRO_NORMAL)
			cl_flag = true;

		if (cqctx->cur_dvfs_status == HVGR_THRO_LOW &&
			cqctx->pre_dvfs_status == HVGR_THRO_NORMAL)
			cl_low_flag[i] = true;

		queue_hint = cqctx->cur_dvfs_status == HVGR_THRO_NORMAL ?
			HVGR_THRO_MEDIUM : cqctx->cur_dvfs_status;
		if (queue_hint < hint)
			hint = queue_hint;
	}

	for (i = 0; i < CQ_CHANNEL_NUM; i++) {
		if (!cl_low_flag[i])
			break;
	}

#ifdef CONFIG_HVGR_GPU_THROTTLE_DEVFREQ
	freq = hvgr_pm_trans_thro_hint_to_freq(gdev, hint);
	if ((i == CQ_CHANNEL_NUM) && (hint == HVGR_THRO_LOW))
		hvgr_pm_set_force_freq(gdev, freq, true);
	else
		hvgr_pm_set_force_freq(gdev, freq, false);
#endif
	now = ktime_get();
	hvgr_pm_set_cl_job_flag(gdev, now, cl_flag);
}

void hvgr_cq_sched_update_dvfs_status(struct hvgr_cq_ctx *cqctx, uint32_t status, uint32_t aciton)
{
	if (unlikely(cqctx == NULL))
		return;

	switch (aciton) {
	case HVGR_THRO_ACTION_RESTORE_INITAL:
		cqctx->pre_dvfs_status = status;
		cqctx->cur_dvfs_status = status;
		break;
	case HVGR_THRO_ACTION_BIND:
		if (cqctx->pre_dvfs_status == HVGR_THRO_IDLE)
			cqctx->cur_dvfs_status = status;
		else
			cqctx->cur_dvfs_status = cqctx->pre_dvfs_status;
		break;
	case HVGR_THRO_ACTION_JOB_BOUNDARY:
	case HVGR_THRO_ACTION_DO_STOP:
		cqctx->pre_dvfs_status = cqctx->cur_dvfs_status;
		cqctx->cur_dvfs_status = status;
		break;
	default:
		hvgr_err(cqctx->ctx->gdev, HVGR_CQ, "Dvfs action is invalid.");
		break;
	}
}

void hvgr_cq_schedule_irq(struct hvgr_device * const gdev)
{
	uint32_t irq_top;
	struct hvgr_cq_dev *gcqctx = &gdev->cq_dev;
	uint32_t chn;

	if (gcqctx->gpu_sch_sts == HVGR_GPU_IDLE) {
		hvgr_debug(gdev, HVGR_CQ, "%s all jobs are done, no need to handle irq.", __func__);
		return;
	}

	irq_top = hvgr_read_reg(gdev, cq_control_reg(gdev, CQ_IRQ_TOP));
	while (irq_top != 0) {
		for (chn = 0; chn < CQ_CHANNEL_NUM; chn++) {
			if ((irq_top & (1U << chn)) == 0)
				continue;
			hvgr_cq_schedule_do_channel_irq(gdev, chn);
		}

		if (gcqctx->gpu_sch_sts == HVGR_GPU_IDLE)
			break;

		irq_top = hvgr_read_reg(gdev, cq_control_reg(gdev, CQ_IRQ_TOP));
	}
}

void hvgr_cq_schedule_reset(struct hvgr_device * const gdev, enum sch_rst_status reset_stage)
{
	switch (reset_stage) {
	case HVGR_SCH_SOFT_CLEAR:
		hvgr_cq_schedule_soft_clr(gdev);
		break;
	case HVGR_SCH_SOFT_STOP:
		hvgr_cq_schedule_softstop(gdev);
		break;
	case HVGR_SCH_SOFT_CLEAR_GPU_TASK:
		hvgr_cq_schedule_reset_clear_task(gdev);
		break;
	case HVGR_SCH_DUMP_REGISTER:
		hvgr_cq_dump_debug_register(gdev);
		break;
	default:
		break;
	}
}

void hvgr_cq_mod_status(struct hvgr_cq_ctx * const cq_ctx, enum cq_status new_sts,
	const char *name, uint64_t time)
{
	hvgr_info(cq_ctx->ctx->gdev, HVGR_CQ, "%s ctx_%u,qid=%u,pos=%d,name=%s,old_st=%u,new_st=%u",
		__func__, cq_ctx->ctx->id, cq_ctx->queue_id,
		cq_ctx->pos, name, cq_ctx->status, new_sts);
	cq_ctx->cq_sts_mod[cq_ctx->pos].old_status = cq_ctx->status;
	cq_ctx->cq_sts_mod[cq_ctx->pos].new_status = new_sts;
	cq_ctx->cq_sts_mod[cq_ctx->pos].func_name = name;
	cq_ctx->cq_sts_mod[cq_ctx->pos].time = time;
	cq_ctx->cq_sts_mod[cq_ctx->pos].priority = cq_ctx->priority;
	cq_ctx->cq_sts_mod[cq_ctx->pos].channel_id = cq_ctx->channel_id;
	cq_ctx->pos = ++(cq_ctx->pos) == CQ_STATUS_MOD_RECORD_MAX ? 0 : cq_ctx->pos;
	cq_ctx->status = new_sts;
}

void hvgr_cq_show_status_mod_record(struct hvgr_cq_ctx * const cq_ctx)
{
	uint32_t i;
	struct hvgr_device * const gdev = cq_ctx->ctx->gdev;

	if (unlikely(cq_ctx->pos >= CQ_STATUS_MOD_RECORD_MAX)) {
		hvgr_err(cq_ctx->ctx->gdev, HVGR_CQ, "%s pos over flow", __func__);
		return;
	}

	for (i = cq_ctx->pos; i < CQ_STATUS_MOD_RECORD_MAX; i++) {
		if (cq_ctx->cq_sts_mod[i].func_name == NULL)
			continue;
		hvgr_debug(gdev, HVGR_CQ, "%s ctx_%u,qid=%u pos=%u func=%s,old_sts=%u,new_sts=%u \n"
			"time=%lu,priority=%u,channel_id=%u",
			__func__, cq_ctx->ctx->id, cq_ctx->queue_id, i,
			cq_ctx->cq_sts_mod[i].func_name,
			cq_ctx->cq_sts_mod[i].old_status,
			cq_ctx->cq_sts_mod[i].new_status,
			cq_ctx->cq_sts_mod[i].time,
			cq_ctx->cq_sts_mod[i].priority,
			cq_ctx->cq_sts_mod[i].channel_id);
	}

	for (i = 0; i < cq_ctx->pos; i++) {
		if (cq_ctx->cq_sts_mod[i].func_name == NULL)
			continue;
		hvgr_debug(gdev, HVGR_CQ, "%s ctx_%u,qid=%u pos=%u func=%s,old_sts=%u,new_sts=%u \n"
			"time=%lu,priority=%u,channel_id=%u",
			__func__, cq_ctx->ctx->id, cq_ctx->queue_id, i,
			cq_ctx->cq_sts_mod[i].func_name,
			cq_ctx->cq_sts_mod[i].old_status,
			cq_ctx->cq_sts_mod[i].new_status,
			cq_ctx->cq_sts_mod[i].time,
			cq_ctx->cq_sts_mod[i].priority,
			cq_ctx->cq_sts_mod[i].channel_id);
	}
}

