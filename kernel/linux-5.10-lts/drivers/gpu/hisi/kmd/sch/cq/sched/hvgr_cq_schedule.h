/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2021-2022. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */

#ifndef HVGR_CQ_SCHEDULE_H
#define HVGR_CQ_SCHEDULE_H

#include <linux/ktime.h>
#include "hvgr_defs.h"
#include "hvgr_cq_data.h"
#include "hvgr_sch_api.h"

#define CQ_NEED_SCHEDULE_TIMER_FLAG        1U
#define CQ_NEED_UNBIND_TIMER_FLAG          2U
#define CQ_MAX_SUB_CHANNEL_NUM             8U
#define MEM_PAGE_SHIFT                     12U
#define MEM_PAGE_SIZE                      (1ul << MEM_PAGE_SHIFT)
#define mem_page_align_size(size)          (((size) + MEM_PAGE_SIZE - 1) & \
	(~(MEM_PAGE_SIZE - 1)))

#define CQ_SCH_TIME_ROBIN_MS               100000u        /* 100ms */
#define CQ_SCH_TIMEOUT_ROBIN               5u             /* 0.5s */
#define CQ_ENTRY_TIME_CL                   3000U          /* 3s */
#define CQ_ENTRY_TIME                      1000U          /* 1s */
#define CQ_TIME_MS2TICK                    100u

#define CQ_SCH_TIMEOUT_UNBIND              (10 * HZ)      /* 10s */
#define CQ_WAIT_FENCE_TIMEOUT              1500u    /* 1.5s */

#define HVGR_CQ_PROTECT_MODE_ACTIVE        (1U << 11)
#define HVGR_CQ_PROTECT_TRY_MAX_TIMES      10

#define bind_end_flush_l2_set_no_flush(value) ((value) & (~(0x3 << 8)))
#define bind_end_flush_set_bin_en(value) ((((value) & (~(0xf << 10))) | (1 << 10)))

long hvgr_cq_schedule_init(struct hvgr_device * const gdev);

void hvgr_cq_schedule_unsubmit(struct hvgr_ctx * const ctx, struct hvgr_cq_ctx *cq_ctx);

void hvgr_cq_schedule_irq(struct hvgr_device * const gdev);

bool hvgr_is_cl_task(struct hvgr_device *gdev, uint32_t channel_id);

bool hvgr_cq_is_queue_error(struct hvgr_cq_ctx const *cq_ctx);

bool hvgr_cq_has_queue_error(struct hvgr_ctx const *ctx);

long hvgr_cq_clear_err(struct hvgr_ctx * const ctx, uint32_t queue_id);

void hvgr_update_freq_hint(struct hvgr_device *gdev);

void hvgr_cq_sched_update_dvfs_status(struct hvgr_cq_ctx *cqctx, uint32_t status, uint32_t aciton);

void hvgr_cq_schedule_channel(struct hvgr_device * const gdev, uint32_t channel_id,
	enum protect_flag flag, bool need_stop);

long hvgr_cq_schedule_unqueue(struct hvgr_cq_dev * const gcqctx,
	struct hvgr_cq_ctx * const cq_ctx);

bool hvgr_cq_is_queue_running(struct hvgr_cq_dev *gcqctx, struct hvgr_cq_ctx * const cq_ctx);

void hvgr_cq_schedule_submit_change_to_hw(struct hvgr_cq_ctx * const cq_ctx);


void hvgr_cq_unbind_all_channel(struct hvgr_device *gdev);

void hvgr_cq_schedule_cq_out(struct hvgr_cq_ctx * const cq_ctx, bool enqueue_flag);

bool hvgr_cq_is_need_unbind(bool empty_flag, struct hvgr_cq_ctx * const cq_ctx);

void hvgr_cq_unbind_channel(struct hvgr_cq_ctx * const cq_ctx,
	uint32_t channel_id, uint32_t sche_timer_flag);

void hvgr_cq_schedule_cq_in(struct hvgr_cq_ctx * const cq_ctx, uint32_t channel_id);

bool hvgr_cq_is_need_into_pending(bool enqueue_flag, struct hvgr_cq_ctx * const cq_ctx);

long hvgr_cq_schedule_enqueue(struct hvgr_cq_dev * const gcqctx,
	struct hvgr_cq_ctx * const cq_ctx);

void hvgr_cq_schedule_stop_submit(struct hvgr_device * const gdev);

int hvgr_cq_suspend(struct hvgr_device *gdev);

void hvgr_cq_resume(struct hvgr_device *gdev);

void hvgr_cq_schedule_reset(struct hvgr_device * const gdev, enum sch_rst_status reset_stage);

void hvgr_cq_start_and_schedule(struct hvgr_device * const gdev);

void hvgr_cq_hts_schedule(struct hvgr_device * const gdev, struct hvgr_cq_ctx *cq_ctx,
	enum sch_type type);

bool hvgr_cq_schedule_find_queue(struct hvgr_cq_dev * const gcqctx,
	struct hvgr_cq_ctx * const cq_ctx);

bool hvgr_cq_schedule_queue_is_empty(struct hvgr_cq_dev * const gcqctx);

#if defined(HVGR_WAITFENCE_LOWPOWER) && HVGR_WAITFENCE_LOWPOWER != 0
bool hvgr_cq_is_deal_wait_fence(struct hvgr_cq_ctx * const cq_ctx);
#endif

void hvgr_cq_schedule_start(struct hvgr_ctx * const ctx,
	struct hvgr_cq_dev * const gcqctx, struct hvgr_cq_ctx * const cq_ctx);

void hvgr_flush_pm_release_work(struct hvgr_cq_dev * const gcqctx);

void hvgr_cq_schedule_channel_start_all(struct hvgr_device * const gdev);

bool hvgr_is_cqctx_in_protect_running_queue(struct hvgr_cq_ctx *cq_ctx);

#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0))
void hvgr_schedule_pm_release_work(struct work_struct *data);
#else
void hvgr_schedule_pm_release_work(struct kthread_work *data);
#endif

bool hvgr_cq_is_chn_running(struct hvgr_cq_dev *gcqctx);

void hvgr_cq_schedule_channel_start_no_check(struct hvgr_device * const gdev, uint32_t channel_id);

#define hvgr_cq_mod_status_macro(cq_ctx, new_sts) \
	hvgr_cq_mod_status(cq_ctx, new_sts, __func__, (uint64_t)ktime_to_ns(ktime_get()))
void hvgr_cq_mod_status(struct hvgr_cq_ctx * const cq_ctx, enum cq_status new_sts,
	const char *name, uint64_t time);

void hvgr_cq_show_status_mod_record(struct hvgr_cq_ctx * const cq_ctx);

#endif
