/* SPDX-License-Identifier: GPL-2.0 */
/*
 * lcd_kit_hybrid_core.h
 *
 * head file for hybrid switch core
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
#ifndef __LCD_KIT_HYBRID_CORE_H_
#define __LCD_KIT_HYBRID_CORE_H_

#include <linux/types.h>
#define CONT_MIPI_CLK 6
#define MIPI_CLK_BIT_OFFSET 1
#define MIPI_CLK_252MHZ_BUF    "00010"
#define MIPI_CLK_260MHZ 0x0001
#define MIPI_CLK_252MHZ 0x0002
#define MIPI_CLK_240MHZ 0x0004
#define MIPI_CLK_245MHZ 0x0008
#define MIPI_CLK_255MHZ 0x0010

enum hybrid_mode {
	/* AOD enum */
	HYBRID_ALPM_ON_HIGH_LIGHT = 1,
	HYBRID_ALPM_ON_LOW_LIGHT = 3,
	HYBRID_ALPM_ON_MIDDLE_LIGHT = 6,
	HYBRID_ALPM_ON_HBM_LIGHT = 16,
	HYBRID_ALPM_ON_NO_LIGHT = 17,

	HYBRID_ALPM_ENTER = 0,
	HYBRID_ALPM_EXIT = 2,
	HYBRID_ALPM_TE_ON = 11,
	HYBRID_ALPM_TE_OFF = 12,

	/* lcd on/off enum inner usage */
	HYBRID_LCD_ON = 13,
	HYBRID_LCD_OFF = 14,
	HYBRID_LCD_IDLE = 0,

	/* hybrid ctrl enum */
	HYBRID_OFF_SWITCH_MCU = 7,
	HYBRID_ON_SWITCH_MCU = 8,
	HYBRID_IDLE_SWITCH_MCU = 4,

	HYBRID_OFF_SWITCH_AP = 9,
	HYBRID_ON_SWITCH_AP = 10,
	HYBRID_IDLE_SWITCH_AP = 15,

	HYBRID_ON_SWITCH_STR = 18,
	HYBRID_OFF_SWITCH_STR = 19
};

enum display_state {
	DISPLAY_OFF = 0,
	DISPLAY_ON = 1,
	DISPLAY_AOD = 2,

	AP_DISPLAY_ON = 11,
	AP_DISPLAY_AOD = 12,
	AP_DISPLAY_OFF = 13,
	AP_DISPLAY_DEEP_SLEEP = 14,

	DISPLAY_ON_STR = 21,
	DISPLAY_OFF_STR = 22,
};

struct hybrid_display_msg {
	//DDic id
	uint32_t chipid;
	// hybrid_control
	bool hybrid_ctr;
	/* lcd state */
	u8 lcd_state;
	//device_state -- power_state
	bool power_state;
	//deepsleep
	u8 deep_sleep;
};

/*
 * initialize the lcd hybrid
 * @param panel the display panel to initialize
 */
void lcd_kit_hybrid_init(void);

/*
 * request mipi switch
 * @param value 0 for release, 1 for request.
 */
int hybrid_request_display(int value);

/*
 * check if AP hold display mipi
 * @return true for display at AP, false otherwise.
 */
bool hybrid_control_check(void);

bool hybrid_control_check_mcu(void);

int lcd_kit_get_active_panel_id(void);

struct lcd_kit_common_ops *get_hybrid_common_ops(void);

void lcd_kit_set_aod_panel_state(int panel_id, uint32_t status);

int lcd_kit_hybrid_mode(u32 panel_id, enum hybrid_mode mode);
int lcd_kit_hybrid_mode_async(u32 panel_id, enum hybrid_mode mode);

void lcd_kit_hybrid_esd_on(void);

void lcd_kit_hybrid_esd_off(void);

/* Quick on at gesture wakeup*/
void lcd_kit_hybrid_gesture_qon(void);

void hybrid_set_display_msg(u8 state);
void hybrid_get_display_msg(void);

void lcd_kit_hybrid_hw_reset(void);
void lcd_kit_hybrid_gesture_qon_reset(void);
#endif
