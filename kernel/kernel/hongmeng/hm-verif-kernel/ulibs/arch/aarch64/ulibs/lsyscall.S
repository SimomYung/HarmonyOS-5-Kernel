/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: lsyscall entry
 * Author: Huawei OS Kernel Lab
 * Create: Mon Apr 10 09:04:42 2023
 */
#ifndef __ASSEMBLY__
#define __ASSEMBLY__
#endif

.global arch_lsyscall_entry7
.type arch_lsyscall_entry7, %function

arch_lsyscall_entry7:
	mov	x8, x0
	mov	x0, x1
	mov	x1, x2
	mov	x2, x3
	mov	x3, x4
	mov	x4, x5
	mov	x5, x6
	mov	x6, x7
	svc	#0
	ret
.size arch_lsyscall_entry7, . - arch_lsyscall_entry7

.global arch_lsyscall_entry6
.type arch_lsyscall_entry6, %function

arch_lsyscall_entry6:
	mov	x8, x0
	mov	x0, x1
	mov	x1, x2
	mov	x2, x3
	mov	x3, x4
	mov	x4, x5
	mov	x5, x6
	svc	#0
	ret
.size arch_lsyscall_entry6, . - arch_lsyscall_entry6

.global arch_lsyscall_entry5
.type arch_lsyscall_entry5, %function

arch_lsyscall_entry5:
	mov	x8, x0
	mov	x0, x1
	mov	x1, x2
	mov	x2, x3
	mov	x3, x4
	mov	x4, x5
	svc	#0
	ret
.size arch_lsyscall_entry5, . - arch_lsyscall_entry5

.global arch_lsyscall_entry4
.type arch_lsyscall_entry4, %function

arch_lsyscall_entry4:
	mov	x8, x0
	mov	x0, x1
	mov	x1, x2
	mov	x2, x3
	mov	x3, x4
	svc	#0
	ret
.size arch_lsyscall_entry4, . - arch_lsyscall_entry4

.global arch_lsyscall_entry3
.type arch_lsyscall_entry3, %function

arch_lsyscall_entry3:
	mov	x8, x0
	mov	x0, x1
	mov	x1, x2
	mov	x2, x3
	svc	#0
	ret
.size arch_lsyscall_entry3, . - arch_lsyscall_entry3

.global arch_lsyscall_entry2
.type arch_lsyscall_entry2, %function

arch_lsyscall_entry2:
	mov	x8, x0
	mov	x0, x1
	mov	x1, x2
	svc	#0
	ret
.size arch_lsyscall_entry2, . - arch_lsyscall_entry2

.global arch_lsyscall_entry1
.type arch_lsyscall_entry1, %function

arch_lsyscall_entry1:
	mov	x8, x0
	mov	x0, x1
	svc	#0
	ret
.size arch_lsyscall_entry1, . - arch_lsyscall_entry1

.global arch_lsyscall_entry0
.type arch_lsyscall_entry0, %function

arch_lsyscall_entry0:
	mov	x8, x0
	svc	#0
	ret
.size arch_lsyscall_entry0, . - arch_lsyscall_entry0
