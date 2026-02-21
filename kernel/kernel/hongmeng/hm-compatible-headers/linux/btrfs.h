/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Add linux/btrfs.h to compat linux interface
 * Author: Huawei OS Kernel Lab
 * Create: Wed Nov 17 11:43:01 2021
 */

#ifndef LINUX_BTRFS_H
#define LINUX_BTRFS_H

#include <linux/types.h>
#include <linux/ioctl.h>

#define BTRFS_IOCTL_MAGIC 0x94

struct btrfs_ioctl_search_key {
	__u64 tree_id;
	__u64 min_objectid;
	__u64 max_objectid;
	__u64 min_offset;
	__u64 max_offset;
	__u64 min_transid;
	__u64 max_transid;
	__u32 min_type;
	__u32 max_type;
	__u32 nr_items;
	__u32 unused;
	__u64 unused1;
	__u64 unused2;
	__u64 unused3;
	__u64 unused4;
};

struct btrfs_ioctl_search_header {
	__u64 transid;
	__u64 objectid;
	__u64 offset;
	__u32 type;
	__u32 len;
};

#define BTRFS_SEARCH_ARGS_BUFSIZE (4096 - sizeof(struct btrfs_ioctl_search_key))

struct btrfs_ioctl_search_args {
	struct btrfs_ioctl_search_key	key;
	char				buf[BTRFS_SEARCH_ARGS_BUFSIZE];
};

#define BTRFS_IOC_TREE_SEARCH _IOWR(BTRFS_IOCTL_MAGIC, 17, \
					struct btrfs_ioctl_search_args)

#endif
