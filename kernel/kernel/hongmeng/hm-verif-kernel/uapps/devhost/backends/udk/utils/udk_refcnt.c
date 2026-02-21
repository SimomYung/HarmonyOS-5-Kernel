/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 * Description: Implementation of refcnt in udk
 * Author: Huawei OS Kernel Lab
 * Create: Thu Nov 12 15:24:37 2020
 */

#include <udk/refcnt.h>

#include <stdlib.h>
#include <errno.h>
#include <lib/utils.h>
#include <libhmsync/raw_scopedptr.h>

struct udk_refcnt {
	struct raw_refcnt refcnt;
	udk_refcnt_release_func release;
	void *owner;
};

static void udk_refcnt_raw_release(struct raw_refcnt *refcnt)
{
	struct udk_refcnt *ptr = container_of(refcnt, struct udk_refcnt, refcnt);

	if (ptr->release != NULL) {
		ptr->release(ptr);
	}
	free(ptr);
}

struct udk_refcnt *udk_refcnt_alloc(int count, udk_refcnt_release_func func, void *owner)
{
	struct udk_refcnt *refcnt = NULL;

	refcnt = (struct udk_refcnt *)malloc(sizeof(struct udk_refcnt));
	if (refcnt == NULL) {
		return NULL;
	}
	refcnt->release = func;
	refcnt->owner = owner;
	raw_refcnt_init(&refcnt->refcnt, count, udk_refcnt_raw_release);
	return refcnt;
}

void *udk_refcnt_owner_of(struct udk_refcnt *refcnt)
{
	if (refcnt == NULL) {
		return NULL;
	}
	return refcnt->owner;
}

static int udk_refcnt_wrapper(struct udk_refcnt *refcnt, int (*func)(struct raw_refcnt *))
{
	int ret;

	if (refcnt == NULL) {
		return -EINVAL;
	}
	ret = func(&refcnt->refcnt);
	if (ret < 0) {
		return -hmerrno2posix(ret);
	}
	return ret;
}

int udk_refcnt_counter(struct udk_refcnt *refcnt)
{
	return udk_refcnt_wrapper(refcnt, raw_refcnt_counter);
}

int udk_refcnt_get(struct udk_refcnt *refcnt)
{
	return udk_refcnt_wrapper(refcnt, raw_refcnt_get);
}

int udk_refcnt_put(struct udk_refcnt *refcnt)
{
	return udk_refcnt_wrapper(refcnt, raw_refcnt_put);
}

int udk_refcnt_release(struct udk_refcnt *refcnt)
{
	return udk_refcnt_wrapper(refcnt, raw_refcnt_release);
}
