/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: lnxbase shrinker internal APIs
 * Author: Huawei OS Kernel Lab
 * Create: Tue Apr 11 20:23:47 2023
 */

#include "shrinker.h"

#include <limits.h>
#include <libmem/utils.h>
#include <libhwsecurec/securec.h>
#include <libstrict/strict.h>
#include <hmasm/page.h>
#include <hongmeng/panic.h>
#include <devhost/log.h>
#include <lnxbase/lnxbase.h>

void lnxbase_sub_shrinker_register(struct lnxbase_shrinker *shrinker,
				   struct lnxbase_sub_shrinker *sub_shrinker)
{
	BUG_ON(sub_shrinker == NULL);
	BUG_ON(shrinker == NULL);
	BUG_ON(sub_shrinker->shrink == NULL);
	RAW_RWLOCK_WR_GUARD(_, &shrinker->rwlock);
	dlist_insert_tail(&shrinker->sub_dnode, &sub_shrinker->dnode);
}

void lnxbase_sub_shrinker_unregister(struct lnxbase_shrinker *shrinker,
				   struct lnxbase_sub_shrinker *sub_shrinker)
{
	struct lnxbase_sub_shrinker *sub_shrinker_node;
	struct lnxbase_sub_shrinker *tmp;
	bool found = false;

	BUG_ON(sub_shrinker == NULL);
	BUG_ON(shrinker == NULL);
	if (sub_shrinker->priv == NULL) {
		dh_error("lnxbase unregister sub_shrinker failed with NULL shrinker\n");
		return;
	}

	RAW_RWLOCK_WR_GUARD_START(_, &shrinker->rwlock);
	dlist_for_each_entry_safe(sub_shrinker_node, tmp, &shrinker->sub_dnode,
				  struct lnxbase_sub_shrinker, dnode) {
		if (sub_shrinker_node->priv == sub_shrinker->priv) {
			found = true;
			dlist_delete(&sub_shrinker_node->dnode);
			break;
		}
	}
	RAW_RWLOCK_WR_GUARD_END(_, &shrinker->rwlock);

	if (found) {
		free(sub_shrinker_node);
	} else {
		dh_error("lnxbase unregister sub_shrinker failed with Non-exist shrinker\n");
	}
}

static unsigned long lnxbase_sub_query(struct lnxbase_shrinker *shrinker)
{
	struct lnxbase_sub_shrinker *shrinker_node;
	unsigned long free_cnt = 0;

	if (raw_rwlock_tryrdlock(&shrinker->rwlock) != E_HM_OK) {
		return free_cnt;
	}
	dlist_for_each_entry(shrinker_node, &shrinker->sub_dnode, struct lnxbase_sub_shrinker, dnode) {
		if (shrinker_node->query != NULL) {
			free_cnt += shrinker_node->query(shrinker_node->priv);
		}
	}
	raw_rwlock_unlock(&shrinker->rwlock);

	return free_cnt;
}

static unsigned long lnxbase_sub_shrink(struct lnxbase_shrinker *shrinker, int max_nr)
{
	struct lnxbase_sub_shrinker *shrinker_node;
	unsigned long cnt;
	unsigned long freed_cnt = 0;
	int free_nr = max_nr;

	BUG_ON(shrinker == NULL);
	if (max_nr < SHRINK_SIZE_ALL) {
		dh_error("lnxbase sub shrink failed, max_nr=%d\n", max_nr);
		return 0;
	}

	if (raw_rwlock_tryrdlock(&shrinker->rwlock) != E_HM_OK) {
		return freed_cnt;
	}
	dlist_for_each_entry(shrinker_node, &shrinker->sub_dnode, struct lnxbase_sub_shrinker, dnode) {
		if (free_nr == 0) {
			break;
		}
		cnt = shrinker_node->shrink(shrinker_node->priv, free_nr);
		freed_cnt += cnt;
		if (max_nr == SHRINK_SIZE_ALL) {
			continue;
		}
		free_nr = ((unsigned long)(long)free_nr > cnt) ? (free_nr - (int)(signed long)cnt) : 0;
	}
	raw_rwlock_unlock(&shrinker->rwlock);

	return freed_cnt;
}

static unsigned long lnxbase_query(struct devhost_shrinker *_shrinker, enum devhost_shrinker_mode type)
{
	struct lnxbase_shrinker *shrinker;
	unsigned long free_cnt = 0;

	BUG_ON(_shrinker == NULL);
	shrinker = container_of(_shrinker, struct lnxbase_shrinker, shrinker);

	if (shrinker->query == NULL) {
		return 0;
	}

	switch (type) {
	case DEVHOST_SHRINKER_FAST:
		free_cnt += shrinker->query(shrinker);
		break;
	case DEVHOST_SHRINKER_SLOW:
		free_cnt += lnxbase_sub_query(shrinker);
		break;
	case MAX_SHRINKER_MODE:
	default:
		free_cnt += shrinker->query(shrinker);
		free_cnt += lnxbase_sub_query(shrinker);
		break;
	}

	return free_cnt;
}

