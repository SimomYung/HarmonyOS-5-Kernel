/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Transfs extension entry
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jul 21 07:26:46 2021
 */

#include <sys/types.h>
#include <devhost/plugin.h>
#include <lnxbase/lnxbase.h>
#include <liblinux/pal.h>
#include <lnxbase/export.h>
#include <lnxbase/ksymtab.h>
#include <devhost/transfs.h>
#include "table.h"

EXPORT_SYMBOL(devhost_register_transfs);
EXPORT_SYMBOL(transfs_iterate_supers);

PUBLIC_SYMBOL
int dh_extension_init(struct lnxbase_ctx *ctx)
{
	int ret;
	const struct lnxbase_syminfo transfs_si = __LIBLINUX_SYMINFO_INITIALIZER;

	ret = lnxbase_register_fget(TRANSFS_FD, devhost_transfs_fget);
	if (ret < 0) {
		hm_error("register transfs fget failed, %d\n", ret);
		return ret;
	}

	ret = lnxbase_register_fput(TRANSFS_FD, devhost_transfs_fput);
	if (ret < 0) {
		hm_error("register transfs fput failed, %d\n", ret);
		return ret;
	}

	ret = lnxbase_register_dentry(devhost_transfs_get_dentry, devhost_transfs_put_dentry);
	if (ret < 0) {
		hm_error("register transfs get dentry ops failed, %d\n", ret);
		return ret;
	}

	ret = lnxbase_load_symbols(ctx, "transfs_ext", &transfs_si);
	if (ret < 0) {
		hm_error("load transfs extension symbols, %d\n", ret);
		return ret;
	}

	ret = libdh_dso_plugin_init(__DH_PLUGIN_NAME__());
	if (ret < 0) {
		hm_error("init transfs plugin failed, %d\n", ret);
		return -hmerrno2posix(ret);
	}
	return 0;
}
