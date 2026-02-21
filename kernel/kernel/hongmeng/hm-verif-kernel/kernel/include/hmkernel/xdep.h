/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: Implementation of futex
 * Author: Huawei OS Kernel Lab
 * Create: Fri Oct 25 09:00:00 2024
 */

#ifndef HMKERNEL_XDEP_H
#define HMKERNEL_XDEP_H

#include <hmkernel/tcb.h>

#ifdef CONFIG_XDEP_SYNC

enum xdep_sync_type {
	XDEP_RAW_MUTEX,
	XDEP_RAW_RWLOCK_SINGLE,
	XDEP_RAW_RWLOCK_MULTI,
	XDEP_SYNC_TYPE_UNKNOWN,
};

enum xdep_boost_type {
	XDEP_BOOST,
	XDEP_UNBOOST,
	XDEP_UNBOOST_GUARD,
	XDEP_BOOST_FAIL_BUSY,
};

bool xdep_is_critical_tcb(struct tcb_s *tcb);
cref_t xdep_holder_actv_of_tcb_cref(struct tcb_s *waiter_tcb, int val);
void xdep_boost_single_holder(struct tcb_s *waiter_tcb, int val, cref_t holder_cref, enum xdep_sync_type sync_type);
void xdep_boost_multi_holders(struct tcb_s *waiter_tcb, const void __user *u_holder_crefs);
int xdep_unboost_index_max_of(struct tcb_s *waiter_tcb, cref_t *holder_crefs);
void xdep_unboost(cref_t *holder_crefs, int index_max);

#ifdef CONFIG_XDEP_PRIO_GUARD
void xdep_prio_guard_unboost_nolock(struct tcb_s *tcb);
#endif /* CONFIG_XDEP_PRIO_GUARD */

#endif /* CONFIG_XDEP_SYNC */

#endif /* HMKERNEL_XDEP_H */
