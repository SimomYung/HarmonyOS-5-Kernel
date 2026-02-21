/**
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
#include <securec.h>
#include <linux/platform_device.h>
#include <linux/dma-mapping.h>
#include <linux/platform_device.h>
#include <linux/of_device.h>
#include "ukmd_log.h"
#include "cmdlist_drv.h"
#include "cmdlist_mem_dma.h"
#include "cmdlist_mem_ion.h"
#include "cmdlist_mem_mgr.h"

struct user_mem_info {
	struct list_head list_node;
	void *vir_addr;
	uint32_t size;
};

static uint32_t get_pool_size(struct cmdlist_mem_mgr *mem_mgr)
{
	return (uint32_t)gen_pool_size(mem_mgr->mem_pool);
}

static uint32_t get_pool_avail_size(struct cmdlist_mem_mgr *mem_mgr)
{
	return (uint32_t)gen_pool_avail(mem_mgr->mem_pool);
}

static void *cmdlist_mem_alloc(struct cmdlist_mem_mgr *mem_mgr,
	uint32_t size, dma_addr_t *phy_addr, uint32_t *out_buffer_size)
{
	void *vir_addr = NULL;
	uint32_t alloc_size;

	if (unlikely(!phy_addr || !out_buffer_size)) {
		ukmd_pr_err("check input param error!");
		return NULL;
	}
	*out_buffer_size = 0;

	if (unlikely(IS_ERR_OR_NULL(mem_mgr->mem_pool))) {
		ukmd_pr_err("check mem_pool failed!");
		return NULL;
	}

	alloc_size = (uint32_t)roundup(size, PAGE_SIZE / 4);
	vir_addr = gen_pool_dma_alloc(mem_mgr->mem_pool, alloc_size, phy_addr);
	if (unlikely(!vir_addr)) {
		ukmd_pr_err("gen pool alloc failed, size:%#x!", alloc_size);
		return NULL;
	}
	
	*phy_addr = mem_mgr->pool_dma_addr + (vir_addr - mem_mgr->pool_vir_addr);
	memset_s(vir_addr, alloc_size, 0, alloc_size);
	*out_buffer_size = alloc_size;
	ukmd_pr_debug("available size=%llx got size=%#x", gen_pool_avail(mem_mgr->mem_pool), alloc_size);

	return vir_addr;
}

static void cmdlist_mem_free(struct cmdlist_mem_mgr *mem_mgr, void *vir_addr, uint32_t size)
{
	if (unlikely(!vir_addr)) {
		ukmd_pr_err("check input param error!");
		return;
	}

	if (unlikely(IS_ERR_OR_NULL(mem_mgr->mem_pool))) {
		ukmd_pr_err("check mem_pool failed!");
		return;
	}
	ukmd_pr_debug("mem_free --> size:%#x", size);
	gen_pool_free(mem_mgr->mem_pool, (unsigned long)(uintptr_t)vir_addr, size);
}

static void cmdlist_mem_register(struct cmdlist_mem_mgr *mem_mgr)
{
	if (mem_mgr->memory_type == MEMORY_TYPE_DMA) {
		mem_mgr->mem_init = cmdlist_mem_dma_init;
		mem_mgr->mem_deinit = cmdlist_mem_dma_deinit;
		mem_mgr->mmap = cmdlist_mem_dma_mmap;
	} else { // ION
		mem_mgr->mem_init = cmdlist_mem_ion_init;
		mem_mgr->mem_deinit = cmdlist_mem_ion_deinit;
		mem_mgr->mmap = cmdlist_mem_ion_mmap;
	}
	mem_mgr->mem_alloc = cmdlist_mem_alloc;
	mem_mgr->mem_free = cmdlist_mem_free;
	mem_mgr->get_pool_size = get_pool_size;
	mem_mgr->get_pool_avail_size = get_pool_avail_size;
}

int32_t cmdlist_mem_mgr_probe(struct cmdlist_mem_mgr *mem_mgr, struct device *of_dev,
	size_t sum_node_size)
{
	int32_t ret = of_property_read_u32(of_dev->of_node, "memory_type", &mem_mgr->memory_type);
	if (unlikely(ret != 0)) {
		ukmd_pr_err("get memory_type fail, ret=%d", ret);
		mem_mgr->memory_type = MEMORY_TYPE_DMA;
	}
	ukmd_pr_info("get memory_type: %d", mem_mgr->memory_type);
	mem_mgr->max_size = sum_node_size;

	cmdlist_mem_register(mem_mgr);

	if (unlikely(mem_mgr->mem_init(mem_mgr, of_dev) != 0)) {
		ukmd_pr_err("cmdlist_mem_dma_init fail");
		return -1;
	}

	/**
	 * devm_gen_pool_create: The pool will be automatically destroyed by the device management code
	 */
	mem_mgr->mem_pool = devm_gen_pool_create(of_dev, 4 /* order */, -1,
		of_dev->of_node->name);
	if (unlikely(IS_ERR_OR_NULL(mem_mgr->mem_pool))) {
		ukmd_pr_err("create memory pool failed %d!", PTR_ERR(mem_mgr->mem_pool));
		goto err_pool_create;
	}

	ret = gen_pool_add_virt(mem_mgr->mem_pool, (unsigned long)(uintptr_t)mem_mgr->pool_vir_addr,
		mem_mgr->pool_dma_addr, mem_mgr->max_size, -1);
	if (unlikely(ret != 0)) {
		ukmd_pr_err("memory pool add failed %d!", ret);
		goto err_pool_create;
	}

	sema_init(&mem_mgr->mem_sem, 1);
	INIT_LIST_HEAD(&mem_mgr->user_info_head);
	return 0;

