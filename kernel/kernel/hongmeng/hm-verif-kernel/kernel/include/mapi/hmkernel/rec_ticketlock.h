/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: rec ticketlock for mapi
 * Author: Huawei OS Kernel Lab
 * Create: Wed Aug 28 10:45:14 2024
 */
#ifndef MAPI_HMKERNEL_REC_TICKETLOCK_H
#define MAPI_HMKERNEL_REC_TICKETLOCK_H

#include <hmkernel/compiler.h>
#include <hmkernel/types.h>
#include <hmkernel/smp.h>
#include <hmkernel/rec_ticketlock_struct.h>

#ifdef CONFIG_SMP
#include <hmkernel/lock_trace.h>

/*
 * KERNEL REC TICKETLOCK
 */
#define KREC_TICKETLOCK_INIT(type) {			\
		.__lock = REC_TICKETLOCK_INIT(),		\
		KLOCK_INIT_LOCK_TYPE(type)		\
	}

#define KREC_TICKETLOCK(name, type) struct krec_ticketlock_s name = KREC_TICKETLOCK_INIT(type);

static inline void __krec_ticketlock_init(struct krec_ticketlock_s *lock,
					__maybe_unused unsigned int lock_type)
{
	rec_ticketlock_init(&lock->__lock);
#ifdef CONFIG_KLOCK_TRACE
	lock->lock_type = lock_type;
#endif
}

static inline bool __krec_ticketlock_trylock(struct krec_ticketlock_s *lock)
{
	unsigned int owner_id = smp_current_cpu_id();
	bool is_owned = (rec_ticketlock_tryacquire(&lock->__lock, owner_id) == 1);
#ifdef CONFIG_KLOCK_TRACE
	if (is_owned) {
		lock_trace_push(ptr_to_vaddr(lock), lock->lock_type, true,
				owner_id, (vaddr_t)__builtin_return_address(0));
	}
#endif
	return is_owned;
}

static inline __always_inline void __krec_ticketlock_lock(struct krec_ticketlock_s *lock)
{
	unsigned int owner_id = smp_current_cpu_id();
#ifdef CONFIG_KLOCK_TRACE
	lock_trace_push(ptr_to_vaddr(lock), lock->lock_type, false,
			owner_id, (vaddr_t)__builtin_return_address(0));
#endif
	rec_ticketlock_acquire(&lock->__lock, owner_id);
#ifdef CONFIG_KLOCK_TRACE
	lock_trace_update_status(owner_id);
#endif
}

static inline __always_inline void __krec_ticketlock_unlock(struct krec_ticketlock_s *lock)
{
#ifdef CONFIG_KLOCK_TRACE
	lock_trace_pop(ptr_to_vaddr(lock), smp_current_cpu_id());
#endif
	rec_ticketlock_release(&lock->__lock);
}
#else

#define KREC_TICKETLOCK_INIT(type) {}
#define KREC_TICKETLOCK(name, type) struct krec_ticketlock_s name = KREC_TICKETLOCK_INIT(type);

static inline void __krec_ticketlock_init(struct krec_ticketlock_s *lock, unsigned int lock_type)
{
	UNUSED(lock, lock_type);
}

static inline bool __krec_ticketlock_trylock(struct krec_ticketlock_s *lock)
{
	UNUSED(lock);
	return true;
}

static inline __always_inline void __krec_ticketlock_lock(struct krec_ticketlock_s *lock)
{
	UNUSED(lock);
}

static inline __always_inline void __krec_ticketlock_unlock(struct krec_ticketlock_s *lock)
{
	UNUSED(lock);
}

#endif

#define krec_ticketlock_init(l, t)	__krec_ticketlock_init(l, t)
#define krec_ticketlock_trylock(l)	__krec_ticketlock_trylock(l)
#define krec_ticketlock_lock(l)		__krec_ticketlock_lock(l)
#define krec_ticketlock_unlock(l)	__krec_ticketlock_unlock(l)

#endif /* MAPI_HMKERNEL_REC_TICKETLOCK_H */
