/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Operations on specific data of raw_thread
 * Author: Huawei OS Kernel Lab
 * Create: Tue Feb 19 12:37:33 2019
 */

#include <hongmeng/types.h>
#include <hongmeng/errno.h>
#include <libhmsync/atomic.h>
#include <libhmsync/raw_thread.h>
#include <libstrict/strict.h>

#include "raw_thread_impl.h"

/*
 * Only call PTHREAD_THREAD_DESTRUCTOR_INTERATIONS times to run destructor of a thread
 * to destroy the raw thread key.
 */
#define PTHREAD_THREAD_DESTRUCTOR_INTERATIONS 4
#define BIT_TRANS 16U

void *__raw_tsd_main[RAW_THREAD_KEYS_MAX];
static void (*volatile tsd_dtors[RAW_THREAD_KEYS_MAX])(void *);

static void default_dtor(void *dummy)
{
	UNUSED(dummy);
}

int raw_thread_key_create(raw_thread_key_t *k, void(*_dtor)(void *))
{
	unsigned int i = (unsigned int)((((uintptr_t)&k) / BIT_TRANS) % __U(RAW_THREAD_KEYS_MAX));
	unsigned int j = i;
	struct raw_thread *self = __raw_thread_self();
	int ret = E_HM_OK;
	void(*dtor)(void *) = _dtor;

	if (k == NULL) {
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		ret = E_HM_AGAIN;
		/* for main thread, maybe not initialized */
		if (CMPT_THREAD_REG(self, THREAD_TSD) == NULL) {
			CMPT_THREAD_REG(self, THREAD_TSD) = __raw_tsd_main;
		}

		if (dtor == NULL) {
			dtor = default_dtor;
		}

		do {
			if (raw_atomic_long_cas(ptr_to_type(tsd_dtors + j, raw_atomic_long_t *), 0, (long)(uintptr_t)dtor) == 0) {
				*k = j;
				ret = E_HM_OK;
				break;
			}
		} while ((j = (j + 1U) % __U(RAW_THREAD_KEYS_MAX)) != i);
	}
	return ret;
}

int raw_thread_key_delete(raw_thread_key_t k)
{
	int rc = E_HM_OK;

	if (k >= __U(RAW_THREAD_KEYS_MAX)) {
		rc = E_HM_INVAL;
	} else {
		tsd_dtors[k] = NULL;
	}
	return rc;
}

void __raw_thread_tsd_run_dtors(struct raw_thread *_thread)
{
	int i, j;
	int not_finished;
	struct raw_thread *thread = _thread;

	if (thread == NULL) {
		thread = __raw_thread_self();
	}

	/* fast path to run destructors for tsd */
	not_finished = CMPT_THREAD_REG(thread, THREAD_TSD_USED);

	for (i = 0; not_finished != 0 && i < PTHREAD_THREAD_DESTRUCTOR_INTERATIONS; i++) {
		not_finished = 0;
		for (j = 0; j < RAW_THREAD_KEYS_MAX; j++) {
			void(*dtor)(void *) = tsd_dtors[j];
			if ((CMPT_THREAD_REG(thread, THREAD_TSD)[j] != NULL) && dtor != NULL) {
				void *tmp = CMPT_THREAD_REG(thread, THREAD_TSD)[j];

				/* dtor may use CMPT_THREAD_REG(thread, tsd)[j] */
				dtor(tmp);
				CMPT_THREAD_REG(thread, THREAD_TSD)[j] = NULL;
				not_finished = 1;
			}
		}
	}
}

static void __raw_thread_setspecific(struct raw_thread *target, raw_thread_key_t k, const void *x)
{
	if (x != CMPT_THREAD_REG(target, THREAD_TSD)[k]) {
		CMPT_THREAD_REG(target, THREAD_TSD)[k] = (void *)x;
		CMPT_THREAD_REG(target, THREAD_TSD_USED) = 1;
	}
}

int raw_thread_setspecific(raw_thread_key_t k, const void *x)
{
	int rc = E_HM_OK;

	if (k >= __U(RAW_THREAD_KEYS_MAX)) {
		rc = E_HM_INVAL;
	}  else {
		struct raw_thread *self = __raw_thread_self();

		__raw_thread_setspecific(self, k, x);
	}
	return rc;
}

int raw_thread_target_setspecific(struct raw_thread *target, raw_thread_key_t k, const void *x)
{
	int rc = E_HM_OK;

	if (k >= __U(RAW_THREAD_KEYS_MAX)) {
		rc = E_HM_INVAL;
	}  else {
		__raw_thread_setspecific(target, k, x);
	}
	return rc;
}

void *raw_thread_getspecific(raw_thread_key_t k)
{
	void *ret = NULL;

	if (k < __U(RAW_THREAD_KEYS_MAX)) {
		struct raw_thread *self = __raw_thread_self();

		ret = CMPT_THREAD_REG(self, THREAD_TSD)[k];
	}
	return ret;
}

void raw_thread_specific_init(unsigned int *tls_offset)
{
	*tls_offset = (unsigned int)TLS_OFFSET_TSD;
}
