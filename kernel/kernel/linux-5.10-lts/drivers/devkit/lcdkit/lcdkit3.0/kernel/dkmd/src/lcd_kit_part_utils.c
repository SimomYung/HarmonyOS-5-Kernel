/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 *
 * lcd_kit_part_utils.c
 *
 * lcdkit utils function for lcd driver
 *
 * Copyright (c) 2018-2022 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */

#include "lcd_kit_utils.h"
#include "lcd_kit_disp.h"
#include "lcd_kit_parse.h"
#include "lcd_kit_adapt.h"
#include "lcd_kit_local_hbm.h"
#include "lcd_kit_panel_pfm.h"
#include "display_engine_interface.h"
#include "lcd_kit_elvss_control.h"
#include "lcd_kit_partial_refresh.h"

static void lcd_kit_dump_cmds_part(struct mipi_dsi_tx_params *params)
{
	unsigned int i;
	unsigned int j;
	int n = (int)params->write_cmds.cmds_num;
	LCD_KIT_DEBUG("cmds_num:%d\n", params->write_cmds.cmds_num);

	for (i = 0; i < n; i++) {
		LCD_KIT_DEBUG("cmd[%d]:dtype:0x%x wait:0x%x waittype:0x%x dlen:0x%x",
			i, params->write_cmds.cmds[i].dtype, params->write_cmds.cmds[i].wait,
			params->write_cmds.cmds[i].waittype, params->write_cmds.cmds[i].dlen);
		LCD_KIT_DEBUG("cmd->payload:\n");
		for (j = 0; j < (unsigned int)params->write_cmds.cmds[i].dlen; j++)
			LCD_KIT_DEBUG("0x%x\n", params->write_cmds.cmds[i].payload[j]);
	}
}

static int lcd_kit_cmds_to_dsi_cmds_refresh(struct lcd_kit_dsi_cmd_desc *lcd_kit_cmds,
	struct dsi_cmd_desc *cmd)
{
	int ret = LCD_KIT_OK;

	if (!lcd_kit_cmds || !cmd) {
		LCD_KIT_ERR("cmds info is NULL\n");
		return LCD_KIT_FAIL;
	}

	lcd_kit_cmds_to_dsi_desc_cmds(lcd_kit_cmds, cmd, 1);
	ret = memcpy_s(cmd->payload, lcd_kit_cmds->dlen * sizeof(char), lcd_kit_cmds->payload,
	    lcd_kit_cmds->dlen * sizeof(char));
	if (ret != LCD_KIT_OK) {
		LCD_KIT_ERR("copy payload form fail\n");
		return LCD_KIT_FAIL;
	}

	return LCD_KIT_OK;
}

int lcd_kit_set_multi_region_info(int panel_id, uint32_t ys, uint32_t ye, bool mode)
{
	common_info->part_refresh.multi_region.ys = ys;
	common_info->part_refresh.multi_region.ye = ye;
	common_info->part_refresh.multi_region.is_in = mode;
	common_info->part_refresh.multi_region.cal_ip = true;
	LCD_KIT_DEBUG("get multi_region_info, region[%d %d], is_in[%d]", ys, ye, mode);
	return LCD_KIT_OK;
}

