/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Work queue routines of UDK
 * Author: Huawei OS Kernel Lab
 * Create: Thu Sep 05 19:05:13 2019
 */
#include <udk/workqueue.h>

#include <udk/log.h>
#include <udk/timer.h>
#include <udk/compiler.h>
#include <internal/init.h>
#include <internal/workqueue.h>
#include <libhmsync/raw_workqueue.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhwsecurec/securec.h>

#define UDK_DEFAULT_WQ_PREFIX	"udk-wq-comm"

raw_static_assert((sizeof(struct udk_work) - sizeof(udk_work_handler)) == sizeof(struct raw_work),
		  udk_work_size_is_wrong);
raw_static_assert(sizeof(struct udk_workqueue_attr) == sizeof(struct raw_workqueue_attr),
		  udk_workqueue_attr_size_is_wrong);

static struct raw_workqueue udk_default_wq;

static int work_wrapper(struct raw_work *arg)
{
	struct udk_work *work = (struct udk_work *)arg;
	return work->func(work);
}

int udk_work_init(struct udk_work *work, udk_work_handler func, unsigned int prio)
{
	work->func = func;
	return -hmerrno2posix(raw_work_init((struct raw_work *)(uintptr_t)work, work_wrapper, prio));
}

void udk_work_destroy(struct udk_work *work)
{
	raw_work_destroy((struct raw_work *)(uintptr_t)work);
}

struct udk_workqueue *udk_workqueue_alloc(const struct udk_workqueue_attr *attr)
{
	struct udk_workqueue *wq = NULL;
	int ret;

	wq = (struct udk_workqueue *)malloc(sizeof(struct raw_workqueue));
	if (wq == NULL) {
		udk_error("udk workqueue alloc failed\n");
		return NULL;
	}

	ret = raw_workqueue_init((struct raw_workqueue *)(uintptr_t)wq,
				 (struct raw_workqueue_attr *)(uintptr_t)attr);
	if (ret != 0) {
		udk_error("udk workqueue init failed\n");
		free(wq);
		return NULL;
	}
	return wq;
}

int udk_workqueue_init(struct udk_workqueue *wq, const struct udk_workqueue_attr *a)
{
	return -hmerrno2posix(raw_workqueue_init((struct raw_workqueue *)(uintptr_t)wq,
						 (struct raw_workqueue_attr *)(uintptr_t)a));
}

int udk_workqueue_reset(struct udk_workqueue *wq)
{
	return -hmerrno2posix(raw_workqueue_destroy((struct raw_workqueue *)(uintptr_t)wq));
}

int udk_workqueue_flush(struct udk_workqueue *wq)
{
	return -hmerrno2posix(raw_workqueue_flush((struct raw_workqueue *)(uintptr_t)wq));
}

int udk_workqueue_destroy(struct udk_workqueue *wq)
{
	int ret;

	if (wq == NULL) {
		udk_error("udk workqueue is NULL\n");
		return -EINVAL;
	}

	ret = raw_workqueue_destroy((struct raw_workqueue *)(uintptr_t)wq);
	if (ret != 0) {
		free(wq);
		udk_error("udk workqueue destroy failed: %s\n", hmstrerror(ret));
		return -hmerrno2posix(ret);
	}
	free(wq);
	return 0;
}

int udk_workqueue_create(struct udk_workqueue *workqueue,
			 unsigned int nr_workers,
			 unsigned int nr_queues)
{
	int ret;
	struct raw_workqueue_attr attr;

	raw_workqueue_attr_init(&attr, nr_workers, nr_queues);
	raw_workqueue_attr_set_name(&attr, UDK_DEFAULT_WQ_PREFIX);
	ret = raw_workqueue_init((struct raw_workqueue *)(uintptr_t)workqueue, &attr);
	if (ret != 0) {
		udk_error("raw work queue init failed: %s\n", hmstrerror(ret));
	}
	return -hmerrno2posix(ret);
}

struct udk_work *udk_create_work(unsigned int prio,
				 int (*handle_work)(struct udk_work *))
{
	struct udk_work *work = NULL;
	int ret;

	work = malloc(sizeof(struct udk_work));
	if (work == NULL) {
		return NULL;
	}

	work->func = handle_work;
	ret = raw_work_init((struct raw_work *)(uintptr_t)work, work_wrapper, prio);
	if (ret != 0) {
		free(work);
		return NULL;
	}

	return work;
}

