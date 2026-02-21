/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Kernel space timer management
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jun 20 18:34:21 2019
 */
#include "hm_timer_utils.h"
#include "hm_timer_internal.h"

#include <libhmucap/kobjpool.h>
#include <errno.h>
#include <inttypes.h>
#include <sys/time.h>
#include <time/vdso.h>
#include <libalgo/id_generator.h>
#include <libhmlog/hmlog.h>
#include <hongmeng/errno.h>
#include <hongmeng/panic.h>
#include <hongmeng/syscall.h>
#include <hmkernel/vtimekeep.h>
#include <libhmucap/ucap.h>
#include <libstrict/strict.h>
#include <libstrict/overflow.h>
#include <libhmsync/thread.h>
#include <libhmsync/raw_mutex_guard.h>
#include <libhmsrv_sys/hm_vdso.h>
#include <libhmsrv_sys/hm_timer.h>
#include <libhmsrv_sys/hm_futex.h>
#include <libhmsrv_sys/hm_thread.h>
#include <libhmsrv_sys/hm_sysctrl.h>
#include <libhmactv/actv.h>
#include <libsysif/sysmgr/api.h>

#include <hmkernel/sched/sched.h>
#include <hmkernel/sched_module.h>
#include <hmkernel/sched/priority.h>

#include <hmkernel/sysconf.h>
#include <hmkernel/power/energy_model.h>

#define DEFAULT_TOCK_FREQ 1000000ULL
#define DFX_LATE_SEC      (10ULL)
#define DFX_EARLY_ALLOW_RANGE_SHIFT (2ULL)

struct utimer_s {
	unsigned int id;
	unsigned int state;
	vatomic64_t handling;
	enum utimer_clock_type type;
	clockid_t clk_ori_id;
	clockid_t clk_id;
	struct rbt_node rnode_pool;
	struct rbt_node rnode;
	unsigned long long expire;
	unsigned long long interval;
	unsigned long long overrun;
	struct raw_mutex lock;
	struct raw_refcnt refcnt;
	union sigval data;
	void (*handle_expired)(union sigval);
	pid_t owner_pid;
	pid_t owner_tid;
	char owner_name[UTIMER_OWNER_NAME_LEN];
};

struct utimer_clock_type_s {
	struct rbt_tree tree;
	struct utimer_s *first_timer;
};

struct utimer_pool_s {
	unsigned int state;
	struct utimer_clock_type_s type[UTIMER_CLOCK_MAX];
	struct rbt_tree tree;
	struct utimer_s *first_timer;
	unsigned long long first_expire;
	int utimer_futex;
};

static struct id_generator *utimer_id_gen = NULL;
static struct utimer_pool_s *g_utimer_pool = NULL;
static struct utimer_pool_s *g_alarm_utimer_pool = NULL;
static struct raw_mutex utimer_pool_lock = RAW_MUTEX_INITIALIZER;

static __u64 g_clock_freq = DEFAULT_TOCK_FREQ;

static int clock_gettime_values(clockid_t clk_id, __u64 *cur_values)
{
	int err;
	struct timespec cur_ts = {0};

	err = hm_clock_gettime(clk_id, &cur_ts);
	if (err < 0) {
		__hmattr_sideff_resolved("no relative side effect");
		return err;
	}

	err = timespec_to_values(&cur_ts, cur_values);
	if (err != 0) {
		hm_warn("timespec_to_values failed, err=%s\n", hmstrerror(err));
		__hmattr_sideff_resolved("no relative side effect");
		return err;
	}

	return E_HM_OK;
}

static enum utimer_clock_type posix_clock_to_uclock(clockid_t clk_id)
{
	enum utimer_clock_type type;

	if (clk_id == CLOCK_REALTIME || clk_id == CLOCK_REALTIME_ALARM) {
		type = UTIMER_REALTIME;
	} else if (clk_id == CLOCK_BOOTTIME || clk_id == CLOCK_BOOTTIME_ALARM) {
		type = UTIMER_BOOTTIME;
	} else {
		type = UTIMER_MONOTONIC;
	}

	return type;
}

static clockid_t uclock_to_posix_clock(enum utimer_clock_type type)
{
	clockid_t clk_id;

	if (type == UTIMER_MONOTONIC) {
		clk_id = CLOCK_MONOTONIC;
	} else if (type == UTIMER_REALTIME) {
		clk_id = CLOCK_REALTIME;
	} else {
		clk_id = CLOCK_BOOTTIME;
	}

	return clk_id;
}

static int sigev_thread_copy_and_check(clockid_t clk_id,
				       const struct sigevent *sev,
				       struct sigevent *event)
{
	int err;

	switch (clk_id) {
	case CLOCK_MONOTONIC:
	case CLOCK_REALTIME:
#ifdef CONFIG_MODULE_TIMER_RBTREE
	case CLOCK_BOOTTIME:
#ifdef CONFIG_RTC
	case CLOCK_REALTIME_ALARM:
	case CLOCK_BOOTTIME_ALARM:
	case CLOCK_POWEROFF_ALARM:
#endif /* endif CONFIG_RTC */
#endif /* endif CONFIG_MODULE_TIMER_RBTREE */
		err = E_HM_OK;
		break;
	default:
		err = E_HM_INVAL;
		break;
	}

	if (err == E_HM_OK) {
		/* sev cannot be NULL */
		if (sev != NULL) {
			if (safe_copy((void *)event, (void *)sev, sizeof(struct sigevent)) != E_HM_OK) {
				err = E_HM_INVAL;
			}

			if (event->sigev_notify != SIGEV_THREAD) {
				err = E_HM_INVAL;
			}
		} else {
			err = E_HM_INVAL;
		}
	}

	return err;
}

static int prepare_utimer_thread_attr(raw_thread_attr_t *attr)
{
	int err = E_HM_OK;
	struct raw_sched_param param;

	err = raw_thread_attr_init(attr);
	if (err == E_HM_OK) {
		err = raw_thread_attr_setinheritsched(attr, RAW_THREAD_EXPLICIT_SCHED);
	}

	if (err == E_HM_OK) {
		err = raw_thread_attr_setschedpolicy(attr, SCHED_RR);
	}

	if (err == E_HM_OK) {
		mem_zero_s(param);
		/* NOTE: need to choose a suitable priority */
		param.priority = HM_PRIORITY_UTIMERMGR;
		err = raw_thread_attr_setschedparam(attr, &param);
	}

	return err;
}

/* Caller should hold utimer_pool_lock and utimer->lock */
static void utimer_pool_insert_timer_rbtree_nolock(
		struct utimer_pool_s *utimer_pool, struct utimer_s *utimer)
{
	bool is_rchild = false;
	struct rbt_tree *tree = &utimer_pool->type[utimer->type].tree;
	struct rbt_node *rnode = tree->root;
	struct rbt_node *parent = NULL;
	struct utimer_s *tmp = NULL;

	while (rnode != NULL) {
		parent = rnode;
		/*
		 * When two nodes with the same value exist in the tree, the new
		 * node is inserted as the right child of the old node.
		 */
		tmp = rbt_entry(rnode, struct utimer_s, rnode);
		if (tmp->expire > utimer->expire) {
			rnode = rnode->rbt_left;
			is_rchild = false;
		} else {
			rnode = rnode->rbt_right;
			is_rchild = true;
		}
	}

	rbt_insert(tree, &utimer->rnode, parent, is_rchild);
}

