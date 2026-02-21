/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 *
 * Header file for device driver PMIC DIE CRACK CHECK
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 */
#ifndef __PMIC_DIE_CRACK_CHECK_H__
#define __PMIC_DIE_CRACK_CHECK_H__

#define ENB_CNT 4
#define RESULT_CNT 3

struct enable_regs_info {
	unsigned int enable_reg;
	unsigned int enable_mask;
	unsigned int enable_val;
	unsigned int disable_val;
	unsigned int default_val;
};

struct result_regs_info {
	unsigned int result_reg;
	unsigned int expect_val;
	unsigned int result_mask;
};

struct pmic_die_crack_check {
	struct device *dev;
	struct regmap *regmap;
	struct enable_regs_info *enable_regs;
	unsigned int enable_regs_num;
	struct result_regs_info *result_regs;
	unsigned int result_regs_num;
	const char *pmu_name;
	unsigned int slave_id;
	struct delayed_work die_crack_check_work;
};

#endif