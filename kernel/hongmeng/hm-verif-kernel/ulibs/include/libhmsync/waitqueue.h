/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Desctiption: Definitions about wait_queue
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jul 03 10:27:40 2020
 */
#ifndef SYSMGR_INCLUDE_AAPT_HMSYSMGR_WAITQUEUE_H
#define SYSMGR_INCLUDE_AAPT_HMSYSMGR_WAITQUEUE_H

#include <lib/dlist.h>
#include <libhmsync/raw_mutex.h>
#include <libhmsrv_sys/hm_futex.h>

struct wait_queue_head {
	struct raw_mutex mutex;
	struct dlist_node head;
};

/*
 * struct wait_queue - wait queue structure
 * @val:	used in futex
 * @private:	points to target data
 * @node:	list node
 */
struct wait_queue_node {
	volatile int val;
	void *private;
	struct dlist_node node;
};

static inline bool wait_queue_empty(struct wait_queue_head *waiter_head)
{
	bool empty;

	raw_mutex_lock(&waiter_head->mutex);
	empty = dlist_empty(&waiter_head->head);
	raw_mutex_unlock(&waiter_head->mutex);

	return empty;
}

static inline void wait_queue_add(struct wait_queue_head *waiter_head,
				  struct wait_queue_node *waiter)
{
	raw_mutex_lock(&waiter_head->mutex);
	dlist_insert_tail(&waiter_head->head, &waiter->node);
	raw_mutex_unlock(&waiter_head->mutex);
}

static inline void wait_queue_remove(struct wait_queue_head *waiter_head,
				     struct wait_queue_node *waiter)
{
	raw_mutex_lock(&waiter_head->mutex);
	dlist_delete(&waiter->node);
	raw_mutex_unlock(&waiter_head->mutex);
}

static inline int wait_queue_wait(struct wait_queue_node *waiter,
				  const struct timespec *ts)
{
	return hm_futex_wait_interruptible(&waiter->val, 0, ts, 0);
}

static inline int wait_queue_wake(struct wait_queue_node *waiter)
{
	waiter->val = 1;
	return hm_futex_wake(&waiter->val, 1, 0);
}

#define wait_queue_for_each_waiter(waiter, n, parent)		\
	dlist_for_each_entry_safe((waiter), (n),		\
				  &(parent)->wait_list.head,	\
				  struct wait_queue_node, node)	\

#define waitqueue_declare(name, object)			\
	struct wait_queue_node name = {			\
		.val = 0,				\
		.private = (object),			\
		.node = {NULL, NULL}			\
	};						\

#endif
