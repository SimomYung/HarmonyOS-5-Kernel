/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Declarations of ebr-safe red-black tree with latch mechanism
 * Author: Huawei OS Kernel Lab
 * Create: Thu Dec 24 11:27:14 2020
 */

#ifndef ULIBS_LIBHMEBR_RBTREE_H
#define ULIBS_LIBHMEBR_RBTREE_H

#include <libalgo/rbtree.h>
#include <vsync/atomic.h>
#include <libhmebr/ebr_singleton.h>

#define NR_RBT_NODES 2

struct ebr_rbt_node {
	struct rbt_node node[NR_RBT_NODES];
};

struct ebr_rbt_tree {
	vatomic64_t sequence;
	struct rbt_tree trees[NR_RBT_NODES];
	int (*key_cmp)(const void *key, const struct ebr_rbt_node *node);
	int (*node_cmp)(const struct ebr_rbt_node *node1,
			const struct ebr_rbt_node *node2);
};

int ebr_rbt_init(struct ebr_rbt_tree *tree,
		 int (*key_cmp)(const void *key, const struct ebr_rbt_node *node),
		 int (*node_cmp)(const struct ebr_rbt_node *node1,
				 const struct ebr_rbt_node *node2));
struct ebr_rbt_node *ebr_rbt_find(struct ebr_rbt_tree *tree, const void *key);
int ebr_rbt_insert(struct ebr_rbt_tree *tree, struct ebr_rbt_node *node);
int ebr_rbt_remove(struct ebr_rbt_tree *tree, struct ebr_rbt_node *node);
int ebr_rbt_apply(struct ebr_rbt_tree *tree,
		  int (*callback)(struct ebr_rbt_node *, void *),
		  void *arg);
bool ebr_rbt_is_removable_node(struct ebr_rbt_tree *tree,
			       struct ebr_rbt_node *node);
/* another version of insert for ebr-safe latch rbtree */
extern void rbt_insert_ebr(struct rbt_tree *tree, struct rbt_node *node,
			   struct rbt_node *parent, bool is_rchild);
#endif
