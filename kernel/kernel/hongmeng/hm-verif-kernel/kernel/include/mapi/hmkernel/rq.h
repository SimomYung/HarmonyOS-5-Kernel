/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Export kernel rq.h to mapi
 * Author: Huawei OS Kernel Lab
 * Create: Thu Oct 17 16:27:37 2019
 */
#ifndef MAPI_HMKERNEL_RQ_H
#define MAPI_HMKERNEL_RQ_H

#include <uapi/hmkernel/rq.h>
#include <hmkernel/atomic.h>
#include <hmkernel/types.h>
#include <hmkernel/const.h>

#ifdef CONFIG_SMP
#ifdef CONFIG_GLOBAL_RT_RQ
struct sched_rt_info_s {
	vatomic32_t curr_prio;
	/* current running rt tcb exclude irq */
	struct tcb_s *curr;
};
#else
struct sched_rt_info_s {
	unsigned int curr_prio;
};
#endif
#endif

static inline bool
sched_need_module_rq(unsigned int rq)
{
#ifdef CONFIG_MODULE_SCHED_CLASS
	return ((rq >= HM_RUNQUEUE_SOFT_RT_MIN) &&
		(rq <= HM_RUNQUEUE_SOFT_RT_MAX));
#else
	UNUSED(rq);
	return false;
#endif
}

#ifdef CONFIG_MODULE_SCHED_CLASS
JAM_INLINE bool
check_rq_hook(unsigned int rq);
#else
static inline bool
check_rq_hook(unsigned int rq)
{
	UNUSED(rq);
	return true;
}
#endif

#endif
