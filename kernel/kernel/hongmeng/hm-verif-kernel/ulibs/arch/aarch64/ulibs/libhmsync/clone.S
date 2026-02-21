# Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
# Description: HMbuild
# Author: Huawei OS Kernel Lab
# Create: Tue Dec 25 12:04:46 2024

/*
 * ____clone(rpcinfo, buf, flags, stack, ptid, tls, ctid, func, arg,   modifier)
 *           x0,      x1,  x2,    x3,    x4,   x5,  x6,   x7,   stack, stack
 *
 * rpcinfo = __RPC_INFO_ENCODE(__sysmgr_method_proc_hm_clone, 0, 0)
 */

#ifndef __ASSEMBLY__
#define __ASSEMBLY__
#endif

#include <hmasm/kern_syscall.h>

.global ____clone
.hidden ____clone
.type   ____clone, %function
____clone:
	// align stack and save func, arg
	and x3, x3, #-16
	ldr x8, [sp] /* arg */
	stp x7, x8, [x3, #-16]!
#ifdef PAC_FWCFI_SYSSERVICE
	ldr x7, [sp, #8]
#endif

	svc __FAST_SYSCALL_RPC_GROUP1_NOBUF

	cbz x0, 1f
	// parent
	ret
	// child
1:	ldp x1, x0, [sp], #16
	mov x29, #0
#ifdef PAC_FWCFI_SYSSERVICE
	blraa x1, x7
#else
	blr x1
#endif
	mov x8, #93 // SYS_exit
	svc #0
