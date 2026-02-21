/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Prototypes and type definitions for file_info module
 * Author: Huawei OS Kernel Lab
 * Create: Thu Sep 14 04:44:42 2023
 */
#ifndef __DEVHOST_PLUGIN_FILE_INFO_MODULE_FILE_INFO_H__
#define __DEVHOST_PLUGIN_FILE_INFO_MODULE_FILE_INFO_H__

struct dma_buf_info;
struct dma_fence_info;
struct fences_info;
struct ion_info;
struct liblinux_mem_info;
int dmaheap_debug_dma_buf_info(struct file *file, struct dma_buf_info *file_info);
int memcheck_debug_fence_info(struct file *file, unsigned int offset, unsigned int max_fences,
			      struct dma_fence_info *file_info, struct fences_info *fences_array);
int memcheck_debug_ion_info(struct file *file, struct ion_info *file_info);
int liblinux_fill_mem_info(struct liblinux_mem_info *mem_info);
void liblinux_lowmem_debug(short oom_score_adj, unsigned long long mem_total, bool is_emergency);
void liblinux_get_gpu_mem_usage(uint32_t pid, unsigned long *gpu_info);

#endif /* __DEVHOST_PLUGIN_FILE_INFO_MODULE_FILE_INFO_H__ */
