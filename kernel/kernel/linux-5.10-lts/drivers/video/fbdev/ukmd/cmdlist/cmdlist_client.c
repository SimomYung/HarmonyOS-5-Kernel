/**
 * @file disp_cmdlist.c
 * @brief The ioctl the interface file for cmdlist node.
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
#include <linux/module.h>
#include <linux/syscalls.h>
#include "chash.h"
#include "cmdlist_client.h"
#include "cmdlist_node.h"
#include "ukmd_acquire_fence.h"
#include "ukmd_log.h"

struct user_client_info {
	struct list_head list_node;
	uint32_t id;
	uint32_t type;
	uint32_t scene_id;
};

void cmdlist_client_init_cmdlist_id(struct cmdlist_table *cmd_table)
{
	atomic_set(&cmd_table->cmdlist_next_id, 0);
}

static bool cmdlist_client_compare(const struct cnode *a, const struct cnode *b)
{
	struct cmdlist_client *na = cnode_entry(a, struct cmdlist_client, hash_node);
	struct cmdlist_client *nb = cnode_entry(b, struct cmdlist_client, hash_node);

	return na->key == nb->key;
}

static int32_t cmdlist_record_process_info(struct cmdlist_table *cmd_table, int32_t tgid,
	const struct cmdlist_node_client *user_client)
{
	struct user_client_info *client_info = NULL;
	struct client_user_info_node *node =
		(struct client_user_info_node *)(void *)get_user_info_node(&cmd_table->user_info_head, tgid);
	if (unlikely(!node)) {
		ukmd_pr_err("cannot find tgid=%d!", tgid);
		return -1;
	}

	client_info = (struct user_client_info *)kzalloc(sizeof(struct user_client_info), GFP_KERNEL);
	if (unlikely(!client_info)) {
		ukmd_pr_err("kzalloc return err!");
		return -ENOMEM;
	}

	client_info->id = user_client->id;
	client_info->type = user_client->type;
	client_info->scene_id = user_client->scene_id;
	ukmd_pr_debug("add to tgid=%d head!", tgid);
	list_add_tail(&client_info->list_node, &node->client_list_head);
	return 0;
}

static void cmdlist_client_release_node(struct cnode *node, void *data)
{
	struct cmdlist_client *client = NULL;

	if (unlikely(!node || !data)) {
		ukmd_pr_err("node or data is nullptr!");
		return;
	}

	client = cnode_entry(node, struct cmdlist_client, hash_node);
	if (unlikely(!client)) {
		ukmd_pr_err("client is nullptr!");
		return;
	}

	ukmd_pr_debug("--> release client <%u, %pK, %u>", client->key, client->node, client->node_type);
	ukmd_pr_debug("release node[%pK].scene[%d].mode[%d].size[%#x].prev=%pK.next=%pK.list_cmd_header=%pK.fence=%pK",
		client->node, client->node->scene_id, client->node->cmd_mode, client->node->node_size,
		client->node->cnode.prev, client->node->cnode.next, client->node->list_cmd_header, client->node->fence);

	cmdlist_release_node(*((uint32_t *)data), client->node);
	chash_remove(&client->hash_node);
	kfree(client);
	client = NULL;
}

int32_t cmdlist_client_open(struct cmdlist_table *cmd_table, uint32_t dev_id, int32_t tgid)
{
	struct client_user_info_node *node;

	node = (struct client_user_info_node *)(void *)get_user_info_node(&cmd_table->user_info_head, tgid);
	if (!node) {
		node = kzalloc(sizeof(struct client_user_info_node), GFP_KERNEL);
		if (likely(node)) {
			node->user_tgid = tgid;
			node->ref_cnt = 1;
			INIT_LIST_HEAD(&node->client_list_head);
			list_add_tail(&node->list_node, &cmd_table->user_info_head);
		} else {
			ukmd_pr_err("kzalloc return err!");
			return -ENOMEM;
		}
	} else {
		ukmd_pr_warn("device has opened before by tgid=%d of %u times", tgid, node->ref_cnt);
		++node->ref_cnt;
	}
	
	return 0;
}

static void free_client_user_info_node(struct cmdlist_table *cmd_table,
	uint32_t dev_id, struct list_head *client_info_head, bool free_client)
{
	struct user_client_info *user_node = NULL;
	struct user_client_info *_u_node_ = NULL;
	struct cmdlist_client *client = NULL;

	list_for_each_entry_safe(user_node, _u_node_, client_info_head, list_node) {
		if (unlikely(!user_node))
			continue;

		if (free_client) {
			// free cmdlist client
			client = cmdlist_get_current_client(cmd_table, user_node->scene_id, user_node->id);
			if (client && client->node)
				cmdlist_client_release_node(&client->hash_node, (void *)&dev_id);
		}
		list_del(&user_node->list_node);
		kfree(user_node);
		user_node = NULL;
	}
}

void cmdlist_client_release(struct cmdlist_table *cmd_table, uint32_t dev_id, int32_t tgid)
{
	struct client_user_info_node *node;

	node = (struct client_user_info_node *)(void *)get_user_info_node(&cmd_table->user_info_head, tgid);
	if (unlikely(!node)) {
		ukmd_pr_warn("cannot find tgid=%d", tgid);
	} else {
		ukmd_pr_warn("device has opened before by tgid=%d of %u times", tgid, node->ref_cnt);
		if (node->ref_cnt > 0)
			--node->ref_cnt;

		if (node->ref_cnt == 0) {
			free_client_user_info_node(cmd_table, dev_id, &node->client_list_head, true);
			list_del(&node->list_node);
			kfree(node);
		}
	}
}

/**
 * @brief init cmdlist device and cmdlist table
 *
 * @param cmd_table cmdlist client table
 * @return int32_t 0: success -1: fail
 */
