/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: cpuinfo
 * Author: Huawei OS Kernel Lab
 * Create: Sat Aug 17 11:41:15 2024
 */
#ifndef AARCH64_HMASM_CPU_CPUINFO_H
#define AARCH64_HMASM_CPU_CPUINFO_H

#include <hmasm/types.h>

struct aarch32_cpuinfo_s {
	u32 reg_id_pfr0_el1;
	u32 reg_id_pfr1_el1;
	u32 reg_id_pfr2_el1;

	u32 reg_id_dfr0_el1;
	u32 reg_id_dfr1_el1;

	u32 reg_id_mmfr0_el1;
	u32 reg_id_mmfr1_el1;
	u32 reg_id_mmfr2_el1;
	u32 reg_id_mmfr3_el1;
	u32 reg_id_mmfr4_el1;
	u32 reg_id_mmfr5_el1;

	u32 reg_id_isar0_el1;
	u32 reg_id_isar1_el1;
	u32 reg_id_isar2_el1;
	u32 reg_id_isar3_el1;
	u32 reg_id_isar4_el1;
	u32 reg_id_isar5_el1;
	u32 reg_id_isar6_el1;

	u32 reg_mvfr0_el1;
	u32 reg_mvfr1_el1;
	u32 reg_mvfr2_el1;
};

struct aarch64_cpuinfo_s {
	u64 reg_ctr_el0;
	u64 reg_cntfrq_el0;
	u64 reg_dczid_el0;
	u64 reg_midr_el1;
	u64 reg_revidr_el1;

	u64 reg_id_aa64pfr0_el1;
	u64 reg_id_aa64pfr1_el1;
	u64 reg_id_aa64zfr0_el1;

	u64 reg_id_aa64dfr0_el1;
	u64 reg_id_aa64dfr1_el1;

	u64 reg_id_aa64isar0_el1;
	u64 reg_id_aa64isar1_el1;

	u64 reg_id_aa64mmfr0_el1;
	u64 reg_id_aa64mmfr1_el1;
	u64 reg_id_aa64mmfr2_el1;

	struct aarch32_cpuinfo_s aarch32;

	/* pseudo-ZCR for recording maximum ZCR_EL1 LEN value */
	u64 reg_zcr_el1;
};

void arch_cpuinfo_store_boot_cpu(void);
void arch_cpuinfo_store_cpu(unsigned int cpuid);

#ifndef __ASSEMBLY__
#include <hmasm/sysreg.h>
#include <hmkernel/compiler.h>
#define read_cpuid(reg) read_sysreg_s(SYS_ ## reg)
#define read_cpuid32(reg) (u32)read_sysreg_s(SYS_ ## reg)

/* cpu id never change at run time */
static inline u32 __const read_cpuid_midr(void)
{
	return (u32)read_cpuid(MIDR_EL1);
}

static inline u64 __const read_cpuid_mpidr(void)
{
	return read_cpuid(MPIDR_EL1);
}

static inline u32 __const read_cpuid_cachetype(void)
{
	return (u32)read_cpuid(CTR_EL0);
}
#endif /* __ASSEMBLY__ */

#endif
