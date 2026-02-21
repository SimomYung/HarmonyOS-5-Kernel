/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Wed Apr 29 16:21:09 2020
 */
#ifndef __ASSEMBLY__
#define __ASSEMBLY__
#endif

#include <hmasm/kern_syscall.h>
#include <hmkernel/futex.h>
#include <hmkernel/errno.h>

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
 *              |  128 bytes |               |                |
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
 *  4. For self init: Assignment of callee-saved register (r10):
 *     r10: addr of actv_pure_attr
 *     long actv_pure_self_init_hdlr(unsigned long actv_pure_attr, unsigned long rpcinfo)
 *
 *  5. For handle rpc: Assignment of callee-saved register (r4-r8, r10, r11):
 *     r4: kernel use r4 passing rpcinfo
 *
 *     r11: operation table entry
 *     r10: operation table size
 *     r8: arguments 3 for WaitForEvent:
 *       prototype of wait_for_event: (long retval, void *data_ptr,
 *                                     void *buf_ptr, unsigned long data_buf_size)
 *       data_ptr are data send to another actv, buf_ptr is buffer used
 *       to receive data from caller. buf_ptr is fixed.
 *       data_buf_size encode size of data and buf. it is not fixed.
 *       Use r8 for buf_ptr, reduce a ldr operation.
 *     r7: 0
 *     r6, r5: temp
 *
 *     for callnum from 65 to 127: exception: kernel use r3, r5, r6 for __arch_exception_info.
 *     Let's convert it to argument list:
 *              xxx_handler(unsigned long long sender, unsigned long cred,
 *                          struct __arch_exception_info expinfo)
 */

