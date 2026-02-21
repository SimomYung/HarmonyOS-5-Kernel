// SPDX-License-Identifier: GPL-2.0-only
#include <linux/kallsyms.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/sched/debug.h>
#include <linux/sched/task_stack.h>
#include <liblinux/pal.h>

#include <asm/stacktrace.h>

void dump_trace(struct task_struct *tsk, const struct stacktrace_ops *ops, void *data)
{
	struct stackframe frame;
	unsigned long fp;

	if (tsk == current) {
		start_backtrace(&frame,
				(unsigned long)__builtin_frame_address(0),
				(unsigned long)dump_trace);
	} else {
		panic("liblinux: try dump_trace non-current task \"%s\"\n",
		      tsk->comm);
	}

	fp = frame.fp;

	while (1) {
		ops->address(data, frame.pc, 1);
		if (unwind_frame(tsk, &frame)) {
			break;
		}

		if (!(tsk->flags & PF_KTHREAD)) {
			if (frame.fp <= fp) {
				break;
			}
		}
		fp = frame.fp;
	}
}

static void _print_addr(void *data, unsigned long address, int reliable)
{
	const char *loglvl = data;
	if (!reliable)
		return;
	printk("%s [<%08lx>] %s%pS\n", loglvl, address, reliable ? "" : "? ",
		(void *)address);
}

static const struct stacktrace_ops stackops = {
	.address = _print_addr
};

void __override show_stack(struct task_struct *task, unsigned long *stack,
		       const char *loglvl)
{
	struct pt_regs *segv_regs = current->thread.segv_regs;
	int i;

	if (task == NULL) {
		task = current;
	}

	if (!stack) {
		stack = get_stack_pointer(task, segv_regs);
		if (!stack)
			return;
	}

	printk("%sStack:\n", loglvl);
	for (i = 0; i < 3 * STACKSLOTS_PER_LINE; i++) {
		if (kstack_end(stack))
			break;
		if (i && ((i % STACKSLOTS_PER_LINE) == 0))
			pr_cont("\n");
		pr_cont(" %08lx", *stack++);
	}

	printk("%sCall Trace:\n", loglvl);
	dump_trace(task, &stackops, (void *)loglvl);
}

/*
 * The linux version is not compiled for now (kernel/sched/core.o)
 * If it is compiled later, we should add __override attribute here
 */
void sched_show_task(struct task_struct *p)
{
	if ((p->flags & PF_KTHREAD) == 0) {
		pr_err("[%s]: dumping target is not a kthread, tid may be wrong\n", __func__);
	}
	liblinux_pal_thread_dump(p->pid);
}
