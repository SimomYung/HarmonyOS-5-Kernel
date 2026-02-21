/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: internal guardmgr interfaces
 * Author: Huawei OS Kernel Lab
 * Create: Sun Jul 9 14:11:05 2023
 */

#ifndef SYSMGR_INCLUDE_GUARDMGR_H
#define SYSMGR_INCLUDE_GUARDMGR_H

typedef int (*guardmgr_hungtask_notifier)(void);

void guardmgr_log_lockowner(void);
void guardmgr_dump_lockowner(void);
int guardmgr_register_hungtask_notifier(guardmgr_hungtask_notifier notify_func);

#endif /* SYSMGR_INCLUDE_GUARDMGR_H */
