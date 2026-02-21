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

#include <dpu/soc_dpu_define.h>
#include <dpu/dpu_itfsw.h>
#include <ukmd_cmdlist.h>
#include "dpu_comp_mgr.h"
#include "dpu_comp_init.h"
#include "dpu_lut_init.h"
#ifdef SURPORT_LOCAL_DIMMING
#include "localdimming/dpu_ld_init.h"
#endif
#include "dpu_cmdlist.h"
#include "dkmd_object.h"
#include "dpu_comp_config_utils.h"
#include "cmdlist_interface.h"
#include "dpu_connector.h"
#include "res_mgr.h"

static void dpu_comp_itfch_config(struct dpu_composer *dpu_comp,
	struct dkmd_connector_info *pinfo)
{
	uint32_t pre_itfch_offset = (uint32_t)(DPU_ITF_CH0_OFFSET + pinfo->base.pipe_sw_itfch_idx * 0x100);

	dpu_pr_debug("dpu_comp_itfch_config %d", pinfo->base.pipe_sw_itfch_idx);

	set_reg(DPU_ITF_CH_IMG_SIZE_ADDR(dpu_comp->comp_mgr->dpu_base + pre_itfch_offset),
			(((pinfo->base.dsc_out_height - 1) << 16) | (pinfo->base.dsc_out_width - 1)), 32, 0);

	set_reg(DPU_ITF_CH_ITFSW_DATA_SEL_ADDR(dpu_comp->comp_mgr->dpu_base + pre_itfch_offset), 0x0, 32, 0);

	if (pinfo->colorbar_enable) {
		set_reg(DPU_ITF_CH_DPP_CLRBAR_CTRL_ADDR(dpu_comp->comp_mgr->dpu_base + pre_itfch_offset),
				((pinfo->base.xres / 12 - 1) << 24) | 0x1, 32, 0);

		set_reg(DPU_ITF_CH_CLRBAR_START_ADDR(dpu_comp->comp_mgr->dpu_base + pre_itfch_offset), 0x1, 32, 0);
	}

	set_reg(DPU_ITF_CH_REG_CTRL_FLUSH_EN_ADDR(dpu_comp->comp_mgr->dpu_base + pre_itfch_offset), 0x1, 32, 0);

	if (pinfo->sw_post_chn_idx[PRIMARY_CONNECT_CHN_IDX] == CONNECTOR_ID_DSI2)
		set_reg(DPU_ITF_CH_VSYNC_DELAY_CNT_CTRL_ADDR(dpu_comp->comp_mgr->dpu_base + pre_itfch_offset), 0x1b3, 32, 0);
	else
		set_reg(DPU_ITF_CH_VSYNC_DELAY_CNT_CTRL_ADDR(dpu_comp->comp_mgr->dpu_base + pre_itfch_offset), 0x1a1, 32, 0);
}

