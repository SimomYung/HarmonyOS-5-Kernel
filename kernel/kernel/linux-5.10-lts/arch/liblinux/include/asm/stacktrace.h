/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef __LIBLINUX_ASM_STACKTRACE_H
#define __LIBLINUX_ASM_STACKTRACE_H

#include <asm/memory.h>
#include <linux/uaccess.h>
#include <linux/ptrace.h>

#ifdef CONFIG_64BIT
#define STACKSLOTS_PER_LINE 4
#else
#define STACKSLOTS_PER_LINE 8
#endif

struct stack_frame {
	struct stack_frame *next_frame;
	unsigned long return_address;
};

struct stacktrace_ops {
	void (*address)(void *data, unsigned long address, int reliable);
};

#ifdef CONFIG_FRAME_POINTER
static inline unsigned long
get_frame_pointer(struct task_struct *task, struct pt_regs *regs)
{
	if (task == current)
		return (unsigned long)__builtin_frame_address(0);
	WARN(1, "liblinux: try get frame pointer of non-current task \"%s\"\n",
	      task->comm);
	return 0;
}
#else
static inline unsigned long
get_frame_pointer(struct task_struct *task, struct pt_regs *segv_regs)
{
	return 0;
}
#endif

static inline unsigned long
*get_stack_pointer(struct task_struct *task, struct pt_regs *regs)
{
	if (task == current)
		return __builtin_frame_address(0);
	WARN(1, "liblinux: try get stack pointer of non-current task \"%s\"\n",
	      task->comm);
	return NULL;
}

void dump_trace(struct task_struct *tsk, const struct stacktrace_ops *ops, void *data);

/* for arm64 stack without silk */
enum stack_type {
	STACK_TYPE_UNKNOWN,
	STACK_TYPE_TASK,
	STACK_TYPE_IRQ,
	__NR_STACK_TYPES
};

struct stack_info {
	unsigned long low;
	unsigned long high;
	enum stack_type type;
};

struct stackframe {
	unsigned long fp;
	unsigned long pc;
	DECLARE_BITMAP(stacks_done, __NR_STACK_TYPES);
	unsigned long prev_fp;
	enum stack_type prev_type;
#ifdef CONFIG_FUNCTION_GRAPH_TRACER
	int graph;
#endif
};

extern int unwind_frame(struct task_struct *tsk, struct stackframe *frame);
extern void walk_stackframe(struct task_struct *tsk, struct stackframe *frame,
			    bool (*fn)(void *, unsigned long), void *data);
extern void dump_backtrace(struct pt_regs *regs, struct task_struct *tsk,
			   const char *loglvl);

static inline bool on_accessible_stack(const struct task_struct *tsk,
				       unsigned long sp,
				       struct stack_info *info)
{
#ifdef CONFIG_ARM64
	unsigned long stack_end = read_sysreg(tpidr_el0);

	/*
	 * the execution of ldk could be found in both thread context and actv context,
	 * the frames of thread could be tested with NULL fp while actv could not.
	 * the actv should be checked with stack_end, see also the function of
	 * dump_user_callstack() in sysmgr.
	 */
	if (!(tsk->flags & PF_KTHREAD)) {
		if (sp >= stack_end) {
			return false;
		}
	}
#endif

	return true;
}

static inline void start_backtrace(struct stackframe *frame,
				   unsigned long fp, unsigned long pc)
{
	frame->fp = fp;
	frame->pc = pc;
#ifdef CONFIG_FUNCTION_GRAPH_TRACER
	frame->graph = 0;
#endif

	/*
	 * Prime the first unwind.
	 *
	 * In unwind_frame() we'll check that the FP points to a valid stack,
	 * which can't be STACK_TYPE_UNKNOWN, and the first unwind will be
	 * treated as a transition to whichever stack that happens to be. The
	 * prev_fp value won't be used, but we set it to 0 such that it is
	 * definitely not an accessible stack address.
	 */
	bitmap_zero(frame->stacks_done, __NR_STACK_TYPES);
	frame->prev_fp = 0;
	frame->prev_type = STACK_TYPE_UNKNOWN;
}

#endif /* __LIBLINUX_ASM_STACKTRACE_H */
