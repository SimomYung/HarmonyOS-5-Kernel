/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Add linux/fs.h to compat linux interface
 * Author: Huawei OS Kernel Lab
 * Create: Thu Sep 23 14:39:10 2021
 */

#ifndef _LINUX_FS_H_
#define _LINUX_FS_H_

#include <sys/mount.h>
#include <asm-generic/ioctl.h>
#include <linux/types.h>
#include <linux/fiemap.h>

#define BLOCK_SIZE_BITS	10
#define BLOCK_SIZE	(1<<BLOCK_SIZE_BITS)

#define SEEK_SET	0

struct fstrim_range {
	__u64 start;
	__u64 len;
	__u64 minlen;
};

#define BLKDISCARD	_IO(0x12,119)
#define BLKIOMIN	_IO(0x12,120)
#define BLKIOOPT	_IO(0x12,121)
#define BLKALIGNOFF	_IO(0x12,122)
#define BLKPBSZGET	_IO(0x12,123)
#define BLKSECDISCARD	_IO(0x12,125)
#define BLKZEROOUT	_IO(0x12,127)

#define FIFREEZE	_IOWR('X', 119, int)
#define FITHAW		_IOWR('X', 120, int)
#define FITRIM		_IOWR('X', 121, struct fstrim_range)

#define FS_IOC_FIEMAP	_IOWR('f', 11, struct fiemap)

#define FS_IOC_GETFLAGS	_IOR('f', 1, long)
#define FS_IOC_SETFLAGS	_IOW('f', 2, long)

#define	FS_COMPR_FL	0x00000004
#define FS_SYNC_FL	0x00000008
#define FS_IMMUTABLE_FL	0x00000010
#define FS_APPEND_FL	0x00000020
#define FS_DIRSYNC_FL	0x00010000

#endif
