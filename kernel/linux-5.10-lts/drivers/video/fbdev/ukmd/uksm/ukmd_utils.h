/**
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#ifndef UKMD_UTILS_H
#define UKMD_UTILS_H

#include <linux/io.h>

#ifndef max_of_three
#define max_of_three(a, b, c) ((a) > (b) ? ((a) > (c) ? (a) : (c)) : ((b) > (c) ? (b) : (c)))
#endif

#ifndef ceil_div
#define ceil_div(val, al) (((val) + ((al) - 1)) / (al))
#endif

#define PERCENT 100
#define HZ_TO_US 1000000

#define FPGA_EVENT_TIMEOUT_MS 10000
#define ASIC_EVENT_TIMEOUT_MS 300

#define outp32(addr, val)   writel(val, addr)
#define inp32(addr)         readl(addr)

#define DSS_ERR_CODE_DEADBEED 0xDEADBEEDu

enum ACC_INDEX {
	BIG_DPU = 0,
	LITE_DPU,
	AAE_ACC,
	ACC_MAX
};

static inline void set_reg(char __iomem *addr, uint32_t val, uint8_t bw, uint8_t bs)
{
	uint32_t mask, temp;

	mask = GENMASK(bs + bw - 1, bs);
	temp = readl(addr);
	temp &= ~mask;

	/**
	 * @brief if you want to check config value, please uncomment following code
	 * dpu_pr_debug("addr:%#x value:%#x ", addr, temp | ((val << bs) & mask));
	 */
	writel(temp | ((val << bs) & mask), addr);
}

bool check_addr_status_is_valid(const char __iomem* check_addr, uint32_t status,
	uint32_t udelay_time, uint32_t times);

uint32_t read_reg_with_retry(char __iomem *reg_addr);
#endif /* UKMD_UTILS_H */
