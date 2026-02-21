/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: liblinux PAL implement for modules operations
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan 11 10:45:12 2019
 */
#include <liblinux/pal.h>

#include <lnxbase/lnxbase.h>
#include <lnxbase/vmap.h>
#include <libstrict/strict.h>
#include <hongmeng/errno.h>
#include <libhmsrv_sys/hm_mem.h>

#include <devhost/log.h>
#include "../internal.h"
int liblinux_pal_module_alloc_init(const void *base, unsigned long size)
{
	int ret;

	ret = hm_mem_mprotect(base, size, PROT_NONE);
	if (ret < 0) {
		dh_error("protect MODULE-RSVD region failed, ret=%s\n",
			 hmstrerror(ret));
		return -hmerrno2posix(ret);
	}

	ret = hm_mem_madvise(base, size, MADV_FLAG_DONTNEED);
	if (ret < 0) {
		dh_error("unpopulate MODULE-RSVD region failed, ret=%s\n",
			 hmstrerror(ret));
		return -hmerrno2posix(ret);
	}

	ret = hm_mem_set_anon_name(base, size, MODULE_RSVD_ANON_NAME);
	if (ret < 0) {
		dh_warn("set anon name for MODULE-RSVD region failed, ret=%s\n",
			hmstrerror(ret));
	}

	return 0;
}

static int __module_alloc_free(struct vmap_node *node)
{
	UNUSED(node);
	/*
	 * CAUSION: this should never happened,
	 * module_alloc shoud only be released by module_alloc_release.
	 */
	panic("INVALID MODULE_ALLOC FREE\n");
	return -EINVAL;
}

#ifdef CONFIG_HKIP
int liblinux_pal_module_alloc_prepare(void *addr, unsigned long size)
#else
int liblinux_pal_module_alloc_prepare(const void *addr, unsigned long size)
#endif
{
	struct vmap_node *node = NULL;
	int ret;
	/* mark region as RW */
#ifdef CONFIG_HKIP
	void *raddr = hm_mem_mmap(addr, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON | MAP_FIXED, -1, 0);
	if (raddr == MAP_FAILED) {
		ret = -EINVAL;
		goto err_mprotect;
	}
#else
	ret = hm_mem_mprotect(addr, size, PROT_READ | PROT_WRITE);
	if (ret < 0) {
		ret = -hmerrno2posix(ret);
		goto err_mprotect;
	}
#endif
	/* alloc vmap node */
	node = (struct vmap_node *)malloc(sizeof(*node));
	if (node == NULL) {
		ret = -ENOMEM;
		goto err_alloc_vmap;
	}
	mem_zero_s(*node);
	node->vaddr = addr;
	node->size = size;
	node->flags = 0;
	node->type = VMAP_TYPE_ALLOC;
	node->release = __module_alloc_free;
	node->caller = lnxbase_return_address(1U);

	/* add to vmap tree */
	ret = vmap_tree_insert(node);
	if (ret < 0) {
		dh_error("insert module_alloc vmap node failed, size=0x%lx, ret=%d\n",
			 size, ret);
		goto err_insert_vmap;
	}

	return 0;

err_insert_vmap:
	free(node);
err_alloc_vmap:
	(void)hm_mem_mprotect(addr, size, PROT_NONE);
err_mprotect:
	return ret;
}

#ifdef CONFIG_HKIP
int liblinux_pal_module_alloc_prepare_hkip(void *addr, unsigned long size)
{
	struct vmap_node *node = NULL;
	int ret;
	/* mark region as RW */
	void *raddr = hm_mem_mmap(addr, size, PROT_READ | PROT_WRITE | MPROT_HKIP, MAP_PRIVATE | MAP_ANON | MAP_FIXED, -1, 0);
	if (raddr == MAP_FAILED) {
		ret = -EINVAL;
		goto err_mprotect;
	}
	/* alloc vmap node */
	node = (struct vmap_node *)malloc(sizeof(*node));
	if (node == NULL) {
		ret = -ENOMEM;
		goto err_alloc_vmap;
	}
	mem_zero_s(*node);
	node->vaddr = addr;
	node->size = size;
	node->flags = 0;
	node->type = VMAP_TYPE_ALLOC;
	node->release = __module_alloc_free;
	node->caller = lnxbase_return_address(1U);

	/* add to vmap tree */
	ret = vmap_tree_insert(node);
	if (ret < 0) {
		dh_error("insert module_alloc vmap node failed, size=0x%lx, ret=%d\n",
			 size, ret);
		goto err_insert_vmap;
	}

	return 0;

err_insert_vmap:
	free(node);
err_alloc_vmap:
	(void)hm_mem_mprotect(addr, size, PROT_NONE);
err_mprotect:
	return ret;
}
#else
int liblinux_pal_module_alloc_prepare_hkip(void *addr, unsigned long size)
{
	(void)addr;
	(void)size;
	return 0;
}
#endif

#ifdef CONFIG_HKIP
int liblinux_pal_module_alloc_release(void *addr, unsigned long *psize_out)
#else
int liblinux_pal_module_alloc_release(const void *addr, unsigned long *psize_out)
#endif
{
	struct vmap_node *node = NULL;
	int ret;
	node = vmap_tree_remove(addr);
	if (node == NULL) {
		return -EINVAL;
	}
	ret = hm_mem_mprotect(addr, node->size, PROT_NONE);
	if (ret < 0) {
		dh_warn("protect released MODULE-RSVD region failed, ret=%s\n",
			hmstrerror(ret));
	}
#ifdef CONFIG_HKIP
	ret = hm_mem_munlock(addr, node->size);
	if (ret != 0) {
		dh_warn("munlock MODULE-RSVD region failed, ret = %s\n",
			hmstrerror(ret));
	}
#endif
	ret = hm_mem_madvise(addr, node->size, MADV_FLAG_DONTNEED);
	if (ret < 0) {
		dh_warn("unpopulate released MODULE-RSVD region failed, ret=%s\n",
			 hmstrerror(ret));
	}

	ret = hm_mem_set_anon_name(addr, node->size, MODULE_RSVD_ANON_NAME);
	if (ret < 0) {
		dh_warn("set anon name for MODULE-RSVD region failed, ret=%s\n",
			hmstrerror(ret));
	}

	if (psize_out != NULL)
		*psize_out = node->size;

	free(node);

	return 0;
}

int liblinux_pal_request_module(const char *name)
{
	return -hmerrno2posix(devhost_pal_request_module(name));
}

int liblinux_pal_fill_mod_symbol(const char *name, const struct liblinux_mod_syminfo *si)
{
	/* do nothing, all code have been moved to devhost.ko */
	UNUSED(name);
	UNUSED(si);

	return 0;
}

int liblinux_pal_drop_ko_symbol(const void *module_core)
{
	/* do nothing, all code have been moved to devhost.ko */
	UNUSED(module_core);

	return 0;
}
