/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Tue Sep 03 22:21:06 2019
 */

#include <libalgo/radix_tree.h>
#include <hongmeng/errno.h>
#include <libhwsecurec/securec.h>
#include <libhmsync/raw_mutex.h>
#include <libhmsync/bitops/generic.h>
#include <libpreempt/preempt.h>

#include "radixtree_internal.h"

#define max(a, b) ({			\
		   __typeof__ (a) _a = (a);	\
		   __typeof__ (b) _b = (b);	\
		   _a > _b ? _a : _b;})

struct radix_tree_iterator {
	struct radix_tree_node *leaf;
	unsigned int offset;
};

struct radix_tree_pool {
	int nr;
	struct radix_tree_node *nodes; /* nodes->parent points to next pool node */
};

static int nr_radix_pools;
struct radix_tree_pool *radix_pools;

static inline struct radix_tree_pool *pool_percpu_get(void)
{
	int cpuid = preempt_process_disable();
	BUG_ON_D(cpuid < 0 || cpuid >= nr_radix_pools);
	BUG_ON_D(radix_pools == NULL);
	return &radix_pools[cpuid];
}

static inline void pool_percpu_put(void)
{
	(void)preempt_process_enable();
}

static inline void pool_clear(struct radix_tree_pool *rtp)
{
	while (rtp->nr > 0) {
		struct radix_tree_node *node = rtp->nodes;
		rtp->nodes = node->parent;
		rtp->nr--;
		radix_tree_node_free(node);
	}
}

static inline void pool_insert(struct radix_tree_pool *rtp, struct radix_tree_node *node)
{
	node->parent = rtp->nodes;
	rtp->nodes = node;
	rtp->nr++;
}

static inline struct radix_tree_node *pool_remove(struct radix_tree_pool *rtp)
{
	struct radix_tree_node *node = NULL;

	if (rtp->nr > 0) {
		node = rtp->nodes;
		rtp->nodes = node->parent;
		rtp->nr--;
	}

	return node;
}

int radix_tree_pool_init(struct radix_tree_root *root, int nr_pools)
{
	int ret = E_HM_NOMEM;

	if (nr_radix_pools == 0) {
		radix_pools = (struct radix_tree_pool *)calloc((size_t)nr_pools, sizeof(struct radix_tree_pool));
		if (radix_pools != NULL) {
			nr_radix_pools = nr_pools;
			root->use_pool = true;
			ret = E_HM_OK;
		}
	} else if (nr_radix_pools == nr_pools) {
		root->use_pool = true;
		ret = E_HM_OK;
	} else {
		BUG_ON_D(nr_radix_pools != nr_pools);
		root->use_pool = false;
		ret = E_HM_INVAL;
	}

	return ret;
}

static int __radix_tree_pool_begin(int nr)
{
	int rc = E_HM_OK;
	struct radix_tree_node *node = NULL;
	struct radix_tree_pool *rtp = NULL;

	rtp = pool_percpu_get();
	while (rtp->nr < nr) {
		pool_percpu_put();

		node = (struct radix_tree_node *)radix_tree_node_malloc();
		if (node == NULL) {
			rc = E_HM_NOMEM;
			break;
		}

		rtp = pool_percpu_get();
		pool_insert(rtp, node);
	}

	return rc;
}

int radix_tree_pool_begin(struct radix_tree_root *root __maybe_unused, bool charge)
{
	BUG_ON_D(root->use_pool == false);
	return __radix_tree_pool_begin(charge ? (int)RADIX_TREE_PRELOAD_SIZE : 0);
}

static inline void pool_drain(struct radix_tree_pool *tmp)
{
	struct radix_tree_pool *rtp = pool_percpu_get();
	while (rtp->nr > (int)RADIX_TREE_PRELOAD_SIZE) {
		struct radix_tree_node *node = pool_remove(rtp);
		pool_insert(tmp, node);
	}
	pool_percpu_put();
}

void radix_tree_pool_end(struct radix_tree_root *root __maybe_unused)
{
	BUG_ON_D(root->use_pool == false);
	struct radix_tree_pool tmp = {0};
	pool_drain(&tmp);
	pool_percpu_put();
	pool_clear(&tmp);
}

static inline struct radix_tree_node *malloc_from_pool(void)
{
	struct radix_tree_pool *rtp = pool_percpu_get();
	struct radix_tree_node *node = pool_remove(rtp);
	pool_percpu_put();
	return node;
}

