/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Libdh uobject set functions
 * Author: Huawei OS Kernel Lab
 * Create: Tue Apr 07 10:58:43 2020
 */

#include <libdevhost/uobject.h>
#include <libhmlog/hmlog.h>
#include <libstrict/strict.h>

static void uobject_refcnt_release(struct raw_refcnt *refcnt)
{
	struct uobject *obj = container_of(refcnt, struct uobject, refcnt);
	if (obj->release != NULL) {
		obj->release(obj);
	}
}

void uobject_init(struct uobject *obj, int count, uobject_release_func func)
{
	raw_refcnt_init(&obj->refcnt, count, uobject_refcnt_release);
	dlist_init(&obj->node);
	obj->release = func;
}

static bool uobject_set_default_cmp_func(const struct uobject *obj, void *arg)
{
	return (obj == arg);
}

void uobject_set_init(struct uobject_set *set, uobject_set_cmp_func func)
{
	struct raw_mutex_attr attr;

	if (set != NULL) {
		dlist_init(&set->head);
		raw_mutex_attr_init(&attr);
		raw_mutex_attr_settype(&attr, RAW_MUTEX_RECURSIVE);
		raw_mutex_init_with_attr(&set->mutex, &attr);
		if (func == NULL) {
			set->cmp_func = uobject_set_default_cmp_func;
		} else {
			set->cmp_func = func;
		}
	}
}

static int uobject_set_default_destroy(struct uobject *obj, void *arg)
{
	UNUSED(arg);
	dlist_delete(&obj->node);
	(void)uobject_put(obj);
	return 0;
}

void uobject_set_destroy(struct uobject_set *set)
{
	int ret;
	ret = uobject_set_foreach(set, uobject_set_default_destroy, NULL);
	if (ret < 0) {
		hm_warn("libdh: uobject: uobject set destroy failed\n");
	}
	return;
}

static struct uobject *find_uobject_by_arg(const struct uobject_set *set, uobject_set_cmp_func func, void *arg)
{
	struct uobject *obj = NULL;
	struct uobject *result = NULL;
	dlist_for_each_entry(obj, &set->head, struct uobject, node) {
		if (func(obj, arg)) {
			result = obj;
			break;
		}
	}
	return result;
}

define_refcnt_function3(uobject,
			offset_of(struct uobject, refcnt),
			RAW_MUTEX_GUARD(_, &set->mutex),
			find_uobject_by_arg,
			struct uobject_set *, set,
			uobject_set_cmp_func, func,
			void *, arg);

struct uobject *uobject_set_acquire_by_func(struct uobject_set *set,
					    uobject_set_cmp_func func,
					    void *arg)
{
	struct uobject *obj = NULL;
	if (set != NULL) {
		obj = (struct uobject *)acquire_uobject_ptr_ref(set, func, arg);
	}
	return obj;
}

int uobject_set_insert_by_func(struct uobject_set *set,
			       struct uobject *obj,
			       uobject_set_cmp_func func,
			       void *arg)
{
	int ret = E_HM_OK;
	struct uobject *pos = NULL;
	if (set == NULL || obj == NULL) {
		ret = E_HM_INVAL;
	} else {
		RAW_MUTEX_GUARD(_, &set->mutex);
		pos = (struct uobject *)acquire_uobject_ptr_ref(set, func, arg);
		if (pos != NULL) {
			(void)uobject_put(pos);
			ret = E_HM_OBJEXIST;
		} else {
			(void)uobject_get(obj);
			dlist_insert_tail(&set->head, &obj->node);
		}
	}
	return ret;
}

int uobject_set_delete(struct uobject_set *set, struct uobject *obj)
{
	int ret = E_HM_OK;
	if (set == NULL || obj == NULL) {
		ret = E_HM_INVAL;
	} else {
		RAW_MUTEX_GUARD(_, &set->mutex);
		dlist_delete(&obj->node);
		(void)uobject_put(obj);
	}
	return ret;
}

int uobject_set_foreach(struct uobject_set *set, uobject_set_foreach_func func, void *arg)
{
	int ret = E_HM_OK;
	struct uobject *obj = NULL;
	struct uobject *tmp = NULL;
	if (set == NULL || func == NULL) {
		ret = E_HM_INVAL;
	} else {
		RAW_MUTEX_GUARD(_, &set->mutex);
		dlist_for_each_entry_safe(obj, tmp, &set->head, struct uobject, node) {
			ret = func(obj, arg);
			if (ret < 0) {
				break;
			}
		}
	}
	return ret;
}
