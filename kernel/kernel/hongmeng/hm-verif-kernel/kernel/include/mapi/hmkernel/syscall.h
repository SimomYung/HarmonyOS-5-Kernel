/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Descriptiono: Syscall for modules
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jun 23 09:46:44 2021
 */
#ifndef MAPI_HMKERNEL_SYSCALL_H
#define MAPI_HMKERNEL_SYSCALL_H

#include <hmkernel/macro.h>

#include <hmasm/sysarg.h>
#include <hmasm/kern_syscall.h>

#define __SYSARG_CALL(...)				\
arch_sysarg_fast(__VA_ARGS__)

#define __SYSARG_CALL_lp64(a, b, c, d, ...)		\
aarch64_sysarg_fast(a, b, c->regs.__regs[d])

#define __SYSARG_CALL_compat(...)			\
aarch32_sysarg_fast(__VA_ARGS__)

#define __SYSARG_CALL_ilp32(a, b, c, d, e, f, ...)	\
ilp32_sysarg_fast(a, b, c->regs.__regs[d], e, f)

#define __SC_FETCH_OP_GEN(n, t, a, suffix) ({			\
	int ___err;						\
	___err = __SYSARG_CALL##suffix((void *)(&a), sizeof(t),	\
				       uctx, (unsigned long)(n),\
				       __is_signed(t),		\
				       __is_fixed64(t),		\
				       is_fastpath, cpu_local);	\
	___err;							\
})

/* used with __MAPSTAT_OP as an expression */
#define __SC_FETCH_OP(n, t, a)		\
__SC_FETCH_OP_GEN(n, t, a, )

#define __SC_FETCH_OP_lp64(n, t, a)	\
__SC_FETCH_OP_GEN(n, t, a, _lp64)

#define __SC_FETCH_OP_compat(n, t, a)	\
__SC_FETCH_OP_GEN(n, t, a, _compat)

#define __SC_FETCH_OP_ilp32(n, t, a)	\
__SC_FETCH_OP_GEN(n, t, a, _ilp32)

#define __FAST_SYSCALL_ARGSx(x, suffix, e, ...)		\
	__MAPSTAT_DEFVAR(x, __SC_DEF_VAR, __VA_ARGS__);	\
	__MAPSTAT_OP(x, __SC_FETCH_OP##suffix, e, __VA_ARGS__);

struct arch_cpu_local_s;
JAM_INLINE void set_need_postproc(struct arch_cpu_local_s *cpu_local);

#endif
