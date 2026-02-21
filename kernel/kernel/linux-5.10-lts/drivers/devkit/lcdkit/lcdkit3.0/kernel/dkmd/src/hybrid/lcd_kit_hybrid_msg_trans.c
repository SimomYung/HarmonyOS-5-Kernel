// SPDX-License-Identifier: GPL-2.0
/*
 * lcd_kit_hybrid_msg_trans.c
 *
 * source file for sending lcd msg to mcu
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
#include <linux/delay.h>
#include <linux/slab.h>
#include "securec.h"
#include "lcd_kit_common.h"
#include "lcd_kit_hybrid_msg_trans.h"
#include "lcd_kit_disp.h"
#include "lcd_kit_hybrid_parse.h"
#include "ext_sensorhub_api.h"

#define TID 0x20
#define SID 0x01
#define CID 0x9F
#define SUBCMD 0x00

#define DEFAULT_DDIC_ID 0x4E
#define SEND_RETRY_DELAY 3000
#define SEND_RETRY_COUNT 10
#define SCHD_WORK_RT_DELAY 5000

static struct DDICMap ddic_map_[MAX_DDICMAP_SIZE];

static uint32_t get_chip_id(void)
{
	int i;
	int panel_id = lcd_kit_get_active_panel_id();

	if (!disp_info->compatible)
		return -EINVAL;

	for (i = 0; i < MAX_DDICMAP_SIZE; i++) {
		if (ddic_map_[i].compatible == NULL)
			continue;

		if (!strcmp(disp_info->compatible, ddic_map_[i].compatible))
			return ddic_map_[i].ddicID;
	}
	return DEFAULT_DDIC_ID;
}

void lcd_kit_hybrid_msg_trans_init(void)
{
	hybrid_parse_ddic_id(ddic_map_);
}

int send_msg_to_mcu(struct hybrid_display_msg *display_msg)
{
	int i = 0;
	int ret = LCD_KIT_FAIL;
	u8 subcmd = SUBCMD;
	struct command cmd;
	unsigned char *buffer = NULL;

	if (!display_msg)
		return LCD_KIT_FAIL;

	display_msg->chipid = get_chip_id();

	LCD_KIT_INFO("display_msg send, chipid = %d, hybrid_ctr = %u, lcd_state = %u, power_state = %u, deep_sleep = %u\n",
		display_msg->chipid, display_msg->hybrid_ctr, display_msg->lcd_state,
		display_msg->power_state, display_msg->deep_sleep);
	buffer = kzalloc(sizeof(subcmd) + sizeof(struct hybrid_display_msg), GFP_KERNEL);
	if (!buffer)
		return LCD_KIT_FAIL;

	if (memcpy_s(buffer, sizeof(subcmd), &subcmd, sizeof(subcmd)) != EOK)
		goto free_buffer;

	if (memcpy_s(buffer + sizeof(subcmd), sizeof(struct hybrid_display_msg),
		display_msg, sizeof(struct hybrid_display_msg)) != EOK)
		goto free_buffer;

	cmd.send_buffer = buffer;
	cmd.send_buffer_len = sizeof(subcmd) + sizeof(struct hybrid_display_msg);
	cmd.service_id = SID;
	cmd.command_id = CID;

	for (i = 0; i < SEND_RETRY_COUNT; ++i) {
		ret = send_command(DISPLAY_CHANNEL, &cmd, false, NULL);
		LCD_KIT_INFO("send hybrid_display_msg ret: %d\n", ret);
		if (ret >= 0)
			break;
		msleep(SEND_RETRY_DELAY);
	}

	if (i >= SEND_RETRY_COUNT) {
		LCD_KIT_INFO("send err after retry\n");
		goto free_buffer;
	}

	kfree(buffer);
	buffer = NULL;
	return LCD_KIT_OK;

free_buffer:
	kfree(buffer);
	buffer = NULL;
	return LCD_KIT_FAIL;
}
