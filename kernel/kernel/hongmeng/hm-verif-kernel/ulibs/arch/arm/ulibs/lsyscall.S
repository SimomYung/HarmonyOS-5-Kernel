 /*
  * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
  * Description: lsyscall entry
  * Author: Huawei OS Kernel Lab
  * Create: Mon Apr 10 08:57:33 2023
  */

#ifndef __ASSEMBLY__
#define __ASSEMBLY__
#endif

.global arch_lsyscall_entry
.type arch_lsyscall_entry, %function

arch_lsyscall_entry:
	.cfi_startproc
	mov     ip, sp
	stmfd   sp!, {r4, r5, r6, r7}
	.cfi_def_cfa_offset 16
	.cfi_rel_offset r4, 0
	.cfi_rel_offset r5, 4
	.cfi_rel_offset r6, 8
	.cfi_rel_offset r7, 12
	mov     r7, r0
	mov     r0, r1
	mov     r1, r2
	mov     r2, r3
	ldmfd   ip, {r3, r4, r5, r6}
	svc	#0
	ldmfd   sp!, {r4, r5, r6, r7}
	.cfi_def_cfa_offset 0
	bx		lr
	.cfi_endproc
.size arch_lsyscall_entry, . - arch_lsyscall_entry
