/**
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
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

#include "dkmd_log.h"
#include "dkmd_object.h"
#include "dkmd_res_mgr.h"
#include "cmdlist_interface.h"
#include "dpu_comp_config_utils.h"
#include <dpu/dpu_lbuf.h>
#include <dpu/soc_dpu_define.h>
#include "soc_pctrl_interface.h"
#include "doze.h"
#include "dpu_config_utils.h"
#include "dpu_panel_partial_ctl.h"

#define GRAY_CLOCK 640

struct swid_info g_sdma_swid_tlb_info[SDMA_SWID_NUM] = {
	{DPU_SCENE_ONLINE_0,   0,  21},
	{DPU_SCENE_ONLINE_1,  22,  39},
	{DPU_SCENE_ONLINE_2,  40,  47},
	{DPU_SCENE_OFFLINE_0, 50,  57},
};

struct swid_info g_wch_swid_tlb_info[WCH_SWID_NUM] = {
	{DPU_SCENE_ONLINE_0,  49,  49},
	{DPU_SCENE_OFFLINE_0, 58,  58},
};

typedef union
{
	uint32_t value;
	struct
	{
		uint32_t te_interval_0  :8;
		uint32_t te_interval_1  :8;
		uint32_t te_interval_2  :8;
		uint32_t te_interval_3  :8;
	} param;
} te_interval;

void dpu_comp_wch_axi_sel_set_reg(char __iomem *dpu_base)
{
	void_unused(dpu_base);
}

char __iomem *dpu_comp_get_tbu1_base(char __iomem *dpu_base)
{
	if (dpu_base == NULL) {
		dpu_pr_err("dpu_base is nullptr!\n");
		return NULL;
	}

	return dpu_base + DPU_SMMU1_OFFSET;
}

void dpu_level1_clock_lp(uint32_t cmdlist_id)
{
	uint32_t cmdlist_dev_id = CMDLIST_DEV_ID_DPU;
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x00013100, 0x00200000); // TOP_ GLB
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x00088000, 0x00400000); // AVHR_GLB
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x000D8000, 0x00000000); // DISP_GLB
}

void dpu_level2_clock_lp(uint32_t cmdlist_id)
{
	uint32_t cmdlist_dev_id = CMDLIST_DEV_ID_DPU;
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x00012740, 0x00000000); // CMD
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x0006C0E4, 0x00000000); // WCH0
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x0006D0E4, 0x00000000); // WCH1
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x00019868, 0x00000000); // DBCU
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x0001986C, 0x00000000); // DBCU

	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x00015004, 0x00000000); // PIPE_CTRL0 (SDMA)
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x0001A004, 0x00000000); // PIPE_CTRL1 (PRE_WRAP)
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x0001B004, 0x00000000); // PIPE_CTRL 2 (OV)
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x000D6C04, 0x00000000); // PIPE_CTRL 3 (POST_WRAP)
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x00016004, 0x00000000); // PIPE_CTRL4 (ITF)
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x00013A00, 0x00000000); // TOP_GLB (LBUF_CTRL0)
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x00013A80, 0x00000001); // TOP_GLB (LBUF_PART0)
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x00088100, 0x00000000); // AVHR_GLB (LBUF_CTL1)
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x00088180, 0x00000000); // AVHR_GLB (LBUF_PART1)
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x00088200, 0x00000000); // AVHR_GLB (LBUF_CTL2)
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x00088280, 0x00000000); // AVHR_GLB (LBUF_PART2)
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x000D8100, 0x00000000); // DISP_GLB (LBUF_CTL3)
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x000D8200, 0x00000000); // DISP_GLB (LBUF_PART3)

	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x00017660, 0x00000000); // LBUF_CMN_WRAP DBUF

	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x00017250, 0x00000000); // RD_CTRL(ITF)
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x00018A50, 0x00000000); // RD_CTRL(PRE_WRAP)
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x00018E50, 0x00000000); // RD_CTRL(OV)
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x000D7E50, 0x00000000); // RD_CTRL(PST_WRAP)

	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x000088400, 0x00000000); // AVHR_GLB(WR_CTRL1)
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x000088408, 0x00000000); // AVHR_GLB(WR_CTRL2)
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x000D8300, 0x00000000); // DISP_GLB(WR_CTRL3)
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x000CD008, 0x00000000); // ARSR0
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x00078008, 0x00000000); // ARSR1

	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x0007F008, 0x00000000); // VSCF0
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x00083008, 0x00000000); // VSCF1

	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x0008D008, 0x00000000); // CLD0
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x0001C000, 0x00000000); // RCH_OV0
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x0001C008, 0x00000000); // RCH_OV1
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x0001C010, 0x00000000); // RCH_OV2
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x0001C018, 0x00000000); // RCH_OV3
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x0001C020, 0x00000000); // RCH_OV WRAP

	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x000B4008, 0x00004000); // DPP0
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x000DD008, 0x00004000); // DPP1

	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x000B0008, 0x00000000); // DDIC TOP

	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x000D407C, 0x00000000); // DSC0 DIV
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x000D4248, 0x00000000); // DSC0 TOP

	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x0001313C, 0x00000000); // TOP_GLB(DSI/DP)

	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x00040340, 0x00000000); // DACC_CFG (scene0)
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x00040348, 0x00000000); // DACC_CFG (scene1)
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x00040350, 0x00000000); // DACC_CFG (scene2)
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x00040360, 0x00000000); // DACC_CFG (scene4)

	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x00011070, 0x00000001); // ITF_CH0
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x00011170, 0x00000001); // ITF_CH1
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x00011270, 0x00000001); // ITF_CH2
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x000114F4, 0x00000000); // PIPE_SW

	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x00011414, 0x00000000); // PIPE_SW
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x0001144C, 0x00000000); // PIPE_SW
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x00011468, 0x00000000); // PIPE_SW
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x00011484, 0x00000000); // PIPE_SW

	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x0008F008, 0x00000000); // UVSAM_WRAP
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x00013140, 0x0000001D); // GLB_DPI
}

void dpu_ch1_level2_clock_lp(uint32_t cmdlist_id)
{
}

void dpu_memory_lp(uint32_t cmdlist_id)
{
	uint32_t cmdlist_dev_id = CMDLIST_DEV_ID_DPU;
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x00013A08, 0x00000001); // TOP_GLB  (LBUF_CTL0)
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x00088108, 0x00000001); // AVHR_GLB (LBUF_CTL1 )
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x00088208, 0x00000001); // AVHR_GLB (LBUF_CTL2)
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x000D8108, 0x00000001); // DISP_GLB  (LBUF_CTL3)
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x0004003c, 0x00000000); // DACC LB EN
	/* mem clk always open (UFEI), need closed */
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x0006C03C, 0x00000000); // WCH0 DMA MEM
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x0006C040, 0x00000000); // WCH0 DMA MEM
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x0006C218, 0x00000000); // WCH0 SCF MEM
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x0006C290, 0x00000000); // WCH0 SCF_LB_MEM
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x0006C588, 0x00000010); // WCH0 ROT_MEM
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x0006D03C, 0x00000000); // WCH1 DMA MEM
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x0006D040, 0x00000000); // WCH1 DMA MEM
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x0006D218, 0x00000000); // WCH1 SCF MEM
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x0006D290, 0x00000000); // WCH1 SCF_LB_MEM
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x0006D588, 0x00000010); // WCH1 ROT_MEM
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x00013A88, 0x00000000); // TOP_GLB  (LBUF PART0)
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x00088188, 0x00000000); // AVHR_GLB (LBUF PART1)
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x00088288, 0x00000000); // AVHR_GLB (LBUF PART2)
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x000D8208, 0x00000000); // DISP_GLB ( LBUF PART3)
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x000D8280, 0x00000000); // DISP_GLB ( WCH_DBUF)
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x000CD218, 0x00000000); // ARSR0 SCF_COEF MEM
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x000CD290, 0x00000000); // ARSR0 SCF_LB MEM
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x000CD3E8, 0x00000000); // ARSR0 LB MEM
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x000CD3EC, 0x00000000); // ARSR0 COEF MEM

	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x00078218, 0x00000000); // ARSR1 SCF_COEF MEM
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x00078290, 0x00000000); // ARSR1 SCF_LB MEM
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x000783E8, 0x00000000); // ARSR1 LB MEM
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x000783EC, 0x00000000); // ARSR1 COEF MEM

	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x0007F290, 0x00000000); // VSCF0_LB_MEM
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x0007F218, 0x00000000); // VSCF0_COEF_MEM
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x00083290, 0x00000000); // VSCF1
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x00083218, 0x00000000); // VSCF1_COEF_MEM
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x00060238, 0x00000000); // HDR GTM_MEM
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x00060A04, 0x00000000); // HDR DEGAMMA_MEM
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x00060A18, 0x00000000); // HDR GMP_MEM
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x00060A34, 0x00000000); // HDR GAMMA_MEM

	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x0001C060, 0x00000000); // RCH_OV0
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x0001C064, 0x00000000); // RCH_OV1
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x0001C068, 0x00000000); // RCH_OV2
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x0001C06C, 0x00000000); // RCH_OV3
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x000B40B0, 0x00000000); // DPP0 CTC MEM
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x000B4500, 0x00000000); // DPP0 SPR MEM
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x000B4704, 0x00000000); // DPP0 DEGAMMA MEM
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x000B4804, 0x00000000); // DPP0 ROIGAMMA0 MEM
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x000B4904, 0x00000000); // DPP0 ROIGAMMA0 MEM
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x000B49A4, 0x00000000); // DPP0 GMP MEM
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x000B55BC, 0x00000000); // DPP0 DITHER_MEM
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x000B58A4, 0x00000000); // DPP0 SPR_GAMMA MEM
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x000B5904, 0x00000000); // DPP0 ALSC_DEGAMMA_MEM

	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x000DD704, 0x00000000); // DPP1 DEGAMA_MEM
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x000DD9A4, 0x00000000); // DPP1 GMP_MEM
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x000DE5BC, 0x00000000); // DPP1  DITHER_MEM
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x000DE8A4, 0x00000000); // DPP1  SPR_GAMA_MEM
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x000B010C, 0x00000000); // DDIC0 DITHER_MEM
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x000B0404, 0x00000000); // DDIC0 ODC_ MEM
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x000B188C, 0x00000000); // DDIC0 DeBurnin_ MEM
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x000B2C84, 0x00000000); // DDIC0 DEMURA MEM
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x000D4084, 0x00000000); // DSC0
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x0008F060, 0x00000000); // UYSAMP_MEM
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x0008F100, 0x00000000); // UYSAMP_DBLK_MEM

	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x00015008, 0x00000000); // PIPE_CTRL0 SDMA
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x0001A008, 0x00000000); // PIPE_CTRL1 PRE_WRAP
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x0001B008, 0x00000000); // PIPE_CTRL2 OV
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x000D6C08, 0x00000000); // PIPE_CTRL3 PST_WRAP
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x00016008, 0x00000000); // PIPE_CTRL4 TFSW
}

