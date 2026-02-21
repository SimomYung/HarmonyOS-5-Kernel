/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Implementations of sem in udk
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jul 20 17:41:25 2021
 */
#include <udk/sem.h>

#include <hongmeng/errno.h>
#include <libhmsync/raw_sem.h>

int udk_sem_init(struct udk_sem *sem, int value)
{
	return -hmerrno2posix(raw_sem_init((struct raw_sem *)(uintptr_t)sem,
					   (unsigned int)value));
}

int udk_sem_wait(struct udk_sem *sem)
{
	return -hmerrno2posix(raw_sem_wait((struct raw_sem *)(uintptr_t)sem));
}

int udk_sem_kill(struct udk_sem *sem)
{
	return -hmerrno2posix(raw_sem_wait_killable((struct raw_sem *)(uintptr_t)sem));
}

int udk_sem_timedwait(struct udk_sem *sem, const struct timespec *at)
{
	return -hmerrno2posix(raw_sem_timedwait((struct raw_sem *)(uintptr_t)sem, at));
}

int udk_sem_post(struct udk_sem *sem)
{
	return -hmerrno2posix(raw_sem_post((struct raw_sem *)(uintptr_t)sem));
}
