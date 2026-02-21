/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: Prototypes for devhost process APIs
 * Author: Huawei OS Kernel Lab
 * Create: Mon July 1 10:04:17 2024
 */
#ifndef __DEVHOST_API_PROCESS_H__
#define __DEVHOST_API_PROCESS_H__

/*
 * CAUSION: This file will be used in linux source,
 *          **DO NOT** include any HMOS headers.
 */
typedef void (*task_exit_notify_fn_t)(unsigned long pid, unsigned long uid);
void devhost_task_exit_notify_register(task_exit_notify_fn_t func);

#endif /* __DEVHOST_API_PROCESS_H__ */
