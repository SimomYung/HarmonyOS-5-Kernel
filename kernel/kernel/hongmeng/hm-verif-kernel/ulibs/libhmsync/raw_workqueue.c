/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Interfaces of raw_workqueue
 * Author: Huawei OS Kernel Lab
 * Create: Thu Mar 07 15:11:53 2019
 */

#include <stdbool.h>
#include <lib/utils.h>
#include <hongmeng/panic.h>
#include <hongmeng/errno.h>
#include <libhwsecurec/securec.h>
#include <libhmsync/raw_workqueue.h>
#include <libhmsync/raw_mutex_guard.h>
#include <libstrict/strict.h>
#include <hmkernel/sched/sched.h>
#include <hmkernel/sched/priority.h>
#include <hmkernel/attributes/hmattributes.h>
#include <libhmsrv_sys/hm_thread.h>
#include <libhmlog/hmlog_ratelimit.h>

#define DEFAULT_NR_WORKERS 4

enum {
	/* wait for specific work to finish */
	WAIT_FOR_SPECIFIC_WORK,

	/* wait for specific priority to finish */
	WAIT_FOR_PRIORITY,

	/* wait for all work to finish */
	WAIT_FOR_ALL_WORKS,
};

enum {
	WQ_INIT,
	WQ_EXIT,
	WQ_CANCEL,
};

struct raw_waiter {
	int type;
	void *private;

	/* waiter list link node */
	struct dlist_node node;

	/* use waiter list mutex and this cond */
	struct raw_thread_cond cond;
};

static inline bool raw_wq_is_freezable(struct raw_workqueue *wq)
{
	return (wq->flags & WQ_FREEZABLE_FLAG) != 0U;
}

static inline struct raw_work *raw_work_of_node(const struct dlist_node *node)
{
	return container_of(node, struct raw_work, node);
}

static inline unsigned int raw_work_priority_of(const struct raw_work *work)
{
	return work->u.priority;
}

static inline void raw_work_set_priority(struct raw_work *work,
					 unsigned int prio)
{
	if (prio > __U(RAW_WORK_PRIO_MAX)) {
		prio = RAW_WORK_PRIO_MAX;
	}
	work->u.priority = prio;
}

static inline void raw_work_set_canceled(struct raw_work *work)
{
	work->u.canceled = 1;
}

static void add_workqueue_waiter(struct raw_waiter_list *waiters,
				 struct raw_waiter *w)
{
	dlist_insert_tail(&waiters->head, &w->node);
}

static void remove_workqueue_waiter(struct raw_workqueue *wq,
				    struct raw_waiter *w)
{
	UNUSED(wq);
	dlist_delete(&w->node);
}

static int check_specific_work_condition(struct raw_workqueue *wq,
					 struct raw_waiter *w)
{
	int ret;
	unsigned int prio;
	struct raw_work *work = NULL;
	if (wq == NULL || w == NULL) {
		ret = 0;
	} else {
		work = w->private;

		BUG_ON(work == NULL);
		prio = raw_work_priority_of(work);

		RAW_MUTEX_GUARD(_, &wq->wq_mutex);
		ret = ((raw_prioqueue_is_queued(&wq->wait_wq, prio, &work->node) != 0) ||
		       (raw_prioqueue_is_queued(&wq->handling_wq, prio, &work->node) != 0));
	}
	return ret;
}

static int check_priority_condition(struct raw_workqueue *wq,
				    struct raw_waiter *w)
{
	struct {
		unsigned int prio;
	} *p = w->private;
	unsigned int prio = p->prio;

	RAW_MUTEX_GUARD(_, &wq->wq_mutex);
	return ((raw_prioqueue_has_prio_work(&wq->wait_wq, prio) != 0) ||
		(raw_prioqueue_has_prio_work(&wq->handling_wq, prio) != 0));
}

