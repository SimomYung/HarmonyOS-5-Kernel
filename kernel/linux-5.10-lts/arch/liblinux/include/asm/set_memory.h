/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef __LIBLINUX_ASM_SET_MEMORY_H
#define __LIBLINUX_ASM_SET_MEMORY_H

#include <asm-generic/set_memory.h>
/* for init section */
int set_memory_rx(unsigned long addr, int numpages);

#endif /* !__LIBLINUX_ASM_SET_MEMORY_H */
