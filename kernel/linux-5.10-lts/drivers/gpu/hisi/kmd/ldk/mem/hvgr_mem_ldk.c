/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Override some interfaces in kernel/oh-ldk/drivers/gpu/hisi/kmd/mem
 * Author: Huawei OS Kernel Lab
 * Create: Thur Oct 19 14:59:26 2023
 */

#include <linux/mman.h>
#include <linux/vmalloc.h>
#include <linux/rwsem.h>
#include <linux/mm.h>
#include <linux/delay.h>
#include <linux/bitops.h>
#include <linux/rcupdate.h>

#include "hvgr_dm_api.h"
#include "hvgr_mem_opts.h"
#include "hvgr_log_api.h"
#include "hvgr_mem_api.h"

#define PFN_ARRAY_SIZE 256

long hvgr_mem_setup_cpu_mmu(struct vm_area_struct *vma, struct hvgr_setup_cpu_mmu_paras *paras)
{
	int ret;
	uint32_t idx;
	uint32_t index = 0;
	struct pfn_range array[PFN_ARRAY_SIZE];
	struct page **pages = paras->page_array;
	uint64_t page_nums = paras->page_nums;
	unsigned long addr = paras->addr;
	unsigned long addr_start = addr;

	for (idx = 0; idx < page_nums; idx++) {
		if (unlikely(pages[idx] == NULL)) {
			pr_err("insert pages page_%u is NULL\n", idx);
			goto error;
		}

		if (index >= PFN_ARRAY_SIZE) {
			ret = remap_pfn_range_batch(vma, addr_start, array, index, paras->vm_page_prot);
			if (ret != 0) {
				pr_err("insert pages vm insert pfn fail 0x%lx %d\n", addr_start, ret);
				goto error;
			}
			index = 0;
			addr_start = addr;
		}
		array[index].start = page_to_pfn(pages[idx]);
		array[index].end = page_to_pfn(pages[idx]) + 1;
		index++;
		addr += PAGE_SIZE;
	}

	if (index > 0) {
		ret = remap_pfn_range_batch(vma, addr_start, array, index, paras->vm_page_prot);
		if (ret != 0) {
			pr_err("insert1 pages vm insert pfn fail 0x%lx %d\n", addr_start, ret);
			goto error;
		}
	}

	return 0;

error:
	if (addr_start > paras->addr)
		zap_vma_ptes(vma, paras->addr, addr_start - paras->addr);
	return -1;
}

vm_fault_t hvgr_mem_vm_fault(struct vm_fault *vmf)
{
	vm_fault_t ret = VM_FAULT_SIGBUS;
	struct vm_area_struct *vma = vmf->vma;
	struct hvgr_mem_cpu_map *cpu_map = vma->vm_private_data;
	uint32_t page_offset;
	uint64_t insert_addr;
	phys_addr_t phys;
	struct hvgr_device *gdev = hvgr_get_device();
	struct pfn_range array[PFN_ARRAY_SIZE];
	unsigned long index = 0;
	unsigned long addr_start;
	pgprot_t page_prot = vma->vm_page_prot;
	if (gdev == NULL)
		return VM_FAULT_SIGBUS;

	if (cpu_map == NULL || cpu_map->area == NULL || cpu_map->ctx == NULL) {
		hvgr_err(gdev, HVGR_MEM, "vm 0x%lx addr 0x%lx null", vma->vm_start, vmf->address);
		return VM_FAULT_SIGBUS;
	}

	hvgr_info(gdev, HVGR_MEM, "%s:%d vm_fault area 0x%lx %lu fault vma:0x%lx~0x%lx 0x%lx 0x%lx",
		__func__, __LINE__, cpu_map->area->gva, cpu_map->area->pages, vma->vm_start,
		vma->vm_end, vma->vm_pgoff, vmf->address);

	mutex_lock(&cpu_map->ctx->mem_ctx.area_mutex);
	if (cpu_map->ctx->ctx_sw.is_background) {
		hvgr_debug(gdev, HVGR_MEM, "ctx_%u is backfround vm_fault 0x%lx addr 0x%lx",
			cpu_map->ctx->id, vma->vm_start, vmf->address);
		goto err_exit;
	}

	if (atomic_read(&cpu_map->area->import.data.umm.free_tlb_cnt) != 0) {
		hvgr_err(gdev, HVGR_MEM, "vm_fault dma buffer page was freed, do not allow memory growable");
		goto err_exit;
	}

	page_offset = vma->vm_pgoff - (cpu_map->area->gva >> PAGE_SHIFT);
	if ((page_offset >= cpu_map->area->pages) ||
		((vmf->address - vma->vm_start) >=
			((cpu_map->area->pages - page_offset) << PAGE_SHIFT))) {
		hvgr_err(gdev, HVGR_MEM, "vm_fault vma 0x%lx~0x%lx area 0x%lx %lu 0x%lx fault 0x%lx out of range",
			vma->vm_start, vma->vm_end, cpu_map->area->gva, cpu_map->area->pages,
			vma->vm_pgoff, vmf->address);
		goto err_exit;
	}

	insert_addr = vma->vm_start & PAGE_MASK;
	addr_start = insert_addr;
	while (page_offset < cpu_map->area->pages && insert_addr < vma->vm_end) {
		if (index >= PFN_ARRAY_SIZE) {
			ret = vmf_insert_pfn_batch_prot(vma, addr_start, array, index, page_prot);
			if (ret != VM_FAULT_NOPAGE) {
				hvgr_err(gdev, HVGR_MEM, "vm fault addr 0x%lx insert addr 0x%lx fail %d\n",
					vmf->address, insert_addr, (int)ret);
				goto err_exit;
			}
			index = 0;
			addr_start = insert_addr;
		}

		phys = page_to_phys(cpu_map->area->page_array[page_offset]);
		array[index].start = PFN_DOWN(phys);
		array[index].end = array[index].start + 1;
		index++;
		page_offset++;
		insert_addr += PAGE_SIZE;
		gdev->mem_dev.statics.grow_pages++;
	}

	if (index > 0) {
		ret = vmf_insert_pfn_batch_prot(vma, addr_start, array, index, page_prot);
		if (ret != VM_FAULT_NOPAGE) {
			hvgr_err(gdev, HVGR_MEM, "vm fault addr 0x%lx insert addr 0x%lx fail %d\n",
				vmf->address, insert_addr, (int)ret);
			goto err_exit;
		}
	}

	ret = VM_FAULT_NOPAGE;
err_exit:
	mutex_unlock(&cpu_map->ctx->mem_ctx.area_mutex);
	return ret;
}

