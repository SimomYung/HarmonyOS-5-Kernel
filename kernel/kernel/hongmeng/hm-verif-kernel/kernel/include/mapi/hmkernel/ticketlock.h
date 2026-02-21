/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: ticketlock
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jun 28 11:20:51 2023
 */
#ifndef MAPI_HMKERNEL_TICKETLOCK_H
#define MAPI_HMKERNEL_TICKETLOCK_H

#include <hmkernel/compiler.h>
#include <hmkernel/types.h>
#include <hmkernel/smp.h>
#include <hmkernel/ticketlock_struct.h>

#ifdef CONFIG_SMP
#include <hmkernel/lock_trace.h>
#include <vsync/ticketlock.h>

/*
 * KERNEL TICKETLOCK
 */
#define KTICKETLOCK_INIT(type) {			\
		.__lock = TICKETLOCK_INIT(),		\
		KLOCK_INIT_LOCK_TYPE(type)		\
	}

#define KTICKETLOCK(name, type) struct kticketlock_s name = KTICKETLOCK_INIT(type);

static inline void __kticketlock_init(struct kticketlock_s *lock, __maybe_unused unsigned int lock_type)
{
	ticketlock_init(&lock->__lock);
#ifdef CONFIG_KLOCK_TRACE
	lock->lock_type = lock_type;
#endif
}

static inline bool __kticketlock_trylock(struct kticketlock_s *lock)
{
	bool is_owned = (ticketlock_tryacquire(&lock->__lock) == 1);
#ifdef CONFIG_KLOCK_TRACE
	if (is_owned) {
		lock_trace_push(ptr_to_vaddr(lock), lock->lock_type, true,
				smp_current_cpu_id(),
				(vaddr_t)__builtin_return_address(0));
	}
#endif
	return is_owned;
}

static inline __always_inline void __kticketlock_lock(struct kticketlock_s *lock)
{
#ifdef CONFIG_KLOCK_TRACE
	unsigned int cpu = smp_current_cpu_id();
	lock_trace_push(ptr_to_vaddr(lock), lock->lock_type, false, cpu,
			(vaddr_t)__builtin_return_address(0));
#endif
	ticketlock_acquire(&lock->__lock);
#ifdef CONFIG_KLOCK_TRACE
	lock_trace_update_status(cpu);
#endif
}

static inline __always_inline void __kticketlock_unlock(struct kticketlock_s *lock)
{
#ifdef CONFIG_KLOCK_TRACE
	lock_trace_pop(ptr_to_vaddr(lock), smp_current_cpu_id());
#endif
	ticketlock_release(&lock->__lock);
}
#else

#define KTICKETLOCK_INIT(type) {}
#define KTICKETLOCK(name, type) struct kticketlock_s name = KTICKETLOCK_INIT(type);

static inline void __kticketlock_init(struct kticketlock_s *lock, unsigned int lock_type)
{
	UNUSED(lock, lock_type);
}

static inline bool __kticketlock_trylock(struct kticketlock_s *lock)
{
	UNUSED(lock);
	return true;
}

static inline __always_inline void __kticketlock_lock(struct kticketlock_s *lock)
{
	UNUSED(lock);
}

static inline __always_inline void __kticketlock_unlock(struct kticketlock_s *lock)
{
	UNUSED(lock);
}

#endif

#define kticketlock_init(l, t)	__kticketlock_init(l, t)
#define kticketlock_trylock(l)	__kticketlock_trylock(l)
#define kticketlock_lock(l)	__kticketlock_lock(l)
#define kticketlock_unlock(l)	__kticketlock_unlock(l)

#endif /* !MAPI_HMKERNEL_TICKETLOCK_H */
