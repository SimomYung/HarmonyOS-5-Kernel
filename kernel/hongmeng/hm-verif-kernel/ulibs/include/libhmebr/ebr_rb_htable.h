/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Declarations of ebr-safe hash table based-on red-black tree
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jan 04 11:27:14 2021
 */

#ifndef ULIBS_LIBHMEBR_EBR_RB_HTABLE_H
#define ULIBS_LIBHMEBR_EBR_RB_HTABLE_H

#include <string.h>
#include <libhmebr/ebr_rbtree.h>
#include <vsync/atomic.h>

#define DEFAULT_ENTROPY	1
typedef struct ebr_rbt_node ebr_rbt_hnode;

struct ebr_rbt_htable {
	struct ebr_rbt_tree *table;
	uint32_t size;
	uint32_t entropy;
	size_t cnt_max;
	vatomic64_t cnt;
	struct ebr_rbt_htable_ops *ops;
};

struct ebr_rbt_htable_ops {
	void *(*alloc)(size_t size);
	void (*release)(void *);
	uint32_t (*key_hash)(const struct ebr_rbt_htable *htable,
			     const void *key);
	uint32_t (*node_hash)(const struct ebr_rbt_htable *htable,
			      const ebr_rbt_hnode *node);
	int (*key_cmp)(const void *key, const ebr_rbt_hnode *node);
	int (*node_cmp)(const ebr_rbt_hnode *node1,
			const ebr_rbt_hnode *node2);
};

int ebr_rbt_htable_init(struct ebr_rbt_htable *htable,
			struct ebr_rbt_htable_ops *ops,
			uint32_t size, size_t max);
int ebr_rbt_htable_destroy(struct ebr_rbt_htable *htable);
bool ebr_rbt_htable_is_empty(struct ebr_rbt_htable *htable);
int ebr_rbt_htable_set_entropy(struct ebr_rbt_htable *htable, uint32_t entropy);
ebr_rbt_hnode *ebr_rbt_htable_find(struct ebr_rbt_htable *htable,
					  const void *key);
int ebr_rbt_htable_insert(struct ebr_rbt_htable *htable,
			  ebr_rbt_hnode *node);
int ebr_rbt_htable_remove(struct ebr_rbt_htable *htable,
			  ebr_rbt_hnode *node);
int ebr_rbt_htable_apply(struct ebr_rbt_htable *htable,
			 int (*callback)(ebr_rbt_hnode *, void *),
			 void *arg);
uint32_t ebr_rbt_hash_u32(const struct ebr_rbt_htable *htable, uint32_t key);
uint32_t ebr_rbt_hash_u64(const struct ebr_rbt_htable *htable, uint64_t key);
uint32_t ebr_rbt_hash_str(const struct ebr_rbt_htable *htable, const char *str);
#endif
