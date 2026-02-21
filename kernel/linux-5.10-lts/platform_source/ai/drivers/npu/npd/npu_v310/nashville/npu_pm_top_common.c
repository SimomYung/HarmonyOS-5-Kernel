/*
 * npu_pm_top_common.c
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

#include "npu_pm_top_common.h"
#include "npu_log.h"
#include "npu_arch_exception.h"
#include "npu_except_mgr.h"
#include "npu_atf_subsys.h"
#include "npu_platform.h"
#include "npu_pm_internal.h"
#include <platform_include/cee/linux/npu_pm.h>
#include <linux/iommu/mm_svm.h>

int npu_top_powerup(u32 work_mode, u32 subip_set)
{
	int ret = 0;
	int32_t except_bits;
	struct npu_platform_info *plat_info = NULL;
	unused(subip_set);
	plat_info = npu_plat_get_info();
	cond_return_error(plat_info == NULL, -1, "get platform info failed\n");

	npu_drv_warn("top common power up start\n");
	npu_drv_boot_time_tag("start top common power up\n");

	except_bits = npu_get_exception_bits();
	if (except_bits != 0) {
		npu_drv_err("get exception bits failed, ret:%d", except_bits);
		return -NPU_EXCEPTION_GLOBAL;
	}

#ifndef NPU_POWER_UP_SELF_CONTROL
	npu_drv_warn("call npu_pm_power_on\n");
	ret = npu_pm_power_on();
	cond_return_error(ret != 0, ret, "top common power up failed, ret:%d\n", ret);
#endif
	ret = atfd_service_npu_smc(NPU_POWER_UP_TOP_SPECIFY, 0, work_mode, 0);
	cond_goto_error(ret != 0, TOP_SPECIFY_UP_FAIL, ret, ret,
		"top specify power up failed, ret:%d\n", ret);

	npu_drv_warn("mm_smmu_poweron start\n");
	ret = mm_smmu_poweron(plat_info->pdev);
	cond_goto_error(ret != 0, SMMU_POWER_ON_FAIL, ret, ret,
		"smmu poweron failed, ret:%d\n", ret);

	npu_drv_warn("top common power up succ\n");
	return ret;
SMMU_POWER_ON_FAIL:
	(void)atfd_service_npu_smc(NPU_POWER_DOWN_TOP_SPECIFY, 0, work_mode, 0);
TOP_SPECIFY_UP_FAIL:
#ifndef NPU_POWER_UP_SELF_CONTROL
	(void)npu_pm_power_off();
#endif
	return ret;
}

int npu_top_powerdown(u32 work_mode, u32 subip_set)
{
	int ret = 0;
	struct npu_platform_info *plat_info = NULL;
	unused(subip_set);

	npu_drv_warn("top common power down start\n");

	plat_info = npu_plat_get_info();
	cond_return_error(plat_info == NULL, -1, "get platform info failed\n");
	(void)mm_smmu_poweroff(plat_info->pdev);

	ret = atfd_service_npu_smc(NPU_POWER_DOWN_TOP_SPECIFY, 0, work_mode, 0);
	if (ret != 0)
		npu_drv_err("top specify power down failed, ret:%d", ret);

#ifndef NPU_POWER_UP_SELF_CONTROL
	ret = npu_pm_power_off();
	if (ret) {
		npu_drv_err("top common power down failed, ret:%d", ret);
		return ret;
	}
#endif

	npu_clear_exception_bits();

	npu_drv_warn("top common power down succ\n");
	return ret;
}
