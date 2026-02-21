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
#include <dpu/soc_dpu_define.h>
#include "dkmd_object.h"
#include "dpu_debug_dump.h"

// 1: for 800 820 840 900 9200 dacc inint done
#define DACC_INIT_DONE				(0x22050)
#define DACC_TAG_ADDR				(DACC_OFFSET + 0x15c00)
#define DACC_CROSSBAR_ADDR			(DACC_OFFSET + 0x2F000)
#define DACC_TMP_ADDR(scene_id)		(DACC_OFFSET + 0x16000 + (scene_id) * 0x400)
#define DACC_PRE0_OFFSET			(DACC_OFFSET + 0x2E000)
#define DACC_PRE1_OFFSET			(DACC_PRE0_OFFSET + 0x300)
#define DACC_PRE2_OFFSET			(DACC_PRE0_OFFSET + 0x500)
#define DACC_PRE3_OFFSET			(0xFFFFFFFF)
#define DACC_PRE4_OFFSET			(DACC_PRE0_OFFSET + 0x700)
#define DACC_POST_ADDR(scene_id)	(DACC_OFFSET + 0x2EE70 + (scene_id) * 0x10)
#define DACC_ITF_ADDR				(DACC_OFFSET + 0x2EE00)
#define DACC_TAG_SIZE				(0x400)
#define DACC_CROSSBAR_SIZE			(0x160)
#define DACC_TMP_SIZE				(0x1D0)
#define DACC_PRE_SIZE				(0x1D0)
#define DACC_POST_SIZE				(0x20)
#define DACC_ITF_SIZE				(0x30)
#define DACC_LOG_ADDR               (0x13800)
#define DACC_LOG_SIZE               (0x400)
#define DACC_POST_SCENE_0_WB_INFO_REG			(DACC_PRE0_OFFSET + 0xE60)
#define DACC_CFG_ADDR							(DACC_OFFSET + DMC_OFFSET)
#define DACC_CFG_LUT_LAYER_ID_REG				(DACC_CFG_ADDR + 0x60)
#define DACC_CFG_LAYER_DIM_FLAG_REG(scene_id)	(DACC_CFG_ADDR + 0x460 + (scene_id) * 0x400)
#define DACC_CFG_LAYER_NUM_REG(scene_id)		(DACC_CFG_ADDR + 0x47c + (scene_id) * 0x400)
#define DACC_CFG_DBG_INFO(scene_id)				(DACC_CFG_ADDR + 0x52c + (scene_id) * 0x400)
#define DACC_CFG_DBG_INFO_SIZE					(0x20)
#define DACC_SCENE_3							(3)

static uint32_t g_dacc_pre_addr[] = {DACC_PRE0_OFFSET, DACC_PRE1_OFFSET,
										DACC_PRE2_OFFSET, DACC_PRE3_OFFSET,
										DACC_PRE4_OFFSET};

static void dacc_reg_dump(uint32_t *dpu_dacc_dump, uint32_t dump_size, uint32_t dacc_dump_addr)
{
	uint32_t i;
	uint32_t inc_step = 4;
	uint32_t val1 = 0;
	uint32_t val2 = 0;
	uint32_t val3 = 0;
	uint32_t val4 = 0;
	uint32_t offset = dacc_dump_addr;

	for (i = 0; i < ((dump_size) / (inc_step * 4)); i++) {
		val1 = inp32(dpu_dacc_dump++);
		val2 = inp32(dpu_dacc_dump++);
		val3 = inp32(dpu_dacc_dump++);
		val4 = inp32(dpu_dacc_dump++);
		dpu_pr_warn("addr:%#x val:0x%08x 0x%08x 0x%08x 0x%08x", offset, val1, val2, val3, val4);
		offset += inc_step * 4;
	}
}

