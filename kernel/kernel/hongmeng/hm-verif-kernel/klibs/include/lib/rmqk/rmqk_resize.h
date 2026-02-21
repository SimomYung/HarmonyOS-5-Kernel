/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Relaxed Multi-level Queue (RMQ)
 * Author: Huawei OS Kernel Lab
 * Create: Fri Sep 01 09:47:00 2023
 */
#ifndef KLIBS_TRACE_RMQK_RESIZE_H
#define KLIBS_TRACE_RMQK_RESIZE_H

#define RMQ_RESIZE_STATE_BITS				(RMQ_LOG2(RMQ_ACTIVE_CAPSULE_NUM) + 1)
#define RMQ_RESIZE_STATE(v)				((v) & ((1UL << RMQ_RESIZE_STATE_BITS) - 1))
#define RMQ_RESIZE_CNT(v)				((v) >> RMQ_RESIZE_STATE_BITS)
#define RMQ_RESIZE_COMPOSE(h, l)			(((h) << RMQ_RESIZE_STATE_BITS) | (l))

#define RMQ_RESIZE_SHRINK_ING(s)			(s)
#define RMQ_RESIZE_SHRINK_PRODUCER_BEGIN(s)		((s) - 1)
#define RMQ_RESIZE_SHRINK_PRODUCER_READY(s)		((s) == (RMQ_ACTIVE_CAPSULE_NUM + 1))
#define RMQ_RESIZE_SHRINK_CALLBACKING(s)		((s) == (RMQ_ACTIVE_CAPSULE_NUM + 2))

#define RMQ_RESIZE_SHRINK_ACT_START(a)			(vatomic64_add(&(a), 1))
#define RMQ_RESIZE_SHRINK_ACT_FORWARD(a, n, idx)	\
	(vatomic64_cmpxchg(&(a), (n), RMQ_RESIZE_COMPOSE(RMQ_RESIZE_CNT(n), (idx) + 1)))
#define RMQ_RESIZE_SHRINK_ACT_CALLBACK(a, n)		(vatomic64_cmpxchg(&(a), (n), (n) + 1))
#define RMQ_RESIZE_SHRINK_ACT_FINISH(a)			(vatomic64_add(&(a), RMQ_ACTIVE_CAPSULE_NUM - 2))

static inline __s64
__rmq_resize_calculate_ratio(__u64 buf_size)
{
	return (__s64)(buf_size - RMQ_METADATA_SIZE) / RMQ_CAPSULE_SIZE / RMQ_ACTIVE_CAPSULE_NUM;
}

