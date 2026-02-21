/*
 * Huawei Touchscreen Driver
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
 
#ifndef __POWER_ADAPTER__
#define __POWER_ADAPTER__

enum thp_power_mode {
	USE_INDEPENDENT_POWER = 0,
	MULTI_PANEL_USE_SINGEL_POWER,
	TP_USE_TDDI_POWER,
	MAX_POWER_USE_TYPE = 255,
};

struct thp_power_adapter {
	struct thp_power_ops *ops;
};

struct thp_power_adapter *thp_power_adapt(enum thp_power_mode power_mode);
struct thp_power_adapter *power_adapter_init(int power_type);
void thp_power_adapter_release(struct thp_power_adapter **power_adapter);
#endif