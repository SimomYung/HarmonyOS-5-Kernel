/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef __LIBLINUX_ASM_MMU_CONTEXT_H
#define __LIBLINUX_ASM_MMU_CONTEXT_H

#include_next <asm-generic/mmu_context.h>

static inline const struct cpumask *
task_cpu_possible_mask(struct task_struct *p)
{
	return cpu_possible_mask;
}

#define task_cpu_possible_mask task_cpu_possible_mask

#endif /* __LIBLINUX_ASM_MMU_CONTEXT_H */
