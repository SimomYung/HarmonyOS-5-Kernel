/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: intapi of actv pool
 * Author: Huawei OS Kernel Lab
 * Create: Wed July 24 15:55:19 2024
 */

#ifndef INTAPI_ACTIVATION_ACTV_POOL_H
#define INTAPI_ACTIVATION_ACTV_POOL_H

#include <libhmsync/raw_workqueue.h>

struct raw_workqueue *sysmgr_get_actvpool_wq(void);

#endif /* !INTAPI_ACTIVATION_ACTV_POOL_H */
