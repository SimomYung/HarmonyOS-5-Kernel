/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef __LIBLINUX_ASM_SECTIONS_H
#define __LIBLINUX_ASM_SECTIONS_H

#include <asm-generic/sections.h>

extern char _text[], _stext[], _etext[];
extern char _data[], _sdata[], _edata[];
extern char __initdata_begin[], __initdata_end[];
extern char __inittext_begin[], __inittext_end[];
extern char __exittext_begin[], __exittext_end[];

/* reserved region for vendor hook init */
typedef void (*inithook_t)(void);
extern inithook_t __vendor_hook_initcall_start[], __vendor_hook_initcall_end[];

/* reserved region for modules */
extern char __liblinux_modules_pool[];
extern char __liblinux_modules_pool_end[];

#endif /* __LIBLINUX_ASM_SECTIONS_H */