int lcd_kit_get_part_region_ip_val(int panel_id)
{
	unsigned int ddic_type;
	int ret = LCD_KIT_OK;
	char read_value[PART_REFRESH_REG_VALUE_NUM_MAX] = {0};
	struct dpu_panel_info *panel_info = lcd_kit_get_dpu_pinfo(panel_id);
	struct lcd_kit_adapt_ops *adapt_ops = lcd_kit_get_adapt_ops();
	if (!panel_info) {
		LCD_KIT_ERR("pinfo is null\n");
		return LCD_KIT_FAIL;
	}

	if (!adapt_ops || !adapt_ops->mipi_rx) {
		LCD_KIT_ERR("mipi_rx is NULL\n");
		return LCD_KIT_FAIL;
	}

	ddic_type = panel_info->dfr_info.ddic_type;
	if (ddic_type) {
		LCD_KIT_INFO("xhs not need\n");
		return LCD_KIT_FAIL;
	}

	LCD_KIT_DEBUG("begin read Part Refresh Reg\n");

	ret = adapt_ops->mipi_rx(panel_id, NULL, (uint8_t *)read_value,
		PART_REFRESH_REG_VALUE_NUM_MAX - 1, &common_info->part_refresh.part_refresh_reg_cmd);
	if (ret) {
		LCD_KIT_ERR("mipi read part refresh ip reg info fail\n");
		return LCD_KIT_FAIL;
	}

	common_info->part_refresh.ip1_offset_param =
		(read_value[IP1_F8] << EIGHT_BIT) + read_value[IP1_F9];
	common_info->part_refresh.ip2_offset_param =
		(read_value[IP2_F5] << EIGHT_BIT) + read_value[IP2_F6];
	if (!common_info->part_refresh.ip1_offset_param || !common_info->part_refresh.ip2_offset_param) {
		common_info->part_refresh.ip1_offset_param = PART_REFRESH_IP1_OFFSET_PARAM_DEFAULT;
		common_info->part_refresh.ip2_offset_param = PART_REFRESH_IP2_OFFSET_PARAM_DEFAULT;
	}

	LCD_KIT_DEBUG("read Part Refresh Reg:%x %x\n",
		common_info->part_refresh.ip1_offset_param, common_info->part_refresh.ip2_offset_param);
	return ret;
}

void lcd_kit_mipi_tx_params_convert(int panel_id, struct lcd_kit_dsi_panel_cmds *cmds,
	struct mipi_dsi_tx_params *dsi0_params, const struct dkmd_cmds_info *cmds_info)
{
	int i;

	if (!cmds) {
		LCD_KIT_ERR("cmds is null, or cmd_cnt <= 0!\n");
		return;
	}

	if (!dsi0_params) {
		LCD_KIT_ERR("dsi0_params is null, or cmds is null!\n");
		return;
	}

	dsi0_params->is_isr_safe = LOCK;
	dsi0_params->write_cmds.cmds_num = (uint32_t)cmds->cmd_cnt;
	dsi0_params->trans_mode = cmds->async_param.sync_mode;
	dsi0_params->power_mode = cmds->link_state;
	dsi0_params->cmds_type = cmds_info->cmds_type;
	dsi0_params->has_refresh = cmds->async_param.refresh_flag;

	// transfer lcd_kit cmd to dsi cmd format
	for (i = 0; i < cmds->cmd_cnt && i < ASYNC_CMDS_MAX; i++)
		lcd_kit_cmds_to_dsi_cmds_refresh(&cmds->cmds[i],
			&dsi0_params->write_cmds.cmds[i]);
}

int lcd_kit_get_bl_async_cmds(int panel_id,
	const struct dkmd_cmds_info *cmds_info, struct mipi_dsi_tx_params *params)
{
	int ret;
	unsigned int dsi0_index = 0;
	unsigned int connector_id;
	int bl_level;
	struct lcd_kit_dsi_panel_cmds *bl_panel_cmd = &common_info->backlight.bl_cmd;

	if (!cmds_info || !params) {
		LCD_KIT_ERR("not support or info is NULL\n");
		return LCD_KIT_FAIL;
	}
	/* data[0]: part/full state  data[1]: bl_level */
	if (cmds_info->data[0] && common_info->part_refresh.support)
		bl_panel_cmd = &common_info->backlight.bl_part_cmd;

	bl_level = cmds_info->data[1];
	lcd_kit_set_bl_cmds(panel_id, bl_level, bl_panel_cmd);
	connector_id = DPU_PINFO->connector_id;
	ret = lcd_kit_get_dsi_index(&dsi0_index, connector_id);
	if (ret) {
		LCD_KIT_ERR("get dsi0 index error\n");
		return LCD_KIT_FAIL;
	}

	lcd_kit_mipi_tx_params_convert(panel_id, bl_panel_cmd, params, cmds_info);
	return LCD_KIT_OK;
}

