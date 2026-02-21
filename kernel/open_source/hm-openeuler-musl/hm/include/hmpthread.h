/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jan 08 14:53:17 2019
 */

#ifndef HM_INCLUDE_HMPTHREAD_H
#define HM_INCLUDE_HMPTHREAD_H

#include <pthread.h>

pid_t pthread_mutex_get_owner(const pthread_mutex_t *m);
pid_t pthread_tid(const pthread_t t);
void *pthread_stack(const pthread_t t);

#endif
