/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: API of Capability Syscall
 * Author: Huawei OS Kernel Lab
 * Create: Thu Sep 13 03:39:49 2018 -0700
 */
#ifndef ULIBS_HONGMENG_CAP_SYSCALL_H
#define ULIBS_HONGMENG_CAP_SYSCALL_H

#include <hmkernel/ipc/signal.h>
#include <hmkernel/ipc/hmsignal.h>
#include <hmkernel/mm/page.h>
#include <hmkernel/mm/kexec.h>
#include <hmkernel/sysconf.h>
#include <hmkernel/vtimekeep.h>
#include <hmkernel/capability.h>
#include <hmkernel/vnotify_common.h>
#include <hmkernel/kload.h>
#include <hongmeng/macro.h>
#include <lib/hmpsf/hmpsf_def.h>
#include <lib/hmprobe/utils.h>
#include <hmkernel/drivers/hibernate_def.h>

#define __SYSCALL_CAP_DEFINEx(x, cap, method, ...)	\
extern int __syscall_cap_##cap##method(cref_t target_cref, __SC_MAP(x, __SC_DECL, __VA_ARGS__)); \
extern int __sysproxycall_cap_##cap##method(rref_t cnode_rref, cref_t target_cref, \
					    __SC_MAP(x, __SC_DECL, __VA_ARGS__)); \
static inline int				\
syscap_##cap##method(cref_t target_cref, __SC_MAP(x, __SC_DECL, __VA_ARGS__))	\
{						\
	return __syscall_cap_##cap##method(target_cref, __SC_MAP(x, __SC_USE, __VA_ARGS__)); \
} \
static inline int				\
sysproxycap_##cap##method(rref_t cnode_rref, cref_t target_cref,	\
			  __SC_MAP(x, __SC_DECL, __VA_ARGS__))		\
{						\
	return __sysproxycall_cap_##cap##method(cnode_rref,		\
			target_cref, __SC_MAP(x, __SC_USE, __VA_ARGS__)); \
}

#define __SYSCALL_CAP_DEFINE0(cap, method)	\
extern int __syscall_cap_##cap##method(cref_t target_cref); \
extern int __sysproxycall_cap_##cap##method(rref_t cnode_rref, cref_t target_cref); \
static inline int				\
syscap_##cap##method(cref_t target_cref)		\
{						\
	return __syscall_cap_##cap##method(target_cref); \
} \
static inline int				\
sysproxycap_##cap##method(rref_t cnode_rref, cref_t target_cref)		\
{						\
	return __sysproxycall_cap_##cap##method(cnode_rref, target_cref); \
}

#define __SYSCALL_CAP_DEFINE1(cap, method, ...)	__SYSCALL_CAP_DEFINEx(1, cap, method, __VA_ARGS__)
#define __SYSCALL_CAP_DEFINE2(cap, method, ...)	__SYSCALL_CAP_DEFINEx(2, cap, method, __VA_ARGS__)
#define __SYSCALL_CAP_DEFINE3(cap, method, ...)	__SYSCALL_CAP_DEFINEx(3, cap, method, __VA_ARGS__)
#define __SYSCALL_CAP_DEFINE4(cap, method, ...)	__SYSCALL_CAP_DEFINEx(4, cap, method, __VA_ARGS__)
#define __SYSCALL_CAP_DEFINE5(cap, method, ...)	__SYSCALL_CAP_DEFINEx(5, cap, method, __VA_ARGS__)
#define __SYSCALL_CAP_DEFINE6(cap, method, ...)	__SYSCALL_CAP_DEFINEx(6, cap, method, __VA_ARGS__)
#define __SYSCALL_CAP_DEFINE7(cap, method, ...)	__SYSCALL_CAP_DEFINEx(7, cap, method, __VA_ARGS__)

#include <hongmeng/cap_syscall_list.h>

#endif
