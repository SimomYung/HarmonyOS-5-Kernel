/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: performance monitor unit
 * Author: Huawei OS Kernel Lab
 * Create: Wed Aug 19 19:53:03 CST 2021
 */
#ifndef KLIBS_PERFRA_PMU_AARCH64_H
#define KLIBS_PERFRA_PMU_AARCH64_H

#include <hmasm/types.h>
#include <hmasm/sysreg_utils.h>

#ifdef __aarch64__

#define __aarch64_define_pmuv3reg_ro(name, el)		\
__AARCH64_DEFINE_sysreg_rd(__pmu_aarch64_sysreg_read, name, el)	\
__AARCH64_DEFINE_sysreg_rd32(__pmu_aarch64_sysreg_read32, name, el) \
static inline __u64 __pmu_aarch64_##name##_read(void)		\
{								\
	return __pmu_aarch64_sysreg_read_##name##_##el();	\
}								\
static inline __u32 __pmu_aarch64_##name##_read32(void)		\
{								\
	return __pmu_aarch64_sysreg_read32_##name##_##el();	\
}

#define __aarch64_define_pmuv3reg_wo(name, el)		\
__AARCH64_DEFINE_sysreg_wr(__pmu_aarch64_sysreg_write, name, el) \
__AARCH64_DEFINE_sysreg_wr32(__pmu_aarch64_sysreg_write32, name, el) \
static inline void __pmu_aarch64_##name##_write(__u64 v)	\
{								\
	__pmu_aarch64_sysreg_write_##name##_##el(v);		\
}								\
static inline void __pmu_aarch64_##name##_write32(__u32 v)	\
{								\
	__pmu_aarch64_sysreg_write32_##name##_##el(v);		\
}

#define __aarch64_define_pmuv3reg(name, el)			\
	__aarch64_define_pmuv3reg_ro(name, el)		\
	__aarch64_define_pmuv3reg_wo(name, el)

__aarch64_define_pmuv3reg(pmcntenset, el0)
__aarch64_define_pmuv3reg(pmcntenclr, el0)
__aarch64_define_pmuv3reg(pmovsclr, el0)
__aarch64_define_pmuv3reg(pmovsset, el0)
__aarch64_define_pmuv3reg(pmintenclr, el1)
__aarch64_define_pmuv3reg(pmintenset, el1)
__aarch64_define_pmuv3reg(pmuserenr, el0)
__aarch64_define_pmuv3reg(pmcr, el0)
__aarch64_define_pmuv3reg_wo(pmswinc, el0)

__aarch64_define_pmuv3reg(pmselr, el0)
__aarch64_define_pmuv3reg(pmxevcntr, el0)
__aarch64_define_pmuv3reg(pmxevtyper, el0)

__aarch64_define_pmuv3reg(pmccntr, el0)
__aarch64_define_pmuv3reg(pmccfiltr, el0)

__aarch64_define_pmuv3reg(pmevcntr0, el0)
__aarch64_define_pmuv3reg(pmevcntr1, el0)
__aarch64_define_pmuv3reg(pmevcntr2, el0)
__aarch64_define_pmuv3reg(pmevcntr3, el0)
__aarch64_define_pmuv3reg(pmevcntr4, el0)
__aarch64_define_pmuv3reg(pmevcntr5, el0)
__aarch64_define_pmuv3reg(pmevcntr6, el0)
__aarch64_define_pmuv3reg(pmevcntr7, el0)
__aarch64_define_pmuv3reg(pmevcntr8, el0)
__aarch64_define_pmuv3reg(pmevcntr9, el0)
__aarch64_define_pmuv3reg(pmevcntr10, el0)
__aarch64_define_pmuv3reg(pmevcntr11, el0)
__aarch64_define_pmuv3reg(pmevcntr12, el0)
__aarch64_define_pmuv3reg(pmevcntr13, el0)
__aarch64_define_pmuv3reg(pmevcntr14, el0)
__aarch64_define_pmuv3reg(pmevcntr15, el0)
__aarch64_define_pmuv3reg(pmevcntr16, el0)
__aarch64_define_pmuv3reg(pmevcntr17, el0)
__aarch64_define_pmuv3reg(pmevcntr18, el0)
__aarch64_define_pmuv3reg(pmevcntr19, el0)
__aarch64_define_pmuv3reg(pmevcntr20, el0)
__aarch64_define_pmuv3reg(pmevcntr21, el0)
__aarch64_define_pmuv3reg(pmevcntr22, el0)
__aarch64_define_pmuv3reg(pmevcntr23, el0)
__aarch64_define_pmuv3reg(pmevcntr24, el0)
__aarch64_define_pmuv3reg(pmevcntr25, el0)
__aarch64_define_pmuv3reg(pmevcntr26, el0)
__aarch64_define_pmuv3reg(pmevcntr27, el0)
__aarch64_define_pmuv3reg(pmevcntr28, el0)
__aarch64_define_pmuv3reg(pmevcntr29, el0)
__aarch64_define_pmuv3reg(pmevcntr30, el0)

