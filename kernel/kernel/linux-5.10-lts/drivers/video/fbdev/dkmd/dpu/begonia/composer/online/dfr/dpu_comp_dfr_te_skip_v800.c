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
#include <linux/ktime.h>
#include <linux/types.h>
#include <linux/wait.h>
#include <linux/spinlock.h>
#include <linux/notifier.h>

#include "dpu_comp_dfr_te_skip.h"
#include <dpu/soc_dpu_define.h>
#include "cmdlist_interface.h"
#include "dpu_comp_mgr.h"
#include "ukmd_cmdlist.h"
#include "dkmd_lcd_interface.h"
#include "dkmd_dfr_interface.h"
#include "dkmd_peri.h"
#include "dpu_conn_mgr.h"
#include "panel_mgr.h"
#include "dpu_comp_maintain.h"
#include "dkmd_cmds_info.h"

#define TE_120HZ 120
#define TE_144HZ 144
#define FPS_144HZ 144
#define PORCH_SWITCH_DELAY_MAX_CNT 3

#define UEVENT_BUF_LEN 120
#define LTPO_RAM_SIZE 0x1FF
enum {
	CMDLIST_SEND_FRM,
	CMDLIST_REFRESH_FRM,
	CMDLIST_REFRESH_WITH_1HZ_DCS,
	CMDLIST_REFRESH_PPU_FULL,
	CMDLIST_REFRESH_PPU_HIGH,
	CMDLIST_REFRESH_FRM_EXT,
	CMDLIST_SEND_WITH_120HZ_DCS,
	CMDLIST_SEND_WITH_144HZ_DCS,
	CMDLIST_TYPE_MAX
};

struct dfr_te_skip_ctrl {
	bool resend_safe_frm_rate; /* need resend safe frame rate in the first frame commit after power on. */
	uint32_t active_id;
	uint32_t cmdlist_scene_id;
	uint32_t header_cmdlist_ids[CMDLIST_TYPE_MAX];
	uint32_t reg_cmdlist_ids[CMDLIST_TYPE_MAX];
	struct notifier_block *notifier;

	int32_t porch_switch_delay_cnt;
	bool is_porch_switch;
};

/* te skip mode implemented by dacc, only support one scene,
 * so we use global private data, if need support multi scenes,
 * please alloc it dynamiclly, and attach it to connector peri device.
 */
struct dfr_te_skip_ctrl g_te_skip_ctrl = { 0 };

static int32_t cmdlist_mipi_dsi_swrite_to(struct dsi_cmd_desc *cm, uint32_t dsi_offset,
	uint32_t cmdlist_id, uint32_t scene_id)
{
	uint32_t hdr = 0;
	uint32_t len;

	if ((cm->dlen != 0) && (cm->payload == 0)) {
		dpu_pr_err("NO payload error!");
		return 0;
	}

	/* mipi dsi short write with 0, 1 2 parameters, total 3 param */
	if (cm->dlen > 2) {
		dpu_pr_err("cm->dlen is invalid");
		return -EINVAL;
	}
	len = cm->dlen;

	hdr |= dsi_hdr_dtype(cm->dtype);
	hdr |= dsi_hdr_vc(cm->vc);
	/* The two  fields are reserved in old platfrom */
	hdr |= dsi_hdr_groupflag(cm->groupflag);
	hdr |= dsi_hdr_singlemode(cm->singlemode);
	if (len == 1) {
		hdr |= dsi_hdr_data1((uint32_t)(cm->payload[0]));
		hdr |= dsi_hdr_data2(0);
	} else if (len == 2) {
		hdr |= dsi_hdr_data1((uint32_t)(cm->payload[0]));
		hdr |= dsi_hdr_data2((uint32_t)(cm->payload[1]));
	} else {
		hdr |= dsi_hdr_data1(0);
		hdr |= dsi_hdr_data2(0);
	}

	/* used for low power cmds trans under video mode */
	hdr |= cm->dtype & GEN_VID_LP_CMD;

	ukmd_set_reg(CMDLIST_DEV_ID_DPU,
		scene_id, cmdlist_id, DPU_DSI_GEN_HP_HDR_ADDR(dsi_offset), hdr);

	dpu_pr_info("hdr = %#x!", hdr);
	return (int32_t)len;  /* 4 bytes */
}

static int32_t cmdlist_mipi_dsi_lwrite_to(struct dsi_cmd_desc *cm, uint32_t dsi_offset,
	uint32_t cmdlist_id, uint32_t scene_id)
{
	uint32_t hdr = 0;
	uint32_t i = 0;
	uint32_t j = 0;
	uint32_t pld = 0;

	if ((cm->dlen != 0) && (cm->payload == 0)) {
		dpu_pr_err("NO payload error!");
		return 0;
	}

	if (cm->dlen > DLEN_MAX) {
		dpu_pr_err("invalid dlen:%u", cm->dlen);
		return 0;
	}

	/* fill up payload, 4bytes set reg, remain 1 byte(8 bits) set reg */
	for (i = 0;  i < cm->dlen; i += 4) {
		if ((i + 4) <= cm->dlen) {
			pld = *((uint32_t *)(cm->payload + i));
		} else {
			for (j = i; j < cm->dlen; j++)
				pld |= (((uint32_t)cm->payload[j] & 0x0ff) << ((j - i) * 8));

			dpu_pr_info("pld = %#x!", pld);
		}

		ukmd_set_reg(CMDLIST_DEV_ID_DPU,
			scene_id, cmdlist_id, DPU_DSI_GEN_HP_PLD_DATA_ADDR(dsi_offset), pld);
		pld = 0;
	}

	/* fill up header */
	hdr |= dsi_hdr_dtype(cm->dtype);
	hdr |= dsi_hdr_vc(cm->vc);
	hdr |= dsi_hdr_wc(cm->dlen);
	/* The two  fields are reserved in old platfrom */
	hdr |= dsi_hdr_groupflag(cm->groupflag);
	hdr |= dsi_hdr_singlemode(cm->singlemode);

	/* used for low power cmds trans under video mode */
	hdr |= cm->dtype & GEN_VID_LP_CMD;
	ukmd_set_reg(CMDLIST_DEV_ID_DPU,
		scene_id, cmdlist_id, DPU_DSI_GEN_HP_HDR_ADDR(dsi_offset), hdr);

	dpu_pr_info("hdr = %#x!", hdr);

	return (int32_t)cm->dlen;
}

static void dfr_te_skip_print_dsi_cmds(struct dsi_cmds *cmds)
{
	uint32_t i, j;
	dpu_check_and_no_retval(!cmds, err, "cmds is null");

	for (i = 0; i < cmds->cmd_num; i++) {
		dpu_pr_debug("dtype: %u, vc: %u, dlen: %u", cmds->cmds[i].dtype, cmds->cmds[i].vc, cmds->cmds[i].dlen);
		dpu_pr_debug("payload: ");
		for (j = 0; j < cmds->cmds[i].dlen; j++)
			dpu_pr_debug("0x%x", cmds->cmds[i].payload[j]);
	}
}

static void dfr_te_skip_print_dfr_info(struct dfr_info *dinfo)
{
	uint32_t i;
	uint32_t j;
	struct dimming_sequence dimming_seq;
	dpu_check_and_no_retval(!dinfo, err, "dinfo is null");
	dpu_pr_debug("oled type:%d", dinfo->oled_info.oled_type);

	if (dinfo->oled_info.oled_type != PANEL_OLED_LTPO)
		return;

	dpu_pr_debug("ltpo version:%d, dfr mode:%d", dinfo->oled_info.ltpo_info.ver, dinfo->dfr_mode);
	dpu_pr_debug("ltpo te freq num:%d", dinfo->oled_info.ltpo_info.te_freq_num);
	dpu_pr_info("ltpo te freq:[%d, %d]", dinfo->oled_info.ltpo_info.te_freqs[0], dinfo->oled_info.ltpo_info.te_freqs[1]);
	dpu_pr_debug("ltpo te mask num:%d", dinfo->oled_info.ltpo_info.te_mask_num);
	dpu_pr_info("ltpo te mask:[%d, %d]", dinfo->oled_info.ltpo_info.te_masks[0], dinfo->oled_info.ltpo_info.te_masks[1]);

	if (dinfo->dfr_mode == DFR_MODE_LONGH_TE_SKIP_BY_MCU && dinfo->oled_info.ltpo_info.te_freq_num == 2) {
		dpu_pr_debug("ltpo te 120hz dcs cmd:");
		dfr_te_skip_print_dsi_cmds(&dinfo->oled_info.ltpo_info.dsi_cmds[PANEL_LTPO_DSI_CMD_TE_120HZ]);
		dpu_pr_debug("ltpo te 144hz dcs cmd:");
		dfr_te_skip_print_dsi_cmds(&dinfo->oled_info.ltpo_info.dsi_cmds[PANEL_LTPO_DSI_CMD_TE_144HZ]);
	}

	for (i = 0; i < DIMMING_TYPE_MAX; i++) {
		dimming_seq = dinfo->oled_info.ltpo_info.dimming_sequence[i];
		dpu_pr_info("%u dimming type, seq num: %u", dimming_seq.type, dimming_seq.dimming_seq_num);
		if (dimming_seq.type >= DIMMING_TYPE_MAX || dimming_seq.dimming_seq_num > DIMMING_SEQ_LEN_MAX)
			continue;
		for (j = 0; j < dimming_seq.dimming_seq_num; j++)
			dpu_pr_info("node %u: %u, %u", j, dimming_seq.dimming_seq_list[j].frm_rate,
				dimming_seq.dimming_seq_list[j].repeat_num);
	}

	dpu_pr_debug("refresh dcs cmd:");
	dfr_te_skip_print_dsi_cmds(&dinfo->oled_info.ltpo_info.dsi_cmds[PANEL_LTPO_DSI_CMD_REFRESH]);
	dpu_pr_debug("1hz refresh dcs cmd:");
	dfr_te_skip_print_dsi_cmds(&dinfo->oled_info.ltpo_info.dsi_cmds[PANEL_LTPO_DSI_CMD_REFRESH_1HZ]);
}

