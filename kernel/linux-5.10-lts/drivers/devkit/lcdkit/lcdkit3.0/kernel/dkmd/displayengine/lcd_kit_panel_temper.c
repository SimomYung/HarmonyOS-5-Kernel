/*
 * lcd_kit_panel_temp_compensation.c
 *
 * display engine dkmd IRC function in lcd
 *
 * Copyright (c) 2023-2024 Huawei Technologies Co., Ltd.
 *
 */

#include "lcd_kit_panel_temper.h"
 
#include "display_engine_kernel.h"
#include "dkmd_lcd_interface.h"
#include "lcd_kit_adapt.h"
#include "lcd_kit_parse.h"
#include "lcd_kit_utils.h"
#include "lcd_kit_disp.h"
 
#define TEMPER_BRIFHTNESS_DISABLE_PANEL_VERSION_NUM_MAX 4
 
static char temper_compensation_disable_version[VERSION_NUM_MAX][LCD_PANEL_VERSION_SIZE] = {
	" VER:V4", " VER:VN1", " VER:63A", " VER:63B"
};

enum PpuTempCompensationStatus {
    PPU_TEMP_COMPENSATION_STATUS_OFF,
    PPU_TEMP_COMPENSATION_STATUS_ON,
    PPU_TEMP_COMPENSATION_STATUS_INVALID
};
 
struct de_context_temper {
	bool temper_compensation_enable;
	bool temper_compensation_brightness_disable_version_func_init;
	bool temper_compensation_brightness_is_disable_panel_version;
	bool temper_compensation_brightness_disable_ppu;
};
 
static struct de_context_temper g_de_context_temper = {
	.temper_compensation_enable = false,
	.temper_compensation_brightness_disable_version_func_init = false,
	.temper_compensation_brightness_is_disable_panel_version = false,
	.temper_compensation_brightness_disable_ppu = false,
};
 

static int lcd_kit_get_init_display_length(void)
{
	if (PUBLIC_INFO->product_type == NORMAL_TYPE)
		return 1;
	else
		return MAX_PANEL;
}

 void display_engine_temper_cmds_parse(int panel_id, struct device_node *np)
{
	lcd_kit_parse_u32(np, "lcd-kit,temper-compensation-brightness-support",
		&disp_info->de_temper.lcd_kit_temper_compensation_brightness_support, 0);
	lcd_kit_parse_u32(np, "lcd-kit,temper-compensation-brightness-dbv",
		&disp_info->de_temper.lcd_kit_temper_brightness_dbv, 0);
	lcd_kit_parse_u32(np, "lcd-kit,temper-compensation-brightness-disable-version-support",
		&disp_info->de_temper.lcd_kit_temper_compensation_brightness_disable_version_support, 0);
	lcd_kit_parse_u32(np, "lcd-kit,temper-compensation-brightness-disable-ppu-support",
		&disp_info->de_temper.lcd_kit_temper_compensation_brightness_disable_ppu_support, 0);
	lcd_kit_parse_u32(np, "lcd-kit,temper-compensation-brightness-disable-ppu-temper-max",
		&disp_info->de_temper.lcd_kit_temper_compensation_brightness_disable_ppu_temper_max, 0);
	lcd_kit_parse_u32(np, "lcd-kit,temper-compensation-brightness-disable-ppu-temper-min",
		&disp_info->de_temper.lcd_kit_temper_compensation_brightness_disable_ppu_temper_min, 0);
	LCD_KIT_INFO("temper brightness support: %u\n",
		disp_info->de_temper.lcd_kit_temper_compensation_brightness_support);
	LCD_KIT_INFO("temper brightness dbv: %u\n",
		disp_info->de_temper.lcd_kit_temper_brightness_dbv);
	LCD_KIT_INFO("temper brightness disable version support: %u\n",
		disp_info->de_temper.lcd_kit_temper_compensation_brightness_disable_version_support);
	LCD_KIT_INFO("temper brightness disable ppu support: %u\n",
		disp_info->de_temper.lcd_kit_temper_compensation_brightness_disable_ppu_support);
	LCD_KIT_INFO("temper brightness disable ppu temper max: %u\n",
		disp_info->de_temper.lcd_kit_temper_compensation_brightness_disable_ppu_temper_max);
	LCD_KIT_INFO("temper brightness disable ppu temper min: %u\n",
		disp_info->de_temper.lcd_kit_temper_compensation_brightness_disable_ppu_temper_min);
	return;
}

