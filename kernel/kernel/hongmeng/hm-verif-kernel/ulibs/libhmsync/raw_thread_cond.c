/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Interfaces of raw_thread_cond
 * Author: Huawei OS Kernel Lab
 * Create: Mon Feb 18 15:11:53 2019
 */
#include <hongmeng/types.h>
#include <hongmeng/errno.h>
#include <libhmsync/raw_thread_cond.h>
#include <libhwsecurec/securec.h>
#include <libhmsrv_sys/hm_futex.h>
#include <hmkernel/attributes/hmattributes.h>

#include "raw_thread_impl.h"

#define LOCKED_HAS_WAITER 2

/*
 * struct raw_waiter
 *
 * Waiter objects have automatic storage on the waiting thread, and are used
 * in building a linked list representing waiters currently waiting on the
 * condition variable or a group of waiters woken together by a braodcast or
 * signal; in the case of signal, this is a degenerate list of one number.
 *
 * Waiter lists attached to the condition variable itself are protected by the
 * lock on the condition variable. Detached waiter lists are never modified again,
 * but can only be traversed in reverse order, and are protected by the "barrier"
 * locks in each node, which are unlocked in turn to control the wake order.
 *
 * Since process-shared condition variable semantics don't necessarily allow one
 * thread to see another's automatic storage (they may be in different processes),
 * the waiter list is not used for the process-shared case, but the structure is
 * still used to store data needed by the cancellation cleanup handler.
 */
struct raw_waiter {
	struct raw_waiter *prev, *next;
	volatile int state, barrier;
	volatile int *notify;
};

enum {
	CV_WAITING,
	CV_SIGNALED,
	CV_LEAVING,
};

#define SHARED_BIT_SHIFT 31
#define INTERRUPTIBLE_BIT_SHIFT 30

static void __raw_wait(volatile int *addr, volatile int *waiters, int val)
{
	int ret = E_HM_OK;
	int spins = 100;

	/* optimize spins */
	while ((spins-- != 0) && ((waiters == NULL) || (*waiters == 0))) {
		if (*addr != val) {
			ret = E_HM_INVAL;
			break;
		}
		raw_spin();
	}

	if (ret == E_HM_OK) {
		if (waiters != NULL) {
			(void)raw_atomic_int_inc(ptr_to_type(waiters, raw_atomic_int_t *));
		}

		/* futex wait on addr */
		while (*addr == val) {
			(void)raw_futex_wait(addr, val, FUTEX_TYPE_PRIVATE);
		}

		if (waiters != NULL) {
			(void)raw_atomic_int_dec(ptr_to_type(waiters, raw_atomic_int_t *));
		}
	}
}

/*
 * Self-synchronized-destruction-safe lock functions:
 * State:
 *   0: -> unlock state
 *   1: -> locked, no waiters.
 *   2: -> locked, has waiters.
 */
static void __raw_lock(volatile int *l)
{
	if (raw_atomic_int_cas(ptr_to_type(l, raw_atomic_int_t *), 0, 1) != 0) {
		(void)raw_atomic_int_cas(ptr_to_type(l, raw_atomic_int_t *), 1, LOCKED_HAS_WAITER);
		while (raw_atomic_int_cas(ptr_to_type(l, raw_atomic_int_t *), 0, LOCKED_HAS_WAITER) != 0) {
			__raw_wait(l, NULL, LOCKED_HAS_WAITER);
		}
	}
}

static void __raw_unlock(volatile int *l)
{
	int old = raw_atomic_int_swap(ptr_to_type(l, raw_atomic_int_t *), 0);
	if (old == LOCKED_HAS_WAITER) {
		(void)raw_futex_wake(l, 1, 1);
	}
}


static void __raw_unlock_requeue(volatile int *l, volatile int *r, int w)
{
	(void)raw_atomic_int_set(ptr_to_type(l, raw_atomic_int_t *), 0);
	if (w != 0) {
		(void)raw_futex_wake(l, 1, 1);
	} else {
		(void)raw_futex_requeue(l, 0, 1, r);
	}
}

static inline bool cond_is_shared(unsigned int attr)
{
	return ((attr >> SHARED_BIT_SHIFT) & __U(1)) != __U(0);
}

static inline bool cond_is_interruptible(unsigned int attr)
{
	return ((attr >> INTERRUPTIBLE_BIT_SHIFT) & __U(1)) != __U(0);
}

int raw_thread_cond_init(struct raw_thread_cond *cond,
			 const struct raw_thread_cond_attr *attr)
{
	int rc = E_HM_OK;

	if (cond == NULL) {
		rc = E_HM_INVAL;
	} else {
		mem_zero_s(*cond);
		if (attr != NULL) {
			/* Don't support shared condition variable now */
			cond->attr = attr->attr;
		}
	}
	return rc;
}