#ifdef __HM_RMQ_RESIZE__
#if RMQ_RESIZE_EXPAND_CONSERVATIVE
static inline void
__rmq_resize_set_gpos(rmq_t *q, __u64 ratio_and_gpos, __u64 old_ratio, __u64 new_ratio)
{
	__u64 delta = RMQ_MAX2(old_ratio, new_ratio) * RMQ_ACTIVE_CAPSULE_NUM;
	while (true) {
		__u64 pos = RMQ_VARIABLE_POS(ratio_and_gpos);
		__u64 new_ratio_and_gpos = RMQ_VARIABLE_COMPOSE(new_ratio, pos + delta);
		__u64 old_ratio_and_gpos = vatomic64_cmpxchg(&q->global.ratio_and_gpos, ratio_and_gpos, new_ratio_and_gpos);
		if (old_ratio_and_gpos == ratio_and_gpos) {
			break;
		}
		ratio_and_gpos = old_ratio_and_gpos;
	}
}
#else
static inline void
__rmq_resize_fixing(rmq_t *q, __u64 old_ratio, __u64 curr_gpos, __u64 new_ratio, __u64 next_gpos)
{
	__u64 *old_gpos_lst = malloc(old_ratio * RMQ_ACTIVE_CAPSULE_NUM * sizeof(__u64));
	BUG_ON(old_gpos_lst == NULL);
	__u64 *new_gpos_lst = malloc(old_ratio * RMQ_ACTIVE_CAPSULE_NUM * sizeof(__u64));
	BUG_ON(new_gpos_lst == NULL);

	__u64 old_end_gpos = curr_gpos;
	__u64 old_start_gpos = RMQ_MAX2((__s64)(RMQ_ACTIVE_CAPSULE_NUM - RMQ_CORE_NUM),
					(__s64)(old_end_gpos - old_ratio * RMQ_ACTIVE_CAPSULE_NUM + 1));
	__u64 old_middle_gpos = RMQ_MAX2((__s64)(RMQ_ACTIVE_CAPSULE_NUM - RMQ_CORE_NUM),
					(__s64)(old_end_gpos - RMQ_ACTIVE_CAPSULE_NUM + 1));

	__u64 new_end_idx = next_gpos % (new_ratio * RMQ_ACTIVE_CAPSULE_NUM);
	for (__u64 old_iter_gpos = old_start_gpos; old_iter_gpos <= old_end_gpos; old_iter_gpos++) {
		__u64 old_iter_dc_idx = old_iter_gpos % (old_ratio * RMQ_ACTIVE_CAPSULE_NUM);
		__u64 new_iter_gpos = new_end_idx >= old_iter_dc_idx
				? (next_gpos - (new_end_idx - old_iter_dc_idx))
				: (next_gpos - (new_ratio * RMQ_ACTIVE_CAPSULE_NUM - (old_iter_dc_idx - new_end_idx)));
		old_gpos_lst[old_iter_dc_idx] = old_iter_gpos;
		new_gpos_lst[old_iter_dc_idx] = new_iter_gpos;
	}

	for (__u64 old_iter_gpos = old_middle_gpos; old_iter_gpos <= old_end_gpos; old_iter_gpos++) {
		__u64 old_iter_mc_idx = old_iter_gpos % RMQ_ACTIVE_CAPSULE_NUM;
		rmq_metadata_t *mc = &q->metadata[old_iter_mc_idx];

		__u64 new_gpos = next_gpos - (old_end_gpos - old_iter_gpos);
		__u64 except_rnd = new_gpos / RMQ_ACTIVE_CAPSULE_NUM;
		__u64 next_rnd = except_rnd + 1;

		__u64 new_allocated = RMQ_CAPSULE_VAR_COMPOSE(except_rnd, RMQ_CAPSULE_DATA_SIZE);
		__u64 old_allocated = vatomic64_get_max(&mc->allocated, new_allocated);
		__u64 curr_rnd = RMQ_CAPSULE_VAR_RND(old_allocated);
		if (curr_rnd >= next_rnd) {
			continue;
		}

		__u64 old_gpos = old_iter_gpos;
		__u64 old_except_rnd = old_gpos / RMQ_ACTIVE_CAPSULE_NUM;
		__u64 old_next_rnd = old_except_rnd + 1;
		if (curr_rnd > old_next_rnd) {
			continue;
		}

		__u64 allocated_idx = RMQ_CAPSULE_VAR_IDX(old_allocated);
		BUG_ON(allocated_idx > RMQ_CAPSULE_DATA_SIZE, "allocated_idx:%lx\n", (long)allocated_idx);

		__u64 round_diff = old_next_rnd - curr_rnd;
		__u64 old_num_of_capsule = old_ratio * RMQ_ACTIVE_CAPSULE_NUM;
		__u64 old_iter_dc_idx_closing = (old_gpos - RMQ_ACTIVE_CAPSULE_NUM * round_diff) % old_num_of_capsule;
		rmq_data_t *dc_closing = &q->data[old_iter_dc_idx_closing];
		__u64 committed = (allocated_idx != RMQ_CAPSULE_DATA_SIZE)
				? __rmq_capsule_fillin_dummy(mc, dc_closing, allocated_idx, RMQ_CAPSULE_DATA_SIZE - allocated_idx)
				: vatomic64_read(&mc->committed);

		__u64 committed_idx = RMQ_CAPSULE_VAR_IDX(committed);
		BUG_ON(committed_idx > RMQ_CAPSULE_DATA_SIZE, "committed_idx:%lx\n", (long)committed_idx);
		if (committed_idx != RMQ_CAPSULE_DATA_SIZE) {
			continue;
		}

		__u64 next_committed = RMQ_CAPSULE_VAR_COMPOSE(next_rnd, 0);
		if (vatomic64_cmpxchg(&mc->committed, committed, next_committed) != committed) {
			continue;
		}

		__u64 next_allocated = RMQ_CAPSULE_VAR_COMPOSE(next_rnd, RMQ_CAPSULE_DATA_SIZE);
		if (vatomic64_cmpxchg(&mc->allocated, new_allocated, next_allocated) != new_allocated) {
			__rmq_capsule_fillin_dummy(mc, dc_closing, RMQ_CAPSULE_INIT_SIZE, RMQ_CAPSULE_DATA_SIZE);
			continue;
		}

		__u64 old_end_gpos2 = old_iter_gpos - RMQ_ACTIVE_CAPSULE_NUM * round_diff;
		__u64 old_start_gpos2 = RMQ_MAX2((__s64)(RMQ_ACTIVE_CAPSULE_NUM - RMQ_CORE_NUM),
						(__s64)(old_iter_gpos - RMQ_ACTIVE_CAPSULE_NUM * old_ratio));
		old_start_gpos2 = old_end_gpos2 - ((old_end_gpos2 - old_start_gpos2)
				/ RMQ_ACTIVE_CAPSULE_NUM * RMQ_ACTIVE_CAPSULE_NUM);
		for (__u64 iter_gpos = old_start_gpos2; iter_gpos <= old_end_gpos2; iter_gpos += RMQ_ACTIVE_CAPSULE_NUM) {
			__u64 old_iter_dc_idx_fixing = iter_gpos % old_num_of_capsule;
			rmq_data_t *dc_fixing = &q->data[old_iter_dc_idx_fixing];
			__u64 old_producer_stamp = dc_fixing->head.producer_stamp;
			if (old_producer_stamp == old_gpos_lst[old_iter_dc_idx_fixing]) {
				vatomic_fence_rel();
				dc_fixing->head.producer_stamp = new_gpos_lst[old_iter_dc_idx_fixing];
			}
		}
		vatomic64_add(&mc->committed, RMQ_CAPSULE_DATA_SIZE);
	}
	free(old_gpos_lst);
	free(new_gpos_lst);
}

