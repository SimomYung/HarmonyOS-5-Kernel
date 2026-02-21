/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Add linux/raw.h to compat linux interface
 * Author: Huawei OS Kernel Lab
 * Create: Wed Nov 17 19:08:30 2021
 */

#ifndef LINUX_RAW_H
#define LINUX_RAW_H

#include <linux/types.h>
#include <linux/ioctl.h>

#define RAW_SETBIND	_IO( 0xac, 0 )
#define RAW_GETBIND	_IO( 0xac, 1 )

struct raw_config_request {
	int	raw_minor;
	__u64	block_major;
	__u64	block_minor;
};

#endif
