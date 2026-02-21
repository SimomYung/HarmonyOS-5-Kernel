/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: interface for network can filter
 * Author: Huawei OS Kernel Lab
 * Create: Mon Dec 30 19:27:00 2019
 */

#include <stdio.h>

#include <lib/dlist.h>
#include <libhmsync/raw_rwlock.h>
#include <libstrict/strict.h>

#include "ux_can_filter.h"

typedef struct ux_can_filter {
	struct dlist_node head;
	uint32_t can_id;
	uint32_t can_mask;
} ux_can_filter_t;

static struct raw_rwlock ux_can_filter_lock = RAW_RDLOCK_INITIALIZER;
static DLIST_HEAD(ux_can_filter_list);

#define filter_for_each_entry(pos) \
	dlist_for_each_entry(pos, &ux_can_filter_list, ux_can_filter_t, head)

#define filter_for_each_entry_safe(pos, n) \
	dlist_for_each_entry_safe(pos, n, &ux_can_filter_list, \
				  ux_can_filter_t, head)

static ux_can_filter_t *__ux_can_filter_alloc(void)
{
	ux_can_filter_t *filter = NULL;

	filter = malloc(sizeof(ux_can_filter_t));
	if (filter == NULL) {
		return NULL;
	}

	dlist_init(&filter->head);

	return filter;
}

static int __ux_can_filter_register(ux_can_filter_t *filter)
{
	dlist_insert_tail(&ux_can_filter_list, &filter->head);
	return 0;
}

static void __ux_can_filter_unregister(ux_can_filter_t *filter)
{
	dlist_delete(&filter->head);
}

int ux_can_filters_register(ux_netdev_t *dev, struct can_filter *filter,
			   int count)
{
	ux_can_filter_t *cfilter = NULL;
	int i;

	raw_rwlock_wrlock(&ux_can_filter_lock);
	for (i = 0; i < count; i++) {
		cfilter = __ux_can_filter_alloc();
		if (cfilter == NULL) {
			raw_rwlock_unlock(&ux_can_filter_lock);
			return E_HM_NOMEM;
		}

		cfilter->can_id = filter->can_id;
		cfilter->can_mask = filter->can_mask;

		if (__ux_can_filter_register(cfilter)) {
			raw_rwlock_unlock(&ux_can_filter_lock);
			return E_HM_INVAL;
		}
	}
	raw_rwlock_unlock(&ux_can_filter_lock);

	return 0;
}

void ux_can_filters_unregister(ux_netdev_t *dev, struct can_filter *filter,
			      int count)
{
	ux_can_filter_t *cfilter = NULL;
	ux_can_filter_t *tmp = NULL;

	raw_rwlock_wrlock(&ux_can_filter_lock);
	filter_for_each_entry_safe(cfilter, tmp) {
		if (cfilter->can_id == filter->can_id &&
		    cfilter->can_mask == filter->can_mask) {
			__ux_can_filter_unregister(cfilter);
		}
	}
	raw_rwlock_unlock(&ux_can_filter_lock);
}

int ux_can_filters_dispatch(ux_netdev_t *dev, ux_pktbuf_t *pkt)
{
	ux_can_filter_t *filter = NULL;

	raw_rwlock_rdlock(&ux_can_filter_lock);

	filter_for_each_entry(filter) {
	}

	raw_rwlock_unlock(&ux_can_filter_lock);

	return 0;
}