static int check_all_work_condition(struct raw_workqueue *wq,
				    const struct raw_waiter *w)
{
	UNUSED(w);
	RAW_MUTEX_GUARD(_, &wq->cond_mutex);
	return wq->nr_works > 0;
}

static int check_wait_condition(struct raw_workqueue *wq, struct raw_waiter *w)
{
	int result = 0;

	switch (w->type) {
	case WAIT_FOR_SPECIFIC_WORK:
		result = check_specific_work_condition(wq, w);
		break;
	case WAIT_FOR_PRIORITY:
		result = check_priority_condition(wq, w);
		break;
	case WAIT_FOR_ALL_WORKS:
		result = check_all_work_condition(wq, w);
		break;
	default:
		hm_panic("waiter_type=%d not supported\n", w->type);
		break;
	}
	return result;
}

static void notify_waiter(struct raw_workqueue *wq)
{
	struct raw_waiter *w = NULL, *pos = NULL;
	struct raw_waiter_list *waiters = &wq->waiters;

	raw_mutex_lock(&waiters->mutex);
	dlist_for_each_entry_safe(w, pos, &wq->waiters.head, struct raw_waiter, node) {
		if (check_wait_condition(wq, w) == 0) {
			remove_workqueue_waiter(wq, w);
			(void)raw_thread_cond_broadcast(&w->cond);
		}
	}
	raw_mutex_unlock(&waiters->mutex);
}

static void handle_work(struct raw_work *work)
{
	int err;

	BUG_ON(work == NULL);
	BUG_ON(work->func == NULL);

	err = work->func(work);
	if (err != E_HM_OK) {
		hm_error("handle work failed: return value=%d, err=%s\n", err,
			 hmstrerror(err));
	}
}

static int handle_flush(struct raw_workqueue *wq, int type, void *private)
{
	int err = E_HM_OK;
	struct raw_waiter w;
	struct raw_waiter_list *waiters = NULL;

	if (wq == NULL) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		waiters = &wq->waiters;

		mem_zero_s(w);
		w.type = type;
		w.private = private;
		(void)raw_thread_cond_init(&w.cond, NULL);

		raw_mutex_lock(&waiters->mutex);
		if (check_wait_condition(wq, &w) != 0) {
			add_workqueue_waiter(&wq->waiters, &w);
			err = raw_thread_cond_wait(&w.cond, &waiters->mutex);
			if (err != 0) {
				hm_warn("raw_thread_cond_wait failed, err=%s\n",
					hmstrerror(err));
			}
		}
		raw_mutex_unlock(&waiters->mutex);
	}

	return err;
}

int raw_work_init(struct raw_work *work, raw_work_handler func,
		  unsigned int prio)
{
	int err = E_HM_OK;

	if (work == NULL || func == NULL || prio > __U(RAW_WORK_PRIO_MAX)) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		mem_zero_s(*work);
		work->func = func;
		work->u.priority = prio;
		dlist_init(&work->node);
	}
	return err;
}

void raw_work_destroy(struct raw_work *work)
{
	struct raw_workqueue *wq = NULL;
	int err;

	if (work != NULL) {
		wq = work->wq;
	}

	if (wq != NULL) {
		raw_mutex_lock(&wq->wq_mutex);
		err = raw_prioqueue_remove(&wq->handling_wq, raw_work_priority_of(work), &work->node);
		if (err != E_HM_OK) {
			hm_error("prioqueue remove failed, err=%s\n", hmstrerror(err));
		}
		work->wq = NULL;
		raw_mutex_unlock(&wq->wq_mutex);

		if (err == E_HM_OK) {
			raw_mutex_lock(&wq->cond_mutex);
			wq->nr_works--;
			raw_mutex_unlock(&wq->cond_mutex);
		}
	}
}

static struct raw_workqueue_attr *raw_workqueue_default_attr(void)
{
	static int inited = 0;
	static struct raw_workqueue_attr s_attr;