void dpu_memory_no_lp(uint32_t cmdlist_id)
{
	uint32_t cmdlist_dev_id = CMDLIST_DEV_ID_DPU;
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x0001273C, 0x00000008); // CMD

	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x0006C03C, 0x88888888); // WCH0 DMA MEM
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x0006C040, 0x88888888); // WCH0 DMA MEM
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x0006C218, 0x88888888); // WCH0 SCF MEM
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x0006C290, 0x88888888); // WCH0 SCF_LB_MEM
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x0006C588, 0x88888888); // WCH0 ROT_MEM
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x0006D03C, 0x88888888); // WCH1 DMA MEM
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x0006D040, 0x88888888); // WCH1 DMA MEM
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x0006D218, 0x00000088); // WCH1 SCF MEM
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x0006D290, 0x88888888); // WCH1 SCF_LB_MEM
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x0006D588, 0x88888888); // WCH1 ROT_MEM
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x00013A88, 0x00000008); // TOP_GLB  (LBUF PART0)
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x00088188, 0x00000008); // AVHR_GLB (LBUF PART1)
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x00088288, 0x00000008); // AVHR_GLB (LBUF PART2)
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x000D8208, 0x00000008); // DISP_GLB ( LBUF PART3)
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x000D8280, 0x00000080); // DISP_GLB ( WCH_DBUF)
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x000CD218, 0x00000088); // ARSR0 SCF_COEF MEM
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x000CD290, 0x00000008); // ARSR0 SCF_LB MEM
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x000CD3E8, 0x00000008); // ARSR0 LB MEM
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x000CD3EC, 0x00000088); // ARSR0 COEF MEM

	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x00078218, 0x00000088); // ARSR1 SCF_COEF MEM
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x00078290, 0x00000008); // ARSR1 SCF_LB MEM
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x000783E8, 0x00000008); // ARSR1 LB MEM
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x000783EC, 0x00000088); // ARSR1 COEF MEM

	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x0007F290, 0x00000008); // VSCF0_LB_MEM
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x0007F218, 0x00000088); // VSCF0_COEF_MEM
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x00083290, 0x00000008); // VSCF1
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x00083218, 0x00000088); // VSCF1_COEF_MEM
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x00060238, 0x00000008); // HDR GTM_MEM
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x00060A04, 0x00000008); // HDR DEGAMMA_MEM
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x00060A18, 0x00000008); // HDR GMP_MEM
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x00060A34, 0x00000008); // HDR GAMMA_MEM

	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x0001C060, 0x00000008); // RCH_OV0
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x0001C064, 0x00000008); // RCH_OV1
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x0001C068, 0x00000008); // RCH_OV2
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x0001C06C, 0x00000008); // RCH_OV3
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x000B40B0, 0x00000008); // DPP0 CTC MEM
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x000B4500, 0x00000008); // DPP0 SPR MEM
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x000B4704, 0x00000008); // DPP0 DEGAMMA MEM
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x000B4804, 0x00000008); // DPP0 ROIGAMMA0 MEM
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x000B4904, 0x00000008); // DPP0 ROIGAMMA0 MEM
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x000B49A4, 0x00000008); // DPP0 GMP MEM
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x000B55BC, 0x00000008); // DPP0 DITHER_MEM
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x000B58A4, 0x00000008); // DPP0 SPR_GAMMA MEM
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x000B5904, 0x00000008); // DPP0 ALSC_DEGAMMA_MEM

	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x000DD704, 0x00000008); // DPP1 DEGAMA_MEM
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x000DD9A4, 0x00000008); // DPP1 GMP_MEM
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x000DE5BC, 0x00000008); // DPP1  DITHER_MEM
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x000DE8A4, 0x00000008); // DPP1  SPR_GAMA_MEM
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x000B010C, 0x00000008); // DDIC0 DITHER_MEM
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x000B0404, 0x00000008); // DDIC0 ODC_ MEM
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x000B188C, 0x00000000); // DDIC0 DeBurnin_ MEM
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x000B2C84, 0x00000008); // DDIC0 DEMURA MEM
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x000D4084, 0x88888888); // DSC0
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x0008F060, 0x00000008); // UYSAMP_MEM
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x0008F100, 0x00000008); // UYSAMP_DBLK_MEM

	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x00015008, 0x00000010); // PIPE_CTRL0 SDMA
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x0001A008, 0x00000010); // PIPE_CTRL1 PRE_WRAP
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x0001B008, 0x00000010); // PIPE_CTRL2 OV
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x000D6C08, 0x00000010); // PIPE_CTRL3 PST_WRAP
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x00016008, 0x00000010); // PIPE_CTRL4 TFSW
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id, 0x0004003c, 0x00000000); // DACC LB EN
}

