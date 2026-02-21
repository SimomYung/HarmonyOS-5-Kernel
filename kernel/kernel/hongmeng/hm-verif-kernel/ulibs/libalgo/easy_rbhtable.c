/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Rbtree-based eash hash table implementation
 * Author: Huawei OS Kernel Lab
 * Create: Mon Dec 3 19:29:37 2018
 */
#include <hongmeng/errno.h>
#include <hongmeng/panic.h>
#include <libalgo/rb_htable.h>
#include <libhmsync/raw_mutex_guard.h>
#include <libhmsync/raw_rwlock_guard.h>

#define extr_container(p, offset)	((void *)(((char *)(p)) - (offset)))
#define extr_vptr(c, offset)		((void *)(((char *)(c)) + (offset)))
#define extr_val(c, type, offset)	(*(type *)(extr_vptr(c, offset)))
#define extr_ptr(c, type, offset)	((type)(extr_vptr(c, offset)))
#define extr_dstr(c, type, offset)      ((type)(*(unsigned long *)(extr_vptr(c, offset))))
#define cmp_int(x, y)			(((x) > (y)) ? 1 : (((x) < (y)) ? -1 : 0))
#define cmp_str(x, y)			strcmp(x, y)

#define easy_htable_lookup_DEFINE(tag, ktype, extr_key, cmp_key)		\
void *easy_htable_lookup_##tag(struct easy_htable *htable, ktype key,		\
			       void (*const call_back)(void *))			\
{										\
	int cmp;								\
	ktype cur_key = (ktype)0;						\
	void *cur_data = NULL;							\
	struct rbt_hnode *cur = NULL;						\
	struct rbt_bucket *bucket = NULL;					\
	bool found = false;							\
										\
	if ((htable != NULL) && (htable->ht.table != NULL)) {			\
		bucket = rbt_htable_bucket(&htable->ht,				\
				rbt_htable_hash_##tag(&htable->ht, key));	\
										\
		RAW_RWLOCK_RD_GUARD(_, &bucket->rwlock);				\
		cur = bucket->tree.root;					\
		while (cur != NULL) {						\
			cur_data = extr_container(cur, htable->node_off);	\
			cur_key = extr_key(cur_data, ktype, htable->key_off);	\
			cmp = cmp_key(cur_key, key);				\
			if (cmp > 0) {						\
				cur = cur->rbt_left;				\
			} else if (cmp < 0) {					\
				cur = cur->rbt_right;				\
			} else {						\
				found = true;					\
				break;						\
			}							\
		}								\
		if (found && (call_back != NULL)) {				\
			call_back(cur_data);					\
		}								\
	}									\
										\
	return found ? cur_data : NULL;						\
}

#define easy_htable_insert_DEFINE(tag, ktype, extr_key, cmp_key)		\
static void *easy_htable_lookup_nolock_##tag(const struct easy_htable *htable,	\
					     ktype key,				\
					     struct rbt_bucket *bucket,		\
					     struct rbt_hnode **parent,		\
					     bool *is_right)			\
{										\
	int cmp;								\
	ktype cur_key;								\
	void *cur_data = NULL;							\
	struct rbt_hnode *cur = NULL;						\
	bool found = false;							\
										\
	cur = bucket->tree.root;						\
	while (cur != NULL) {							\
		cur_data = extr_container(cur, htable->node_off);		\
		cur_key = extr_key(cur_data, ktype, htable->key_off);		\
		cmp = cmp_key(cur_key, key);					\
		if (cmp > 0) {							\
			if (cur->rbt_left == NULL) {				\
				*parent = cur;					\
				*is_right = false;				\
				break;						\
			}							\
			cur = cur->rbt_left;					\
		} else if (cmp < 0) {						\
			if (cur->rbt_right == NULL) {				\
				*parent = cur;					\
				*is_right = true;				\
				break;						\
			}							\
			cur = cur->rbt_right;					\
		} else {							\
			found = true;						\
			break;							\
		}								\
	}									\
										\
	return found ? cur_data : NULL;						\
}										\
										\
