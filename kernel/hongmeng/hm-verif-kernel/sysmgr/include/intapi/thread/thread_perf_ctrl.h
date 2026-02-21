/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Header of perf ctrl
 * Author: Huawei OS Kernel Lab
 * Create: Thu Aug 29 11:16:28 2023
 */

#ifndef SYSMGR_INCLUDE_THREAD_PERF_CTRL_H
#define SYSMGR_INCLUDE_THREAD_PERF_CTRL_H

int dev_perf_ctrl_ioctl(unsigned int cmd, unsigned long arg);
int perf_ctrl_set_task_min_util(void *uarg);
#endif /* SYSMGR_INCLUDE_THREAD_PERF_CTRL_H */