void dpu_ch1_memory_no_lp(uint32_t cmdlist_id)
{
}

static void dpu_pipe_ctrl_init(uint32_t cmdlist_id)
{
	uint32_t cmdlist_dev_id = CMDLIST_DEV_ID_DPU;
	uint32_t lb_num_rd, lb_num_wr;
	dpu_pr_debug("pipe_ctrl_init !!");

	lb_num_wr = PART0_LB_NUM_MAX;
	lb_num_rd = 0;
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id,
		DPU_PIPE_CTRL_PIPE_SCN0_FLUX_CTRL_ADDR(DPU_SDMA_PIPE_CTRL_OFFSET), (lb_num_rd << 8) | lb_num_wr);

	lb_num_wr = PART1_LB_NUM_MAX;
	lb_num_rd = PART0_LB_NUM_MAX;
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id,
		DPU_PIPE_CTRL_PIPE_SCN0_FLUX_CTRL_ADDR(DPU_PRE_PIPE_CTRL_OFFSET), (lb_num_rd << 8) | lb_num_wr);

	lb_num_wr = PART2_LB_NUM_MAX;
	lb_num_rd = PART1_LB_NUM_MAX;
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id,
		DPU_PIPE_CTRL_PIPE_SCN0_FLUX_CTRL_ADDR(DPU_OV_PIPE_CTRL_OFFSET), (lb_num_rd << 8) | lb_num_wr);

	lb_num_wr = PART3_LB_NUM_MAX;
	lb_num_rd = PART2_LB_NUM_MAX;
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id,
		DPU_PIPE_CTRL_PIPE_SCN0_FLUX_CTRL_ADDR(DPU_POST_PIPE_CTRL_OFFSET), (lb_num_rd << 8) | lb_num_wr);

	lb_num_wr = 0;
	lb_num_rd = PART3_LB_NUM_MAX;
	ukmd_set_reg(cmdlist_dev_id, DPU_SCENE_INITAIL, cmdlist_id,
		DPU_PIPE_CTRL_PIPE_SCN0_FLUX_CTRL_ADDR(DPU_ITF_PIPE_CTRL_OFFSET), (lb_num_rd << 8) | lb_num_wr);
}

void dpu_lbuf_init(uint32_t cmdlist_id)
{
	dpu_pipe_ctrl_init(cmdlist_id);
}

/* for pandisplay config scene1 LB3 , scene0 LB3 had configed in dpu_pipe_ctrl_init */
void dpu_comp_lbuf_init(struct dpu_composer *dpu_comp)
{
	uint32_t lb_num_wr, lb_num_rd;
	char __iomem *dpu_base = dpu_comp->comp_mgr->dpu_base;

	if (dpu_comp->comp_scene_id == DPU_SCENE_ONLINE_1) {
		lb_num_wr = PART3_LB_NUM_MAX;
		lb_num_rd = PART2_LB_NUM_MAX;
		set_reg(DPU_PIPE_CTRL_PIPE_SCN0_FLUX_CTRL_ADDR(dpu_base + DPU_POST_PIPE_CTRL_OFFSET +
			DPU_PIPE_CTRL_SCNX_FLUX_CTRL_OFFSET(dpu_comp->comp_scene_id)), (lb_num_rd << 8) | lb_num_wr, 32, 0);

		lb_num_wr = 0;
		lb_num_rd = PART3_LB_NUM_MAX;
		set_reg(DPU_PIPE_CTRL_PIPE_SCN0_FLUX_CTRL_ADDR(dpu_base + DPU_ITF_PIPE_CTRL_OFFSET +
			DPU_PIPE_CTRL_SCNX_FLUX_CTRL_OFFSET(dpu_comp->comp_scene_id)), (lb_num_rd << 8) | lb_num_wr, 32, 0);
	}
}

void dpu_sdma_debug_init(uint32_t cmdlist_id)
{
	void_unused(cmdlist_id);
}

/* chip has implemented the pre-dss_dfs_ok configuration as 1,
 * so no configuration required here
 */
void dpu_power_on_state_for_ddr_dfs(char __iomem *pctrl_base, bool is_power_on)
{
	void_unused(pctrl_base);
	void_unused(is_power_on);
}

void dpu_enable_m1_qic_intr(char __iomem *actrl_base)
{
	void_unused(actrl_base);
}

/* dynamic on and off are not supported */
bool dpu_enable_lp_flag(void)
{
	return true;
}

uint32_t dpu_get_ch_sid(void)
{
	uint32_t ch_sid = 1;
	return ch_sid;
}

void dpu_dacc_resume(char __iomem *dpu_base)
{
	if (unlikely(!dpu_base)) {
		dpu_pr_err("dpu_base is NULL");
		return;
	}
	/* set DFR_DACC_RESUME flag */
	outp32(DPU_DFR_DACC_RESUME_ADDR(dpu_base), 1);
	outp32(DFR_BASE_TIMESTAMP_LOW32(dpu_base), 0);
	outp32(DFR_BASE_TIMESTAMP_HIGH32(dpu_base), 0);
}

void dpu_check_dbuf_state(char __iomem *dpu_base, uint32_t scene_id)
{
	uint32_t dbuf0_state;

	dbuf0_state = inp32(DPU_LBUF_DBUF0_STATE_ADDR(dpu_base + DPU_LBUF_OFFSET));
	dpu_pr_debug("dbuf0_state = 0x%x, scene_id = %u", dbuf0_state, scene_id);
	if ((dbuf0_state & BIT(5)) == BIT(5)) /* dbuf0_flux_req */
		dpu_pr_warn("dbuf0 trigger fluex_req %d, state=0x%x, ctrl=0x%x, fluex_req_bef=0x%x, fluex_req_aft=0x%x",
			scene_id, dbuf0_state,
			inp32(DPU_LBUF_DBUF0_CTRL_ADDR(dpu_base + DPU_LBUF_OFFSET + DPU_LBUF_SCNX_DBUF_THD_OFFSET(scene_id))),
			inp32(DPU_LBUF_DBUF0_THD_FLUX_REQ_BEF_ADDR(dpu_base +
				DPU_LBUF_OFFSET + DPU_LBUF_SCNX_DBUF_THD_OFFSET(scene_id))),
			inp32(DPU_LBUF_DBUF0_THD_FLUX_REQ_AFT_ADDR(dpu_base +
				DPU_LBUF_OFFSET + DPU_LBUF_SCNX_DBUF_THD_OFFSET(scene_id))));
}

bool dpu_is_smmu_bypass(void)
{
	return false;
}

static uint64_t gray2decimal(uint64_t x)
{
	uint64_t y = x;
	while (x >>= 1)
		y ^= x;
	return y;
}

