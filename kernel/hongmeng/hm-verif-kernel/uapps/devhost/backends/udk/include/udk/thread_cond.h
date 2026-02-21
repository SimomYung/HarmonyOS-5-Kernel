/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Thread_cond interfaces of udk
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jul 20 17:19:34 2021
 */
#ifndef __UDK_THREAD_COND_H__
#define __UDK_THREAD_COND_H__

#include <udk/timer.h>
#include <udk/mutex.h>
#include <libhmsync/raw_thread_cond.h>

#define UDK_COND_INTERRUPTIBLE_ATTR	COND_INTERRUPT_FLAG
#define UDK_COND_SHARED_ATTR		COND_SHARED_FLAG

struct udk_thread_cond {
	int seq;
	int waiters;
	int lock;

	void *head;
	void *tail;

	unsigned int attr;
};

struct udk_thread_cond_attr {
	unsigned int attr;
};

int udk_thread_cond_init(struct udk_thread_cond *cond,
				const struct udk_thread_cond_attr *attr);
int udk_thread_cond_wait(struct udk_thread_cond *cond,
				struct udk_mutex *mutex);
int udk_thread_cond_timedwait(struct udk_thread_cond *cond,
				     struct udk_mutex *mutex,
				     const struct timespec *ts);
int udk_thread_cond_signal(struct udk_thread_cond *cond);
int udk_thread_cond_broadcast(struct udk_thread_cond *cond);

#endif /* __UDK_THREAD_COND_H__ */
