/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 * Description: Udk notifier
 * Author: Huawei OS Kernel Lab
 * Create: Tue Dec 29 09:43:06 2020
 */

#include <udk/notifier.h>

#include <stdlib.h>
#include <udk/sync.h>
#include <udk/errno.h>
#include <udk/lib/dlist.h>

struct udk_notifier {
	udk_notifier_func func;
	int priority;
	struct dlist_node node;
};

struct udk_notifier_list {
	struct dlist_node head;
};

struct udk_blocking_notifier_list {
	struct udk_rwlock *rwlock;
	struct udk_notifier_list list;
};

struct udk_notifier *udk_notifier_alloc(udk_notifier_func func, int priority)
{
	struct udk_notifier *notifier = NULL;

	if (func == NULL) {
		return NULL;
	}
	notifier = (struct udk_notifier *)malloc(sizeof(struct udk_notifier));
	if (notifier == NULL) {
		return NULL;
	}
	notifier->func = func;
	dlist_init(&notifier->node);
	notifier->priority = priority;
	return notifier;
}

void udk_notifier_destroy(struct udk_notifier *notifier)
{
	if (notifier == NULL) {
		return;
	}
	free(notifier);
}

static void udk_notifier_list_init(struct udk_notifier_list *list)
{
	dlist_init(&list->head);
}

struct udk_notifier_list *udk_notifier_list_alloc(void)
{
	struct udk_notifier_list *list = NULL;

	list = (struct udk_notifier_list *)malloc(sizeof(struct udk_notifier_list));
	if (list == NULL) {
		return NULL;
	}
	udk_notifier_list_init(list);
	return list;
}

static void udk_notifier_list_reset(struct udk_notifier_list *list)
{
	while (dlist_empty(&list->head) == 0) {
		dlist_delete(dlist_get_first(&list->head));
	}
}

void udk_notifier_list_destroy(struct udk_notifier_list *list)
{
	if (list == NULL) {
		return;
	}
	udk_notifier_list_reset(list);
	free(list);
}

int udk_notifier_list_insert(struct udk_notifier_list *list, struct udk_notifier *notifier)
{
	struct udk_notifier *pos = NULL;

	if (list == NULL || notifier == NULL) {
		return -EINVAL;
	}
	if (dlist_empty(&list->head) == 1) {
		dlist_insert_head(&list->head, &notifier->node);
	} else {
		dlist_for_each_entry(pos, &list->head, struct udk_notifier, node) {
			if (notifier == pos) {
				return -EINVAL;
			}
			if (notifier->priority > pos->priority) {
				dlist_insert(pos->node.prev, &notifier->node);
				break;
			}
			if (dlist_get_last(&list->head) == &pos->node) {
				dlist_insert_tail(&list->head, &notifier->node);
				break;
			}
		}
	}
	return 0;
}

int udk_notifier_list_delete(struct udk_notifier_list *list, struct udk_notifier *notifier)
{
	struct udk_notifier *pos = NULL;
	struct udk_notifier *n = NULL;

	if (list == NULL || notifier == NULL) {
		return -EINVAL;
	}
	dlist_for_each_entry_safe(pos, n, &list->head, struct udk_notifier, node) {
		if (notifier == pos) {
			dlist_delete(&notifier->node);
			return 0;
		}
	}
	return -ENOENT;
}

static int __notifier_list_call(const struct udk_notifier_list *list, unsigned long event,
			        int nr_to_call, int *nr_calls, void *data)
{
	struct udk_notifier *pos = NULL;
	int ret = UDK_NOTIFY_OK;

	if (list == NULL) {
		return -EINVAL;
	}
	dlist_for_each_entry(pos, &list->head, struct udk_notifier, node) {
		if (nr_to_call == 0) {
			break;
		}
		nr_to_call--;

		if (pos->func != NULL) {
			ret = pos->func(pos, event, data);
			if (((unsigned int)ret & UDK_NOTIFY_STOP) != 0) {
				break;
			}
		}

		if (nr_calls != NULL) {
			(*nr_calls)++;
		}
	}
	return ret;
}

int udk_notifier_list_call(const struct udk_notifier_list *list, unsigned long event, void *data)
{
	return __notifier_list_call(list, event, -1, NULL, data);
}

int udk_notifier_list_call_robust(const struct udk_notifier_list *list,
				  unsigned long event, unsigned long revent, void *data)
{
	int ret;
	int nr_calls = 0;

	ret = __notifier_list_call(list, event, -1, &nr_calls, data);
	if (ret & UDK_NOTIFY_STOP) {
		(void)__notifier_list_call(list, revent, nr_calls, NULL, data);
	}
	return ret;
}

struct udk_blocking_notifier_list *udk_blocking_notifier_list_alloc(void)
{
	struct udk_blocking_notifier_list *list = NULL;

	list = (struct udk_blocking_notifier_list *)malloc(sizeof(struct udk_blocking_notifier_list));
	if (list == NULL) {
		return NULL;
	}
	list->rwlock = udk_rwlock_alloc();
	if (list->rwlock == NULL) {
		free(list);
		return NULL;
	}
	udk_notifier_list_init(&list->list);
	return list;
}

void udk_blocking_notifier_list_destroy(struct udk_blocking_notifier_list *list)
{
	if (list == NULL) {
		return;
	}
	udk_notifier_list_reset(&list->list);
	if (list->rwlock != NULL) {
		free(list->rwlock);
		list->rwlock = NULL;
	}
	free(list);
}

int udk_blocking_notifier_list_insert(struct udk_blocking_notifier_list *list,
				      struct udk_notifier *notifier)
{
	int ret;

	if (list == NULL || notifier == NULL) {
		return -EINVAL;
	}
	(void)udk_rwlock_wrlock(list->rwlock);
	ret = udk_notifier_list_insert(&list->list, notifier);
	(void)udk_rwlock_unlock(list->rwlock);
	return ret;
}

int udk_blocking_notifier_list_delete(struct udk_blocking_notifier_list *list,
				      struct udk_notifier *notifier)
{
	int ret;

	if (list == NULL || notifier == NULL) {
		return -EINVAL;
	}
	(void)udk_rwlock_wrlock(list->rwlock);
	ret = udk_notifier_list_delete(&list->list, notifier);
	(void)udk_rwlock_unlock(list->rwlock);
	return ret;
}

int udk_blocking_notifier_list_call(struct udk_blocking_notifier_list *list,
				    unsigned long event, void *data)
{
	int ret;

	if (list == NULL) {
		return -EINVAL;
	}
	(void)udk_rwlock_rdlock(list->rwlock);
	ret = udk_notifier_list_call(&list->list, event, data);
	(void)udk_rwlock_unlock(list->rwlock);
	return ret;
}

int udk_blocking_notifier_list_call_robust(struct udk_blocking_notifier_list *list,
					   unsigned long event, unsigned long revent, void *data)
{
	int ret;

	if (list == NULL) {
		return -EINVAL;
	}
	(void)udk_rwlock_rdlock(list->rwlock);
	ret = udk_notifier_list_call_robust(&list->list, event, revent, data);
	(void)udk_rwlock_unlock(list->rwlock);
	return ret;
}
