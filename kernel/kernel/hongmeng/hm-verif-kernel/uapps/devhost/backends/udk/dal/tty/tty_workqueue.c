/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: TTY utils for async workqueue
 * Author: Huawei OS Kernel Lab
 * Create: Sun Sep 27 18:54:50 2020
 */

#include <libsched/priority.h>
#include <udk/workqueue.h>

#include "tty_wrap.h"
#include "tty_workqueue.h"

#define TTY_WQ_TNAME		"tty-wq"
/* define 1 worker and 1 queue to process tty works */
#define TTY_WORK_NUM_DEFAULT	1
#define TTY_WORK_PRIO_NUM	1
/* define high worker priority */
#define TTY_PRIORITY_DEFAULT	HM_PRIORITY_USER_MAX

static struct udk_workqueue *g_tty_wq = NULL;

struct tty_async_work {
	struct udk_work work;
	tty_async_func_t func;
	void *data;
};

static int tty_workqueue_attr_init(struct udk_workqueue_attr *attr)
{
	int ret;

	mem_zero_s(*attr);
	attr->nr_workers = TTY_WORK_NUM_DEFAULT;
	attr->nr_queues = TTY_WORK_PRIO_NUM;
	attr->thread_priority = (int)TTY_PRIORITY_DEFAULT;

	ret = snprintf_s(attr->name, UDK_THREADPOOL_NAME_LEN,
			 UDK_THREADPOOL_NAME_LEN - 1, "%s", TTY_WQ_TNAME);
	if (ret < 0) {
		tty_warn("tty: set workqueue attr name failed\n");
		return -EINVAL;
	}
	return 0;
}

int tty_workq_init(void)
{
	int ret;
	struct udk_workqueue_attr attr;

	ret = tty_workqueue_attr_init(&attr);
	if (ret < 0) {
		tty_warn("tty: init workqueue attr failed\n");
		return ret;
	}

	g_tty_wq = udk_workqueue_alloc(&attr);
	if (g_tty_wq == NULL) {
		tty_err("tty: failed to alloc workqueue\n");
		return -ENOMEM;
	}

	return 0;
}

void tty_workq_exit(void)
{
	if (g_tty_wq != NULL) {
		(void)udk_workqueue_destroy(g_tty_wq);
		g_tty_wq = NULL;
	}
}

static int __handle_async_work(struct udk_work *work)
{
	struct tty_async_work *awork = NULL;

	if (work == NULL) {
		return -EINVAL;
	}
	awork = container_of(work, struct tty_async_work, work);

	if (awork->func == NULL) {
		return -EINVAL;
	}
	(*awork->func)(awork->data);

	udk_work_destroy(&awork->work);
	tty_mem_free(awork);

	return 0;
}

void tty_async_schedule(tty_async_func_t func, void *data)
{
	struct tty_async_work *awork = NULL;
	int ret;

	if (func == NULL) {
		tty_warn("tty: invalid async work func\n");
		return;
	}

	awork = (struct tty_async_work *)tty_mem_zalloc(sizeof(*awork));
	if (awork == NULL) {
		tty_warn("tty: failed to alloc async work\n");
		func(data);
		return;
	}

	(void)udk_work_init(&awork->work, __handle_async_work, 0);
	awork->func = func;
	awork->data = data;

	ret = udk_workqueue_add_work(g_tty_wq,
				     &awork->work);
	if (ret < 0) {
		tty_warn("tty: add work to workqueue failed, ret: %s\n", strerror(-ret));
		udk_work_destroy(&awork->work);
		tty_mem_free(awork);
		func(data);
	}

	return;
}
tty_export_symbol(tty_async_schedule);
