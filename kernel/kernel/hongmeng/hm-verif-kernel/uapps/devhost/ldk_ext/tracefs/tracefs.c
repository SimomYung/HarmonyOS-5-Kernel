/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Devhost tracefs
 * Author: Huawei OS Kernel Lab
 * Create: Fri Aug 11 03:29:27 2023
 */
#include <hongmeng/errno.h>
#include <lnxbase/lnxbase.h>
#include <lnxbase/export.h>
#include <devhost/plugin.h>
#include <devhost/log.h>
#include <libdevhost/devhost.h>
#include <libsysif/devhost/api.h>

#include "tracefs_api.h"
#include "tracefs.h"
#ifdef PAC_FWCFI_SYSSERVICE
#include <lib/ext_pac.h>
#endif

struct devhost_tracefs_ops *tracefs_ops;

int devhost_trace_enable_disable_ldk_event(unsigned int tp_index, bool enable)
{
	BUG_ON(tracefs_ops == NULL);

	int rc;

	rc = tracefs_ops->enable_disable_ldk_event(tp_index, enable);
	if (rc != 0) {
		return posix2hmerrno(-rc);
	}
	return E_HM_OK;
}

int devhost_trace_ldk_event_format(unsigned int tp_index, char *format)
{
	BUG_ON(tracefs_ops == NULL);

	int rc;

	rc = tracefs_ops->ldk_event_format(tp_index, format);
	if (rc != 0) {
		return posix2hmerrno(-rc);
	}
	return E_HM_OK;
}

int devhost_trace_print_ldk_event(unsigned int tp_index, void *entry, char *event_str)
{
	BUG_ON(tracefs_ops == NULL);

	int rc;

	rc = tracefs_ops->print_ldk_event(tp_index, entry, event_str);
	if (rc != 0) {
		return posix2hmerrno(-rc);
	}
	return E_HM_OK;
}

PUBLIC_SYMBOL
int devhost_tracefs_ops_init(struct devhost_tracefs_ops *ops)
{
	BUG_ON(ops == NULL);

	tracefs_ops = ops;
	return 0;
}
EXPORT_SYMBOL(devhost_tracefs_ops_init);
#ifdef PAC_FWCFI_SYSSERVICE
extern void (*const __tracefs_ro_after_init_hdlr_start []) (void);
extern void (*const __tracefs_ro_after_init_hdlr_end []) (void);
#endif

PUBLIC_SYMBOL
int dh_extension_init(struct lnxbase_ctx *ctx)
{
	int ret;
	const struct lnxbase_syminfo tracefs_si = __LIBLINUX_SYMINFO_INITIALIZER;

#ifdef PAC_FWCFI_SYSSERVICE
	ext_hdlr_init((uintptr_t)&__tracefs_ro_after_init_hdlr_start, (uintptr_t)(&__tracefs_ro_after_init_hdlr_end));
#endif
	ret = lnxbase_load_symbols(ctx, "tracefs_ext", &tracefs_si);
	if (ret < 0) {
		dh_error("dh_tracefs: load extension symbols, %s\n", strerror(ret));
		return ret;
	}

	ret = libdh_dso_plugin_init(__DH_PLUGIN_NAME__());
	if (ret < 0) {
		dh_error("dh_tracefs: init plugin failed, %s\n", hmstrerror(ret));
		return -hmerrno2posix(ret);
	}
	return 0;
}
