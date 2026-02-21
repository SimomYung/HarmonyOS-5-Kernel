/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2024. All rights reserved.
 * Description: cache
 * Author: Huawei OS Kernel Lab
 * Create: Sat Jun 23 09:19:38 2018
 */
#ifndef AARCH64_HMASM_CACHE_H
#define AARCH64_HMASM_CACHE_H

#include <hmasm/memory.h>
#include <hmasm/cpu/cpuinfo.h>
#include <uapi/hmkernel/cache.h>
#include <hmkernel/bitops/bits.h>
#include <hmkernel/errno.h>
#include <hmkernel/compiler.h>

#define CTR_IDC_SHIFT		28
#define CLIDR_LOUU(clidr)	(((clidr) >> CLIDR_LOUU_SHIFT) & 0x7)
#define CLIDR_LOC(clidr)	(((clidr) >> CLIDR_LOC_SHIFT) & 0x7)
#define CLIDR_LOUIS(clidr)	(((clidr) >> CLIDR_LOUIS_SHIFT) & 0x7)

extern void flush_dcache_all(void);
extern void clean_dcache_all(void);
extern int flush_cache_range(vaddr_t vstart, vaddr_t vend);
extern int flush_dcache_range(vaddr_t start, vaddr_t end);
extern int clean_dcache_range(vaddr_t start, vaddr_t end);
extern int clean_dcache_pou_range(vaddr_t start, vaddr_t end);
extern int inval_dcache_range(vaddr_t start, vaddr_t end);
extern int flush_icache_range(vaddr_t start, vaddr_t end);
extern void flush_cache_all_smp(void);
extern void flush_icache_all_smp(void);
extern void flush_cache_all_non_smp(void);
extern void flush_icache_all_non_smp(void);
extern void inval_cache_all_non_smp(void);

#ifdef CONFIG_SMP
#define flush_cache_all flush_cache_all_smp
#define flush_icache_all flush_icache_all_smp
#else
#define flush_cache_all flush_cache_all_non_smp
#define flush_icache_all flush_icache_all_non_smp
#endif

/*
 * Read the effective value of CTR_EL0.
 *
 * IDC,bit[28]:
 * Data cache clean requirements for instruction to data coherence.
 *
 * The value of this field is an IMPLEMENTATION DEFINED choice of:
 * 0b0 Data cache clean to the Point of Unification is required for instruction to data coherence,
 * unless CLIDR_EL1.LoC == 0b000 or (CLIDR_EL1.LoUIS == 0b000 and CLIDR_EL1.LoUU == 0b000).
 * 0b1 Data cache clean to the Point of Unification is not required for instruction to data coherence.
 *
 * If CTR_EL0.DIC is 1 then the value reported in this field must also be 1.
 *
 * The Effective value of IDC is 1 if any of the following are true:
 * - CTR_EL0.IDC == 1.
 * - CLIDR_EL1.LoC == 0b000.
 * - CLIDR_EL1.LoUIS == 0b000 and CLIDR_EL1.LoUU == 0b000.
 *
 * All PEs in the same Inner Shareable shareability domain must have a common Effective value of IDC.
 *
 * This routine provides the CTR_EL0 with the IDC field updated to the
 * effective state.
 */
static inline u32 __const read_cpuid_effective_cachetype(void)
{
	u32 ctr = read_cpuid_cachetype();
	if (!(ctr & BIT(CTR_IDC_SHIFT))) {
		u64 clidr = read_sysreg("clidr_el1");

		if (CLIDR_LOC(clidr) == 0 ||
		    (CLIDR_LOUIS(clidr) == 0 && CLIDR_LOUU(clidr) == 0))
			ctr |= BIT(CTR_IDC_SHIFT);
	}

	return ctr;
}

#endif
