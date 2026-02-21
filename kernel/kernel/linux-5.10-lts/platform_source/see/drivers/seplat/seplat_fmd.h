/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: seplat big data fmd report.
 * Create : 2024/06/18
 */
#ifndef __SEPLAT_FMD_H_
#define __SEPLAT_FMD_H_

#include "seplat_common.h"

#define SEPLAT_FMD_EVENT_NUM_1           1
#define SEPLAT_FMD_EVENT_NUM_2           2
#define SEPLAT_FMD_EVENT_NUM_3           3
#define SEPLAT_FMD_EVENT_NUM_4           4

#define SEPLAT_DSM_RESULT_MAX_LEN        8
#define SEPLAT_DSM_STATION_MAX_LEN       8
#define SEPLAT_DSM_NAME_MAX_LEN          32
#define SEPLAT_DSM_VALUE_MAX_LEN         64
#define SEPLAT_ITEM_ID_RT_FLASH_ECC      703011001

enum seplat_dsm_type {
	SEPLAT_DSM_TYPE_BEGINE = 0,
	SEPLAT_DSM_TYPE_FLASH_ECC,
	SEPLAT_DSM_TYPE_END,
};

struct seplat_dsm_event {
	int errno;
	int item_id;
	char result[SEPLAT_DSM_RESULT_MAX_LEN];
	char test_name[SEPLAT_DSM_NAME_MAX_LEN];
	char value[SEPLAT_DSM_VALUE_MAX_LEN];
	char min_threshold[SEPLAT_DSM_VALUE_MAX_LEN];
	char max_threshold[SEPLAT_DSM_VALUE_MAX_LEN];
};

int seplat_dsm_report_fmd(enum seplat_dsm_type type, struct seplat_dsm_event *event_info, int event_num);

#endif /* __SEPLAT_FMD_H_ */
