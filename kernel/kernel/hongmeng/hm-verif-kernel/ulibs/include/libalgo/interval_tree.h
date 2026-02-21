/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Interval tree algorithm implementation
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jun 12 11:53:14 2023
 */
#ifndef ULIBS_INTERVAL_TREE_H
#define ULIBS_INTERVAL_TREE_H

#include <stdint.h>
#include <stdbool.h>
#include <lib/utils.h>
#include <lib/dlist.h>
#include <libalgo/rbtree.h>
#include <libhmsync/raw_rwlock.h>

#ifndef NULL
#define NULL ((void *)0)
#endif

struct interval_node {
	struct rbt_node rbnode;
	uint64_t subtree_last;
	uint64_t start;
	uint64_t end;
#ifdef CONFIG_INTERVAL_TREE_DNODE
	struct dlist_node dnode;
#endif
};

struct interval_tree {
	struct aug_rbt_tree arbt;
};

void interval_tree_init(struct interval_tree *tree);
void interval_tree_insert(struct interval_tree *tree, struct interval_node *node, uint64_t start, uint64_t end);
void interval_tree_insert_after(struct interval_tree *tree,
				struct interval_node *prev,
				struct interval_node *node, uint64_t start,
				uint64_t end);
void interval_tree_remove(struct interval_tree *tree, struct interval_node *node);

bool interval_tree_empty(const struct interval_tree *tree);

struct interval_node *interval_tree_first_node(const struct interval_tree *tree);
struct interval_node *interval_tree_next_node(const struct interval_node *node);

struct interval_node *interval_tree_first_node_range(const struct interval_tree *tree,
						     uint64_t start, uint64_t end);
struct interval_node *interval_tree_next_node_range(const struct interval_node *node,
						     uint64_t start, uint64_t end);
#define interval_node_entry(node, type, member) \
	((node) ? container_of(node, type, member) : NULL)

#define interval_tree_first_entry(tree, type, member) \
	({ \
		struct interval_node *__node = interval_tree_first_node(tree); \
		interval_node_entry(__node, type, member); \
	})
#define interval_tree_next_entry(node, type, member) \
	({ \
		struct interval_node *__node = interval_tree_next_node(node); \
		interval_node_entry(__node, type, member); \
	})
#define interval_tree_first_entry_range(tree, type, member, start, end) \
	({ \
		struct interval_node *__node = interval_tree_first_node_range(tree, start, end); \
		interval_node_entry(__node, type, member); \
	})
#define interval_tree_next_entry_range(tree, node, type, member, start, end) \
	({ \
		struct interval_node *__node = interval_tree_next_node_range(node, start, end); \
		interval_node_entry(__node, type, member); \
	})

#define interval_tree_iter(iter, tree, type, member)			\
	for ((iter) = interval_tree_first_entry(tree, type, member);	\
	     (iter) != NULL;						\
	     (iter) = interval_tree_next_entry(&(iter)->member, type, member))
#define interval_tree_iter_range(iter, tree, type, member, start, end)				\
	for ((iter) = interval_tree_first_entry_range(tree, type, member, start, end);	\
	     (iter) != NULL;									\
	     (iter) = interval_tree_next_entry_range(tree, &(iter)->member, type, member, start, end))
#endif
