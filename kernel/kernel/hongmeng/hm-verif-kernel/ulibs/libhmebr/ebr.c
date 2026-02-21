/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Implementation of ebr
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jun 26 10:55:00 2020
 */
#include <libhmebr/ebr.h>

#include <vsync/atomic.h>
#include <libhmlog/hmlog.h>
#include <hmkernel/barrier.h>
#include <libhmsrv_sys/hm_timer.h>
#include <libhmsrv_sys/hm_futex.h>

#include "ebr_common.h"
#include "ebr_defer.h"

void smr_rwlock_rdlock(void *arg)
{
	(void)raw_rwlock_rdlock((struct raw_rwlock *)arg);
}

void smr_rwlock_wrlock(void *arg)
{
	(void)raw_rwlock_wrlock((struct raw_rwlock *)arg);
}

void smr_rwlock_unlock(void *arg)
{
	(void)raw_rwlock_unlock((struct raw_rwlock *)arg);
}

int ebr_init_with_watermark(struct epoch_set *es, int watermark)
{
	int ret = E_HM_OK;
	smr_rwlock_lib_t g_lock = {
		.read_acq	= smr_rwlock_rdlock,
		.read_rel	= smr_rwlock_unlock,
		.write_acq	= smr_rwlock_wrlock,
		.write_rel	= smr_rwlock_unlock,
		.arg		= NULL,
	};
	if (unlikely(es == NULL)) {
		ret = E_HM_INVAL;
	} else {
		raw_rwlock_init(&es->rwlock, NULL);
		raw_mutex_init(&es->init_lock);
		g_lock.arg = &es->rwlock;
		gdump_init(&es->gdump, g_lock);
		es->key = UINT_MAX;
		es->refcnt = 0;
		es->call_obj = NULL;
		es->reclaim_watermark = watermark;
	}

	return ret;
}

static int do_post_destroy(struct epoch_set *es)
{
	int ret = E_HM_OK;

	raw_mutex_lock(&es->init_lock);
	es->refcnt--;
	if (es->refcnt == 0U) {
		call_ebr_obj_destroy(es);
		ret = raw_thread_key_delete(es->key);
		if (ret == E_HM_OK) {
			es->key = UINT_MAX;
		}
		gdump_destroy(&es->gdump);
	}
	raw_mutex_unlock(&es->init_lock);

	return ret;
}

static void epoch_unit_dtor(void *eu)
{
	int ret = E_HM_OK;
	struct epoch_set *es = NULL;
	struct epoch_unit *unit = (struct epoch_unit *)eu;

	if (unit != NULL) {
		es = unit->es;
		gdump_deregister(&es->gdump, &unit->gdump);
		ebr_unit_free(unit);

		ret = do_post_destroy(es);
		if (ret < 0) {
			hm_warn("do post destroy failed\n");
		}
	}
}

static int do_post_init(struct epoch_set *es)
{
	int ret = E_HM_OK;

	raw_mutex_lock(&es->init_lock);
	if (es->refcnt == 0U) {
		/* 1. init call_obj in epoch set */
		ret = call_ebr_obj_create(es);
		if (ret == E_HM_OK) {
			/* 3. create tsd key for each unit to store epoch set */
			ret = raw_thread_key_create(&es->key, epoch_unit_dtor);
			if (ret < 0) {
				call_ebr_obj_destroy(es);
			}
		}
	}
	if (ret == E_HM_OK) {
		es->refcnt++;
	}
	raw_mutex_unlock(&es->init_lock);

	return ret;
}

static struct epoch_unit *epoch_unit_create(struct epoch_set *es)
{
	struct epoch_unit *unit = NULL;

	unit = (struct epoch_unit *)ebr_unit_malloc(sizeof(*unit));
	if (unit != NULL) {
		unit->es = es;
	}

	return unit;
}

static int do_epoch_unit_register(struct epoch_set *es)
{
	int ret = E_HM_OK;
	struct epoch_unit *unit = NULL;

	ret = do_post_init(es);
	if (ret == E_HM_OK) {
		unit = epoch_unit_create(es);
		if (unit == NULL) {
			hm_warn("unit create failed\n");
			(void)do_post_destroy(es);
			ret = E_HM_NOMEM;
		}
	}

	if (ret == E_HM_OK) {
		gdump_register(&es->gdump, &unit->gdump);
		(void)raw_thread_setspecific(es->key, unit);
	}

	return ret;
}

int ebr_register_unit(struct epoch_set *es)
{
	int ret = E_HM_OK;
	struct epoch_unit *unit = NULL;

	if (unlikely(es == NULL)) {
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		/*
		 * If the value in tsd[key] is not NULL, it means the epoch unit
		 * in current thread has bean created, return E_HM_OBJEXIST.
		 */
		unit = (struct epoch_unit *)raw_thread_getspecific(es->key);
		if (unit != NULL) {
			ret = E_HM_OBJEXIST;
		}
	}

	if (ret == E_HM_OK) {
		ret = do_epoch_unit_register(es);
	}
	return ret;
}

int ebr_unregister_unit(struct epoch_set *es)
{
	int ret = E_HM_OK;
	struct epoch_unit *unit = NULL;

	if (unlikely(es == NULL)) {
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		unit = (struct epoch_unit *)raw_thread_getspecific(es->key);
		if (unit == NULL) {
			ret = E_HM_INVAL;
		}
	}

	if (ret == E_HM_OK) {
		BUG_ON(unit->es != es);

		ret = raw_thread_setspecific(unit->es->key, NULL);
	}

	if (ret == E_HM_OK) {
		epoch_unit_dtor(unit);
	}

	return ret;
}

/*
 * Marks the beginning of a critical section
 */
int ebr_read_lock(struct epoch_set *es)
{
	struct epoch_unit *unit;

	if (unlikely(es == NULL)) {
		return E_HM_INVAL;
	}

	unit = (struct epoch_unit *)raw_thread_getspecific(es->key);
	if (unlikely(unit == NULL)) {
		/*
		 * If the epoch unit does not exist, then call
		 * epoch_unit_create to create one.
		 */
		int ret = do_epoch_unit_register(es);
		if (unlikely(ret != E_HM_OK)) {
			return ret;
		}
		unit = (struct epoch_unit *)raw_thread_getspecific(es->key);
	} else {
		BUG_ON(unit->es != es);
	}

	gdump_enter(&es->gdump, &unit->gdump);

	return E_HM_OK;
}

/*
 * Marks the end of a critical section
 */
int ebr_read_unlock(struct epoch_set *es)
{
	struct epoch_unit *unit;

	if (unlikely(es == NULL)) {
		return E_HM_INVAL;
	}

	unit = (struct epoch_unit *)raw_thread_getspecific(es->key);
	if (unlikely(unit == NULL)) {
		return E_HM_INVAL;
	}

	gdump_exit(&es->gdump, &unit->gdump);

	return E_HM_OK;
}
