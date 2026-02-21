/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2021. All rights reserved.
 * Description: Declaration for kernel rbtree
 * Author: Huawei OS Kernel Lab
 * Create: Thu Nov 15 21:27:14 2018
 */
#ifndef KLIBS_RBTREE_H
#define KLIBS_RBTREE_H

#include <hmkernel/errno.h>
#include <hmkernel/types.h>
#include <lib/utils.h>

#define rbt_entry(p, type, member) (container_of(p, type, member))

struct rbt_node {
	uptr_t rbt_parent_color;
	struct rbt_node *rbt_left;
	struct rbt_node *rbt_right;
} __attribute__((aligned(sizeof(uptr_t))));

struct rbt_tree {
	struct rbt_node *root;
};

/*
 * Three state of color for node of red-black tree:
 * red, black, and colorless.
 *
 * Colorless tells that the node has no red-black tree
 * to locate in right now.
 */
#define RBT_COLOR_MASK 		0x3UL
#define RBT_COLOR_RED 		0x2UL
#define RBT_COLOR_BLACK 	0x1UL
#define RBT_COLOR_COLORLESS 	0x0UL
#define __rbt_parent(p)		((struct rbt_node *)((p)->rbt_parent_color & (~RBT_COLOR_MASK)))
#define __rbt_color(p) 		((uptr_t)(p)->rbt_parent_color & RBT_COLOR_MASK)
#define __rbt_set_colorless(p) 	((p)->rbt_parent_color &= ~RBT_COLOR_MASK)
#define __rbt_is_colorless(p) 	(__rbt_color(p) == RBT_COLOR_COLORLESS)
#define __rbt_is_red(p) 	((__rbt_color(p) & RBT_COLOR_RED) != 0UL)
#define __rbt_is_black(p) 	((__rbt_color(p) & RBT_COLOR_BLACK) != 0UL)
#define __rbt_set_red(p) 	((p)->rbt_parent_color = (uptr_t)__rbt_parent(p) | RBT_COLOR_RED)
#define __rbt_set_black(p) 	((p)->rbt_parent_color = (uptr_t)__rbt_parent(p) | RBT_COLOR_BLACK)
#define __rbt_set_parent(p, v)	((p)->rbt_parent_color = __rbt_color(p) | (uptr_t)(v))

static inline void __rbt_rotate_left(struct rbt_tree *tree, struct rbt_node *node)
{
	struct rbt_node *parent = NULL;
	struct rbt_node *rchld = NULL;
	struct rbt_node *lgson = NULL;

	/* relink parent <==> right child */
	parent = __rbt_parent(node);
	rchld = node->rbt_right;
	if (parent != NULL) {
		if (parent->rbt_left == node) {
			parent->rbt_left = rchld;
		} else {
			parent->rbt_right = rchld;
		}
		if (rchld != NULL) {
			__rbt_set_parent(rchld, parent);
		}
	} else {
		tree->root = rchld;
		BUG_ON(rchld == NULL);
		__rbt_set_parent(rchld, parent);
	}

	/* relink node <==> left grandson */
	BUG_ON(rchld == NULL);
	lgson = rchld->rbt_left;
	node->rbt_right = lgson;
	if (lgson != NULL) {
		__rbt_set_parent(lgson, node);
	}

	/* relink node <==> rchild */
	rchld->rbt_left = node;
	__rbt_set_parent(node, rchld);
}

static inline void __rbt_rotate_right(struct rbt_tree *tree, struct rbt_node *node)
{
	struct rbt_node *parent = NULL;
	struct rbt_node *lchld = NULL;
	struct rbt_node *rgson = NULL;

	/* relink parent <==> left child */
	parent = __rbt_parent(node);
	lchld = node->rbt_left;
	if (parent != NULL) {
		if (parent->rbt_left == node) {
			parent->rbt_left = lchld;
		} else {
			parent->rbt_right = lchld;
		}
		if (lchld != NULL) {
			__rbt_set_parent(lchld, parent);
		}
	} else {
		tree->root = lchld;
		BUG_ON(lchld == NULL);
		__rbt_set_parent(lchld, parent);
	}

	/* relink node <==> right grandson */
	BUG_ON(lchld == NULL);
	rgson = lchld->rbt_right;
	node->rbt_left = rgson;
	if (rgson != NULL) {
		__rbt_set_parent(rgson, node);
	}

	/* relink node <==> lchild */
	lchld->rbt_right = node;
	__rbt_set_parent(node, lchld);
}

