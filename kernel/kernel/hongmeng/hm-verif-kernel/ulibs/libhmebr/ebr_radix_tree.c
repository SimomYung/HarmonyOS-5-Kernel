/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Implementation for ebr-safe radix tree
 * Author: Huawei OS Kernel Lab
 * Create: Tue Oct 20 19:40:06 2020
 */

#include "../libalgo/radixtree_internal.h"

#include <libhmebr/ebr.h>
#include <libhmebr/ebr_radix_tree.h>
#include <libhwsecurec/securec.h>
#include <hongmeng/errno.h>
#include <libstrict/strict.h>

struct ebr_radix_tree_node {
	struct ebr_entry entry;
	struct radix_tree_node node;
};

size_t ebr_radix_tree_node_size(void)
{
	return sizeof(struct ebr_radix_tree_node);
}

#define max(a, b) ({				\
		   __typeof__ (a) _a = (a);	\
		   __typeof__ (b) _b = (b);	\
		   _a > _b ? _a : _b;})

static inline void set_node_tag(struct radix_tree_node *node,
				unsigned int tag,
				unsigned int offset)
{
	NOFAIL(raw_bitmap_set_bit(node->tags[tag],
				  RADIX_TREE_SLOTS_SIZE, offset));
}

static inline void clear_node_tag(struct radix_tree_node *node,
				  unsigned int tag,
				  unsigned int offset)
{
	NOFAIL(raw_bitmap_clear_bit(node->tags[tag],
				    RADIX_TREE_SLOTS_SIZE, offset));
}

static inline int test_node_tag(const struct radix_tree_node *node,
				unsigned int tag,
				unsigned int offset)
{
	return raw_bitmap_test_bit(node->tags[tag],
				   RADIX_TREE_SLOTS_SIZE, offset);
}
/* search scope is bits [start+1, last] */
static inline unsigned int get_node_tag_next(const struct radix_tree_node *node,
					     unsigned int tag,
					     unsigned int start)
{
	return raw_bitmap_find_next_one(node->tags[tag],
					RADIX_TREE_SLOTS_SIZE, start);
}

/* search scope is bits [start, last] */
static inline unsigned int search_node_tag(const struct radix_tree_node *node,
					   unsigned int tag,
					   unsigned int start)
{
	unsigned int ret;
	if (test_node_tag(node, tag, start) == 1) {
		ret = start;
	} else {
		ret = get_node_tag_next(node, tag, start);
	}
	return ret;
}

static bool is_node_tagged(const struct radix_tree_node *node,
				  unsigned int tag)
{
	bool ret = false;
	unsigned int i = 0U;

	for (i = 0; i < BITS_TO_LONGLONGS(RADIX_TREE_SLOTS_SIZE); i++) {
		if (node->tags[tag][i] != 0U) {
			ret = true;
			break;
		}
	}
	return ret;
}

static void
clear_node_tags(struct radix_tree_node *node,
		unsigned int tag, unsigned int offset)
{
	unsigned int curr_offset = offset;
	while (node != NULL) {
		clear_node_tag(node, tag, curr_offset);
		if (is_node_tagged(node, tag)) {
			break;
		}

		curr_offset = node->offset;
		node = node->parent;
	}
}

static void set_node_tags(const struct radix_tree_node *old,
			  struct radix_tree_node *new)
{
	for (unsigned int tag = 0; tag < __NR_TAGS; tag++) {
		if (is_node_tagged(old, tag)) {
			set_node_tag(new, tag, 0);
		}
	}
}

/*
 * ebr_radix_tree_init: initialize ebr-safe radix tree,
 * @root and @es must not be null.
 *
 * @param	eroot	[I] Radix tree guarded by ebr @es
 * @param	es	[I] Pointer of an EBR instance
 *
 * @return	Returns the result of initialization, E_HM_OK for success.
 * 		On any error, return a negative integer.
 */
int ebr_radix_tree_init(struct ebr_radix_tree_root *eroot, struct epoch_set *es)
{
	int ret = E_HM_OK;
	if (eroot == NULL || es == NULL) {
		ret = E_HM_INVAL;
	} else {
		eroot->eset = es;
		eroot->root.rnode = NULL;
		eroot->root.has_free = true;
	}

	return ret;
}

static unsigned long get_height_max_index(unsigned int height)
{
	unsigned int index = (height + 1U) * RADIX_TREE_SHIFT_SIZE;
	return ~0UL >> (8UL * sizeof(unsigned long) - (unsigned long)index);
}

static unsigned long get_node_max_index(const struct radix_tree_node *node)
{
	return get_height_max_index(node->height);
}

static void set_node_bit_upward(struct radix_tree_root *root,
				struct radix_tree_node *node,
				unsigned int offset)
{
	unsigned int curr_offset = offset;
	struct radix_tree_node *parent = NULL;

	while (node != NULL) {
		parent = node->parent;

		int err = raw_bitmap_set_bit(node->bitmap,
					     RADIX_TREE_SLOTS_SIZE, curr_offset);
		BUG_ON(err != 0);

		if (!raw_bitmap_is_full(node->bitmap, RADIX_TREE_SLOTS_SIZE)) {
			break;
		}

		curr_offset = node->offset;
		node = parent;
	}

	if (node == NULL) {
		root->has_free = false;
	}
}