static int32_t dfr_te_skip_create_cmdlist(struct dpu_comp_dfr_ctrl *dfr_ctrl, int32_t cmdlist_type)
{
	int32_t ret;
	struct dfr_te_skip_ctrl *priv = dfr_ctrl->priv_data;
	uint32_t cmdlist_dev_id;

	dpu_check_and_return(cmdlist_type >= CMDLIST_TYPE_MAX, -1, err, "invalid cmdlist id");

	cmdlist_dev_id = CMDLIST_DEV_ID_DPU;

	priv->header_cmdlist_ids[cmdlist_type] =
		cmdlist_create_user_client(cmdlist_dev_id, priv->cmdlist_scene_id, SCENE_NOP_TYPE, 0, 0);
	if (unlikely(priv->header_cmdlist_ids[cmdlist_type] == 0)) {
		dpu_pr_err("scene_id=%u, create header cmdlist fail", priv->cmdlist_scene_id);
		return -1;
	}

	priv->reg_cmdlist_ids[cmdlist_type] =
		cmdlist_create_user_client(cmdlist_dev_id,
			priv->cmdlist_scene_id, REGISTER_CONFIG_TYPE, 0, PAGE_SIZE);
	if (unlikely(priv->reg_cmdlist_ids[cmdlist_type] == 0)) {
		ukmd_cmdlist_release_locked(cmdlist_dev_id,
			priv->cmdlist_scene_id, priv->header_cmdlist_ids[cmdlist_type]);
		dpu_pr_err("scene_id=%u create reg cmdlist fail", priv->cmdlist_scene_id);
		return -1;
	}

	ret = cmdlist_append_client(cmdlist_dev_id, priv->cmdlist_scene_id,
		priv->header_cmdlist_ids[cmdlist_type], priv->reg_cmdlist_ids[cmdlist_type]);
	if (unlikely(ret != 0)) {
		ukmd_cmdlist_release_locked(cmdlist_dev_id,
			priv->cmdlist_scene_id, priv->header_cmdlist_ids[cmdlist_type]);
		dpu_pr_err("append reg(%u) cmdlist fail", priv->reg_cmdlist_ids[cmdlist_type]);
		return -1;
	}

	return 0;
}

static int32_t dfr_te_skip_write_ddic_cmds_to_cmdlist(uint32_t dsi_offset, struct dsi_cmds *ddic_cmds,
	uint32_t cmdlist_id, uint32_t scene_id)
{
	int i = 0;
	dpu_check_and_return(!ddic_cmds, -1, err, "ddic_cmds is null");
	for (i = 0; i < (int)ddic_cmds->cmd_num; i++) {
		if (ddic_cmds->cmds[i].dtype == DTYPE_DCS_WRITE || ddic_cmds->cmds[i].dtype == DTYPE_DCS_WRITE1) {
			cmdlist_mipi_dsi_swrite_to(&ddic_cmds->cmds[i], dsi_offset, cmdlist_id, scene_id);
		} else if (ddic_cmds->cmds[i].dtype == DTYPE_GEN_LWRITE || ddic_cmds->cmds[i].dtype == DTYPE_DCS_LWRITE) {
			cmdlist_mipi_dsi_lwrite_to(&ddic_cmds->cmds[i], dsi_offset, cmdlist_id, scene_id);
		} else {
			dpu_pr_err("don't support dtype:%d", ddic_cmds->cmds[i].dtype);
			return -1;
		}
	}
	return 0;
}

#define BIT_LDI_EN 0
#define BIT_DUAL_LDI_EN 5
static int32_t dfr_te_skip_set_reg_dsi_ldi(struct dpu_comp_dfr_ctrl *dfr_ctrl, int32_t cmdlist_type)
{
	uint32_t val = 0;
	uint32_t connector_offset = 0;
	struct dpu_connector *connector = NULL;
	struct dfr_te_skip_ctrl *priv = dfr_ctrl->priv_data;
	struct dkmd_connector_info *pinfo = dfr_ctrl->dpu_comp->conn_info;
	uint32_t cmdlist_dev_id;

	dpu_check_and_return(cmdlist_type >= CMDLIST_TYPE_MAX, -1, err, "invalid cmdlist id");
	dpu_check_and_return(!pinfo, -1, err, "pinfo is null");

	connector = get_primary_connector(pinfo);
	dpu_check_and_return(!connector, -1, err, "connector is null");

	connector_offset = g_connector_offset[connector->connector_id];
	cmdlist_dev_id = CMDLIST_DEV_ID_DPU;
	ukmd_set_reg(cmdlist_dev_id, priv->cmdlist_scene_id, priv->reg_cmdlist_ids[cmdlist_type],
		DPU_DSI_LDI_FRM_MSK_UP_ADDR(connector_offset), 0x1);
	if (connector->bind_connector)
		ukmd_set_reg(cmdlist_dev_id, priv->cmdlist_scene_id, priv->reg_cmdlist_ids[cmdlist_type],
			DPU_DSI_LDI_FRM_MSK_UP_ADDR(g_connector_offset[connector->bind_connector->connector_id]),
			0x1);

	val = inp32(DPU_DSI_LDI_CTRL_ADDR(connector->connector_base));
	if (connector->bind_connector) {
		val |= BIT(BIT_DUAL_LDI_EN);
		ukmd_set_reg(cmdlist_dev_id, priv->cmdlist_scene_id, priv->reg_cmdlist_ids[cmdlist_type],
			DPU_DSI_LDI_CTRL_ADDR(connector_offset), val);
	} else {
		val |= BIT(BIT_LDI_EN);
		ukmd_set_reg(cmdlist_dev_id, priv->cmdlist_scene_id, priv->reg_cmdlist_ids[cmdlist_type],
			DPU_DSI_LDI_CTRL_ADDR(connector_offset), val);
	}

	return 0;
}

static int32_t dfr_te_skip_set_reg_dsi_dcs(struct dpu_comp_dfr_ctrl *dfr_ctrl,
	struct dsi_cmds *dsi_cmds, int32_t cmdlist_type)
{
	struct dpu_connector *connector = NULL;
	struct dfr_te_skip_ctrl *priv = dfr_ctrl->priv_data;
	struct dkmd_connector_info *pinfo = dfr_ctrl->dpu_comp->conn_info;

	dpu_check_and_return(cmdlist_type >= CMDLIST_TYPE_MAX, -1, err, "cmdlist type is invalid");
	dpu_check_and_return(!pinfo, -1, err, "pinfo is null");

	if (dsi_cmds->cmd_num == 0) {
		dpu_pr_err("dsi cmds is invalid");
		return 0;
	}
	connector = get_primary_connector(pinfo);
	dpu_check_and_return(!connector, -1, err, "connector is null");

	if (connector->bind_connector && likely(connector->bind_connector->connector_id < CONNECTOR_ID_MAX)) {
		dpu_pr_debug("write ddic cmds to dual mipi");
		dfr_te_skip_write_ddic_cmds_to_cmdlist(g_connector_offset[connector->bind_connector->connector_id],
		dsi_cmds, priv->reg_cmdlist_ids[cmdlist_type], priv->cmdlist_scene_id);
	}

	if (unlikely(connector->connector_id >= CONNECTOR_ID_MAX)) {
		dpu_pr_warn("connector->connector_id=%u is out of range", connector->connector_id);
		return -1;
	}
	return dfr_te_skip_write_ddic_cmds_to_cmdlist(g_connector_offset[connector->connector_id],
		dsi_cmds, priv->reg_cmdlist_ids[cmdlist_type], priv->cmdlist_scene_id);
}

static int32_t dfr_te_skip_setup_cmdlist(struct dpu_comp_dfr_ctrl *dfr_ctrl, int32_t cmdlist_type,
	int32_t dsi_dcs_type, bool set_ldi, bool set_dcs)
{
	struct dfr_te_skip_ctrl *priv = dfr_ctrl->priv_data;
	struct dfr_info *dinfo = dkmd_get_dfr_info(dfr_ctrl->dpu_comp->conn_info);
	dpu_check_and_return(!dinfo, -1, err, "dfr info is null");

	if (dfr_te_skip_create_cmdlist(dfr_ctrl, cmdlist_type) != 0)
		return -1;

	if (set_ldi && (dfr_te_skip_set_reg_dsi_ldi(dfr_ctrl, cmdlist_type) != 0))
		return -1;

	if (dsi_dcs_type >= PANEL_LTPO_DSI_CMD_MAX) {
		dpu_pr_err("dsi dcs type is invalid: %d", dsi_dcs_type);
		return -1;
	}
	if (set_dcs &&
		(dfr_te_skip_set_reg_dsi_dcs(dfr_ctrl, &dinfo->oled_info.ltpo_info.dsi_cmds[dsi_dcs_type], cmdlist_type) != 0))
		return -1;

	dpu_pr_info("cmdlist id: %u, cmdlist type: %d", priv->reg_cmdlist_ids[cmdlist_type], cmdlist_type);
	return 0;
}

