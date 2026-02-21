/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Some esoteric syscalls
 * Author: Huawei OS Kernel Lab
 * Create: Tue Sep 26 20:26:44 2023
 */

#ifndef ASM_LSYSCALL_ESOTERIC_H
#define ASM_LSYSCALL_ESOTERIC_H

#ifdef CONFIG_COMPAT
#define ESOTERIC_SYS_NO_BASE	0x0f0000
#define __NR___ARM_breakpoint	0x0f0001
#define __NR___ARM_cacheflush	0x0f0002
#define __NR___ARM_usr26	0x0f0003
#define __NR___ARM_usr32	0x0f0004
#define __NR___ARM_set_tls	0x0f0005
#define __NR___ARM_get_tls	0x0f0006
#define ESOTERIC_SYS_NO_MAX	0x0f0007

#define esoteric_sys_tbl_idx(num)	(long)(((unsigned long)(num) - __UL(ESOTERIC_SYS_NO_BASE)) & 0xffffUL)
static inline bool arch_lsyscall_is_esoteric(unsigned long sc)
{
	return sc > ESOTERIC_SYS_NO_BASE && sc < ESOTERIC_SYS_NO_MAX;
}

struct tcb_s *arch_esoteric_lsyscall(unsigned long sc, struct tcb_s *tcb);
#else
#include <hmkernel/strict.h>

static inline bool arch_lsyscall_is_esoteric(unsigned long sc)
{
	UNUSED(sc);
	return false;
}
static inline struct tcb_s *
arch_esoteric_lsyscall(unsigned long sc, struct tcb_s *tcb)
{
	UNUSED(sc, tcb);
	panic("not support\n");
}
#endif
#endif
