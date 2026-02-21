/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Implementation of signalfd
 * Author: Huawei OS Kernel Lab
 * Create: Mon Sep 21 15:52:05 2020
 */

#include <vfs.h>
#include <fd.h>

int vfs_signalfd_notify(__u32 cnode_idx, const void *buf, int fdcnt)
{
	return actvcap_fscall(vfs_signalfd_notify, cnode_idx, buf, fdcnt);
}
