/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Tue May 05 03:30:51 2020
 */

#ifndef __ASSEMBLY__
#define __ASSEMBLY__
#endif

#include <hmasm/debug.h>
#include <hmasm/kern_syscall.h>
#include <hmkernel/futex.h>
#include <hmkernel/errno.h>
#include <hmasm/pac.h>
#ifdef PAC_FWCFI_SYSSERVICE
#include <generated/pac.h>
#endif

/*
 *  1. Deployment of activation mem buf:
 *
 *  High Addr   +------------+ <------------------------------+
 *              |  actv pure |                      ^         ^
 *              +------------+                      |         |
 *              |  recv buf  |                      |         |
 *              +------------+ <-------------+      |         |
 *              |    tsd     |               ^      |         |
 *              +------------+               |      |         |
 *              |    TLS     |               |      |         |
 *  TLS REG --> +------------+               |  Reserved Buf  |
 *              |   pthread  |               |      |         |
 *              +------------+               |      |         |
 *              | raw thread |               |      |   Activation Mem Buf
 *              |  specific  |               |      |         |
 *              +------------+               |      |         |
 *              |   retbuf   | Activation Stack Buf |         |
 *              +------------+               |      |         |
 *              | actv_local |               |      V         |
 *              +------------+ <--------------------+         |
 *              |  256 bytes |               |                |
 *  SP REG ---> +------------+               |                |
 *              |    stack   |               |                |
 *              +------------+               |                |
 *              |    guard   |               V                V
 *  Low Addr    +------------+ <-------------+----------------+
 *
 *  2. Related data struct in mem buf:
 *     struct actv_pure {
 *         raw_thread_t thread;
 *     };
 *
 *     struct arch_actv_local {
 *         union {
 *             void *rply_buf;
 *             unsigned long kcall_retval;
 *         };
 *         void *recv_buf;
 *         unsigned long rply_recv_buf_sz;
 *         int (*init)(struct arch_actv_local *);
 *         unsigned long rpcinfo; // written when entring kernel
 *         void **action_table;
 *         unsigned long action_table_sz;
 *     };
 *
 *  3. Prototype of rpc entry:
 *     arch_pure_actv_rpc_entry(struct actv_pure_attr *attr)
 *
 *  4. For self init: Assignment of callee-saved register (x27):
 *     r27: addr of actv_pure_attr
 *     long actv_pure_self_init_hdlr(unsigned long init_attr, unsigned long rpcinfo)
 *
 *  5. For handle rpc: Assignment of callee-saved register (r19...r28):
 *     r19: kernel use r19 passing rpcinfo
 *
 *     r28: operation table entry
 *     r27: operation table size
 *
 *     r26, recv_buf
 *     r25: 0
 *     r24 ... temp
 */

