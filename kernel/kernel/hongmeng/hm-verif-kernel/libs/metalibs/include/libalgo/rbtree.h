/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Red-black tree algorithm implementation
 * Author: Huawei OS Kernel Lab
 * Create: Thu Nov 15 21:27:14 2018
 */
#ifndef ULIBS_LIBRBTREE_RBTREE_H
#define ULIBS_LIBRBTREE_RBTREE_H

#include <stdint.h>
#include <stdbool.h>
#include <lib/utils.h>

/*
 * In following declaration of struct rbt_node, it is aligned to uintptr_t,
 * which ensures that lowest bits of rbt_node variable address should be 0.
 * Here will use both the bits to store red/black attribute of one red-black
 * node.
 */
struct rbt_node {
	uintptr_t rbt_parent_color;
	struct rbt_node *rbt_left;
	struct rbt_node *rbt_right;
} __attribute__((aligned(sizeof(uintptr_t))));

struct rbt_tree {
	struct rbt_node *root;
};

typedef bool (*summary_update_callback)(struct rbt_node *node);

/*
 * RBtree with extra callbacks on basic operations.
 */
struct aug_rbt_tree {
	struct rbt_tree tree;
	summary_update_callback sumry_upd_cb;
};

/*
 * rbt_slot is used to describe one insertable position in a rbtree.
 */
struct rbt_slot {
	struct rbt_node *parent;
	bool is_right_child;
};

/*
 * Three state of color for node of red-black tree:
 * red, black, and colorless.
 *
 * Colorless tells that the node has no red-black tree
 * to locate in right now.
 */
#define RBT_COLOR_MASK		(uintptr_t)0x3
#define RBT_COLOR_RED		(uintptr_t)0x2
#define RBT_COLOR_BLACK		(uintptr_t)0x1
#define RBT_COLOR_COLORLESS	(uintptr_t)0x0

#define __rbt_color(p)		((uintptr_t)((p)->rbt_parent_color) & RBT_COLOR_MASK)
#define __rbt_is_colorless(p)	(__rbt_color(p) == RBT_COLOR_COLORLESS)
#define __rbt_set_parent(p, v)	((p)->rbt_parent_color = __rbt_color(p) | (uintptr_t)(v))
#define __rbt_set_colorless(p)	((p)->rbt_parent_color &= ~RBT_COLOR_MASK)

#define __rbt_is_red(p)		((__rbt_color(p) & RBT_COLOR_RED) != 0UL)
#define __rbt_is_black(p)	((__rbt_color(p) & RBT_COLOR_BLACK) != 0UL)
#define __rbt_set_red(p)	((p)->rbt_parent_color = (uintptr_t)rbt_parent(p) | RBT_COLOR_RED)
#define __rbt_set_black(p)	((p)->rbt_parent_color = (uintptr_t)rbt_parent(p) | RBT_COLOR_BLACK)

static inline struct rbt_node* rbt_parent(const struct rbt_node *node)
{
	return (struct rbt_node *)(node->rbt_parent_color & (~RBT_COLOR_MASK));
}

static inline void aug_rbt_subtree_summary_upd_cb_register(
			struct aug_rbt_tree *aug_tree,
			summary_update_callback upd_cb)
{
	aug_tree->sumry_upd_cb = upd_cb;
}

extern void rbt_subtree_summary_update(struct rbt_node *from,
				       summary_update_callback upd_cb);

extern struct rbt_node* rbt_first(const struct rbt_tree *tree);
extern struct rbt_node* rbt_last(const struct rbt_tree *tree);
extern struct rbt_node* rbt_prev(const struct rbt_node *node);
extern struct rbt_node* rbt_next(const struct rbt_node *node);
extern void rbt_insert(struct rbt_tree *tree, struct rbt_node *node,
		       struct rbt_node *parent, bool is_rchild);
extern void aug_rbt_insert(struct aug_rbt_tree *tree, struct rbt_node *node,
		       struct rbt_node *parent, bool is_rchild);
extern void aug_rbt_insert_with_callback(struct aug_rbt_tree *tree, struct rbt_node *node,
		struct rbt_node *parent, bool is_rchild, summary_update_callback upd_cb);

extern void rbt_remove(struct rbt_tree *tree, struct rbt_node *node);
extern void aug_rbt_remove(struct aug_rbt_tree *aug_tree, struct rbt_node *node);
extern void aug_rbt_remove_with_callback(struct aug_rbt_tree *aug_tree, struct rbt_node *node,
		summary_update_callback upd_cb);
extern void rbt_destroy_tree(struct rbt_tree *tree,
			     void (*call_back)(struct rbt_node *));
extern void rbt_node_init_from(struct rbt_node *new_node,
			       const struct rbt_node *old_node,
			       const struct rbt_node *parent);
extern void rbt_tree_init(struct rbt_tree *tree);
extern void aug_rbt_tree_init(struct aug_rbt_tree *aug_tree);
extern bool rbt_is_removable_node(const struct rbt_node *node);
extern void __rbt_insert_fixup(struct rbt_tree *tree, struct rbt_node *node,
			       summary_update_callback upd_cb);

#define rbt_entry(p, type, member) (container_of(p, type, member))
#define RBT_ROOT { NULL, }

#define DEFINE_rbt_search(name, type, node_mem, key_mem)		\
static type *								\
name##_rbt_search(struct rbt_tree *tree,				\
		  const typeof(((type *)0)->key_mem) key,		\
		  struct rbt_node **pparent_node)			\
{									\
	struct rbt_node *_node = tree->root;				\
	struct rbt_node *_parent = NULL;				\
	/* tree is NULL */						\
	if (_node == NULL) {						\
		_parent = NULL;						\
	}								\
	while (_node != NULL) {						\
		if (rbt_entry(_node, type, node_mem)->key_mem == key) {	\
			_parent = rbt_parent(_node);			\
			break;						\
		}							\
		/* not equal, continue to find correct one */		\
		if (rbt_entry(_node, type, node_mem)->key_mem < key) {	\
			/* in right subtree */				\
			if (_node->rbt_right == NULL) {			\
				/* no right child */			\
				_parent = _node;			\
				_node = NULL;				\
				break;					\
			} else {					\
				/* has right child, continue */ 	\
				_node = _node->rbt_right;		\
			}						\
		} else {						\
			/* in left subtree */				\
			if (_node->rbt_left == NULL) {			\
				/* no left child */			\
				_parent = _node;			\
				_node = NULL;				\
				break;					\
			} else {					\
				/* has left child, continue */		\
				_node = _node->rbt_left;		\
			}						\
		}							\
	}								\
	if (pparent_node != NULL) {					\
		*pparent_node = _parent;				\
	}								\
	return ((_node != NULL) ? rbt_entry(_node, type, node_mem) : NULL);\
}									\

#define rbt_first_entry(tree, type, member)				\
	(((tree)->root == NULL) ? NULL : rbt_entry(rbt_first(tree), type, member))

#define rbt_for_each_entry_safe(ent, nn, tree, type, member)		\
	for ((ent) = rbt_first_entry(tree, type, member),			\
	     (nn) = (((ent) == NULL) ? NULL : rbt_next(&(ent)->member));	\
	     (ent) != NULL;						\
	     (ent) = (((nn) == NULL) ? NULL : rbt_entry(nn, type, member)),	\
	     (nn) = (((ent) == NULL) ? NULL : rbt_next(&(ent)->member)))

#endif
