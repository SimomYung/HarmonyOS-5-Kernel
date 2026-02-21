/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Reb-black tree implementation
 * Author: Huawei OS Kernel Lab
 * Create: Thu Nov 15 21:27:14 2018
 */

#include <libalgo/rbtree.h>

#undef	BUG_ON
#define	BUG_ON(cond)	\
do {	\
	if ((cond)) {	\
		__builtin_trap(); \
		__builtin_unreachable(); \
	}	\
} while (__false)

#include <libstrict/strict_base.h>

#ifndef NULL
#define NULL ((void *)0)
#endif

struct rbt_node* rbt_prev(const struct rbt_node *node)
{
	struct rbt_node *p = NULL;

	p = node->rbt_left;
	if (p != NULL) {
		/* search downward in left sub tree */
		while ((p != NULL) && (p->rbt_right != NULL)) {
			p = p->rbt_right;
		}
	} else {
		/* search upward the tree */
		p = rbt_parent(node);
		while ((p != NULL) && (p->rbt_right != node)) {
			node = p;
			p = rbt_parent(node);
		}
	}

	return p;
}

struct rbt_node* rbt_next(const struct rbt_node *node)
{
	struct rbt_node *p = NULL;

	p = node->rbt_right;
	if (p != NULL) {
		/* search downward in right sub tree */
		while ((p != NULL) && (p->rbt_left != NULL)) {
			p = p->rbt_left;
		}
	} else {
		/* search upward the tree */
		p = rbt_parent(node);
		while ((p != NULL) && (p->rbt_left != node)) {
			node = p;
			p = rbt_parent(node);
		}
	}

	return p;
}

struct rbt_node* rbt_first(const struct rbt_tree *tree)
{
	struct rbt_node *p = NULL;
	p = tree->root;
	while ((p != NULL) && (p->rbt_left != NULL)) {
		p = p->rbt_left;
	}
	return p;
}

struct rbt_node* rbt_last(const struct rbt_tree *tree)
{
	struct rbt_node *p = NULL;

	p = tree->root;
	while ((p != NULL) && (p->rbt_right != NULL)) {
		p = p->rbt_right;
	}
	return p;
}

static void __rbt_rotate_left(struct rbt_tree *tree, struct rbt_node *node,
			      summary_update_callback upd_cb)
{
	struct rbt_node *parent = NULL;
	struct rbt_node *rchild = NULL;
	struct rbt_node *lgson = NULL;

	/* relink parent <==> right child */
	parent = rbt_parent(node);
	rchild = node->rbt_right;
	if (parent != NULL) {
		if (parent->rbt_left == node) {
			parent->rbt_left = rchild;
		} else {
			parent->rbt_right = rchild;
		}
		if (rchild != NULL) {
			__rbt_set_parent(rchild, parent);
		}
	} else {
		tree->root = rchild;
		__rbt_set_parent(rchild, parent);
	}

	/* relink node <==> left grandson */
	IMPOSSIBLE(rchild == NULL, "prerequisite of left rotate ensures right child non-null");
	lgson = rchild->rbt_left;
	node->rbt_right = lgson;
	if (lgson != NULL) {
		__rbt_set_parent(lgson, node);
	}

	/* relink node <==> rchild */
	rchild->rbt_left = node;
	__rbt_set_parent(node, rchild);

	if (upd_cb != NULL) {
		(void)upd_cb(node);
		(void)upd_cb(rchild);
	}
}

static void __rbt_rotate_right(struct rbt_tree *tree, struct rbt_node *node,
			       summary_update_callback upd_cb)
{
	struct rbt_node *parent = NULL;
	struct rbt_node *lchild = NULL;
	struct rbt_node *rgson = NULL;

	/* relink parent <==> left child */
	parent = rbt_parent(node);
	lchild = node->rbt_left;
	if (parent != NULL) {
		if (parent->rbt_left == node) {
			parent->rbt_left = lchild;
		} else {
			parent->rbt_right = lchild;
		}
		if (lchild != NULL) {
			__rbt_set_parent(lchild, parent);
		}
	} else {
		tree->root = lchild;
		__rbt_set_parent(lchild, parent);
	}

	/* relink node <==> right grandson */
	IMPOSSIBLE(lchild == NULL, "prerequisite of right rotate ensures left child non-null");
	rgson = lchild->rbt_right;
	node->rbt_left = rgson;
	if (rgson != NULL) {
		__rbt_set_parent(rgson, node);
	}

	/* relink node <==> lchild */
	lchild->rbt_right = node;
	__rbt_set_parent(node, lchild);

	if (upd_cb != NULL) {
		(void)upd_cb(node);
		(void)upd_cb(lchild);
	}
}

