/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Iaware notify extension entry
 * Author: Huawei OS Kernel Lab
 * Create: Tue Mar 28 12:15:54 2023
 */

#include <sys/types.h>
#include <devhost/backend.h>
#include <devhost/plugin.h>
#include <lnxbase/lnxbase.h>
#include <liblinux/pal.h>
#include <lnxbase/export.h>
#include <lnxbase/ksymtab.h>
#include <libdevhost/devhost.h>

#include "iaware_notify_api.h"
#include "iaware_notify.h"

struct devhost_iaware_notify_ops *iaware_notify_ops __read_mostly;

PUBLIC_SYMBOL
int devhost_iaware_notify_ops_init(struct devhost_iaware_notify_ops *ops)
{
	BUG_ON(ops == NULL);

	iaware_notify_ops = ops;
	return 0;
}
EXPORT_SYMBOL(devhost_iaware_notify_ops_init);

PUBLIC_SYMBOL
int dh_extension_init(struct lnxbase_ctx *ctx)
{
	int ret;
	const struct lnxbase_syminfo iaware_notify_si = __LIBLINUX_SYMINFO_INITIALIZER;

	ret = devhost_regist_handler_to_iaware();
	if (ret < 0) {
		return ret;
	}

	ret = set_default_whitelist();
	if (ret < 0) {
		return ret;
	}

	ret = devhost_postinit(devhost_iaware_notify_procfs_init, NULL);
	if (ret < 0) {
		return ret;
	}

	ret = lnxbase_load_symbols(ctx, "iaware_notify_ext", &iaware_notify_si);
	if (ret < 0) {
		hm_error("dh_iaware_notify: load extension symbols, %s\n", strerror(ret));
		return ret;
	}

	ret = libdh_dso_plugin_init(__DH_PLUGIN_NAME__());
	if (ret < 0) {
		hm_error("dh_iaware notify: init plugin failed, %s\n", hmstrerror(ret));
		return -hmerrno2posix(ret);
	}

	return 0;
}
