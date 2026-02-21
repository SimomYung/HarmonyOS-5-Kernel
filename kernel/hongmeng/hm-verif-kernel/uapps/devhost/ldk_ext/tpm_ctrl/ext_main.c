/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Tpmctrl extension entry
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jan 16 17:26:46 2023
 */

#include <sys/types.h>
#include <devhost/plugin.h>
#include <lnxbase/lnxbase.h>
#include <liblinux/pal.h>
#include <lnxbase/export.h>
#include <lnxbase/ksymtab.h>
#include "tpm_ctrl_if.h"

EXPORT_SYMBOL(devhost_register_tpm_ctrl);

PUBLIC_SYMBOL
int dh_extension_init(struct lnxbase_ctx *ctx)
{
	int ret;
	const struct lnxbase_syminfo tpm_ctrl_si = __LIBLINUX_SYMINFO_INITIALIZER;

	ret = lnxbase_load_symbols(ctx, "tpm_ctrl_ext", &tpm_ctrl_si);
	if (ret < 0) {
		hm_error("load tpm_ctrl extension symbols, %d\n", ret);
		return ret;
	}
	return 0;
}
