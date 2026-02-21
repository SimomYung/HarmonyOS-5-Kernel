/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Memmgr rootfs management
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jan 05 04:37:19 2021
 */
#ifndef INTAPI_MEM_MEMMGR_ROOTFS_H
#define INTAPI_MEM_MEMMGR_ROOTFS_H

#include <stdint.h>
#include <lib/cpio.h>

enum rootfs_cpio_type {
	ROOTFS_CPIO_HONGMENG,
	ROOTFS_CPIO_LINUX,
	ROOTFS_CPIO_TYPE_MAX
};
int memmgr_rootfs_load(enum rootfs_cpio_type cpio_type);
void get_real_cpio_info(struct __sysctrl_rootfs_va_size *info, struct cpio_header **cpio_start,
			uint64_t *cpio_size);
#endif
