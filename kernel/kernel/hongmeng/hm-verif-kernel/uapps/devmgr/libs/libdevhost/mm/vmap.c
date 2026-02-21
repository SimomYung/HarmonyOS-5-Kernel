/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Virtual address space management
 * Author: Huawei OS Kernel Lab
 * Create: Wed Apr 22 17:08:17 2020
 */
#include <libdevhost/mm.h>
#include "vmap_internal.h"

#include <libmem/utils.h>
#include <libhmsync/raw_mutex_guard.h>

static struct rbt_tree __vmap_tree = RBT_ROOT;
static struct raw_mutex __vmap_lock = RAW_MUTEX_INITIALIZER;
DEFINE_rbt_search(vmap, struct libdh_vmap_node, node, vaddr);

static int __do_vmap_insert(struct libdh_vmap_node *vnode)
{
	int ret = E_HM_OK;
	struct rbt_node *p = __vmap_tree.root;
	struct libdh_vmap_node *cur = NULL;
	bool is_right = false;

	RAW_MUTEX_GUARD(_, &__vmap_lock);

	while (p != NULL) {
		cur = rbt_entry(p, struct libdh_vmap_node, node);
		if (ptr_to_ulong(vnode->vaddr) > ptr_to_ulong(cur->vaddr)) {
			if (p->rbt_right == NULL) {
				is_right = true;
				break;
			}
			p = p->rbt_right;
		} else if (ptr_to_ulong(vnode->vaddr) < ptr_to_ulong(cur->vaddr)) {
			if (p->rbt_left == NULL) {
				is_right = false;
				break;
			}
			p = p->rbt_left;
		} else {
			ret = E_HM_OBJEXIST;
			break;
		}
	}

	if (ret == E_HM_OK) {
		rbt_insert(&__vmap_tree, &vnode->node, p, is_right);
	}

	return ret;
}

int libdh_vmap_insert(struct libdh_vmap_node *vnode)
{
	int ret = E_HM_OK;

	if ((vnode == NULL) || (vnode->release == NULL) ||
	    !PAGE_ALIGNED(vnode->vaddr) || !PAGE_ALIGNED(vnode->size)) {
		ret = E_HM_INVAL;
	} else {
		ret = __do_vmap_insert(vnode);
	}

	return ret;
}

struct libdh_vmap_node *__libdh_vmap_remove(const void *vaddr)
{
	struct libdh_vmap_node *vnode = NULL;

	RAW_MUTEX_GUARD(_, &__vmap_lock);

	vnode = vmap_rbt_search(&__vmap_tree, vaddr, NULL);
	if (vnode != NULL) {
		rbt_remove(&__vmap_tree, &vnode->node);
	}
	return vnode;
}
