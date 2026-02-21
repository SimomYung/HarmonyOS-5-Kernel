/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Add kbox Extension
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jul 26 16:52:38 2021
 */
#include <lnxbase/ksymtab.h>
#include <lnxbase/lnxbase.h>

#include <hmkernel/compiler.h>

__attribute__((visibility ("default")))
int dh_extension_init(struct lnxbase_ctx *ctx)
{
	const struct lnxbase_syminfo dfx_si = __LIBLINUX_SYMINFO_INITIALIZER;
	return lnxbase_load_symbols(ctx, "dso_dfx", &dfx_si);
}
