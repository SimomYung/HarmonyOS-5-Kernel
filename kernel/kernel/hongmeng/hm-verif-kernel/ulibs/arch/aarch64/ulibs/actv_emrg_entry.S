/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Actv Emrg Entry
 * Author: Huawei OS Kernel Lab
 * Create: Dec 2 23:00:20 2020
 */

#ifndef __ASSEMBLY__
#define __ASSEMBLY__
#endif

#include <hmasm/kern_syscall.h>
#include <hmkernel/futex.h>

.global arch_actv_emrg_entry
.type arch_actv_emrg_entry, %function

/*
 *  struct actv_emrg_attr {
 *      int *futex_addr;
 *  }
 *
 *  Assignment of callee-saved register (r19...r28):
 *     r28: futex_addr
 *
 *     r19...r26, temp
 *
 *  arch_actv_emrg_entry(struct actv_emrg_attr *attr)
 *
 */
arch_actv_emrg_entry:
	mov x28, x0
	str wzr, [x0]		/* set futex value be zero */
	mov x0, #0
	mov x1, #0
	mov x2, #0
	mov x3, #0
	mov sp, x0		/* now we do not use stack in emrg entry */
	svc __FAST_SYSCALL_RPC_WFE	/* sysfast_wait_for_event(0, NULL, NULL, 0UL) */
	mov x0, x28
	mov x1, #0
	mov x2, FUTEX_MASK_TYPE_SIMPLE_EMRG
	svc __FAST_SYSCALL_FUTEX_WAIT_SIMPLE	/* sysfast_futex_wait_simple(futex_addr, 0, FUTEX_MASK_TYPE_SIMPLE_EMRG) */
	mov x1, #0
	mov x2, #0
	mov x3, #0
	svc __FAST_SYSCALL_RPC_WFE	/* sysfast_actv_done_emrg(retval, NULL, NULL, 0UL), where the retval is returned by sys_futex_wait_simple */
