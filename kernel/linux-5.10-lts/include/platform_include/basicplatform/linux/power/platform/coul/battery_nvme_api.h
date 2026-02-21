/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: coul core headfile
 *
 * This software is licensed under the terms of the GNU General Public
 * License, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#ifndef _BATTERY_NVME_API_H_
#define _BATTERY_NVME_API_H_
#include <battery_nvme_manager.h>

#ifdef CONFIG_COUL_NVME_MANAGER
/* coul_core to battery_nvme_manager */
void save_batt_nv_library(int batt_index, const char *batt_sn, struct battery_nvme_info *from_coul_core);
/* battery_nvme_manager to coul_core */
int quary_batt_nv_library(int batt_index, const char *batt_sn, struct battery_nvme_info *to_coul_core);
void batt_nv_cycle_clear_all(void);
void batt_nv_cycle_restore_all(void);
#else

#ifndef unused_var
#define unused_var(var) ((void)(var))
#endif

static inline void save_batt_nv_library(int batt_index,
	const char *batt_sn, struct battery_nvme_info *from_coul_core)
{
	unused_var(batt_index);
	unused_var(batt_sn);
	unused_var(from_coul_core);
}

static inline int quary_batt_nv_library(int batt_index,
	const char *batt_sn, struct battery_nvme_info *to_coul_core)
{
	unused_var(batt_index);
	unused_var(batt_sn);
	unused_var(to_coul_core);
	return 0;
}

static inline void batt_nv_cycle_clear_all(void)
{
}

static inline void batt_nv_cycle_restore_all(void)
{
}
#endif

#endif

