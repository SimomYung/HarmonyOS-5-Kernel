/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Add linux/fiemap.h to compat linux interface
 * Author: Huawei OS Kernel Lab
 * Create: Wed Nov 17 12:18:30 2021
 */

#ifndef LINUX_FIEMAP_H
#define LINUX_FIEMAP_H

#include <linux/types.h>

struct fiemap_extent {
	__u64 fe_logical;
	__u64 fe_physical;
	__u64 fe_length;
	__u64 fe_reserved64[2];
	__u32 fe_flags;
	__u32 fe_reserved[3];
};

struct fiemap {
	__u64 fm_start;
	__u64 fm_length;
	__u32 fm_flags;
	__u32 fm_mapped_extents;
	__u32 fm_extent_count;
	__u32 fm_reserved;
	struct fiemap_extent fm_extents[0];
};

#define FIEMAP_FLAG_SYNC		0x00000001

#define FIEMAP_EXTENT_LAST		0x00000001
#define FIEMAP_EXTENT_UNKNOWN		0x00000002
#define FIEMAP_EXTENT_DELALLOC		0x00000004
#define FIEMAP_EXTENT_DATA_INLINE	0x00000200
#define FIEMAP_EXTENT_SHARED		0x00002000

#endif
