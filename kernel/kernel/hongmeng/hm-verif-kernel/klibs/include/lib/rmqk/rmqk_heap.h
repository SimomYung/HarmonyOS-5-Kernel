/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: RMQ
 * Author: Huawei OS Kernel Lab
 * Create: Wed Sep 20 17:13:09 2023
 */
#ifndef KLIBS_TRACE_RMQK_CONSUMER_SORTED_HEAP_H
#define KLIBS_TRACE_RMQK_CONSUMER_SORTED_HEAP_H

#define RMQ_HEAP_MAX_SIZE			RMQ_ACTIVE_CAPSULE_NUM

typedef struct rmp_heap_item_s {
	__u64	time_stamp;
	__u8	index;
} rmp_heap_item_t;

typedef struct rmq_heap_s {
	__u8	size;
	rmp_heap_item_t items[RMQ_HEAP_MAX_SIZE];
} rmq_heap_t;

#define RMQ_HEAP_PARENT(i)		(((i) - 1) >> 1)
#define RMQ_HEAP_LCHILD(i)		(((i) << 1) + 1)
#define RMQ_HEAP_RCHILD(i)		(((i) + 1) << 1)

static inline void
__rmq_heap_exchange(rmp_heap_item_t *a, rmp_heap_item_t *b)
{
	rmp_heap_item_t tmp = *a;
	*a = *b;
	*b = tmp;
}

static inline void
__rmq_heap_init(rmq_heap_t *h)
{
	h->size = 0;
}

static inline rmp_heap_item_t *
__rmq_heap_item_at(rmq_heap_t *h, __u8 idx)
{
	return &h->items[idx];
}

static inline rmp_heap_item_t *
__rmq_heap_alloc(rmq_heap_t *h)
{
	BUG_ON(h->size == RMQ_ACTIVE_CAPSULE_NUM);
	return __rmq_heap_item_at(h, h->size);
}

static inline void
__rmq_heap_insert(rmq_heap_t *h)
{
	BUG_ON(h->size == RMQ_ACTIVE_CAPSULE_NUM);
	__u8 current = h->size;
	h->size++;
	while (true) {
		if (current <= 0) {
			break;
		}
		__u8 parent = RMQ_HEAP_PARENT(current);
		rmp_heap_item_t *c = &h->items[current];
		rmp_heap_item_t *p = &h->items[parent];
		if (p->time_stamp <= c->time_stamp) {
			break;
		}
		__rmq_heap_exchange(p, c);
		current = RMQ_HEAP_PARENT(current);
	}
}

static inline rmp_heap_item_t *
__rmq_heap_peek(rmq_heap_t *h)
{
	return h->size != 0U ? __rmq_heap_item_at(h, 0) : NULL;
}

static inline void
__rmq_heap_update_head(rmq_heap_t *h)
{
	__u8 curr = 0;
	while (true) {
		__u8 left = RMQ_HEAP_LCHILD(curr);
		__u8 right = RMQ_HEAP_RCHILD(curr);
		__u8 largest = curr;
		if (left < h->size && h->items[left].time_stamp < h->items[largest].time_stamp) {
			largest = left;
		}
		if (right < h->size && h->items[right].time_stamp < h->items[largest].time_stamp) {
			largest = right;
		}
		if (largest == curr) {
			break;
		}
		__rmq_heap_exchange(&h->items[curr], &h->items[largest]);
		curr = largest;
	}
}

static inline void
__rmq_heap_delete(rmq_heap_t *h)
{
	BUG_ON(h->size == 0);
	h->size--;
	__rmq_heap_exchange(&h->items[0], &h->items[h->size]);
	__rmq_heap_update_head(h);
}
#endif