static __u64 utimer_gettime(const struct utimer_s *utimer)
{
	__u64 cur_values = 0ULL;

	(void)clock_gettime_values(uclock_to_posix_clock(utimer->type), &cur_values);

	return cur_values;
}

static bool is_utimer_a_before_b(struct utimer_s *a, struct utimer_s *b)
{
	bool ret = false;

	if (a->type != b->type) {
		__u64 a_timeout = utimer_gettime(a);
		__u64 b_timeout = utimer_gettime(b);

		a_timeout = a->expire > a_timeout ? a->expire - a_timeout : 0;
		b_timeout = b->expire > b_timeout ? b->expire - b_timeout : 0;

		ret = a_timeout < b_timeout;
	} else {
		ret = a->expire < b->expire;
	}

	return ret;
}

/* Caller should hold utimer_pool_lock and utimer->lock */
static bool utimer_pool_insert_timer_nolock(
		struct utimer_pool_s *utimer_pool, struct utimer_s *utimer)
{
	bool is_first_utimer = false;
	bool is_first_utimer_on_tree = false;
	enum utimer_clock_type type = utimer->type;

	if (utimer_pool->type[type].first_timer == NULL ||
	    ((utimer != utimer_pool->type[type].first_timer) &&
	     is_utimer_a_before_b(utimer, utimer_pool->type[type].first_timer))) {
		is_first_utimer_on_tree = true;
		utimer_pool->type[type].first_timer = utimer;
	}
	if (is_first_utimer_on_tree && (utimer_pool->first_timer == NULL ||
	    is_utimer_a_before_b(utimer, utimer_pool->first_timer))) {
		is_first_utimer = true;
		utimer_pool->first_timer = utimer;
		utimer_pool->state = UTIMER_POOL_TIMEDWAIT;
	}

	utimer_pool_insert_timer_rbtree_nolock(utimer_pool, utimer);
	utimer->state = UTIMER_ON_TREE;

	return is_first_utimer;
}

/* Caller should hold utimer_pool_lock and utimer->lock */
static void utimer_pool_update_first_nolock(
		struct utimer_pool_s *utimer_pool)
{
	__u64 timeout;
	__u64 min_timeout = (__u64)-1;
	__u64 curr_time = 0ULL;
	enum utimer_clock_type tgt_type = UTIMER_CLOCK_MAX;

	for (unsigned int i = 0; i < (unsigned int)UTIMER_CLOCK_MAX; i++) {
		if (utimer_pool->type[i].first_timer != NULL) {
			curr_time = utimer_gettime(utimer_pool->type[i].first_timer);
			timeout = utimer_pool->type[i].first_timer->expire;
			timeout = timeout > curr_time ? timeout - curr_time : 0ULL;
			if (min_timeout > timeout) {
				min_timeout = timeout;
				tgt_type = (enum utimer_clock_type)i;
			}
		}
	}

	/* when the tree of utimer_pool is NULL, set utimer_pool state to UTIMER_POOL_WAIT */
	if (tgt_type == UTIMER_CLOCK_MAX) {
		utimer_pool->first_timer = NULL;
		utimer_pool->state = UTIMER_POOL_WAIT;
	} else {
		struct utimer_s *utimer = utimer_pool->type[tgt_type].first_timer;
		utimer_pool->first_timer = utimer;
	}
}

/* Caller should hold utimer_pool_lock and utimer->lock */
static void utimer_pool_remove_nolock(
		struct utimer_pool_s *utimer_pool, struct utimer_s *utimer)
{
	struct rbt_tree *tree = NULL;
	struct rbt_node *next_node = NULL;
	struct utimer_s *next_utimer = NULL;
	enum utimer_clock_type type;

	type = utimer->type;
	tree = &utimer_pool->type[type].tree;
	if (utimer == utimer_pool->type[type].first_timer) {
		next_node = rbt_first(tree);
		BUG_ON(utimer != rbt_entry(next_node, struct utimer_s, rnode));
		next_node = rbt_next(next_node);
		if (next_node != NULL) {
			next_utimer = rbt_entry(next_node, struct utimer_s, rnode);
		}
		utimer_pool->type[type].first_timer = next_utimer;
	}

	if (utimer == utimer_pool->first_timer) {
		utimer_pool_update_first_nolock(utimer_pool);
	}

	rbt_remove(tree, &utimer->rnode);
}

static void update_default_clock_freq(void)
{
	int read_size;
	unsigned long ret_value;

	read_size = hm_sysctrl_read_conf(__SYSCONF_CLOCK_FREQ,
					 (void *)&ret_value, sizeof(ret_value));
	if ((read_size == (int)sizeof(ret_value)) && (ret_value != 0UL)) {
		g_clock_freq = (__u64)ret_value;
	}
}

static void update_utimer_expire_and_overrun(__u64 cur_values, struct utimer_s *utimer)
{
	__u64 min_interval = 0ULL;

	if (g_clock_freq == DEFAULT_TOCK_FREQ) {
		update_default_clock_freq();
	}

	utimer->overrun += (cur_values - utimer->expire) / utimer->interval;
	min_interval = g_clock_freq / CONFIG_VTIMEDEV_PERIODIC_HZ;
	if (utimer->interval < min_interval) {
		utimer->expire = cur_values + min_interval;
	} else {
		utimer->expire = cur_values + utimer->interval;
	}
}

static inline int get_utimer(struct utimer_s *utimer)
{
	return raw_refcnt_get(&(utimer->refcnt));
}

static inline void put_utimer(struct utimer_s *utimer)
{
	(void)raw_refcnt_put(&(utimer->refcnt));
}

static struct utimer_s *utimer_pool_lookup_id_rbtree_nolock(
		struct utimer_pool_s *utimer_pool, unsigned int timerid)
{
	struct rbt_tree *tree = &utimer_pool->tree;
	struct rbt_node *rnode = tree->root;
	struct utimer_s *cur = NULL;
	struct utimer_s *tmp = NULL;

	while (rnode != NULL) {
		tmp = rbt_entry(rnode, struct utimer_s, rnode_pool);
		if (tmp->id == timerid) {
			cur = tmp;
			break;
		} else if (tmp->id > timerid) {
			rnode = rnode->rbt_left;
		} else {
			rnode = rnode->rbt_right;
		}
	}

	return cur;
}

static void utimer_pool_insert_id_rbtree_nolock(
		struct utimer_pool_s *utimer_pool, struct utimer_s *utimer)
{
	bool is_rchild = false;
	struct rbt_tree *tree = &utimer_pool->tree;
	struct rbt_node *rnode = tree->root;
	struct rbt_node *parent = NULL;
	struct utimer_s *tmp = NULL;
	int err = E_HM_OK;

	while (rnode != NULL) {
		parent = rnode;
		tmp = rbt_entry(rnode, struct utimer_s, rnode_pool);
		if (tmp->id > utimer->id) {
			rnode = rnode->rbt_left;
			is_rchild = false;
		} else if (tmp->id < utimer->id) {
			rnode = rnode->rbt_right;
			is_rchild = true;
		} else {
			err = E_HM_EXIST;
			hm_panic("insert utimer failed, err=%s\n", hmstrerror(err));
			break;
		}
	}

	if (err == E_HM_OK) {
		rbt_insert(tree, &utimer->rnode_pool, parent, is_rchild);
	}
}