int32_t cmdlist_client_table_setup(struct cmdlist_table *cmd_table)
{
	int ret = 0;

	if (unlikely(!cmd_table)) {
		ukmd_pr_err("input error, cmd_table is null pointer!");
		return -1;
	}

	if (chash_is_valid(&cmd_table->ht)) {
		ukmd_pr_warn("bkts is already initialized!");
		return ret;
	}

	ret = chash_init(&cmd_table->ht, cmd_table->scene_num, cmdlist_client_compare);
	if (unlikely(ret != 0))
		ukmd_pr_err("init hash table failed!");

	INIT_LIST_HEAD(&cmd_table->user_info_head);

	return ret;
}

static void cmdlist_client_remove_list(struct cmdlist_table *cmd_table, uint32_t dev_id)
{
	struct user_info_node *user_node = NULL;
	struct user_info_node *_u_node_ = NULL;
	struct client_user_info_node *node;

	list_for_each_entry_safe(user_node, _u_node_, &cmd_table->user_info_head, list_node) {
		if (unlikely(!user_node))
			continue;

		node = (struct client_user_info_node *)user_node;
		free_client_user_info_node(cmd_table, dev_id, &node->client_list_head, false);

		list_del(&user_node->list_node);
		kfree(user_node);
		user_node = NULL;
	}
}

/**
 * @brief remove cmdlist table and list
 *
 * @param cmd_table cmdlist client table
 */
void cmdlist_client_table_remove(struct cmdlist_table *cmd_table, uint32_t dev_id)
{
	if (unlikely(!cmd_table)) {
		ukmd_pr_err("input error, cmd_table is null pointer!\n");
		return;
	}

	cmdlist_client_remove_list(cmd_table, dev_id);
	chash_deinit(&cmd_table->ht, cmdlist_client_release_node, (void *)&dev_id);
}

// ----------------------------------------------------------------------------
static uint32_t get_cmdlist_mode(uint32_t type)
{
	if ((type & REGISTER_CONFIG_TYPE) != 0)
		return CONFIG_MODE;

	if ((type & (DATA_TRANSPORT_TYPE | DM_TRANSPORT_TYPE)) != 0)
		return TRANSPORT_MODE;

	return NOP_MODE;
}

/**
 * @brief create cmdlist node by type, node_size and dst_addr
 *
 * @param cmd_table cmdlist client table
 * @param user_client use this client type, node_size and dst_addr
 * @return int32_t 0: success -1: fail
 */

static void dump_client_node_by_tgid(struct cmdlist_table *cmd_table) 
{
	struct client_user_info_node *user_info_node_head;
	struct user_info_node *user_node = NULL;
	struct user_info_node *_u_node_ = NULL;
	struct user_client_info *client_info = NULL;
	struct user_client_info *_c_info = NULL;
	uint32_t cmdlist_node_count;

	list_for_each_entry_safe(user_node, _u_node_, &cmd_table->user_info_head, list_node) {
		if (unlikely(!user_node))
			continue;
		user_info_node_head = (struct client_user_info_node *)user_node;
		ukmd_pr_err("user_tgid = %d, ref_cout = %u:", user_info_node_head->user_tgid, user_info_node_head->ref_cnt);
		cmdlist_node_count = 0;
		list_for_each_entry_safe(client_info, _c_info, &user_info_node_head->client_list_head, list_node) {
			if (unlikely(!client_info))
				continue;
			cmdlist_node_count++;
		}
		ukmd_pr_err("this tgid = %d contains %u node", user_info_node_head->user_tgid, cmdlist_node_count);
	}
}