ktime_t get_te_hw_timestamp_us(char __iomem *dpu_base, int32_t sw_itfch_idx)
{
	uint64_t graycnt_l = 0;
	uint64_t graycnt_h = 0;
	uint64_t graycnt = 0;
	uint64_t timestamp = 0;

	if (sw_itfch_idx == PIPE_SW_PRE_ITFCH0) {
		graycnt_l = inp32(DPU_ITF_CH_VSYNC_TIMESTAMP_L_ADDR(dpu_base + DPU_ITF_CH0_OFFSET));
		graycnt_h = inp32(DPU_ITF_CH_VSYNC_TIMESTAMP_H_ADDR(dpu_base + DPU_ITF_CH0_OFFSET));
	} else if (sw_itfch_idx == PIPE_SW_PRE_ITFCH1) {
		graycnt_l = inp32(DPU_ITF_CH_VSYNC_TIMESTAMP_L_ADDR(dpu_base + DPU_ITF_CH1_OFFSET));
		graycnt_h = inp32(DPU_ITF_CH_VSYNC_TIMESTAMP_H_ADDR(dpu_base + DPU_ITF_CH1_OFFSET));
	}

	graycnt = (graycnt_h << 32) | graycnt_l;
	graycnt = gray2decimal(graycnt);

	timestamp = graycnt * 100 / GRAY_CLOCK;

	return (ktime_t)timestamp * 1000;
}

void dpu_comp_abnormal_dump_axi(char __iomem *dpu_base)
{
	void_unused(dpu_base);
}
void dpu_comp_abnormal_dump_qic(char __iomem *media1_ctrl_base)
{
	void_unused(media1_ctrl_base);
}

void dpu_comp_set_glb_rs_reg(struct dpu_composer *dpu_comp, bool enable)
{
	uint32_t clear_bit = 0;
	struct dkmd_connector_info *pinfo = NULL;

	if (unlikely(!dpu_comp || !dpu_comp->conn_info || !dpu_comp->comp_mgr || !dpu_comp->comp_mgr->dpu_base)) {
		dpu_pr_err("dpu_comp or conn_info or dpu_base is null");
		return;
	}

	if (!enable) {
		outp32(DPU_GLB_RS_CLEAR_ADDR(dpu_comp->comp_mgr->dpu_base + DPU_GLB0_OFFSET), 0);
		return;
	}

	pinfo = dpu_comp->conn_info;
	clear_bit = BIT(pinfo->sw_post_chn_idx[PRIMARY_CONNECT_CHN_IDX]);
	if (pinfo->sw_post_chn_num > 1)
		clear_bit = clear_bit | BIT(pinfo->sw_post_chn_idx[EXTERNAL_CONNECT_CHN_IDX]);
	outp32(DPU_GLB_RS_CLEAR_ADDR(dpu_comp->comp_mgr->dpu_base + DPU_GLB0_OFFSET), clear_bit);

	dpu_pr_info("sw_post_chn_idx:%d, clear_bit:%#x", pinfo->sw_post_chn_idx[PRIMARY_CONNECT_CHN_IDX], clear_bit);
}

void dpu_ppc_set_1st_part_cmd_addr(char __iomem *dpu_base, struct panel_partial_ctrl *priv, uint32_t ppc_config_id)
{
	dpu_pr_info("+");
	if ((ppc_config_id != PPC_CONFIG_ID_F_MODE_1ST_PART_IDX) &&
		(ppc_config_id != PPC_CONFIG_ID_M_MODE_1ST_PART_IDX) &&
		(ppc_config_id != PPC_CONFIG_ID_G_MODE_1ST_PART_IDX)) {
		dpu_pr_err("invalid ppc_config_id=%u", ppc_config_id);
		return;
	}

	outp32(PPC_DACC_CMDLIST_ADDR2(dpu_base), priv->cmdlist_phy_addr[ppc_config_id + 0][PPC_1ST_DSI_IDX]);
	outp32(PPC_DACC_CMDLIST_ADDR3(dpu_base), priv->cmdlist_phy_addr[ppc_config_id + 0][PPC_2ND_DSI_IDX]);
	outp32(PPC_DACC_CMDLIST_ADDR4(dpu_base), priv->cmdlist_phy_addr[ppc_config_id + 1][PPC_1ST_DSI_IDX]);
	outp32(PPC_DACC_CMDLIST_ADDR5(dpu_base), priv->cmdlist_phy_addr[ppc_config_id + 1][PPC_2ND_DSI_IDX]);
	outp32(PPC_DACC_CMDLIST_ADDR6(dpu_base), priv->cmdlist_phy_addr[ppc_config_id + 2][PPC_1ST_DSI_IDX]);
	outp32(PPC_DACC_CMDLIST_ADDR7(dpu_base), priv->cmdlist_phy_addr[ppc_config_id + 2][PPC_2ND_DSI_IDX]);
	outp32(PPC_DACC_CMDLIST_ADDR8(dpu_base), priv->cmdlist_phy_addr[ppc_config_id + 3][PPC_1ST_DSI_IDX]);
	outp32(PPC_DACC_CMDLIST_ADDR9(dpu_base), priv->cmdlist_phy_addr[ppc_config_id + 3][PPC_2ND_DSI_IDX]);
	outp32(PPC_DACC_CMDLIST_ADDR10(dpu_base), priv->cmdlist_phy_addr[ppc_config_id + 4][PPC_1ST_DSI_IDX]);
	outp32(PPC_DACC_CMDLIST_ADDR11(dpu_base), priv->cmdlist_phy_addr[ppc_config_id + 4][PPC_2ND_DSI_IDX]);
	outp32(PPC_DACC_CMDLIST_ADDR12(dpu_base), priv->cmdlist_phy_addr[ppc_config_id + 5][PPC_1ST_DSI_IDX]);
	outp32(PPC_DACC_CMDLIST_ADDR13(dpu_base), priv->cmdlist_phy_addr[ppc_config_id + 5][PPC_2ND_DSI_IDX]);
	outp32(PPC_DACC_CMDLIST_ADDR14(dpu_base), priv->cmdlist_phy_addr[ppc_config_id + 6][PPC_1ST_DSI_IDX]);
	outp32(PPC_DACC_CMDLIST_ADDR15(dpu_base), priv->cmdlist_phy_addr[ppc_config_id + 6][PPC_2ND_DSI_IDX]);
}

void dpu_ppc_set_2nd_part_cmd_addr(char __iomem *dpu_base, struct panel_partial_ctrl *priv, uint32_t ppc_config_id)
{
	if ((ppc_config_id != PPC_CONFIG_ID_F_MODE_SECOND_PART_IDX) &&
		(ppc_config_id != PPC_CONFIG_ID_M_MODE_SECOND_PART_IDX) &&
		(ppc_config_id != PPC_CONFIG_ID_G_MODE_SECOND_PART_IDX)) {
		dpu_pr_err("invalid ppc_config_id=%u", ppc_config_id);
		return;
	}

	outp32(PPC_DACC_CMDLIST_ADDR0(dpu_base), priv->cmdlist_phy_addr[ppc_config_id][PPC_1ST_DSI_IDX]);
	outp32(PPC_DACC_CMDLIST_ADDR1(dpu_base), priv->cmdlist_phy_addr[ppc_config_id][PPC_2ND_DSI_IDX]);
}

