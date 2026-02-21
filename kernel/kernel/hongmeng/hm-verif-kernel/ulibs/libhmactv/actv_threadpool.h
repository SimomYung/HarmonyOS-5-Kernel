/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Actv thread pool function declaration
 * Author: Huawei OS Kernel Lab
 * Create: Mon Apr 20 11:31:54 2020
 */
#ifndef ULIBS_LIBHMACTV_ACTV_THREADPOOL_H
#define ULIBS_LIBHMACTV_ACTV_THREADPOOL_H

#include "../libhmsync/raw_thread_impl.h"

struct raw_rwlock;

int actv_threadpool_insert(struct raw_thread *thread);
void actv_threadpool_remove(struct raw_thread *thread);
struct raw_thread *actv_threadpool_search(unsigned int actv_id);
struct raw_thread *actv_threadpool_search_nolock(unsigned int actv_id);
struct raw_rwlock *actv_threadpool_lock(void);

#endif
