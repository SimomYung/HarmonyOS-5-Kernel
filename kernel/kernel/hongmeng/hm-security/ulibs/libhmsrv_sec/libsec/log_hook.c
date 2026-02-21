/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Log information denied by security framework
 * Author: Huawei OS Kernel Lab
 * Create: Wed May 26 17:48:10 2021
 */
#include <libsec/log.h>
#include <libhmsrv_audit/audit.h>
#include <libhmaudit/audit_log.h>

static sec_deny_log_hook_func_t g_sec_deny_log_hook_func = NULL;
void sec_set_deny_log_hook(const sec_deny_log_hook_func_t func)
{
	g_sec_deny_log_hook_func = func;
}

void sec_deny_log_hook(const char *fmt, ...)
{
	va_list args;
	if (g_sec_deny_log_hook_func != NULL) {
		va_start(args, fmt);
		g_sec_deny_log_hook_func(HM_AUDIT_AVC, TYPE_AUDIT_COMMON | TYPE_AUDIT_FORMAT, fmt, args);
		va_end(args);
	}
}