static void dpu_comp_pipe_sw_init(struct dpu_composer *dpu_comp)
{
	uint32_t primary_post_ch_offset = 0x0;
	uint32_t external_post_ch_offset = 0x0;
	uint32_t ctrl_sig_en = 0x0;
	struct dkmd_connector_info *pinfo = dpu_comp->conn_info;

	/* dual-mipi or cphy 1+1 */
	ctrl_sig_en = (pinfo->sw_post_chn_num > EXTERNAL_CONNECT_CHN_IDX) ? 0x0 : 0x1;
	primary_post_ch_offset = pinfo->sw_post_chn_idx[PRIMARY_CONNECT_CHN_IDX] * 0x1C;
	if (pinfo->sw_post_chn_idx[PRIMARY_CONNECT_CHN_IDX] == 5)
		primary_post_ch_offset = 0x0100;
	dpu_pr_debug("dpu_comp_pipe_sw_init %d, %d, %d", pinfo->base.pipe_sw_itfch_idx, dpu_comp->comp_scene_id, primary_post_ch_offset);

	set_reg(DPU_PIPE_SW_SIG_CTRL_0_ADDR(dpu_comp->comp_mgr->dpu_base + DPU_PIPE_SW_OFFSET) \
		+ primary_post_ch_offset, BIT(dpu_comp->comp_scene_id), 32, 0);

	set_reg(DPU_PIPE_SW_SW_POS_CTRL_SIG_EN_0_ADDR(dpu_comp->comp_mgr->dpu_base + DPU_PIPE_SW_OFFSET) \
		+ primary_post_ch_offset, ctrl_sig_en, 32, 0);

	set_reg(DPU_PIPE_SW_DAT_CTRL_0_ADDR(dpu_comp->comp_mgr->dpu_base + DPU_PIPE_SW_OFFSET) \
		+ primary_post_ch_offset, BIT(dpu_comp->comp_scene_id), 32, 0);

	set_reg(DPU_PIPE_SW_SW_POS_CTRL_DAT_EN_0_ADDR(dpu_comp->comp_mgr->dpu_base + DPU_PIPE_SW_OFFSET) \
		+ primary_post_ch_offset, 0x1, 32, 0);

	/* dual-mipi or cphy 1+1 */
	if (pinfo->sw_post_chn_num > EXTERNAL_CONNECT_CHN_IDX) {
		external_post_ch_offset = pinfo->sw_post_chn_idx[EXTERNAL_CONNECT_CHN_IDX] * 0x1C;

		set_reg(DPU_PIPE_SW_SIG_CTRL_0_ADDR(dpu_comp->comp_mgr->dpu_base + DPU_PIPE_SW_OFFSET) \
			+ external_post_ch_offset, BIT(dpu_comp->comp_scene_id), 32, 0);

		set_reg(DPU_PIPE_SW_SW_POS_CTRL_SIG_EN_0_ADDR(dpu_comp->comp_mgr->dpu_base + DPU_PIPE_SW_OFFSET) \
			+ external_post_ch_offset, 0x1, 32, 0);

		set_reg(DPU_PIPE_SW_DAT_CTRL_0_ADDR(dpu_comp->comp_mgr->dpu_base + DPU_PIPE_SW_OFFSET) \
			+ external_post_ch_offset, BIT(dpu_comp->comp_scene_id), 32, 0);

		set_reg(DPU_PIPE_SW_SW_POS_CTRL_DAT_EN_0_ADDR(dpu_comp->comp_mgr->dpu_base + DPU_PIPE_SW_OFFSET) \
			+ external_post_ch_offset, 0x1, 32, 0);

		/* TODO: check split swap and size config */
		set_reg(DPU_PIPE_SW_SPLIT_HSIZE_ADDR(dpu_comp->comp_mgr->dpu_base + DPU_PIPE_SW_OFFSET),
			((pinfo->base.dsc_out_width / 2 - 1) << 13) | (pinfo->base.dsc_out_width - 1), 32, 0);

		if (pinfo->split_swap_enable)
			set_reg(DPU_PIPE_SW_SPLIT_CTL_ADDR(dpu_comp->comp_mgr->dpu_base + DPU_PIPE_SW_OFFSET), 0x1, 32, 0);
		else
			set_reg(DPU_PIPE_SW_SPLIT_CTL_ADDR(dpu_comp->comp_mgr->dpu_base + DPU_PIPE_SW_OFFSET), 0x0, 32, 0);
	}

	dpu_comp_itfch_config(dpu_comp, pinfo);
}

void dpu_comp_release(struct dpu_composer *dpu_comp)
{
	if (unlikely(!dpu_comp)) {
		dpu_pr_err("dpu_comp is null\n");
		return;
	}

	if (unlikely(!dpu_comp->comp_mgr)) {
		dpu_pr_err("dpu_comp->comp_mgr is null\n");
		return;
	}

	ukmd_cmdlist_release_locked(CMDLIST_DEV_ID_DPU,
		DPU_SCENE_INITAIL, dpu_comp->comp_mgr->init_scene_cmdlist);
	dpu_comp->comp_mgr->init_scene_cmdlist = INVALID_CMDLIST_ID;
}

