/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Implementation of net stack interfacesa
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jun 8 22:14:50 2022
 */

#ifndef DEVHOST_LDK_EXT_NET_SOCKET_H
#define DEVHOST_LDK_EXT_NET_SOCKET_H

#include <devhost/file.h>
#include <hmnet/hm_socket.h>

struct filp_node *alloc_socket_filp_node(__u32 cnode_idx, unsigned long long *kshm_key_out,
					 unsigned long *kobj_offset_out);
int call_bpf(int cmd, void *attr, unsigned int size);
int call_dev_ioctl(int cmd, void *arg_buf);
extern int hmnetd_fusion_entry(int argc, char **argv, fusion_ops_t *ops);

#endif
