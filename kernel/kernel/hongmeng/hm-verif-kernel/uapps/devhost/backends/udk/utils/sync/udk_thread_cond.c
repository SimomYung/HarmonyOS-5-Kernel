/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Implementations of thread_cond in udk
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jul 20 17:23:10 2021
 */
#include <string.h>
#include <udk/thread_cond.h>

#include <udk/log.h>
#include <udk/timer.h>
#include <udk/mutex.h>
#include <udk/compiler.h>
#include <hongmeng/errno.h>
#include <libhmsync/raw_mutex.h>
#include <libhmsync/raw_thread_cond.h>

raw_static_assert(sizeof(struct udk_thread_cond) == sizeof(struct raw_thread_cond),
		  udk_thread_cond_size_is_wrong);
raw_static_assert(sizeof(struct udk_thread_cond_attr) == sizeof(struct raw_thread_cond_attr),
		  udk_thread_cond_attr_size_is_wrong);

int udk_thread_cond_init(struct udk_thread_cond *cond, const struct udk_thread_cond_attr *attr)
{
	return -hmerrno2posix(raw_thread_cond_init((struct raw_thread_cond *)(uintptr_t)cond,
						   (const struct raw_thread_cond_attr *)(uintptr_t)attr));
}

int udk_thread_cond_wait(struct udk_thread_cond *cond, struct udk_mutex *mutex)
{
	return -hmerrno2posix(raw_thread_cond_wait((struct raw_thread_cond *)(uintptr_t)cond,
						   (struct raw_mutex *)(uintptr_t)mutex));
}

int udk_thread_cond_timedwait(struct udk_thread_cond *cond,
			      struct udk_mutex *mutex,
			      const struct timespec *ts)
{
	return -hmerrno2posix(raw_thread_cond_timedwait((struct raw_thread_cond *)(uintptr_t)cond,
							(struct raw_mutex *)(uintptr_t)mutex, ts));
}

int udk_thread_cond_signal(struct udk_thread_cond *cond)
{
	return -hmerrno2posix(raw_thread_cond_signal((struct raw_thread_cond *)(uintptr_t)cond));
}

int udk_thread_cond_broadcast(struct udk_thread_cond *cond)
{
	return -hmerrno2posix(raw_thread_cond_broadcast((struct raw_thread_cond *)(uintptr_t)cond));
}

void udk_wait_cond_init(struct udk_wait_cond_s *wait_cond)
{
	int ret;

	if (wait_cond == NULL) {
		udk_warn("wait cond is NULL\n");
		return;
	}
	udk_mutex_init(&wait_cond->lock);
	ret = udk_thread_cond_init(&wait_cond->cond, NULL);
	if (ret < 0) {
		udk_error("udk thread cond init failed, %s\n", strerror(-ret));
		return;
	}
}

static void do_wakeup_all(struct udk_wait_cond_s *wait_cond, unsigned int attr)
{
	int ret;

	udk_mutex_lock(&wait_cond->lock);
	wait_cond->cond.attr |= attr;
	ret = udk_thread_cond_broadcast(&wait_cond->cond);
	if (ret < 0) {
		udk_error("thread cond signal failed, %s\n", strerror(-ret));
	}
	udk_mutex_unlock(&wait_cond->lock);
}

void udk_wakeup_all(struct udk_wait_cond_s *wait_cond)
{
	if (wait_cond == NULL) {
		udk_warn("wait cond is NULL\n");
		return;
	}
	do_wakeup_all(wait_cond, 0);
}