__aarch64_define_pmuv3reg(pmevtyper0, el0)
__aarch64_define_pmuv3reg(pmevtyper1, el0)
__aarch64_define_pmuv3reg(pmevtyper2, el0)
__aarch64_define_pmuv3reg(pmevtyper3, el0)
__aarch64_define_pmuv3reg(pmevtyper4, el0)
__aarch64_define_pmuv3reg(pmevtyper5, el0)
__aarch64_define_pmuv3reg(pmevtyper6, el0)
__aarch64_define_pmuv3reg(pmevtyper7, el0)
__aarch64_define_pmuv3reg(pmevtyper8, el0)
__aarch64_define_pmuv3reg(pmevtyper9, el0)
__aarch64_define_pmuv3reg(pmevtyper10, el0)
__aarch64_define_pmuv3reg(pmevtyper11, el0)
__aarch64_define_pmuv3reg(pmevtyper12, el0)
__aarch64_define_pmuv3reg(pmevtyper13, el0)
__aarch64_define_pmuv3reg(pmevtyper14, el0)
__aarch64_define_pmuv3reg(pmevtyper15, el0)
__aarch64_define_pmuv3reg(pmevtyper16, el0)
__aarch64_define_pmuv3reg(pmevtyper17, el0)
__aarch64_define_pmuv3reg(pmevtyper18, el0)
__aarch64_define_pmuv3reg(pmevtyper19, el0)
__aarch64_define_pmuv3reg(pmevtyper20, el0)
__aarch64_define_pmuv3reg(pmevtyper21, el0)
__aarch64_define_pmuv3reg(pmevtyper22, el0)
__aarch64_define_pmuv3reg(pmevtyper23, el0)
__aarch64_define_pmuv3reg(pmevtyper24, el0)
__aarch64_define_pmuv3reg(pmevtyper25, el0)
__aarch64_define_pmuv3reg(pmevtyper26, el0)
__aarch64_define_pmuv3reg(pmevtyper27, el0)
__aarch64_define_pmuv3reg(pmevtyper28, el0)
__aarch64_define_pmuv3reg(pmevtyper29, el0)
__aarch64_define_pmuv3reg(pmevtyper30, el0)

#define __PMU_AARCH64_PMCR_EL0_E	(1ULL << 0U)
#define __PMU_AARCH64_PMCR_EL0_P	(1ULL << 1U)
#define __PMU_AARCH64_PMCR_EL0_C	(1ULL << 2U)
#define __PMU_AARCH64_PMCR_EL0_D	(1ULL << 3U)
#define __PMU_AARCH64_PMCR_EL0_DP	(1ULL << 5U)
#define __pmu_aarch64_pmcr_el0_e_extract(x)		(((x) >> 0U) & 1U)
#define __pmu_aarch64_pmcr_el0_p_extract(x)		(((x) >> 1U) & 1U)
#define __pmu_aarch64_pmcr_el0_c_extract(x)		(((x) >> 2U) & 1U)
#define __pmu_aarch64_pmcr_el0_d_extract(x)		(((x) >> 3U) & 1U)
#define __pmu_aarch64_pmcr_el0_dp_extract(x)		(((x) >> 5U) & 1U)


#define __PMU_AARCH64_PMCNTENSET_EL0_C				(1ULL << 31U)
#define __pmu_aarch64_pmcntenset_el0_p_n(n)			(1ULL << (n))
#define __pmu_aarch64_pmcntenset_el0_c_extract(x)		(((x) >> 31U) & 1U)
#define __pmu_aarch64_pmcntenset_el0_p_n_extract(x, n)		(((x) >> (n)) & 1U)

#define __PMU_AARCH64_PMCNTENCLR_EL0_C				(1ULL << 31U)
#define __pmu_aarch64_pmcntenclr_el0_p_n(n)			(1ULL << (n))
#define __pmu_aarch64_pmcntenclr_el0_c_extract(x)		(((x) >> 31U) & 1U)
#define __pmu_aarch64_pmcntenclr_el0_p_n_extract(x, n)		(((x) >> (n)) & 1U)

#define __PMU_AARCH64_EVENT_NUMBER_SW_INCR		0x0000
#define __PMU_AARCH64_EVENT_NUMBER_L1D_CACHE_REFILL	0x0003
#define __PMU_AARCH64_EVENT_NUMBER_L1D_CACHE		0x0004
#define __PMU_AARCH64_EVENT_NUMBER_BR_MIS_PRED		0x0010
#define __PMU_AARCH64_EVENT_NUMBER_CPU_CYCLES		0x0011
#define __PMU_AARCH64_EVENT_NUMBER_BR_PRED		0x0012

