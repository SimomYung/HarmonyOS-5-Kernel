/**
 * @file cmdlist_drv.h
 * @brief Interface for cmdlist driver function
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
#ifndef __CMDLIST_DRV_H__
#define __CMDLIST_DRV_H__

#include <linux/fs.h>
#include <linux/types.h>
#include <linux/wait.h>
#include <linux/platform_device.h>
#include <linux/semaphore.h>
#include "chrdev/ukmd_chrdev.h"
#include "cmdlist_client.h"

#define DPU_CMDLIST "dpu_cmdlist"
#define AAE_CMDLIST "aae_cmdlist"

/* 128 bit */
#define ONE_ITEM_SIZE (128 / 8)

/* One cmdlist node is inlcude cmd_header(size: 16Byte) and cmd_item[0..N](size: 16Byte)
 * N: tatal_items[13:0] = 0x3FFF = 16383
 */
#define ITEMS_MAX_NUM 0x3FFF

#define CMDLIST_TGID_MAX_NUM 8

enum cmdlist_arch_version {
	GENERIC_DEVICETREE_CMDLIST,
	CMDLIST_VERSION_MAX,
};

struct cmdlist_match_data {
	enum cmdlist_arch_version version;
	int32_t (*of_device_setup)(struct platform_device *pdev);
	void (*of_device_release)(struct platform_device *pdev);
};

struct user_info_node {
	struct list_head list_node;
	int32_t user_tgid;
	uint32_t ref_cnt;
};

struct cmdlist_private {
	struct platform_device *pdev;
	struct device *of_dev;

	struct ukmd_chrdev chrdev;

	struct cmdlist_table cmd_table;
	struct semaphore table_sem;

	size_t sum_pool_size;
	void *pool_vir_addr;
	dma_addr_t pool_phy_addr;
	struct gen_pool *memory_pool;

	int32_t ref_count;
	int32_t device_initialized;
	uint32_t dev_id;
	struct list_head user_info_head; // record every user tgid when open device
};

#define CMDLIST_DEV_MAX_NUM 4
extern struct cmdlist_private g_cmdlist_privs[CMDLIST_DEV_MAX_NUM];

static inline struct cmdlist_private *cmdlist_get_priv_by_inode(struct inode *inode)
{
	struct ukmd_chrdev *chrdev = NULL;

	if (unlikely(!inode))
		return NULL;

	chrdev = container_of(inode->i_cdev, struct ukmd_chrdev, cdev);
	if (!chrdev)
		return NULL;

	return container_of(chrdev, struct cmdlist_private, chrdev);
}

static inline struct cmdlist_private *cmdlist_get_priv_by_id(uint32_t dev_id)
{
	if (unlikely(dev_id >= CMDLIST_DEV_MAX_NUM))
		return NULL;

	return &g_cmdlist_privs[dev_id];
}

// No longer check cmd_priv or lock, user need check and lock it
static inline struct user_info_node *cmdlist_get_user_info_node(struct cmdlist_private *cmd_priv, int32_t user_tgid)
{
	struct user_info_node *user_node = NULL;
	struct user_info_node *_u_node_ = NULL;

	list_for_each_entry_safe(user_node, _u_node_, &cmd_priv->user_info_head, list_node) {
		if (unlikely(!user_node))
			continue;

		if (user_node->user_tgid == user_tgid)
			return user_node;
	}
	return NULL;
}

uint32_t cmdlist_get_dev_id_by_name(const char *dev_name);
#endif