static inline void
__rmq_resize_set_gpos(rmq_t *q, __u64 ratio_and_gpos, __u64 old_ratio, __u64 new_ratio)
{
	while (true) {
		__u64 old_gpos = RMQ_VARIABLE_POS(ratio_and_gpos);
		__u64 new_gpos;
		if (new_ratio > old_ratio) {
			__u64 old_idx = old_gpos % (old_ratio * RMQ_ACTIVE_CAPSULE_NUM);
			__u64 new_rnd = old_gpos / (new_ratio * RMQ_ACTIVE_CAPSULE_NUM);
			__u64 new_idx = old_gpos % (new_ratio * RMQ_ACTIVE_CAPSULE_NUM);
			new_gpos = (new_rnd + (new_idx > old_idx) + 1) * new_ratio * RMQ_ACTIVE_CAPSULE_NUM + old_idx;
		} else {
			new_gpos = old_gpos + old_ratio * RMQ_ACTIVE_CAPSULE_NUM;
		}
		__u64 new_ratio_and_gpos = RMQ_VARIABLE_COMPOSE(new_ratio, new_gpos);
		__u64 old_ratio_and_gpos = vatomic64_cmpxchg(&q->global.ratio_and_gpos, ratio_and_gpos, new_ratio_and_gpos);
		if (old_ratio_and_gpos == ratio_and_gpos) {
			if (new_ratio > old_ratio) {
				__rmq_resize_fixing(q, old_ratio, old_gpos, new_ratio, new_gpos);
			}
			break;
		}
		ratio_and_gpos = old_ratio_and_gpos;
	}
}
#endif /* RMQ_RESIZE_EXPAND_CONSERVATIVE */
#else
static inline void
__rmq_resize_set_gpos(rmq_t *q, __u64 ratio_and_gpos, __u64 old_ratio, __u64 new_ratio)
{
	UNUSED(q, ratio_and_gpos, old_ratio, new_ratio);
	BUG_ON(true);
}
#endif /* __HM_RMQ_RESIZE__ */

