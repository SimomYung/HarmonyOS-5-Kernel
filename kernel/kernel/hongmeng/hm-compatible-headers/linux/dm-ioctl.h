/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Add linux/dm-ioctl.h to compat linux interface
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jun 28 15:38:06 2022
 */

#ifndef LINUX_DM_IOCTL_H
#define LINUX_DM_IOCTL_H

#include <linux/types.h>
#include <linux/ioctl.h>

#define DM_MAX_TYPE_NAME	16
#define DM_NAME_LEN		128
#define DM_UUID_LEN		129

struct dm_ioctl {
	__u32	version[3];
	__u32	data_size;

	__u32	data_start;

	__u32	target_count;
	__s32	open_count;
	__u32	flags;

	__u32	event_nr;
	__u32	padding;

	__u64	dev;

	char	name[DM_NAME_LEN];
	char	uuid[DM_UUID_LEN];
	char	data[7];
};

struct dm_target_spec {
	__u64	sector_start;
	__u64	length;
	__s32	status;
	__u32	next;
	char	target_type[DM_MAX_TYPE_NAME];
};

enum {
	DM_VERSION_CMD = 0,
	DM_REMOVE_ALL_CMD,
	DM_LIST_DEVICES_CMD,

	DM_DEV_CREATE_CMD,
	DM_DEV_REMOVE_CMD,
	DM_DEV_RENAME_CMD,
	DM_DEV_SUSPEND_CMD,
	DM_DEV_STATUS_CMD,
	DM_DEV_WAIT_CMD,

	DM_TABLE_LOAD_CMD,
	DM_TABLE_CLEAR_CMD,
	DM_TABLE_DEPS_CMD,
	DM_TABLE_STATUS_CMD,

	DM_LIST_VERSIONS_CMD,
	DM_TARGET_MSG_CMD,
	DM_DEV_SET_GEOMETRY_CMD,
	DM_DEV_ARM_POLL_CMD,
	DM_GET_TARGET_VERSION_CMD,
};
#define DM_IOCTL		0xfd
#define DM_DEV_CREATE		_IOWR(DM_IOCTL, DM_DEV_CREATE_CMD, struct dm_ioctl)
#define DM_DEV_REMOVE		_IOWR(DM_IOCTL, DM_DEV_REMOVE_CMD, struct dm_ioctl)

#define DM_DEV_SUSPEND		_IOWR(DM_IOCTL, DM_DEV_SUSPEND_CMD, struct dm_ioctl)
#define DM_DEV_STATUS		_IOWR(DM_IOCTL, DM_DEV_STATUS_CMD, struct dm_ioctl)
#define DM_TABLE_LOAD		_IOWR(DM_IOCTL, DM_TABLE_LOAD_CMD, struct dm_ioctl)
#define DM_READONLY_FLAG	(1 << 0)

#define DM_STATUS_TABLE_FLAG	(1 << 4)

#endif
