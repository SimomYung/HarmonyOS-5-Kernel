/*
 * This is for vdec reserved ion management
 *
 * Copyright (c) 2024-2024 Huawei Technologies CO., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#include <linux/atomic.h>
#include <linux/dma-mapping.h>
#include <linux/dma-iommu.h>
#include <linux/dma-buf.h>
#include <linux/iommu.h>
#include <linux/iommu/mm_iommu.h>
#include <asm/cacheflush.h>
#include <linux/scatterlist.h>
#include <linux/ion.h>
#include <linux/ion/mm_ion.h>
#include <linux/syscalls.h>
#include <linux/time.h>
#include <linux/ktime.h>
#include <linux/timekeeping.h>
#include <platform_include/basicplatform/linux/mm/mem_trace.h>
#include "vcodec_vdec_memory.h"
#include "vcodec_vdec_plat.h"
#include "smmu_tbu.h"
#include "vcodec_mcu_ddr_map.h"
#include "vcodec_vdec_dbg.h"

#define VDEC_MCU_HEAP_ENTRY_NUM (VDEC_MCU_HEAP_TOTAL_SIZE / VDEC_MCU_HEAP_ONE_BLOCK_SIZE)
#define VDEC_SEGBUF_IOVA_ENTRY_NUM (VDEC_SEG_BUF_TOTAL_SIZE / VDEC_SEG_BUF_ONE_BLOCK_SIZE)

struct vdec_iova_entry {
	bool used;
	uint32_t iova;
	uint32_t size;
	void *vir;
	struct sg_table *table;
};

static struct vdec_iova_entry g_segbuf_table[VDEC_SEGBUF_IOVA_ENTRY_NUM];
static struct vdec_iova_entry g_mcu_heap_table[VDEC_MCU_HEAP_ENTRY_NUM];
static struct mutex g_segbuf_lock;
static const uint32_t g_orders[] = { 10, 8, 4, 0 };
static atomic_t g_vdec_alloc_pages_num = ATOMIC_INIT(0);
#define VDEC_NUM_ORDERS ARRAY_SIZE(g_orders)

int get_vdec_alloc_pages(void)
{
	return atomic_read(&g_vdec_alloc_pages_num);
}

static uint32_t vdec_order_to_size(uint32_t order)
{
	return PAGE_SIZE << order;
}

static struct page *vdec_alloc_largest_pages(gfp_t gfp_mask, uint32_t size,
						 uint32_t max_order)
{
	struct page *page = NULL;
	gfp_t gfp_flags = 0;
	uint32_t i = 0;

	for (i = 0; i < VDEC_NUM_ORDERS; i++) {
		if (size < vdec_order_to_size(g_orders[i]))
			continue;
		if (max_order < g_orders[i])
			continue;

		if (g_orders[i] >= 8) {
			gfp_flags = gfp_mask & (~__GFP_RECLAIM);
			gfp_flags |= __GFP_NOWARN;
			gfp_flags |= __GFP_NORETRY;
		} else if (g_orders[i] >= 4) {
			gfp_flags = gfp_mask & (~__GFP_DIRECT_RECLAIM);
			gfp_flags |= __GFP_NOWARN;
			gfp_flags |= __GFP_NORETRY;
		} else {
			gfp_flags = gfp_mask;
		}

		page = alloc_pages(gfp_flags, g_orders[i]);
		if (!page)
			continue;
		__inval_dcache_area(page_address(page), page_size(page));
		atomic_add(1 << g_orders[i], &g_vdec_alloc_pages_num);
		return page;
	}

	return NULL;
}

static struct sg_table *vdec_alloc_sg_table(uint32_t size)
{
	struct page *page = NULL;
	struct page *next_page = NULL;
	struct list_head pages_list;
	struct sg_table *table = NULL;
	struct scatterlist *sg = NULL;
	uint32_t alloc_pages_num = 0;
	uint32_t max_order = g_orders[0];
	int32_t size_remaining = PAGE_ALIGN(size);

	INIT_LIST_HEAD(&pages_list);

	while (size_remaining > 0) {
		page = vdec_alloc_largest_pages(
			GFP_KERNEL | __GFP_COMP, (uint32_t)size_remaining, max_order);
		if (!page) {
			dprint(PRN_ERROR, "alloc largest pages failed");
			goto free_pages;
		}

		list_add_tail(&page->lru, &pages_list);
		size_remaining -= (int32_t)page_size(page);
		max_order = compound_order(page);
		alloc_pages_num++;
	}

	table = vmalloc(sizeof(struct sg_table));
	if (!table) {
		dprint(PRN_ERROR, "alloc fail");
		goto free_pages;
	}

	if (sg_alloc_table(table, alloc_pages_num, GFP_KERNEL)) {
		dprint(PRN_ERROR, "sg alloc table fail");
		goto free_table;
	}

	sg = table->sgl;
	list_for_each_entry(page, &pages_list, lru) {
		sg_set_page(sg, page, page_size(page), 0);
		sg = sg_next(sg);
	}

	dprint(PRN_ALWS, "pages num:%d,size:0x%x", alloc_pages_num, size);
	return table;

free_table:
	vfree(table);
free_pages:
	list_for_each_entry_safe(page, next_page, &pages_list, lru) {
		atomic_sub(1 << compound_order(page), &g_vdec_alloc_pages_num);
		__free_pages(page, compound_order(page));
	}

	return NULL;
}

static void vdec_free_sg_table(struct sg_table *table)
{
	struct scatterlist *sg = NULL;
	uint32_t i = 0;

	dprint(PRN_DBG, "table = 0x%pK", table);
	for_each_sgtable_sg(table, sg, i) {
		struct page *p = sg_page(sg);
		atomic_sub(1 << compound_order(p), &g_vdec_alloc_pages_num);
		__free_pages(p, compound_order(p));
	}
	sg_free_table(table);
	vfree(table);
}

static void vdec_init_mcu_heap_iova(void)
{
	uint32_t i;

	dprint(PRN_DBG, "vdec reserved size:0x%x", VDEC_MCU_HEAP_TOTAL_SIZE);
	for (i = 0; i < VDEC_MCU_HEAP_ENTRY_NUM; ++i) {
		g_mcu_heap_table[i].used = false;
		g_mcu_heap_table[i].iova = VDEC_MCU_HEAP_BASE + VDEC_MCU_HEAP_ONE_BLOCK_SIZE * i;
		g_mcu_heap_table[i].size = VDEC_MCU_HEAP_ONE_BLOCK_SIZE;
		g_mcu_heap_table[i].vir = NULL;
		g_mcu_heap_table[i].table = NULL;
	}
}

// Before the power-off, release all the memory that is not released
// from the ground address to the highest address.
static void vdec_deinit_mcu_heap_iova(void)
{
	int i;
	uint32_t iova;
	uint32_t size;
	size_t unmapped;
	struct iommu_domain *domain = NULL;
	struct device *dev;

	dev = vdec_get_entry()->device;
	domain = iommu_get_domain_for_dev(dev);

	for (i = VDEC_MCU_HEAP_ENTRY_NUM - 1; i >= 0 ; --i) {
		if (g_mcu_heap_table[i].table == NULL)
			continue;
		
		iova = g_mcu_heap_table[i].iova;
		size = g_mcu_heap_table[i].size;
		dprint(PRN_ALWS, "iova[%d] need release", i);

		smmu_pre_update_rdend(iova);
		unmapped = iommu_unmap(domain, iova, size);
		if (unmapped != size) {
			dprint(PRN_ERROR, "unmap fail,size:0x%lx,ummapped:0x%lx", size, unmapped);
			continue;
		}

		vdec_free_sg_table(g_mcu_heap_table[i].table);

		g_mcu_heap_table[i].size = 0;
		g_mcu_heap_table[i].table = NULL;
		g_mcu_heap_table[i].vir = NULL;
		g_mcu_heap_table[i].used = false;
		dprint(PRN_ALWS, "free mcu heap iova[%d], size:0x%lx", i, size);
	}
}

int32_t vdec_alloc_mcu_heap_iova(uint32_t *addr)
{
	int i;
	uint32_t iova;
	uint32_t len = VDEC_MCU_HEAP_ONE_BLOCK_SIZE;
	size_t map_len;
	struct iommu_domain *domain = NULL;
	struct sg_table *table = NULL;
	struct device *dev;

	// Find the memory that has not been released after the last application.
	// If the memory is found, you do not need to update the smmu_pre configuration
	iova = 0;
	for (i = 0; i < VDEC_MCU_HEAP_ENTRY_NUM; ++i) {
		if ((g_mcu_heap_table[i].used == false) && (g_mcu_heap_table[i].table != NULL)) {
			iova = g_mcu_heap_table[i].iova;
			dprint(PRN_ALWS, "reuse iova[%d]", i);
			g_mcu_heap_table[i].used = true;
			*addr = iova;
			return 0;
		}
	}

	// A new memory needs to be applied for.
	iova = 0;
	for (i = 0; i < VDEC_MCU_HEAP_ENTRY_NUM; ++i) {
		if (g_mcu_heap_table[i].used == false) {
			iova = g_mcu_heap_table[i].iova;
			break;
		}
	}

	if (iova == 0) {
		dprint(PRN_ERROR, "mcu heap iova out of memory");
		return -ENOMEM;
	}

	dev = vdec_get_entry()->device;
	domain = iommu_get_domain_for_dev(dev);

	table = vdec_alloc_sg_table(len);
	if (IS_ERR_OR_NULL(table)) {
		dprint(PRN_ERROR, "alloc sg table failed");
		goto table_fail;
	}

	map_len = iommu_map_sg(domain, iova, table->sgl,
					(unsigned int)sg_nents(table->sgl),
					IOMMU_READ | IOMMU_WRITE);
	if (map_len != len) {
		dprint(PRN_ERROR, "map sg table failed,map_len:0x%lx,len:0x%x",
			map_len, len);
		goto map_fail;
	}
	mm_iommu_flush_ttwc(dev, domain);

	g_mcu_heap_table[i].size = len;
	g_mcu_heap_table[i].table = table;
	g_mcu_heap_table[i].vir = NULL;
	g_mcu_heap_table[i].used = true;
	dprint(PRN_ALWS, "alloc mcu heap iova[%d], size:0x%x", i, len);

	// The dynamic heap memory increases.
	// After the memory is obtained, the end address of smmu_pre is expanded.
	smmu_pre_update_rdend(iova + VDEC_MCU_HEAP_ONE_BLOCK_SIZE);
	*addr = iova;
	return 0;

map_fail:
	vdec_free_sg_table(table);
table_fail:
	g_mcu_heap_table[i].used = false;
	return -ENOMEM;
}

int vdec_free_mcu_heap_iova(uint32_t free_iova)
{
	int i, j;
	uint32_t iova = 0;
	uint32_t size = 0;
	size_t unmapped;
	struct iommu_domain *domain = NULL;
	struct device *dev;

	dev = vdec_get_entry()->device;
	domain = iommu_get_domain_for_dev(dev);

	for (i = 0; i < VDEC_MCU_HEAP_ENTRY_NUM; ++i) {
		if (g_mcu_heap_table[i].used && g_mcu_heap_table[i].iova == free_iova) {
			g_mcu_heap_table[i].used = false;
			break;
		}
	}

	if (i >= VDEC_MCU_HEAP_ENTRY_NUM) {
		dprint(PRN_ERROR, "iova not found:0x%x", free_iova);
		return -EINVAL;
	}

	// If the memory in the back is applied for and not released,
	// the memory in the front is not released.
	for (j = i + 1; j < VDEC_MCU_HEAP_ENTRY_NUM; ++j) {
		if (g_mcu_heap_table[j].used) {
			dprint(PRN_ALWS, "iova[%d]is used, iova[%d] not free", j, i);
			return 0;
		}
	}

	// From the current block, the subsequent memory is not used.
	// Therefore, the memory is completely released and the smmu_pre configuration is updated.
	// Because the dynamic heap memory increases upward, shrink the end address of smmu_pre when the memory is released.
	smmu_pre_update_rdend(g_mcu_heap_table[i].iova);
	for (j = i; j < VDEC_MCU_HEAP_ENTRY_NUM && g_mcu_heap_table[j].table != NULL; ++j) {
		iova = g_mcu_heap_table[j].iova;
		size = g_mcu_heap_table[j].size;
		unmapped = iommu_unmap(domain, iova, size);
		if (unmapped != size) {
			dprint(PRN_ERROR, "unmap fail,size:0x%lx,ummapped:0x%lx", size, unmapped);
			continue;
		}

		vdec_free_sg_table(g_mcu_heap_table[j].table);

		g_mcu_heap_table[j].size = 0;
		g_mcu_heap_table[j].table = NULL;
		g_mcu_heap_table[j].vir = NULL;
		g_mcu_heap_table[j].used = false;
		dprint(PRN_ALWS, "free mcu heap iova[%d], size:0x%lx", j, size);
	}

	return 0;
}

static void vdec_get_alloced_page(struct sg_table *table, struct page **pages)
{
	unsigned int i, j;
	struct scatterlist *sg = NULL;
	unsigned int npages_this_entry = 0;
	struct page *page = NULL;

	for_each_sg(table->sgl, sg, table->nents, i) {
		npages_this_entry = (unsigned int)(PAGE_ALIGN(sg->length) / PAGE_SIZE);
		page = sg_page(sg);
		for (j = 0; j < npages_this_entry; j++)
			*(pages++) = page++;
	}
}

static void *vdec_mem_map_kernel(struct sg_table *table, unsigned long length)
{
	void *vaddr = NULL;
	unsigned int npages = (unsigned int)(PAGE_ALIGN(length) / PAGE_SIZE);
	struct page **pages;

	pages = vmalloc(sizeof(struct page *) * npages);
	if (pages == NULL)
		return NULL;

	vdec_get_alloced_page(table, pages);

	vaddr = vmap(pages, npages, VM_MAP, __pgprot(PROT_NORMAL_NC));
	vfree(pages);
	if (vaddr == NULL) {
		dprint(PRN_ERROR, "vmap failed");
		return NULL;
	}
	return vaddr;
}

static void vdec_mem_unmap_kernel(const void *va)
{
	if (va == NULL) {
		dprint(PRN_ERROR, "va is NULL");
		return;
	}
	vunmap(va);
}
// Apply for ion memory and map the memory according to the specified iova address.
// Map the virtual address that can be accessed by the AP.
// Caution：The length and address must be 4K aligned.
int vdec_alloc_iova_with_va(uint32_t iova, uint32_t len, struct vdec_hw_hal_mem_info *mem)
{
	size_t map_len;
	struct iommu_domain *domain = NULL;
	struct sg_table *table = NULL;
	void *va = NULL;
	struct device *dev;

	dprint(PRN_DBG, "alloc size:0x%x", len);
	if (iova == 0) {
		dprint(PRN_ERROR, "iova error");
		return -EINVAL;
	}

	dev = vdec_get_entry()->device;
	domain = iommu_get_domain_for_dev(dev);

	table = vdec_alloc_sg_table(len);
	if (IS_ERR_OR_NULL(table)) {
		dprint(PRN_ERROR, "alloc sg table failed");
		return -ENOMEM;
	}

	va = vdec_mem_map_kernel(table, len);
	if (va == NULL) {
		dprint(PRN_ERROR, "map vir failed");
		goto map_vir_fail;
	}

	map_len = iommu_map_sg(domain, iova, table->sgl, (unsigned int)sg_nents(table->sgl),
							IOMMU_READ | IOMMU_WRITE);
	if (map_len != len) {
		dprint(PRN_ERROR, "map sg table failed,map_len:0x%lx,len:0x%x", map_len, len);
		goto map_iova_fail;
	}
	mm_iommu_flush_ttwc(dev, domain);

	mem->iova = iova;
	mem->iova_size = len;
	mem->hal_table = table;
	mem->virt_addr = va;

	dprint(PRN_DBG, "alloc vir:%pK, size:0x%x, value:0x%x",
			mem->virt_addr, mem->iova_size, *((uint32_t*)va));
	return 0;

map_iova_fail:
	vdec_mem_unmap_kernel(va);
map_vir_fail:
	vdec_free_sg_table(table);

	return -ENOMEM;
}

int vdec_free_iova_with_va(struct vdec_hw_hal_mem_info *mem)
{
	size_t unmapped;
	struct iommu_domain *domain = NULL;
	struct device *dev;

	dev = vdec_get_entry()->device;
	domain = iommu_get_domain_for_dev(dev);

	if (mem->iova == 0) {
		dprint(PRN_ERROR, "iova equal zero not found");
		return -EINVAL;
	}

	unmapped = iommu_unmap(domain, mem->iova, mem->iova_size);
	if (unmapped != mem->iova_size) {
		dprint(PRN_ERROR, "unmap fail,len:0x%lx,ummapped:0x%lx", mem->iova_size, unmapped);
		return -EINVAL;
	}
	vdec_mem_unmap_kernel(mem->virt_addr);
	vdec_free_sg_table(mem->hal_table);

	dprint(PRN_DBG, "free vir:%pK, size:0x%lx", mem->virt_addr, mem->iova_size);
	mem->iova = 0;
	mem->iova_size = 0;
	mem->hal_table = NULL;
	mem->virt_addr = NULL;
	return 0;
}

// Apply for ion memory and map the memory based on the specified iova address.
// Caution：The length and address must be 4K aligned.
int vdec_alloc_iova(uint32_t iova, uint32_t len, struct vdec_hw_hal_mem_info *mem)
{
	size_t map_len;
	struct iommu_domain *domain = NULL;
	struct sg_table *table = NULL;
	struct device *dev;

	if (iova == 0) {
		dprint(PRN_ERROR, "dyn iova out of memory");
		return -EINVAL;
	}

	dev = vdec_get_entry()->device;
	domain = iommu_get_domain_for_dev(dev);

	table = vdec_alloc_sg_table(len);
	if (IS_ERR_OR_NULL(table)) {
		dprint(PRN_ERROR, "alloc sg table failed");
		return -ENOMEM;
	}

	map_len = iommu_map_sg(domain, iova, table->sgl, (unsigned int)sg_nents(table->sgl),
							IOMMU_READ | IOMMU_WRITE);
	if (map_len != len) {
		dprint(PRN_ERROR, "map sg table failed,map_len:0x%lx,len:0x%x", map_len, len);
		goto map_iova_fail;
	}
	mm_iommu_flush_ttwc(dev, domain);

	mem->iova = iova;
	mem->iova_size = len;
	mem->hal_table = table;
	mem->virt_addr = NULL;

	dprint(PRN_DBG, "alloc size:0x%x", mem->iova_size);
	return 0;

map_iova_fail:
	vdec_free_sg_table(table);

	return -ENOMEM;
}

int vdec_free_iova(struct vdec_hw_hal_mem_info *mem)
{
	size_t unmapped;
	struct iommu_domain *domain = NULL;
	struct device *dev;

	dev = vdec_get_entry()->device;
	domain = iommu_get_domain_for_dev(dev);

	if (mem->iova == 0) {
		dprint(PRN_ERROR, "iova equal zero not found");
		return -EINVAL;
	}

	unmapped = iommu_unmap(domain, mem->iova, mem->iova_size);
	if (unmapped != mem->iova_size) {
		dprint(PRN_ERROR, "unmap fail,len:0x%lx,ummapped:0x%lx", mem->iova_size, unmapped);
		return -EINVAL;
	}
	vdec_free_sg_table(mem->hal_table);

	dprint(PRN_DBG, "free size:0x%lx", mem->iova_size);
	mem->iova = 0;
	mem->iova_size = 0;
	mem->hal_table = NULL;
	mem->virt_addr = NULL;
	return 0;
}

static void vdec_init_segbuffer(void)
{
	int i;

	uint32_t iova_start_addr = VDEC_SEG_BUF_BASE;
	uint32_t iova_size = VDEC_SEG_BUF_TOTAL_SIZE;
	mutex_init(&g_segbuf_lock);
	dprint(PRN_DBG, "vdec segbuffer size:0x%x", iova_size);
	// segbuffer the iova expands from high to low.
	for (i = 0; i < VDEC_SEGBUF_IOVA_ENTRY_NUM; ++i) {
		g_segbuf_table[i].used = false;
		g_segbuf_table[i].iova = iova_start_addr + VDEC_SEG_BUF_TOTAL_SIZE - VDEC_SEG_BUF_ONE_BLOCK_SIZE * (i + 1);
		g_segbuf_table[i].size = VDEC_SEG_BUF_ONE_BLOCK_SIZE;
		g_segbuf_table[i].vir = NULL;
		g_segbuf_table[i].table = NULL;
	}
}

// Before the power-off, release all the memory that is not released
// from the ground address to the highest address.
static void vdec_deinit_segbuffer(void)
{
	int i;
	uint32_t iova = 0;
	uint32_t size = 0;
	size_t unmapped;
	struct iommu_domain *domain = NULL;
	struct device *dev;

	dev = vdec_get_entry()->device;
	domain = iommu_get_domain_for_dev(dev);
	mutex_destroy(&g_segbuf_lock);

	for (i = VDEC_SEGBUF_IOVA_ENTRY_NUM - 1; i >= 0; --i) {
		if (g_segbuf_table[i].table != NULL) {
			iova = g_segbuf_table[i].iova;
			size = g_segbuf_table[i].size;
			dprint(PRN_ALWS, "segbuf iova[%d] need release", i);

			smmu_pre_update_rdstart(iova + VDEC_SEG_BUF_ONE_BLOCK_SIZE);
			unmapped = iommu_unmap(domain, iova, size);
			if (unmapped != size) {
				dprint(PRN_ERROR, "unmap fail,size:0x%x,ummapped:0x%lx", size, unmapped);
				continue;
			}

			vdec_free_sg_table(g_segbuf_table[i].table);

			g_segbuf_table[i].size = 0;
			g_segbuf_table[i].table = NULL;
			g_segbuf_table[i].vir = NULL;
			g_segbuf_table[i].used = false;
			dprint(PRN_ALWS, "free segbuf iova[%d], size:0x%lx", i, size);
		}
	}
}

int32_t vdec_alloc_segbuffer(uint32_t *addr)
{
	int i;
	int32_t ret = 0;
	uint32_t iova;
	uint32_t len = VDEC_SEG_BUF_ONE_BLOCK_SIZE;
	size_t map_len;
	struct iommu_domain *domain = NULL;
	struct sg_table *table = NULL;
	struct device *dev;

	// Find the memory that has not been released after the last application.
	// If the memory is found, you do not need to update the smmu_pre configuration
	iova = 0;
	mutex_lock(&g_segbuf_lock);
	for (i = 0; i < VDEC_SEGBUF_IOVA_ENTRY_NUM; ++i) {
		if ((g_segbuf_table[i].used == false) && (g_segbuf_table[i].table != NULL)) {
			iova = g_segbuf_table[i].iova;
			dprint(PRN_ALWS, "reuse segbuf iova[%d]", i);
			g_segbuf_table[i].used = true;
			*addr = iova;
			goto unlock;
		}
	}

	// A new memory needs to be applied for.
	iova = 0;
	for (i = 0; i < VDEC_SEGBUF_IOVA_ENTRY_NUM; ++i) {
		if (g_segbuf_table[i].used == false) {
			iova = g_segbuf_table[i].iova;
			break;
		}
	}

	if (iova == 0) {
		dprint(PRN_ERROR, "vdec iova out of memory");
		ret = -ENOMEM;
		goto unlock;
	}

	dev = vdec_get_entry()->device;
	domain = iommu_get_domain_for_dev(dev);

	table = vdec_alloc_sg_table(len);
	if (IS_ERR_OR_NULL(table)) {
		dprint(PRN_ERROR, "alloc sg table failed");
		ret = -ENOMEM;
		goto unlock;
	}

	map_len = iommu_map_sg(domain, iova, table->sgl,
							(unsigned int)sg_nents(table->sgl),
							IOMMU_READ | IOMMU_WRITE);
	if (map_len != len) {
		dprint(PRN_ERROR, "map sg table failed,map_len:0x%lx,len:0x%x",
				map_len, len);
		vdec_free_sg_table(table);
		ret = -ENOMEM;
		goto unlock;
	}
	mm_iommu_flush_ttwc(dev, domain);

	g_segbuf_table[i].size = len;
	g_segbuf_table[i].table = table;
	g_segbuf_table[i].vir = NULL;
	g_segbuf_table[i].used = true;
	dprint(PRN_ALWS, "alloc segbuf iova[%d], size:0x%x", i, len);

	// The value of the segbuffer increases downward.
	// After the memory is obtained, the start address of the smmu_pre is expanded.
	smmu_pre_update_rdstart(iova);
	*addr = iova;
unlock:
	mutex_unlock(&g_segbuf_lock);
	return ret;
}

int32_t vdec_free_segbuffer(uint32_t free_iova)
{
	int i, j;
	int32_t ret = 0;
	uint32_t iova = 0;
	uint32_t size = 0;
	size_t unmapped;
	struct iommu_domain *domain = NULL;
	struct device *dev;

	dev = vdec_get_entry()->device;
	domain = iommu_get_domain_for_dev(dev);

	mutex_lock(&g_segbuf_lock);
	for (i = 0; i < VDEC_SEGBUF_IOVA_ENTRY_NUM; ++i) {
		if (g_segbuf_table[i].used && g_segbuf_table[i].iova == free_iova) {
			g_segbuf_table[i].used = false;
			break;
		}
	}

	if (i >= VDEC_SEGBUF_IOVA_ENTRY_NUM) {
		dprint(PRN_ERROR, "iova not found:0x%x", free_iova);
		ret = -EINVAL;
		goto unlock;
	}

	// If the back memory is being used, the front memory should not be released
	for (j = i + 1; j < VDEC_SEGBUF_IOVA_ENTRY_NUM; ++j) {
		if (g_segbuf_table[j].used) {
			dprint(PRN_ALWS, "segbuf iova[%d] is used, iova[%d] not free", j, i);
			goto unlock;
		}
	}

	// When the current block is released, if the previous blocks are not used,
	// the blocks are released and the smmu_pre configuration is updated.
	// The segbuffer grows downward. When the memory is released, the start address of smmu_pre is shrunk.
	for (j = i; j >= 0 && g_segbuf_table[j].used == false; --j) {
		smmu_pre_update_rdstart(g_segbuf_table[j].iova + VDEC_SEG_BUF_ONE_BLOCK_SIZE);
		iova = g_segbuf_table[j].iova;
		size = g_segbuf_table[j].size;
		unmapped = iommu_unmap(domain, iova, size);
		if (unmapped != size) {
			dprint(PRN_ERROR, "unmap fail,size:0x%lx,ummapped:0x%lx", size, unmapped);
			continue;
		}

		vdec_free_sg_table(g_segbuf_table[j].table);

		g_segbuf_table[j].size = 0;
		g_segbuf_table[j].table = NULL;
		g_segbuf_table[j].vir = NULL;
		g_segbuf_table[j].used = false;
		dprint(PRN_ALWS, "free segbuf iova[%d], size:0x%lx", j, size);
	}

unlock:
	mutex_unlock(&g_segbuf_lock);
	return ret;
}

void vdec_init_reserved_iova(void)
{
	vdec_init_mcu_heap_iova();
	vdec_init_segbuffer();
}

void vdec_deinit_reserved_iova(void)
{
	vdec_deinit_mcu_heap_iova();
	vdec_deinit_segbuffer();
}
void vdec_suspend_iova_smmu_pre(void)
{
	show_smmu_pre_intercept();
}

void vdec_resume_iova_smmu_pre(void)
{
	int i;
	uint32_t start_addr = VDEC_SEG_BUF_BASE + VDEC_SEG_BUF_TOTAL_SIZE;
	uint32_t end_addr = VDEC_MCU_HEAP_BASE;

	for (i = 0; i < VDEC_SEGBUF_IOVA_ENTRY_NUM; ++i) {
		if (g_segbuf_table[i].table != NULL) {
			start_addr = g_segbuf_table[i].iova;
		} else {
			break;
		}
	}

	for (i = 0; i < VDEC_MCU_HEAP_ENTRY_NUM; ++i) {
		if (g_mcu_heap_table[i].table != NULL) {
			end_addr = g_mcu_heap_table[i].iova + VDEC_MCU_HEAP_ONE_BLOCK_SIZE;
		} else {
			break;
		}
	}
	smmu_pre_update_rdstart(start_addr);
	smmu_pre_update_rdend(end_addr);
	dprint(PRN_ALWS, "resume smmu pre config, start:0x%x, end:0x%x", start_addr, end_addr);
}