static void clear_node_bit_upward(struct radix_tree_root *root,
				  struct radix_tree_node *node,
				  unsigned int offset)
{
	unsigned int curr_offset = offset;

	while (node != NULL) {
		int err = 0;

		if (raw_bitmap_test_bit(node->bitmap, RADIX_TREE_SLOTS_SIZE,
					curr_offset) == 0) {
			break;
		}

		err = raw_bitmap_clear_bit(node->bitmap,
					   RADIX_TREE_SLOTS_SIZE,
					   curr_offset);
		BUG_ON(err != 0);

		curr_offset = node->offset;
		node = node->parent;
	}

	if (node == NULL) {
		root->has_free = true;
	}
}

static void free_node(struct radix_tree_node *node, const struct epoch_set *es)
{
	enum ebr_mcache_type mcache_type = EBR_MCACHE_DEFAULT;
	if (es != NULL) {
		mcache_type = es->mcache_type;
	}
	ebr_radix_tree_node_free((void *)node, mcache_type);
}

static __attribute__((unused)) void do_radix_tree_reclaim_node(const struct ebr_entry *entry)
{
	struct ebr_radix_tree_node *enode = NULL;

	enode = container_of(entry, struct ebr_radix_tree_node, entry);
	free_node(enode, NULL);
}

static void
do_ebr_radix_tree_destroy(struct ebr_radix_tree_root *eroot, struct radix_tree_root *root,
			  struct radix_tree_node *node)
{
	unsigned int offset = 0U;
	bool loop_stop = false;
	while (!loop_stop) {
		struct radix_tree_node *child = node->slots[offset];
		if ((child != NULL) && (node->height > 0U)) {
			node = child;
			offset = 0U;
			continue;
		}

		offset++;
		while (offset == RADIX_TREE_SLOTS_SIZE) {
			struct radix_tree_node *parent = NULL;
			struct ebr_radix_tree_node *enode = NULL;
			void *rnode = NULL;

			offset = node->offset;
			parent = node->parent;

			/* deferring node reclamation through call_ebr */
			enode = container_of(node, struct ebr_radix_tree_node,
					     node);

			rnode = root->rnode;
			if (rnode == node) {
				ebr_assign_pointer(root->rnode, NULL);
			} else {
				ebr_assign_pointer(parent->slots[offset], NULL);
			}
			(void)synchronize_ebr(eroot->eset);
			free_node(enode, eroot->eset);

			if (rnode == node) {
				root->has_free = true;
				loop_stop = true;
				break;
			}
			node = parent;
			offset += 1;
		}
	}
}

/*
 * ebr_radix_tree_destory: destroy ebr-safe radix tree @eroot,
 * lock must be held before destroy, and also make sure items will be
 * taken care of explicitly since ebr_radix_tree_destroy only reclaims
 * tree nodes not the items referenced by its pointers stored in the tree.
 *
 * @param	eroot	[I] Radix tree guarded by ebr
 *
 * @return	Returns nothing
 */
void ebr_radix_tree_destroy(struct ebr_radix_tree_root *eroot)
{
	struct radix_tree_node *node = NULL;
	struct radix_tree_root *root = NULL;

	if (eroot != NULL) {
		root = &eroot->root;
		if (root->rnode != NULL) {
			node = root->rnode;
			do_ebr_radix_tree_destroy(eroot, root, node);
		}
	}
}

/* radix tree node allocation */
static struct radix_tree_node *
alloc_node(struct radix_tree_node *parent, unsigned int offset,
	   unsigned int height, const struct epoch_set *es)
{
	struct ebr_radix_tree_node *new_node = NULL;
	struct radix_tree_node *ret_node = NULL;

	enum ebr_mcache_type mcache_type = EBR_MCACHE_DEFAULT;
	if (es != NULL) {
		mcache_type = es->mcache_type;
	}
	if (offset < RADIX_TREE_SLOTS_SIZE) {
		new_node = (struct ebr_radix_tree_node *)ebr_radix_tree_node_zmalloc(mcache_type);
		if (new_node != NULL) {
			new_node->node.offset = offset;
			new_node->node.height = height;
			new_node->node.parent = parent;
			new_node->node.count = 0;

			if (parent != NULL) {
				ebr_assign_pointer(parent->slots[offset], &new_node->node);
				parent->count++;
			}

			ret_node = &new_node->node;
		}
	}
	return ret_node;
}

static void *
do_radix_tree_growth(struct radix_tree_root *root, unsigned long start, const struct epoch_set *es)
{
	struct radix_tree_node *node = (struct radix_tree_node *)root->rnode;
	struct radix_tree_node *ret_node = NULL;
	int err = E_HM_OK;
	unsigned int max_height = 0U;

	if (node != NULL) {
		max_height = node->height;
	}

	while (start > get_height_max_index(max_height)) {
		max_height += 1U;
		if (node->count != 0U) {
			struct radix_tree_node *top_node = NULL;
			struct radix_tree_node *new_node = NULL;

			new_node = alloc_node(NULL, 0U, max_height, es);
			if (new_node == NULL) {
				err = E_HM_NOMEM;
				break;
			}

			top_node = node;
			ebr_assign_pointer(new_node->slots[0], top_node);
			new_node->count++;
			if (raw_bitmap_is_full(top_node->bitmap,
					       RADIX_TREE_SLOTS_SIZE)) {
				(void)raw_bitmap_set_bit(new_node->bitmap,
							 RADIX_TREE_SLOTS_SIZE,
							 0U);
			}
			ebr_assign_pointer(top_node->parent, new_node);
			set_node_tags(top_node, new_node);
			ebr_assign_pointer(root->rnode, new_node);
			node = (struct radix_tree_node *)root->rnode;
		}
	}

	if (err == E_HM_OK) {
		if (node->count == 0U) {
			node->height = max_height;
		}

		root->has_free = true;
		ret_node = node;
	}
	return ret_node;
}

