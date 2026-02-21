/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: This is the implementation of udk_timer related interfaces
 * Author: Huawei OS Kernel Lab
 * Create: Thu Sep 05 15:57:30 2019
 */
#include <udk/timer.h>

#include <libalgo/rbtree.h>
#include <hmkernel/rq.h>
#include <hongmeng/errno.h>
#include <hongmeng/syscall.h>
#include <hongmeng/types.h>
#include <internal/init.h>
#include <libhmsync/raw_thread.h>
#include <libhmsync/raw_mutex.h>
#include <libhmsync/raw_thread_cond.h>
#include <libstrict/strict.h>
#include <libhmsrv_sys/hm_thread.h>
#include <libhmsrv_sys/hm_timer.h>
#include <hmkernel/sched/sched.h>
#include <hmkernel/sched/priority.h>
#include <limits.h>

#include <devhost/interrupt.h>

#define DEFAULT_PRECISION	CONFIG_UDK_TIMER_PRECISION
#define UDK_TIMER_MAGIC		(0xFF457867U)
#define MAGIC_CHECK(timer)	BUG_ON((timer)->magic != UDK_TIMER_MAGIC)

static raw_thread_t timer_tid = NULL;
/* timer_tree is ordered by expire_time */
static struct rbt_tree timer_tree = RBT_ROOT;
static struct timespec timer_tree_expire = { .tv_sec = 0, .tv_nsec = 0 };
/* This mutex protects timer_tree, timer_tree_expire and running_timer */
static struct raw_mutex timer_tree_mutex = RAW_MUTEX_INITIALIZER;
static struct raw_thread_cond timer_tree_cond;
static const struct udk_timer *running_timer = NULL;
static struct raw_thread_cond running_timer_cond;

/* Must be called with timer_tree_mutex hold */
static inline void set_running_timer(const struct udk_timer *timer)
{
	running_timer = timer;
}

/* Must be called with timer_tree_mutex hold */
static inline void unset_running_timer(void)
{
	running_timer = NULL;
	(void)raw_thread_cond_signal(&running_timer_cond);
}

static inline bool is_restart(struct udk_timer *timer)
{
	return (timer->attr.flags & UDK_TIMER_FLAGS_PERIODIC) != 0;
}

static int udk_timer_compare(const struct udk_timer *a, const struct udk_timer *b)
{
	int ret;
	ret = timespec_compare(&a->expire, &b->expire);
	if (ret == 0) {
		unsigned long a_addr = ptr_to_ulong(a);
		unsigned long b_addr = ptr_to_ulong(b);
		if (a_addr > b_addr) {
			ret = 1;
		} else if (a_addr < b_addr) {
			ret = -1;
		}
	}
	return ret;
}

static inline void udk_timer_update(struct udk_timer *timer)
{
	struct timespec now;
	long long nsec;
	long long precision_ns = (long long)timer->attr.precision * NSEC_PER_MSEC;

	mem_zero_s(now);
	(void)hm_clock_gettime(CLOCK_MONOTONIC, &now);
	nsec = (long long)now.tv_nsec + ((long long)(unsigned long long)timer->interval * NSEC_PER_MSEC);
	nsec =  (nsec - 1  + precision_ns) / precision_ns * precision_ns;
	timer->expire.tv_sec = now.tv_sec + (long)(nsec / NSEC_PER_SEC);
	timer->expire.tv_nsec = (long)(nsec % NSEC_PER_SEC);
	return;
}

static struct udk_timer *udk_timer_tree_search(struct rbt_tree *tree,
					       const struct udk_timer *timer,
					       struct rbt_node **parent,
					       bool *is_right)
{
	struct rbt_node *m = tree->root;
	struct rbt_node *n = NULL;
	bool right = false;
	struct udk_timer *result = NULL;

	while (m != NULL) {
		int cmp_res;
		struct udk_timer *cur = rbt_entry(m, struct udk_timer, node);
		cmp_res = udk_timer_compare(timer, cur);
		if (cmp_res == 0) {
			result = cur;
			break;
		} else if (cmp_res > 0) {
			if (m->rbt_right == NULL) {
				n = m;
				right = true;
			}
			m = m->rbt_right;
		} else {
			if (m->rbt_left == NULL) {
				n = m;
				right = false;
			}
			m = m->rbt_left;
		}
	}
	if (parent != NULL && is_right != NULL) {
		*parent = n;
		*is_right = right;
	}

	return result;
}

