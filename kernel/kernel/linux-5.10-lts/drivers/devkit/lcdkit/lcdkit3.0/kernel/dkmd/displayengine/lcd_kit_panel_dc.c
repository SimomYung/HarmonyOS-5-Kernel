/*
 * lcd_kit_panel_dc.c
 *
 * display engine dkmd DC function in lcd
 *
 * Copyright (c) 2024-2024 Huawei Technologies Co., Ltd.
 *
 */

#include "lcd_kit_panel_dc.h"

#include "lcd_kit_adapt.h"
#include "lcd_kit_parse.h"
#include "lcd_kit_utils.h"
#include "lcd_kit_disp.h"
#include "lcd_kit_common.h"

void lcd_kit_parse_panel_dc(int panel_id, struct device_node *np)
{
	char buf[MAX_STR_LEN] = {0};
	char state_buf[MAX_STR_LEN] = {0};
	int i;
	int ret = LCD_KIT_OK;

	/* panel dc code */
	LCD_KIT_INFO("DISPLAY_ENGINE_PNALE_DC parse panel xml data");
	if (!common_info) {
		LCD_KIT_INFO("DISPLAY_ENGINE_PNALE_DC common_info == NULL");
		return ;
	}
	lcd_kit_parse_u32(np, "lcd-kit,panel-svm-support",
		&common_info->panel_svm.support, 0);
	if (!common_info->panel_svm.support)
		return;
	for (i = 0; i < SOFTWARE_ID_CMDS_ADD_NUM_MAX; i++) {
		ret = snprintf_s(buf, MAX_STR_LEN, MAX_STR_LEN - 1, "lcd-kit,panel-svm-12-pulse-cmds%d", i);
		if (ret < 0) {
			LCD_KIT_ERR("snprintf_s cmd buf fail\n");
			return;
		}
		ret = snprintf_s(state_buf, MAX_STR_LEN, MAX_STR_LEN - 1, "lcd-kit,panel-svm-12-pulse-cmds%d-state", i);
		if (ret < 0) {
			LCD_KIT_ERR("snprintf_s cmd state buf fail\n");
			return;
		}
		lcd_kit_parse_dcs_cmds(np, buf, state_buf, &common_info->panel_svm.ota_12pulse_cmds[i]);
	}
	for (i = 0; i < SOFTWARE_ID_CMDS_ADD_NUM_MAX; i++) {
		ret = snprintf_s(buf, MAX_STR_LEN, MAX_STR_LEN - 1, "lcd-kit,panel-svm-3-pulse-cmds%d", i);
		if (ret < 0) {
			LCD_KIT_ERR("snprintf_s cmd buf fail\n");
			return;
		}
		ret = snprintf_s(state_buf, MAX_STR_LEN, MAX_STR_LEN - 1, "lcd-kit,panel-svm-3-pulse-cmds%d-state", i);
		if (ret < 0) {
			LCD_KIT_ERR("snprintf_s cmd state buf fail\n");
			return;
		}
		lcd_kit_parse_dcs_cmds(np, buf, state_buf, &common_info->panel_svm.ota_3pulse_cmds[i]);
	}
}

int display_engine_set_param_panel_dc(int panel_id, bool panel_dc_enable)
{
	int ret = LCD_KIT_OK;
	struct lcd_kit_adapt_ops *adapt_ops = NULL;
	int cmds_add_num = common_info->panel_software_id.number;

	adapt_ops = lcd_kit_get_adapt_ops();
	if (!adapt_ops || !adapt_ops->mipi_tx) {
		LCD_KIT_ERR("mipi tx not support!\n");
		return LCD_KIT_FAIL;
	}
	if (!common_info->panel_svm.support) {
		LCD_KIT_INFO("ap svm ctrl not support\n");
		return LCD_KIT_FAIL;
	}

	if (!lcd_kit_get_panel_on_state(panel_id)) {
		LCD_KIT_ERR("panel is power off\n");
		return LCD_KIT_FAIL;
	}

	if (panel_dc_enable) {
		common_info->panel_svm.is_svm_12pulse = true;
		if (common_info->panel_svm.ota_12pulse_cmds[cmds_add_num].cmds)
			ret = adapt_ops->mipi_tx(panel_id, NULL, &common_info->panel_svm.ota_12pulse_cmds[cmds_add_num]);
		if (ret != LCD_KIT_OK)
			LCD_KIT_ERR("mipi_tx failed! ret = %d\n", ret);
	} else {
		common_info->panel_svm.is_svm_12pulse = false;
		if (common_info->panel_svm.ota_3pulse_cmds[cmds_add_num].cmds)
			ret = adapt_ops->mipi_tx(panel_id, NULL, &common_info->panel_svm.ota_3pulse_cmds[cmds_add_num]);
		if (ret != LCD_KIT_OK)
			LCD_KIT_ERR("mipi_tx failed! ret = %d\n", ret);
	}
	LCD_KIT_INFO("DISPLAY_ENGINE_PNALE_DC %d for panel %d, software id support is %d, number is %d -\n",
		panel_dc_enable, panel_id, common_info->panel_software_id.support, cmds_add_num);
	return ret;
}