static inline void __rbt_insert_fixup_red_uncle(struct rbt_node *gparent,
						struct rbt_node *uncle,
						struct rbt_node **parent,
						struct rbt_node **node)
{
	/*
	 * insert balance case 1:
	 * uncle red
	 */
	__rbt_set_black(*parent);
	__rbt_set_black(uncle);
	__rbt_set_red(gparent);
	*node = gparent;
	*parent = rbt_parent(*node);
}

static void __rbt_insert_fixup_black_uncle_parent_is_lchild(
		struct rbt_tree *tree, struct rbt_node *gparent,
		struct rbt_node *parent, struct rbt_node *node,
		summary_update_callback upd_cb)
{
	if (parent->rbt_left == node) {
		/*
		 * insert balance case 2:
		 * uncle black, node and parent same side
		 */
		__rbt_rotate_right(tree, gparent, upd_cb);
		__rbt_set_red(gparent);
		__rbt_set_black(parent);
	} else {
		/*
		 * insert balance case 3:
		 * uncle black, node and parent different sides
		 */
		__rbt_rotate_left(tree, parent, upd_cb);
		__rbt_rotate_right(tree, gparent, upd_cb);
		__rbt_set_red(gparent);
		__rbt_set_black(node);
	}
}

static void __rbt_insert_fixup_black_uncle_parent_is_rchild(
		struct rbt_tree *tree, struct rbt_node *gparent,
		struct rbt_node *parent, struct rbt_node *node,
		summary_update_callback upd_cb)
{
	if (parent->rbt_right == node) {
		/*
		 * insert balance case 2:
		 * uncle black, node and parent same side
		 */
		__rbt_rotate_left(tree, gparent, upd_cb);
		__rbt_set_red(gparent);
		__rbt_set_black(parent);
	} else {
		/*
		 * insert balance case 3:
		 * uncle black, node and parent different sides
		 */
		__rbt_rotate_right(tree, parent, upd_cb);
		__rbt_rotate_left(tree, gparent, upd_cb);
		__rbt_set_red(gparent);
		__rbt_set_black(node);
	}
}

void __rbt_insert_fixup(struct rbt_tree *tree, struct rbt_node *node,
			summary_update_callback upd_cb)
{
	struct rbt_node *gparent = NULL;
	struct rbt_node *parent = NULL;
	struct rbt_node *uncle = NULL;

	parent = rbt_parent(node);
	while ((parent != NULL) && __rbt_is_red(parent)) {
		gparent = rbt_parent(parent);
		if (gparent == NULL) {
			break;
		}
		if (gparent->rbt_left == parent) {
			uncle = gparent->rbt_right;
			if ((uncle != NULL) && __rbt_is_red(uncle)) {
				__rbt_insert_fixup_red_uncle(gparent, uncle,
							     &parent, &node);
			} else {
				__rbt_insert_fixup_black_uncle_parent_is_lchild(
				    tree, gparent, parent, node, upd_cb);
				break;
			}
		} else if (gparent->rbt_right == parent) {
			uncle = gparent->rbt_left;
			if ((uncle != NULL) && __rbt_is_red(uncle)) {
				__rbt_insert_fixup_red_uncle(gparent, uncle,
							     &parent, &node);
			} else {
				__rbt_insert_fixup_black_uncle_parent_is_rchild(
				    tree, gparent, parent, node, upd_cb);
				break;
			}
		} else {
			continue;
		}
	}

	__rbt_set_black(tree->root);
}

void rbt_subtree_summary_update(struct rbt_node *from,
				summary_update_callback upd_cb)
{
	if (upd_cb != NULL) {
		struct rbt_node *node = NULL;
		for (node = from; node != NULL; node = rbt_parent(node)) {
			if (!upd_cb(node)) {
				break;
			}
		}
	}
}

