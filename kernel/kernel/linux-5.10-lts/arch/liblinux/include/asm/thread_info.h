/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Based on arch/arm/include/asm/thread_info.h
 */
#ifndef __LIBLINUX_ASM_THREAD_INFO_H
#define __LIBLINUX_ASM_THREAD_INFO_H

#include <asm/page.h>
#include <linux/const.h>

/* thread information allocation */
#ifdef CONFIG_64BIT
#define THREAD_SIZE_ORDER       2
#else
#define THREAD_SIZE_ORDER       1
#endif
#define THREAD_SIZE             (PAGE_SIZE << THREAD_SIZE_ORDER)

/*
 * By aligning VMAP'd stacks to 2 * THREAD_SIZE, we can detect overflow by
 * checking sp & (1 << THREAD_SHIFT), which we can do cheaply in the entry
 * assembly.
 */
#ifdef CONFIG_VMAP_STACK
#define THREAD_ALIGN		(2 * THREAD_SIZE)
#else
#define THREAD_ALIGN		THREAD_SIZE
#endif

#ifndef __ASSEMBLY__

#include <asm/stack_pointer.h>
#include <asm/types.h>

typedef unsigned long mm_segment_t;
struct task_struct;

struct preempt_info {
	u32 reserved;		/* reserved */
	u32 state;		/* thread preempt state */
	u32 cpuid;		/* thread running cpu */
	u32 need_resched;
};

/*
 * low level task data that entry.S needs immediate access to.
 */
struct thread_info {
	unsigned long		tls;
	unsigned long		flags;		/* low level flags */
	mm_segment_t		addr_limit;	/* address limit */
	void				*irq_save_caller;
#ifdef CONFIG_ARM64_SW_TTBR0_PAN
	u64			ttbr0;		/* saved TTBR0_EL1 */
#endif
	struct preempt_info	*preempt;	/* preempt info */
	int			preempt_count;	/* 0 => preemptable, <0 => bug */
	int			irq_disabled;	/* 1 => disabled, 0: enabled */
#ifdef CONFIG_SHADOW_CALL_STACK
	void			*scs_base;
	void			*scs_sp;
#endif
	liblinux_pal_thread_handle_t thread_handle;
	struct list_head list;
#ifdef CONFIG_LIBLINUX_BATCH_UACCESS
	char *copy_to_buff;	/* copy_to_user batched cache */
	unsigned long current_buff_addr;
	unsigned long copy_to_len;
#endif
	int umask;
	bool preempt_bypass;
	pid_t vtgid;
	void			*kobj_ctx;
	uintptr_t		owner; /* this variable must be the last one */
};

#define thread_saved_pc(tsk)	\
	({ panic("liblinux does not support get task pc"); 0; })
#define thread_saved_sp(tsk)	\
	({ panic("liblinux does not support get task sp"); 0; })
#define thread_saved_fp(tsk)	\
	({ panic("liblinux does not support get task fp"); 0; })

void arch_setup_new_exec(void);
#define arch_setup_new_exec     arch_setup_new_exec

void arch_release_task_struct(struct task_struct *tsk);

#endif

#define TIF_SIGPENDING		0	/* signal pending */
#define TIF_NEED_RESCHED	1	/* rescheduling necessary */
#define TIF_NOTIFY_RESUME	2	/* callback before returning to user */
#define TIF_FOREIGN_FPSTATE	3	/* CPU's FP state is not current's */
#define TIF_UPROBE		4	/* uprobe breakpoint or singlestep */
#define TIF_FSCHECK		5	/* Check FS is USER_DS on return */
#define TIF_MTE_ASYNC_FAULT	6	/* MTE Asynchronous Tag Check Fault */
#define TIF_SYSCALL_TRACE	8	/* syscall trace active */
#define TIF_SYSCALL_AUDIT	9	/* syscall auditing */
#define TIF_SYSCALL_TRACEPOINT	10	/* syscall tracepoint for ftrace */
#define TIF_SECCOMP		11	/* syscall secure computing */
#define TIF_SYSCALL_EMU		12	/* syscall emulation active */
#define TIF_MEMDIE		18	/* is terminating due to OOM killer */
#define TIF_FREEZE		19
#define TIF_RESTORE_SIGMASK	20
#define TIF_SINGLESTEP		21
#define TIF_32BIT		22	/* AARCH32 process */
#define TIF_SVE			23	/* Scalable Vector Extension in use */
#define TIF_SVE_VL_INHERIT	24	/* Inherit sve_vl_onexec across exec */
#define TIF_SSBD		25	/* Wants SSB mitigation */
#define TIF_TAGGED_ADDR		26	/* Allow tagged user addresses */
#define TIF_32BIT_AARCH64	27	/* 32 bit process on AArch64(ILP32) */
#ifdef CONFIG_OPT_TIMER_ENERGY_EFFICIENCY
#define TIF_TIMER_DEFERRABLE	33	/* Want deferrable when setting up timers. */
#endif

