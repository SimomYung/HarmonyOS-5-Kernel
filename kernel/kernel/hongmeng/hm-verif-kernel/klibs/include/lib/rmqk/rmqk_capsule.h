/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Relaxed Multi-level Queue (RMQ)
 * Author: Huawei OS Kernel Lab
 * Create: Fri Sep 01 09:47:00 2023
 */
#ifndef KLIBS_RMQK_CAPSULE_H
#define KLIBS_RMQK_CAPSULE_H

static inline void
__rmq_capsule_metadata_init(rmq_metadata_t *mc, __u64 idx)
{
	__u64 init_value = RMQ_CAPSULE_VAR_COMPOSE(1, RMQ_CAPSULE_INIT_SIZE);
	vatomic64_init(&mc->allocated, init_value);
	vatomic64_init(&mc->committed, init_value);
}

static inline __u64
__rmq_capsule_fillin_dummy(rmq_metadata_t *mc, rmq_data_t *dc, __u64 start_idx, __u64 dummy_size)
{
	vatomic_fence_rel();
	rmq_entry_t *e = __rmq_entry_get(dc->buf, start_idx);
	e->size = 0;
	return vatomic64_add_get(&mc->committed, dummy_size);
}

static inline void
__rmq_capsule_close(rmq_t *q, rmq_metadata_t *mc, rmq_data_t *dc, __u64 except_rnd)
{
	__u64 new_allocated = RMQ_CAPSULE_VAR_COMPOSE(except_rnd, RMQ_CAPSULE_DATA_SIZE);
	__u64 old_allocated = vatomic64_get_max(&mc->allocated, new_allocated);
	__u64 allocated_idx = RMQ_CAPSULE_VAR_IDX(old_allocated);
	__u64 curr_rnd = RMQ_CAPSULE_VAR_RND(old_allocated);
	if (curr_rnd <= except_rnd && allocated_idx != RMQ_CAPSULE_DATA_SIZE) {
		__u64 dummy_size = RMQ_CAPSULE_DATA_SIZE - allocated_idx;
		__rmq_capsule_fillin_dummy(mc, dc, allocated_idx, dummy_size);
	}
}
#endif