static int32_t dfr_te_skip_setup_base_cmdlist(struct dpu_comp_dfr_ctrl *dfr_ctrl, struct dfr_info *dinfo)
{
	struct dfr_te_skip_ctrl *priv = dfr_ctrl->priv_data;

	dpu_check_and_return(!dinfo, -1, err, "dfr info is null");

	/* normal */
	if (dfr_te_skip_setup_cmdlist(dfr_ctrl, CMDLIST_SEND_FRM, 0, true, false) != 0)
		return -1;

	if (dinfo->dfr_mode == DFR_MODE_LONGH_TE_SKIP_BY_MCU && dinfo->oled_info.ltpo_info.te_freq_num == 2) {
		/* switch te to 120hz */
		if (dfr_te_skip_setup_cmdlist(dfr_ctrl, CMDLIST_SEND_WITH_120HZ_DCS,
			PANEL_LTPO_DSI_CMD_TE_120HZ, true, true) != 0)
			return -1;

		/* switch te to 144hz */
		if (dfr_te_skip_setup_cmdlist(dfr_ctrl, CMDLIST_SEND_WITH_144HZ_DCS,
			PANEL_LTPO_DSI_CMD_TE_144HZ, true, true) != 0)
			return -1;
	}

	priv->active_id = priv->reg_cmdlist_ids[CMDLIST_SEND_FRM];
	return 0;
}

#define LTPO_VER1_TE_RATE 90
#define SAFE_FRM_RATE_DEFAULT 120
#define LTPO_VER1_TE_RATE2 120
#define SAFE_FRM_RATE_LTPS_MAX 144
static int32_t dfr_te_skip_check_safe_frm_rate(struct dpu_comp_dfr_ctrl *dfr_ctrl, uint32_t safe_frm_rate)
{
	uint32_t parse_frm_rate = safe_frm_rate;
	struct dfr_info *dinfo = dkmd_get_dfr_info(dfr_ctrl->dpu_comp->conn_info);
	dpu_check_and_return(!dinfo, -1, err, "dfr info is null");
	
	dpu_pr_debug("check safe frm rate enter");
	if (dinfo->dfr_mode != DFR_MODE_LONGH_TE_SKIP_BY_MCU && safe_frm_rate > SAFE_FRM_RATE_DEFAULT) {
		dpu_pr_warn("safe_frm_rate is max, need set default safe frm rate!");
		parse_frm_rate = SAFE_FRM_RATE_DEFAULT;
	}

	if (dinfo->dfr_mode == DFR_MODE_LONGH_TE_SKIP_BY_MCU && safe_frm_rate > SAFE_FRM_RATE_LTPS_MAX) {
		dpu_pr_warn("safe_frm_rate is max, need set default safe frm rate!");
		parse_frm_rate = SAFE_FRM_RATE_LTPS_MAX;
	}
	
	if (parse_frm_rate == 0) {
		dpu_pr_debug("No need to send safe frm rate");
		if (g_debug_dpu_safe_frm_rate == 0)
			return -1;
		parse_frm_rate = g_debug_dpu_safe_frm_rate;
	}

	/* after switch te 90Hz, can not send safe frame rate */
	if (dfr_ctrl->cur_te_rate == LTPO_VER1_TE_RATE && parse_frm_rate != LTPO_VER1_TE_RATE) {
		dpu_pr_info("current te is %u, can not send safe frm rate !", dfr_ctrl->cur_te_rate);
		return -1;
	}
	/* after switch te 120Hz, can not 90Hz safe frm rate */
	if (dfr_ctrl->cur_te_rate == LTPO_VER1_TE_RATE2 && parse_frm_rate == LTPO_VER1_TE_RATE) {
		dpu_pr_info("current te is %u, can not send safe frm rate %u!", dfr_ctrl->cur_te_rate, parse_frm_rate);
		return -1;
	}
	dfr_ctrl->cur_safe_frm_rate = parse_frm_rate;
	return 0;
}

static void dfr_te_update_safe_frm_rate_to_dacc(uint32_t safe_frm_rate,
	struct dfr_te_skip_ctrl *priv, struct composer_manager *comp_mgr, dma_addr_t phy_addr)
{
	/* scene 6, 0x1EC, update safe frm rate cmdlist addr */
	outp32(DPU_DFR_MIN_FRAME_ADDR(comp_mgr->dpu_base), phy_addr);
	/* 0x1E4, used for safe frm rate */
	outp32(DPU_DFR_MIN_FRM_RATE_ADDR(comp_mgr->dpu_base), safe_frm_rate);
	return;
}

static int32_t dfr_te_skip_setup_dimming_cmdlist(struct dpu_comp_dfr_ctrl *dfr_ctrl)
{
	dma_addr_t phy_addr;
	struct dfr_te_skip_ctrl *priv = dfr_ctrl->priv_data;
	char __iomem *dpu_base = dfr_ctrl->dpu_comp->comp_mgr->dpu_base;
	struct dkmd_connector_info *pinfo = NULL;
	struct dfr_info *dinfo = NULL;
	dpu_check_and_return(!dfr_ctrl->dpu_comp->conn_info, -1, err, "conn_info is null");
	pinfo = dfr_ctrl->dpu_comp->conn_info;
	dinfo = dkmd_get_dfr_info(pinfo);
	
	/* refresh */
	if (dfr_te_skip_setup_cmdlist(dfr_ctrl, CMDLIST_REFRESH_FRM,
		PANEL_LTPO_DSI_CMD_REFRESH, false, true) != 0) {
		dpu_pr_warn("create refresh cmdlist failed");
		return -1;
	}
	
	phy_addr = cmdlist_get_phy_addr(CMDLIST_DEV_ID_DPU,
		priv->cmdlist_scene_id, priv->reg_cmdlist_ids[CMDLIST_REFRESH_FRM]);
	if (phy_addr != 0) {
		/* 0x1E0, used for refresh cmdlist addr */
		outp32(DPU_DFR_REFRESH_ADDR(dpu_base), phy_addr);
	} else {
		dpu_pr_warn("invalid refresh cmdlist");
		return -1;
	}

	if (dinfo->ddic_type == DDIC_TYPE_F01) {
		/* refresh_ext */
		if (dfr_te_skip_setup_cmdlist(dfr_ctrl, CMDLIST_REFRESH_FRM_EXT,
			PANEL_LTPO_DSI_CMD_REFRESH_EXT, false, true) != 0) {
			dpu_pr_warn("create refresh_ext cmdlist failed");
			return -1;
		}

		phy_addr = cmdlist_get_phy_addr(CMDLIST_DEV_ID_DPU,
			priv->cmdlist_scene_id, priv->reg_cmdlist_ids[CMDLIST_REFRESH_FRM_EXT]);
		if (phy_addr != 0) {
			outp32(DPU_DFR_REFRESH_EXT_ADDR(dpu_base), phy_addr);
		} else {
			dpu_pr_warn("invalid refresh_ext cmdlist");
			return -1;
		}
	}

	if (!is_ppu_support(&pinfo->base))
		return 0;
	
	/* ppu mode, full refresh cmds */
	if (dfr_te_skip_setup_cmdlist(dfr_ctrl, CMDLIST_REFRESH_PPU_FULL,
		PANEL_LTPO_DSI_CMD_REFRESH_PPU_FULL, false, true) != 0) {
		dpu_pr_warn("create ppu full refresh cmdlist failed");
		return -1;
	}
	
	phy_addr = cmdlist_get_phy_addr(CMDLIST_DEV_ID_DPU,
		priv->cmdlist_scene_id, priv->reg_cmdlist_ids[CMDLIST_REFRESH_PPU_FULL]);
	if (phy_addr != 0) {
		outp32(DPU_DFR_FULL_FEFRESH_CMDLIST(dpu_base), phy_addr);
	} else {
		dpu_pr_warn("invalid refresh cmdlist");
		return -1;
	}
	
	/* ppu mode, high region refresh cmds */
	if (dfr_te_skip_setup_cmdlist(dfr_ctrl, CMDLIST_REFRESH_PPU_HIGH,
		PANEL_LTPO_DSI_CMD_REFRESH_PPU_HIGH, false, true) != 0) {
		dpu_pr_warn("create ppu high region refresh cmdlist failed");
		return -1;
	}
	
	phy_addr = cmdlist_get_phy_addr(CMDLIST_DEV_ID_DPU,
		priv->cmdlist_scene_id, priv->reg_cmdlist_ids[CMDLIST_REFRESH_PPU_HIGH]);
	if (phy_addr != 0) {
		outp32(DPU_DFR_HIGH_REFRESH_CMDLIST(dpu_base), phy_addr);
	} else {
		dpu_pr_warn("invalid refresh cmdlist");
		return -1;
	}
	
	return 0;
}

static int32_t dfr_te_skip_dimming_done_isr_notify(struct notifier_block *self,
	unsigned long action, void *data)
{
	struct ukmd_listener_data *listener_data = (struct ukmd_listener_data *)data;
	struct dpu_comp_dfr_ctrl *dfr_ctrl = (struct dpu_comp_dfr_ctrl *)(listener_data->data);
	struct composer_manager *comp_mgr = dfr_ctrl->dpu_comp->comp_mgr;

	composer_mgr_disable_dimming(comp_mgr, dfr_ctrl->dpu_comp->comp.index);

	return 0;
}

static struct notifier_block dfr_te_skip_dimming_done_isr_notifier = {
	.notifier_call = dfr_te_skip_dimming_done_isr_notify,
};

