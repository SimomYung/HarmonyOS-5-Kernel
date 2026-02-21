// SPDX-License-Identifier: GPL-2.0-only
#include <linux/export.h>
#include <linux/fdtable.h>
#include <linux/fs_struct.h>
#include <linux/init_task.h>
#include <linux/kernel.h>
#include <linux/proc_ns.h>
#include <linux/sched/debug.h>
#include <linux/seq_file.h>
#include <linux/stackprotector.h>
#include <linux/stop_machine.h>
#include <linux/writeback.h>
#include <asm/fixmap.h>
#include <asm/processor.h>
#include <asm/stacktrace.h>
#include <linux/highuid.h>
#include <linux/delay.h>

int overflowuid = DEFAULT_OVERFLOWUID;
int overflowgid = DEFAULT_OVERFLOWGID;
EXPORT_SYMBOL(overflowuid);
EXPORT_SYMBOL(overflowgid);

/* mm */
struct wb_domain global_wb_domain;
const struct vm_operations_struct generic_file_vm_ops;

/* kernel/extable.c */
u32 __initdata __visible __weak main_extable_sort_needed = 1;

/* kernel/fork.c */
struct kmem_cache *fs_cachep;
__cacheline_aligned DEFINE_RWLOCK(tasklist_lock);  /* outer */
void put_task_stack(struct task_struct *tsk)
{
	refcount_dec_and_test(&tsk->stack_refcount);
}

/* kernel/reboot.c */
DEFINE_MUTEX(system_transition_mutex);
struct pid *cad_pid;

/* kernel/hung_task.c */
#ifdef CONFIG_DETECT_HUNG_TASK
unsigned long __read_mostly sysctl_hung_task_timeout_secs = CONFIG_DEFAULT_HUNG_TASK_TIMEOUT;
#endif

/* kernel/process.c */
#if defined(CONFIG_STACKPROTECTOR) && !defined(CONFIG_STACKPROTECTOR_PER_TASK)
unsigned long __weak __stack_chk_guard __ro_after_init;
EXPORT_SYMBOL(__stack_chk_guard);
#endif

/* kernel/alternative.c */
void apply_alternatives_module(void *start, size_t length)
{
}

void mark_constdata_ro(void)
{
    pr_info("liblinux: UN-IMPL: %s\n", __func__);
}

/* arch interrupt support ("arch/{arm64,arm}/kernel/irq.c */
unsigned long irq_err_count;
int arch_show_interrupts(struct seq_file *p, int prec)
{
	seq_printf(p, "%*s: %10lu\n", prec, "Err", irq_err_count);
	return 0;
}

/* define the fowlling gloab data for INIT_TASK and INIT_THREAD_INFO */
struct pid init_struct_pid = {
	.count		= REFCOUNT_INIT(1),
	.tasks		= {
		{ .first = NULL },
		{ .first = NULL },
		{ .first = NULL },
	},
	.level		= 0,
	.numbers	= { {
		.nr		= 0,
		.ns		= &init_pid_ns,
	}, }
};

struct files_struct init_files;
struct nsproxy init_nsproxy = {
#ifdef CONFIG_NET
	.net_ns = &init_net,
#endif
	.uts_ns	= &init_uts_ns,
};
struct mm_struct init_mm;

/* kernel/pid.c */
/*
 * PID-map pages start out as NULL, they get allocated upon
 * first use and are never deallocated. This way a low pid_max
 * value does not cause lots of bitmaps to be allocated, but
 * the scheme scales to up to 4 million PIDs, runtime.
 */
struct pid_namespace init_pid_ns = {
	.kref = KREF_INIT(2),
	.idr = IDR_INIT(init_pid_ns.idr),
	.pid_allocated = PIDNS_ADDING,
	.level = 0,
	.child_reaper = &init_task,
	.user_ns = &init_user_ns,
	.ns.inum = PROC_PID_INIT_INO,
#ifdef CONFIG_PID_NS
	.ns.ops = &pidns_operations,
#endif
};
EXPORT_SYMBOL_GPL(init_pid_ns);

/* used in INIT_TASK */
long do_no_restart_syscall(struct restart_block *param)
{
	return -EINTR;
}

unsigned long get_wchan(struct task_struct *p)
{
	struct stackframe frame;
	unsigned long ret = 0;
	int count = 0;

	/* only support get_wchan in __schedule_timeout before blocking */
	if (p != current || p->state == TASK_RUNNING)
		return 0;

	get_task_struct(p);

	start_backtrace(&frame, (unsigned long)__builtin_frame_address(0),
			(unsigned long)get_wchan);

	do {
		if (unwind_frame(p, &frame))
			break;
		if (!in_sched_functions(frame.pc)) {
			ret = frame.pc;
			break;
		}
	} while (count++ < 16);

	put_task_struct(p);
	return ret;
}

/* a fake stop_machine */
int __weak stop_machine(cpu_stop_fn_t fn, void *data, const struct cpumask *cpus)
{
	unsigned long flags;
	int ret;
	local_irq_save(flags);
	ret = fn(data);
	local_irq_restore(flags);
	return ret;
}
EXPORT_SYMBOL_GPL(stop_machine);

/*
 * cpu topology table
 */
struct cpu_topology cpu_topology[NR_CPUS];
EXPORT_SYMBOL_GPL(cpu_topology);

void __weak arch_time_sync(void) {}

void __override msleep(unsigned int msecs)
{
	unsigned long timeout = 0;

	if (msecs <= 20) {
		/* range is msecs to msecs + 1 */
		usleep_range((msecs * USEC_PER_MSEC), ((msecs + 1) * USEC_PER_MSEC));
		return;
	}

	timeout = msecs_to_jiffies(msecs) + 1;
	while (timeout)
		timeout = schedule_timeout_uninterruptible(timeout);
}

void bust_spinlocks(int yes)
{
	if (yes)
		++oops_in_progress;
	else
		--oops_in_progress;
}
