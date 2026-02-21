/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 * Description: Headerfile for ebr-safe radix_tree
 * Author: Huawei OS Kernel Lab
 * Create: Tue Oct 20 19:35:06 2020
 */
#ifndef ULIBS_INCLUDE_EBR_RADIX_TREE_H
#define ULIBS_INCLUDE_EBR_RADIX_TREE_H

#include <libalgo/radix_tree.h>
#include <libhmebr/ebr_singleton.h>
#include <libhmebr/ebr.h>

/*
 * The EBR-safe Radix tree assumes tree traversing without any
 * blocking synchronization. Only EBR guards should be used
 * on readers side to prevent reclamation of currently visible
 * nodes.
 * Updates should be done using blocking synchronization such
 * as lock, mutexes and etc. Updates are non-destructive for
 * concurrent readers, readers can perfectly run in parallel
 * with any updater, but concurrent updates without proper
 * synchronization are prohibited and can lead to inconsistency
 * in Radix tree or even to application crash.
 * Here is the list of functions that can be safely used on
 * readers side, protected only by EBR guards:
 *
 * ebr_radix_tree_is_empty()
 * ebr_radix_tree_find_item()
 * ebr_radix_tree_get_next()
 * ebr_radix_tree_find_items()
 * ebr_radix_tree_find_tagged_items()
 * ebr_radix_tree_tagged()
 * ebr_radix_tree_tag_test()
 *
 * The caller must ensure that these functions are called
 * under EBR guards. If so, any modifications of the tree
 * can be done concurrently with these functions execution.
 * These functions doesn't guarantee data consistency for
 * tree items. It is assumed that items should have own
 * synchronization to provide data consistency for them.
 *
 * Notes:
 * The result of ebr_radix_tree_tagged(),
 * ebr_radix_tree_tag_test(), ebr_radix_tree_find_tagged_items()
 * may be affected by concurrent tags modifications.
 * The ebr_radix_tree_find_items() and ebr_radix_tree_find_tagged_items()
 * doesn't provide an atomic snapshot of the tree. The result may be
 * affected by functions that modify tree tags or functions that delete
 * tree items. These functions are implemented in a way when multiple
 * lookups with individual locks are performed to search items.
 * The ebr_radix_tree_for_each() and ebr_radix_tree_for_each_tagged()
 * implemented in the same way as ebr_radix_tree_find_items() and
 * ebr_radix_tree_find_tagged_items() and can restart lookup from
 * the tree root in case of index reached node boundary.
 *
 * For all EBR-safe functions concurrently inserted items can be
 * missed in lookup. This is expected due to this is the same story
 * as in case when lookup under lock is executed before insertion and
 * newly inserted item was not observed.
 */

/*
 * Initialization macro of ebr-safe radix tree,
 * must take an epoch set as parameter.
 */
#define EBR_RADIX_TREE_INIT(epoch_set) {	\
	.eset = (epoch_set),			\
	.root = RADIX_TREE_INIT			\
}

struct ebr_radix_tree_root {
	struct epoch_set *eset;
	struct radix_tree_root root;
};

size_t ebr_radix_tree_node_size(void);
void *ebr_radix_tree_node_zmalloc(enum ebr_mcache_type type);
void ebr_radix_tree_node_free(void *ptr, enum ebr_mcache_type type);
int ebr_radix_tree_init(struct ebr_radix_tree_root *eroot, struct epoch_set *es);
void ebr_radix_tree_destroy(struct ebr_radix_tree_root *eroot);
int ebr_radix_tree_insert_item(struct ebr_radix_tree_root *eroot,
			       unsigned long index, void *item);
void *ebr_radix_tree_find_item(struct ebr_radix_tree_root *eroot,
			       unsigned long index);
void *ebr_radix_tree_delete_item(struct ebr_radix_tree_root *eroot,
				 unsigned long index);
void *ebr_radix_tree_get_next(struct ebr_radix_tree_root *eroot,
			      unsigned long *index);
int ebr_radix_tree_apply(struct ebr_radix_tree_root *eroot,
			 int (*callback)(unsigned long index, void *item, void *data),
			 void *data);
void *ebr_radix_tree_replace_item(struct ebr_radix_tree_root *eroot,
				  void *item, unsigned long index);
int ebr_radix_tree_alloc_slot(struct ebr_radix_tree_root *eroot,
			      unsigned long *startp,
			      unsigned long end, void *item);
int ebr_radix_tree_find_items(struct ebr_radix_tree_root *eroot,
			      unsigned long *start, int nr, void **items, unsigned long *indexs);
int ebr_radix_tree_find_items_match(struct ebr_radix_tree_root *eroot,
	unsigned long *start, int nr, void **items, unsigned long *indexs, bool (*match)(void *item));
bool ebr_radix_tree_tagged(struct ebr_radix_tree_root *eroot, unsigned int tag);
void *ebr_radix_tree_tag_set(struct ebr_radix_tree_root *eroot,
			     unsigned long index, unsigned int tag);
void *ebr_radix_tree_tag_clear(struct ebr_radix_tree_root *eroot,
			       unsigned long index, unsigned int tag);
int ebr_radix_tree_tag_test(struct ebr_radix_tree_root *eroot,
			    unsigned long index, unsigned int tag);
int ebr_radix_tree_find_tagged_items(struct ebr_radix_tree_root *eroot,
				     unsigned long *start, unsigned int tag,
				     int nr, void **items);
int ebr_radix_tree_find_tagged_items_match(struct ebr_radix_tree_root *eroot,
				     unsigned long *start, unsigned int tag,
				     int nr, void **items, bool (*match)(void *item));

#define ebr_radix_tree_is_empty(eroot)				\
({								\
	BUG_ON((eroot) == NULL);				\
	(_Bool)(ebr_dereference((eroot)->root.rnode) == NULL);	\
})

#endif
