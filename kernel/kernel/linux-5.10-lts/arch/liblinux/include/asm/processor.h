/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef __LIBLINUX_ASM_PROCESSOR_H
#define __LIBLINUX_ASM_PROCESSOR_H

struct mm_struct;

#include <vdso/processor.h>
#include <asm/processor-generic.h>
#include <linux/random.h>
static inline void arch_thread_struct_whitelist(unsigned long *offset,
						unsigned long *size)
{
	*offset = *size = 0;
}

unsigned long get_wchan(struct task_struct *p);

#define task_pt_regs(p) \
	((struct pt_regs *)(THREAD_SIZE + task_stack_page(p)) - 1)

#define TASK_UNMAPPED_BASE      PAGE_ALIGN(TASK_SIZE / 3)

#endif
