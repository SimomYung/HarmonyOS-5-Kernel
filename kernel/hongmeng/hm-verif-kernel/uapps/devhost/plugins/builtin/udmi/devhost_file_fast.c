/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Fast file table management
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jun 27 20:00:00 2023
 */

#include "types.h"
#include "devhost.h"
#include "devhost_api.h"
#include "devhost_kobj.h"
#include <devhost/file.h>
#ifdef CONFIG_SEHARMONY
#include <libsysif/devhost/server.h>
#include <devhost/security_seharmony.h>
#endif

static void devhost_release_filp_node_fast(struct raw_compounded_auto_refcnt *ref)
{
	int ret = E_HM_OK;
	struct filp_node *node = NULL;

	node = refcnt_to_filp_node(ref);
	BUG_ON(node == NULL || node->devinfo == NULL);

	devhost_poll_close(node);
	devhost_epoll_close2(node);

	if ((node->devinfo->p->fops != NULL) && (node->devinfo->p->fops->close != NULL)) {
		ret = node->devinfo->p->fops->close(node->filp);
		if (ret < 0) {
			hm_warn("fops->close failed, ret=%d\n", ret);
		}
	}

	/* cleanup ctrlmem kobj resources */
	if (node->kobj_ctx != NULL) {
		devhost_put_kobj(node->kobj_ctx);
		node->kobj_ctx = NULL;
	}

	devinfo_put(node->devinfo);
	free(node);
}

PUBLIC_SYMBOL
struct filp_node *devhost_alloc_filp_node_fast(struct device_info *devinfo)
{
	struct filp_node *node = NULL;

	if (devinfo == NULL) {
		return NULL;
	}

	node = (struct filp_node *)calloc(1, sizeof(*node));
	if (node == NULL) {
		return NULL;
	}

	node->devinfo = devinfo_get(devinfo);
	raw_compounded_auto_refcnt_init(&node->refcnt, 1U, devhost_release_filp_node_fast);
	filp_poll_list_init(node);
#ifdef CONFIG_SEHARMONY
	(void)devhost_seharmony_check(fops_open, ptr_to_u64(node));
#endif

	return node;
}