static inline void __rbt_insert_fixup_red_uncle(struct rbt_node *grand,
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
	__rbt_set_red(grand);
	*node = grand;
	*parent = __rbt_parent(*node);
}

static inline void __rbt_insert_fixup_black_uncle(struct rbt_tree *tree,
						  struct rbt_node *grand,
						  struct rbt_node *parent,
						  struct rbt_node *node,
						  bool parent_is_lchild)
{
	if (parent_is_lchild) {
		if (parent->rbt_left == node) {
			/*
			 * insert balance case 2:
			 * uncle black, node and parent same side
			 */
			__rbt_rotate_right(tree, grand);
			__rbt_set_red(grand);
			__rbt_set_black(parent);
		} else {
			/*
			 * insert balance case 3:
			 * uncle black, node and parent different sides
			 */
			__rbt_rotate_left(tree, parent);
			__rbt_rotate_right(tree, grand);
			__rbt_set_red(grand);
			__rbt_set_black(node);
		}
	} else {
		if (parent->rbt_right == node) {
			/*
			 * insert balance case 2:
			 * uncle black, node and parent same side
			 */
			__rbt_rotate_left(tree, grand);
			__rbt_set_red(grand);
			__rbt_set_black(parent);
		} else {
			/*
			 * insert balance case 3:
			 * uncle black, node and parent different sides
			 */
			__rbt_rotate_right(tree, parent);
			__rbt_rotate_left(tree, grand);
			__rbt_set_red(grand);
			__rbt_set_black(node);
		}
	}
}

static inline void __rbt_insert_fixup(struct rbt_tree *tree, struct rbt_node *node)
{
	struct rbt_node *grand = NULL;
	struct rbt_node *parent = NULL;
	struct rbt_node *uncle = NULL;

	parent = __rbt_parent(node);
	while (parent != NULL && __rbt_is_red(parent)) {
		grand = __rbt_parent(parent);
		if (grand == NULL) {
			break;
		}
		if (grand->rbt_left == parent) {
			uncle = grand->rbt_right;
			if (uncle != NULL && __rbt_is_red(uncle)) {
				__rbt_insert_fixup_red_uncle(grand, uncle,
							     &parent, &node);
			} else {
				__rbt_insert_fixup_black_uncle(tree, grand,
							       parent, node, true);
				break;
			}
		} else if (grand->rbt_right == parent) {
			uncle = grand->rbt_left;
			if (uncle != NULL && __rbt_is_red(uncle)) {
				__rbt_insert_fixup_red_uncle(grand, uncle,
							     &parent, &node);
			} else {
				__rbt_insert_fixup_black_uncle(tree, grand,
							       parent, node, false);
				break;
			}
		} else {
			/* Do nothing */
		}
	}

	__rbt_set_black(tree->root);
}

static inline void __rbt_color_swap(struct rbt_node *x, struct rbt_node *y)
{
	/* swap the color of two nodes */
	if ((__rbt_color(x) | __rbt_color(y)) == (RBT_COLOR_RED | RBT_COLOR_BLACK)) {
		if (__rbt_is_red(x)) {
			__rbt_set_black(x);
			__rbt_set_red(y);
		} else {
			__rbt_set_black(y);
			__rbt_set_red(x);
		}
	}
}

