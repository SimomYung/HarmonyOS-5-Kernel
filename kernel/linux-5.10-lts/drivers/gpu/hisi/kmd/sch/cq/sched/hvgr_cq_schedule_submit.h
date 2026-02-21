/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2021-2022. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */

#ifndef HVGR_SSID_H
#define HVGR_SSID_H

#include "hvgr_defs.h"

#define HVGR_SCH_ERR          (-EINVAL)
#define HVGR_SCH_END          0
#define HVGR_SCH_CONTINUE     2

void hvgr_cq_schedule_channel_all(struct hvgr_device * const gdev, bool sch_switch);

long hvgr_cq_schedule_submit(struct hvgr_ctx * const ctx, uint32_t queue_id);

long hvgr_cq_pm_wq_init(struct hvgr_device * const gdev);

long hvgr_cq_pm_wq_term(struct hvgr_device * const gdev);

#if defined(HVGR_WAITFENCE_LOWPOWER) && HVGR_WAITFENCE_LOWPOWER != 0
long hvgr_cq_wait_fence_wq_init(struct hvgr_device * const gdev);

long hvgr_cq_wait_fence_wq_term(struct hvgr_device * const gdev);
#endif

long hvgr_cq_schedule_submit_without_lock(struct hvgr_ctx * const ctx, struct hvgr_cq_ctx *cq_ctx,
    bool is_fence);

void hvgr_queue_pm_release_work(struct hvgr_cq_dev * const gcqctx);

#endif