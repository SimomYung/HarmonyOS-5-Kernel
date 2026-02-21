/*
 * lcd_kit_partial_refresh.h
 *
 * lcdkit partial refresh function head file for lcd driver
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

#ifndef _LCD_KIT_PARTIAL_REFRESH_H_
#define _LCD_KIT_PARTIAL_REFRESH_H_
#include <linux/types.h>
#include "lcd_kit_utils.h"

/* Indicates the sequence number of the corresponding command. */
#define CMDS_ZERO 0
#define CMDS_ONE 1
#define CMDS_TWO 2
#define CMDS_THREE 3
#define CMDS_FOUR 4
#define CMDS_FIVE 5
#define CMDS_SIX 6
#define CMDS_SEVEN 7
#define CMDS_NINE 9
#define CMDS_TEN 10
#define CMDS_TWELVE 12
#define CMDS_THIRTEEN 13
#define CMDS_SIXTEEN 16
#define CMDS_SEVENTEEN 17
#define CMDS_TWENTY 20
#define CMDS_TWENTYONE 21

/* Number of plyload */
#define PLYLOAD_ZERO 0
#define PLYLOAD_ONE 1
#define PLYLOAD_TWO 2
#define PLYLOAD_THREE 3
#define PLYLOAD_FOUR 4

/* Dirty region coordinate position */
#define COORADINATE_X 0
#define COORADINATE_Y 1
#define COORADINATE_W 2
#define COORADINATE_H 3

/* Number of shifted 8 bits */
#define EIGHT_BIT 8
#define EIGHT_BIT_ONE 1
#define EIGHT_BIT_TWO 2
#define EIGHT_BIT_THREE 3

/* ip reg position */
#define IP1_F8 0
#define IP1_F9 1
#define IP2_F5 2
#define IP2_F6 3

#define MIN_OF_XHS_CMDS 2
#define MIN_OF_CMDS_INFO_DATA 2
#define PART_REFRESH_REG_VALUE_NUM_MAX 20
#define PART_REFRESH_IP1_OFFSET_PARAM_DEFAULT 12
#define PART_REFRESH_IP2_OFFSET_PARAM_DEFAULT 332

struct rd_region_info {
	uint32_t region_b_start;
	uint32_t region_c_start;
	uint32_t ip1_offset_b;
	uint32_t ip1_offset_c;
	uint32_t ip2_offset_b;
	uint32_t ip2_offset_c;
};

// get raydium three regions info
void lcd_kit_rd_get_refresh_cmds(int panel_id,
	const struct dkmd_cmds_info *cmds_info, struct mipi_dsi_tx_params *params);
void lcd_kit_get_rd_region_refresh_info(const struct dkmd_cmds_info *cmds_info);
#endif

