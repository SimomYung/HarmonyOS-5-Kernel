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
#include <dpu/soc_dpu_define.h>
#include <dpu/soc_dpu_interface.h>
#include <dpu/dpu_dm.h>
#include "dpu_config_utils.h"
#include "dkmd_log.h"
#include "dkmd_object.h"
#include "dpu_debug_dump.h"

void dpu_comp_abnormal_dump_sdma_info(char __iomem *module_base)
{
	uint32_t i;
 
	for (i = 0; i < OPR_SDMA_NUM; ++i) {
		dpu_pr_warn("SDMA[%u]:\n"
			"\tSDMA_DBG_OUT0=%#x\n"
			"\tSDMA_DBG_MONITOR0=%#x\t\tSDMA_DBG_MONITOR1=%#x\n\tSDMA_DBG_MONITOR2=%#x\t\tSDMA_DBG_MONITOR3=%#x\n"
			"\tSDMA_DBG_MONITOR4=%#x\t\tSDMA_DBG_MONITOR5=%#x\n\tSDMA_W_DBG0=%#x\t\tSDMA_W_DBG1=%#x\n"
			"\tSDMA_W_DBG2=%#x\t\tSDMA_W_DBG3=%#x\tSDMA_CTRL=%#x\t\n\n", i,
			inp32(DPU_GLB_SDMA_DBG_OUT0_ADDR(module_base, i)), /* bit0-14:sdma cmd num that not get ACK from bus */
			inp32(DPU_GLB_SDMA_DBG_MONITOR0_ADDR(module_base, i)), inp32(DPU_GLB_SDMA_DBG_MONITOR1_ADDR(module_base, i)),
			inp32(DPU_GLB_SDMA_DBG_MONITOR2_ADDR(module_base, i)), inp32(DPU_GLB_SDMA_DBG_MONITOR3_ADDR(module_base, i)),
			inp32(DPU_GLB_SDMA_DBG_MONITOR4_ADDR(module_base, i)), inp32(DPU_GLB_SDMA_DBG_MONITOR5_ADDR(module_base, i)),
			inp32(DPU_GLB_SDMA_W_DBG0_ADDR(module_base, i)), inp32(DPU_GLB_SDMA_W_DBG1_ADDR(module_base, i)),
			inp32(DPU_GLB_SDMA_W_DBG2_ADDR(module_base, i)), inp32(DPU_GLB_SDMA_W_DBG3_ADDR(module_base, i)),
			inp32(DPU_GLB_SDMA_CTRL0_ADDR(module_base, i)));
 
		if (((inp32(DPU_GLB_SDMA_DBG_MONITOR4_ADDR(module_base, i)) & 0x3fff) != 0) && (
		    (g_dpu_config_data.version.info.version == DPU_ACCEL_DPUV800) || (g_dpu_config_data.version.info.version == DPU_ACCEL_DPUV820))) {
			g_dpu_dsm_info.hebc_err = inp32(DPU_GLB_SDMA_DBG_MONITOR4_ADDR(module_base, i)) & 0x3fff;
			dpu_pr_warn("pls check hebc status: smda[%d]_mon[0x%x]", i, g_dpu_dsm_info.hebc_err);
		}
	}
}