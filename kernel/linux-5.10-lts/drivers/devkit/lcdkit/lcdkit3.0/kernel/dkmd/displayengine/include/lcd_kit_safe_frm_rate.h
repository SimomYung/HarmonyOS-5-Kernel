/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: safe frame rate control decision interface.
 */

#ifndef __LCD_KIT_SAFE_FRM_RATE_
#define __LCD_KIT_SAFE_FRM_RATE_

#include <linux/kernel.h>

#include "lcd_kit_common.h"
#include "dkmd_lcd_interface.h"

int lcd_kit_brightness_trigger_send_sfr(int panel_id, unsigned int full_mode_sfr, unsigned int partial_mode_sfr);
int lcd_kit_ppu_trigger_send_sfr(int panel_id, unsigned int ppu_sfr);
int lcd_kit_safe_frm_rate_ctrl(int panel_id, const struct sfr_info *sfr_info);
void lcd_kit_safe_frm_init(int panel_id, struct device_node *np);
void safe_frm_rate_work_info_workqueue_init(int panel_id);
void display_engine_sfr_update_with_brightness(int panel_id, uint32_t brightness_dbv);
void display_engine_sfr_enter_ap_event(int panel_id, uint32_t value);

#endif