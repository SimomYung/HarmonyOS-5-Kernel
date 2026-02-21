/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Implementation of ebr deferring synchronization
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jul 29 10:55:00 2020
 */
#include "ebr_defer.h"

#include <malloc.h>
#include <vsync/atomic.h>
#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>
#include <libmem/debug.h>
#include <hmkernel/barrier.h>
#ifdef __HOST_LLT__
#include <libstrict/strict.h>
#endif
#include <libhmsrv_sys/hm_thread.h>
#include "cb_stack.h"
#include "ebr_common.h"

#define EBR_WORKER_NAME_MAX_LEN 16
#define EBR_WORKER_NAME_PREFIX  "ebr"

#ifndef __HOST_LLT__
static void send_batch_signal(struct epoch_set *es, bool blocking);
static void *call_ebr_worker(void *arg);
#endif

#ifdef __HOST_LLT__

int call_ebr_obj_create(struct epoch_set *es)
{
	return E_HM_OK;
}

void call_ebr_obj_destroy(struct epoch_set *es)
{
	UNUSED(es);
}

int call_ebr(struct epoch_set *es, struct ebr_entry *entry,
	     void (*cb_func)(struct ebr_entry *entry))
{
	UNUSED(es);
	UNUSED(entry);
	UNUSED(cb_func);

	return E_HM_OK;
}

#else

#define EE_DUMP_SIZE 64
static void ebr_dump(const struct epoch_set *es, struct ebr_entry *ee)
{
	if (es != NULL) {
		hm_error("es info: reclaim_watermark = %d mcache_type = %d init_loc = %d\n",
			es->reclaim_watermark, es->mcache_type, es->init_loc);
		hm_error("dump epoch set %p\n", es);
		dump_mem_content((uintptr_t)es, sizeof(struct epoch_set));
		if (es->call_obj) {
			hm_error("dump call_obj %p\n", es->call_obj);
			dump_mem_content((uintptr_t)(es->call_obj), sizeof(struct call_ebr_obj));
		}
	}

	if (ee != NULL) {
		hm_error("dump ebr entry %p\n", ee);
		/* 2 * EE_DUMP_SIZE to dump mem before and after ee object */
		dump_mem_content((uintptr_t)ee - EE_DUMP_SIZE, 2 * EE_DUMP_SIZE);
	}
}

