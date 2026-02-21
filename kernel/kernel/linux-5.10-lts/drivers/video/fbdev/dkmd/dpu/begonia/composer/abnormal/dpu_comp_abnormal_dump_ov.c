/**
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2025. All rights reserved.
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
#include <dpu/soc_dpu_define.h>
#include <dpu/soc_dpu_interface.h>
#include "dkmd_log.h"
#include "dkmd_object.h"
#include "dpu_debug_dump.h"

void dpu_comp_abnormal_dump_ov_info(char __iomem *dpu_base)
{
	char __iomem *module_base = dpu_base + DPU_GLB0_OFFSET;

	g_dpu_dsm_info.ov_cnt[0] = inp32(DPU_GLB_DBG_OV_YCNT0_ADDR(module_base));
	g_dpu_dsm_info.ov_cnt[1] = inp32(DPU_GLB_DBG_OV_YCNT1_ADDR(module_base));
	g_dpu_dsm_info.ov_cnt[2] = inp32(DPU_GLB_DBG_OV_YCNT2_ADDR(module_base));
	g_dpu_dsm_info.ov_cnt[3] = inp32(DPU_GLB_DBG_OV_YCNT3_ADDR(module_base));
	dpu_pr_warn("OV:\n"
		"\tDBG_OV_YCNT0=%#x\tDBG_OV_YCNT1=%#x\tDBG_OV_YCNT2=%#x\tDBG_OV_YCNT3=%#x\tDSI_VCOUNT=%#x\n\n",
		g_dpu_dsm_info.ov_cnt[0], g_dpu_dsm_info.ov_cnt[1],
		g_dpu_dsm_info.ov_cnt[2], g_dpu_dsm_info.ov_cnt[3],
		inp32(DPU_DSI_VCOUNT_ADDR(dpu_base)));
	udelay(10);
	dpu_pr_warn("OV:\n"
		"\tDBG_OV_YCNT0=%#x\tDBG_OV_YCNT1=%#x\tDBG_OV_YCNT2=%#x\tDBG_OV_YCNT3=%#x\tDSI_VCOUNT=%#x\n\n",
		inp32(DPU_GLB_DBG_OV_YCNT0_ADDR(module_base)), inp32(DPU_GLB_DBG_OV_YCNT1_ADDR(module_base)),
		inp32(DPU_GLB_DBG_OV_YCNT2_ADDR(module_base)), inp32(DPU_GLB_DBG_OV_YCNT3_ADDR(module_base)),
		inp32(DPU_DSI_VCOUNT_ADDR(dpu_base)));
	udelay(10);
	dpu_pr_warn("OV:\n"
		"\tDBG_OV_YCNT0=%#x\tDBG_OV_YCNT1=%#x\tDBG_OV_YCNT2=%#x\tDBG_OV_YCNT3=%#x\tDSI_VCOUNT=%#x\n\n",
		inp32(DPU_GLB_DBG_OV_YCNT0_ADDR(module_base)), inp32(DPU_GLB_DBG_OV_YCNT1_ADDR(module_base)),
		inp32(DPU_GLB_DBG_OV_YCNT2_ADDR(module_base)), inp32(DPU_GLB_DBG_OV_YCNT3_ADDR(module_base)),
		inp32(DPU_DSI_VCOUNT_ADDR(dpu_base)));
}