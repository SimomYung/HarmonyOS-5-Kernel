/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Arch support of supervs
 * Author: Huawei OS Kernel Lab
 * Create: Mon Aug 28 14:46:10 2023
 */

#ifndef A64_HMASM_CTRLMEM_SUPERVS_H
#define A64_HMASM_CTRLMEM_SUPERVS_H

#include <mapi/hmasm/ctrlmem/supervs.h>

#include <hmasm/memory.h>
#include <hmasm/sysreg.h>
#include <hmasm/barrier.h>
#include <hmasm/processor.h>
#include <hmkernel/strict.h>
#include <mapi/hmasm/types.h>

struct ctrlmem_kobj_vspace_s;
struct pgcfg_s;
struct arch_uctx;
struct ctrlmem_vspace_tlbflush_dropidx_ctx_s;

#ifdef CONFIG_CTRLMEM_SUPERVS

void arch_ctrlmem_vspace_supervs_fix_init(struct arch_uctx *uctx);

u64 arch_ctrlmem_vspace_supervs_fix_ttbr0(u64 orig_ttbr0v);

JAM_INLINE void __arch_ctrlmem_vspace_supervs_plugsub(paddr_t pagepa, paddr_t subpa,
						      unsigned int slotidx, bool aptable_ro, bool aptable_pr);

JAM_INLINE void arch_ctrlmem_vspace_supervs_set_aptable_ro_bit(paddr_t pagepa, unsigned int slotidx);

JAM_INLINE void arch_ctrlmem_vspace_supervs_plugsub(paddr_t pagepa,
						    const struct ctrlmem_kobj_vspace_s *kobj_vspace,
						    unsigned int supervsidx);

JAM_INLINE void arch_ctrlmem_vspace_supervs_unplugsub(paddr_t pagepa,
						      struct ctrlmem_vspace_tlbflush_dropidx_ctx_s *ctx);

JAM_INLINE void arch_ctrlmem_vspace_supervs_unplugsub_done(void);

void arch_ctrlmem_vspace_supervs_bind(paddr_t pagepa,
				      const struct ctrlmem_kobj_vspace_s *kobj_vspace);

void arch_ctrlmem_vspace_supervs_unbind(paddr_t pagepa);

static inline unsigned long arch_ctrlmem_vspace_supervs_enable(void)
{
	u64 tcr_old = read_sysreg("tcr_el1");
	u64 tcr_new = (tcr_old & ~(u64)(ULL(TCR_T0SZ_MASK))) |
			TCR_T0SZ(CONFIG_AARCH64_SUPERVS_VA_BITS);
	write_sysreg("tcr_el1", tcr_new);
	isb();
	return tcr_old;
}

static inline unsigned long arch_ctrlmem_vspace_supervs_disable(void)
{
	u64 tcr_old = read_sysreg("tcr_el1");
	u64 tcr_new = (tcr_old & ~(u64)(ULL(TCR_T0SZ_MASK))) |
			TCR_T0SZ(CONFIG_AARCH64_VA_BITS);
	write_sysreg("tcr_el1", tcr_new);
	isb();
	return tcr_old;
}

static inline void arch_ctrlmem_vspace_supervs_restore(unsigned long flags)
{
	write_sysreg("tcr_el1", flags);
	isb();
}

/*
 * Don't want to instroduce pgtblhdlr in this header, implement it in c file.
 */
void arch_ctrlmem_vspace_supervs_fix_pgcfg(struct pgcfg_s *cfg);

JAM_INLINE unsigned long arch_ctrlmem_vspace_supervs_fix_rpc_info(const struct arch_uctx *uctx,
								  const struct arch_uctx *caller_uctx,
								  unsigned long rpc_info);

JAM_INLINE void __arch_ctrlmem_vspace_supervs_tlbflush_fixup(
		unsigned int supervsidx, vaddr_t va, unsigned long size);