static inline void free_to_pool(struct radix_tree_node *node)
{
	struct radix_tree_pool *rtp = pool_percpu_get();
	pool_insert(rtp, node);
	pool_percpu_put();
}

static inline void free_node(const struct radix_tree_root *root, struct radix_tree_node *node)
{
	if (root->use_pool == true) {
		free_to_pool(node);
	} else {
		radix_tree_node_free(node);
	}
}

static inline struct radix_tree_node *malloc_node(const struct radix_tree_root *root)
{
	struct radix_tree_node *node = NULL;

	if (root->use_pool == true) {
		node = malloc_from_pool();
	} else {
		node = radix_tree_node_malloc();
	}

	return node;
}

static inline void node_tag_set(struct radix_tree_node *node,
				unsigned int tag,
				unsigned int offset)
{
	NOFAIL(raw_bitmap_set_bit(node->tags[tag],
				  RADIX_TREE_SLOTS_SIZE, offset));
}

static inline void node_tag_clear(struct radix_tree_node *node,
				  unsigned int tag,
				  unsigned int offset)
{
	NOFAIL(raw_bitmap_clear_bit(node->tags[tag],
				    RADIX_TREE_SLOTS_SIZE, offset));
}

static inline int node_tag_test(const struct radix_tree_node *node,
				unsigned int tag,
				unsigned int offset)
{
	return raw_bitmap_test_bit(node->tags[tag],
				   RADIX_TREE_SLOTS_SIZE, offset);
}

/* search scope is bits [start+1, last] */
static inline unsigned int
node_tag_next_one(const struct radix_tree_node *node,
		  unsigned int tag,
		  unsigned int start)
{
	return raw_bitmap_find_next_one(node->tags[tag],
					RADIX_TREE_SLOTS_SIZE, start);
}

/* search scope is bits [start, last] */
static inline unsigned int
node_tag_search_one(const struct radix_tree_node *node,
		    unsigned int tag,
		    unsigned int start)
{
	unsigned int ret;

	if (node_tag_test(node, tag, start) == 1) {
		ret = start;
	} else {
		ret = node_tag_next_one(node, tag, start);
	}

	return ret;
}

static bool is_node_tagged(const struct radix_tree_node *node,
				  unsigned int tag)
{
	unsigned int i = 0U;
	bool ret_value = false;

	for (i = 0U; i < BITS_TO_LONGLONGS(RADIX_TREE_SLOTS_SIZE); i++) {
		if (node->tags[tag][i] != 0ULL) {
			ret_value = true;
			break;
		}
	}
	return ret_value;
}

bool radix_tree_is_empty(const struct radix_tree_root *root)
{
	BUG_ON(root == NULL);
	return root->rnode == NULL;
}

static unsigned long __height_max_index(unsigned int height)
{
	return (((1UL << RADIX_TREE_SHIFT_SIZE) << (height * RADIX_TREE_SHIFT_SIZE)) - 1);
}

static unsigned long __node_max_index(const struct radix_tree_node *node)
{
	return __height_max_index(node->height);
}

static void
__node_set_bit_upward(struct radix_tree_root *root,
		      struct radix_tree_node *node, unsigned int _offset)
{
	unsigned int offset = _offset;
	while (node != NULL) {
		int err = raw_bitmap_set_bit(node->bitmap, RADIX_TREE_SLOTS_SIZE, offset);
		BUG_ON(err != 0);

		if (!raw_bitmap_is_full(node->bitmap, RADIX_TREE_SLOTS_SIZE)) {
			break;
		}
		offset = node->offset;
		node = node->parent;
	}

	if (node == NULL) {
		root->has_free = false;
	}
}

static void
__node_clear_bit_upward(struct radix_tree_root *root,
			struct radix_tree_node *node, unsigned int _offset)
{
	unsigned int offset = _offset;
	while (node != NULL) {
		if (raw_bitmap_test_bit(node->bitmap, RADIX_TREE_SLOTS_SIZE, offset) == 0) {
			break;
		}

		int err = raw_bitmap_clear_bit(node->bitmap, RADIX_TREE_SLOTS_SIZE, offset);
		BUG_ON(err != 0);

		offset = node->offset;
		node = node->parent;
	}

	if (node == NULL) {
		root->has_free = true;
	}
}