int lcd_kit_get_dual_bl_async_cmds(int panel_id,
	const struct dkmd_cmds_info *cmds_info, struct mipi_dsi_tx_params *params0,
	struct mipi_dsi_tx_params *params1)
{
	int ret;
	unsigned int dsi0_index = 0;
	unsigned int connector_id;
	int bl_level;
	struct lcd_kit_dsi_panel_cmds *bl_panel_cmd = &common_info->backlight.bl_cmd;

	if (!cmds_info || !params0 || !params1) {
		LCD_KIT_ERR("not support or info is NULL\n");
		return LCD_KIT_FAIL;
	}
	/* data[0]: part/full state  data[1]: bl_level */
	if (cmds_info->data[0] && common_info->part_refresh.support)
		bl_panel_cmd = &common_info->backlight.bl_part_cmd;

	bl_level = cmds_info->data[1];
	lcd_kit_set_bl_cmds(panel_id, bl_level, bl_panel_cmd);
	connector_id = DPU_PINFO->connector_id;
	ret = lcd_kit_get_dsi_index(&dsi0_index, connector_id);
	if (ret) {
		LCD_KIT_ERR("get dsi0 index error\n");
		return LCD_KIT_FAIL;
	}

	lcd_kit_mipi_tx_params_convert(panel_id, bl_panel_cmd, params0, cmds_info);
	lcd_kit_mipi_tx_params_convert(panel_id, bl_panel_cmd, params1, cmds_info);
	return LCD_KIT_OK;
}

void lcd_kit_set_xhs_cmds(int panel_id,
	const struct dkmd_cmds_info *cmds_info, struct mipi_dsi_tx_params *params)
{
	int i = CMDS_ONE;

	if (params->write_cmds.cmds_num < MIN_OF_XHS_CMDS) {
		LCD_KIT_ERR("cmds num is error\n");
		return;
	}

	params->write_cmds.cmds[i].payload[PLYLOAD_ONE] =
		common_info->part_refresh.ys & 0xff;
	params->write_cmds.cmds[i].payload[PLYLOAD_TWO] =
		(common_info->part_refresh.ys >> EIGHT_BIT) & 0xff;
	params->write_cmds.cmds[i].payload[PLYLOAD_THREE] =
		(common_info->part_refresh.ye - 1) & 0xff;
	params->write_cmds.cmds[i].payload[PLYLOAD_FOUR] =
		((common_info->part_refresh.ye - 1) >> EIGHT_BIT) & 0xff;
	LCD_KIT_DEBUG("y_start is %u, y_end is %u\n",
			common_info->part_refresh.ys, common_info->part_refresh.ye);
	LCD_KIT_DEBUG("[%d, %d, %d, %d]\n",
			params->write_cmds.cmds[i].payload[PLYLOAD_ONE],
			params->write_cmds.cmds[i].payload[PLYLOAD_TWO],
			params->write_cmds.cmds[i].payload[PLYLOAD_THREE],
			params->write_cmds.cmds[i].payload[PLYLOAD_FOUR]);
}

int lcd_kit_get_part_region_refresh_cmds(int panel_id,
	const struct dkmd_cmds_info *cmds_info, struct mipi_dsi_tx_params *params)
{
	unsigned int ddic_type;
	struct dpu_panel_info *panel_info = lcd_kit_get_dpu_pinfo(panel_id);
	if (!panel_info) {
		LCD_KIT_ERR("pinfo is null\n");
		return LCD_KIT_FAIL;
	}

	if (!common_info->dirty_region.support || !cmds_info || !params) {
		LCD_KIT_ERR("dirty_region not support or params error\n");
		return LCD_KIT_FAIL;
	}

	common_info->part_refresh.ys = cmds_info->data[LCD_Y_RES_INDEX];
	common_info->part_refresh.ye =
			common_info->part_refresh.ys + cmds_info->data[LCD_H_RES_INDEX];

	ddic_type = panel_info->dfr_info.ddic_type;
	LCD_KIT_DEBUG("the ddic type is %d\n", ddic_type);
	if (ddic_type) {
		lcd_kit_mipi_tx_params_convert(panel_id, &common_info->dirty_region.cmds, params, cmds_info);
		lcd_kit_set_xhs_cmds(panel_id, cmds_info, params);
	} else {
		lcd_kit_get_rd_region_refresh_info(cmds_info);
		lcd_kit_mipi_tx_params_convert(panel_id, &common_info->dirty_region.cmds, params, cmds_info);
	}
	lcd_kit_dump_cmds_part(params);
	return LCD_KIT_OK;
}

