/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: Relaxed Multi-level Queue (RMQ)
 * Author: Huawei OS Kernel Lab
 * Create: Fri Aug 02 01:46:18 2024
 */
#ifndef KLIBS_TRACE_RMQK_DEBUG_H
#define KLIBS_TRACE_RMQK_DEBUG_H

#define NR_NUM_PER_LINE 8
#define NR_CAPSULE_LINE (RMQ_PAGE_SIZE / sizeof(long long))
static inline void dump_capsule_head(rmq_t *q, __u64 idx)
{
	rmq_data_t *dc = &q->data[idx];
	__u64 time_stamp = dc->head.time_stamp;
	__u64 producer_stamp = dc->head.producer_stamp;
	__u64 consumer_stamp = dc->head.consumer_stamp;
	rmq_log_info("No.%llu\n"
		     "time_stamp:(%llu)\n"
		     "producer_stamp:(%llu , %llu)\n"
		     "consumer_stamp:(%llu , %llu)\n",
		     idx,
		     time_stamp,
		     RMQ_CAPSULE_VAR_RND(producer_stamp), RMQ_CAPSULE_VAR_IDX(producer_stamp),
		     RMQ_CAPSULE_VAR_RND(consumer_stamp), RMQ_CAPSULE_VAR_IDX(consumer_stamp));
}

static inline void dump_capsule_data(rmq_t *q, __u64 idx)
{
	rmq_data_t *dc = &q->data[idx];
	long long *data = (long long *)(void *)dc;

	for (unsigned i = 0U; i < NR_CAPSULE_LINE; i += NR_NUM_PER_LINE) {
		rmq_log_info("%llx %llx %llx %llx %llx %llx %llx %llx\n",
			     data[i], data[i + 1], data[i + 2], data[i + 3], /* 1 2 3 4: are index */
			     data[i + 4], data[i + 5], data[i + 6], data[i + 7]); /* 4 5 6 7: are index */
	}
}

