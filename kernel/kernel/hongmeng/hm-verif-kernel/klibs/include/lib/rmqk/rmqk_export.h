/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: Exposed interfaces of rmq
 * Author: Huawei OS Kernel Lab
 * Create: Mon Apr 08 20:12:21 2024
 */
#ifndef KLIBS_RMQK_EXPORT_H
#define KLIBS_RMQK_EXPORT_H

#include <hmkernel/types.h>
#ifdef __KERNEL__
#include <hmkernel/panic.h>
#include <lib/string.h>
#include <lib/atomic.h>
#include <hmkernel/strict.h>
#else
#include <hongmeng/panic.h>
#include <libhwsecurec/securec.h>
#include <vsync/atomic.h>
#include <libstrict/strict.h>
#endif /* __KERNEL__ */

#include <lib/rmqk/rmqk_util.h>
#include <lib/rmqk/rmqk_config.h>
#include <lib/rmqk/rmqk_struct_basic.h>

extern int hm_rmq_clock_id_of(void *q);
extern __u64 hm_rmq_get_memory_usage(void *q);
extern void hm_rmq_init(void *buf, __u64 buf_size);
extern bool hm_rmq_resize_check(void *q, __u64 new_size);
extern __u64 hm_rmq_resize(void *q, __u64 new_size);
extern bool hm_mmq_get_snapshot_enabled(void *q);
extern bool hm_rmq_is_snapshot_allocated(void *q);
extern void hm_rmq_set_snapshot_enabled(void *q, bool enabled);
extern void hm_rmq_set_snapshot_allocated(void *q, bool allocate);
extern __s64 hm_rmq_enqueue_get_entry(void *q, __u8 cid, void *pe, __u64 size);
extern void hm_rmq_enqueue_confirm_entry(void *pe);
extern __s64 hm_rmq_dequeue_get_entry(void *q, void *ce);
extern bool hm_rmq_dequeue_batch(void *q, void *cbb);
extern void hm_rmq_dequeue_confirm_entry(void *ce);
extern void hm_rmq_consumer_sorted_init(void *cs);
extern __s64 hm_rmq_dequeue_sorted(void *q, void *cs, void *ce);
extern void hm_rmq_consumer_sweep(void *q);
extern void hm_rmq_consumer_forward(void *q);
extern void hm_rmq_consumer_backward(void *q);
extern bool hm_rmq_is_record_enabled(void *q);
extern int hm_trace_rmq_dequeue_batch_copy(void *dst, void *src, size_t size);
extern __u64 hm_trace_rmq_timestamp(void *rmq);
extern bool hm_rmq_set_clock_id(void *q, int clock_id, bool sweep);
extern void hm_rmq_enable_disable_record(void *q, bool enable);
extern bool hm_rmq_is_snapshot_enabled(void *q);
extern unsigned int hm_rmq_get_futex_waiters(void *q);
extern unsigned int hm_rmq_get_consumer_sort_size(void);
extern __uptr_t hm_trace_cal_futex_offset(unsigned int futex_type);
extern rmq_global_t *hm_rmq_get_global(void *rmq);
typedef struct rmq_consumer_entry_s rmq_consumer_entry_t;
extern size_t hm_rmq_entry_size(rmq_consumer_entry_t *entry);
extern bool hm_rmq_is_tracing_block_enabled(void *q);
extern void hm_rmq_set_tracing_block_enabled(void *q, bool enable);
extern void hm_rmq_shrink_heartbeat(void *q);
extern void hm_rmq_dump(void *q);
#endif