static inline bool
__rmq_shrink_producer_ready(rmq_t *q, __u64 flag)
{
	__u64 start_idx = RMQ_RESIZE_SHRINK_PRODUCER_BEGIN(RMQ_RESIZE_STATE(flag));
	__u64 gpos = vatomic64_read(&q->global.resize.gpos_last);
	__u64 idx = start_idx;
	while (idx < RMQ_ACTIVE_CAPSULE_NUM) {
		__u64 rnd_excepted = (gpos + idx) / RMQ_ACTIVE_CAPSULE_NUM + 1;
		rmq_metadata_t *mc = &q->metadata[idx];
		__u64 committed = vatomic64_read(&mc->committed);
		__u64 threshold = RMQ_CAPSULE_VAR_COMPOSE(rnd_excepted, RMQ_CAPSULE_DATA_SIZE);
		if (committed < threshold) {
			__u64 committed_idx = RMQ_CAPSULE_VAR_IDX(committed);
			if (committed_idx != RMQ_CAPSULE_DATA_SIZE) {
				break;
			}
			if (vatomic64_cmpxchg(&mc->committed, committed, threshold) != committed) {
				break;
			}
		}
		idx++;
	}
	if (idx == start_idx) {
		return false;
	}
	RMQ_RESIZE_SHRINK_ACT_FORWARD(q->global.resize.flag, flag, idx);
	return true;
}

static inline bool
__rmq_shrink_consumer_ready(rmq_t *q)
{
#if RMQ_RESIZE_WHILE_READING
	__u64 epoch_last = vatomic64_read(&q->global.resize.epoch_last);
	if ((epoch_last & 1) == 0) {
		return true;
	}
	__u64 epoch = vatomic64_read(&q->consumer.epoch);
	return epoch_last != epoch;
#else
	return true;
#endif
}

static inline void
__rmq_shrink_heartbeat(rmq_t *q)
{
	while (true) {
		__u64 flag = vatomic64_read(&q->global.resize.flag);
		__u64 state = RMQ_RESIZE_STATE(flag);
		if (RMQ_RESIZE_SHRINK_ING(state) == 0ULL) {
			return;
		} else if (RMQ_RESIZE_SHRINK_CALLBACKING(state)) {
			return;
		} else if (RMQ_RESIZE_SHRINK_PRODUCER_READY(state)) {
			if (!__rmq_shrink_consumer_ready(q)) {
				return;
			}
			if (RMQ_RESIZE_SHRINK_ACT_CALLBACK(q->global.resize.flag, flag) != flag) {
				continue;
			}
#if RMQ_RESIZE_SHRINK_CALLBACK_STATIC
			hm_trace_rmq_shrink_wake();
#else
			void (*f)(void *) = q->global.resize.callback.func;
			if (f) {
				f(q->global.resize.callback.args);
			}
#endif
			RMQ_RESIZE_SHRINK_ACT_FINISH(q->global.resize.flag);
			return;
		} else {
			if (!__rmq_shrink_producer_ready(q, flag)) {
				return;
			}
		}
	}
}

static inline void
__rmq_resize_force_closing(rmq_t *q, __u64 ratio_and_gpos)
{
	rmq_producer_t p;
	vatomic64_write(&p.ratio_and_ppos, ratio_and_gpos);
	for (__u64 i = 0; i < RMQ_ACTIVE_CAPSULE_NUM; i++) {
		__u64 ratio_and_ppos = vatomic64_read(&p.ratio_and_ppos);
		__rmq_producer_advance(q, &p, RMQ_CORE_NUM, ratio_and_ppos);
	}
}

static inline void
__rmq_shrink(rmq_t *q, __u64 ratio_and_gpos, __u64 old_ratio, __u64 new_ratio)
{
	__rmq_resize_set_gpos(q, ratio_and_gpos, old_ratio, new_ratio);
	__u64 epoch = vatomic64_read(&q->consumer.epoch);
	vatomic64_write(&q->global.resize.epoch_last, epoch);
	vatomic64_write(&q->global.resize.gpos_last, RMQ_VARIABLE_POS(ratio_and_gpos));
	__rmq_resize_force_closing(q, ratio_and_gpos);
	RMQ_RESIZE_SHRINK_ACT_START(q->global.resize.flag);
	__rmq_shrink_heartbeat(q);
}

static inline void
__rmq_expand(rmq_t *q, __u64 ratio_and_gpos, __u64 old_ratio, __u64 new_ratio)
{
	vatomic_fence_rel();
	for (__u64 idx = old_ratio * RMQ_ACTIVE_CAPSULE_NUM; idx < new_ratio * RMQ_ACTIVE_CAPSULE_NUM; idx++) {
		rmq_data_t *dc = &q->data[idx];
		__rmq_datahead_init(&dc->head, 0, 0);
	}
	__rmq_resize_set_gpos(q, ratio_and_gpos, old_ratio, new_ratio);
}
#endif
