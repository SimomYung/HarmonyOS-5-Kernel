/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Relaxed Multi-level Queue (RMQ)
 * Author: Huawei OS Kernel Lab
 * Create: Fri Sep 01 09:47:00 2023
 */
#ifndef KLIBS_TRACE_RMQK_PRODUCER_H
#define KLIBS_TRACE_RMQK_PRODUCER_H

static inline void
__rmq_producer_init(rmq_producer_t *p, __u64 ratio, __u64 ppos)
{
	__u64 ratio_and_ppos = RMQ_VARIABLE_COMPOSE(ratio, ppos);
	vatomic64_init(&p->ratio_and_ppos, ratio_and_ppos);
}

static inline void
__rmq_producer_cache_update(rmq_t *q, rmq_producer_cache_t *cache, __u64 ratio_and_ppos)
{
	__u64 ppos = RMQ_VARIABLE_POS(ratio_and_ppos);
	__u64 nc = RMQ_VARIABLE_RATIO(ratio_and_ppos) * RMQ_ACTIVE_CAPSULE_NUM;
	cache->ratio_and_ppos_last = ratio_and_ppos;
	cache->rnd = ppos / RMQ_ACTIVE_CAPSULE_NUM + 1;
	cache->mc = &q->metadata[ppos % RMQ_ACTIVE_CAPSULE_NUM];
	cache->dc = &q->data[ppos % nc];
}

static inline void
__rmq_producer_meta_update(rmq_data_t *dc, rmq_metadata_t *mc, __u64 nppos, __u8 cid, __u64 time_stamp)
{
	vatomic_fence_rel();
	dc->head.time_stamp = time_stamp;
	dc->head.producer_stamp = nppos;
	dc->head.consumer_stamp = nppos;
	dc->head.core_id = cid;
}

static inline __u64
__rmq_producer_inactivate(rmq_t *q, __u64 rnd_diff, __u64 num_of_capsule, __u64 old_allocated, __u64 nppos)
{
	__u64 allocated_idx = RMQ_CAPSULE_VAR_IDX(old_allocated);
	rmq_metadata_t *mc = &q->metadata[nppos % RMQ_ACTIVE_CAPSULE_NUM];
	BUG_ON(allocated_idx > RMQ_CAPSULE_DATA_SIZE);
	if (allocated_idx != RMQ_CAPSULE_DATA_SIZE) {
		rmq_data_t *dc = &q->data[(nppos - RMQ_ACTIVE_CAPSULE_NUM * rnd_diff) % num_of_capsule];
		return __rmq_capsule_fillin_dummy(mc, dc, allocated_idx, RMQ_CAPSULE_DATA_SIZE - allocated_idx);
	} else {
		return vatomic64_read(&mc->committed);
	}
}

static inline bool
__rmq_producer_advance(rmq_t *q, rmq_producer_t *p, __u8 cid, __u64 ratio_and_ppos)
{
	for (__u64 i = 0; i < RMQ_RETRY_NUM; i++) {
		__u64 ratio_and_gpos = vatomic64_add_get(&q->global.ratio_and_gpos, 1);
		__u64 nppos = RMQ_VARIABLE_POS(ratio_and_gpos);
		BUG_ON(nppos == 0);
		BUG_ON(RMQ_VARIABLE_POS(ratio_and_ppos) >= nppos);
		__u64 ratio = RMQ_VARIABLE_RATIO(ratio_and_gpos);
		__u64 num_of_capsule = ratio * RMQ_ACTIVE_CAPSULE_NUM;

		__u64 except_rnd = nppos / RMQ_ACTIVE_CAPSULE_NUM;
		__u64 next_rnd = except_rnd + 1;

		rmq_metadata_t *mc = &q->metadata[nppos % RMQ_ACTIVE_CAPSULE_NUM];

		__u64 new_allocated = RMQ_CAPSULE_VAR_COMPOSE(except_rnd, RMQ_CAPSULE_DATA_SIZE);
		__u64 old_allocated = vatomic64_get_max(&mc->allocated, new_allocated);
		__u64 curr_rnd = RMQ_CAPSULE_VAR_RND(old_allocated);
		if (curr_rnd >= next_rnd) {
			continue;
		}

		__u64 committed = __rmq_producer_inactivate(q, next_rnd - curr_rnd,
				num_of_capsule, old_allocated, nppos);
		__u64 committed_idx = RMQ_CAPSULE_VAR_IDX(committed);
		BUG_ON(committed_idx > RMQ_CAPSULE_DATA_SIZE);
		if (committed_idx != RMQ_CAPSULE_DATA_SIZE) {
			continue;
		}

		__u64 next_allocated = RMQ_CAPSULE_VAR_COMPOSE(next_rnd, RMQ_CAPSULE_INIT_SIZE);
		__u64 next_committed = RMQ_CAPSULE_VAR_COMPOSE(next_rnd, 0);
		if (vatomic64_cmpxchg(&mc->committed, committed, next_committed) != committed) {
			continue;
		}

		__u64 time_stamp = hm_trace_rmq_timestamp(q);
		vatomic64_write(&mc->time_stamp, time_stamp);

		rmq_data_t *dc = &q->data[nppos % num_of_capsule];
		__rmq_producer_meta_update(dc, mc, nppos, cid, time_stamp);

		if (vatomic64_cmpxchg(&mc->allocated, new_allocated, next_allocated) != new_allocated) {
			__rmq_capsule_fillin_dummy(mc, dc, RMQ_CAPSULE_INIT_SIZE, RMQ_CAPSULE_DATA_SIZE);
			continue;
		}

		vatomic64_add(&mc->committed, RMQ_CAPSULE_INIT_SIZE);

		vatomic64_cmpxchg(&p->ratio_and_ppos, ratio_and_ppos, RMQ_VARIABLE_COMPOSE(ratio, nppos));
		return true;
	}
	return false;
}

