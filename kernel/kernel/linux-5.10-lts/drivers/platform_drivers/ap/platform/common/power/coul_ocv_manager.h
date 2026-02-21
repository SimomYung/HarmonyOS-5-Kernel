/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2027. All rights reserved.
 * Description: coul ocv manager headfile
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

#ifndef _COUL_OCV_MANAGER_H_
#define _COUL_OCV_MANAGER_H_

#define COUL_SEL_OCV_TYPE       0x01
#define COUL_SEL_OCV_BY_TYPE    0x02

#define POWER_SVC_SUCC          0xAB
#define POWER_SVC_FAIL          0

#define TENTH                    10
#define SOC_FULL                 100

struct ocv_chg_data {
    int enable;
    int chg_stop_soc;
    int chg_start_soc;
    int chg_highcap;
    int dchg_highcap_thred;
    int chg_thred;
    int dchg_up_thred;
    int dchg_down_thred;
    int dischg_ocv_soc;
    struct coul_battery_data *batt_data;
};

struct coul_shmem_to_tzsp {
    struct ocv_chg_data ocd;
    int soc;
    int ocv_chg;
    int ocv_dchg;
    enum ocv_table_type ocv_type;
    int result;
};
#endif
