/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Sem interfaces of udk
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jul 20 17:39:07 2021
 */
#ifndef __UDK_SEM_H__
#define __UDK_SEM_H__

#include <vsync/atomic.h>
#include <udk/timer.h>

struct udk_sem {
	vatomic32_t value;
	vatomic32_t waiters;
};

int udk_sem_init(struct udk_sem *sem, int value);
int udk_sem_wait(struct udk_sem *sem);
int udk_sem_post(struct udk_sem *sem);
int udk_sem_kill(struct udk_sem *sem);
int udk_sem_timedwait(struct udk_sem *sem, const struct timespec *at);

#endif /* __UDK_SEM_H__ */