err_pool_create:
	mem_mgr->mem_deinit(mem_mgr, of_dev);
	return -1;
}

static void free_mem_user_info_node(struct cmdlist_mem_mgr *mem_mgr,
	struct list_head *mem_info_head, bool free_mem)
{
	struct user_mem_info *user_node = NULL;
	struct user_mem_info *_u_node_ = NULL;

	list_for_each_entry_safe(user_node, _u_node_, mem_info_head, list_node) {
		if (unlikely(!user_node))
			continue;

		if (free_mem) // when one process release, need free mem, when driver remove, no need
			mem_mgr->mem_free(mem_mgr, user_node->vir_addr, user_node->size);

		list_del(&user_node->list_node);
		kfree(user_node);
	}
}

static void cmdlist_mem_remove_list(struct cmdlist_mem_mgr *mem_mgr)
{
	struct user_info_node *user_node = NULL;
	struct user_info_node *_u_node_ = NULL;
	struct mem_user_info_node *node;

	list_for_each_entry_safe(user_node, _u_node_, &mem_mgr->user_info_head, list_node) {
		if (unlikely(!user_node))
			continue;

		node = (struct mem_user_info_node *)user_node;
		free_mem_user_info_node(mem_mgr, &node->mem_list_head, false);

		list_del(&user_node->list_node);
		kfree(user_node);
		user_node = NULL;
	}
}

void cmdlist_mem_mgr_remove(struct cmdlist_mem_mgr *mem_mgr)
{
	if (unlikely(IS_ERR_OR_NULL(mem_mgr->mem_pool))) {
		ukmd_pr_err("create memory pool failed %d!", PTR_ERR(mem_mgr->mem_pool));
		return;
	}

	ukmd_pr_info("pool memery status: %d, %d!",
		mem_mgr->get_pool_size(mem_mgr), mem_mgr->get_pool_avail_size(mem_mgr));
	down(&mem_mgr->mem_sem);
	cmdlist_mem_remove_list(mem_mgr);
	mem_mgr->mem_deinit(mem_mgr, mem_mgr->cmd_priv->of_dev);
	up(&mem_mgr->mem_sem);
}

int32_t cmdlist_mem_mgr_open(struct cmdlist_mem_mgr *mem_mgr, int32_t tgid)
{
	struct mem_user_info_node *node;

	down(&mem_mgr->mem_sem);
	node = (struct mem_user_info_node *)(void *)get_user_info_node(&mem_mgr->user_info_head, tgid);
	if (!node) {
		node = kzalloc(sizeof(struct mem_user_info_node), GFP_KERNEL);
		if (likely(node)) {
			node->user_tgid = tgid;
			node->ref_cnt = 1;
			INIT_LIST_HEAD(&node->mem_list_head);
			list_add_tail(&node->list_node, &mem_mgr->user_info_head);
		} else {
			ukmd_pr_err("kzalloc return err!");
			up(&mem_mgr->mem_sem);
			return -ENOMEM;
		}
	} else {
		ukmd_pr_warn("device has opened before by tgid=%d of %u times", tgid, node->ref_cnt);
		++node->ref_cnt;
	}
	
	up(&mem_mgr->mem_sem);
	return 0;
}