/* this macro is used for combine assist membuf with actv kobj */
.macro arch_actv_pure_comb_kobj
	mov r0, r11		/* arg0 is the pointer point to struct actv_pure */
	ldr r1, [r10, #40]	/* fetch the addr of actv_pure_stack_comb_kobj */

	/*
	 * call attr->comb_kobj_hdlr:
	 * 1) set tls
	 * 2) set actv_cref
	 *
	 * after doing this, holding userspace lock is allowed
	 */
	blx r1
.endm

/*
 * This macro is used for prepare args, sp and
 * call actv_pure_self_init_hdlr.
 */
.macro arch_actv_pure_self_init, label
	/* clear the actv_pure, actv_local, sstack_base and sstack in actv_pure_attr */
	mov r0, #0
	str r0, [r10]
	str r0, [r10, #4]

	mov r0, r10		/* arg0 is the pointer point to struct actv_pure_attr */
	mov r1, r4		/* arg1 is rpcinfo */
	ldr r2, [r10, #16]	/* fetch the addr of actv_pure_self_init_hdlr */

	/*
	 * call actv_pure_self_init_hdlr:
	 * 0) alloc membuf
	 * 1) alloc actv id
	 * 2) touch recv buffer
	 * 3) init actv stack
	 * 4) call attr->actv_init if it is not NULL
	 * 5) return E_HM_ACTIVATION_RPC_RETRY if self init success
	 *
	 * after actv_pure_self_init_hdlr, init_attr and actv_local is not required,
	 * so r10 and r11 can be used later.
	 */
	blx r2

	/*
	 * if attr->actv_local == NULL, no mem is avaible for current
	 * actv, and thus do not switch sp.
	 */
	ldr r1, [r10, #4]	/* read actv_local and stroe it in r1 */
	mov lr, #0		/* set lr be 0 before finishing the self init */
	cmp r1, #0
	beq _init_end_\label

	/*
	 * if attr->actv_local != NULL, membuf of the current is initialized
	 * successfully, and thus switch the sp to the actv_local.
	 */
	mov sp, r1	/* read actv_local and stroe it in sp */

	/*
	 * Alloc 128 bytes from stack. Kernel passes no more
	 * than 128 bytes (1 << 5) * sizeof(unsigned long)
	 */
	sub sp, sp, #128

	mov lr, #1		/* set lr be 1 after self init */
_init_end_\label:
.endm

/*
 * This macro is used for preparing callee-saved regs,
 * return RPC_RETRY after self init and begin to handle next rpc.
 */
.macro arch_actv_pure_rpc_core
	/* prepare callee-saved regs for handling rpc */
	mov ip, sp		 /* Set base FP: start a new C callchain */
	ldr r11, [sp, #(128+20)] /* save table base address in actv_local */
	ldr r10, [sp, #(128+24)] /* save table size to r10, callee-saved register */
	ldr r8, [sp, #128 + 4]   /* save buf_ptr to r8, used for wfe */
	mov r7, #0		 /* set r7 be 0 */

	/* after self init, return E_HM_ACTIVATION_RPC_RETRY through wfe. */
	/* r0 must be an errno returned from self init, so do not change it again. */
	mov r1, #0		/* set rply buf be NULL */
	mov r2, r8		/* set recv buf from r8 */
	ldr r3, [sp, #128 + 8]	/* set rply_recv_sz from actv local */
	svc __FAST_SYSCALL_RPC_WFE
	b 4f

	/* the follow code is as the same as arch_actv_rpc_entry */
1:
	ldr r1, [sp, #128]
	mov r2, r8
	ldr r3, [sp, #128 + 8]
	mov lr, #1 /* lr should be set none zero after self init */
	svc __FAST_SYSCALL_RPC_WFE
	/* reset rply_buf/kcall_retval to NULL/0. r7 is ensure to be 0. */

	/*
	 * the r0 is the error code if wfe failed, otherwise the r0 is the `{xact/caller}_cnode_idx`
	 * of rpc call. the cnode idx has a maximum of 31 bits, so the highest bit of
	 * `{xact/caller}_cnode_idx` will not be 1.
	 */
	cmp r0, #0
	bge 4f
	mov r1, #0
	svc __FAST_SYSCALL_RPC_WFE
4:
	str r7, [sp, #128]

	/* r2: credential, r4: rpcinfo */
	/* See __RPC_INFO_ENCODE: extract callno:
	 *     ((r4 >> 16) & ((1<<11)-1) * 4)
	 *     ((r4 >> 14) & 0x1ffc)
	 */
	str r4, [sp, #128 + 16]  // save callinfo on stack
	lsr r4, r4, #14
	ldr r9, =0x1ffc
	and r9, r9, r4
	cmp r9, #0
	cmpne r9, r10
	bcs 2f  /* callnumber is invalid */
	ldr r9, [r11, r9]
3:
	blx r9
	b 1b
2:
	ldr r9, [r11]
	mov r3, sp
	add r3, r3, #128
	b 3b
	.LTORG
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
	mov r10, r0		/* store actv_pure_attr in r10 */

	mov r0, #0		/* set retval be 0 for sysfast_wait_for_event */

_actv_ready:
	ldr r11, [r10, #24]	/* read assist_actv_pure and store it in r11 */
	ldr sp, [r10, #28]	/* read assist_actv_local and stroe it in sp */

	/*
	 * Alloc 128 bytes from stack. Kernel passes no more
	 * than 128 bytes (1 << 5) * sizeof(unsigned long)
	 */
	sub sp, sp, #128

	mov r3, #0		/* set data_buf_sz be 0 */
	mov r2, #0		/* set buf_ptf be NULL */
	mov r1, #0		/* set data_ptr be NULL */
	svc __FAST_SYSCALL_RPC_WFE /* move actv state to ready from uninited */

	b _grab_start

_wait:
	/*
	 * Do the wait operation below, fetch all args from actv_pure_attr.
	 * The unique user of this wait_rpc_entry, namely sysmgr, should
	 * set g_hmsigmask be -1 when init actv_pure_attr.
	 * Thus sysfast_futex_wait_simple should not return errno and do not need
	 * to check its retval.
	 */
	add r0, r10, #20			/* addr of the futex value */
	ldr r1, [r0]				/* read the lock */
	cmp r1, #0				/* check if free */
	beq _grab_entry				/* free now, go grab */
	mov r2, #FUTEX_MASK_TYPE_SIMPLE_ALL	/* set futex mask type be MASK ALL */
	svc __FAST_SYSCALL_FUTEX_WAIT_SIMPLE	/* sysfast_futex_wait_simple(&attr->g_futex_val, g_futex_val, FUTEX_MASK_TYPE_SIMPLE_ALL) */

/* grab the lock */
_grab_start:
	lsr r4, r4, #16
	ldr r9, =0x7ff
	and r9, r9, r4				/* extract callnum from rpcinfo: ((r4 >> 16) & ((1<<11)-1) */
	cmp r9, #83				/* check whether the rpc is vsfault upcall */
	bne _loop
	mov r0, #ERR_AS_MACRO(E_HM_NOOBJ)	/* return E_HM_NOOBJ to kernel directly for handling vsfault */
	b _actv_ready

_grab_entry:
	dmb ish
_loop:
	add r2, r10, #20
	ldrex r0, [r2]			/* read the lock */
	cmp r0, #0			/* check if zero, zero means free */
	bne _wait			/* branch to _wait */
	mov r1, #1
	strexeq r0, r1, [r2]		/* try to grab the lock */
	cmpeq r0, #0			/* check if set success */
	bne _loop			/* retry if set failed */
	dmb ish

	/* here comes the success operation */
	/* combine assist membuf with actv kobj */
	arch_actv_pure_comb_kobj
	/* prepare args, sp and call actv_pure_self_init_hdlr */
	arch_actv_pure_self_init wait

	/* save errno in r2 */
	mov r2, r0

	/* clear the lock */
	mov r0, #0
	dmb ish
	str r0, [r10, #20]			/* free the lock */
	add r0, r10, #20			/* addr of the futex value */
	mov r1, #1
	svc __FAST_SYSCALL_FUTEX_WAKE_SIMPLE	/* sysfast_futex_wake_simple(&attr->g_futex_val, 1) */

	/* restore errno from r2 to r0 */
	mov r0, r2

	/*
	 * lr == 0: self init fail
	 * lr == 1: self init success
	 * if self init fail, just return errno by r0 to client and
	 * current actv should do self init again when been used next time.
	 *
	 * Note: r0 is used to return the errno to client.
	 */
	cmp lr, #0
	beq _actv_ready

	/* prepare callee-saved regs, return RPC_RETRY and begin to handle next rpc */
	arch_actv_pure_rpc_core

/*
 * arch_actv_pure_pre_init_rpc_entry will do actv init before been
 * called for the first time.
 */
.global arch_actv_pure_pre_init_rpc_entry
.type arch_actv_pure_pre_init_rpc_entry, %function
arch_actv_pure_pre_init_rpc_entry:
	mov lr, #0		/* set lr be 0 before self init */
	mov r10, r0		/* store actv_pure_attr in r10 */
	ldr r11, [r0, #24]	/* read assist_actv_pure and store it in r11 */
	ldr sp, [r0, #28]	/* read assist_actv_local and stroe it in sp */

	/*
	 * Alloc 128 bytes from stack. Kernel passes no more
	 * than 128 bytes (1 << 5) * sizeof(unsigned long)
	 */
	sub sp, sp, #128

	/* combine assist membuf with actv kobj */
	arch_actv_pure_comb_kobj
	/* prepare args, sp and call actv_pure_self_init_hdlr */
	arch_actv_pure_self_init pre_init

	/*
	 * lr == 0: self init fail
	 * lr == 1: self init success
	 * if self init success, continue to provide service.
	 */
	cmp lr, #0
	beq _fail_init
	/* prepare callee-saved regs, return RPC_RETRY and begin to handle next rpc */
	arch_actv_pure_rpc_core

_fail_init:
	/*
	 * if self init fail, branch to _fail_init and return retval to caller. In
	 * this case actv can not be used later and should be destroyed by the caller.
	 *
	 * Note: r0 is the retval returned by self init.
	 */
	mov r1, #0		/* set data_ptr be NULL */
	mov r2, #0		/* set buf_ptf be NULL */
	mov r3, #0		/* set data_buf_sz be 0 */
	svc __FAST_SYSCALL_RPC_WFE /* move actv state to ready from uninited */
	/*
	 * The actv that pre self init fail should be destroyed by server and thus
	 * should not run the below instruction. Otherwise, by 'udf' the actv will
	 * crash.
	 */
	udf #0
