/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Implementation of mutex in udk
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jul 20 16:44:03 2021
 */
#include <udk/mutex.h>

#include <pthread.h>
#include <hongmeng/errno.h>
#include <libhmsync/raw_mutex.h>
#include <hongmeng/compiler.h>

raw_static_assert(sizeof(struct udk_mutex) == sizeof(struct raw_mutex), udk_mutex_not_same_as_raw_mutex);

int udk_shared_mutex_init(void *mutex)
{
	return pthread_mutex_init((pthread_mutex_t *)(uintptr_t)mutex, NULL);
}

int udk_shared_mutex_lock(void *mutex)
{
	return pthread_mutex_lock((pthread_mutex_t *)(uintptr_t)mutex);
}

int udk_shared_mutex_unlock(void *mutex)
{
	return pthread_mutex_unlock((pthread_mutex_t *)(uintptr_t)mutex);
}

void udk_mutex_init(struct udk_mutex *mutex)
{
	raw_mutex_init((struct raw_mutex *)(uintptr_t)mutex);
}

void udk_mutex_init_recursive(struct udk_mutex *mutex)
{
	raw_mutex_init_recursive((struct raw_mutex *)(uintptr_t)mutex);
}

void udk_mutex_lock(struct udk_mutex *mutex)
{
	raw_mutex_lock((struct raw_mutex *)(uintptr_t)mutex);
}

void udk_mutex_unlock(struct udk_mutex *mutex)
{
	raw_mutex_unlock((struct raw_mutex *)(uintptr_t)mutex);
}

int udk_mutex_trylock(struct udk_mutex *mutex)
{
	return -hmerrno2posix(raw_mutex_trylock((struct raw_mutex *)(uintptr_t)mutex));
}
