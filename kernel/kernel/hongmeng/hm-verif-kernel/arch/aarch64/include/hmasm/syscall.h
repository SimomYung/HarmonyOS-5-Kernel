/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Sat Jul 28 01:28:58 2018
 */
#ifndef A64_ASM_SYSCALL_H
#define A64_ASM_SYSCALL_H

#include <hmkernel/compiler.h>

#include <hmasm/ucontext.h>
#include <hmasm/cpu_local.h>
#include <hmasm/kern_syscall.h>
#undef ARCH_ASM_KERN_SYSCALL_H
#undef UAPI_ARCH_ASM_KERN_SYSCALL_H
#undef __FAST_SYSCALL

#define __FAST_SYSCALL(name, number)	__FAST_SYSCALL_##name = number,
enum {
#include <hmasm/kern_syscall.h>
	__FAST_SYSCALL_MAX,
};
#undef __FAST_SYSCALL

struct tcb_s;

extern void __noreturn
arch_syscall_entry(struct arch_uctx *uctx, unsigned int sysno,
		   struct arch_cpu_local_s *cpu_local);

/*
 * Implemented in entery.S
 * Note that the correct way to restart a syscall is:
 * vspace_switch()  <-- switch vspace back to previous tcb (get tcb from cpu_local if needed)
 * unlock_kernel()
 * arch_restart_syscall()
 */
extern void __noreturn
arch_restart_syscall(const struct arch_uctx *uctx,
		     struct arch_cpu_local_s *cpu_local);

extern void __noreturn
arch_tailcall_entry(const struct arch_uctx *uctx,
		    const struct arch_cpu_local_s *cpu_local,
		    struct tcb_s *caller_tcb);
void arch_syscall_restart(struct tcb_s *caller_tcb);

#ifdef __HOST_LLT__
void __noinline llt_el0_svc(struct arch_uctx *uctx, struct arch_cpu_local_s *cpu_local, unsigned int sysno);
#endif
#endif
