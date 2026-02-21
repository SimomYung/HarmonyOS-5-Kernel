/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef __LIBLINUX_ASM_PROCESSOR_GENERIC_H
#define __LIBLINUX_ASM_PROCESSOR_GENERIC_H

struct pt_regs;
struct task_struct;

#include <asm/ptrace.h>

/* TODO: need remove */
#ifndef INIT_ARCH_THREAD
struct arch_thread {
	unsigned long __rsvd;
};
#define INIT_ARCH_THREAD {0}
#endif
#ifndef EMPTY_REGS
#define EMPTY_REGS { .regs = {} }
#endif


struct thread_struct {
	struct pt_regs regs;
	struct pt_regs *segv_regs;	/* just for compile */
	struct arch_thread arch;
#ifdef CONFIG_ARM64
	void *sve_state;
	unsigned int sve_vl;
#endif
	unsigned long long fault_vr_key;
};

#define INIT_THREAD {				\
	.regs		= EMPTY_REGS,		\
	.arch		= INIT_ARCH_THREAD,	\
}

#endif /* __LIBLINUX_ASM_PROCESSOR_GENERIC_H */
