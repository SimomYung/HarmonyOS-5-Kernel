/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Interfaces of raw_workqueue
 * Author: Huawei OS Kernel Lab
 * Create: Thu Mar 7 15:11:53 2019
 */

#ifndef LIBHMSYNC_RAW_WORKQUEUE_H
#define LIBHMSYNC_RAW_WORKQUEUE_H

#include <libhmsync/raw_prioqueue.h>
#include <libhmsync/raw_threadpool.h>
#include <libhmsync/raw_thread_cond.h>

struct raw_work;
typedef int (*raw_work_handler)(struct raw_work *);

/* raw work priority */
#define RAW_WORK_PRIO_MAX 15
#define RAW_WORK_PRIO_MIN 0
#define RAW_WORK_PRIO_DEFAULT ((RAW_WORK_PRIO_MAX + RAW_WORK_PRIO_MIN) / 2)
#define RAW_WORK_PRIO_BIT 8U
#define RAW_WORK_PRIO_BIT_MASK ((1U << RAW_WORK_PRIO_BIT) - 1U)

struct raw_workqueue;
struct raw_work {
	struct {
		unsigned int canceled   : 1; /* 1 means canceled */
		unsigned int priority   : 4; /* priority: 0-15 */
		unsigned int reserved   : 27;
	} u;

	struct dlist_node node;
	raw_work_handler func;
	struct raw_workqueue *wq;
};

static inline int raw_work_is_canceled(struct raw_work *work)
{
	return work->u.canceled;
}

int raw_work_init(struct raw_work *work, raw_work_handler func, unsigned int prio);
void raw_work_destroy(struct raw_work *work);

enum {
	/* freeze during system suspend. If this flag is set, append
	 * COND_INTERRUPT_FLAG to wq->cond.attr, so that the freezer module can
	 * interupt woker which blocked at raw_thread_cond_wait in worker(),
	 * then try_freeze_worker to freeze the worker thread. */
	WQ_FREEZABLE_FLAG = 1U,
};

struct raw_workqueue_attr {
	/* initial number of worker threads */
	unsigned int nr_workers;

	/* number of queues, equals to number of priority, nr_queues should not
	 * bigger than RAW_WORK_PRIO_MAX(15) + 1, 1 byte is enough. So we reuse
	 * the upper 3 bytes to store more wq configuration like WQ_FREEZABLE_FLAG */
	unsigned int nr_queues;

	int inherit;
	int sched_policy;
	int thread_priority;
	struct raw_thread_cond_attr cond_attr;

	/* name of workqueue thread */
	char name[RAW_THREADPOOL_NAME_LEN];
};

static inline unsigned int raw_workqueue_attr_nr_queues_of(const struct raw_workqueue_attr *attr)
{
	return attr->nr_queues & RAW_WORK_PRIO_BIT_MASK;
}

static inline unsigned int raw_workqueue_attr_flags_of(const struct raw_workqueue_attr *attr)
{
	return attr->nr_queues >> RAW_WORK_PRIO_BIT;
}

struct raw_waiter_list {
	struct raw_mutex mutex;
	struct dlist_node head;
};

struct raw_workqueue {
	/*
	 * number of works, condvar will check this attribute.
	 * if nr_works==0, worker thread will begin to wait
	 *
	 * protected by cond_mutex
	 */
	int nr_works;
	int state;

	/* wq_mutex protects prioqueue */
	struct raw_mutex wq_mutex;
	struct raw_prioqueue wait_wq;
	struct raw_prioqueue handling_wq;

	/* init once created, don't need lock */
	unsigned int nr_workers;
	unsigned int nr_queues : 8;
	unsigned int flags : 24;

	/* waiter list for flush, protected by waiters.mutex */
	struct raw_waiter_list waiters;

	/*
	 * cond_mutex & condvar are used to do thread wait/notification:
	 * - when nr_works==0, worker thread begin to wait on the condvar.
	 * - when add work to wq, nr_work++, will signal a worker to wakeup to handler work.
	 */
	struct raw_mutex cond_mutex;
	struct raw_thread_cond cond;

	/* threadpool: init at workqueue init, don't lock */
	struct raw_threadpool *pool;
};

void raw_workqueue_thread_ctx_setup_register(void (*ctx_setup)(void));
int raw_workqueue_init(struct raw_workqueue *wq,
		       const struct raw_workqueue_attr *attr);
int raw_workqueue_destroy(struct raw_workqueue *wq);
int raw_workqueue_add_work(struct raw_workqueue *wq, struct raw_work *work);
int raw_workqueue_cancel_work(struct raw_workqueue *wq, struct raw_work *work);
int raw_workqueue_cancel_all(struct raw_workqueue *wq);

/* NOTE: flush family API not implemented yet */
int raw_workqueue_flush(struct raw_workqueue *wq);
int raw_workqueue_flush_util(struct raw_workqueue *wq, struct raw_work *work);
int raw_workqueue_flush_prio(struct raw_workqueue *wq, unsigned int prio);

void raw_workqueue_attr_init(struct raw_workqueue_attr *attr,
			     unsigned int nr_workers, unsigned int nr_queues);
int raw_workqueue_attr_set_inherit(struct raw_workqueue_attr *attr, int inherit);
int raw_workqueue_attr_set_policy(struct raw_workqueue_attr *attr, int policy);
int raw_workqueue_attr_set_prio(struct raw_workqueue_attr *attr, int prio);
int raw_workqueue_attr_set_name(struct raw_workqueue_attr *attr, const char *name);
int raw_workqueue_attr_set_flags(struct raw_workqueue_attr *attr, unsigned int flags);

#endif
