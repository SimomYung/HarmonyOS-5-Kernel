/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: RMQ
 * Author: Huawei OS Kernel Lab
 * Create: Tue Apr 16 20:37:34 2024
 */
#ifndef KLIBS_TRACE_RMQK_STRUCT_CORE_H
#define KLIBS_TRACE_RMQK_STRUCT_CORE_H

#define RMQ_METADATA_SIZE				sizeof(rmq_t)

#ifndef RMQ_CORE_NUM
#define RMQ_CORE_NUM					32UL
#endif

#ifndef RMQ_ACTIVE_CAPSULE_NUM
#define RMQ_ACTIVE_CAPSULE_NUM				(64UL)
#endif

#ifndef RMQ_RETRY_NUM
#define RMQ_RETRY_NUM					RMQ_ACTIVE_CAPSULE_NUM
#endif

typedef struct rmq_s {
	rmq_info_t			info RMQ_ATTR_PAGE_ALIGNED;
	rmq_global_t			global RMQ_ATTR_PAGE_ALIGNED;
	rmq_producer_t			producer[RMQ_CORE_NUM] RMQ_ATTR_CACHELINE_ALIGNED;
	rmq_consumer_t			consumer RMQ_ATTR_CACHELINE_ALIGNED;
	rmq_metadata_t			metadata[RMQ_ACTIVE_CAPSULE_NUM] RMQ_ATTR_CACHELINE_ALIGNED;
	rmq_data_t			data[] RMQ_ATTR_PAGE_ALIGNED;
} rmq_t RMQ_ATTR_CACHELINE_ALIGNED;

#endif /* KLIBS_TRACE_RMQK_STRUCT_CORE_H */
