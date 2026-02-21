/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: UDK fops core implementation
 * Author: Huawei OS Kernel Lab
 * Create: Thur May 25 15:50:54 2023
 */

#ifndef __UDK_INTERNAL_WORKQUEUE_H__
#define __UDK_INTERNAL_WORKQUEUE_H__

struct udk_workqueue;
int udk_workqueue_default_init(struct udk_workqueue **wq);

#endif
