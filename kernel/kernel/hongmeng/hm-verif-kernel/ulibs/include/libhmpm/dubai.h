/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: Dubai hwlog api.
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jul 15 10:48:41 2024
 */
#ifndef ULIBS_LIBHMPM_DUBAI_H
#define ULIBS_LIBHMPM_DUBAI_H

#define MAX_DUBAI_LOG_LEN 256

struct pm_dubai_ctrl_log {
	unsigned int log_lv;
	char tag[MAX_DUBAI_LOG_LEN];
	char msg[MAX_DUBAI_LOG_LEN];
};

#endif
