/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Watch dog module
 * Author: Huawei OS Kernel Lab
 * Create: Mon Mar 30 10:17:33 CST 2020
 */

#ifndef UAPI_HMKERNEL_WATCHDOG_MODULE_H
#define UAPI_HMKERNEL_WATCHDOG_MODULE_H

enum __watchdog_cmd {
	__WATCHDOG_UPDATE_WATCHDOG = 0,
	__WATCHDOG_UPDATE_HIGHDOG_REASON,
	__WATCHDOG_TRIGGER_SNAPSHOT,
	__WATCHDOG_UPDATE_SERVICE_REBOOT_REASON,
};

#endif