/* is support brightness disbale panel version */
static bool lcd_kit_local_temper_with_brightness_disable_panel_version(int panel_id)
{
	if (!disp_info || !disp_info->de_temper.lcd_kit_temper_compensation_brightness_disable_version_support) {
		LCD_KIT_DEBUG("not support temper brightness disable panel version function\n");
		return false;
	}
	if (!disp_info->panel_version.lcd_panel_version) {
		LCD_KIT_DEBUG("panel version info not get\n");
		return false;
	}
	if (g_de_context_temper.temper_compensation_brightness_disable_version_func_init &&
		(!g_de_context_temper.temper_compensation_brightness_is_disable_panel_version)) {
		LCD_KIT_DEBUG("this function is already init and this panel version is support\n");
		return false;
	}
	if (g_de_context_temper.temper_compensation_brightness_disable_version_func_init &&
		g_de_context_temper.temper_compensation_brightness_is_disable_panel_version) {
		LCD_KIT_DEBUG("this function is already init and this panel version is not support\n");
		return true;
	}
	LCD_KIT_DEBUG("panel version: %s\n", disp_info->panel_version.lcd_panel_version);
	for (int i = 0; i < TEMPER_BRIFHTNESS_DISABLE_PANEL_VERSION_NUM_MAX; i++) {
		if (strcmp(temper_compensation_disable_version[i], disp_info->panel_version.lcd_panel_version) == 0) {
			g_de_context_temper.temper_compensation_brightness_is_disable_panel_version = true;
			break;
		}
	}
	g_de_context_temper.temper_compensation_brightness_disable_version_func_init = true;
	return g_de_context_temper.temper_compensation_brightness_is_disable_panel_version;
}
 
void display_engine_temper_update_with_brightness(int panel_id, uint32_t brightness_dbv)
{
	LCD_KIT_DEBUG("set temper dbv: %u\n", brightness_dbv);
	if (!common_info->temper_thread.enable) {
		LCD_KIT_DEBUG("not support temper\n");
		return;
	}
	if (!disp_info->de_temper.lcd_kit_temper_compensation_brightness_support) {
		LCD_KIT_DEBUG("not support temper brightness function\n");
		return;
	}
	if (lcd_kit_local_temper_with_brightness_disable_panel_version(panel_id)) {
		LCD_KIT_DEBUG("this panel version not support temper brightness function\n");
		return;
	}
	bool temperEnable = brightness_dbv < disp_info->de_temper.lcd_kit_temper_brightness_dbv;
	if (g_de_context_temper.temper_compensation_enable != temperEnable) {
		g_de_context_temper.temper_compensation_enable = temperEnable;
		display_engine_set_param_panel_temper(panel_id, temperEnable);
		if (!g_de_context_temper.temper_compensation_enable)
			display_engine_temper_brightness_update_ppu_status(panel_id, 0);
		LCD_KIT_DEBUG("set temper %d dbv: %u\n", temperEnable, brightness_dbv);
	}
	return;
}

void display_engine_temper_brightness_update_ppu_status(int panel_id, int32_t temper_value)
{
	if (!disp_info->de_temper.lcd_kit_temper_compensation_brightness_disable_ppu_support)
		return;
	bool ppuDisable = (
		temper_value < disp_info->de_temper.lcd_kit_temper_compensation_brightness_disable_ppu_temper_min ||
		temper_value > disp_info->de_temper.lcd_kit_temper_compensation_brightness_disable_ppu_temper_max) &&
		g_de_context_temper.temper_compensation_enable;
	if (ppuDisable != g_de_context_temper.temper_compensation_brightness_disable_ppu) {
		g_de_context_temper.temper_compensation_brightness_disable_ppu = ppuDisable;
		LCD_KIT_INFO("update ppu status: %d\n",
			g_de_context_temper.temper_compensation_brightness_disable_ppu);
		if (g_de_context_temper.temper_compensation_brightness_disable_ppu)
			notify_de_event(panel_id, PPU_TEMP_COMPENSATION_STATUS_ON);
		else
			notify_de_event(panel_id, PPU_TEMP_COMPENSATION_STATUS_OFF);
	}
}

int display_engine_set_param_panel_temper(int panel_id, bool panel_temper_enable)
{
	int ret = LCD_KIT_OK;
	int panel_nums = 0;
	int temp_panel_id = panel_id;
	struct lcd_kit_adapt_ops *adapt_ops = NULL;

	adapt_ops = lcd_kit_get_adapt_ops();
	if (!adapt_ops) {
		LCD_KIT_ERR("can not get adapt_ops!\n");
		return LCD_KIT_FAIL;
	}
	if (!common_info->temper_thread.enable) {
		LCD_KIT_INFO("not support temper\n");
		return LCD_KIT_OK;
	}
	LCD_KIT_INFO("DISPLAY_ENGINE_DDIC_TEMPER %d +\n", panel_temper_enable);
	panel_nums = lcd_kit_get_init_display_length();
	for (panel_id = 0; panel_id < panel_nums; panel_id++)
		common_info->temper_thread.dark_enable = panel_temper_enable;
	panel_id = temp_panel_id;
	if (panel_temper_enable) {
		common_ops->start_temper_hrtimer(panel_id);
		LCD_KIT_INFO("temper timer is starting!\n");
	} else {
		if (common_info->temper_thread.isAhTemper) {
			common_ops->stop_temper_hrtimer(panel_id);
			LCD_KIT_INFO("temper timer is stoping!\n");
			if (common_info->temper_thread.temper_exit_cmd.cmds)
				ret = adapt_ops->mipi_tx(panel_id, NULL, &common_info->temper_thread.temper_exit_cmd);
		} else {
			common_ops->stop_temper_hrtimer(panel_id);
			LCD_KIT_INFO("temper timer is stoping!\n");
		}
		if (ret) {
				LCD_KIT_ERR("mipi_tx error\n");
				return LCD_KIT_FAIL;
		}
	}
	LCD_KIT_INFO("DISPLAY_ENGINE_DDIC_TEMPER %d -\n", panel_temper_enable);
	return LCD_KIT_OK;
}
