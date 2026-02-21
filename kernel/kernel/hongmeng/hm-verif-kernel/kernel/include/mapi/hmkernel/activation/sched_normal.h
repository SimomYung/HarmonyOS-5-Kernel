/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Normal sched info for modules
 * Author: Huawei OS Kernel Lab
 * Create: Mon Feb 27 17:52:19 2023
 */
#ifndef MAPI_HMKERNEL_ACTIVATION_SCHED_NORMAL_H
#define MAPI_HMKERNEL_ACTIVATION_SCHED_NORMAL_H

#include <hmkernel/types.h>
#include <hmkernel/strict.h>
#include <hmkernel/cpuset.h>
#include <hmkernel/activation/sched.h>

#ifdef CONFIG_ACTV_SCHED_NORMAL
JAM_INLINE bool
actv_sched_need_sync(const struct actv_s *actv);
JAM_INLINE bool
actv_sched_preempt_disabled(const struct actv_s *actv);
JAM_INLINE unsigned int
actv_sched_preempt_cpu_of(const struct actv_s *actv);
JAM_INLINE void
actv_sched_set_resched(struct actv_s *actv);
JAM_INLINE void
actv_sched_clear_preempt_info(struct actv_s *actv);
JAM_INLINE void
actv_sched_init(const struct actv_s *curr_actv, struct actv_s *next_actv);
bool actv_sched_info_sync(struct actv_s *actv, bool disabled);
JAM_INLINE unsigned int
actv_pool_sched_flags_of(const struct actv_pool_s *pool);
JAM_INLINE bool
actv_pool_affinity_is_enabled(const struct actv_pool_s *pool);
JAM_INLINE cpuset_t
actv_pool_affinity_of(const struct actv_pool_s *pool);
JAM_INLINE void
actv_set_restore_affinity(struct actv_s *actv, cpuset_t affinity);
JAM_INLINE cpuset_t
actv_affinity_of(const struct actv_s *actv);
JAM_INLINE bool
actv_need_restore_affinity(const struct actv_s *actv);
#ifdef CONFIG_SCHED_BIND_CPU
JAM_INLINE void
actv_sched_bind_cpu(struct tcb_s *tcb);
JAM_INLINE void
actv_sched_unbind_cpu(struct tcb_s *tcb);
#endif
#else
static inline bool
actv_sched_need_sync(const struct actv_s *actv)
{
	UNUSED(actv);
	return false;
}

static inline bool
actv_sched_preempt_disabled(const struct actv_s *actv)
{
	UNUSED(actv);
	return false;
}

static inline unsigned int
actv_sched_preempt_cpu_of(const struct actv_s *actv)
{
	UNUSED(actv);
	return 0U;
}

static inline void
actv_sched_set_resched(struct actv_s *actv)
{
	UNUSED(actv);
}

static inline void
actv_sched_clear_preempt_info(struct actv_s *actv)
{
	UNUSED(actv);
}

static inline void
actv_sched_init(const struct actv_s *curr_actv, struct actv_s *next_actv)
{
	UNUSED(curr_actv, next_actv);
}

static inline bool
actv_sched_info_sync(struct actv_s *actv, bool disabled)
{
	UNUSED(actv, disabled);
	return false;
}

static inline unsigned int
actv_pool_sched_flags_of(const struct actv_pool_s *pool)
{
	UNUSED(pool);
	return 0;
}

static inline bool
actv_pool_affinity_is_enabled(const struct actv_pool_s *pool)
{
	UNUSED(pool);
	return false;
}

static inline cpuset_t
actv_pool_affinity_of(const struct actv_pool_s *pool)
{
	UNUSED(pool);
	return cpuset_full;
}

static inline void
actv_set_restore_affinity(struct actv_s *actv, cpuset_t affinity)
{
	UNUSED(actv, affinity);
}

static inline cpuset_t
actv_affinity_of(const struct actv_s *actv)
{
	UNUSED(actv);
	return cpuset_full;
}

static inline bool
actv_need_restore_affinity(const struct actv_s *actv)
{
	UNUSED(actv);
	return false;
}

#ifdef CONFIG_SCHED_BIND_CPU
static inline void
actv_sched_bind_cpu(struct tcb_s *tcb)
{
	UNUSED(actv);
}

static inline void
actv_sched_unbind_cpu(struct tcb_s *tcb)
{
	UNUSED(tcb);
}
#endif
#endif
#endif
