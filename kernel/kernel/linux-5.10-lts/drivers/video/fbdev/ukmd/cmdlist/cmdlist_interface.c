/**
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
#include "uksm_debug.h"

#include "cmdlist_interface.h"
#include "chash.h"
#include "cmdlist_node.h"
#include "cmdlist_client.h"
#include "cmdlist_client_mgr.h"
#include "cmdlist_drv.h"
#include "ukmd_acquire_fence.h"

#define BUF_SYNC_TIMEOUT_MSEC (1 * MSEC_PER_SEC)
#define ITEM_CHECK_SLIDE_NUM 4
#define ITEM_CHECK_TOTAL_NUM 64

static struct cnode *get_valid_cmdlist_header(struct cmdlist_table *cmd_table,
	uint32_t scene_id, uint32_t cmdlist_id)
{
	struct cmdlist_client *client = NULL;

	client = cmdlist_get_current_client(cmd_table, scene_id, cmdlist_id);
	if (unlikely(!client)) {
		ukmd_pr_warn("invalid scene_id:%u, cmdlist_id:%u!", scene_id, cmdlist_id);
		return NULL;
	}

	if (unlikely(client->node_type != SCENE_NOP_TYPE)) {
		ukmd_pr_warn("invalid scene header client type=%d!", client->node_type);
		return NULL;
	}

	if (unlikely(!client->node)) {
		ukmd_pr_warn("header client node is null!");
		return NULL;
	}

	return &client->node->cnode;
}

/* To check cmdlist items constraint: the sum of 4(ITEM_CHECK_SLIDE_NUM) consecutive nodes' item should no
 * less than 64(ITEM_CHECK_TOTAL_NUM)
 */
static int32_t cmdlist_check_cmd_items_valid(struct cnode *list_header)
{
	uint32_t node_count = 0;
	uint32_t item_count = 0;
	struct cmdlist_node *node = NULL, *_node_ = NULL;
	struct cmdlist_node *slide_right = NULL;
	struct cmdlist_node *slide_left = clist_first_entry(list_header, typeof(*slide_left), cnode);

	if (unlikely(!slide_left))
		return 0;

	clist_for_each_entry_safe(node, _node_, list_header, typeof(*node), cnode) {
		++node_count;
		if (!node || !node->list_cmd_header) {
			ukmd_pr_err("node%u: node or list_cmd_header is NULL", node_count);
			return -1;
		}

		item_count += node->list_cmd_header->total_items.bits.items_count;
		if (node_count == ITEM_CHECK_SLIDE_NUM)
			break;
	}

	// total node number is less than ITEM_CHECK_SLIDE_NUM, match the constraint
	if (node_count < ITEM_CHECK_SLIDE_NUM)
		return 0;

	if (item_count < ITEM_CHECK_TOTAL_NUM) {
		ukmd_pr_err("First %d nodes' total item number is less than %d", ITEM_CHECK_SLIDE_NUM, ITEM_CHECK_TOTAL_NUM);
		return -1;
	}

	// node now is at slide right
	for (slide_right = cnode_entry(&node->cnode, typeof(*node), cnode);
		!clist_is_tail(list_header, &slide_right->cnode);
		slide_right = clist_next_entry(slide_right, list_header, typeof(*node), cnode)) {
		++node_count;
		slide_left = clist_next_entry(slide_left, list_header, typeof(*node), cnode);
		item_count -= slide_left->list_cmd_header->total_items.bits.items_count;
		item_count += slide_right->list_cmd_header->total_items.bits.items_count;
		if (item_count < ITEM_CHECK_TOTAL_NUM) {
			ukmd_pr_err("%d nodes' total item number beginning at node %u is less than %d",
				ITEM_CHECK_SLIDE_NUM, node_count, ITEM_CHECK_TOTAL_NUM);
			return -1;
		}
	}

	return 0;
}

