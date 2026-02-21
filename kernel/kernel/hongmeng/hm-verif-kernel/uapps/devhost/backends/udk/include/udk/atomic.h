/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 * Description: Prototypes of atomic in udk
 * Author: Huawei OS Kernel Lab
 * Create: Mon Nov 16 15:32:36 2020
 */
#ifndef	__UDK_ATOMIC_H__
#define	__UDK_ATOMIC_H__

struct udk_atomic_int;

struct udk_atomic_int *udk_atomic_int_alloc(int val);
void udk_atomic_int_destroy(struct udk_atomic_int *v);
int udk_atomic_int_add(struct udk_atomic_int *v, int val);
int udk_atomic_int_sub(struct udk_atomic_int *v, int val);
int udk_atomic_int_inc(struct udk_atomic_int *v);
int udk_atomic_int_dec(struct udk_atomic_int *v);
int udk_atomic_int_set(struct udk_atomic_int *v, int val);
int udk_atomic_int_read(struct udk_atomic_int *v);
int udk_atomic_int_swap(struct udk_atomic_int *v, int val);
int udk_atomic_int_cas(struct udk_atomic_int *v, int old_v, int new_v);
int udk_atomic_int_try_cmpxchg(struct udk_atomic_int *v, int *old_v, int new_v);

#endif /* __UDK_ATOMIC_H__ */
