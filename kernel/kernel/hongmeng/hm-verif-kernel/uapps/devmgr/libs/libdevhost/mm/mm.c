/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Implementation of libdh iomem
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jul 17 17:32:42 2020
 */
#include <libdevhost/mm.h>

#include <libmem/utils.h>
#include <hongmeng/types.h>
#include <libsysif/sysmgr/api.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhwsecurec/securec.h>
#include <libkasan/kasan.h>

#include "vmap_internal.h"


bool libdh_check_remap_flags_valid(unsigned int flags)
{
	if (flags == LIBDH_REMAP_NONE) {
		return false;
	}

	flags &= (LIBDH_REMAP_DEVICE | LIBDH_REMAP_NORMAL_NC | LIBDH_REMAP_CACHE);
	if ((flags != LIBDH_REMAP_DEVICE) && (flags != LIBDH_REMAP_NORMAL_NC) &&
	    (flags != LIBDH_REMAP_CACHE)) {
		return false;
	}
	return true;
}

/*
 * node release function: unmap the virtual address space and free node
 *
 * #param    node  [I]  node which has infomation to munmap
 *
 * @return   E_HM_OK for success, hmerrno for errors
 */
static int __libdh_vma_mem_munmap(struct libdh_vmap_node *node)
{
	int ret = E_HM_OK;

	ret = hm_mem_munmap(ptr_to_type(node->vaddr, void *), (size_t)node->size);
	if (ret != E_HM_OK) {
		hm_warn("unmap failed, err=%s\n", hmstrerror(ret));
	}

	free(node);
	return ret;
}

/*
 * create a new vmap node and insert to vmap tree
 *
 *  @param    vaddr  [I]  the key of node, base virtual address
 *  @param    size   [I]  size of virtual address range
 *
 *  @return   E_HM_OK for success, hmerrno for errors
 *  @note     vaddr should be page aligned
 */
static int libdh_create_vmap_node(const void *vaddr, unsigned long size)
{
	int ret = E_HM_OK;
	struct libdh_vmap_node *node = NULL;

	if (!PAGE_ALIGNED(vaddr)) {
		return E_HM_INVAL;
	}

	node = (struct libdh_vmap_node *)malloc(sizeof(*node));
	if (node == NULL) {
		return E_HM_NOMEM;
	}
	node->vaddr = vaddr;
	node->size  = size;
	libdh_init_vmap_release(node, __libdh_vma_mem_munmap);

	ret = libdh_vmap_insert(node);
	if (ret < 0) {
		hm_warn("insert vmap node failed\n");
		free(node);
	}

	return ret;
}

void *libdh_vma_reserve(const void *vaddr, unsigned long size,
			unsigned int prot, unsigned int flags)
{
	int ret = E_HM_OK;
	struct __actvret_hmcall_mem_ioremap_prepare func_ret;

	if (!PAGE_ALIGNED(vaddr) || !PAGE_ALIGNED(size) ||
	    !libdh_check_remap_flags_valid(flags)) {
		return NULL;
	}

	if (libdh_remap_flag_test(flags, LIBDH_REMAP_NORMAL_NC)) {
		prot |= __U(MPROT_NOCACHE);
	}

	ret = actvxactcall_hmcall_mem_ioremap_prepare(
			libdh_remap_flag_test(flags, LIBDH_VM_TO_USER), /* `fwd` */
			true, /* `allow refwd` */
			vaddr, size, prot, 0, &func_ret);
	if (ret != E_HM_OK) {
		hm_error("ioremap_prepare failed: size=0x%lx, prot=0x%x, err=%s\n",
			 size, prot, hmstrerror(ret));
		return NULL;
	}

	if (libdh_remap_flag_test(flags, LIBDH_REMAP_FIXED) &&
	    (ptr_to_u64(vaddr) != func_ret.rvaddr)) {
		hm_warn("MAP FIXED: check virtual address failed\n");
		goto err_out;
	}

	if (!libdh_remap_flag_test(flags, LIBDH_VM_TO_USER)) {
		ret = libdh_create_vmap_node(u64_to_ptr(func_ret.rvaddr, const void),
					     size);
		if (ret < 0) {
			goto err_out;
		}
	}

	return u64_to_ptr(func_ret.rvaddr, void);

err_out:
	ret = hm_mem_munmap(u64_to_ptr(func_ret.rvaddr, void), (size_t)size);
	if (ret != E_HM_OK) {
		hm_warn("unmap failed, err=%s\n", hmstrerror(ret));
	}
	return NULL;
}

