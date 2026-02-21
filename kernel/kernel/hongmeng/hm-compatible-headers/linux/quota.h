/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Add linux/quota.h to compat linux interface
 * Author: Huawei OS Kernel Lab
 * Create: Mon Sep 13 10:02:35 2021
 */

#ifndef _LINUX_QUOTA_
#define _LINUX_QUOTA_
#include <linux/types.h>

struct if_dqblk {
	__u64 dqb_bhardlimit;
	__u64 dqb_bsoftlimit;
	__u64 dqb_curspace;
	__u64 dqb_ihardlimit;
	__u64 dqb_isoftlimit;
	__u64 dqb_curinodes;
	__u64 dqb_btime;
	__u64 dqb_itime;
	__u32 dqb_valid;
};
#endif
