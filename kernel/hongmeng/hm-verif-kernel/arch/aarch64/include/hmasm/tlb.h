/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jun 13 01:24:55 2018
 */
#ifndef AARCH64_ASM_TLB_H
#define AARCH64_ASM_TLB_H

#include <hmasm/page.h>
#include <hmasm/memory.h>
#include <hmasm/barrier.h>
#include <hmkernel/types.h>
#include <hmkernel/ctrlmem/vspace.h>

#include <hmasm/ctrlmem/vspace.h>
#include <hmkernel/cpufeatures.h>

#define TLBI_VA_BITS    44
#define TLBI_VA_SHIFT   12
#define TLBI_ASID_SHIFT 48

#define TLBI_IPA_BITS   36

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

#if defined CONFIG_AARCH64_PAGE_4K
# define A64_TLBI_TTL_GRANULE	1
#elif defined CONFIG_AARCH64_PAGE_16K
# define A64_TLBI_TTL_GRANULE	2
#elif defined CONFIG_AARCH64_PAGE_64K
# define A64_TLBI_TTL_GRANULE	3
#else
# error "Impossible"
#endif
#define A64_TLBI_TTL_LEVEL_3	3
#define A64_TLBI_TTL_LEVEL_2	2
#define A64_TLBI_TTL_LEVEL_1	1

#define A64_TLBI_TTL_LEVEL_FACTOR	512

#define A64_TLBI_TTL_GRANUAL_SHIFT	2
#define A64_TLBI_TTL_SHIFT	44

static inline unsigned long __tlb_invalidate_ttl_by_size(unsigned long size)
{
	unsigned long ttl;

	if (likely(size <= PAGE_SIZE)) {
		ttl = A64_TLBI_TTL_LEVEL_3;
	} else if (size <= PAGE_SIZE * A64_TLBI_TTL_LEVEL_FACTOR) {
		ttl = A64_TLBI_TTL_LEVEL_2;
	} else {
		ttl = A64_TLBI_TTL_LEVEL_1;
	}
	return ttl;
}
/*
 * See D5-53 TTL field encodings in TLB instructions that apply to a single address
 */
static inline unsigned long tlb_invalidate_ttl_by_size(unsigned long size)
{
	unsigned long ttl = 0;
	if (arch_cpu_features_has_a64ttl() && (size != 0)) {
		ttl = __tlb_invalidate_ttl_by_size(size) |
			(A64_TLBI_TTL_GRANULE << A64_TLBI_TTL_GRANUAL_SHIFT);
		ttl <<= A64_TLBI_TTL_SHIFT;
	}
	return ttl;
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
#ifdef CONFIG_CTRLMEM_SUPERVS
	/*
	 * When unmap normal page, flush all va according to va. NO need to give
	 * asid since all asid (all supervs vspace) should do this flushing.
	 *
	 * asid == 0 is kernel unmapping.
	 */
	if (asid != 0) {
		unsigned int supervsidx = arch_ctrlmem_vspace_supervsidx_from_busyidx(a64_asid_to_busy_idx(asid));
		vaddr_t va2 = (supervsidx * __ARCH_SUPERVS_SUBSPACE_SIZE) + va;
#if (CONFIG_CTRLMEM_NR_SUPERVSS > 1)
		unsigned long op2 = MASK(va2 >> TLBI_VA_SHIFT, TLBI_VA_BITS);
		op2 |= ttl;
		asm volatile ("tlbi vaale1is, %0\n" :: "r" (op2));
#else
		/*
		 * When there is only 1 supervss, use vale1is which apply to excat supervs vspace
		 * asid. Although using tlbi vaale1is should have same result, testing result
		 * shows plenty of performance penalty when using vaale1is for unknown reason.
		 */
		u64 asid2 = a64_asid_from_busy_idx(__U(__ARCH_CTRLMEM_VSCTRL_BUSY_VSPACE_NR) - 1U);
		unsigned long op2 = MASK(va2 >> TLBI_VA_SHIFT, TLBI_VA_BITS) |
			(((unsigned long)asid2) << TLBI_ASID_SHIFT);
		op2 |= ttl;
		asm volatile ("tlbi vale1is, %0\n" :: "r" (op2));
#endif
	}
#endif
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
