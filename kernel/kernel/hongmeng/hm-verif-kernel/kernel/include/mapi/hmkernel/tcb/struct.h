/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Related definitions of tcb that need to be exported to modules.
 * Author: Huawei OS Kernel Lab
 * Create: Mon Dec 30 18:53:36 2019
 */

#ifndef MAPI_HMKERNEL_TCB_STRUCT_H
#define MAPI_HMKERNEL_TCB_STRUCT_H

#include <hmkernel/kernel.h>
#include <hmkernel/bitops/bitops.h>

/*
 * TCB state transition:
 *
 *   (offline by preemted/yield)
 *   +------------------------+
 *   |                        |
 *   |                        |
 *   V                        |
 * READY ----(online)----> RUNNING
 *   ^                        |
 *   |                        |
 *   |          (block with different reasons)
 *   |                        |
 *   |                        |
 *   |       (unblock)        V
 *   +-------------------- BLOCKED
 *
 *             RUNNING
 *                |
 *                |
 * (tcb based actv is with hard exception)
 *                |
 *                |
 *                V
 *               DEAD
 *
 * NOTE: A tcb only exits when the base actv of it with a hard exception.
 *
 *       Currently, the hard exception can be raised by:
 *       - hmsignal: HMSIGNAL_KILL
 *       - exception
 *       - actv upcall
 */
ENUM(4, tcb_state_e, TCB_STATE_,
	READY,
	RUNNING,
	BLOCKED,
	DEAD)

enum tcb_block_reason {
	TCB_BLOCK_FUTEX,
	TCB_BLOCK_RECV,
	TCB_BLOCK_TRIGGER,
	TCB_BLOCK_RT_FUTEX,
	TCB_BLOCK_EVENTPOLL = 4,
#ifdef CONFIG_ACTV_POOL_WITH_THREADS
	/*
	 * Indicate the TCB is added to an actvpool and is used to
	 * provide the RPC service like a real Activation.
	 */
	TCB_BLOCK_ACTVPOOL = 5,
#endif
#ifdef CONFIG_ULWT
	TCB_BLOCK_ULWT_MON = 6,
#endif
	TCB_BLOCK_PAUSE = 7,
	TCB_BLOCK_SLEEP = 8,
#ifdef CONFIG_USER_FUTEX
	TCB_BLOCK_AT_ADDR = 9,
#endif
	TCB_BLOCK_SUSPEND = 10,
#ifdef CONFIG_TSM_FFRT
	TCB_BLOCK_ULWT_KINFO = 11,
#endif
	TCB_NO_BLOCK = 12,
};

/* This is used to mark the high-irq-load cpu, because the I/O nterrupt is bound to cpu 6(udk-irq/6) */
#define HIGH_IRQ_CPU 6U

/* These are available bits that indicate the sched state of a tcb, used by tcb->sched_flag */
#define SCHED_FLAG_RESCHED		1UL
#define SCHED_FLAG_YIELD		BIT(1)
#define SCHED_FLAG_WAKE_SYNC		BIT(2)

/* if resched happends for a global preempt tcb, GPREEMPT_RESCHED is set */
#define SCHED_FLAG_GPREEMPT_RESCHED	BIT(4)
#define SCHED_FLAG_CPU_FIXED		BIT(5)
#define SCHED_FLAG_SERVER_BOOST_ENTER		BIT(6)
#define SCHED_FLAG_SERVER_BOOST_LEAVE		BIT(7)

/*
 * BITS 8-15 reserved for load-balance hint
 * higher bit indicates stronger constraint
 * LE: cluster `no bigger than` tcb_cpu's is allowed
 * GE: cluster `no smaller than` tcb_cpu's is allowed
 * SKIP: skip core-selection when tcb enqueues
 */
#define SCHED_FLAGS_BALANCE			BIT_RANGE(8, 15)
#define SCHED_FLAG_BALAN_CAPACITY_LE		BIT(9)
#define SCHED_FLAG_BALAN_CAPACITY_GE		BIT(10)
#define SCHED_FLAG_BALAN_SKIP			BIT(11)
#define SCHED_FLAG_BALAN_PIN			BIT(12)

/*
 * BITS 16-24 reserved for skip reasons
 * tcb may be skipped for next round of pick, due to following detailed reasons
 * !!! These flags should not be set when kernel resched is disabled.
 */
#define SCHED_FLAGS_SKIP					BIT_RANGE(16, 24)

#define SCHED_FLAG_SKIP_PREEMPT_EXP			BIT(16)
#define SCHED_FLAG_SKIP_PREEMPT_FASTPATH_FAIL		BIT(17)
#define SCHED_FLAG_SKIP_PREEMPT_ACTV_OFFLINE_FAIL	BIT(18)
/* tcb should force enqueue self when schedule, since it's cpu/affinity may be changed */
#define SCHED_FLAG_REPICK			BIT(19)
/* tcb cannot be picked until preempt condition is met */
#define SCHED_FLAG_PREEMPT_BUSY			BIT(20)

/* The load needs to be trans during the wakeup tcb until offline */
#define SCHED_FLAG_LOAD_TRANS			BIT(25)
/* tcb in IO state */
#define SCHED_FLAG_IOWAIT			BIT(26)
#define SCHED_FLAG_CHOSEN			BIT(27)
#define SCHED_FLAG_IN_SOFT_PHASE 		BIT(28)
#define SCHED_FLAG_PREEMPT_KILL 		BIT(29)

/*
 * BITS [28, 30] reserved for rt_bandwidth_opt
 */
/* task throttled cause the quota of rt_bandwidth exausted */
#define SCHED_FLAG_SUPPRESSED		BIT(30)

/* tcb is serving as a tcb_current */
#define SCHED_FLAG_CURR			BIT(31)

#define SECCOMP_SYSCALL_NU		512U
#define SECCOMP_TCB_BITMAP_SIZE		(SECCOMP_SYSCALL_NU / (sizeof(u64) * BITS_PER_BYTE))

#define SCHED_FLAGS_NEXT_CLR	( SCHED_FLAG_RESCHED		| \
			  SCHED_FLAGS_SKIP		| \
			  SCHED_FLAGS_BALANCE		| \
			  SCHED_FLAG_YIELD		| \
			  SCHED_FLAG_WAKE_SYNC		| \
			  SCHED_FLAG_LOAD_TRANS		| \
			  SCHED_FLAG_SUPPRESSED		| \
			  SCHED_FLAG_CHOSEN		| \
			  SCHED_FLAG_IOWAIT)
#endif