int easy_htable_insert_##tag(struct easy_htable *htable,			\
			     ktype key, struct rbt_hnode *node)			\
{										\
	void *cur_data = NULL;							\
	struct rbt_hnode *parent = NULL;					\
	bool is_right = false;							\
	struct rbt_bucket *bucket = NULL;					\
	int rc = E_HM_OK;							\
										\
	if ((htable == NULL) || (htable->ht.table == NULL) || (node == NULL)) {	\
		rc = E_HM_INVAL;						\
	}									\
	if (rc == E_HM_OK) {							\
		bucket = rbt_htable_bucket(&htable->ht,				\
				rbt_htable_hash_##tag(&htable->ht, key));	\
										\
		RAW_RWLOCK_WR_GUARD(_, &bucket->rwlock);				\
		cur_data = easy_htable_lookup_nolock_##tag(htable, key, bucket,	\
							   &parent, &is_right);	\
		if (cur_data == NULL) {						\
			rbt_insert(&bucket->tree, node, parent, is_right);	\
		} else	{							\
			rc = E_HM_OBJEXIST;					\
		}								\
	}									\
										\
	return rc;								\
}

#define easy_htable_remove_DEFINE(tag, ktype)					\
int easy_htable_remove_##tag(struct easy_htable *htable, ktype key,		\
			     struct rbt_hnode *node)				\
{										\
	struct rbt_bucket *bucket = NULL;					\
	int rc = E_HM_OK;							\
										\
	if ((htable == NULL) || (htable->ht.table == NULL) || (node == NULL)) {	\
		rc = E_HM_INVAL;						\
	}									\
	if (rc == E_HM_OK) {							\
		bucket = rbt_htable_bucket(&htable->ht,				\
				rbt_htable_hash_##tag(&htable->ht, key));	\
		RAW_RWLOCK_WR_GUARD(_, &bucket->rwlock);				\
		rbt_remove(&bucket->tree, node);				\
	}									\
										\
	return rc;								\
}

int easy_htable_init(struct easy_htable *htable, void *(*alloc)(size_t sz),
		     uint32_t size, size_t max, size_t key_off, size_t node_off)
{
	int rc = E_HM_OK;

	if ((htable == NULL) || (alloc == NULL)) {
		rc = E_HM_INVAL;
	}
	if (rc == E_HM_OK) {
		rc = rbt_htable_init(&htable->ht, alloc, size, max);
		if (rc == E_HM_OK) {
			htable->node_off = node_off;
			htable->key_off = key_off;
		}
	}

	return rc;
}

void easy_htable_destroy(struct easy_htable *htable, void (*free)(void *table))
{
	int rc;

	if ((htable != NULL) && (free != NULL)) {
		rc = rbt_htable_destroy(&htable->ht, free);
		if (rc != E_HM_OK) {
			hm_warn("rbt_htable_destroy failed: %s\n", hmstrerror(rc));
		}
	}
}

easy_htable_lookup_DEFINE(u32, uint32_t, extr_val, cmp_int);
easy_htable_insert_DEFINE(u32, uint32_t, extr_val, cmp_int);
easy_htable_remove_DEFINE(u32, uint32_t);
easy_htable_lookup_DEFINE(u64, uint64_t, extr_val, cmp_int);
easy_htable_insert_DEFINE(u64, uint64_t, extr_val, cmp_int);
easy_htable_remove_DEFINE(u64, uint64_t);
easy_htable_lookup_DEFINE(str, const char *, extr_ptr, cmp_str);
easy_htable_insert_DEFINE(str, const char *, extr_ptr, cmp_str);
easy_htable_remove_DEFINE(str, const char *);
easy_htable_lookup_DEFINE(dstr, const char *, extr_dstr, cmp_str);
easy_htable_insert_DEFINE(dstr, const char *, extr_dstr, cmp_str);
easy_htable_remove_DEFINE(dstr, const char *);