static unsigned int
get_next_level_offset(const struct radix_tree_node *node,
		      unsigned long index)
{
	unsigned long off = (index >> (node->height * RADIX_TREE_SHIFT_SIZE)) &
			    RADIX_TREE_MASK;
	return (unsigned int)off;
}

static unsigned long
get_next_level_index(const struct radix_tree_node *node,
		     unsigned int offset)
{
	return (unsigned long)offset << (node->height * RADIX_TREE_SHIFT_SIZE);
}

static void rollback_node_allocation(struct radix_tree_root *root,
				     struct radix_tree_node *node,
				     struct epoch_set *es)
{
	struct radix_tree_node *parent = NULL;
	struct radix_tree_node *node_tmp = node;

	BUG_ON(root == NULL || node_tmp == NULL);

	if (node_tmp->count == 0U) {
		for (;;) {
			struct ebr_radix_tree_node *enode = NULL;
			unsigned int offset = 0;

			parent = node_tmp->parent;
			offset = node_tmp->offset;

			/* deferring node reclamation through call_ebr */
			enode = container_of(node_tmp, struct ebr_radix_tree_node, node);
			if (parent != NULL) {
				ebr_assign_pointer(parent->slots[offset], NULL);
			} else {
				ebr_assign_pointer(root->rnode, NULL);
			}
			(void)synchronize_ebr(es);
			free_node(enode, es);
			if (parent == NULL) {
				root->has_free = true;
				break;
			}
			parent->count--;
			if (parent->count != 0U) {
				break;
			}

			node_tmp = parent;
		}
	}
}

#define publish_slot(root, node, offset, item) \
do {							\
	set_node_bit_upward(root, node, offset);	\
	ebr_assign_pointer((node)->slots[(offset)], item);	\
	(node)->count++;					\
} while (0)

static int
do_item_insertion(struct radix_tree_root *root,
		  unsigned long index, void *item,
		  struct epoch_set *es, struct radix_tree_node *child)
{
	int err = E_HM_OK;
	unsigned int offset = 0U;
	unsigned int height;
	struct radix_tree_node *node = NULL;
	struct radix_tree_node *child_tmp = child;

	height = child_tmp->height + 1U;
	while (height != 0U) {
		height -= 1U;
		if (child_tmp == NULL) {
			child_tmp = alloc_node(node, offset, height, es);
			if (child_tmp == NULL) {
				rollback_node_allocation(root, node, es);
				err = E_HM_NOMEM;
				break;
			}
		}
		node = child_tmp;
		offset = get_next_level_offset(node, index);
		/* for inner nodes, each bit(1) for every slot means its
		 * child node is full; for leaf node, each bit(1) tells
		 * whether there is an item in the slot
		 */
		if (raw_bitmap_test_bit(node->bitmap, RADIX_TREE_SLOTS_SIZE,
					offset) != 0) {
			err = E_HM_OBJEXIST;
			break;
		}
		child_tmp = node->slots[offset];
	}

	if (err == E_HM_OK) {
		publish_slot(root, node, offset, item);
	}

	return err;
}

/*
 * ebr_radix_tree_insert_item: insert an @item into @eroot, and @item uses
 * @index as its key, although @eroot is guarded by EBR, still make sure lock
 * must be held before entering this function.
 *
 * @param	eroot	[I] Radix tree guarded by ebr
 * @param	index	[I] The key associated with item
 * @param	item	[I] Pointer of the item ready to be inserted
 *
 * @return	Returns the result of insertion. E_HM_OK for success.
 * 		On any error, return a negative integer.
 */
int ebr_radix_tree_insert_item(struct ebr_radix_tree_root *eroot,
			       unsigned long index, void *item)
{
	struct radix_tree_root *root = NULL;
	int ret = E_HM_OK;
	bool is_empty = false;

	if ((eroot == NULL) || (item == NULL)) {
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		root = &eroot->root;
		if (root->rnode == NULL) {
			is_empty = true;
			root->rnode = alloc_node(NULL, 0, 0, eroot->eset);
			if (root->rnode == NULL) {
				ret = E_HM_NOMEM;
			}
		}
	}

	if (ret == E_HM_OK) {
		struct radix_tree_node *child = root->rnode;

		if (index > get_node_max_index(child)) {
			child = do_radix_tree_growth(root, index, eroot->eset);
			if (child == NULL) {
				ret = E_HM_NOMEM;
			}
		}
		if (ret == E_HM_OK) {
			ret = do_item_insertion(root, index, item, eroot->eset, child);
		}
		if (ret < 0 && is_empty) {
			ebr_radix_tree_destroy(eroot);
		}
	}

	return ret;
}

static struct radix_tree_node *
do_radix_tree_find_node(const struct radix_tree_root *root, unsigned long index)
{
	unsigned int offset = 0;
	struct radix_tree_node *node = NULL;
	struct radix_tree_node *ret_node = NULL;

	node = ebr_dereference(root->rnode);
	if ((node != NULL) && (index <= get_node_max_index(node))) {
		for (;;) {
			struct radix_tree_node *child = NULL;

			offset = get_next_level_offset(node, index);
			child = ebr_dereference(node->slots[offset]);
			if (child == NULL || node->height == 0U) {
				ret_node = node;
				break;
			}
			node = child;
		}
	}

	return ret_node;
}

