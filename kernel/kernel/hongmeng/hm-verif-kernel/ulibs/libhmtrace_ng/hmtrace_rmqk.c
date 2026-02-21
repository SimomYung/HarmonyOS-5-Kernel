/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: The wrapper for all rmq interfaces
 * Author: Huawei OS Kernel Lab
 * Create: Tue Apr 09 12:47:58 2024
 */
#include <lib/trace/trace_misc.h>
#include <lib/rmqk/rmqk.h>

#ifdef CONFIG_TRACEMGR
void hm_rmq_init(void *buf, __u64 buf_size)
{
	rmq_init(buf, buf_size);
}

bool hm_rmq_resize_check(void *q, __u64 new_size)
{
	return rmq_resize_check((rmq_t *)q, new_size);
}

__u64 hm_rmq_resize(void *q, __u64 new_size)
{
	return rmq_resize((rmq_t *)q, new_size);
}

bool hm_rmq_is_snapshot_enabled(void *q)
{
	return rmq_get_snapshot_enabled((rmq_t *)q);
}

bool hm_rmq_is_snapshot_allocated(void *q)
{
	return rmq_get_snapshot_allocated((rmq_t *)q);
}

void hm_rmq_set_snapshot_enabled(void *q, bool enabled)
{
	rmq_set_snapshot_enabled((rmq_t *)q, enabled);
}

void hm_rmq_set_snapshot_allocated(void *q, bool allocate)
{
	rmq_set_snapshot_allocated((rmq_t *)q, allocate);
}

__s64 hm_rmq_enqueue_get_entry(void *q, __u8 cid, void *pe, __u64 size)
{
	return rmq_enqueue_get_entry((rmq_t *)q, cid, (rmq_producer_entry_t *)pe, size);
}

void hm_rmq_enqueue_confirm_entry(void *pe)
{
	rmq_enqueue_confirm_entry((rmq_producer_entry_t *)pe);
}

void hm_rmq_consumer_sweep(void *q)
{
	rmq_consumer_sweep((rmq_t *)q);
}

bool hm_rmq_is_record_enabled(void *q)
{
	return rmq_is_record_enabled((rmq_t *)q);
}

bool hm_rmq_set_clock_id(void *q, int clock_id, bool sweep)
{
	return rmq_set_clock_id((rmq_t *)q, clock_id, sweep);
}

__s64 hm_rmq_dequeue_get_entry(void *q, void *ce)
{
	return rmq_dequeue_get_entry((rmq_t *)q, (rmq_consumer_entry_t *)ce);
}

bool hm_rmq_dequeue_batch(void *q, void *cbb)
{
	return rmq_dequeue_batch((rmq_t *)q, (rmq_consumer_buffer_batch_t *)cbb);
}

void hm_rmq_dequeue_confirm_entry(void *ce)
{
	return rmq_dequeue_confirm_entry((rmq_consumer_entry_t *)ce);
}

void hm_rmq_consumer_sorted_init(void *cs)
{
	return rmq_consumer_sorted_init((rmq_consumer_sorted_t *)cs);
}

__s64 hm_rmq_dequeue_sorted(void *q, void *cs, void *ce)
{
	return rmq_dequeue_sorted((rmq_t *)q, (rmq_consumer_sorted_t *)cs, (rmq_consumer_entry_t *)ce);
}

void hm_rmq_consumer_forward(void *q)
{
	rmq_consumer_forward((rmq_t *)q);
}

void hm_rmq_consumer_backward(void *q)
{
	rmq_consumer_backward((rmq_t *)q);
}

void hm_rmq_enable_disable_record(void *q, bool enable)
{
	rmq_enable_disable_record((rmq_t *)q, enable);
}

int hm_rmq_clock_id_of(void *q)
{
	return rmq_clock_id_of((rmq_t *)q);
}

__u64 hm_rmq_get_memory_usage(void *q)
{
	return rmq_get_memory_usage((rmq_t *)q);
}

unsigned int hm_rmq_get_consumer_sort_size(void)
{
	return sizeof(rmq_consumer_sorted_t);
}

__uptr_t hm_trace_cal_futex_offset(unsigned int futex_type)
{
	__uptr_t offset = INVALID_VADDR;
	switch (futex_type) {
	case TRACE_RMQ_DEQUEUE_FUTEX:
		offset = 0;
		break;
	case TRACE_SNAPSHOT_RMQ_SHRINK_FUTEX:
		offset = offset_of(rmq_global_t, snapshot_shrink_futex)
			- offset_of(rmq_global_t, dequeue_futex);
		break;
	case TRACE_RMQ_SHRINK_FUTEX:
		offset = offset_of(rmq_global_t, shrink_futex)
			- offset_of(rmq_global_t, dequeue_futex);
		break;
	default:
		break;
	}
	return offset;
}