	if (inited == 0) {
		mem_zero_s(s_attr);

		s_attr.nr_workers = DEFAULT_NR_WORKERS;
		s_attr.nr_queues = RAW_WORK_PRIO_MAX + 1;
		inited = 1;
	}
	return &s_attr;
}

static int raw_workqueue_enqueue(struct raw_workqueue *wq,
				  struct raw_work *work)
{
	int ret = E_HM_OK;

	RAW_MUTEX_GUARD(_, &wq->wq_mutex);
	if (dlist_empty(&work->node)) {
		(void)raw_prioqueue_enqueue(&wq->wait_wq, raw_work_priority_of(work), &work->node);
	} else {
		ret = E_HM_INVAL;
	}
	return ret;
}

static int raw_workqueue_remove(struct raw_workqueue *wq, struct raw_work *work)
{
	RAW_MUTEX_GUARD(_, &wq->wq_mutex);
	return raw_prioqueue_remove(&wq->wait_wq, raw_work_priority_of(work),
				    &work->node);
}

static struct raw_work *raw_workqueue_dequeue(struct raw_workqueue *wq)
{
	unsigned int prio = 0;
	struct dlist_node *node = NULL;
	struct raw_work *ret_work = NULL;
	int ret;

	raw_mutex_lock(&wq->wq_mutex);
	node = raw_prioqueue_dequeue(&wq->wait_wq, &prio);
	if (node != NULL) {
		ret = raw_prioqueue_enqueue(&wq->handling_wq, prio, node);
		if (ret != E_HM_OK) {
			hm_error("prioqueue enqueue failed, err=%s\n", hmstrerror(ret));
		}
	}
	raw_mutex_unlock(&wq->wq_mutex);

	if (node != NULL) {
		ret_work = raw_work_of_node(node);
	}

	return ret_work;
}

static inline bool workqueue_should_wait(struct raw_workqueue *wq)
{
	RAW_MUTEX_GUARD(_, &wq->wq_mutex);
	return (wq->nr_works == 0 || raw_prioqueue_is_empty(&wq->wait_wq) != 0 ||
		wq->state == WQ_EXIT);
}

static void (*g_wq_thread_ctx_setup)(void) = NULL;
void raw_workqueue_thread_ctx_setup_register(void (*ctx_setup)(void))
{
	g_wq_thread_ctx_setup = ctx_setup;
}

static inline void set_worker_freezable(void)
{
	int err = hm_thread_set_freezable();
	BUG_ON(err != E_HM_OK && err != E_HM_NOSYS, "woker thread set freezable failed, err=%s\n", hmstrerror(err));
}

static inline void try_freeze_worker(void)
{
	int err = hm_thread_try_to_freeze();
	if (err < 0 && err != E_HM_NOSYS) {
		hm_warn_ratelimited("worker thread try to freeze failed, err=%s\n", hmstrerror(err));
	}
}

static void *worker(void *args)
{
	int err;
	struct raw_work *work = NULL;
	struct raw_workqueue *wq = args;
	bool freezable = raw_wq_is_freezable(wq);

	if (g_wq_thread_ctx_setup != NULL) {
		g_wq_thread_ctx_setup();
	}

	if (freezable) {
		set_worker_freezable();
	}

	for (;;) {
		raw_mutex_lock(&wq->cond_mutex);
		while (workqueue_should_wait(wq)) {
			if (wq->state == WQ_EXIT) {
				raw_mutex_unlock(&wq->cond_mutex);
				__hmattr_sideff_resolved("no relative side effect");
				return NULL;
			}
			err = raw_thread_cond_wait(&wq->cond, &wq->cond_mutex);
			if (err != 0 && ((freezable && err != E_HM_HMSIGINTR &&
					  err != E_HM_RESTARTBLOCK) ||
					 !freezable)) {
				hm_warn("raw_thread_cond_wait failed, err=%s\n", hmstrerror(err));
			}
			if (freezable) {
				raw_mutex_unlock(&wq->cond_mutex);
				try_freeze_worker();
				raw_mutex_lock(&wq->cond_mutex);
			}
		}
		raw_mutex_unlock(&wq->cond_mutex);

		if (freezable) {
			try_freeze_worker();
		}

		/* pick up a work to handle */
		work = raw_workqueue_dequeue(wq);
		if (work != NULL) {
			if (wq->state == WQ_CANCEL) {
				raw_work_set_canceled(work);
			}
			work->wq = wq;
			handle_work(work);
		}
		notify_waiter(wq);
	}
	return NULL;
}

