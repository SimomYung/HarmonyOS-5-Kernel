/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Wed Sep 25 10:55:28 2019
 */

#ifndef __ASSEMBLY__
#define __ASSEMBLY__
#endif

#include <hmasm/kern_syscall.h>

.global arch_actv_rpc_entry
.type arch_actv_rpc_entry, %function

/*
 * C prototype:
 *
 * Put following structure at the bottom of stack:
 *
 * struct arch_actv_local {
 *     union {
 *         void *rply_buf;
 *         unsigned long kcall_retval;
 *     };
 *     unsigned long rply_recv_buf_sz;
 *     void *recv_buf;
 *     int (*init)(struct arch_actv_local *);
 *     unsigned long rpcinfo;
 *     void **action_table;
 *     unsigned long action_table_sz;
 *     unsigned long __unused;
 * };
 *
 *  arch_actv_rpc_entry(struct arch_actv_meta *actv, int (*(func[]))(unsigned long long, ...))
 *
 *  Assignment of callee-saved register (r19...r28):
 *     r19: kernel use r19 passing rpcinfo
 *
 *     r28: operation table entry
 *     r27: operation table size
 *
 *     r26, recv_buf
 *     r25: 0
 *     r24 ... temp
 */
arch_actv_rpc_entry:
	/*
	 * x0 is a pointer points to struct arch_actv_local. It is put
	 * at the bottom of activation stack.
	 * By checking alignment, 'struct arch_actv_local' can be found
	 * through sp, no need to alloc a register
	 */
	mov sp, x0

	/*
	 * Alloc 256 bytes from stack. Kernel passes no more
	 * than 256 bytes
	 */
	sub sp, sp, #256
	mov x29, sp		/* Set base FP: start a new C callchain */
	ldr x28, [sp, #(256+40)] /* Save table base address to x28, which is a callee-saved register */
	ldr x27, [sp, #(256+48)] /* Save table base size to x27, which is a callee-saved register */
	ldr x2, [sp, #(256+24)]	/* entry of init function */
#ifdef PAC_FWCFI_SYSSERVICE
	mov x10, #0x24e3
	blraa x2, x10
#else
	blr x2			/* call init */
#endif
	/* store something in callee-saved register */
	/*
	 * WFE syscall:
	 *  See above
	 */
	mov x25, #0
	ldr x26, [sp, #(256 + 16)] // recv_buf, fixed

	/*
	 * proto of __FAST_SYSCALL_ACTV_DONE_INIT:
	 *   x0: retval, x1: recv_buf, x2: recv_buf_sz , in rply_recv_buf_sz.
	 */
	mov x1, x26			// recv_buf, fixed in x26
	ldr x2, [sp, #256 + 8]		// recv_buf_sz, in (struct arch_actv_local)
	svc __FAST_SYSCALL_ACTV_DONE_INIT
	b 4f
1:
	ldp x1, x3, [sp, #(256)] 	// rply_buf/kcall_retval, not fixed
	mov x2, x26			// recv_buf, fixed
	svc __FAST_SYSCALL_RPC_WFE

	/*
	 * the x0 is the error code if wfe failed, otherwise the x0 is the `sender` of rpc call.
	 * the cnode idx has a maximum of 31 bits, so the highest bit of `sender` will not be 1.
	 */
	tbnz x0, #63, .Lwfe
4:
	str x25, [sp, #(256)]		// set rply_buf/kcall_retval to NULL/0
	/* x0: sender, x1: credential, x19: rpcinfo */
	/* See __RPC_INFO_ENCODE: extract callno */
	ubfx x23, x19, #16, #11
	lsl x23, x23, #3                // offset of entry of table
	cmp x23, #0x0			// if caller intentionally invoke callno 0,
	ccmp x23, x27, #0x2, ne 	// set nzcv to 0x2 (cs); else: compare x23 and x27
	str x19, [sp, #256 + 32]
	b.cs 2f
#ifdef CONFIG_COMPAT
	ubfx x22, x19, #32, #1          /* extract compat bit */
	tbz x22, #0, .Lcheck32          /* not compat client */
	add x23, x23, x27               /* offset one table size */
	mov x2, sp
	b 5f
.Lcheck32:
#endif
#ifdef CONFIG_ILP32
	ubfx x22, x19, #34, #1          /* extract ilp32 bit */
	tbz x22, #0, .Lcheck            /* not ilp32 client */
	add x23, x23, x27               /* offset one table size */
#ifdef CONFIG_COMPAT
	add x23, x23, x27               /* offset one more table size */
#endif
.Lcheck:
#endif
5:
	ldr x23, [x28, x23]
3:
#ifdef PAC_FWCFI_SYSSERVICE
	mov x10, #22806
	blraa x23, x10
#else
	blr x23
#endif
	b 1b

2:	ldr x23, [x28]
	add x2, sp, #256
	b 3b
.Lwfe:
	mov x1, #0
	svc __FAST_SYSCALL_RPC_WFE
	b 4b
