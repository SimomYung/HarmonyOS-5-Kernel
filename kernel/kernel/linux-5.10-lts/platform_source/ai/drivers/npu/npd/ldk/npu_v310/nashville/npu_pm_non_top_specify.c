/*
 * npu_pm_non_top_specify.c
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

#include "npu_pm_non_top_specify.h"

#include <linux/random.h>
#include <linux/interrupt.h>
#include "npu_atf_subsys.h"
#include "npu_pm_internal.h"
#include "npu_platform.h"
#include "npu_log.h"
#include "bbox/npu_dfx_black_box.h"

int npu_aicore_eff_powerup(u32 work_mode, u32 subip_set)
{
	int ret;
	unused(subip_set);

	npu_drv_warn("aicore eff power up start\n");

	ret = atfd_service_npu_smc(NPU_POWER_UP_NON_TOP_SPECIFY,
		NPU_ATF_PM_VOTE_REG_FIELD_LITE_EFF, work_mode, 0);
	if (ret != 0)
		npu_drv_err("aicore eff power up  failed, ret:%d\n", ret);
	else
		npu_drv_warn("aicore eff power up succ\n");

	return ret;
}

int npu_aicore_eff_powerdown(u32 work_mode, u32 subip_set)
{
	int ret;
	unused(subip_set);

	npu_drv_warn("aicore eff power down start\n");

	ret = atfd_service_npu_smc(NPU_POWER_DOWN_NON_TOP_SPECIFY,
		NPU_ATF_PM_VOTE_REG_FIELD_LITE_EFF, work_mode, 0);
	if (ret != 0)
		npu_drv_err("aicore eff power down failed, ret:%d\n", ret);
	else
		npu_drv_warn("aicore eff power down succ\n");

	return ret;
}

int npu_tscpu_powerup(u32 work_mode, u32 subip_set)
{
	int ret;
	u64 canary = 0;
	struct npu_platform_info *plat_info = NULL;
	unused(subip_set);

	plat_info = npu_plat_get_info();
	cond_return_error(plat_info == NULL, -1, "get platform info failed\n");

	npu_drv_warn("tscpu nosec power up start\n");

	get_random_bytes(&canary, sizeof(canary));
	if (canary == 0)
		get_random_bytes(&canary, sizeof(canary));

	ret = atfd_service_npu_smc(NPU_POWER_UP_NON_TOP_SPECIFY_NOSEC, 0, work_mode, canary);
	if (ret != 0) {
		npu_drv_err("tscpu nosec power up failed, ret:%d\n", ret);
		return ret;
	} else {
		npu_drv_debug("tscpu nosec power up succ\n");
	}

	enable_irq((unsigned int)(plat_info->dts_info.irq_cq_update));
	enable_irq((unsigned int)(plat_info->dts_info.irq_mailbox_ack));
	enable_irq((unsigned int)(plat_info->dts_info.irq_dfx_cq));
	enable_irq((unsigned int)(plat_info->dts_info.irq_smartisp_ack));

	if (npu_sync_ts_time() != 0)
		npu_drv_warn("npu_sync_ts_time fail.\n");

	npu_drv_warn("tscpu nosec power up succ\n");
	return ret;
}

int npu_tscpu_powerdown(u32 work_mode, u32 subip_set)
{
	int ts_ret;
	int ret;
	struct npu_platform_info *plat_info = NULL;
	unused(subip_set);

	plat_info = npu_plat_get_info();
	cond_return_error(plat_info == NULL, -1, "get platform info failed\n");

	npu_drv_warn("tscpu nosec power down start\n");

	ts_ret = npu_plat_powerdown_ts(0, work_mode);
	if (ts_ret != 0)
		npu_drv_err("powerdown ts failed, ts_ret:%d\n", ts_ret);

	disable_irq((unsigned int)(plat_info->dts_info.irq_cq_update));
	disable_irq((unsigned int)(plat_info->dts_info.irq_mailbox_ack));
	disable_irq((unsigned int)(plat_info->dts_info.irq_dfx_cq));
	disable_irq((unsigned int)(plat_info->dts_info.irq_smartisp_ack));

	ret = atfd_service_npu_smc(NPU_POWER_DOWN_NON_TOP_SPECIFY_NOSEC, 0, work_mode, 0);
	if (ret != 0)
		npu_drv_err("tscpu nosec power down failed, ret:%d\n", ret);
	else
		npu_drv_warn("tscpu nosec power down succ\n");

	ret += ts_ret;
	return ret;
}