static void
__node_clear_tag_upward(struct radix_tree_node *node,
			unsigned int tag, unsigned int _offset)
{
	unsigned int offset = _offset;
	while (node != NULL) {
		node_tag_clear(node, tag, offset);
		if (is_node_tagged(node, tag)) {
			break;
		}

		offset = node->offset;
		node = node->parent;
	}
}

static void *
alloc_child_node(struct radix_tree_root *root, struct radix_tree_node *parent,
		 unsigned int offset, unsigned int height)
{
	struct radix_tree_node *newnode = NULL;

	if (offset < RADIX_TREE_SLOTS_SIZE) {
		newnode = malloc_node(root);
		if (newnode == NULL) {
			hm_error("malloc_node new radix_tree_node error\n");
		} else {
			mem_zero_s(*newnode);
			newnode->offset = offset;
			newnode->height = height;
			newnode->parent = parent;
			newnode->count = 0;

			if (parent != NULL) {
				parent->slots[offset] = newnode;
				parent->count++;
			}
		}
	}

	return newnode;
}

static void rollback_node_allocation(struct radix_tree_root *root,
				     struct radix_tree_node *node)
{
	struct radix_tree_node *parent = NULL;
	struct radix_tree_node *tmp = node;
	unsigned int offset = 0;

	BUG_ON(root == NULL || tmp == NULL);

	if (tmp->count == 0U) {
		for (;;) {
			parent = tmp->parent;
			offset = tmp->offset;
			free_node(root, tmp);
			if (parent == NULL) {
				root->rnode = NULL;
				root->has_free = true;
				break;
			}
			parent->slots[offset] = NULL;
			parent->count--;
			if (parent->count != 0U) {
				break;
			}

			tmp = parent;
		}
	}
}

static void __set_new_rnode_tags(const struct radix_tree_node *old,
				 struct radix_tree_node *new)
{
	for (unsigned int tag = 0; tag < __NR_TAGS; tag++) {
		if (is_node_tagged(old, tag)) {
			node_tag_set(new, tag, 0);
		}
	}
}

static void *__radix_tree_grow(struct radix_tree_root *root, unsigned long start)
{
	BUG_ON(root == NULL);
	BUG_ON(root->rnode == NULL);

	int err = E_HM_OK;
	struct radix_tree_node *pnode = (struct radix_tree_node *)root->rnode;
	unsigned int max_height = pnode->height;
	void *item = NULL;

	while (start > __height_max_index(max_height)) {
		max_height += 1U;
		if (pnode->count != 0U) {
			struct radix_tree_node *newnode = alloc_child_node(root, NULL, 0U, max_height);
			if (newnode == NULL) {
				err = E_HM_NOMEM;
				break;
			}
			struct radix_tree_node *top_node = root->rnode;
			root->rnode = newnode;

			/* update pnode when root->rnode is changed. */
			pnode = (struct radix_tree_node *)root->rnode;
			top_node->parent = newnode;
			newnode->slots[0] = top_node;
			newnode->count++;
			if (raw_bitmap_is_full(top_node->bitmap, RADIX_TREE_SLOTS_SIZE)) {
				(void)raw_bitmap_set_bit(newnode->bitmap, RADIX_TREE_SLOTS_SIZE, 0U);
			}
			__set_new_rnode_tags(top_node, newnode);
		}
	}
	if (err == E_HM_OK) {
		if (pnode->count == 0U) {
			pnode->height = max_height;
		}
		root->has_free = true;
		item = root->rnode;
	}

	return item;
}

static unsigned int __next_level_offset(const struct radix_tree_node *node,
					unsigned long index)
{
	return (unsigned int)((index >> (node->height * RADIX_TREE_SHIFT_SIZE)) &
			      RADIX_TREE_MASK);
}

static unsigned long __next_level_index(const struct radix_tree_node *node,
					unsigned int offset)
{
	return (unsigned long)offset << (node->height * RADIX_TREE_SHIFT_SIZE);
}

static struct radix_tree_node *__alloc_slot(struct radix_tree_root *root, struct radix_tree_node *node,
					    unsigned int *offset, unsigned int *height)
{
	*offset = raw_bitmap_find_next_zero(node->bitmap, RADIX_TREE_SLOTS_SIZE, *offset);
	while (*offset >= RADIX_TREE_SLOTS_SIZE) {
		*offset = node->offset + 1U;
		node = node->parent;
		/*
		 * Reaching rnode means running out of empty slots.
		 * So grow the tree to accommodate acquire.
		 */
		if (node == NULL) {
			node = __radix_tree_grow(root, __node_max_index(root->rnode) + 1UL);
			if (node == NULL) {
				break;
			}
			*offset = node->offset + 1U;
		}
		*height = node->height;
	}
	return node;
}

