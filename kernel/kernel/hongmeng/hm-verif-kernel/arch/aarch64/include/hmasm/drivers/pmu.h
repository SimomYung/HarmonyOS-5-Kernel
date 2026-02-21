/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jan 16 16:39:00 2019
 */
#ifndef AARCH64_ASM_DRIVERS_PMU_H
#define AARCH64_ASM_DRIVERS_PMU_H

#include <hmasm/processor.h>

#define ARMV8_EVENT_COUNTERS_MAX_NR	32UL  /* 0-31, 5-bits field N of register PMCR_EL0 */

#define DEFINE_pmuv3reg_ro(name, el)		\
DEFINE_sysreg_rd(name, el)			\
static inline u64 arm_pmu_##name##_read(void)	\
{						\
	return sysreg_read_##name##_##el();		\
}

#define DEFINE_pmuv3reg_wo(name, el)		\
DEFINE_sysreg_wr(name, el)			\
static inline void arm_pmu_##name##_write(u64 v)\
{						\
	sysreg_write_##name##_##el(v);			\
}

#define DEFINE_pmuv3reg(name, el)		\
DEFINE_sysreg(name, el)			\
static inline u64 arm_pmu_##name##_read(void)	\
{						\
	return sysreg_read_##name##_##el();		\
}						\
static inline void arm_pmu_##name##_write(u64 v)\
{						\
	sysreg_write_##name##_##el(v);			\
}

DEFINE_pmuv3reg(pmcntenset, el0)
DEFINE_pmuv3reg(pmcntenclr, el0)
DEFINE_pmuv3reg(pmovsset, el0)
DEFINE_pmuv3reg(pmovsclr, el0)
DEFINE_pmuv3reg(pmintenclr, el1)
DEFINE_pmuv3reg(pmintenset, el1)
DEFINE_pmuv3reg(pmuserenr, el0)
DEFINE_pmuv3reg(pmcr, el0)

DEFINE_pmuv3reg(pmselr, el0)
DEFINE_pmuv3reg(pmxevcntr, el0)
DEFINE_pmuv3reg(pmxevtyper, el0)

DEFINE_sysreg(pmccntr, el0)
DEFINE_pmuv3reg(pmccfiltr, el0)
DEFINE_pmuv3reg_wo(pmswinc, el0)

DEFINE_sysreg_rd(id_aa64dfr0, el1)

DEFINE_sysreg(pmevcntr0, el0)
DEFINE_sysreg(pmevcntr1, el0)
DEFINE_sysreg(pmevcntr2, el0)
DEFINE_sysreg(pmevcntr3, el0)
DEFINE_sysreg(pmevcntr4, el0)
DEFINE_sysreg(pmevcntr5, el0)
DEFINE_sysreg(pmevcntr6, el0)
DEFINE_sysreg(pmevcntr7, el0)
DEFINE_sysreg(pmevcntr8, el0)
DEFINE_sysreg(pmevcntr9, el0)
DEFINE_sysreg(pmevcntr10, el0)
DEFINE_sysreg(pmevcntr11, el0)
DEFINE_sysreg(pmevcntr12, el0)
DEFINE_sysreg(pmevcntr13, el0)
DEFINE_sysreg(pmevcntr14, el0)
DEFINE_sysreg(pmevcntr15, el0)
DEFINE_sysreg(pmevcntr16, el0)
DEFINE_sysreg(pmevcntr17, el0)
DEFINE_sysreg(pmevcntr18, el0)
DEFINE_sysreg(pmevcntr19, el0)
DEFINE_sysreg(pmevcntr20, el0)
DEFINE_sysreg(pmevcntr21, el0)
DEFINE_sysreg(pmevcntr22, el0)
DEFINE_sysreg(pmevcntr23, el0)
DEFINE_sysreg(pmevcntr24, el0)
DEFINE_sysreg(pmevcntr25, el0)
DEFINE_sysreg(pmevcntr26, el0)
DEFINE_sysreg(pmevcntr27, el0)
DEFINE_sysreg(pmevcntr28, el0)
DEFINE_sysreg(pmevcntr29, el0)
DEFINE_sysreg(pmevcntr30, el0)

DEFINE_sysreg(pmevtyper0, el0)
DEFINE_sysreg(pmevtyper1, el0)
DEFINE_sysreg(pmevtyper2, el0)
DEFINE_sysreg(pmevtyper3, el0)
DEFINE_sysreg(pmevtyper4, el0)
DEFINE_sysreg(pmevtyper5, el0)
DEFINE_sysreg(pmevtyper6, el0)
DEFINE_sysreg(pmevtyper7, el0)
DEFINE_sysreg(pmevtyper8, el0)
DEFINE_sysreg(pmevtyper9, el0)
DEFINE_sysreg(pmevtyper10, el0)
DEFINE_sysreg(pmevtyper11, el0)
DEFINE_sysreg(pmevtyper12, el0)
DEFINE_sysreg(pmevtyper13, el0)
DEFINE_sysreg(pmevtyper14, el0)
DEFINE_sysreg(pmevtyper15, el0)
DEFINE_sysreg(pmevtyper16, el0)
DEFINE_sysreg(pmevtyper17, el0)
DEFINE_sysreg(pmevtyper18, el0)
DEFINE_sysreg(pmevtyper19, el0)
DEFINE_sysreg(pmevtyper20, el0)
DEFINE_sysreg(pmevtyper21, el0)
DEFINE_sysreg(pmevtyper22, el0)
DEFINE_sysreg(pmevtyper23, el0)
DEFINE_sysreg(pmevtyper24, el0)
DEFINE_sysreg(pmevtyper25, el0)
DEFINE_sysreg(pmevtyper26, el0)
DEFINE_sysreg(pmevtyper27, el0)
DEFINE_sysreg(pmevtyper28, el0)
DEFINE_sysreg(pmevtyper29, el0)
DEFINE_sysreg(pmevtyper30, el0)

static inline u64 arm_pmu_pmccntr_read(void)
{
	return sysreg_read_pmccntr_el0();
}

static inline void arm_pmu_pmccntr_write(u64 v)
{
	sysreg_write_pmccntr_el0(v);
}

#endif
