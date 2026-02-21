/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Interfaces of raw_thread_cond
 * Author: Huawei OS Kernel Lab
 * Create: Mon Feb 18 15:11:53 2019
 */

#ifndef LIBHMSYNC_RAW_THREAD_COND_H
#define LIBHMSYNC_RAW_THREAD_COND_H

#include <libhmsync/raw_mutex.h>
#include <libhmsync/atomic.h>

#define COND_INTERRUPT_FLAG 0x40000000
#define COND_SHARED_FLAG 0x80000000

struct raw_thread_cond {
	int seq;
	int waiters;
	int lock;

	void *head;
	void *tail;

	unsigned int attr;
};

struct raw_thread_cond_attr {
	unsigned int attr;
};

struct timespec;
int raw_thread_cond_init(struct raw_thread_cond *cond,
			 const struct raw_thread_cond_attr *attr);
int raw_thread_cond_destroy(struct raw_thread_cond *cond);
int raw_thread_cond_wait(struct raw_thread_cond *cond, struct raw_mutex *mutex);
int raw_thread_cond_timedwait(struct raw_thread_cond *cond,
			      struct raw_mutex *mutex, const struct timespec *ts);
int raw_thread_cond_signal(struct raw_thread_cond *cond);
int raw_thread_cond_broadcast(struct raw_thread_cond *cond);

#endif
