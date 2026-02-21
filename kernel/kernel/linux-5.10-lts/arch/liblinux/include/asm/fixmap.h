/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef __LIBLINUX_ASM_FIXMAP_H
#define __LIBLINUX_ASM_FIXMAP_H

#include <linux/kernel.h>
#include <linux/sizes.h>
#include <linux/pgtable.h>
#include <asm/page.h>

enum fixed_addresses {
	FIX_HOLE,
	FIX_TEXT_POKE0,
	__end_of_permanent_fixed_addresses,
	__end_of_fixed_addresses
};

#define FIXMAP_PAGE_IO		PAGE_KERNEL

#define __early_set_fixmap	__set_fixmap

#define __late_set_fixmap	__set_fixmap
#define __late_clear_fixmap(idx) __set_fixmap((idx), 0, FIXMAP_PAGE_CLEAR)

extern void __set_fixmap(enum fixed_addresses idx,
			 phys_addr_t phys, pgprot_t prot);

#include <asm-generic/fixmap.h>

#endif /* __LIBLINUX_ASM_FIXMAP_H */
