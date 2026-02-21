/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Interface of watchdog
 * Author: Huawei OS Kernel Lab
 * Create: Sat Jul 10 14:20:11 2021
 */

#ifndef ULIBS_INCLUDE_LIBHMSRV_SYS_HM_WATCHDOG_H
#define ULIBS_INCLUDE_LIBHMSRV_SYS_HM_WATCHDOG_H

int hm_watchdog_enable(void);

int hm_watchdog_disable(void);

int hm_stop_feed_harddog(unsigned int timeout);

#endif