static void utimer_pool_remove_id_rbtree_nolock(
		struct utimer_pool_s *utimer_pool, struct utimer_s *utimer)
{
	struct rbt_tree *tree = &utimer_pool->tree;

	rbt_remove(tree, &utimer->rnode_pool);
}

/* need to hold utimer_pool_lock */
static struct utimer_s *utimer_pool_timer_rbtree_first(struct rbt_tree *tree)
{
	struct rbt_node *p = NULL;
	struct utimer_s *first_utimer = NULL;

	p = rbt_first(tree);
	if (p != NULL) {
		first_utimer = rbt_entry(p, struct utimer_s, rnode);
	}

	return first_utimer;
}

static bool utimer_is_expired(const struct utimer_s *utimer)
{
	bool is_expired = false;
	__u64 cur_time = utimer_gettime(utimer);
	if (utimer->expire <= cur_time) {
		is_expired = true;
	}

	return is_expired;
}

static bool utimer_is_late(const struct utimer_s *utimer, __u64 sec)
{
	bool is_late = false;
	__u64 cur_time = utimer_gettime(utimer);
	if ((cur_time > utimer->expire) && ((cur_time - utimer->expire) >= (sec * g_clock_freq))) {
		is_late = true;
	}

	return is_late;
}

static void utimer_pool_dump_when_late_nolock(const struct utimer_pool_s *utimer_pool,
					      const struct utimer_s *utimer)
{
	__u64 timeout = 0ULL;
	__u64 curr_time = 0ULL;

	if (!utimer_is_expired(utimer)) {
		hm_error("utimer.dump: expire early, cur_tock %llu, expire %llu, interval %llu, overrun %llu, "
			 "clk_ori_id %d, clk_id %d, owner_pid %d, owner_tid %d, owner_name %s\n",
			 utimer_gettime(utimer), utimer->expire, utimer->interval, utimer->overrun,
			 utimer->clk_id, utimer->clk_ori_id, utimer->owner_pid, utimer->owner_tid,
			 utimer->owner_name);
	}

	if (utimer_is_late(utimer, DFX_LATE_SEC)) {
		hm_error("utimer.dump: expire to be late, cur_tock %llu, expire %llu, interval %llu, overrun %llu, "
			 "clk_ori_id %d, clk_id %d, owner_pid %d, owner_tid %d, owner_name %s\n",
			 utimer_gettime(utimer), utimer->expire, utimer->interval, utimer->overrun,
			 utimer->clk_id, utimer->clk_ori_id, utimer->owner_pid, utimer->owner_tid,
			 utimer->owner_name);

		if (utimer_pool->first_timer != NULL) {
			curr_time = utimer_gettime(utimer_pool->first_timer);
			timeout = utimer_pool->first_timer->expire;
			hm_error("utimer.dump: utimer_pool first: cur_time %llu, timeout %llu\n",
				 curr_time, timeout);
		}

		for (unsigned int i = 0; i < (unsigned int)UTIMER_CLOCK_MAX; i++) {
			if (utimer_pool->type[i].first_timer != NULL) {
				curr_time = utimer_gettime(utimer_pool->type[i].first_timer);
				timeout = utimer_pool->type[i].first_timer->expire;
				hm_error("utimer.dump: clkid:%u first: cur_time %llu, timeout %llu\n",
					 i, curr_time, timeout);
			}
		}
	}
}

/* need to hold utimer_pool_lock */
static void utimer_pool_wakeup_clock_inlock(
		struct utimer_pool_s *utimer_pool, enum utimer_clock_type type)
{
	int err;
	struct utimer_s *pos = NULL;
	struct rbt_tree *tree = NULL;
	void (*handle_expired)(union sigval) = NULL;
	union sigval sigval_data = {0};

	tree = &utimer_pool->type[type].tree;
	/*
	 * NOTE: If there are too many expiration timers, the comparison
	 * need to be reconsidered
	 */
	for (pos = utimer_pool_timer_rbtree_first(tree); (pos != NULL) && (utimer_is_expired(pos));
	     pos = utimer_pool_timer_rbtree_first(tree)) {
		/* 1. Remove the expired utimers from the utimer_pool tree */
		raw_mutex_lock(&pos->lock);
		utimer_pool_remove_nolock(utimer_pool, pos);

		/* 2. Get the utimer refcnt */
		err = get_utimer(pos);
		if (err < 0) {
			hm_error("refcnt get failed, err=%s, id=%u, expire=%llu, state=%u, count=%d\n",
				 hmstrerror(err), pos->id, pos->expire, pos->state,
				 raw_atomic_int_read(&pos->refcnt.refcnt_nr.count));
			raw_mutex_unlock(&pos->lock);
			continue;
		}

		/* 3. Reset the utimer to UTIMER_ON_LIST */
		pos->state = UTIMER_ON_LIST;
		vatomic64_inc(&pos->handling);

		utimer_pool_dump_when_late_nolock(utimer_pool, pos);

		handle_expired = pos->handle_expired;
		sigval_data = pos->data;

		/* 4. Release the utimer lock and utimer_pool_lock */
		raw_mutex_unlock(&pos->lock);
		raw_mutex_unlock(&utimer_pool_lock);

		/* 5. Handle the callback function of utimer */
		handle_expired(sigval_data);

		raw_mutex_lock(&utimer_pool_lock);
		raw_mutex_lock(&pos->lock);

		vatomic64_dec(&pos->handling);
		if ((pos->interval != 0ULL) && (pos->state == UTIMER_ON_LIST)) {
			/* 6. Update current time on demand */
			__u64 cur_values = utimer_gettime(pos);
			update_utimer_expire_and_overrun(cur_values, pos);
			(void)utimer_pool_insert_timer_nolock(utimer_pool, pos);
		}
		raw_mutex_unlock(&pos->lock);
		raw_mutex_unlock(&utimer_pool_lock);

		/* 7. Put the utimer refcnt */
		put_utimer(pos);

		/* 8. Hold the utimer_pool_lock */
		raw_mutex_lock(&utimer_pool_lock);
	}
}

static void utimer_pool_wakeup_all_clock(
		struct utimer_pool_s *utimer_pool)
{
	raw_mutex_lock(&utimer_pool_lock);
	for (unsigned int i = 0; i < UTIMER_CLOCK_MAX; i++) {
		utimer_pool_wakeup_clock_inlock(utimer_pool, (enum utimer_clock_type)i);
	}
	raw_mutex_unlock(&utimer_pool_lock);
}

static unsigned int utimer_pool_calc_futex_timedwait_flags_nolock(
		struct utimer_pool_s *utimer_pool)
{
	unsigned int flags = 0U;

#ifdef CONFIG_MODULE_TIMER_RBTREE
	switch (utimer_pool->first_timer->clk_id) {
	case CLOCK_REALTIME:
		flags = FUTEX_REALTIME_CLOCK;
		break;
	case CLOCK_BOOTTIME:
		flags = FUTEX_BOOTTIME_CLOCK;
		break;
#ifdef CONFIG_RTC
	case CLOCK_REALTIME_ALARM:
		flags = (FUTEX_ALARM_CLOCK | FUTEX_REALTIME_CLOCK);
		break;
	case CLOCK_BOOTTIME_ALARM:
		flags = (FUTEX_ALARM_CLOCK | FUTEX_BOOTTIME_CLOCK);
		break;
#endif /* endif CONFIG_RTC */
	default:
		break;
	}
#endif /* endif CONFIG_MODULE_TIMER_RBTREE */

