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

#include <linux/delay.h>
#include "ukmd_utils.h"
#include "ukmd_log.h"
#define RETEY_TIME 100

static uint32_t get_bits(uint32_t val, uint8_t bs, uint8_t bw)
{
	uint32_t mask = (1 << bw) - 1;
	mask <<= bs;
	return (val & mask) >> bs;
}

bool check_addr_status_is_valid(const char __iomem* check_addr, uint32_t status,
	uint32_t udelay_time, uint32_t times)
{
	uint32_t i = 0;
	uint32_t val = 0;
	if (unlikely(!check_addr))
		return false;

	val = inp32(check_addr);

	do {
		ukmd_pr_info("val: %#x ?= %#x", val, status);
		if ((val & status) == status)
			return true;
		udelay(udelay_time);
		val = inp32(check_addr);
	} while (i++ < times);

	return false;
}

uint32_t read_reg_with_retry(char __iomem *reg_addr)
{
	uint32_t value = 0;
	uint32_t try_cnt = 0;

	do {
		value = inp32(reg_addr);
		if (value != DSS_ERR_CODE_DEADBEED)
			return value;
		try_cnt++;
	} while (try_cnt < RETEY_TIME);

    ukmd_pr_err("read reg failed after %d retry, deadbeed still happened!", RETEY_TIME);
	return value;
}

bool wait_reg_state(const char __iomem *addr, const struct ukmd_reg_state *reg_status,
	uint32_t udelay_time, uint32_t try_times)
{
	uint32_t try_cnt = 0;
	if (unlikely(!addr) || unlikely(!reg_status))
		return false;

	do {
		if (get_bits(readl(addr), reg_status->bs, reg_status->bw) == reg_status->expected_val)
			return true;
		try_cnt++;
		udelay(udelay_time);
	} while (try_cnt < try_times);

	return false;
}