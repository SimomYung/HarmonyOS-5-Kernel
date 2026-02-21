/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Implementation of preadv2 lsyscall hooks
 * Author: Huawei OS Kernel Lab
 * Create: Fri Aug 18 11:00:51 2023
 */

#ifndef _UAPI_LINUX_STATX_H
#define _UAPI_LINUX_STATX_H

#include <hongmeng/types.h>
struct statx_timestamp {
	__s64	tv_sec;
	__u32	tv_nsec;
	__s32	__reserved;
};

struct statx {
	/* 0x00 */
	__u32	stx_mask;
	__u32	stx_blksize;
	__u64	stx_attributes;
	/* 0x10 */
	__u32	stx_nlink;
	__u32	stx_uid;
	__u32	stx_gid;
	__u16	stx_mode;
	__u16	__spare0[1];
	/* 0x20 */
	__u64	stx_ino;
	__u64	stx_size;
	__u64	stx_blocks;
	__u64	stx_attributes_mask;
	/* 0x40 */
	struct statx_timestamp	stx_atime;
	struct statx_timestamp	stx_btime;
	struct statx_timestamp	stx_ctime;
	struct statx_timestamp	stx_mtime;
	/* 0x80 */
	__u32	stx_rdev_major;
	__u32	stx_rdev_minor;
	__u32	stx_dev_major;
	__u32	stx_dev_minor;
	/* 0x90 */
	__u64	__spare2[14];
	/* 0x100 */
};

#define HM_STATX_TYPE		0x00000001U
#define HM_STATX_MODE		0x00000002U
#define HM_STATX_NLINK		0x00000004U
#define HM_STATX_UID		0x00000008U
#define HM_STATX_GID		0x00000010U
#define HM_STATX_ATIME		0x00000020U
#define HM_STATX_MTIME		0x00000040U
#define HM_STATX_CTIME		0x00000080U
#define HM_STATX_INO		0x00000100U
#define HM_STATX_SIZE		0x00000200U
#define HM_STATX_BLOCKS		0x00000400U
#define HM_STATX_BASIC_STATS	0x000007ffU
#define HM_STATX_BTIME		0x00000800U
#define HM_STATX_ALL		0x00000fffU
#define HM_STATX_MNT_ID		0x00001000U
#define HM_STATX__RESERVED	0x80000000U

#define STATX_ATTR_VERITY	0x00100000

#endif /* _UAPI_LINUX_STAT_H */
