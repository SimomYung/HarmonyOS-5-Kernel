/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: priority wrapper header file for user space
 * Author: Huawei OS Kernel Lab
 * Create: Mon Feb 17 14:55:08 2020
 */
#ifndef _LIBSCHED_PRIORITY_H
#define _LIBSCHED_PRIORITY_H


#ifdef CONFIG_MODULE_SCHED_CLASS
/* priority definition to align with POSIX. */
#include <hmkernel/sched/priority.h>

#else

/* Hongmeng internal priority definition, which is NOT compitible with POSIX. */
#include <hmkernel/rq.h>

#define HM_NR_PRIORITIES                HM_NR_RUNQUEUES

/*
 * NOTE: priority definity for OOM.
 * Dynamically increase priority is un-recommended behavior,
 * here just to be backward compatible with its behavior now.
 */
#define HM_PRIORITY_OOM_KILLER		HM_RUNQUEUE_VALID_MAX

/*
 * NOTE: HM_PRIORITY_FORCE_RECLIAM is a temporary definition for reclaimer.
 * Dynamically increase priority is un-recommended behavior,
 * here just to be backward compatible with its behavior now.
 */
#define HM_PRIORITY_FORCE_RECLIAM	HM_RUNQUEUE_VALID_MAX

#define HM_PRIORITY_SYSMGR_INIT         HM_RUNQUEUE_SYSMGR_INIT

#define HM_PRIORITY_USER_MAX            HM_RUNQUEUE_USER_MAX
#define HM_PRIORITY_USER_DEFAULT        HM_RUNQUEUE_USER_DEFAULT
#define HM_PRIORITY_USER_MIN            HM_RUNQUEUE_USER_MIN

#define HM_PRIORITY_ELASTIC		HM_RUNQUEUE_USER_MAX
#define HM_PRIORITY_WQ			HM_PRIORITY_ELASTIC
#define HM_PRIORITY_SYSMGR_WQ		(HM_RUNQUEUE_USER_DEFAULT + 1U)
#define HM_PRIORITY_SYSMGR_AUDIT	HM_PRIORITY_USER_MAX

/* below priority is not allowed to be set by user */
#define HM_PRIORITY_IDLE		HM_RUNQUEUE_IDLE
#define HM_PRIORITY_SUSPEND		HM_RUNQUEUE_SUSPEND
#endif

#endif
