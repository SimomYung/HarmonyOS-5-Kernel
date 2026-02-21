/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Relaxed Multi-level Queue (RMQ)
 * Author: Huawei OS Kernel Lab
 * Create: Fri Sep 01 09:47:00 2023
 */

#ifndef KLIBS_TRACE_RMQK_ENTRY_H
#define KLIBS_TRACE_RMQK_ENTRY_H

/* The entry size should be aligned with 4 */
#define RMQ_ENTRY_ALIGN_BIT			2U
#define RMQ_ENTRY_ALIGN_MASK			((1U << RMQ_ENTRY_ALIGN_BIT) - 1)

static inline rmq_entry_t *
__rmq_entry_get(void *start, __u64 offset)
{
	return (rmq_entry_t *)__rmq_get_by_offset(start, offset);
}

static inline __u64
__rmq_entry_align(__u64 size)
{
	return (size + RMQ_ENTRY_ALIGN_MASK) & (~RMQ_ENTRY_ALIGN_MASK);
}

static inline __u64
__rmq_entry_total_size(__u64 size)
{
	return RMQ_ENTRY_METADATA_SIZE + __rmq_entry_align(size);
}
#endif