int raw_thread_cond_destroy(struct raw_thread_cond *cond)
{
	int rc = E_HM_OK;

	if (cond == NULL) {
		rc = E_HM_INVAL;
	} else if ((cond_is_shared(cond->attr) || cond_is_interruptible(cond->attr)) &&
		   (cond->waiters != 0)) {
		int cnt;
		/*
		 * NOTE: Add 'or' operation for atomic ops.
		 * Now we don't support shared condition variables, ignore it now.
		 * Handle: raw_atomic_int_or(cond->waiters, 0x80000000);
		 */
		(void)raw_atomic_int_inc(ptr_to_type(&cond->seq, raw_atomic_int_t *));
		(void)raw_futex_wake(&cond->seq, -1, 1);
		while (((unsigned int)(cnt = cond->waiters) & 0x7fffffffU) != 0U) {
			__raw_wait(&cond->waiters, NULL, cnt);
		}
	} else {
		/* do nothings */
	}
	return rc;
}

static int __private_raw_thread_cond_signal(struct raw_thread_cond *cond, int _n)
{
	int cur;
	volatile int ref = 0;
	struct raw_waiter *p = NULL, *first = NULL;
	int n = _n;

	if (cond == NULL) {
		__hmattr_sideff_resolved("no relative side effect");
		return E_HM_INVAL;
	}

	__raw_lock(&cond->lock);
	p = cond->tail;
	while (n != 0 && p != NULL) {
		if (raw_atomic_int_cas(ptr_to_type(&p->state, raw_atomic_int_t *),
				       CV_WAITING,
				       CV_SIGNALED) != CV_WAITING) {
			ref++;
			p->notify = &ref;
		} else {
			n--;
			if (first == NULL) {
				first = p;
			}
		}
		p = p->prev;
	}

	/* Split the list, leaving any remainder on the condvar */
	if (p != NULL) {
		if (p->next != NULL) {
			p->next->prev = NULL;
		}
		p->next = NULL;
	} else {
		cond->head = NULL;
	}
	cond->tail = p;
	__raw_unlock(&cond->lock);

	/*
	 * Wait for any waiters in the CV_LEAVING state to remove themselves
	 * from the list before returning or allowing signaled threads to proceed.
	 */
	while ((cur = ref) != 0) {
		__raw_wait(&ref, NULL, cur);
	}
	if (first != NULL) {
		__raw_unlock(&first->barrier);
	}
	return 0;
}

static inline int check_cond_args(const struct raw_thread_cond *cond,
				  const struct raw_mutex *mutex)
{
	return (cond == NULL || mutex == NULL) ? E_HM_INVAL : E_HM_OK;
}

static bool prehandle_cond(struct raw_thread_cond *cond, struct raw_waiter *node,
			   int *seq, int volatile **futex)
{
	bool shared;
	bool inter;

	BUG_ON(cond == NULL);
	BUG_ON(node == NULL);
	BUG_ON(seq == NULL);
	BUG_ON(futex == NULL);

	inter = cond_is_interruptible(cond->attr);
	shared = cond_is_shared(cond->attr);
	if (shared || inter) {
		*futex = &cond->seq;
		*seq = cond->seq;
		(void)raw_atomic_int_inc(ptr_to_type(&cond->waiters, raw_atomic_int_t *));
	} else {
		__raw_lock(&cond->lock);

		*seq = node->barrier = LOCKED_HAS_WAITER;
		*futex = &node->barrier;
		node->state = CV_WAITING;
		node->next = cond->head;
		cond->head = node;

		if (cond->tail == NULL) {
			cond->tail = node;
		} else {
			node->next->prev = node;
		}

		__raw_unlock(&cond->lock);
	}

	return shared;
}

static void handle_cv_waiting(struct raw_thread_cond *cond,
			      struct raw_waiter *node)
{
	/*
	 * Access to condvar object is valid because this waiter was not yet signaled
	 * and a new signal/broadcast can't return after seeing a CV_LEAVING waiter
	 * without getting notified via the futex notify below.
	 */
	__raw_lock(&cond->lock);

	if (cond->head == node) {
		cond->head = node->next;
	} else if (node->prev != NULL) {
		node->prev->next = node->next;
	} else {
		/* do nothing */
	}

	if (cond->tail == node) {
		cond->tail = node->prev;
	} else if (node->next != NULL) {
		node->next->prev = node->prev;
	} else {
		/* do nothing */
	}

	__raw_unlock(&cond->lock);

	if (node->notify != NULL) {
		if (raw_atomic_int_dec(ptr_to_type(node->notify, raw_atomic_int_t *)) == 0) {
			(void)raw_futex_wake(node->notify, 1, 1);
		}
	}
}