static int32_t ukmd_cmdlist_exec_hardware_link(struct cnode *list_header)
{
	struct cmdlist_node *node = NULL, *_node_ = NULL;
	struct cmdlist_node *prev_node = NULL;
	struct cmdlist_client *client = NULL;

	if (cmdlist_check_cmd_items_valid(list_header) != 0)
		return -1;

	/**
	* Block scenarios exist multiple DM nodes, each DM node corresponding to
	* the task of a complete so each DM node in a node need to configure
	* the task end flag.
	*
	*  scene_header
	*       ├── DM_node0
	*       │     └── node ── ... ── node(task end)
	*       │
	*       └── DM_node1 --- cldx --- sclx (reuse operator isn't need to link)
	*              │          └── node --- node
	*              │
	*              └── node ── ... ── node(task end)
	*
	* Here are some constraints:
	* 1. A task cannot have short continuous multiple nodes.
	*    When a task include multiple configuration node, arbitrary continuous
	*    four nodes cmd_item is greater than the sum of 64, or it requires nodes
	*    cmd_item is not greater than 4.
	* 2. Command node contains the number of cmd_items (namely the total_items
	*    cmd_header) cannot be equal to 1, if there are any cmd_item only one,
	*    you can copy the data, and make total_items equal to 2.
	* 3. "Configuration mode" and "transport node", only switch once, can't switch
	*    over and over again, Generally, transport configuration node is always in
	*    the front.
	* 4. Using a one-dimensional transport mode, must ensure that the payload size
	*    is 16 byte(128bit) alignment.
	*/
	clist_for_each_entry_safe(node, _node_, list_header, typeof(*node), cnode) {
		/**
		 * @brief this fence fd maybe not effect fd, but can make said fence
		 * also has not been released, so need wait here.
		 */
		if (node) {
			client = (struct cmdlist_client *)node->client;
			if (node->fence)
				ukmd_acquire_fence_wait(node->fence, BUF_SYNC_TIMEOUT_MSEC);
		}

		if (prev_node && prev_node->list_cmd_header && node) {
			if (client && ((client->node_type & DM_TRANSPORT_TYPE) != 0)) {
				prev_node->list_cmd_header->cmd_flag.bits.task_end = 1;
				prev_node->list_cmd_header->cmd_flag.bits.last = 1;
			} else {
				prev_node->list_cmd_header->cmd_flag.bits.task_end = 0;
				prev_node->list_cmd_header->cmd_flag.bits.last = 0;
				prev_node->list_cmd_header->next_list = node->cmd_header_addr;
			}
		}

		if (client && ((client->node_type & OPR_REUSE_TYPE) == 0))
			prev_node = node;
	}
#ifdef CONFIG_UKMD_DEBUG_ENABLE
	/* dump all linked node, include dm, cfg, reused node */
	if (g_cmdlist_dump_enable) {
		clist_for_each_entry_safe(node, _node_, list_header, typeof(*node), cnode) {
			if (node && (node->scene_id != 7))
				cmdlist_dump_node(node);
		}
	}
#endif
	return 0;
}

/**
 * @brief According to scene id and cmdlist id found it a frame of configuration
 *       information, and then set up the hardware connection, get the frame header
 *       configuration address at the same time
 *
 * @param dev_id cmdlist device id, indicate dpu_cmdlist or aae_cmdlist
 * @param scene_id hardware scene id: 0~3 online composer scene, 4~6 offline composer scene
 * @param cmdlist_id cmdlist_id should be the scene header
 * @return dma_addr_t the frame header configuration address (DM data node header addr)
 */
dma_addr_t ukmd_cmdlist_get_dma_addr(uint32_t dev_id, uint32_t scene_id, uint32_t cmdlist_id)
{
	dma_addr_t cmdlist_flush_addr = 0;
	struct cnode *list_header = NULL;
	struct cmdlist_node *header_node = NULL;
	struct cmdlist_private *cmd_priv = cmdlist_get_priv_by_id(dev_id);

	if (!cmd_priv) {
		ukmd_pr_err("cannot get cmdlist device from dev_id=%u", dev_id);
		return cmdlist_flush_addr;
	}

	down(&cmd_priv->client_mgr.client_sem);
	list_header = get_valid_cmdlist_header(&cmd_priv->client_mgr.cmd_table, scene_id, cmdlist_id);
	if (unlikely(!list_header))
		goto out_return;

	if (ukmd_cmdlist_exec_hardware_link(list_header) != 0) {
		ukmd_pr_err("scene_id=%u cmdlist_id=%u, node list has NULL pointer", scene_id, cmdlist_id);
		goto out_return;
	}

	header_node = clist_first_entry(list_header, typeof(*header_node), cnode);
	if (header_node)
		cmdlist_flush_addr = header_node->cmd_header_addr;

out_return:
	up(&cmd_priv->client_mgr.client_sem);
	return cmdlist_flush_addr;
}