void dpu_ppc_init_interactive_reg(char __iomem *dpu_base, struct panel_partial_ctrl *priv)
{
	dpu_pr_info("+");

	outp32(PPC_DACC_ENABLE(dpu_base), 0x1);
	outp32(PPC_DACC_STATUS_CONFIG(dpu_base), 0);
	outp32(PPC_DACC_SECTION_CONFIG(dpu_base), 0);
	// fixme config isr
 
	/* delay open ESTV to solve the problem of historical residual images */
	/* 0x1D8 + 0x80, used for panel estv support.First capture isr open estv after receiving the first packet */
	outp32(PPC_DACC_ENABLE_PANEL_ESTV(dpu_base), priv->ppc_enable_panel_estv_support);
	/* 0x1DC + 0x80, used for panel estv support factory, force open ESTV at N TE */
	outp32(PPC_DACC_ENABLE_PANEL_ESTV_FACTORY(dpu_base), priv->ppc_enable_panel_estv_support_factory);
	/* 0x1E0 + 0x80, used for force open ESTV at N TE, N based on experience value and is configurable */
	outp32(PPC_DACC_PANEL_ESTV_WAIT_TE_CNT(dpu_base), priv->ppc_panel_estv_wait_te_cnt);
}

int32_t dpu_vote_enable_doze2(void)
{
	int32_t ret = 0;

	if (g_enable_dpu_doze == 0) {
		dpu_pr_debug("doze vote is not enabled");
		return 0;
	}
	dpu_pr_debug("vote enable doze2!");
	ret = vote_enable_doze(DOZE_CLIENT_DSS_UNSEC);
	if (ret != 0)
		dpu_pr_err("vote enable doze2 failed!");

	return ret;
}

int32_t dpu_vote_disable_doze2(void)
{
	int32_t ret = 0;

	if (g_enable_dpu_doze == 0) {
		dpu_pr_debug("doze vote is not enabled");
		return 0;
	}
	dpu_pr_debug("vote disable doze2!");
	ret = vote_disable_doze(DOZE_CLIENT_DSS_UNSEC);
	if (ret != 0)
		dpu_pr_err("vote disable doze2 failed!");

	return ret;
}

void dpu_vote_enable_doze1(void)
{
	return;
}

void dpu_vote_disable_doze1(void)
{
	return;
}

void dpu_sdma_load_balance_config(char __iomem *dpu_base)
{
	uint32_t val = 0x1;

	if (unlikely(!dpu_base)) {
		dpu_pr_err("dpu_base is NULL");
		return;
	}
	if (g_dpu_config_data.version.info.soc_type == DPU_SOC_TYPE_CS)
		val = 0x7;
	// sdma0, es:bit14=0, bit13=0, bit12=1 cs:bit14=1, bit13=1, bit12=1, meanwhile pref_en_r2=1
	set_reg(DPU_GLB_SDMA_CTRL0_ADDR(dpu_base + DPU_GLB0_OFFSET, 0), val, 3, 12);
	set_reg(DPU_DBCU_MIF_CTRL_SMARTDMA_0_ADDR(dpu_base + DPU_DBCU_OFFSET), 0x1, 1, 0);
	// sdma1, es:bit14=0, bit13=0, bit12=1 cs:bit14=1, bit13=1, bit12=1, meanwhile pref_en_r2=1
	set_reg(DPU_GLB_SDMA_CTRL0_ADDR(dpu_base + DPU_GLB0_OFFSET, 1), val, 3, 12);
	set_reg(DPU_DBCU_MIF_CTRL_SMARTDMA_1_ADDR(dpu_base + DPU_DBCU_OFFSET), 0x1, 1, 0);
	// sdma2, bit14=0, bit13=0, bit12=1, meanwhile pref_en_r2=1
	set_reg(DPU_GLB_SDMA_CTRL0_ADDR(dpu_base + DPU_GLB0_OFFSET, 2), 0x1, 3, 12);
	set_reg(DPU_DBCU_MIF_CTRL_SMARTDMA_2_ADDR(dpu_base + DPU_DBCU_OFFSET), 0x1, 1, 0);
}