static inline unsigned int default_priority(const struct raw_workqueue *wq)
{
	return (wq->nr_queues) >> 1;
}

void raw_workqueue_attr_init(struct raw_workqueue_attr *attr,
			     unsigned int nr_workers, unsigned int nr_queues)
{
	mem_zero_s(*attr);
	attr->nr_workers = nr_workers;
	attr->nr_queues = nr_queues & RAW_WORK_PRIO_BIT_MASK;
}

int raw_workqueue_attr_set_flags(struct raw_workqueue_attr *attr, unsigned int flags)
{
	int err = E_HM_OK;

	/* only support WQ_FREEZABLE_FLAG now */
	if ((flags & ~((unsigned int)WQ_FREEZABLE_FLAG)) != 0U) {
		err = E_HM_INVAL;
	} else {
		attr->nr_queues |= (flags << RAW_WORK_PRIO_BIT);
	}
	return err;
}

int raw_workqueue_attr_set_inherit(struct raw_workqueue_attr *attr, int inherit)
{
	int err = E_HM_OK;

	if (inherit != RAW_THREAD_INHERIT_SCHED && inherit != RAW_THREAD_EXPLICIT_SCHED) {
		err = E_HM_INVAL;
	} else {
		attr->inherit = inherit;
	}
	return err;
}

int raw_workqueue_attr_set_policy(struct raw_workqueue_attr *attr, int policy)
{
	int err = E_HM_OK;

	if (policy < SCHED_OTHER || policy >= SCHED_NULL) {
		err = E_HM_INVAL;
	} else {
		attr->sched_policy = policy;
	}
	return err;
}

int raw_workqueue_attr_set_prio(struct raw_workqueue_attr *attr, int prio)
{
	int err = E_HM_OK;

	if (prio > HM_PRIORITY_WQ || prio < 0) {
		err = E_HM_INVAL;
	} else {
		attr->thread_priority = prio;
	}
	return err;
}

int raw_workqueue_attr_set_name(struct raw_workqueue_attr *attr, const char *name)
{
	int err = E_HM_OK;

	if (attr == NULL || name == NULL) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		err = snprintf_s(attr->name, RAW_THREADPOOL_NAME_LEN,
				 RAW_THREADPOOL_NAME_LEN - 1, "%s", name);
		if (err < 0) {
			hm_warn("set workqueue attr failed\n");
			err = E_HM_INVAL;
		} else {
			err = E_HM_OK;
		}
	}
	return err;
}

static int wq_prioqueue_init(struct raw_workqueue *wq,
			     const struct raw_workqueue_attr *attr)
{
	int err = E_HM_OK;

	err = raw_prioqueue_init(&wq->wait_wq, raw_workqueue_attr_nr_queues_of(attr));
	if (err != E_HM_OK) {
		(void)raw_thread_cond_destroy(&wq->cond);
		raw_mutex_destroy(&wq->wq_mutex);
		raw_mutex_destroy(&wq->cond_mutex);
		hm_error("failed to init wait prioqueue: %s\n", hmstrerror(err));
	}

	if (err == E_HM_OK) {
		err = raw_prioqueue_init(&wq->handling_wq, raw_workqueue_attr_nr_queues_of(attr));
		if (err != E_HM_OK) {
			raw_prioqueue_destroy(&wq->wait_wq);
			(void)raw_thread_cond_destroy(&wq->cond);
			raw_mutex_destroy(&wq->wq_mutex);
			raw_mutex_destroy(&wq->cond_mutex);
			hm_error("failed to init handling prioqueue: %s\n",
				 hmstrerror(err));
		}
	}

	if (err == E_HM_OK) {
		wq->nr_queues = raw_workqueue_attr_nr_queues_of(attr);
		wq->state = WQ_INIT;
	}

	return err;
}

