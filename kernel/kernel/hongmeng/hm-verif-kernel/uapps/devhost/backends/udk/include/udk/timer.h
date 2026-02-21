/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: This is the header of udk_timer interfaces
 * Author: Huawei OS Kernel Lab
 * Create: Thu Sep 05 16:06:35 2019
 */

#ifndef __DEVHOST_UDK_TIMER_H__
#define __DEVHOST_UDK_TIMER_H__

#include <udk/types.h>
#include <time.h>
#include <libalgo/rbtree.h>

#define UDK_TIMER_FLAGS_PERIODIC	(1U)

struct udk_timer_attr {
	unsigned int flags;
	unsigned int precision;
};

/* interval is ms */
struct udk_timer {
	unsigned int magic;
	void (*func)(unsigned long);
	unsigned long data;
	unsigned long interval;
	struct timespec expire;
	struct rbt_node node;
	struct udk_timer_attr attr;
};

void udk_timer_init(struct udk_timer *timer,
		    void (*func)(unsigned long),
		    unsigned long data,
		    unsigned long interval);
int udk_timer_setflags(struct udk_timer *timer, unsigned int flags);
int udk_timer_setprecision(struct udk_timer *timer, unsigned int precision_ms);
int udk_timer_add(struct udk_timer *timer);
int udk_timer_del(struct udk_timer *timer);
int udk_timer_del_sync(struct udk_timer *timer);
int udk_timer_mod(struct udk_timer *timer, unsigned long interval);
bool udk_timer_is_pending(const struct udk_timer *timer);
bool udk_timer_is_pending_lockless(const struct udk_timer *timer);

int udk_clock_gettime(clockid_t clk_id, struct timespec *time_spec);
int udk_nanosleep(const struct timespec *req, struct timespec *rem);

static inline int timespec_compare(const struct timespec *a, const struct timespec *b)
{
	if (a->tv_sec > b->tv_sec) {
		return 1;
	} else if (a->tv_sec < b->tv_sec) {
		return -1;
	}
	if (a->tv_nsec > b->tv_nsec) {
		return 1;
	} else if (a->tv_nsec < b->tv_nsec) {
		return -1;
	}
	return 0;
}

#endif /* __DEVHOST_UDK_TIMER_H__ */
