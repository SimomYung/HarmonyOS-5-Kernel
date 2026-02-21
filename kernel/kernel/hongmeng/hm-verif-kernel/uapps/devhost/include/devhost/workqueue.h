/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Utils for async workqeue
 * Author: Huawei OS Kernel Lab
 * Create: Wed Aug 7 15:01:02 2024
 */

#ifndef __DEVHOST_WORKQUEUE_H__
#define __DEVHOST_WORKQUEUE_H__

#include <libhmsync/raw_workqueue.h>

#ifdef CONFIG_NET_FUSION
struct raw_workqueue *devhost_get_actvpool_wq(void);
#else
static inline struct raw_workqueue *devhost_get_actvpool_wq(void) { return NULL; }
#endif

#endif /* __DEVHOST_WORKQUEUE_H__ */
