# Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
# Description: HMbuild
# Author: Huawei OS Kernel Lab
# Create: Fri Dec 27 09:53:50 2024

/*
 * ____clone(rpcinfo, buf, flags, stack, ptid,  tls,   ctid,  func,  arg,   modifier)
 *           r0&r1    r2,  stack, stack, stack, stack, stack, stack, stack, stack
 *
 * rpcinfo = __RPC_INFO_ENCODE(__sysmgr_method_proc_hm_clone, 0, 6)
 */

#ifndef __ASSEMBLY__
#define __ASSEMBLY__
#endif

#include <hmasm/kern_syscall.h>

/*
 *           stack layout
 *   high   +------------+
 *          |  modifier  |
 *          +------------+
 *          |    arg     |
 *          +------------+
 *          |    func    |
 *          +------------+
 *          |    ctid    |
 *          +------------+
 *          |    tls     |
 *          +------------+
 *          |    ptid    |
 *          +------------+
 *          |    stack   |
 *          +------------+
 *          |    flags   |
 *          +------------+
 *          |    flags   |
 *          +------------+  <--- sp(should be here befor svc __FAST_SYSCALL_RPC_GROUP1_NOBUF)
 *          |    r4      |
 *          +------------+
 *          |    r5      |
 *   low    +------------+  <--- sp(after push {r4, r5})
 */
.global ____clone
.hidden ____clone
.type   ____clone, %function
____clone:
	push {r4, r5}
	ldr r4, [sp, #32] /* func */
	ldr r5, [sp, #36] /* arg */
	add sp, sp, #8
	svc __FAST_SYSCALL_RPC_GROUP1_NOBUF
	tst r0, r0
	beq 1f
	sub sp, sp, #8
	pop {r4, r5}
	bx lr

1:	mov r0, r5
	bl 3f
2:	mov r7, #1
	svc 0
	b 2b

3:	bx r4
