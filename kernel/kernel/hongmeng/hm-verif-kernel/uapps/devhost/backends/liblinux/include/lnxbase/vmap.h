/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Prototypes for liblinux virtual address space management
 * Author: Huawei OS Kernel Lab
 * Create: Wed Apr 22 17:08:17 2020
 */
#ifndef __DEVHOST_LIBLINUX_VMAP_H__
#define __DEVHOST_LIBLINUX_VMAP_H__

#include <libhmsync/raw_mutex.h>
#include <libhmsync/raw_scopedptr.h>
#include <libalgo/rbtree.h>
#include <libmem/flags.h>
#include <liblinux/pal.h>

#define FLAG_VMAP_MERGED 0x0001
#define VMAP_REF_GET 0
#define VMAP_REF_PUT 1
#define VMAP_MERGED_DONE 1
#define VMAP_MERGED_NONE 0

/* flags value equal to VM_* in linux */
#define VMAP_TYPE_IOREMAP 0x1 /* ioremap */
#define VMAP_TYPE_ALLOC 0x2 /* vmalloc */
#define VMAP_TYPE_MAP 0x4 /* vmap page */
#define VMAP_TYPE_USERMAP 0x8 /* user map is not supported */
/* in linux, this flag is used when vmap dma coherent area */
#define VMAP_TYPE_DMA_COHERENT 0x10
/* flags not exist in linux */
#define VMAP_TYPE_KMAP		0x10000000 /* for kmap */
#define VMAP_TYPE_DMA		0x08000000 /* for dma non coherent */
#define VMAP_TYPE_VMEMMAP	0x04000000 /* for vmemmap */

struct vmap_tree {
	struct rbt_tree base;
	struct raw_mutex lock;
	bool readonly;
};

#define VMAP_TREE_INITIALIZER {		\
	.base = RBT_ROOT,		\
	.lock = RAW_MUTEX_INITIALIZER,	\
	.readonly = false,		\
}

#define DEFINE_VMAP_TREE(name) \
	struct vmap_tree name = VMAP_TREE_INITIALIZER

struct vmap_node {
	struct rbt_node node;
	const void *vaddr; /* key: aligned vaddr */
	unsigned long size;
	uint32_t flags;
	unsigned long type;
	struct raw_refcnt_nr refcnt;
	int (*release)(struct vmap_node *node);
	const void *caller;
};

struct vmap_range {
	const void *vaddr;
	unsigned long size;
};

static inline void vmap_tree_set_readonly(struct vmap_tree *vtree)
{
	BUG_ON(vtree->readonly);
	vtree->readonly = true;
}

static inline uint32_t trans_vmflag_to_mapflag(const unsigned long vm_flags)
{
	if ((vm_flags & LIBLINUX_PAL_VM_USERMAP) != 0U) {
		return MFLAG_MAP_VMUSER;
	} else {
		return 0U;
	}
}

int __vmap_tree_insert(struct vmap_tree *vtree, struct vmap_node *vnode);
int __vmap_tree_insert_merge(struct vmap_tree *vtree, struct vmap_node *vnode);
struct vmap_node *__vmap_tree_remove(struct vmap_tree *vtree, const void *addr);
struct vmap_node *__vmap_tree_find(struct vmap_tree *vtree, const void *addr);

int vmap_tree_insert(struct vmap_node *vnode);
struct vmap_node *vmap_tree_remove(const void *addr);
struct vmap_node *vmap_tree_find(const void *addr);
int vmap_tree_insert_merge(struct vmap_node *vnode);
int vmap_tree_remove_split(const void *start, unsigned long size);
int vmap_tree_refcnt_split(const void *start, unsigned long size, int flag);
int vmap_tree_show_all(char *info, unsigned long size, unsigned long *rsize);
int vmap_tree_foreach(int (*fn)(struct vmap_node *vnode, void *args), void *args);
int vmap_tree_foreach_trylock(int (*fn)(struct vmap_node *vnode, void *args), void *args);
struct vmap_node *vmap_get_vnode(unsigned long type, uint32_t flags);

struct ioremap_node {
	struct vmap_node vmap;
	unsigned long phys_addr;
};

#endif /* __DEVHOST_LIBLINUX_VMAP_H__ */
