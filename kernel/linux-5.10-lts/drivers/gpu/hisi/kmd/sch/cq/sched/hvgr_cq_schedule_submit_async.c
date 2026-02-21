/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2021-2022. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */

#include <linux/bitmap.h>
#include <linux/types.h>

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

#include "hvgr_cq_schedule.h"
#include "hvgr_cq_driver_base.h"
#include "hvgr_wq.h"
#include "hvgr_msync.h"
#include "hvgr_softjob.h"
#include "hvgr_cq_queue.h"
#include "hvgr_cq_debug_switch.h"
#include "hvgr_cq.h"
#include "hvgr_assert.h"
#include "hvgr_sch_policy_manager.h"
#include "hvgr_cq_driver_adapt.h"
#include "hvgr_protect_mode.h"
#include "hvgr_hts_api.h"
#include "hvgr_sch_dfx.h"
#include "hvgr_cq_schedule_submit.h"
#if defined(HVGR_SIMULATE) && HVGR_SIMULATE
#include "hvgr_cq_simulate.h"
#endif

#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0))
static void hvgr_schedule_pm_request_work(struct work_struct *data);
#else
static void hvgr_schedule_pm_request_work(struct kthread_work *data);
#endif
static void hvgr_queue_pm_request_work(struct hvgr_cq_dev * const gcqctx)
{
#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0))
	(void)queue_work(gcqctx->sched_pm_request_wq, &gcqctx->sched_pm_request_work);
#else
	(void)kthread_queue_work(gcqctx->sched_pm_request_wq, &gcqctx->sched_pm_request_work);
#endif
}
void hvgr_cq_schedule_channel_all(struct hvgr_device * const gdev, bool sch_switch)
{
	unsigned long flags;
	struct hvgr_cq_dev *gcqctx = &gdev->cq_dev;

	hvgr_info(gdev, HVGR_CQ, "%s in", __func__);

	spin_lock_irqsave(&gcqctx->schedule_lock, flags);
	if (sch_switch)
		hvgr_sch_switch_on_request(gdev);
	hvgr_hts_wait_list_in_schedule(gdev, HVGR_SCH_ONLY_INQUQUE);

	if (hvgr_cq_schedule_queue_is_empty(&gdev->cq_dev) || (!gcqctx->submit_allow)) {
		hvgr_info(gdev, HVGR_CQ, "%s pending queue null or sch=%d off, return.\n",
			__func__, gcqctx->submit_allow);
		spin_unlock_irqrestore(&gcqctx->schedule_lock, flags);
		return;
	}

	if (atomic_cmpxchg(&gcqctx->pm_pwr_flag, 0, 1) == 0) {
		hvgr_queue_pm_request_work(gcqctx);
		spin_unlock_irqrestore(&gcqctx->schedule_lock, flags);
		hvgr_info(gdev, HVGR_CQ, "%s need request gpu and out", __func__);
		return;
	}

	spin_unlock_irqrestore(&gcqctx->schedule_lock, flags);
	hvgr_info(gdev, HVGR_CQ, "%s out", __func__);
}


static long hvgr_cq_schedule_need_continue(struct hvgr_cq_ctx * const cq_ctx,
	struct hvgr_cq_dev * const gcqctx, bool is_fence)
{
	if (unlikely((cq_ctx->flags & CQ_KMD_CTX_START_SUBMIT) == 0)) {
		hvgr_err(cq_ctx->ctx->gdev, HVGR_CQ,
			"%s queue_id=%u flags=0x%x do not allow to submit!",
			__func__, cq_ctx->queue_id, cq_ctx->flags);
		hvgr_ctx_ioctl_record_print(cq_ctx->ctx);
		return HVGR_SCH_ERR;
	}

	if (hvgr_is_cqctx_in_protect_running_queue(cq_ctx)) {
		hvgr_info(cq_ctx->ctx->gdev, HVGR_CQ,
			"%s do not allow in pending list", __func__);
		return HVGR_SCH_END;
	}

#if defined(HVGR_WAITFENCE_LOWPOWER) && HVGR_WAITFENCE_LOWPOWER != 0
	if (!is_fence) {
		if (hvgr_cq_is_deal_wait_fence(cq_ctx)) {
			hvgr_info(cq_ctx->ctx->gdev, HVGR_CQ,
				"%s now is deal wait fence, not allow submit", __func__);
			return HVGR_SCH_END;
		}
	}
#endif

	if (!gcqctx->submit_allow) {
		hvgr_info(cq_ctx->ctx->gdev, HVGR_CQ, "%s queue_id=%u sch close, status=%u",
			__func__, cq_ctx->queue_id, cq_ctx->status);
		if (cq_ctx->status == CQ_STATUS_IDLE)
			(void)hvgr_cq_schedule_enqueue(gcqctx, cq_ctx);
		return HVGR_SCH_END;
	}

