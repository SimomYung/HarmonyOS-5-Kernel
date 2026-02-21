/*
 * virtual_network.h
 *
 * virtual network kernel module implementation
 *
 * Copyright (c) 2024-2024 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */

#ifndef _VIRTUAL_NETWORK_H_
#define _VIRTUAL_NETWORK_H_

#include <linux/types.h>

typedef struct vnet_app_info {
    u32 uid;
    int policy_id;
} vnet_app_info_t;

void vnet_clear_app_infos(void);
int vnet_add_app_info(vnet_app_info_t *app_info);
void vnet_remove_app_info(u32 uid);
// policy_id should larger than 0. return false when policy_id <= 0 or uid not exit; else return true.
bool vnet_set_policy_for_uid(u32 uid, int policy_id);
// return -1 when uid not exit; return 0 when policy not set for uid; otherwise return the policy_id of uid.
int vnet_get_policy_for_uid(u32 uid);

bool vnet_is_available(void);

#endif /* _VIRTUAL_NETWORK_H_ */