static void __rbt_insert(struct rbt_tree *tree, struct rbt_node *node,
			 struct rbt_node *parent, bool is_rchild,
			 summary_update_callback upd_cb)
{
	node->rbt_left = NULL;
	node->rbt_right = NULL;
	node->rbt_parent_color = RBT_COLOR_COLORLESS;

	if (parent == NULL) {
		tree->root = node;
		__rbt_set_parent(node, NULL);
		__rbt_set_black(node);
		if (upd_cb != NULL) {
			upd_cb(node);
		}
	} else {
		if (is_rchild) {
			parent->rbt_right = node;
		} else {
			parent->rbt_left = node;
		}

		__rbt_set_parent(node, parent);
		__rbt_set_red(node);

		rbt_subtree_summary_update(node, upd_cb);

		if (__rbt_is_red(parent)) {
			__rbt_insert_fixup(tree, node, upd_cb);
		}
	}
}

void rbt_insert(struct rbt_tree *tree, struct rbt_node *node,
		struct rbt_node *parent, bool is_rchild)
{
	__rbt_insert(tree, node, parent, is_rchild, NULL);
}

void aug_rbt_insert(struct aug_rbt_tree *aug_tree, struct rbt_node *node,
		    struct rbt_node *parent, bool is_rchild)
{
	__rbt_insert(&aug_tree->tree, node, parent, is_rchild, aug_tree->sumry_upd_cb);
}

static void __rbt_color_swap(struct rbt_node *x, struct rbt_node *y)
{
	/* swap the color of two nodes */
	if ((__rbt_color(x) | __rbt_color(y)) == (RBT_COLOR_RED | RBT_COLOR_BLACK)) {
		if (__rbt_is_red(x)) {
			__rbt_set_red(y);
			__rbt_set_black(x);
		} else {
			__rbt_set_red(x);
			__rbt_set_black(y);
		}
	}
}

static void __rbt_remove_swap(struct rbt_tree *tree, struct rbt_node *node,
			      struct rbt_node *sg,
			      summary_update_callback upd_cb)
{
	struct rbt_node *p = NULL;
	struct rbt_node *q = NULL;

	/* swap color */
	__rbt_color_swap(node, sg);

	/* relink parent */
	p = rbt_parent(node);
	q = rbt_parent(sg);
	__rbt_set_parent(sg, p);
	__rbt_set_parent(node, q);

	if (p != NULL) {
		/* node parent should not be null (scapegoat parent cannot be NULL) */
		if (p->rbt_left == node) {
			p->rbt_left = sg;
		} else {
			p->rbt_right = sg;
		}

		if (q->rbt_left == sg) {
			q->rbt_left = node;
		} else {
			q->rbt_right = node;
		}
	} else {
		/* node parent is null */
		if (q->rbt_left == sg) {
			q->rbt_left = node;
		} else {
			q->rbt_right = node;
		}
		tree->root = sg;
	}

	/* relink children */
	p = node->rbt_left;
	q = node->rbt_right;
	node->rbt_left = sg->rbt_left;
	node->rbt_right = sg->rbt_right;
	sg->rbt_left = p;
	sg->rbt_right = q;
	__rbt_set_parent(sg->rbt_left, sg);
	__rbt_set_parent(sg->rbt_right, sg);
	if (node->rbt_right != NULL) {
		__rbt_set_parent(node->rbt_right, node);
	}

	if (upd_cb != NULL) {
		(void)upd_cb(node);
		rbt_subtree_summary_update(rbt_parent(node), upd_cb);
		(void)upd_cb(sg);
	}
}

static void __rbt__remove_fixup_red_sib(struct rbt_tree *tree,
					struct rbt_node *parent,
					bool sib_is_lchild,
					struct rbt_node **sib,
					summary_update_callback upd_cb)
{
	/*
	 * remove fixup case 1:
	 * sibling red
	 */
	__rbt_set_black(*sib);
	__rbt_set_red(parent);
	if (sib_is_lchild) {
		__rbt_rotate_right(tree, parent, upd_cb);
		*sib = parent->rbt_left;
	} else {
		__rbt_rotate_left(tree, parent, upd_cb);
		*sib = parent->rbt_right;
	}
}

