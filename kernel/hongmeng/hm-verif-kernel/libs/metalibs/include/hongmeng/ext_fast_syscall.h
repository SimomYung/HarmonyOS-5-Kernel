/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Ext fastcall self define utils
 * Author: Huawei OS Kernel Lab
 * Create: Tue Aug 11 19:02:20 2020
 */
#ifndef ULIBS_HONGMENG_EXT_FAST_SYSCALL_H
#define ULIBS_HONGMENG_EXT_FAST_SYSCALL_H

#include <hmasm/kern_syscall.h>
#include <hongmeng/macro.h>

#define __EXT_FASTCALL(x, name, index, cat, scno, ...)	\
extern int __syscall_fast_ext_##cat##_##index(__SC_MAP(x, __SC_DECL, __VA_ARGS__));\
static inline int				\
sysfast_##name(__SC_MAP(x, __SC_DECL, __VA_ARGS__))\
{						\
	return __syscall_fast_ext_##cat##_##index(__SC_MAP(x, __SC_USE, __VA_ARGS__));\
}						\
static inline unsigned int			\
__ext_fastcall_scno_of_##name(void)		\
{						\
	return scno;				\
}

#define __EXT_FASTCALL_GLOBAL(x, name, index, ...)	\
	__EXT_FASTCALL(x, name, index, global, __EXT_FASTCALL_MKCALLNO_GLOBAL(index), __VA_ARGS__)
#define __EXT_FASTCALL_PER_CNODE(x, name, index, ...)	\
	__EXT_FASTCALL(x, name, index, per_cnode, __EXT_FASTCALL_MKCALLNO_PER_CNODE(index), __VA_ARGS__)
#define __EXT_FASTCALL_PER_THREAD(x, name, index, ...)	\
	__EXT_FASTCALL(x, name, index, per_thread, __EXT_FASTCALL_MKCALLNO_PER_THREAD(index), __VA_ARGS__)

#endif
