/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: API of Fast Syscall
 * Author: Huawei OS Kernel Lab
 * Create: Thu Sep 13 03:39:49 2018 -0700
 */
#ifndef ULIBS_HONGMENG_FAST_SYSCALL_H
#define ULIBS_HONGMENG_FAST_SYSCALL_H

#include <hmasm/kern_syscall.h>
#include <hongmeng/macro.h>
#include <hmkernel/ipc/hmsignal.h>
#include <hmkernel/futex.h>

#define __SYSCALL_FAST_DEFINE0(name)	\
extern int __syscall_fast_##name(void); \
static inline int				\
sysfast_##name(void) \
{						\
	return __syscall_fast_##name();\
}

#define __SYSCALL_FAST_DEFINEx(x, name, ...)	\
extern int __syscall_fast_##name(__SC_MAP(x, __SC_DECL, __VA_ARGS__)); \
static inline int				\
sysfast_##name(__SC_MAP(x, __SC_DECL, __VA_ARGS__)) \
{						\
	return __syscall_fast_##name(__SC_MAP(x, __SC_USE, __VA_ARGS__));\
}

#define __SYSCALL_FAST_DEFINE1(name, ...)	__SYSCALL_FAST_DEFINEx(1, name, __VA_ARGS__)
#define __SYSCALL_FAST_DEFINE2(name, ...)	__SYSCALL_FAST_DEFINEx(2, name, __VA_ARGS__)
#define __SYSCALL_FAST_DEFINE3(name, ...)	__SYSCALL_FAST_DEFINEx(3, name, __VA_ARGS__)
#define __SYSCALL_FAST_DEFINE4(name, ...)	__SYSCALL_FAST_DEFINEx(4, name, __VA_ARGS__)
#define __SYSCALL_FAST_DEFINE5(name, ...)	__SYSCALL_FAST_DEFINEx(5, name, __VA_ARGS__)
#define __SYSCALL_FAST_DEFINE6(name, ...)	__SYSCALL_FAST_DEFINEx(6, name, __VA_ARGS__)
#define __SYSCALL_FAST_DEFINE7(name, ...)	__SYSCALL_FAST_DEFINEx(7, name, __VA_ARGS__)
#define __SYSCALL_FAST_DEFINE8(name, ...)	__SYSCALL_FAST_DEFINEx(8, name, __VA_ARGS__)
#define __SYSCALL_FAST_DEFINE9(name, ...)	__SYSCALL_FAST_DEFINEx(9, name, __VA_ARGS__)
#define __SYSCALL_FAST_DEFINE10(name, ...)	__SYSCALL_FAST_DEFINEx(10, name, __VA_ARGS__)

#include <hongmeng/fast_syscall_list.h>
#include <hongmeng/ctrlmem_syscall.h>

#endif