static void dfr_te_skip_setup_dimming_listener(struct dpu_comp_dfr_ctrl *dfr_ctrl)
{
	struct ukmd_isr *isr_ctrl = &dfr_ctrl->dpu_comp->comp_mgr->mdp_isr_ctrl;
	struct dfr_te_skip_ctrl *priv = (struct dfr_te_skip_ctrl *)dfr_ctrl->priv_data;

	priv->notifier = &dfr_te_skip_dimming_done_isr_notifier;

	ukmd_isr_register_listener(isr_ctrl, priv->notifier, NOTIFY_DIMMING_DONE, dfr_ctrl);
}

static void dfr_te_skip_write_dimming_seq_to_dacc(struct dimming_sequence *dimming_seq,
	char __iomem *dpu_base, struct dfr_te_skip_ctrl *priv)
{
	uint32_t i;
	char __iomem *dimming_seq_base = NULL;
	uint32_t dimming_seq_num = 0;
	uint32_t dimming_frame_info = 0;

	dimming_seq_num = inp32(DPU_DFR_DIMMING_SEQ_LEN_ADDR(dpu_base));
	switch (dimming_seq->type) {
	case DIMMING_NORMAL:
		/* bit0-bit8: normal dimming seq num */
		dimming_seq_num |= (dimming_seq->dimming_seq_num & 0xFF);
		/* write dimming seq list */
		dimming_seq_base = DPU_DFR_DIMMING_SEQ_BASE_ADDR(dpu_base);
		break;
	default:
		dpu_pr_warn("not support dimming type %d", dimming_seq->type);
		return;
	}

	dpu_pr_debug("dimming seq num is %u", dimming_seq_num);
	outp32(DPU_DFR_DIMMING_SEQ_LEN_ADDR(dpu_base), dimming_seq_num);
	for (i = 0; i < dimming_seq->dimming_seq_num; i += 2) {
		dimming_frame_info = 0;
		dimming_frame_info |= dimming_seq->dimming_seq_list[i].frm_rate & 0xFF;
		dimming_frame_info |= (dimming_seq->dimming_seq_list[i].repeat_num & 0xFF) << 8;
		dimming_frame_info |= (dimming_seq->dimming_seq_list[i + 1].frm_rate & 0xFF) << 16;
		dimming_frame_info |= (dimming_seq->dimming_seq_list[i + 1].repeat_num & 0xFF) << 24;
		outp32(dimming_seq_base, dimming_frame_info);
		dimming_seq_base += 4;
	}
}

static void dfr_te_skip_write_dimming_gear_to_dacc(struct dimming_gear_config *dimming_gear,
	char __iomem *dimming_gear_base)
{
	uint32_t dimming_gear_info = 0;

	dimming_gear_info |= dimming_gear->frm_rate & 0xFF;
	dimming_gear_info |= (dimming_gear->dimming_gear1 & 0xFF) << 8;
	dimming_gear_info |= (dimming_gear->dimming_gear2 & 0xFF) << 16;
	dimming_gear_info |= (dimming_gear->dimming_end & 0xFF) << 24;
	dpu_pr_debug("dimming_gear_info %u", dimming_gear_info);
	outp32(dimming_gear_base, dimming_gear_info);
}

static void dfr_te_skip_setup_dimming(struct dpu_comp_dfr_ctrl *dfr_ctrl)
{
	uint32_t i;
	uint32_t dimming_num;
	char __iomem *dpu_base = NULL;
	struct dfr_te_skip_ctrl *priv = dfr_ctrl->priv_data;
	struct dfr_info *dinfo = dkmd_get_dfr_info(dfr_ctrl->dpu_comp->conn_info);
	struct dimming_sequence *dimming_seq = NULL;
	struct dimming_gear_config *dimming_gear = NULL;
	char __iomem *dimming_gear_base = NULL;
	dpu_check_and_no_retval(!dinfo, err, "dfr info is null");

	if (dfr_te_skip_setup_dimming_cmdlist(dfr_ctrl) != 0) {
		dpu_pr_warn("setup dimming cmdlist failed");
		return;
	}

	dpu_base = dfr_ctrl->dpu_comp->comp_mgr->dpu_base;
	for (i = 0; i < DIMMING_TYPE_MAX; i++) {
		dimming_seq = &dinfo->oled_info.ltpo_info.dimming_sequence[i];
		if (!dimming_seq || dimming_seq->dimming_seq_num == 0 ||
			dimming_seq->dimming_seq_num > DIMMING_SEQ_LEN_MAX) {
			dpu_pr_warn("dimming_seq is null or dimming seq num is invalid");
			continue;
		}
		dfr_te_skip_write_dimming_seq_to_dacc(dimming_seq, dpu_base, priv);
	}

	/* write dimming gear to dacc */
	if (dinfo->oled_info.ltpo_info.dimming_gear_len != 0) {
		dpu_pr_debug("dimming_gear_len is %u", dinfo->oled_info.ltpo_info.dimming_gear_len);
		dimming_num = inp32(DPU_DFR_DIMMING_SEQ_LEN_ADDR(dpu_base));
		dimming_num |= (dinfo->oled_info.ltpo_info.dimming_gear_len & 0xFF) << 8;
		outp32(DPU_DFR_DIMMING_SEQ_LEN_ADDR(dpu_base), dimming_num);
		dimming_gear_base = DPU_DFR_DIMMING_GEAR_CONFIG_ADDR(dpu_base);
		for (i = 0; i < min(dinfo->oled_info.ltpo_info.dimming_gear_len, DIMMING_GEAR_CONFIG_MAX); i++) {
			dimming_gear = &dinfo->oled_info.ltpo_info.dimming_gear_config[i];
			if (!dimming_gear || !dimming_gear_base || dimming_gear->frm_rate == 0) {
				dpu_pr_warn("dimming_gear is null");
				continue;
			}
			dfr_te_skip_write_dimming_gear_to_dacc(dimming_gear, dimming_gear_base);
			dimming_gear_base += 4;
		}
	}

	/* enable frm dimming */
	if (dinfo->oled_info.ltpo_info.dimming_enable)
		outp32(DPU_DFR_ENABLE_FRM_DIMMING_ADDR(dpu_base), 1);

	dfr_te_skip_setup_dimming_listener(dfr_ctrl);
}

static uint32_t dfr_te_skip_get_te_mask(struct dpu_comp_dfr_ctrl *dfr_ctrl)
{
	int i;
	struct dfr_info *dinfo = dkmd_get_dfr_info(dfr_ctrl->dpu_comp->conn_info);
	
	dpu_check_and_return(!dinfo, 0, err, "dfr info is null");
	dpu_check_and_return(dinfo->oled_info.ltpo_info.te_mask_num == 0, 0, err, "invalid te mask num");
	
	if (dfr_ctrl->cur_frm_rate == 0)
		return dinfo->oled_info.ltpo_info.te_masks[0];
	
	for (i = 0; i < TE_FREQ_NUM_MAX; i++) {
		if (dinfo->oled_info.ltpo_info.te_freqs[i] % dfr_ctrl->cur_frm_rate == 0)
			return dinfo->oled_info.ltpo_info.te_masks[i];
	}
	
	dpu_pr_err("frm rate is %uhz, no valid te freq to devide", dfr_ctrl->cur_frm_rate);
	
	// use te mask default value 0
	return 0;
}

/* 120 & 90 divided strategy */
static uint32_t dfr_te_skip_get_te_rate(struct dpu_comp_dfr_ctrl *dfr_ctrl)
{
	int i;
	struct dfr_info *dinfo = dkmd_get_dfr_info(dfr_ctrl->dpu_comp->conn_info);

	dpu_check_and_return(!dinfo, 0, err, "dfr info is null");
	dpu_check_and_return(dinfo->oled_info.ltpo_info.te_freq_num == 0, 0, err, "invalid te freq");

	if (dfr_ctrl->cur_frm_rate == 0)
		return dinfo->oled_info.ltpo_info.te_freqs[0];
	for (i = 0; i < TE_FREQ_NUM_MAX; i++) {
		if (dinfo->oled_info.ltpo_info.te_freqs[i] % dfr_ctrl->cur_frm_rate == 0)
			return dinfo->oled_info.ltpo_info.te_freqs[i];
	}

	dpu_pr_err("frm rate is %uhz, no valid te freq to devide", dfr_ctrl->cur_frm_rate);
	return 0;
}

