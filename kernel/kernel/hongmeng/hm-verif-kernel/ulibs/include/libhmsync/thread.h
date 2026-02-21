/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Basic interfaces of all thread models
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jan 24 12:37:33 2019 -0400
 */

#ifndef ULIB_INCLUDE_LIBHMSYNC_THREAD_H
#define ULIB_INCLUDE_LIBHMSYNC_THREAD_H

#include <unistd.h>
#include <stdbool.h>
#include <hmkernel/errno.h>
#include <hongmeng/syscall.h>
#include <hmkernel/rq.h>
#include <libhwsecurec/securec.h>

#ifdef __cplusplus
extern "C" {
#endif

extern int
thread_set_sched_priority(cref_t cref, unsigned int prio);
int thread_get_current_tid(void);
cref_t thread_tid2cref(int tid);
cref_t thread_module_sched_ref(void);

int hm_thread_set_lt_group(int tid, int group);
int hm_thread_set_lt_boost(int tid, int boost);
int hm_thread_freezer_count(int tid, bool count, bool fwd);

#ifdef __cplusplus
}
#endif

#endif
