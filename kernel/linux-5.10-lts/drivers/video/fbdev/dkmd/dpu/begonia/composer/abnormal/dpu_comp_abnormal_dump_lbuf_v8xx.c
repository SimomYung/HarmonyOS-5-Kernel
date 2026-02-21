/**
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
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
#include "dpu/dpu_base_addr.h"
#include <linux/delay.h>
#include <dpu/dpu_dm.h>
#include <linux/interrupt.h>
#include "dpu_comp_mgr.h"
#include "dpu_comp_vactive.h"
#include "dpu_comp_abnormal_handle.h"
#include "dpu_comp_smmu.h"
#include "dpu_opr_config.h"
#include "dpu_config_utils.h"
#include "dksm_debug.h"
#include "dpu_config_utils.h"
#include "dpu_conn_mgr.h"
#include "dpu_debug_dump.h"

// range example:[DM_SCENE_INFO_OFFSET, DM_UVUP_OFFSET)
void dpu_comp_abnormal_dump_lbuf(char __iomem *dpu_base)
{
	char __iomem *module_base0 = NULL;
	char __iomem *module_base1 = NULL;
	char __iomem *module_base3 = NULL;
	module_base0 = dpu_base + DPU_GLB0_OFFSET;
	module_base1 = dpu_base + DPU_AVHR_GLB_OFFSET;
	module_base3 = dpu_base + DPU_DISP_GLB_OFFSET;
 
	dpu_pr_warn("LBUF:\n"
		"LB0 info: LBUF0_RF_USE_NUM=%#x, LBUF0_DBG_RF_NUM=%#x, LBUF0_DBG_RF_NUM1=%#x, LBUF0_DBG_FRM_CNT=%#x\n"
		"LB1 info: LBUF1_RF_USE_NUM=%#x, LBUF1_DBG_RF_NUM=%#x, LBUF1_DBG_RF_NUM1=%#x, LBUF1_DBG_FRM_CNT=%#x\n"
		"LB2 info: LBUF2_RF_USE_NUM=%#x, LBUF2_DBG_RF_NUM=%#x, LBUF2_DBG_RF_NUM1=%#x, LBUF2_DBG_FRM_CNT=%#x\n"
		"LB3 info: LBUF3_RF_USE_NUM=%#x, LBUF3_DBG_RF_NUM=%#x, LBUF3_DBG_RF_NUM1=%#x, LBUF3_DBG_FRM_CNT=%#x\n",
		inp32(DPU_GLB_LBUF_RF_USE_NUM_ADDR(module_base0)),
		inp32(DPU_GLB_LBUF_DBG_RF_NUM_ADDR(module_base0)),
		inp32(DPU_GLB_LBUF_DBG_RF_NUM1_ADDR(module_base0)),
		inp32(DPU_GLB_LBUF_DBG_FRM_CNT_ADDR(module_base0)),
		inp32(DPU_AVHR_GLB_LBUF1_RF_USE_NUM_ADDR(module_base1)),
		inp32(DPU_AVHR_GLB_LBUF1_DBG_RF_NUM_ADDR(module_base1)),
		inp32(DPU_AVHR_GLB_LBUF1_DBG_RF_NUM1_ADDR(module_base1)),
		inp32(DPU_AVHR_GLB_LBUF1_DBG_FRM_CNT_ADDR(module_base1)),
		inp32(DPU_AVHR_GLB_LBUF2_RF_USE_NUM_ADDR(module_base1)),
		inp32(DPU_AVHR_GLB_LBUF2_DBG_RF_NUM_ADDR(module_base1)),
		inp32(DPU_AVHR_GLB_LBUF2_DBG_RF_NUM1_ADDR(module_base1)),
		inp32(DPU_AVHR_GLB_LBUF2_DBG_FRM_CNT_ADDR(module_base1)),
		inp32(DPU_DISP_GLB_LBUF_RF_USE_NUM_ADDR(module_base3)),
		inp32(DPU_DISP_GLB_LBUF_DBG_RF_NUM_ADDR(module_base3)),
		inp32(DPU_DISP_GLB_LBUF_DBG_RF_NUM1_ADDR(module_base3)),
		inp32(DPU_DISP_GLB_LBUF_DBG_FRM_CNT_ADDR(module_base3)));

	dpu_pr_warn("INFO SRC:\n"
		"SCENE0_INFO_DEBUG0=%#x, SCENE1_INFO_DEBUG0=%#x, SCENE2_INFO_DEBUG0=%#x, SCENE4_INFO_DEBUG0=%#x\n",
		inp32(DPU_GLB_SCENE0_INFO_DEBUG0_ADDR(module_base0)),
		inp32(DPU_GLB_SCENE1_INFO_DEBUG0_ADDR(module_base0)),
		inp32(DPU_GLB_SCENE2_INFO_DEBUG0_ADDR(module_base0)),
		inp32(DPU_GLB_SCENE4_INFO_DEBUG0_ADDR(module_base0)));
	g_dpu_dsm_info.info_src = inp32(DPU_GLB_SCENE0_INFO_DEBUG0_ADDR(module_base0));
	dpu_pr_warn("PIPE CTRL:\n"
		"SDMA_PIPE_CTRL:DBG6=%#x, DBG7=%#x, DBG8=%#x, DBG9=%#x\n"
		"OV_PIPE_CTRL:DBG7=%#x, DBG10=%#x\n",
		inp32(DPU_PIPE_CTRL_PIPE_DBG6_ADDR(dpu_base + DPU_SDMA_PIPE_CTRL_OFFSET)),
		inp32(DPU_PIPE_CTRL_PIPE_DBG7_ADDR(dpu_base + DPU_SDMA_PIPE_CTRL_OFFSET)),
		inp32(DPU_PIPE_CTRL_PIPE_DBG8_ADDR(dpu_base + DPU_SDMA_PIPE_CTRL_OFFSET)),
		inp32(DPU_PIPE_CTRL_PIPE_DBG9_ADDR(dpu_base + DPU_SDMA_PIPE_CTRL_OFFSET)),
		inp32(DPU_PIPE_CTRL_PIPE_DBG7_ADDR(dpu_base + DPU_OV_PIPE_CTRL_OFFSET)),
		inp32(DPU_PIPE_CTRL_PIPE_DBG10_ADDR(dpu_base + DPU_OV_PIPE_CTRL_OFFSET)));
}
