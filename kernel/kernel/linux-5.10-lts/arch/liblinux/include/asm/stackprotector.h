/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef __LIBLINUX_ASM_STACKPROTECTOR_H
#define __LIBLINUX_ASM_STACKPROTECTOR_H

#include <linux/compiler.h>

extern unsigned long __stack_chk_guard;

static __always_inline void boot_init_stack_canary(void)
{
	/* get canary the way as the hmlibc */
	__stack_chk_guard = (unsigned long)&__stack_chk_guard * 1103515245;
}

#endif /* __LIBLINUX_ASM_STACKPROTECTOR_H */
