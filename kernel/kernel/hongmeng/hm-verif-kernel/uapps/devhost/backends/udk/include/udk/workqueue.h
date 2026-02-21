/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Work queue routines of UDK
 * Author: Huawei OS Kernel Lab
 * Create: Thu Sep 05 15:47:08 2019
 */
#ifndef __UDK_WORKQUEUE_H__
#define __UDK_WORKQUEUE_H__

/* Workqueue */
#include <udk/lib/dlist.h>
#include <udk/thread_cond.h>
struct udk_work;
typedef int (*udk_work_handler)(struct udk_work *);

/* work priority */
#define RAW_WORK_PRIO_MAX 15
#define RAW_WORK_PRIO_MIN 0
#define RAW_WORK_PRIO_DEFAULT ((RAW_WORK_PRIO_MAX + RAW_WORK_PRIO_MIN) / 2)

/* default work attr */
#define UDK_WORK_PRIO_DEFAULT 15
#define UDK_WORK_NUM_DEFAULT  1
#define UDK_THREADPOOL_NAME_LEN 16

/* udk_workqueue is the same as raw_workqueue, udk_workqueue defined incompletely */
struct udk_workqueue;
struct udk_work {
	struct {
		unsigned int canceled   : 1; /* 1 means canceled */
		unsigned int priority   : 4; /* priority: 0-15 */
		unsigned int reserved   : 27;
	} u;

	struct dlist_node node;
	udk_work_handler wrapper_func; /* for raw_work_handler */
	struct udk_workqueue *wq;
	/* fields above should be the same as raw_work */
	udk_work_handler func;
};

struct udk_workqueue_attr {
	unsigned int nr_workers;
	unsigned int nr_queues;
	int inherit;
	int sched_policy;
	int thread_priority;
	struct udk_thread_cond_attr cond_attr;
	char name[UDK_THREADPOOL_NAME_LEN];
};

int udk_work_init(struct udk_work *work, udk_work_handler func, unsigned int prio);
void udk_work_destroy(struct udk_work *work);
struct udk_workqueue *udk_workqueue_alloc(const struct udk_workqueue_attr *attr);
int udk_workqueue_init(struct udk_workqueue *queue, const struct udk_workqueue_attr *attr);
int udk_workqueue_reset(struct udk_workqueue *queue);
int udk_workqueue_flush(struct udk_workqueue *queue);
int udk_workqueue_destroy(struct udk_workqueue *queue);
int udk_workqueue_add_work(struct udk_workqueue *queue, struct udk_work *work);
int udk_workqueue_cancel_work(struct udk_workqueue *wq, struct udk_work *work);

int udk_workqueue_create(struct udk_workqueue *workqueue, unsigned int nr_workers, unsigned int nr_queues);
struct udk_work *udk_create_work(unsigned int prio,
				 int (*handle_work)(struct udk_work *));

int udk_workqueue_add_work_default(struct udk_work *work);
int udk_workqueue_cancel_work_default(struct udk_work *work);

int udk_workqueue_flush_util(struct udk_workqueue *wq, struct udk_work *work);

/* udk delayed work */
#include <udk/timer.h>
struct udk_delayed_work {
	struct udk_work work;
	struct udk_timer timer;
	struct udk_workqueue *expected_queue;
};
int udk_delayed_work_init(struct udk_delayed_work *dwork, udk_work_handler func, unsigned int prio);
void udk_delayed_work_destroy(struct udk_delayed_work *work);
int udk_workqueue_add_delayed_work(struct udk_workqueue *queue, struct udk_delayed_work *work,
				   unsigned long delay_ms);
int udk_workqueue_cancel_delayed_work(struct udk_workqueue *wq, struct udk_delayed_work *work);
#endif /* __UDK_WORKQUEUE_H__ */
