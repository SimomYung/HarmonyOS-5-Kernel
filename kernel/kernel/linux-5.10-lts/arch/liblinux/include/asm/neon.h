/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef __LIBLINUX_ASM_NEON_H
#define __LIBLINUX_ASM_NEON_H

#if !defined(CONFIG_ARM64) && !defined(CONFIG_ARM)
# error "liblinux: NEON support only valid on ARM/ARM64 platforms"
#endif

void kernel_neon_begin(void);
void kernel_neon_end(void);

#endif
