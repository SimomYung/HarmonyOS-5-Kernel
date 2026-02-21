/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Probe extension entry
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jan 16 17:26:46 2023
 */

#include <sys/types.h>
#include <devhost/plugin.h>
#include <lnxbase/lnxbase.h>
#include <liblinux/pal.h>
#include <lnxbase/export.h>
#include <lnxbase/ksymtab.h>
#ifdef PAC_FWCFI_SYSSERVICE
#include <lib/ext_pac.h>
#endif

#ifdef PAC_FWCFI_SYSSERVICE
extern void (*const __probe_ro_after_init_hdlr_start []) (void);
extern void (*const __probe_ro_after_init_hdlr_end []) (void);
#endif

PUBLIC_SYMBOL
int dh_extension_init(struct lnxbase_ctx *ctx)
{
	int ret;
	const struct lnxbase_syminfo probe_si = __LIBLINUX_SYMINFO_INITIALIZER;

#ifdef PAC_FWCFI_SYSSERVICE
	ext_hdlr_init((uintptr_t)&__probe_ro_after_init_hdlr_start, (uintptr_t)(&__probe_ro_after_init_hdlr_end));
#endif
	ret = lnxbase_load_symbols(ctx, "probe_ext", &probe_si);
	if (ret < 0) {
		hm_error("load devhost_probe extension symbols, %d\n", ret);
		return ret;
	}

	ret = libdh_dso_plugin_init(__DH_PLUGIN_NAME__());
	if (ret < 0) {
		hm_error("init devhost_probe plugin failed, %s\n", hmstrerror(ret));
		return -hmerrno2posix(ret);
	}

	return 0;
}