#define _TIF_SIGPENDING		(1 << TIF_SIGPENDING)
#define _TIF_NEED_RESCHED	(1 << TIF_NEED_RESCHED)
#define _TIF_NOTIFY_RESUME	(1 << TIF_NOTIFY_RESUME)
#define _TIF_FOREIGN_FPSTATE	(1 << TIF_FOREIGN_FPSTATE)
#define _TIF_SYSCALL_TRACE	(1 << TIF_SYSCALL_TRACE)
#define _TIF_SYSCALL_AUDIT	(1 << TIF_SYSCALL_AUDIT)
#define _TIF_SYSCALL_TRACEPOINT	(1 << TIF_SYSCALL_TRACEPOINT)
#define _TIF_SECCOMP		(1 << TIF_SECCOMP)
#define _TIF_SYSCALL_EMU	(1 << TIF_SYSCALL_EMU)
#define _TIF_UPROBE		(1 << TIF_UPROBE)
#define _TIF_FSCHECK		(1 << TIF_FSCHECK)
#define _TIF_SINGLESTEP		(1 << TIF_SINGLESTEP)
#define _TIF_32BIT		(1 << TIF_32BIT)
#define _TIF_SVE		(1 << TIF_SVE)
#define _TIF_MTE_ASYNC_FAULT	(1 << TIF_MTE_ASYNC_FAULT)
#define _TIF_32BIT_AARCH64	(1 << TIF_32BIT_AARCH64)

#define _TIF_WORK_MASK		(_TIF_NEED_RESCHED | _TIF_SIGPENDING | \
				 _TIF_NOTIFY_RESUME | _TIF_FOREIGN_FPSTATE | \
				 _TIF_UPROBE | _TIF_FSCHECK | _TIF_MTE_ASYNC_FAULT)

#define _TIF_SYSCALL_WORK	(_TIF_SYSCALL_TRACE | _TIF_SYSCALL_AUDIT | \
				 _TIF_SYSCALL_TRACEPOINT | _TIF_SECCOMP | \
				 _TIF_SYSCALL_EMU)

#ifdef CONFIG_SHADOW_CALL_STACK
#define INIT_SCS							\
	.scs_base	= init_shadow_call_stack,			\
	.scs_sp		= init_shadow_call_stack,
#else
#define INIT_SCS
#endif

#define LIBLINUX_INVALID_THREAD_HANDLE  (1ULL << 63)
#ifdef CONFIG_LIBLINUX_THREAD_BLOCK
 #define INIT_LIBLINUX_THREAD						\
 	.thread_handle	= {						\
 		.handle = LIBLINUX_INVALID_THREAD_HANDLE,		\
 		.tid = -1,						\
		.tcb_cref = 0,				\
 	},								\
	.umask = 0022,							\
	.list	= {},
#else
#define INIT_LIBLINUX_THREAD						\
	.thread_handle	= {						\
		.handle = LIBLINUX_INVALID_THREAD_HANDLE,		\
		.tid = -1,						\
	},								\
	.umask = 0022,							\
	.list	= {},
#endif

#define INIT_THREAD_INFO(tsk)						\
{									\
	.flags		= _TIF_FOREIGN_FPSTATE,				\
	.preempt_count	= INIT_PREEMPT_COUNT,				\
	.addr_limit	= KERNEL_DS,					\
	INIT_SCS							\
	INIT_LIBLINUX_THREAD						\
}

#ifndef __ASSEMBLY__
/*
 * Check whether a thread is LDK managed kthread or not.
 * All non-LDK kthread will inherent thread_handle from INIT_THREAD,
 * which has the INVALID handle.
 */
static inline bool is_thread_managed(struct thread_info *ti)
{
	return (ti->thread_handle.handle != LIBLINUX_INVALID_THREAD_HANDLE);
}
#endif /* !__ASSEMBLY__ */

#endif /* __LIBLINUX_ASM_THREAD_INFO_H */
