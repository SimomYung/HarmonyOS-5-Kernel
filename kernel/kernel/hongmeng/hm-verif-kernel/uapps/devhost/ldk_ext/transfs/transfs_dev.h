/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Transfs device informations
 * Author: Huawei OS Kernel Lab
 * Create: Thu Oct 21 16:00:39 2021
 */

#ifndef __TRANSFS_DEV_H__
#define __TRANSFS_DEV_H__

#include <devhost/device.h>

struct trans_dev_file {
	/* current mount point superblock info */
	unsigned int sb_index;
	unsigned int block_size_shift;
	/* Number of bits occupied by the dentry index in the I/O sector */
	unsigned int sector_node_bits;
};

int transfs_parent_device_init(void);
int transfs_device_init(struct device_info **devinfo_ret, uint32_t *dev_id_ret);
int transfs_device_destroy(struct device_info *devinfo);

#endif
