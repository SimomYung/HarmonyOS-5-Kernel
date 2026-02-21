/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Relaxed Multi-level Queue (RMQ)
 * Author: Huawei OS Kernel Lab
 * Create: Fri Sep 01 09:47:00 2023
 */
/*
 * This header file is the realization of RMQ algorithm. It's not allowed for you to include this file
 * directly because this file is all inline functions, which will leading to a big space consumption in the
 * text section. Currently, we add wrapper for rmq in kernel and ulibs and you can include the wrapper file
 * instead.
 */
#ifdef __HM_ENTIRE_RMQK__
#ifndef KLIBS_TRACE_RMQK_H
#define KLIBS_TRACE_RMQK_H

#include <hmkernel/types.h>
#ifdef __KERNEL__
#include <hmkernel/panic.h>
#include <lib/string.h>
#include <lib/atomic.h>
#define rmq_log_info klog_info
#else
#include <hongmeng/panic.h>
#include <libhwsecurec/securec.h>
#include <vsync/atomic.h>
#define rmq_log_info hm_info
#endif /* __KERNEL__ */

#include <lib/rmqk/rmqk_util.h>
#include <lib/rmqk/rmqk_config.h>
#include <lib/rmqk/rmqk_struct_basic.h>
#include <lib/rmqk/rmqk_struct_core.h>
#include <lib/rmqk/rmqk_entry.h>
#include <lib/rmqk/rmqk_capsule.h>
#include <lib/rmqk/rmqk_producer.h>
#include <lib/rmqk/rmqk_consumer.h>
#include <lib/rmqk/rmqk_heap.h>
#include <lib/rmqk/rmqk_consumer_sorted.h>
#include <lib/rmqk/rmqk_resize.h>
#include <lib/rmqk/rmqk_api.h>
#include <lib/rmqk/rmqk_dump.h>
#endif /* KLIBS_TRACE_RMQK_H */
#endif /* __HM_ENTIRE_RMQK__ */
