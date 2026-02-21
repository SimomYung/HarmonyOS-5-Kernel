/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2024. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */

#ifndef HVGR_DATAN_SWITCH_H
#define HVGR_DATAN_SWITCH_H

#include "hvgr_defs.h"

#define DATAN_BUFF_LEN                          100U
#define DATAN_DEBUGFS_NAME_LEN_MAX              32U
#define DATAN_SWITCH_FILE_MODE					0644

struct hvgr_datan_debugfs_files {
	char name[DATAN_DEBUGFS_NAME_LEN_MAX];
	umode_t mode;
	const struct file_operations *fops;
};

int hvgr_datan_switch_init(struct hvgr_device * const gdev);

#endif