int32_t cmdlist_create_client(struct cmdlist_table *cmd_table, int32_t tgid,
	struct cmdlist_node_client *user_client)
{
	struct cmdlist_client *client = NULL;
	struct cmdlist_node *node = NULL;

	if (unlikely(!cmd_table || !user_client)) {
		ukmd_pr_err("input error, cmd_table or user_client is null pointer!");
		return -1;
	}

	if (user_client->scene_id > cmd_table->scene_num) {
		ukmd_pr_warn("invalid scene_id:%u!", user_client->scene_id);
		return -1;
	}

	if (!chash_is_valid(&cmd_table->ht)) {
		ukmd_pr_warn("invalid bkts scene_id:%u!", user_client->scene_id);
		return -1;
	}

	client = (struct cmdlist_client *)kzalloc(sizeof(struct cmdlist_client), GFP_KERNEL);
	if (unlikely(!client)) {
		ukmd_pr_err("kzalloc return err!");
		return -ENOMEM;
	}
	client->key = (uint32_t)atomic_inc_return(&cmd_table->cmdlist_next_id);
	client->scene_id = user_client->scene_id;
	client->node_type = user_client->type;
	client->tgid = tgid;
	client->has_appended = false;

	node = cmdlist_create_node(user_client->dev_id, user_client->scene_id,
		get_cmdlist_mode(user_client->type), user_client->node_size, user_client->dst_addr);
	if (unlikely(!node)) {
		ukmd_pr_err("cmdlist create node failed!");
		dump_client_node_by_tgid(cmd_table);
		kfree(client);
		return -1;
	}
	node->client = client;
	client->node = node;
	chash_add(&cmd_table->ht, client->scene_id, &client->hash_node);

	user_client->id = client->key;
	user_client->node_size = (uint32_t)node->node_size;
	user_client->phyaddr = node->cmd_header_addr;
	user_client->viraddr = (uint64_t)(uintptr_t)node->list_cmd_header;

	if ((tgid != -1) && (cmdlist_record_process_info(cmd_table, tgid, user_client) != 0)) {
		ukmd_pr_err("cmdlist record process create failed!");
		cmdlist_release_node(user_client->dev_id, node);
		chash_remove(&client->hash_node);
		kfree(client);
		return -1;
	}

	ukmd_pr_debug("--> create client <%u, %pK, %u>", client->key, client->node, client->node_type);
	ukmd_pr_debug("create node[%pK].scene[%d].mode[%d].size[%#x].prev=%pK.next=%pK.list_cmd_header=0x%llx.fence=%pK",
		node, node->scene_id, node->cmd_mode, node->node_size, node->cnode.prev, node->cnode.next,
		node->list_cmd_header, node->fence);

	return 0;
}

struct cmdlist_client *cmdlist_get_current_client(struct cmdlist_table *cmd_table,
	uint32_t scene_id, uint32_t cmdlist_id)
{
	struct cnode *node = NULL;
	struct cmdlist_client compare_client = {
		.key = cmdlist_id,
		.scene_id = scene_id,
	};

	if (unlikely(!cmd_table)) {
		ukmd_pr_err("input error, cmd_table is null pointer!\n");
		return NULL;
	}

	if (scene_id > cmd_table->scene_num) {
		ukmd_pr_warn("invalid scene_id:%u!", scene_id);
		return NULL;
	}

	if (!chash_is_valid(&cmd_table->ht)) {
		ukmd_pr_warn("invalid bkts scene_id:%u!", scene_id);
		return NULL;
	}

	node = chash_find(&cmd_table->ht, scene_id, &compare_client.hash_node);
	if (unlikely(!node)) {
		ukmd_pr_warn("can't find client_id[%u] and scene_id[%u]!", cmdlist_id, scene_id);
		return NULL;
	}

	return cnode_entry(node, struct cmdlist_client, hash_node);
}

