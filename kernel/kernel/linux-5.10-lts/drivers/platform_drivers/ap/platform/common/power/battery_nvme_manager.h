/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: battery nvme manager headfile
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

#ifndef _BATTERY_NVME_MANAGER_H_
#define _BATTERY_NVME_MANAGER_H_

#define BATT_NVME_QUERY                    0x01
#define BATT_NVME_CLEAR                    0x02
#define BATT_NVME_RESTORE                  0x03

#define MAX_TEMPS               10
#define POWER_SVC_SUCC          0xAB
#define POWER_SVC_FAIL          0

#define WITHOUT_SN              "N/A"

#define MAX_COUNT    3
#define MAX_BATTERIES_WITHOUT_SN 3

#define MAX_BATTERY_SN_LENGTH 20
struct battery_nvme_info {
	char sn[MAX_BATTERY_SN_LENGTH];
	int cycle;
	int limit_fcc;
	short temp[MAX_TEMPS];
	short real_fcc[MAX_TEMPS];
	int latest_record_index;
	int fcc_check_sum_ext;
	short qmax;
	int last_fcc_cycle;
	char cleared;
	char reserved[12];
};

struct battery_library {
    struct battery_nvme_info batteries[MAX_COUNT];
    unsigned char index;
};

struct battery_shmem_to_tzsp {
	struct battery_library library;
	char sn[MAX_BATTERY_SN_LENGTH];
};

#endif
