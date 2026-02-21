/*
 * npu_pm_non_top_common.c
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
#include <linux/iommu/mm_svm.h>

#include "npu_pm_non_top_common.h"
#include "npu_atf_subsys.h"
#include "npu_pm_internal.h"
#include "npu_log.h"
#include "npu_platform.h"

int npu_aicore_gene_powerup(u32 work_mode, u32 subip_set)
{
	int ret;
	unused(subip_set);

	npu_drv_warn("aicore gene power up start\n");

	ret = atfd_service_npu_smc(NPU_POWER_UP_NON_TOP_SPECIFY,
		NPU_ATF_PM_VOTE_REG_FIELD_LITE_GENE, work_mode, 0);
	if (ret) {
		npu_drv_err("aicore gene power up failed, ret:%d", ret);
		return ret;
	}

	npu_drv_warn("aicore gene power up succ\n");
	return ret;
}

int npu_aicore_gene_powerdown(u32 work_mode, u32 subip_set)
{
	int ret;
	unused(subip_set);

	npu_drv_warn("aicore gene power down start\n");

	ret = atfd_service_npu_smc(NPU_POWER_DOWN_NON_TOP_SPECIFY,
		NPU_ATF_PM_VOTE_REG_FIELD_LITE_GENE, work_mode, 0);
	if (ret)
		npu_drv_err("aicore gene power down failed, ret:%d", ret);

	npu_drv_warn("aicore gene power down succ\n");
	return ret;
}

