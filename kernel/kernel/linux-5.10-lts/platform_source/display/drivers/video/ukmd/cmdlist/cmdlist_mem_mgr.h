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

#ifndef __CMDLIST_MEM_MGR_H__
#define __CMDLIST_MEM_MGR_H__

#include <linux/types.h>
#include <linux/genalloc.h>
#include <linux/semaphore.h>

struct cmdlist_private;

struct mem_user_info_node {
	struct list_head list_node;
	int32_t user_tgid;
	uint32_t ref_cnt;
	struct list_head mem_list_head;
};

struct cmdlist_mem_mgr {
	struct cmdlist_private *cmd_priv;
	uint32_t memory_type;
	size_t max_size;
	void *mem_info; // for memory manage, etc. ion need map/unmap sg table
	struct gen_pool *mem_pool;
	struct semaphore mem_sem;
	void *pool_vir_addr;
	uint64_t pool_dma_addr;
	size_t pool_dma_size;
	struct list_head user_info_head; // record every user tgid for release

	int32_t (*mem_init)(struct cmdlist_mem_mgr *mem_mgr, struct device *of_dev);
	void (*mem_deinit)(struct cmdlist_mem_mgr *mem_mgr, struct device *of_dev);
	int32_t (*mmap)(struct cmdlist_mem_mgr *mem_mgr, struct vm_area_struct *vma);
	void *(*mem_alloc)(struct cmdlist_mem_mgr *mem_mgr, uint32_t size, dma_addr_t *phy_addr, uint32_t *out_buffer_size);
	void (*mem_free)(struct cmdlist_mem_mgr *mem_mgr, void *vir_addr, uint32_t size);
	uint32_t (*get_pool_size)(struct cmdlist_mem_mgr *mem_mgr);
	uint32_t (*get_pool_avail_size)(struct cmdlist_mem_mgr *mem_mgr);
};

/**
 * @brief memory manager driver probe
 *
 * @param mem_mgr memory manager
 * @param of_dev device of dts tree
 * @param sum_node_size maximum memory size
 * @return int32_t 0: success -1: fail
 */
int32_t cmdlist_mem_mgr_probe(struct cmdlist_mem_mgr *mem_mgr, struct device *of_dev,
	size_t sum_node_size);

/**
 * @brief memory manager driver remove
 *
 * @param mem_mgr memory manager
 * @return void
 */
void cmdlist_mem_mgr_remove(struct cmdlist_mem_mgr *mem_mgr);

/**
 * @brief memory alloc/free from user space ioctl
 *
 * @param mem_mgr memory manager
 * @param tgid tgid of free user
 * @param size size of allocating memory
 * @param viraddr allocated address
 * @return void
 */
int32_t cmdlist_mem_mgr_ioctl(struct cmdlist_mem_mgr *mem_mgr, int32_t tgid, uint32_t cmd,
	unsigned long arg);

/**
 * @brief for user open device
 *
 * @param mem_mgr memory manager
 * @param tgid tgid of opening user
 * @return int32_t 0: success -1: fail
 */
int32_t cmdlist_mem_mgr_open(struct cmdlist_mem_mgr *mem_mgr, int32_t tgid);

/**
 * @brief for user close device
 *
 * @param mem_mgr memory manager
 * @param tgid tgid of free user
 * @return void
 */
 void cmdlist_mem_mgr_release(struct cmdlist_mem_mgr *mem_mgr, int32_t tgid);

/**
 * @brief for user map user space address
 *
 * @param mem_mgr memory manager
 * @param vma vma struct
 */
int32_t cmdlist_mem_mgr_mmap(struct cmdlist_mem_mgr *mem_mgr, struct vm_area_struct *vma);

#endif