/*
 * ebr_radix_tree_find_item: get an item associated with key @index from @eroot.
 * Lookup operation for @eroot is totally ebr-safe, which means no needs to hold
 * a lock, and it promises the nodes can not be reclaimed during lookup.
 * Followings are the sample code of lookup under ebr protection:
 * -------------------------------------------
 * ebr_read_lock(eroot->eset);
 * void *ret = ebr_radix_tree_find_item(eroot, index);
 * if (ret == NULL) {
 * 	ebr_read_unlock(eroot->eset);
 * 	return;
 * }
 * ebr_read_unlock(eroot->eset);
 * -------------------------------------------
 *
 * @param	eroot	[I] Radix tree guarded by ebr
 * @param	index	[I] The key used to locate item
 *
 * @return	Returns the pointer of the specified item. If @index does
 * 		not exsit, returns pointer of NULL.
 */
void *ebr_radix_tree_find_item(struct ebr_radix_tree_root *eroot,
			       unsigned long index)
{
	unsigned int offset = 0U;
	struct radix_tree_node *node = NULL;
	struct radix_tree_root *root = NULL;
	void *ret = NULL;

	if (eroot != NULL) {
		root = &eroot->root;
		node = do_radix_tree_find_node(root, index);
		if (node != NULL) {
			offset = get_next_level_offset(node, index);
			ret = ebr_dereference(node->slots[offset]);
			if (node->height != 0U) {
				ret = NULL;
			}
		}
	}

	return ret;
}

/*
 * ebr_radix_tree_raplace_item: replace the item associated with key @index
 * from @eroot. Although @eroot is guarded by EBR, still make sure lock
 * must be held before entering this function.
 *
 * @param	eroot	[I] Radix tree guarded by ebr
 * @param	item	[I] New item to replace the old item associated @index
 * @param	index	[I] The key used to locate item for replacement
 *
 * @return	Returns the pointer of old item, and if @index does not exist,
 * 		return pointer of NULL.
 *
 * @note	Reclamation of old item must be handled by user
 */
void *ebr_radix_tree_replace_item(struct ebr_radix_tree_root *eroot,
				  void *item, unsigned long index)
{
	unsigned int offset = 0U;
	struct radix_tree_node *node = NULL;
	struct radix_tree_root *root = NULL;
	void *old_item = NULL;

	/* The new item should not be NULL */
	if ((eroot != NULL) && (item != NULL)) {
		root = &eroot->root;
		node = do_radix_tree_find_node(root, index);
		if (node != NULL) {
			offset = get_next_level_offset(node, index);
			if ((node->height == 0U) && (node->slots[offset] != NULL)) {
				old_item = node->slots[offset];
				ebr_assign_pointer(node->slots[offset], item);
			}
		}
	}

	return old_item;
}

static void do_radix_tree_delete_node(struct radix_tree_root *root,
				      struct radix_tree_node *node,
				      unsigned int offset,
				      struct epoch_set *es)
{
	unsigned int curr_offset = offset;
	struct radix_tree_node *node_tmp = node;
	for (;;) {
		struct radix_tree_node *parent = NULL;
		struct ebr_radix_tree_node *enode = NULL;

		ebr_assign_pointer(node_tmp->slots[curr_offset], NULL);
		node_tmp->count--;

		if (node_tmp->count != 0U) {
			clear_node_bit_upward(root, node_tmp, curr_offset);
			for (unsigned int tag = 0; tag < __NR_TAGS; tag++) {
				clear_node_tags(node_tmp, tag, curr_offset);
			}
			break;
		}

		curr_offset = node_tmp->offset;
		parent = node_tmp->parent;

		enode = container_of(node_tmp, struct ebr_radix_tree_node, node);
		if (parent != NULL) {
			ebr_assign_pointer(parent->slots[curr_offset], NULL);
		} else {
			ebr_assign_pointer(root->rnode, NULL);
		}
		(void)synchronize_ebr(es);
		free_node(enode, es);
		if (parent == NULL) {
			root->has_free = true;
			break;
		}
		node_tmp = parent;
	}
}

/*
 * ebr_radix_tree_delete_item: delete the item associated with key @index
 * from @eroot. Although @eroot is guarded by EBR, still make sure lock
 * must be held before entering this function.
 *
 * @param	eroot	[I] Radix tree guarded by ebr
 * @param	index	[I] The key used to locate item for deletion
 *
 * @return	Returns the pointer of the specified item, and if @index does not
 * 		exsit, return pointer of NULL.
 *
 * @note	Reclamation of removed item must be taken care of by user
 */
void *ebr_radix_tree_delete_item(struct ebr_radix_tree_root *eroot,
				 unsigned long index)
{
	unsigned int offset = 0U;
	int err = E_HM_OK;
	struct radix_tree_node *node = NULL;
	struct radix_tree_root *root = NULL;
	void *item = NULL;

	if (eroot == NULL) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		root = &eroot->root;
		if (root->rnode == NULL) {
			err = E_HM_INVAL;
		}
	}

	if (err == E_HM_OK) {
		node = do_radix_tree_find_node(root, index);
		if (node == NULL) {
			err = E_HM_INVAL;
		}
	}

	if (err == E_HM_OK) {
		offset = get_next_level_offset(node, index);
		if (node->height != 0U || node->slots[offset] == NULL) {
			err = E_HM_INVAL;
		}
	}

	if (err == E_HM_OK) {
		item = node->slots[offset];
		do_radix_tree_delete_node(root, node, offset, eroot->eset);
	}

	return item;
}

