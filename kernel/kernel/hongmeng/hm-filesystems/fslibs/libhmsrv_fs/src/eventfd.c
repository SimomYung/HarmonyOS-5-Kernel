/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Implementation of eventfd
 * Author: Huawei OS Kernel Lab
 * Create: Mon Sep 21 15:52:05 2020
 */

#include <vfs.h>
#include <fd.h>

int vfs_write_eventfd(int fd, const void *buf, size_t nbyte, __u32 cnode_idx)
{
	int err;

	err = actvcap_fscall(vfs_write_eventfd, fd, buf, nbyte, cnode_idx);
	if (err < 0) {
		hm_warn("write eventfd failed %s\n", hmstrerror(err));
	}

	return err;
}
