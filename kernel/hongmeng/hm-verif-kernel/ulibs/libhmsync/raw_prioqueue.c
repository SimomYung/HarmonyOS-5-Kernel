/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Interfaces of priority queue
 * Author: Huawei OS Kernel Lab
 * Create: Fri Mar 29 14:11:53 2019
 */

#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>
#include <libhmsync/bitops.h>
#include <libstrict/overflow.h>
#include <libhwsecurec/securec.h>
#include <libhmsync/raw_prioqueue.h>

int raw_prioqueue_init(struct raw_prioqueue *queue, unsigned int nr_prio)
{
	int rc = E_HM_OK;
	unsigned int i, longlong_sz;
	unsigned long long *bitmap = NULL;
	struct dlist_node *queues = NULL;
	size_t bitmap_sz = 0;
	size_t queues_sz = 0;
	size_t sizeof_dlist_node, sizeof_ull;

	if (queue == NULL || nr_prio > RAW_PRIOQUEUE_MAX) {
		rc = E_HM_INVAL;
	}

	if (rc == E_HM_OK) {
		sizeof_dlist_node = sizeof(struct dlist_node);
		sizeof_ull = sizeof(unsigned long long);
		longlong_sz = (unsigned int)BITS_TO_LONGLONGS(nr_prio);

		noovfl_umul(queues_sz, nr_prio, sizeof_dlist_node, "queue_sz is overflow");
		noovfl_umul(bitmap_sz, longlong_sz, sizeof_ull, "bitmap_sz is overflow");

		if (queues_sz == 0U || bitmap_sz == 0U) {
			rc = E_HM_INVAL;
		}
	}

	if (rc == E_HM_OK) {
		bitmap = malloc(bitmap_sz);
		if (bitmap == NULL) {
			hm_error("failed to malloc bitmap for prioqueue\n");
			rc = E_HM_NOMEM;
		}
	}

	if (rc == E_HM_OK) {
		queues = malloc(queues_sz);
		if (queues == NULL) {
			free(bitmap);
			hm_error("failed to malloc queue for prioqueue\n");
			rc = E_HM_NOMEM;
		} else {
			mem_zero_b(bitmap, bitmap_sz);
			for (i = 0; i < nr_prio; i++) {
				dlist_init(&queues[i]);
			}

			mem_zero_s(*queue);
			queue->bitmap = bitmap;
			queue->queues = queues;
			queue->nr_queues = nr_prio;
		}
	}
	return rc;
}

int raw_prioqueue_enqueue(struct raw_prioqueue *queue, unsigned int prio,
			  struct dlist_node *node)
{
	unsigned int bits;
	int rc = E_HM_OK;

	if (queue == NULL || node == NULL || prio >= queue->nr_queues) {
		rc = E_HM_INVAL;
	} else {
		BUG_ON(queue->bitmap == NULL);
		BUG_ON(queue->queues == NULL);

		bits = PRIO_TO_BITS(prio, queue->nr_queues);
		NOFAIL(raw_bitmap_set_bit(queue->bitmap, queue->nr_queues, bits));
		dlist_insert_tail(&queue->queues[prio], node);
	}
	return rc;
}

struct dlist_node *raw_prioqueue_dequeue(struct raw_prioqueue *queue,
					 unsigned int *p_prio)
{
	unsigned int bits;
	unsigned int prio;
	struct dlist_node *head = NULL, *node = NULL;

	if (queue != NULL) {
		BUG_ON(queue->bitmap == NULL);
		BUG_ON(queue->queues == NULL);

		bits = raw_bitmap_find_first_one(queue->bitmap, queue->nr_queues);
		if (bits != queue->nr_queues) {
			prio = BITS_TO_PRIO(bits, queue->nr_queues);
			head = &queue->queues[prio];
			node = dlist_get_first(head);
			BUG_ON(node == NULL);
			dlist_delete(node);

			if (dlist_empty(head)) {
				NOFAIL(raw_bitmap_clear_bit(queue->bitmap,
							    queue->nr_queues, bits));
			}
			if (p_prio != NULL) {
				*p_prio = prio;
			}
		}
	}
	return node;
}

int raw_prioqueue_is_queued(struct raw_prioqueue *queue, unsigned int prio,
			    const struct dlist_node *node)
{
	int found = 0;
	struct dlist_node *head = NULL, *pos = NULL;

	if (queue == NULL || node == NULL || prio >= queue->nr_queues) {
		found = E_HM_INVAL;
	} else {
		BUG_ON(queue->bitmap == NULL);
		BUG_ON(queue->queues == NULL);

		head = &queue->queues[prio];
		dlist_for_each(pos, head) {
			if ((const struct dlist_node *)pos == node) {
				found = 1;
				break;
			}
		}
	}
	return found;
}

int raw_prioqueue_remove(struct raw_prioqueue *queue, unsigned int prio,
			 struct dlist_node *node)
{
	int found = 0;

	if (queue == NULL || node == NULL || prio >= queue->nr_queues) {
		found = E_HM_INVAL;
	} else {
		BUG_ON(queue->bitmap == NULL);
		BUG_ON(queue->queues == NULL);

		found = raw_prioqueue_is_queued(queue, prio, node);
		if (found > 0) {
			dlist_delete(node);
			if (dlist_empty(&queue->queues[prio])) {
				NOFAIL(raw_bitmap_clear_bit(queue->bitmap, queue->nr_queues, PRIO_TO_BITS(prio, queue->nr_queues)));
			}
			found = E_HM_OK;
		} else {
			found = E_HM_NOOBJ;
		}
	}
	return found;
}

void raw_prioqueue_destroy(struct raw_prioqueue *queue)
{
	if (queue != NULL) {
		if (queue->bitmap != NULL) {
			free(queue->bitmap);
		}
		if (queue->queues != NULL) {
			free(queue->queues);
		}
		queue->queues = NULL;
		queue->nr_queues = 0;
		queue->bitmap = NULL;
	}
}
