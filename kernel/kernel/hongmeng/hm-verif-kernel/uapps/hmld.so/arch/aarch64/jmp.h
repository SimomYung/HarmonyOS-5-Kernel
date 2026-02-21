/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Wed Apr 17 10:24:58 2019
 */

#ifndef HMLDSO_ARCH_AARCH64_JMP_H
#define HMLDSO_ARCH_AARCH64_JMP_H

static inline void jmp_ventry(void *entry, void *sp)
{
	asm volatile (
		"mov  x1,  %0\n"
		"mov  sp,  %1\n"
		"br   x1"
		:
		: "r"(entry), "r"(sp)
		: "x1", "memory");
}

#endif
