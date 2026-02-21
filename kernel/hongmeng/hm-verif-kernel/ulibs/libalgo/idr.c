/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Fri Feb 21 17:11:25 2020
 */
#include <libalgo/idr.h>
#include <hongmeng/errno.h>
#include <libhwsecurec/securec.h>
#include <libhmsync/raw_rwlock_guard.h>

#define index_to_id(x) (int)(unsigned int)(x)
#define id_to_index(x) (unsigned long)(unsigned int)(x)

void idr_init(struct idr *idp)
{
	struct raw_rwlock_attr attr;

	mem_zero_s(attr);
	if (idp != NULL) {
		idp->next = IDR_MIN_ID;
		raw_rwlock_init(&idp->lock, &attr);
		radix_tree_init(&idp->root);
	}
}

void idr_free(struct idr *idp)
{
	if (idp != NULL) {
		RAW_RWLOCK_WR_GUARD(_, &idp->lock);
		idp->next = IDR_MIN_ID;
		radix_tree_destroy(&idp->root);
	}
}

void idr_free_nolock(struct idr *idp)
{
	if (idp != NULL) {
		idp->next = IDR_MIN_ID;
		radix_tree_destroy(&idp->root);
	}
}

static int idr_alloc_within_interval(struct idr *idr, void *item, int start, int end)
{
	unsigned long index = id_to_index(start);
	int rc;

	RAW_RWLOCK_WR_GUARD(_, &idr->lock);

	rc = radix_tree_alloc_slot(&idr->root, &index, id_to_index(end), item);
	if (rc >= 0) {
		rc = index_to_id(index);
	}
	return rc;
}

int idr_alloc(struct idr *idr, void *item)
{
	int rc;

	if (idr == NULL || item == NULL) {
		rc = E_HM_INVAL;
	} else {
		rc = idr_alloc_within_interval(idr, item, IDR_MIN_ID, IDR_MAX_ID);
	}
	return rc;
}

/*
 * idr_alloc_cyclic() - Allocate an ID cyclically.
 *
 * Alloc an unused ID within range [start, end].
 * "cyclic" means the search for new id will start at the last allocated ID,
 * and if none unused id found, the search will start at @start.
 *
 * Return: nonnegative - the new ID associated with @item.
 *         negative - hmerrno no failure.
 */
int idr_alloc_cyclic(struct idr *idr, void *item, int start, int end)
{
	int rc = E_HM_OK;

	if (idr == NULL || item == NULL || start < 0 || end < 0) {
		rc = E_HM_INVAL;
	}
	if (rc == E_HM_OK) {
		if (start >= idr->next) {
			rc = idr_alloc_within_interval(idr, item, start, end);
		} else {
			rc = idr_alloc_within_interval(idr, item, idr->next, end);
			if (rc == E_HM_NOSPC) {
				rc = idr_alloc_within_interval(idr, item, start, end);
			}
		}
	}
	if (rc >= 0) {
		if (rc == IDR_MAX_ID) {
			idr->next = 0;
		} else {
			idr->next = rc + 1;
		}
	}
	return rc;
}

void *idr_remove(struct idr *idp, unsigned int id)
{
	void *item = NULL;

	if (idp != NULL) {
		RAW_RWLOCK_WR_GUARD(_, &idp->lock);
		item = radix_tree_delete_item(&idp->root, id);
	}

	return item;
}

void *idr_remove_nolock(struct idr *idp, unsigned int id)
{
	void *item = NULL;

	if (idp != NULL) {
		item = radix_tree_delete_item(&idp->root, id);
	}

	return item;
}

void *idr_find(struct idr *idp, unsigned int id)
{
	void *item = NULL;

	if (idp != NULL) {
		RAW_RWLOCK_RD_GUARD(_, &idp->lock);
		item = radix_tree_find_item(&idp->root, id);
	}
	return item;
}

void *idr_find_nolock(struct idr *idp, unsigned int id)
{
	void *item = NULL;

	if (idp != NULL) {
		item = radix_tree_find_item(&idp->root, id);
	}
	return item;
}

int idr_set(struct idr *idp, void *item, unsigned int id)
{
	int rc;

	if (idp == NULL || item == NULL) {
		rc = E_HM_INVAL;
	} else {
		RAW_RWLOCK_WR_GUARD(_, &idp->lock);
		rc = radix_tree_insert_item(&idp->root, id, item);
	}
	return rc;
}

int idr_set_nolock(struct idr *idp, void *item, unsigned int id)
{
	int rc;

	if (idp == NULL || item == NULL) {
		rc = E_HM_INVAL;
	} else {
		rc = radix_tree_insert_item(&idp->root, id, item);
	}
	return rc;
}

bool idr_is_empty(struct idr *idp)
{
	BUG_ON(idp == NULL);

	RAW_RWLOCK_RD_GUARD(_, &idp->lock);
	return radix_tree_is_empty(&idp->root);
}

int idr_last_id_of(struct idr *idp)
{
	int ret;
	BUG_ON(idp == NULL);

	RAW_RWLOCK_RD_GUARD(_, &idp->lock);
	ret = idp->next;
	return (ret == 0) ? IDR_MAX_ID : (ret - 1);
}

int idr_set_next_id(struct idr *idp, int next)
{
	int err = E_HM_INVAL;

	BUG_ON(idp == NULL);

	if (next >= 0) {
		RAW_RWLOCK_WR_GUARD(_, &idp->lock);
		idp->next = next;
		err = E_HM_OK;
	}
	return err;
}

/*
 * idr_get_next use to search next id which bigger than @*nextidp and equal it,
 * return the pointer which next id assosiate with.
 */
void *idr_get_next(struct idr *idp, unsigned int *nextidp)
{
	void *ret = NULL;
	if (idp != NULL && nextidp != NULL) {
		RAW_RWLOCK_RD_GUARD(_, &idp->lock);

		unsigned long index = (unsigned long)(*nextidp);
		ret = radix_tree_get_next(&idp->root, &index);
		*nextidp = (unsigned int)index;
	}
	return ret;
}

void *idr_replace(struct idr *idp, void *item, unsigned int id)
{
	void *ret_item = NULL;

	if (idp != NULL && item != NULL) {
		RAW_RWLOCK_WR_GUARD(_, &idp->lock);
		ret_item = radix_tree_replace_item(&idp->root, item, id);
	}

	return ret_item;
}

int idr_for_each(struct idr *idp,
		 int (*callback)(unsigned int id, void *ptr, void *data), void *data)
{
	int rc = E_HM_OK;

	if (idp == NULL) {
		rc = E_HM_INVAL;
	}

	if (rc == E_HM_OK) {
		RAW_RWLOCK_RD_GUARD(_, &idp->lock);
		unsigned long index = 0;
		void *item = radix_tree_get_next(&idp->root, &index);

		while (item != NULL) {
			unsigned int id = (unsigned int)index;
			rc = callback(id, item, data);
			if (rc != E_HM_OK) {
				break;
			}
			index++;
			item = radix_tree_get_next(&idp->root, &index);
		}
	}
	return rc;
}

/* To use this function, idr must be initialized with recursive mutex */
void *idr_find_ex(struct idr *idp, unsigned int id, bool ignore_invalid_flag,
		  int (*hook)(void *ptr, bool ignore_invalid_flag))
{
	int err;
	void *ret = NULL;

	RAW_RWLOCK_RD_GUARD(_, &idp->lock);
	ret = idr_find(idp, id);
	if (ret != NULL) {
		err = hook(ret, ignore_invalid_flag);
		if (err != E_HM_OK) {
			ret = NULL;
		}
	}
	return ret;
}
