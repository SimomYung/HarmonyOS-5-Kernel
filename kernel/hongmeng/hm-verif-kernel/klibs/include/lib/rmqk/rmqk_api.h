/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Relaxed Multi-level Queue (RMQ)
 * Author: Huawei OS Kernel Lab
 * Create: Fri Sep 01 09:47:00 2023
 */
#ifndef KLIBS_RMQK_API_H
#define KLIBS_RMQK_API_H

static inline void
rmq_init(void *buf, __u64 buf_size)
{
	rmq_t *q = (rmq_t *)buf;
	q->info.memsize = buf_size;

	BUG_ON(RMQ_CORE_NUM >= RMQ_UINT8_MAX);
	BUG_ON(RMQ_ACTIVE_CAPSULE_NUM < RMQ_CORE_NUM);
	BUG_ON(RMQ_CAPSULE_SIZE != RMQ_ALIGN_DOWN_WITH_POWER2(RMQ_CAPSULE_SIZE));
	BUG_ON(buf_size < RMQ_METADATA_SIZE);
	__s64 ratio = __rmq_resize_calculate_ratio(buf_size);
	BUG_ON(ratio < (__s64)RMQ_RATIO_MINIMUM);

	vatomic64_init(&q->global.resize.flag, 0);
	q->global.resize.callback.func = NULL;
	vatomic64_init(&q->global.ratio_and_gpos, RMQ_VARIABLE_COMPOSE((__u64)ratio, RMQ_ACTIVE_CAPSULE_NUM - 1));
	vatomic32_init(&q->info.record_disabled, 0);
	vatomic32_init(&q->info.snapshot, 0);
	vatomic32_init(&q->info.snapshot_allocated, 0);
	vatomic32_init(&q->info.clock_id, TRACE_CLOCK_BOOT);
	vatomic32_init(&q->info.tracing_block_enabled, 0);
	vatomic32_init(&q->global.dequeue_futex, 1);
	vatomic32_init(&q->global.dequeue_futex_waiters, 0);

	for (__u64 idx = 0; idx < RMQ_CORE_NUM; idx++) {
		rmq_producer_t *p = &q->producer[idx];
		__rmq_producer_init(p, (__u64)ratio, RMQ_ACTIVE_CAPSULE_NUM - RMQ_CORE_NUM + idx);
	}

	rmq_consumer_t *c = &q->consumer;
	__rmq_consumer_init(c);

	for (__u64 idx = 0; idx < RMQ_ACTIVE_CAPSULE_NUM; idx++) {
		rmq_metadata_t *mc = &q->metadata[idx];
		__rmq_capsule_metadata_init(mc, idx);
	}

	for (__u64 i = 0; i < RMQ_ACTIVE_CAPSULE_NUM - RMQ_CORE_NUM; i++) {
		rmq_data_t *dc = &q->data[i];
		__rmq_datahead_init(&dc->head, 0, 0);
	}

	for (__u64 i = 0; i < RMQ_CORE_NUM; i++) {
		__u64 idx = i + RMQ_ACTIVE_CAPSULE_NUM - RMQ_CORE_NUM;
		rmq_data_t *dc = &q->data[idx];
		__rmq_datahead_init(&dc->head, idx, (__u8)i);
	}

	for (__u64 i = RMQ_ACTIVE_CAPSULE_NUM; i < (__u64)ratio * RMQ_ACTIVE_CAPSULE_NUM; i++) {
		rmq_data_t *dc = &q->data[i];
		__rmq_datahead_init(&dc->head, 0, 0);
	}
}

static inline void
rmq_enqueue_make_cache(rmq_t *q, __u8 cid, rmq_producer_cache_t *cache)
{
	BUG_ON(cid >= RMQ_CORE_NUM);
	rmq_producer_t *p = &q->producer[cid];
	__u64 ratio_and_ppos = vatomic64_read(&p->ratio_and_ppos);
	__rmq_producer_cache_update(q, cache, ratio_and_ppos);
}

static inline __s64
rmq_enqueue_get_entry_by_cache(rmq_t *q, __u8 cid, rmq_producer_cache_t *cache, rmq_producer_entry_t *pe, __u64 size)
{
	BUG_ON(cid >= RMQ_CORE_NUM);
	__s64 result;
	while (true) {
		result = __rmq_producer_get_entry_by_cache(q, cid, cache, pe, size);
		if (result == RMQ_FAIL) {
			rmq_producer_t *p = &q->producer[cid];
			__u64 ratio_and_ppos = vatomic64_read(&p->ratio_and_ppos);
			if (ratio_and_ppos != cache->ratio_and_ppos_last) {
				__rmq_producer_cache_update(q, cache, ratio_and_ppos);
				continue;
			}
			__rmq_shrink_heartbeat(q);
			if (!__rmq_producer_advance(q, p, cid, ratio_and_ppos)) {
				break;
			}
		} else {
			break;
		}
	}
	return result;
}

