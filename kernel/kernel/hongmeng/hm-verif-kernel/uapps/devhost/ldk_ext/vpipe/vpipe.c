/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Add Vpipe Extension
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jul 26 16:52:38 2021
 */
#include <lnxbase/ksymtab.h>
#include <lnxbase/lnxbase.h>

#include <hmkernel/compiler.h>
#ifdef PAC_FWCFI_SYSSERVICE
#include <lib/ext_pac.h>
#endif

#ifdef PAC_FWCFI_SYSSERVICE
extern void (*const __vpipe_ro_after_init_hdlr_start []) (void);
extern void (*const __vpipe_ro_after_init_hdlr_end []) (void);
#endif

__attribute__((visibility ("default")))
int dh_extension_init(struct lnxbase_ctx *ctx)
{
	const struct lnxbase_syminfo vpipe_si = __LIBLINUX_SYMINFO_INITIALIZER;
#ifdef PAC_FWCFI_SYSSERVICE
	ext_hdlr_init((uintptr_t)&__vpipe_ro_after_init_hdlr_start, (uintptr_t)(&__vpipe_ro_after_init_hdlr_end));
#endif
	return lnxbase_load_symbols(ctx, "dso_vpipe", &vpipe_si);
}
