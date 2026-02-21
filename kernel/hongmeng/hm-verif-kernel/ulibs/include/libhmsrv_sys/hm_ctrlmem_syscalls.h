/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: ctrlmem syscall wrapper
 * Author: Huawei OS Kernel Lab
 * Create: Sat May 06 18:24:51 2023
 */

#ifndef ULIBS_INCLUDE_LIBHMSRV_SYS_CTRLMEM_SYSCALLS_SYSCALLS_H
#define ULIBS_INCLUDE_LIBHMSRV_SYS_CTRLMEM_SYSCALLS_SYSCALLS_H

#include <hongmeng/syscall.h>

#include <libhmsrv_sys/hm_ctrlmem_syscall_proxy.h>
#include <hmasm/pgtbl/pgtblhdlr.h>

/*
 * Provide ctrlmem syscall wrapper.
 * Usage:
 *
 * static inline sysctrlmem_xxx(void *ptr, u64 val, void *arg1, |
 *                              unsigned long arg2, __u64 arg3) |
 * {                                                            |
 *     return sysctrlmem_v64(ptr, __sysctrlmem_val_u64(val),    |
 *                                __sysctrlmem_arg_ptr(arg1),   |
 *                                __sysctrlmem_arg_ulong(arg2), |
 *                                __sysctrlmem_arg_u64(arg3));  |
 * }                                                            |
 */
#ifndef __ctrlmem_syscall_override__
# if __SIZEOF_LONG__ == 4
#  ifndef BITS_PER_LONG
#   define BITS_PER_LONG	(__CHAR_BIT__ * __SIZEOF_LONG__)
#  endif

/*
 * When __SIZEOF_LONG__ == 4, register wide is expected to be 32.
 * all 64 bits long arguments should be splitted into 2 parts, and
 * argument number is different.
 */

#  define __sysctrlmem_arg_u64(x)	(unsigned long)((x) & 0xfffffffful), \
					(unsigned long)((x) >> BITS_PER_LONG) & 0xfffffffful
#  define __sysctrlmem_val_u64(v)	(unsigned long)((v) & 0xfffffffful), \
					(unsigned long)((v) >> BITS_PER_LONG) & 0xfffffffful
#  define __sysctrlmem_fn(v, n)		sysproxy_ctrlmem_write_r32_v##v##_arg##n
#  define __sysctrlmem_cas_fn(v, n)	sysproxy_ctrlmem_cas_r32_v##v##_arg##n
# elif __SIZEOF_LONG__ == 8
/*
 * When __SIZEOF_LONG__ == 8, register wide is expected to be 64.
 * all 64 bits long arguments should be delivered through one register.
 */
#  define __sysctrlmem_arg_u64(x)	(unsigned long)(x)
#  define __sysctrlmem_val_u64(v)	(unsigned long)(v)
#  define __sysctrlmem_fn(v, n)		sysproxy_ctrlmem_write_r64_v##v##_arg##n
#  define __sysctrlmem_cas_fn(v, n)	sysproxy_ctrlmem_cas_r64_v##v##_arg##n
# else
#  error SIZEOF_LONG is neither 4 nor 8
# endif

# define __sysctrlmem_arg_u32(x)	(unsigned long)(x)
# define __sysctrlmem_arg_ulong(x)	(unsigned long)(x)
# define __sysctrlmem_arg_ptr(x)	(unsigned long)(x)
# define __sysctrlmem_arg_paddr(x)	__sysctrlmem_arg_u64(x)
# define __sysctrlmem_val_u32(v)	(unsigned long)(v)
#endif

/*
 * if define __ctrlmem_syscall_override__
 * need to inject __sysctrlmem_arg_xxx from outside to use sysctrlmem
 * check if it's all in
 */
#ifndef __sysctrlmem_arg_u32
# error "Should define __sysctrlmem_arg_u32"
#endif

#ifndef __sysctrlmem_arg_ulong
# error "Should define __sysctrlmem_arg_ulong"
#endif

#ifndef __sysctrlmem_arg_ptr
# error "Should define __sysctrlmem_arg_ptr"
#endif

#ifndef __sysctrlmem_arg_paddr
# error "Should define __sysctrlmem_arg_paddr"
#endif

#ifndef __sysctrlmem_val_u32
# error "Should define __sysctrlmem_val_u32"
#endif

/*
 * Generate function name, combine function name base, wide of v and number of arguments.
 */
