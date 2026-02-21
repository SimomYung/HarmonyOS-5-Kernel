/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Hash table backed by red-black tree
 * Author: Huawei OS Kernel Lab
 * Create: Mon Dec 3 19:29:37 2018
 */
#ifndef ULIBS_LIBHTABLE_RB_HTABLE_H
#define ULIBS_LIBHTABLE_RB_HTABLE_H

#include <string.h>
#include <libalgo/rbtree.h>
#include <libhmsync/atomic.h>
#include <libhmsync/raw_mutex.h>
#include <libhmsync/raw_rwlock.h>

#define DEFAULT_ENTROPY	1

#define rbt_hnode	rbt_node
#define RBT_HTABLE_INITIALIZER {NULL, 0U, 0U, 0UL, RAW_ATOMIC_VAL_INIT(0)}

struct rbt_bucket {
	struct rbt_tree tree;
	struct raw_rwlock rwlock;
};

struct rbt_htable {
	struct rbt_bucket *table;
	uint32_t size;
	uint32_t entropy; /* entropy for hashing */
	long cnt_max;
	raw_atomic_long_t cnt;
};

/* generic interfaces for rb-based hash table */
extern struct rbt_bucket *rbt_htable_bucket(struct rbt_htable *htable, uint32_t hcode);
extern int rbt_htable_init(struct rbt_htable *htable, void *(*alloc)(size_t sz),
			   uint32_t size, size_t max);
extern int rbt_htable_destroy(struct rbt_htable *htable, void (*free)(void *table));
extern int rbt_htable_set_entropy(struct rbt_htable *htable, uint32_t entropy);
extern int rbt_htable_insert(struct rbt_htable *htable, struct rbt_bucket *bucket,
			     struct rbt_hnode *node, struct rbt_hnode *parent, bool is_right);
extern void rbt_htable_remove(struct rbt_htable *htable, struct rbt_bucket *bucket,
			      struct rbt_hnode *node);

/* usual hash funtions */
extern uint32_t rbt_htable_hash_u32(const struct rbt_htable *htable, uint32_t key);
extern uint32_t rbt_htable_hash_u64(const struct rbt_htable *htable, uint64_t key);
extern uint32_t rbt_htable_hash_str(const struct rbt_htable *htable, const char *str);
static inline uint32_t rbt_htable_hash_dstr(const struct rbt_htable *htable,
					    const char *str)
{
	return rbt_htable_hash_str(htable, str);
}

/*
 * Simplified version of rbt_htable which provides easy-to-use lookup/insert/remove
 * functions for user. All these operations are concurrent safe.
 */
struct easy_htable {
	struct rbt_htable ht;
	size_t node_off;
	size_t key_off;
};

extern int easy_htable_init(struct easy_htable *htable, void *(*alloc)(size_t sz),
			    uint32_t size, size_t max, size_t key_off, size_t node_off);
extern void easy_htable_destroy(struct easy_htable *htable, void (*free)(void *table));
extern void *easy_htable_lookup_u32(struct easy_htable *htable, uint32_t key,
				    void(*const call_back)(void *));
extern int easy_htable_insert_u32(struct easy_htable *htable, uint32_t key, struct rbt_hnode *node);
extern int easy_htable_remove_u32(struct easy_htable *htable, uint32_t key, struct rbt_hnode *node);
extern void *easy_htable_lookup_u64(struct easy_htable *htable, uint64_t key,
				    void(*const call_back)(void *));
extern int easy_htable_insert_u64(struct easy_htable *htable, uint64_t key, struct rbt_hnode *node);
extern int easy_htable_remove_u64(struct easy_htable *htable, uint64_t key, struct rbt_hnode *node);
/*
 * Two string-type APIs for easy_htable.
 * str means the string hash key is a char array member embedded
 * in a structure while dstr means the string hash key is a char *
 * member of a structure.
 */
extern void *easy_htable_lookup_str(struct easy_htable *htable, const char *key,
				    void(*const call_back)(void *));
extern int easy_htable_insert_str(struct easy_htable *htable, const char *key, struct rbt_hnode *node);
extern int easy_htable_remove_str(struct easy_htable *htable, const char *key, struct rbt_hnode *node);
extern void *easy_htable_lookup_dstr(struct easy_htable *htable, const char *key,
				     void(*const call_back)(void *));
extern int easy_htable_insert_dstr(struct easy_htable *htable, const char *key, struct rbt_hnode *node);
extern int easy_htable_remove_dstr(struct easy_htable *htable, const char *key, struct rbt_hnode *node);

#endif