bool ukmd_cmdlist_is_kernel_mode(uint32_t dev_id)
{
	struct cmdlist_private *cmd_priv = cmdlist_get_priv_by_id(dev_id);

	if (!cmd_priv) {
		ukmd_pr_err("cannot get cmdlist device from dev_id=%u", dev_id);
		return true;
	}

	return cmd_priv->client_mgr.use_kernel_mode == 1;
}

uint64_t ukmd_cmdlist_user_get_dma_addr(uint32_t dev_id, uint32_t scene_id, uint64_t cmdlist_id)
{
	struct cmdlist_private *cmd_priv = cmdlist_get_priv_by_id(dev_id);
	uint64_t va_offset;

	if (!cmd_priv) {
		ukmd_pr_err("cannot get cmdlist device from dev_id=%u", dev_id);
		return 0;
	}

	if ((cmdlist_id & CMDLIST_ID_USER_BIT) != 0) {
		if (cmd_priv->client_mgr.use_kernel_mode == 0) {
			ukmd_pr_debug("cmdlist user mode enable, dev_id=%u cmdlist_id=%llx", dev_id, cmdlist_id);
			va_offset = CMDLIST_VA_OFFSET(cmdlist_id);
			if (va_offset >= (uint64_t)cmd_priv->mem_mgr.pool_dma_size) {
				ukmd_pr_err("address offset=%llx overflow total size=%llx, dev_id=%u",
					va_offset, dev_id, cmd_priv->mem_mgr.pool_dma_size);
				return 0;
			}

			return va_offset + cmd_priv->mem_mgr.pool_dma_addr;
		}
		ukmd_pr_err("cmdlist_id=%llx invalid, dev_id=%u", cmdlist_id, dev_id);
	}

	ukmd_pr_debug("cmdlist kernel mode enable, dev_id=%u", dev_id);
	return ukmd_cmdlist_get_dma_addr(dev_id, scene_id, (uint32_t)cmdlist_id);
}

bool ukmd_cmdlist_is_va_enable(uint32_t dev_id)
{
	struct cmdlist_private *cmd_priv = cmdlist_get_priv_by_id(dev_id);

	if (!cmd_priv) {
		ukmd_pr_err("cannot get cmdlist device from dev_id=%u", dev_id);
		return false;
	}

	// ION only support iova
	return cmd_priv->mem_mgr.memory_type == MEMORY_TYPE_ION;
}

static void cmdlist_release_locked(uint32_t dev_id, uint32_t scene_id, uint32_t cmdlist_id,
	bool is_hal)
{
	struct cmdlist_node_client user_client = {
		.dev_id = dev_id,
		.id = cmdlist_id,
		.type = SCENE_NOP_TYPE,
		.scene_id = scene_id,
	};
	struct cmdlist_private *cmd_priv = cmdlist_get_priv_by_id(dev_id);

	if (!cmd_priv) {
		ukmd_pr_err("cannot get cmdlist device from dev_id=%u", dev_id);
		return;
	}

	down(&cmd_priv->client_mgr.client_sem);

	// kernel thread no need to use cmdlist_client_mgr to release, only manage user space create
	if (!is_hal)
		(void)k_cmdlist_release_client_or_list(&cmd_priv->client_mgr.cmd_table, &user_client);
	else
		(void)cmdlist_release_client_or_list(&cmd_priv->client_mgr.cmd_table, 0, &user_client);

	up(&cmd_priv->client_mgr.client_sem);
}

void ukmd_cmdlist_release_locked(uint32_t dev_id, uint32_t scene_id, uint32_t cmdlist_id)
{
	cmdlist_release_locked(dev_id, scene_id, cmdlist_id, false);
}

void ukmd_cmdlist_present_release_locked(uint32_t dev_id, uint32_t scene_id, uint64_t cmdlist_id)
{
	if (!ukmd_cmdlist_is_kernel_mode(dev_id)) {
		return;
	}

	// when kernel mode, cmdlist_id only use 32 bit
	cmdlist_release_locked(dev_id, scene_id, (uint32_t)cmdlist_id, true);
}