void dpu_config_lp_mode_stub(struct composer_manager *comp_mgr)
{
	char __iomem *dpu_base = comp_mgr->dpu_base;
	if (g_debug_dss_clk_lp_mode)
		return;

	set_reg(dpu_base + 0x00013100, 0xFFFFFFFF, 32, 0); // TOP_ GLB
	set_reg(dpu_base + 0x00088000, 0xFFFFFFFF, 32, 0); // AVHR_GLB
	set_reg(dpu_base + 0x000D8000, 0xFFFFFFFF, 32, 0); // DISP_GLB

	set_reg(dpu_base + 0x00012740, 0xFFFFFFFF, 32, 0); // CMD
	set_reg(dpu_base + 0x0006C0E4, 0xFFFFFFFF, 32, 0); // WCH0
	set_reg(dpu_base + 0x0006D0E4, 0xFFFFFFFF, 32, 0); // WCH1
	set_reg(dpu_base + 0x00019868, 0xFFFFFFFF, 32, 0); // DBCU
	set_reg(dpu_base + 0x0001986C, 0xFFFFFFFF, 32, 0); // DBCU
	set_reg(dpu_base + 0x00015004, 0x000000FF, 32, 0); // PIPE_CTRL0 (SDMA)
	set_reg(dpu_base + 0x0001A004, 0x000000FF, 32, 0); // PIPE_CTRL1 (PRE_WRAP)
	set_reg(dpu_base + 0x0001B004, 0x000000FF, 32, 0); // PIPE_CTRL 2 (OV)
	set_reg(dpu_base + 0x000D6C04, 0x000000FF, 32, 0); // PIPE_CTRL 3 (POST_WRAP)
	set_reg(dpu_base + 0x00016004, 0x000000FF, 32, 0); // PIPE_CTRL4 (ITF)
	set_reg(dpu_base + 0x00013A00, 0xFFFFFFFF, 32, 0); // TOP_GLB (LBUF_CTRL0)
	set_reg(dpu_base + 0x00013A80, 0xFFFFFFFF, 32, 0); // TOP_GLB (LBUF_PART0)
	set_reg(dpu_base + 0x00088100, 0xFFFFFFFF, 32, 0); // AVHR_GLB (LBUF_CTL1)
	set_reg(dpu_base + 0x00088180, 0xFFFFFFFF, 32, 0); // AVHR_GLB (LBUF_PART1)
	set_reg(dpu_base + 0x00088200, 0xFFFFFFFF, 32, 0); // AVHR_GLB (LBUF_CTL2)
	set_reg(dpu_base + 0x00088280, 0xFFFFFFFF, 32, 0); // AVHR_GLB (LBUF_PART2)
	set_reg(dpu_base + 0x000D8100, 0xFFFFFFFF, 32, 0); // DISP_GLB (LBUF_CTL3)
	set_reg(dpu_base + 0x000D8200, 0xFFFFFFFF, 32, 0); // DISP_GLB (LBUF_PART3)
	set_reg(dpu_base + 0x00017660, 0x00000001, 32, 0); // LBUF_CMN_WRAP DBUF
	set_reg(dpu_base + 0x00017250, 0xFFFFFFFF, 32, 0); // RD_CTRL(ITF)
	set_reg(dpu_base + 0x00018A50, 0xFFFFFFFF, 32, 0); // RD_CTRL(PRE_WRAP)
	set_reg(dpu_base + 0x00018E50, 0xFFFFFFFF, 32, 0); // RD_CTRL(OV)
	set_reg(dpu_base + 0x000D7E50, 0xFFFFFFFF, 32, 0); // RD_CTRL(PST_WRAP)
	set_reg(dpu_base + 0x00088400, 0xFFFFFFFF, 32, 0); // AVHR_GLB(WR_CTRL1)
	set_reg(dpu_base + 0x00088408, 0xFFFFFFFF, 32, 0); // AVHR_GLB(WR_CTRL2)
	set_reg(dpu_base + 0x000D8300, 0xFFFFFFFF, 32, 0); // DISP_GLB(WR_CTRL3)
	set_reg(dpu_base + 0x000CD008, 0xFFFFFFFF, 32, 0); // ARSR0
	set_reg(dpu_base + 0x00078008, 0xFFFFFFFF, 32, 0); // ARSR1
	set_reg(dpu_base + 0x0007F008, 0xFFFFFFFF, 32, 0); // VSCF0
	set_reg(dpu_base + 0x00083008, 0xFFFFFFFF, 32, 0); // VSCF1
	set_reg(dpu_base + 0x0008D008, 0x00000003, 32, 0); // CLD0
	set_reg(dpu_base + 0x0001C000, 0xFFFFFFFF, 32, 0); // RCH_OV0
	set_reg(dpu_base + 0x0001C008, 0xFFFFFFFF, 32, 0); // RCH_OV1
	set_reg(dpu_base + 0x0001C010, 0xFFFFFFFF, 32, 0); // RCH_OV2
	set_reg(dpu_base + 0x0001C018, 0xFFFFFFFF, 32, 0); // RCH_OV3
	set_reg(dpu_base + 0x0001C020, 0xFFFFFFFF, 32, 0); // RCH_OV WRAP
	set_reg(dpu_base + 0x000B4008, 0xFFFFFFFF, 32, 0); // DPP0
	set_reg(dpu_base + 0x000DD008, 0xFFFFFFFF, 32, 0); // DPP1
	set_reg(dpu_base + 0x000B0008, 0xFFFFFFFF, 32, 0); // DDIC TOP
	set_reg(dpu_base + 0x000D407C, 0xFFFFFFFF, 32, 0); // DSC0 DIV
	set_reg(dpu_base + 0x000D4248, 0xFFFFFFFF, 32, 0); // DSC0 TOP
	set_reg(dpu_base + 0x0001313C, 0x0000001F, 32, 0); // TOP_GLB(DSI/DP)

	set_reg(dpu_base + 0x00011070, 0x00000003, 32, 0); // ITF_CH0
	set_reg(dpu_base + 0x00011170, 0x00000003, 32, 0); // ITF_CH1
	set_reg(dpu_base + 0x00011270, 0x00000003, 32, 0); // ITF_CH2
	set_reg(dpu_base + 0x000114F4, 0x0000003F, 32, 0); // PIPE_SW
	set_reg(dpu_base + 0x00011414, 0x00000001, 32, 0); // PIPE_SW
	set_reg(dpu_base + 0x0001144C, 0x00000001, 32, 0); // PIPE_SW
	set_reg(dpu_base + 0x00011468, 0x00000001, 32, 0); // PIPE_SW
	set_reg(dpu_base + 0x00011484, 0x00000001, 32, 0); // PIPE_SW
	set_reg(dpu_base + 0x0008F008, 0xFFFFFFFF, 32, 0); // UVSAM_WRAP

	set_reg(dpu_base + 0x00013A08, 0x00000000, 32, 0); // TOP_GLB  (LBUF_CTL0)
	set_reg(dpu_base + 0x00088108, 0x00000000, 32, 0); // AVHR_GLB (LBUF_CTL1 )
	set_reg(dpu_base + 0x00088208, 0x00000000, 32, 0); // AVHR_GLB (LBUF_CTL2)
	set_reg(dpu_base + 0x000D8108, 0x00000000, 32, 0); // DISP_GLB  (LBUF_CTL3)
	dpu_pr_info("attention: open all clk gate");
}

bool is_async_mode(struct dpu_bl_ctrl *bl_ctrl)
{
	struct dfr_info *dfr_info = NULL;
	if(!bl_ctrl){
		dpu_pr_info("bl_ctrl is nullptr");
		return false;
	}

	dfr_info = dkmd_get_dfr_info(bl_ctrl->conn_info);
	if(!dfr_info){
		dpu_pr_info("dfr_info is nullptr");
		return false;
	}

	if (dfr_info->dfr_mode == DFR_MODE_TE_SKIP_BY_MCU || dfr_info->dfr_mode == DFR_MODE_LONGH_TE_SKIP_BY_MCU)
		return true;

	return false;
}

int32_t dpu_send_bl_cmds(struct dpu_bl_ctrl *bl_ctrl)
{
	if(!bl_ctrl){
		dpu_pr_err("bl_ctrl is nullptr");
		return -1;
	}

    if (!is_async_mode(bl_ctrl)) {
        if (!bl_ctrl->conn_info) {
            dpu_pr_err("bl_ctrl->conn_info is nullptr");
            return -1;
        }

        return pipeline_next_ops_handle(
            bl_ctrl->conn_info->conn_device, bl_ctrl->conn_info, SET_BACKLIGHT, &bl_ctrl->bl_level);
    }

    return composer_present_dfr_send_dcs_by_riscv(bl_ctrl->parent_composer, bl_ctrl->bl_level);
}

void dpu_composer_get_online_crc(struct composer *comp, uint32_t *crc_val)
{
	struct dpu_composer *dpu_comp = to_dpu_composer(comp);
	char __iomem *dpu_base;

	if (!dpu_comp) {
		dpu_pr_info("dpu_comp is nullptr");
		return;
	}

	dpu_base = dpu_comp->comp_mgr->dpu_base;
	if (!dpu_base) {
		dpu_pr_info("dpu_base is nullptr");
		return;
	}

	if ((g_debug_online_crc_enable != 0) && (g_debug_idle_enable == 0))
		*crc_val = inp32(DPU_GLB_CRC_RESULT0_ADDR(dpu_base + DPU_GLB0_OFFSET));

	dpu_pr_info("get crc value 0x%x", *crc_val);
}

void dpu_get_cur_peri_level(struct dpu_composer *dpu_comp)
{
	void_unused(dpu_comp);
}

void dpu_comp_abnormal_dsm_dump_dvfs(struct dpu_composer *dpu_comp)
{
	void_unused(dpu_comp);
}