int udk_workqueue_add_work(struct udk_workqueue *wq, struct udk_work *work)
{
	return -hmerrno2posix(raw_workqueue_add_work((struct raw_workqueue *)(uintptr_t)wq,
						     (struct raw_work *)(uintptr_t)work));
}

int udk_workqueue_add_work_default(struct udk_work *work)
{
	return udk_workqueue_add_work((struct udk_workqueue *)&udk_default_wq, work);
}

int udk_workqueue_cancel_work(struct udk_workqueue *wq, struct udk_work *work)
{
	return -hmerrno2posix(raw_workqueue_cancel_work((struct raw_workqueue *)(uintptr_t)wq,
							(struct raw_work *)(uintptr_t)work));
}

int udk_workqueue_cancel_work_default(struct udk_work *work)
{
	return udk_workqueue_cancel_work((struct udk_workqueue *)&udk_default_wq, work);
}

int udk_workqueue_flush_util(struct udk_workqueue *wq, struct udk_work *work)
{
	return -hmerrno2posix(raw_workqueue_flush_util((struct raw_workqueue *)(uintptr_t)wq,
							(struct raw_work *)(uintptr_t)work));
}

static int __udk_workqueue_default_init(void)
{
	int ret;

	ret = udk_workqueue_create((struct udk_workqueue *)&udk_default_wq,
				   UDK_WORK_NUM_DEFAULT, UDK_WORK_PRIO_DEFAULT);
	if (ret < 0) {
		udk_error("udk_workqueue_init_default failed: %s\n",
			  hmstrerror(ret));
	}

	return ret;
}

int udk_workqueue_default_init(struct udk_workqueue **wq)
{
	int ret;

	ret = __udk_workqueue_default_init();
	if (ret == 0) {
		*wq = (struct udk_workqueue *)&udk_default_wq;
	}

	return ret;
}

/* delayed work */
static void timer_handler(unsigned long data)
{
	int ret;
	struct udk_delayed_work *dwork = (struct udk_delayed_work *)data;

	if (dwork == NULL)
		return;

	ret = udk_workqueue_add_work(dwork->expected_queue, &dwork->work);
	if (ret < 0) {
		udk_warn("add to default workqueue failed, %s\n", strerror(-ret));
		return;
	}
	ret = udk_timer_del(&dwork->timer);
	if (ret < 0) {
		udk_warn("delete timer failed, %s\n", strerror(-ret));
		return;
	}
}

int udk_delayed_work_init(struct udk_delayed_work *dwork, udk_work_handler func, unsigned int prio)
{
	int ret;

	if ((dwork == NULL) || (func == NULL))
		return -EINVAL;

	ret = udk_work_init(&dwork->work, func, prio);
	if (ret < 0) {
		udk_error("udk delayed work init failed, %s\n", strerror(-ret));
		return -hmerrno2posix(ret);
	}
	udk_timer_init(&dwork->timer, timer_handler,
		       (unsigned long)(uintptr_t)dwork, ULONG_MAX);

	return 0;
}

void udk_delayed_work_destroy(struct udk_delayed_work *dwork)
{
	if (dwork == NULL)
		return;

	(void)udk_timer_del(&dwork->timer);
	udk_work_destroy(&dwork->work);
}

int udk_workqueue_add_delayed_work(struct udk_workqueue *queue,
				   struct udk_delayed_work *dwork,
				   unsigned long delay_ms)
{
	int ret;

	if ((queue == NULL) || (dwork == NULL))
		return -EINVAL;

	dwork->expected_queue = queue;
	ret = udk_timer_mod(&dwork->timer, delay_ms);
	if (ret < 0) {
		udk_error("mod timer failed, %s\n", strerror(-ret));
		return ret;
	}
	return 0;
}

int udk_workqueue_cancel_delayed_work(struct udk_workqueue *wq, struct udk_delayed_work *dwork)
{
	int ret;

	if (wq == NULL || dwork == NULL)
		return -EINVAL;

	ret = udk_timer_del(&dwork->timer);
	if (ret < 0)
		return ret;

	return udk_workqueue_cancel_work(wq, &dwork->work);
}

udk_init_call(__udk_workqueue_default_init);
