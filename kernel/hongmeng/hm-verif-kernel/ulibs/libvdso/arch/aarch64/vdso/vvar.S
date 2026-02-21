/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Sat Oct 26 15:37:02 2019
 */

.section ".text", "ax"

.global vdso_get_data
.type vdso_get_data, %function
vdso_get_data:
	adrp x0, 1f
	add x0, x0, #:lo12:1f
#ifdef __ILP32__
	ldr w9, [x0]
#else
	ldr x9, [x0]
#endif
	add x0, x0, x9
	ret

/* Local alias */
.global __vdso_get_data
.set __vdso_get_data, vdso_get_data

.section .rodata
.hidden _vdso_data
#ifdef __ILP32__
1: .word _vdso_data-1b
#else
1: .quad _vdso_data-1b
#endif