void dpu_print_vsync_connect_info(struct dpu_composer *dpu_comp)
{
	uint32_t te_count_tmp = 0;
	uint32_t is_vsync_te = 0;
	uint32_t send_frm_flag = 0;
	uint32_t async_has_refresh = 0;
	uint32_t timer_count = 0;
	uint32_t current_safe_frm_rate = 0;
	uint32_t dimming_status = 0;
	uint32_t dimming_te_cnt = 0;
	uint32_t dimming_cur_frm_rate = 0;
	uint8_t i = 0;
	uint8_t base_addr = 0;
	te_interval te_int[MAX_TE_INTERVAL_COUNT];
	dpu_comp_status_t *check_dimming_status = NULL;
	char __iomem *dpu_base = NULL;

	dpu_check_and_no_retval(!dpu_comp, err, "dpu_comp is null");
	check_dimming_status = &dpu_comp->comp_mgr->dimming_status;
	dpu_check_and_no_retval(dpu_comp_status_is_disable(check_dimming_status), debug, "dimming status is off");
	dpu_base = dpu_comp->comp_mgr->dpu_base;
	if (!dpu_base) {
		dpu_pr_info("dpu_base is nullptr");
		return;
	}

	te_count_tmp = inp32(DFR_DACC_TE_COUNT_TMP(dpu_base));
	is_vsync_te = inp32(DFR_DACC_IS_VSYNC_TE(dpu_base));
	send_frm_flag = inp32(DFR_DACC_SEND_FRM_FLAG(dpu_base));
	async_has_refresh = inp32(DFR_DACC_ASYNC_HAS_REFRESH(dpu_base));
	timer_count = inp32(DFR_DACC_TIMER_COUNT(dpu_base));
	current_safe_frm_rate = inp32(DFR_DACC_CURRENT_SAFE_FRM_RATE(dpu_base));
	dimming_status = inp32(DFR_DACC_DIMMING_STATUS(dpu_base));
	dimming_te_cnt = inp32(DFR_DACC_FRM_DIMMING_TE_CNT(dpu_base));
	dimming_cur_frm_rate = inp32(DFR_DACC_FRM_DIMMING_CUR_FRM_RATE(dpu_base));
	dpu_pr_info("te_count_tmp:%u, is_vsync_te:%u, send_frm_flag:%u, async_has_refresh:%u,"
				"timer_count:%u, current_safe_frm_rate:%u, dimining_ops=%u, dimming_te_cnt=%u, dimming_frm_rate=%u",
				te_count_tmp, is_vsync_te, send_frm_flag, async_has_refresh,
				timer_count, current_safe_frm_rate, dimming_status, dimming_te_cnt, dimming_cur_frm_rate);
	for (i = 0; i < MAX_TE_INTERVAL_COUNT; i++) {
		te_int[i].value = inp32(DPU_DFR_TE_INTERVAL_RECORD(dpu_base + base_addr));
		dpu_pr_info("index_%u: te_count_0:%u, te_count_1:%u, te_count_2:%u, te_count_3:%u.", i,
			te_int[i].param.te_interval_0, te_int[i].param.te_interval_1,
			te_int[i].param.te_interval_2, te_int[i].param.te_interval_3);
		base_addr += 4;
	}
	dpu_pr_info("last write index: %u", inp32(DPU_DFR_TE_INTERVAL_RECORD(dpu_base + base_addr)));
}

void dpu_idle_enter_sd(struct dpu_composer *dpu_comp)
{
	char __iomem *dpu_base;

	if (g_dpu_config_data.version.info.soc_type == DPU_SOC_TYPE_ES) {
		return;
	}

	if (unlikely(!g_debug_idle_shut_down_enable)) {
		return;
	}

	if (unlikely(!dpu_comp)) {
		dpu_pr_warn("dpu_comp is nullptr");
		return;
	}

	dpu_base = dpu_comp->comp_mgr->dpu_base;
	if (unlikely(!dpu_base)) {
		dpu_pr_warn("dpu_base is nullptr");
		return;
	}

	set_reg(SOC_DACC_DBG_SEL_REG_ADDR(dpu_base + DACC_OFFSET + DMC_OFFSET), 0x1, 32, 0);
	udelay(1);
}

void dpu_idle_exit_sd(struct dpu_composer *dpu_comp)
{
	char __iomem *dpu_base;

	if (g_dpu_config_data.version.info.soc_type == DPU_SOC_TYPE_ES) {
		return;
	}

	if (unlikely(!g_debug_idle_shut_down_enable)) {
		return;
	}

	if (unlikely(!dpu_comp)) {
		dpu_pr_warn("dpu_comp is nullptr");
		return;
	}

	dpu_base = dpu_comp->comp_mgr->dpu_base;
	if (unlikely(!dpu_base)) {
		dpu_pr_warn("dpu_base is nullptr");
		return;
	}

	set_reg(SOC_DACC_DBG_SEL_REG_ADDR(dpu_base + DACC_OFFSET + DMC_OFFSET), 0x0, 32, 0);
	udelay(1);
}

