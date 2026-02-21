/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Rwlock interfaces of udk
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jul 20 17:10:32 2021
 */
#ifndef __UDK_RWLOCK_H__
#define __UDK_RWLOCK_H__

struct udk_rwlock;

struct udk_rwlock *udk_rwlock_alloc(void);
struct udk_rwlock *udk_rwlock_alloc_shared(void);
void udk_rwlock_destroy(struct udk_rwlock *rwlock);
int udk_rwlock_rdlock(struct udk_rwlock *rwlock);
int udk_rwlock_tryrdlock(struct udk_rwlock *rwlock);
int udk_rwlock_wrlock(struct udk_rwlock *rwlock);
int udk_rwlock_trywrlock(struct udk_rwlock *rwlock);
int udk_rwlock_unlock(struct udk_rwlock *rwlock);

#endif /* __UDK_RWLOCK_H__ */