	return flags;
}

static int utimer_pool_wait_next(
		struct utimer_pool_s *utimer_pool)
{
	int err = E_HM_TIMEDOUT;
	unsigned int flags = 0U;
	unsigned long long timeout = 0ULL;

	raw_mutex_lock(&utimer_pool_lock);
	if (utimer_pool->first_timer != NULL) {
		__u64 curr_time = utimer_gettime(utimer_pool->first_timer);
		/* Except for CLOCK_MONOTONIC, all other clock types are in TIMER_ABSTIME mode */
		if (utimer_pool->first_timer->clk_id != CLOCK_MONOTONIC) {
			flags = utimer_pool_calc_futex_timedwait_flags_nolock(utimer_pool);
			if (utimer_pool->first_timer->expire > curr_time) {
				timeout = utimer_pool->first_timer->expire;
			}
		} else {
			if (utimer_pool->first_timer->expire > curr_time) {
				timeout = utimer_pool->first_timer->expire - curr_time;
			}
		}
	}
	raw_mutex_unlock(&utimer_pool_lock);

	if (timeout != 0ULL) {
		bool fixup_cond = false;
		if (utimer_pool == g_alarm_utimer_pool) {
			fixup_cond = true;
		}

		if (!fixup_cond) {
			err = hm_futex_timedwait(&utimer_pool->utimer_futex, 0, &timeout, NULL, flags);
			utimer_pool->utimer_futex = 0;
		} else {
#define FIXUP_COND_NS_PER_S				1000000000ULL
#define FIXUP_COND_EXEPMT				10
			struct timespec pre_ts = {0};
			hm_clock_gettime(CLOCK_BOOTTIME, &pre_ts);
			__u64 pre_ts_now = (__u64)pre_ts.tv_sec * FIXUP_COND_NS_PER_S + (__u64)pre_ts.tv_nsec;

			err = hm_futex_timedwait(&utimer_pool->utimer_futex, 0, &timeout, NULL, flags);
			utimer_pool->utimer_futex = 0;

			struct timespec post_ts = {0};
			hm_clock_gettime(CLOCK_BOOTTIME, &post_ts);
			__u64 post_ts_now = (__u64)post_ts.tv_sec * FIXUP_COND_NS_PER_S + (__u64)post_ts.tv_nsec;

			if (post_ts_now - pre_ts_now < FIXUP_COND_NS_PER_S / FIXUP_COND_EXEPMT) {
				struct timespec sleep_time = {0};
				sleep_time.tv_sec = 0;
				sleep_time.tv_nsec = FIXUP_COND_NS_PER_S / FIXUP_COND_EXEPMT;
				hm_nanosleep(&sleep_time, NULL);
 			}
 		}

	}

	return err;
}

static inline void utimer_pool_futex_wait(
		struct utimer_pool_s *utimer_pool)
{
	/* return value is ignored as we don't care its cause */
	(void)hm_futex_wait(&utimer_pool->utimer_futex, 0, NULL, 0);
	utimer_pool->utimer_futex = 0;
}

static inline void utimer_pool_futex_wake(
		struct utimer_pool_s *utimer_pool)
{
	utimer_pool->utimer_futex = 1;
	/* return value is ignored as we don't care its reason */
	(void)hm_futex_wake(&utimer_pool->utimer_futex, 1, 0);
}

#define UTIMER_ALLOC_SIZE_MAX		4096

static int utimer_read_little_core_cpuset(unsigned int *cpuset)
{
	unsigned long cpu_num = 0UL;
	unsigned int *cpu_topology = NULL;
	size_t size;
	int err = hm_sysctrl_read_conf(__SYSCONF_CONFIG_CPU, (void *)&cpu_num, sizeof(cpu_num));
	if (err < 0) {
		hm_warn("read config cpu num sysconf failed: %s\n", hmstrerror(err));
		return err;
	}

	size = sizeof(unsigned int) * cpu_num * PM_CONFIG_CPUTOPO_NR_LEVELS;
	if (size >= UTIMER_ALLOC_SIZE_MAX) {
		hm_warn("malloc size %zu is abnormal\n", size);
		return E_HM_INVAL;
	}

	cpu_topology = (unsigned int *)malloc(size);
	if (cpu_topology == NULL) {
		hm_warn("cpu topology buffer malloc failed\n");
		return E_HM_NOMEM;
	}
	mem_zero_b(cpu_topology, size);

	err = hm_sysctrl_read_conf(__SYSCONF_CPU_TOPOLOGY, (void *)cpu_topology, (unsigned int)size);
	if (err < 0) {
		hm_warn("read cpu topology sysconf failed: %s\n", hmstrerror(err));
		free(cpu_topology);
		return err;
	}
	*cpuset = 0U;
	for (unsigned long i = 0UL; i < cpu_num; i++) {
		if (cpu_topology[i * PM_CONFIG_CPUTOPO_NR_LEVELS + PM_CPUTOPO_CLUSTER_INDEX] == 0) {
			*cpuset |= 1 << i;
		}
	}
	free(cpu_topology);

	return E_HM_OK;
}

static void *utimer_pool_hdlr_loop(
		struct utimer_pool_s *utimer_pool,
		const char *utimer_name)
{
	int err;

	unsigned int cpuset;
	unsigned int state = utimer_pool->state;
	err = utimer_read_little_core_cpuset(&cpuset);
	if (err != E_HM_OK) {
		hm_warn("utimer_read_little_core_cpuset failed, err=%s\n", hmstrerror(err));
		cpuset = 1U;
	}
	(void)hm_thread_sched_setaffinity(0, sizeof(cpuset), &cpuset);

	(void)hm_thread_setname(hm_thread_self_cref(), utimer_name, NULL);

	err = hm_thread_set_lt_group(0, SCHED_LT_POWER_GROUP);
	if (err != E_HM_OK && err != E_HM_NOSYS) {
		/* only print failed info */
		hm_warn("set %s sched domain failed %s", utimer_name, hmstrerror(err));
	}

	while (utimer_pool->state != UTIMER_POOL_FREE) {
		if (utimer_pool->state == UTIMER_POOL_TIMEDWAIT) {
			err = utimer_pool_wait_next(utimer_pool);
			/* Only when futex is time out, wake up other utimers. */
			if (err == E_HM_TIMEDOUT) {
				utimer_pool_wakeup_all_clock(utimer_pool);
			}
		} else {
			utimer_pool_futex_wait(utimer_pool);
		}
	}

	return NULL;
}

static void *utimer_pool_hdlr(void *args)
{
	UNUSED(args);
	return utimer_pool_hdlr_loop(g_utimer_pool, "utimermgr");
}

static void *utimer_pool_alarm_hdlr(void *args)
{
	UNUSED(args);
	return utimer_pool_hdlr_loop(g_alarm_utimer_pool, "utimermgr-a");
}

static int utimer_thread_create(void)
{
	int err;
	raw_thread_attr_t attr;
	raw_thread_t thread = NULL;

	raw_thread_attr_t alarm_attr;
	raw_thread_t alarm_thread = NULL;

	err = prepare_utimer_thread_attr(&attr);
	if (err == E_HM_OK) {
		prepare_utimer_thread_attr(&alarm_attr);
	}
	if (err == E_HM_OK) {
		err = raw_thread_create(&thread, &attr, utimer_pool_hdlr, NULL);
	}
	if (err == E_HM_OK) {
		err = raw_thread_create(&alarm_thread, &alarm_attr, utimer_pool_alarm_hdlr, NULL);
	}

	return err;
}