static int do_alloc_slot(struct radix_tree_root *root,
			 struct radix_tree_node *_child,
			 unsigned long *startp, unsigned long end, void *item)
{
	struct radix_tree_node *node = NULL;
	unsigned int offset = 0U;
	unsigned int height = 0U;
	int rc = E_HM_OK;
	struct radix_tree_node *child = _child;

	height = child->height + 1U;
	while (height != 0U) {
		height -= 1U;
		if (child == NULL) {
			child = alloc_child_node(root, node, offset, height);
			if (child == NULL) {
				rollback_node_allocation(root, node);
				rc = E_HM_NOMEM;
				break;
			}
		}

		node = child;
		offset = __next_level_offset(node, *startp);
		if (raw_bitmap_test_bit(node->bitmap, RADIX_TREE_SLOTS_SIZE, offset) != 0) {
			node = __alloc_slot(root, node, &offset, &height);
			if (node == NULL) {
				rc = E_HM_NOMEM;
				break;
			}
			*startp = (*startp & (~__node_max_index(node))) +
				  __next_level_index(node, offset);
			if (*startp > end) {
				rc = E_HM_NOSPC;
				break;
			}
		}
		child = node->slots[offset];
	}

	if (rc == E_HM_OK) {
		BUG_ON(node == NULL);
		__node_set_bit_upward(root, node, offset);
		node->slots[offset] = item;
		node->count++;
	}

	return rc;
}

/*
 * radix_tree_alloc_slot: alloc a free slot to insert an item
 *			  within the range of @startp to @end
 *
 * @root: which radix-tree to alloc
 * @startp: the id associated with the pointer
 * @end: the maximum id that can be alloced
 * @item: the item ready to be inserted
 * Returns: E_HM_OK for alloc success, or other return value for failed
 */
int radix_tree_alloc_slot(struct radix_tree_root *root, unsigned long *startp,
			  unsigned long end, void *item)
{
	struct radix_tree_node *child = NULL;
	bool is_rnode_allocated = false;
	int rc = E_HM_OK;

	if ((root == NULL) || (startp == NULL) || (item == NULL)) {
		rc = E_HM_INVAL;
	}

	if (rc == E_HM_OK && root->rnode == NULL) {
		root->rnode = alloc_child_node(root, NULL, 0U, 0U);
		if (root->rnode == NULL) {
			rc = E_HM_NOMEM;
		} else {
			is_rnode_allocated = true;
		}
	}
	if (rc == E_HM_OK && !root->has_free) {
		unsigned long max_index = __node_max_index(root->rnode);
		*startp = max(*startp, max_index + 1U);
	}
	if (rc == E_HM_OK && *startp > end) {
		rc = E_HM_NOSPC;
	}
	if (rc == E_HM_OK && root->rnode != NULL) {
		child = root->rnode;
		if (*startp > __node_max_index(child)) {
			child = __radix_tree_grow(root, *startp);
			if (child == NULL) {
				rc = E_HM_NOMEM;
			}
		}
	}

	if (rc == E_HM_OK && child != NULL) {
		rc = do_alloc_slot(root, child, startp, end, item);
	}

	if (rc != E_HM_OK && is_rnode_allocated) {
		radix_tree_destroy(root);
	}
	return rc;
}

static int do_insert_item(struct radix_tree_root *root,
			  struct radix_tree_node *_child,
			  unsigned long index, void *item)
{
	struct radix_tree_node *node = NULL;
	unsigned int offset = 0;
	unsigned int height;
	int rc = E_HM_OK;
	struct radix_tree_node *child = _child;

	height = child->height + 1U;
	while (height != 0U) {
		height -= 1U;
		if (child == NULL) {
			child = alloc_child_node(root, node, offset, height);
			if (child == NULL) {
				rollback_node_allocation(root, node);
				rc = E_HM_NOMEM;
				break;
			}
		}

		node = child;
		offset = __next_level_offset(node, index);
		/* can't insert an item to a non-empty slot */
		if (raw_bitmap_test_bit(node->bitmap, RADIX_TREE_SLOTS_SIZE, offset) != 0) {
			rc = E_HM_OBJEXIST;
			break;
		}
		child = node->slots[offset];
	}

	if (rc == E_HM_OK) {
		__node_set_bit_upward(root, node, offset);
		node->slots[offset] = item;
		node->count++;
	}

	return rc;
}

