/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: interface for network procfs
 * Author: Huawei OS Kernel Lab
 * Create: Fri Mar 29 14:06:09 2024
 */
#include "security/sec_hook.h"
#include "ux_procfs.h"

struct procfs_special_fn_t {
	special_read_fn read;
	special_write_fn write;
};

static struct procfs_special_fn_t procfs_special_fn[PROCFS_SP_MAX];

void ux_procfs_register_special_func(enum procfs_special_callnum callnum,
				     special_read_fn read_fn, special_write_fn write_fn)
{
	BUG_ON(callnum >= PROCFS_SP_MAX);
	procfs_special_fn[callnum].read = read_fn;
	procfs_special_fn[callnum].write = write_fn;
}

int ux_procfs_dispatch_special_read(enum procfs_special_callnum callnum, void *buf, size_t count, int type)
{
	int ret;

	ret = sec_chk_div_as_netd();
	if (ret != E_HM_OK) {
		return ret;
	}

	if (callnum >= PROCFS_SP_MAX) {
		return E_HM_INVAL;
	}

	if (procfs_special_fn[callnum].read == NULL) {
		return E_HM_INVAL;
	}
	return procfs_special_fn[callnum].read(buf, count, type);
}

int ux_procfs_dispatch_special_write(enum procfs_special_callnum callnum, const void *buf, size_t count, int type)
{
	int ret;

	ret = sec_chk_div_as_netd();
	if (ret != E_HM_OK) {
		return ret;
	}

	if (callnum >= PROCFS_SP_MAX) {
		return E_HM_INVAL;
	}

	if (procfs_special_fn[callnum].write == NULL) {
		return E_HM_INVAL;
	}
	return procfs_special_fn[callnum].write(buf, count, type);
}
