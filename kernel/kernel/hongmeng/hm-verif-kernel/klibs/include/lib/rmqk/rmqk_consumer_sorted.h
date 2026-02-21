/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Relaxed Multi-level Queue (RMQ)
 * Author: Huawei OS Kernel Lab
 * Create: Fri Sep 01 09:47:00 2023
 */
#ifndef KLIBS_RMQK_CONSUMER_SORTED_H
#define KLIBS_RMQK_CONSUMER_SORTED_H
#if RMQ_CONSUMER_MODE == RMQ_CONSUMER_MODE_CLOSING
#if RMQ_CONSUMER_SORTED_USING_HEAP
typedef struct rmq_consumer_sorted_s {
	rmq_heap_t					heap;
	rmq_entry_t *					entries[RMQ_ACTIVE_CAPSULE_NUM];
	rmq_consumer_buffer_batch_t			cbbs[RMQ_ACTIVE_CAPSULE_NUM];
} RMQ_ATTR_CACHELINE_ALIGNED rmq_consumer_sorted_t;

static inline void
rmq_consumer_sorted_init(rmq_consumer_sorted_t *cs)
{
	rmq_heap_t *h = &cs->heap;
	__rmq_heap_init(h);
	for (__u8 i = 0; i < RMQ_ACTIVE_CAPSULE_NUM; i++) {
		cs->entries[i] = NULL;
		rmp_heap_item_t *item = __rmq_heap_item_at(h, i);
		item->index = i;
	}
}

static inline void
rmq_consumer_sorted_check_heap(rmq_t *q, rmq_consumer_sorted_t *cs)
{
	rmq_consumer_t *c = &q->consumer;
	rmq_heap_t *h = &cs->heap;
	while (h->size < RMQ_HEAP_MAX_SIZE) {
		rmp_heap_item_t *item = __rmq_heap_alloc(h);
		__u8 i = item->index;
		rmq_consumer_buffer_batch_t *cbb = &cs->cbbs[i];
		while (true) {
			if (!__rmq_consumer_advance(q, c, cbb, RMQ_COPY_TO_LOCAL_MEM)) {
				return;
			}
			cbb->head.length += (__uptr_t)cbb->data;
			rmq_entry_t *e = __rmq_entry_get(cbb->data, 0);
			if (!(e->size == 0 || ((__uptr_t)e) > cbb->head.length)) {
				item->time_stamp = cbb->head.time_stamp + e->time_stamp_offset;
				cs->entries[i] = e;
				__rmq_heap_insert(h);
				break;
			}
		}
	}
}

static inline __s64
rmq_dequeue_sorted(rmq_t *q, rmq_consumer_sorted_t *cs, rmq_consumer_entry_t *ce)
{
	rmq_heap_t *h = &cs->heap;
	rmq_consumer_sorted_check_heap(q, cs);
	rmp_heap_item_t *item = __rmq_heap_peek(h);
	if (!item) {
		return RMQ_FAIL;
	}
	__u8 i = item->index;

	rmq_consumer_buffer_batch_t *cbb = &cs->cbbs[i];
	rmq_entry_t *e = cs->entries[i];
	ce->addr = (void *)e->data;
	ce->time_stamp = item->time_stamp;
	ce->core_id = cbb->head.core_id;

	rmq_entry_t *ne = __rmq_entry_get(e, __rmq_entry_total_size(e->size));
	if (!(ne->size == 0 || ((__uptr_t)ne) > cbb->head.length)) {
		item->time_stamp = RMQ_MAX2(item->time_stamp, cbb->head.time_stamp + ne->time_stamp_offset);
		cs->entries[i] = ne;
		__rmq_heap_update_head(h);
	} else {
		__rmq_heap_delete(h);
	}
	return e->size;
}
#else
typedef struct rmq_consumer_sorted_s {
	__u64						time_stamps[RMQ_ACTIVE_CAPSULE_NUM];
	rmq_entry_t *					entries[RMQ_ACTIVE_CAPSULE_NUM];
	rmq_consumer_buffer_batch_t			cbbs[RMQ_ACTIVE_CAPSULE_NUM];
} RMQ_ATTR_CACHELINE_ALIGNED rmq_consumer_sorted_t;

static inline bool
__rmq_consumer_sorted_fill_entry(rmq_consumer_buffer_batch_t *cbb, __u64 *ts, rmq_entry_t **entry)
{
	rmq_entry_t *e = (*entry == NULL)
		? __rmq_entry_get(cbb->data, 0)
		: __rmq_entry_get(*entry, __rmq_entry_total_size((*entry)->size));

	if (e->size == 0 || ((__uptr_t)e) > cbb->head.length) {
		*ts = RMQ_UINT64_MAX;
		*entry = NULL;
		return false;
	} else {
		*ts = cbb->head.time_stamp + e->time_stamp_offset;
		*entry = e;
		return true;
	}
}

static inline void
__rmq_consumer_sorted_fill_cbb(rmq_t *q, rmq_consumer_sorted_t *cs)
{
	rmq_consumer_t *c = &q->consumer;
	for (__u64 i = 0; i < RMQ_ACTIVE_CAPSULE_NUM; i++) {
		if (cs->entries[i]) {
			continue;
		}
		rmq_consumer_buffer_batch_t *cbb = &cs->cbbs[i];
		while (true) {
			if (!__rmq_consumer_advance(q, c, cbb, RMQ_COPY_TO_LOCAL_MEM)) {
				return;
			}
			cbb->head.length += (__uptr_t)cbb->data;
			if (__rmq_consumer_sorted_fill_entry(cbb, &cs->time_stamps[i], &cs->entries[i])) {
				break;
			}
		}
	}
}

static inline __u64
__rmq_consumer_sorted_find_which(rmq_t *q, rmq_consumer_sorted_t *cs)
{
	__u64 idx = RMQ_ACTIVE_CAPSULE_NUM;
	__u64 time_stamp = RMQ_UINT64_MAX;
	for (__u64 i = 0; i < RMQ_ACTIVE_CAPSULE_NUM; i++) {
		if (cs->time_stamps[i] < time_stamp) {
			time_stamp = cs->time_stamps[i];
			idx = i;
		}
	}
	return idx;
}

static inline void
rmq_consumer_sorted_init(rmq_consumer_sorted_t *cs)
{
	for (__u64 i = 0; i < RMQ_ACTIVE_CAPSULE_NUM; i++) {
		cs->time_stamps[i] = RMQ_UINT64_MAX;
		cs->entries[i] = NULL;
	}
}

static inline __s64
rmq_dequeue_sorted(rmq_t *q, rmq_consumer_sorted_t *cs, rmq_consumer_entry_t *ce)
{
	__rmq_consumer_sorted_fill_cbb(q, cs);
	__u64 i = __rmq_consumer_sorted_find_which(q, cs);
	if (i == RMQ_ACTIVE_CAPSULE_NUM) {
		return RMQ_FAIL;
	}

	rmq_consumer_buffer_batch_t *cbb = &cs->cbbs[i];
	rmq_entry_t *e = cs->entries[i];
	ce->addr = (void *)e->data;
	ce->time_stamp = cs->time_stamps[i];
	ce->core_id = cbb->head.core_id;

	if (__rmq_consumer_sorted_fill_entry(cbb, &cs->time_stamps[i], &cs->entries[i])) {
		cs->time_stamps[i] = RMQ_MAX2(cs->time_stamps[i], ce->time_stamp);
	}
	return e->size;
}
#endif
#endif
#endif /* KLIBS_RMQK_CONSUMER_SORTED_H */
