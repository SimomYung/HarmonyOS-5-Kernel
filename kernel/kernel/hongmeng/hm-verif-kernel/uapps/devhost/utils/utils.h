/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Utils helper API prototypes
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan 11 10:45:12 2019
 */
#ifndef __DEVHOST_UTILS_H__
#define __DEVHOST_UTILS_H__

#include <hmkernel/capability.h>
#include <libhmsync/raw_workqueue.h>

#define BUILD_BUG_ON(condition) ((void)sizeof(char[1 - 2*((!!(condition)) ? 1 : 0)]))

/* async support */
typedef void (*devhost_async_func_t) (void *data);
int devhost_workq_init(void);
void devhost_async_schedule(devhost_async_func_t func, void *data);
void devhost_smpcall_wq_schedule(unsigned int cpu, void (*callback)(void *), void *data);
int devhost_workq_attr_set_name(struct raw_workqueue_attr *attr, const char *name);

#endif