static unsigned int get_next_slot(struct radix_tree_node *node,
				  struct radix_tree_node **next,
				  unsigned int tag,
				  unsigned int offset)
{
	struct radix_tree_node *tmp = NULL;
	unsigned int i = offset;

	if (tag == __NR_TAGS) {
		for (; i < RADIX_TREE_SLOTS_SIZE; i++) {
			tmp = ebr_dereference(node->slots[i]);
			if (tmp != NULL) {
				*next = tmp;
				break;
			}
		}
	} else {
		while (true) {
			i = search_node_tag(node, tag, i);
			if (i == RADIX_TREE_SLOTS_SIZE) {
				break;
			}
			tmp = ebr_dereference(node->slots[i]);
			if (tmp != NULL) {
				*next = tmp;
				break;
			}
		}
	}

	return i;
}

struct ebr_radix_tree_iterator {
	struct radix_tree_node *node;
	struct radix_tree_node *rnode;
	unsigned long index;
	unsigned long next_index;
	unsigned int tag;
};

static inline void do_radix_tree_iter_init(struct ebr_radix_tree_iterator *iter,
					   struct radix_tree_node *rnode,
					   unsigned long start,
					   unsigned int tag)
{
	iter->node = NULL;
	iter->rnode = rnode;
	iter->index = 0;
	iter->next_index = start;
	iter->tag = tag;
};

static void *
do_get_first_item_from(struct ebr_radix_tree_iterator *iter)
{
	unsigned long index = 0;
	struct radix_tree_node *pos = NULL;
	struct radix_tree_node *ret = NULL;

	index = iter->next_index;
	pos = iter->node == NULL ? iter->rnode : iter->node;
	while (index <= get_node_max_index(iter->rnode)) {
		unsigned int offset = 0;
		struct radix_tree_node *child = NULL;
		unsigned long child_index = 0;

		offset = get_next_level_offset(pos, index);
		offset = get_next_slot(pos, &child, iter->tag, offset);
		if (offset == RADIX_TREE_SLOTS_SIZE) {
			index = (index & (~get_node_max_index(pos))) +
				get_node_max_index(pos) + 1U;
			if (pos == iter->rnode) {
				/* terminate loop when index come back to head */
				break;
			}
			pos = iter->rnode;
		} else {
			if (pos->height == 0U) {
				iter->index = (index & (~get_node_max_index(pos))) +
					      get_next_level_index(pos, offset);
				offset++;
				iter->next_index = (index & (~get_node_max_index(pos))) +
						   get_next_level_index(pos, offset);

				iter->node = (offset == RADIX_TREE_SLOTS_SIZE) ?
					     iter->rnode : pos;
				ret = child;
				break;
			} else {
				child_index = (index & (~get_node_max_index(pos))) +
					      get_next_level_index(pos, offset);
				index = index < child_index ? child_index : index;
				pos = child;
			}
		}
	}
	return ret;
}

static void *get_first_item_from(struct ebr_radix_tree_iterator *iter)
{
	struct radix_tree_node *ret = NULL;

	if ((iter != NULL) && (iter->rnode != NULL) &&
	    (iter->next_index <= get_node_max_index(iter->rnode)) &&
	    (iter->index <= iter->next_index)) {
		ret = do_get_first_item_from(iter);
	}
	return ret;
}

/*
 * ebr_radix_tree_get_next: get the item associated with key @index from @eroot,
 * if @index does not exsit, then return the next exsiting item and its index.
 * Similar to find, it's also ebr-safe, which means no needs to hold a lock.
 *
 * @param	eroot	[I] Radix-tree guarded by ebr
 * @param	index	[I,O] The key to locate the item, and stores next index
 *
 * @return	Returns the pointer of located item, and if the item and all
 * 		its next items do not exist, return pointer of NULL.
 */
void *ebr_radix_tree_get_next(struct ebr_radix_tree_root *eroot,
			      unsigned long *index)
{
	void *data = NULL;
	int err = E_HM_OK;
	struct ebr_radix_tree_iterator iter;
	struct radix_tree_node *rnode = NULL;
	struct radix_tree_root *root = NULL;

	if (eroot == NULL || index == NULL) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		root = &eroot->root;
		rnode = ebr_dereference(root->rnode);
		if (rnode == NULL) {
			err = E_HM_INVAL;
		}
	}

	if (err == E_HM_OK) {
		do_radix_tree_iter_init(&iter, rnode, *index, __NR_TAGS);
		data = get_first_item_from(&iter);
		if (data != NULL) {
			*index = iter.index;
		}
	}

	return data;
}

/*
 * ebr_radix_tree_apply: traverse all items in @eroot, and apply @callback
 * for each item. Apply operation is only ebr-safe when @callback does not
 * modify the structure of @eroot.
 *
 * @param	eroot		[I] Radix-tree guarded by ebr
 * @param	callback	[I] callback function applied for each item in
 * 				the tree, and takeing item, its index, and data
 * 				as parameters
 * @param	data		[I] Third parameter of @callback, specified by user
 *
 * @return	Returns the result of apply operation. E_HM_OK for success.
 * 		On any error, return a negative integer.
 */
