/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Watchdog module interface
 * Author: Huawei OS Kernel Lab
 * Create: Tue Nov 26 15:36:10 2019
 */
#ifndef MAPI_HMKERNEL_WATCHDOG_MODULE_H
#define MAPI_HMKERNEL_WATCHDOG_MODULE_H

#include <hmkernel/compiler.h>
#include <uapi/hmkernel/watchdog_module.h>

void update_timeout_of_highdog(unsigned int sec_kern);
void wdt_kvic_trigger_pending_fire(unsigned int arg, bool force_migrate);
void wdt_update_highdog_reason(unsigned int reason);
void wdt_update_service_reboot_reason(const char *highdog_service, unsigned int highdog_code);

#endif
