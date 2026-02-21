/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: kernel spinlock header
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jun 14 15:22:55 2023
 */
#ifndef MAPI_HMKERNEL_SPINLOCK_H
#define MAPI_HMKERNEL_SPINLOCK_H

#include <hmkernel/compiler.h>
#include <hmkernel/types.h>
#include <hmkernel/smp.h>
#include <hmkernel/spinlock_struct.h>

#ifdef CONFIG_SMP
#include <hmkernel/lock_trace.h>
#include <vsync/spinlock.h>

/*
 * KERNEL SPINLOCK
 */
#define KSPINLOCK_INIT(type) {				\
		.__lock = SPINLOCK_INIT(),		\
		KLOCK_INIT_LOCK_TYPE(type)		\
	}

#define KSPINLOCK(name, type) struct kspinlock_s name = KSPINLOCK_INIT(type);

static inline void __kspinlock_init(struct kspinlock_s *lock, __maybe_unused unsigned int lock_type)
{
	spinlock_init(&lock->__lock);
#ifdef CONFIG_KLOCK_TRACE
	lock->lock_type = lock_type;
#endif
}

static inline bool __kspinlock_trylock(struct kspinlock_s *lock)
{
	bool is_owned = (spinlock_tryacquire(&lock->__lock) == 1);
#ifdef CONFIG_KLOCK_TRACE
	if (is_owned) {
		lock_trace_push(ptr_to_vaddr(lock), lock->lock_type, true,
				smp_current_cpu_id(),
				(vaddr_t)__builtin_return_address(0));
	}
#endif
	return is_owned;
}

static inline __always_inline void __kspinlock_lock(struct kspinlock_s *lock)
{
#ifdef CONFIG_KLOCK_TRACE
	unsigned int cpu = smp_current_cpu_id();
	lock_trace_push(ptr_to_vaddr(lock), lock->lock_type, false, cpu,
			(vaddr_t)__builtin_return_address(0));
#endif
	spinlock_acquire(&lock->__lock);
#ifdef CONFIG_KLOCK_TRACE
	lock_trace_update_status(cpu);
#endif
}

static inline __always_inline void __kspinlock_unlock(struct kspinlock_s *lock)
{
#ifdef CONFIG_KLOCK_TRACE
	lock_trace_pop(ptr_to_vaddr(lock), smp_current_cpu_id());
#endif
	spinlock_release(&lock->__lock);
}
#else

#define KSPINLOCK_INIT(type) {}
#define KSPINLOCK(name, type) struct kspinlock_s name = KSPINLOCK_INIT(type);

static inline void __kspinlock_init(struct kspinlock_s *lock, unsigned int lock_type)
{
	UNUSED(lock, lock_type);
}

static inline bool __kspinlock_trylock(struct kspinlock_s *lock)
{
	UNUSED(lock);
	return true;
}

static inline __always_inline void __kspinlock_lock(struct kspinlock_s *lock)
{
	UNUSED(lock);
}

static inline __always_inline void __kspinlock_unlock(struct kspinlock_s *lock)
{
	UNUSED(lock);
}
#endif

#define kspinlock_init(l, t)	__kspinlock_init(l, t)
#define kspinlock_trylock(l)	__kspinlock_trylock(l)
#define kspinlock_lock(l)	__kspinlock_lock(l)
#define kspinlock_unlock(l)	__kspinlock_unlock(l)

#endif /* !MAPI_HMKERNEL_SPINLOCK_H */