uint32_t cmdlist_create_user_client(uint32_t dev_id,
	uint32_t scene_id, uint32_t node_type, uint32_t dst_dma_addr, uint32_t size)
{
	struct cmdlist_node_client user_client = {
		.dev_id = dev_id,
		.id = 0,
		.scene_id = scene_id,
		.node_size = size,
		.type = node_type,
		.dst_addr = dst_dma_addr,
	};

	struct cmdlist_private *cmd_priv = cmdlist_get_priv_by_id(dev_id);

	if (!cmd_priv) {
		ukmd_pr_err("cannot get cmdlist device from dev_id=%u", dev_id);
		return user_client.id;
	}

	if (!(node_type & SCENE_NOP_TYPE))
		user_client.node_size += 16; /* add cmdlist header size */

	down(&cmd_priv->client_mgr.client_sem);

	// kernel thread no need to use cmdlist_client_mgr to create, only manage user space create
	cmdlist_create_client(&cmd_priv->client_mgr.cmd_table, -1, &user_client);

	up(&cmd_priv->client_mgr.client_sem);

	return user_client.id;
}

bool cmdlist_is_ready(uint32_t dev_id)
{
	struct cmdlist_private *cmd_priv = cmdlist_get_priv_by_id(dev_id);
	return (cmd_priv != NULL) && (cmd_priv->device_initialized != 0);
}
void *cmdlist_get_payload_addr(uint32_t dev_id, uint32_t scene_id, uint32_t cmdlist_id)
{
	void *payload_addr = NULL;
	struct cmdlist_client *client = NULL;
	struct cmdlist_private *cmd_priv = cmdlist_get_priv_by_id(dev_id);

	if (!cmd_priv) {
		ukmd_pr_err("cannot get cmdlist device from dev_id=%u", dev_id);
		return payload_addr;
	}

	down(&cmd_priv->client_mgr.client_sem);

	client = cmdlist_get_current_client(&cmd_priv->client_mgr.cmd_table, scene_id, cmdlist_id);
	if (unlikely(!client || !client->node)) {
		ukmd_pr_err("invalid cmdlist client!");
		up(&cmd_priv->client_mgr.client_sem);
		return NULL;
	}
	payload_addr = (void *)client->node->list_cmd_item;

	up(&cmd_priv->client_mgr.client_sem);

	return payload_addr;
}

uint32_t cmdlist_get_phy_addr(uint32_t dev_id, uint32_t scene_id, uint32_t cmdlist_id)
{
	uint32_t header_phy_addr = 0;
	struct cmdlist_client *client = NULL;
	struct cmdlist_private *cmd_priv = cmdlist_get_priv_by_id(dev_id);

	if (!cmd_priv) {
		ukmd_pr_err("cannot get cmdlist device from dev_id=%u", dev_id);
		return header_phy_addr;
	}

	down(&cmd_priv->client_mgr.client_sem);

	client = cmdlist_get_current_client(&cmd_priv->client_mgr.cmd_table, scene_id, cmdlist_id);
	if (unlikely(!client || !client->node)) {
		ukmd_pr_err("invalid cmdlist client!");
		up(&cmd_priv->client_mgr.client_sem);
		return header_phy_addr;
	}
	header_phy_addr = client->node->cmd_header_addr;
	ukmd_pr_debug("header_phy_addr=0x%llx", header_phy_addr);

	up(&cmd_priv->client_mgr.client_sem);

	return header_phy_addr;
}

int32_t cmdlist_append_client(uint32_t dev_id,
	uint32_t scene_id, uint32_t scene_header_id, uint32_t cmdlist_id)
{
	int ret;
	struct cmdlist_node_client user_client = {
		.dev_id = dev_id,
		.id = scene_header_id,
		.scene_id = scene_id,
		.append_next_id = cmdlist_id,
	};
	struct cmdlist_private *cmd_priv = cmdlist_get_priv_by_id(dev_id);

	if (!cmd_priv) {
		ukmd_pr_err("cannot get cmdlist device from dev_id=%u", dev_id);
		return -1;
	}

	down(&cmd_priv->client_mgr.client_sem);

	ret = cmdlist_append_next_client(&cmd_priv->client_mgr.cmd_table, -1, &user_client);

	up(&cmd_priv->client_mgr.client_sem);

	return ret;
}

int32_t cmdlist_flush_client(uint32_t dev_id, uint32_t scene_id, uint32_t cmdlist_id)
{
	struct cmdlist_node_client user_client = {
		.dev_id = dev_id,
		.id = cmdlist_id,
		.scene_id = scene_id,
		.valid_payload_size = 0,
	};
	struct cmdlist_private *cmd_priv = cmdlist_get_priv_by_id(dev_id);

	if (!cmd_priv) {
		ukmd_pr_err("cannot get cmdlist device from dev_id=%u", dev_id);
		return -1;
	}

	down(&cmd_priv->client_mgr.client_sem);

	cmdlist_signal_client(&cmd_priv->client_mgr.cmd_table, -1, &user_client);

	up(&cmd_priv->client_mgr.client_sem);

	return 0;
}

