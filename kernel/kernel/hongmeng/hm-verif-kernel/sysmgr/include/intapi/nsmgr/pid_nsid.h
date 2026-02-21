/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Get nsid of caller's pidns
 * Author: Huawei OS Kernel Lab
 * Create: Thu Nov 02 21:33:42 2023
 */

#ifndef SYSMGR_INCLUDE_NSMGR_PID_NSID_H
#define SYSMGR_INCLUDE_NSMGR_PID_NSID_H
int nsmgr_pid_nsid_get(unsigned int *nsid);
int nsmgr_pid_nsid_put(unsigned int nsid);
int ns_level_of(unsigned int nsid, unsigned int *level);
int acquire_level_by_cnode_idx(uint32_t cnode_idx, unsigned int *level);
#endif