int ebr_radix_tree_apply(struct ebr_radix_tree_root *eroot,
			 int (*callback)(unsigned long index, void *item, void *data),
			 void *data)
{
	int err = E_HM_OK;
	void *slot = NULL;
	struct ebr_radix_tree_iterator iter;
	struct radix_tree_node *rnode = NULL;
	struct radix_tree_root *root = NULL;

	if (eroot == NULL || callback == NULL) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		root = &eroot->root;
		rnode = ebr_dereference(root->rnode);
		if (rnode == NULL) {
			err = E_HM_RESFAULT;
		}
	}

	if (err == E_HM_OK) {
		do_radix_tree_iter_init(&iter, rnode, 0, __NR_TAGS);
		slot = get_first_item_from(&iter);
		while (slot != NULL) {
			void *next_slot = NULL;
			unsigned long pre_index = 0U;

			pre_index = iter.index;
			next_slot = get_first_item_from(&iter);
			err = callback(pre_index, slot, data);
			if (err != E_HM_OK) {
				break;
			}
			slot = next_slot;
		}
	}

	return err;
}

static struct radix_tree_node *
alloc_slot(struct radix_tree_root *root,
	   struct radix_tree_node *node,
	   unsigned int *offset,
	   unsigned int *height,
	   struct epoch_set *es)
{
	unsigned int tmp_off = *offset;
	unsigned int tmp_hgt = *height;

	tmp_off = raw_bitmap_find_next_zero(node->bitmap, RADIX_TREE_SLOTS_SIZE,
					    *offset);
	while (tmp_off >= RADIX_TREE_SLOTS_SIZE) {
		tmp_off = node->offset + 1U;
		node = node->parent;
		/*
		 * Reaching rnode means running out of empty slots.
		 * So grow the tree to accommodate acquire.
		 */
		if (node == NULL) {
			unsigned long max_index = 0UL;

			max_index = get_node_max_index(root->rnode) + 1UL;
			node = do_radix_tree_growth(root, max_index, es);
			if (node == NULL) {
				break;
			}
			tmp_off = node->offset + 1U;
		}
		tmp_hgt = node->height;
	}
	if (node != NULL) {
		*offset = tmp_off;
		*height = tmp_hgt;
	}

	return node;
}

static int
__do_radix_tree_alloc_slot(struct ebr_radix_tree_root *eroot,
			   unsigned long *startp, unsigned long end,
			   void *item, struct radix_tree_node *child)
{
	unsigned int offset = 0U;
	unsigned int height = 0U;
	struct radix_tree_node *node = NULL;
	struct radix_tree_root *root = &eroot->root;
	unsigned long tmp_sp = *startp;
	int err = E_HM_OK;
	struct radix_tree_node *child_tmp = child;

	height = child_tmp->height + 1U;
	while (height != 0U) {
		height -= 1U;
		if (child_tmp == NULL) {
			child_tmp = alloc_node(node, offset, height, eroot->eset);
			if (child_tmp == NULL) {
				rollback_node_allocation(root, node, eroot->eset);
				err = E_HM_NOMEM;
				break;
			}
		}
		node = child_tmp;
		offset = get_next_level_offset(node, tmp_sp);
		if (raw_bitmap_test_bit(node->bitmap, RADIX_TREE_SLOTS_SIZE,
					offset) != 0) {
			node = alloc_slot(root, node, &offset, &height, eroot->eset);
			if (node == NULL) {
				err = E_HM_NOMEM;
				break;
			}
			tmp_sp = (tmp_sp & (~get_node_max_index(node))) +
				  get_next_level_index(node, offset);
			if (tmp_sp > end) {
				err = E_HM_NOSPC;
				break;
			}
			*startp = tmp_sp;
		}
		child_tmp = node->slots[offset];
	}

	if (err == E_HM_OK) {
		publish_slot(root, node, offset, item);
	}

	return err;
}

static int do_radix_tree_alloc_slot(struct ebr_radix_tree_root *eroot,
				    unsigned long *startp, unsigned long end,
				    void *item)
{
	int err = E_HM_OK;
	struct radix_tree_root *root = &eroot->root;
	struct radix_tree_node *child = root->rnode;
	unsigned long tmp_sp = *startp;

	if (tmp_sp > get_node_max_index(child)) {
		child = do_radix_tree_growth(root, tmp_sp, eroot->eset);
		if (child == NULL) {
			err = E_HM_NOMEM;
		}
	}
	if (err == E_HM_OK) {
		err = __do_radix_tree_alloc_slot(eroot, startp, end, item, child);
	}

	return err;
}

/*
 * ebr_radix_tree_alloc_slot: alloc a free slot of @eroot and insert an @item
 * within the range of @startp to @end. Like other functions of changing the
 * structure of @eroot, lock must be held before entering.
 *
 * @param	eroot	[I] Radix tree guarded by ebr
 * @param	startp	[I] Pointer of key associated with @item
 * @param	end	[I] The maximum key that can be allocated
 * @param	item	[I] The item ready to be inserted
 *
 * @return	Return the result of slot allocation. E_HM_OK for success.
 *		On any error, return a negative integer.
 */
