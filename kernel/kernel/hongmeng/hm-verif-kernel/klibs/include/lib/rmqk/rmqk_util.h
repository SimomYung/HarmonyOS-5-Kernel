/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: RMQ
 * Author: Huawei OS Kernel Lab
 * Create: Wed Sep 20 17:13:09 2023
 */
#ifndef KLIBS_TRACE_RMQK_UTIL_H
#define KLIBS_TRACE_RMQK_UTIL_H

#define RMQ_FAIL					((__s64)0)
#define RMQ_INVALID_SIZE				((__s64)-1)

#define RMQ_RATIO_MAXIMUM				((1UL << (64 - RMQ_VARIABLE_POS_BITS)) - 1)
#define RMQ_RATIO_MINIMUM				(1UL)

#define RMQ_RESIZE_EXPAND_CONSERVATIVE			(0)
#define RMQ_CONSUMER_READING_ADDITIONAL_OFFSET		(0)
#define RMQ_RESIZE_WHILE_READING			(1)
#define RMQ_RESIZE_EXPAND				((__u64)1)
#define RMQ_RESIZE_SHRINK				((__u64)2)
#define RMQ_RESIZE_UNCHANGED				((__u64)0)

#define RMQ_CONSUMER_MODE_SKIPPING			(0)
#define RMQ_CONSUMER_MODE_CLOSING			(1)
#define RMQ_CONSUMER_MODE_WAITING			(2)
#define RMQ_UINT8_MAX					(0xff)
#define RMQ_UINT32_MAX					(0xffffffff)
#define RMQ_UINT64_MAX					(0xffffffffffffffff)

#define RMQ_LOG2(x)			\
	((unsigned)(8UL * sizeof (unsigned long long) - (unsigned long)__builtin_clzll((x)) - 1UL))
#define RMQ_MIN2(a, b)			({ __typeof__(a) _a = (a); __typeof__(b) _b = (b); _a < _b ? _a : _b; })
#define RMQ_MAX2(a, b)			({ __typeof__(a) _a = (a); __typeof__(b) _b = (b); _a > _b ? _a : _b; })
#define RMQ_ALIGN_DOWN_WITH_POWER2(a)	(1UL << (64UL - __builtin_clzll((__u64)(a)) - 1UL))

enum trace_clock {
	TRACE_CLOCK_BOOT,
	TRACE_CLOCK_MONO,
	TRACE_CLOCK_NUM,
};

enum rmq_copy_type {
	RMQ_NO_COPY,
	RMQ_COPY_TO_LOCAL_MEM,
	RMQ_COPY_TO_USER,
};

extern int hm_trace_rmq_dequeue_batch_copy(void *dst, void *src, size_t size);
extern __u64 hm_trace_rmq_timestamp(void *rmq);

static inline void *
__rmq_get_by_offset(void *start, __u64 offset)
{
	return (void *)(((__uptr_t)start) + ((__uptr_t)offset));
}

static inline __u64
__rmq_get_offset(void *start, void *end)
{
	return ((__uptr_t)end) - ((__uptr_t)start);
}
#endif
