/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 * Description: Rwlock
 * Author: Huawei OS Kernel Lab
 * Create: Aug 13 15:45 2021
 */

#ifndef MAPI_HMKERNEL_RWLOCK_H
#define MAPI_HMKERNEL_RWLOCK_H

#include <hmkernel/compiler.h>
#include <hmkernel/types.h>
#include <hmkernel/smp.h>
#include <hmkernel/rwlock_struct.h>

#ifdef CONFIG_SMP
#include <lib/atomic.h>
#include <uapi/hmkernel/barrier.h>
#include <hmkernel/lock_trace.h>

/*
 * KERNEL RWLOCK
 */
#define KRWLOCK_INIT(type) {			\
		.rwlock = RWLOCK_INIT(),	\
		KLOCK_INIT_LOCK_TYPE(type)	\
	}

#define KRWLOCK(name, type) struct krwlock_s name = KRWLOCK_INIT(type);

static inline void __krwlock_init(struct krwlock_s *lock, __maybe_unused unsigned int lock_type)
{
	rwlock_init(&lock->rwlock);
#ifdef CONFIG_KLOCK_TRACE
	lock->lock_type = lock_type;
#endif
}

static inline void __read_lock(struct krwlock_s *lock)
{
#ifdef CONFIG_KLOCK_TRACE
	unsigned int curr_cpu = smp_current_cpu_id();
	lock_trace_push(ptr_to_vaddr(lock), lock->lock_type, false, curr_cpu,
			(vaddr_t)__builtin_return_address(0));
#endif
	rwlock_read_acquire(&lock->rwlock);
#ifdef CONFIG_KLOCK_TRACE
	lock_trace_update_status(curr_cpu);
#endif
}

static inline void __read_unlock(struct krwlock_s *lock)
{
#ifdef CONFIG_KLOCK_TRACE
	lock_trace_pop(ptr_to_vaddr(lock), smp_current_cpu_id());
#endif
	rwlock_read_release(&lock->rwlock);
}

static inline int __read_trylock(struct krwlock_s *lock)
{
	/*
	 * Return value judgment should follow the function call.
	 * So it could not be written as
	 *
	 *  int ret = xxx
	 * then
	 *  return ret.
	 */
#ifdef CONFIG_KLOCK_TRACE
	int ret = rwlock_read_tryacquire(&lock->rwlock);
	if (ret != 0) {
		lock_trace_push(ptr_to_vaddr(lock), lock->lock_type, true,
				smp_current_cpu_id(),
				(vaddr_t)__builtin_return_address(0));
	}
	return ret;
#else
	return rwlock_read_tryacquire(&lock->rwlock);
#endif
}

static inline void __write_lock(struct krwlock_s *lock)
{
#ifdef CONFIG_KLOCK_TRACE
	unsigned int curr_cpu = smp_current_cpu_id();
	lock_trace_push(ptr_to_vaddr(lock), lock->lock_type, false, curr_cpu,
			(vaddr_t)__builtin_return_address(0));
#endif
	rwlock_write_acquire(&lock->rwlock);
#ifdef CONFIG_KLOCK_TRACE
	lock_trace_update_status(curr_cpu);
#endif
}

static inline void __write_unlock(struct krwlock_s *lock)
{
#ifdef CONFIG_KLOCK_TRACE
	lock_trace_pop(ptr_to_vaddr(lock), smp_current_cpu_id());
#endif
	rwlock_write_release(&lock->rwlock);
}

static inline int __write_trylock(struct krwlock_s *lock)
{
#ifdef CONFIG_KLOCK_TRACE
	int ret = rwlock_write_tryacquire(&lock->rwlock);
	if (ret != 0) {
		lock_trace_push(ptr_to_vaddr(lock), lock->lock_type, true,
				smp_current_cpu_id(),
				(vaddr_t)__builtin_return_address(0));
	}
	return ret;
#else
	return rwlock_write_tryacquire(&lock->rwlock);
#endif
}

#else

#define KRWLOCK_INIT(type) {}
#define KRWLOCK(name, type) struct krwlock_s name = KRWLOCK_INIT(type)

static inline void __krwlock_init(struct krwlock_s *lock, unsigned int lock_type)
{
	UNUSED(lock, lock_type);
}

static inline void __read_lock(struct krwlock_s *lock)
{
	UNUSED(lock);
}

static inline void __read_unlock(struct krwlock_s *lock)
{
	UNUSED(lock);
}

static inline int __read_trylock(struct krwlock_s *lock)
{
	UNUSED(lock);
	return 0;
}

static inline void __write_lock(struct krwlock_s *lock)
{
	UNUSED(lock);
}

static inline void __write_unlock(struct krwlock_s *lock)
{
	UNUSED(lock);
}

static inline int __write_trylock(struct krwlock_s *lock)
{
	UNUSED(lock);
	return 0;
}

#endif /* !CONFIG_SMP */

/* Per-cpu krwlock */
#define krwlock_init(l, t)	__krwlock_init(l, t)

#define read_lock(l)		__read_lock(l)
#define read_trylock(l)		__read_trylock(l)
#define read_unlock(l)		__read_unlock(l)

#define write_lock(l)		__write_lock(l)
#define write_trylock(l)	__write_trylock(l)
#define write_unlock(l)		__write_unlock(l)

#endif /* !MAPI_HMKERNEL_RWLOCK_H */
