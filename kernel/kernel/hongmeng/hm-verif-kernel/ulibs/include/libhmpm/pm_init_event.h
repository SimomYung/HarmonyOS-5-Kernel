/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: Definition for tppmgr init event notifier api
 * Author: Huawei OS Kernel Lab
 * Create: Tue June 4 16:21:59 2024
 */

#ifndef ULIBS_LIBHMPM_INIT_EVENT_H
#define ULIBS_LIBHMPM_INIT_EVENT_H

enum pm_init_event {
	INIT_EVENT_FREQ_DRIVER,
	INIT_EVENT_NR,
};

int hm_pm_init_notify(enum pm_init_event event);

#endif
