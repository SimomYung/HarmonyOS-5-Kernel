/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jul 4 10:51:56 2018
 */
#ifndef AARCH64_ASM_STACKUNWIND_H
#define AARCH64_ASM_STACKUNWIND_H

#include <hmkernel/types.h>
#include <hmkernel/compiler.h>
#include <hmkernel/memory.h>
#include <hmasm/registers.h>
#include <mapi/hmasm/stackunwind.h>

struct stack_frame_range_s {
	void *start;
	void *end;
};

extern struct stack_frame_s
arch_stack_unwind(struct stack_frame_s frame);

extern struct stack_frame_s
arch_stack_frame_from(const struct arch_regs *regs);

extern struct stack_frame_s
arch_user_stack_unwind(struct stack_frame_s frame);

extern struct stack_frame_s
arch_actv_stack_unwind(struct stack_frame_s frame, u64 offset, bool priv);

/*
 * low ---------------------------------------------------> high
 * |sp            |fp                      |sp'           |fp'
 * v              v                        v              v
 * [args][dynamic][fp][lr][local vars][...][args][dynamic][fp][lr][......]
 *                        '------------------------------'
 *                         frame range
 * '--------------------------------------'------------------------------'
 *  stack frame                             stack frame
 */
#define ARCH_OFFSET_TO_FP 2

extern void
arch_stack_current_frame_range(struct stack_frame_range_s *fr);

#ifdef CONFIG_FRAME_POINTER
static inline void
arch_stack_frame_range_unwind(struct stack_frame_range_s *fr)
{
	unsigned long *pfp = (unsigned long *)fr->end;
	bool valid_ptr = is_kernel_ptr(pfp);

	fr->start = valid_ptr ? (void *)(pfp + ARCH_OFFSET_TO_FP) : NULL;
	fr->end = valid_ptr ? (void *)((unsigned long *)pfp[0]) : NULL;
}
#else
static inline void
arch_stack_frame_range_unwind(struct stack_frame_range_s *fr)
{
	fr->start = NULL;
	fr->end = NULL;
}
#endif

#define arch_valid_stack_frame_range(fr, stack_bottom, stack_top)	\
	((stack_bottom) < (fr)->start && (fr)->end <= (stack_top))

#define arch_each_stack_frame_range_continue(fr, stack_bottom, stack_top)	\
	for (arch_stack_frame_range_unwind(fr);					\
	     arch_valid_stack_frame_range(fr, stack_bottom, stack_top);		\
	     arch_stack_frame_range_unwind(fr))

#endif
