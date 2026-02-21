/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef __LIBLINUX_ASM_TLBFLUSH_H
#define __LIBLINUX_ASM_TLBFLUSH_H

#ifndef __ASSEMBLY__

#include <linux/mm_types.h>
#include <linux/semaphore.h>

static inline void local_flush_tlb_all(void) {}
static inline void local_flush_tlb_page(unsigned long addr) {}
static inline void flush_tlb_all(void) {}
static inline void flush_tlb_mm(struct mm_struct *mm) {}
static inline void flush_tlb_page(struct vm_area_struct *vma, unsigned long uaddr) {}
static inline void flush_tlb_range(struct vm_area_struct *vma, unsigned long start, unsigned long end) {}
static inline void flush_tlb_kernel_range(unsigned long start, unsigned long end) {}

#endif /* !__ASSEMBLY__ */

#endif /* __LIBILNUX_ASM_TLBFLUSH_H */
