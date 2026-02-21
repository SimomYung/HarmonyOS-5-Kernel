/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * based on arch/arm64/include/asm/module.h
 */
#ifndef __LIBLINUX_ASM_MODULE_H
#define __LIBLINUX_ASM_MODULE_H

#include <asm-generic/module.h>
#include <linux/version.h>

#ifdef CONFIG_ARM64_MODULE_PLTS
struct mod_plt_sec {
	int			plt_shndx;
	int			plt_num_entries;
	int			plt_max_entries;
};

struct mod_arch_specific {
	struct mod_plt_sec	core;
	struct mod_plt_sec	init;

	/* for CONFIG_DYNAMIC_FTRACE */
	struct plt_entry	*ftrace_trampolines;
};
#endif

u64 module_emit_plt_entry(struct module *mod, Elf64_Shdr *sechdrs,
			  void *loc, const Elf64_Rela *rela,
			  Elf64_Sym *sym);

u64 module_emit_veneer_for_adrp(struct module *mod, Elf64_Shdr *sechdrs,
				void *loc, u64 val);

#define module_alloc_base ((u64)__liblinux_modules_pool)

struct plt_entry {
	/*
	 * A program that conforms to the AArch64 Procedure Call Standard
	 * (AAPCS64) must assume that a veneer that alters IP0 (x16) and/or
	 * IP1 (x17) may be inserted at any branch instruction that is
	 * exposed to a relocation that supports long branches. Since that
	 * is exactly what we are dealing with here, we are free to use x16
	 * as a scratch register in the PLT veneers.
	 */
	union {
		__le32	mov0;	/* movn x16, #0x....			*/
		__le32	adrp;	/* fake for arch/arm64/module-plt.c	*/
	};
	union {
		__le32	mov1;	/* movk	x16, #0x...., lsl #16		*/
		__le32	add;	/* fake for arch/arm64/module-plt.c	*/
	};
	__le32	mov2;	/* movk	x16, #0x...., lsl #32		*/
	__le32	br;	/* br	x16				*/
};

static inline bool is_forbidden_offset_for_adrp(void *place)
{
	return IS_ENABLED(CONFIG_ARM64_ERRATUM_843419) &&
	       cpus_have_const_cap(ARM64_WORKAROUND_843419) &&
	       ((u64)place & 0xfff) >= 0xff8;
}

struct plt_entry get_plt_entry(u64 dst, void *pc);
bool plt_entries_equal(const struct plt_entry *a, const struct plt_entry *b);

#endif /* __LIBLINUX_ASM_MODULE_H */
