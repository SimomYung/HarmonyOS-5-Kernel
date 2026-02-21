/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Add linux/if_addrlabel.h to compat linux interface
 * Author: Huawei OS Kernel Lab
 * Create: Fri Sep 24 15:25:15 2021
 */

#ifndef __LINUX_IF_ADDRLABEL_H
#define __LINUX_IF_ADDRLABEL_H

#include <linux/types.h>

struct ifaddrlblmsg {
	__u8        ifal_family;
	__u8        __ifal_reserved;
	__u8        ifal_prefixlen;
	__u8        ifal_flags;
	__u32       ifal_index;
	__u32       ifal_seq;
};

enum {
	IFAL_ADDRESS = 1,
	IFAL_LABEL = 2,
	__IFAL_MAX
};

#endif
