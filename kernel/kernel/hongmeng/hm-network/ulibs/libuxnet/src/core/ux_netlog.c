/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: interface for network log
 * Author: Huawei OS Kernel Lab
 * Create: Sat Feb 22 12:04:17 2020
 */

#include "ux_netlog.h"

static uint32_t __ux_netlog_module_mask;
static enum hmlog_level __ux_netlog_level;

static int ux_tcpip_log_level;
static void (*ux_tcpip_set_log_level_hook)(int);

void ux_hmnetd_log_mask(uint32_t mask)
{
	__ux_netlog_module_mask = mask;
	net_info(HMNET_CORE, "set module mask to 0x%x\n", mask);
}

void ux_hmnetd_log_level(int level)
{
	__ux_netlog_level = (enum hmlog_level)level;
	net_info(HMNET_CORE, "set netd log level to %d\n", level);
}

bool ux_is_module_enabled(int module)
{
	return ((uint32_t)module & __ux_netlog_module_mask) != 0;
}

enum hmlog_level ux_get_log_level(void)
{
	return __ux_netlog_level;
}

static void ux_set_tcpip_log_level(int level)
{
	ux_tcpip_log_level = level;
	if (ux_tcpip_set_log_level_hook != NULL) {
		ux_tcpip_set_log_level_hook(level);
	}
}

void ux_register_tcpip_set_log_level_hook(void (*hook)(int))
{
	ux_tcpip_set_log_level_hook = hook;
	ux_tcpip_set_log_level_hook(ux_tcpip_log_level);
}

void ux_hmnetd_log_default(void)
{
	ux_hmnetd_log_mask(HMNET_LOG_DEFAULT_MASK);
	ux_hmnetd_log_level((int)HMLOG_INFO);
	ux_set_tcpip_log_level((int)HMLOG_WARN);
}

void ux_hmnetd_log_setting(uint32_t set)
{
	if ((set & HMNET_LOG_SETTING_LABEL) != 0U) {
		ux_hmnetd_log_level((int)(set & HMNET_LOG_SETTING_MASK));
	} else if ((set & HMNET_LOG_SETTING_AF_LABEL) != 0U) {
		ux_set_tcpip_log_level((int)(set & HMNET_LOG_SETTING_MASK));
	} else {
		ux_hmnetd_log_mask(set & HMNET_LOG_SETTING_MASK);
	}
}
