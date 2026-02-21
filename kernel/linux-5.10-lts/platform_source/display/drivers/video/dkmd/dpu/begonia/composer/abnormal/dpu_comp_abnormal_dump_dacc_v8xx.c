/**
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
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

#include <linux/io.h>
#include "dpu/soc_dpu_define.h"
#include "dkmd_object.h"
#include "dpu_debug_dump.h"

void dpu_comp_abnormal_dump_dacc(char __iomem *dpu_base, uint32_t scene_id)
{
	char __iomem *module_base = dpu_base + DACC_OFFSET + DMC_OFFSET;
	void_unused(scene_id);

	g_dpu_dsm_info.dacc_pc = inp32(SOC_DACC_CORE_MON_PC_REG_ADDR(module_base));
	dpu_pr_warn("DACC:\n\n"
		"CORE_STATUS_REG=%#x\t\tDACC_CORE_MON_PC_REG=%#x\n\n",
		inp32(SOC_DACC_CORE_STATUS_REG_ADDR(module_base)), g_dpu_dsm_info.dacc_pc);
}