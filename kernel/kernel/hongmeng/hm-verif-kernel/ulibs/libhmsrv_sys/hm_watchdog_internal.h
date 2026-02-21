/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: internal functions for hm_watchdog
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jan 26 16:03:44 2022
 */

#ifndef ULIBS_HM_WATCHDOG_INTERNAL_H
#define ULIBS_HM_WATCHDOG_INTERNAL_H

#include <hmkernel/watchdog.h>
#include <hongmeng/ext_fast_syscall.h>

__EXT_FASTCALL_GLOBAL(2, watchdog_config, EXT_FASTCALL_GLOBAL_WATCHDOG_CONF,
		      enum wdt_config_act_type, act, const struct watchdog_config *, conf)

#endif
