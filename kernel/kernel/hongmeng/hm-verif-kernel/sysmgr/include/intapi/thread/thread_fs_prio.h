/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: Header of set prio for fs
 * Author: Huawei OS Kernel Lab
 * Create: Thu Aug 2 11:16:28 2024
 */

#ifndef SYSMGR_INCLUDE_THREAD_FS_PRIO_H
#define SYSMGR_INCLUDE_THREAD_FS_PRIO_H

int thread_sched_extend_rt_prio_for_fs(unsigned int tid, unsigned int prio);

#endif /* SYSMGR_INCLUDE_THREAD_FS_PRIO_H */
