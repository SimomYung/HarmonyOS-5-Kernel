/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef __LIBLINUX_ASM_SIMD_H
#define __LIBLINUX_ASM_SIMD_H

#include <linux/compiler.h>
#include <linux/preempt.h>

static __must_check inline bool may_use_simd(void)
{
	return IS_ENABLED(CONFIG_LIBLINUX_KERNEL_MODE_SIMD) && !in_interrupt();
}

#endif /* __LIBLINUX_ASM_SIMD_H */
