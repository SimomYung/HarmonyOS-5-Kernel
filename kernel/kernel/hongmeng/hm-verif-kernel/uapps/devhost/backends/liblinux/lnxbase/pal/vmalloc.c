/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: liblinux PAL implement for vmalloc
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan 11 10:45:12 2019
 */
#include <liblinux/pal.h>

#include <stdlib.h>
#include <sys/mman.h>
#include <libmem/types.h>
#include <libmem/utils.h>
#include <libalgo/rbtree.h>
#include <hongmeng/errno.h>
#include <hongmeng/panic.h>
#include <hongmeng/types.h>
#include <libstrict/strict.h>
#include <libhmsync/raw_mutex.h>
#include <libhmsync/raw_rwlock.h>
#include <libhmsrv_sys/hm_mem.h>

#include <lnxbase/vmap.h>
#include <devhost/log.h>
#include <libdevhost/mm.h>

/* rbt node for vmalloc */
struct vmalloc_node {
	struct vmap_node vmap;
	void *base;		/* alloc base, for free */
};

static int vmalloc_accumulate_pages(struct vmap_node *vnode, void *args)
{
	unsigned long *total_nr_pages = (unsigned long *)args;

	if (____IS(vnode->type & VMAP_TYPE_ALLOC)) {
		*total_nr_pages += (PAGE_ALIGN_UP(vnode->size) >> PAGE_SHIFT);
	}
	return 0;
}

unsigned long liblinux_pal_vmalloc_nr_pages(void)
{
	unsigned long total_nr_pages = 0;
	vmap_tree_foreach(vmalloc_accumulate_pages, &total_nr_pages);
	return total_nr_pages;
}

int liblinux_pal_vmalloc_nr_pages_trylock(unsigned long *total_nr_pages)
{
	int ret = 0;

	if (total_nr_pages == NULL) {
		return -EINVAL;
	}

	*total_nr_pages = 0;
	ret = vmap_tree_foreach_trylock(vmalloc_accumulate_pages, total_nr_pages);
	if (ret != E_HM_OK) {
		return -hmerrno2posix(ret);
	}
	return 0;
}

#define min(x, y) ((x) > (typeof(x))(y) ? (typeof(x))(y) : (x))
#define MAX_VMALLOC_QUERY_LENGTH (PAGE_SIZE * PAGE_SIZE)
/**
 * @brief Query the actually mapped physical pages of a vmalloc area.
 *
 * @param vnode struct vmap_node represent a vmalloc area.
 * @param vec a pre-allocated array for storing result, its length must be PAGE_SIZE.
 * @param used_size param out, the size of actually in used physical memory.
 * @param nr_pages_out param out, physical nr_pages.
 * @return int return posix errno.
 */
int liblinux_pal_vmalloc_pmem_stat(struct vmap_node *vnode, unsigned char *vec,
								   unsigned long *used_size, unsigned int *nr_pages_out)
{
	int ret = 0;
	unsigned int nr_pages = 0;
	unsigned long va_start = ptr_to_ulong(vnode->vaddr);
	unsigned long va_start_aligned = PAGE_ALIGN_DOWN(va_start);
	unsigned long va_end = ptr_to_ulong(vnode->vaddr) + vnode->size;
	unsigned long va_end_aligned = PAGE_ALIGN_UP(va_end);
	unsigned long iter_va_start = va_start_aligned;
	unsigned long iter_aligned_size = va_end_aligned - va_start_aligned;
	unsigned long iter_query_size, length, i;

	while (iter_aligned_size > 0) {
		iter_query_size = min(iter_aligned_size, MAX_VMALLOC_QUERY_LENGTH);
		length = iter_query_size >> PAGE_SHIFT;

		ret = liblinux_pal_mincore(ulong_to_ptr(iter_va_start, void), iter_query_size, vec);
		if (ret < 0) {
			dh_error("%s liblinux_pal_mincore query failed, err=%s", __func__, hmstrerror(posix2hmerrno(-ret)));
			return ret;
		}

		for (i = 0; i < length; i++) {
			if (vec[i] == 1) {
				nr_pages++;
			}
		}

		iter_va_start += iter_query_size;
		iter_aligned_size -= iter_query_size;
	}

	*nr_pages_out = nr_pages;
	*used_size = (nr_pages << PAGE_SHIFT) - ((va_start - va_start_aligned) + (va_end_aligned - va_end));
	return 0;
}

static void *__really_vmalloc(struct vmalloc_node *node,
			      unsigned long start, unsigned long align,
			      unsigned long prot, unsigned long vm_flags)
{
	void *rvaddr = NULL;

	rvaddr = hm_mem_mmap(ulong_to_ptr(ALIGN_UP(start, align), void),
				node->vmap.size, (uint32_t)prot,
				MAP_PRIVATE | MAP_ANONYMOUS | trans_vmflag_to_mapflag(vm_flags), 0, 0);
	if (rvaddr == MAP_FAILED) {
		/* mmap failed, return NULL */
		rvaddr = NULL;
		goto err_out;
	}

	node->vmap.vaddr = rvaddr;
	node->base = rvaddr;

err_out:
	return rvaddr;
}

static void __really_vfree(struct vmalloc_node *node)
{
	int ret;

	ret = hm_mem_munmap(node->base, node->vmap.size);
	if (ret != E_HM_OK) {
		dh_warn("unmap failed, err=%s\n", hmstrerror(ret));
	}
}

