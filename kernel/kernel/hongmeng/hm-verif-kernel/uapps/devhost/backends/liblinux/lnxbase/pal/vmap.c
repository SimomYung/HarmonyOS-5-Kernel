/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: liblinux virtual address space management
 * Author: Huawei OS Kernel Lab
 * Create: Wed Apr 22 17:08:17 2020
 */
#include <lnxbase/vmap.h>

#include <errno.h>
#include <stdlib.h>
#include <hmasm/page.h>
#include <hongmeng/types.h>
#include <libhmsync/raw_mutex_guard.h>
#include <libhmsync/raw_scopedptr.h>
#include <libhwsecurec/securec.h>
#include <devhost/log.h>

#define VMAP_TREE_BASE(tree) (&(tree)->base)

static DEFINE_VMAP_TREE(__default_vtree);
DEFINE_rbt_search(vmap, struct vmap_node, node, vaddr);

static int __vmap_tree_insert_nolock(struct vmap_tree *vtree, struct vmap_node *vnode)
{
	struct rbt_node **pnode = &VMAP_TREE_BASE(vtree)->root;
	struct rbt_node *parent = NULL;
	struct vmap_node *cur = NULL;
	bool is_right = false;

	if (vtree->readonly) {
		return -EINVAL;
	}

	if (vnode == NULL) {
		return -EINVAL;
	}

	while (*pnode != NULL) {
		cur = rbt_entry(*pnode, struct vmap_node, node);
		if (vnode->vaddr > cur->vaddr) {
			pnode = &((*pnode)->rbt_right);
		} else if (vnode->vaddr < cur->vaddr) {
			pnode = &((*pnode)->rbt_left);
		} else {
			return -EEXIST;
		}
	}

	parent = &cur->node;
	is_right = (&(parent->rbt_right) == pnode) ? true : false;

	rbt_insert(VMAP_TREE_BASE(vtree), &vnode->node, parent, is_right);

	return 0;
}

int __vmap_tree_insert(struct vmap_tree *vtree, struct vmap_node *vnode)
{
	RAW_MUTEX_GUARD(_, &vtree->lock);
	return __vmap_tree_insert_nolock(vtree, vnode);
}

int vmap_tree_insert(struct vmap_node *vnode)
{
	return __vmap_tree_insert(&__default_vtree, vnode);
}

static struct vmap_node *__vmap_tree_find_nolock(struct vmap_tree *vtree, const void *addr)
{
	struct rbt_node *p = VMAP_TREE_BASE(vtree)->root;
	struct vmap_node *cur = NULL;

	while (p != NULL) {
		cur = rbt_entry(p, struct vmap_node, node);
		if (addr >= cur->vaddr && addr < ptr_add(cur->vaddr, cur->size)) {
			return cur;
		}
		if (addr > cur->vaddr) {
			p = p->rbt_right;
		} else if (addr < cur->vaddr) {
			p = p->rbt_left;
		} else {
			return cur;
		}
	}

	return NULL;
}

struct vmap_node *__vmap_tree_find(struct vmap_tree *vtree, const void *addr)
{
	if (vtree->readonly) {
		return __vmap_tree_find_nolock(vtree, addr);
	}
	RAW_MUTEX_GUARD(_, &vtree->lock);
	return __vmap_tree_find_nolock(vtree, addr);
}

static int __vmap_tree_node_info(struct rbt_node *root, char *info,
				unsigned long size, unsigned long *len)
{
	struct vmap_node *cur = NULL;
	int refcnt;
	int ret;

	if (root == NULL) {
		return 0;
	}
	ret = __vmap_tree_node_info(root->rbt_left, info, size, len);
	if (ret < 0) {
		return ret;
	}

	cur = rbt_entry(root, struct vmap_node, node);
	refcnt = raw_refcnt_nr_counter(&cur->refcnt);
	ret = sprintf_s(info + *len, size - *len,
			"%#-16lx %#-16x %#-16x\n",
			cur->size,
			cur->flags,
			(unsigned int)refcnt);
	if (ret < 0) {
		return ret;
	}
	*len += (unsigned long)(unsigned int)ret;

	ret = __vmap_tree_node_info(root->rbt_right, info, size, len);
	if (ret < 0) {
		return ret;
	}

	return 0;
}