static inline void
__rmq_producer_fillin_entry(rmq_metadata_t *mc, rmq_data_t *dc, rmq_producer_entry_t *pe,
			  __u64 allocated_idx, __u64 size, __u32 time_stamp_offset)
{
	vatomic_fence_rel();
	rmq_entry_t *e = __rmq_entry_get(dc->buf, allocated_idx);
	e->time_stamp_offset = time_stamp_offset;
	e->size = (__u16)size;
	pe->handler = (void *)&mc->committed;
	pe->addr = (void *)e->data;
}

static inline __s64
__rmq_producer_get_entry_by_cache(rmq_t *q, __u8 cid, rmq_producer_cache_t *cache, rmq_producer_entry_t *pe, __u64 size)
{
	if (size == 0) {
		return RMQ_INVALID_SIZE;
	}

	for (__u64 i = 0; i < RMQ_RETRY_NUM; i++) {
		rmq_metadata_t *mc = cache->mc;
		rmq_data_t *dc = cache->dc;

		__u64 allocated = vatomic64_read(&mc->allocated);
		if (RMQ_CAPSULE_VAR_RND(allocated) != cache->rnd) {
			return RMQ_FAIL;
		}

		__u64 allocated_idx = RMQ_CAPSULE_VAR_IDX(allocated);
		if (allocated_idx == RMQ_CAPSULE_DATA_SIZE) {
			return RMQ_FAIL;
		}

		__u64 entry_total_size = __rmq_entry_total_size(size);
		if (entry_total_size > RMQ_MAXINUM_DATA_SIZE) {
			return RMQ_INVALID_SIZE;
		}

		__s64 time_stamp_offset = (__s64)(hm_trace_rmq_timestamp(q) - vatomic64_read(&mc->time_stamp));
		if (allocated_idx + entry_total_size <= RMQ_CAPSULE_BOUND && time_stamp_offset <= RMQ_UINT32_MAX) {
			if (time_stamp_offset < 0) {
				continue;
			}
			if (vatomic64_cmpxchg(&mc->allocated, allocated, allocated + entry_total_size) != allocated) {
				continue;
			}
			BUG_ON(dc->head.core_id != cid);
			__rmq_producer_fillin_entry(mc, dc, pe, allocated_idx, size, (__u32)time_stamp_offset);
			return (__s64)size;
		} else {
			__u64 dummy_size = RMQ_CAPSULE_DATA_SIZE - allocated_idx;
			BUG_ON(dummy_size < RMQ_ENTRY_METADATA_SIZE);
			if (vatomic64_cmpxchg(&mc->allocated, allocated, allocated + dummy_size) != allocated) {
				continue;
			}
			__rmq_capsule_fillin_dummy(mc, dc, allocated_idx, dummy_size);
			return RMQ_FAIL;
		}
	}
	return RMQ_FAIL;
}

static inline void
__rmq_producer_confirm_entry(rmq_producer_entry_t *pe)
{
	rmq_entry_t *e = __rmq_entry_get(pe->addr, -RMQ_ENTRY_METADATA_SIZE);
	__u64 entry_total_size = __rmq_entry_total_size(e->size);
	vatomic64_add((vatomic64_t *)pe->handler, entry_total_size);
}
#endif