void dpu_init(struct composer_manager *comp_mgr)
{
	uint32_t cmdlist_id = INVALID_CMDLIST_ID;
	uint32_t cmdlist_dev_id;

	if (unlikely(!comp_mgr)) {
		dpu_pr_err("comp_mgr is null");
		return;
	}

	cmdlist_dev_id = CMDLIST_DEV_ID_DPU;
	if (comp_mgr->init_scene_cmdlist == INVALID_CMDLIST_ID) {
		/* 1. request initialize cmdlist scene id */
		comp_mgr->init_scene_cmdlist = cmdlist_create_user_client(cmdlist_dev_id,
			DPU_SCENE_INITAIL, SCENE_NOP_TYPE, 0, 0);

		/* 2. prepare cmdlist buffer content */
		cmdlist_id = cmdlist_create_user_client(cmdlist_dev_id,
			DPU_SCENE_INITAIL, REGISTER_CONFIG_TYPE, 0, PAGE_SIZE);
		if (dpu_enable_lp_flag()) {
			dpu_level1_clock_lp(cmdlist_id);
			dpu_level2_clock_lp(cmdlist_id);
			dpu_ch1_level2_clock_lp(cmdlist_id);
			dpu_memory_lp(cmdlist_id);
		} else {
			dpu_memory_no_lp(cmdlist_id);
			dpu_ch1_memory_no_lp(cmdlist_id);
		}

		if (dpu_lut_init(comp_mgr)) {
			dpu_pr_err("dpu_lut_init fail");
			ukmd_cmdlist_release_locked(cmdlist_dev_id,
				DPU_SCENE_INITAIL, comp_mgr->init_scene_cmdlist);
			comp_mgr->init_scene_cmdlist = INVALID_CMDLIST_ID;
			return;
		}

		dpu_lbuf_init(cmdlist_id);
		dpu_sdma_debug_init(cmdlist_id);

#ifdef SURPORT_LOCAL_DIMMING
		if (dpu_ld_init(comp_mgr)) {
			dpu_pr_err("cmdlist get payload fail");
			ukmd_cmdlist_release_locked(cmdlist_dev_id,
				DPU_SCENE_INITAIL, comp_mgr->init_scene_cmdlist);
			comp_mgr->init_scene_cmdlist = INVALID_CMDLIST_ID;
			return;
		}
#endif
		cmdlist_append_client(cmdlist_dev_id,
			DPU_SCENE_INITAIL, comp_mgr->init_scene_cmdlist, cmdlist_id);
		cmdlist_flush_client(cmdlist_dev_id,
			DPU_SCENE_INITAIL, comp_mgr->init_scene_cmdlist);
	}

	dpu_cmdlist_init_commit(comp_mgr->dpu_base,
		ukmd_cmdlist_get_dma_addr(cmdlist_dev_id,
			DPU_SCENE_INITAIL, comp_mgr->init_scene_cmdlist));

	// use ukmd_cmdlist_dump_scene(cmdlist_dev_id, DPU_SCENE_INITAIL) to dump init cmdlist node
	dpu_enable_m1_qic_intr(comp_mgr->actrl_base);
	dpu_config_lp_mode_stub(comp_mgr);
}

void dpu_comp_simple_init(struct dpu_composer *dpu_comp)
{
	struct dkmd_connector_info *pinfo = dpu_comp->conn_info;
	uint32_t dpi_en;

	if (unlikely(!pinfo)) {
		dpu_pr_err("pinfo is null");
		return;
	}

	if (is_dp_panel(&pinfo->base)) {
		dpi_en = 1;
		(void)pipeline_next_ops_handle(pinfo->conn_device, pinfo, ENABLE_DPI, (void *)&dpi_en);
	}

	if (is_offline_panel(&pinfo->base) || is_dp_panel(&pinfo->base))
		return;

	dpu_ops_deep_sleep_enter(dpu_comp);
	composer_dvfs_dump_timer_init(dpu_comp);
	dpu_comp->curr_active_level = 0;
}

void dpu_comp_init(struct dpu_composer *dpu_comp)
{
	struct dkmd_connector_info *pinfo = dpu_comp->conn_info;
	uint32_t dpi_en;

	if (unlikely(!pinfo)) {
		dpu_pr_err("pinfo is null");
		return;
	}

	if (is_dp_panel(&pinfo->base)) {
		dpi_en = 1;
		(void)pipeline_next_ops_handle(pinfo->conn_device, pinfo, ENABLE_DPI, (void *)&dpi_en);
	}

	if (is_offline_panel(&pinfo->base) || is_dp_panel(&pinfo->base))
		return;

	dpu_comp_lbuf_init(dpu_comp);
	dpu_comp_pipe_sw_init(dpu_comp);

	dpu_ops_deep_sleep_enter(dpu_comp);
	composer_dvfs_dump_timer_init(dpu_comp);
	dpu_comp->curr_active_level = 0;
}

void send_reset_hardware_event(struct composer *comp)
{
	char *envp[2] = { "ResetHW=1", NULL };

	if (!comp) {
		dpu_pr_err("comp is NULL Pointer");
		return;
	}

	kobject_uevent_env(&(comp->base.peri_device->dev.kobj), KOBJ_CHANGE, envp);
	dpu_pr_info("reset hardware event!");
}