static struct utimer_pool_s *utimer_pool_alloc(void)
{
	struct utimer_pool_s *utimer_pool = (struct utimer_pool_s *)malloc(sizeof(struct utimer_pool_s));
	if (utimer_pool != NULL) {
		mem_zero_s(*utimer_pool);
		for (unsigned int i = 0; i < (unsigned int)UTIMER_CLOCK_MAX; i++) {
			utimer_pool->type[i].tree.root = NULL;
			utimer_pool->type[i].first_timer = NULL;
		}
		utimer_pool->state = UTIMER_POOL_WAIT;
		utimer_pool->tree.root = NULL;
		utimer_pool->utimer_futex = 0;
	}
	return utimer_pool;
}

static void utimer_pool_dealloc(
		struct utimer_pool_s *utimer_pool)
{
	free(utimer_pool);
}

static int utimer_pool_create_nolock(
		struct utimer_pool_s **utimer_pool_rp,
		struct utimer_pool_s **alarm_utimer_pool_rp)
{
	int err = E_HM_OK;
	struct id_generator_attr attr;
	struct utimer_pool_s *utimer_pool = NULL;
	struct utimer_pool_s *alarm_utimer_pool = NULL;

	mem_zero_s(attr);
	attr.min_index = POSIX_TIMER_MAX_NUM + 1;
	attr.max_index = UTIMER_MAX_NUM;

	err = id_generator_create(&attr, &utimer_id_gen);
	if (err == E_HM_OK) {
		utimer_pool = utimer_pool_alloc();
		if (utimer_pool == NULL) {
			err = E_HM_NOMEM;
			id_generator_destroy(utimer_id_gen);
		}
	}
	if (err == E_HM_OK) {
		alarm_utimer_pool = utimer_pool_alloc();
		if (alarm_utimer_pool == NULL) {
			err = E_HM_NOMEM;
			utimer_pool_dealloc(utimer_pool);
			id_generator_destroy(utimer_id_gen);
		}
	}
	if (err == E_HM_OK) {
		*utimer_pool_rp = utimer_pool;
		*alarm_utimer_pool_rp = alarm_utimer_pool;
	}

	return err;
}

static void utimer_pool_destroy_nolock(
		struct utimer_pool_s *utimer_pool,
		struct utimer_pool_s *alarm_utimer_pool)
{
	utimer_pool_dealloc(utimer_pool);
	utimer_pool_dealloc(alarm_utimer_pool);
	id_generator_destroy(utimer_id_gen);
}

static int utimer_pool_init(void)
{
	int err = E_HM_OK;

	raw_mutex_lock(&utimer_pool_lock);
	if (g_utimer_pool == NULL && g_alarm_utimer_pool == NULL) {
		err = utimer_pool_create_nolock(&g_utimer_pool, &g_alarm_utimer_pool);
		if (err == E_HM_OK) {
			err = utimer_thread_create();
		}
		if (err != E_HM_OK) {
			utimer_pool_destroy_nolock(g_utimer_pool, g_alarm_utimer_pool);
		}
	}
	raw_mutex_unlock(&utimer_pool_lock);

	return err;
}

static int utimer_owner_dump_sort_owner(const void *utimer_lhs_raw, const void *utimer_rhs_raw)
{
	struct utimer_s *utimer_lhs = *(struct utimer_s **)utimer_lhs_raw;
	struct utimer_s *utimer_rhs = *(struct utimer_s **)utimer_rhs_raw;

	int ret = 0;
	if (utimer_lhs->owner_pid < utimer_rhs->owner_pid) {
		ret = -1;
	} else if (utimer_lhs->owner_pid == utimer_rhs->owner_pid) {
		if (utimer_lhs->owner_tid < utimer_rhs->owner_tid) {
			ret = -1;
		} else if (utimer_lhs->owner_tid == utimer_rhs->owner_tid) {
			ret = 0;
		} else {
			ret = 1;
		}
	} else {
		ret = 1;
	}
	return ret;
}
struct utimer_owner_freq_s {
	pid_t owner_pid;
	struct utimer_s *owner_utimer;
	int owner_freq;
};
static int utimer_owner_dump_sort_pid_freq(const void *pid_freq_lhs_raw, const void *pid_freq_rhs_raw)
{
	struct utimer_owner_freq_s *pid_freq_lhs = (struct utimer_owner_freq_s *)pid_freq_lhs_raw;
	struct utimer_owner_freq_s *pid_freq_rhs = (struct utimer_owner_freq_s *)pid_freq_rhs_raw;

	int ret = 0;
	if (pid_freq_lhs->owner_freq < pid_freq_rhs->owner_freq) {
		ret = 1;
	} else if (pid_freq_lhs->owner_freq == pid_freq_rhs->owner_freq) {
		ret = 0;
	} else {
		ret = -1;
	}
	return ret;
}
#define UTIMER_DUMP_NR		0x5
static int utimer_owner_dump(void)
{
	int err = E_HM_OK;

	struct utimer_s *utimer = NULL;
	struct rbt_node *tmp = NULL;

	struct utimer_pool_s *utimer_pool_list[0x2] = {
		g_utimer_pool,
		g_alarm_utimer_pool,
	};

	raw_mutex_lock(&utimer_pool_lock);

	int utimer_count = 0;
	struct utimer_s **utimer_list = NULL;
	if (err == E_HM_OK) {
		for (int pool_idx = 0; pool_idx < 0x2; pool_idx++) {
			rbt_for_each_entry_safe(utimer, tmp, &utimer_pool_list[pool_idx]->tree,
						struct utimer_s, rnode_pool) {
				utimer_count += 1;
			}
		}
		utimer_list = malloc(sizeof(struct utimer_s *) * utimer_count);
		if (utimer_list == NULL) {
			hm_error("utimer.dump: no memory: utimer=%d\n", utimer_count);
			err = E_HM_NOMEM;
		}
	}

	if (err == E_HM_OK) {
		int utimer_index = 0;
		for (int pool_idx = 0; pool_idx < 0x2; pool_idx++) {
			rbt_for_each_entry_safe(utimer, tmp, &utimer_pool_list[pool_idx]->tree,
						struct utimer_s, rnode_pool) {
				utimer_list[utimer_index] = utimer;
				utimer_index += 1;
			}
		}
		BUG_ON(utimer_index != utimer_count);
		qsort(utimer_list, (size_t)utimer_count, sizeof(struct utimer_s *), utimer_owner_dump_sort_owner);
	}

	int proc_count = 0;
	struct utimer_owner_freq_s *utimer_owner_freq_list = NULL;
	if (err == E_HM_OK) {
		for (int pid_idx = 0; pid_idx < utimer_count; pid_idx++) {
			if (pid_idx == 0 || utimer_list[pid_idx - 1]->owner_pid != utimer_list[pid_idx]->owner_pid) {
				proc_count++;
			}
		}
		utimer_owner_freq_list = calloc((size_t)proc_count, sizeof(struct utimer_owner_freq_s));
		if (utimer_owner_freq_list == NULL) {
			hm_error("utimer.dump: no memory: utimer=%d proc=%d\n", utimer_count, proc_count);
			err = E_HM_NOMEM;
		}
	}

	if (err == E_HM_OK) {
		int proc_index = 0;
		for (int pid_idx = 0; pid_idx < utimer_count; pid_idx++) {
			if (pid_idx == 0 || utimer_list[pid_idx - 1]->owner_pid != utimer_list[pid_idx]->owner_pid) {
				proc_index++;

				utimer_owner_freq_list[proc_index - 1].owner_pid = utimer_list[pid_idx]->owner_pid;
				/* Record first, in case pid != tid */
				utimer_owner_freq_list[proc_index - 1].owner_utimer = utimer_list[pid_idx];
			}
			utimer_owner_freq_list[proc_index - 1].owner_freq += 1;
			if (utimer_list[pid_idx]->owner_pid == utimer_list[pid_idx]->owner_tid) {
				/* When pid == tid, should be the proc */
				utimer_owner_freq_list[proc_index - 1].owner_utimer = utimer_list[pid_idx];
			}
		}
		BUG_ON(proc_count != proc_index);
		qsort(utimer_owner_freq_list, (size_t)proc_count,
			sizeof(struct utimer_owner_freq_s), utimer_owner_dump_sort_pid_freq);

		for (int pid_idx = 0; pid_idx < utimer_count && pid_idx < UTIMER_DUMP_NR; pid_idx++) {
			struct utimer_s *utimer_pid = utimer_owner_freq_list[pid_idx].owner_utimer;
			hm_error("utimer.dump: rank%d pid=%d name=%s freq=%d\n",
				 pid_idx,
				 utimer_pid->owner_pid, utimer_pid->owner_name, utimer_owner_freq_list[pid_idx].owner_freq);
		}
	}

	if (utimer_owner_freq_list != NULL) {
		free(utimer_owner_freq_list);
	}
	if (utimer_list != NULL) {
		free(utimer_list);
	}

	raw_mutex_unlock(&utimer_pool_lock);
	return err;
}

