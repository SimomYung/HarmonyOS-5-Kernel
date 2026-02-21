/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Internal header file for radix tree
 * Author: Huawei OS Kernel Lab
 * Create: Fri Mar 24 18:24:18 2023
 */

#ifndef RADIXTREE_INTERNEL_H
#define RADIXTREE_INTERNEL_H

#include <libhmsync/bitops/generic.h>

#ifdef CONFIG_RADIX_TREE_SHIFT_SIZE
#define RADIX_TREE_SHIFT_SIZE CONFIG_RADIX_TREE_SHIFT_SIZE
#else
#define RADIX_TREE_SHIFT_SIZE 4
#endif

#define RADIX_TREE_SLOTS_SIZE (1U << RADIX_TREE_SHIFT_SIZE)
#define RADIX_TREE_MASK (RADIX_TREE_SLOTS_SIZE - 1U)

#define __NR_TAGS 3U

#define DIV_ROUND_UP(n, d) (((n) + (d) - 1) / (d))

#define RADIX_TREE_INDEX_BITS  (8 * sizeof(unsigned long)) /* 8 is CHAR_BITS */

#define RADIX_TREE_MAX_PATH (DIV_ROUND_UP(RADIX_TREE_INDEX_BITS, RADIX_TREE_SHIFT_SIZE))

/*
 * The radix tree is variable-height
 * so an insert operation will build the branch to its corresponding item,
 * and it also need build the branch to existing items if the size has to be increased (by radix_tree_grow)
 *
 * The worst case is a zero height tree with only a single item at index 0,
 * and then insert a item at index ULONG_MAX. This requires 2 new branches
 * of RADIX_TREE_MAX_PATH size to be created, with only the root node shared.
 */
#define RADIX_TREE_PRELOAD_SIZE (RADIX_TREE_MAX_PATH * 2 - 1)

/*
 * struct radix_tree_node
 * @height: Height of the node.
 * @offset: Offset in parent slots array.
 * @count:  Count of child.
 * @parent: Parent node.
 * @bitmap: To describe the slot array.
 *	    If node is a leaf(height equals to 0), it indicates whether there
 *	    is an item, else it indicates whether the child node is full.
 * @tags:   Bitmaps of tags.
 * slots:   Slot to store child node or item.
 */
struct radix_tree_node {
	/* should have the same layout with __ctrlmem_kobj_fspgcache_rdtnode_s */
	unsigned int height;
	unsigned int offset;
	unsigned int count;
	struct radix_tree_node *parent;
	void *slots[RADIX_TREE_SLOTS_SIZE];
	DEFINE_RAW_BITMAP(bitmap, RADIX_TREE_SLOTS_SIZE);
	unsigned long long tags[__NR_TAGS][BITS_TO_LONGLONGS(RADIX_TREE_SLOTS_SIZE)];
};

#endif