static int __vmap_tree_show_all(struct vmap_tree *vtree,
				char *info, unsigned long size, unsigned long *rsize)
{
	struct rbt_node *root = VMAP_TREE_BASE(vtree)->root;
	unsigned long len = 0;
	int ret;

	ret = sprintf_s(info, size,
			"%s%-16s %-16s %-16s\n",
			"vmap tree node info:\n",
			"size",	"flags", "refcnt");
	if (ret < 0) {
		return ret;
	}
	len += (unsigned long)(unsigned int)ret;

	ret = __vmap_tree_node_info(root, info, size, &len);
	if (ret < 0) {
		return ret;
	}
	info[len] = '\0';
	if (rsize != NULL) {
		*rsize = len;
	}

	return 0;
}

static struct vmap_node *
__vmap_tree_remove_nolock(struct vmap_tree *vtree, const void *addr)
{
	struct vmap_node *vnode = NULL;

	if (vtree->readonly) {
		return NULL;
	}

	vnode = vmap_rbt_search(VMAP_TREE_BASE(vtree), (void *)addr, NULL);
	if (vnode == NULL) {
		return NULL;
	}

	rbt_remove(VMAP_TREE_BASE(vtree), &vnode->node);

	return vnode;
}

struct vmap_node *__vmap_tree_remove(struct vmap_tree *vtree, const void *addr)
{
	RAW_MUTEX_GUARD(_, &vtree->lock);
	return __vmap_tree_remove_nolock(vtree, addr);
}

static struct vmap_node *__vmap_tree_node_merge(struct vmap_node *mnode, const struct vmap_node *snode)
{
	unsigned long mvaddr_end;
	unsigned long svaddr_end;

	if (mnode == NULL || snode == NULL) {
		return NULL;
	}

	if (mnode->type != snode->type) {
		panic("merge node of different type: %lx and %lx\n",
		      mnode->type, snode->type);
		return NULL;
	}

	mvaddr_end = ptr_to_ulong(mnode->vaddr) + mnode->size;
	svaddr_end = ptr_to_ulong(snode->vaddr) + snode->size;

	if (mnode->vaddr > snode->vaddr) {
		mnode->size += ptr_to_ulong(mnode->vaddr) - ptr_to_ulong(snode->vaddr);
		mnode->vaddr = snode->vaddr;
	}

	if (mvaddr_end < svaddr_end) {
		mnode->size += svaddr_end - mvaddr_end;
	}
	return mnode;
}

static int __vmap_tree_merge_try(struct vmap_tree *vtree, struct vmap_node *vnode, int *merged)
{
	struct vmap_node *pre = NULL;
	struct vmap_node *next = NULL;
	struct vmap_node *free_node = NULL;
	int merged_local = VMAP_MERGED_NONE;
	int ret;

	if (merged != NULL) {
		*merged = merged_local;
	}

	if (vnode == NULL) {
		dh_error("vmap merge failed, vnode is null\n");
		return -EINVAL;
	}

	pre = __vmap_tree_find_nolock(vtree, ulong_to_ptr(ptr_to_ulong(vnode->vaddr) - PAGE_SIZE, void));
	if (pre != NULL) {
		if ((pre->flags & FLAG_VMAP_MERGED) == FLAG_VMAP_MERGED
			&& raw_refcnt_nr_counter(&vnode->refcnt) == raw_refcnt_nr_counter(&pre->refcnt)) {
			(void)__vmap_tree_remove_nolock(vtree, vnode->vaddr);
			(void)__vmap_tree_node_merge(pre, vnode);
			free_node = vnode;
		}
	}

	next = __vmap_tree_find_nolock(vtree, ulong_to_ptr(ptr_to_ulong(vnode->vaddr) + vnode->size, void));
	if (next != NULL && (next->flags & FLAG_VMAP_MERGED) == FLAG_VMAP_MERGED
		&& raw_refcnt_nr_counter(&vnode->refcnt) == raw_refcnt_nr_counter(&next->refcnt)) {
		if (free_node != NULL) {
			(void)__vmap_tree_remove_nolock(vtree, next->vaddr);
			(void)__vmap_tree_node_merge(pre, next);
			next->release(next);
		} else {
			(void)__vmap_tree_remove_nolock(vtree, vnode->vaddr);
			(void)__vmap_tree_remove_nolock(vtree, next->vaddr);
			(void)__vmap_tree_node_merge(next, vnode);
			ret = __vmap_tree_insert_nolock(vtree, next);
			if (ret < 0) {
				vnode->release(vnode);
				return ret;
			}
			free_node = vnode;
		}
	}

	if (free_node != NULL) {
		free_node->release(free_node);
		merged_local = VMAP_MERGED_DONE;
	}

	if (merged != NULL) {
		*merged = merged_local;
	}
	return 0;
}

