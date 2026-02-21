/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Relaxed Multi-level Queue (RMQ)
 * Author: Huawei OS Kernel Lab
 * Create: Fri Sep 01 09:47:00 2023
 */
#ifndef KLIBS_TRACE_RMQK_CONSUMER_H
#define KLIBS_TRACE_RMQK_CONSUMER_H

static inline void __rmq_shrink_heartbeat(rmq_t *q);

static inline void
__rmq_datahead_init(rmq_datahead_t *dh, __u64 stamp, __u8 core_id)
{
	dh->producer_stamp = stamp;
	dh->consumer_stamp = stamp;
	dh->core_id = core_id;
}

static inline void
__rmq_consumer_last_init(rmq_consumer_last_t *cl)
{
	cl->cpos = RMQ_ACTIVE_CAPSULE_NUM - RMQ_CORE_NUM;
	cl->consumed_idx = RMQ_CAPSULE_INIT_SIZE;
	cl->producer_stamp = 0;
	cl->consumer_stamp = 0;
}

static inline void
__rmq_consumer_init(rmq_consumer_t *c)
{
	vatomic64_init(&c->epoch, 0);
	c->cpos = RMQ_ACTIVE_CAPSULE_NUM - RMQ_CORE_NUM;
	c->consumed_idx = RMQ_CAPSULE_INIT_SIZE;
	c->reserved_idx = RMQ_CAPSULE_INIT_SIZE;
	rmq_datahead_t *dh = &c->data.head;
	__rmq_datahead_init(dh, c->cpos - 1, 0);
	rmq_consumer_last_t *cl = &c->last;
	__rmq_consumer_last_init(cl);
}

static inline void
__rmq_consumer_flush(rmq_consumer_t *c)
{
	c->cpos++;
	c->consumed_idx = RMQ_CAPSULE_INIT_SIZE;
	c->reserved_idx = RMQ_CAPSULE_INIT_SIZE;
}

static inline bool
__rmq_consumer_update_metadata(__u64 cpos, __u64 consumed_idx, __u64 reserved_idx, rmq_datahead_t *dh,
	rmq_consumer_t *c, rmq_consumer_buffer_batch_t *cbb, enum rmq_copy_type copy_type)
{
	c->cpos = cpos;
	c->consumed_idx = consumed_idx;
	c->reserved_idx = reserved_idx;
	c->data.head = *dh;

	if (cbb != NULL) {
		__u64 batch_size = reserved_idx - consumed_idx;
		if (copy_type == RMQ_COPY_TO_USER) {
			char buf[RMQ_CBB_METADATA_SIZE];
			rmq_consumer_buffer_batch_t *cbb_local = (rmq_consumer_buffer_batch_t *) buf;
			cbb_local->head.time_stamp = dh->time_stamp;
			cbb_local->head.core_id = dh->core_id;
			cbb_local->head.length = batch_size;
			return hm_trace_rmq_dequeue_batch_copy(cbb, cbb_local, RMQ_CBB_METADATA_SIZE);
		} else {
			cbb->head.time_stamp = dh->time_stamp;
			cbb->head.core_id = dh->core_id;
			cbb->head.length = batch_size;
		}
	}
	return false;
}

static inline bool
__rmq_consumer_update_data(rmq_data_t *dc, __u64 reserved_idx, __u64 committed_idx,
	rmq_consumer_t *c, rmq_consumer_buffer_batch_t *cbb, enum rmq_copy_type copy_type)
{
	void *src = __rmq_entry_get(dc->buf, reserved_idx);
	__u64 batch_size = committed_idx - reserved_idx;
	void *dest;
	if (cbb == NULL) {
		BUG_ON(copy_type == RMQ_COPY_TO_USER);
		dest = __rmq_entry_get(c->data.buf, reserved_idx);
	} else {
		dest = (void *)((char *)cbb + RMQ_CBB_METADATA_SIZE);
	}

	if (copy_type == RMQ_COPY_TO_USER) {
		return hm_trace_rmq_dequeue_batch_copy(dest, src, batch_size);
	} else {
		#ifdef __KERNEL__
			memcpy(dest, src, batch_size);
		#else
			NOFAIL(memcpy_s(dest, batch_size, src, batch_size));
		#endif
		return false;
	}
}

static inline void
__rmq_consumer_increase_epoch(rmq_consumer_t *c)
{
#if RMQ_RESIZE_WHILE_READING
	vatomic64_write(&c->epoch, vatomic64_read(&c->epoch) + 1);
#endif
}

static inline bool
__rmq_consumer_advance_is_busy(bool current, __u64 committed, __u64 committed_idx, rmq_metadata_t *mc)
{
	if (current && committed_idx != RMQ_CAPSULE_DATA_SIZE) {
		__u64 allocated = vatomic64_read(&mc->allocated);
		if (allocated != committed) {
			return true;
		}
	}
	return false;
}

#define __RMQ_CONSUMER_ADVANCE_FAILURE_RETURN				\
	{								\
		__rmq_consumer_increase_epoch(c);			\
		__rmq_shrink_heartbeat(q);				\
		return false;						\
	}

#define __RMQ_CONSUMER_ADVANCE_FAILURE_CONTINUE				\
	{								\
		__rmq_consumer_flush(c);				\
		continue;						\
	}

#define __RMQ_CONSUMER_ADVANCE_FAILURE_HANDLING				\
	{								\
		if (!can_skip) {					\
			__RMQ_CONSUMER_ADVANCE_FAILURE_RETURN;		\
		} else {						\
			__RMQ_CONSUMER_ADVANCE_FAILURE_CONTINUE;	\
		}							\
	}

