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
#include <linux/slab.h>
#include <linux/syscalls.h>
#include <linux/platform_device.h>
#include <linux/dma-mapping.h>
#include <linux/platform_device.h>
#include <linux/of_device.h>
#include <linux/iommu/mm_iommu.h>
#ifdef CONFIG_DKMD_DPU_OHOS
#include <linux/dma-heap.h>
#include <linux/dmabuf/mm_dma_heap.h>
#include "uapi/linux/dma_heap_ext.h"
#else
#include <ion.h>
#include <linux/ion.h>
#include <linux/ion/mm_ion.h>
#endif
#ifdef DPU_RES_MGR
#include "res_mgr.h"
#endif
#include "ukmd_log.h"
#include "cmdlist_common.h"
#include "cmdlist_drv.h"
#include "cmdlist_mem_mgr.h"
#include "cmdlist_mem_ion.h"

static int32_t cmdlist_mem_ion_map_va(struct cmdlist_mem_mgr *mem_mgr, struct device *of_dev)
{
	unsigned long buf_size = 0;
	int32_t rc;
	struct ion_mem_info *mem_info = (struct ion_mem_info *)mem_mgr->mem_info;

	mem_mgr->pool_dma_addr = kernel_iommu_map_dmabuf(of_dev, mem_info->dmabuf, 0, &buf_size);
	if ((mem_mgr->pool_dma_addr == 0) || (buf_size < mem_mgr->max_size)) {
		ukmd_pr_err("get iova_size(0x%llx) smaller than size(0x%x)", buf_size, mem_mgr->max_size);
		if (mem_mgr->pool_dma_addr != 0) {
			(void)kernel_iommu_unmap_dmabuf(of_dev, mem_info->dmabuf, mem_mgr->pool_dma_addr);
			mem_mgr->pool_dma_addr = 0;
		}
		return -1;
	}

	rc = dma_buf_begin_cpu_access(mem_info->dmabuf, DMA_BIDIRECTIONAL);
	if (rc < 0) {
		ukmd_pr_err("dma_buf_begin_cpu_access failed, rc=%d", rc);
		goto err_unmap_dmabuf;
	}

	mem_mgr->pool_vir_addr = dma_buf_kmap(mem_info->dmabuf, 0);
	if (!mem_mgr->pool_vir_addr) {
		ukmd_pr_err("dma_buf_kmap failed");
		goto err_end_cpu_acc;
	}
	mem_mgr->pool_dma_size = buf_size;

	return 0;

err_end_cpu_acc:
	rc = dma_buf_end_cpu_access(mem_info->dmabuf, DMA_BIDIRECTIONAL);
	if (rc < 0)
		ukmd_pr_err("dma_buf_end_cpu_access failed, rc=%d", rc);

err_unmap_dmabuf:
	(void)kernel_iommu_unmap_dmabuf(of_dev, mem_info->dmabuf, mem_mgr->pool_dma_addr);
	mem_mgr->pool_dma_addr = 0;
	return -ENOMEM; 
}

static void cmdlist_mem_ion_unmap_va(struct cmdlist_mem_mgr *mem_mgr, struct device *of_dev)
{
	int32_t rc;
	struct ion_mem_info *mem_info = (struct ion_mem_info *)mem_mgr->mem_info;
	if (unlikely(!mem_info)) {
		ukmd_pr_err("mem_info is null!");
		return;
	}

	dma_buf_kunmap(mem_info->dmabuf, 0, mem_mgr->pool_vir_addr);
	
	rc = dma_buf_end_cpu_access(mem_info->dmabuf, DMA_BIDIRECTIONAL);
	if (rc < 0)
		ukmd_pr_err("dma_buf_end_cpu_access failed, rc=%d", rc);
	
	(void)kernel_iommu_unmap_dmabuf(of_dev, mem_info->dmabuf, mem_mgr->pool_dma_addr);
	mem_mgr->pool_dma_addr = 0;
}

#ifdef CONFIG_DKMD_DPU_OHOS
static struct dma_buf *cmdlist_mem_ion_alloc_dma_buf(size_t size)
{
	return dma_heap_buffer_alloc_kernel("system_heap", size, 0);
}