int __vmap_tree_insert_merge(struct vmap_tree *vtree, struct vmap_node *vnode)
{
	int ret;
	int merged = VMAP_MERGED_NONE;

	ret =__vmap_tree_merge_try(vtree, vnode, &merged);
	if (ret < 0) {
		return ret;
	}
	if (merged == VMAP_MERGED_DONE) {
		return 0;
	}

	return __vmap_tree_insert_nolock(vtree, vnode);
}

static int __vmap_tree_dup_node(struct vmap_tree *vtree,
				struct vmap_range range, int count,
				struct vmap_node *refnode)
{
	struct vmap_node *vnode = NULL;
	int ret;

	vnode = (struct vmap_node *)malloc(sizeof(struct vmap_node));
	if (vnode == NULL) {
		return -ENOMEM;
	}
	mem_zero_s(*vnode);

	vnode->vaddr = range.vaddr;
	vnode->size = range.size;
	vnode->flags = refnode->flags;
	vnode->type = refnode->type;
	vnode->release = refnode->release;
	raw_refcnt_nr_init(&vnode->refcnt, count);

	if (count == 0) {
		return vnode->release(vnode);
	}

	ret = __vmap_tree_insert_merge(vtree, vnode);
	if (ret < 0) {
		free(vnode);
	}
	return ret;
}

int vmap_tree_insert_merge(struct vmap_node *vnode)
{
	RAW_MUTEX_GUARD(_, &__default_vtree.lock);
	return __vmap_tree_insert_merge(&__default_vtree, vnode);
}

struct vmap_node *vmap_tree_remove(const void *addr)
{
	return __vmap_tree_remove(&__default_vtree, addr);
}

static int vmap_tree_refcnt_set(struct vmap_tree *vtree, struct vmap_node *cur, int flag)
{
	int refcnt;

	if (flag == VMAP_REF_GET) {
		refcnt = raw_refcnt_nr_get(&cur->refcnt);
	} else {
		refcnt = raw_refcnt_nr_put(&cur->refcnt);
	}
	if (refcnt == 0) {
		(void)__vmap_tree_remove_nolock(vtree, cur->vaddr);
		return cur->release(cur);
	}
	return __vmap_tree_merge_try(vtree, cur, NULL);
}

int vmap_tree_refcnt_split(const void *start, unsigned long size, int flag)
{
	struct vmap_tree *vtree = &__default_vtree;
	struct vmap_node *cur = NULL;
	unsigned long cur_end;
	unsigned long end = ptr_to_ulong(start) + size;
	int count, new_count, ret;
	struct vmap_range range;

	if ((flag != VMAP_REF_GET && flag != VMAP_REF_PUT)
			|| start == NULL || size == 0) {
		dh_error("vmap refcnt split inval error, size=0x%lx\n", size);
		return -EINVAL;
	}

	RAW_MUTEX_GUARD(_, &vtree->lock);
	cur = __vmap_tree_find_nolock(vtree, start);
	if (cur == NULL) {
		return -ENOENT;
	}

	cur_end = ptr_to_ulong(cur->vaddr) + cur->size;
	count = raw_refcnt_nr_counter(&cur->refcnt);

	if (cur->vaddr == start && cur_end == end) {
		return vmap_tree_refcnt_set(vtree, cur, flag);
	}

	if (end > cur_end) {
		dh_error("vmap refcnt split end overflow\n");
		return -EINVAL;
	}

	if (cur->vaddr == start) {
		(void)__vmap_tree_remove_nolock(vtree, cur->vaddr);
		cur->size -= size;
		cur->vaddr = ulong_to_ptr(end, void);
		ret = __vmap_tree_insert_nolock(vtree, cur);
		if (ret < 0) {
			return ret;
		}
	} else if (cur_end == end) {
		cur->size -= size;
	} else {
		cur->size = ptr_to_ulong(start) - ptr_to_ulong(cur->vaddr);
		range.vaddr = ulong_to_ptr(end, void);
		range.size = cur_end - end;
		ret = __vmap_tree_dup_node(vtree, range, count, cur);
		if (ret != 0) {
			return ret;
		}
	}

	new_count = flag == VMAP_REF_GET ? count + 1 : count - 1;
	range.vaddr = start;
	range.size = size;
	return __vmap_tree_dup_node(vtree, range, new_count, cur);
}

