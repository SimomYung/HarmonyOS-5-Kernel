/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: Deep calibration for DDIC demura control interface.
 */
 
#ifndef __LCD_KIT_PANEL_DEEP_CALIBRATION_
#define __LCD_KIT_PANEL_DEEP_CALIBRATION_
 
#include <linux/kernel.h>
 
#include "lcd_kit_common.h"
 
int display_engine_set_param_panel_deep_calibration(int panel_id, bool panel_deep_calibration_enable);
 
#endif