static inline __s64
rmq_enqueue_get_entry(rmq_t *q, __u8 cid, rmq_producer_entry_t *pe, __u64 size)
{
	rmq_producer_cache_t cache;
	rmq_enqueue_make_cache(q, cid, &cache);
	return rmq_enqueue_get_entry_by_cache(q, cid, &cache, pe, size);
}

static inline void
rmq_enqueue_confirm_entry(rmq_producer_entry_t *pe)
{
	__rmq_producer_confirm_entry(pe);
}

static inline __s64
rmq_enqueue(rmq_t *q, __u8 cid, void *buf, __u64 size)
{
	rmq_producer_entry_t pe;
	__s64 result = rmq_enqueue_get_entry(q, cid, &pe, size);
	if ((__u64)result == size) {
#ifdef __KERNEL__
		memcpy(pe.addr, buf, size);
#else
		NOFAIL(memcpy_s(pe.addr, size, buf, size));
#endif
		rmq_enqueue_confirm_entry(&pe);
	}
	return result;
}

static inline __s64
rmq_dequeue_get_entry(rmq_t *q, rmq_consumer_entry_t *ce)
{
	rmq_consumer_t *c = &q->consumer;
	__s64 result;
	while (true) {
		result = __rmq_consumer_get_entry(q, c, ce);
		if (result == RMQ_FAIL) {
			if (!__rmq_consumer_advance(q, c, NULL, RMQ_COPY_TO_LOCAL_MEM)) {
				break;
			}
		} else {
			break;
		}
	}
	return result;
}

static inline bool
rmq_dequeue_batch(rmq_t *q, rmq_consumer_buffer_batch_t *cbb)
{
	rmq_consumer_t *c = &q->consumer;
	return __rmq_consumer_advance(q, c, cbb, RMQ_COPY_TO_USER);
}

static inline void
rmq_dequeue_confirm_entry(rmq_consumer_entry_t *ce)
{
	__rmq_consumer_confirm_entry(ce);
}

static inline __s64
rmq_dequeue(rmq_t *q, void *buf, __u64 size)
{
	rmq_consumer_entry_t ce;
	__s64 result = rmq_dequeue_get_entry(q, &ce);
	if (result != RMQ_FAIL) {
		rmq_entry_t *e = __rmq_entry_get(ce.addr, -RMQ_ENTRY_METADATA_SIZE);
		rmq_consumer_buffer_t *cb = (rmq_consumer_buffer_t *)buf;
		if (size < (sizeof(rmq_consumer_buffer_t) + e->size)) {
			return RMQ_INVALID_SIZE;
		}
		cb->time_stamp = ce.time_stamp;
		cb->core_id = ce.core_id;
#ifdef __KERNEL__
		memcpy(cb->data, ce.addr, size);
#else
		NOFAIL(memcpy_s(cb->data, size, ce.addr, size));
#endif
		rmq_dequeue_confirm_entry(&ce);
	}
	return result;
}

static inline void
rmq_consumer_backward(rmq_t *q)
{
	rmq_consumer_t *c = &q->consumer;
	rmq_consumer_last_t *cl = &q->consumer.last;
	c->cpos = cl->cpos;
	c->consumed_idx = cl->consumed_idx;
	c->reserved_idx = cl->consumed_idx;
	c->data.head.producer_stamp = cl->producer_stamp;
	c->data.head.consumer_stamp = cl->consumer_stamp;
}

static inline void
rmq_consumer_forward(rmq_t *q)
{
	rmq_consumer_t *c = &q->consumer;
	rmq_consumer_last_t *cl = &q->consumer.last;
	cl->cpos = c->cpos;
	cl->consumed_idx = c->consumed_idx;
	cl->producer_stamp = c->data.head.producer_stamp;
	cl->consumer_stamp = c->data.head.consumer_stamp;
}

static inline void
rmq_consumer_sweep(rmq_t *q)
{
	bool ret;
	rmq_consumer_t *c = &q->consumer;
	__u64 ratio_and_gpos = vatomic64_read(&q->global.ratio_and_gpos);
	__u64 prod_newest = RMQ_VARIABLE_POS(ratio_and_gpos);

	while (true) {
		ret = __rmq_consumer_advance(q, c, NULL, RMQ_NO_COPY);
		if ((!ret) || (c->cpos == prod_newest)) {
			break;
		}
	}
	c->consumed_idx = c->reserved_idx;
	rmq_consumer_forward(q);
}

