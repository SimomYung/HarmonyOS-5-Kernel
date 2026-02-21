/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: ocv select and table monitor functions for coul module.
 *
 * This software is licensed under the terms of the GNU General Public
 * License, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */

#ifndef _COUL_OCV_MONITOR_H_
#define _COUL_OCV_MONITOR_H_

#include <platform_include/basicplatform/linux/power/platform/coul/coul_core.h>

bool coul_ocv_check_dense_area(int batt_index, int ocv_uv);
bool coul_ocv_is_dischg_state(int batt_index, int soc_10x);

enum ocv_table_type coul_ocv_select_type(int batt_index, int ocv_uv, int soc_10x);
int coul_ocv_cal_tbl_pc(int batt_index, int ocv_uv, enum ocv_table_type ocv_type, int temp_10x);
int coul_ocv_cal_tbl_ocv(int batt_index, int soc_10x, enum ocv_table_type ocv_type, int temp_10x);

void coul_ocv_chg_event_process(int batt_index, unsigned int evt);
int coul_ocv_register(int batt_index, struct coul_battery_data *batt_data,
    const struct device_node *np);

#endif