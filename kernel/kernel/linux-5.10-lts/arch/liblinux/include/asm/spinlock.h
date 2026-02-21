/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef __LIBLINUX_ASM_SPINLOCK_H
#define __LIBLINUX_ASM_SPINLOCK_H

#ifdef CONFIG_LIBLINUX_USE_NATIVE_SPINLOCK

/*
 * use linux native spinlock
 */
#include_next <asm/spinlock.h>

#else

/*
 * use spinlock in PAL
 */
#include <liblinux/pal.h>

#include <asm/processor.h>
#include <asm/spinlock_types.h>
#include <linux/compiler.h>

static inline void arch_spin_unlock_wait(arch_spinlock_t *lock)
{
	liblinux_pal_mutex_lock(lock);
	liblinux_pal_mutex_unlock(lock);
}

#define arch_spin_lock_flags(lock, flags) arch_spin_lock(lock)

static inline void arch_spin_lock(arch_spinlock_t *lock)
{
	liblinux_pal_mutex_lock(lock);
}

static inline int arch_spin_trylock(arch_spinlock_t *lock)
{
	return !liblinux_pal_mutex_trylock(lock);
}

static inline void arch_spin_unlock(arch_spinlock_t *lock)
{
	liblinux_pal_mutex_unlock(lock);
}

static inline int arch_spin_value_unlocked(arch_spinlock_t lock)
{
	int ret = liblinux_pal_mutex_trylock(&lock);
	if (ret == 0) {
		liblinux_pal_mutex_unlock(&lock);
		return 1;
	}
	return 0;
}

static inline int arch_spin_is_locked(arch_spinlock_t *lock)
{
	return liblinux_pal_mutex_is_locked(lock);
}

static inline int arch_spin_is_contended(arch_spinlock_t *lock)
{
	int ret = liblinux_pal_mutex_trylock(lock);
	if (ret == 0) {
		liblinux_pal_mutex_unlock(lock);
		return 0;
	}
	return 1;
}

#define arch_spin_is_contended	arch_spin_is_contended

#define arch_write_lock        arch_spin_lock
#define arch_write_trylock     arch_spin_trylock
#define arch_write_unlock      arch_spin_unlock
#define arch_write_can_lock(l) (!arch_spin_is_contended(l))
#define arch_read_lock         arch_spin_lock
#define arch_read_trylock      arch_spin_trylock
#define arch_read_unlock       arch_spin_unlock
#define arch_read_can_lock(l)  (!arch_spin_is_contended(l))

#define arch_read_lock_flags(lock, flags)  arch_read_lock(lock)
#define arch_write_lock_flags(lock, flags) arch_write_lock(lock)

#define arch_spin_relax(lock)  cpu_relax()
#define arch_read_relax(lock)  cpu_relax()
#define arch_write_relax(lock) cpu_relax()

#ifdef CONFIG_ARM64
#define smp_mb__before_spinlock() smp_mb()
#endif

#endif /* !CONFIG_LIBLINUX_USE_NATIVE_SPINLOCK */
#endif /* __LIBLINUX_ASM_SPINLOCK_H */