static inline void __rbt_remove_swap(struct rbt_tree *tree, struct rbt_node *node, struct rbt_node *sg)
{
	struct rbt_node *m = NULL;
	struct rbt_node *n = NULL;

	/* swap color */
	__rbt_color_swap(node, sg);

	/* relink parent */
	m = __rbt_parent(node);
	n = __rbt_parent(sg);
	__rbt_set_parent(sg, m);
	__rbt_set_parent(node, n);

	if (m != NULL) {
		/* node parent should not be null (scapegoat parent cannot be NULL) */
		if (m->rbt_left == node) {
			m->rbt_left = sg;
		} else {
			m->rbt_right = sg;
		}

		if (n->rbt_left == sg) {
			n->rbt_left = node;
		} else {
			n->rbt_right = node;
		}
	} else {
		/* node parent is null */
		if (n->rbt_left == sg) {
			n->rbt_left = node;
		} else {
			n->rbt_right = node;
		}
		tree->root = sg;
	}

	/* relink children */
	m = node->rbt_left;
	n = node->rbt_right;
	node->rbt_left = sg->rbt_left;
	node->rbt_right = sg->rbt_right;
	sg->rbt_left = m;
	sg->rbt_right = n;
	__rbt_set_parent(sg->rbt_left, sg);
	__rbt_set_parent(sg->rbt_right, sg);
	if (node->rbt_right != NULL) {
		__rbt_set_parent(node->rbt_right, node);
	}
}

static inline void __rbt__remove_fixup_red_sib(struct rbt_tree *tree,
					       struct rbt_node *parent,
					       bool sib_is_lchld,
					       struct rbt_node **sib)
{
	/*
	 * remove fixup case 1:
	 * sibling red
	 */
	__rbt_set_black(*sib);
	__rbt_set_red(parent);
	if (!sib_is_lchld) {
		__rbt_rotate_left(tree, parent);
		*sib = parent->rbt_right;
	} else {
		__rbt_rotate_right(tree, parent);
		*sib = parent->rbt_left;
	}
}

static inline bool __rbt_remove_fixup_black_sib(struct rbt_tree *tree,	bool sib_is_lchild,
						struct rbt_node **parent, struct rbt_node **node,
						struct rbt_node **sib)
{
	struct rbt_node *sibl = NULL;
	bool is_fixup_over = true;

	sibl = *sib;
	if (sibl->rbt_right != NULL && __rbt_is_red(sibl->rbt_right)) {
		if (sib_is_lchild) {
			/*
			 * remove fixup case 2:
			 * sibling black, near nephew red
			 */
			__rbt_set_black(sibl->rbt_right);
			__rbt_set_red(sibl);
			__rbt_rotate_left(tree, sibl);
			sibl = (*parent)->rbt_left;
			BUG_ON(sibl == NULL);
			__rbt_color_swap(*parent, sibl);
			BUG_ON(sibl->rbt_left == NULL);
			__rbt_set_black(sibl->rbt_left);
			__rbt_rotate_right(tree, *parent);
		} else {
			/*
			 * remove fixup case 3:
			 * sibling black, far nephew red
			 */
			__rbt_color_swap(*parent, sibl);
			__rbt_set_black(sibl->rbt_right);
			__rbt_rotate_left(tree, *parent);
		}
	} else if (sibl->rbt_left != NULL && __rbt_is_red(sibl->rbt_left)) {
		if (sib_is_lchild) {
			/*
			 * remove fixup case 3:
			 * sibling black, far nephew red
			 */
			__rbt_color_swap(*parent, sibl);
			__rbt_set_black(sibl->rbt_left);
			__rbt_rotate_right(tree, *parent);
		} else {
			/*
			 * remove fixup case 2:
			 * sibling black, near nephew red
			 */
			__rbt_set_black(sibl->rbt_left);
			__rbt_set_red(sibl);
			__rbt_rotate_right(tree, sibl);
			sibl = (*parent)->rbt_right;
			BUG_ON(sibl == NULL);
			__rbt_color_swap(*parent, sibl);
			BUG_ON(sibl->rbt_right == NULL);
			__rbt_set_black(sibl->rbt_right);
			__rbt_rotate_left(tree, *parent);
		}
	} else if (__rbt_is_red(*parent)) {
		/*
		 * remove fixup case 4:
		 * sibling black, parent red, no nephew
		 */
		__rbt_set_black(*parent);
		__rbt_set_red(sibl);
	} else {
		/*
		 * remove fixup case 5:
		 * sibling black, parent black, no nephew
		 */
		__rbt_set_red(sibl);
		*node = *parent;
		*parent = __rbt_parent(*node);
		is_fixup_over = false;
	}

	return is_fixup_over;
}

