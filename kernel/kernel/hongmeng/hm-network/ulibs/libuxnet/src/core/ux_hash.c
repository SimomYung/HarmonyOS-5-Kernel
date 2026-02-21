/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Hash function and Operations of lists for network unix
 * Author: Huawei OS Kernel Lab
 * Create: Mon Dec 25 17:21:00 2023
 */

#include <hongmeng/errno.h>
#include <hongmeng/panic.h>
#include <libalgo/rb_htable.h>

#include "ux_hash.h"

#define extr_container(p, offset) ((void *)(((char *)(p)) - (offset)))

struct htable_fn_t {
	cmp_key_fn cmp_key;
	calc_hval_fn calc_hval;
};

static struct htable_fn_t htable_fn[H_MAX];

void ux_htable_register(enum hash_type type, cmp_key_fn cmp_key, calc_hval_fn calc_hval)
{
	BUG_ON(type >= H_MAX);
	htable_fn[type].cmp_key = cmp_key;
	htable_fn[type].calc_hval = calc_hval;
}

static unsigned int __calc_hval(const struct rbt_htable *htable, enum hash_type type, void *key)
{
	unsigned int hash = htable_fn[type].calc_hval(key);
	return rbt_htable_hash_u32(htable, hash);
}

int ux_htable_remove(struct easy_htable *htable, enum hash_type type, void *key,
			  struct rbt_hnode *node)
{
	struct rbt_bucket *bucket = NULL;
	int rc = E_HM_INVAL;

	if ((htable != NULL) && (htable->ht.table != NULL) && (node != NULL)) {
		bucket = rbt_htable_bucket(&htable->ht, __calc_hval(&htable->ht, type, key));
		rbt_remove(&bucket->tree, node);
		rc = E_HM_OK;
	}

	return rc;
}

void *ux_htable_lookup(struct easy_htable *htable, enum hash_type type, void *key)
{
	long cmp;
	void *cur_data = NULL;
	struct rbt_hnode *cur = NULL;
	struct rbt_bucket *bucket = NULL;
	bool found = false;

	if ((htable != NULL) && (htable->ht.table != NULL)) {
		bucket = rbt_htable_bucket(&htable->ht, __calc_hval(&htable->ht, type, key));
		cur = bucket->tree.root;
		while (cur != NULL) {
			cur_data = extr_container(cur, htable->node_off);
			cmp = htable_fn[type].cmp_key(cur_data, key);
			if (cmp > 0) {
				cur = cur->rbt_left;
			} else if (cmp < 0) {
				cur = cur->rbt_right;
			} else {
				found = true;
				break;
			}
		}
	}

	return found ? cur_data : NULL;
}

static int __htable_insert_rbt(const struct easy_htable *htable, enum hash_type type, void *key,
				struct rbt_bucket *bucket, struct rbt_hnode *node)
{
	long cmp;
	void *cur_data = NULL;
	struct rbt_hnode *cur = NULL;

	cur = bucket->tree.root;
	while (cur != NULL) {
		cur_data = extr_container(cur, htable->node_off);
		cmp = htable_fn[type].cmp_key(cur_data, key);
		if (cmp > 0) {
			if (cur->rbt_left == NULL) {
				rbt_insert(&bucket->tree, node, cur, false);
				return E_HM_OK;
			}
			cur = cur->rbt_left;
		} else if (cmp < 0) {
			if (cur->rbt_right == NULL) {
				rbt_insert(&bucket->tree, node, cur, true);
				return E_HM_OK;
			}
			cur = cur->rbt_right;
		} else {
			/* The node already exists in the rbt. */
			return E_HM_EXIST;
		}
	}

	/* The rbt does not have any node */
	rbt_insert(&bucket->tree, node, NULL, false);
	return E_HM_OK;
}

int ux_htable_insert(struct easy_htable *htable, enum hash_type type, void *key, struct rbt_hnode *node)
{
	int ret = E_HM_INVAL;
	struct rbt_bucket *bucket = NULL;

	if ((htable != NULL) && (htable->ht.table != NULL) && (node != NULL)) {
		bucket = rbt_htable_bucket(&htable->ht, __calc_hval(&htable->ht, type, key));
		ret = __htable_insert_rbt(htable, type, key, bucket, node);
	}

	return ret;
}
