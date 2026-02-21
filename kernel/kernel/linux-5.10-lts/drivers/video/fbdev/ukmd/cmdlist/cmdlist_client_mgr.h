/**
 * @file cmdlist_client_mgr.h
 * @brief Cmdlist node managment
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

#ifndef __CMDLIST_NODE_MGR_H__
#define __CMDLIST_NODE_MGR_H__

#include <linux/types.h>
#include <linux/semaphore.h>
#include "cmdlist_client.h"

struct cmdlist_private;
struct cmdlist_node_client;

struct cmdlist_client_mgr {
	struct cmdlist_private *cmd_priv;
	struct semaphore client_sem;
	size_t sum_node_size;
	struct cmdlist_table cmd_table;
	uint8_t use_kernel_mode;
	uint8_t addr_mode;
	uint8_t rsv[2];
	uint32_t ref_cnt;
};

int32_t cmdlist_client_mgr_probe(struct cmdlist_client_mgr *client_mgr, const struct device_node *np);
void cmdlist_client_mgr_remove(struct cmdlist_client_mgr *client_mgr);

int32_t cmdlist_client_mgr_ioctl(struct cmdlist_client_mgr *client_mgr, int32_t tgid, uint32_t cmd,
	unsigned long arg);

int32_t cmdlist_client_mgr_open(struct cmdlist_client_mgr *client_mgr, int32_t tgid);
void cmdlist_client_mgr_release(struct cmdlist_client_mgr *client_mgr, int32_t tgid);

#endif
