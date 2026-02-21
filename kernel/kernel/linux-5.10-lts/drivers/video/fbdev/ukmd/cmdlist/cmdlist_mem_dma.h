/**
 * @file cmdlist_mem_mgr.h
 * @brief Memory managment for cmdlist and other users which may need ion or dma memory
 *
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef __CMDLIST_MEM_DMA_H__
#define __CMDLIST_MEM_DMA_H__

struct cmdlist_mem_mgr;
struct device;
struct vm_area_struct;

int32_t cmdlist_mem_dma_init(struct cmdlist_mem_mgr *mem_mgr, struct device *of_dev);
void cmdlist_mem_dma_deinit(struct cmdlist_mem_mgr *mem_mgr, struct device *of_dev);
int32_t cmdlist_mem_dma_mmap(struct cmdlist_mem_mgr *mem_mgr, struct vm_area_struct *vma);

#endif
