/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Driver module loader support
 * Author: Huawei OS Kernel Lab
 * Create: Mon Sep 09 17:05:15 2019
 */
#include "devhost.h"
#include <devhost/log.h>

#include <unistd.h>
#include <lib/dlist.h>
#include <devhost/plugin/advice_ep.h>
#include <hongmeng/errno.h>
#include <libsysif/devmgr/api.h>
#include <libsysif/sysmgr/api.h>
#include <libhmsync/raw_mutex.h>
#include <libdevhost/uobject.h>
#include <libhwsecurec/securec.h>

#include "types.h"
#include "devhost_api.h"

static DLIST_HEAD(g_postinit_hooks);
static struct raw_mutex g_postinit_lock = RAW_MUTEX_INITIALIZER;
struct postinit_hook {
	struct dlist_node node;
	int (*function)(void *arg);
	void *arg;
};

int devhost_postinit(int (*function)(void *arg), void *arg)
{
	struct postinit_hook *hook = NULL;

	if (function == NULL) {
		return E_HM_INVAL;
	}

	/* alloc hook, will be release after hook is called */
	hook = (struct postinit_hook *)malloc(sizeof(*hook));
	if (hook == NULL) {
		return E_HM_NOMEM;
	}
	dlist_init(&hook->node);
	hook->function = function;
	hook->arg = arg;

	/* add to postinit hooks */
	raw_mutex_lock(&g_postinit_lock);
	dlist_insert_tail(&g_postinit_hooks, &hook->node);
	raw_mutex_unlock(&g_postinit_lock);

	return 0;
}

DEFINE_ADVICE_EP_PROTOTYPE(devhost_backend_init_done);
int devhost_do_postinit(void)
{
	struct postinit_hook *hook = NULL;
	int ret = 0;

	raw_mutex_lock(&g_postinit_lock);
	while (!dlist_empty(&g_postinit_hooks)) {
		hook = dlist_first_entry(&g_postinit_hooks, struct postinit_hook, node);
		dlist_delete(&hook->node);
		raw_mutex_unlock(&g_postinit_lock);

		ret = (*hook->function)(hook->arg);
		free(hook);
		if (ret != 0) {
			return ret;
		}

		raw_mutex_lock(&g_postinit_lock);
	}
	raw_mutex_unlock(&g_postinit_lock);

	DECLARE_ADVICE_EP(devhost_backend_init_done, ret, NULL);
	if (ret != 0) {
		dh_error("devhost backend init done ep failed, %d\n", ret);
		return ret;
	}

	return actvxactcapcall_drvcall_devmgr_core_devhost_init_done(false, true,
								     libdh_get_devmgr_ap());
}
