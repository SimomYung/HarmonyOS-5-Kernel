/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: lnxbase shrinker internal APIs
 * Author: Huawei OS Kernel Lab
 * Create: Tue Apr 11 20:26:33 2023
 */

#ifndef __LNXBASE_POOL_SHRINKER_INTERNAL_H__
#define __LNXBASE_POOL_SHRINKER_INTERNAL_H__

#include <limits.h>
#include <lib/dlist.h>
#include <liblinux/pal.h>
#include <libhmsync/raw_rwlock_guard.h>
#include <devhost/log.h>
#include <devhost/shrinker.h>

typedef unsigned long (*lnxbase_shrinker_query)(void *_shrinker);
typedef unsigned long (*lnxbase_shrinker_shrink)(void *_shrinker, int max_nr);

struct lnxbase_sub_shrinker {
	unsigned long (*query)(void *priv);
	unsigned long (*shrink)(void *priv, int free_nr);
	struct dlist_node dnode;
	void *priv;
};

struct lnxbase_shrinker {
	struct raw_rwlock rwlock;
	struct dlist_node sub_dnode;
	struct devhost_shrinker shrinker;
	lnxbase_shrinker_query query;
	lnxbase_shrinker_shrink shrink;
};

struct shrinkable_allocator {
	struct liblinux_page_allocator allocator;
	struct lnxbase_shrinker shrinker;
};

void lnxbase_shrinker_allocator_init(struct shrinkable_allocator *allocator,
				     struct liblinux_page_allocator *page_allocator,
				     lnxbase_shrinker_query query,
				     lnxbase_shrinker_shrink shrink);

void lnxbase_sub_shrinker_register(struct lnxbase_shrinker *shrinker,
				   struct lnxbase_sub_shrinker *sub_shrinker);

void lnxbase_sub_shrinker_unregister(struct lnxbase_shrinker *shrinker,
				   struct lnxbase_sub_shrinker *sub_shrinker);

void lnxbase_shrinker_register(struct lnxbase_sub_shrinker *sub_shrinker);

#define shrinker_to_allocator(__shrinker, type, allocator_member) ({ \
	struct shrinkable_allocator *__shrinker_allocator = container_of((__shrinker), \
						struct shrinkable_allocator, shrinker); \
	type *__allocator; __allocator = container_of(__shrinker_allocator, type, allocator_member); \
	__allocator; })

#define page_to_allocator(__page_allocator, type, allocator_member) ({ \
	struct shrinkable_allocator *__shrinker_allocator = container_of((__page_allocator), \
						struct shrinkable_allocator, allocator); \
	type *__allocator; __allocator = container_of(__shrinker_allocator, type, allocator_member); \
	__allocator; })

#endif /* __LNXBASE_POOL_SHRINKER_INTERNAL_H__ */
