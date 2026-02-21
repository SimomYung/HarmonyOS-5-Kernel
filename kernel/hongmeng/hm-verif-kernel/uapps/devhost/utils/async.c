/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Utils for async workqeue
 * Author: Huawei OS Kernel Lab
 * Create: Tue Dec 17 15:01:02 2019
 */
#include "utils.h"

#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>
#include <libhmsync/raw_workqueue.h>
#include <libhmsrv_sys/hm_procmgr.h> /* for 'hm_getpid()' */

#include "devhost.h"
#include <devhost/log.h>
#ifdef CONFIG_NET_FUSION
#include <devhost/workqueue.h>
#endif

#define DEVHOST_WQ_THREAD_NR	1
/* Reserve 2 bytes for adding serial number for workq thread name */
#define DH_WQ_TNAME_MAX_LEN	(RAW_THREADPOOL_NAME_LEN - 2)

static struct raw_workqueue g_default_wq;

#ifdef CONFIG_NET_FUSION
struct raw_workqueue *devhost_get_actvpool_wq(void)
{
	return &g_default_wq;
}
#endif

int devhost_workq_attr_set_name(struct raw_workqueue_attr *attr, const char *name)
{
	int ret = E_HM_OK;
	int hm_pid;
	char new_name[DH_WQ_TNAME_MAX_LEN] = {0};

	hm_pid = hm_getpid();
	if (hm_pid < 0) {
		dh_debug("get pid failed, err=%d\n", hm_pid);
		return hm_pid;
	}

	ret = snprintf_s(new_name, DH_WQ_TNAME_MAX_LEN,
			 DH_WQ_TNAME_MAX_LEN - 1, "%s/%d",
			 name, hm_pid);
	if (ret < 0) {
		return E_HM_INVAL;
	}

	ret = raw_workqueue_attr_set_name(attr, new_name);
	if (ret < 0) {
		return ret;
	}

	return E_HM_OK;
}

int devhost_workq_init(void)
{
	int ret = E_HM_OK;
	struct raw_workqueue_attr attr;
	raw_workqueue_attr_init(&attr, DEVHOST_WQ_THREAD_NR, 1); /* nr_queues = 1 */

	ret = devhost_workq_attr_set_name(&attr, DH_DEFAULT_WORKQ_TNAME);
	if (ret < 0) {
		dh_warn("set default workq thread name failed, ret=%s\n",
			hmstrerror(ret));
	}

	return raw_workqueue_init(&g_default_wq, &attr);
}

struct async_work {
	struct raw_work work;
	devhost_async_func_t func;
	void *data;
};

static int __handle_async_work(struct raw_work *work)
{
	struct async_work *awork = container_of(work, struct async_work, work);

	(*awork->func)(awork->data);

	raw_work_destroy(&awork->work);
	free(awork);

	return 0;
}

void devhost_async_schedule(devhost_async_func_t func, void *data)
{
	struct async_work *awork = NULL;
	int ret = E_HM_OK;

	BUG_ON(func == NULL);

	awork = (struct async_work *)malloc(sizeof(*awork));
	if (awork == NULL) {
		goto try_sync_call;
	}

	(void)raw_work_init(&awork->work, __handle_async_work, 0);
	awork->func = func;
	awork->data = data;

	ret = raw_workqueue_add_work(&g_default_wq, &awork->work);
	if (ret < 0) {
		raw_work_destroy(&awork->work);
		free(awork);
		goto try_sync_call;
	}

	return;

try_sync_call:
	func(data);
}
