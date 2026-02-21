/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: IRC switch control interface.
 */

#ifndef __LCD_KIT_PANEL_TEMP_COMPENSATION_
#define __LCD_KIT_PANEL_TEMP_COMPENSATION_

#include <linux/kernel.h>

#include "lcd_kit_common.h"
#include "lcd_kit_disp.h"

int display_engine_set_param_panel_temper(int panel_id, bool panel_temper_enable);
void display_engine_temper_cmds_parse(int panel_id, struct device_node *np);
void display_engine_temper_update_with_brightness(int panel_id, uint32_t brightness_dbv);
void display_engine_temper_brightness_update_ppu_status(int panel_id, int32_t temper_value);

#endif