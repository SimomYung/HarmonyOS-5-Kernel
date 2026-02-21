/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Native interfaces of watchdog related operation
 * Author: Huawei OS Kernel Lab
 * Create: Sat Jul 10 14:20:11 2021
 */

#include <stddef.h>
#include <libhmsrv_sys/hm_watchdog.h>

#include "hm_watchdog_internal.h"

static inline int hm_watchdog_config(enum wdt_config_act_type act,
				     const struct watchdog_config *conf)
{
	return sysfast_watchdog_config(act, conf);
}

int hm_watchdog_enable(void)
{
	return hm_watchdog_config(WDT_CONF_WATCHDOG_ENABLE, NULL);
}

int hm_watchdog_disable(void)
{
	return hm_watchdog_config(WDT_CONF_WATCHDOG_DISABLE, NULL);
}

int hm_stop_feed_harddog(unsigned int timeout)
{
	struct watchdog_config conf;
	conf.harddog_timeout = timeout;

	return hm_watchdog_config(WDT_CONF_HARDDOG_STOP_FEED, &conf);
}
