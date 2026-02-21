/*
 * npu_pm_non_top_common.h
 *
 * about npu hwts plat
 *
 * Copyright (c) 2023-2023 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */

#ifndef __NPU_PM_NON_TOP_COMMON_H
#define __NPU_PM_NON_TOP_COMMON_H
#include "npu_common.h"

#define NPU_ATF_PM_VOTE_REG_FIELD_LITE_GENE   2
#define NPU_ATF_PM_VOTE_REG_FIELD_TINY_GENE   3

int npu_aicore_gene_powerup(u32 work_mode, u32 subip_set);

int npu_aicore_gene_powerdown(u32 work_mode, u32 subip_set);

#endif