/*
 * vcodec_vdec_memory.c
 *
 * Implement vdec memory function which is used only by formal version
 *
 * Copyright (c) 2024-2024 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */

#include <linux/dma-mapping.h>
#include <linux/dma-iommu.h>
#include <linux/dma-buf.h>
#include <linux/iommu.h>
#include <asm/cacheflush.h>
#include <linux/scatterlist.h>
#ifdef CONFIG_VCODEC_OHOS
#include <linux/dma-heap.h>
#include <linux/dmabuf/mm_dma_heap.h>
#include "uapi/linux/dma_heap_ext.h"
#else
#include <linux/ion.h>
#include <linux/ion/mm_ion.h>
#endif
#include <linux/syscalls.h>
#include <linux/mm_lb/mm_lb.h>
#include "vcodec_vdec.h"
#include "vcodec_vdec_memory.h"
#include "vcodec_vdec_plat.h"
#include "smmu/smmu.h"

static const uint32_t g_orders[] = {10, 8, 4, 0};
#define VDEC_NUM_ORDERS ARRAY_SIZE(g_orders)

struct vcodec_vdec_memory_ctx {
	uint32_t reserved;
};

static struct vcodec_vdec_memory_ctx* memory_ctx(void)
{
	static struct vcodec_vdec_memory_ctx ctx = {0};
	return &ctx;
}

struct vcodec_vdec_memory_ctx* vcodec_vdec_memory_request_ctx(void)
{
	struct vcodec_vdec_memory_ctx* ctx = memory_ctx();

	int32_t ret = smmu_map_reg();
	if (ret) {
		dprint(PRN_ERROR, "smmu map reg failed");
		return NULL;
	}
	ret = smmu_init();
	if (ret) {
		smmu_unmap_reg();
		dprint(PRN_ERROR, "smmu init failed");
		return NULL;
	}

	return ctx;
}

void vcodec_vdec_memory_free_ctx(struct vcodec_vdec_memory_ctx* ctx)
{
	if (unlikely(ctx != memory_ctx()))
		return;

	smmu_deinit();
	smmu_unmap_reg();
}

#ifdef CONFIG_VCODEC_OHOS
static int32_t vdec_alloc_dma_buffer(struct device *dev, uint32_t size, struct vdec_hw_hal_mem_info *mem_info)
{
	char* name = "system_heap";
	struct dma_heap * heap = NULL;
	struct dma_buf *dmabuff = NULL;
	struct dma_buf_attachment *attach = NULL;
	struct sg_table *sgt = NULL;
 
	dmabuff = dma_heap_buffer_alloc_kernel(name, size, 0);
	if (IS_ERR_OR_NULL(dmabuff)) {
		dprint(PRN_ERROR, "fail to get dma buffer\n");
		return -ENOMEM;
	}
 
	attach = dma_buf_attach(dmabuff, dev);
	if (IS_ERR_OR_NULL(attach)) {
		dprint(PRN_ERROR, "fail to attach dma buf\n");
		goto err_dma_put;
	}
 
	sgt = dma_buf_map_attachment(attach, DMA_BIDIRECTIONAL);
	if (IS_ERR_OR_NULL(sgt)) {
		dprint(PRN_ERROR, "fail to map_attachmen\n");
		goto err_dma_detach;
	}
 
	mem_info->dmabuf = dmabuff;
	mem_info->hal_table = sgt;
	mem_info->attach = attach;
	return 0;
 
err_dma_detach:
	dma_buf_detach(dmabuff, attach);
err_dma_put:
	dma_heap_buffer_free_kernel(dmabuff);
 
	return -ENOMEM;
}
 
static void vdec_free_dma_buffer(struct vdec_hw_hal_mem_info *mem_info)
{
	if (!mem_info->attach || !mem_info->dmabuf || !mem_info->hal_table) {
		dprint(PRN_ERROR, "invalid param\n");
		return;
	}
 
	dma_buf_unmap_attachment(mem_info->attach, mem_info->hal_table, DMA_BIDIRECTIONAL);
	dma_buf_detach(mem_info->dmabuf, mem_info->attach);
	dma_heap_buffer_free_kernel(mem_info->dmabuf);
}

#else

extern int ion_alloc(size_t len, unsigned int heap_id_mask, unsigned int flags);
static int32_t vdec_alloc_dma_buffer(struct device *dev, uint32_t size,
				struct vdec_hw_hal_mem_info *mem_info)
{
	int32_t share_fd = -1;
	struct dma_buf *dmabuff = NULL;
	struct dma_buf_attachment *attach = NULL;
	struct sg_table *sgt = NULL;
	unsigned int flags = ION_FLAG_NOT_ZERO_BUFFER;