void dfr_te_skip_setup_priv_data(struct dpu_comp_dfr_ctrl *dfr_ctrl)
{
	struct dfr_te_skip_ctrl *priv;
	char __iomem *dpu_base = NULL;
	struct dfr_info *dinfo = NULL;
	uint32_t init_config = 0;
	uint32_t i = 0;
	dpu_check_and_no_retval(!dfr_ctrl, err, "dfr_ctrl is null");
	dpu_check_and_no_retval(!dfr_ctrl->dpu_comp, err, "dpu_comp is null");
	dpu_check_and_no_retval(!dfr_ctrl->dpu_comp->conn_info, err, "conn_info is null");

	dinfo = dkmd_get_dfr_info(dfr_ctrl->dpu_comp->conn_info);

	dpu_check_and_no_retval(!dinfo, err, "dfr info is null");
	dpu_check_and_no_retval(!dfr_ctrl->dpu_comp->comp_mgr, err, "comp_mgr is null");

	dpu_base = dfr_ctrl->dpu_comp->comp_mgr->dpu_base;
	dpu_check_and_no_retval(dinfo->dfr_mode != DFR_MODE_TE_SKIP_BY_MCU &&
		dinfo->dfr_mode != DFR_MODE_LONGH_TE_SKIP_BY_MCU, err, "invalid dfr mode:%d", dinfo->dfr_mode);
	dpu_check_and_no_retval(dinfo->oled_info.oled_type != PANEL_OLED_LTPO,
		err, "invalid oled type:%d", dinfo->oled_info.oled_type);

	dfr_te_skip_print_dfr_info(dinfo);

	dfr_ctrl->priv_data = (void*)&g_te_skip_ctrl;
	priv = (struct dfr_te_skip_ctrl *)dfr_ctrl->priv_data;

	priv->cmdlist_scene_id = DPU_SCENE_ONLINE_3;
	dfr_ctrl->pre_te_rate = 0;
	dfr_ctrl->pre_te_mask_num = 0;
	dfr_ctrl->vsync_offset_threshold = 1500;

	if (dfr_te_skip_setup_base_cmdlist(dfr_ctrl, dinfo) != 0)
		return;
	for (i = 0; i < LTPO_RAM_SIZE; i += 4)
		outp32(DPU_DFR_ENABLE_ADDR(dpu_base) + i, 0);
	outp32(DPU_DFR_TE_IDX_ADDR(dpu_base), dfr_ctrl->dpu_comp->conn_info->base.lcd_te_idx);
	/* unmask acpu notify dacc intr */
	outp32(DPU_RISCV_INTR_MASK_ADDR(dpu_base), 0xFF00);
	/* notifiy dacc resume to dacc 0x80： bit7 is acpu interrupt */
	outp32(DPU_RISCV_INTR_TRIG_ADDR(dpu_base), 0x80);
	/* init config 0x1F4 */
	init_config = inp32(DPU_DFR_INIT_CONFIG_ADDR(dpu_base));
	init_config &= 0xFFFFFC00;
	dpu_pr_info("ddic_type is %d", dinfo->ddic_type);
	/* bit0 ~bit1      : ddic_type */
	if (dinfo->ddic_type < DDIC_TYPE_INVALID)
		init_config |= (dinfo->ddic_type & 0x3);
	else
		dpu_pr_warn("invalid ddic_type, init_config bit1~bit2 set 0");

	/* bit2~bit9 : fps */
	init_config |= (dfr_ctrl->dpu_comp->conn_info->base.fps & 0xFF) << 2;
	dpu_pr_info("dfr_te_skip_setup_priv_data init_config = %u", init_config);
	outp32(DPU_DFR_INIT_CONFIG_ADDR(dpu_base), init_config);
	/* enable dacc dfr */
	outp32(DPU_DFR_ENABLE_ADDR(dpu_base), 1);
	outp32(DPU_DFR_DACC_CMDS_ASYNC_ADDR(dpu_base), 1);

	dfr_te_skip_setup_dimming(dfr_ctrl);
	dpu_ppu_setup_priv_data(&dfr_ctrl->ppu_ctrl, dfr_ctrl);

	/* do not set safe frame rate in 90Hz TE case */
	if (dfr_ctrl->cur_te_rate != LTPO_VER1_TE_RATE)
		priv->resend_safe_frm_rate = true;
}

void dfr_te_skip_release_priv_data(struct dpu_comp_dfr_ctrl *dfr_ctrl)
{
	struct dpu_composer *dpu_comp = NULL;
	struct dfr_te_skip_ctrl *priv = NULL;
	struct ukmd_isr *isr_ctrl = NULL;
	char __iomem *dpu_base = NULL;
	int32_t i;

	dpu_check_and_no_retval(!dfr_ctrl, err, "dfr_ctrl is null");
	dpu_check_and_no_retval(!dfr_ctrl->dpu_comp, err, "dfr info is null");
	dpu_check_and_no_retval(!dfr_ctrl->dpu_comp->conn_info, err, "conn_info is null");
	dpu_check_and_no_retval(!dfr_ctrl->priv_data, err, "dfr_ctrl->priv_data is null");

	dpu_comp = dfr_ctrl->dpu_comp;
	priv = (struct dfr_te_skip_ctrl *)dfr_ctrl->priv_data;
	isr_ctrl = &dpu_comp->comp_mgr->mdp_isr_ctrl;
	dpu_base = dpu_comp->comp_mgr->dpu_base;
	dpu_pr_info("+");

	for (i = 0; i < CMDLIST_TYPE_MAX; i++) {
		if (priv->header_cmdlist_ids[i] != 0) {
			ukmd_cmdlist_release_locked(CMDLIST_DEV_ID_DPU,
				priv->cmdlist_scene_id, priv->header_cmdlist_ids[i]);
			priv->header_cmdlist_ids[i] = 0;
		}
	}

	ukmd_isr_unregister_listener(isr_ctrl, priv->notifier, NOTIFY_DIMMING_DONE);
	/* disable frm dimming */
	outp32(DPU_DFR_ENABLE_FRM_DIMMING_ADDR(dpu_base), 0);
	/* disable dacc dfr */
	outp32(DPU_DFR_ENABLE_ADDR(dpu_base), 0);
	outp32(DPU_DFR_DACC_CMDS_ASYNC_ADDR(dpu_base), 0);
	/* restore te rate */
	outp32(DPU_DFR_TE_RATE_ADDR(dpu_base), dfr_te_skip_get_te_rate(dfr_ctrl));
	/* update te mask */
	outp32(DPU_DFR_TE_MASK_NUM_ADDR(dpu_base), dfr_te_skip_get_te_mask(dfr_ctrl));
	composer_mgr_disable_dimming(dpu_comp->comp_mgr, dpu_comp->comp.index);
}

void dfr_te_skip_power_off_sub(struct dpu_comp_dfr_ctrl *dfr_ctrl)
{
	dpu_ppu_release_priv_data(&dfr_ctrl->ppu_ctrl);
}

static void dfr_te_skip_switch_te_rate(struct dpu_comp_dfr_ctrl *dfr_ctrl, struct sfr_info *sfr_info)
{
	uint32_t safe_frm_rate = 1;
	uint32_t current_config = 0;
	char __iomem *dpu_base = dfr_ctrl->dpu_comp->comp_mgr->dpu_base;
	struct dfr_info *dinfo = dkmd_get_dfr_info(dfr_ctrl->dpu_comp->conn_info);
	struct dfr_te_skip_ctrl *priv = dfr_ctrl->priv_data;
	dpu_check_and_no_retval(!dinfo, err, "dinfo is null");
	dpu_check_and_no_retval(!priv, err, "priv is null");

	/* ltpo1.0 */
	if (dinfo->oled_info.ltpo_info.ver != PANEL_LTPO_V1 &&
		dinfo->dfr_mode != DFR_MODE_LONGH_TE_SKIP_BY_MCU)
		return;
	if ((dfr_ctrl->pre_te_rate == 0 || dfr_ctrl->pre_te_rate == dfr_ctrl->cur_te_rate) &&
		(dfr_ctrl->resend_switch_cmds == false)) {
		return;
	}

	switch (dfr_ctrl->cur_te_rate) {
	case 120:
		safe_frm_rate = 1; /* switch frm rate to 120, set safe frm rate 1Hz */
		current_config |= BIT(0);
		break;
	case 90:
		safe_frm_rate = 90; /* switch frm rate to 90, set safe frm rate 90Hz */
		current_config |= BIT(1);
		break;
	default:
		dpu_pr_err("invalid te freq: %u", dfr_ctrl->cur_te_rate);
		break;
	}

	if (dinfo->dfr_mode == DFR_MODE_LONGH_TE_SKIP_BY_MCU) {
		if (dfr_ctrl->cur_te_rate == TE_144HZ)
			priv->active_id = priv->reg_cmdlist_ids[CMDLIST_SEND_WITH_144HZ_DCS];
		else if (dfr_ctrl->cur_te_rate == TE_120HZ)
			priv->active_id = priv->reg_cmdlist_ids[CMDLIST_SEND_WITH_120HZ_DCS];
		safe_frm_rate = dfr_ctrl->cur_frm_rate;
	}

	dfr_ctrl->follow_frame = true;
	sfr_info->info_type |= (INFO_TYPE_SFR_SEND | INFO_TYPE_SFR_RESEND);
	sfr_info->full_mode_sfr = safe_frm_rate & 0x0000FFFF;
	sfr_info->ppu_mode_sfr = (safe_frm_rate >> 16) & 0x0000FFFF;
	if (dfr_ctrl->resend_switch_cmds) {
		dpu_pr_info("Need to adjust te");
		dfr_ctrl->resend_switch_cmds = false;
		outp32(DPU_DFR_DACC_TE_CHECK_ADDR(dpu_base), current_config);
	}		
}

/* 120hz to 144hz return true, 144hz to 120hz set delay cnt */
static bool dfr_ltpo_dsi_timing_switch(struct dpu_comp_dfr_ctrl *dfr_ctrl,
	struct dfr_te_skip_ctrl *priv)
{
	if (dfr_ctrl->pre_frm_rate == dfr_ctrl->cur_frm_rate)
		return false;
	
	if (dfr_ctrl->pre_te_rate == dfr_ctrl->cur_te_rate)
		return false;
	if (dfr_ctrl->pre_te_rate == TE_144HZ) {
		priv->porch_switch_delay_cnt = PORCH_SWITCH_DELAY_MAX_CNT;
		return false;
	}
	return true;
}

