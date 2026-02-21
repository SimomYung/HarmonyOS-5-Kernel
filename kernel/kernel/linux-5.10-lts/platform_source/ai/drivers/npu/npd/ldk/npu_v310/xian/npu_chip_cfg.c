/*
 * npu_chip_cfg.c
 *
 * about chip config
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
 */

#include <linux/io.h>
#include <npu_shm_config.h>
#include "npu_log.h"
#include "npu_common.h"
#include "npu_platform.h"
#include "npu_atf_subsys.h"
#include "npu_adapter.h"
#include "soc_spec.h"
#include "soc_spec_info.h"
#include "soc_spec_common.h"

/*
 * return value : 1 support ispnn; 0 not support ispnn
 */
bool npu_plat_is_support_ispnn(void)
{
	return false;
}

/*
 * return value : 1 support sc; 0 not support sc
 */
bool npu_plat_is_support_sc(void)
{
	return true;
}

/*
 * return value : NPU_NON_BYPASS, NPU_BYPASS
 */
int npu_plat_bypass_status(void)
{
#ifdef CONFIG_SOC_PG_INFO
	struct soc_component_pg_info pg_info = {0};
	get_soc_pg_info_by_index(SOC_COMPONENT_NPU_INFO, &pg_info);
	npu_drv_warn("[SOC_PG_INFO]efuse_value[%u], tailor_strategy[%u]", pg_info.efuse_value, pg_info.tailor_strategy);
	if (pg_info.tailor_strategy != SOC_COMPONENT_NOT_TAILOR &&
        pg_info.tailor_strategy != SOC_COMPONENT_TAILOR_STRATEGY_2) {
		npu_drv_warn("[SOC_PG_INFO]npu plat bypass");
		return NPU_BYPASS;
	}
#endif
	return NPU_NON_BYPASS;
}