static inline void rmq_dump(rmq_t *q)
{
	__u64 ratio_and_gpos = vatomic64_read(&q->global.ratio_and_gpos);
	__u64 gpos = RMQ_VARIABLE_POS(ratio_and_gpos);
	__u64 num_of_capsule = RMQ_VARIABLE_RATIO(ratio_and_gpos) * RMQ_ACTIVE_CAPSULE_NUM;
	__u64 flag = vatomic64_read(&q->global.resize.flag);
	__u64 count = RMQ_RESIZE_CNT(flag);
	__u64 state = RMQ_RESIZE_STATE(flag);
	__u64 gpos_last = vatomic64_read(&q->global.resize.gpos_last);
	__u64 epoch_last = vatomic64_read(&q->global.resize.epoch_last);
	rmq_consumer_t *c = &q->consumer;
	rmq_consumer_last_t *lc = &q->consumer.last;

	rmq_log_info("#metadata pages:%lu\n"
		     "[Global]\n"
		     "active:(%llu, %llu)\n"
		     "total:(%llu, %llu)\n"
		     "#capsules:%llu\n"
		     "flag:(%llu, %llu)\n"
		     "gpos_last:(%llu)\n"
		     "epoch_last:(%llu)\n",
		     (unsigned int)sizeof(rmq_t) / RMQ_PAGE_SIZE,
		     gpos / RMQ_ACTIVE_CAPSULE_NUM, gpos % RMQ_ACTIVE_CAPSULE_NUM,
		     gpos / num_of_capsule, gpos % num_of_capsule,
		     num_of_capsule,
		     count, state, gpos_last, epoch_last);
	rmq_log_info("[Producer]\n");
	for (__u64 idx = 0; idx < RMQ_CORE_NUM; idx++) {
		rmq_producer_t *p = &q->producer[idx];
		__u64 ratio_and_ppos = vatomic64_read(&p->ratio_and_ppos);
		__u64 ppos = RMQ_VARIABLE_POS(ratio_and_ppos);

		rmq_log_info("No.%llu\n"
			     "active:(%llu, %llu)\n"
			     "total:(%llu, %llu)\n"
			     "#capsules:%llu\n",
			     idx,
			     ppos / RMQ_ACTIVE_CAPSULE_NUM, ppos % RMQ_ACTIVE_CAPSULE_NUM,
			     ppos / num_of_capsule, ppos % num_of_capsule, num_of_capsule);
	}

	rmq_log_info("[Consumer]\n"
		     "active:(%llu, %llu)\n"
		     "total:(%llu, %llu)\n"
		     "consumed_idx:(%llu)\n"
		     "reserved_idx:(%llu)\n"
		     "core-id:(%u)\n"
		     "resize-epoch:(%llu)\n"
		     "producer_stamp:(%llu, %llu)\n"
		     "consumer_stamp:(%llu, %llu)\n",
		     c->cpos / RMQ_ACTIVE_CAPSULE_NUM, c->cpos % RMQ_ACTIVE_CAPSULE_NUM,
		     c->cpos / num_of_capsule, c->cpos % num_of_capsule,
		     c->consumed_idx,
		     c->reserved_idx,
		     (unsigned int)c->data.head.core_id,
		     (unsigned long long)vatomic64_read(&c->epoch),
		     RMQ_CAPSULE_VAR_RND(c->data.head.producer_stamp),
		     RMQ_CAPSULE_VAR_IDX(c->data.head.producer_stamp),
		     RMQ_CAPSULE_VAR_RND(c->data.head.consumer_stamp),
		     RMQ_CAPSULE_VAR_IDX(c->data.head.consumer_stamp));

	rmq_log_info("[Consumer-Last]\n"
		     "active:(%llu, %llu)\n"
		     "total:(%llu, %llu)\n"
		     "consumed_idx:(%llu)\n"
		     "producer_stamp:(%llu, %llu)\n"
		     "consumer_stamp:(%llu, %llu)\n",
		     lc->cpos / RMQ_ACTIVE_CAPSULE_NUM, lc->cpos % RMQ_ACTIVE_CAPSULE_NUM,
		     lc->cpos / num_of_capsule, lc->cpos % num_of_capsule, lc->consumed_idx,
		     RMQ_CAPSULE_VAR_RND(lc->producer_stamp),
		     RMQ_CAPSULE_VAR_IDX(lc->producer_stamp),
		     RMQ_CAPSULE_VAR_RND(lc->consumer_stamp),
		     RMQ_CAPSULE_VAR_IDX(lc->consumer_stamp));

	rmq_log_info("[Capsules]\n");
	for (__u64 idx = 0; idx < RMQ_ACTIVE_CAPSULE_NUM; idx++) {
		rmq_metadata_t *mc = &q->metadata[idx];
		__u64 allocated = vatomic64_read(&mc->allocated);
		__u64 committed = vatomic64_read(&mc->committed);
		rmq_log_info("No.%llu\n"
			     "allocated:(%llu, %llu)\n"
			     "committed:(%llu, %llu)\n",
			     idx,
			     RMQ_CAPSULE_VAR_RND(allocated), RMQ_CAPSULE_VAR_IDX(allocated),
			     RMQ_CAPSULE_VAR_RND(committed), RMQ_CAPSULE_VAR_IDX(committed));
		if (RMQ_CAPSULE_VAR_IDX(allocated) != RMQ_CAPSULE_VAR_IDX(committed)) {
			__u64 data_idx = (RMQ_CAPSULE_VAR_RND(committed) * RMQ_ACTIVE_CAPSULE_NUM + idx) % num_of_capsule;
			dump_capsule_data(q, data_idx);
		}
	}

	rmq_log_info("[Data]\n");
	for (__u64 idx = 0; idx < num_of_capsule; idx++) {
		dump_capsule_head(q, idx);
	}
	return;
}
#endif