rmq_global_t *hm_rmq_get_global(void *rmq)
{
	return &((rmq_t *)rmq)->global;
}

size_t hm_rmq_entry_size(rmq_consumer_entry_t *entry)
{
	rmq_entry_t *e = __rmq_entry_get(entry->addr, -RMQ_ENTRY_METADATA_SIZE);

	return (size_t)e->size;
}

bool hm_rmq_is_tracing_block_enabled(void *q)
{
	return hm_rmq_is_record_enabled((rmq_t *)q) && rmq_is_tracing_block_enabled((rmq_t *)q);
}

void hm_rmq_set_tracing_block_enabled(void *q, bool enable)
{
	rmq_set_tracing_block_enabled((rmq_t *)q, enable);
}

void hm_rmq_shrink_heartbeat(void *q)
{
	__rmq_shrink_heartbeat((rmq_t *)q);
}

void hm_rmq_dump(void *q)
{
	rmq_dump((rmq_t *)q);
}
#else
void hm_rmq_init(void *buf, __u64 buf_size)
{
	UNUSED(buf, buf_size);
}

bool hm_rmq_resize_check(void *q, __u64 new_size)
{
	UNUSED(q, new_size);
	return false;
}

__u64 hm_rmq_resize(void *q, __u64 new_size)
{
	UNUSED(q, new_size);
	return 0;
}

bool hm_rmq_is_snapshot_enabled(void *q)
{
	UNUSED(q);
	return false;
}

bool hm_rmq_is_snapshot_allocated(void *q)
{
	UNUSED(q);
	return false;
}

void hm_rmq_set_snapshot_enabled(void *q, bool enabled)
{
	UNUSED(q, enabled);
}

void hm_rmq_set_snapshot_allocated(void *q, bool allocate)
{
	UNUSED(q, allocate);
}

__s64 hm_rmq_enqueue_get_entry(void *q, __u8 cid, void *pe, __u64 size)
{
	UNUSED(q, cid, pe, size);
	return 0;
}

void hm_rmq_enqueue_confirm_entry(void *pe)
{
	UNUSED(pe);
}

void hm_rmq_consumer_sweep(void *q)
{
	UNUSED(q);
}

bool hm_rmq_is_record_enabled(void *q)
{
	UNUSED(q);
	return false;
}

bool hm_rmq_set_clock_id(void *q, int clock_id, bool sweep)
{
	UNUSED(q, clock_id, sweep);
	return false;
}

__s64 hm_rmq_dequeue_get_entry(void *q, void *ce)
{
	UNUSED(q, ce);
	return 0;
}

bool hm_rmq_dequeue_batch(void *q, void *cbb)
{
	UNUSED(q, cbb);
	return false;
}

void hm_rmq_dequeue_confirm_entry(void *ce)
{
	UNUSED(ce);
}

void hm_rmq_consumer_sorted_init(void *cs)
{
	UNUSED(cs);
}

__s64 hm_rmq_dequeue_sorted(void *q, void *cs, void *ce)
{
	UNUSED(q, cs, ce);
	return 0;
}

void hm_rmq_consumer_forward(void *q)
{
	UNUSED(q);
}

void hm_rmq_consumer_backward(void *q)
{
	UNUSED(q);
}

void hm_rmq_enable_disable_record(void *q, bool enable)
{
	UNUSED(q, enable);
}

int hm_rmq_clock_id_of(void *q)
{
	UNUSED(q);
	return 0;
}

__u64 hm_rmq_get_memory_usage(void *q)
{
	UNUSED(q);
	return 0;
}

unsigned int hm_rmq_get_consumer_sort_size(void)
{
	return 0;
}

__uptr_t hm_trace_cal_futex_offset(unsigned int futex_type)
{
	UNUSED(futex_type);
	return INVALID_VADDR;
}

rmq_global_t *hm_rmq_get_global(void *rmq)
{
	UNUSED(rmq);
	return NULL;
}

size_t hm_rmq_entry_size(rmq_consumer_entry_t *entry)
{
	UNUSED(entry);
	return 0;
}

bool hm_rmq_is_tracing_block_enabled(void *q)
{
	UNUSED(q);
	return false;
}

void hm_rmq_set_tracing_block_enabled(void *q, bool enable)
{
	UNUSED(q, enable);
}

void hm_rmq_shrink_heartbeat(void *q)
{
	UNUSED(q);
}

void hm_rmq_dump(void *q)
{
	UNUSED(q);
}
#endif
