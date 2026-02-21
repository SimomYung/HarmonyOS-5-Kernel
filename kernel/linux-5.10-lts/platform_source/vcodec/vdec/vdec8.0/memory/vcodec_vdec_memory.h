/*
 * vcodec_vdec_memory.h
 *
 * Headers define of vcodec vdec memory
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
#ifndef VCODEC_VDEC_MEMORY_H
#define VCODEC_VDEC_MEMORY_H

#include <linux/dma-mapping.h>
#include <linux/dma-buf.h>
#include <linux/device.h>
#include <linux/fb.h>
#include <linux/version.h>
#include <linux/iommu/mm_iommu.h>
#include "vcodec_types.h"

#define MEM_BUF_MAPPED     1
#define MEM_BUF_UNMAPPED   0
#define VDEC_PXPC_SC_SIZE  (1024 * 1024)

struct vdec_dma_mem {
	uint32_t size;
	void *virt_addr;
	dma_addr_t dma_handle;
	uint32_t iova_addr;
};

struct vdec_hw_hal_mem_info {
	uint32_t is_map_virtual;
	uint32_t is_alloc_sc;
	int32_t share_fd;
	uint32_t iova;
	void *virt_addr;
	uint64_t iova_size;
	struct sg_table *hal_table;
	struct dma_buf *dmabuf;
	struct dma_buf_attachment *attach;
};

struct vdec_buffer_record {
	int32_t share_fd;
	UADDR iova;
	uint64_t vir_addr;
	uint32_t iova_size;
};

struct vdec_fd_listnode {
	struct list_head head;
	int32_t share_fd;
	uint32_t iova;
	uint32_t iova_size;
	struct dma_buf *dmabuf;
};

struct vdec_fd_list {
	struct mutex lock;
	struct list_head fdhead;
};

struct vcodec_vdec_memory_ctx;

struct vcodec_vdec_memory_ctx* vcodec_vdec_memory_request_ctx(void);
void vcodec_vdec_memory_free_ctx(struct vcodec_vdec_memory_ctx* ctx);

// common interface
int32_t vdec_mem_probe(void *dev);
int32_t vdec_mem_iommu_map(void *dev, struct vdec_buffer_record *buf_record,
	struct file *file_handle, struct dma_buf **dmabuf);
int32_t vdec_mem_iommu_unmap(void *dev, int32_t share_fd, struct file *file_handle,
	struct dma_buf *dmabuf, uint32_t iova_addr);
void *vdec_dma_buf_get(int32_t share_fd);
void vdec_dma_buf_put(void *dmabuf);
uint32_t *vdec_mmap(uint32_t addr, uint32_t range);
void vdec_munmap(uint32_t *mem_addr);
int32_t vdec_mem_iommu_vir_map(void *dev, struct vdec_hw_hal_mem_info *mem_info);
int32_t vdec_mem_iommu_vir_unmap(void *dev, struct vdec_hw_hal_mem_info *mem_info);
void vdec_share_fd_list_init(struct vdec_fd_list *list);
int32_t vdec_share_fd_list_get(struct vdec_fd_list *list, int32_t share_fd,
	uint32_t *iova_addr, uint32_t *iova_size);
int32_t vdec_share_fd_list_add(struct vdec_fd_list *list, int32_t share_fd,
	struct dma_buf *dmabuf, uint32_t iova_addr, uint32_t iova_size);
int32_t vdec_share_fd_list_remove(struct vdec_fd_list *list, int32_t share_fd,
	struct dma_buf **dmabuf, uint32_t *iova_addr);

// interface different in smmu bypass and formal version
int32_t vdec_alloc_dma_and_map_vir(struct device *dev, uint32_t size, struct vdec_hw_hal_mem_info *mem_info);
void vdec_free_dma_and_unmap_vir(struct device *dev, struct vdec_hw_hal_mem_info *mem_info);
int32_t vdec_alloc_dma_and_map(struct device *dev, uint32_t size, struct vdec_hw_hal_mem_info *mem_info);
void vdec_free_dma_and_unmap(struct device *dev, struct vdec_hw_hal_mem_info *mem_info);
int32_t vdec_mem_alloc_with_sc(struct device *dev, uint32_t size, struct vdec_hw_hal_mem_info *mem_info);
void vdec_mem_free_with_sc(struct device *dev, struct vdec_hw_hal_mem_info *mem_info);
int vdec_alloc_iova(uint32_t iova, uint32_t len, struct vdec_hw_hal_mem_info *mem);
int vdec_free_iova(struct vdec_hw_hal_mem_info *mem);
int vdec_alloc_iova_with_va(uint32_t iova, uint32_t len, struct vdec_hw_hal_mem_info *mem);
int vdec_free_iova_with_va(struct vdec_hw_hal_mem_info *mem);
int32_t vdec_alloc_mcu_heap_iova(uint32_t *addr);
int vdec_free_mcu_heap_iova(uint32_t free_iova);
int32_t vdec_alloc_segbuffer(uint32_t *addr);
int vdec_free_segbuffer(uint32_t free_iova);
void vdec_init_reserved_iova(void);
void vdec_deinit_reserved_iova(void);
void vdec_suspend_iova_smmu_pre(void);
void vdec_resume_iova_smmu_pre(void);
#endif