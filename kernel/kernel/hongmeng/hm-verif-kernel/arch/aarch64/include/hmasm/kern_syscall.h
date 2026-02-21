/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Mon Dec 3 03:04:57 2018
 */

/* Note that this file can be included multiple times to generate fastcall
 * enums. Use following way for it:
 *
 * #include <hmasm/kern_syscall.h>
 * #undef UAPI_ARCH_ASM_KERN_SYSCALL_H
 *
 * #ifdef ARCH_ASM_KERN_SYSCALL_H
 * #undef ARCH_ASM_KERN_SYSCALL_H
 * #endif
 *
 * #undef __FAST_SYSCALL
 *
 * ...
 * #define __FAST_SYSCALL(a, b) xxx
 * #include <hmasm/kern_syscall.h>
 * #undef __FAST_SYSCALL
 */

/* Avoid change this guard */
#ifndef ARCH_ASM_KERN_SYSCALL_H
#define ARCH_ASM_KERN_SYSCALL_H

#include <uapi/hmasm/kern_syscall.h>

#include <hmkernel/bitops/bitops.h>

#ifndef SYSCALL_SCNO_CAP_OF
# define SYSCALL_SCNO_CAP_OF(scno)	MASK((scno), 5u)
#endif

#ifndef SYSCALL_SCNO_METHOD_OF
# define SYSCALL_SCNO_METHOD_OF(scno)	MASK(((scno) >> 5u), 9u)
#endif

#ifndef SYSCALL_SCNO_IS_32BIT
# define SYSCALL_SCNO_IS_32BIT(scno)	MASK(((scno) >> 14u), 1u)
#endif

#ifndef SYSCALL_SCNO_IS_PROXY
# define SYSCALL_SCNO_IS_PROXY(scno)	MASK(((scno) >> 15u), 1u)
#endif

#endif
