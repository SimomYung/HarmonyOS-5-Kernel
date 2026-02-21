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
#define DMA_HEAP_FLAG_CAM_CMA_BUFFER    (0x1 << 4)
#define DMA_HEAP_FLAG_NPU_HUGE_BUFFER   (0x1 << 5)
#define DMA_HEAP_FLAG_DIRECT_RECLAIM    (0x1 << 6)
#define DMA_HEAP_FLAG_DRM_REDRAW    	(0x1 << 7)
#define DMA_HEAP_FLAG_NPU_SHARE         (0x1 << 22)

#ifdef CONFIG_MM_SECMEM_DRM_TEE_API_SUPPORT
#define DMA_HEAP_FLAGS_DRM_TEE_SHIFT 10
#define DMA_HEAP_FLAGS_DRM_TEE_MASK 0xF << 10

#define DMA_HEAP_FLAGS_2_TA_RW_INDEX(flags) \
	((flags & DMA_HEAP_FLAGS_DRM_TEE_MASK) >> DMA_HEAP_FLAGS_DRM_TEE_SHIFT)
#endif /* CONFIG_MM_SECMEM_DRM_TEE_API_SUPPORT */

#endif /* _DMA_HEAP_EXT_H */