int32_t dfr_te_skip_switch_frm_rate(struct dpu_comp_dfr_ctrl *dfr_ctrl, uint32_t frame_rate)
{
	char __iomem *dpu_base = dfr_ctrl->dpu_comp->comp_mgr->dpu_base;
	struct dkmd_connector_info *pinfo = dfr_ctrl->dpu_comp->conn_info;
	struct dfr_info *dinfo = dkmd_get_dfr_info(dfr_ctrl->dpu_comp->conn_info);
	struct dpu_panel_ops *pops = get_panel_ops(dfr_ctrl->dpu_comp->conn_info->base.id);
	struct dfr_te_skip_ctrl *priv = dfr_ctrl->priv_data;
	uint32_t dimming_status;
	struct sfr_info sfr_info = {0};

	dpu_check_and_return(!pinfo, -1, err, "pinfo is null");
	dpu_check_and_return(!dinfo, -1, err, "dfr info is null");
	dpu_check_and_return(!pops, -1, err, "pops is null");
	dpu_check_and_return(!priv, -1, err, "priv is null");

	if (dfr_ctrl->pre_te_rate == 0 && dfr_ctrl->cur_te_rate == LTPO_VER1_TE_RATE) {
		dpu_pr_info("resume 90hz te");
		pipeline_next_ops_handle(pinfo->conn_device, pinfo, CMDS_SYNC_TX,
			&dinfo->oled_info.ltpo_info.dsi_cmds[PANEL_LTPO_DSI_CMD_TE_90HZ]);
		outp32(DPU_DFR_ENABLE_FRM_DIMMING_ADDR(dpu_base), 0);
	}

	if (dfr_ctrl->pre_te_rate == 0 && dfr_ctrl->cur_te_rate == TE_144HZ &&
		dinfo->dfr_mode == DFR_MODE_LONGH_TE_SKIP_BY_MCU) {
		dpu_pr_info("resume 144hz te");
		pipeline_next_ops_handle(pinfo->conn_device, pinfo, CMDS_SYNC_TX,
			&dinfo->oled_info.ltpo_info.dsi_cmds[PANEL_LTPO_DSI_CMD_TE_144HZ]);
		outp32(DPU_DFR_ENABLE_FRM_DIMMING_ADDR(dpu_base), 0);
	}

	dfr_ctrl->pre_te_mask_num = dfr_ctrl->cur_te_mask_num;
	dfr_ctrl->pre_te_rate = dfr_ctrl->cur_te_rate;
	dfr_ctrl->pre_frm_rate = dfr_ctrl->cur_frm_rate;
	dfr_ctrl->cur_frm_rate = frame_rate;
	dfr_ctrl->cur_te_rate = dfr_te_skip_get_te_rate(dfr_ctrl);
	dfr_ctrl->cur_te_mask_num = dfr_te_skip_get_te_mask(dfr_ctrl);
	if (dfr_ctrl->pre_frm_rate != dfr_ctrl->cur_frm_rate || dfr_ctrl->resend_switch_cmds) {
		dpu_pr_info("switch frame rate from %dhz to %dhz", dfr_ctrl->pre_frm_rate, dfr_ctrl->cur_frm_rate);
		dpu_pr_info("switch te rate from %u to %u", dfr_ctrl->pre_te_rate, dfr_ctrl->cur_te_rate);
		dpu_pr_info("switch te mask num from %u to %u", dfr_ctrl->pre_te_mask_num, dfr_ctrl->cur_te_mask_num);
		if (pops->set_current_fps)
			pops->set_current_fps(dfr_ctrl->cur_frm_rate);
		/* Trigger safe frame rate decision when fps switch */
		/* AGP switch frm rate, sfr need to set with frame */
		sfr_info.info_type = INFO_TYPE_AFR_SEND;
		sfr_info.afr = dfr_ctrl->cur_frm_rate;
		dfr_te_skip_switch_te_rate(dfr_ctrl, &sfr_info);
		pipeline_next_ops_handle(dfr_ctrl->dpu_comp->conn_info->conn_device,
			dfr_ctrl->dpu_comp->conn_info, NOTIFY_SFR_INFO, &sfr_info);
		dfr_ctrl->follow_frame = false;
	}

	if (dinfo->dfr_mode == DFR_MODE_LONGH_TE_SKIP_BY_MCU) {
		/* mipi porch parameter switch */
		if (dfr_ltpo_dsi_timing_switch(dfr_ctrl, priv) && !priv->is_porch_switch) {
			dpu_pr_info("switch mipi rate from %d to %d\n", dfr_ctrl->pre_frm_rate, dfr_ctrl->cur_frm_rate);
			priv->is_porch_switch = true;
		}
		/* porch switch delay 2 frame when 144Hz switch to 120Hz  */
		if (priv->porch_switch_delay_cnt > 0 && !priv->is_porch_switch) {
			priv->porch_switch_delay_cnt--;
			if (priv->porch_switch_delay_cnt == 0)
				priv->is_porch_switch = true;
			dpu_pr_info("delay switch mipi rate to %d\n", dfr_ctrl->cur_frm_rate);
		}
	}

	dpu_ppu_process(&dfr_ctrl->ppu_ctrl);
	dfr_ctrl->cur_te_rate = dpu_ppu_get_te_rate(&dfr_ctrl->ppu_ctrl);
	dfr_ctrl->cur_te_mask_num = dpu_ppu_get_te_mask_num(&dfr_ctrl->ppu_ctrl);
	/* update frame rate to dacc */
	outp32(DPU_DFR_FRM_RATE_ADDR(dpu_base), dfr_ctrl->cur_frm_rate);
	/* update te rate to dacc */
	outp32(DPU_DFR_TE_RATE_ADDR(dpu_base), dfr_ctrl->cur_te_rate);
	/* update te mask to dacc */
	outp32(DPU_DFR_TE_MASK_NUM_ADDR(dpu_base), dfr_ctrl->cur_te_mask_num);

	/* ltpo1.0: when te rate is 90Hz, disable dimming */
	if (dfr_ctrl->pre_te_rate == dfr_ctrl->cur_te_rate)
		return 0;
	dpu_pr_info("cur te rate is %u, cur te mask num is %u, switch dimming status", dfr_ctrl->cur_te_rate, dfr_ctrl->cur_te_mask_num);
	dimming_status = dfr_ctrl->cur_te_rate == LTPO_VER1_TE_RATE ? 0 : 1;
	/* ltpo2.0: disable dimming */
	dimming_status = dinfo->dfr_mode == DFR_MODE_LONGH_TE_SKIP_BY_MCU ? 0 : dimming_status;
	outp32(DPU_DFR_ENABLE_FRM_DIMMING_ADDR(dpu_base), dimming_status);
	return 0;
}

static void dfr_te_skip_check_send_safe_frm_rate(struct dpu_comp_dfr_ctrl *dfr_ctrl,
	struct dfr_te_skip_ctrl *priv, struct composer_manager *comp_mgr)
{
	struct sfr_info sfr_info = {0};

	if (priv->resend_safe_frm_rate) {
		priv->resend_safe_frm_rate = false;
		sfr_info.info_type = INFO_TYPE_SFR_RESEND;
		pipeline_next_ops_handle(dfr_ctrl->dpu_comp->conn_info->conn_device,
			dfr_ctrl->dpu_comp->conn_info, NOTIFY_SFR_INFO, &sfr_info);
		dpu_pr_info("Need resend safe frm rate in the first frame commit after power on");
	}
}

static void dfr_te_skip_update_timestamp(struct dpu_comp_dfr_ctrl *dfr_ctrl)
{
	uint64_t expect_timestamp_low;
	uint64_t expect_timestamp_high;
	uint64_t current_time;
	uint32_t timesatmp_info = 0;
	expect_timestamp_high = (dfr_ctrl->request_time_manos >> 32) & (0xFFFFFFFF);
	expect_timestamp_low = dfr_ctrl->request_time_manos & (0xFFFFFFFF);
 
	/* update timestamp */
	outp32(DFR_EXPECT_TIMESTAMP_LOW32(dfr_ctrl->dpu_comp->comp_mgr->dpu_base), expect_timestamp_low);
	outp32(DFR_EXPECT_TIMESTAMP_HIGH32(dfr_ctrl->dpu_comp->comp_mgr->dpu_base), expect_timestamp_high);
	current_time = (uint64_t)ktime_to_ns(ktime_get());
 
	dpu_pr_debug("display frmId: %llu enable: %d RequestTimeNanos:%llu, cur_time:%llu, timebuf:%llu,"
		"expected timestamp low_32 is %u, expected timestamp high_32 is %u",
		dfr_ctrl->frame_id, dfr_ctrl->time_nanos_type, dfr_ctrl->request_time_manos,
		current_time, dfr_ctrl->request_time_manos - current_time, expect_timestamp_low, expect_timestamp_high);
	
	/* bit 0: enbale time flag; bit 1: adaptive vsync, bit 2-bit31: frame id */
	timesatmp_info = (uint32_t)((dfr_ctrl->time_nanos_type & 0x3) | ((dfr_ctrl->frame_id & 0xFFFFFFFF) << 2));
	dpu_pr_debug("frame_id is %llu, enableFlag is %u, display info is %u",
		dfr_ctrl->frame_id, dfr_ctrl->time_nanos_type, timesatmp_info);
	/* update frame info */
	outp32(DFR_FRAME_INFO_DISPLAYED(dfr_ctrl->dpu_comp->comp_mgr->dpu_base), timesatmp_info);
}