static int udk_timer_tree_insert(struct rbt_tree *tree, struct udk_timer *timer)
{
	bool is_right = false;
	struct rbt_node *parent = NULL;
	struct udk_timer *data = NULL;

	data = udk_timer_tree_search(tree, timer, &parent, &is_right);
	if (data != NULL) {
		return -EEXIST;
	}
	rbt_insert(tree, &timer->node, parent, is_right);
	return 0;
}

static int udk_timer_tree_remove(struct rbt_tree *tree, struct udk_timer *timer)
{
	/* keep consistent with ldk, ignore if timer doesn't exist */
	if (rbt_is_removable_node(&timer->node)) {
		rbt_remove(tree, &timer->node);
	}

	return 0;
}

static struct udk_timer *udk_timer_get_next(void)
{
	struct udk_timer *timer = NULL;
	struct timespec now, expire;
	int err;

	mem_zero_s(now);
	raw_mutex_lock(&timer_tree_mutex);
	unset_running_timer();
	while (timer == NULL) {
		timer = rbt_first_entry(&timer_tree, struct udk_timer, node);
		while (timer == NULL) {
			timer_tree_expire.tv_sec = LONG_MAX;
			timer_tree_expire.tv_nsec = 0;
			(void)raw_thread_cond_wait(&timer_tree_cond, &timer_tree_mutex);
			timer = rbt_first_entry(&timer_tree, struct udk_timer, node);
		}

		(void)hm_clock_gettime(CLOCK_MONOTONIC, &now);
		expire = timer->expire;
		if (timespec_compare(&expire, &now) > 0) {
			timer_tree_expire = expire;
			(void)raw_thread_cond_timedwait(&timer_tree_cond,
							&timer_tree_mutex,
							&expire);
			/* Rescan timer tree in case timer is invalid */
			timer = NULL;
		}
	}
	timer_tree_expire.tv_sec = 0;
	timer_tree_expire.tv_nsec = 0;

	/* Reset timer expire time before exec func */
	err = udk_timer_tree_remove(&timer_tree, timer);
	if (err < 0) {
		hm_warn("udk_timer process remove timer failed\n");
	}
	if (is_restart(timer)) {
		udk_timer_update(timer);
		err = udk_timer_tree_insert(&timer_tree, timer);
		if (err < 0) {
			hm_warn("udk_timer process insert timer failed\n");
		}
	}
	set_running_timer(timer);
	raw_mutex_unlock(&timer_tree_mutex);

	return timer;
}

#define UDK_TNAME "udk_timer"
static void* udk_timer_process(void *arg)
{
	UNUSED(arg);
	struct udk_timer *timer = NULL;
	struct sched_param param = {0};

#ifdef __HOST_UT__
	return NULL;
#endif
	BUG_ON(hm_thread_setname(0, UDK_TNAME, NULL) != 0);
	BUG_ON(hm_thread_sched_getparam(0, &param) != 0);
	param.sched_priority = HM_PRIORITY_DH_NORMAL;
	BUG_ON(hm_thread_sched_setscheduler(0, SCHED_RR, &param) != 0);

	while ((timer = udk_timer_get_next()) != NULL) {
		timer->func(timer->data);
	}

	return NULL;
}

static int udk_timer_process_init(void)
{
	raw_thread_attr_t attr;

	if (timer_tid != NULL) {
		hm_error("udk_timer has been inited\n");
		return E_HM_INVAL;
	}

	(void)raw_thread_cond_init(&timer_tree_cond, NULL);
	(void)raw_thread_cond_init(&running_timer_cond, NULL);

	BUG_ON(raw_thread_attr_init(&attr) != 0);
	BUG_ON(raw_thread_create(&timer_tid, &attr, udk_timer_process, NULL) != 0);

	hm_info("udk timer init success, default precision=%u\n", DEFAULT_PRECISION);

	return 0;
}
udk_init_call(udk_timer_process_init);

