/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Wed Sep 25 15:02:00 2019
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
 *     void *recv_buf;
 *     unsigned long rply_recv_buf_sz;
 *     int (*init)(struct arch_actv_local *);
 *     unsigned long rpcinfo; // written when entring kernel
 *     void **action_table;
 *     unsigned long action_table_sz;
 * };
 *
 *  arch_actv_rpc_entry(struct arch_actv_local *actv,
 *			void **action_table,
 *			unsigned long action_table_size);
 *
 *  Assignment of callee-saved register (r4-r8, r10, r11):
 *     r4: kernel use r4 passing rpcinfo
 *
 *     r11: operation table entry
 *     r10: operation table size
 *     r8: arguments 3 for WaitForEvent:
 *       prototype of wait_for_event: (long retval, void *data_ptr,
 *				       void *buf_ptr, unsigned long data_buf_size)
 *       data_ptr are data send to another actv, buf_ptr is buffer used
 *       to receive data from caller. buf_ptr is fixed.
 *       data_buf_size encode size of data and buf. it is not fixed.
 *       Use r8 for buf_ptr, reduce a ldr operation.
 *     r7: 0
 *     r6, r5: temp
 *
 *     for callnum from 65 to 127: exception: kernel use r3, r5, r6 for __arch_exception_info.
 *     Let's convert it to argument list:
 *		xxx_handler(unsigned long long sender, unsigned long cred,
 *			    struct __arch_exception_info expinfo)
 */
arch_actv_rpc_entry:
	/*
	 * x0 is a pointer points to struct arch_actv_local.
	 * It is put at the bottom of activation stack.
	 * By checking alignment, 'struct arch_actv_local' can
	 * be found through sp, no need to alloc a register
	 */
	mov sp, r0

	/*
	 * Alloc 128 bytes from stack. Kernel passes no more
	 * than 128 bytes (1 << 5) * sizeof(unsigned long)
	 */
	sub sp, sp, #128
	mov ip, sp		/* Set base FP: start a new C callchain */
	ldr r11, [sp, #(128+20)] /* Save table base address to r11, which is a callee-saved register */
	ldr r10, [sp, #(128+24)] /* Save table size to r10, callee-saved register */
	ldr r2, [sp, #(128+12)]	/* entry of init function */
	blx r2			/* call init */

	/* store something in callee-saved register */
	/*
	 * entry of all WFE syscall:
	 *   See above. r8 is used for buf_ptr, other 2 arguments are not fixed.
	 */
	ldr r8, [sp, #128 + 4]
	mov r7, #0

	/*
	 * proto of __FAST_SYSCALL_ACTV_DONE_INIT:
	 *   r0: retval, r1: recv_buf, r2: recv_buf_sz (in rply_recv_buf_sz, kernel consider buf part)
	 */
	mov r1, r8		// reply buf
	ldr r2, [sp, #128 + 8]	// reply buf sz
	svc __FAST_SYSCALL_ACTV_DONE_INIT
	b 4f
1:
	ldr r1, [sp, #128]
	mov r2, r8
	ldr r3, [sp, #128 + 8]
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

	/* r0: credential, r4: rpcinfo */
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
