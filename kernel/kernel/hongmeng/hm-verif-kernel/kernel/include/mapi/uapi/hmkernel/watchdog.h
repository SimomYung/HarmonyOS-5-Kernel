/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: watchdog.h for uapi
 * Author: Huawei OS Kernel Lab
 * Create: Wed Aug 18 09:44:43 2021
 */

#ifndef UAPI_HMKERNEL_WATCHDOG_H
#define UAPI_HMKERNEL_WATCHDOG_H

#define EXT_FASTCALL_GLOBAL_WATCHDOG_CONF	4	/* see kern_syscall.h */

enum wdt_config_act_type {
	WDT_CONF_WATCHDOG_ENABLE,
	WDT_CONF_WATCHDOG_DISABLE,
	WDT_CONF_HARDDOG_STOP_FEED,
};

struct watchdog_config {
	unsigned int harddog_timeout;
};

#endif