static unsigned long lnxbase_shrink(struct devhost_shrinker *_shrinker, enum devhost_shrinker_mode type, int max_nr)
{
	struct lnxbase_shrinker *shrinker;
	unsigned long freed_size = 0;
	unsigned long free_page_cnt = 0;

	BUG_ON(_shrinker == NULL);

	if (max_nr < SHRINK_SIZE_ALL) {
		dh_error("lnxbase shrink failed, max_nr=%d\n", max_nr);
		return freed_size;
	}

	shrinker = container_of(_shrinker, struct lnxbase_shrinker, shrinker);
	if (shrinker->shrink == NULL) {
		return freed_size;
	}

	switch (type) {
	case DEVHOST_SHRINKER_FAST:
		freed_size = shrinker->shrink(shrinker, max_nr);
		break;
	case DEVHOST_SHRINKER_SLOW:
		/* the pages maybe allocated by others, so shrink more than max_nr */
		free_page_cnt = lnxbase_sub_shrink(shrinker, (max_nr << 1));
		if (free_page_cnt > 0) {
			freed_size = shrinker->shrink(shrinker, max_nr);
		}
		break;
	case MAX_SHRINKER_MODE:
	default:
		dh_error("shrinker type: %u is wrong\n", (unsigned int)type);
		break;
	}

	return (freed_size / PAGE_SIZE);
}

void lnxbase_shrinker_allocator_init(struct shrinkable_allocator *allocator,
				     struct liblinux_page_allocator *page_allocator,
				     lnxbase_shrinker_query query,
				     lnxbase_shrinker_shrink shrink)
{
	allocator->allocator = *page_allocator;
	allocator->shrinker.query = query;
	allocator->shrinker.shrink = shrink;
	dlist_init(&allocator->shrinker.sub_dnode);
	raw_rwlock_init(&allocator->shrinker.rwlock, NULL);
	allocator->shrinker.shrinker.query = lnxbase_query;
	allocator->shrinker.shrinker.shrink = lnxbase_shrink;
	dlist_init(&allocator->shrinker.shrinker.dnode);

	devhost_shrinker_register(&allocator->shrinker.shrinker);
}

static inline
struct lnxbase_sub_shrinker *shrinker_to_sub_shrinker(struct devhost_shrinker *shrinker)
{
	struct lnxbase_sub_shrinker *sub_shrinker = NULL;
	struct lnxbase_shrinker *lnx_shrinker = container_of(shrinker, struct lnxbase_shrinker, shrinker);

	sub_shrinker = dlist_first_entry(&lnx_shrinker->sub_dnode, struct lnxbase_sub_shrinker, dnode);
	return sub_shrinker;
}

static unsigned long fast_query(struct devhost_shrinker *_shrinker, enum devhost_shrinker_mode type)
{
	struct lnxbase_sub_shrinker *sub_shrinker;
	unsigned long free_cnt = 0;

	UNUSED(type);
	BUG_ON(_shrinker == NULL);

	sub_shrinker = shrinker_to_sub_shrinker(_shrinker);
	if (sub_shrinker != NULL && sub_shrinker->query != NULL) {
		free_cnt = sub_shrinker->query(sub_shrinker->priv);
	}

	return free_cnt;
}

static unsigned long fast_shrink(struct devhost_shrinker *_shrinker,
				 enum devhost_shrinker_mode type, int max_nr)
{
	struct lnxbase_sub_shrinker *sub_shrinker;
	unsigned long freed_cnt = 0;

	UNUSED(type);
	BUG_ON(_shrinker == NULL);

	sub_shrinker = shrinker_to_sub_shrinker(_shrinker);
	if (sub_shrinker != NULL && sub_shrinker->shrink != NULL) {
		freed_cnt = sub_shrinker->shrink(sub_shrinker->priv, max_nr);
	}

	return freed_cnt;
}

void lnxbase_shrinker_register(struct lnxbase_sub_shrinker *sub_shrinker)
{
	struct lnxbase_shrinker *shrinker;

	shrinker = (struct lnxbase_shrinker*)malloc(sizeof(struct lnxbase_shrinker));
	if (shrinker == NULL) {
		dh_error("malloc lnxbase shrinker failed\n");
		return;
	}
	mem_zero_s(*shrinker);

	shrinker->shrinker.query = fast_query;
	shrinker->shrinker.shrink = fast_shrink;
	dlist_init(&shrinker->sub_dnode);
	raw_rwlock_init(&shrinker->rwlock, NULL);
	lnxbase_sub_shrinker_register(shrinker, sub_shrinker);
	devhost_shrinker_register(&shrinker->shrinker);
}
