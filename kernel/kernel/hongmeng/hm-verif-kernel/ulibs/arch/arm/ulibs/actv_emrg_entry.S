/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Actv Emrg Entry
 * Author: Huawei OS Kernel Lab
 * Create: Dec 2 23:05:20 2020
 */

#ifndef __ASSEMBLY__
#define __ASSEMBLY__
#endif

#include <hmasm/kern_syscall.h>
#include <hmkernel/futex.h>

.global arch_actv_emrg_entry
.type arch_actv_emrg_entry, %function

/*
 *  Assignment of callee-saved register (r4-r8, r10, r11):
 *     r11: futex_addr
 *
 *     r4...r8, temp
 *
 *  arch_actv_emrg_entry(struct actv_emrg_attr *attr)
 *
 */
arch_actv_emrg_entry:
	mov r11, r0
	mov r1, #0
	str r1, [r0]		/* set futex value be zero */
	mov r0, #0
	mov r2, #0
	mov r3, #0
	mov sp, r0		/* now we do not use stack in emrg entry */
	svc __FAST_SYSCALL_RPC_WFE	/* sysfast_wait_for_event(0, NULL, NULL, 0UL) */
	mov r0, r11
	mov r1, #0
	mov r2, #FUTEX_MASK_TYPE_SIMPLE_EMRG
	svc __FAST_SYSCALL_FUTEX_WAIT_SIMPLE	/* sysfast_futex_wait_simple(futex_addr, 0, FUTEX_MASK_TYPE_SIMPLE_EMRG) */
	mov r1, #0
	mov r2, #0
	mov r3, #0
	svc __FAST_SYSCALL_RPC_WFE	/* sysfast_actv_done_emrg(retval, NULL, NULL, 0UL), where the retval is returned by sys_futex_wait_simple */