int vmap_tree_remove_split(const void *start, unsigned long size)
{
	struct vmap_tree *vtree = &__default_vtree;
	struct vmap_node *cur = NULL;
	unsigned long cur_end;
	unsigned long end = ptr_to_ulong(start) + size;
	int count;
	int ret;
	struct vmap_range range;

	if (start == NULL || size == 0) {
		return -EINVAL;
	}

	RAW_MUTEX_GUARD(_, &vtree->lock);
	cur = __vmap_tree_find_nolock(vtree, start);
	if (cur == NULL) {
		return -ENOENT;
	}

	cur_end = ptr_to_ulong(cur->vaddr) + cur->size;
	if (cur->vaddr == start && cur_end == end) {
		(void)__vmap_tree_remove_nolock(vtree, cur->vaddr);
		return cur->release(cur);
	}

	if ((cur->flags & FLAG_VMAP_MERGED) != FLAG_VMAP_MERGED) {
		return -EFAULT;
	}

	if (end > cur_end) {
		return -EINVAL;
	}

	if (cur->vaddr == start) {
		cur->size -= end - size;
		cur->vaddr = ulong_to_ptr(end, void);
		return 0;
	}

	if (cur_end == end) {
		cur->size -= size;
		return 0;
	}

	cur->size = ptr_to_ulong(start) - ptr_to_ulong(cur->vaddr);
	count = raw_refcnt_nr_counter(&cur->refcnt);
	range.vaddr = ulong_to_ptr(end, void);
	range.size = cur_end - end;
	ret = __vmap_tree_dup_node(vtree, range, count, cur);
	if (ret != 0) {
		return ret;
	}

	return 0;
}

struct vmap_node *vmap_tree_find(const void *addr)
{
	return __vmap_tree_find(&__default_vtree, addr);
}

int vmap_tree_show_all(char *info, unsigned long size, unsigned long *rsize)
{
	RAW_MUTEX_GUARD(_, &__default_vtree.lock);
	return __vmap_tree_show_all(&__default_vtree, info, size, rsize);
}

static int __vmap_tree_foreach_nolock(struct vmap_tree *vtree,
				      int (*fn)(struct vmap_node *vnode, void *args),
				      void *args)
{
	struct rbt_node *node = NULL;
	struct vmap_node *cur = NULL;
	int ret = 0;

	rbt_for_each_entry_safe(cur, node, VMAP_TREE_BASE(vtree), struct vmap_node, node) {
		ret = fn(cur, args);
		if (ret != 0) {
			break;
		}
	}
	return ret;
}

static inline
int __vmap_tree_foreach(struct vmap_tree *vtree,
			int (*fn)(struct vmap_node *vnode, void *args), void *args)
{
	RAW_MUTEX_GUARD(_, &vtree->lock);
	return __vmap_tree_foreach_nolock(vtree, fn, args);
}

int vmap_tree_foreach(int (*fn)(struct vmap_node *vnode, void *args), void *args)
{
	return __vmap_tree_foreach(&__default_vtree, fn, args);
}

static int __vmap_tree_foreach_trylock(struct vmap_tree *vtree,
			int (*fn)(struct vmap_node *vnode, void *args), void *args)
{
	int ret = E_HM_OK;

	ret = raw_mutex_trylock(&vtree->lock);
	if (ret != E_HM_OK) {
		dh_error("acquire vmap tree mutex lock failed, err=%s.\n", hmstrerror(ret));
		return ret;
	}

	ret = __vmap_tree_foreach_nolock(vtree, fn, args);
	raw_mutex_unlock(&vtree->lock);
	return ret;
}

/**
 * This function return HongMeng errno, cannot called directly from liblinux
 */
int vmap_tree_foreach_trylock(int (*fn)(struct vmap_node *vnode, void *args), void *args)
{
	return __vmap_tree_foreach_trylock(&__default_vtree, fn, args);
}

struct vmap_node *vmap_get_vnode(unsigned long type, uint32_t flags)
{
	struct vmap_node *node = NULL;

	node = (struct vmap_node *)malloc(sizeof(struct vmap_node));
	if (node == NULL) {
		return node;
	}

	mem_zero_s(*node);
	node->flags = flags;
	node->type = type;
	raw_refcnt_nr_init(&node->refcnt, 1);

	return node;
}