static inline int __rbt_remove_fixup(struct rbt_tree *tree, struct rbt_node *node)
{
	struct rbt_node *parent = NULL;
	struct rbt_node *cur = NULL;
	struct rbt_node *sib = NULL;
	bool sib_is_lchild = false;
	int rc = E_HM_OK;

	/*
	 * A black node with no child certainly has one sibling node in a rb-tree.
	 * And each of its ancestor nodes certainly has two children. Therefore,
	 * variable sib cannot be NULL in following statements.
	 */
	cur = node;
	parent = __rbt_parent(node);
	while (cur != NULL && parent != NULL) {
		if (parent->rbt_left == cur) {
			sib = parent->rbt_right;
			sib_is_lchild = false;
		} else {
			sib = parent->rbt_left;
			sib_is_lchild = true;
		}
		if (sib != NULL) {
			if (__rbt_is_red(sib)) {
				__rbt__remove_fixup_red_sib(tree, parent, sib_is_lchild, &sib);
			}
			if (__rbt_remove_fixup_black_sib(tree, sib_is_lchild, &parent, &cur, &sib)) {
				break;
			}
		} else {
			rc = E_HM_NOOBJ;
			break;
		}
	}

	return rc;
}

/* Node to delete should be the ones that have one child at most. Therefore,
 * it should be used with __rbt_remove_swap to delete one node.
 */
static inline void __rbt_remove_node(struct rbt_tree *tree, struct rbt_node *node)
{
	struct rbt_node *inheritor = NULL;
	struct rbt_node *parent = NULL;

	parent = __rbt_parent(node);
	if (parent == NULL) {
		/* case 1: node to delete is root */
		if (node->rbt_left != NULL) {
			inheritor = node->rbt_left;
		} else {
			inheritor = node->rbt_right;
		}
		tree->root = inheritor;
	} else if (parent->rbt_left == node) {
		/* case 2: node to delete is left child */
		if (node->rbt_left != NULL) {
			inheritor = node->rbt_left;
		} else {
			inheritor = node->rbt_right;
		}
		parent->rbt_left = inheritor;
	} else {
		/* case 3: node to delete is right child */
		if (node->rbt_left != NULL) {
			inheritor = node->rbt_left;
		} else {
			inheritor = node->rbt_right;
		}
		parent->rbt_right = inheritor;
	}

	if (inheritor != NULL) {
		__rbt_set_parent(inheritor, parent);
	}

	node->rbt_left = NULL;
	node->rbt_right = NULL;
	__rbt_set_colorless(node);
	__rbt_set_parent(node, NULL);
}

static inline struct rbt_node *rbt_prev(struct rbt_node *node)
{
	struct rbt_node *p = node->rbt_left;
	struct rbt_node *c = node;
	/* search left sub tree */
	while (p != NULL) {
		if (p->rbt_right != NULL) {
			p = p->rbt_right;
		} else {
			break;
		}
	}

	/* search upward the tree */
	if (p == NULL) {
		p = __rbt_parent(c);
		while (p != NULL) {
			if (p->rbt_right == c) {
				break;
			} else {
				c = p;
				p = __rbt_parent(p);
			}
		}
	}

	return p;
}

static inline struct rbt_node *rbt_next(struct rbt_node *node)
{
	struct rbt_node *p = node->rbt_right;
	struct rbt_node *c = node;
	/* search right sub tree */
	while (p != NULL) {
		if (p->rbt_left != NULL) {
			p = p->rbt_left;
		} else {
			break;
		}
	}

	/* search upward the tree */
	if (p == NULL) {
		p = __rbt_parent(c);
		while (p != NULL) {
			if (p->rbt_left == c) {
				break;
			} else {
				c = p;
				p = __rbt_parent(p);
			}
		}
	}

	return p;
}

static inline struct rbt_node *rbt_first(struct rbt_tree *tree)
{
	struct rbt_node *p = tree->root;
	while (p != NULL) {
		if (p->rbt_left != NULL) {
			p = p->rbt_left;
		} else {
			break;
		}
	}

	return p;
}

static inline struct rbt_node *rbt_last(struct rbt_tree *tree)
{
	struct rbt_node *p = tree->root;
	while (p != NULL) {
		if (p->rbt_right != NULL) {
			p = p->rbt_right;
		} else {
			break;
		}
	}

	return p;
}

