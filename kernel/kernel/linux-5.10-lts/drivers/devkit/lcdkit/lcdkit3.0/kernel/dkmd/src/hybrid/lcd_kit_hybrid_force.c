// SPDX-License-Identifier: GPL-2.0
/*
 * lcd_kit_hybrid_force.c
 *
 * source file for force switch control
 *
 * Copyright (c) 2022-2022 Huawei Technologies Co., Ltd.
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

#include <linux/notifier.h>
#include <linux/types.h>
#include "lcd_kit_hybrid_force.h"
#include "lcd_kit_hybrid_core.h"
#include "lcd_kit_adapt.h"
#include "watch_low_power_manager.h"

#ifdef CONFIG_FACTORY_MODE
static int lcd_kit_hybrid_str_notify(struct notifier_block __always_unused *nb, unsigned long mode,
	void __always_unused *data)
{
	int ret;
	u32 panel_id = 0;

	switch (mode) {
	case WATCH_LOW_POWER_ENTER:
		/* enter str mode */
		LCD_KIT_INFO("enter\n");
		panel_id = lcd_kit_get_active_panel_id();
		ret = lcd_kit_hybrid_mode(panel_id, HYBRID_ON_SWITCH_STR);
		LCD_KIT_INFO("get mode:%d, ret:%d\n", mode, ret);
		break;
	case WATCH_LOW_POWER_EXIT:
		/* exit str mode */
		LCD_KIT_INFO("exit\n");
		panel_id = lcd_kit_get_active_panel_id();
		ret = lcd_kit_hybrid_mode(panel_id, HYBRID_OFF_SWITCH_STR);
		LCD_KIT_INFO("get mode:%d, ret:%d\n", mode, ret);
		break;
	default:
		break;
	}

	return 0;
}

static struct notifier_block g_lcdkit_hybrid_str_notify = {
	.notifier_call = lcd_kit_hybrid_str_notify,
	.priority = 0,
};
#endif

void lcd_kit_hybrid_str_init(void)
{
#ifdef CONFIG_FACTORY_MODE
	if (register_watch_low_power_exit_notifier(WATCH_LOW_POWER_EXIT, &g_lcdkit_hybrid_str_notify) < 0)
		LCD_KIT_ERR("register lcd hybrid pm_notifier failed!");
#endif /* CONFIG_FACTORY_MODE */
}