	if (!hvgr_cq_is_chn_running(gcqctx)) {
		if (cq_ctx->status == CQ_STATUS_IDLE) {
			(void)hvgr_cq_schedule_enqueue(gcqctx, cq_ctx);
			return HVGR_SCH_CONTINUE;
		} else {
			return HVGR_SCH_END;
		}
	}

	return HVGR_SCH_CONTINUE;
}

long hvgr_cq_schedule_submit_without_lock(struct hvgr_ctx * const ctx, struct hvgr_cq_ctx *cq_ctx,
	bool is_fence)
{
	struct hvgr_cq_dev *gcqctx = &ctx->gdev->cq_dev;
	long ret;

	ret = hvgr_cq_schedule_need_continue(cq_ctx, gcqctx, is_fence);
	if (ret != HVGR_SCH_CONTINUE)
		return ret;


	if (hvgr_cq_is_chn_running(gcqctx)) {
		hvgr_cq_schedule_start(ctx, gcqctx, cq_ctx);
		hvgr_hts_wait_list_in_schedule(ctx->gdev, HVGR_SCH_SUBMIT_ALL);
		hvgr_datan_jobtrace(ctx, JOBTRACE_TYPE_OBJ, KDATAN_TL_SUBMIT_QUEUE, ctx, cq_ctx->queue_id);
		return 0;
	}

	if ((atomic_cmpxchg(&gcqctx->pm_pwr_flag, 0, 1) == 0)) {
		hvgr_queue_pm_request_work(gcqctx);
		return 0;
	}
	return 0;
}

long hvgr_cq_schedule_submit(struct hvgr_ctx * const ctx, uint32_t queue_id)
{
	unsigned long flags;
	struct hvgr_cq_dev *gcqctx = &ctx->gdev->cq_dev;
	struct hvgr_cq_ctx *cq_ctx = NULL;
	long ret;

	hvgr_info(ctx->gdev, HVGR_SCH, "%s queue_id=%u\n", __func__, queue_id);
	/* Make sure the cq is legal. */
	cq_ctx = hvgr_cq_find_cqctx(ctx, queue_id);
	if (cq_ctx == NULL || cq_ctx->ctx == NULL) {
		hvgr_err(ctx->gdev, HVGR_SCH,
			"%s can not find cq ctx, queue_id=%u\n", __func__, queue_id);
		return -EINVAL;
	}

	cq_ctx->put_index_n = hvgr_get_queue_put_index(cq_ctx);
	hvgr_datan_jobtrace(ctx, JOBTRACE_TYPE_OBJ, KDATAN_JD_DUMP_PT, ctx);
	hvgr_datan_jobtrace(ctx, JOBTRACE_TYPE_OBJ, KDATAN_SET_PUT_IDX, ctx, queue_id,
		cq_ctx->sw_wr_ctx->queue_put_index);

#if defined(HVGR_SIMULATE) && HVGR_SIMULATE
	if (hvgr_cq_schedule_start_simu(cq_ctx) == 0)
		return 0;
#endif

	hvgr_systrace_begin("async:hvgr_cq_schedule_submit_without_lock ctx_%u, queue_id=%u",
		ctx->id, queue_id);
	spin_lock_irqsave(&gcqctx->schedule_lock, flags);
	ret = hvgr_cq_schedule_submit_without_lock(ctx, cq_ctx, false);
	if (ret == HVGR_SCH_ERR) {
		spin_unlock_irqrestore(&gcqctx->schedule_lock, flags);
		hvgr_systrace_end();
		return ret;
	}
	spin_unlock_irqrestore(&gcqctx->schedule_lock, flags);
	hvgr_systrace_end();
	return 0;
}

static void hvgr_cq_power_off_after_wq(struct hvgr_device * const gdev,
	struct hvgr_cq_dev * const gcqctx)
{
	if (gcqctx->gpu_sch_sts != HVGR_GPU_IDLE)
		return;

	if (atomic_cmpxchg(&gcqctx->pm_pwr_flag, 1, 0) != 1)
		hvgr_err(gdev, HVGR_SCH, "%s start power off, refcount=%d not right.\n",
			__func__, atomic_read(&gcqctx->pm_pwr_flag));

	hvgr_queue_pm_release_work(gcqctx);
}