static int utimerid_generate(unsigned int *id)
{
	int err;

	err = id_generator_alloc(utimer_id_gen, id);
	if (err != E_HM_OK) {
		/* according to the man manual of error definition */
		if (err == E_HM_NOSPC) {
			err = E_HM_AGAIN;
			utimer_owner_dump();
		}
	}

	return err;
}

static void delete_utimer(struct raw_refcnt *refcnt)
{
	struct utimer_s *utimer = NULL;

	utimer = container_of(refcnt, struct utimer_s, refcnt);
	id_generator_free(utimer_id_gen, (unsigned int)utimer->id);
	free(utimer);
}

static void utimer_init(struct utimer_s *utimer, unsigned int timer_id,
			clockid_t clock_id, struct sigevent *event)
{
	clockid_t clk_id = clock_id;

#ifndef CONFIG_MODULE_TIMER_RBTREE
	clk_id = CLOCK_MONOTONIC;
#endif
	utimer->id = timer_id;
	utimer->state = UTIMER_DELETING;
	vatomic64_init(&utimer->handling, 0);
	utimer->clk_ori_id = clk_id;
	utimer->clk_id = clk_id;
	utimer->type = posix_clock_to_uclock(clk_id);
	utimer->handle_expired = event->sigev_notify_function;
	utimer->data = event->sigev_value;
	utimer->overrun = 0ULL;
	raw_mutex_init(&utimer->lock);
	raw_refcnt_init(&utimer->refcnt, 1, delete_utimer);
}

static inline bool is_alarm_clock(clockid_t clk_id)
{
	bool ret = false;
	if (clk_id == CLOCK_REALTIME || clk_id == CLOCK_MONOTONIC || clk_id == CLOCK_BOOTTIME) {
		ret = false;
	} else if (clk_id == CLOCK_REALTIME_ALARM || clk_id == CLOCK_BOOTTIME_ALARM
			|| clk_id == CLOCK_POWEROFF_ALARM) {
		ret = true;
	} else {
		BUG_ON(false);
	}
	return ret;
}

static bool utimer_pool_first_expired(struct utimer_pool_s *utimer_pool)
{
	struct utimer_s *utimer = NULL;
	__u64 cur_time = 0ULL;
	__u64 ms_250 = g_clock_freq >> DFX_EARLY_ALLOW_RANGE_SHIFT;

	if (utimer_pool == NULL) {
		return false;
	}

	if (utimer_pool->first_timer != NULL) {
		utimer = utimer_pool->first_timer;
		cur_time = utimer_gettime(utimer);
		/* Considering delayed wakeup due to timerslack mechanism */
		if ((utimer->expire < cur_time) && ((cur_time - utimer->expire) > ms_250)) {
			hm_error("utimer.dump: utimer_pool first expired, cur_tock %llu, expire %llu, interval %llu, overrun %llu, "
				 "clk_ori_id %d, clk_id %d, owner_pid %d, owner_tid %d, owner_name %s\n",
				 utimer_gettime(utimer), utimer->expire, utimer->interval, utimer->overrun,
				 utimer->clk_id, utimer->clk_ori_id, utimer->owner_pid, utimer->owner_tid,
				 utimer->owner_name);
			return true;
		}
	}

	for (unsigned int i = 0; i < (unsigned int)UTIMER_CLOCK_MAX; i++) {
		if (utimer_pool->type[i].first_timer != NULL) {
			utimer = utimer_pool->type[i].first_timer;
			cur_time = utimer_gettime(utimer);
			/* Considering delayed wakeup due to timerslack mechanism */
			if ((utimer->expire < cur_time) && ((cur_time - utimer->expire) > ms_250)) {
				hm_error("utimer.dump: utimer_pool %u first expired, cur_tock %llu, expire %llu, interval %llu, overrun %llu, "
					 "clk_ori_id %d, clk_id %d, owner_pid %d, owner_tid %d, owner_name %s\n", i,
					 utimer_gettime(utimer), utimer->expire, utimer->interval, utimer->overrun,
					 utimer->clk_id, utimer->clk_ori_id, utimer->owner_pid, utimer->owner_tid,
					 utimer->owner_name);
				return true;
			}
		}
	}

	return false;
}

static inline void uimer_pool_wakeup_expired(struct utimer_pool_s *utimer_pool)
{
	if (utimer_pool_first_expired(utimer_pool)) {
		utimer_pool_futex_wake(utimer_pool);
	}
}

/*
 * event->sigev_notify is SIGEV_THREAD
 */
int utimer_create_sigev_thread(clockid_t clk_id, const struct sigevent *sev, int *timerid)
{
	int err;
	unsigned int timer_id = 0U;
	struct sigevent event;
	struct utimer_s *utimer = NULL;

	mem_zero_s(event);

	err = sigev_thread_copy_and_check(clk_id, sev, &event);
	if (err == E_HM_OK) {
		/* 1. Init utimer pool on demand */
		err = utimer_pool_init();
	}

	if (err == E_HM_OK) {
		/* 2. Generate timer_id */
		err = utimerid_generate(&timer_id);
	}

	if (err == E_HM_OK) {
		/* 3. Add utimer to utimer pool */
		utimer = (struct utimer_s *)malloc(sizeof(struct utimer_s));
		if (utimer != NULL) {
			mem_zero_s(*utimer);
			utimer_init(utimer, timer_id, clk_id, &event);
			utimer->owner_pid = hm_actv_local_thread_pid_of();
			utimer->owner_tid = hm_actv_local_thread_tid_of();
			NOFAIL(strncpy_s(utimer->owner_name, UTIMER_OWNER_NAME_LEN,
					hm_actv_local_thread_name_of(), UTIMER_OWNER_NAME_LEN - 1));
		} else {
			err = E_HM_NOMEM;
			id_generator_free(utimer_id_gen, timer_id);
		}
	}

	if (err == E_HM_OK) {
		/* 4. Copy utimer id */
		if (safe_copy((void *)timerid, (void *)&timer_id, sizeof(int)) != 0) {
			put_utimer(utimer);
			err = E_HM_POSIX_FAULT;
		} else {
			raw_mutex_lock(&utimer_pool_lock);
			if (is_alarm_clock(clk_id)) {
				utimer_pool_insert_id_rbtree_nolock(g_alarm_utimer_pool, utimer);
				uimer_pool_wakeup_expired(g_alarm_utimer_pool);
			} else {
				utimer_pool_insert_id_rbtree_nolock(g_utimer_pool, utimer);
				uimer_pool_wakeup_expired(g_utimer_pool);
			}
			utimer->state = UTIMER_ON_LIST;
			raw_mutex_unlock(&utimer_pool_lock);
		}
	}

	return err;
}