static void udk_timer_attr_init(struct udk_timer_attr *attr)
{
	BUG_ON(attr == NULL);
	attr->precision = DEFAULT_PRECISION;
	attr->flags = 0;
}

void udk_timer_init(struct udk_timer *timer,
		    void (*func)(unsigned long),
		    unsigned long data,
		    unsigned long interval)
{
	if (timer == NULL) {
		return;
	}
	mem_zero_s(*timer);
	timer->func = func;
	timer->data = data;
	timer->interval = interval;
	timer->magic = UDK_TIMER_MAGIC;
	udk_timer_attr_init(&timer->attr);
}

int udk_timer_setflags(struct udk_timer *timer, unsigned int flags)
{
	if (timer_tid == NULL || timer == NULL) {
		return -EINVAL;
	}
	MAGIC_CHECK(timer);
	timer->attr.flags = flags;

	return 0;
}

int udk_timer_setprecision(struct udk_timer *timer, unsigned int precision_ms)
{
	if (timer_tid == NULL || timer == NULL || precision_ms == 0U) {
		return -EINVAL;
	}
	MAGIC_CHECK(timer);
	timer->attr.precision = precision_ms;

	return 0;
}

int udk_timer_add(struct udk_timer *timer)
{
	int err;
	if (timer_tid == NULL || timer == NULL || timer->func == NULL) {
		return -EINVAL;
	}
	MAGIC_CHECK(timer);
	raw_mutex_lock(&timer_tree_mutex);

	udk_timer_update(timer);
	err = udk_timer_tree_insert(&timer_tree, timer);
	if (err == 0 && timespec_compare(&timer_tree_expire, &timer->expire) > 0) {
		(void)raw_thread_cond_signal(&timer_tree_cond);
	}
	raw_mutex_unlock(&timer_tree_mutex);

	return err;
}

static int udk_timer_del_internal(struct udk_timer *timer, bool is_sync)
{
	int err;
	if (timer_tid == NULL || timer == NULL) {
		return -EINVAL;
	}
	MAGIC_CHECK(timer);
	raw_mutex_lock(&timer_tree_mutex);
	err = udk_timer_tree_remove(&timer_tree, timer);
	if (err == 0) {
		while (is_sync && running_timer == timer) {
			(void)raw_thread_cond_wait(&running_timer_cond, &timer_tree_mutex);
		}
	}
	raw_mutex_unlock(&timer_tree_mutex);
	return err;
}

int udk_timer_del(struct udk_timer *timer)
{
	return udk_timer_del_internal(timer, false);
}

int udk_timer_del_sync(struct udk_timer *timer)
{
	return udk_timer_del_internal(timer, true);
}

int udk_timer_mod(struct udk_timer *timer, unsigned long interval)
{
	int err;
	if (timer_tid == NULL || timer == NULL) {
		return -EINVAL;
	}
	MAGIC_CHECK(timer);
	raw_mutex_lock(&timer_tree_mutex);
	(void)udk_timer_tree_remove(&timer_tree, timer);
	timer->interval = interval;
	udk_timer_update(timer);
	err = udk_timer_tree_insert(&timer_tree, timer);
	if (err == 0 && timespec_compare(&timer_tree_expire, &timer->expire) > 0) {
		(void)raw_thread_cond_signal(&timer_tree_cond);
	}
	raw_mutex_unlock(&timer_tree_mutex);
	return err;
}

bool udk_timer_is_pending(const struct udk_timer *timer)
{
	bool pending = false;

	if (timer == NULL) {
		return false;
	}

	raw_mutex_lock(&timer_tree_mutex);
	pending = rbt_is_removable_node(&timer->node);
	raw_mutex_unlock(&timer_tree_mutex);

	return pending;
}

bool udk_timer_is_pending_lockless(const struct udk_timer *timer)
{
	if (timer == NULL) {
		return false;
	}

	return rbt_is_removable_node(&timer->node);
}

int udk_clock_gettime(clockid_t clk_id, struct timespec *time_spec)
{
	return -hmerrno2posix(hm_clock_gettime(clk_id, time_spec));
}

int udk_nanosleep(const struct timespec *req, struct timespec *rem)
{
	return -hmerrno2posix(hm_nanosleep(req, rem));
}
