/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jul 05 15:51:29 2019
 */
#include <asm/cacheflush.h>
#include <linux/mm.h>
#include <linux/mman.h>
#include <linux/export.h>
#include <linux/version.h>
#include <linux/securec.h>
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,19,0)
#include <linux/dma-direct.h>
#endif
#include <linux/highmem.h>
#include <linux/dma-mapping.h>
#include <linux/scatterlist.h>
#include <lnxbase/lnxbase.h>

#if LINUX_VERSION_CODE < KERNEL_VERSION(4,4,206)
typedef struct dma_attrs *dma_attr;
static inline bool is_skip_cpu_sync(dma_attr attrs)
{
	return false;
}
#else
typedef unsigned long dma_attr;
static inline bool is_skip_cpu_sync(dma_attr attrs)
{
	return (attrs & DMA_ATTR_SKIP_CPU_SYNC) ? true : false;
}
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(4,9,0)
/*
 * __dma_flush_area() prototype for linux before 4.9,
 * it is implement as a weak symbol in devhost `symbols.c`.
 */
extern void __dma_flush_area(const void *, size_t);
#endif

static void __dma_map_pg_area(struct page *pg, size_t off, size_t len, int dir)
{
	unsigned int cmd;

	if (!PageHighMem(pg)) {
		__dma_map_area(page_address(pg) + off, len, dir);
		return;
	}

	if (dir == DMA_FROM_DEVICE) {
		cmd = LIBLINUX_PAL_DCACHE_INVAL_RANGE;
	} else {
		cmd = LIBLINUX_PAL_DCACHE_CLEAN_RANGE;
	}

	lnxbase_flush_cache_range_by_pa(cmd, (unsigned long long)page_to_phys(pg) + off,
					(unsigned long long)page_to_phys(pg) + len);
}

static void __dma_unmap_pg_area(struct page *pg, size_t off, size_t len, int dir)
{
	if (!PageHighMem(pg)) {
		__dma_unmap_area(page_address(pg) + off, len, dir);
		return;
	}

	if (dir == DMA_FROM_DEVICE) {
		lnxbase_flush_cache_range_by_pa(LIBLINUX_PAL_DCACHE_INVAL_RANGE,
						(unsigned long long)page_to_phys(pg) + off,
						(unsigned long long)page_to_phys(pg) + len);
	}
}

static void *dma_cma_devhost_alloc(struct device *dev, size_t size, gfp_t gfp)
{
#ifdef CONFIG_LIBLINUX_DMA_CMA
	struct page *page = NULL;

	page = dma_alloc_contiguous(dev, size, gfp);
	if (!page) {
		page = alloc_pages(gfp | __GFP_DMA, get_order(size));
		if (!page)
			return NULL;
	}
	return page_to_virt(page);
#else
	return alloc_pages_exact(size, gfp | __GFP_DMA);
#endif
}

static void dma_cma_devhost_free(struct device *dev, void *vaddr, size_t size)
{
#ifdef CONFIG_LIBLINUX_DMA_CMA
	dma_free_contiguous(dev, virt_to_page(vaddr), size);
#else
	free_pages_exact(vaddr, size);
#endif
}

static void *dma_devhost_alloc(struct device *dev, size_t size,
			       dma_addr_t *dma_handle, gfp_t gfp,
			       dma_attr attrs)
{
	int ret;
	void *naddr = NULL;
	void *vaddr = NULL;
	phys_addr_t paddr;
	unsigned long prot = PROT_READ | PROT_WRITE;
	unsigned int flag = LIBLINUX_PAL_REMAP_NORMAL_NC;

	size = PAGE_ALIGN(size);
	vaddr = dma_cma_devhost_alloc(dev, size, gfp);
	if (vaddr == NULL) {
		return NULL;
	}
	__dma_flush_area(vaddr, size);

	naddr = liblinux_pal_vm_prepare(0, size, prot, flag);
	if (naddr == NULL) {
		dma_cma_devhost_free(dev, vaddr, size);
		return NULL;
	}

	paddr = virt_to_phys(vaddr);
	ret = liblinux_pal_vm_mmap(paddr, (unsigned long)(uintptr_t)naddr,
				   size, prot, flag);
	if (ret < 0) {
		liblinux_pal_vm_unmap(naddr);
		dma_cma_devhost_free(dev, vaddr, size);
		return NULL;
	}

	ret = memset_s(naddr, size, 0, size);
	BUG_ON(ret != 0);

	*dma_handle = phys_to_dma(dev, paddr);
	return naddr;
}

static void dma_devhost_free(struct device *dev, size_t size,
			     void *cpu_addr, dma_addr_t dma_addr,
			     dma_attr attrs)
{
	void *vaddr = phys_to_virt(dma_to_phys(dev, dma_addr));
	liblinux_pal_vm_unmap(cpu_addr);
	dma_cma_devhost_free(dev, vaddr, size);
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 10, 0)
static bool is_device_dma_coherent(struct device *dev)
{
	return dev_is_dma_coherent(dev);
}
#endif

