/*
* Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
* Description: devhost shrinker support
* Author: Huawei OS Kernel Lab
* Create: Fri Mar 31 10:31:07 2023
*/
#include <limits.h>
#include <devhost_api.h>
#include <hongmeng/types.h>
#include <libhwsecurec/securec.h>
#include <libhmsync/raw_rwlock_guard.h>

#include "types.h"
#include "devhost.h"
#include "devhost_shrinker.h"
#include <devhost/log.h>
#include <devhost/shrinker.h>

struct devhost_shrinker_group {
	struct raw_rwlock rwlock;
	struct dlist_node head;
};

static struct devhost_shrinker_group shrinker_group = {
	.rwlock = RAW_RWLOCK_INITIALIZER(shrinker_group.rwlock),
	.head = DLIST_HEAD_INIT(shrinker_group.head),
};

PUBLIC_SYMBOL
void devhost_shrinker_register(struct devhost_shrinker *shrinker)
{
	BUG_ON(shrinker == NULL);
	BUG_ON(shrinker->shrink == NULL);
	RAW_RWLOCK_WR_GUARD(_, &shrinker_group.rwlock);
	dlist_insert_tail(&shrinker_group.head, &shrinker->dnode);
}

static unsigned long _do_shrink_in_order(enum devhost_shrinker_mode type, int req_pages)
{
	struct devhost_shrinker *shrinker_node;
	unsigned long cnt = 0;
	unsigned long cnt_sum = 0;

	if (raw_rwlock_tryrdlock(&shrinker_group.rwlock) != E_HM_OK) {
		return cnt_sum;
	}
	dlist_for_each_entry(shrinker_node, &shrinker_group.head, struct devhost_shrinker, dnode) {
		if (req_pages == 0) {
			break;
		}
		cnt = shrinker_node->shrink(shrinker_node, type, req_pages);
		cnt_sum += cnt;
		if (req_pages == SHRINK_SIZE_ALL) {
			continue;
		}
		req_pages = ((unsigned long)(long)req_pages > cnt) ? (req_pages - (int)(signed long)cnt) : 0;
	}
	raw_rwlock_unlock(&shrinker_group.rwlock);

	return cnt_sum;
}

/* Obtain the required in proportion, accurate to three decimal places by multipled 1000 */
#define get_req_nr_by_ratio(req, each, all) 	(((req) * ((each) * 1000 / (all)) + (1000 - 1)) / 1000)
static unsigned long _do_shrink_in_balance(enum devhost_shrinker_mode type, int req_pages)
{
	struct devhost_shrinker *shrinker_node;
	unsigned long cnt = 0;
	unsigned long cnt_sum = 0;
	int query_all_nr = 0;
	int query_each_nr = 0;
	int req_each_nr = 0;

	query_all_nr = devhost_shrinker_query(type);
	if (query_all_nr == 0) {
		return 0;
	}
	if (req_pages == SHRINK_SIZE_ALL || req_pages > query_all_nr) {
		return _do_shrink_in_order(type, req_pages);
	}

	if (raw_rwlock_tryrdlock(&shrinker_group.rwlock) != E_HM_OK) {
		return cnt_sum;
	}
	dlist_for_each_entry(shrinker_node, &shrinker_group.head, struct devhost_shrinker, dnode) {
		query_each_nr = shrinker_node->query(shrinker_node, type);
		if (query_each_nr == 0) {
			continue;
		}
		req_each_nr = get_req_nr_by_ratio(req_pages, query_each_nr, query_all_nr);
		cnt = shrinker_node->shrink(shrinker_node, type, req_each_nr);
		cnt_sum += cnt;
		if (cnt_sum >= (unsigned long)req_pages) {
			break;
		}
	}
	raw_rwlock_unlock(&shrinker_group.rwlock);

	return cnt_sum;
}

static int _return_verified_value(unsigned long shrinked_pages, int max_nr)
{
	if (shrinked_pages > INT_MAX) {
		/*
		 * Overflow may happen if we return cnt or INT_MAX.
		 *
		 * When max_nr is SHRINK_SIZE_ALL, we return 0, while numbers returned
		 * has no effect to this shrink action.
		 * Or We return except numbers, to finish this shrink.
		 */
		dh_info("devhost shrink too much memory, %lu pages\n", shrinked_pages);
		shrinked_pages = (unsigned long)(long)(max_nr == SHRINK_SIZE_ALL ? 0 : max_nr);
	}

	return (int)shrinked_pages;
}

PUBLIC_SYMBOL
int devhost_shrinker_do_shrink(enum devhost_shrinker_mode type, int max_nr)
{
	unsigned long shrinked_pages = 0;

	switch (type) {
	case DEVHOST_SHRINKER_FAST:
		shrinked_pages = _do_shrink_in_balance(type, max_nr);
		break;
	case DEVHOST_SHRINKER_SLOW:
		shrinked_pages = _do_shrink_in_order(type, max_nr);
		break;
	case MAX_SHRINKER_MODE:
		shrinked_pages = _do_shrink_in_balance(DEVHOST_SHRINKER_FAST, max_nr);
		if (max_nr == SHRINK_SIZE_ALL) {
			/* shrink all slow memory */
			shrinked_pages += _do_shrink_in_order(DEVHOST_SHRINKER_SLOW, max_nr);
		} else if (shrinked_pages < (unsigned long)max_nr) {
			shrinked_pages += _do_shrink_in_order(DEVHOST_SHRINKER_SLOW, (max_nr - (int)shrinked_pages));
		}
		break;
	default:
		break;
	}

	return _return_verified_value(shrinked_pages, max_nr);
}

PUBLIC_SYMBOL
int devhost_shrinker_shrink(int max_nr)
{
	if (max_nr < SHRINK_SIZE_ALL) {
		dh_error("devhost shrink failed, max_nr=%d\n", max_nr);
		return E_HM_INVAL;
	}

	return devhost_shrinker_do_shrink(MAX_SHRINKER_MODE, max_nr);
}

PUBLIC_SYMBOL
int devhost_shrinker_query(int type)
{
	struct devhost_shrinker *shrinker_node;
	unsigned long pages_cnt = 0;

	if (raw_rwlock_tryrdlock(&shrinker_group.rwlock) != E_HM_OK) {
		return pages_cnt;
	}
	dlist_for_each_entry(shrinker_node, &shrinker_group.head, struct devhost_shrinker, dnode) {
		if (shrinker_node->query != NULL) {
			pages_cnt += shrinker_node->query(shrinker_node, type);
		}
	}
	raw_rwlock_unlock(&shrinker_group.rwlock);

	return (int)pages_cnt;
}
