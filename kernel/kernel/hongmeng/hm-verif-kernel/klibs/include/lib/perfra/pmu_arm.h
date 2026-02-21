/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: performance monitor unit
 * Author: Huawei OS Kernel Lab
 * Create: Wed Aug 19 19:53:03 CST 2021
 */
#ifndef KLIBS_PERFRA_PMU_ARM_H
#define KLIBS_PERFRA_PMU_ARM_H

#include <hmasm/types.h>
#include <hmasm/sysreg_utils.h>

#ifdef __arm__

#define __pmu_arm_define_pmuv1reg_ro(name, code)		\
__arm_define_sysreg_rd(pmu_arm_sysreg_read, name, p15, code)	\
static inline __u32 __pmu_arm_##name##_read(void)		\
{								\
	return __pmu_arm_sysreg_read_##name();			\
}

#define __pmu_arm_define_pmuv1reg_wo(name, code)		\
__arm_define_sysreg_wr(pmu_arm_sysreg_write, name, p15, code)	\
static inline void __pmu_arm_##name##_write(__u32 v)		\
{								\
	__pmu_arm_sysreg_write_##name(v);			\
}

#define __pmu_arm_define_pmuv1reg(name, code)			\
	__pmu_arm_define_pmuv1reg_ro(name, code)		\
	__pmu_arm_define_pmuv1reg_wo(name, code)

__pmu_arm_define_pmuv1reg(pmcr,  "0, %0, c9, c12, 0")
__pmu_arm_define_pmuv1reg(pmcntenset,  "0, %0, c9, c12, 1")
__pmu_arm_define_pmuv1reg(pmcntenclr,  "0, %0, c9, c12, 2")

__pmu_arm_define_pmuv1reg(pmovsclr,  "0, %0, c9, c12, 3")
__pmu_arm_define_pmuv1reg(pmselr,  "0, %0, c9, c12, 5")
__pmu_arm_define_pmuv1reg(pmxevtyper,  "0, %0, c9, c13, 1")
__pmu_arm_define_pmuv1reg(pmxevcntr,  "0, %0, c9, c13, 2")
__pmu_arm_define_pmuv1reg(pmuserenr,  "0, %0, c9, c14, 0")
__pmu_arm_define_pmuv1reg(pmintenset,  "0, %0, c9, c14, 1")
__pmu_arm_define_pmuv1reg(pmintenclr,  "0, %0, c9, c14, 2")
__pmu_arm_define_pmuv1reg(pmovsset,  "0, %0, c9, c14, 3")
__pmu_arm_define_pmuv1reg_wo(pmswinc,  "0, %0, c9, c12, 4")

__pmu_arm_define_pmuv1reg(pmccntr,  "0, %0, c9, c13, 0")

__pmu_arm_define_pmuv1reg_ro(pmceid0,  "0, %0, c9, c12, 6")
__pmu_arm_define_pmuv1reg_ro(pmceid1,  "0, %0, c9, c12, 7")

#define __PMU_ARM_PMCR_E	(1U << 0U)
#define __PMU_ARM_PMCR_P	(1U << 1U)
#define __PMU_ARM_PMCR_C	(1U << 2U)
#define __PMU_ARM_PMCR_D	(1U << 3U)
#define __PMU_ARM_PMCR_DP	(1U << 5U)
#define __pmu_arm_pmcr_e_extract(x)		(((x) >> 0U) & 1U)
#define __pmu_arm_pmcr_p_extract(x)		(((x) >> 1U) & 1U)
#define __pmu_arm_pmcr_c_extract(x)		(((x) >> 2U) & 1U)
#define __pmu_arm_pmcr_d_extract(x)		(((x) >> 3U) & 1U)
#define __pmu_arm_pmcr_dp_extract(x)		(((x) >> 5U) & 1U)


#define __PMU_ARM_PMCNTENSET_C				(1U << 31U)
#define __pmu_arm_pmcntenset_p_n(n)			(1U << (n))
#define __pmu_arm_pmcntenset_c_extract(x)		(((x) >> 31U) & 1U)
#define __pmu_arm_pmcntenset_p_n_extract(x, n)		(((x) >> (n)) & 1U)

#define __PMU_ARM_PMCNTENCLR_C				(1U << 31U)
#define __pmu_arm_pmcntenclr_p_n(n)			(1U << (n))
#define __pmu_arm_pmcntenclr_c_extract(x)		(((x) >> 31U) & 1U)
#define __pmu_arm_pmcntenclr_p_n_extract(x, n)		(((x) >> (n)) & 1U)

