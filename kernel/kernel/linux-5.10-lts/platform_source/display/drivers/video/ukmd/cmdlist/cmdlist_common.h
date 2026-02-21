/**
 * @file cmdlist_common.h
 * @brief Common Sturct/Interface/Defination for cmdlist driver function
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
#ifndef __CMDLIST_COMMON_H__
#define __CMDLIST_COMMON_H__

#include <linux/types.h>
#include <linux/list.h>

enum cmdlist_device_id {
	CMDLIST_DEV_ID_DPU,
	CMDLIST_DEV_ID_AAE,
	CMDLIST_DEV_MAX_NUM = 4,
};

struct user_info_node {
	struct list_head list_node;
	int32_t user_tgid;
	uint32_t ref_cnt;
};

// No check or lock, user need check and lock it
static inline struct user_info_node *get_user_info_node(struct list_head *user_info_head, int32_t user_tgid)
{
	struct user_info_node *user_node = NULL;
	struct user_info_node *_u_node_ = NULL;

	list_for_each_entry_safe(user_node, _u_node_, user_info_head, list_node) {
		if (unlikely(!user_node))
			continue;

		if (user_node->user_tgid == user_tgid)
			return user_node;
	}
	return NULL;
}

#endif