void cmdlist_mem_mgr_release(struct cmdlist_mem_mgr *mem_mgr, int32_t tgid)
{
	struct mem_user_info_node *node;

	down(&mem_mgr->mem_sem);
	node = (struct mem_user_info_node *)(void *)get_user_info_node(&mem_mgr->user_info_head, tgid);
	if (unlikely(!node)) {
		ukmd_pr_warn("cannot find tgid=%d", tgid);
	} else {
		ukmd_pr_warn("device has opened before by tgid=%d of %u times", tgid, node->ref_cnt);
		if (node->ref_cnt > 0)
			--node->ref_cnt;

		if (node->ref_cnt == 0) {
			free_mem_user_info_node(mem_mgr, &node->mem_list_head, true);
			list_del(&node->list_node);
			kfree(node);
		}
	}
	
	up(&mem_mgr->mem_sem);
}

static int32_t cmdlist_mem_mgr_alloc(struct cmdlist_mem_mgr *mem_mgr, int32_t tgid,
	struct cmdlist_mem_request *mem_req, void __user *argp)
{
	void *vir_addr = NULL;
	dma_addr_t phy_addr;
	struct user_mem_info *client_info;
	struct mem_user_info_node *node;

	vir_addr = cmdlist_mem_alloc(mem_mgr, mem_req->size, &phy_addr, &mem_req->size);
	if (unlikely(!vir_addr)) {
		ukmd_pr_err("gen pool alloc failed, size:%#x!", mem_req->size);
		return -1;
	}

	node = (struct mem_user_info_node *)(void *)get_user_info_node(&mem_mgr->user_info_head, tgid);
	if (unlikely(!node)) {
		ukmd_pr_err("cannot find tgid=%d!", tgid);
		cmdlist_mem_free(mem_mgr, vir_addr, mem_req->size);
		return -1;
	}
	
	client_info = (struct user_mem_info *)kzalloc(sizeof(struct user_mem_info), GFP_KERNEL);
	if (unlikely(!client_info)) {
		ukmd_pr_err("kzalloc return err!");
		cmdlist_mem_free(mem_mgr, vir_addr, mem_req->size);
		return -ENOMEM;
	}
	client_info->vir_addr = vir_addr;
	client_info->size = mem_req->size;
	
	ukmd_pr_debug("add to tgid=%d head!", tgid);
	list_add_tail(&client_info->list_node, &node->mem_list_head);
	
	mem_req->va_offset = ((uint64_t)(uintptr_t)vir_addr - (uint64_t)(uintptr_t)mem_mgr->pool_vir_addr);
	if (copy_to_user(argp, mem_req, sizeof(*mem_req)) != 0) {
		ukmd_pr_err("copy_to_user failed!");
		list_del(&client_info->list_node);
		kfree(client_info);
		cmdlist_mem_free(mem_mgr, vir_addr, mem_req->size);
		return -EFAULT;
	}

	ukmd_pr_info("available size=%llx got size=%#x va_offset=%llx",
		gen_pool_avail(mem_mgr->mem_pool), mem_req->size, mem_req->va_offset);

	return 0;
}

static struct user_mem_info *get_mem_user_info_node(struct list_head *mem_info_head,
	void *vir_addr, uint32_t size)
{
	struct user_mem_info *user_node = NULL;
	struct user_mem_info *_u_node_ = NULL;

	list_for_each_entry_safe(user_node, _u_node_, mem_info_head, list_node) {
		if (unlikely(!user_node))
			continue;

		if (user_node->vir_addr == vir_addr && user_node->size == size)
			return user_node;
	}
	return NULL;
}

