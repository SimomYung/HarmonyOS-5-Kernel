/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 *
 * scharger_v800_cur_balance_mos_ctrl.h
 *
 * SchargerV800 current balance mose control header.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#ifndef __SCHARGER_V800_CUR_BALANCE_MOS_CTRL_H__
#define __SCHARGER_V800_CUR_BALANCE_MOS_CTRL_H__

#define DISCHARGE_MONITOR_WORK_DELAY 200
#define VBAT_DELTA_V_THRE 50
#define IBAT_THRE 500
#define VBAT_THRE 3200
#define BATT_AUX_NAME  "battery_gauge_aux"
#define BATT_MAIN_NAME "battery_gauge"

enum charge_state {
	STATE_CHARGING,
	STATE_DISCHARGING_ABOVE_THRE,
	STATE_DISCHARGING_BELOW_THRE,
	STATE_SUSPEND,
};

void scharger_v800_set_cur_balance_mos_state(struct scharger_buck *buck, enum charge_state state);
void scharger_cur_balance_mos_init(struct scharger_buck *buck);
int scharger_get_batt_capacity(int *cap);
int scharger_get_coul_vbat(char *batt_name, int *vbat);
#endif