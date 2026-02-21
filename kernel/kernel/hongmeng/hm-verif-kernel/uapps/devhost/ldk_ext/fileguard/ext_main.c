/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: Define interface of fileguard_ext
 * Author: Huawei OS Kernel Lab
 * Create: Sat Jun 08 11:54:30 2024
 */

#include <sys/types.h>
#include <devhost/backend.h>
#include <devhost/plugin.h>
#include <lnxbase/lnxbase.h>
#include <lnxbase/export.h>
#include <lnxbase/ksymtab.h>
#include <libdevhost/devhost.h>

#include "fileguard_api.h"
#include "fileguard_hdr.h"
#ifdef PAC_FWCFI_SYSSERVICE
#include <lib/ext_pac.h>
#endif

#ifdef PAC_FWCFI_SYSSERVICE
extern void (*const __fileguard_ro_after_init_hdlr_start []) (void);
extern void (*const __fileguard_ro_after_init_hdlr_end []) (void);
#endif

PUBLIC_SYMBOL
int dh_extension_init(struct lnxbase_ctx *ctx)
{
	int ret;
	const struct lnxbase_syminfo fileguard_si = __LIBLINUX_SYMINFO_INITIALIZER;

#ifdef PAC_FWCFI_SYSSERVICE
	ext_hdlr_init((uintptr_t)&__fileguard_ro_after_init_hdlr_start, (uintptr_t)(&__fileguard_ro_after_init_hdlr_end));
#endif
	ret = lnxbase_load_symbols(ctx, "fileguard_ext", &fileguard_si);
	if (ret < 0) {
		hm_error("dh_fileguard: load extension symbols, %s\n", strerror(ret));
		return ret;
	}

	ret = libdh_dso_plugin_init(__DH_PLUGIN_NAME__());
	if (ret < 0) {
		hm_error("dh_fileguard: init plugin failed, %s\n", hmstrerror(ret));
		return -hmerrno2posix(ret);
	}

	return 0;
}
