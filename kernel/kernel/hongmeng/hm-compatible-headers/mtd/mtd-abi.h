/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Add mtd/mtd-abi.h to compat linux interface
 * Author: Huawei OS Kernel Lab
 * Create: Mon Nov 29 10:52:50 2021
 */

#ifndef __MTD_ABI_H__
#define __MTD_ABI_H__

#include <linux/types.h>
#include <asm/ioctl.h>

#define MTD_NANDFLASH		4
#define MTD_MLCNANDFLASH	8

#define MTD_ABSENT		0
#define MTD_RAM			1
#define MTD_ROM			2
#define MTD_NORFLASH		3
#define MTD_DATAFLASH		6
#define MTD_UBIVOLUME		7

struct erase_info_user64 {
	__u64 start;
	__u64 length;
};

struct erase_info_user {
	__u32 start;
	__u32 length;
};

struct mtd_oob_buf64 {
	__u64 start;
	__u32 pad;
	__u32 length;
	__u64 usr_ptr;
};

struct mtd_oob_buf {
	__u32 start;
	__u32 length;
	unsigned char *ptr;
};

enum {
	MTD_OPS_PLACE_OOB = 0,
	MTD_OPS_AUTO_OOB = 1,
	MTD_OPS_RAW = 2,
};

struct mtd_write_req {
	__u64 start;
	__u64 len;
	__u64 ooblen;
	__u64 usr_data;
	__u64 usr_oob;
	__u8 mode;
	__u8 padding[7];
};

struct mtd_info_user {
	__u8 type;
	__u32 flags;
	__u32 size;
	__u32 erasesize;
	__u32 writesize;
	__u32 oobsize;
	__u64 padding;
};

#define MEMGETINFO		_IOR('M', 1, struct mtd_info_user)
#define MEMERASE		_IOW('M', 2, struct erase_info_user)
#define MEMWRITEOOB		_IOWR('M', 3, struct mtd_oob_buf)
#define MEMREADOOB		_IOWR('M', 4, struct mtd_oob_buf)
#define MEMGETBADBLOCK		_IOW('M', 11, __kernel_loff_t)
#define MEMSETBADBLOCK		_IOW('M', 12, __kernel_loff_t)
#define ECCGETSTATS		_IOR('M', 18, struct mtd_ecc_stats)
#define MEMERASE64		_IOW('M', 20, struct erase_info_user64)
#define MEMWRITEOOB64		_IOWR('M', 21, struct mtd_oob_buf64)
#define MEMREADOOB64		_IOWR('M', 22, struct mtd_oob_buf64)
#define MEMWRITE		_IOWR('M', 24, struct mtd_write_req)

struct mtd_ecc_stats {
	__u32 corrected;
	__u32 failed;
	__u32 badblocks;
	__u32 bbtblocks;
};

#endif