int lcd_kit_get_dual_part_region_refresh_cmds(int panel_id,
	const struct dkmd_cmds_info *cmds_info, struct mipi_dsi_tx_params *params0,
	struct mipi_dsi_tx_params *params1)
{
	unsigned int ddic_type;
	struct dpu_panel_info *panel_info = lcd_kit_get_dpu_pinfo(panel_id);
	if (!panel_info) {
 		LCD_KIT_ERR("pinfo is null\n");
		return LCD_KIT_FAIL;
	}

	if (!common_info->dirty_region.support || !cmds_info || !params0 || !params1) {
		LCD_KIT_ERR("dirty_region not support or params error\n");
		return LCD_KIT_FAIL;
	}

	common_info->part_refresh.ys = cmds_info->data[LCD_Y_RES_INDEX];
	common_info->part_refresh.ye =
			common_info->part_refresh.ys + cmds_info->data[LCD_H_RES_INDEX];

	ddic_type = panel_info->dfr_info.ddic_type;
	LCD_KIT_DEBUG("the ddic type is %d\n", ddic_type);
	if (ddic_type) {
		lcd_kit_mipi_tx_params_convert(panel_id, &common_info->dirty_region.cmds, params0, cmds_info);
		lcd_kit_set_xhs_cmds(panel_id, cmds_info, params0);
		lcd_kit_mipi_tx_params_convert(panel_id, &common_info->dirty_region.cmds, params1, cmds_info);
		lcd_kit_set_xhs_cmds(panel_id, cmds_info, params1);
	} else {
		lcd_kit_get_rd_region_refresh_info(cmds_info);
		lcd_kit_mipi_tx_params_convert(panel_id, &common_info->dirty_region.cmds, params0, cmds_info);
		lcd_kit_mipi_tx_params_convert(panel_id, &common_info->dirty_region.cmds, params1, cmds_info);
	}
	lcd_kit_dump_cmds_part(params0);
	lcd_kit_dump_cmds_part(params1);
	return LCD_KIT_OK;
}

int lcd_kit_calc_partial_rect(int panel_id,
	const struct dkmd_cmds_info *cmds_info, struct mipi_dsi_tx_params *params)
{
	int ddic_type;
	struct dpu_panel_info *panel_info = lcd_kit_get_dpu_pinfo(panel_id);
	if (!panel_info) {
		LCD_KIT_ERR("pinfo is null\n");
		return LCD_KIT_FAIL;
	}

	if (!common_info->part_refresh.support || !cmds_info || !params) {
		LCD_KIT_ERR("not support or info is NULL\n");
		return LCD_KIT_FAIL;
	}

	if (!params->write_cmds.cmds || !panel_info) {
		LCD_KIT_ERR("params or panel_info is null!\n");
		return LCD_KIT_FAIL;
	}

	common_info->part_refresh.ys = cmds_info->data[LCD_Y_RES_INDEX];
	common_info->part_refresh.ye =
		common_info->part_refresh.ys + cmds_info->data[LCD_H_RES_INDEX];
	LCD_KIT_DEBUG("calc_partial_rect y_start is %u, y_end is %u\n",
			common_info->part_refresh.ys, common_info->part_refresh.ye);

	ddic_type = panel_info->dfr_info.ddic_type;
	LCD_KIT_DEBUG("the ddic type is %d\n", ddic_type);
	if (ddic_type) {
		if (common_info->part_refresh.multi_region.is_in) {
			lcd_kit_mipi_tx_params_convert(panel_id,
				&common_info->part_refresh.bureau_brush_cmd, params, cmds_info);
		} else {
			lcd_kit_mipi_tx_params_convert(panel_id,
				&common_info->part_refresh.part_refresh_cmd, params, cmds_info);
		}
		lcd_kit_set_xhs_cmds(panel_id, cmds_info, params);
	} else {
		lcd_kit_rd_get_refresh_cmds(panel_id, cmds_info, params);
	}
	lcd_kit_dump_cmds_part(params);
	return LCD_KIT_OK;
}