static bool __rbt_remove_fixup_black_sib(struct rbt_tree *tree,
					 bool sib_is_lchild,
					 struct rbt_node **parent,
					 struct rbt_node **node,
					 struct rbt_node *const *sib,
					 summary_update_callback upd_cb)
{
	struct rbt_node *sibling = NULL;
	bool is_fixup_over = true;

	sibling = *sib;
	if ((sibling->rbt_right != NULL) && __rbt_is_red(sibling->rbt_right)) {
		if (sib_is_lchild) {
			/* remove fixup case 2: sibling black, near nephew red */
			__rbt_set_black(sibling->rbt_right);
			__rbt_set_red(sibling);
			__rbt_rotate_left(tree, sibling, upd_cb);
			sibling = (*parent)->rbt_left;
			IMPOSSIBLE(sibling == NULL, "parent of black node certainly has two non-null child");
			__rbt_color_swap(*parent, sibling);
			IMPOSSIBLE(sibling->rbt_left == NULL, "above sibling left rotate ensures new left child non-null");
			__rbt_set_black(sibling->rbt_left);
			__rbt_rotate_right(tree, *parent, upd_cb);
		} else {
			/* remove fixup case 3: sibling black, far nephew red */
			__rbt_color_swap(*parent, sibling);
			__rbt_set_black(sibling->rbt_right);
			__rbt_rotate_left(tree, *parent, upd_cb);
		}
	} else if ((sibling->rbt_left != NULL) && __rbt_is_red(sibling->rbt_left)) {
		if (sib_is_lchild) {
			/* remove fixup case 3: sibling black, far nephew red */
			__rbt_color_swap(*parent, sibling);
			__rbt_set_black(sibling->rbt_left);
			__rbt_rotate_right(tree, *parent, upd_cb);
		} else {
			/* * remove fixup case 2: sibling black, near nephew red */
			__rbt_set_black(sibling->rbt_left);
			__rbt_set_red(sibling);
			__rbt_rotate_right(tree, sibling, upd_cb);
			sibling = (*parent)->rbt_right;
			IMPOSSIBLE(sibling == NULL, "parent of black node certainly has two non-null child");
			__rbt_color_swap(*parent, sibling);
			IMPOSSIBLE(sibling->rbt_right == NULL, "above sibling right rotate ensures new right child non-null");
			__rbt_set_black(sibling->rbt_right);
			__rbt_rotate_left(tree, *parent, upd_cb);
		}
	} else if (__rbt_is_red(*parent)) {
		/* remove fixup case 4: sibling black, parent red, no nephew */
		__rbt_set_black(*parent);
		__rbt_set_red(sibling);
	} else {
		/* remove fixup case 5: sibling black, parent black, no nephew */
		__rbt_set_red(sibling);
		*node = *parent;
		*parent = rbt_parent(*node);
		is_fixup_over = false;
	}

	return is_fixup_over;
}

static void __rbt_remove_fixup(struct rbt_tree *tree, struct rbt_node *node,
			       summary_update_callback upd_cb)
{
	struct rbt_node *parent = NULL;
	struct rbt_node *cur = NULL;
	struct rbt_node *sib = NULL;
	bool sib_is_lchild = false;

	/*
	 * A black node with no child certainly has one sibling node in a rb-tree.
	 * And each of its ancestor nodes certainly has two children. Therefore,
	 * variable sib cannot be NULL in following statements.
	 */
	cur = node;
	parent = rbt_parent(node);
	while ((cur != NULL) && (parent != NULL)) {
		if (parent->rbt_left == cur) {
			sib = parent->rbt_right;
			sib_is_lchild = false;
		} else {
			sib = parent->rbt_left;
			sib_is_lchild = true;
		}
		IMPOSSIBLE(sib == NULL, "parent of black node certainly has two non-null child");
		if (__rbt_is_red(sib)) {
			__rbt__remove_fixup_red_sib(tree, parent, sib_is_lchild,
						    &sib, upd_cb);
		}
		if (__rbt_remove_fixup_black_sib(tree, sib_is_lchild, &parent,
						 &cur, &sib, upd_cb)) {
			break;
		}
	}
}

/* Node to delete should be the ones that have one child at most. Therefore,
 * it should be used with __rbt_remove_swap to delete one node.
 */
