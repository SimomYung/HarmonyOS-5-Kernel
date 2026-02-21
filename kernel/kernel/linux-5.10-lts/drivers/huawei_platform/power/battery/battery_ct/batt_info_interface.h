/*
 * batt_info_interface.h
 *
 * battery information interface head file
 *
 * Copyright (c) 2012-2025 Huawei Technologies Co., Ltd.
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

#ifndef _BATT_INFO_INTERFACE_H_
#define _BATT_INFO_INTERFACE_H_

#include "batt_early_param.h"
#include "batt_aut_checker.h"
#include "batt_info.h"

/* batt storage info */
#define BATT_ACTIVATION_YEAR_MAX                     225
#define BATT_ACTIVATION_MONTH_MIN                    1
#define BATT_ACTIVATION_MONTH_MAX                    12
#define BATT_CYCLE_MAX                               0xFFFF
#define BATT_SOH_MAX                                 100
#define BLANK_UPDATE_WAIT_INTERVAL                   1500  /* 1500ms */

int get_battery_backup_id(char *buf, int buf_size);
int get_battery_type_by_id(unsigned int index, char *name, unsigned int name_size);
void update_battery_cycle(struct batt_chk_data *checker_data);
void update_battery_real_soh(struct batt_chk_data *checker_data);
void update_battery_display_soh(struct batt_chk_data *checker_data);

#endif /* _BATT_INFO_INTERFACE_H_ */