static inline bool
__rmq_consumer_advance(rmq_t *q, rmq_consumer_t *c, rmq_consumer_buffer_batch_t *cbb,
		       enum rmq_copy_type copy_type)
{
	__rmq_consumer_increase_epoch(c);

	while (true) {
		__u64 ratio_and_gpos = vatomic64_read(&q->global.ratio_and_gpos);
		__u64 prod_newest = RMQ_VARIABLE_POS(ratio_and_gpos);
		__u64 cons_excepted = c->cpos;
		if (cons_excepted > prod_newest) {
			__RMQ_CONSUMER_ADVANCE_FAILURE_RETURN;
		}

		__u64 num_of_capsule = RMQ_VARIABLE_RATIO(ratio_and_gpos) * RMQ_ACTIVE_CAPSULE_NUM;
		__s64 prod_oldest = (__s64)prod_newest - (__s64)num_of_capsule + 1 + RMQ_CONSUMER_READING_ADDITIONAL_OFFSET;
		bool out_of_date = ((__s64)cons_excepted < prod_oldest);
		if (out_of_date) {
			c->cpos = cons_excepted = (__u64)prod_oldest;
		}
		bool can_skip = cons_excepted != prod_newest && (RMQ_CONSUMER_MODE != RMQ_CONSUMER_MODE_WAITING);

		rmq_metadata_t *mc = &q->metadata[cons_excepted % RMQ_ACTIVE_CAPSULE_NUM];
		rmq_data_t *dc = &q->data[cons_excepted % num_of_capsule];
		__u64 rnd_diff = num_of_capsule / RMQ_ACTIVE_CAPSULE_NUM;
		__u64 rnd_excepted = cons_excepted / RMQ_ACTIVE_CAPSULE_NUM + 1;

		#if RMQ_CONSUMER_MODE == RMQ_CONSUMER_MODE_CLOSING
		__rmq_capsule_close(q, mc, dc, rnd_excepted);
		#endif

		__u64 committed = vatomic64_read(&mc->committed);
		__u64 committed_rnd = RMQ_CAPSULE_VAR_RND(committed);
		if (committed_rnd < rnd_excepted) {
			__RMQ_CONSUMER_ADVANCE_FAILURE_CONTINUE;
		}
		if (committed_rnd - rnd_excepted >= rnd_diff) {
			__RMQ_CONSUMER_ADVANCE_FAILURE_CONTINUE;
		}

		bool advanced = cons_excepted != c->cpos || out_of_date;
		__u64 reserved_idx = advanced ? RMQ_CAPSULE_INIT_SIZE : c->reserved_idx;
		bool current = committed_rnd == rnd_excepted;
		__u64 committed_idx = current ? RMQ_CAPSULE_VAR_IDX(committed) : RMQ_CAPSULE_DATA_SIZE;
		if (committed_idx <= reserved_idx) {
			__RMQ_CONSUMER_ADVANCE_FAILURE_HANDLING;
		}

		if (__rmq_consumer_advance_is_busy(current, committed, committed_idx, mc)) {
			__RMQ_CONSUMER_ADVANCE_FAILURE_HANDLING;
		}

		rmq_datahead_t dh = dc->head;
		vatomic_fence_acq();

		if (dh.producer_stamp != cons_excepted ||
			dh.consumer_stamp <= c->data.head.consumer_stamp ||
			dh.producer_stamp <= c->data.head.producer_stamp ) {
			__RMQ_CONSUMER_ADVANCE_FAILURE_HANDLING;
		}

		if (copy_type != RMQ_NO_COPY) {
			if (__rmq_consumer_update_data(dc, reserved_idx, committed_idx, c, cbb, copy_type)) {
				__RMQ_CONSUMER_ADVANCE_FAILURE_RETURN;
			}
		}
		vatomic_fence_acq();

		committed = vatomic64_read(&mc->committed);
		if (RMQ_CAPSULE_VAR_RND(committed) - rnd_excepted >= rnd_diff) {
			__RMQ_CONSUMER_ADVANCE_FAILURE_CONTINUE;
		}

#if RMQ_CONSUMER_MODE == RMQ_CONSUMER_MODE_CLOSING
		BUG_ON(reserved_idx != RMQ_CAPSULE_INIT_SIZE);
#endif

		if (__rmq_consumer_update_metadata(cons_excepted, reserved_idx, committed_idx, &dh, c, cbb, copy_type)) {
			__RMQ_CONSUMER_ADVANCE_FAILURE_RETURN;
		}

		__rmq_consumer_increase_epoch(c);
		__rmq_shrink_heartbeat(q);
		return true;
	}
}

static inline __s64
__rmq_consumer_get_entry(rmq_t *q, rmq_consumer_t *c, rmq_consumer_entry_t *ce)
{
	BUG_ON(c->consumed_idx > c->reserved_idx,
	       "consumed_idx:%lx, reserved_idx:%lx\n", (long)c->consumed_idx, (long)c->reserved_idx);
	if (c->consumed_idx < c->reserved_idx) {
		rmq_entry_t *e = __rmq_entry_get(c->data.buf, c->consumed_idx);
		if (e->size != 0) {
			ce->handler = (void *)&c->consumed_idx;
			ce->addr = (void *)e->data;
			ce->time_stamp = c->data.head.time_stamp + e->time_stamp_offset;
			ce->core_id = c->data.head.core_id;
			return e->size;
		}
		__rmq_consumer_flush(c);
	}
	return RMQ_FAIL;
}

static inline void
__rmq_consumer_confirm_entry(rmq_consumer_entry_t *ce)
{
	rmq_entry_t *e = __rmq_entry_get(ce->addr, -RMQ_ENTRY_METADATA_SIZE);
	__u64 entry_total_size = __rmq_entry_total_size(e->size);
	(*(__u64 *)ce->handler) += entry_total_size;
}
#endif