static int wq_threadpool_create(struct raw_threadpool_attr *threadpool_attr,
				struct raw_workqueue *wq,
				const struct raw_workqueue_attr *attr)
{
	int err = E_HM_OK;

	err = raw_threadpool_attr_init(threadpool_attr, attr->nr_workers, attr->inherit,
				       attr->sched_policy, attr->thread_priority);
	if (err != E_HM_OK) {
		raw_prioqueue_destroy(&wq->handling_wq);
		raw_prioqueue_destroy(&wq->wait_wq);
		(void)raw_thread_cond_destroy(&wq->cond);
		raw_mutex_destroy(&wq->wq_mutex);
		raw_mutex_destroy(&wq->cond_mutex);
		hm_debug("thread pool attr init failed, err=%s\n", hmstrerror(err));
	}

	if (err == E_HM_OK) {
		if (attr->name[0] != '\0') {
			NOFAIL((strncpy_s(threadpool_attr->name, RAW_THREADPOOL_NAME_LEN,
					  attr->name, RAW_THREADPOOL_NAME_LEN - 1) != 0));
		}

		wq->pool = raw_threadpool_create(threadpool_attr, worker, wq);
		if (wq->pool == NULL) {
			raw_prioqueue_destroy(&wq->handling_wq);
			raw_prioqueue_destroy(&wq->wait_wq);
			(void)raw_thread_cond_destroy(&wq->cond);
			raw_mutex_destroy(&wq->wq_mutex);
			raw_mutex_destroy(&wq->cond_mutex);
			hm_error("failed to create raw thread pool for workqueue: nr_workers=%u\n",
				 attr->nr_workers);
			err = E_HM_INVAL;
		}
	}

	return err;
}

int raw_workqueue_init(struct raw_workqueue *wq,
		       const struct raw_workqueue_attr *_attr)
{
	int err = E_HM_OK;
	struct raw_threadpool_attr threadpool_attr;
	const struct raw_workqueue_attr *attr = _attr;

	if (wq == NULL) {
		err = E_HM_INVAL;
	}
	if (err == E_HM_OK) {
		if (attr == NULL) {
			attr = raw_workqueue_default_attr();
		}
		if ((int)raw_workqueue_attr_nr_queues_of(attr) > RAW_WORK_PRIO_MAX + 1) {
			err = E_HM_INVAL;
		}
	}
	if (err == E_HM_OK) {
		mem_zero_s(*wq);
		wq->flags = raw_workqueue_attr_flags_of(attr);
		raw_mutex_init(&wq->cond_mutex);
		raw_mutex_init(&wq->wq_mutex);
		err = raw_thread_cond_init(&wq->cond, &attr->cond_attr);
		if (err != 0) {
			hm_error("thread cond init failed:%s\n", hmstrerror(err));
		}
	}

	/* if wq is freezable, append COND_INTERRUPT_FLAG, so
	 * that the freezer module can interupt woker which
	 * blocked at raw_thread_cond_wait */
	if (err == E_HM_OK && raw_wq_is_freezable(wq)) {
		wq->cond.attr |= COND_INTERRUPT_FLAG;
	}

	if (err == E_HM_OK) {
		/* create prioqueue for workqueue */
		err = wq_prioqueue_init(wq, attr);
	}
	if (err == E_HM_OK) {
		/* init waiters */
		dlist_init(&wq->waiters.head);
		raw_mutex_init(&wq->waiters.mutex);
		/* create threadpool for workqueue */
		err = wq_threadpool_create(&threadpool_attr, wq, attr);
	}

	return err;
}

