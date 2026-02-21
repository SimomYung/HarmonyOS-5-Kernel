README file for Hongmeng Red-black tree
========================================

Red-black tree is a type of self-balancing binary search tree, which stores
data by sortable keys.

In comparation with AVL tree, RB tree provides better performance for insertion and
deleteion, though slighly slower for search.

HM implementation for RB tree
---------------------------------------
HM's rbtree implementation lives in file "rbtree.c". To use it,

	"#include "rbtree.h""

Other than classical implementations of data structures, each instance of
rbt_nodt_t in  HM RB tree embeds itself in the data structure it orgnizes.
In addition, HM RB tree merely provides basic interfaces for tree and other
operations such as key compare and data synchronize are all left up to the
user.

Creating a new RB tree
---------------------
Data node for RB tree:

	struct example_type {
		struct rbt_node node;
		int key;
	};

User can get instance of the data struct "example_type" by one pointer of
struct rbt_node using rbt_entry(p, type, member).


All nodes of each RB tree is orgnized by rbt_tree stucture, which will be
initialized as empty:

	struct rbt_tree example_tree = RBT_ROOT;

Insert
------
To insert a new node, user should find its position in the RB tree first,
and then insert it into the RB tree.

Example:

	int example_insert(struct rbt_tree *tree, struct example_type *data)
	{
		int cmp;
		struct example_type *cur;

		struct rbt_node *p = tree->root;
		while (p != NULL) {
			cur = rbt_entry(p, struct exmaple_type, node);
			cmp = data->key - cur->key;
			if (cmp == 0) {
				return -1;
			} else if (cmp > 0) {
				if (p->rbt_left != NULL) {
					p = p->rbt_left;
					continue;
				} else {
					p->rbt_left = &data.node;
					break;
				}
			} else {
				if (p->rbt_right != NULL) {
					p = p->rbt_right;
					continue;
				} else {
					p->rbt_right = &data.node;
					break;
				}
			}
		}
		rbt_insert(tree, p, &data.node);
		return 0;
	}

Search
------
As a binary search tree, use can easy to search one node from the root.

Example:

	struct example_type *example_search(struct rbt_tree *tree, int key)
	{
		int cmp;
		struct example_type *data;

		struct rbt_node *p = tree->root;
		while (p != NULL) {
			data = rbt_entry(p, struct exmaple_type, node);
			cmp = data->key - key;
			if (cmp == 0) {
				return cur;
			} else if (cmp > 0) {
				p = p->rbt_left;
			} else {
				p = p->rbt_right;
			}
		}

		return NULL;
	}

Delete
------
User can delete one node after search it from the tree using above search function.

Example:

	void example_type example_delete(struct rbt_tree *tree, int key)
	{
		struct example_type *data = example_search(tree, key);

		if (data != NULL) {
			rbt_remove(tree, &data->node);
			example_free(data);
		}
	}

Note: rbt_remove removes node with balancing action, which is useless and inefficient
when destructing the whole tree. rbt_destroy_tree will provide better performance to
destruct the whole tree.

Iterating
------------------------------------------------------------------
HM RB tree provides four functions for tree iterating:

	struct rbt_node* rbt_first(struct rbt_tree *tree);~
	struct rbt_node* rbt_last(struct rbt_tree *tree);~
	struct rbt_node* rbt_prev(struct rbt_node *node);~
	struct rbt_node* rbt_next(struct rbt_node *node);~

User can use rbt_first/rbt_last to get the start/end of the tree, then
use rbt_next/rbt_prev to iterate the tree in ascending/descending order.

Example:
	rbt_nodt_t *p;
	for (p = rbt_first(&example_tree); p != NULL; p = rbt_next(p)) {
		printf("key=%d\n", rbt_entry(p, struct example_type, node)->key);
	}
