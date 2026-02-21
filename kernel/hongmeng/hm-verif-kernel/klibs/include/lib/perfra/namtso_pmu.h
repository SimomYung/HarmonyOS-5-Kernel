/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Basic namtso ops
 * Author: Huawei OS Kernel Lab
 * Create: Sep 12 16:56:22 2023
 */

#ifndef AARCH64_ASM_DRIVERS_NAMTSO_PMU_H
#define AARCH64_ASM_DRIVERS_NAMTSO_PMU_H

#include <hmasm/barrier.h>
#include <hmasm/processor.h>
#include <hmkernel/bitops/bitops.h>
#include <hmkernel/io.h>
#include <hmkernel/cpuset.h>

#ifdef CONFIG_AARCH64_NAMTSO_DRIVER
#define NAMTSO_PMEVCNTR0_OFFSET         0x0
#define NAMTSO_PMCCNTR_L_OFFSET         0x0F8
#define NAMTSO_PMCCNTR_H_OFFSET         0x0FC
#define NAMTSO_PMCCNTR_OFFSET           0x0F8
#define NAMTSO_ACTLR_OFFSET             0x340
#define NAMTSO_PMEVTYPER0_OFFSET        0x400
#define NAMTSO_PMCNTENSET_OFFSET        0xC00
#define NAMTSO_PMCNTENCLR_OFFSET        0xC20
#define NAMTSO_PMINTENSET_OFFSET        0xC40
#define NAMTSO_PMINTENCLR_OFFSET        0xC60
#define NAMTSO_PMOVSCLR_OFFSET          0xC80
#define NAMTSO_PMOVSSET_OFFSET          0xCC0
#define NAMTSO_PMCR_OFFSET              0xE04

#define PMEVCNTR_STEP                   0x8
#define PMEVTYPER_STEP                  0x4

#define U32_MAX_MASK                    ((1UL << 32) - 1)

extern cpuset_t associated_cpumask;
extern void __iomem *namtso_base_addr;

static inline u32 namtso_pmu_read_pmcr(void)
{
	return io_reg_read32(namtso_base_addr, NAMTSO_PMCR_OFFSET);
}

static inline u32 namtso_pmu_pmcntenset_read(void)
{
	return io_reg_read32(namtso_base_addr, NAMTSO_PMCNTENSET_OFFSET);
}

static inline void namtso_pmu_write_pmcr(u32 val)
{
	io_reg_write32(namtso_base_addr, NAMTSO_PMCR_OFFSET, val);
}

static inline void namtso_pmu_pmintenclr_write(u32 val)
{
	io_reg_write32(namtso_base_addr, NAMTSO_PMINTENCLR_OFFSET, val);
}

static inline void namtso_pmu_pmcntenclr_write(u32 val)
{
	io_reg_write32(namtso_base_addr, NAMTSO_PMCNTENCLR_OFFSET, val);
}

static inline void namtso_pmu_pmintenset_write(u32 val)
{
	io_reg_write32(namtso_base_addr, NAMTSO_PMINTENSET_OFFSET, val);
}

static inline void namtso_pmu_pmcntenset_write(u32 val)
{
	io_reg_write32(namtso_base_addr, NAMTSO_PMCNTENSET_OFFSET, val);
}

static inline u32 namtso_pmu_read_reset_overflow(void)
{
	u32 val = (u32)io_reg_read32(namtso_base_addr, NAMTSO_PMOVSCLR_OFFSET);
	/* Clear bits */
	io_reg_write32(namtso_base_addr, NAMTSO_PMOVSCLR_OFFSET, val);
	return val;
}

/* All event counters are 32bit, onle Cycle counter is 64bit */
static inline u32 namtso_pmu_get_counter_offset(unsigned int counter_idx)
{
	return (u32)(NAMTSO_PMEVCNTR0_OFFSET + (counter_idx * PMEVCNTR_STEP));
}

static inline u32 namtso_pmu_get_typer_offset(unsigned int typer_idx)
{
	return (u32)(NAMTSO_PMEVTYPER0_OFFSET + (typer_idx * PMEVTYPER_STEP));
}

static inline u32 namtso_pmu_read_counter(unsigned int counter_idx)
{
	u32 offset = namtso_pmu_get_counter_offset(counter_idx);
	return io_reg_read32(namtso_base_addr, offset);
}

static inline void namtso_pmu_write_counter(unsigned int counter_idx, u32 val)
{
	u32 offset = namtso_pmu_get_counter_offset(counter_idx);
	io_reg_write32(namtso_base_addr, offset, val);
}

static inline void namtso_pmu_write_event(unsigned int typer_idx, u32 event)
{
	u32 offset = namtso_pmu_get_typer_offset(typer_idx);
	io_reg_write32(namtso_base_addr, offset, event);
}

static inline u32 namtso_pmu_read_pmxevtyper(unsigned int typer_idx)
{
	u32 offset = namtso_pmu_get_typer_offset(typer_idx);
	return io_reg_read32(namtso_base_addr, offset);
}

static inline u64 namtso_pmu_read_pmccntr(void)
{
	u64 val64;
	u64 val_l, val_h;

#if defined(CONFIG_AARCH64_NAMTSO_COUNTER64)
	val64 = io_reg_read64(namtso_base_addr, NAMTSO_PMCCNTR_OFFSET);
#else
	val_l = (u64)io_reg_read32(namtso_base_addr, NAMTSO_PMCCNTR_L_OFFSET);
	val_h = (u64)io_reg_read32(namtso_base_addr, NAMTSO_PMCCNTR_H_OFFSET);
	val64 = (val_h << 32) | val_l;
#endif

	return val64;
}

static inline void namtso_pmu_write_pmccntr(u64 val)
{
	u32 val_l, val_h;

#if defined(CONFIG_AARCH64_NAMTSO_COUNTER64)
	io_reg_write64(namtso_base_addr, NAMTSO_PMCCNTR_OFFSET, val);
#else
	val_l = val & U32_MAX_MASK;
	val_h = (val >> 32) & U32_MAX_MASK;

	io_reg_write32(namtso_base_addr, NAMTSO_PMCCNTR_L_OFFSET, val_l);
	io_reg_write32(namtso_base_addr, NAMTSO_PMCCNTR_H_OFFSET, val_h);
#endif
}

static inline void namtso_pmu_disable_counter(unsigned int counter_idx)
{
	io_reg_write32(namtso_base_addr, NAMTSO_PMCNTENCLR_OFFSET, BIT(counter_idx));
}

static inline void namtso_pmu_enable_counter(unsigned int counter_idx)
{
	io_reg_write32(namtso_base_addr, NAMTSO_PMCNTENSET_OFFSET, BIT(counter_idx));
}

static inline void namtso_pmu_counter_interrupt_enable(unsigned int counter_idx)
{
	io_reg_write32(namtso_base_addr, NAMTSO_PMINTENSET_OFFSET, BIT(counter_idx));
}

static inline void namtso_pmu_counter_interrupt_disable(unsigned int counter_idx)
{
	io_reg_write32(namtso_base_addr, NAMTSO_PMINTENCLR_OFFSET, BIT(counter_idx));
}

#endif
#endif
