/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Thread tid related info
 * Author: Huawei OS Kernel Lab
 * Create: Tue Oct 31 13:12:36 2023
 */
#ifndef H_SYSMGR_INCLUDE_THREAD_TID_H
#define H_SYSMGR_INCLUDE_THREAD_TID_H

#include <bits/alltypes.h>

int tid_seq_of(pid_t tid, unsigned int nsid, uint64_t *seq);

#endif
