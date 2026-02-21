/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Headerfile for radix_tree
 * Author: Huawei OS Kernel Lab
 * Create: Tue Sep 03 22:21:06 2019
 */
#ifndef ULIBS_LIBALGO_RADIX_TREE_H
#define ULIBS_LIBALGO_RADIX_TREE_H

#include <stdbool.h>
#include <hongmeng/types.h>
#include <libhmsync/raw_mutex.h>
struct radix_tree_root {
	void *rnode;
	bool has_free;
	bool use_pool;
};

#define RADIX_TREE_INIT {	\
	.rnode = NULL,		\
	.has_free = __true,	\
	.use_pool = __false	\
}

static inline void radix_tree_init(struct radix_tree_root *root)
{
	if (root != NULL) {
		root->rnode = NULL;
		root->has_free = __true;
		root->use_pool = __false;
	}
}

int  radix_tree_pool_init(struct radix_tree_root *root, int nr_pools);
/*
 * if charge && percpu_pool->nr < RADIX_TREE_PRELOAD_SIZE, fill the missing node to pool.
 * On success, return zero, with preemption disabled.
 * On error, return -ENOMEM, with preemption not disabled.
 */
int  radix_tree_pool_begin(struct radix_tree_root *root, bool charge);
/*
 * if percpu_pool->nr > RADIX_TREE_PRELOAD_SIZE, drop the excess node from pool.
 * On success, return zero, with preemption not disabled.
 */
void radix_tree_pool_end(struct radix_tree_root *root);
bool radix_tree_is_empty(const struct radix_tree_root *root);
int radix_tree_alloc_slot(struct radix_tree_root *root, unsigned long *startp, unsigned long end, void *item);
int radix_tree_insert_item(struct radix_tree_root *root, unsigned long index, void *item);
void *radix_tree_find_item(const struct radix_tree_root *root, unsigned long index);
void *radix_tree_delete_item(struct radix_tree_root *root, unsigned long index);
void radix_tree_destroy(struct radix_tree_root *root);
void *radix_tree_replace_item(struct radix_tree_root *root, void *item, unsigned long index);
int radix_tree_apply(struct radix_tree_root *root,
		     int (*callback)(unsigned long index, void *item, const void *data),
		     void *data);
void *radix_tree_get_next(const struct radix_tree_root *root, unsigned long *index);
int radix_tree_find_items(const struct radix_tree_root *root,
			  unsigned long *start, int nr, void **items);

bool radix_tree_tagged(const struct radix_tree_root *root, unsigned int tag);
void *radix_tree_tag_set(const struct radix_tree_root *root,
			 unsigned long index, unsigned int tag);
void *radix_tree_tag_clear(const struct radix_tree_root *root,
			   unsigned long index, unsigned int tag);
int radix_tree_tag_test(const struct radix_tree_root *root,
			unsigned long index, unsigned int tag);
int radix_tree_find_tagged_items(const struct radix_tree_root *root,
				 unsigned long *start, unsigned int tag,
				 int nr, void **items);
void *radix_tree_node_malloc(void);
void radix_tree_node_free(void *ptr);
size_t struct_radix_tree_node_size(void);

#endif