int radix_tree_insert_item(struct radix_tree_root *root,
			   unsigned long index, void *item)
{
	struct radix_tree_node *child = NULL;
	bool is_rnode_allocated = false;
	int rc = E_HM_OK;

	if ((root == NULL) || (item == NULL)) {
		rc = E_HM_INVAL;
	}

	if (rc == E_HM_OK && root->rnode == NULL) {
		root->rnode = alloc_child_node(root, NULL, 0, 0);
		if (root->rnode == NULL) {
			rc = E_HM_NOMEM;
		} else {
			is_rnode_allocated = true;
		}
	}

	if (rc == E_HM_OK && root->rnode != NULL) {
		child = root->rnode;
		if (index > __node_max_index(child)) {
			child = __radix_tree_grow(root, index);
			if (child == NULL) {
				rc = E_HM_NOMEM;
			}
		}
	}

	if (rc == E_HM_OK && child != NULL) {
		rc = do_insert_item(root, child, index, item);
	}

	if (rc != E_HM_OK && is_rnode_allocated) {
		radix_tree_destroy(root);
	}
	return rc;
}

/*
 * __radix_tree_find_node tries to traverse the tree to get items until
 * it cannot traverse and return to the bottom node
 */
static struct radix_tree_node *__radix_tree_find_node(const struct radix_tree_root *root,
						      unsigned long index)
{
	struct radix_tree_node *node = root->rnode;
	struct radix_tree_node *ret_node = NULL;
	unsigned int offset = 0;

	if ((root->rnode != NULL) && (index <= __node_max_index(node))) {
		for (;;) {
			offset = __next_level_offset(node, index);
			if (node->slots[offset] == NULL || node->height == 0U) {
				ret_node = node;
				break;
			}
			node = node->slots[offset];
		}
	}
	return ret_node;
}

void *radix_tree_find_item(const struct radix_tree_root *root, unsigned long index)
{
	struct radix_tree_node *node = NULL;
	unsigned int offset = 0U;
	void *find_item = NULL;

	if (root != NULL) {
		node = __radix_tree_find_node(root, index);
		if (node != NULL) {
			offset = __next_level_offset(node, index);
			if (node->height == 0U && node->slots[offset] != NULL) {
				find_item = node->slots[offset];
			}
		}
	}
	return find_item;
}

void *radix_tree_replace_item(struct radix_tree_root *root, void *item, unsigned long index)
{
	struct radix_tree_node *node = NULL;
	unsigned int offset = 0U;
	void *old_item = NULL;

	if ((root != NULL) && (item != NULL)) {
		node = __radix_tree_find_node(root, index);
		if (node != NULL) {
			offset = __next_level_offset(node, index);
			if (node->height == 0U && node->slots[offset] != NULL) {
				old_item = node->slots[offset];
				node->slots[offset] = item;
			}
		}
	}
	return old_item;
}

static void __node_delete_upward(struct radix_tree_root *root,
				 struct radix_tree_node *_node, unsigned int _offset)
{
	unsigned int offset = _offset;
	struct radix_tree_node *node = _node;
	for (;;) {
		node->slots[offset] = NULL;
		node->count--;

		if (node->count != 0U) {
			__node_clear_bit_upward(root, node, offset);
			for (unsigned int tag = 0; tag < __NR_TAGS; tag++) {
				__node_clear_tag_upward(node, tag, offset);
			}
			break;
		}

		offset = node->offset;
		struct radix_tree_node *parent = node->parent;
		free_node(root, node);
		node = parent;
		if (node == NULL) {
			root->rnode = NULL;
			root->has_free = true;
			break;
		}
	}
}

void *radix_tree_delete_item(struct radix_tree_root *root, unsigned long index)
{
	struct radix_tree_node *node = NULL;
	unsigned int offset = 0U;
	void *item = NULL;

	if (root != NULL) {
		node = __radix_tree_find_node(root, index);
		if (node != NULL) {
			offset = __next_level_offset(node, index);
			if (node->height == 0U && node->slots[offset] != NULL) {
				item = node->slots[offset];
				__node_delete_upward(root, node, offset);
			}
		}
	}
	return item;
}

/*
 * Get the next exist slots of offset >= @offset.
 */