static struct user_client_info *get_client_user_info_node(struct list_head *client_list_head,
	uint32_t id)
{
	struct user_client_info *user_node = NULL;
	struct user_client_info *_u_node_ = NULL;

	list_for_each_entry_safe(user_node, _u_node_, client_list_head, list_node) {
		if (unlikely(!user_node))
			continue;

		if (user_node->id == id)
			return user_node;
	}
	return NULL;
}

static void cmdlist_free_process_info(struct cmdlist_table *cmd_table, int32_t tgid, uint32_t id)
{
	struct user_client_info *client_info = NULL;
	struct client_user_info_node *node =
		(struct client_user_info_node *)(void *)get_user_info_node(&cmd_table->user_info_head, tgid);
	if (unlikely(!node)) {
		ukmd_pr_err("cannot find tgid=%d!", tgid);
		return;
	}

	client_info = get_client_user_info_node(&node->client_list_head, id);
	if (unlikely(!client_info)) {
		ukmd_pr_err("cannot find client, tgid=%d id=%u!", tgid, id);
		return;
	}
	ukmd_pr_debug("del tgid=%d sub node!", tgid);
	list_del(&client_info->list_node);
	kfree(client_info);
}

static void _cmdlist_release_client(struct cmdlist_table *cmd_table, struct cmdlist_client *client,
	uint32_t dev_id, bool free_process_record)
{
	if (free_process_record && client->tgid != -1)
		cmdlist_free_process_info(cmd_table, client->tgid, client->key);

	cmdlist_client_release_node(&client->hash_node, (void *)&dev_id);
}

static void _cmdlist_release_client_or_list(struct cmdlist_table *cmd_table,
	struct cmdlist_node_client *user_client, bool free_process_record)
{
	struct cmdlist_client *header = NULL;
	struct cmdlist_client *client = NULL;
	struct cmdlist_node *node = NULL, *_node_ = NULL;

	if (unlikely(!cmd_table || !user_client)) {
		ukmd_pr_err("cmd_table or user_client is nullptr");
		return;
	}

	header = cmdlist_get_current_client(cmd_table, user_client->scene_id, user_client->id);
	if (unlikely(!header) || unlikely(!header->node)) {
		ukmd_pr_warn("invalid scene_id:%u, cmdlist_id:%u!", user_client->scene_id, user_client->id);
		return;
	}

	if (header->node_type != SCENE_NOP_TYPE) {
		ukmd_pr_warn("--> release client<%u, %pK, %u>", header->key, header->node, header->node_type);
		_cmdlist_release_client(cmd_table, header, user_client->dev_id, free_process_record);
		return;
	}

	clist_for_each_entry_safe(node, _node_, &header->node->cnode, typeof(*node), cnode) {
		if (node && node->client) {
			client = (struct cmdlist_client *)node->client;
			ukmd_pr_debug("--> release client<%u, %pK, %u>", client->key, client->node, client->node_type);
			_cmdlist_release_client(cmd_table, client, user_client->dev_id, free_process_record);
		}
	}

	ukmd_pr_debug("--> release scene client<%u, %pK, %u>", header->key, header->node, header->node_type);
	_cmdlist_release_client(cmd_table, header, user_client->dev_id, free_process_record);
}

int32_t cmdlist_release_client_or_list(struct cmdlist_table *cmd_table, int32_t tgid,
	struct cmdlist_node_client *user_client)
{
	void_unused(tgid);
	_cmdlist_release_client_or_list(cmd_table, user_client, true);
	return 0;
}

int32_t k_cmdlist_release_client_or_list(struct cmdlist_table *cmd_table,
	struct cmdlist_node_client *user_client)
{
	_cmdlist_release_client_or_list(cmd_table, user_client, false);
	return 0;
}

int32_t cmdlist_append_next_client(struct cmdlist_table *cmd_table, int32_t tgid,
	struct cmdlist_node_client *user_client)
{
	struct cmdlist_client *client = NULL;
	struct cnode *flush_list_header = NULL;

	if (unlikely(!cmd_table || !user_client)) {
		ukmd_pr_err("input error, cmd_table or user_client is null pointer!");
		return -1;
	}

	client = cmdlist_get_current_client(cmd_table, user_client->scene_id, user_client->id);
	if (unlikely(!client || !client->node)) {
		ukmd_pr_err("invalid current cmdlist client!");
		return -1;
	}