static liblinux_mem_hook_t _vmalloc_stat_hook = NULL;
static struct raw_rwlock _hook_lock = RAW_RDLOCK_INITIALIZER;
int liblinux_dfx_vmalloc_hook(liblinux_mem_hook_t hook)
{
	(void)raw_rwlock_wrlock(&_hook_lock);
	_vmalloc_stat_hook = hook;
	(void)raw_rwlock_unlock(&_hook_lock);
	return 0;
}

static void _liblinux_dfx_call_vmalloc_hook(enum liblinux_dfx_mem_event ev, void *addr,
						unsigned long size, const void *data)
{
	if (_vmalloc_stat_hook == NULL) {
		return;
	}
	(void)raw_rwlock_rdlock(&_hook_lock);
	if (_vmalloc_stat_hook != NULL) {
		_vmalloc_stat_hook((int)ev, addr, size, data);
	}
	(void)raw_rwlock_unlock(&_hook_lock);
}

static int __vfree(struct vmap_node *vmap)
{
	struct vmalloc_node *node = NULL;

	node = container_of(vmap, struct vmalloc_node, vmap);

	BUG_ON(node->base == NULL);

	_liblinux_dfx_call_vmalloc_hook(LIBLINUX_DFX_VMALLOC_FREE, node->base, node->vmap.size, NULL);

	__really_vfree(node);
	free(node);
	return 0;
}

void *liblinux_pal_vmalloc_range_ex(unsigned long size, unsigned long align,
				 unsigned long start, unsigned long end,
				 unsigned long prot, unsigned long vm_flags, const void *caller)
{
	struct vmalloc_node *node = NULL;
	void *rvaddr = NULL;

	if ((align & (align - 1)) != 0) { /* align is not 2^n */
		dh_error("vmalloc failed: align=0x%lx\n", align);
		return NULL;
	}
	node = (struct vmalloc_node *)malloc(sizeof(*node));
	if (node == NULL) {
		return NULL;
	}
	mem_zero_s(*node);
	node->vmap.size =  ALIGN_UP(size, align);

	/* really vmalloc */
	rvaddr = __really_vmalloc(node, start, align, prot, vm_flags);
	if (rvaddr == NULL) {
		dh_error("vmalloc failed: size=0x%lx, align=0x%lx, start=0x%p, end=0x%p, prot=0x%lx\n",
			size, align, ulong_to_ptr(start, void), ulong_to_ptr(end, void), prot);
		goto err_out;
	}

	/* check overflow */
	if ((ptr_to_ulong(rvaddr) < start) ||
	    (ptr_to_ulong(rvaddr) >= end - size)) {
		goto err_overflow;
	}

	node->vmap.flags = vm_flags;
	node->vmap.type = VMAP_TYPE_ALLOC;
	node->vmap.release = __vfree;
	node->vmap.caller = caller;
	/* insert to vmap tree */
	if (vmap_tree_insert(&node->vmap) < 0) {
		dh_error("insert vmalloc vmap_node failed: size=0x%lx\n", node->vmap.size);
		goto err_insert;
	}

	_liblinux_dfx_call_vmalloc_hook(LIBLINUX_DFX_VMALLOC_ALLOC, node->base, node->vmap.size, caller);

	return rvaddr;

err_insert:
err_overflow:
	__really_vfree(node);
err_out:
	free(node);
	return NULL;
}

void *liblinux_pal_vmalloc_range(unsigned long size, unsigned long align,
				 unsigned long start, unsigned long end,
				 unsigned long prot, const void *caller)
{
	return liblinux_pal_vmalloc_range_ex(size, align, start, end, prot, 0UL, caller);
}

int liblinux_pal_check_vmalloc_size(void* kaddr, unsigned long end_index)
{
	struct vmap_node *vmap = NULL;
	struct vmalloc_node *node = NULL;

	vmap = vmap_tree_find(kaddr);
	if (vmap == NULL) {
		dh_error("vmap_tree_find err\n");
		return E_HM_FAULT;
	}

	if (vmap->type == VMAP_TYPE_ALLOC) {
		node = container_of(vmap, struct vmalloc_node, vmap);
		if (node == NULL) {
			dh_error("vmalloc_node err\n");
			return E_HM_FAULT;
		}
		if ((vmap->flags & LIBLINUX_PAL_VM_USERMAP) == 0) {
			dh_error("vmap flags or type err. flags 0x%x, type 0x%lx\n", vmap->flags, vmap->type);
			return E_HM_FAULT;
		}
	}

	if ((end_index > vmap->size) || (vmap->size == 0)) {
		dh_error("remap size err. vmap size 0x%lx  kaddr 0x%p end_index 0x%lx \n",
			vmap->size, kaddr, end_index);
		return E_HM_FAULT;
	}
	return 0;
}

int liblinux_pal_remap_vmalloc_range(unsigned long uaddr, unsigned long kaddr,
	unsigned long size, unsigned long prot, unsigned int flags)
{
	return libdh_vmalloc_mmap_page(uaddr, kaddr, size, prot, flags);
}

void liblinux_pal_vfree(const void *addr)
{
	struct vmap_node *vmap = NULL;

	vmap = vmap_tree_remove(addr);
	if (vmap == NULL) {
		return;
	}

	__vfree(vmap);
}

void *liblinux_pal_malloc(unsigned long size)
{
	if (unlikely(size == 0)) {
		return NULL;
	}
	return malloc(size);
}

void *liblinux_pal_calloc(size_t nmemb, size_t size)
{
	if (unlikely((size == 0) || (nmemb == 0))) {
		return NULL;
	}
	return calloc(nmemb, size);
}

void liblinux_pal_free(void *addr)
{
	free(addr);
}

int liblinux_pal_mincore(const void *addr, unsigned long size, unsigned char *vec)
{
	return hm_mem_mincore(addr, size, vec);
}