int32_t ukmd_cmdlist_clear_client(uint32_t dev_id, uint32_t scene_id, uint32_t cmdlist_id)
{
	struct cmdlist_node_client user_client = {
		.dev_id = dev_id,
		.id = cmdlist_id,
		.scene_id = scene_id,
		.valid_payload_size = 0,
	};
	struct cmdlist_private *cmd_priv = cmdlist_get_priv_by_id(dev_id);

	if (!cmd_priv) {
		ukmd_pr_err("cannot get cmdlist device from dev_id=%u", dev_id);
		return -1;
	}

	down(&cmd_priv->client_mgr.client_sem);

	cmdlist_clear_client(&cmd_priv->client_mgr.cmd_table, &user_client);

	up(&cmd_priv->client_mgr.client_sem);

	return 0;
}

void ukmd_set_reg(uint32_t dev_id,
	uint32_t scene_id, uint32_t cmdlist_id, uint32_t addr, uint32_t value)
{
	struct cmdlist_client *client = NULL;
	struct cmdlist_private *cmd_priv = cmdlist_get_priv_by_id(dev_id);

	if (!cmd_priv) {
		ukmd_pr_err("cannot get cmdlist device from dev_id=%u", dev_id);
		return;
	}

	down(&cmd_priv->client_mgr.client_sem);

	client = cmdlist_get_current_client(&cmd_priv->client_mgr.cmd_table, scene_id, cmdlist_id);
	if (unlikely(!client || !client->node)) {
		ukmd_pr_err("invalid cmdlist client!");
		up(&cmd_priv->client_mgr.client_sem);
		return;
	}
	up(&cmd_priv->client_mgr.client_sem);

	cmdlist_set_reg(client->node, addr, value);
}

#ifdef CONFIG_UKMD_DEBUG_ENABLE
int32_t ukmd_cmdlist_dump_scene(uint32_t dev_id, uint32_t scene_id)
{
	struct cmdlist_node_client user_client = {
		.dev_id = dev_id,
		.scene_id = scene_id,
	};
	struct cmdlist_private *cmd_priv = cmdlist_get_priv_by_id(dev_id);

	if (!cmd_priv) {
		ukmd_pr_err("cannot get cmdlist device from dev_id=%u", dev_id);
		return -1;
	}

	down(&cmd_priv->client_mgr.client_sem);

	cmdlist_dump_scene(&cmd_priv->client_mgr.cmd_table, -1, &user_client);

	up(&cmd_priv->client_mgr.client_sem);

	return 0;
}

int32_t ukmd_cmdlist_dump_by_id(uint32_t dev_id, uint32_t scene_id, uint32_t cmdlist_id)
{
	struct cmdlist_node_client user_client = {
		.dev_id = dev_id,
		.scene_id = scene_id,
		.id = cmdlist_id,
	};
	struct cmdlist_private *cmd_priv = cmdlist_get_priv_by_id(dev_id);

	if (!cmd_priv) {
		ukmd_pr_err("cannot get cmdlist device from dev_id=%u", dev_id);
		return -1;
	}

	down(&cmd_priv->client_mgr.client_sem);

	cmdlist_dump_client(&cmd_priv->client_mgr.cmd_table, -1, &user_client);

	up(&cmd_priv->client_mgr.client_sem);

	return 0;
}

void ukmd_cmdlist_dump_dm(uint32_t dev_id, uint32_t scene_id, uint32_t cmdlist_id)
{
	struct cnode *list_header = NULL;
	struct cmdlist_node *node = NULL, *_node_ = NULL;
	struct cmdlist_private *cmd_priv = cmdlist_get_priv_by_id(dev_id);

	if (!cmd_priv) {
		ukmd_pr_err("cannot get cmdlist device from dev_id=%u", dev_id);
		return;
	}

	if (!g_cmdlist_dump_enable)
		return;

	list_header = get_valid_cmdlist_header(&cmd_priv->client_mgr.cmd_table, scene_id, cmdlist_id);
	if (unlikely(!list_header)) {
		ukmd_pr_err("list_header is NULL!");
		return;
	}

	clist_for_each_entry_safe(node, _node_, list_header, typeof(*node), cnode) {
		if (node && (node->scene_id != 7)) {
			cmdlist_dump_node(node);
			return;
		}
	}
}
#endif
