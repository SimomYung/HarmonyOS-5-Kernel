/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: Memory manager interfaces of UDK only enabled under fusion mode
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jul 03 09:35:13 2019
 */
#include <limits.h>

#include <hongmeng/types.h>
#include <hmkernel/mm/pgtable.h>
#include <libdevhost/mm.h>
#include <libmem/types.h>
#include <libsysif/utils_common.h>

#include <intapi/mem/map.h>
#include <intapi/mem/sysmgr_iomem.h>
#include <intapi/mem/sysmgr_page.h>

#include <udk/errno.h>
#include <udk/log.h>
#include <udk/mm_fusion.h>
#include <udk/bitops.h>

int udk_vmap(struct mem_raw_ranges *pfn_array,
		int array_len, unsigned int prot,
		unsigned int flags, void *rvaddr)
{
	uint64_t hint;
	int ret;

	if (pfn_array == NULL) {
		return -EINVAL;
	}
	hint = libdh_get_pbha_hint(flags);
	ret = udkmgr_vmap(pfn_array, array_len, prot, hint, rvaddr);
	return -hmerrno2posix(ret);
}

int udk_vunmap(unsigned long va, unsigned long size)
{
	int ret;

	if (va == 0UL) {
		return -EINVAL;
	}
	ret = udkmgr_vunmap(va, size);
	return -hmerrno2posix(ret);
}

int udk_iomem_vmap(struct mem_raw_ranges *pfn_array, size_t cnt,
		unsigned int prot, unsigned int flags, uintptr_t *ret_vaddr)
{
	uint32_t cnode_idx;
	uint64_t hint;
	int ret;

	if (ret_vaddr == NULL) {
		return -EINVAL;
	}
	cnode_idx = sysif_actv_fwd_cnode_idx();
	hint = libdh_iomem_map_hint(prot, flags);
	ret = udkmgr_iomem_vmap_to_devhost(pfn_array, cnt, prot, hint, cnode_idx, ret_vaddr);
	return -hmerrno2posix(ret);
}

int udk_iomem_vunmap(uintptr_t vaddr, size_t size)
{
	uint32_t cnode_idx = sysif_actv_fwd_cnode_idx();
	int ret = udkmgr_iomem_vunmap_from_devhost(vaddr, size, cnode_idx);
	return -hmerrno2posix(ret);
}

uint64_t udk_alloc_pages(uint32_t flags, uint32_t order)
{
	return sysmgr_alloc_pages(flags, order);
}

uint64_t udk_alloc_pages_batch(uint32_t flags, uint32_t order,
		unsigned long size, void *pa_array,
		unsigned int *pg_num)
{
	return sysmgr_alloc_pages_batch(flags, order, size, pa_array, pg_num);
}

void udk_free_pages(uint64_t pa)
{
	sysmgr_free_pages(pa);
}

void udk_add_to_pglist(uint64_t pa, struct dlist_node *pglist)
{
	sysmgr_add_to_pglist(pa, pglist);
}

void udk_delete_from_pglist(uint64_t pa)
{
	sysmgr_delete_from_pglist(pa);
}

void udk_traverse_for_pglist(struct dlist_node *pglist, uint64_t *pa, size_t *size)
{
	sysmgr_traverse_for_pglist(pglist, pa, size);
}

uint32_t udk_fetch_page_order(uint64_t pa)
{
	return sysmgr_fetch_page_order(pa);
}

int udk_page_set_private_data(uint64_t pa, unsigned long data)
{
	int err = sysmgr_page_set_private_data(pa, data);
	return -hmerrno2posix(err);
}

unsigned long udk_page_private_data_of(uint64_t pa)
{
	return sysmgr_page_private_data_of(pa);
}
