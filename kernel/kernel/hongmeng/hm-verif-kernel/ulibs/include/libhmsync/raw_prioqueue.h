/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Interfaces of priority queue
 * Author: Huawei OS Kernel Lab
 * Create: Fri Mar 29 14:11:53 2019
 */

#ifndef LIBHMSYNC_RAW_PRIOQUEUE_H
#define LIBHMSYNC_RAW_PRIOQUEUE_H

#include <lib/dlist.h>
#include <libhmsync/bitops.h>

#define RAW_PRIOQUEUE_MAX 256U
#define PRIO_TO_BITS(prio, nr_queues) (((nr_queues) - (prio)) - 1U)
#define BITS_TO_PRIO(bits, nr_queues) (((nr_queues) - (bits)) - 1U)

struct raw_prioqueue {
	unsigned int nr_queues;

	unsigned long long *bitmap;
	struct dlist_node *queues;
};

static inline int raw_prioqueue_is_empty(struct raw_prioqueue *queue)
{
	unsigned int bits = raw_bitmap_find_first_one(queue->bitmap, queue->nr_queues);
	return (bits == queue->nr_queues);
}

static inline int raw_prioqueue_has_prio_work(struct raw_prioqueue *queue,
					      unsigned int prio)
{
	unsigned int bits = raw_bitmap_find_first_one(queue->bitmap, queue->nr_queues);
	return (bits != queue->nr_queues &&
		BITS_TO_PRIO(bits, queue->nr_queues) >= prio);
}

int raw_prioqueue_init(struct raw_prioqueue *queue, unsigned int nr_prio);
int raw_prioqueue_enqueue(struct raw_prioqueue *queue, unsigned int prio,
			  struct dlist_node *node);
struct dlist_node *raw_prioqueue_dequeue(struct raw_prioqueue *queue,
					 unsigned int *p_prio);
int raw_prioqueue_remove(struct raw_prioqueue *queue, unsigned int prio,
			 struct dlist_node *node);
void raw_prioqueue_destroy(struct raw_prioqueue *queue);
int raw_prioqueue_is_queued(struct raw_prioqueue *queue, unsigned int prio,
			    const struct dlist_node *node);

#endif
