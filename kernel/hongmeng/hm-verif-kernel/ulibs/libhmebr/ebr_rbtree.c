/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: implementation of ebr-safe red-black tree with latch mechanism
 * Author: Huawei OS Kernel Lab
 * Create: Thu Dec 24 10:57:14 2020
 */

#include <libhmebr/ebr_rbtree.h>
#include <libhmebr/ebr.h>
#include <libalgo/rbtree.h>

static void __rbt_insert_ebr(struct rbt_tree *tree, struct rbt_node *node,
		      struct rbt_node *parent, bool is_rchild,
		      summary_update_callback upd_cb)
{
	node->rbt_left = NULL;
	node->rbt_right = NULL;
	node->rbt_parent_color = RBT_COLOR_COLORLESS;

	if (parent == NULL) {
		__rbt_set_parent(node, NULL);
		__rbt_set_black(node);
		ebr_assign_pointer(tree->root, node);
	} else {
		__rbt_set_parent(node, parent);
		__rbt_set_red(node);

		if (is_rchild) {
			ebr_assign_pointer(parent->rbt_right, node);
		} else {
			ebr_assign_pointer(parent->rbt_left, node);
		}

		rbt_subtree_summary_update(parent, upd_cb);

		if (__rbt_is_red(parent)) {
			__rbt_insert_fixup(tree, node, upd_cb);
		}
	}
}

void rbt_insert_ebr(struct rbt_tree *tree, struct rbt_node *node,
		    struct rbt_node *parent, bool is_rchild)
{
	__rbt_insert_ebr(tree, node, parent, is_rchild, NULL);
}

static int do_insert(struct ebr_rbt_tree *tree, struct ebr_rbt_node *node,
		     int (*node_cmp)(const struct ebr_rbt_node *node1,
		     const struct ebr_rbt_node *node2),
		     unsigned int idx)
{
	int ret = E_HM_OK;
	int err = E_HM_OK;
	bool is_rchild = false;
	struct rbt_tree *rb_tree = NULL;
	struct rbt_node *rb_parent = NULL;
	struct rbt_node *rb_current = NULL;
	struct ebr_rbt_node *current = NULL;

	rb_tree = &tree->trees[idx];
	rb_current = ebr_dereference(rb_tree->root);
	while (rb_current != NULL) {
		current = container_of(rb_current, struct ebr_rbt_node,
				       node[idx]);
		ret = node_cmp(node, current);
		if (ret < 0) {
			rb_parent = rb_current;
			rb_current = ebr_dereference(rb_current->rbt_left);
			is_rchild = false;
		} else if (ret > 0) {
			rb_parent = rb_current;
			rb_current = ebr_dereference(rb_current->rbt_right);
			is_rchild = true;
		} else {
			err = E_HM_OBJEXIST;
			break;
		}
	}
	if (err == E_HM_OK) {
		rbt_insert_ebr(rb_tree, &node->node[idx], rb_parent, is_rchild);
	}

	return err;
}

static struct ebr_rbt_node *do_find(struct ebr_rbt_tree *tree, const void *key,
				    int (*key_cmp)(const void *key,
				    const struct ebr_rbt_node *node),
				    unsigned long long idx)
{
	int ret = 0;
	struct rbt_tree *rb_tree;
	struct rbt_node *rb_current;
	struct ebr_rbt_node *current;

	rb_tree = &tree->trees[idx];
	rb_current = ebr_dereference(rb_tree->root);
	while (rb_current != NULL) {
		current = container_of(rb_current, struct ebr_rbt_node, node[idx]);
		ret = key_cmp(key, current);
		if (ret < 0) {
			rb_current = ebr_dereference(rb_current->rbt_left);
		} else if (ret > 0) {
			rb_current = ebr_dereference(rb_current->rbt_right);
		} else {
			return current;
		}
	}

	return NULL;
}

int ebr_rbt_init(struct ebr_rbt_tree *tree,
		 int (*key_cmp)(const void *key, const struct ebr_rbt_node *node),
		 int (*node_cmp)(const struct ebr_rbt_node *node1,
		 const struct ebr_rbt_node *node2))
{
	int ret = E_HM_OK;
	if (tree == NULL || key_cmp == NULL || node_cmp == NULL) {
		ret = E_HM_INVAL;
	} else {
		vatomic64_write(&tree->sequence, 0);
		tree->trees[0].root = NULL;
		tree->trees[1].root = NULL;
		tree->key_cmp = key_cmp;
		tree->node_cmp = node_cmp;
	}

	return ret;
}

struct ebr_rbt_node *ebr_rbt_find(struct ebr_rbt_tree *tree, const void *key)
{
	unsigned long long seq;
	struct ebr_rbt_node *node = NULL;

	if (tree != NULL) {
		do {
			seq = vatomic64_read_acq(&tree->sequence);
			node = do_find(tree, key, tree->key_cmp, seq & 1ULL);
			vatomic_fence_acq();
		} while (vatomic64_read_acq(&tree->sequence) != seq);
	}

	return node;
}

int ebr_rbt_insert(struct ebr_rbt_tree *tree, struct ebr_rbt_node *node)
{
	int ret = E_HM_OK;

	if (tree == NULL) {
		ret = E_HM_INVAL;
	} else {
		vatomic64_inc_rel(&tree->sequence);
		vatomic_fence_rel();
		ret = do_insert(tree, node, tree->node_cmp, 0);
		vatomic64_inc_rel(&tree->sequence);
		if (ret == E_HM_OK) {
			vatomic_fence_rel();
			ret = do_insert(tree, node, tree->node_cmp, 1);
		}
	}

	return ret;
}

int ebr_rbt_remove(struct ebr_rbt_tree *tree, struct ebr_rbt_node *node)
{
	int ret = E_HM_OK;
	if (tree == NULL) {
		ret = E_HM_INVAL;
	} else {
		vatomic64_inc_rel(&tree->sequence);
		vatomic_fence_rel();
		rbt_remove(&tree->trees[0], &node->node[0]);

		vatomic64_inc_rel(&tree->sequence);
		vatomic_fence_rel();
		rbt_remove(&tree->trees[1], &node->node[1]);
	}

	return ret;
}

int ebr_rbt_apply(struct ebr_rbt_tree *tree,
		  int (*callback)(struct ebr_rbt_node *enode, void *arg), void *arg)
{
	int ret = E_HM_OK;
	struct rbt_node *node = NULL;
	struct rbt_node *next_node = NULL;
	struct ebr_rbt_node *enode = NULL;

	if (tree == NULL || callback == NULL) {
		ret = E_HM_INVAL;
	} else {
		node = rbt_first(&tree->trees[0]);
		while (node != NULL) {
			next_node = rbt_next(node);

			enode = container_of(node, struct ebr_rbt_node, node[0]);
			ret = callback(enode, arg);
			if (ret < 0) {
				break;
			}
			node = next_node;
		}
	}

	return ret;
}

bool ebr_rbt_is_removable_node(struct ebr_rbt_tree *tree,
			       struct ebr_rbt_node *node)
{
	unsigned long long seq = 0;
	bool is = false;

	do {
		seq = vatomic64_read_acq(&tree->sequence);
		is = rbt_is_removable_node(&node->node[seq & 1U]);
		vatomic_fence_acq();
	} while (vatomic64_read_acq(&tree->sequence) != seq);

	return is;
}
