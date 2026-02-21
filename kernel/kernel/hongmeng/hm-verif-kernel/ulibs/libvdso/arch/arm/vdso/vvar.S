/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Sat Oct 26 15:37:02 2019
 */

.section ".text", "ax"

.global vdso_get_data
.type vdso_get_data, %function
vdso_get_data:
	push {r5, r6}
	adr r5, 1f
	ldr r6, 1f
	add r6, r6, r5
	mov r0, r6
	pop {r5, r6}
	bx lr

/* Local alias */
.global __vdso_get_data
.set __vdso_get_data, vdso_get_data

.hidden _vdso_data
1: .word _vdso_data-1b