struct ctrlmem_kobj_vspace_rangeop_tlbflush_ctx_s;
JAM_INLINE void arch_ctrlmem_vspace_supervs_tlbflush_fixup(
		struct ctrlmem_kobj_vspace_rangeop_tlbflush_ctx_s *ctx,
		vaddr_t va, unsigned long size);

JAM_INLINE void arch_ctrlmem_vspace_supervs_tlbflush_dropidx_fixup(
		struct ctrlmem_vspace_tlbflush_dropidx_ctx_s *ctx);

#else

static inline void arch_ctrlmem_vspace_supervs_fix_init(struct arch_uctx *uctx)
{
	UNUSED(uctx);
}

static inline void __arch_ctrlmem_vspace_supervs_plugsub( paddr_t pagepa,
							  paddr_t subpa,
							  unsigned int slotidx,
							  bool aptable_ro,
							  bool aptable_pr)
{
	UNUSED(pagepa, subpa, slotidx, aptable_ro);
}

static inline void arch_ctrlmem_vspace_supervs_set_aptable_ro_bit(paddr_t pagepa,
								  unsigned int slotidx)
{
	UNUSED(pagepa, slotidx);
}

static inline void arch_ctrlmem_vspace_supervs_plugsub(paddr_t pagepa,
						       const struct ctrlmem_kobj_vspace_s *kobj_vspace,
						       unsigned int supervsidx)
{
	UNUSED(pagepa, kobj_vspace, supervsidx);
}

static inline void arch_ctrlmem_vspace_supervs_unplugsub(paddr_t pagepa,
							struct ctrlmem_vspace_tlbflush_dropidx_ctx_s *ctx)
{
	UNUSED(pagepa, ctx);
}

static inline void arch_ctrlmem_vspace_supervs_unplugsub_done(void)
{
}

static inline void arch_ctrlmem_vspace_supervs_bind(paddr_t pagepa,
						    const struct ctrlmem_kobj_vspace_s *kobj_vspace)
{
	UNUSED(pagepa, kobj_vspace);
}

static inline void arch_ctrlmem_vspace_supervs_unbind(paddr_t pagepa)
{
	UNUSED(pagepa);
}

static inline unsigned long arch_ctrlmem_vspace_supervs_enable(void)
{
	return 0UL;
}

static inline unsigned long arch_ctrlmem_vspace_supervs_disable(void)
{
	return 0UL;
}

static inline void arch_ctrlmem_vspace_supervs_restore(unsigned long flags)
{
	UNUSED(flags);
}

static inline void arch_ctrlmem_vspace_supervs_fix_pgcfg(struct pgcfg_s *cfg)
{
	UNUSED(cfg);
}

static inline unsigned long arch_ctrlmem_vspace_supervs_fix_rpc_info(const struct arch_uctx *uctx,
								     const struct arch_uctx *caller_uctx,
								     unsigned long rpc_info)
{
	UNUSED(uctx, caller_uctx);
	return rpc_info;
}

static inline void __arch_ctrlmem_vspace_supervs_tlbflush_fixup(
		__maybe_unused unsigned int supervsidx,
		__maybe_unused vaddr_t va,
		__maybe_unused unsigned long size)
{
	UNUSED(supervsidx, va, size);
}

struct ctrlmem_kobj_vspace_rangeop_tlbflush_ctx_s;
static inline void arch_ctrlmem_vspace_supervs_tlbflush_fixup(
		__maybe_unused struct ctrlmem_kobj_vspace_rangeop_tlbflush_ctx_s *ctx,
		__maybe_unused vaddr_t va,
		__maybe_unused unsigned long size)
{
	UNUSED(ctx, va, size);
}

static inline void arch_ctrlmem_vspace_supervs_tlbflush_dropidx_fixup(
		__maybe_unused const struct ctrlmem_vspace_tlbflush_dropidx_ctx_s *ctx)
{
	UNUSED(ctx);
}

#endif

#endif
