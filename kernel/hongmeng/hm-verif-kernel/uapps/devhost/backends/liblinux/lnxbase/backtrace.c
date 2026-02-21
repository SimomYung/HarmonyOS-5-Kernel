/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Lnxbase backtrace features
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jun 13 12:05:41 2022
 */

#include <securec.h>
#include <lnxbase/lnxbase.h>
#include <hongmeng/types.h>
#include "internal.h"

static int (*__resolve_caller)(char *buf, size_t buf_sz, const void *caller);
static void *(*__return_address)(unsigned int n);
int lnxbase_enable_backtrace(struct lnxbase_ctx *ctx,
			     int (*caller_resolver)(char *buf, size_t buf_sz, const void *caller),
			     void *(return_address)(unsigned int n))
{
	ctx->features |= LNXBASE_FEATURE_BACKTRACE;
	__resolve_caller = caller_resolver;
	__return_address = return_address;
	return 0;
}

int lnxbase_resolve_caller(char *buf, size_t buf_sz, const void *caller)
{
	if (__resolve_caller != NULL) {
		return __resolve_caller(buf, buf_sz, caller);
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
