/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: interface for network dispatch
 * Author: Huawei OS Kernel Lab
 * Create: Fri Oct 11 11:12:02 2019
 */

#include <hongmeng/syscall.h>
#include <libhmlog/hmlog.h>
#include <libhmucap/ucap.h>
#include <libhwsecurec/securec.h>
#include <libhmsync/raw_thread.h>
#include <libhmsync/raw_workqueue.h>
#include "ux_workqueue.h"
#include "ux_netlog.h"

struct net_work {
	struct raw_work work;
	net_work_handler func;
	void *arg;
};

static struct raw_workqueue queue;

static struct raw_workqueue *net_get_workqueue(void)
{
	return &queue;
}

static int __handle_async_work(struct raw_work *work)
{
	struct net_work *mywork = container_of(work, struct net_work, work);

	(*mywork->func)(mywork->arg);

	raw_work_destroy(&mywork->work);
	free(mywork);

	return 0;
}

int net_add_async_work(net_work_handler func, void *data)
{
	struct net_work *mywork = NULL;
	int err;

	if (func == NULL) {
		return E_HM_INVAL;
	}

	mywork = malloc(sizeof(struct net_work));
	if (mywork == NULL) {
		net_error(HMNET_CORE, "malloc net_work failed\n");
		return E_HM_NOMEM;
	}

	mywork->func = func;
	mywork->arg = data;

	err = raw_work_init(&mywork->work, __handle_async_work, RAW_WORK_PRIO_MAX);
	if (err != E_HM_OK) {
		net_error(HMNET_CORE, "raw_work_init failed\n");
		goto err_out;
	}

	err = raw_workqueue_add_work(net_get_workqueue(), &mywork->work);
	if (err != E_HM_OK) {
		net_error(HMNET_CORE, "raw_workqueue_add_work failed\n");
		goto err_out;
	}

	return err;

err_out:
	free(mywork);
	mywork = NULL;
	return err;
}

static int __attribute__((constructor)) net_init_workqueue(void)
{
	struct raw_workqueue_attr attr;
	int err;

	bzero(&attr, sizeof(attr));
	raw_workqueue_attr_init(&attr, 1, 1);
	NOFAIL(raw_workqueue_attr_set_name(&attr, "net-wq"));
	err = raw_workqueue_init(&queue, &attr);
	if (err != E_HM_OK) {
		net_error(HMNET_CORE, "raw work queue init failed: %s\n", hmstrerror(err));
		return err;
	}

	return E_HM_OK;
}