/* this macro is used for combine assist membuf with actv kobj */
.macro arch_actv_pure_comb_kobj
	mov x0, x28		/* arg0 is the pointer point to struct actv_pure */
	ldr x1, [x27, #80]	/* fetch the addr of actv_pure_stack_comb_kobj */

	/*
	 * call attr->comb_kobj_hdlr:
	 * 1) set tls
	 * 2) set actv_cref
	 *
	 * after doing this, holding userspace lock is allowed.
	 */
	blr_pacfix x1, x10, PAC_MODIFIER_COMB_KOBJ_HDLR
.endm

/*
 * This macro is used for prepare args, sp and
 * call actv_pure_self_init_hdlr.
 */
.macro arch_actv_pure_self_init, label
	/* clear the actv_pure, actv_local, sstack_base and sstack in actv_pure_attr */
	mov x0, #0
	str x0, [x27]
	str x0, [x27, #8]
#ifdef CONFIG_NEED_SHADOW_STACK
	str x0, [x27, #16]
	str x0, [x27, #24]
#endif

	mov x0, x27		/* arg0 is the pointer point to struct actv_pure_attr */
	mov x1, x19		/* arg1 is the rpcinfo */
	ldr x2, [x27, #32]	/* fetch the addr of actv_pure_self_init_hdlr */

	/*
	 * call actv_pure_self_init_hdlr:
	 * 0) alloc membuf and sstack
	 * 1) alloc actv id
	 * 2) touch recv buffer
	 * 3) init actv stack
	 * 4) call attr->actv_init if it is not NULL
	 * 5) return E_HM_ACTIVATION_RPC_RETRY if self init success
	 *    return E_HM_NOMEM if alloc membuf and sstack failed. and in this case,
	 *    E_HM_NOMEM should be returned to client and this actv should do self
	 *    init again when been used next time.
	 *
	 * Note: if self init success, init_attr and actv_local is not required,
	 * so x27 and x28 can be used later.
	 */
	blr_pacfix x2, x10, PAC_MODIFIER_SELF_INIT_HDLR

	/*
	 * if attr->actv_local == NULL, no mem is avaible for current
	 * actv, and thus do not switch sp.
	 */
	ldr x1, [x27, #8]	/* read actv_local and store it in x1 */
	mov lr, #0		/* set lr be 0 before finishing the self init */
	cbz x1, _init_end_\label

	/*
	 * if attr->actv_local != NULL, membuf of the current is initialized
	 * successfully, and thus switch the sp to the actv_local.
	 */
#ifdef CONFIG_NEED_SHADOW_STACK
	ldr x18, [x27, #24]	/* read sstack and store it in x18 */
	str xzr, [x27, #24]
#endif
	mov sp, x1		/* store actv_local in sp */
	/*
	 * Alloc 256 bytes from stack. Kernel passes no more
	 * than 256 bytes (1 << 5) * sizeof(unsigned long)
	 */
	sub sp, sp, #256

	mov lr, #1		/* set lr be 1 after self init */
_init_end_\label:
.endm

/*
 * This macro is used for preparing callee-saved regs,
 * return RPC_RETRY after self init and begin to handle next rpc.
 */
.macro arch_actv_pure_rpc_core, label
	/* prepare callee-saved regs for handling rpc */
	mov x29, sp			/* Set base FP: start a new C callchain */
	ldr x28, [sp, #(256+40)]	/* Save table base address to x28, which is a callee-saved register */
	ldr x27, [sp, #(256+48)]	/* Save table base size to x27, which is a callee-saved register */
	ldr x26, [sp, #(256 + 16)]	/* save recv_buf to x26, used for buf_ptr, fixed */
	mov x25, #0

	/* after self init, x0 must be a errno, especially E_HM_ACTIVATION_RPC_RETRY */
	mov x1, #0			/* set rply_buf be NULL */
	mov x2, x26			/* set recv_buf from x26 */
	ldr x3, [sp, #256 + 8]		/* set recv_buf_sz, in (struct arch_actv_local) */
	svc __FAST_SYSCALL_RPC_WFE
	b 4f
1:
	ldp x1, x3, [sp, #(256)] 	/* rply_buf/kcall_retval, not fixed */
	mov x2, x26			/* recv_buf, fixed */
	mov lr, #1			/* lr should be set none zero after self init */
	svc __FAST_SYSCALL_RPC_WFE

	/*
	 * the x0 is the error code if wfe failed, otherwise the x0 is the `sender` of rpc call.
	 * the cnode idx has a maximum of 31 bits, so the highest bit of `sender` will not be 1.
	 */
	tbnz x0, #63, .Lwfe_\label
4:
	str x25, [sp, #(256)]		/* set rply_buf/kcall_retval to NULL/0 */
	/* x1: credential, x19: rpcinfo */
	/* See __RPC_INFO_ENCODE: extract callno */
	ubfx x23, x19, #16, #11
	lsl x23, x23, #3                /* offset of entry of table */
	cmp x23, #0x0			/* if caller intentionally invoke callno 0, */
	ccmp x23, x27, #0x2, ne 	/* set nzcv to 0x2 (cs); else: compare x23 and x27 */
	str x19, [sp, #256 + 32]
	b.cs 2f
#ifdef CONFIG_COMPAT
	ubfx x22, x19, #32, #1          /* extract compat bit */
	tbz x22, #0, .Lcheck32_\label   /* not compat client */
	add x23, x23, x27               /* offset one table size */
	mov x2, sp
	b 5f
.Lcheck32_\label:
#endif
#ifdef CONFIG_ILP32
	ubfx x22, x19, #34, #1          /* extract ilp32 bit */
	tbz x22, #0, .Lcheck_\label     /* not ilp32 client */
	add x23, x23, x27               /* offset one table size */
#ifdef CONFIG_COMPAT
	add x23, x23, x27               /* offset one more table size */
#endif
.Lcheck_\label:
#endif
5:
	ldr x23, [x28, x23]
3:
	blr_pacfix x23, x11, PAC_MODIFIER_VOID
	b 1b

2:	ldr x23, [x28]
	add x2, sp, #256
	b 3b
.Lwfe_\label:
	mov x1, #0
	svc __FAST_SYSCALL_RPC_WFE
	b 4b
.endm

/*
 * arch_actv_pure_wait_rpc_entry use futex to limit only
 * one actv can do self init.
 * sysmgr will use this entry since only limited actv in
 * ex_actv_pool can handle the pagefault in self init.
 */
.global arch_actv_pure_wait_rpc_entry
.type arch_actv_pure_wait_rpc_entry, %function
arch_actv_pure_wait_rpc_entry:
	mov lr, #0		/* set lr be 0 before self init */
	mov x27, x0		/* store actv_pure_attr in x27 */

	mov x0, #0		/* set retval be 0 for sysfast_wait_for_event */

_actv_ready:
	ldr x28, [x27, #48]	/* read assist_actv_pure and store it in x28 */
	ldr x1, [x27, #56]	/* read assist_actv_local and store it in x1 */
#ifdef CONFIG_NEED_SHADOW_STACK
	ldr x18, [x27, #72]	/* read assist_sstack and store it in x18 */
#endif
	mov sp, x1		/* store assist_actv_local in sp */

	/*
	 * Alloc 256 bytes from stack. Kernel passes no more
	 * than 256 bytes (1 << 5) * sizeof(unsigned long)
	 */
	sub sp, sp, #256

	/* x0 has been set before enter _actv_ready */
	mov x3, #0		/* set data_buf_sz be 0 */
	mov x2, #0		/* set buf_ptf be NULL */
	mov x1, #0		/* set data_ptr be NULL */
	svc __FAST_SYSCALL_RPC_WFE /* move actv state to ready */

	b _grab_start

_wait:
	/*
	 * Do the wait operation below, fetch all args from actv_pure_attr.
	 * Thus sysfast_futex_wait should not return errno and do not need
	 * to check its retval.
	 */
	add x0, x27, #40			/* addr of the futex value */
	ldr w1, [x0]				/* read the lock */
	cmp w1, #0				/* check if free */
	beq _grab_entry				/* free now, go grab */
	mov x2, FUTEX_MASK_TYPE_SIMPLE_ALL	/* set futex mask type be MASK ALL */
	svc __FAST_SYSCALL_FUTEX_WAIT_SIMPLE	/* sysfast_futex_wait_simple(&attr->g_futex_val, g_futex_val, FUTEX_MASK_TYPE_SIMPLE_ALL) */

/* grab the lock */
_grab_start:
	ubfx x23, x19, #16, #11			/* extract callnum from rpcinfo: ((x19 >> 16) & ((1<<11)-1) */
	cmp x23, #83				/* check whether the rpc is vsfault upcall */
	bne _grab_entry
	mov x0, ERR_AS_MACRO(E_HM_NOOBJ)	/* return E_HM_NOOBJ to kernel directly for handling vsfault */
	b _actv_ready

_grab_entry:
	add x2, x27, #40
	ldaxr w0, [x2]			/* read the lock */
	cbnz w0, _wait			/* zero means free, branch to _wait if not */
	mov w1, #1
	stxr w0, w1, [x2]		/* try to grab the lock */
	cbnz w0, _grab_entry		/* check if set success, and retry if set failed */

	/* here comes the success operation */
	/* combine assist membuf with actv kobj */
	arch_actv_pure_comb_kobj
	/* prepare args, sp and call actv_pure_self_init_hdlr */
	arch_actv_pure_self_init wait

	/* save errno in x2 */
	mov x2, x0

	/* clear the lock */
	add x0, x27, #40			/* addr of the futex value */
	stlr wzr, [x0]				/* free the lock */
	mov x1, #1
	svc __FAST_SYSCALL_FUTEX_WAKE_SIMPLE	/* sysfast_futex_wake_simple(&attr->g_futex_val, 1) */

	/* restore errno from x2 to x0 */
	mov x0, x2

	/*
	 * lr == 0: self init fail
	 * lr == 1: self init success
	 * if self init fail, just return errno by x0 to client and
	 * current actv should do self init again when been used next time.
	 *
	 * Note: x0 is used to return the errno to client.
	 */
	cbz lr, _actv_ready

	/* prepare callee-saved regs, return RPC_RETRY and begin to handle next rpc */
	arch_actv_pure_rpc_core wait

/*
 * arch_actv_pure_pre_init_rpc_entry will do actv init before been
 * called for the first time.
 */
.global arch_actv_pure_pre_init_rpc_entry
.type arch_actv_pure_pre_init_rpc_entry, %function
arch_actv_pure_pre_init_rpc_entry:
	mov lr, #0		/* set lr be 0 before self init */
	mov x27, x0		/* store actv_pure_attr in x27 */
	ldr x28, [x0, #48]	/* read assist_actv_pure and store it in x28 */
	ldr x1, [x0, #56]	/* read assist_actv_local and store it in x1 */
#ifdef CONFIG_NEED_SHADOW_STACK
	ldr x18, [x0, #72]	/* read assist_sstack and store it in x18 */
#endif
	mov sp, x1		/* store assist_actv_local in sp */

	/*
	 * Alloc 256 bytes from stack. Kernel passes no more
	 * than 256 bytes (1 << 5) * sizeof(unsigned long)
	 */
	sub sp, sp, #256

	/* combine assist membuf with actv kobj */
	arch_actv_pure_comb_kobj
	/* prepare args, sp and call actv_pure_self_init_hdlr */
	arch_actv_pure_self_init pre_init

	/*
	 * lr == 0: self init fail
	 * lr == 1: self init success
	 * if self init success, continue to provide service.
	 */
	cbz lr, _fail_init
	/* prepare callee-saved regs, return RPC_RETRY and begin to handle next rpc */
	arch_actv_pure_rpc_core pre_init

_fail_init:
	/*
	 * if self init fail, branch to _fail_init and return retval to caller. In
	 * this case actv can not be used later and should be destroyed by the caller.
	 *
	 * Note: x0 is the retval returned by self init.
	 */
	mov x1, #0		/* set data_ptr be NULL */
	mov x2, #0		/* set buf_ptf be NULL */
	mov x3, #0		/* set data_buf_sz be 0 */
	svc __FAST_SYSCALL_RPC_WFE /* move actv state to ready from uninited */
	/*
	 * The actv that pre self init fail should be destroyed by server and thus
	 * should not run the below instruction. Otherwise, by 'brk' the actv will
	 * crash.
	 */
	brk #BRK_PANIC
