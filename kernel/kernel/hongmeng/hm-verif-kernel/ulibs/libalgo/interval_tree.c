/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: interval tree implementation
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jun 12 11:53:14 2023
 */
#include <libalgo/interval_tree.h>
#include <libhmlog/hmlog.h>
#include <hongmeng/panic.h>
#include <hongmeng/types.h>

static inline uint64_t start_of(struct interval_node *node)
{
	return node->start;
}

static inline uint64_t end_of(struct interval_node *node)
{
	return node->end;
}

static bool interval_tree_last_update(struct rbt_node *rbnode)
{
	struct interval_node *node = NULL;
	struct interval_node *ch = NULL;
	uint64_t old;
	bool updated = false;

	if (rbnode != NULL) {
		node = rbt_entry(rbnode, struct interval_node, rbnode);
		old = node->subtree_last;
		node->subtree_last = end_of(node);
		if (rbnode->rbt_left != NULL) {
			ch = rbt_entry(rbnode->rbt_left, struct interval_node, rbnode);
			if (ch->subtree_last > node->subtree_last) {
				node->subtree_last = ch->subtree_last;
			}
		}
		if (rbnode->rbt_right != NULL) {
			ch = rbt_entry(rbnode->rbt_right, struct interval_node, rbnode);
			if (ch->subtree_last > node->subtree_last) {
				node->subtree_last = ch->subtree_last;
			}
		}
		updated = (node->subtree_last != old);
	}

	return updated;
}

void interval_tree_init(struct interval_tree *tree)
{
	aug_rbt_tree_init(&tree->arbt);
	aug_rbt_subtree_summary_upd_cb_register(&tree->arbt, interval_tree_last_update);
}

bool interval_tree_empty(const struct interval_tree *tree)
{
	return tree->arbt.tree.root == NULL;
}

struct interval_node *interval_tree_first_node(const struct interval_tree *tree)
{
	return rbt_first_entry(&(tree)->arbt.tree, struct interval_node, rbnode);
}

#ifdef CONFIG_INTERVAL_TREE_DNODE
enum {
	INTERVAL_LESS = -1,
	INTERVAL_EQUAL = 0,
	INTERVAL_GREATER = 1,
};
static int interval_compare(struct interval_node *a, struct interval_node *b)
{
	int ret = INTERVAL_EQUAL;
	if (start_of(a) < start_of(b)) {
		ret = INTERVAL_LESS;
	} else if (start_of(a) > start_of(b)) {
		ret = INTERVAL_GREATER;
	} else if (end_of(a) < end_of(b)) {
		ret = INTERVAL_LESS;
	} else if (end_of(a) > end_of(b)) {
		ret = INTERVAL_GREATER;
	}
	return ret;
}

void interval_tree_insert(struct interval_tree *tree, struct interval_node *node, uint64_t start, uint64_t end)
{
	struct interval_node *pos = NULL;
	struct rbt_node *rbpos = NULL;
	int cmp = INTERVAL_GREATER;

	rbpos = tree->arbt.tree.root;
	node->subtree_last = 0;
	node->start = start;
	node->end = end;
	dlist_init(&node->dnode);
	__rbt_set_colorless(&node->rbnode);
	while (rbpos) {
		struct rbt_node *rbnext = NULL;

		pos = rbt_entry(rbpos, struct interval_node, rbnode);
		cmp = interval_compare(node, pos);
		if (cmp == INTERVAL_EQUAL) {
			break;
		}
		rbnext = (cmp == INTERVAL_GREATER) ? rbpos->rbt_right : rbpos->rbt_left;
		if (rbnext == NULL) {
			break;
		}
		rbpos = rbnext;
	}
	if (cmp == INTERVAL_EQUAL) {
		dlist_insert(&pos->dnode, &node->dnode);
	} else {
		aug_rbt_insert_with_callback(&tree->arbt, &node->rbnode, rbpos, cmp == INTERVAL_GREATER, interval_tree_last_update);
	}
}