/* force kill all the worker threads */
int raw_workqueue_destroy(struct raw_workqueue *wq)
{
	int err = E_HM_OK;

	if (wq == NULL) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		/* first destroy threadpool, we can destroy other resource */
		if (wq->pool != NULL) {
			/* notify workqueue will exit */
			raw_mutex_lock(&wq->cond_mutex);
			wq->state = WQ_EXIT;
			err = raw_thread_cond_broadcast(&wq->cond);
			if (err != 0) {
				hm_warn("thread cond broadcast failed, err=%s\n", hmstrerror(err));
			}
			raw_mutex_unlock(&wq->cond_mutex);
			raw_threadpool_destroy(wq->pool);
		}
		raw_prioqueue_destroy(&wq->handling_wq);
		raw_prioqueue_destroy(&wq->wait_wq);
		(void)raw_thread_cond_destroy(&wq->cond);
		raw_mutex_destroy(&wq->wq_mutex);
		raw_mutex_destroy(&wq->cond_mutex);
	}
	return err;
}

int raw_workqueue_add_work(struct raw_workqueue *wq, struct raw_work *work)
{
	int err = E_HM_OK;
	unsigned int prio;

	/*
	 * work->wq != NULL indicates the work is already in a workqueue,
	 * thus it can't be added again.
	 */
	if (wq == NULL || work == NULL || work->wq != NULL) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		prio = raw_work_priority_of(work);
		if (prio > (unsigned int)(wq->nr_queues - 1U)) {
			/*
			 * if work priority is higher than wq max workqueue priority,
			 * use default priority for the work, half of the max wq priority.
			 */
			prio = default_priority(wq);
			raw_work_set_priority(work, prio);
		}

		work->wq = wq;
		err = raw_workqueue_enqueue(wq, work);
		if (err == E_HM_OK) {
			/* notify worker thread to work */
			raw_mutex_lock(&wq->cond_mutex);
			wq->nr_works++;
			err = raw_thread_cond_signal(&wq->cond);
			if (err != 0) {
				hm_warn("raw_thread_cond_signal failed, err=%s\n", hmstrerror(err));
			}
			raw_mutex_unlock(&wq->cond_mutex);
		}
	}
	return err;
}

int raw_workqueue_cancel_work(struct raw_workqueue *wq, struct raw_work *work)
{
	int err = E_HM_OK;
	if (wq == NULL || work == NULL) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		err = raw_workqueue_remove(wq, work);
		if (err == E_HM_OK) {
			raw_mutex_lock(&wq->cond_mutex);
			wq->nr_works--;
			raw_mutex_unlock(&wq->cond_mutex);

			raw_work_set_canceled(work);
			work->wq = NULL;
		} else {
			err = E_HM_BUSY;
		}
	}
	return err;
}

int raw_workqueue_cancel_all(struct raw_workqueue *wq)
{
	int err = E_HM_OK;

	if (wq == NULL) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		raw_mutex_lock(&wq->cond_mutex);
		if (wq->state == WQ_EXIT) {
			err = E_HM_BUSY;
		} else {
			wq->state = WQ_CANCEL;
		}
		raw_mutex_unlock(&wq->cond_mutex);
	}

	return err;
}

int raw_workqueue_flush(struct raw_workqueue *wq)
{
	return handle_flush(wq, WAIT_FOR_ALL_WORKS, NULL);
}

int raw_workqueue_flush_util(struct raw_workqueue *wq, struct raw_work *work)
{
	return handle_flush(wq, WAIT_FOR_SPECIFIC_WORK, work);
}

int raw_workqueue_flush_prio(struct raw_workqueue *wq, unsigned int prio)
{
	struct {
		unsigned int prio;
	} param = {.prio = prio};

	return handle_flush(wq, WAIT_FOR_PRIORITY, &param);
}
