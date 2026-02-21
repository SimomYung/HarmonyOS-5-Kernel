/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Headerfile for idr
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan 11 10:45:12 2019
 */
#ifndef ULIBS_LIBALGO_IDR_H
#define ULIBS_LIBALGO_IDR_H

#include <stdlib.h>
#include <limits.h>
#include <libalgo/radix_tree.h>
#include <libhmsync/raw_rwlock.h>

struct idr {
	int next;
	struct raw_rwlock lock;
	struct radix_tree_root root;
};

#define IDR_MIN_ID ((int)0)
#define IDR_MAX_ID INT_MAX

#define IDR_INIT(name) { .next = IDR_MIN_ID, .root = RADIX_TREE_INIT, .lock = RAW_RDLOCK_INITIALIZER }
#define DEFINE_IDR(name) struct idr name = IDR_INIT(name)

void idr_init(struct idr *idp);
void idr_free(struct idr *idp);
void idr_free_nolock(struct idr *idp);
int idr_alloc(struct idr *idr, void *item);
int idr_alloc_cyclic(struct idr *idr, void *item, int start, int end);
void *idr_remove(struct idr *idp, unsigned int id);
void *idr_remove_nolock(struct idr *idp, unsigned int id);
void *idr_find(struct idr *idp, unsigned int id);
void *idr_find_nolock(struct idr *idp, unsigned int id);
int idr_set(struct idr *idp, void *item, unsigned int id);
int idr_set_nolock(struct idr *idp, void *item, unsigned int id);
bool idr_is_empty(struct idr *idp);
int idr_last_id_of(struct idr *idp);
int idr_set_next_id(struct idr *idp, int next);
void *idr_get_next(struct idr *idp, unsigned int *nextidp);
void *idr_replace(struct idr *idp, void *item, unsigned int id);
int idr_for_each(struct idr *idp, int (*callback)(unsigned int id, void *ptr, void* data), void *data);
void *idr_find_ex(struct idr *idp, unsigned int id, bool ignore_invalid_flag,
		  int (*hook)(void *ptr, bool ignore_invalid_flag));

#endif  /* ULIBS_LIBALGO_IDR_H */
