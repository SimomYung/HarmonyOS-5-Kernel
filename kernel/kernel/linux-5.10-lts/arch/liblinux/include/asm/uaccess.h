/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef __LIBLINUX_ASM_UACCESS_H
#define __LIBLINUX_ASM_UACCESS_H

#ifndef __ASSEMBLY__

#include <liblinux/pal.h>

#include <asm/pgtable.h>
#include <asm/current.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/compiler.h>
#include <linux/sched.h>
#include <asm/extable.h>

#define VERIFY_READ	0
#define VERIFY_WRITE	1

#ifndef KERNEL_DS
#define KERNEL_DS	UL(-1)
#endif
#ifndef USER_DS
#define USER_DS		((UL(1) << VA_BITS) - 1)
#endif

#define get_ds()  	(KERNEL_DS)
#define get_fs()  	(task_thread_info(current)->addr_limit)

static inline void set_fs(mm_segment_t fs)
{
	task_thread_info(current)->addr_limit = fs;
}

#ifdef CONFIG_X86
#define segment_eq(a, b) ((a).seg == (b).seg)
#else
#define segment_eq(a, b) ((a) == (b))
#endif

#define user_addr_max()		(-1UL)

#define access_ok(addr, size)	((void)(addr), (void)(size), 1)

#define uaccess_kernel()	(get_fs() == KERNEL_DS)

#define _ASM_EXTABLE(from, to)						\
	"	.pushsection	__ex_table, \"a\"\n"			\
	"	.align		3\n"					\
	"	.long		(" #from " - .), (" #to " - .)\n"	\
	"	.popsection\n"

static inline void uaccess_disable(void) {}
static inline void uaccess_enable(void) {}

#define __put_user(x, ptr)					\
({								\
	__typeof__(*(ptr)) __val = (__typeof__(*(ptr)))(x);	\
	__typeof__(*(ptr)) __user *__p = (ptr);			\
	int __pu_err = 0;					\
	if (get_fs() == USER_DS) {				\
		__pu_err = liblinux_pal_copy_to_user(__p, &__val, sizeof(*__p)) < 0 ? -EFAULT : 0; \
	} else {						\
		*(__p) = x;					\
	}							\
	__pu_err;						\
})

#define __get_user(x, ptr)					\
({								\
	__typeof__(*(ptr)) __user *__p = (ptr);			\
	int __gu_err = 0;					\
	if (get_fs() == USER_DS) {				\
		__gu_err = liblinux_pal_copy_from_user((void *)&(x), __p, sizeof(*__p)) < 0 ? ((x) = 0, -EFAULT) : 0; \
	} else {						\
		x = *(__p);					\
	}							\
	__gu_err;						\
})

#define put_user	__put_user
#define get_user	__get_user

#define INLINE_COPY_TO_USER
#define INLINE_COPY_FROM_USER

extern unsigned long __must_check
__arch_copy_from_user(void *to, const void __user *from, unsigned long n);
extern unsigned long __must_check
__arch_copy_to_user(void __user *to, const void *from, unsigned long n);
extern unsigned long __must_check
__copy_in_user(void __user *to, const void __user *from, unsigned long n);
extern unsigned long __must_check
__clear_user(void __user *to, unsigned long n);

#define raw_copy_from_user(to, from, n)	__arch_copy_from_user(to, from, n)
#define raw_copy_to_user(to, from, n)	__arch_copy_to_user(to, from, n)
#define raw_copy_in_user(to, from, n)	__copy_in_user(to, from, n)
#define clear_user(to, n)		__clear_user(to, n)

extern long strncpy_from_user(char *dest, const char __user *src, long count);

/*
 * Unlike strnlen, strnlen_user includes the nul terminator in
 * its returned count. Callers should check for a returned value
 * greater than N as an indication the string is too long.
 */
extern __must_check long strnlen_user(const char __user *str, long n);

#ifdef CONFIG_ARM64
static inline bool uaccess_ttbr0_disable(void)
{
	return false;
}

static inline bool uaccess_ttbr0_enable(void)
{
	return false;
}
#endif

#endif /* !__ASSEMBLY__ */
#endif /* __LIBLINUX_ASM_UACCESS_H */
