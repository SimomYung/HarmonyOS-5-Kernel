/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef __LIBILNUX_ASM_CACHE_H
#define __LIBILNUX_ASM_CACHE_H

#include <asm/cputype.h>

#ifdef CONFIG_ARM64
#define L1_CACHE_SHIFT         (6)
#else
#error un-support arch
#endif
#define L1_CACHE_BYTES         (1 << L1_CACHE_SHIFT)

#ifdef CONFIG_ARCH_HAS_CACHE_LINE_SIZE
#define cache_line_size()	L1_CACHE_BYTES
#endif

#ifndef __ASSEMBLY__
static inline int cache_line_size_of_cpu(void)
{
	return cache_line_size();
}
#endif

#define ARCH_DMA_MINALIGN	L1_CACHE_BYTES

#endif /* __LIBLINUX_ASM_CACHE_H */
