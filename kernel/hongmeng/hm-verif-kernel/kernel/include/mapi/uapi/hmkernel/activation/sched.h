/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Sched info for user
 * Author: Huawei OS Kernel Lab
 * Create: Mon Nov 29 11:51:02 2021
 */
#ifndef MAPI_UAPI_HMKERNEL_ACTIVATION_SCHED_H
#define MAPI_UAPI_HMKERNEL_ACTIVATION_SCHED_H

/*
 * Preemption now only work in module, if a module rq tcb is interrupted by kernel rq tcb
 * after disable process preempt, we should not update sched state or preempt state may be
 * cleared. So default state must be SCHED_STATE_SYNCED here.
 */
#define SCHED_STATE_SYNCED		0U /* sched state has been synced, default state */
#define SCHED_STATE_PREEMPT_ENABLED	1U /* process preempt enabled */
#define SCHED_STATE_PREEMPT_DISABLED	2U /* process preempt disabled */
#define SCHED_STATE_PREEMPT_BUSY	3U /* process preempt is disabled by another thread, try again */

#endif