static void cmdlist_mem_ion_free_dma_buf(struct dma_buf *dmabuf)
{
	dma_heap_buffer_free_kernel(dmabuf);
}
#else
static struct dma_buf *cmdlist_mem_ion_alloc_dma_buf(size_t size)
{
	struct dma_buf *dmabuf;
	int32_t share_fd = ion_alloc(size, 1 << ION_SYSTEM_HEAP_ID, 0);
	if (unlikely(share_fd < 0)) {
		ukmd_pr_err("dma alloc 0x%zx Byte coherent failed!", size);
		return NULL;
	}

	dmabuf = dma_buf_get(share_fd);
	ksys_close((unsigned int)share_fd);
	return dmabuf;
}

static void cmdlist_mem_ion_free_dma_buf(struct dma_buf *dmabuf)
{
	dma_buf_put(dmabuf);
}
#endif

static int32_t cmdlist_mem_ion_init_mem_info(struct cmdlist_mem_mgr *mem_mgr, struct device *of_dev)
{
	struct ion_mem_info *mem_info = NULL;
	struct dma_buf *dmabuf = NULL;
	struct dma_buf_attachment *attach = NULL;
	struct sg_table *sgt = NULL;

	mem_info = (struct ion_mem_info *)kzalloc(sizeof(struct ion_mem_info), GFP_KERNEL);
	if (unlikely(!mem_info)) {
		ukmd_pr_err("mem_info alloc failed!");
		return -ENOMEM;
	}
	mem_mgr->mem_info = (void *)mem_info;

	dmabuf = cmdlist_mem_ion_alloc_dma_buf(mem_mgr->max_size);
	if (unlikely(IS_ERR_OR_NULL(dmabuf))) {
		ukmd_pr_err("fail to get dma buffer");
		goto err_kfree;
	}

	attach = dma_buf_attach(dmabuf, of_dev);
	if (unlikely(IS_ERR_OR_NULL(attach))) {
		ukmd_pr_err("fail to attach dma buf");
		goto err_dma_put;
	}

	sgt = dma_buf_map_attachment(attach, DMA_BIDIRECTIONAL);
	if (unlikely(IS_ERR_OR_NULL(sgt))) {
		ukmd_pr_err("fail to map_attachmen");
		goto err_dma_detach;
	}

	mem_info->dmabuf = dmabuf;
	mem_info->attach = attach;
	mem_info->sg_table = sgt;
	return 0;

err_dma_detach:
	dma_buf_detach(dmabuf, attach);
err_dma_put:
	cmdlist_mem_ion_free_dma_buf(dmabuf);
err_kfree:
	kfree(mem_info);
	return -ENOMEM;
}

static void cmdlist_mem_ion_deinit_mem_info(struct cmdlist_mem_mgr *mem_mgr)
{
	struct ion_mem_info *mem_info = (struct ion_mem_info *)mem_mgr->mem_info;
	if (unlikely(!mem_info)) {
		ukmd_pr_err("mem_info is null!");
		return;
	}

	dma_buf_unmap_attachment(mem_info->attach, mem_info->sg_table, DMA_BIDIRECTIONAL);
	dma_buf_detach(mem_info->dmabuf, mem_info->attach);
	cmdlist_mem_ion_free_dma_buf(mem_info->dmabuf);
	kfree(mem_info);
	mem_mgr->mem_info = NULL;
}

static struct device *get_res_mgr_of_dev(uint32_t dev_id)
{
	struct device *of_dev = NULL;

#ifdef DPU_RES_MGR
	if (dev_id == CMDLIST_DEV_ID_DPU)
		of_dev = dpu_res_get_device();
#endif

	return of_dev;
}

