// SPDX-License-Identifier: GPL-2.0
/*
 * lcd_kit_hybrid_aod.c
 *
 * source file for hybrid aod control
 *
 * Copyright (c) 2024-2024 Huawei Technologies Co., Ltd.
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
#include "lcd_kit_hybrid_aod.h"

#include "huawei_ts_kit_hybrid_core.h"
#include "lcd_kit_hybrid_swctrl.h"
#include "lcd_kit_adapt.h"
#include "lcd_kit_hybrid_core.h"

static int g_alpm_status;

static int alpm_on_light(struct display_hybrid_ctrl *display_ctrl,
			struct lcd_kit_dsi_panel_cmds *cmds)
{
	int ret;
	int panel_id = lcd_kit_get_active_panel_id();

	if (!cmds)
		return -EINVAL;

	ret = lcd_kit_cmds_tx(panel_id, NULL, cmds);
	if (ret)
		LCD_KIT_WARNING("mipi tx failed!\n");

	return ret;
}

int alpm_on_high_light(struct display_hybrid_ctrl *display_ctrl)
{
	if (!display_ctrl)
		return -EINVAL;
	return alpm_on_light(display_ctrl, &display_ctrl->hybrid_info.alpm_on_hl_cmds);
}

int alpm_on_low_light(struct display_hybrid_ctrl *display_ctrl)
{
	if (!display_ctrl)
		return -EINVAL;
	return alpm_on_light(display_ctrl, &display_ctrl->hybrid_info.alpm_on_ll_cmds);
}

int alpm_on_middle_light(struct display_hybrid_ctrl *display_ctrl)
{
	if (!display_ctrl)
		return -EINVAL;
	return alpm_on_light(display_ctrl, &display_ctrl->hybrid_info.alpm_on_ml_cmds);
}

int alpm_on_hbm_light(struct display_hybrid_ctrl *display_ctrl)
{
	if (!display_ctrl)
		return -EINVAL;
	return alpm_on_light(display_ctrl, &display_ctrl->hybrid_info.alpm_on_hbml_cmds);
}

int alpm_on_no_light(struct display_hybrid_ctrl *display_ctrl)
{
	if (!display_ctrl)
		return -EINVAL;
	return alpm_on_light(display_ctrl, &display_ctrl->hybrid_info.alpm_on_nl_cmds);
}

int alpm_send_frame(struct display_hybrid_ctrl *display_ctrl)
{
	if (!display_ctrl)
		return -EINVAL;

	hybrid_dss_on(display_ctrl, COMPOSER_ON_MODE_DOZE);
	return hybrid_te_on(display_ctrl);
}

int alpm_stop_send_frame(struct display_hybrid_ctrl *display_ctrl)
{
	if (!display_ctrl)
		return -EINVAL;

	hybrid_dss_off(display_ctrl);
	return 0;
}

int alpm_exit(struct display_hybrid_ctrl *display_ctrl)
{
	int ret = 0;
	u8 lcd_status = 0;
	int panel_id = lcd_kit_get_active_panel_id();
	struct lcd_kit_common_ops *comm_ops = get_hybrid_common_ops();

	LCD_KIT_INFO("get in\n");

	if (!display_ctrl)
		return -EINVAL;

	send_hybrid_ts_cmd(TS_KIT_HYBRID_RESUME);
	lcd_kit_delay(1, LCD_KIT_WAIT_MS, false);
	ret = lcd_kit_cmds_tx(panel_id, NULL, &display_ctrl->hybrid_info.alpm_exit_cmds);
	if (ret != LCD_KIT_OK) {
		LCD_KIT_INFO("Fail to exit AOD\n");
		return ret;
	}

	ret = hybrid_get_lcd_status(display_ctrl, &lcd_status);
	if (ret != LCD_KIT_OK)
		LCD_KIT_INFO("Fail to read lcd status");

	if (lcd_status != STATUS_NORMAL) {
		if (!comm_ops || !comm_ops->panel_on_lp) {
			LCD_KIT_INFO("common ops is null");
			return -EINVAL;
		}
		ret = comm_ops->panel_on_lp(panel_id, NULL);
		if (ret) {
			LCD_KIT_INFO("Fail to panel on when exit AOD");
			return ret;
		}
	}

	ret = lcd_kit_cmds_tx(panel_id, NULL, &display_ctrl->hybrid_info.te_on_cmds);
	if (ret != LCD_KIT_OK) {
		LCD_KIT_ERR("send te off cmds error\n");
		return ret;
	}

	alpm_set_state_flag(ALPM_STATE_OUT);
	lcd_kit_hybrid_esd_on();
	if (display_ctrl->display_sw_ops && display_ctrl->display_sw_ops->send_hybrid_state)
		display_ctrl->display_sw_ops->send_hybrid_state(display_ctrl->display_sw_ops,
			AP_DISPLAY_ON);

	hybrid_lcd_status_check(display_ctrl);
	return ret;
}

int alpm_doze_suspend(struct display_hybrid_ctrl *display_ctrl)
{
	int ret;
	int panel_id = lcd_kit_get_active_panel_id();

	if (!display_ctrl)
		return -EINVAL;

	if (!alpm_get_state()) {
		LCD_KIT_ERR("Not in doze mode, go into doze first");
		return -EINVAL;
	}

	ret = lcd_kit_cmds_tx(panel_id, NULL, &display_ctrl->hybrid_info.te_off_cmds);
	if (ret != LCD_KIT_OK) {
		LCD_KIT_ERR("send te off cmds error\n");
		return ret;
	}

	if (common_info->dirty_region.support) {
		LCD_KIT_INFO("set full dirty region\n");
		return lcd_kit_cmds_tx(panel_id, NULL, &display_ctrl->hybrid_info.dirty_region_cmds);
	}
	return ret;
}

int alpm_doze(struct display_hybrid_ctrl *display_ctrl)
{
	int ret = 0;
	int panel_id = lcd_kit_get_active_panel_id();

	if (!display_ctrl)
		return -EINVAL;

	if (alpm_get_state()) {
		ret = lcd_kit_cmds_tx(panel_id, NULL, &display_ctrl->hybrid_info.te_on_cmds);
		if (ret != LCD_KIT_OK)
			LCD_KIT_ERR("send te off cmds error\n");
		return ret;
	}

	lcd_kit_hybrid_esd_off();
	ret = alpm_on_no_light(display_ctrl);
	if (ret) {
		lcd_kit_hybrid_esd_on();
		LCD_KIT_ERR("Fail to enable AOD\n");
		return ret;
	}
	send_hybrid_ts_cmd(TS_KIT_HYBRID_IDLE);
	alpm_set_state_flag(ALPM_STATE_IN);

	if (display_ctrl->display_sw_ops && display_ctrl->display_sw_ops->send_hybrid_state)
		display_ctrl->display_sw_ops->send_hybrid_state(display_ctrl->display_sw_ops,
			AP_DISPLAY_AOD);

	hybrid_lcd_status_check(display_ctrl);
	return ret;
}

int alpm_get_state(void)
{
	return g_alpm_status && hybrid_control_check();
}

void alpm_set_state(struct display_hybrid_ctrl *display_ctrl)
{
	int ret;
	u8 lcd_status = 0;

	if (!display_ctrl)
		return;

	if (!hybrid_control_check()) {
		g_alpm_status = ALPM_STATE_OUT;
		return;
	}

	ret = hybrid_get_lcd_status(display_ctrl, &lcd_status);
	if (ret != LCD_KIT_OK) {
		g_alpm_status = ALPM_STATE_OUT;
		return;
	}

	if (lcd_status == STATUS_IDLE)
		g_alpm_status = ALPM_STATE_IN;
	else
		g_alpm_status = ALPM_STATE_OUT;
}

void alpm_set_state_flag(int flag)
{
	LCD_KIT_INFO("set AOD status to %d", flag);
	g_alpm_status = flag;
}

int lcd_kit_enter_deep_sleep(struct display_hybrid_ctrl *display_ctrl)
{
	int ret;
	int panel_id = lcd_kit_get_active_panel_id();

	if (!display_ctrl)
		return -EINVAL;

	ret = lcd_kit_cmds_tx(panel_id, NULL, &display_ctrl->hybrid_info.ddic_deep_sleep_cmd);
	if (ret)
		LCD_KIT_WARNING("mipi rx failed!\n");
	return ret;
}
