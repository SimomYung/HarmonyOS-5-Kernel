/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef __LIBLINUX_ASM_SPINLOCK_TYPES_H
#define __LIBLINUX_ASM_SPINLOCK_TYPES_H

#ifdef CONFIG_LIBLINUX_USE_NATIVE_SPINLOCK
#include_next <asm/spinlock_types.h>
#else
#if !defined(__LINUX_SPINLOCK_TYPES_H) && !defined(__ASM_SPINLOCK_H)
# error "please don't include this file directly"
#endif

typedef struct liblinux_pal_mutex_t arch_spinlock_t;

#define __ARCH_SPIN_LOCK_UNLOCKED LIBLINUX_MUTEX_INITIALIZER

typedef struct liblinux_pal_mutex_t arch_rwlock_t;

#define __ARCH_RW_LOCK_UNLOCKED LIBLINUX_MUTEX_INITIALIZER

#endif /* CONFIG_LIBLINUX_USE_NATIVE_SPINLOCK */
#endif /* __LIBLINUX_ASM_SPINLOCK_TYPES_H */
