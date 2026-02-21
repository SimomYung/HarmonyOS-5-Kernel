/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Arch support of supervs
 * Author: Huawei OS Kernel Lab
 * Create: Mon Aug 28 14:46:10 2023
 */

#ifndef A64_HMASM_CTRLMEM_SUPERVS_H
#define A64_HMASM_CTRLMEM_SUPERVS_H

#include <hmasm/memory.h>
#include <hmasm/sysreg.h>
#include <hmasm/barrier.h>
#include <hmasm/processor.h>
#include <hmkernel/strict.h>

struct ctrlmem_kobj_vspace_s;
struct pgcfg_s;
struct arch_uctx;
struct ctrlmem_vspace_tlbflush_dropidx_ctx_s;

#ifdef CONFIG_CTRLMEM_SUPERVS

void arch_ctrlmem_vspace_supervs_fix_init(struct arch_uctx *uctx);

JAM_INLINE void arch_ctrlmem_vspace_supervs_plugsub(paddr_t pagepa,
						    const struct ctrlmem_kobj_vspace_s *kobj_vspace,
						    unsigned int sub_busyidx);

JAM_INLINE void arch_ctrlmem_vspace_supervs_flushsub(paddr_t pagepa,
						     struct ctrlmem_vspace_tlbflush_dropidx_ctx_s *ctx,
						     unsigned int host_busyidx);

JAM_INLINE void arch_ctrlmem_vspace_supervs_unplugsub(paddr_t pagepa,
						      const struct ctrlmem_kobj_vspace_s *kobj_vspace,
						      struct ctrlmem_vspace_tlbflush_dropidx_ctx_s *ctx,
						      unsigned int host_busyidx);

JAM_INLINE void arch_ctrlmem_vspace_supervs_flushsub_done(void);

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

JAM_INLINE void arch_ctrlmem_vspace_supervs_fix_subvs(struct arch_uctx *uctx);

#else

static inline void arch_ctrlmem_vspace_supervs_fix_init(struct arch_uctx *uctx)
{
	UNUSED(uctx);
}

static inline void arch_ctrlmem_vspace_supervs_plugsub(paddr_t pagepa,
						       const struct ctrlmem_kobj_vspace_s *kobj_vspace,
						       unsigned int sub_busyidx)
{
	UNUSED(pagepa, kobj_vspace, sub_busyidx);
}

static inline void arch_ctrlmem_vspace_supervs_flushsub(paddr_t pagepa,
							struct ctrlmem_vspace_tlbflush_dropidx_ctx_s *ctx,
							unsigned int host_busyidx)
{
	UNUSED(pagepa, ctx, host_busyidx);
}

static inline void arch_ctrlmem_vspace_supervs_unplugsub(paddr_t pagepa,
							 const struct ctrlmem_kobj_vspace_s *kobj_vspace,
							 struct ctrlmem_vspace_tlbflush_dropidx_ctx_s *ctx,
							 unsigned int host_busyidx)
{
	UNUSED(pagepa, kobj_vspace, ctx, host_busyidx);
}

static inline void arch_ctrlmem_vspace_supervs_flushsub_done(void)
{
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

static inline void arch_ctrlmem_vspace_supervs_fix_subvs(struct arch_uctx *uctx)
{
	UNUSED(uctx);
}

#endif

#endif
