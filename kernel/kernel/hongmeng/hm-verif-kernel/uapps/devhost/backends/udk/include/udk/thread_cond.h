/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Thread_cond interfaces of udk
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jul 20 17:19:34 2021
 */
#ifndef __UDK_THREAD_COND_H__
#define __UDK_THREAD_COND_H__

#include <udk/sync.h>
#include <udk/errno.h>
#include <udk/timer.h>
#include <udk/mutex.h>
#include <udk/lib/dlist.h>
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

/* Encapsulation of thread condition */
struct udk_wait_cond_s {
	struct udk_mutex lock;
	struct udk_thread_cond cond;
};

void udk_wait_cond_init(struct udk_wait_cond_s *wait_cond);

#define do_wait_cond(wait_cond, condition)					\
({										\
	int __err = 0;								\
	int __eret = 0;								\
										\
	udk_mutex_lock(&(wait_cond)->lock);					\
	while (!(condition)) {		\
		__err = udk_thread_cond_wait(&(wait_cond)->cond, &(wait_cond)->lock); \
	}									\
	__eret = ((condition) ? 0 : __err);						\
	udk_mutex_unlock(&(wait_cond)->lock);					\
										\
	(__eret);								\
})

#define udk_wait_cond(wait_cond, condition)					\
({										\
	if ((wait_cond) != NULL) {						\
		do_wait_cond((wait_cond), (condition));				\
	}									\
})

void udk_wakeup_all(struct udk_wait_cond_s *wait_cond);

#endif /* __UDK_THREAD_COND_H__ */
