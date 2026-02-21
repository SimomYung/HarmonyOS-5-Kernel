/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Override mm interfaces
 * Author: Huawei OS Kernel Lab
 * Create: Thur Feb 4 15:50:54 2023
 */
#include <dlfcn.h>
#include <unistd.h>
#include <sys/types.h>
#include <libmem/types.h>
#include <hongmeng/errno.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhmsrv_sys/hm_ashm.h>
#include <libpreempt/preempt.h>

#include <devhost/backend.h>

#include <udk/log.h>

struct devhost_umap_ops *g_udk_umap_ops;

int devhost_backend_register_umap_ops(struct devhost_umap_ops *ops)
{
	if (g_udk_umap_ops != NULL) {
		return -EEXIST;
	}
	g_udk_umap_ops = ops;

	return 0;
}

int devhost_backend_umap_munmap(void *data, int vs_is_releasing)
{
	int ret;
	struct devhost_umap_ops *ops = g_udk_umap_ops;

	if (ops == NULL || ops->munmap == NULL) {
		/* it is normal for devhost umap ops to be null */
		return 0;
	}

	ret = ops->munmap(data, vs_is_releasing);

	return ret;
}
