/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2022. All rights reserved.
 * Description: arm64 ctrlmem vspace
 * Author: Huawei OS Kernel Lab
 * Create: Wed Sep 08 00:30:08 2021
 */

#ifndef AARCH64_ASM_CTRLMEM_VSPACE_H
#define AARCH64_ASM_CTRLMEM_VSPACE_H

#include <mapi/hmasm/ctrlmem/vspace.h>

#include <hmkernel/ctrlmem/vspace/tlbflush.h>
#include <hmkernel/panic.h>
#include <hmkernel/const.h>

#include <hmasm/pgtbl/pgtblattr.h>
#include <hmasm/processor.h>
#include <hmasm/barrier.h>

#define __A64_ASID_RESERVED	8u

static inline unsigned int a64_asid_from_busy_idx(unsigned int idx)
{
	/*
	 * Remove this BUG_ON is possible to reduce 2 instructions
	 * in vspace_switch hotpath but it makes gdb unable to do
	 * 'si' correctly. Don't know the reason...
	 */
	BUG_ON(idx >= __ARCH_CTRLMEM_VSCTRL_BUSY_VSPACE_NR);
	return __A64_ASID_RESERVED + idx;
}

static inline unsigned int a64_asid_to_busy_idx(unsigned int asid)
{
	BUG_ON(asid < __A64_ASID_RESERVED);
	return asid - __A64_ASID_RESERVED;
}

/*
 * define as macro for static assert
 */
#define __a64_asid_cpu_reserved(cpuid)	(__A64_ASID_RESERVED + __U(__ARCH_CTRLMEM_VSCTRL_BUSY_VSPACE_NR) + (cpuid))

static inline unsigned int a64_asid_cpu_reserved(unsigned int cpuid)
{
	return __a64_asid_cpu_reserved(cpuid);
}

struct ctrlmem_vspace_slot_s;
struct ctrlmem_vspace_tlbflush_dropidx_ctx_s;
extern void arch_ctrlmem_vspace_flush_tlb_idx(struct ctrlmem_vspace_tlbflush_dropidx_ctx_s *ctx);

extern void arch_ctrlmem_vspace_flush_tlb(const struct ctrlmem_vspace_slot_s *slot,
					  struct ctrlmem_kobj_vspace_s *kobjvs,
					  const void __user *va);

static inline unsigned int arch_ctrlmem_vspace_curr_busy_idx_un(void)
{
	u64 ttbr0 = read_sysreg("ttbr0_el1");
	unsigned int asid = TTBR_ASID_extract(ttbr0);
	return (asid >= __A64_ASID_RESERVED) ? a64_asid_to_busy_idx(asid) : __ARCH_CTRLMEM_VSCTRL_BUSY_VSPACE_NR;
}

unsigned int __hot arch_ctrlmem_vspace_slot_to_asid(const struct ctrlmem_vspace_slot_s *slot);

/* Deprecated */
static inline void arch_ctrlmem_vspace_flush_tlb_prepare(struct ctrlmem_vspace_tlbflush_dropidx_ctx_s *ctx)
{
	if (!ctx->dsb_done) {
		dsb(ishst);
		ctx->dsb_done = true;
	}
}

/* Deprecated */
extern void __hot arch_ctrlmem_vspace_flush_tlb_invalidate(vaddr_t va,
							   u64 asid);

/* Deprecated */
static inline void arch_ctrlmem_vspace_flush_tlb_commit(void)
{
	dsb(ish);
	isb();
}

/*
 * In case no PRIVILEVED_SERVICE setting, when vspace_switch,
 * there is only 0 (nothing to do) and 3 (replace non-privileged vspace)
 * can appear.
 */
#ifndef CONFIG_PRIVILEGED_SERVICE
# define ARCH_CTRLMEM_VSPACE_ONLINE_FREQUENT_ACTIONS	0, 3
#else
/*
 * In case PRIVILEVED_SERVICE is set, when vspace_switch,
 * the most frequent case is 0 (nothing todo), 1 (change TTBR0 from 0 to non-0)
 * and 2 (change TTBR0 from non-0 to 0). These could happen when normal
 * user app invoke privileged service and return from privileged serivce.
 */
#ifdef CONFIG_SPINHDLR_ENABLED
# define ARCH_CTRLMEM_VSPACE_ONLINE_FREQUENT_ACTIONS	0, 3
#else
# define ARCH_CTRLMEM_VSPACE_ONLINE_FREQUENT_ACTIONS	0, 1, 2
#endif
#endif

/* The number of reserved vspace slots which can not be used for supervs uapps. */
#ifdef CONFIG_CTRLMEM_SUPERVS
#define ARCH_CTRLMEM_VSCTRL_NR_NOSUPERVS_UAPPS		(__ARCH_CTRLMEM_VSCTRL_NR_NOSUPERVS_FACTOR * CONFIG_NR_CPUS)
#else
#define ARCH_CTRLMEM_VSCTRL_NR_NOSUPERVS_UAPPS		0
#endif

#endif
