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