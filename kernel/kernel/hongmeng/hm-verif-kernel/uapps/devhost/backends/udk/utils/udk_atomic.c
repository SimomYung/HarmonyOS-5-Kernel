/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 * Description: Implementation of atomic in udk
 * Author: Huawei OS Kernel Lab
 * Create: Mon Nov 16 15:24:22 2020
 */

#include <udk/atomic.h>
#include <libhmsync/atomic.h>
#include <stdlib.h>

struct udk_atomic_int *udk_atomic_int_alloc(int val)
{
	struct udk_atomic_int *v = NULL;

	v = malloc(sizeof(raw_atomic_int_t));
	if (v == NULL) {
		return NULL;
	}
	raw_atomic_int_init((raw_atomic_int_t *)v, val);
	return v;
}

void udk_atomic_int_destroy(struct udk_atomic_int *v)
{
	if (v != NULL) {
		free(v);
	}
}

int udk_atomic_int_add(struct udk_atomic_int *v, int val)
{
	return raw_atomic_int_add((raw_atomic_int_t *)v, val);
}

int udk_atomic_int_sub(struct udk_atomic_int *v, int val)
{
	return raw_atomic_int_sub((raw_atomic_int_t *)v, val);
}

int udk_atomic_int_inc(struct udk_atomic_int *v)
{
	return raw_atomic_int_inc((raw_atomic_int_t *)v);
}

int udk_atomic_int_dec(struct udk_atomic_int *v)
{
	return raw_atomic_int_dec((raw_atomic_int_t *)v);
}

int udk_atomic_int_set(struct udk_atomic_int *v, int val)
{
	return raw_atomic_int_set((raw_atomic_int_t *)v, val);
}

int udk_atomic_int_read(struct udk_atomic_int *v)
{
	return raw_atomic_int_read((raw_atomic_int_t *)v);
}

int udk_atomic_int_swap(struct udk_atomic_int *v, int val)
{
	return raw_atomic_int_swap((raw_atomic_int_t *)v, val);
}

int udk_atomic_int_cas(struct udk_atomic_int *v, int old_v, int new_v)
{
	return raw_atomic_int_cas((raw_atomic_int_t *)v, old_v, new_v);
}

int udk_atomic_int_try_cmpxchg(struct udk_atomic_int *v, int *old_v, int new_v)
{
	return raw_atomic_int_try_cmpxchg((raw_atomic_int_t *)v, old_v, new_v);
}
