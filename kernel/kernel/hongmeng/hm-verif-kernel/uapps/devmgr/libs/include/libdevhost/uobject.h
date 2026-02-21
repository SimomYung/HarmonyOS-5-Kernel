/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 * Description: Libdh uobject set header
 * Author: Huawei OS Kernel Lab
 * Create: Tue Apr 07 10:58:46 2020
 */

#ifndef ULIBS_INCLUDE_DEVHOST_UOBJECT_H
#define ULIBS_INCLUDE_DEVHOST_UOBJECT_H

#include <stdbool.h>
#include <hmkernel/errno.h>
#include <libhmsync/raw_mutex.h>
#include <libhmsync/raw_mutex_guard.h>
#include <libhmsync/raw_scopedptr.h>
#include <lib/dlist.h>

struct uobject;
typedef void (*uobject_release_func)(struct uobject *obj);

struct uobject {
	struct raw_refcnt refcnt;
	struct dlist_node node;
	uobject_release_func release;
};

void uobject_init(struct uobject *obj, int count, uobject_release_func func);

static inline __hmerr_t uobject_get(struct uobject *obj)
{
	__hmerr_t ret = E_HM_INVAL;
	if (obj != NULL) {
		ret = raw_refcnt_get(&obj->refcnt);
		BUG_ON(ret < 0);
	}
	return ret;
}

static inline __hmerr_t uobject_put(struct uobject *obj)
{
	__hmerr_t ret = E_HM_INVAL;
	if (obj != NULL) {
		ret = raw_refcnt_put(&obj->refcnt);
	}
	return ret;
}

static inline __hmerr_t uobject_release(struct uobject *obj)
{
	__hmerr_t ret = E_HM_INVAL;
	if (obj != NULL) {
		ret = raw_refcnt_release(&obj->refcnt);
	}
	return ret;
}

static inline __hmerr_t uobject_counter(struct uobject *obj)
{
	__hmerr_t ret = E_HM_INVAL;
	if (obj != NULL) {
		ret = raw_refcnt_counter(&obj->refcnt);
	}
	return ret;
}

typedef bool (*uobject_set_cmp_func)(const struct uobject *obj, void *arg);
typedef int (*uobject_set_foreach_func)(struct uobject *obj, void *arg);

struct uobject_set {
	struct dlist_node head;
	struct raw_mutex mutex; /* It must be a recursive mutex */
	uobject_set_cmp_func cmp_func;
};

void uobject_set_init(struct uobject_set *set, uobject_set_cmp_func func);
void uobject_set_destroy(struct uobject_set *set);
int uobject_set_insert_by_func(struct uobject_set *set,
			       struct uobject *obj,
			       uobject_set_cmp_func func,
			       void *arg);
struct uobject *uobject_set_acquire_by_func(struct uobject_set *set,
					    uobject_set_cmp_func func,
					    void *arg);
int uobject_set_delete(struct uobject_set *set, struct uobject *obj);
int uobject_set_foreach(struct uobject_set *set, uobject_set_foreach_func func, void *arg);

static inline __hmerr_t uobject_set_insert(struct uobject_set *set, struct uobject *obj, void *arg)
{
	__hmerr_t ret = E_HM_INVAL;
	if (set != NULL) {
		ret = uobject_set_insert_by_func(set, obj, set->cmp_func, arg);
	}
	return ret;
}

static inline struct uobject *uobject_set_acquire(struct uobject_set *set, void *arg)
{
	struct uobject *obj = NULL;
	if (set != NULL) {
		obj = uobject_set_acquire_by_func(set, set->cmp_func, arg);
	}
	return obj;
}

#endif /* ULIBS_INCLUDE_DEVHOST_UOBJECT_H */
