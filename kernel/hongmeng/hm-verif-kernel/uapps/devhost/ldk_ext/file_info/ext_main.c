/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: File_info extension entry
 * Author: Huawei OS Kernel Lab
 * Create: Wed Sep 13 09:13:06 2023
 */

#include <sys/types.h>
#include <devhost/plugin.h>
#include <lnxbase/lnxbase.h>
#include <liblinux/pal.h>
#include <lnxbase/export.h>
#include <lnxbase/ksymtab.h>
#include "file_info_ext.h"

PUBLIC_SYMBOL
int dh_extension_init(struct lnxbase_ctx *ctx)
{
	int ret;
	const struct lnxbase_syminfo file_info_si = __LIBLINUX_SYMINFO_INITIALIZER;

	ret = lnxbase_load_symbols(ctx, "file_info_ext", &file_info_si);
	if (ret < 0) {
		hm_error("load file_info extension symbols, %d\n", ret);
		return ret;
	}

	ret = libdh_dso_plugin_init(__DH_PLUGIN_NAME__());
	if (ret < 0) {
		hm_error("init file_info plugin failed, %s\n", hmstrerror(ret));
		return -hmerrno2posix(ret);
	}

	ret = devhost_file_info_vfs_cnode_idx_init();
	if (ret != E_HM_OK) {
		return -hmerrno2posix(ret);
	}

	ret = devhost_vmalloc_info_create_file();
	if (ret < 0) {
		hm_error("fail to create vmalloc_info proc file, err=%d\n", ret);
		return  -hmerrno2posix(ret);
	}

	return 0;
}
