/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: Add sys rpc from ldk and sysmgr(udk)
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jul 05 15:25:38 2024
 */
#include <errno.h>
#include <stdbool.h>

#include <hongmeng/errno.h>
#include <hongmeng/types.h>

#include <devhost/rpc.h>
#include <devhost/memory.h>
#include <lnxbase/export.h>
#include <liblinux/pal.h>

#include "../internal.h"

int liblinux_pal_rpc_invoke(uint64_t rpc_key, int cmd, void *rd,
			    size_t rd_size, void *wr,
			    size_t wr_size, bool is_xact)
{
	int devid = 0;
	struct rpc_ctx req  = { .buf = rd, .size = (unsigned long)rd_size };
	struct rpc_ctx resp = { .buf = wr, .size = (unsigned long)wr_size };

	if (rpc_key == 0) {
		return -EOPNOTSUPP;
	} else {
		return devhost_rpc_invoke(rpc_key, devid, cmd, &req, &resp, NULL, is_xact);
	}
}

int liblinux_pal_copy_from_caller(void *dst, const void *src, unsigned long n)
{
	int ret;

	ret = devhost_pal_copy_from_caller(dst, src, n);
	if (ret < 0) {
		return -hmerrno2posix(ret);
	}
	return ret;
}

int liblinux_pal_copy_to_caller(void *dst, const void *src, unsigned long n)
{
	int ret;

	ret = devhost_pal_copy_to_caller(dst, src, n);
	if (ret < 0) {
		return -hmerrno2posix(ret);
	}
	return ret;
}
