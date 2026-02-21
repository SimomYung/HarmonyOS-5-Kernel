/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: Headers for sysctl.c
 * Author: Huawei OS Kernel Lab
 * Create: Tue MAY 23 10:56:30 CST 2024
 */

#ifndef __LIBDH_LINUX_PROC_NET_H__
#define __LIBDH_LINUX_PROC_NET_H__

#include "procfs.h"

#define PROCFS_PATH_NAME_MAX 256
#define PROCFS_ENTRY_MAX_NUM 128
#define FLAG_NET_PROCFS 0xff
#define PROCFS_SYS_MAX_NUM 1024

struct procfs_entry_node {
	unsigned int mode;
	void *ctx;
	char path[PROCFS_PATH_NAME_MAX];
};

struct proc_path_list {
	int len;
	int max_len;
	unsigned int nsid;
	struct procfs_entry_node path[];
};

void liblinux_create_procfs_in_batches(struct net *net, int *ret);
int liblinux_create_netdev_procfs_batch(struct net_device *dev);
#endif /* __LIBDH_LINUX_PROC_NET_H__ */
