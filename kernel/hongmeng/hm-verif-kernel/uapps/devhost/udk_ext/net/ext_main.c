/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Net socket extension entry
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jun 11:20:49 2022
 */
#include <hmnet/hm_socket.h>

#include <hmkernel/errno.h>
#include <devhost/plugin.h>

int hmnetd_fusion_entry(int argc, char **argv, fusion_ops_t *ops);
int dh_udk_extension_init(void);

PUBLIC_SYMBOL
int dh_udk_extension_init(void)
{
	hm_info("start hmnetd in fusion mode\n");

	int ret = hmnetd_fusion_entry(0, NULL, NULL);
	if (ret < 0) {
		hm_error("start hmnetd in fusion mode failed, %d\n", ret);
		return -hmerrno2posix(ret);
	}

	return 0;
}
