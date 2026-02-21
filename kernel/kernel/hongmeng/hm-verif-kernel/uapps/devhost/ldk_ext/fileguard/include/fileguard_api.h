/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: Declare interface of fileguard_api
 * Author: Huawei OS Kernel Lab
 * Create: Sat Jun 08 11:54:30 2024
 */

#ifndef __DEVHOST_PLUGIN_FILEGUARD_API_H__
#define __DEVHOST_PLUGIN_FILEGUARD_API_H__

struct devhost_file_guard_ops {
	int (*msg_handler)(const void *message, unsigned int len);
};

typedef int (*fileguard_actvcall_func)(const void *message, unsigned int len);

void devhost_fileguard_ops_register(fileguard_actvcall_func actvcall_hdr);

int network_actvcall_message_handler(const void *message, unsigned int len);

#endif /* __DEVHOST_PLUGIN_KSTATE_API_H__ */