int32_t cmdlist_mem_ion_init(struct cmdlist_mem_mgr *mem_mgr, struct device *of_dev)
{
	// To map/unmap iova, we need use res_mgr device, which defiend in dts, contains smmu domain
	struct device *dev = get_res_mgr_of_dev(mem_mgr->cmd_priv->dev_id);
	int32_t ret = -1;

	if (!dev) {
		ukmd_pr_err("dev is NULL, dev_id=%u!", mem_mgr->cmd_priv->dev_id);
		return ret;
	}

	ret = cmdlist_mem_ion_init_mem_info(mem_mgr, dev);
	if (unlikely(ret != 0)) {
		ukmd_pr_err("mem_info init failed!");
		return ret;
	}

	ret = cmdlist_mem_ion_map_va(mem_mgr, dev);
	if (unlikely(ret != 0)) {
		ukmd_pr_err("mem_info init failed!");
		cmdlist_mem_ion_deinit_mem_info(mem_mgr);
		return ret;
	}

	return 0;
}

void cmdlist_mem_ion_deinit(struct cmdlist_mem_mgr *mem_mgr, struct device *of_dev)
{
	// To map/unmap iova, we need use res_mgr device, which defiend in dts, contains smmu domain
	struct device *dev = get_res_mgr_of_dev(mem_mgr->cmd_priv->dev_id);
	if (!dev) {
		ukmd_pr_err("dev is NULL, dev_id=%u!", mem_mgr->cmd_priv->dev_id);
		return;
	}

	cmdlist_mem_ion_unmap_va(mem_mgr, dev);
	cmdlist_mem_ion_deinit_mem_info(mem_mgr);
	mem_mgr->mem_pool = NULL;
	mem_mgr->pool_dma_addr = 0;
}

int32_t cmdlist_mem_ion_mmap(struct cmdlist_mem_mgr *mem_mgr, struct vm_area_struct *vma)
{
	uint32_t i = 0;
	int32_t ret = 0;
	struct scatterlist *sg = NULL;
	struct page *page = NULL;
	unsigned long size;
	unsigned long addr;
	unsigned long offset;
	unsigned long len;
	unsigned long remainder;
	struct ion_mem_info *mem_info = (struct ion_mem_info *)mem_mgr->mem_info;
	if (unlikely(!mem_info)) {
		ukmd_pr_err("mem_info is null!");
		return -EINVAL;
	}

	if (unlikely(!vma)) {
		ukmd_pr_err("vma is nullptr!");
		return -EINVAL;
	}

	vma->vm_page_prot = pgprot_writecombine(vma->vm_page_prot);

	addr = vma->vm_start;
	offset = vma->vm_pgoff * PAGE_SIZE;

	if (unlikely(vma->vm_end <= vma->vm_start)) {
		ukmd_pr_err("vm_end minus vm_start is less than 0");
		return -EINVAL;
	}

	size = vma->vm_end - vma->vm_start;

	down(&mem_mgr->mem_sem);
	if (size > mem_mgr->pool_dma_size) {
		ukmd_pr_err("vma size=%lx is out of range(%llx)!", size, mem_mgr->pool_dma_size);
		up(&mem_mgr->mem_sem);
		return -EINVAL;
	}

	for_each_sg(mem_info->sg_table->sgl, sg, mem_info->sg_table->nents, i) {
		if (unlikely(!sg)) {
		    ukmd_pr_err("sg is NULL");
			up(&mem_mgr->mem_sem);
		    return -EINVAL;
		}

		page = sg_page(sg);
		if (unlikely(!page)) {
		    ukmd_pr_err("page is NULL");
			up(&mem_mgr->mem_sem);
		    return -EINVAL;
		}
		remainder = vma->vm_end - addr;
		len = sg->length;

		if (offset >= sg->length) {
			offset -= sg->length;
			continue;
		}

		if (offset) {
			page += offset / PAGE_SIZE;
			len = sg->length - offset;
			offset = 0;
		}

		len = (unsigned long)min(len, remainder);
		ret = remap_pfn_range(vma, addr, page_to_pfn(page), len, vma->vm_page_prot);
		if (ret != 0) {
			ukmd_pr_err("failed to remap_pfn_range! ret=%d", ret);
			up(&mem_mgr->mem_sem);
			return ret;
		}

		addr += len;
		if (addr >= vma->vm_end) {
			ret = 0;
			break;
		}
	}
	up(&mem_mgr->mem_sem);

	return ret;
}