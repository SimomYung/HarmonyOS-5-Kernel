/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Wed Feb 26 16:21:32 2020
 */
#include <errno.h>
#include "syscall.h"
#include "libc.h"

int __setxid(int nr, int id, int eid, int sid)
{
	int ret = (int)__syscall(nr, id, eid, sid);
	if (ret != 0) {
		if (ret < 0) {
			errno = -ret;
		} else {
			errno = ret;
		}
		ret = -1;
	}
	return ret;
}
