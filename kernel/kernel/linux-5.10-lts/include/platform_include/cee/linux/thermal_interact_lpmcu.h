/* SPDX-License-Identifier: GPL-2.0
 * thermal_interact_lpmcu.h
 *
 * header of thermal lpmcu interact
 *
 * Copyright (C) 2025 Huawei Technologies Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */
#ifndef __THERMAL_INTERACT_LPMCU_H
#define __THERMAL_INTERACT_LPMCU_H

#include <linux/list.h>

#define MAX_LEN	15

struct thermal_lpmcu_ops {
	const char name[MAX_LEN];
	int (*lowtemp_thermal_handler)(unsigned int);
	struct list_head ops_list;
};

#ifdef CONFIG_LPMCU_INTERACT_THERMAL
void ddr_to_isp_callback_register(int (*ddr_to_isp_handler)(unsigned int));
void ddr_to_isp_callback_unregister(void);
int thermal_lpmcu_ops_register(struct thermal_lpmcu_ops *thermal_ops);
void thermal_lpmcu_ops_unregister(struct thermal_lpmcu_ops *thermal_ops);
int thermal_isp_mbox_init(void);
#else
void ddr_to_isp_callback_register(int (*ddr_to_isp_handler)(unsigned int))
{
}

void ddr_to_isp_callback_unregister(void)
{
}

int thermal_lpmcu_ops_register(struct thermal_lpmcu_ops *thermal_ops)
{
        return 0;
}

void thermal_lpmcu_ops_unregister(struct thermal_lpmcu_ops *thermal_ops)
{
}

int thermal_isp_mbox_init(void)
{
        return 0;
}
#endif
#endif
