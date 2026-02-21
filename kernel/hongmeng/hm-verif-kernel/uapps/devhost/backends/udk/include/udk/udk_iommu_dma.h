/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: udk iommu dma interfaces
 * Author: Kirin Plat Drv Department
 * Create: Wed Mar 13 15:45:12 2024
 */

#ifndef __UDK_IOMMU_DMA_H__
#define __UDK_IOMMU_DMA_H__

#include <udk/device.h>
#include <udk/lib/scatterlist.h>

#define IOMMU_DMA_BIDIRECTIONAL	0
#define IOMMU_DMA_TO_DEVICE		(1 << 0)
#define IOMMU_DMA_FROM_DEVICE	(1 << 1)
#define IOMMU_DMA_DIR_MASK		(0x3UL)

void *udk_iommu_dma_alloc(struct udk_device *dev, size_t size, uintptr_t *iova);
void udk_iommu_dma_free(struct udk_device *dev, size_t size, void *vaddr, uintptr_t iova);
uintptr_t udk_iommu_dma_map_single(struct udk_device *dev,
			uintptr_t paddr, size_t size, unsigned int attr);
int udk_iommu_dma_unmap_single(struct udk_device *dev,
			uintptr_t iova, size_t size, unsigned int attr);
uintptr_t udk_iommu_dma_map_sg(struct udk_device *dev,
			struct udk_sg_table *sgt, unsigned int attr);
int udk_iommu_dma_unmap_sg(struct udk_device *dev,
			uintptr_t iova, struct udk_sg_table *sgt, unsigned int attr);
uintptr_t udk_iommu_dma_map_sg_refill(struct udk_device *dev,
			struct udk_sg_table *sgt, unsigned int attr);
int udk_iommu_dma_unmap_refilled_sg(struct udk_device *dev,
			uintptr_t iova, struct udk_sg_table *sgt, unsigned int attr);

#endif /* __UDK_IOMMU_DMA_H__ */
