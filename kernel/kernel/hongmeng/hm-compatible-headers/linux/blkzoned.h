/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Add linux/blkzoned.h to compat linux interface
 * Author: Huawei OS Kernel Lab
 * Create: Wed Nov 17 11:34:26 2021
 */

#ifndef LINUX_BLKZONED_H
#define LINUX_BLKZONED_H

#include <linux/types.h>
#include <linux/ioctl.h>

enum blk_zone_report_flags {
	BLK_ZONE_REP_CAPACITY	= (1 << 0),
};

struct blk_zone {
	__u64	start;
	__u64	len;
	__u64	wp;
	__u8	type;
	__u8	cond;
	__u8	non_seq;
	__u8	reset;
	__u8	resv[4];
	__u64	capacity
	__u8	reserved[24];
};

struct blk_zone_report {
	__u64		sector;
	__u32		nr_zones;
	__u32		flags;
	struct blk_zone zones[0];
};

struct blk_zone_range {
	__u64		sector;
	__u64		nr_sectors;
};

#define BLKREPORTZONE	_IOWR(0x12, 130, struct blk_zone_report)
#define BLKRESETZONE	_IOW(0x12, 131, struct blk_zone_range)
#define BLKOPENZONE	_IOW(0x12, 134, struct blk_zone_range)
#define BLKCLOSEZONE	_IOW(0x12, 135, struct blk_zone_range)
#define BLKFINISHZONE	_IOW(0x12, 136, struct blk_zone_range)

#endif