	ukmd_pr_debug("--> scene client <%u, %pK, %u>", client->key, client->node, client->node_type);
	ukmd_pr_debug("head node[%pK].scene[%d].mode[%d].size[%#x].prev=%pK.next=%pK.list_cmd_header=%pK.fence=%pK",
		client->node, client->node->scene_id, client->node->cmd_mode, client->node->node_size,
		client->node->cnode.prev, client->node->cnode.next,
		client->node->list_cmd_header, client->node->fence);
	if (unlikely(client->node_type != SCENE_NOP_TYPE)) {
		ukmd_pr_err("not scene header type=%d!", client->node_type);
		return -1;
	}
	flush_list_header = &client->node->cnode;

	client = cmdlist_get_current_client(cmd_table, user_client->scene_id, user_client->append_next_id);
	if (unlikely(!client)) {
		ukmd_pr_err("Invalid next cmdlist client!");
		return -1;
	}
	if (unlikely(client->node_type == SCENE_NOP_TYPE)) {
	    ukmd_pr_err("forbid use same scene header type to append!");
	    return -1;
	}
	if (unlikely(client->has_appended)) {
		ukmd_pr_err("forbid append the same node");
		return -1;
	}
	client->has_appended = true;
	clist_add_tail(flush_list_header, &(client->node->cnode));
	ukmd_pr_debug("--> append client <%u, %pK, %u>", client->key, client->node, client->node_type);
	ukmd_pr_debug("append node[%pK].scene[%d].mode[%d].size[%#x].prev=%pK.next=%pK.list_cmd_header=%pK.fence=%pK",
		client->node, client->node->scene_id, client->node->cmd_mode, client->node->node_size,
		client->node->cnode.prev, client->node->cnode.next,
		client->node->list_cmd_header, client->node->fence);

	return 0;
}

int32_t cmdlist_signal_client(struct cmdlist_table *cmd_table, int32_t tgid,
	struct cmdlist_node_client *user_client)
{
	struct cmdlist_client *client = NULL;
	if (unlikely(!cmd_table || !user_client)) {
		ukmd_pr_err("input error, cmd_table or user_client is null pointer!");
		return -1;
	}

	client = cmdlist_get_current_client(cmd_table, user_client->scene_id, user_client->id);
	if (unlikely(!client || !client->node)) {
		ukmd_pr_err("invalid current cmdlist client");
		return -1;
	}
	ukmd_pr_debug("--> signal client <%u, %pK, %u>", client->key, client->node, client->node_type);
	ukmd_pr_debug("signal node[%pK].scene[%d].mode[%d].size[%#x].prev=%pK.next=%pK.list_cmd_header=%pK.fence=%pK",
		client->node, client->node->scene_id, client->node->cmd_mode, client->node->node_size,
		client->node->cnode.prev, client->node->cnode.next,
		client->node->list_cmd_header, client->node->fence);
	/* 1. signal the async fence */
	if (client->node->fence != NULL) {
		ukmd_acquire_fence_signal_release(client->node->fence);
		client->node->fence = NULL;
	}

	/* 2. refresh config cmdlist item count */
	if ((client->node_type & REGISTER_CONFIG_TYPE) != 0) {
		if (user_client->valid_payload_size > CMDLIST_ITEMS_COUNT_MAX) {
			ukmd_pr_err("invalid valid payload size");
			return -1;
		}
		/* Command node contains the number of cmd_items (namely the total_items
		 * cmd_header) cannot be equal to 1, if there are any cmd_item only one,
		 * you can copy the data, and make total_items equal to 2
		 */
		if (user_client->valid_payload_size < 2) {
			ukmd_pr_err("config cmdlist error item count: %d", user_client->valid_payload_size);
			user_client->valid_payload_size = 2;
		}
		client->node->list_cmd_header->total_items.bits.items_count = user_client->valid_payload_size;
	}

	return 0;
}

int32_t cmdlist_clear_client(struct cmdlist_table *cmd_table, struct cmdlist_node_client *user_client)
{
	struct cmdlist_client *client = NULL;
	if (unlikely(!cmd_table || !user_client)) {
		ukmd_pr_err("input error, cmd_table or user_client is null pointer!");
		return -1;
	}

	client = cmdlist_get_current_client(cmd_table, user_client->scene_id, user_client->id);
	if (unlikely(!client || !client->node)) {
		ukmd_pr_err("invalid current cmdlist client");
		return -1;
	}

	cmdlist_clear_node(client->node);
	return 0;
}

