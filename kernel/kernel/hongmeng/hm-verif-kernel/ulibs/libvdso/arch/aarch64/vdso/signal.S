/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: Signal trampoline
 * Author: Huawei OS Kernel Lab
 * Create: Thu Feb 29 08:59:59 2024
 */

#include <syscall.h>

.global vdso_sigtramp1
.type vdso_sigtramp1, %function
vdso_sigtramp1:
	mov sp, x0
	/* Stack frame is now compatible with rt_sigframe. */
	bl  vdso_sigtramp2
.size vdso_sigtramp1, . - vdso_sigtramp1

.global __kernel_rt_sigreturn
.type __kernel_rt_sigreturn, %function
__kernel_rt_sigreturn:
	/*
	 * Function vdso_sigtramp2 does not return. The following instruction
	 * sequence is used as a hint of signal stack frame for unwinders.
	 */
	mov x8, #__NR_rt_sigreturn
	svc #0
.size __kernel_rt_sigreturn, . - __kernel_rt_sigreturn
