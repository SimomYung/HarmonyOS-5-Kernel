/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 * Description: Recursive spinlock
 * Author: Huawei OS Kernel Lab
 * Create: Feb 05 16:49 2020
 */

#ifndef MAPI_HMKERNEL_RECLOCK_H
#define MAPI_HMKERNEL_RECLOCK_H

#include <hmkernel/compiler.h>
#include <hmkernel/types.h>
#include <hmkernel/smp.h>
#include <hmkernel/reclock_struct.h>

#ifdef CONFIG_SMP
#include <lib/atomic.h>
#include <uapi/hmkernel/barrier.h>
#include <hmkernel/lock_trace.h>

#ifdef CONFIG_REC_SPINLOCK_WFE
#include <hmasm/rec_spinlock.h>
#endif

/*
 * Recursive spinlock
 *
 * The owner holds the lock (cnt + 1) times
 *
 * Unlocked state: (owner == __REC_LOCK_OWNER_INVAL && cnt == 0)
 */
#define __REC_LOCK_OWNER_INVAL (unsigned int)(-1)

#define REC_SPINLOCK_INIT(type) {				\
		.owner = VATOMIC_INIT(__REC_LOCK_OWNER_INVAL),	\
		.cnt = 0,					\
		KLOCK_INIT_LOCK_TYPE(type)			\
	}

#define REC_SPINLOCK(name, type)	struct rec_spinlock_s name = REC_SPINLOCK_INIT(type);

static inline void __rec_spinlock_init(struct rec_spinlock_s *lock, unsigned int lock_type)
{
	vatomic32_write_rlx(&lock->owner, __REC_LOCK_OWNER_INVAL);
	lock->cnt = 0;
#ifdef CONFIG_KLOCK_TRACE
	lock->lock_type = lock_type;
#endif
}

static inline bool __rec_spinlock_trylock(struct rec_spinlock_s *lock, unsigned int ownerid, bool need_trace)
{
#ifdef CONFIG_REC_SPINLOCK_WFE
	u32 is_owned = __do_rec_spinlock_trylock(lock, ownerid);
#else
	unsigned int lock_owner = vatomic32_read_rlx(&lock->owner);
	bool is_owned = (lock_owner == ownerid);
	if (is_owned) {
		lock->cnt++;
	} else if (lock_owner == __REC_LOCK_OWNER_INVAL) {
		is_owned = vatomic32_cmpxchg(&lock->owner, __REC_LOCK_OWNER_INVAL, ownerid) == __REC_LOCK_OWNER_INVAL;
	} else {
		/* do nothing */
	}
#endif

#ifdef CONFIG_KLOCK_TRACE
	if (need_trace && is_owned) {
		lock_trace_push(ptr_to_vaddr(lock), lock->lock_type, true, ownerid,
				(vaddr_t)__builtin_return_address(0));
	}
#endif

	return (bool)is_owned;
}

static inline __always_inline void __rec_spinlock_lock(struct rec_spinlock_s *lock, unsigned int ownerid)
{
#ifdef CONFIG_KLOCK_TRACE
	lock_trace_push(ptr_to_vaddr(lock), lock->lock_type, false, ownerid,
			(vaddr_t)__builtin_return_address(0));
#endif

#ifdef CONFIG_REC_SPINLOCK_WFE
	__do_rec_spinlock_spin_lock_wfe(lock, ownerid);
#else
	while (__rec_spinlock_trylock(lock, ownerid, false) == false) {
		/* do nothing */
	};
	smp_rmb();
#endif

#ifdef CONFIG_KLOCK_TRACE
	lock_trace_update_status(ownerid);
#endif
}

static inline __always_inline void __rec_spinlock_unlock(struct rec_spinlock_s *lock)
{
#ifdef CONFIG_KLOCK_TRACE
	unsigned int cpuid = smp_current_cpu_id();
	lock_trace_pop(ptr_to_vaddr(lock), cpuid);
#endif

#ifdef CONFIG_REC_SPINLOCK_WFE
	__do_rec_spinlock_unlock_wfe(lock);
#else
	if (lock->cnt == 0) {
		smp_mb();
		vatomic32_write_rlx(&lock->owner, __REC_LOCK_OWNER_INVAL);
	} else {
		lock->cnt--;
	}
#endif
}

#else

#define REC_SPINLOCK_INIT(type) {}
#define REC_SPINLOCK(name, type)	struct rec_spinlock_s name = REC_SPINLOCK_INIT(type);

static inline void __rec_spinlock_init(struct rec_spinlock_s *lock, unsigned int lock_type)
{
	UNUSED(lock, lock_type);
}

static inline bool __rec_spinlock_trylock(struct rec_spinlock_s *lock, unsigned int ownerid, bool need_trace)
{
	UNUSED(lock, ownerid, need_trace);
	return false;
}

static inline void __rec_spinlock_lock(struct rec_spinlock_s *lock, unsigned int ownerid)
{
	UNUSED(lock, ownerid);
}

static inline void __rec_spinlock_unlock(struct rec_spinlock_s *lock)
{
	UNUSED(lock);
}

#endif /* !CONFIG_SMP */

/* Per-cpu rec spinlock */
#define rec_spinlock_init(l, t) __rec_spinlock_init(l, t)
#define rec_spinlock_trylock(l) __rec_spinlock_trylock(l, smp_current_cpu_id(), true)
#define rec_spinlock_lock(l) __rec_spinlock_lock(l, smp_current_cpu_id())
#define rec_spinlock_unlock(l) __rec_spinlock_unlock(l)

#endif /* !MAPI_HMKERNEL_RECLOCK_H */
