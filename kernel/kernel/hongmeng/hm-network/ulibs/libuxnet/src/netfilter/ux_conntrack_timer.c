/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: timer for conntrack
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan 29 15:39:58 2021
 */
#include <netdb.h>
#include <lib/dlist.h>
#include <libhmlog/hmlog.h>
#include <libstrict/strict.h>
#include <libhmsync/raw_mutex.h>
#include <libhmsync/raw_thread.h>
#include <libhmsrv_sys/hm_timer.h>
#include <libhmsrv_sys/hm_thread.h>
#include <libhmsync/raw_thread_cond.h>

#include "ux_conntrack.h"

#define UX_CT_TIMER_PERIOD 120

static struct raw_thread_cond ux_timer_cond;
static struct raw_mutex ux_timer_mutex = RAW_MUTEX_INITIALIZER;

static int ux_ct_get_timer_state(void)
{
	struct timespec ts;
	struct timespec expire;

	mem_zero_s(expire);
	raw_mutex_lock(&ux_timer_mutex);
	(void)hm_clock_gettime(CLOCK_MONOTONIC, &ts);
	expire.tv_sec = ts.tv_sec + UX_CT_TIMER_PERIOD;
	expire.tv_nsec = ts.tv_nsec;
	(void)raw_thread_cond_timedwait(&ux_timer_cond, &ux_timer_mutex, &expire);
	raw_mutex_unlock(&ux_timer_mutex);

	return 1;
}

static void *ux_ct_timer_gc_thread(void *args)
{
	UNUSED(args);
	while (ux_ct_get_timer_state() != 0) {
		ux_ct_gc_handler();
	}

	return NULL;
}

void ux_timer_init(ux_ct_timer_t *timer,
		   unsigned long time, unsigned long interval)
{
	timer->expire = time + interval;
}

static int ux_ct_timer_gc_init(void)
{
	int ret;
	raw_thread_t thread = NULL;

	ret = raw_thread_cond_init(&ux_timer_cond, NULL);
	if (ret < 0) {
		net_error(HMNET_NETFILTER, "raw_thread_cond_init failed, ret=%d\n", ret);
		return ret;
	}

	ret = raw_thread_create(&thread, NULL, ux_ct_timer_gc_thread, NULL);
	if (ret < 0) {
		net_error(HMNET_NETFILTER, "raw_thread_create failed, ret=%d\n", ret);
		return ret;
	}

	return ret;
}

int ux_ct_timer_create_init(void)
{
	int ret;

	/* Release ct resource in expired case */
	ret = ux_ct_timer_gc_init();

	return ret;
}
