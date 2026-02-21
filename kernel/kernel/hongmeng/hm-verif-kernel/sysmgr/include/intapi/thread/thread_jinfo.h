/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Thread Journal Info
 * Author: Huawei OS Kernel Lab
 * Create: Mon Sep 18 11:11:09 2023
 */
#ifndef H_SYSMGR_INCLUDE_THREAD_JINFO_H
#define H_SYSMGR_INCLUDE_THREAD_JINFO_H

void *get_current_thread_jinfo(void);
void set_current_thread_jinfo(void *ji);
void clear_current_thread_jinfo(void);

#endif
