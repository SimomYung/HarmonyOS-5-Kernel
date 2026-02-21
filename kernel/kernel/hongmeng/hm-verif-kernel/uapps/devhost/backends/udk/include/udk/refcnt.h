/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 * Description: Prototypes of refcnt in udk
 * Author: Huawei OS Kernel Lab
 * Create: Thu Nov 12 15:36:29 2020
 */

#ifndef	__UDK_REFCNT_H__
#define	__UDK_REFCNT_H__

struct udk_refcnt;
typedef void (*udk_refcnt_release_func)(struct udk_refcnt *);

struct udk_refcnt *udk_refcnt_alloc(int count, udk_refcnt_release_func func, void *owner);
void *udk_refcnt_owner_of(struct udk_refcnt *refcnt);
int udk_refcnt_counter(struct udk_refcnt *refcnt);
int udk_refcnt_get(struct udk_refcnt *refcnt);
int udk_refcnt_put(struct udk_refcnt *refcnt);
int udk_refcnt_release(struct udk_refcnt *refcnt);

#endif /* __UDK_REFCNT_H__ */
