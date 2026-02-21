/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Add linux/errqueue.h to compat linux interface
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jun 08 11:47:41 2022
 */

#ifndef _LINUX_ERRQUEUE_H
#define _LINUX_ERRQUEUE_H

#include <linux/types.h>

struct sock_ee_data_rfc4884 {
	__u16	len;
	__u8	flags;
	__u8	reserved;
};

struct sock_extended_err {
	__u32	ee_errno;
	__u8	ee_origin;
	__u8	ee_type;
	__u8	ee_code;
	__u8	ee_pad;
	__u32   ee_info;
	union	{
		__u32   ee_data;
		struct sock_ee_data_rfc4884 ee_rfc4884;
	};
};

#endif
