/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Based on arch/riscv/include/asm/tlb.h
 */
#ifndef __LIBLINUX_ASM_TLB_H
#define __LIBLINUX_ASM_TLB_H

struct mmu_gather;

static void tlb_flush(struct mmu_gather *tlb);

#define tlb_flush tlb_flush
#include <asm-generic/tlb.h>

static inline void tlb_flush(struct mmu_gather *tlb)
{
	flush_tlb_mm(tlb->mm);
}

#endif /* __LIBLINUX_ASM_TLB_H */
