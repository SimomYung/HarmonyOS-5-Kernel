/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2022. All rights reserved.
 * Description: HongMeng specific rq definitions
 * Author: Huawei OS Kernel Lab
 * Create: Sat Jul 28 14:02:13 2018
 */
#ifndef UAPI_HMKERNEL_RQ_H
#define UAPI_HMKERNEL_RQ_H

#define HM_NR_RUNQUEUES	64U

/* between 32 and 63 is hard RT range, is managed by kernel.
 * enqueue kernel rqs need to meet these conditions:
 * 1. only sysmgr can set tcb to hard RT.
 * 2. tcb can be chosen even though preempt is disable.
 * 3. tcb`s priority level is above all the sched policies. */
#define HM_RUNQUEUE_HARD_RT_MAX 63U
/* reserver 8 runqueues for kernel */

/* above runqueues is not allowed to be set by user */
#define HM_RUNQUEUE_USER_MAX	55U
#define HM_RUNQUEUE_SNAPSHOT 	HM_RUNQUEUE_USER_MAX

#define HM_RUNQUEUE_USER_DEFAULT 51U
#define HM_RUNQUEUE_SYSMGR_INIT	50U

#define HM_RUNQUEUE_HARD_RT_MIN 32U
/* define this runqueue to ensure tcb migrate success in irq capcall */
#define HM_RUNQUEUE_IRQ_SWITCH_CPU  HM_RUNQUEUE_HARD_RT_MIN

/* [2, 31] is soft RT range, which is managed by the module */
#define HM_RUNQUEUE_SOFT_RT_MAX 31U
#define HM_RUNQUEUE_SOFT_RT_MIN 2U
#define HM_RUNQUEUE_BOOST	31U
#define HM_RUNQUEUE_STOP	30U
#define HM_RUNQUEUE_PREV_IDLE	2U
/* below runqueues is not allowed to be set by user */
#define HM_RUNQUEUE_USER_MIN	2U
#define HM_RUNQUEUE_IDLE	1U
#define HM_RUNQUEUE_SUSPEND	0U

#endif
