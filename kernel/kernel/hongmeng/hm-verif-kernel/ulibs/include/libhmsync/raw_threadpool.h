/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Interfaces of raw_threadpool
 * Author: Huawei OS Kernel Lab
 * Create: Thu Mar 7 15:11:53 2019
 */

#ifndef LIBHMSYNC_RAW_THREADPOOL_H
#define LIBHMSYNC_RAW_THREADPOOL_H

#include <libhmsync/raw_thread.h>

/* maybe changed later */
#define THREADPOOL_MAX_THREAD  16

#define RAW_THREADPOOL_NAME_LEN 16
typedef void *(*thread_handler)(void *);

struct raw_threadpool_attr {
	unsigned int nr_threads;
	int inherit;
	int sched_policy;
	int thread_priority;
	char name[RAW_THREADPOOL_NAME_LEN];
};

struct raw_threadpool {
	unsigned int nr_threads;

	/* thread function and args, used to create more threads dynamically */
	void *args;
	thread_handler func;

	/* thread handlers */
	raw_thread_t threads[0];
};

int raw_threadpool_attr_init(struct raw_threadpool_attr *attr, unsigned int nr_threads,
			     int inherit, int policy, int thread_priority);
struct raw_threadpool *raw_threadpool_create(const struct raw_threadpool_attr *attr,
					     thread_handler func, void *args);
void raw_threadpool_destroy(struct raw_threadpool *pool);

#endif