static int timer_settime_copy_and_check(const struct itimerspec *val,
					struct itimerspec *val_copy)
{
	int err = E_HM_OK;

	if (val == NULL) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		if (safe_copy((void *)val_copy, (void *)val,
			      sizeof(struct itimerspec)) != 0) {
			err = E_HM_POSIX_FAULT;
		}
	}

	if (err == E_HM_OK) {
		if (val_copy->it_value.tv_sec < 0 ||
		    val_copy->it_value.tv_nsec < 0 ||
		    val_copy->it_value.tv_nsec >= NSEC_PER_SEC) {
			err = E_HM_INVAL;
		}
	}

	if (err == E_HM_OK) {
		if (val_copy->it_interval.tv_sec < 0 ||
		    val_copy->it_interval.tv_nsec < 0 ||
		    val_copy->it_interval.tv_nsec >= NSEC_PER_SEC) {
			err = E_HM_INVAL;
		}
	}

	return err;
}

static struct utimer_s *utimer_pool_lookup_id_tree(
		int timerid, struct utimer_pool_s **utimer_pool_rp)
{
	int err;
	struct utimer_s *ret_utimer = NULL;
	struct utimer_pool_s *utimer_pool = NULL;

	raw_mutex_lock(&utimer_pool_lock);
	if (g_utimer_pool != NULL) {
		ret_utimer = utimer_pool_lookup_id_rbtree_nolock(g_utimer_pool, (unsigned int)timerid);
		utimer_pool = g_utimer_pool;
		if (ret_utimer == NULL) {
			ret_utimer = utimer_pool_lookup_id_rbtree_nolock(g_alarm_utimer_pool, (unsigned int)timerid);
			utimer_pool = g_alarm_utimer_pool;
		}
	}
	if (ret_utimer != NULL) {
		err = get_utimer(ret_utimer);
		if (err < 0) {
			ret_utimer = NULL;
		}
	}

	if (ret_utimer != NULL) {
		*utimer_pool_rp = utimer_pool;
	}
	raw_mutex_unlock(&utimer_pool_lock);

	return ret_utimer;
}

static int values_to_itimerspec(uint64_t expire, uint64_t interval, struct itimerspec *time)
{
	int err;
	struct timespec tmp;

	err = values_to_timespec(expire, &tmp);
	if (err == E_HM_OK) {
		time->it_value.tv_sec = tmp.tv_sec;
		time->it_value.tv_nsec = tmp.tv_nsec;

		err = values_to_timespec(interval, &tmp);
		if (err == E_HM_OK) {
			time->it_interval.tv_sec = tmp.tv_sec;
			time->it_interval.tv_nsec = tmp.tv_nsec;
		}
	}

	return err;
}

static int timer_gettime_for_utimer(const struct utimer_s *utimer, struct itimerspec *value)
{
	int err;
	__u64 cur_tocks = 0ULL;

	err = clock_gettime_values(uclock_to_posix_clock(utimer->type), &cur_tocks);
	if (err == E_HM_OK) {
		if (cur_tocks >= utimer->expire) {
			err = values_to_itimerspec(0ULL, utimer->interval, value);
		} else {
			err = values_to_itimerspec(utimer->expire - cur_tocks, utimer->interval, value);
		}
	}

	return err;
}

int utimer_gettime_kernel(int timerid, struct itimerspec *value)
{
	int err = E_HM_OK;
	struct utimer_pool_s *utimer_pool = NULL;
	struct utimer_s *utimer = NULL;
	struct itimerspec value_copy;

	mem_zero_s(value_copy);

	if (err == E_HM_OK) {
		utimer = utimer_pool_lookup_id_tree(timerid, &utimer_pool);
		if (utimer == NULL) {
			err = E_HM_INVAL;
		} else {
			/* only need to hold utimer->lock for gettime */
			raw_mutex_lock(&utimer->lock);
			err = timer_gettime_for_utimer(utimer, &value_copy);
			raw_mutex_unlock(&utimer->lock);
			put_utimer(utimer);
		}
	}

	if (err == E_HM_OK) {
		if (safe_copy((void *)value, (void *)&value_copy,
			      sizeof(struct itimerspec)) != 0) {
			err = E_HM_POSIX_FAULT;
		}
	}

	return err;
}

static int caculate_overrun_count(const struct utimer_s *utimer,
				  unsigned long long *count)
{
	int err;
	__u64 cur_time = 0ULL;

	err = clock_gettime_values(uclock_to_posix_clock(utimer->type), &cur_time);
	if (err == E_HM_OK) {
		if (cur_time > utimer->expire) {
			*count = (cur_time - utimer->expire) / utimer->interval +
				 utimer->overrun;
		} else {
			*count = utimer->overrun;
		}
	}

	return err;
}

int utimer_getoverrun_kernel(int timerid, unsigned long long *count)
{
	int err = E_HM_OK;
	struct utimer_pool_s *utimer_pool = NULL;
	struct utimer_s *utimer = NULL;

	utimer = utimer_pool_lookup_id_tree(timerid, &utimer_pool);
	if (utimer == NULL) {
		err = E_HM_INVAL;
	}

	/* only need to hold utimer->lock for getoverrun */
	if (err == E_HM_OK) {
		raw_mutex_lock(&utimer->lock);
		if (utimer->interval != 0ULL) {
			err = caculate_overrun_count(utimer, count);
		} else {
			*count = 0;
		}
		raw_mutex_unlock(&utimer->lock);
		put_utimer(utimer);
	}

	return err;
}

/* Caller should hold utimer_pool_lock and utimer->lock */
static void utimer_pool_remove_maybe_wake_nolock(
		struct utimer_pool_s *utimer_pool, struct utimer_s *utimer)
{
	if (utimer_pool->first_timer == utimer) {
		utimer_pool_remove_nolock(utimer_pool, utimer);
		utimer_pool_futex_wake(utimer_pool);
	} else {
		utimer_pool_remove_nolock(utimer_pool, utimer);
	}
}

/* Caller should hold utimer_pool_lock and utimer->lock */
static int utimer_pool_settime_value_nolock(
		struct utimer_pool_s *utimer_pool,
		struct utimer_s *utimer, int flags)
{
	int err = E_HM_OK;
	bool is_update_utimer_tree;

