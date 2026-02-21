/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: Udk shrinker frame
 * Author: Huawei OS Kernel Lab
 * Create: Mon Apr 29 16:30:06 2024
 */
#include <lib/dlist.h>

#include <udk/mm.h>
#include <udk/log.h>
#include <udk/iolib.h>
#include <devhost/log.h>
#include <udk/shrinker.h>
#include <libhwsecurec/securec.h>
#include <libhmsync/raw_rwlock_guard.h>

struct udk_shrinker_list {
	struct raw_rwlock rwlock;
	struct dlist_node head;
};

struct udk_dal_shrinker {
	struct dlist_node dnode;
	struct udk_shrinker *udk_shrinker;
};

static struct udk_shrinker_list g_shrinker_list = {
	.rwlock = RAW_RWLOCK_INITIALIZER(g_shrinker_list.rwlock),
	.head = DLIST_HEAD_INIT(g_shrinker_list.head),
};

/* max_nr equal SHRINK_SIZE_ALL */
static int __udk_shrinker_shrink_all(int max_nr, int *ret_nr)
{
	int err = E_HM_OK;
	int dal_err = 0;
	struct udk_dal_shrinker *udk_dal_shrinker = NULL;
	struct udk_shrinker *udk_shrinker = NULL;
	struct udk_shrink_control udk_shrinkctl = {
		.gfp_mask = GFP_KERNEL,
		.nr_to_scan = max_nr,
	};

	if (raw_rwlock_tryrdlock(&g_shrinker_list.rwlock) != E_HM_OK) {
		err = E_HM_BUSY;
	}

	if (err == E_HM_OK) {
		dlist_for_each_entry(udk_dal_shrinker, &g_shrinker_list.head,
				     struct udk_dal_shrinker, dnode) {
			udk_shrinker = udk_dal_shrinker->udk_shrinker;
			if (udk_shrinker != NULL) {
				dal_err = udk_shrinker->scan_objects(udk_shrinker,
								     &udk_shrinkctl);
				if (dal_err != SHRINK_STOP) {
					*ret_nr += dal_err;
				}
			}
		}
	}

	if (err != E_HM_BUSY) {
		raw_rwlock_unlock(&g_shrinker_list.rwlock);
	}

	return err;
}

/* max_nr is not equal to SHRINK_SIZE_ALL */
static int __udk_shrinker_shrink_nr(int max_nr, int *ret_nr)
{
	int err = E_HM_OK;
	int dal_err = 0;
	struct udk_dal_shrinker *udk_dal_shrinker = NULL;
	struct udk_shrinker *udk_shrinker = NULL;
	struct udk_shrink_control udk_shrinkctl = {
		.gfp_mask = GFP_KERNEL,
		.nr_to_scan = max_nr,
	};

	if (raw_rwlock_tryrdlock(&g_shrinker_list.rwlock) != E_HM_OK) {
		err = E_HM_BUSY;
	}

	if (err == E_HM_OK) {
		dlist_for_each_entry(udk_dal_shrinker, &g_shrinker_list.head,
				     struct udk_dal_shrinker, dnode) {
			udk_shrinker = udk_dal_shrinker->udk_shrinker;
			if (udk_shrinker == NULL) {
				continue;
			}
			dal_err = udk_shrinker->scan_objects(udk_shrinker, &udk_shrinkctl);
			if (dal_err != SHRINK_STOP) {
				*ret_nr += dal_err;
				if (*ret_nr > max_nr) {
					break;
				}
				if (udk_shrinkctl.nr_to_scan > dal_err) {
					udk_shrinkctl.nr_to_scan -= dal_err;
				}
			}
		}
	}

	if (err != E_HM_BUSY) {
		raw_rwlock_unlock(&g_shrinker_list.rwlock);
	}

	return err;
}

int udk_shrinker_shrink(int max_nr, int *ret_nr)
{
	int err = E_HM_OK;

	if (max_nr < SHRINK_SIZE_ALL || ret_nr == NULL) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		if (max_nr == SHRINK_SIZE_ALL) {
			err = __udk_shrinker_shrink_all(max_nr, ret_nr);
		} else {
			err = __udk_shrinker_shrink_nr(max_nr, ret_nr);
		}
	}

	return err;
}

int udk_shrinker_query(int flag)
{
	int err = E_HM_OK;
	int dal_err = 0;
	struct udk_dal_shrinker *udk_dal_shrinker = NULL;
	struct udk_shrinker *udk_shrinker = NULL;
	int pages_cnt = 0;
	struct udk_shrink_control udk_shrinkctl = {
		.gfp_mask = GFP_KERNEL,
		.nr_to_scan = 0,
	};

	if (raw_rwlock_tryrdlock(&g_shrinker_list.rwlock) != E_HM_OK) {
		err = E_HM_BUSY;
	}

	if (err == E_HM_OK) {
		dlist_for_each_entry(udk_dal_shrinker, &g_shrinker_list.head,
				     struct udk_dal_shrinker, dnode) {
			udk_shrinker = udk_dal_shrinker->udk_shrinker;
			if (udk_shrinker != NULL) {
				dal_err = udk_shrinker->count_objects(udk_shrinker,
								      &udk_shrinkctl);
				if (dal_err != SHRINK_EMPTY) {
					pages_cnt += dal_err;
				}
			}
		}
	}

	if (err != E_HM_BUSY) {
		raw_rwlock_unlock(&g_shrinker_list.rwlock);
	}

	return (err < 0) ? 0 : pages_cnt;
}

int udk_register_shrinker(struct udk_shrinker *udk_shrinker)
{
	int err = E_HM_OK;
	struct udk_dal_shrinker *udk_dal_shrinker = NULL;

	if (udk_shrinker == NULL) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		udk_dal_shrinker = (struct udk_dal_shrinker *)udk_malloc(sizeof(*udk_dal_shrinker));
		if (udk_dal_shrinker == NULL) {
			err = E_HM_NOMEM;
		}
	}

	if (err == E_HM_OK) {
		udk_dal_shrinker->udk_shrinker = udk_shrinker;
		RAW_RWLOCK_WR_GUARD(_, &g_shrinker_list.rwlock);
		dlist_insert_tail(&g_shrinker_list.head, &udk_dal_shrinker->dnode);
	}

	return err;
}
