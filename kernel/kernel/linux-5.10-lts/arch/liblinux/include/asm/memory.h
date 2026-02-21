/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef __LIBLINUX_ASM_MEMORY_H
#define __LIBLINUX_ASM_MEMORY_H

#if !defined(CONFIG_ARM64) && !defined(CONFIG_ARM)
# error "this wrapper header should only be used by arm/arm64"
#endif

/*
 * stub wrapper for include by arm/arm64 asm headers
 */

#include <linux/sizes.h>
#include <asm/page.h>

#define IRQ_STACK_SIZE		THREAD_SIZE
#define OVERFLOW_STACK_SIZE	SZ_4K

#define PHYS_MASK_SHIFT		(CONFIG_ARM64_PA_BITS)
#define PHYS_MASK		((UL(1) << PHYS_MASK_SHIFT) - 1)

#ifdef CONFIG_KASAN
#define KASAN_SHADOW_OFFSET	_AC(CONFIG_KASAN_SHADOW_OFFSET, UL)
#endif

#ifndef __ASSEMBLY__
#include <asm/io.h>	/* for virt_to_phys() */
#include <platform_include/see/prmem_defs.h>
#endif

#define pfn_to_kaddr(pfn)      __va((pfn) << PAGE_SHIFT)

#endif /* __LIBLINUX_ASM_MEMORY_H */
