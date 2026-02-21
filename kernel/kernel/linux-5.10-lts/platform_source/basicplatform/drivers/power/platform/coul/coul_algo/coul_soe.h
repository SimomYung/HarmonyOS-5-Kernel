/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: coul_soe.h
 *
 * smartstar coulometer functions
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
#ifndef _COUL_SOE_H_
#define _COUL_SOE_H_

#define SOE_PARAMS_GAIN         1000000
#define DELTA_SOE_GAIN          1000
#define CHARGE_COUNT_COE        10
#define DISCHARGE_COUNT_COE     9
#define SOE_INTERVAL            20
#define DELTA_SOE_PC_DEFAULT    50
#define DELTA_SOE_SMOOTH        30

void coul_soe_initial(int batt_index, int soe_enable, struct soe_params *soe_param, struct soe_information *soe_info);
int cou_calc_soe_cap(struct soe_params *soe_params, struct soe_information *soe_info, int *soe_value);
#endif