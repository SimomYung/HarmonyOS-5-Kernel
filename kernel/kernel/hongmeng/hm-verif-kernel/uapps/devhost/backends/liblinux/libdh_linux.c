/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Entry point of liblinux backend
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jul 10 16:58:35 2019
 */
#include "libdh_linux.h"

#include <hongmeng/errno.h>
#include <libstrict/strict.h>
#include <libhwsecurec/securec.h>

#include <devhost/log.h>
#include <devhost/security.h>
#include <lnxbase/lnxbase.h>
#include <lnxbase/helper.h>

/* liblinux post-init hook */
static int liblinux_postinit(void *arg)
{
	UNUSED(arg);

	/* do initcalls */
	liblinux_do_initcalls();

	return E_HM_OK;
}

static struct lnxbase_ctx *g_ctx = NULL;

int dh_post_dev_populate_call(void)
{
	return lnxbase_root_notify(g_ctx);
}

/* 4096 is enough for both aarch64 and arm32
 * on arm32, the command line will be truncated to 1024 by linux automatically */
#define COMMAND_LINE_SIZE 4096
char boot_command_line[COMMAND_LINE_SIZE];

int dh_backend_init(struct dh_backend_ctor *ctor, const char *backend_cmdline)
{
	int ret;

	dh_info("initializing linux subsystem ...\n");

	liblinux_devices_autoprobe = 0; /* disable device autoprobe */
	liblinux_drivers_autoprobe = 1; /* enable driver autoprobe */

	if (backend_cmdline != NULL) {
		ret = strcpy_s(boot_command_line, COMMAND_LINE_SIZE, backend_cmdline);
		if (ret != 0) {
			dh_error("boot command line is too long: %s\n", backend_cmdline);
			return -EINVAL;
		}
	}
	liblinux_kernel_init();

#ifndef CONFIG_DEVHOST_LIBLINUX_5_10_M536A
	lnxbase_register_reclaim_func(&liblinux_drop_slab, "liblinux_drop_slab");
#endif

	ret = lnxbase_helper_simple_init(ctor, liblinux_postinit,
					 (void *)devhost_get_backend_args(),
					 &g_ctx);
	if (ret < 0) {
		return ret;
	}

	ret = lnxbase_enable_post_dev_populate(g_ctx, dh_extralib_init);
	if (ret < 0) {
		lnxbase_destroy(g_ctx);
		g_ctx = NULL;
		return ret;
	}

	return 0;
}
