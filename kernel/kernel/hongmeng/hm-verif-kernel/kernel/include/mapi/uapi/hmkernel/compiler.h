/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2021. All rights reserved.
 * Description: Compiler
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jun 7 10:56:55 2018
 */
#ifndef UAPI_HMKERNEL_COMPILER_H
#define UAPI_HMKERNEL_COMPILER_H

#include <hmasm/compiler.h>
#include <hmkernel/attributes/hmattributes.h>
#include <hmkernel/types.h>

/*
 * For __HOST_LLT__: x86 compiler provide __always_inline in
 * /usr/include/x86_64-linux-gnu/sys/cdefs.h
 */
#ifdef __always_inline
# undef __always_inline
#endif

#define __same_type(t1, t2)	(__builtin_types_compatible_p(t1, t2) == 1)
#define __is_signed(t)		(__same_type(t, int) || __same_type(t, long) || \
				 __same_type(t, long long))
#define __is_fixed64(t)		(__same_type(t, unsigned long long) || \
				 __same_type(t, long long))

/* Use __ prefix to avoid polluting namespace */
#define __section(s)	__attribute__ ((__section__(#s)))
#define __used		__attribute__((used))
#define __maybe_unused	__attribute__((unused))
#define __visible	__attribute__((externally_visible))
#define __printf(a, b)	__attribute__((format(printf, a, b)))
#define __noreturn	__attribute__((noreturn))
#define __aligned(a)	__attribute__((aligned(a)))
#define __noinline	__attribute__((noinline))
#define __alias(name)	__attribute__((alias(#name)))
#define __pure		__attribute__((pure))
#define __deprecated    __attribute__((deprecated))
#define __weak		__attribute__((weak))

#ifndef __clang__
#define __error(s)	__attribute__((error(s)))
#endif

/* for LLT: /usr/include/x86_64-linux-gnu/sys/cdefs.h gives another __always_inline */
#ifndef __always_inline
# define __always_inline	__attribute__((always_inline))
#endif

#define barrier()	asm volatile ("" ::: "memory")

#ifdef __clang__
#define __no_stack_protector	__attribute__((no_stack_protector))
#else
#define __no_stack_protector	__attribute__((optimize("-fno-stack-protector")))
#endif

/*
 * The macro `__bool_to_long` and `unlikely` are very carefully written, in order to
 * follow the MISRA-C rules while keeping its binary-level semantics correct.
 *
 * MISRA-C forbids conversion from bool to other integer types. Meanwhile, the
 * builtin function `__builtin_expect` uses `long` as its argument types. Hence
 * we need to find a way coverting a bool value to a long number without using
 * force type case expressions, e.g. `((long)(x))`. Some converting pattern, e.g.
 * `((x) ? 1L : 0L)`, according to our experiments, can trigger a bug in gcc which
 * reverts the branch prediction result. This is the only implementation we found
 * that is both semantically correct and free from MISRA-C warnings.
 *
 * BE CAREFUL WHEN MODIFYING THEM !
 */
#define __bool_to_long(x) (((x) ? __false : __true) ? 0L : 1L)
#define __long_to_bool(x) (((x) == 0L) ? __false : __true)
#define __likely(x)	__long_to_bool(__builtin_expect(__bool_to_long(x), 1L))
#define __unlikely(x)	__long_to_bool(__builtin_expect(__bool_to_long(x), 0L))

/* The cold functions will be optimized for size but not performance. */
#define __cold		__attribute__((__cold__))
/* The hot functions will be optimized more aggressively and code locality is improved. */
#define __hot		__attribute__((__hot__))
#define __const		__attribute__((__const__))

/* Split read mostly data from others to reduce muti-core synchronization overhead. */
#define __read_mostly	__section(.data.read_mostly)

#ifdef CONFIG_KERNEL_TEXT_SECTION
#define __cap_text __section(.capability.text)
#else
#define __cap_text
#endif

/* Data which will be updated after init */
#define __rw_after_init	__section(.data.rw_after_init)
#ifndef __HOST_UT__
#define __rw_after_init_bss	__section(.rw_after_init_bss)
#else
#define __rw_after_init_bss
#endif

/* Data which will not be updated after init */
#ifndef __HOST_UT__
#define __ro_after_init	__section(.ro_after_init)
#define __ro_after_init_hdlr  __section(.ro_after_init_hdlr)
#else
#define __ro_after_init
#define __ro_after_init_hdlr
#endif
#if !defined(__HOST_UT__) && defined(CONFIG_VFS_FUSION)
#define __ro_after_init_fs	__section(.ro_after_init_fs)
#define __ro_after_init_hdlr_fs __section(.ro_after_init_hdlr_fs)
#else
#define __ro_after_init_fs
#define __ro_after_init_hdlr_fs
#endif

/* The init/exit functions are splitted from normal code, and marked as cold. */
#define __init		__section(.init.text) __cold
#define __exit		__section(.exit.text) __cold
#define __initdata	__section(.init.data)
#define __exitdata	__section(.exit.data)

#define __read_once(v)		(*((volatile typeof(v) *) (&(v))))
#define __write_once(x, v)	(*(volatile typeof(x) *) (&(x))) = (typeof(x))(v)

#define KCFI_TAG 0x35c00

#define CERTAIN_NOFAIL(s) do {                 \
	int ___err;                             \
	___err = (s);                           \
	(void)(___err);                         \
	__rawattr_verifier_assert(___err != 0); \
} while (false)

/*
 * Force a compilation error if condition is true, or return 0 (type int)
 */
#define BUILD_BUG_ON_OR_RETURN_ZERO(condition)				\
	((int)(sizeof(char[0 - ((!!(condition)) ? 1 : 0)])))

#define __must_be_const_string(cstr)					\
	BUILD_BUG_ON_OR_RETURN_ZERO(!(__builtin_constant_p(cstr) &&	\
				      __same_type(typeof(cstr), const char[])))

#define CONSTANT_STRLEN(a)      (sizeof(a) - 1 + __must_be_const_string(a))

#define ___pac_ptr __attribute__((pac_protected_ptr))
#define ___pac_data __attribute__((pac_protected_data))
#define __sec_data(component, semantic) ___sec_##component##_##semantic

#ifndef __clang__
#define static_assert_by_typedef(condition, name)  \
        typedef void (*__STATIC_ASSERT_##name)(int __STATIC_ASSERT_COND_##name[1 - 2 * ((condition) ? 0 : 1)]) \
        __attribute__((used))
#else
#define static_assert_by_typedef(condition, name)  \
        typedef void (*__STATIC_ASSERT_##name)(int __STATIC_ASSERT_COND_##name[1 - 2 * ((condition) ? 0 : 1)])
#endif

#ifdef __clang__
#if __has_attribute(fallthrough)
#define __fallthrough __attribute__((fallthrough))
#else /* __has_attribute(fallthrough) */
#define __fallthrough
#endif
#else /* __clang__ */
#define __fallthrough __attribute__((fallthrough))
#endif

#endif
