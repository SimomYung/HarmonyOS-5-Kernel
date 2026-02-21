/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Define structures for file_info
 * Author: Huawei OS Kernel Lab
 * Create: Thu Sep 14 09:40:18 2023
 */
#ifndef __DEVHOST_API_FILE_INFO_H__
#define __DEVHOST_API_FILE_INFO_H__

#if defined(MODULE)
#include <linux/types.h>
#else
#include <stddef.h>
#endif
struct dma_buf_info;
struct dma_fence_info;
struct fences_info;
struct ion_info;
struct devhost_mem_info;
struct devhost_file_info_ops {
	int (*fill_dma_buf_info)(void *file, struct dma_buf_info *file_info);
	int (*fill_fence_info)(void *file, unsigned int offset, unsigned int max_fences,
			       struct dma_fence_info *file_info, struct fences_info *fences_array);
	int (*fill_ion_info)(void *file, struct ion_info *file_info);
	int (*fill_mem_info)(struct devhost_mem_info *mem_info);
	int (*signal_lowmem_debug)(short oom_score_adj, unsigned long long mem_total, bool is_emergency);
};
int devhost_file_info_ops_init(struct devhost_file_info_ops *ops);
#endif
