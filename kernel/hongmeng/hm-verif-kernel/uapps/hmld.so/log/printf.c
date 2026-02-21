/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Implementation of printf in hmld
 * Author: Huawei OS Kernel Lab
 * Create: Wed Apr 10 13:24:58 2019
 */
#include <libhwsecurec/securec.h>
#include <stdio.h>
#include <stdarg.h>

#include <lib/printf.h>
#include <hongmeng/syscall.h>

#define LOG_BUF_SIZE 272
static char log_buf[LOG_BUF_SIZE];

int hm_log(const char *, ...) __attribute__((alias("printf")));

__attribute__((format(printf, 1, 0)))
int printf(const char *fmt, ...)
{
	int ret;
	va_list ap;

	va_start(ap, fmt);
	ret = vsnprintf_s(log_buf, LOG_BUF_SIZE, LOG_BUF_SIZE - 1, fmt, ap);
	va_end(ap);
	if (ret < 0) {
		char *buf = "printf failed\n";
		(void)sysfast_syslog(HMLOG_DEBUG, buf, strlen(buf), NULL);
		return ret;
	}

	log_buf[LOG_BUF_SIZE - 1] = '\0';
	(void)sysfast_syslog(HMLOG_DEBUG, log_buf, (size_t)(ssize_t)ret, NULL);
	return ret;
}