static void dacc_dump(char __iomem *dpu_base, uint32_t scene_id)
{
	uint32_t *dpu_dacc_tag_dump = (uint32_t *)(dpu_base + DACC_TAG_ADDR);
	uint32_t *dpu_dacc_crossbar_dump = (uint32_t *)(dpu_base + DACC_CROSSBAR_ADDR);
	uint32_t *dpu_dacc_log_dump = (uint32_t *)(dpu_base + DACC_OFFSET + DACC_LOG_ADDR);
	uint32_t *dpu_dacc_itf_dump = (uint32_t *)(dpu_base + DACC_ITF_ADDR);
	uint32_t val1 = 0;
	uint32_t *dpu_dacc_tmp_dump = NULL;
	uint32_t *dpu_dacc_pre_dump = NULL;
	uint32_t *dpu_dacc_post_dump = NULL;
	uint32_t *dpu_dacc_dbg_info_dump = NULL;

	if (scene_id >= SCENE_NUM_MAX) {
		dpu_pr_warn("scene_id is err %u", scene_id);
		return;
	}

	dpu_dacc_tmp_dump = (uint32_t *)(dpu_base + DACC_TMP_ADDR(scene_id));
	dpu_dacc_post_dump = (uint32_t *)(dpu_base + DACC_POST_ADDR(scene_id));
	dpu_dacc_dbg_info_dump = (uint32_t *)(dpu_base + DACC_CFG_DBG_INFO(scene_id));

	dpu_pr_warn("dacc reg dump begin(scene_id:%u):", scene_id);

	val1 = inp32(dpu_base + DACC_OFFSET + DACC_INIT_DONE);
	dpu_pr_warn("dacc_init_done: %u", val1);

	dpu_pr_warn("dacc_tag:");
	dacc_reg_dump(dpu_dacc_tag_dump, DACC_TAG_SIZE, DACC_TAG_ADDR);
	dpu_pr_warn("dacc_crossbar:");
	dacc_reg_dump(dpu_dacc_crossbar_dump, DACC_CROSSBAR_SIZE, DACC_CROSSBAR_ADDR);
	dpu_pr_warn("dacc_tmp:");
	dacc_reg_dump(dpu_dacc_tmp_dump, DACC_TMP_SIZE, DACC_TMP_ADDR(scene_id));
	if(scene_id != DACC_SCENE_3) {
		dpu_pr_warn("dacc_pre:");
		dpu_dacc_pre_dump = (uint32_t *)(dpu_base + g_dacc_pre_addr[scene_id]);
		dacc_reg_dump(dpu_dacc_pre_dump, DACC_PRE_SIZE, g_dacc_pre_addr[scene_id]);
	}
	dpu_pr_warn("dacc_post:");
	dacc_reg_dump(dpu_dacc_post_dump, DACC_POST_SIZE, DACC_POST_ADDR(scene_id));
	dacc_reg_dump(dpu_dacc_itf_dump, DACC_ITF_SIZE, DACC_ITF_ADDR);
	dpu_pr_warn("dacc_log:");
	dacc_reg_dump(dpu_dacc_log_dump, DACC_LOG_SIZE, DACC_OFFSET + DACC_LOG_ADDR);

	/* dacc post reg */
	val1 = inp32((uint32_t *)(dpu_base + DACC_POST_SCENE_0_WB_INFO_REG));
	dpu_pr_warn("dacc post reg: 0x%08x", val1);

	/* dacc cfg */
    dpu_pr_warn("dacc cfg:");
	val1 = inp32((uint32_t *)(dpu_base + DACC_CFG_LUT_LAYER_ID_REG));
    dpu_pr_warn("lut layer id reg: 0x%08x", val1);
	val1 = inp32((uint32_t *)(dpu_base + DACC_CFG_LAYER_DIM_FLAG_REG(scene_id)));
    dpu_pr_warn("layer dim flag reg: 0x%08x", val1);
	val1 = inp32((uint32_t *)(dpu_base + DACC_CFG_LAYER_NUM_REG(scene_id)));
	dpu_pr_warn("layer num reg: 0x%08x", val1);
    
	dpu_pr_warn("dacc cfg debug info:");
	dacc_reg_dump(dpu_dacc_dbg_info_dump, DACC_CFG_DBG_INFO_SIZE, DACC_CFG_DBG_INFO(scene_id));
}

void dpu_comp_abnormal_dump_dacc(char __iomem *dpu_base, uint32_t scene_id)
{
	char __iomem *module_base = dpu_base + DACC_OFFSET + DMC_OFFSET;

	g_dpu_dsm_info.dacc_pc = inp32(SOC_DACC_CORE_MON_PC_REG_ADDR(module_base));
	dpu_pr_warn("DACC:\n\n"
		"CORE_STATUS_REG=%#x\t\tDACC_CORE_MON_PC_REG=%#x\n\n",
		inp32(SOC_DACC_CORE_STATUS_REG_ADDR(module_base)), g_dpu_dsm_info.dacc_pc);

	dacc_dump(dpu_base, scene_id);
}