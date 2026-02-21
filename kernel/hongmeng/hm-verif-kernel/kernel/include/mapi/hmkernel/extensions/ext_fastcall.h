/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Ext fastcall header for modules
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jun 22 22:09:24 2021
 */
#ifndef MAPI_HMKERNEL_EXT_FASTCALL_H
#define MAPI_HMKERNEL_EXT_FASTCALL_H

#include <hmkernel/syscall.h>

enum ext_fastcall_type {
	EXT_FASTCALL_TYPE_UNDEFINED = 0,
	EXT_FASTCALL_TYPE_GLOBAL,
	EXT_FASTCALL_TYPE_PER_CNODE,
	EXT_FASTCALL_TYPE_PER_THREAD,
};

struct ext_fastcall_desc {
	unsigned int scno;
	enum ext_fastcall_type type;
	struct tcb_s *(*syscall_op)(struct tcb_s *caller);
};

int ext_fastcall_register(const struct ext_fastcall_desc *desc, unsigned int ext_fastcall_nr);

#define EXT_FAST_SYSCALL_DEFINEx(x, name, caller, ...)	\
static inline struct tcb_s *				\
sys_##name(struct tcb_s *caller, __MAP(x, __SC_DEF, __VA_ARGS__));\
static inline struct tcb_s *				\
name(struct tcb_s *caller)				\
{							\
	int __err = 0;					\
	struct tcb_s *__ret = NULL;			\
	struct arch_uctx *uctx = tcb_curr_uctx_of(caller);\
	const bool is_fastpath = false;			\
	struct arch_cpu_local_s *cpu_local = NULL;	\
							\
	__FAST_SYSCALL_ARGSx(x, , __err, __VA_ARGS__);	\
							\
	if (__err != 0) {				\
		__ret = ERR_TO_TCB(__err);		\
	} else {					\
		__ret = sys_##name(caller, __MAP(x, __SC_CALL, __VA_ARGS__));\
	}						\
	return __ret;					\
}							\
static inline struct tcb_s *				\
sys_##name(struct tcb_s *caller, __MAP(x, __SC_DEF, __VA_ARGS__))

#define EXT_FAST_SYSCALL_DEFINE7(name, caller, ...) EXT_FAST_SYSCALL_DEFINEx(7, name, caller, __VA_ARGS__)
#define EXT_FAST_SYSCALL_DEFINE6(name, caller, ...) EXT_FAST_SYSCALL_DEFINEx(6, name, caller, __VA_ARGS__)
#define EXT_FAST_SYSCALL_DEFINE5(name, caller, ...) EXT_FAST_SYSCALL_DEFINEx(5, name, caller, __VA_ARGS__)
#define EXT_FAST_SYSCALL_DEFINE4(name, caller, ...) EXT_FAST_SYSCALL_DEFINEx(4, name, caller, __VA_ARGS__)
#define EXT_FAST_SYSCALL_DEFINE3(name, caller, ...) EXT_FAST_SYSCALL_DEFINEx(3, name, caller, __VA_ARGS__)
#define EXT_FAST_SYSCALL_DEFINE2(name, caller, ...) EXT_FAST_SYSCALL_DEFINEx(2, name, caller, __VA_ARGS__)
#define EXT_FAST_SYSCALL_DEFINE1(name, caller, ...) EXT_FAST_SYSCALL_DEFINEx(1, name, caller, __VA_ARGS__)
#define EXT_FAST_SYSCALL_DEFINE0(name, caller)		\
static inline struct tcb_s *sys_##name(struct tcb_s *caller);\
static inline struct tcb_s *				\
name(struct tcb_s *caller)				\
{							\
	return sys_##name(caller);			\
}							\
static inline struct tcb_s *sys_##name(struct tcb_s *caller)

#endif
