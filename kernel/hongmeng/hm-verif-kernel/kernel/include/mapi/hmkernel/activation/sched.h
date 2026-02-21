/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Sched info for modules
 * Author: Huawei OS Kernel Lab
 * Create: Thu Nov 25 14:35:51 2021
 */
#ifndef MAPI_HMKERNEL_ACTIVATION_SCHED_H
#define MAPI_HMKERNEL_ACTIVATION_SCHED_H

#include <uapi/hmkernel/activation/sched.h>
#include <hmkernel/activation/struct.h>

#define ACTV_SCHED_SYNC_NORMAL		(1U) /* actv local area is inited, need sync state/cpu/resched info from local area */
#define ACTV_SCHED_SYNC_SKIP_NORMAL	(1U << 1) /* skip sync sched info of this actv because vspace does not match */
#define ACTV_SCHED_SYNC_LT		(1U << 2) /* need sync load tracking info to local area */

#define ACTV_SCHED_FLAG_NORMAL		(ACTV_SCHED_SYNC_NORMAL	| \
					 ACTV_SCHED_SYNC_SKIP_NORMAL)

#define ACTV_SCHED_FLAG_RESTORE_AFFINITY	(1U << 3)

#define ACTV_SCHED_SYNC_FS			(1U << 4) /* need sync fs info of actv before schedule */
#define ACTV_SCHED_RESTORE_UCTX			(1U << 5) /* need restore uctx from fs info of local area */
#define ACTV_SCHED_HANDLE_ITIMER		(1U << 6) /* need to handle itimer */
#define ACTV_SCHED_BIND_CPU			(1U << 7) /* need bind current cpu */

struct actv_s;
struct actv_pool_s;

#ifdef CONFIG_ACTV_SCHED
JAM_INLINE unsigned int
actv_schedflags_of(const struct actv_s *actv);
JAM_INLINE void
actv_schedflags_set(struct actv_s *actv, unsigned int flag);
JAM_INLINE void
actv_schedflags_clr(struct actv_s *actv, unsigned int flag);
JAM_INLINE bool
actv_schedflags_tst(const struct actv_s *actv, unsigned int flag);
JAM_INLINE void
actv_sched_set_recursive_preempt(struct actv_s *actv);
JAM_INLINE bool
actv_sched_is_recursive_preempt(const struct actv_s *actv);
#else
static inline unsigned int
actv_schedflags_of(const struct actv_s *actv)
{
	UNUSED(actv);
	return 0;
}

static inline void
actv_schedflags_set(struct actv_s *actv, unsigned int flag)
{
	UNUSED(actv, flag);
}

static inline void
actv_schedflags_clr(struct actv_s *actv, unsigned int flag)
{
	UNUSED(actv, flag);
}

static inline bool
actv_schedflags_tst(const struct actv_s *actv, unsigned int flag)
{
	UNUSED(actv, flag);
	return false;
}
static inline void
actv_sched_set_recursive_preempt(struct actv_s *actv)
{
	UNUSED(actv);
}
static inline bool
actv_sched_is_recursive_preempt(const struct actv_s *actv)
{
	UNUSED(actv);
	return false;
}
#endif
#endif