#ifdef CONFIG_LIBLINUX_HVGR_BATCH_SYNC_OPT
static void hvgr_mem_sync_pages(struct hvgr_ctx * const ctx, dma_addr_t dma_addr,
		       uint64_t size, uint64_t direct)
{
	if (direct == HVGR_MEM_SYNC_TO_CPU)
		dma_sync_single_for_cpu(ctx->gdev->dev, dma_addr, size, DMA_BIDIRECTIONAL);
	else if (direct == HVGR_MEM_SYNC_TO_DEV)
		dma_sync_single_for_device(ctx->gdev->dev, dma_addr, size, DMA_BIDIRECTIONAL);
}

void hvgr_mem_sync_page_batch(struct hvgr_ctx * const ctx, struct page **page_arr,
		       uint64_t uva, uint64_t offset_page, uint64_t offset_page_byte,
		       uint64_t size, uint64_t sync_pages, uint64_t direct)
{
	uint64_t sync_size;
	dma_addr_t dma_addr, dma_addr_b;
	uint32_t idx;

	dma_addr_b = hvgr_mem_get_page_dma_addr(page_arr[offset_page]) + offset_page_byte;
	sync_size = (uint64_t)min(((uint64_t)PAGE_SIZE - offset_page_byte), size);

	for (idx = 1; idx < sync_pages; idx++) {
		dma_addr = hvgr_mem_get_page_dma_addr(page_arr[offset_page + idx]);
		if (dma_addr != (dma_addr_b + sync_size)) {
			hvgr_mem_sync_pages(ctx, dma_addr_b, sync_size, direct);
			dma_addr_b = dma_addr;
			sync_size = 0;
		}
		if (idx == sync_pages - 1)
			sync_size += ((uva + size - 1) & (PAGE_SIZE - 1)) + 1;
		else
			sync_size += PAGE_SIZE;
	}

	hvgr_mem_sync_pages(ctx, dma_addr_b, sync_size, direct);
}
#endif

#if defined(CONFIG_LIBLINUX) && !defined(CONFIG_LIBLINUX_CDC)
long hvgr_mem_vma_zap_pte(struct hvgr_ctx * const ctx, unsigned long addr,
	unsigned long len)
{
	struct vm_area_struct *vma = NULL;
	struct hvgr_mem_cpu_map *cpu_map = NULL;

	vma = find_vma_intersection(ctx->mem_ctx.process_mm, addr, (addr + len));
	if (vma == NULL || vma->vm_start > addr || vma->vm_ops != hvgr_mem_get_vm_opts() ||
		vma->vm_private_data == NULL) {
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u get zap area key 0x%lx:0x%lx fail.",
			ctx->id, addr, len);
		return -1;
	}

	cpu_map = vma->vm_private_data;
	if (cpu_map->ctx != ctx) {
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u get zap area key 0x%lx:0x%lx ctx not us.",
			ctx->id, addr, len);
		return -1;
	}

	return liblinux_zap_vma_ptes(vma, addr, len);
}
#endif