static dma_addr_t dma_devhost_map_page(struct device *dev, struct page *page,
				      unsigned long offset, size_t size,
				      enum dma_data_direction dir,
				      dma_attr attrs)
{
	phys_addr_t paddr = page_to_phys(page) + offset;
	dma_addr_t dma_addr = phys_to_dma(dev, paddr);

	if (!is_device_dma_coherent(dev) && !is_skip_cpu_sync(attrs)) {
		__dma_map_pg_area(page, offset, size, dir);
	}
	return dma_addr;
}

static void dma_devhost_unmap_page(struct device *dev, dma_addr_t dma_handle,
				   size_t size, enum dma_data_direction dir,
				   dma_attr attrs)
{
	if (!is_device_dma_coherent(dev) && !is_skip_cpu_sync(attrs)) {
		phys_addr_t paddr = dma_to_phys(dev, dma_handle);
		unsigned long offset = paddr & ~PAGE_MASK;
		__dma_unmap_pg_area(phys_to_page(paddr), offset, size, dir);
	}
}

static int dma_devhost_map_sg(struct device *dev, struct scatterlist *sgl, int nents,
			      enum dma_data_direction dir, dma_attr attrs)
{
	int i;
	struct scatterlist *sg = NULL;

	for_each_sg(sgl, sg, nents, i) {
		BUG_ON(!sg_page(sg));

		sg_dma_address(sg) = phys_to_dma(dev, sg_phys(sg));
		sg_dma_len(sg) = sg->length;

		if (!is_device_dma_coherent(dev) && !is_skip_cpu_sync(attrs)) {
			__dma_map_pg_area(sg_page(sg), sg->offset, sg->length, dir);
		}
	}

	return nents;
}

static void dma_devhost_unmap_sg(struct device *dev, struct scatterlist *sgl, int nents,
				 enum dma_data_direction dir, dma_attr attrs)
{
	int i;
	struct scatterlist *sg;

	if (!is_device_dma_coherent(dev) && !is_skip_cpu_sync(attrs)) {
		for_each_sg(sgl, sg, nents, i) {
			BUG_ON(!sg_page(sg));
			__dma_unmap_pg_area(sg_page(sg), sg->offset, sg->length, dir);
		}
	}
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0)
static int dma_devhost_mapping_error(struct device *dev, dma_addr_t dma_addr)
{
	return 0;
}
#endif

static int dma_devhost_supported(struct device *dev, u64 mask)
{
	return 1;
}

static void dma_devhost_sync_single_for_cpu(struct device *dev,
					    dma_addr_t dma_handle, size_t size,
					    enum dma_data_direction dir)
{
	__dma_unmap_area(phys_to_virt(dma_handle), size, dir);
}
static void dma_devhost_sync_single_for_device(struct device *dev,
					       dma_addr_t dma_handle, size_t size,
					       enum dma_data_direction dir)
{
	__dma_map_area(phys_to_virt(dma_handle), size, dir);
}

static void dma_devhost_sync_sg_for_cpu(struct device *dev,
					struct scatterlist *sgl, int nents,
					enum dma_data_direction dir)
{
	int i;
	struct scatterlist *sg = NULL;

	for_each_sg(sgl, sg, nents, i) {
		BUG_ON(!sg_page(sg));
		__dma_unmap_area(sg_virt(sg), sg->length, dir);
	}
}

static void dma_devhost_sync_sg_for_device(struct device *dev,
					   struct scatterlist *sgl, int nents,
					   enum dma_data_direction dir)
{
	int i;
	struct scatterlist *sg = NULL;

	for_each_sg(sgl, sg, nents, i) {
		BUG_ON(!sg_page(sg));
		__dma_map_area(sg_virt(sg), sg->length, dir);
	}
}

struct dma_map_ops dma_devhost_ops __read_mostly = {
	.alloc			= dma_devhost_alloc,
	.free			= dma_devhost_free,
	.map_page		= dma_devhost_map_page,
	.unmap_page		= dma_devhost_unmap_page,
	.map_sg			= dma_devhost_map_sg,
	.unmap_sg		= dma_devhost_unmap_sg,
#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0)
	.mapping_error		= dma_devhost_mapping_error,
#endif
	.dma_supported		= dma_devhost_supported,
	.sync_single_for_cpu	= dma_devhost_sync_single_for_cpu,
	.sync_single_for_device = dma_devhost_sync_single_for_device,
	.sync_sg_for_cpu	= dma_devhost_sync_sg_for_cpu,
	.sync_sg_for_device	= dma_devhost_sync_sg_for_device,
};
