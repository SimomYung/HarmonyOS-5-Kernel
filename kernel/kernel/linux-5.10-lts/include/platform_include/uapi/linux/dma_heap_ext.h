 /*
 * dma_heap_ext.h
 *
 * Copyright (C) Huawei Technologies Co., Ltd. 2022. All rights reserved.
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

#ifndef _DMA_HEAP_EXT_H
#define _DMA_HEAP_EXT_H

/*
 * This header defines some macros that dma-heap is open to kernel mode.
 *
 * This file is completely consistent with the definition data of the
 * dma_heap_ext.h file under vendor/hisi/ap/inlcude/memory/,
 * please ensure the consistency of the two files
 */
#define DMA_HEAP_FLAG_CACHED            (0x1 << 0)
#define DMA_HEAP_FLAG_SECURE_BUFFER     (0x1 << 3)
#define DMA_HEAP_FLAG_CAM_CMA_BUFFER    (0x1UL << 4)
#define DMA_HEAP_FLAG_NPU_HUGE_BUFFER   (0x1 << 5)
#define DMA_HEAP_FLAG_DIRECT_RECLAIM    (0x1 << 6)
#define DMA_HEAP_FLAG_DRM_REDRAW    	(0x1 << 7)
#define DMA_HEAP_FLAG_NPU_SHARE         (0x1 << 22)

#ifdef CONFIG_TRANSPARENT_HUGEPAGE_DEVICE 
#define DMA_HEAP_FLAG_HUGEPAGE_BUFFER	(0x1 << 8)
#endif

#ifdef CONFIG_MM_SECMEM_DRM_TEE_API_SUPPORT
#define DMA_HEAP_FLAGS_DRM_TEE_SHIFT 10
#define DMA_HEAP_FLAGS_DRM_TEE_MASK 0xF << 10

#define DMA_HEAP_FLAGS_2_TA_RW_INDEX(flags) \
	((flags & DMA_HEAP_FLAGS_DRM_TEE_MASK) >> DMA_HEAP_FLAGS_DRM_TEE_SHIFT)
#endif /* CONFIG_MM_SECMEM_DRM_TEE_API_SUPPORT */

/**
 * 1. SysCache Prot: Start from BIT(12), use 8 bits (bit 12 ~ 19) totally
 * 2. Outer-shareable flag: use BIT(20).
 *    DMA_HEAP_FLAG_MM_LB_OSH_MASK equal to ION_FLAG_MM_LB_OSH_MASK
 */
#define DMA_HEAP_FLAG_MM_LB_SHIFT       12
#define DMA_HEAP_FLAG_MM_LB_MASK        (0xffUL << DMA_HEAP_FLAG_MM_LB_SHIFT)

#define DMA_HEAP_FLAG_MM_LB_OSH_SHIFT   20
#define DMA_HEAP_FLAG_MM_LB_OSH_MASK    (0x1UL << DMA_HEAP_FLAG_MM_LB_OSH_SHIFT)

#define DMA_HEAP_FLAG_2_PLC_ID(flags) \
	(((flags) & DMA_HEAP_FLAG_MM_LB_MASK) >> DMA_HEAP_FLAG_MM_LB_SHIFT)

/* indicate the master use the dma buffer allocated form dmabuff heap */
#define DMA_HEAP_FLAGS_USAGE_ID_DSS 0
#define DMA_HEAP_FLAGS_USAGE_ID_MAX 1

/* bit32~bit35 */
#define DMA_HEAP_FLAGS_USAGE_SHIFT 32
#define DMA_HEAP_FLAGS_USAGE_MASK (0xFUL << DMA_HEAP_FLAGS_USAGE_SHIFT)
#define DMA_HEAP_FLAGS_2_USAGE_FLAGS(flags) \
	(((flags) & DMA_HEAP_FLAGS_USAGE_MASK) >> DMA_HEAP_FLAGS_USAGE_SHIFT)

#define DMA_HEAP_FLAG_USAGE_GEN_MASK(id) (0x1UL << (DMA_HEAP_FLAGS_USAGE_SHIFT + (id)))
#define DMA_HEAP_FLAG_USAGE_DSS_MASK DMA_HEAP_FLAG_USAGE_GEN_MASK(DMA_HEAP_FLAGS_USAGE_ID_DSS)

/* dmabuff heap notify chain support event types */
#define DMA_HEAP_NOTIFY_EVENT_ALLOC 0
#define DMA_HEAP_NOTIFY_EVENT_MAX 1

#endif /* _DMA_HEAP_EXT_H */