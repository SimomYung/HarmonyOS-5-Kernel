/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Relaxed Multi-level Queue (RMQ)
 * Author: Huawei OS Kernel Lab
 * Create: Fri Sep 01 09:47:00 2023
 */
#ifndef KLIBS_TRACE_RMQK_STRUCT_BASIC_H
#define KLIBS_TRACE_RMQK_STRUCT_BASIC_H

#define RMQ_ATTR_CACHELINE_ALIGNED			__attribute__((aligned(RMQ_CACHELINE_SIZE)))
#define RMQ_ATTR_PAGE_ALIGNED				__attribute__((aligned(RMQ_PAGE_SIZE)))

#define RMQ_CAPSULE_SIZE_LOG2				(RMQ_LOG2(RMQ_CAPSULE_SIZE) + 1)
#define RMQ_CAPSULE_VAR_IDX(v)				((v) & ((1UL << RMQ_CAPSULE_SIZE_LOG2) - 1))
#define RMQ_CAPSULE_VAR_RND(v)				((v) >> RMQ_CAPSULE_SIZE_LOG2)
#define RMQ_CAPSULE_VAR_COMPOSE(h, l)			(((h) << RMQ_CAPSULE_SIZE_LOG2) | (l))

#define RMQ_VARIABLE_POS_BITS				50
#define RMQ_VARIABLE_POS(v)				((v) & ((1ULL << RMQ_VARIABLE_POS_BITS) - 1))
#define RMQ_VARIABLE_RATIO(v)				((v) >> RMQ_VARIABLE_POS_BITS)
#define RMQ_VARIABLE_COMPOSE(h, l)			(((h) << RMQ_VARIABLE_POS_BITS) | (l))

typedef struct rmq_info_s {
	__u64				memsize;
	vatomic32_t			record_disabled;
	vatomic32_t			snapshot;
	vatomic32_t			snapshot_allocated;
	vatomic32_t			clock_id;
	vatomic32_t			tracing_block_enabled;
} RMQ_ATTR_CACHELINE_ALIGNED rmq_info_t;

typedef struct rmq_metadata_s {
	vatomic64_t			time_stamp;
	vatomic64_t			allocated;
	vatomic64_t			committed;
} RMQ_ATTR_CACHELINE_ALIGNED rmq_metadata_t;

typedef struct rmq_datahead_s {
	__u64				time_stamp;
	__u64				producer_stamp;
	__u64				consumer_stamp;
	__u8				core_id;
} __attribute__((packed)) rmq_datahead_t;

typedef struct rmq_entry_s {
	__u32 				time_stamp_offset;
	__u16				size;
	__u8				data[];
} __attribute__((packed)) rmq_entry_t;

#define RMQ_CAPSULE_INIT_SIZE		(sizeof(rmq_datahead_t))
#define RMQ_CAPSULE_DATA_SIZE		(RMQ_CAPSULE_SIZE - RMQ_CAPSULE_INIT_SIZE)
#define RMQ_CAPSULE_BOUND		(RMQ_CAPSULE_DATA_SIZE - RMQ_ENTRY_METADATA_SIZE)

typedef union rmq_data_s {
	rmq_datahead_t			head;
	__u8				buf[RMQ_CAPSULE_SIZE];
} RMQ_ATTR_CACHELINE_ALIGNED rmq_data_t;
#ifdef __KERNEL__
static_assert(sizeof(rmq_data_t) == RMQ_CAPSULE_SIZE, sizeof_rmq_data_t_not_equal_RMQ_CAPSULE_SIZE);
#endif

typedef struct rmq_resize_callback_s {
	void *				func;
	__u8				args[RMQ_RESIZE_SHRINK_CALLBACK_ARGS_SIZE];
} RMQ_ATTR_CACHELINE_ALIGNED rmq_resize_callback_t;

typedef struct rmq_resize_s {
	vatomic64_t			flag;
	vatomic64_t			gpos_last;
	vatomic64_t			epoch_last;
	rmq_resize_callback_t		callback;
} RMQ_ATTR_CACHELINE_ALIGNED rmq_resize_t;

typedef struct rmq_global_s {
	vatomic64_t			ratio_and_gpos;
	rmq_resize_t			resize;
	vatomic32_t			dequeue_futex_waiters;
	vatomic32_t			dequeue_futex;
	vatomic32_t			shrink_futex;
	vatomic32_t			snapshot_shrink_futex;
} RMQ_ATTR_CACHELINE_ALIGNED rmq_global_t;

typedef struct rmq_producer_s {
	vatomic64_t			ratio_and_ppos;
} RMQ_ATTR_CACHELINE_ALIGNED rmq_producer_t;

typedef struct rmq_consumer_last_s {
	__u64				cpos;
	__u64				consumed_idx;
	__u64				producer_stamp;
	__u64				consumer_stamp;
} rmq_consumer_last_t;

typedef struct rmq_consumer_s {
	vatomic64_t			epoch;
	__u64				cpos;
	__u64				consumed_idx;
	__u64				reserved_idx;
	rmq_data_t			data;
	rmq_consumer_last_t		last;
} RMQ_ATTR_CACHELINE_ALIGNED rmq_consumer_t;

typedef struct rmq_producer_cache_s {
	__u64				ratio_and_ppos_last;
	__u64				rnd;
	rmq_metadata_t *		mc;
	rmq_data_t *			dc;
} rmq_producer_cache_t;

typedef struct rmq_producer_entry_s {
	void *				handler;
	void *				addr;
} rmq_producer_entry_t;

typedef struct rmq_consumer_entry_s {
	void *				handler;
	void *				addr;
	__u64				time_stamp;
	__u8				core_id;
} rmq_consumer_entry_t;

typedef struct rmq_consumer_buffer_s {
	__u64				time_stamp;
	__u8				core_id;
	__u8				data[];
} __attribute__((packed)) rmq_consumer_buffer_t;

typedef struct rmq_consumer_buffer_batch_head_s {
	__u64				time_stamp;
	__u64				length;
	__u8				core_id;
} __attribute__((packed)) rmq_consumer_buffer_batch_head_t;

typedef struct rmq_consumer_buffer_batch_s {
	rmq_consumer_buffer_batch_head_t head;
	__u8				data[RMQ_CAPSULE_DATA_SIZE];
} __attribute__((packed)) RMQ_ATTR_PAGE_ALIGNED rmq_consumer_buffer_batch_t;
#ifdef __KERNEL__
static_assert(sizeof(rmq_consumer_buffer_batch_t) == sizeof(rmq_data_t), sizeof_rmq_consumer_buffer_batch_t_not_equal);
#endif

#define RMQ_CBB_METADATA_SIZE		sizeof(rmq_consumer_buffer_batch_head_t)
#define RMQ_ENTRY_METADATA_SIZE		sizeof(rmq_entry_t)
#define RMQ_MAXINUM_DATA_SIZE		(RMQ_CAPSULE_BOUND - RMQ_CAPSULE_INIT_SIZE)
#endif
