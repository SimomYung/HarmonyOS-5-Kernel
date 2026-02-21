/*
 * lcd_kit_panel_deep_calibration.c
 *
 * display engine deep calibration in lcd
 *
 * Copyright (c) 2024-2024 Huawei Technologies Co., Ltd.
 *
 */
 
#include "lcd_kit_panel_deep_calibration.h"
 
int display_engine_set_param_panel_deep_calibration(int panel_id, bool panel_deep_calibration_enable)
{
	if (!common_info->c_calib.support) {
		LCD_KIT_INFO("color calib not support\n");
		return LCD_KIT_OK;
	}
	LCD_KIT_INFO("DISPLAY_ENGINE_DEEP_CALIBRATION %d +\n", panel_deep_calibration_enable);
	if (panel_deep_calibration_enable) {
		common_ops->set_ddic_cmd(panel_id, LCD_KIT_DDIC_CMD_TYPE_C_CALIB , LCD_KIT_DDIC_CMD_OP_CLOSE);
		LCD_KIT_INFO("Ap demura:ON, DDIC demura: OFF!\n");
	} else {
		common_ops->set_ddic_cmd(panel_id, LCD_KIT_DDIC_CMD_TYPE_C_CALIB , LCD_KIT_DDIC_CMD_OP_OPEN);
		LCD_KIT_INFO("Ap demura:OFF, DDIC demura: ON!\n");
	}
	LCD_KIT_INFO("DISPLAY_ENGINE_DEEP_CALIBRATION %d -\n", panel_deep_calibration_enable);
	return LCD_KIT_OK;
}