void dpu_ops_deep_sleep_enter(struct dpu_composer *dpu_comp)
{
	uint32_t i;
	char __iomem *dpu_base = dpu_comp->comp_mgr->dpu_base;

	if (g_dpu_config_data.version.info.soc_type == DPU_SOC_TYPE_ES) {
		return;
	}

	if (unlikely(!g_debug_ops_deep_sleep_enable)) {
		return;
	}

	dpu_pr_info("+");

	for (i = DPU_SCENE_ONLINE_0;i <= DPU_SCENE_OFFLINE_0;i++) {
		set_reg(dpu_base + DACC_OFFSET + DMC_OFFSET + 0x0464 + i*0x400, 0x00000001, 32, 0); // DACC GERNERAL_CFG_0
	}

	set_reg(DPU_ARSR_SCF_COEF_MEM_CTRL_ADDR(dpu_base + DPU_ARSR0_OFFSET), 0x000000AA, 32, 0); // ARSR0 DS
	set_reg(DPU_ARSR_COEF_MEM_CTRL_ADDR(dpu_base + DPU_ARSR0_OFFSET), 0x000000AA, 32, 0); // ARSR0 DS
	set_reg(DPU_ARSR_ARSR2P_LB_MEM_CTRL_ADDR(dpu_base + DPU_ARSR0_OFFSET), 0x000000A, 32, 0); // ARSR0 DS

	set_reg(DPU_ARSR_SCF_COEF_MEM_CTRL_ADDR(dpu_base + DPU_ARSR1_OFFSET), 0x000000AA, 32, 0); // ARSR1 DS
	set_reg(DPU_ARSR_COEF_MEM_CTRL_ADDR(dpu_base + DPU_ARSR1_OFFSET), 0x000000AA, 32, 0); // ARSR1 DS
	set_reg(DPU_ARSR_ARSR2P_LB_MEM_CTRL_ADDR(dpu_base + DPU_ARSR1_OFFSET), 0x000000A, 32, 0); // ARSR1 DS

	set_reg(DPU_VSCF_SCF_COEF_MEM_CTRL_ADDR(dpu_base + DPU_VSCF0_OFFSET), 0x000000AA, 32, 0); // VSCF0 DS
	set_reg(DPU_VSCF_SCF_LB_MEM_CTRL_ADDR(dpu_base + DPU_VSCF0_OFFSET), 0x0000000A, 32, 0); // VSCF0 DS

	set_reg(DPU_VSCF_SCF_COEF_MEM_CTRL_ADDR(dpu_base + DPU_VSCF1_OFFSET), 0x000000AA, 32, 0); // VSCF1 DS
	set_reg(DPU_VSCF_SCF_LB_MEM_CTRL_ADDR(dpu_base + DPU_VSCF1_OFFSET), 0x0000000A, 32, 0); // VSCF1 DS

	set_reg(DPU_UVSAMP_MEM_CTRL_ADDR(dpu_base + DPU_UVUP_OFFSET), 0x0000000A, 32, 0); // UVUP DS
	set_reg(DPU_UVSAMP_DBLK_MEM_CTRL_ADDR(dpu_base + DPU_UVUP_OFFSET), 0x0000000A, 32, 0); // UVUP DS

	set_reg(DPU_HDR_MEM_CTRL_ADDR(dpu_base + DPU_HDR_OFFSET), 0x0000000A, 32, 0); // HDR DS
	set_reg(DPU_HDR_DEGAMMA_MEM_CTRL_ADDR(dpu_base + DPU_HDR_OFFSET), 0x0000000A, 32, 0); // HDR DS
	set_reg(DPU_HDR_GMP_MEM_CTRL_ADDR(dpu_base + DPU_HDR_OFFSET), 0x0000000A, 32, 0); // HDR DS
	set_reg(DPU_HDR_GAMMA_MEM_CTRL_ADDR(dpu_base + DPU_HDR_OFFSET), 0x0000000A, 32, 0); // HDR DS

	set_reg(DPU_WCH_SCF_COEF_MEM_CTRL_ADDR(dpu_base + DPU_WCH1_OFFSET), 0x000000AA, 32, 0); // WCH1 DS
	set_reg(DPU_WCH_SCF_LB_MEM_CTRL_ADDR(dpu_base + DPU_WCH1_OFFSET), 0x0000000A, 32, 0); // WCH1 DS

	set_reg(DPU_DPP_DEGAMA_MEM_CTRL_ADDR(dpu_base + DPU_DPP0_OFFSET), 0x0000000A, 32, 0); // DPP0 DS
	set_reg(DPU_DPP_ROIGAMA0_MEM_CTRL_ADDR(dpu_base + DPU_DPP0_OFFSET), 0x0000000A, 32, 0); // DPP0 DS
	set_reg(DPU_DPP_ROIGAMA1_MEM_CTRL_ADDR(dpu_base + DPU_DPP0_OFFSET), 0x0000000A, 32, 0); // DPP0 DS
	set_reg(DPU_DPP_SPR_GAMA_MEM_CTRL_ADDR(dpu_base + DPU_DPP0_OFFSET), 0x0000000A, 32, 0); // DPP0 DS
	set_reg(DPU_DPP_ALSC_DEGAMMA_MEM_CTRL_ADDR(dpu_base + DPU_DPP0_OFFSET), 0x0000000A, 32, 0); // DPP0 DS
	set_reg(DPU_DPP_GMP_MEM_CTRL_ADDR(dpu_base + DPU_DPP0_OFFSET), 0x0000000A, 32, 0); // DPP0 DS
	set_reg(DPU_DPP_DITHER_MEM_CTRL_ADDR(dpu_base + DPU_DPP0_OFFSET), 0x0000000A, 32, 0); // DPP0 DS
	set_reg(DPU_DPP_CTC_MEM_CTRL_ADDR(dpu_base + DPU_DPP0_OFFSET), 0x0000000A, 32, 0); // DPP0 DS
	set_reg(DPU_DPP_SPR_MEM_CTRL_ADDR(dpu_base + DPU_DPP0_OFFSET), 0x0000000A, 32, 0); // DPP0 DS

	set_reg(DPU_DPP1_DEGAMA_MEM_CTRL_ADDR(dpu_base + DPU_DPP1_OFFSET), 0x0000000A, 32, 0); // DPP1 DS
	set_reg(DPU_DPP1_SPR_GAMA_MEM_CTRL_ADDR(dpu_base + DPU_DPP1_OFFSET), 0x0000000A, 32, 0); // DPP1 DS
	set_reg(DPU_DPP1_DITHER_MEM_CTRL_ADDR(dpu_base + DPU_DPP1_OFFSET), 0x0000000A, 32, 0); // DPP1 DS
	set_reg(DPU_DPP1_GMP_MEM_CTRL_ADDR(dpu_base + DPU_DPP1_OFFSET), 0x0000000A, 32, 0); // DPP1 DS

	set_reg(DPU_DDIC_DITHER_MEM_CTRL_ADDR(dpu_base + DPU_DDIC0_OFFSET), 0x0000000A, 32, 0); // DDIC DS
	if (DPU_DDIC_DLBCD_MEM_CTRL_ADDR(dpu_base + DPU_DDIC0_OFFSET) != 0) {
		set_reg(DPU_DDIC_DLBCD_MEM_CTRL_ADDR(dpu_base + DPU_DDIC0_OFFSET), 0x0000000A, 32, 0); // DDIC DS
	}
	set_reg(DPU_ODC_MEM_CTRL_ADDR(dpu_base +
		DPU_DDIC0_OFFSET + DPU_DDIC_ODC_OFFSET), 0x0000000A, 32, 0); // DDIC DS
	set_reg(DPU_DEBURNIN_MEM_CTRL2_ADDR(dpu_base +
		DPU_DDIC0_OFFSET + DPU_DDIC_DEBURNIN_OFFSET), 0x00000002, 32, 0); // DDIC DS
	set_reg(DPU_DEBURNIN_MEM_CTRL1_ADDR(dpu_base +
		DPU_DDIC0_OFFSET + DPU_DDIC_DEBURNIN_OFFSET), 0x00000002, 32, 0); // DDIC DS
	set_reg(DPU_DEMURA_MEM_CTRL_ADDR(dpu_base +
		DPU_DDIC0_OFFSET + DPU_DDIC_DEMURA_OFFSET), 0x00000002, 32, 0); // DDIC DS

	set_reg(DPU_DSC_MEM_CTRL_ADDR(dpu_base + DPU_DSC0_OFFSET), 0x0000000A, 32, 0); // DSC DS
}

bool is_edp_cmd_mode(struct dkmd_connector_info *conn_info)
{
	return false;
}

uint32_t dpu_get_refresh_type(char __iomem *dpu_base)
{
	if (unlikely(dpu_base == NULL)) {
		dpu_pr_err("dpu_base is nullptr!");
		return PANEL_REFRESH_TYPE_INVALID;
	}

	return inp32(DPU_DFR_REFRESH_TYPE_ADDR(dpu_base));
}

bool dpu_comp_enter_idle_check(struct dkmd_connector_info *pinfo, struct dpu_composer *dpu_comp)
{
	void_unused(pinfo);
	void_unused(dpu_comp);
	return false;
}

uint32_t dpu_read_clear_extra_refresh_cnt(char __iomem *dpu_base)
{
	uint32_t extra_refresh_cnt = 0;

	if (unlikely(dpu_base == NULL)) {
		dpu_pr_err("dpu_base is nullptr!");
		return 0;
	}

	extra_refresh_cnt = inp32(DPU_DFR_EXTRA_REFRESH_CNT_ADDR(dpu_base));
	outp32(DPU_DFR_EXTRA_REFRESH_CNT_ADDR(dpu_base), 0);
	
	return extra_refresh_cnt;
}

uint32_t get_mdp_irq_unmask(void)
{
	return ~(DPU_DACC_NS_INT | DPU_WCH0_NS_INT | DPU_WBUF_NS_INT);
}

void dpu_comp_set_data_continue_cnt(struct dpu_composer *dpu_comp, uint64_t dss_clk)
{
	void_unused(dpu_comp);
	void_unused(dss_clk);
}

void dpu_ppc_set_1st_cmdlist_cnt(char __iomem *dpu_base, struct panel_partial_ctrl *priv, uint32_t cmdlist_cnt)
{
	void_unused(priv);
	outp32(PPC_DACC_SECTION_CONFIG(dpu_base), cmdlist_cnt);
}

void dpu_disable_ppc(char __iomem *dpu_base, struct panel_partial_ctrl *priv)
{
	void_unused(priv);
	outp32(PPC_DACC_ENABLE(dpu_base), 0);
}

void dpu_start_ppc(char __iomem *dpu_base, struct panel_partial_ctrl *priv)
{
	void_unused(priv);
	outp32(PPC_DACC_STATUS_CONFIG(dpu_base), 0x1);
}

bool dpu_is_single_tbu(void)
{
	return false;
}

bool can_pre_init_config(void)
{
	return true;
}