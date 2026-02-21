/**
 * @file cmdlist_mem.c
 * @brief Cmdlist memory management interface
 *
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
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
#include "ukmd_log.h"

#include "chash.h"
#include "cmdlist_node.h"
#include "cmdlist_client.h"
#include "cmdlist_dev.h"
#include "cmdlist_drv.h"
#include "ukmd_acquire_fence.h"

void *cmdlist_mem_alloc(uint32_t dev_id,
	uint32_t size, dma_addr_t *phy_addr, uint32_t *out_buffer_size)
{
	void *vir_addr = NULL;
	uint32_t alloc_size;
	struct cmdlist_private *cmd_priv = cmdlist_get_priv_by_id(dev_id);

	if (unlikely(!cmd_priv || !phy_addr || !out_buffer_size)) {
		ukmd_pr_err("check input param error!");
		return NULL;
	}
	*out_buffer_size = 0;

	if (IS_ERR_OR_NULL(cmd_priv->memory_pool)) {
		ukmd_pr_err("check memory_pool failed!");
		return NULL;
	}

	alloc_size = roundup(size, PAGE_SIZE / 4);
	vir_addr = (void *)(uintptr_t)gen_pool_dma_alloc(cmd_priv->memory_pool, alloc_size, phy_addr);
	if (!vir_addr) {
		ukmd_pr_err("gen pool alloc failed, size:%#x!", alloc_size);
		return NULL;
	}
	memset(vir_addr, 0, alloc_size);
	*out_buffer_size = alloc_size;
	ukmd_pr_debug("available size=%llx got size=%#x", gen_pool_avail(cmd_priv->memory_pool), alloc_size);

	return vir_addr;
}

void cmdlist_mem_free(uint32_t dev_id, void *vir_addr, uint32_t size)
{
	struct cmdlist_private *cmd_priv = cmdlist_get_priv_by_id(dev_id);

	if (unlikely(!cmd_priv || !vir_addr)) {
		ukmd_pr_err("check input param error!");
		return;
	}

	if (IS_ERR_OR_NULL(cmd_priv->memory_pool)) {
		ukmd_pr_err("check memory_pool failed!");
		return;
	}
	ukmd_pr_debug("mem_free --> size:%#x", size);
	gen_pool_free(cmd_priv->memory_pool, (unsigned long)(uintptr_t)vir_addr, size);
	vir_addr = NULL;
}
