/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LIBLINUX_ASM_CURRENT_H
#define __LIBLINUX_ASM_CURRENT_H

#ifndef CONFIG_THREAD_INFO_IN_TASK
#  include <asm-generic/current.h>
#else
#  define current get_current()
#endif /* !CONFIG_THREAD_INFO_IN_TASK */

#ifndef __ASSEMBLY__
#ifdef CONFIG_THREAD_INFO_IN_TASK
struct task_struct;
extern struct task_struct *liblinux_thread_setup_p(void);
#ifdef CONFIG_LTO
extern __attribute__((const)) unsigned int get_liblinux_thread_offset(void);
extern __attribute__((const)) unsigned int get_liblinux_thread_key(void);
#else
extern const unsigned int liblinux_thread_offset;
extern const unsigned int liblinux_thread_key;
#endif
#ifndef NULL
#define NULL ((void *)0)
#endif

/*
 * This depends on the raw thread layout in ulibs:
 *    struct raw thread
 *        ...
 *        struct pthread
 *            ...
 *            tsd[key]  : struct task_struct
 *              |
 *            offset
 *              |
 *    TLS reg pointer
*/
#define TLS_TO_CURRENT(tls, offset, key) \
	((*(void ***)((tls) - (offset)))[(key)])

#ifdef __aarch64__
static inline unsigned long raw_get_tls(void)
{
	/*
	 * reference to `https://gcc.gnu.org/onlinedocs/gcc/Extended-Asm.html#Volatile-1`.
     * the reason we can remove `volatile` here: `code will always return the same result`.
     * so many `current` read at one function can optimizate as once by compiler.
     * using function attribute `const/pure` with `volatile` has no effect.
	 */
	unsigned long val = 0UL;
	asm ("mrs %0, tpidr_el0\n" : "=r"(val));
	return val;
}

static inline void *raw_get_current(void)
{
	unsigned long tls = raw_get_tls();
#ifdef CONFIG_LTO
    return TLS_TO_CURRENT(tls, get_liblinux_thread_offset(), get_liblinux_thread_key());
#else
    return TLS_TO_CURRENT(tls, liblinux_thread_offset, liblinux_thread_key);
#endif
}
#else
# error "support ARM64 only"
#endif

static __always_inline struct task_struct *get_current(void)
{
	struct task_struct *p = (struct task_struct *)raw_get_current();
	return p;
}
#endif /* CONFIG_THREAD_INFO_IN_TASK */
#endif /* __ASSEMBLY__ */

#endif /* __LIBILNUX_ASM_CURRENT_H */