#define __PMU_AARCH64_EVENT_NUMBER_INST_RETIRED		0x0008
#define __PMU_AARCH64_EVENT_NUMBER_INST_SPEC		0x001B

#define __PMU_EVENT_NUMBER_SW_INCR		__PMU_AARCH64_EVENT_NUMBER_SW_INCR
#define __PMU_EVENT_NUMBER_L1D_CACHE_REFILL	__PMU_AARCH64_EVENT_NUMBER_L1D_CACHE_REFILL
#define __PMU_EVENT_NUMBER_L1D_CACHE		__PMU_AARCH64_EVENT_NUMBER_L1D_CACHE
#define __PMU_EVENT_NUMBER_BR_MIS_PRED		__PMU_AARCH64_EVENT_NUMBER_BR_MIS_PRED
#define __PMU_EVENT_NUMBER_CPU_CYCLES		__PMU_AARCH64_EVENT_NUMBER_CPU_CYCLES
#define __PMU_EVENT_NUMBER_BR_PRED		__PMU_AARCH64_EVENT_NUMBER_BR_PRED

#define __PMU_EVENT_NUMBER_INST_RETIRED		__PMU_AARCH64_EVENT_NUMBER_INST_RETIRED
#define __PMU_EVENT_NUMBER_INST_SPEC		__PMU_AARCH64_EVENT_NUMBER_INST_SPEC

static inline void pmu_pmccntr_enable(void)
{
	__u64 pmcr = __pmu_aarch64_pmcr_read();
	__u64 pmcntenset = __pmu_aarch64_pmcntenset_read();

	pmcr |= __PMU_AARCH64_PMCR_EL0_E;
	pmcr &= ~__PMU_AARCH64_PMCR_EL0_D;
	pmcr &= ~__PMU_AARCH64_PMCR_EL0_DP;
	pmcntenset |= __PMU_AARCH64_PMCNTENSET_EL0_C;

	__pmu_aarch64_pmcr_write(pmcr);
	__pmu_aarch64_pmcntenset_write(pmcntenset);
}

static inline void pmu_pmccntr_disable(void)
{
	__u64 pmcntenclr = __pmu_aarch64_pmcntenclr_read();
	pmcntenclr |= __PMU_AARCH64_PMCNTENCLR_EL0_C;
	__pmu_aarch64_pmcntenclr_write(pmcntenclr);
}

static inline __u64 pmu_pmccntr_read(void)
{
	return __pmu_aarch64_pmccntr_read();
}
static inline void pmu_pmccntr_write(__u64 v)
{
	__pmu_aarch64_pmccntr_write(v);
}

static inline __u32 pmu_pmccntr_read32(void)
{
	return __pmu_aarch64_pmccntr_read32();
}
static inline void pmu_pmccntr_write32(__u32 v)
{
	__pmu_aarch64_pmccntr_write32(v);
}

static inline void pmu_event_enable(__u32 event_counter, __u64 event_number)
{
	/* configure */
	__pmu_aarch64_pmselr_write(event_counter);
	__pmu_aarch64_pmxevtyper_write(event_number);

	/* enable */
	__u64 pmcr = __pmu_aarch64_pmcr_read();
	__u64 pmcntenset = __pmu_aarch64_pmcntenset_read();
	pmcr |= __PMU_AARCH64_PMCR_EL0_E;
	pmcntenset |= __pmu_aarch64_pmcntenset_el0_p_n(event_counter);
	__pmu_aarch64_pmcr_write(pmcr);
	__pmu_aarch64_pmcntenset_write(pmcntenset);
}
static inline void pmu_event_disable(__u32 event_counter)
{
	__u64 pmcntenclr = __pmu_aarch64_pmcntenclr_read();
	pmcntenclr |= __pmu_aarch64_pmcntenclr_el0_p_n(event_counter);
	__pmu_aarch64_pmcntenclr_write(pmcntenclr);
}

static inline __u64 pmu_event_read(__u32 event_counter)
{
	__pmu_aarch64_pmselr_write(event_counter);
	return __pmu_aarch64_pmxevcntr_read();
}
static inline void pmu_event_write(__u32 event_counter, __u64 val)
{
	__pmu_aarch64_pmselr_write(event_counter);
	__pmu_aarch64_pmxevcntr_write(val);
}

static inline __u32 pmu_event_read32(__u32 event_counter)
{
	__pmu_aarch64_pmselr_write(event_counter);
	return __pmu_aarch64_pmxevcntr_read32();
}
static inline void pmu_event_write32(__u32 event_counter, __u32 val)
{
	__pmu_aarch64_pmselr_write(event_counter);
	__pmu_aarch64_pmxevcntr_write32(val);
}
#endif

#endif