int32_t dfr_te_skip_commit(struct dpu_comp_dfr_ctrl *dfr_ctrl)
{
	struct composer_manager *comp_mgr = dfr_ctrl->dpu_comp->comp_mgr;
	char __iomem *dpu_base = comp_mgr->dpu_base;
	struct dfr_te_skip_ctrl *priv = dfr_ctrl->priv_data;
	dma_addr_t phy_addr = 0;
	struct dfr_info *dinfo = dkmd_get_dfr_info(dfr_ctrl->dpu_comp->conn_info);

	dpu_check_and_return(!dinfo, -1, err, "dfr info is null");
	dpu_check_and_return(!priv, -1, err, "priv is null");

	dpu_pr_debug("active id: %u", priv->active_id);
	phy_addr = cmdlist_get_phy_addr(CMDLIST_DEV_ID_DPU, priv->cmdlist_scene_id, priv->active_id);
	if (phy_addr == 0) {
		dpu_pr_err("invalid active cmdlist %u, commit failed", priv->active_id);
		return -1;
	}
#ifdef CONFIG_DKMD_DPU_OHOS
	dfr_te_skip_update_timestamp(dfr_ctrl);
#endif
	/* scene 6, 0x1D8, used for cmdlist addr */
	outp32(DPU_DFR_PRESENT_ADDR(dpu_base), phy_addr);

	/* notifiy frame update to dacc 0x10: bit4 is acpu interrupt */
	outp32(DPU_RISCV_INTR_TRIG_ADDR(dpu_base), 0x10);

	if (dinfo->oled_info.ltpo_info.dimming_enable)
		composer_mgr_enable_dimming(comp_mgr, dfr_ctrl->dpu_comp->comp.index);

	/* check send safe frm rate */
	dfr_te_skip_check_send_safe_frm_rate(dfr_ctrl, priv, comp_mgr);
	return 0;
}

static int32_t dfr_te_skip_send_dcs_cmds_by_async(struct dpu_composer *dpu_comp,
	struct disp_effect_params *effect_params)
{
	struct dkmd_connector_info *pinfo = NULL;
	struct mipi_dsi_cmds_info dsi_cmds_info = { 0 };
	struct comp_online_present *present = NULL;
	uint32_t bl_level = 0;
	uint32_t i = 0;

	dpu_check_and_return(!dpu_comp, -1, err, "dpu_comp is null");
	pinfo = dpu_comp->conn_info;
	dpu_check_and_return(!pinfo, -1, err, "pinfo is null");
	present = (struct comp_online_present *)dpu_comp->present_data;

	for (; i < effect_params->effect_num; i++) {
		if (effect_params->params[i].effect_type == DISP_EFFECT_WITH_BL) {
			bl_level = effect_params->params[i].effect_values[0];
			break;
		}
		dpu_pr_warn("async not support param type is %u", effect_params->params[i].effect_type);
	}

	dsi_cmds_info.cmds_info.cmds_type = DKMD_CMDS_TYPE_BL;
	dsi_cmds_info.cmds_info.data_len = 2;
	dsi_cmds_info.cmds_info.data[0] = (int32_t)(present->dfr_ctrl.ppu_ctrl.panel_refresh_mode);
	dsi_cmds_info.cmds_info.data[1] = (int32_t)bl_level;
	dsi_cmds_info.follow_frame = true;
	dsi_cmds_info.is_force_sync = false;
	dpu_pr_info("panel_refresh_mode = %d", dsi_cmds_info.cmds_info.data[0]);
	pipeline_next_ops_handle(pinfo->conn_device, pinfo, SEND_CMDS, &dsi_cmds_info);
	pipeline_next_ops_handle(pinfo->conn_device, pinfo, TRACE_SCREEN_BL, &bl_level);
	return 0;
}

int32_t dfr_te_skip_send_dcs_cmds(struct dpu_comp_dfr_ctrl *dfr_ctrl, struct disp_effect_params *effect_params)
{
	struct dpu_bl_ctrl *bl_ctrl = NULL;

	dpu_check_and_return(!dfr_ctrl, -1, err, "dfr_ctrl is null");
	dpu_check_and_return(!effect_params, -1, err, "effect_params is null");
	dpu_check_and_return(!dfr_ctrl->dpu_comp, -1, err, "dpu_composer is null");
	dpu_check_and_return(effect_params->effect_num > EFFECT_PARAMS_MAX_NUM, -1, err, "effect num exceeds max num");
	if ((effect_params->delay != 0 || effect_params->effect_num == 0) && (g_debug_dpu_send_dcs_cmds == 0)) {
		dpu_pr_debug("Nothing to send !");
		return 0;
	}

	bl_ctrl = &dfr_ctrl->dpu_comp->bl_ctrl;

	/* debug update bl_level */
	if (g_debug_dpu_send_dcs_cmds)
		dpu_backlight_debug_level(effect_params);
	dpu_backlight_update_level(bl_ctrl, effect_params);

	return dfr_te_skip_send_dcs_cmds_by_async(dfr_ctrl->dpu_comp, effect_params);
}

int32_t dfr_te_skip_send_dcs_cmds_by_riscv(struct dpu_comp_dfr_ctrl *dfr_ctrl, uint32_t bl_level)
{
	struct dpu_composer *dpu_comp = NULL;
	struct composer_manager *comp_mgr = NULL;
	struct dfr_info *dinfo = NULL;
	struct dkmd_connector_info *pinfo;
	struct mipi_dsi_cmds_info dsi_cmds_info = { 0 };
	struct comp_online_present *present = NULL;

	dpu_check_and_return(!dfr_ctrl, -1, err, "dfr_ctrl is null");
	dinfo = dkmd_get_dfr_info(dfr_ctrl->dpu_comp->conn_info);
	dpu_check_and_return(!dinfo, -1, err, "dfr info is null");

	dpu_comp = dfr_ctrl->dpu_comp;
	dpu_check_and_return(!dpu_comp, -1, err, "dpu_comp is null");
	comp_mgr = dpu_comp->comp_mgr;
	pinfo = dpu_comp->conn_info;
	present = (struct comp_online_present *)dpu_comp->present_data;

	if (dinfo->oled_info.ltpo_info.dimming_enable)
		composer_mgr_enable_dimming(comp_mgr, dpu_comp->comp.index);

	dpu_pr_debug("update dcs cmds register is true");

	dsi_cmds_info.cmds_info.cmds_type = DKMD_CMDS_TYPE_BL;
	dsi_cmds_info.cmds_info.data_len = 2;
	dsi_cmds_info.cmds_info.data[0] = (int32_t)(present->dfr_ctrl.ppu_ctrl.panel_refresh_mode);
	dsi_cmds_info.cmds_info.data[1] = (int32_t)bl_level;
	dsi_cmds_info.follow_frame = false;
	if (dpu_comp->bl_ctrl.is_force_sync) {
		dpu_comp->bl_ctrl.is_force_sync = 0;
		dpu_pr_info("sync bl");
		dsi_cmds_info.is_force_sync = true;
	} else {
		dsi_cmds_info.is_force_sync = false;
	}
	dpu_pr_info("panel_refresh_mode = %d", dsi_cmds_info.cmds_info.data[0]);
	pipeline_next_ops_handle(pinfo->conn_device, pinfo, SEND_CMDS, &dsi_cmds_info);
	pipeline_next_ops_handle(pinfo->conn_device, pinfo, TRACE_SCREEN_BL, &bl_level);

	return 0;
}

int32_t dfr_te_skip_set_safe_frm_rate(struct dpu_comp_dfr_ctrl *dfr_ctrl, uint32_t safe_frm_rate)
{
	struct sfr_info sfr_info = {0};

	dpu_check_and_return(!dfr_ctrl, -1, err, "dfr_ctrl is null");
	dpu_check_and_return(!dfr_ctrl->dpu_comp, -1, err, "dpu_comp is null");
	dpu_check_and_return(!dfr_ctrl->dpu_comp->conn_info, -1, err, "conn_info is null");

	sfr_info.info_type = INFO_TYPE_SFR_SEND;
	sfr_info.full_mode_sfr = safe_frm_rate & 0x0000FFFF;
	sfr_info.ppu_mode_sfr = (safe_frm_rate >> 16) & 0x0000FFFF;

	dpu_pr_info("full_mode_sfr = %u, ppu_mode_sfr = %u", sfr_info.full_mode_sfr, sfr_info.ppu_mode_sfr);

	pipeline_next_ops_handle(dfr_ctrl->dpu_comp->conn_info->conn_device,
		dfr_ctrl->dpu_comp->conn_info, NOTIFY_SFR_INFO, &sfr_info);

	return 0;
}

