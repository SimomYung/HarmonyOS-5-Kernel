/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: UDK fops core implementation
 * Author: Huawei OS Kernel Lab
 * Create: Thur May 25 15:50:54 2023
 */

#include <hongmeng/errno.h>
#include <libalgo/idr.h>
#include <libhmsync/raw_mutex.h>
#include <libhmsync/raw_rwlock_guard.h>
#include <libhwsecurec/securec.h>

#include <devhost/file.h>
#include <udk/log.h>

#include "device.h"
#include "file.h"

DEFINE_MCACHE(block_ctx, 40, sizeof(struct block_ctx))

int udk_fusion_filp_node_free(struct filp_node *node)
{
	int ret = 0;
	struct block_ctx *blk_ctx = NULL;

	BUG_ON((node == NULL) || (node->devinfo == NULL));

	if ((node->devinfo->p->fops != NULL) && (node->devinfo->p->fops->close != NULL)) {
		ret = node->devinfo->p->fops->close(node->filp);
		if (ret < 0) {
			udk_warn("fops->close err=%s\n", strerror(ret));
		}
	}

	if ((node->devinfo->p->blkops != NULL) && (node->blk_ctx != NULL)) {
		blk_ctx = node->blk_ctx;
		(void)hm_ucap_reject(0, blk_ctx->rref_notify);
		mcache_block_ctx_free(blk_ctx);
		node->blk_ctx = NULL;
	}
	udk_fusion_devinfo_put(node->devinfo);
	free(node);

	return 0;
}

struct filp_node *udk_fusion_filp_node_alloc(__u32 cnode_idx, struct device_info *info)
{
	struct filp_node *node = NULL;

	if (info == NULL) {
		return NULL;
	}

	node = (struct filp_node *)malloc(sizeof(*node));
	if (node == NULL) {
		return NULL;
	}

	mem_zero_s(*node);
	raw_mutex_init(&node->ctx_lock);
	node->devinfo = info;
	node->cnode_idx = cnode_idx;
	(void)udk_fusion_devinfo_get(info);
	filp_poll_list_init(node);

	return node;
}
