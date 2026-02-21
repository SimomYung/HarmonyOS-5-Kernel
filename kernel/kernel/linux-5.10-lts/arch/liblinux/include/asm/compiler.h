/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LIBLINUX_ASM_COMPILER_H
#define __LIBLINUX_ASM_COMPILER_H
#include <asm/pointer_auth_common.h>

#define __override __section(".text.override")

/*
 * The EL0/EL1 pointer bits used by a pointer authentication code.
 * This is dependent on TBI0/TBI1 being enabled, or bits 63:56 would also apply.
 */
/* Valid for EL0 TTBR0 and EL1 TTBR1 instruction pointers */
#ifdef CONFIG_ARM64_PTR_AUTH_EXT
#define ptrauth_clear_pac(ptr)						\
        pauth_xpaci(ptr)
#else
#define ptrauth_clear_pac(ptr) ptr
#endif

#endif /* __LIBLINUX_ASM_COMPILER_H */