	/* CLOCK_REALTIME timers require special handling. If TIMER_ABSTIME is not set,
	 * they are treated in the same way as CLOCK_MONOTONIC. */
	if (utimer->clk_ori_id == CLOCK_REALTIME) {
		if (((unsigned int)flags & (unsigned int)TIMER_ABSTIME) != 0U) {
			utimer->clk_id = CLOCK_REALTIME;
			utimer->type = posix_clock_to_uclock(CLOCK_REALTIME);
		} else {
			utimer->clk_id = CLOCK_MONOTONIC;
			utimer->type = posix_clock_to_uclock(CLOCK_MONOTONIC);
		}
	}

	if (((unsigned int)flags & (unsigned int)TIMER_ABSTIME) == 0U) {
		__u64 cur_tocks = 0ULL;
		(void)clock_gettime_values(uclock_to_posix_clock(utimer->type), &cur_tocks);
		utimer->expire += cur_tocks;
	}

	is_update_utimer_tree = utimer_pool_insert_timer_nolock(utimer_pool, utimer);
	if (is_update_utimer_tree) {
		utimer_pool_futex_wake(utimer_pool);
	}

	return err;
}

static int utimer_pool_settime_nolock(
		struct utimer_pool_s *utimer_pool,
		struct utimer_s *utimer, int flags, const struct itimerspec *value)
{
	int err;

	err = timespec_to_values(&value->it_interval, &utimer->interval);
	if (err == E_HM_OK) {
		if (utimer->clk_id == CLOCK_BOOTTIME_ALARM || utimer->clk_id == CLOCK_REALTIME_ALARM) {
			if (value->it_value.tv_sec != 0 || value->it_value.tv_nsec != 0) {
				err = timespec_to_values(&value->it_value, &utimer->expire);
			}
		} else {
			err = timespec_to_values(&value->it_value, &utimer->expire);
		}
	}

	if (err == E_HM_OK) {
		if (value->it_value.tv_sec != 0 || value->it_value.tv_nsec != 0) {
			err = utimer_pool_settime_value_nolock(utimer_pool, utimer, flags);
			uimer_pool_wakeup_expired(utimer_pool);
		}
	}

	return err;
}

static int utimer_get_remaining_nolock(struct utimer_s *utimer, struct itimerspec *old)
{
	int err = E_HM_OK;
	struct itimerspec old_copy;

	mem_zero_s(old_copy);
	err = timer_gettime_for_utimer(utimer, &old_copy);
	if (err == E_HM_OK) {
		if (safe_copy((void *)old, (void *)&old_copy, sizeof(struct itimerspec)) != E_HM_OK) {
			err = E_HM_POSIX_FAULT;
		}
	}

	return err;
}

int utimer_settime_kernel(int timerid, int flags, const struct itimerspec *value, struct itimerspec *old)
{
	int err;
	struct itimerspec value_copy;
	struct utimer_pool_s *utimer_pool = NULL;
	struct utimer_s *utimer = NULL;

	mem_zero_s(value_copy);
	err = timer_settime_copy_and_check(value, &value_copy);
	if (err == E_HM_OK) {
		utimer = utimer_pool_lookup_id_tree(timerid, &utimer_pool);
		if (utimer == NULL) {
			err = E_HM_INVAL;
		}
	}

	/* need to hold utimer_pool_lock and utimer->lock for settime */
	if (err == E_HM_OK) {
		/* If the timer has already set the time, cancel the previously set */
		raw_mutex_lock(&utimer_pool_lock);
		raw_mutex_lock(&utimer->lock);
		utimer->overrun = 0ULL;
		if (utimer->state == UTIMER_ON_TREE) {
			utimer_pool_remove_maybe_wake_nolock(utimer_pool, utimer);
			utimer->state = UTIMER_ON_LIST;
		}

		if (old != NULL) {
			err = utimer_get_remaining_nolock(utimer, old);
		}

		if (utimer->state != UTIMER_DELETING) {
			err = utimer_pool_settime_nolock(utimer_pool, utimer, flags, &value_copy);
		}
		raw_mutex_unlock(&utimer->lock);
		raw_mutex_unlock(&utimer_pool_lock);
		put_utimer(utimer);
	}

	return err;
}

static int utimer_delete_nolock(struct utimer_pool_s *utimer_pool, struct utimer_s *utimer)
{
	int err = E_HM_OK;

	if (utimer->state == UTIMER_ON_TREE) {
		utimer_pool_remove_maybe_wake_nolock(utimer_pool, utimer);
	}
	if (utimer->state != UTIMER_DELETING) {
		utimer_pool_remove_id_rbtree_nolock(utimer_pool, utimer);
		utimer->state = UTIMER_DELETING;
		put_utimer(utimer);
	}

	uimer_pool_wakeup_expired(utimer_pool);

	return err;
}

#define UTIMER_DEL_RETRY_INTERVAL_US 1000
int utimer_delete_kernel_sync(int timerid)
{
	int err = E_HM_OK;
	struct utimer_pool_s *utimer_pool = NULL;
	struct utimer_s *utimer = NULL;

	utimer = utimer_pool_lookup_id_tree(timerid, &utimer_pool);
	if (utimer == NULL) {
		err = E_HM_INVAL;
	}

	/* need to hold utimer_pool_lock and utimer->lock for delete */
	if (err == E_HM_OK) {
		/* waiting for utimer handing */
		while(1) {
			raw_mutex_lock(&utimer_pool_lock);
			raw_mutex_lock(&utimer->lock);
			if (vatomic64_read(&utimer->handling) == 0) {
				break;
			}
			raw_mutex_unlock(&utimer->lock);
			raw_mutex_unlock(&utimer_pool_lock);

			hm_usleep(UTIMER_DEL_RETRY_INTERVAL_US);
		}

		err = utimer_delete_nolock(utimer_pool, utimer);

		raw_mutex_unlock(&utimer->lock);
		raw_mutex_unlock(&utimer_pool_lock);

		/* decrease refcnt increased by utimer_pool_lookup_id_tree */
		put_utimer(utimer);
	}

	return err;
}

int utimer_delete_kernel(int timerid)
{
	int err = E_HM_OK;
	struct utimer_pool_s *utimer_pool = NULL;
	struct utimer_s *utimer = NULL;

	utimer = utimer_pool_lookup_id_tree(timerid, &utimer_pool);
	if (utimer == NULL) {
		err = E_HM_INVAL;
	}

	/* need to hold utimer_pool_lock and utimer->lock for delete */
	if (err == E_HM_OK) {
		raw_mutex_lock(&utimer_pool_lock);
		raw_mutex_lock(&utimer->lock);

		err = utimer_delete_nolock(utimer_pool, utimer);

		raw_mutex_unlock(&utimer->lock);
		raw_mutex_unlock(&utimer_pool_lock);

		put_utimer(utimer);
	}

	return err;
}

int utimer_next_alarm_utimer_pid_and_name(int *pid_rp, char *name_rp, int name_size)
{
	int err = E_HM_OK;
	struct utimer_s *first_utimer = NULL;

	raw_mutex_lock(&utimer_pool_lock);
	first_utimer = g_alarm_utimer_pool->first_timer;
	if (first_utimer != NULL) {
		*pid_rp = first_utimer->owner_pid;
		if (strncpy_s(name_rp, (size_t)name_size, first_utimer->owner_name, (size_t)(name_size - 1)) != 0) {
			err = E_HM_POSIX_FAULT;
		}
	}
	raw_mutex_unlock(&utimer_pool_lock);

	return err;
}