static unsigned int __next_slot(const struct radix_tree_node *node, unsigned int offset)
{
	unsigned int i = offset;
	for (; i < RADIX_TREE_SLOTS_SIZE; i++) {
		if (node->slots[i] != NULL) {
			break;
		}
	}
	return i;
}

/*
 * Get the first index and the node which the first index located.
 */
static struct radix_tree_node *__get_first_item_node(const struct radix_tree_root *root, unsigned long *index)
{
	struct radix_tree_node *node = root->rnode;
	struct radix_tree_node *ret_node = NULL;
	unsigned int first_offset = 0U;

	for (;;) {
		first_offset = __next_slot(node, 0);
		if (first_offset == RADIX_TREE_SLOTS_SIZE) {
			break;
		}

		*index += __next_level_index(node, first_offset);
		if (node->height == 0U) {
			ret_node = node;
			break;
		}
		node = node->slots[first_offset];
	}

	return ret_node;
}

/*
 * Get the next index and the node which the next index located which startd by current_node
 */
static struct radix_tree_node *__get_next_item_node(struct radix_tree_node *current_node, unsigned long *index)
{
	struct radix_tree_node *node = current_node;
	unsigned int current_offset = __next_level_offset(node, *index);
	unsigned int next_offset = current_offset + 1U;

	*index -= __next_level_index(node, current_offset);
	for (;;) {
		while (next_offset == RADIX_TREE_SLOTS_SIZE) {
			struct radix_tree_node *parent = node->parent;
			if (parent == NULL) {
				__hmattr_sideff_resolved("no relative side effect");
				return NULL;
			}
			next_offset = node->offset;
			node = node->parent;
			*index -= __next_level_index(node, next_offset);
			next_offset++;
		}

		void *child = node->slots[next_offset];
		if (child != NULL) {
			*index += __next_level_index(node, next_offset);
			if (node->height > 0U) {
				node = (struct radix_tree_node *)child;
				next_offset = 0U;
				continue;
			} else {
				__hmattr_sideff_resolved("no relative side effect");
				return node;
			}
		}
		next_offset++;
	}
}

void *radix_tree_get_next(const struct radix_tree_root *root, unsigned long *index)
{
	void *ret = NULL;
	struct radix_tree_node *node = NULL;
	unsigned int offset = 0;

	if (root != NULL && index != NULL) {
		node = __radix_tree_find_node(root, *index);
	}

	if (node != NULL) {
		offset = __next_level_offset(node, *index);
		*index = (*index & (~__node_max_index(node))) +
			 __next_level_index(node, offset);

		if (node->height == 0U && node->slots[offset] != NULL) {
			ret = node->slots[offset];
		}
	}

	if (node != NULL && ret == NULL) {
		node = __get_next_item_node(node, index);
		if (node != NULL) {
			offset = __next_level_offset(node, *index);
			ret = node->slots[offset];
		}
	}
	return ret;
}

static int __find_items(const struct radix_tree_root *root,
			unsigned long *start, int nr, void **items)
{
	struct radix_tree_node *node = NULL;
	unsigned int offset = 0;
	int rc = 0;
	bool done = false;

	node = __radix_tree_find_node(root, *start);
	if (node != NULL) {
		offset = __next_level_offset(node, *start);
		*start = (*start & (~__node_max_index(node))) +
			 __next_level_index(node, offset);
		if (node->height == 0U && node->slots[offset] != NULL) {
			items[rc] = node->slots[offset];
			rc++;
		}

		while (rc < nr) {
			node = __get_next_item_node(node, start);
			if (node == NULL) {
				*start = 0;
				done = true;
				break;
			}
			offset = __next_level_offset(node, *start);
			items[rc] = node->slots[offset];
			rc++;
		}

		if (!done) {
			if (*start < __node_max_index(root->rnode)) {
				(*start)++;
			} else {
				*start = 0;
			}
		}
	}
	return rc;
}

/*
 * radix_tree_find_items() - find up to @nr items.
 * @root:  Radix tree root.
 * @start: The index to start searching.
 *         Return an index that may be used in the next search.
 *         If the end of the tree is searched, 0 will be returned.
 * @nr:    Search up to @nr items.
 * @items: Place to store the items.
 *
 * Return: The number of items that have been searched.
 */
