/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Define interface of ext_main
 * Author: Huawei OS Kernel Lab
 * Create: Tue May 23 16:31:54 CST 2023
 */

#include <sys/types.h>
#include <devhost/backend.h>
#include <devhost/plugin.h>
#include <lnxbase/lnxbase.h>
#include <lnxbase/export.h>
#include <lnxbase/ksymtab.h>
#include <libdevhost/devhost.h>

#include "kstate_api.h"
#include "kstate.h"
#ifdef PAC_FWCFI_SYSSERVICE
#include <lib/ext_pac.h>
#endif

#ifdef PAC_FWCFI_SYSSERVICE
extern void (*const __kstate_ro_after_init_hdlr_start []) (void);
extern void (*const __kstate_ro_after_init_hdlr_end []) (void);
#endif

PUBLIC_SYMBOL
int dh_extension_init(struct lnxbase_ctx *ctx)
{
	int ret;
	const struct lnxbase_syminfo kstate_si = __LIBLINUX_SYMINFO_INITIALIZER;

#ifdef PAC_FWCFI_SYSSERVICE
	ext_hdlr_init((uintptr_t)&__kstate_ro_after_init_hdlr_start, (uintptr_t)(&__kstate_ro_after_init_hdlr_end));
#endif
	ret = lnxbase_load_symbols(ctx, "kstate_ext", &kstate_si);
	if (ret < 0) {
		hm_error("dh_kstate: load extension symbols, %s\n", strerror(ret));
		return ret;
	}

	ret = libdh_dso_plugin_init(__DH_PLUGIN_NAME__());
	if (ret < 0) {
		hm_error("dh_kstate: init plugin failed, %s\n", hmstrerror(ret));
		return -hmerrno2posix(ret);
	}

	return 0;
}
