/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Declaration of semaphore
 * Author: Huawei OS Kernel Lab
 * Create: Mon Aug 05 15:38:21 2019
 */
#ifndef LIBHMSYNC_RAW_SEM_H
#define LIBHMSYNC_RAW_SEM_H

#include <time.h>
#include <vsync/atomic.h>

#define RAW_SAM_VALUE_MAX 0x7fffffff
struct raw_sem {
	vatomic32_t value;
	vatomic32_t waiters;
};

#define __RAW_SEM_INITIALIZER(n)	\
{					\
	.value = VATOMIC_INIT(n),	\
	.waiters = VATOMIC_INIT(0),	\
}

#define DEFINE_RAW_SEM(name)	\
	struct raw_sem name = __RAW_SEM_INITIALIZER(1)

#define DEFINE_RAW_SEM_WITH_VALUE(name, n)	\
	struct raw_sem name = __RAW_SEM_INITIALIZER(n)

int raw_sem_init(struct raw_sem *sem, unsigned int value);
int raw_sem_trywait(struct raw_sem *sem);
int raw_sem_timedwait(struct raw_sem *sem, const struct timespec *at);
int raw_sem_wait(struct raw_sem *sem);
int raw_sem_post(struct raw_sem *sem);
int raw_sem_wait_killable(struct raw_sem *sem);
int raw_sem_getvalue(struct raw_sem *sem);
int raw_sem_getwaiter(struct raw_sem *sem);

#endif
