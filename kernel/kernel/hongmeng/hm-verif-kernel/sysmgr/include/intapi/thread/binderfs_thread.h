/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: thread interface for binder
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jan 15 17:52:58 2024
 */

#ifndef H_SYSMGR_INCLUDE_BINDERFS_THREAD_H
#define H_SYSMGR_INCLUDE_BINDERFS_THREAD_H

int thread_tokens_of_credential(unsigned long credential, __u64 *tokenid, __u64 *ftokenid);

#endif