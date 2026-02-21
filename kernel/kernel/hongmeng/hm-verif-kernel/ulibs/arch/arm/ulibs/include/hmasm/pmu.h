/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Basic interface for libperf
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jan 16 16:39:00 2019
 */
#ifndef ARM_ULIBS_ASM_PMU_H
#define ARM_ULIBS_ASM_PMU_H

#include <stdint.h>

#define ARM_PMU_PMCR_DP(x)	(((x) & 1u) << 5)
#define ARM_PMU_PMCR_X(x)	(((x) & 1u) << 4)
#define ARM_PMU_PMCR_D(x)	(((x) & 1u) << 3)
#define ARM_PMU_PMCR_C(x)	(((x) & 1u) << 2)
#define ARM_PMU_PMCR_P(x)	(((x) & 1u) << 1)
#define ARM_PMU_PMCR_E(x)	(((x) & 1u))


#define ARM_PMU_PMCR_N_extract(x)	(((x) >> 11) & 0x1fu)
#define ARM_PMU_PMCR_E_extract(x)	((x) & 1u)

#define ARM_PMU_PMUSERENR_EN(x)		((x) & 1u)

#define ARM_PMU_PMCNTEN_C(x)		(((x) & 1u) << 31)
#define ARM_PMU_PMCNTEN_C_extract(x)	(((x) >> 31) & 1u)

/*
 * FLAG: counters overflow flag status reg
 */
#define	ARM_PMU_FLAG_MASK	0xffffffff /* Mask for writable bits */
#define	ARM_PMU_OVERFLOWED_MASK	ARM_PMU_FLAG_MASK

/*
 * PMXEVTYPER: Event selection reg
 */
#define	ARM_PMU_EVTYPE_MASK	0xc80000ff /* Mask for writable bits */
#define	ARM_PMU_EVTYPE_EVENT	0xff /* Mask for EVENT bits */


#define DEFINE_SYSREG_RD(name, cp, code)\
static inline uint32_t			\
sysreg_read_##name(void)		\
{					\
	uint32_t v;				\
	asm volatile("mrc " #cp ", " code" " : "=r" (v));\
	return v;			\
}

#define DEFINE_SYSREG_WR(name, cp, code)\
static inline void			\
sysreg_write_##name(uint32_t v)		\
{					\
	asm volatile("mcr " #cp ", " code " " :: "r" (v)); \
}

#define DEFINE_SYSREG(name, cp, code) \
	DEFINE_SYSREG_RD(name, cp, code) \
	DEFINE_SYSREG_WR(name, cp, code)

#define DEFINE_CP15REG(name, code) DEFINE_SYSREG(name, p15, code)
#define DEFINE_CP15REG_RO(name, code) DEFINE_SYSREG_RD(name, p15, code)
#define DEFINE_CP15REG_WO(name, code) DEFINE_SYSREG_WR(name, p15, code)

#define DEFINE_PMUV1REG_RO(name, code)		\
DEFINE_CP15REG_RO(name, code)			\
static inline uint32_t arm_pmu_##name##_read(void)	\
{						\
	return sysreg_read_##name();		\
}

#define DEFINE_PMUV1REG_WO(name, code)		\
DEFINE_CP15REG_WO(name, code)			\
static inline void arm_pmu_##name##_write(uint32_t v)\
{						\
	sysreg_write_##name(v);			\
}

#define DEFINE_PMUV1REG(name, code)		\
DEFINE_CP15REG(name, code)			\
static inline uint32_t arm_pmu_##name##_read(void)	\
{						\
	return sysreg_read_##name();		\
}						\
static inline void arm_pmu_##name##_write(uint32_t v)\
{						\
	sysreg_write_##name(v);			\
}

DEFINE_PMUV1REG(pmcr,  "0, %0, c9, c12, 0")
DEFINE_PMUV1REG(pmcntenset,  "0, %0, c9, c12, 1")
DEFINE_PMUV1REG(pmcntenclr,  "0, %0, c9, c12, 2")

DEFINE_PMUV1REG(pmovsclr,  "0, %0, c9, c12, 3")
DEFINE_PMUV1REG(pmselr,  "0, %0, c9, c12, 5")
DEFINE_PMUV1REG(pmxevtyper,  "0, %0, c9, c13, 1")
DEFINE_PMUV1REG(pmxevcntr,  "0, %0, c9, c13, 2")
DEFINE_PMUV1REG(pmuserenr,  "0, %0, c9, c14, 0")
DEFINE_PMUV1REG(pmintenset,  "0, %0, c9, c14, 1")
DEFINE_PMUV1REG(pmintenclr,  "0, %0, c9, c14, 2")
DEFINE_PMUV1REG(pmovsset,  "0, %0, c9, c14, 3")
DEFINE_PMUV1REG_WO(pmswinc,  "0, %0, c9, c12, 4")

DEFINE_CP15REG(pmccntr,  "0, %0, c9, c13, 0")

DEFINE_PMUV1REG_RO(pmceid0,  "0, %0, c9, c12, 6")
DEFINE_PMUV1REG_RO(pmceid1,  "0, %0, c9, c12, 7")
/* Debug Feature Register0 */
DEFINE_PMUV1REG_RO(id_arm32dfr0,  "0, %0, c0, c1, 2")

/*
 * pmccntr register in pmuv1 is 32bit,
 * but for the consistency of arm_pmu_common interface,
 * think of pmccntr is a 64bit register.
 * */
static inline uint64_t arm_pmu_pmccntr_read(void)
{
	return (uint64_t)sysreg_read_pmccntr();
}

static inline void arm_pmu_pmccntr_write(uint64_t v)
{
	uint32_t value = (uint32_t)v;
	sysreg_write_pmccntr(value);
}

/* pmuv1 is not support filter */
static inline uint32_t arm_pmu_pmccfiltr_read(void)
{
	return 0;
}

static inline void arm_pmu_pmccfiltr_write(uint32_t v)
{
	return;
}

#endif
