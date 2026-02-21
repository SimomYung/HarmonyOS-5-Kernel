/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef __LIBLINUX_ASM_ASSEMBLER_ARM64_h
#define __LIBLINUX_ASM_ASSEMBLER_ARM64_h

#ifndef CONFIG_ARM64
# error "support ARM64 only"
#endif

#define cond_yield ____cond_yield
#include_next <asm/assembler.h>
#undef cond_yield

.macro		cond_yield, lbl:req, tmp:req, tmp2:req
.endm

#endif