void interval_tree_insert_after(struct interval_tree *tree,
				struct interval_node *prev,
				struct interval_node *node, uint64_t start,
				uint64_t end)
{
	node->subtree_last = 0;
	node->start = start;
	node->end = end;
	dlist_init(&node->dnode);
	__rbt_set_colorless(&node->rbnode);

	BUG_ON_D(interval_compare(node, prev) != INTERVAL_EQUAL,
		 "prev=[0x%lx, 0x%lx), node=[0x%lx, 0x%lx)\n",
		 prev->start, prev->end, start, end);
	dlist_insert(&prev->dnode, &node->dnode);
	(void)tree;
}

static void replace_rbnode(struct rbt_tree *tree, struct rbt_node *new, struct rbt_node *old)
{
	*new = *old;
	old->rbt_parent_color = 0;
	old->rbt_left = NULL;
	old->rbt_right = NULL;
	if (rbt_parent(new) != NULL) {
		if (rbt_parent(new)->rbt_left == old) {
			rbt_parent(new)->rbt_left = new;
		} else {
			rbt_parent(new)->rbt_right = new;
		}
	} else {
		tree->root = new;
	}
	if (new->rbt_left != NULL) {
		__rbt_set_parent(new->rbt_left, new);
	}
	if (new->rbt_right != NULL) {
		__rbt_set_parent(new->rbt_right, new);
	}
}

void interval_tree_remove(struct interval_tree *tree, struct interval_node *node)
{
	if (__rbt_is_colorless(&node->rbnode)) {
		dlist_delete(&node->dnode);
	} else if (!dlist_empty(&node->dnode)) {
		struct interval_node *next = dlist_next_entry(node, struct interval_node, dnode);
		replace_rbnode(&tree->arbt.tree, &next->rbnode, &node->rbnode);
		next->subtree_last = node->subtree_last;
		dlist_delete(&node->dnode);
	} else {
		aug_rbt_remove_with_callback(&tree->arbt, &node->rbnode, interval_tree_last_update);
	}
	node->subtree_last = 0;
	node->start = 0;
	node->end = 0;
}

struct interval_node *interval_tree_next_node(const struct interval_node *node)
{
	struct interval_node *next = dlist_next_entry(node, struct interval_node, dnode);

	if (!__rbt_is_colorless(&next->rbnode)) {
		struct rbt_node *rbnext = rbt_next(&next->rbnode);
		next = (rbnext == NULL) ? NULL : rbt_entry(rbnext, struct interval_node, rbnode);
	}

	return next;
}
#else
void interval_tree_insert(struct interval_tree *tree, struct interval_node *node, uint64_t start, uint64_t end)
{
	struct rbt_node *rbpos = NULL;
	bool rch = false;

	rbpos = tree->arbt.tree.root;
	node->subtree_last = 0;
	node->start = start;
	node->end = end;

	while (rbpos) {
		struct interval_node *pos = rbt_entry(rbpos, struct interval_node, rbnode);
		struct rbt_node *rbnext = NULL;

		rch = start_of(node) > start_of(pos);
		rbnext = rch ? rbpos->rbt_right : rbpos->rbt_left;
		if (rbnext == NULL) {
			break;
		}
		rbpos = rbnext;
	}
	aug_rbt_insert_with_callback(&tree->arbt, &node->rbnode, rbpos, rch, interval_tree_last_update);
}

void interval_tree_insert_after(struct interval_tree *tree,
				struct interval_node *prev,
				struct interval_node *node, uint64_t start,
				uint64_t end)
{
	struct interval_node *parent;
	bool start_not_equal = true;

	node->subtree_last = 0;
	node->start = start;
	node->end = end;

	start_not_equal = (start_of(prev) != start_of(node));
	BUG_ON(start_not_equal);

	if (prev->rbnode.rbt_right == NULL) {
		aug_rbt_insert(&tree->arbt, &node->rbnode, &prev->rbnode, true);
	} else {
		parent = rbt_entry(prev->rbnode.rbt_right, struct interval_node,
				   rbnode);
		while (parent != NULL) {
			if (parent->rbnode.rbt_left != NULL) {
				parent = rbt_entry(parent->rbnode.rbt_left,
						   struct interval_node, rbnode);
			} else {
				break;
			}
		}
		aug_rbt_insert_with_callback(&tree->arbt, &node->rbnode, &parent->rbnode,
					     false, interval_tree_last_update);
	}
}

