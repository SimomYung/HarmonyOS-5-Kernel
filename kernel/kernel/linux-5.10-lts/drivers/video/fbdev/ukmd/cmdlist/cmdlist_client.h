/**
 * @file cmdlist_client.h
 * @brief Implementing interface function for cmdlist node
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

#ifndef __CMDLIST_CLIENT_H__
#define __CMDLIST_CLIENT_H__

#include <ukmd_cmdlist.h>
#include "cmdlist_interface.h"
#include "chash.h"

struct client_user_info_node {
	struct list_head list_node;
	int32_t user_tgid;
	uint32_t ref_cnt;
	struct list_head client_list_head;
};

struct cmdlist_table {
	atomic_t cmdlist_next_id;
	uint32_t scene_num;
	/* each scene has owner table, which would manager cmdlist node in it */
	struct chash_table ht;
	struct list_head user_info_head; // record every user tgid for release
};

struct cmdlist_client {
	struct cnode hash_node;
	uint32_t scene_id;
	uint32_t key;
	uint32_t node_type;
	int32_t tgid;
	struct cmdlist_node *node;
	bool has_appended;
};

/**
 * @brief record process open
 *
 * @param cmd_table cmdlist table
 * @param tgid process id
 * @return 0 for succ, others for fail
 */
int32_t cmdlist_client_open(struct cmdlist_table *cmd_table, uint32_t dev_id, int32_t tgid);

/**
 * @brief record process close
 *
 * @param cmd_table cmdlist table
 * @param tgid process id
 * @return 0 for succ, others for fail
 */
void cmdlist_client_release(struct cmdlist_table *cmd_table, uint32_t dev_id, int32_t tgid);

/**
 * @brief init cmdlist id
 *
 * @param struct cmdlist_table *cmd_table
 * @return void
 */
void cmdlist_client_init_cmdlist_id(struct cmdlist_table *cmd_table);
/**
 * @brief init cmdlist device and cmdlist table
 *
 * @param cmd_table cmdlist client table
 * @return int32_t 0: success -1: fail
 */
int32_t cmdlist_client_table_setup(struct cmdlist_table *cmd_table);

/**
 * @brief remove cmdlist table and list
 *
 * @param cmd_table cmdlist client table
 * @param dev_id cmdlist device id
 */
void cmdlist_client_table_remove(struct cmdlist_table *cmd_table, uint32_t dev_id);

/**
 * @brief create cmdlist node by type, node_size and dst_addr
 *
 * @param cmd_table cmdlist client table
 * @param tgid process pid
 * @param user_client use this client type, node_size and dst_addr
 * @return int32_t 0: success -1: fail
 */
int32_t cmdlist_create_client(struct cmdlist_table *cmd_table, int32_t tgid,
	struct cmdlist_node_client *user_client);

/**
 * @brief user space (IOCTL) release cmdlist client or list by scene_id and cmdlist_id
 *
 * @param cmd_table cmdlist client table
 * @param tgid process pid
 * @param user_client use this client scene_id and cmdlist_id
 */
int32_t cmdlist_release_client_or_list(struct cmdlist_table *cmd_table, int32_t tgid,
	struct cmdlist_node_client *user_client);

/**
 * @brief kernel space release cmdlist client or list by scene_id and cmdlist_id
 *
 * @param cmd_table cmdlist client table
 * @param user_client use this client scene_id and cmdlist_id
 */
int32_t k_cmdlist_release_client_or_list(struct cmdlist_table *cmd_table,
	struct cmdlist_node_client *user_client);

/**
 * @brief append cmdlist node which recorded by user_client with append_next_id to this scene cmdlist node
 *
 * @param cmd_table cmdlist client table
 * @param tgid process pid
 * @param user_client use this client id and append_next_id
 * @return int32_t 0: success -1: fail
 */
int32_t cmdlist_append_next_client(struct cmdlist_table *cmd_table, int32_t tgid,
	struct cmdlist_node_client *user_client);

/**
 * @brief signal this current scene cmdlist node
 *
 * @param cmd_table cmdlist client table
 * @param tgid process pid
 * @param user_client use this client id
 * @return int32_t 0: success -1: fail
 */
int32_t cmdlist_signal_client(struct cmdlist_table *cmd_table, int32_t tgid,
	struct cmdlist_node_client *user_client);

int32_t cmdlist_clear_client(struct cmdlist_table *cmd_table, struct cmdlist_node_client *user_client);

#ifdef CONFIG_UKMD_DEBUG_ENABLE
/**
 * @brief Dump this current scene cmdlist node
 *
 * @param cmd_table cmdlist client table
 * @param tgid process pid
 * @param user_client use this client id
 * @return int32_t 0: success -1: fail
 */
int32_t cmdlist_dump_client(struct cmdlist_table *cmd_table, int32_t tgid,
	struct cmdlist_node_client *user_client);

/**
 * @brief Dump all the same scene cmdlist nodes
 *
 * @param cmd_table cmdlist client table
 * @param tgid process pid
 * @param user_client use the scene_id
 * @return int32_t 0: success -1: fail
 */
int32_t cmdlist_dump_scene(struct cmdlist_table *cmd_table, int32_t tgid,
	struct cmdlist_node_client *user_client);
#endif
/**
 * @brief get cmdlist client by scene id and cmdlist id
 *
 * @param cmd_table cmdlist client table
 * @param scene_id
 * @param cmdlist_id
 * @return struct cmdlist_client*
 */
struct cmdlist_client *cmdlist_get_current_client(struct cmdlist_table *cmd_table,
	uint32_t scene_id, uint32_t cmdlist_id);

#endif
