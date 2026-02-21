/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 *
 * Header file for device driverPMIC
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
#ifndef __PMIC_OC_LEVEL_H__
#define __PMIC_OC_LEVEL_H__


#define OC_LEVEL_ENABLE 0x10
#define OC_LEVEL_DISABLE 0x0
#define OC_ARR_COUNT 3

struct oc_level_arrinf {
	unsigned int oc_reg;
	unsigned int oc_mask;
	unsigned int oc_enb;
};
struct pmic_oc_level {
	struct device *dev;
	struct regmap *regmap;
	struct oc_level_arrinf *oc_arr;
	unsigned int *oc_level_record_regs;
	unsigned int oc_level_cnt;
	unsigned int oc_level_record_num;
	struct delayed_work oc_level_get_work;
};

void pmic_ocl_work_time(int val);

#endif