static void cmdlist_mem_mgr_free(struct cmdlist_mem_mgr *mem_mgr, int32_t tgid,
	struct cmdlist_mem_request *mem_req)
{
	struct user_mem_info *client_info;
	void *vir_addr;
	struct mem_user_info_node *node;

	if (unlikely((mem_req->size > mem_mgr->pool_dma_size) ||
		(mem_req->va_offset >= mem_mgr->pool_dma_size - mem_req->size))) {
		ukmd_pr_err("freeing addr: size=%#x va_offset=%llx overflow pool_dma_size=%llx",
			mem_req->size, mem_req->va_offset, mem_mgr->pool_dma_size);
		return;
	}

	vir_addr = (void *)((uint64_t)(uintptr_t)mem_mgr->pool_vir_addr + mem_req->va_offset);
	node = (struct mem_user_info_node *)(void *)get_user_info_node(&mem_mgr->user_info_head, tgid);
	if (unlikely(!node)) {
		ukmd_pr_warn("cannot find tgid=%d!", tgid);
		return;
	}

	client_info = get_mem_user_info_node(&node->mem_list_head, vir_addr, mem_req->size);
	if (unlikely(!client_info)) {
		ukmd_pr_err("cannot find client, tgid=%d vir_addr=%pK, size=%u!", tgid, vir_addr, mem_req->size);
		return;
	}
	ukmd_pr_debug("del tgid=%d sub node!", tgid);
	list_del(&client_info->list_node);
	kfree(client_info);

	ukmd_pr_info("available size=%llx freeing size=%#x va_offset=%llx",
		gen_pool_avail(mem_mgr->mem_pool), mem_req->size, mem_req->va_offset);
	cmdlist_mem_free(mem_mgr, vir_addr, mem_req->size);
}

static void dump_mem_node_by_tgid(struct cmdlist_mem_mgr *mem_mgr) 
{
	struct mem_user_info_node *user_info_node_head;
	struct user_info_node *user_node = NULL;
	struct user_info_node *_u_node_ = NULL;
	struct user_mem_info *mem_info = NULL;
	struct user_mem_info *_m_info = NULL;
	uint32_t cmdlist_node_count;

	list_for_each_entry_safe(user_node, _u_node_, &mem_mgr->user_info_head, list_node) {
		if (unlikely(!user_node))
			continue;
		user_info_node_head = (struct mem_user_info_node*)user_node;
		ukmd_pr_err("user_tgid = %d, ref_cout = %u:", user_info_node_head->user_tgid, user_info_node_head->ref_cnt);
		cmdlist_node_count = 0;
		list_for_each_entry_safe(mem_info, _m_info, &user_info_node_head->mem_list_head, list_node) {
			if (unlikely(!mem_info))
				continue;
			cmdlist_node_count++;
		}
		ukmd_pr_err("this tgid = %d contains %u node", user_info_node_head->user_tgid, cmdlist_node_count);
	}
}

int32_t cmdlist_mem_mgr_ioctl(struct cmdlist_mem_mgr *mem_mgr, int32_t tgid, uint32_t cmd,
	unsigned long arg)
{
	int32_t ret = 0;
	struct cmdlist_mem_request mem_req = {0};
	void __user *argp = (void __user *)(uintptr_t)arg;

	if (copy_from_user(&mem_req, argp, sizeof(mem_req)) != 0) {
		ukmd_pr_err("copy for user failed!");
		return -EINVAL;
	}

	ukmd_pr_debug("cmd=%#x dev_id=%u tgid=%d size=%u", cmd, mem_mgr->cmd_priv->dev_id, tgid, mem_req.size);

	down(&mem_mgr->mem_sem);
	switch (cmd) {
	case CMDLIST_ALLOC_MEM:
		ret = cmdlist_mem_mgr_alloc(mem_mgr, tgid, &mem_req, argp);
		break;
	case CMDLIST_FREE_MEM:
		cmdlist_mem_mgr_free(mem_mgr, tgid, &mem_req);
		break;
	default:
		ret = -EINVAL;
		ukmd_pr_err("invalid cmd=%#x", cmd);
		break;
	}

	if (unlikely(ret != 0))
		dump_mem_node_by_tgid(mem_mgr);

	up(&mem_mgr->mem_sem);

	return ret;
}