int ebr_radix_tree_alloc_slot(struct ebr_radix_tree_root *eroot,
			      unsigned long *startp,
			      unsigned long end, void *item)
{
	int err = E_HM_OK;
	bool is_empty = false;
	struct radix_tree_root *root = NULL;

	if ((eroot == NULL) || (startp == NULL) || (item == NULL)) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		root = &eroot->root;
		if (root->rnode == NULL) {
			is_empty = true;
			struct radix_tree_node *tmp = alloc_node(NULL, 0U, 0U, eroot->eset);
			if (tmp == NULL) {
				err = E_HM_NOMEM;
			} else {
				ebr_assign_pointer(root->rnode, tmp);
			}
		}
	}

	if (err == E_HM_OK) {
		if (!root->has_free) {
			unsigned long max_index = get_node_max_index(root->rnode);
			*startp = max(*startp, max_index + 1UL);
		}

		if (*startp > end) {
			err = E_HM_NOSPC;
		} else {
			err = do_radix_tree_alloc_slot(eroot, startp, end, item);
			if (err < 0 && is_empty) {
				ebr_radix_tree_destroy(eroot);
			}
		}
	}

	return err;
}

static bool match_everything(void *item)
{
	UNUSED(item);
	return true;
}

static int
do_ebr_radix_tree_find_items(struct ebr_radix_tree_root *eroot,
			    unsigned long *start, int nr, void **items, unsigned long *indexs, bool (*match)(void *item))
{
	struct radix_tree_node *rnode = NULL;
	struct radix_tree_root *root = NULL;
	int nr_found = 0;
	void *item = NULL;
	struct ebr_radix_tree_iterator iter;

	root = &eroot->root;
	rnode = ebr_dereference(root->rnode);
	if (rnode != NULL) {
		do_radix_tree_iter_init(&iter, rnode, *start, __NR_TAGS);
		for (; nr_found < nr;) {
			item = get_first_item_from(&iter);
			if (item == NULL) {
				break;
			}
			if (match(item) == false) {
				continue;
			}
			if (indexs != NULL) {
				indexs[nr_found] = iter.index;
			}
			items[nr_found] = item;
			nr_found++;
		}

		if (get_first_item_from(&iter) == NULL) {
			*start = 0;
		} else {
			*start = iter.index;
		}
	}

	return nr_found;
}

/*
 * ebr_radix_tree_find_items: get the number(@nr) of items start from the key
 * @start from @eroot, store items into @items array, and store items' indexs
 * into @indexs array. Similar to get single item, it's also ebr-safe.
 *
 * @param	eroot	[I] Radix tree guarded by ebr
 * @param	start	[I] The key to start searching
 * @param	nr	[I] The maximum of items to get
 * @param	items	[O] The pointer array to store items
 * @param	indexs	[O] The pointer array to store items' indexs
 *
 * @return	Returns the number of items found
 */
int ebr_radix_tree_find_items_match(struct ebr_radix_tree_root *eroot,
	unsigned long *start, int nr, void **items, unsigned long *indexs, bool (*match)(void *item))
{
	int ret = E_HM_OK;

	if (eroot == NULL || start == NULL || items == NULL || nr < 0) {
		ret = E_HM_INVAL;
	} else if (nr != 0) {
		ret = do_ebr_radix_tree_find_items(eroot, start, nr, items, indexs, match);
	} else {
		/* do nothing */
	}

	return ret;
}

int ebr_radix_tree_find_items(struct ebr_radix_tree_root *eroot,
			      unsigned long *start, int nr, void **items, unsigned long *indexs)
{
	return ebr_radix_tree_find_items_match(eroot, start, nr, items, indexs, match_everything);
}

/*
 * ebr_radix_tree_tagged: check if radix tree is tagged or not according
 * to the its root node. This function does not change the structure of
 * the tree. But, if strong consistency of the result is needed, recommand to
 * hold a lock before entering.
 *
 * @param	eroot	[I] Radix tree guarded by ebr
 * @param	tag	[I] Tag value
 *
 * @return	Returns true if radix tree is tagged, otherwise false.
 */
bool ebr_radix_tree_tagged(struct ebr_radix_tree_root *eroot,
			   unsigned int tag)
{
	bool ret = false;
	struct radix_tree_root *root = NULL;

	if (eroot != NULL) {
		root = &eroot->root;
		BUG_ON(tag >= __NR_TAGS);
		if (!ebr_radix_tree_is_empty(eroot)) {
			ret = is_node_tagged(ebr_dereference(root->rnode), tag);
		}
	}

	return ret;
}

/*
 * ebr_radix_tree_tag_set: tag path to the item associated with the specified
 * @index. This operation does not change the structure of the tree, which means
 * it's normally ebr-safe. But still, aquire a lock if depends on the result.
 *
 * @param	eroot	[I] Radix tree guarded by ebr
 * @param	index	[I] The key associated with the item to be tagged
 * @param	tag	[I] Tag value
 *
 * @return	Return the poniter of if tagged item.
 */
void *ebr_radix_tree_tag_set(struct ebr_radix_tree_root *eroot,
			     unsigned long index, unsigned int tag)
{
	struct radix_tree_node *curr = NULL;
	struct radix_tree_root *root = NULL;
	unsigned int cycles = 0U;
	bool empty = false;

	if (eroot != NULL && eroot->root.rnode != NULL && tag < __NR_TAGS) {
		/* The caller must ensure that @index is already bound to an item. */
		empty = ebr_radix_tree_is_empty(eroot);
		BUG_ON(empty);
		root = &eroot->root;
		curr = root->rnode;

		for (cycles = curr->height + 1U; cycles > 0U; cycles--) {
			unsigned int offset = 0U;

			offset = get_next_level_offset(curr, index);
			set_node_tag(curr, tag, offset);
			curr = curr->slots[offset];
			BUG_ON(curr == NULL);
		}
	}

	return curr;
}

