/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: DC switch control interface.
 */

#ifndef __LCD_KIT_PANEL_DC__
#define __LCD_KIT_PANEL_DC__

#include <linux/kernel.h>

#include "lcd_kit_common.h"

void lcd_kit_parse_panel_dc(int panel_id, struct device_node *np);
int display_engine_set_param_panel_dc(int panel_id, bool panel_dc_enable);

#endif