/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022.
 * Description: register operation helper
 *
 * This file is released under the GPLv2.
 */
#ifndef REG_HELPER_H
#define REG_HELPER_H

#include <linux/io.h>
#include <linux/delay.h>
#include <linux/printk.h>
#include "i2a_debug.h"

static inline uint32_t read_reg(const void __iomem *addr)
{
	uint32_t value = readl(addr);

	if (unlikely(log_reg_rw()))
		pr_info("i2a: reading addr:%#lx, value:%#010x", (uint64_t)addr, value);
	return value;
}

static inline void write_reg(u32 value, void __iomem *addr)
{
	writel(value, addr);
	if (unlikely(log_reg_rw()))
		pr_info("i2a: writing addr:%#lx, value:%#010x, readback:%#010x",
			(uint64_t)addr, value, readl(addr));
}

// irq related
struct irq_bit_info {
	uint32_t bit_mask;
	const char *reg_name;
	const char *bit_name;
};

struct irq_reg_info {
	uint32_t irq_stat_offset;
	uint32_t irq_clear_offset;
	struct irq_bit_info *irq_bit_info;
	uint32_t irq_bit_count;
};

#define def_irq_bit(regtype, bitname) { BIT(regtype ## _ ## bitname ## _START), #regtype, #bitname }
#define def_idi2axi_irq_bit(bitname) def_irq_bit(SOC_IDI2AXI_INT_MSK, bitname)
#endif /* end of include guard: REG_HELPER_H */
