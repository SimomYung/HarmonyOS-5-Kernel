/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Create/Destroy a threadpool
 * Author: Huawei OS Kernel Lab
 * Create: Thu Mar 07 15:11:53 2019
 */

#include <hongmeng/errno.h>
#include <libhmsrv_sys/hm_thread.h>
#include <libhwsecurec/securec.h>
#include <libhmsync/raw_threadpool.h>
#include <hmkernel/sched/priority.h>

/*
 * The threadpool name plus the suffix may exceed the maximum array length,
 * which may cause snprintf failure. Therefore, a padding is added.
 */
#define THREADPOOL_NAME_PADDING 3

int raw_threadpool_attr_init(struct raw_threadpool_attr *attr, unsigned int nr_threads,
			     int inherit, int policy, int thread_priority)
{
	int rc = E_HM_OK;

	if (attr == NULL) {
		hm_debug("attr is NULL\n");
		rc = E_HM_INVAL;
	}
	if (rc == E_HM_OK) {
		if (nr_threads > __U(THREADPOOL_MAX_THREAD)) {
			rc = E_HM_INVAL;
		}
	}
	if (rc == E_HM_OK) {
		mem_zero_s(*attr);
		attr->nr_threads = nr_threads;
		attr->inherit = inherit;
		attr->sched_policy = policy;
		attr->thread_priority = thread_priority;
	}
	return rc;
}

static void destroy_threads(struct raw_threadpool *pool, unsigned int index)
{
	int ret;

	for (unsigned int i = 0; i < index; i++) {
		BUG_ON(pool->threads[i] == NULL);
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
		ret = raw_thread_terminate(pool->threads[i]);
#pragma GCC diagnostic pop
		if (ret != E_HM_OK) {
			hm_warn("destroy thread in threadpool failed, err=%s\n", hmstrerror(ret));
			continue;
		}
		ret = raw_thread_join(pool->threads[i], NULL);
		if (ret != E_HM_OK) {
			hm_warn("join thread in threadpool failed, err=%s\n", hmstrerror(ret));
		}
	}
}

static int __do_create_thread(struct raw_threadpool *pool,
			      const struct raw_threadpool_attr *attr,
			      const raw_thread_attr_t *thread_attr)
{
	int rc;
	int err = E_HM_OK;
	char name[RAW_THREADPOOL_NAME_LEN + THREADPOOL_NAME_PADDING];

	for (unsigned int i = 0; i < attr->nr_threads; i++) {
		err = raw_thread_create(&pool->threads[i], thread_attr, pool->func, pool->args);
		if (err != E_HM_OK) {
			hm_error("create raw thread failed, err=%s\n", hmstrerror(err));
			destroy_threads(pool, i);
			break;
		}

		if (attr->name[0] != '\0') {
			rc = snprintf_s(name, sizeof(name), sizeof(name) - 1U, "%s-%u", attr->name, i);
			if (rc < 0) {
				hm_warn("set threadpool name failed\n");
				continue;
			}
			(void)hm_thread_setname(raw_thread_cref_of(pool->threads[i]), name, NULL);
		}
	}

	return err;
}

static int do_raw_threadpool_create(struct raw_threadpool *pool,
				    const struct raw_threadpool_attr *attr)
{
	int err;
	raw_thread_attr_t thread_attr;
	struct raw_sched_param param;

	mem_zero_s(param);
	err = raw_thread_attr_init(&thread_attr);
	if (err != E_HM_OK) {
		hm_debug("raw_thread_attr_init failed, err=%s\n", hmstrerror(err));
	}

	if (err == E_HM_OK && attr->inherit == RAW_THREAD_EXPLICIT_SCHED) {
		err = raw_thread_attr_setinheritsched(&thread_attr, attr->inherit);
		if (err != E_HM_OK) {
			hm_debug("raw_thread_attr_setinheritsched failed, err=%s\n", hmstrerror(err));
		}

		if (err == E_HM_OK) {
			err = raw_thread_attr_setschedpolicy(&thread_attr, attr->sched_policy);
			if (err != E_HM_OK) {
				hm_debug("raw_thread_attr_setschedpolicy failed, err=%s\n", hmstrerror(err));
			}
		}

		if (err == E_HM_OK) {
			param.priority = (unsigned int)attr->thread_priority;
			err = raw_thread_attr_setschedparam(&thread_attr, &param);
			if (err != E_HM_OK) {
				hm_debug("raw_thread_attr_setschedparam failed, err=%s\n", hmstrerror(err));
			}
		}
	}

	if (err == E_HM_OK) {
		err = __do_create_thread(pool, attr, &thread_attr);
	}

	return err;
}

struct raw_threadpool *raw_threadpool_create(const struct raw_threadpool_attr *attr,
					     thread_handler func, void *args)
{
	size_t size = 0;
	bool ret = false;
	struct raw_threadpool *pool = NULL;

	if (attr == NULL) {
		hm_debug("failed to create threadpool: attr is null\n");
		ret = true;
	}

	if (!ret && (func == NULL)) {
		hm_debug("failed to create threadpool: thread function is null\n");
		ret = true;
	}

	if (!ret && (attr->nr_threads > __U(THREADPOOL_MAX_THREAD))) {
		hm_debug("failed to create threadpool, max thread=%d, real nr_threads=%u\n",
			 THREADPOOL_MAX_THREAD, attr->nr_threads);
		ret = true;
	}

	if (!ret) {
		size = attr->nr_threads * sizeof(raw_thread_t) + sizeof(struct raw_threadpool);
		pool = malloc(size);
		if (pool == NULL) {
			ret = true;
		}
	}

	if (!ret && pool != NULL) {
		NOFAIL(memset_s((void *)pool, size, 0, size));
		pool->nr_threads = attr->nr_threads;
		pool->func = func;
		pool->args = args;

		if (do_raw_threadpool_create(pool, attr) != E_HM_OK) {
			free(pool);
			pool = NULL;
		}
	}

	return pool;
}

void raw_threadpool_destroy(struct raw_threadpool *pool)
{
	unsigned int i;

	if (pool == NULL) {
		hm_debug("pool is NULL\n");
	} else {
		for (i = 0; i < pool->nr_threads; i++) {
			if (pool->threads[i] != NULL) {
				(void)raw_thread_join(pool->threads[i], NULL);
			}
		}

		free(pool);
	}
}