int libdh_vma_map_page(void *vaddr, unsigned long size, paddr_t paddr,
		       unsigned int prot, unsigned int flags)
{
	int ret = E_HM_OK;
	uint64_t hint = 0;

	if (!PAGE_ALIGNED(vaddr) || !PAGE_ALIGNED(size) ||
	    !PAGE_ALIGNED(paddr) || !libdh_check_remap_flags_valid(flags)) {
		return E_HM_INVAL;
	}

	hint = prot_to_hint((uint32_t)prot, false, 0);
	if (libdh_remap_flag_test(flags, LIBDH_REMAP_NORMAL_NC)) {
		hint |= PGTABLE_ATTR_NOCACHE;
	} else {
		if (libdh_remap_flag_test(flags, LIBDH_REMAP_DEVICE)) {
			hint |= PGTABLE_ATTR_DEVICE;
		}
	}

	ret = actvxactcall_hmcall_mem_iomem_mmap_page(
			libdh_remap_flag_test(flags, LIBDH_VM_TO_USER), /* `fwd` */
			true, /* `allow refwd` */
			ERR_TO_REF(E_HM_INVAL), /* obsolete vspace_rref */
			paddr, vaddr, size, hint, 0);
	if (ret != E_HM_OK) {
		hm_error("iomem_mmap_page(size=0x%lx, hint=%"PRIx64") failed, err=%s\n",
			 size, hint, hmstrerror(ret));
		return ret;
	}

	return E_HM_OK;
}

void *libdh_vma_mmap(const void *vaddr, unsigned long size, paddr_t paddr,
		     unsigned int prot, unsigned int flags)
{
	int ret = E_HM_OK;
	void *new_vaddr = NULL;

	new_vaddr = libdh_vma_reserve(vaddr, size, prot, flags);
	if (new_vaddr == NULL) {
		return NULL;
	}

	ret = libdh_vma_map_page(new_vaddr, size, paddr, prot, flags);
	if (ret < 0) {
		(void)libdh_vma_unmap((const void *)new_vaddr);
		return NULL;
	}

	return new_vaddr;
}

int libdh_vma_unmap(const void *addr)
{
	struct libdh_vmap_node *node = NULL;

	if (!PAGE_ALIGNED(addr)) {
		return E_HM_INVAL;
	}

	node = __libdh_vmap_remove(addr);
	if (node == NULL) {
		hm_warn("remove vmap node failed\n");
		return E_HM_INVAL;
	}

	/* libdh_vmap_insert() ensures node->release != NULL */
	BUG_ON(node->release == NULL);
	return node->release(node);
}

void *libdh_ioremap_ex(paddr_t paddr, size_t size,
		       unsigned int prot, unsigned int flags)
{
	int ret = E_HM_OK;
	const unsigned long offset = (unsigned long)paddr & PAGE_OFFMASK;
	paddr_t last_addr;
	unsigned long align_size;
	struct __actvret_hmcall_mem_ioremap_prepare func_ret;
	uint64_t hint;

	if (!libdh_check_remap_flags_valid(flags)) {
		return NULL;
	}

	/*
	 * Page align the mapping address and size, taking account of any
	 * offset.
	 */
	paddr &= PAGE_MASK;
	align_size = PAGE_ALIGN_UP(size + offset);

	/*
	 * Don't allow wraparound or zero size.
	 */
	last_addr = paddr + align_size - 1U;
	if ((align_size == 0UL) || (last_addr < paddr) ||
	    (!libdh_check_remap_flags_valid(flags))) {
		return NULL;
	}

	hm_debug("paddr=0x%llx, size=0x%lx, prot=0x%x\n", paddr, align_size, prot);

	mem_zero_s(func_ret);

	if ((flags & LIBDH_REMAP_NORMAL_NC) != 0) {
		prot |= MPROT_NOCACHE;
	}

	ret = actvcall_hmcall_mem_ioremap_prepare(NULL, align_size, prot, 0,
						  &func_ret);
	if (ret != E_HM_OK) {
		return NULL;
	}

	hint = libdh_iomem_map_hint(prot, flags);
	ret = (int)actvcall_hmcall_mem_iomem_mmap_page(
			ERR_TO_REF(E_HM_INVAL), /* obsolete vspace_rref */
			paddr,
			u64_to_ptr(func_ret.rvaddr, void), align_size, hint, 0);
	if (ret != E_HM_OK) {
		hm_error("iomem_mmap_page(size=0x%lx, hint=%"PRIx64") failed, err=%s\n",
			 align_size, hint,
			 hmstrerror(ret));

		ret = hm_mem_munmap(u64_to_ptr(func_ret.rvaddr, void), (size_t)align_size);
		if (ret != E_HM_OK) {
			hm_warn("unmap failed, err=%s\n", hmstrerror(ret));
		}
		return NULL;
	}

	return ptr_add(func_ret.rvaddr, offset);
}

void libdh_iounmap(const void *vaddr, size_t size)
{
	int ret = E_HM_OK;
	const unsigned long offset = (uintptr_t)vaddr & PAGE_OFFMASK;
	uintptr_t start;
	unsigned long align_size;

	start = (uintptr_t)vaddr & PAGE_MASK;
	align_size = PAGE_ALIGN_UP(size + offset);

	ret = hm_mem_munmap((void *)start, (size_t)align_size);
	if (ret != E_HM_OK) {
		hm_error("unmap failed, err=%s\n", hmstrerror(ret));
		return;
	}
}

void libdh_register_nosave_region(unsigned long pfn_start, unsigned long long pfn_end)
{
	int ret;

	ret = actvcall_hmcall_pwrmgr_register_nosave_region(pfn_start, pfn_end);
	if (ret != E_HM_OK) {
		hm_error("register nosave region failed, err=%s\n", hmstrerror(ret));
	}
}