int32_t dfr_te_skip_send_safe_frm_rate(struct dpu_comp_dfr_ctrl *dfr_ctrl, uint32_t safe_frm_rate)
{
	struct dfr_info *dinfo = NULL;
	struct dkmd_connector_info *pinfo = NULL;
	struct comp_online_present *present = NULL;
	struct mipi_dsi_cmds_info dsi_cmds_info = { 0 };

	dpu_check_and_return(!dfr_ctrl, -1, err, "dfr_ctrl is null");
	dpu_check_and_return(!dfr_ctrl->dpu_comp, -1, err, "dpu_comp is null");
	dpu_check_and_return(!dfr_ctrl->dpu_comp->comp_mgr, -1, err, "comp_mgr is null");
	dpu_check_and_return(!dfr_ctrl->dpu_comp->conn_info, -1, err, "conn_info is null");
	dpu_check_and_return(!dfr_ctrl->dpu_comp->present_data, -1, err, "present_data is null");

	pinfo = dfr_ctrl->dpu_comp->conn_info;
	present = (struct comp_online_present *)dfr_ctrl->dpu_comp->present_data;
	if (dfr_te_skip_check_safe_frm_rate(dfr_ctrl, safe_frm_rate) == -1) {
		dpu_pr_info("no need to set safe frm rate");
		return 0;
	}

	dinfo = dkmd_get_dfr_info(dfr_ctrl->dpu_comp->conn_info);
	dpu_check_and_return(dinfo == NULL, -1, err, "dinfo is null");

	dpu_pr_info("safe frm rate update, frm rate is %u", dfr_ctrl->cur_safe_frm_rate);

	/* The function dfr_te_skip_set_safe_frm_rate calls the function dfr_te_skip_send_safe_frm_rate. 
	   Pay attention to the use of locks to prevent deadlocks. */
	down(&dfr_ctrl->dpu_comp->comp_mgr->power_sem);
	if (!composer_check_power_status(dfr_ctrl->dpu_comp)) {
		dpu_pr_warn("composer %u, panel power off!", dfr_ctrl->dpu_comp->comp.index);
		up(&dfr_ctrl->dpu_comp->comp_mgr->power_sem);
		return 0;
	}

	dpu_comp_active_vsync(dfr_ctrl->dpu_comp);
	outp32(DPU_DFR_MIN_FRM_RATE_ADDR(dfr_ctrl->dpu_comp->comp_mgr->dpu_base), dfr_ctrl->cur_safe_frm_rate);
	dpu_comp_deactive_vsync(dfr_ctrl->dpu_comp);
	up(&dfr_ctrl->dpu_comp->comp_mgr->power_sem);

	if (dinfo->oled_info.ltpo_info.dimming_enable && !dfr_ctrl->follow_frame)
		composer_mgr_enable_dimming(dfr_ctrl->dpu_comp->comp_mgr, dfr_ctrl->dpu_comp->comp.index);

	dsi_cmds_info.cmds_info.cmds_type = DKMD_CMDS_TYPE_SFR;
	dsi_cmds_info.cmds_info.data_len = 2;
	dsi_cmds_info.cmds_info.data[0] = present->dfr_ctrl.ppu_ctrl.panel_refresh_mode;
	dsi_cmds_info.cmds_info.data[1] = dfr_ctrl->cur_safe_frm_rate;
	dsi_cmds_info.follow_frame = dfr_ctrl->follow_frame;
	dsi_cmds_info.is_force_sync = false;

	pipeline_next_ops_handle(pinfo->conn_device, pinfo, SEND_CMDS, &dsi_cmds_info);

	/* for the frame rate statistics in maintain module */
	if (dfr_ctrl->cur_safe_frm_rate > 0) {
		present->comp_maintain.self_refresh_period_us = PERIOD_US_1HZ / dfr_ctrl->cur_safe_frm_rate;
	} else {
		dpu_pr_warn("meet invalide safe frame rate(%u) when set it to maintain!", dfr_ctrl->cur_safe_frm_rate);
	}

	return 0;
}

void dfr_te_skip_self_refresh(struct dpu_comp_dfr_ctrl *dfr_ctrl)
{
	uint32_t current_config = 0;
	if (dfr_ctrl && dfr_ctrl->dpu_comp && dfr_ctrl->dpu_comp->comp_mgr &&
		dfr_ctrl->dpu_comp->comp_mgr->dpu_base) {
		char __iomem *dpu_base = dfr_ctrl->dpu_comp->comp_mgr->dpu_base;
		current_config = inp32(DPU_DFR_INIT_CONFIG_ADDR(dpu_base));
		/* bit10 1 self refresh */
		current_config |= BIT(10);
		dpu_pr_info("dfr_te_skip_self_refresh current_config = %u", current_config);
		outp32(DPU_DFR_INIT_CONFIG_ADDR(dpu_base), current_config);
	}
}

void dfr_te_skip_enable_dimming(struct dpu_comp_dfr_ctrl *dfr_ctrl)
{
	if (dfr_ctrl && dfr_ctrl->dpu_comp && dfr_ctrl->dpu_comp->comp_mgr &&
		dfr_ctrl->dpu_comp->comp_mgr->dpu_base) {
		char __iomem *dpu_base = dfr_ctrl->dpu_comp->comp_mgr->dpu_base;
		outp32(DPU_DFR_ENABLE_FRM_DIMMING_ADDR(dpu_base), 1);
	}
}

void dfr_te_skip_disable_dimming(struct dpu_comp_dfr_ctrl *dfr_ctrl)
{
	if (dfr_ctrl && dfr_ctrl->dpu_comp && dfr_ctrl->dpu_comp->comp_mgr &&
		dfr_ctrl->dpu_comp->comp_mgr->dpu_base) {
		char __iomem *dpu_base = dfr_ctrl->dpu_comp->comp_mgr->dpu_base;
		outp32(DPU_DFR_ENABLE_FRM_DIMMING_ADDR(dpu_base), 0);
	}
}

void dfr_te_skip_enable_safe_frm_rate(struct dpu_comp_dfr_ctrl *dfr_ctrl)
{
	void_unused(dfr_ctrl);
}

void dfr_te_skip_disable_safe_frm_rate(struct dpu_comp_dfr_ctrl *dfr_ctrl, int32_t disable_type)
{
	void_unused(dfr_ctrl);
	void_unused(disable_type);
}

static int32_t dfr_te_skip_update_frm_rate_isr_handler(struct dpu_comp_dfr_ctrl *dfr_ctrl)
{
	struct dpu_connector *connector = NULL;
	struct dkmd_connector_info *pinfo = NULL;
	struct dfr_te_skip_ctrl *priv = NULL;
	struct dfr_info *dinfo = NULL;
 
	dpu_check_and_return(!dfr_ctrl, -1, err, "dfr ctrl is null\n");
	pinfo = dfr_ctrl->dpu_comp->conn_info;
	dpu_check_and_return(!pinfo, -1, err, "pinfo is null\n");
	priv = (struct dfr_te_skip_ctrl *)dfr_ctrl->priv_data;
	dpu_check_and_return(!priv, -1, err, "priv is null\n");
	dinfo = dkmd_get_dfr_info(dfr_ctrl->dpu_comp->conn_info);
	dpu_check_and_return(!dinfo, -1, err, "dfr info is null");
 
	if (!priv->is_porch_switch && dinfo->dfr_mode == DFR_MODE_LONGH_TE_SKIP_BY_MCU)
		return 0;
	dpu_pr_info("switch mipi rate to %d\n", dfr_ctrl->cur_frm_rate);
 
	connector = get_primary_connector(pinfo);
	mipi_dsi_dfr_update(connector, (int32_t)dfr_ctrl->cur_frm_rate, DFR_MODE_LONG_H);
 
	priv->is_porch_switch = false;
	priv->porch_switch_delay_cnt = 0;
 
	return 0;
}

void dfr_te_skip_enter_idle_event(struct dpu_vsync *vsync_ctrl)
{
#ifndef CONFIG_DKMD_DPU_OHOS
	void_unused(vsync_ctrl);
	return;
#else
	struct comp_online_present *present = NULL;
	char *envp[2];
	char state_buf[UEVENT_BUF_LEN];
	struct composer *comp = NULL;
	uint64_t value;
	int ret;

	if (unlikely(vsync_ctrl == NULL)) {
		dpu_pr_err("vsync_ctrl is null");
		return;
	}

	present = (struct comp_online_present *)vsync_ctrl->dpu_comp->present_data;
	if (unlikely(present == NULL)) {
		dpu_pr_err("present is null");
		return;
	}

	if (present->dfr_ctrl.mode != DFR_MODE_TE_SKIP_BY_MCU &&
		present->dfr_ctrl.mode != DFR_MODE_LONGH_TE_SKIP_BY_MCU)
		return;
 
	if(vsync_ctrl->report_idle_event_flag == 0)
		return;
 
	vsync_ctrl->report_idle_event_flag = 0;
	value = (uint64_t)ktime_to_ns(vsync_ctrl->timestamp);
	ret = snprintf_s(state_buf, sizeof(state_buf), sizeof(state_buf) - 1, "IDLE_EVENT=%llu\n", value);
	if (ret < 0) {
		dpu_pr_err("format string failed, truncation occurs");
		return;
	}
 
	envp[0] = state_buf;
	envp[1] = NULL;
	comp = &vsync_ctrl->dpu_comp->comp;
 
	if (!comp) {
		dpu_pr_err("comp is NULL Pointer\n");
		return;
	}
 
	kobject_uevent_env(&(comp->base.peri_device->dev.kobj), KOBJ_CHANGE, envp);
	dpu_pr_info("enter dpu idle, vsync timestamp=%llu!", value);
#endif
}

static struct dfr_ctrl_ops g_dfr_ctrl_ops = {
	.setup_data = dfr_te_skip_setup_priv_data,
	.release_data = dfr_te_skip_release_priv_data,
	.switch_frm_rate = dfr_te_skip_switch_frm_rate,
	.commit = dfr_te_skip_commit,
	.send_dcs_cmds_with_frm = dfr_te_skip_send_dcs_cmds,
	.send_dcs_cmds_with_refresh = dfr_te_skip_send_dcs_cmds_by_riscv,
	.set_safe_frm_rate = dfr_te_skip_set_safe_frm_rate,
	.send_refresh = dfr_te_skip_self_refresh,
	/* decision module(safe frame rate) -> dpu(ap) -> dacc */
	.send_safe_frm_rate = dfr_te_skip_send_safe_frm_rate,
	.enable_dimming = dfr_te_skip_enable_dimming,
	.disable_dimming = dfr_te_skip_disable_dimming,
	.enter_idle_event = dfr_te_skip_enter_idle_event,
	.power_off_sub = dfr_te_skip_power_off_sub,
};

void dfr_te_skip_register_ops(struct dpu_comp_dfr_ctrl *dfr_ctrl) {
	dfr_ctrl->ops = &g_dfr_ctrl_ops;
}

void dfr_longh_te_skip_register_ops(struct dpu_comp_dfr_ctrl *dfr_ctrl) {
	g_dfr_ctrl_ops.update_frm_rate_isr_handler = dfr_te_skip_update_frm_rate_isr_handler;
	dfr_ctrl->ops = &g_dfr_ctrl_ops;
}