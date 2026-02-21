/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Devhost dso-extension demo
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jun 15 11:40:37 2021
 */

#include <hongmeng/errno.h>
#include <hongmeng/panic.h>

#include <devhost/plugin.h>
#include <libhmlog/hmlog.h>
#include <libhmactv/actv.h>
#include <libhmucap/ucap.h>

#include <lnxbase/ksymtab.h>
#include <lnxbase/lnxbase.h>
#ifdef PAC_FWCFI_SYSSERVICE
#include <lib/ext_pac.h>
#endif

static void dso_simple_ev_before(int test_val)
{
	hm_info("dso_simple before event happened with val %d\n", test_val);
}

static void dso_simple_ev_after(int test_val)
{
	hm_info("dso_simple after event happened with val %d\n", test_val);
}

DEFINE_EVENT_EXTENSION(dso_simple_ev_ext, dso_simple_ev_before, dso_simple_ev_after);

static int dso_simple_constructor(const struct libdh_dso_plugin *plugin)
{
	UNUSED(plugin);

	hm_info("init liblinux dso plugin success\n");
	return E_HM_OK;
}

DEFINE_DSO_PLUGIN(dso_plugin_simple, dso_simple_constructor,
	IMPORT_EVENT_EXTENSION(simple_event_test, dso_simple_ev_ext)
);
#ifdef PAC_FWCFI_SYSSERVICE
extern void (*const __demo_ro_after_init_hdlr_start []) (void);
extern void (*const __demo_ro_after_init_hdlr_end []) (void);
#endif

__attribute__((visibility ("default")))
int dh_extension_init(struct lnxbase_ctx *ctx)
{
	int ret;
	const struct lnxbase_syminfo simple_si = __LIBLINUX_SYMINFO_INITIALIZER;

#ifdef PAC_FWCFI_SYSSERVICE
	ext_hdlr_init((uintptr_t)&__demo_ro_after_init_hdlr_start, (uintptr_t)(&__demo_ro_after_init_hdlr_end));
#endif
	ret = libdh_dso_plugin_init(&__dso_plugin_simple.base);
	if (ret < 0) {
		hm_error("init dso plugin failed\n");
		return -hmerrno2posix(ret);
	}

	return lnxbase_load_symbols(ctx, "dso_plugin_simple", &simple_si);
}
