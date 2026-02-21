/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Basic dsu ops
 * Author: Huawei OS Kernel Lab
 * Create: Wed Oct 20 16:56:22 2021
 */
#ifndef AARCH64_ASM_DRIVERS_DSU_PMU_H
#define AARCH64_ASM_DRIVERS_DSU_PMU_H

#include <hmasm/barrier.h>
#include <hmasm/processor.h>

#include <hmkernel/bitops/bitops.h>

#ifdef CONFIG_AARCH64_DSU_DRIVER
#define CLUSTERPMCR_EL1			"s3_0_c15_c5_0"
#define CLUSTERPMCNTENSET_EL1		"s3_0_c15_c5_1"
#define CLUSTERPMCNTENCLR_EL1		"s3_0_c15_c5_2"
#define CLUSTERPMOVSSET_EL1		"s3_0_c15_c5_3"
#define CLUSTERPMOVSCLR_EL1		"s3_0_c15_c5_4"
#define CLUSTERPMSELR_EL1		"s3_0_c15_c5_5"
#define CLUSTERPMINTENSET_EL1		"s3_0_c15_c5_6"
#define CLUSTERPMINTENCLR_EL1		"s3_0_c15_c5_7"
#define CLUSTERPMCCNTR_EL1		"s3_0_c15_c6_0"
#define CLUSTERPMXEVTYPER_EL1		"s3_0_c15_c6_1"
#define CLUSTERPMXEVCNTR_EL1		"s3_0_c15_c6_2"
#define CLUSTERPMMDCR_EL1		"s3_0_c15_c6_3"
#define CLUSTERPMCEID0_EL1		"s3_0_c15_c6_4"
#define CLUSTERPMCEID1_EL1		"s3_0_c15_c6_5"

static inline u32 arm_dsu_pmu_read_pmcr(void)
{
	return (u32)read_sysreg(CLUSTERPMCR_EL1);
}

static inline u32 arm_dsu_pmu_pmcntenset_read(void)
{
	return (u32)read_sysreg(CLUSTERPMCNTENSET_EL1);
}

static inline void arm_dsu_pmu_write_pmcr(u32 val)
{
	write_sysreg(CLUSTERPMCR_EL1, val);
	isb();
}

static inline void arm_dsu_pmu_pmintenclr_write(u32 val)
{
	write_sysreg(CLUSTERPMINTENCLR_EL1, val);
	isb();
}

static inline void arm_dsu_pmu_pmcntenclr_write(u32 val)
{
	write_sysreg(CLUSTERPMCNTENCLR_EL1, val);
	isb();
}

static inline void arm_dsu_pmu_pmintenset_write(u32 val)
{
	write_sysreg(CLUSTERPMINTENSET_EL1, val);
	isb();
}

static inline void arm_dsu_pmu_pmcntenset_write(u32 val)
{
	write_sysreg(CLUSTERPMCNTENSET_EL1, val);
	isb();
}

static inline u32 arm_dsu_pmu_read_reset_overflow(void)
{
	u32 val = (u32)read_sysreg(CLUSTERPMOVSCLR_EL1);
	/* Clear bits */
	write_sysreg(CLUSTERPMOVSCLR_EL1, val);
	isb();
	return val;
}

static inline void arm_dsu_pmu_select_counter(unsigned int counter)
{
	write_sysreg(CLUSTERPMSELR_EL1, counter);
	isb();
}

static inline u64 arm_dsu_pmu_read_counter(unsigned int counter)
{
	arm_dsu_pmu_select_counter(counter);
	return read_sysreg(CLUSTERPMXEVCNTR_EL1);
}

static inline void arm_dsu_pmu_write_counter(unsigned int counter, u64 val)
{
	arm_dsu_pmu_select_counter(counter);
	write_sysreg(CLUSTERPMXEVCNTR_EL1, val);
	isb();
}

static inline void arm_dsu_pmu_write_event(unsigned int counter, u32 event)
{
	arm_dsu_pmu_select_counter(counter);
	write_sysreg(CLUSTERPMXEVTYPER_EL1, event);
	isb();
}

static inline u64 arm_dsu_pmu_read_pmccntr(void)
{
	return read_sysreg(CLUSTERPMCCNTR_EL1);
}

static inline void arm_dsu_pmu_write_pmccntr(u64 val)
{
	write_sysreg(CLUSTERPMCCNTR_EL1, val);
	isb();
}

static inline void arm_dsu_pmu_disable_counter(unsigned int counter)
{
	write_sysreg(CLUSTERPMCNTENCLR_EL1, BIT(counter));
	isb();
}

static inline void arm_dsu_pmu_enable_counter(unsigned int counter)
{
	write_sysreg(CLUSTERPMCNTENSET_EL1, BIT(counter));
	isb();
}

static inline void arm_dsu_pmu_counter_interrupt_enable(unsigned int counter)
{
	write_sysreg(CLUSTERPMINTENSET_EL1, BIT(counter));
	isb();
}

static inline void arm_dsu_pmu_counter_interrupt_disable(unsigned int counter)
{
	write_sysreg(CLUSTERPMINTENCLR_EL1, BIT(counter));
	isb();
}

static inline u32 arm_dsu_pmu_read_pmxevtyper(unsigned int i)
{
	arm_dsu_pmu_select_counter(i);
	return (u32)read_sysreg(CLUSTERPMXEVTYPER_EL1);
}

static inline void arm_dsu_pmu_write_pmxevtyper(unsigned int i, u32 event)
{
	arm_dsu_pmu_select_counter(i);
	write_sysreg(CLUSTERPMXEVTYPER_EL1, event);
	isb();
}

#endif
#endif
