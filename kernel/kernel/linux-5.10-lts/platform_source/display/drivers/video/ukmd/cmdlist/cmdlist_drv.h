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
#include "cmdlist_common.h"
#include "cmdlist_mem_mgr.h"
#include "cmdlist_client_mgr.h"
#include "cmdlist_interface.h"

#define CMDLIST_TGID_MAX_NUM 8

enum cmdlist_arch_version {
	GENERIC_DEVICETREE_CMDLIST,
	CMDLIST_VERSION_MAX,
};

struct cmdlist_match_data {
	enum cmdlist_arch_version version;
	uint32_t dev_id;
	int32_t (*of_device_probe)(struct platform_device *pdev);
	void (*of_device_remove)(struct platform_device *pdev);
};

struct cmdlist_private {
	struct platform_device *pdev;
	struct device *of_dev;
	struct ukmd_chrdev chrdev;
	uint32_t dev_id;
	atomic_t ref_count;
	int32_t device_initialized;

	struct cmdlist_client_mgr client_mgr;
	struct cmdlist_mem_mgr mem_mgr;
};

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
#endif