int lcd_kit_dual_calc_partial_rect(int panel_id,
	const struct dkmd_cmds_info *cmds_info, struct mipi_dsi_tx_params *params0,
	struct mipi_dsi_tx_params *params1)
{
	int ddic_type;
	struct dpu_panel_info *panel_info = lcd_kit_get_dpu_pinfo(panel_id);
	if (!panel_info) {
		LCD_KIT_ERR("pinfo is null\n");
		return LCD_KIT_FAIL;
	}

	if (!common_info->part_refresh.support || !cmds_info || !params0 || !params1) {
		LCD_KIT_ERR("not support or info is NULL\n");
		return LCD_KIT_FAIL;
	}

	if (!params0->write_cmds.cmds || !params1->write_cmds.cmds) {
		LCD_KIT_ERR("params is null!\n");
		return LCD_KIT_FAIL;
	}

	common_info->part_refresh.ys = cmds_info->data[LCD_Y_RES_INDEX];
	common_info->part_refresh.ye =
		common_info->part_refresh.ys + cmds_info->data[LCD_H_RES_INDEX];
	LCD_KIT_DEBUG("calc_partial_rect y_start is %u, y_end is %u\n",
			common_info->part_refresh.ys, common_info->part_refresh.ye);

	ddic_type = panel_info->dfr_info.ddic_type;
	LCD_KIT_DEBUG("the ddic type is %d\n", ddic_type);
	if (ddic_type) {
		lcd_kit_mipi_tx_params_convert(panel_id,
			&common_info->part_refresh.part_refresh_cmd, params0, cmds_info);
		lcd_kit_mipi_tx_params_convert(panel_id,
			&common_info->part_refresh.part_refresh_cmd, params1, cmds_info);
		lcd_kit_set_xhs_cmds(panel_id, cmds_info, params0);
		lcd_kit_set_xhs_cmds(panel_id, cmds_info, params1);
	} else {
		lcd_kit_rd_get_refresh_cmds(panel_id, cmds_info, params0);
		lcd_kit_rd_get_refresh_cmds(panel_id, cmds_info, params1);
	}

	return LCD_KIT_OK;
}

int lcd_kit_get_part_region_control_cmds(int panel_id,
	const struct dkmd_cmds_info *cmds_info, struct mipi_dsi_tx_params *params)
{
	if (!common_info->part_refresh.support || !cmds_info || !params) {
		LCD_KIT_ERR("not support or info is NULL\n");
		return LCD_KIT_FAIL;
	}

	if (!params->write_cmds.cmds) {
		LCD_KIT_ERR("params is null!\n");
		return LCD_KIT_FAIL;
	}

	LCD_KIT_INFO("get_part_region_control %d\n", cmds_info->cmds_type);
	if (cmds_info->cmds_type == DKMD_CMDS_TYPE_PPU_ENTER) {
		lcd_kit_mipi_tx_params_convert(panel_id,
			&common_info->part_refresh.part_refresh_enter_cmd, params, cmds_info);
	} else if (cmds_info->cmds_type == DKMD_CMDS_TYPE_PPU_EXIT) {
		lcd_kit_mipi_tx_params_convert(panel_id,
			&common_info->part_refresh.part_refresh_exit_cmd, params, cmds_info);
			common_info->part_refresh.multi_region.is_in = false;
	}

	return LCD_KIT_OK;
}

