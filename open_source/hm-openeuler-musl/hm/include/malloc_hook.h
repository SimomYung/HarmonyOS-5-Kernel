/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021 ~ 2021. All rights reserved.
 * Description: Atfork hook interfaces for malloc
 * Author: Huawei OS Kernel Lab
 * Create: Thu Dec 09 15:50:08 2021
 */

#ifndef HM_INCLUDE_MALLOC_HOOK_H
#define HM_INCLUDE_MALLOC_HOOK_H

void malloc_lock_parent(void);
void malloc_unlock_parent(void);
void malloc_unlock_child(void);
void malloc_reclaim_thread_memory(long long);

#endif /* HM_INCLUDE_MALLOC_HOOK_H */