#define EBR_BUG_ON(cond, es, ee)	\
do {					\
	bool _c = (cond);		\
	if (_c) {			\
		ebr_dump(es, ee);	\
		BUG_ON(true, #cond);	\
	}				\
} while (0)

static int set_ebr_worker_name(raw_thread_t tid, const char *name)
{
	int err = hm_thread_setname(raw_thread_cref_of(tid), name, NULL);
	if (err < 0) {
		hm_error("set ebr worker name[%s] failed, err[%s]\n",
			 name, hmstrerror(err));
	}
	return err;
}

static void set_worker_state(struct call_ebr_obj *obj, enum worker_state state)
{
	__write_once(obj->state, state);
}

/* Initializaiton lock must be hold before calling this function */
int call_ebr_obj_create(struct epoch_set *es)
{
	int ret = E_HM_OK;
	struct call_ebr_obj *obj = NULL;

	if (es->call_obj == NULL) {
		obj = ebr_malloc(sizeof(struct call_ebr_obj));
		if (obj == NULL) {
			ret = E_HM_NOMEM;
		} else {
			cb_stack_init(&obj->cb_head);
			raw_atomic_int_init(&obj->call_watermark, 0);

			raw_mutex_init(&obj->worker_lock);
			obj->wtid = NULL;

			/* Assign es->call_obj after its initializaiton */
			ebr_assign_pointer(es->call_obj, obj);
		}
	}

	return ret;
}

static void do_reset_call_ebr_obj(struct epoch_set *es)
{
	struct call_ebr_obj *obj = es->call_obj;

	set_worker_state(obj, EBR_WORKER_STOP);
	send_batch_signal(es, true);
	(void)raw_thread_join(obj->wtid, NULL);
	obj->wtid = NULL;
}

static int
do_worker_thread_create(struct epoch_set *es, struct call_ebr_obj *obj)
{
	char ebr_worker_name[EBR_WORKER_NAME_MAX_LEN] = { 0 };
	int ret = E_HM_OK;
	raw_mutex_lock(&obj->worker_lock);
	if (obj->wtid != NULL) {
		goto unlock_worker;
	}

	set_worker_state(obj, EBR_WORKER_START);

	raw_mutex_init(&obj->call_lock);
	ret = raw_thread_cond_init(&obj->call_cond, NULL);
	if (ret != E_HM_OK) {
		goto unlock_worker;
	}

	ret = raw_thread_create(&obj->wtid, NULL, call_ebr_worker, (void *)es);
	if (ret != E_HM_OK) {
		goto destroy_cond;
	}
	/* Set call_ebr worker name, failure is not allowed here!!! */
	ret = snprintf_s(ebr_worker_name, EBR_WORKER_NAME_MAX_LEN,
			 EBR_WORKER_NAME_MAX_LEN - 1, "%s_%s",
			 EBR_WORKER_NAME_PREFIX, es->name);
	if (ret < 0) {
		goto destroy_cond;
	}
	ret = set_ebr_worker_name(obj->wtid, ebr_worker_name);
	if (ret != E_HM_OK) {
		do_reset_call_ebr_obj(es);
	}

destroy_cond:
	if (ret != E_HM_OK) {
		(void)raw_thread_cond_destroy(&obj->call_cond);
	}
unlock_worker:
	raw_mutex_unlock(&obj->worker_lock);
	return ret;
}

int ebr_thread_create(struct epoch_set *es)
{
	int ret = E_HM_OK;
	struct call_ebr_obj *obj = es->call_obj;
	/* Unlucky here, if wtid is NULL, means worker need create */
	if (unlikely(obj->wtid == NULL)) {
		ret = do_worker_thread_create(es, obj);
	}
	return ret;
}

static void worker_thread_destroy(struct epoch_set *es)
{
	struct call_ebr_obj *obj = es->call_obj;

	raw_mutex_lock(&obj->worker_lock);
	if (obj->wtid != NULL) {
		do_reset_call_ebr_obj(es);
		(void)raw_thread_cond_destroy(&obj->call_cond);
	}

	raw_mutex_unlock(&obj->worker_lock);
}

/* Initializaiton lock must be hold before calling this function */
void call_ebr_obj_destroy(struct epoch_set *es)
{
	if ((es != NULL) && (es->call_obj != NULL)) {
		worker_thread_destroy(es);
		ebr_free(es->call_obj);

		es->call_obj = NULL;
	}
}

static bool is_batch_triggered(struct epoch_set *es)
{
	return raw_atomic_int_read_relaxed(&es->call_obj->call_watermark) >=
	       es->reclaim_watermark;
}

/* Make sure dealing with the right epoch_set */
static void send_batch_signal(struct epoch_set *es, bool blocking)
{
	int ret = E_HM_OK;
	struct call_ebr_obj *obj = es->call_obj;

	if (blocking) {
		raw_mutex_lock(&obj->call_lock);
	} else {
		ret = raw_mutex_trylock(&obj->call_lock);
	}
	if (ret == E_HM_OK) {
		(void)raw_thread_cond_signal(&obj->call_cond);
		raw_mutex_unlock(&obj->call_lock);
	}
}

static int
do_call_ebr(struct epoch_set *es, struct ebr_entry *entry,
	    void (*cb_func)(struct ebr_entry *entry))
{
	int ret = E_HM_OK;
	/*
	 * call_obj should not be null, if so, object logically removed has
	 * not been referenced by any readers and it's safe to do reclaimation
	 */
	struct call_ebr_obj *obj = ebr_dereference(es->call_obj);
	if (obj == NULL) {
		cb_func(entry);
	} else {
		ret = ebr_thread_create(es);
		if (ret >= 0) {
			entry->cb_func = cb_func;
			cb_stack_push(&obj->cb_head, &entry->stack_entry);

			smp_wmb();
			(void)raw_atomic_int_inc_relaxed(&obj->call_watermark);
			/* Check if it's ready to do batch reclaimation */
			if (is_batch_triggered(es)) {
				/*
				 * Send the batch signal with non-blocking semantic to worker
				 * considering others may send signal at the same time, so it will
				 * return success always.
				 */
				send_batch_signal(es, false);
			}
		}
	}
	return ret;
}

int call_ebr(struct epoch_set *es, struct ebr_entry *entry,
	     void (*cb_func)(struct ebr_entry *entry))
{
	int ret = E_HM_INVAL;

	if ((es != NULL) && (entry != NULL) && (cb_func != NULL)) {
		ret = do_call_ebr(es, entry, cb_func);
	}

	return ret;
}

static bool is_worker_stop(const struct epoch_set *es)
{
	bool ret = false;
	struct call_ebr_obj *obj = ebr_dereference(es->call_obj);

	if (obj != NULL) {
		ret = __read_once(obj->state) == EBR_WORKER_STOP;
	}
	return ret;
}

static void do_batch_reclaim(struct epoch_set *es)
{
	struct cb_stack_entry *entry_head = NULL;
	struct cb_stack_entry *entry = NULL;
	struct cb_stack_entry *next = NULL;
	struct call_ebr_obj *obj = ebr_dereference(es->call_obj);
	int count = 0;

	if (obj != NULL) {
		entry_head = cb_stack_pop_all(&obj->cb_head);
		/* Waiting for readers to leave critical section */
		(void)synchronize_ebr(es);
		/* For now on, it's safe to excute callbacks of writers */
		cb_stack_for_each_continue_safe(entry, next, entry_head) {
			struct ebr_entry *ee = container_of(entry,
							    struct ebr_entry,
							    stack_entry);
			EBR_BUG_ON(ee->cb_func == NULL, es, ee);
			ee->cb_func(ee);
			count++;
		}
		(void)raw_atomic_int_sub_relaxed(&obj->call_watermark, count);
	}
}

static void wait_batch_signal(struct epoch_set *es)
{
	struct call_ebr_obj *obj = ebr_dereference(es->call_obj);

	if (obj != NULL) {
		raw_mutex_lock(&obj->call_lock);
		while (!is_worker_stop(es) && !is_batch_triggered(es)) {
			(void)raw_thread_cond_wait(&obj->call_cond, &obj->call_lock);
		}
		raw_mutex_unlock(&obj->call_lock);
	}
}

static void *call_ebr_worker(void *arg)
{
	struct epoch_set *es = (struct epoch_set *)arg;

	/* this while loop will be ended during epoch set destruction process */
	while (!is_worker_stop(es)) {
		wait_batch_signal(es);
		do_batch_reclaim(es);
	}
	/*
	 * Considering reclaimation signaled in batch, so
	 * there may have some callbacks left to be processed
	 */
	do_batch_reclaim(es);

	return NULL;
}

#endif
