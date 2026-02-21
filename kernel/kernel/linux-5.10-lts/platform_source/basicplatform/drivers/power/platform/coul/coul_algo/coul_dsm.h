/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: get dts info for coul core method
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

#ifndef _COUL_ALGO_DSM_H_
#define _COUL_ALGO_DSM_H_

#include <platform_include/basicplatform/linux/power/platform/coul/coul_core.h>

#define WAIT_SOC_CHANGE_CHECK   0
#define SOC_CHANGE_CHECKING     1
#define UNFILTER_SOC_MIN 	(-5)
#define UNFILTER_SOC_MAX 	105
#define SOC_EST_ERROR_MAX 	10
#define ABNORMAL_DRC_PERCENT 	5 /* 5% */
#define ABNORMAL_UUC_PERCENT 	10 /* 10% */
#define ABNORMAL_LIMIT_FCC_MAX	105
#define ABNORMAL_LIMIT_FCC_MIN	95
#define SR_OCV_REPORT_TIME_SEC	(1 * 60 * 60) /* 1 hour */
#define UUC_TEMP_THRESHOLD	10
#define ADJUST_SOC_FAST_THRESHOLD	3

struct coul_dsm_check {
	int error_no;
	bool notify_enable;
	void (*dump)(struct smartstar_coul_device *di, char *buf);
	int (*check_error)(struct smartstar_coul_device *di, char *buf);
};

void coul_dsm_info_init(struct smartstar_coul_device *di);
void coul_algo_dsm_check(struct smartstar_coul_device *di);
void coul_algo_dsm_flag_enable(void);
void coul_report_ocv_cali_info(struct smartstar_coul_device *di);
void coul_report_adjust_soc_fast_info(struct smartstar_coul_device *di, int soc_est, int soc);
void coul_para_report(struct smartstar_coul_device *di);
#endif