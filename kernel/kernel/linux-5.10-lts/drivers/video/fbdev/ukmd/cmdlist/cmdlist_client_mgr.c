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
#include <linux/platform_device.h>
#include <linux/of_device.h>
#include "ukmd_log.h"
#include "cmdlist_common.h"
#include "cmdlist_drv.h"
#include "cmdlist_client_mgr.h"

/* 128 bit */
#define ONE_ITEM_SIZE (128 / 8)

/* One cmdlist node is inlcude cmd_header(size: 16Byte) and cmd_item[0..N](size: 16Byte)
 * N: tatal_items[13:0] = 0x3FFF = 16383
 */
#define ITEMS_MAX_NUM 0x3FFF

struct cmdlist_ioctl_data {
	uint32_t ioctl_cmd;
	int32_t (* ioctl_func)(struct cmdlist_table *cmd_table, int32_t tgid, struct cmdlist_node_client *);
};

static struct cmdlist_ioctl_data g_ioctl_data[] = {
	{ CMDLIST_CREATE_CLIENT, cmdlist_create_client },
	{ CMDLIST_SIGNAL_CLIENT, cmdlist_signal_client },
	{ CMDLIST_RELEASE_CLIENT, cmdlist_release_client_or_list },
	{ CMDLIST_APPEND_NEXT_CLIENT, cmdlist_append_next_client },
};

int32_t cmdlist_client_mgr_ioctl(struct cmdlist_client_mgr *client_mgr, int32_t tgid, uint32_t cmd,
	unsigned long arg)
{
	uint32_t  i;
	int32_t ret = 1;
	struct cmdlist_node_client user_client = {0};
	struct cmdlist_node_client __user *argp = (struct cmdlist_node_client __user *)(uintptr_t)arg;

	if (copy_from_user(&user_client, argp, sizeof(user_client)) != 0) {
		ukmd_pr_err("copy for user failed!");
		return -EINVAL;
	}

	if (user_client.scene_id > client_mgr->cmd_table.scene_num) {
		ukmd_pr_err("invalid scene_id:%u", user_client.scene_id);
		return -EINVAL;
	}

	user_client.dev_id = client_mgr->cmd_priv->dev_id;
	ukmd_pr_debug("cmd=%#x dev_id=%u id=%u type=%u scene_id=%u node_size=%u append_next_id=%u", cmd, user_client.dev_id,
		user_client.id, user_client.type, user_client.scene_id, user_client.node_size, user_client.append_next_id);

	down(&client_mgr->client_sem);
	for (i = 0; i < ARRAY_SIZE(g_ioctl_data); i++) {
		if (cmd == g_ioctl_data[i].ioctl_cmd) {
			ret = g_ioctl_data[i].ioctl_func(&client_mgr->cmd_table, tgid, &user_client);
			break;
		}
	}

	if (ret == 1) {
		ukmd_pr_err("unsupported cmd=%#x dev_id=%u id=%u type=%u scene_id=%u", cmd, user_client.dev_id,
			user_client.id, user_client.type, user_client.scene_id);
		up(&client_mgr->client_sem);
		return -1;
	}

	if (copy_to_user(argp, &user_client, sizeof(user_client)) != 0) {
		cmdlist_release_client_or_list(&client_mgr->cmd_table, tgid, &user_client);
		ukmd_pr_err("copy_to_user failed!");
		ret = -EFAULT;
	}
	up(&client_mgr->client_sem);

	return ret;
}

int32_t cmdlist_client_mgr_open(struct cmdlist_client_mgr *client_mgr, int32_t tgid)
{
	int32_t ret;

	down(&client_mgr->client_sem);
	ret = cmdlist_client_open(&client_mgr->cmd_table, client_mgr->cmd_priv->dev_id, tgid);
	up(&client_mgr->client_sem);

	return ret;
}

void cmdlist_client_mgr_release(struct cmdlist_client_mgr *client_mgr, int32_t tgid)
{
	down(&client_mgr->client_sem);
	cmdlist_client_release(&client_mgr->cmd_table, client_mgr->cmd_priv->dev_id, tgid);
	up(&client_mgr->client_sem);
}

static void cmdlist_client_mgr_parse_dt(struct cmdlist_client_mgr *client_mgr, const struct device_node *np)
{
	uint32_t val = 0;
	int32_t ret = of_property_read_u32(np, "scene_num", &client_mgr->cmd_table.scene_num);
	if (ret != 0) {
		ukmd_pr_err("get scene_num fail, ret=%d", ret);
		client_mgr->cmd_table.scene_num = 8; // default value
	}
	ukmd_pr_info("get scene_num: %d", client_mgr->cmd_table.scene_num);

	ret = of_property_read_u32(np, "cmdlist_max_size", &val);
	if (ret == 0) {
		client_mgr->sum_node_size = val;
	} else {
		ukmd_pr_err("get cmdlist_max_size fail, ret=%d", ret);
		client_mgr->sum_node_size = 0x100000; // default value
	}
	ukmd_pr_info("get cmdlist_max_size: %#x", client_mgr->sum_node_size);

	ret = of_property_read_u32(np, "use_kernel_mode", &val);
	if (ret == 0) {
		client_mgr->use_kernel_mode = (uint8_t)val;
	} else {
		ukmd_pr_err("get use_kernel_mode fail, ret=%d", ret);
		client_mgr->use_kernel_mode = 1; // default value
	}
	ukmd_pr_info("get use_kernel_mode: %u", client_mgr->use_kernel_mode);

	ret = of_property_read_u32(np, "addr_mode", &val);
	if (ret == 0) {
		client_mgr->addr_mode = (uint8_t)val;
	} else {
		ukmd_pr_err("get addr_mode fail, ret=%d", ret);
		client_mgr->addr_mode = ADDR_MODE_PA; // default value
	}
	ukmd_pr_info("get addr_mode: %u", client_mgr->addr_mode);
}

int32_t cmdlist_client_mgr_probe(struct cmdlist_client_mgr *client_mgr, const struct device_node *np)
{
	cmdlist_client_mgr_parse_dt(client_mgr, np);

	/* One cmdlist node is inlcude cmd_header(size: 16Byte) and cmd_item[0..N](size: 16Byte)
	 * N: tatal_items[13:0] = 0x3FFF = 16383
	 */
	client_mgr->sum_node_size =
		min(roundup(ITEMS_MAX_NUM * ONE_ITEM_SIZE, PAGE_SIZE) * client_mgr->cmd_table.scene_num,
		client_mgr->sum_node_size);

	cmdlist_client_init_cmdlist_id(&client_mgr->cmd_table);

	if (cmdlist_client_table_setup(&client_mgr->cmd_table) != 0) {
		ukmd_pr_err("cmd table setup failed!");
		return -1;
	}

	sema_init(&client_mgr->client_sem, 1);

	return 0;
}

void cmdlist_client_mgr_remove(struct cmdlist_client_mgr *client_mgr)
{
	down(&client_mgr->client_sem);
	cmdlist_client_table_remove(&client_mgr->cmd_table, client_mgr->cmd_priv->dev_id);
	up(&client_mgr->client_sem);
}