void interval_tree_remove(struct interval_tree *tree, struct interval_node *node)
{
	aug_rbt_remove_with_callback(&tree->arbt, &node->rbnode, interval_tree_last_update);
	node->subtree_last = 0;
	node->start = 0;
	node->end = 0;
}

struct interval_node *interval_tree_next_node(const struct interval_node *node)
{
	struct rbt_node *rbnext = rbt_next(&node->rbnode);

	return (rbnext == NULL) ? NULL : rbt_entry(rbnext, struct interval_node, rbnode);
}
#endif

/*
 * search node [s, e) intersect with range [l, r), i.e. should satisfy:
 * COND1: s < r
 * COND2. e > l
 */

struct interval_node *interval_tree_first_node_range(const struct interval_tree *tree,
						     uint64_t start, uint64_t end)
{
	struct rbt_node *rbpos = interval_tree_empty(tree) ? NULL : tree->arbt.tree.root;
	struct interval_node *found = NULL;

	if (start >= end) {
		rbpos = NULL;
	}
	while (rbpos != NULL) {
		struct interval_node *pos = rbt_entry(rbpos, struct interval_node, rbnode);
		if (pos->subtree_last <= start) {
			break;
		}
		if (rbpos->rbt_left != NULL) {
			struct interval_node *ch = rbt_entry(rbpos->rbt_left, struct interval_node, rbnode);
			if (ch->subtree_last > start) {
				rbpos = rbpos->rbt_left;
				continue;
			}
		}
		if (start_of(pos) >= end) {
			break;
		}
		if (end_of(pos) > start) {
			found = pos;
			break;
		}
		rbpos = rbpos->rbt_right;
	}
	return found;
}

static struct interval_node *__interval_tree_next_node_range(const struct interval_node *node,
							     uint64_t start, uint64_t end)
{
	struct rbt_node *rbpos = NULL;
	struct interval_node *found = NULL;
	bool search_left = false;
	bool search_root = false;
	bool search_right = true;
	bool search_parent = true;

	rbpos = &((struct interval_node *)node)->rbnode;
	if (start >= end) {
		rbpos = NULL;
	}
	while (rbpos != NULL) {
		if (search_left && rbpos->rbt_left != NULL) {
			struct interval_node *ch = rbt_entry(rbpos->rbt_left, struct interval_node, rbnode);
			if (ch->subtree_last > start) {
				rbpos = rbpos->rbt_left;
				search_left = true;
				search_root = true;
				search_right = true;
				search_parent = false;
				continue;
			}
		}
		if (search_root) {
			struct interval_node *pos = rbt_entry(rbpos, struct interval_node, rbnode);
			if (start_of(pos) < end && end_of(pos) > start) {
				found = pos;
				break;
			}
		}
		if (search_right && rbpos->rbt_right != NULL) {
			struct interval_node *ch = rbt_entry(rbpos->rbt_right, struct interval_node, rbnode);
			if (ch->subtree_last >= start) {
				rbpos = rbpos->rbt_right;
				search_left = true;
				search_root = true;
				search_right = true;
				continue;
			}
		}
		if (search_parent && rbt_parent(rbpos) != NULL) {
			search_left = false;
			search_root = (rbpos == rbt_parent(rbpos)->rbt_left);
			search_right = (rbpos == rbt_parent(rbpos)->rbt_left);
			rbpos = rbt_parent(rbpos);
			continue;
		}
		break;
	}
	return found;
}

#ifdef CONFIG_INTERVAL_TREE_DNODE
struct interval_node *interval_tree_next_node_range(const struct interval_node *node,
						    uint64_t start, uint64_t end)
{
	struct interval_node *next = dlist_next_entry(node, struct interval_node, dnode);

	if (!__rbt_is_colorless(&next->rbnode)) {
		next = __interval_tree_next_node_range(next, start, end);
	}

	return next;
}
#else
struct interval_node *interval_tree_next_node_range(const struct interval_node *node,
						    uint64_t start, uint64_t end)
{
	return __interval_tree_next_node_range(node, start, end);
}
#endif
