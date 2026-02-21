/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Lnxbase backtrace features
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jun 13 12:05:41 2022
 */

#include <securec.h>
#include <lnxbase/lnxbase.h>
#include <hongmeng/types.h>
#include <hongmeng/errno.h>
#include <hongmeng/compiler.h>
#include "internal.h"

static void *(*__return_address)(unsigned int n);
static int (*__liblinux_vsnprintf)(char *buf, size_t size, const char *fmt, va_list ap);
int lnxbase_enable_backtrace(struct lnxbase_ctx *ctx,
			     void *(return_address)(unsigned int n),
			     int (*liblinux_vsnprintf)(char *buf, size_t size, const char *fmt, va_list ap))
{
	ctx->features |= LNXBASE_FEATURE_BACKTRACE;
	__return_address = return_address;
	__liblinux_vsnprintf = liblinux_vsnprintf;
	return 0;
}

/**
 * Must check __liblinux_vsnprintf registered before calling lnxbase_liblinux_format_string
 */
static int lnxbase_liblinux_format_string(char *buf, size_t size, const char *fmt, ...)
{
	int ret;
	va_list args;
	va_start(args, fmt);
	ret = __liblinux_vsnprintf(buf, size, fmt, args);
	va_end(args);
	return ret;
}

int lnxbase_resolve_caller(char *buf, size_t buf_sz, const void *caller)
{
	if (likely(__liblinux_vsnprintf != NULL)) {
		return lnxbase_liblinux_format_string(buf, buf_sz, "%pS", caller);
	} else {
		return sprintf_s(buf, buf_sz, "0x%lx", ptr_to_ulong(caller));
	}
}

void *lnxbase_return_address(unsigned int n)
{
	/* skip current frame */
	if (__return_address) {
		return __return_address(n + 1);
	} else {
		return NULL;
	}
}

int lnxbase_liblinux_vformat_string(char *buf, size_t size, const char *fmt, va_list ap)
{
	if (likely(__liblinux_vsnprintf != NULL)) {
		return __liblinux_vsnprintf(buf, size, fmt, ap);
	} else {
		return -hmerrno2posix(E_HM_NOSYS);
	}
}