#define __ctrlmem_syscall_nr_vars(ignored, a, b, c, d, e, f, n, ...) n
#define ctrlmem_syscall_func(v, ...)  __ctrlmem_syscall_nr_vars(ignored, ##__VA_ARGS__, \
								__sysctrlmem_fn(v, 6), __sysctrlmem_fn(v, 5), \
								__sysctrlmem_fn(v, 4), __sysctrlmem_fn(v, 3), \
								__sysctrlmem_fn(v, 2), __sysctrlmem_fn(v, 1), \
								__sysctrlmem_fn(v, 0))

#define __sysctrlmem_v64(callno, ptr, __sysctrlmem_val, ...)	ctrlmem_syscall_func(64, ##__VA_ARGS__)(callno, ptr, \
									     __sysctrlmem_val_u64(__sysctrlmem_val), \
									     ##__VA_ARGS__)
#define __sysctrlmem_v32(callno, ptr, __sysctrlmem_val, ...)	ctrlmem_syscall_func(32, ##__VA_ARGS__)(callno, ptr, \
									     __sysctrlmem_val_u32(__sysctrlmem_val), \
									     ##__VA_ARGS__)

#define __ctrlmem_cas_syscall_nr_vars(ignored, a, b, c, d, e, f, n, ...) n
#define ctrlmem_cas_syscall_func(v, ...)  __ctrlmem_cas_syscall_nr_vars(ignored, ##__VA_ARGS__, \
								__sysctrlmem_cas_fn(v, 6), __sysctrlmem_cas_fn(v, 5), \
								__sysctrlmem_cas_fn(v, 4), __sysctrlmem_cas_fn(v, 3), \
								__sysctrlmem_cas_fn(v, 2), __sysctrlmem_cas_fn(v, 1), \
								__sysctrlmem_cas_fn(v, 0))
#define __sysctrlmem_cas_v64(callno, ptr, val, orig_val, ...) \
			ctrlmem_cas_syscall_func(64, ##__VA_ARGS__)(callno, ptr, \
								    __sysctrlmem_val_u64(val), \
								    __sysctrlmem_val_u64(orig_val), \
								    ##__VA_ARGS__)
#define __sysctrlmem_cas_v32(callno, ptr, val, orig_val, ...) \
			ctrlmem_cas_syscall_func(32, ##__VA_ARGS__)(callno, ptr, \
								    __sysctrlmem_val_u32(val), \
								    __sysctrlmem_val_u32(orig_val), \
								    ##__VA_ARGS__)
/*
 * Here use __sysctrlmem_val..., not '__sysctrlmem_val, ##__VA_ARGS__', to make sure __VA_ARGS__ are
 * macro expanded. ##__VA_ARGS__ is a gcc extension, which is pasted __sysctrlmem_argument
 * and doesn't expand.
 */
#define sysctrlmem_v64(callno, ptr, __sysctrlmem_val...)		__sysctrlmem_v64(callno, ptr, __sysctrlmem_val)
#define sysctrlmem_v32(callno, ptr, __sysctrlmem_val...)		__sysctrlmem_v32(callno, ptr, __sysctrlmem_val)
#define sysctrlmem_cas_v64(callno, ptr, __sysctrlmem_val, __sysctrlmem_orig_val...) \
				__sysctrlmem_cas_v64(callno, ptr, __sysctrlmem_val, __sysctrlmem_orig_val)
#define sysctrlmem_cas_v32(callno, ptr, __sysctrlmem_val, __sysctrlmem_orig_val...) \
				__sysctrlmem_cas_v32(callno, ptr, __sysctrlmem_val, __sysctrlmem_orig_val)

#if __SIZEOF_LONG__ == 8
# define sysctrlmem_vulong		sysctrlmem_v64
# define __sysctrlmem_val_ulong		__sysctrlmem_val_u64
# define sysctrlmem_cas_vulong		sysctrlmem_cas_v64

# define sysctrlmem_pgtbl		sysctrlmem_v64
# define __sysctrlmem_pgtbl_val		__sysctrlmem_val_u64
# define sysctrlmem_pgtbl_cas		sysctrlmem_cas_v64
#else
# define sysctrlmem_vulong		sysctrlmem_v32
# define __sysctrlmem_val_ulong		__sysctrlmem_val_u32
# define sysctrlmem_cas_vulong		sysctrlmem_cas_v32
#ifdef __HMPGTBL_ARM_LPAE__
# define sysctrlmem_pgtbl		sysctrlmem_v64
# define __sysctrlmem_pgtbl_val		__sysctrlmem_val_u64
# define sysctrlmem_pgtbl_cas		sysctrlmem_cas_v64
#else
# define sysctrlmem_pgtbl		sysctrlmem_v32
# define __sysctrlmem_pgtbl_val		__sysctrlmem_val_u32
# define sysctrlmem_pgtbl_cas		sysctrlmem_cas_v32
#endif
#endif

#endif
