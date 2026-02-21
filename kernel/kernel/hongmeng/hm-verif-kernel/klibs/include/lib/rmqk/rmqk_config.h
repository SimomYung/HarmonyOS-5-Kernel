/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: RMQ
 * Author: Huawei OS Kernel Lab
 * Create: Wed Sep 20 17:13:09 2023
 */
#ifndef KLIBS_TRACE_RMQK_CONFIG_H
#define KLIBS_TRACE_RMQK_CONFIG_H

#ifndef RMQ_CONSUMER_MODE
#define RMQ_CONSUMER_MODE				RMQ_CONSUMER_MODE_CLOSING
#endif

#ifndef RMQ_CACHELINE_SIZE
#define RMQ_CACHELINE_SIZE				128UL
#endif

#ifndef RMQ_PAGE_SIZE
#define RMQ_PAGE_SIZE					4096UL
#endif

#ifndef RMQ_CAPSULE_SIZE
#define RMQ_CAPSULE_SIZE				RMQ_PAGE_SIZE
#endif

#ifndef RMQ_RESIZE_SHRINK_CALLBACK_STATIC
extern void hm_trace_rmq_shrink_wake(void);
#define RMQ_RESIZE_SHRINK_CALLBACK_STATIC		1
#endif

#ifndef RMQ_RESIZE_SHRINK_CALLBACK_ARGS_SIZE
#define RMQ_RESIZE_SHRINK_CALLBACK_ARGS_SIZE		(RMQ_CACHELINE_SIZE - sizeof(void *))
#endif

#ifndef RMQ_CONSUMER_SORTED_USING_HEAP
#define RMQ_CONSUMER_SORTED_USING_HEAP			1
#endif

#endif