#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0))
static void hvgr_schedule_pm_request_work(struct work_struct *data)
#else
static void hvgr_schedule_pm_request_work(struct kthread_work *data)
#endif
{
	struct hvgr_device *gdev = NULL;
	struct hvgr_cq_dev *gcqctx = NULL;
	unsigned long flags;

	gcqctx = container_of(data, struct hvgr_cq_dev, sched_pm_request_work);
	gdev = container_of(gcqctx, struct hvgr_device, cq_dev);

	hvgr_systrace_begin("async:hvgr_pm_request_gpu refcount=%d", atomic_read(&gcqctx->pm_pwr_flag));
	if (!hvgr_pm_request_gpu(gdev, false)) {
		atomic_set(&gcqctx->pm_pwr_flag, 0);
		hvgr_pm_release_gpu(gdev);
		hvgr_systrace_end();
		return;
	}
	hvgr_systrace_end();
	/*
	 * flush workqueue make sure sched_pm_release_work completed since when gpu idle
	 * sched_pm_release_wq will re-queue to sched_pm_release_wq
	 */

	hvgr_systrace_begin("async:hvgr_flush_pm_release_work");
	hvgr_flush_pm_release_work(gcqctx);
	hvgr_systrace_end();

	spin_lock_irqsave(&gcqctx->schedule_lock, flags);
	if (!gcqctx->submit_allow)
		goto exit;
	hvgr_cq_schedule_channel_start_all(gdev);
exit:
	hvgr_cq_power_off_after_wq(gdev, gcqctx);
	spin_unlock_irqrestore(&gcqctx->schedule_lock, flags);
	return;
}

long hvgr_cq_pm_wq_init(struct hvgr_device * const gdev)
{
	struct hvgr_cq_dev * const gcqctx = &gdev->cq_dev;

	atomic_set(&gcqctx->pm_pwr_flag, 0);

#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0))
	gcqctx->sched_pm_release_wq = alloc_workqueue("sched_pm_release_wq",
		WQ_MEM_RECLAIM | WQ_HIGHPRI | WQ_UNBOUND, 1);
	if (gcqctx->sched_pm_release_wq == NULL) {
		hvgr_err(gdev, HVGR_CQ, "alloc workqueue sched_pm_release_wq fail.\n");
		return -1;
	}
	INIT_WORK(&gcqctx->sched_pm_release_work, hvgr_schedule_pm_release_work);

	gcqctx->sched_pm_request_wq = alloc_workqueue("sched_pm_request_wq",
		WQ_MEM_RECLAIM | WQ_HIGHPRI | WQ_UNBOUND, 1);
	if (gcqctx->sched_pm_request_wq == NULL) {
		hvgr_err(gdev, HVGR_CQ, "alloc workqueue sched_pm_request_wq fail.\n");
		return -1;
	}
	INIT_WORK(&gcqctx->sched_pm_request_work, hvgr_schedule_pm_request_work);
#else
	gcqctx->sched_pm_release_wq = kthread_create_worker(0, "sched_pm_release_wq");
	if (IS_ERR(gcqctx->sched_pm_release_wq)) {
		hvgr_err(gdev, HVGR_CQ, "alloc workqueue sched_pm_release_wq fail.\n");
		return -1;
	}
	sched_set_fifo(gcqctx->sched_pm_release_wq->task);
	kthread_init_work(&gcqctx->sched_pm_release_work, hvgr_schedule_pm_release_work);

	gcqctx->sched_pm_request_wq = kthread_create_worker(0, "sched_pm_request_wq");
	if (IS_ERR(gcqctx->sched_pm_request_wq)) {
		hvgr_err(gdev, HVGR_CQ, "alloc workqueue sched_pm_request_wq fail.\n");
		return -1;
	}
	sched_set_fifo(gcqctx->sched_pm_request_wq->task);
	kthread_init_work(&gcqctx->sched_pm_request_work, hvgr_schedule_pm_request_work);
#endif

	return 0;
}

long hvgr_cq_pm_wq_term(struct hvgr_device * const gdev)
{
	struct hvgr_cq_dev * const gcqctx = &gdev->cq_dev;

#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0))
	if (gcqctx->sched_pm_release_wq != NULL)
		destroy_workqueue(gcqctx->sched_pm_release_wq);
	if (gcqctx->sched_pm_request_wq != NULL)
		destroy_workqueue(gcqctx->sched_pm_request_wq);
#else
	if (gcqctx->sched_pm_release_wq != NULL)
		kthread_destroy_worker(gcqctx->sched_pm_release_wq);
	if (gcqctx->sched_pm_request_wq != NULL)
		kthread_destroy_worker(gcqctx->sched_pm_request_wq);
#endif

	return 0;
}

#if defined(HVGR_WAITFENCE_LOWPOWER) && HVGR_WAITFENCE_LOWPOWER != 0
long hvgr_cq_wait_fence_wq_init(struct hvgr_device * const gdev)
{
	return 0;
}

long hvgr_cq_wait_fence_wq_term(struct hvgr_device * const gdev)
{
	return 0;
}
#endif
