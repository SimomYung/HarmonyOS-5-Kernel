/*
 * virtual_network_netlink.h
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

#ifndef _VIRTUAL_NETWORK_NETLINK_H_
#define _VIRTUAL_NETWORK_NETLINK_H_

#include <linux/types.h>

typedef struct vnet_netlink_msg_head {
    u32 msg_type;  // vnet_msg_type_t
    u32 msg_len; // sizeof(vnet_netlink_msg_head_t) + length of |msg_data|
    char msg_data[0]; // 具体格式由|msg_type|对应模块自定义
} vnet_netlink_msg_head_t;

typedef enum vnet_msg_type {
    VNET_MSG_TYPE_NULL = 0,
    VNET_MSG_TYPE_CLOUD_NETWORK = 0,
    VNET_MSG_TYPE_BASE = 1,
    VNET_MSG_TYPE_NUM_MAX
} vnet_msg_type_t;

typedef enum vnet_exec_type {
    VNET_TASKLET_EXEC = 0,
    VNET_WORK_EXEC = 1,
    VNET_EXEC_TYPE_NUM_MAX
} vnet_exec_type_t;

// msg_len: size in bytes of |msg_data|
// msg_data: point to vnet_netlink_msg_head_t.msg_data
typedef void (*vnet_netlink_msg_receive_func)(u32 msg_len, const char *msg_data);

typedef struct vnet_netlink_msg_handle {
    vnet_msg_type_t msg_type;
    vnet_netlink_msg_receive_func msg_receive_func;
} vnet_netlink_msg_handle_t;

void vnet_register_netlink_msg_handle(vnet_netlink_msg_handle_t *msg_handle);
void vnet_unregister_netlink_msg_handle(vnet_msg_type_t msg_type);
void vnet_send_netlink_report(const vnet_netlink_msg_head_t *msg, vnet_exec_type_t type);

int vnet_netlink_ctrl_init(void);
void vnet_netlink_ctrl_exit(void);

#endif /* _VIRTUAL_NETWORK_NETLINK_H_ */
