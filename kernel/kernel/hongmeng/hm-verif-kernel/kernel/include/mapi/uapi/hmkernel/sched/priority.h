/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: HongMeng specific priorities definitions
 * Author: Huawei OS Kernel Lab
 * Create: Mon Feb 17 14:55:08 2020
 */
#ifndef UAPI_HMKERNEL_PRIORITY_H
#define UAPI_HMKERNEL_PRIORITY_H

#include <hmkernel/sched/rq.h>
/* priority definition to align with POSIX. */
#define HM_NR_PRIORITIES		140

#define HM_PRIORITY_SCHED_RT_MAX	119
#define HM_PRIORITY_SCHED_USER_RT_MAX	99
#define HM_PRIORITY_SCHED_RT_MIN	1
#define HM_PRIORITY_LIVEPATCH		119
#define HM_PRIORITY_WATCHDOG		119
#define HM_PRIORITY_HGUARD_MAIN		119
#define HM_PRIORITY_HGUARD_WORKER	116

#define HM_PRIORITY_UTIMERMGR		117

#define HM_PRIORITY_DH_KEVTD		110

#define HM_PRIORITY_OOM_KILLER		108
#define HM_PRIORITY_ELASTIC		105
#define HM_PRIORITY_WQ			105
#define HM_PRIORITY_FORCE_RECLIAM	103
#define HM_PRIORITY_SYSMGR_WQ		102
#define HM_PRIORITY_SYSMGR_AUDIT	102
#define HM_PRIORITY_SHUTDOWN		102

/* priority definition for TPPMGR */
#define HM_PRIORITY_TPPMGR_SCHED_IND	102

/* devhost using 107 as masked irq priority */
#define HM_PRIORITY_DH_IRQ_OFF 		107
/* priority definition for devhost */
#define HM_PRIORITY_DH_KVIC		102
#define HM_PRIORITY_DH_HIGH		101
#define HM_PRIORITY_DH_NORMAL		100

#define HM_PRIORITY_NATIVE_SERVICE_BASE	100

#define HM_PRIORITY_SYSMGR_INIT		40

#define HM_PRIORITY_NICE_MAX		19
#define HM_PRIORITY_NICE_MIN		(-20)
#define HM_PRIORITY_NICE_RANGE		((HM_PRIORITY_NICE_MAX - HM_PRIORITY_NICE_MIN) + 1)
#define HM_PRIORITY_NICE		(HM_PRIORITY_NICE_RANGE / 2)
#define HM_PRIORITY_MIN			1

#define HM_PRIORITY_USER_MAX		HM_PRIORITY_SCHED_RT_MAX
#define HM_PRIORITY_USER_DEFAULT	40
#define HM_PRIORITY_USER_MIN		HM_PRIORITY_NICE_MIN

#define HM_PRIORITY_IDLE		0

#define PRIO_PROCESS			0
#define PRIO_PGRP			1
#define PRIO_USER			2

/* unified priority cross multi sched classes.
 * SCHED_RT   : [41, 139]
 * SCHED_OTHER: [1, 40], unice = 20 - knice = [-20, 19]
 */
#define HM_PRIORITY_SCHED_OTHER_OFFSET	HM_PRIORITY_IDLE
#define HM_PRIORITY_SCHED_RT_OFFSET	((HM_PRIORITY_NICE_RANGE) + (HM_PRIORITY_IDLE))

#define nice_to_ices_priority(nice) \
	(HM_PRIORITY_NICE - (nice))
#define ices_priority_to_nice(ices_priority) \
	(HM_PRIORITY_NICE - (ices_priority))

static inline _Bool
sched_check_rt_priority(int priority)
{
	return priority >= HM_PRIORITY_SCHED_RT_MIN &&
	       priority <= HM_PRIORITY_SCHED_RT_MAX;
}

#endif
