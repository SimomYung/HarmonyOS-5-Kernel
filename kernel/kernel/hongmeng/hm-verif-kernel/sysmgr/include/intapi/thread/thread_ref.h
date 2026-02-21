/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: thread ref interface
 * Author: Huawei OS Kernel Lab
 * Create: Fri Feb 2 16:35:22 2024
 */

#ifndef H_SYSMGR_INCLUDE_THREAD_REF_H
#define H_SYSMGR_INCLUDE_THREAD_REF_H
#include <stdbool.h>
#include <stdint.h>

struct sched_attr;

int thread_ref_sched_trans(cref_t src_cref, cref_t dst_cref, bool restore);

uintptr_t thread_ref_of_current(void);
int thread_ref_get(uintptr_t thread_ptr);
void thread_ref_put(uintptr_t thread_ptr);
void thread_ref_set_clear_binder_flag(uintptr_t thread_ptr, bool is_set);
uintptr_t thread_ref_binder_threads_of(uintptr_t thread_ptr, uintptr_t new_binder_threads_ptr);
#endif
