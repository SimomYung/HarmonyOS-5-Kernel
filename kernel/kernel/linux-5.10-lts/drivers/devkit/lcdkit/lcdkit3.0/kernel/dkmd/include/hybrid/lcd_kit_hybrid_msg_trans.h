/* SPDX-License-Identifier: GPL-2.0 */
/*
 * lcd_kit_hybrid_msg_trans.h
 *
 * head file for lcd msg send to mcu
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
#ifndef LCD_KIT_HYBRID_INFO_H
#define LCD_KIT_HYBRID_INFO_H

#include "lcd_kit_hybrid_core.h"
/*
 * Init chipid resource
 */
void lcd_kit_hybrid_msg_trans_init(void);

/*
 * send ddic,lcd_state,hybrid_contral,power_state,deep_sleep msg
 */
int send_msg_to_mcu(struct hybrid_display_msg *display_msg);
#endif

