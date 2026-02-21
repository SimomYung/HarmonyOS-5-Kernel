/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: HMbuild for udk hufs
 * Author: Kirin Platform Solution Dept, Hisilicon
 * Create: Mon Jan 23 17:23:00 UTC 2024
 */
 
#include <udk/log.h>
#include <udk/iolib.h>
#include <udk/module.h>
#include <udk/errno.h>
#include <libhwsecurec/securec.h>
#include <hongmeng/panic.h>
#include <udk/completion.h>
#include <libhmsrv_sys/hm_timer.h>
 
void udk_completion_init(struct udk_completion *comp)
{
	if (comp == NULL) {
		return;
	}

	(void)udk_thread_cond_init(&comp->cond, NULL);
	udk_mutex_init(&comp->mutex);
	comp->done = 0;
}
 
void udk_completion_wait(struct udk_completion *comp)
{
	if (comp == NULL) {
		return;
	}

	int ret = 0;
	udk_mutex_lock(&comp->mutex);
	while (comp->done == 0 && ret == 0) {
		ret = udk_thread_cond_wait(&comp->cond, &comp->mutex);
	}
	if (ret == 0) {
		comp->done--;
	}
	udk_mutex_unlock(&comp->mutex);
}
 
/* timeout is us */
unsigned long udk_completion_wait_timeout(struct udk_completion *comp, uint64_t timeout)
{
	if (comp == NULL) {
		return 0;
	}

	int ret = 0;
	struct timespec now;
	struct timespec ts;
	uint64_t nsec = 0;
 
	mem_zero_s(now);
	mem_zero_s(ts);
 
	ret = udk_clock_gettime(CLOCK_MONOTONIC, &now);
	if (ret < 0) {
		udk_error("udk clock gettime failed, %s\n", strerror(-ret));
		return 0;
	}
	nsec = (uint64_t)now.tv_nsec + (uint64_t)(timeout * NSEC_PER_USEC);
	ts.tv_sec = now.tv_sec + (nsec / NSEC_PER_SEC);
	ts.tv_nsec = nsec % NSEC_PER_SEC;
 
	udk_mutex_lock(&comp->mutex);
	while (comp->done == 0 && ret == 0) {
		ret = udk_thread_cond_timedwait(&comp->cond, &comp->mutex, &ts);
	}
	if (ret == 0) {
		comp->done--;
	}
	udk_mutex_unlock(&comp->mutex);
 
	if (ret == -ETIMEDOUT) {
		return 0;
	}
 
	if (ret < 0) {
		udk_error("wait completion failed, %s\n", strerror(-ret));
	}
	return timeout;
}
 
void udk_complete(struct udk_completion *comp)
{
	if (comp == NULL) {
		return;
	}

	udk_mutex_lock(&comp->mutex);
	comp->done++;
	(void)udk_thread_cond_signal(&comp->cond);
	udk_mutex_unlock(&comp->mutex);
}
 
bool udk_completion_done(struct udk_completion *comp)
{
	if (comp == NULL) {
		return 0;
	}

	return comp->cond.waiters > 0 ? 0 : 1;
}
