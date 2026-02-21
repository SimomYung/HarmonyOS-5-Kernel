/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Implements for network vsock timer management
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jun 2 14:21:46 2021
 */

#include <errno.h>

#include <libhmsrv_sys/hm_timer.h>
#include <libalgo/idr.h>

#include "ux_netlog.h"
#include "ux_vsock_core.h"

struct vsock_timer {
	long timeout;
	timer_t hm_timer_id;

	vsock_timer_handler_t handler;
	void *arg;
};

static DEFINE_IDR(vsock_timer_idr);
static struct raw_mutex vsock_timers_lock = RAW_MUTEX_INITIALIZER;

static void __free_timer(struct vsock_timer *timer, int timer_id)
{
	(void)idr_remove(&vsock_timer_idr, (unsigned int)timer_id);
	(void)hm_timer_delete((int)(uintptr_t)timer->hm_timer_id);
	free(timer);
}

static void __timer_handler(union sigval v)
{
	int timer_id = v.sival_int;
	struct vsock_timer *timer = NULL;
	vsock_timer_handler_t handler = NULL;
	void *arg = NULL;

	raw_mutex_lock(&vsock_timers_lock);
	timer = idr_find(&vsock_timer_idr, (unsigned int)timer_id);
	/**
	* Get the field into temp variables, for timer callback function may
	* also operation on the timer (.etc delete the timer), it will cause deadlock
	*/
	if (timer != NULL) {
		handler = timer->handler;
		arg = timer->arg;

		__free_timer(timer, timer_id);
	}

	raw_mutex_unlock(&vsock_timers_lock);

	if (handler != NULL) {
		handler(arg);
	}
}

static int __init_timer(int timer_id, struct vsock_timer *timer)
{
	struct sigevent ev;
	int ret;

	mem_zero_s(ev);

	ev.sigev_value.sival_int = timer_id;
	ev.sigev_notify = SIGEV_THREAD;
	ev.sigev_notify_function = __timer_handler;

	ret = hm_timer_create(CLOCK_MONOTONIC, &ev, (int *)&timer->hm_timer_id);
	if (ret != 0) {
		net_warn(HMNET_VSOCK, "timer create failed: %s\n", hmstrerror(ret));
	}

	return ret;
}

#define millsec_to_sec(timeout)			((timeout) / 1000)
#define millsec_left_nsec(timeout, mill2sec)	(((timeout) - 1000 * (mill2sec)) * 1000000)
static int __start_timer(struct vsock_timer *timer)
{
	struct itimerspec it;
	int mill2sec = (int)millsec_to_sec(timer->timeout);
	int mill2nsec = (int)millsec_left_nsec(timer->timeout, mill2sec);
	int ret;

	mem_zero_s(it);
	it.it_value.tv_sec = mill2sec;
	it.it_value.tv_nsec = mill2nsec;
	it.it_interval.tv_sec = 0;
	it.it_interval.tv_nsec = 0;

	ret = hm_timer_settime((int)(uintptr_t)timer->hm_timer_id, 0, &it, NULL);
	if (ret != 0) {
		net_warn(HMNET_VSOCK, "timer settime failed: %s\n", hmstrerror(ret));
	}

	return ret;
}

int vsock_add_timer(int interval_ms, vsock_timer_handler_t handler, void *arg)
{
	struct vsock_timer *timer = NULL;
	int timer_id = -1;
	int ret = E_HM_OK;

	timer = calloc(1, sizeof(struct vsock_timer));
	if (timer == NULL) {
		ret = E_HM_NOMEM;
	}

	if (ret == E_HM_OK) {
		timer->timeout = interval_ms;
		timer->handler = handler;
		timer->arg = arg;
		timer_id = idr_alloc(&vsock_timer_idr, timer);
		if (timer_id < 0) {
			ret = timer_id;
		}
	}

	if (ret == E_HM_OK) {
		ret = __init_timer(timer_id, timer);
	}

	if (ret == E_HM_OK) {
		ret = __start_timer(timer);
	}

	if (ret != 0) {
		if (timer_id >= 0) {
			(void)idr_remove(&vsock_timer_idr, (unsigned int)timer_id);
		}

		if (timer != NULL && timer->hm_timer_id != NULL) {
			(void)hm_timer_delete((int)(uintptr_t)timer->hm_timer_id);
		}

		if (timer != NULL) {
			free(timer);
		}
	}

	return (ret == E_HM_OK) ? timer_id : ret;
}

int vsock_cancel_timer(int timer_id)
{
	struct vsock_timer *timer = NULL;
	int ret = E_HM_NOOBJ;

	raw_mutex_lock(&vsock_timers_lock);

	timer = idr_find(&vsock_timer_idr, (unsigned int)timer_id);
	if (timer != NULL) {
		__free_timer(timer, timer_id);
		ret = E_HM_OK;
	}

	raw_mutex_unlock(&vsock_timers_lock);
	return ret;
}