static inline void rbt_insert(struct rbt_tree *tree, struct rbt_node *node,
			      struct rbt_node *parent, bool is_rchild)
{
	node->rbt_left = NULL;
	node->rbt_right = NULL;

	if (parent != NULL) {
		if (is_rchild) {
			parent->rbt_right = node;
		} else {
			parent->rbt_left = node;
		}

		__rbt_set_red(node);
		__rbt_set_parent(node, parent);

		if (__rbt_is_red(parent)) {
			__rbt_insert_fixup(tree, node);
		}
	} else {
		tree->root = node;
		__rbt_set_parent(node, NULL);
		__rbt_set_black(node);
	}
}

static inline int rbt_remove(struct rbt_tree *tree, struct rbt_node *node)
{
	int rc = E_HM_OK;
	/* check node double remove */
	if (__rbt_is_colorless(node)) {
		rc = E_HM_INVAL;
	}

	/* check node with invalid color status */
	if ((rc == E_HM_OK) && __rbt_is_red(node) && __rbt_is_black(node)) {
		rc = E_HM_INVAL;
	}

	if ((rc == E_HM_OK) && (node->rbt_left != NULL) && (node->rbt_right != NULL)) {
		/* replace with its next who has no left child */
		__rbt_remove_swap(tree, node, rbt_next(node));
	}

	/* adjust the color of node to keep rbtree before node remove */
	if ((rc == E_HM_OK) && __rbt_is_black(node)) {
		if (node->rbt_right != NULL) {
			__rbt_set_black(node->rbt_right);
		} else if (node->rbt_left != NULL) {
			__rbt_set_black(node->rbt_left);
		} else {
			rc = __rbt_remove_fixup(tree, node);
		}
	}

	if (rc == E_HM_OK) {
		__rbt_remove_node(tree, node);
	}

	return rc;
}

static inline void rbt_destroy_tree(struct rbt_tree *tree, void (*call_back)(struct rbt_node *))
{
	struct rbt_node *m = NULL;
	struct rbt_node *n = NULL;

	m = rbt_first(tree);
	while (m != NULL) {
		n = rbt_next(m);

		/* It's no need to swap node(__rbt_remove_swap) when remove
		 * node, as it remove the first of the tree each time
		 */
		__rbt_remove_node(tree, m);

		/* Essential operations of outer data struct such as release
		 * when remove one node.
		 */
		call_back(m);
		m = n;
	}
}

static inline void rbt_init(struct rbt_tree *tree)
{
	tree->root = NULL;
}

#define DEFINE_rbt_search(name, type, node_mem, key_mem)		\
static type *								\
name##_rbt_search(struct rbt_tree *tree,				\
		  typeof(((type *)0)->key_mem) key,			\
		  struct rbt_node **pparent_node)			\
{									\
	struct rbt_node *_node = tree->root;				\
	struct rbt_node *_parent = NULL;				\
	/* tree is NULL */						\
	if (_node == NULL)						\
		_parent = NULL;						\
	while (_node != NULL) {						\
		if (rbt_entry(_node, type, node_mem)->key_mem == key) {	\
			_parent = __rbt_parent(_node);			\
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
	if (pparent_node != NULL)					\
		*pparent_node = _parent;				\
	if (_node != NULL)						\
		return rbt_entry(_node, type, node_mem);		\
	return NULL;							\
}									\

#define rbt_first_entry(tree, type, member)				\
	((tree)->root == NULL ? NULL : rbt_entry(rbt_first(tree), type, member))

#define rbt_for_each_entry_safe(ent, nn, tree, type, member)		\
	for ((ent) = rbt_first_entry(tree, type, member),		\
	     (nn) = ((ent) == NULL ? NULL :                             \
		     rbt_next((struct rbt_node *)(uptr_t)&(ent)->member));	\
	     (ent) != NULL;						\
	     (ent) = ((nn) == NULL ? NULL : rbt_entry(nn, type, member)),	\
	     (nn) = ((ent) == NULL ? NULL :                             \
		     rbt_next((struct rbt_node *)(uptr_t)&(ent)->member)))

#endif