static inline __u64
rmq_get_memory_usage(rmq_t *q)
{
	return q->info.memsize;
}

static inline bool
rmq_resize_set_callback(rmq_t *q, void (*f)(void *), void *args, __u64 size)
{
	if (size > RMQ_RESIZE_SHRINK_CALLBACK_ARGS_SIZE) {
		return false;
	}
#if RMQ_RESIZE_SHRINK_CALLBACK_STATIC
	BUG_ON(f != NULL);
#else
	if (!f) {
		return false;
	}
	q->global.resize.callback.func = f;
#endif
#ifdef __KERNEL__
	memcpy(q->global.resize.callback.args, args, size);
#else
	NOFAIL(memcpy_s(q->global.resize.callback.args, size, args, size));
#endif
	return true;
}

static inline bool
rmq_resize_check(rmq_t *q, __u64 new_size)
{
	__s64 next_ratio = __rmq_resize_calculate_ratio(new_size);
	if (next_ratio < (__s64) RMQ_RATIO_MINIMUM) {
		return false;
	}
	if (next_ratio > (__s64) RMQ_RATIO_MAXIMUM) {
		return false;
	}
	__u64 flag = vatomic64_read(&q->global.resize.flag);
	__u64 state = RMQ_RESIZE_STATE(flag);
	if (RMQ_RESIZE_SHRINK_ING(state) != 0ULL) {
		return false;
	}
	return true;
}

static inline __u64
rmq_resize(rmq_t *q, __u64 new_size)
{
	q->info.memsize = new_size;

	__u64 next_ratio = (__u64)__rmq_resize_calculate_ratio(new_size);
	__u64 ratio_and_gpos = vatomic64_read(&q->global.ratio_and_gpos);
	__u64 curr_ratio = RMQ_VARIABLE_RATIO(ratio_and_gpos);
	if (next_ratio < curr_ratio) {
		__rmq_shrink(q, ratio_and_gpos, curr_ratio, next_ratio);
		return RMQ_RESIZE_SHRINK;
	} else if (next_ratio > curr_ratio) {
		__rmq_expand(q, ratio_and_gpos, curr_ratio, next_ratio);
		return RMQ_RESIZE_EXPAND;
	} else {
		return RMQ_RESIZE_UNCHANGED;
	}
}

static inline bool
rmq_is_record_enabled(rmq_t *q)
{
	return vatomic32_read(&(q->info.record_disabled)) == 0U;
}

static inline void
rmq_enable_disable_record(rmq_t *q, bool enable)
{
	if (enable) {
		vatomic32_dec(&(q->info.record_disabled));
	} else {
		vatomic32_inc(&(q->info.record_disabled));
	}
}

static inline bool
rmq_get_snapshot_enabled(rmq_t *q)
{
	return vatomic32_read(&(q->info.snapshot));
}

static inline void
rmq_set_snapshot_enabled(rmq_t *q, bool enabled)
{
	vatomic32_write(&(q->info.snapshot), enabled);
}

static inline bool
rmq_get_snapshot_allocated(rmq_t *q)
{
	return vatomic32_read(&(q->info.snapshot_allocated));
}

static inline void
rmq_set_snapshot_allocated(rmq_t *q, bool allocate)
{
	vatomic32_write(&(q->info.snapshot_allocated), allocate);
}

static inline int
rmq_clock_id_of(rmq_t *q)
{
	return (int)vatomic32_read(&(q->info.clock_id));
}

static inline bool
rmq_set_clock_id(rmq_t *q, int clock_id, bool sweep)
{
	if (clock_id < 0 || clock_id >= TRACE_CLOCK_NUM) {
		return false;
	} else {
		rmq_enable_disable_record(q, false);
		vatomic32_write(&(q->info.clock_id), (unsigned int)clock_id);
		if (sweep == true) {
			rmq_consumer_sweep(q);
		}
		rmq_enable_disable_record(q, true);
		return true;
	}
}

static inline bool
rmq_is_tracing_block_enabled(rmq_t *q)
{
	return (bool)vatomic32_read(&(q->info.tracing_block_enabled));
}

static inline void
rmq_set_tracing_block_enabled(rmq_t *q, bool enable)
{
	vatomic32_write(&(q->info.tracing_block_enabled), (unsigned int)enable);
}
#endif
