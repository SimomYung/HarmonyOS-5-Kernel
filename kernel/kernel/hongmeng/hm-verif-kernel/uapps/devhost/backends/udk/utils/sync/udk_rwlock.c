/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Implementations of rwlock in udk
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jul 20 17:10:13 2021
 */
#include <udk/rwlock.h>

#include <stdlib.h>
#include <hongmeng/errno.h>
#include <libhmsync/raw_rwlock.h>

struct udk_rwlock *udk_rwlock_alloc(void)
{
	struct raw_rwlock *lock = NULL;

	lock = (struct raw_rwlock *)malloc(sizeof(struct raw_rwlock));
	if (lock == NULL) {
		return NULL;
	}
	raw_rwlock_init(lock, NULL);
	return (struct udk_rwlock *)lock;
}

struct udk_rwlock *udk_rwlock_alloc_shared(void)
{
	struct raw_rwlock *lock = NULL;

	lock = (struct raw_rwlock *)malloc(sizeof(struct raw_rwlock));
	if (lock == NULL) {
		return NULL;
	}
	raw_rwlock_init_shared(lock);
	return (struct udk_rwlock *)lock;
}

void udk_rwlock_destroy(struct udk_rwlock *rwlock)
{
	if (rwlock == NULL) {
		return;
	}
	raw_rwlock_destroy((struct raw_rwlock *)rwlock);
	free(rwlock);
}

int udk_rwlock_rdlock(struct udk_rwlock *rwlock)
{
	return -hmerrno2posix(raw_rwlock_rdlock((struct raw_rwlock *)rwlock));
}

int udk_rwlock_tryrdlock(struct udk_rwlock *rwlock)
{
	return -hmerrno2posix(raw_rwlock_tryrdlock((struct raw_rwlock *)rwlock));
}

int udk_rwlock_wrlock(struct udk_rwlock *rwlock)
{
	return -hmerrno2posix(raw_rwlock_wrlock((struct raw_rwlock *)rwlock));
}

int udk_rwlock_trywrlock(struct udk_rwlock *rwlock)
{
	return -hmerrno2posix(raw_rwlock_trywrlock((struct raw_rwlock *)rwlock));
}

int udk_rwlock_unlock(struct udk_rwlock *rwlock)
{
	return -hmerrno2posix(raw_rwlock_unlock((struct raw_rwlock *)rwlock));
}