static int handle_cv_relock(struct raw_waiter *node,
			    struct raw_mutex *mutex,
			    int oldstate,
			    int _e)
{
	int e = _e;
	raw_mutex_lock(mutex);

	if (oldstate != CV_WAITING) {
		if (node->next == NULL) {
			raw_mutex_inc_waiters(mutex);
		}

		/*
		 * Unlock the barrier that's holding back the next waiter, and either wake
		 * it or requeue it to the mutex.
		 */
		if (node->prev != NULL) {
			__raw_unlock_requeue(&(node->prev->barrier), ptr_to_type(&mutex->lock, volatile int *),
					     0);
		} else {
			raw_mutex_dec_waiters(mutex);
		}

		if (e == E_HM_CANCELED) {
			e = 0;
		}
	}
	return e;
}

int raw_thread_cond_timedwait(struct raw_thread_cond *cond,
			      struct raw_mutex *mutex, const struct timespec *ts)
{
	struct raw_waiter node;
	volatile int *futex = NULL;
	int e;
	int oldstate;
	int shared;
	int seq = 0;
	int inter;
	struct __hmsigmask *tmp_mask = NULL;
	struct __hmsigmask mask = {(__u64)-1};

	if (check_cond_args(cond, mutex) != E_HM_OK) {
		__hmattr_sideff_resolved("no relative side effect");
		return E_HM_INVAL;
	}

	mem_zero_s(node);
	shared = prehandle_cond(cond, &node, &seq, &futex);
	inter = cond_is_interruptible(cond->attr);
	/* the mutex protects the cond variable wait list, unlock the mutex now */
	raw_mutex_unlock(mutex);

	if (inter == 0) {
		tmp_mask = &mask;
	}

	do {
		/* we supports CLOCK_MONOTONIC only, later will add setclock. */
		e = raw_futex_timedwait(futex, seq, CLOCK_MONOTONIC, ts, tmp_mask);
	} while (*futex == seq && ((e == 0) || (e == E_HM_INTR && inter == 0)));

	if ((e == E_HM_INTR) && (inter == 0)) {
		e = 0;
	}

	if (shared != 0 || inter != 0) {
		/*
		 * Suppress cancellation if a signal was potentially consumed;
		 * this is a legitimate from spurious wake even if not.
		 */
		if (e == E_HM_CANCELED && cond->seq != seq) {
			e = 0;
		}
		if (raw_atomic_int_dec(ptr_to_type(&cond->waiters, raw_atomic_int_t *)) ==
		    -0x7fffffff) {
			(void)raw_futex_wake(&cond->waiters, 1, 1);
		}
		oldstate = CV_WAITING;
		__hmattr_sideff_resolved("no relative side effect");
		return handle_cv_relock(&node, mutex, oldstate, e);
	}

	oldstate = raw_atomic_int_cas(ptr_to_type(&node.state, raw_atomic_int_t *), CV_WAITING,
				      CV_LEAVING);
	if (oldstate == CV_WAITING) {
		handle_cv_waiting(cond, &node);
	} else {
		/* Lock barrier first to control wake order */
		__raw_lock(&node.barrier);
	}

	return handle_cv_relock(&node, mutex, oldstate, e);
}

int raw_thread_cond_signal(struct raw_thread_cond *cond)
{
	int ret = E_HM_OK;

	if (cond == NULL) {
		ret = E_HM_INVAL;
	} else {
		if (!cond_is_shared(cond->attr) &&
		    !cond_is_interruptible(cond->attr)) {
			ret = __private_raw_thread_cond_signal(cond, 1);
		} else if (cond->waiters == 0) {
			; /* do nothings */
		} else {
			(void)raw_atomic_int_inc(ptr_to_type(&cond->seq, raw_atomic_int_t *));
			(void)raw_futex_wake(&cond->seq, 1, 1);
		}
	}
	return ret;
}

int raw_thread_cond_broadcast(struct raw_thread_cond *cond)
{
	int ret = E_HM_OK;

	if (cond == NULL) {
		ret = E_HM_INVAL;
	} else {
		if (!cond_is_shared(cond->attr) &&
		    !cond_is_interruptible(cond->attr)) {
			ret = __private_raw_thread_cond_signal(cond, -1);
		} else if (cond->waiters == 0) {
			; /* do nothings */
		} else {
			(void)raw_atomic_int_inc(ptr_to_type(&cond->seq, raw_atomic_int_t *));
			(void)raw_futex_wake(&cond->seq, -1, 1);
		}
	}
	return ret;
}

int raw_thread_cond_wait(struct raw_thread_cond *cond, struct raw_mutex *mutex)
{
	return (cond == NULL || mutex == NULL) ? E_HM_INVAL :
	       raw_thread_cond_timedwait(cond, mutex, NULL);
}
