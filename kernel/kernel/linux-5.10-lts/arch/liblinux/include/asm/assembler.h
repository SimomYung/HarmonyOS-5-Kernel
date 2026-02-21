/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef __LIBLINUX_ASM_ASSEMBLER_H
#define __LIBLINUX_ASM_ASSEMBLER_H

#ifdef CONFIG_ARM64
#include <asm/assembler-arm64.h>
#else
# error "un-support arch"
#endif

#endif