/*
 * ebr_radix_tree_tag_clear: untagged the path to the item associated
 * with the specified index. This operation does not change the structure
 * of the tree, which means it's normally ebr-safe. But still,
 * aquire a lock if depends on the result.
 *
 * @param	eroot	[I] Radix tree guarded by ebr
 * @param	index	[I] The key associated with the item to be untagged
 * @param	tag	[I] Tag value
 *
 * @return	Return the poniter of if untagged item.
 */
void *ebr_radix_tree_tag_clear(struct ebr_radix_tree_root *eroot,
			       unsigned long index, unsigned int tag)
{
	int err = E_HM_OK;
	struct radix_tree_node *node = NULL;
	struct radix_tree_node *item = NULL;
	unsigned int offset = 0U;
	struct radix_tree_root *root = NULL;

	if (tag >= __NR_TAGS) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		if (!ebr_radix_tree_tagged(eroot, tag)) {
			err = E_HM_INVAL;
		}
	}

	if (err == E_HM_OK) {
		root = &eroot->root;
		node = do_radix_tree_find_node(root, index);
		if (node == NULL) {
			err = E_HM_INVAL;
		}
	}
	if (err == E_HM_OK) {
		offset = get_next_level_offset(node, index);
		if (test_node_tag(node, tag, offset) == 0) {
			err = E_HM_INVAL;
		}
	}
	if (err == E_HM_OK) {
		item = node->slots[offset];
		clear_node_tags(node, tag, offset);
	}
	return item;
}

/*
 * ebr_radix_tree_tag_test: check if the item associated with
 * the specified index is tagged or not. This operation does not change the
 * structure of the tree, which means it's normally ebr-safe. But still,
 * aquire a lock if depends on the result.
 *
 * @param	eroot	[I] Radix tree guarded by ebr
 * @param	index	[I] The key associated with the item to be tested
 * @param	tag	[I] Tag value
 *
 * @return	Returns true if the item is tagged, or false.
 */
int ebr_radix_tree_tag_test(struct ebr_radix_tree_root *eroot,
			     unsigned long index, unsigned int tag)
{
	int ret = 0;
	struct radix_tree_node *node = NULL;
	unsigned int offset = 0U;
	struct radix_tree_root *root = NULL;

	if (tag < __NR_TAGS) {
		if (ebr_radix_tree_tagged(eroot, tag)) {
			root = &eroot->root;
			node = do_radix_tree_find_node(root, index);
			if (node != NULL) {
				offset = get_next_level_offset(node, index);
				ret = test_node_tag(node, tag, offset);
			}
		}
	}
	return ret;
}

static int
do_ebr_radix_tree_find_tagged_items(struct ebr_radix_tree_root *eroot,
				    unsigned long *start, unsigned int tag,
				    int nr, void **items, bool (*match)(void *item))
{
	struct radix_tree_node *rnode = NULL;
	struct radix_tree_root *root = NULL;
	int nr_found = 0;
	void *item = NULL;
	struct ebr_radix_tree_iterator iter;

	root = &eroot->root;
	rnode = ebr_dereference(root->rnode);
	if (rnode != NULL) {
		do_radix_tree_iter_init(&iter, rnode, *start, tag);
		for (; nr_found < nr;) {
			item = get_first_item_from(&iter);
			if (item == NULL) {
				break;
			}
			if (match(item) == false) {
				continue;
			}
			items[nr_found] = item;
			nr_found++;
		}

		iter.tag = __NR_TAGS;
		if (get_first_item_from(&iter) == NULL) {
			*start = 0;
		} else {
			*start = iter.index;
		}
	}

	return nr_found;
}

/*
 * ebr_radix_tree_find_tagged_items: get the number(@nr) of tagged items start
 * from the key @start from @eroot, and store tagged items into @items array.
 * Similar to get multi-items, it's also ebr-safe.
 *
 * @param	eroot	[I] Radix-tree guarded by ebr
 * @param	start	[I] The key to start searching
 * @param	tag	[I] Tag value
 * @param	nr	[I] The maximum of tagged items to get
 * @param	items	[O] A pointer array to store tagged items
 *
 * @return	Returns the number of tagged items found
 */
int ebr_radix_tree_find_tagged_items_match(struct ebr_radix_tree_root *eroot,
				     unsigned long *start, unsigned int tag,
				     int nr, void **items, bool (*match)(void *item))
{
	int nr_found = 0;

	if (eroot == NULL || start == NULL || items == NULL || nr < 0 ||
	    tag >= __NR_TAGS) {
		nr_found = E_HM_INVAL;
	} else if (nr != 0) {
		nr_found = do_ebr_radix_tree_find_tagged_items(eroot, start, tag, nr, items, match);
	} else {
		/* do nothing */
	}

	return nr_found;
}

int ebr_radix_tree_find_tagged_items(struct ebr_radix_tree_root *eroot,
				     unsigned long *start, unsigned int tag,
				     int nr, void **items)
{
	return ebr_radix_tree_find_tagged_items_match(eroot, start, tag, nr, items, match_everything);
}