	if (mem_info->is_alloc_sc)
		flags |= PID_VDEC << ION_FLAG_MM_LB_SHIFT;

	share_fd = ion_alloc(size, 1 << ION_SYSTEM_HEAP_ID, flags);
	if (share_fd < 0) {
		dprint(PRN_ERROR, "fail to alloc ion buffer,size %d", size);
		return -ENOMEM;
	}

	dmabuff = dma_buf_get(share_fd);
	if (IS_ERR_OR_NULL(dmabuff)) {
		dprint(PRN_ERROR, "fail to get dma buffer");
		goto err_ion_free;
	}

	attach = dma_buf_attach(dmabuff, dev);
	if (IS_ERR_OR_NULL(attach)) {
		dprint(PRN_ERROR, "fail to attach dma buf");
		goto err_dma_put;
	}

	sgt = dma_buf_map_attachment(attach, DMA_BIDIRECTIONAL);
	if (IS_ERR_OR_NULL(sgt)) {
		dprint(PRN_ERROR, "fail to map_attachmen");
		goto err_dma_detach;
	}

	mem_info->dmabuf = dmabuff;
	mem_info->hal_table = sgt;
	mem_info->attach = attach;
	ksys_close((unsigned int)share_fd);
	return 0;

err_dma_detach:
	dma_buf_detach(dmabuff, attach);
err_dma_put:
	dma_buf_put(dmabuff);
err_ion_free:
	ksys_close((unsigned int)share_fd);
	return -ENOMEM;
}

static void vdec_free_dma_buffer(struct vdec_hw_hal_mem_info *mem_info)
{
	if (!mem_info->attach || !mem_info->dmabuf || !mem_info->hal_table) {
		dprint(PRN_ERROR, "invalid param");
		return;
	}

	dma_buf_unmap_attachment(mem_info->attach, mem_info->hal_table, DMA_BIDIRECTIONAL);
	dma_buf_detach(mem_info->dmabuf, mem_info->attach);
	dma_buf_put(mem_info->dmabuf);
}
#endif

// alloc ion and map iova
int32_t vdec_alloc_dma_and_map(struct device *dev, uint32_t size, struct vdec_hw_hal_mem_info *mem_info)
{
    mem_info->is_map_virtual = 0;
	if (vdec_alloc_dma_buffer(dev, size, mem_info)) {
		return -ENOMEM;
	}

	if (vdec_mem_iommu_vir_map(dev, mem_info)) {
        vdec_free_dma_buffer(mem_info);
		return -ENOMEM;
	}

	return 0;
}
// unmap iova & virtual addr and free ion buffer
void vdec_free_dma_and_unmap(struct device *dev, struct vdec_hw_hal_mem_info *mem_info)
{
	vdec_mem_iommu_vir_unmap(dev, mem_info);
    vdec_free_dma_buffer(mem_info);
}

// alloc ion then map iova and virual addr
int32_t vdec_alloc_dma_and_map_vir(struct device *dev, uint32_t size, struct vdec_hw_hal_mem_info *mem_info)
{
    mem_info->is_map_virtual = 1;
    if (vdec_alloc_dma_buffer(dev, size, mem_info)) {
        return -ENOMEM;
    }

    if (vdec_mem_iommu_vir_map(dev, mem_info)) {
        vdec_free_dma_buffer(mem_info);
        return -ENOMEM;
    }

    return 0;
}

// unmap iova & virtual addr and free ion buffer
void vdec_free_dma_and_unmap_vir(struct device *dev, struct vdec_hw_hal_mem_info *mem_info)
{
    vdec_free_dma_and_unmap(dev, mem_info);
    (void)memset_s(mem_info, sizeof(*mem_info), 0, sizeof(*mem_info));
}

// alloc ion with system cache, and map iova
int32_t vdec_mem_alloc_with_sc(struct device *dev, uint32_t size, struct vdec_hw_hal_mem_info *mem_info)
{
	mem_info->is_alloc_sc = 1;
	if (vdec_alloc_dma_buffer(dev, size, mem_info)) {
		dprint(PRN_ERROR, "vdec alloc system cache share fd error");
		mem_info->is_alloc_sc = 0;
		return -ENOMEM;
	}

	// iommu mmap
	mem_info->is_map_virtual = 0;
	if (vdec_mem_iommu_vir_map(dev, mem_info)) {
		dprint(PRN_ERROR, "vdec system cache map iommu virtual address error");
        vdec_free_dma_buffer(mem_info);
		return -ENOMEM;
	}

	return 0;
}

void vdec_mem_free_with_sc(struct device *dev, struct vdec_hw_hal_mem_info *mem_info)
{
	vdec_mem_iommu_vir_unmap(dev, mem_info);
    vdec_free_dma_buffer(mem_info);
}