/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2024. All rights reserved.
 * Description: UDK block operations
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jun 03 15:04:16 2024
 */
#include <stdio.h>
#include <udk/log.h>
#include <udk/block/udk_block.h>
#include <udk/block/udk_disk_stats.h>
#include <udk/block/udk_partition.h>
#include "udk_block_internal.h"

#ifndef CONFIG_MAS_BLK
int hmfs_blk_busyidle_event_subscribe(struct hmsrv_io_ctx *ctx, char *name,
				int namelen, void *priv, void **res)
{
	UNUSED(ctx, name, namelen, priv, res);

	return E_HM_OPNOTSUPP;
}

int hmfs_blk_busyidle_event_unsubscribe(void *pri)
{
	UNUSED(pri);

	return E_HM_OPNOTSUPP;
}
#endif