int lcd_kit_get_dual_part_region_control_cmds(int panel_id,
	const struct dkmd_cmds_info *cmds_info, struct mipi_dsi_tx_params *params0,
	struct mipi_dsi_tx_params *params1)
{
	if (!common_info->part_refresh.support || !cmds_info || !params0 || !params1) {
		LCD_KIT_ERR("not support or info is NULL\n");
		return LCD_KIT_FAIL;
	}

	if (!params0->write_cmds.cmds || !params1->write_cmds.cmds) {
		LCD_KIT_ERR("params is null!\n");
		return LCD_KIT_FAIL;
	}

	LCD_KIT_INFO("get_part_region_control %d\n", cmds_info->cmds_type);
	if (cmds_info->cmds_type == DKMD_CMDS_TYPE_PPU_ENTER) {
		lcd_kit_mipi_tx_params_convert(panel_id,
			&common_info->part_refresh.part_refresh_enter_cmd, params0, cmds_info);
		lcd_kit_mipi_tx_params_convert(panel_id,
			&common_info->part_refresh.part_refresh_enter_cmd, params1, cmds_info);
	} else if (cmds_info->cmds_type == DKMD_CMDS_TYPE_PPU_EXIT) {
		lcd_kit_mipi_tx_params_convert(panel_id,
			&common_info->part_refresh.part_refresh_exit_cmd, params0, cmds_info);
		lcd_kit_mipi_tx_params_convert(panel_id,
			&common_info->part_refresh.part_refresh_exit_cmd, params1, cmds_info);
			common_info->part_refresh.multi_region.is_in = false;
	}

	return LCD_KIT_OK;
}

static struct lcd_kit_dsi_panel_cmds* lcd_kit_get_fps_safe_cmd(struct fps_dual_cmd_table *fps_safe_cmd,
	int cmd_num, const struct dkmd_cmds_info *cmds_info)
{
	int i;
	int32_t frame_rate;
	int32_t refresh_type;
	struct lcd_kit_dsi_panel_cmds *sfr_cmd = NULL;

	if (!cmds_info || sizeof(cmds_info->data) < MIN_OF_CMDS_INFO_DATA) {
		LCD_KIT_ERR("cmds_info is null!\n");
		return NULL;
	}

	/* data[0]: part/full state data[1]: frame rate num */
	frame_rate = cmds_info->data[1];
	refresh_type = cmds_info->data[0];

	LCD_KIT_INFO("frame_rate:%d, refresh_type:%d\n", frame_rate, refresh_type);
	for (i = 0; i < cmd_num; i++) {
		if (frame_rate == fps_safe_cmd[i].val && refresh_type) {
			sfr_cmd = fps_safe_cmd[i].part_cmds;
			break;
		} else if (frame_rate == fps_safe_cmd[i].val && !refresh_type) {
			sfr_cmd = fps_safe_cmd[i].cmds;
			break;
		}
	}
	return sfr_cmd;
}