int radix_tree_find_items(const struct radix_tree_root *root,
			  unsigned long *start, int nr, void **items)
{
	int err = E_HM_OK;

	if (root == NULL || start == NULL || items == NULL || nr < 0) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		if (nr == 0) {
			err = 0;
		} else {
			err = __find_items(root, start, nr, items);
		}
	}
	return err;
}

/*
 * radix_tree_apply: Travel this radix_tree to get all item and index
 *
 * @root: which radix-tree to travel
 * @callback: The callback function will be called for each index in @root
 *	      passing the index, the item and @data.
 *	      @index assosiate with the @item and the @data of
 *	      callback is from the data argument of radix_tree_apply.
 * @data: Data passed to callback function.
 */
int radix_tree_apply(struct radix_tree_root *root,
		     int (*callback)(unsigned long index, void *item, const void *data), void *data)
{
	unsigned int offset = 0U;
	unsigned int pre_offset = 0U;
	unsigned long index = 0U;
	unsigned long pre_index = 0U;
	int rc = E_HM_OK;

	if (root == NULL || callback == NULL) {
		rc = E_HM_INVAL;
	} else if (root->rnode != NULL) {
		struct radix_tree_node *node = NULL;
		struct radix_tree_node *next = NULL;

		node = __get_first_item_node(root, &index);
		while (node != NULL) {
			offset = __next_level_offset(node, index);
			pre_offset = offset;
			pre_index = index;
			next = __get_next_item_node(node, &index);
			rc = callback(pre_index, node->slots[pre_offset], data);
			if (rc != E_HM_OK) {
				break;
			}
			node = next;
		}
	} else {
		/* root->rnode is NULL */
		/* do nothings */
	}

	return rc;
}

void radix_tree_destroy(struct radix_tree_root *root)
{
	struct radix_tree_node *node = NULL;
	unsigned int offset = 0U;

	if ((root == NULL) || (root->rnode == NULL)) {
		__hmattr_sideff_resolved("no relative side effect");
		return;
	}

	node = root->rnode;
	for (;;) {
		struct radix_tree_node *child = node->slots[offset];
		if ((child != NULL) && (node->height > 0U)) {
			node = child;
			offset = 0U;
			continue;
		}

		offset++;

		while (offset == RADIX_TREE_SLOTS_SIZE) {
			struct radix_tree_node *parent = node;
			offset = node->offset + 1U;
			node = node->parent;
			free_node(root, parent);
			if (parent == root->rnode) {
				root->rnode = NULL;
				root->has_free = true;
				__hmattr_sideff_resolved("no relative side effect");
				return;
			}
		}
	}
}

bool radix_tree_tagged(const struct radix_tree_root *root, unsigned int tag)
{
	BUG_ON(tag >= __NR_TAGS);
	return !(root == NULL) &&
	       !radix_tree_is_empty(root) &&
	       is_node_tagged(root->rnode, tag);
}

void *radix_tree_tag_set(const struct radix_tree_root *root,
			 unsigned long index, unsigned int tag)
{
	struct radix_tree_node *curr = NULL;
	unsigned int cycles = 0;
	unsigned int offset = 0;
	bool empty = false;

	if (root != NULL && tag < __NR_TAGS) {
		/* The caller must ensure that @index is already bound to an item. */
		empty = radix_tree_is_empty(root);
		BUG_ON(empty);
		curr = root->rnode;

		for (cycles = curr->height + 1U; cycles > 0U; cycles--) {
			BUG_ON(curr == NULL);

			offset = __next_level_offset(curr, index);
			node_tag_set(curr, tag, offset);
			curr = curr->slots[offset];
		}
	}

	return curr;
}

void *radix_tree_tag_clear(const struct radix_tree_root *root,
			   unsigned long index, unsigned int tag)
{
	struct radix_tree_node *node = NULL;
	struct radix_tree_node *item = NULL;
	unsigned int offset = 0U;

	if (tag < __NR_TAGS && radix_tree_tagged(root, tag)) {
		node = __radix_tree_find_node(root, index);
		if (node != NULL) {
			offset = __next_level_offset(node, index);
			if (node_tag_test(node, tag, offset) != 0) {
				item = node->slots[offset];
				__node_clear_tag_upward(node, tag, offset);
			}
		}
	}
	return item;
}

int radix_tree_tag_test(const struct radix_tree_root *root,
			unsigned long index, unsigned int tag)
{
	struct radix_tree_node *node = NULL;
	unsigned int offset = 0U;
	int rc = 0;

	if (tag < __NR_TAGS && radix_tree_tagged(root, tag)) {
		node = __radix_tree_find_node(root, index);
		if (node != NULL) {
			offset = __next_level_offset(node, index);
			rc = node_tag_test(node, tag, offset);
		}
	}
	return rc;
}