static void __rbt_remove_node(struct rbt_tree *tree, struct rbt_node *node,
			      summary_update_callback upd_cb)
{
	struct rbt_node *successor = NULL;
	struct rbt_node *parent = NULL;

	parent = rbt_parent(node);
	if (parent == NULL) {
		/* case 1: node to delete is root */
		if (node->rbt_left != NULL) {
			successor = node->rbt_left;
		} else {
			successor = node->rbt_right;
		}
		tree->root = successor;
	} else if (parent->rbt_left == node) {
		/* case 2: node to delete is left child */
		if (node->rbt_left != NULL) {
			successor = node->rbt_left;
		} else {
			successor = node->rbt_right;
		}
		parent->rbt_left = successor;
	} else {
		/* case 3: node to delete is right child */
		if (node->rbt_left != NULL) {
			successor = node->rbt_left;
		} else {
			successor = node->rbt_right;
		}
		parent->rbt_right = successor;
	}

	if (successor != NULL) {
		__rbt_set_parent(successor, parent);
	}

	if (upd_cb != NULL) {
		(void)upd_cb(successor);
	}
	rbt_subtree_summary_update(parent, upd_cb);

	node->rbt_left = NULL;
	node->rbt_right = NULL;
	__rbt_set_colorless(node);
	__rbt_set_parent(node, NULL);
}

static void __rbt_remove(struct rbt_tree *tree, struct rbt_node *node,
		  summary_update_callback upd_cb)
{
	/* check node double remove or with invalid color status */
	if (!__rbt_is_colorless(node) && !(__rbt_is_red(node) && __rbt_is_black(node))) {
		if ((node->rbt_left != NULL) && (node->rbt_right != NULL)) {
			/* replace with its next who has no left child */
			__rbt_remove_swap(tree, node, rbt_next(node), upd_cb);
		}

		/* adjust the color of node to keep rbtree before node remove */
		if (__rbt_is_black(node)) {
			if (node->rbt_left != NULL) {
				__rbt_set_black(node->rbt_left);
			} else if (node->rbt_right != NULL) {
				__rbt_set_black(node->rbt_right);
			} else {
				__rbt_remove_fixup(tree, node, upd_cb);
			}
		}

		__rbt_remove_node(tree, node, upd_cb);
	}
}

void rbt_remove(struct rbt_tree *tree, struct rbt_node *node)
{
	__rbt_remove(tree, node, NULL);
}

void aug_rbt_remove(struct aug_rbt_tree *aug_tree, struct rbt_node *node)
{
	__rbt_remove(&aug_tree->tree, node, aug_tree->sumry_upd_cb);
}

void rbt_destroy_tree(struct rbt_tree *tree,
		      void (*const call_back)(struct rbt_node *))
{
	struct rbt_node *p = NULL;
	struct rbt_node *q = NULL;

	p = rbt_first(tree);
	while (p != NULL) {
		q = rbt_next(p);

		/* It's no need to swap node(__rbt_remove_swap) when remove
		 * node, as it remove the first of the tree each time
		 */
		__rbt_remove_node(tree, p, NULL);

		/* Essential operations of outer data struct such as release
		 * when remove one node.
		 */
		call_back(p);
		p = q;
	}
}

void rbt_node_init_from(struct rbt_node *new_node,
			const struct rbt_node *old_node,
			const struct rbt_node *parent)
{
	new_node->rbt_left = NULL;
	new_node->rbt_right = NULL;
	if (parent == NULL) {
		__rbt_set_parent(new_node, NULL);
	} else {
		__rbt_set_parent(new_node, parent);
	}
	if (__rbt_is_red(old_node)) {
		__rbt_set_red(new_node);
	} else {
		__rbt_set_black(new_node);
	}
}

void rbt_tree_init(struct rbt_tree *tree)
{
	tree->root = NULL;
}

void aug_rbt_tree_init(struct aug_rbt_tree *aug_tree)
{
	rbt_tree_init(&aug_tree->tree);
	aug_tree->sumry_upd_cb = NULL;
}

bool rbt_is_removable_node(const struct rbt_node *node)
{
	/*
	 * A removable rbtree node is one node who
	 * has valid color (red or black). It tells
	 * that one node is on a rbtree if it has
	 * valid color. As in some situation, it's
	 * dangerous to remove one invalid node from
	 * a tree if it's not on the tree actually.
	 *
	 * In addition, it helps to check whether a
	 * node is still on the tree(it existed on
	 * the tree before). But it cannot ensure whether
	 * a node is on a specified tree if has no other
	 * information.
	 */
	bool is_removable = true;
	if (__rbt_is_colorless(node) ||
	    (__rbt_is_red(node) && __rbt_is_black(node))) {
		is_removable = false;
	}
	return is_removable;
}