#define __PMU_ARM_EVENT_NUMBER_SW_INCR		0x00
#define __PMU_ARM_EVENT_NUMBER_L1D_CACHE_REFILL	0x03
#define __PMU_ARM_EVENT_NUMBER_L1D_CACHE	0x04
#define __PMU_ARM_EVENT_NUMBER_BR_MIS_PRED	0x10
#define __PMU_ARM_EVENT_NUMBER_CPU_CYCLES	0x11
#define __PMU_ARM_EVENT_NUMBER_BR_PRED		0x12

#define __PMU_ARM_EVENT_NUMBER_INST_RETIRED	0x08
#define __PMU_ARM_EVENT_NUMBER_INST_SPEC	0x1B

#define __PMU_EVENT_NUMBER_SW_INCR		__PMU_ARM_EVENT_NUMBER_SW_INCR
#define __PMU_EVENT_NUMBER_L1D_CACHE_REFILL	__PMU_ARM_EVENT_NUMBER_L1D_CACHE_REFILL
#define __PMU_EVENT_NUMBER_L1D_CACHE		__PMU_ARM_EVENT_NUMBER_L1D_CACHE
#define __PMU_EVENT_NUMBER_BR_MIS_PRED		__PMU_ARM_EVENT_NUMBER_BR_MIS_PRED
#define __PMU_EVENT_NUMBER_CPU_CYCLES		__PMU_ARM_EVENT_NUMBER_CPU_CYCLES
#define __PMU_EVENT_NUMBER_BR_PRED		__PMU_ARM_EVENT_NUMBER_BR_PRED

#define __PMU_EVENT_NUMBER_INST_RETIRED		__PMU_ARM_EVENT_NUMBER_INST_RETIRED
#define __PMU_EVENT_NUMBER_INST_SPEC		__PMU_ARM_EVENT_NUMBER_INST_SPEC

static inline void pmu_pmccntr_enable(void)
{
	__u32 pmcr = __pmu_arm_pmcr_read();
	__u32 pmcntenset = __pmu_arm_pmcntenset_read();

	pmcr |= __PMU_ARM_PMCR_E;
	pmcr &= ~__PMU_ARM_PMCR_D;
	pmcr &= ~__PMU_ARM_PMCR_DP;
	pmcntenset |= __PMU_ARM_PMCNTENSET_C;

	__pmu_arm_pmcr_write(pmcr);
	__pmu_arm_pmcntenset_write(pmcntenset);
}

static inline void pmu_pmccntr_disable(void)
{
	__u32 pmcntenclr = __pmu_arm_pmcntenclr_read();
	pmcntenclr |= __PMU_ARM_PMCNTENCLR_C;
	__pmu_arm_pmcntenclr_write(pmcntenclr);
}

static inline __u64 pmu_pmccntr_read(void)
{
	return __pmu_arm_pmccntr_read();
}
static inline void pmu_pmccntr_write(__u64 v)
{
	__pmu_arm_pmccntr_write((__u32)v);
}

static inline __u32 pmu_pmccntr_read32(void)
{
	return __pmu_arm_pmccntr_read();
}
static inline void pmu_pmccntr_write32(__u32 v)
{
	__pmu_arm_pmccntr_write(v);
}

static inline void pmu_event_enable(__u32 event_counter, __u32 event_number)
{
	/* configure */
	__pmu_arm_pmselr_write(event_counter);
	__pmu_arm_pmxevtyper_write(event_number);

	/* enable */
	__u32 pmcr = __pmu_arm_pmcr_read();
	__u32 pmcntenset = __pmu_arm_pmcntenset_read();
	pmcr |= __PMU_ARM_PMCR_E;
	pmcntenset |= __pmu_arm_pmcntenset_p_n(event_counter);
	__pmu_arm_pmcr_write(pmcr);
	__pmu_arm_pmcntenset_write(pmcntenset);
}
static inline void pmu_event_disable(__u32 event_counter)
{
	__u32 pmcntenclr = __pmu_arm_pmcntenclr_read();
	pmcntenclr |= __pmu_arm_pmcntenclr_p_n(event_counter);
	__pmu_arm_pmcntenclr_write(pmcntenclr);
}

static inline __u32 pmu_event_read32(__u32 event_counter)
{
	__pmu_arm_pmselr_write(event_counter);
	return __pmu_arm_pmxevcntr_read();
}
static inline void pmu_event_write32(__u32 event_counter, __u32 val)
{
	__pmu_arm_pmselr_write(event_counter);
	__pmu_arm_pmxevcntr_write(val);
}

static inline __u64 pmu_event_read(__u32 event_counter)
{
	return (__u64)pmu_event_read32(event_counter);
}
static inline void pmu_event_write(__u32 event_counter, __u64 val)
{
	pmu_event_write32(event_counter, (__u32)val);
}
#endif

#endif
