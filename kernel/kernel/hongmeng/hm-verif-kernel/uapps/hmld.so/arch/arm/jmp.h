/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Implementation of jmp_ventry
 * Author: Huawei OS Kernel Lab
 * Create: Wed Apr 17 10:24:58 2019
 */

#ifndef HMLDSO_ARCH_AARCH64_JMP_H
#define HMLDSO_ARCH_AARCH64_JMP_H

static inline void jmp_ventry(void *entry, void *sp)
{
	asm volatile (
		"mov  r1,  %0\n"
		"mov  sp,  %1\n"
		"bx   r1"
		:
		: "r"(entry), "r"(sp)
		: "r1", "memory");
}

#endif