static struct radix_tree_node *
__radix_tree_find_tagged_leaf(const struct radix_tree_root *root,
			      unsigned long index, unsigned int tag)
{
	struct radix_tree_node *curr = root->rnode;
	struct radix_tree_node *rc = NULL;
	unsigned int cycles = 0;
	unsigned int offset = 0;

	if (index <= __node_max_index(curr)) {
		for (cycles = curr->height + 1U; cycles > 0U; cycles--) {
			if (curr == NULL) {
				rc = NULL;
				break;
			}

			offset = __next_level_offset(curr, index);
			if (node_tag_test(curr, tag, offset) == 0) {
				rc = NULL;
				break;
			}
			rc = curr;
			curr = curr->slots[offset];
		}
	}

	return rc;
}

static void *
__first_tagged_item(const struct radix_tree_root *root, unsigned long *start,
		    unsigned int tag, struct radix_tree_iterator *iter)
{
	unsigned long max_index = __node_max_index(root->rnode);
	struct radix_tree_node *rc = NULL;
	unsigned long idx = 0UL;
	void *ret_item = NULL;

	for (idx = *start; idx <= max_index; idx++) {
		rc = __radix_tree_find_tagged_leaf(root, idx, tag);
		if (rc != NULL) {
			iter->leaf = rc;
			iter->offset = __next_level_offset(rc, idx);
			*start = idx;
			ret_item = rc->slots[iter->offset];
			break;
		}
	}

	return ret_item;
}

static void *
__next_tagged_item(struct radix_tree_iterator *iter, unsigned long *start,
		   unsigned int tag)
{
	struct radix_tree_node *curr = iter->leaf;
	unsigned int offset = iter->offset + 1U;
	unsigned long next_index = *start;

	next_index -= __next_level_index(curr, iter->offset);
	while (curr != NULL) {
		offset = node_tag_search_one(curr, tag, offset);
		if (offset == RADIX_TREE_SLOTS_SIZE) {
			if (curr->parent != NULL) {
				next_index -= __next_level_index(curr->parent,
								 curr->offset);
			}
			offset = curr->offset + 1U;
			curr = curr->parent;
			continue;
		}
		next_index += __next_level_index(curr, offset);
		if (curr->height == 0U) {
			iter->leaf = curr;
			iter->offset = offset;
			*start = next_index;
			curr = curr->slots[offset];
			break;
		}
		curr = curr->slots[offset];
		offset = 0;
	}

	return curr;
}
/*
 * radix_tree_find_get_node_tag() - find up to @nr tagged items.
 * @root:  Radix tree root.
 * @start: The index to start searching.
 *         Return an index that may be used in the next search.
 *         If the end of the tree is searched, 0 will be returned.
 * @tag:   Value of tag.
 * @nr:    Search up to @nr tagged items.
 * @items: Place the tagged items.
 *
 * Return: The number of tagged items that have been searched.
 */
int radix_tree_find_tagged_items(const struct radix_tree_root *root,
				 unsigned long *start, unsigned int tag,
				 int nr, void **items)
{
	struct radix_tree_iterator iter;
	void *item = NULL;
	int rc = 0;

	if (root == NULL || start == NULL || items == NULL ||
	    nr < 0 || tag >= __NR_TAGS) {
		__hmattr_sideff_resolved("no relative side effect");
		return E_HM_INVAL;
	}

	if (!radix_tree_tagged(root, tag) || nr == 0) {
		__hmattr_sideff_resolved("no relative side effect");
		return 0;
	}

	if (*start > __node_max_index(root->rnode)) {
		__hmattr_sideff_resolved("no relative side effect");
		return 0;
	}

	mem_zero_s(iter);
	item = __first_tagged_item(root, start, tag, &iter);
	while (item != NULL) {
		items[rc] = item;
		rc++;
		if (rc == nr) {
			break;
		}
		item = __next_tagged_item(&iter, start, tag);
	}

	if (iter.leaf != NULL) {
		item = __get_next_item_node(iter.leaf, start);
		if (item == NULL) {
			*start = 0;
		}
	}
	return rc;
}

size_t struct_radix_tree_node_size(void)
{
	return sizeof(struct radix_tree_node);
}