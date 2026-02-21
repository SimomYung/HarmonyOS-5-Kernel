/**
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#include <linux/platform_device.h>
#include <linux/dma-mapping.h>
#include <linux/platform_device.h>
#include <linux/of_device.h>
#include "ukmd_log.h"
#include "cmdlist_mem_mgr.h"
#include "cmdlist_mem_dma.h"

int32_t cmdlist_mem_dma_init(struct cmdlist_mem_mgr *mem_mgr, struct device *of_dev)
{
	dma_addr_t dma_handle;

	mem_mgr->pool_vir_addr = dma_alloc_coherent(of_dev, mem_mgr->max_size, &dma_handle, GFP_KERNEL);
	if (unlikely(!mem_mgr->pool_vir_addr)) {
		ukmd_pr_err("dma alloc 0x%zx Byte coherent failed %d!",
			mem_mgr->max_size, PTR_ERR(mem_mgr->pool_vir_addr));
		return -1;
	}
	mem_mgr->pool_dma_addr = dma_handle;
	mem_mgr->pool_dma_size = mem_mgr->max_size;
	ukmd_pr_info("dma alloc 0x%zx Byte coherent succ!", mem_mgr->max_size);
	return 0;
}

void cmdlist_mem_dma_deinit(struct cmdlist_mem_mgr *mem_mgr, struct device *of_dev)
{
	dma_free_coherent(of_dev, mem_mgr->max_size, mem_mgr->pool_vir_addr, mem_mgr->pool_dma_addr);
	mem_mgr->mem_pool = NULL;
	mem_mgr->pool_vir_addr = NULL;
	mem_mgr->pool_dma_addr = 0;
}

int32_t cmdlist_mem_dma_mmap(struct cmdlist_mem_mgr *mem_mgr, struct vm_area_struct *vma)
{
	int32_t ret = 0;
	unsigned long size;
	unsigned long addr;

	if (unlikely(vma->vm_end <= vma->vm_start)) {
		ukmd_pr_err("vma->vm_end minus vm_start is invalid");
		return -1;
	}

	down(&mem_mgr->mem_sem);
	addr = vma->vm_start;
	size = vma->vm_end - vma->vm_start;
	vma->vm_page_prot = pgprot_writecombine(vma->vm_page_prot);
	if (unlikely((size == 0) || (size > mem_mgr->max_size))) {
		ukmd_pr_err("mmap error, size=%#x, max_size=%#x", size, mem_mgr->max_size);
		up(&mem_mgr->mem_sem);
		return -EINVAL;
	}

	ret = remap_pfn_range(vma, addr, (mem_mgr->pool_dma_addr >> PAGE_SHIFT), size, vma->vm_page_prot);
	if (unlikely(ret != 0)) {
		ukmd_pr_err("failed to remap_pfn_range! ret=%d", ret);
		up(&mem_mgr->mem_sem);
		return ret;
	}
	up(&mem_mgr->mem_sem);

	return ret;
}