int lcd_kit_get_safe_frame_part_cmds(int panel_id,
	const struct dkmd_cmds_info *cmds_info, struct mipi_dsi_tx_params *params)
{
	int cmd_num;
	struct lcd_kit_dsi_panel_cmds *sfr_cmd;

	struct fps_dual_cmd_table fps_safe_cmd[] = {
	{ FPS_1, &disp_info->fps.fps_to_1_cmds, &disp_info->fps.fps_to_1_part_cmds },
	{ FPS_2p5, &disp_info->fps.fps_to_2p5_cmds, &disp_info->fps.fps_to_2p5_part_cmds },
	{ FPS_10, &disp_info->fps.fps_to_10_cmds, &disp_info->fps.fps_to_10_part_cmds },
	{ FPS_24, &disp_info->fps.fps_to_24_cmds, &disp_info->fps.fps_to_24_part_cmds },
	{ FPS_30, &disp_info->fps.fps_to_30_cmds, &disp_info->fps.fps_to_30_part_cmds },
	{ FPS_40, &disp_info->fps.fps_to_40_cmds, &disp_info->fps.fps_to_40_part_cmds },
	{ FPS_45, &disp_info->fps.fps_to_45_cmds, &disp_info->fps.fps_to_45_part_cmds },
	{ FPS_60, &disp_info->fps.fps_to_60_cmds, &disp_info->fps.fps_to_60_part_cmds },
	{ FPS_72, &disp_info->fps.fps_to_72_cmds, &disp_info->fps.fps_to_72_part_cmds },
	{ FPS_90, &disp_info->fps.fps_to_90_cmds, &disp_info->fps.fps_to_90_part_cmds },
	{ FPS_120, &disp_info->fps.fps_to_120_cmds, &disp_info->fps.fps_to_24_part_cmds },
	};

	if (!cmds_info || !params) {
		LCD_KIT_ERR("params or info is NULL\n");
		return LCD_KIT_FAIL;
	}

	cmd_num = sizeof(fps_safe_cmd) / sizeof(fps_safe_cmd[0]);
	sfr_cmd = lcd_kit_get_fps_safe_cmd(fps_safe_cmd, cmd_num, cmds_info);
	if (!sfr_cmd) {
		LCD_KIT_ERR("safe_cmd is NULL\n");
		return LCD_KIT_FAIL;
	}

	lcd_kit_mipi_tx_params_convert(panel_id, sfr_cmd, params, cmds_info);
	lcd_kit_dump_cmds_part(params);
	return LCD_KIT_OK;
}

int lcd_kit_get_dual_safe_frame_part_cmds(int panel_id,
	const struct dkmd_cmds_info *cmds_info, struct mipi_dsi_tx_params *params0,
	struct mipi_dsi_tx_params *params1)
{
	int cmd_num;
	struct lcd_kit_dsi_panel_cmds *sfr_cmd;

	struct fps_dual_cmd_table fps_safe_cmd[] = {
	{ FPS_1, &disp_info->fps.fps_to_1_cmds, &disp_info->fps.fps_to_1_part_cmds },
	{ FPS_2p5, &disp_info->fps.fps_to_2p5_cmds, &disp_info->fps.fps_to_2p5_part_cmds },
	{ FPS_10, &disp_info->fps.fps_to_10_cmds, &disp_info->fps.fps_to_10_part_cmds },
	{ FPS_24, &disp_info->fps.fps_to_24_cmds, &disp_info->fps.fps_to_24_part_cmds },
	{ FPS_30, &disp_info->fps.fps_to_30_cmds, &disp_info->fps.fps_to_30_part_cmds },
	{ FPS_40, &disp_info->fps.fps_to_40_cmds, &disp_info->fps.fps_to_40_part_cmds },
	{ FPS_45, &disp_info->fps.fps_to_45_cmds, &disp_info->fps.fps_to_45_part_cmds },
	{ FPS_60, &disp_info->fps.fps_to_60_cmds, &disp_info->fps.fps_to_60_part_cmds },
	{ FPS_72, &disp_info->fps.fps_to_72_cmds, &disp_info->fps.fps_to_72_part_cmds },
	{ FPS_90, &disp_info->fps.fps_to_90_cmds, &disp_info->fps.fps_to_90_part_cmds },
	{ FPS_120, &disp_info->fps.fps_to_120_cmds, &disp_info->fps.fps_to_24_part_cmds },
	};

	if (!cmds_info || !params0 || !params1) {
		LCD_KIT_ERR("params or info is NULL\n");
		return LCD_KIT_FAIL;
	}
	
	cmd_num = sizeof(fps_safe_cmd) / sizeof(fps_safe_cmd[0]);
	sfr_cmd = lcd_kit_get_fps_safe_cmd(fps_safe_cmd, cmd_num, cmds_info);
	if (!sfr_cmd) {
		LCD_KIT_ERR("safe_cmd is NULL\n");
		return LCD_KIT_FAIL;
	}

	lcd_kit_mipi_tx_params_convert(panel_id, sfr_cmd, params0, cmds_info);
	lcd_kit_mipi_tx_params_convert(panel_id, sfr_cmd, params1, cmds_info);
	return LCD_KIT_OK;
}
