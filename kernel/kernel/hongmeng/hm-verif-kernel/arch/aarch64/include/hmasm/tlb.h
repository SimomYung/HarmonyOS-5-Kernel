/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jun 13 01:24:55 2018
 */
#ifndef AARCH64_ASM_TLB_H
#define AARCH64_ASM_TLB_H

#include <hmasm/memory.h>
#include <hmasm/tlbttl.h>
#include <hmasm/tlbconsts.h>
#include <hmasm/barrier.h>
#include <hmkernel/types.h>
#include <hmkernel/ctrlmem/vspace.h>

#include <hmasm/ctrlmem/vspace.h>
#include <hmkernel/cpufeatures.h>

#define tlbi(opt)	asm volatile ("tlbi " #opt "\n");

static inline void tlb_flush_all(void)
{
	dsb(ishst);
	asm volatile ("tlbi vmalle1is\n");
	dsb(ish);
	isb();
}

static inline void tlb_flush_asid(u64 asid)
{
	dsb(ishst);
	asm volatile ("tlbi aside1is, %0\n" :: "r" (((unsigned long)asid) << TLBI_ASID_SHIFT));
	dsb(ish);
	isb();
}

static inline void
tlb_invalidate_all_asid(vaddr_t va, unsigned long size)
{
	unsigned long op = MASK(((unsigned long)va) >> TLBI_VA_SHIFT, TLBI_VA_BITS);
	unsigned long ttl = tlb_invalidate_ttl_by_size(size);
	op |= ttl;
	asm volatile ("tlbi vaale1is, %0\n" :: "r" (op));
}

static inline void
tlb_invalidate_by_asid(vaddr_t va, u64 asid, unsigned long size)
{
	unsigned long op = MASK(((unsigned long)va) >> TLBI_VA_SHIFT, TLBI_VA_BITS) |
		(((unsigned long)asid) << TLBI_ASID_SHIFT);
	unsigned long ttl = tlb_invalidate_ttl_by_size(size);

	op |= ttl;
	/*
	 * since this is unmap_page, always last level.
	 * vae1is should be used when unplugging a mid-level page table.
	 */
	asm volatile ("tlbi vale1is, %0\n" :: "r" (op));
}

static inline void
tlb_flush_unmap_page(vaddr_t va, u64 asid)
{
	dsb(ishst);
	tlb_invalidate_by_asid(va, asid, 0UL);
	dsb(ish);
	isb();
}

static inline void
tlb_flush_unmap_page_all_asid(vaddr_t va)
{
	dsb(ishst);
	tlb_invalidate_all_asid(va, 0UL);
	dsb(ish);
	isb();
}

static inline void tlb_flush_vmid_ipa_nobarrier(vaddr_t ipa)
{
	/*
	 * ipas2e1is instruction operation:
	 * |63 <-----> 40|39 <------> 36|35 <---> 0|
	 *       res0     IPA extension   IPA[47-12]
	 *  IPA extension: is reserved in ARMv8.1 and ARMv8.0
	 */
	u64 op = MASK((ipa) >> TLBI_VA_SHIFT, TLBI_IPA_BITS);

	/*
	 * ipas2e1is should be used when unplugging a mid-level page table.
	 */
	asm volatile ("tlbi ipas2le1is, %0\n" :: "r" (op));
}

static inline void tlb_flush_vmid_ipa_barrier(void)
{
	dsb(ish);
	/*
	 * NOTE: Without the VA of the IPA, we should flush the entire TLB
	 * at stage 1 by VMID.
	 */
	tlbi(vmalle1is);
	dsb(ish);
	isb();
}

struct ctrlmem_kobj_vspace_s;
#ifdef CONFIG_HYPERVISOR
extern bool tlb_flush_is_hyp(struct ctrlmem_kobj_vspace_s *kobj_vspace);
extern bool tlb_flush_try_hyp(struct ctrlmem_kobj_vspace_s *kobj_vspace,
			     const void __user *va,
			     unsigned long size);
extern void tlb_flush_hyp_done(struct ctrlmem_kobj_vspace_s *kobj_vspace);
static inline void tlb_flush_s12_vmid_nobarrier(void)
{
	/* flush the entire TLB at stage 1 and 2 by VMID. */
	asm volatile ("tlbi vmalls12e1is");
}
#else
static inline bool tlb_flush_is_hyp(struct ctrlmem_kobj_vspace_s *kobj_vspace)
{
	UNUSED(kobj_vspace);
	return false;
}
static inline bool tlb_flush_try_hyp(struct ctrlmem_kobj_vspace_s *kobj_vspace,
				    const void __user *va,
				    unsigned long size)
{
	UNUSED(kobj_vspace, va, size);
	return false;
}
static inline void tlb_flush_hyp_done(struct ctrlmem_kobj_vspace_s *kobj_vspace)
{
	UNUSED(kobj_vspace);
}
static inline void tlb_flush_s12_vmid_nobarrier(void)
{
	return;
}
#endif /* CONFIG_HYPERVISOR */
#endif
