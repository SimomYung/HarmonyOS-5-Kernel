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
#include <linux/delay.h>
#include <dpu/dpu_dm.h>
#include <linux/interrupt.h>
#include "dpu/dpu_base_addr.h"
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

void dpu_comp_abnormal_dump_dbuf(char __iomem *dpu_base, uint32_t scene_id)
{
	(void)scene_id;
	dpu_check_and_no_retval(!dpu_base, err, "dpu_base is NULL!\n");

	// waterline-related registers
	dpu_pr_warn("DISP: dbuf freq: debuf0_lev_cnt=%#x.\t debuf1_lev_cnt=%#x. \t debuf2_lev_cnt=%#x.", 
		inp32(DPU_DISP_GLB_DBUF0_STATE_ADDR(dpu_base + DPU_DISP_GLB_OFFSET)), 
		inp32(DPU_DISP_GLB_DBUF1_STATE_ADDR(dpu_base + DPU_DISP_GLB_OFFSET)),
		inp32(DPU_DISP_GLB_DBUF2_STATE_ADDR(dpu_base + DPU_DISP_GLB_OFFSET)));
	
	// waterline status
	dpu_pr_warn("LBUF: dbuf0 trigger fluex_req, state=0x%x, ctrl=0x%x, fluex_req_bef=0x%x, fluex_req_aft=0x%x",
		inp32(DPU_LBUF_DBUF0_STATE_ADDR(dpu_base + DPU_LBUF_OFFSET)),
		inp32(DPU_LBUF_DBUF0_CTRL_ADDR(dpu_base + DPU_LBUF_OFFSET)),
		inp32(DPU_LBUF_DBUF0_THD_FLUX_REQ_BEF_ADDR(dpu_base + DPU_LBUF_OFFSET)),
		inp32(DPU_LBUF_DBUF0_THD_FLUX_REQ_AFT_ADDR(dpu_base + DPU_LBUF_OFFSET)));
	
	// online waterline LBUF
	dpu_pr_warn("GLB DUBF DEBUG LB config online:\n"
		"\t SCENE0_LAYER_SIZE: 0x%x\t SCENE0_DBUF_DEPTH: 0x%x\t SCENE0_PRE_OS_NUM: 0x%x\t, SCENE0_INFO_SEL: 0x%x\n" 
		"\t SCENE0_INFO_PAUSE_NUM: 0x%x",
		inp32(DPU_GLB_SCENE0_LAYER_SIZE_ADDR(dpu_base + DPU_GLB0_OFFSET)),
		inp32(DPU_GLB_SCENE0_DBUF_DEPTH_ADDR(dpu_base + DPU_GLB0_OFFSET)),
		inp32(DPU_GLB_SCENE0_PRE_OS_NUM_ADDR(dpu_base + DPU_GLB0_OFFSET)),
		inp32(DPU_GLB_SCENE0_INFO_SEL_ADDR(dpu_base + DPU_GLB0_OFFSET)),
		inp32(DPU_GLB_SCENE0_INFO_PAUSE_NUM_ADDR(dpu_base + DPU_GLB0_OFFSET)));
 
	// offline waterline LBUF
	dpu_pr_warn("DEBUG LB config: offline: \n"
		"\t SCENE0_LAYER_SIZE: 0x%x\t SCENE0_DBUF_DEPTH: 0x%x\t SCENE0_PRE_OS_NUM: 0x%x\t \n", 
		"\t SCENE0_INFO_SEL: 0x%x, SCENE0_INFO_PAUSE_NUM: 0x%x",
		inp32(DPU_GLB_SCENE4_LAYER_SIZE_ADDR(dpu_base + DPU_GLB0_OFFSET)),
		inp32(DPU_GLB_SCENE4_DBUF_DEPTH_ADDR(dpu_base + DPU_GLB0_OFFSET)),
		inp32(DPU_GLB_SCENE4_PRE_OS_NUM_ADDR(dpu_base + DPU_GLB0_OFFSET)),
		inp32(DPU_GLB_SCENE4_INFO_SEL_ADDR(dpu_base + DPU_GLB0_OFFSET)),
		inp32(DPU_GLB_SCENE4_INFO_PAUSE_NUM_ADDR(dpu_base + DPU_GLB0_OFFSET)));
}

void dpu_comp_abnormal_dump_pll(struct dpu_composer *dpu_comp)
{
	struct dpu_connector *connector = NULL;
	char __iomem *media1_crg_base = NULL;
	char __iomem *periph_crg_base = NULL;

	media1_crg_base = dpu_config_get_ip_base(DISP_IP_BASE_MEDIA1_CRG);
	dpu_check_and_no_retval(!media1_crg_base, err, "media1 crg addr is NULL!\n");
	dpu_pr_warn("Media1 crg addr: CLK_DIV0: 0x%x\t CLK_DIV2: 0x%x\t DSS_DVFS: 0x%x\t, HW_DVFS:0x%x\t",
		inp32(SOC_MEDIA1_CRG_CLKDIV0_ADDR(media1_crg_base)),
		inp32(SOC_MEDIA1_CRG_CLKDIV2_ADDR(media1_crg_base)),
		inp32(SOC_MEDIA1_CRG_DSS_DVFS_ADDR(media1_crg_base)),
		inp32(SOC_MEDIA1_CRG_DSS_DVFS_AUTODIV_ADDR(media1_crg_base)));

	dpu_check_and_no_retval(!dpu_comp, err, "dpu_comp is NULL!\n");
	connector = get_primary_connector(dpu_comp->conn_info);
	dpu_check_and_no_retval(!connector, err, "connector is NULL!\n");
	periph_crg_base = connector->peri_crg_base;
	dpu_check_and_no_retval(!periph_crg_base, err, "peri crg addr is NULL!\n");
	dpu_pr_warn("CRG Periph PLL: \n"
		"\tPLL_FSM_NS_VOTE0 0x%x\t PLL_FSM_NS_VOTE1: 0x%x\t PLL_EN_CTRL1: 0x%x\t PLL_EN_CTRL2: 0x%x\t \n"
		"\t PPLL2_B CTRL0: 0x%x\t, PPLL2_B CTRL1: 0x%x\t, PERI_CRG: 0x%x\t 0x%x\t 0x%x\t 0x%x\t 0x%x\t \n",
		inp32(SOC_CRGPERIPH_PLL_FSM_NS_VOTE0_ADDR(periph_crg_base)),
		inp32(SOC_CRGPERIPH_PLL_FSM_NS_VOTE1_ADDR(periph_crg_base)),
		inp32(SOC_CRGPERIPH_PLL_EN_VOTE_CTRL1_ADDR(periph_crg_base)),
		inp32(SOC_CRGPERIPH_PLL_EN_VOTE_CTRL2_ADDR(periph_crg_base)),
		inp32(SOC_CRGPERIPH_PPLL2_BCTRL0_ADDR(periph_crg_base)),
		inp32(SOC_CRGPERIPH_PPLL2_BCTRL1_ADDR(periph_crg_base)),
		inp32(SOC_CRGPERIPH_HW_DSS_DVFS_STAT_ADDR(periph_crg_base)),
		inp32(SOC_CRGPERIPH_PLL_FSM_CTRL1_ADDR(periph_crg_base)),
		inp32(SOC_CRGPERIPH_PLL_FSM_STAT0_ADDR(periph_crg_base)),
		inp32(SOC_CRGPERIPH_PLL_FSM_STAT1_ADDR(periph_crg_base)),
		inp32(SOC_CRGPERIPH_PLL_VOTE_STAT0_ADDR(periph_crg_base)));
}