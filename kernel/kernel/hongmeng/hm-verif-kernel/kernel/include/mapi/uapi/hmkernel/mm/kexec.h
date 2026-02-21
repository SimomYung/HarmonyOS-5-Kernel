/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Definition of kexec
 * Author: Huawei OS Kernel Lab
 * Create: Wed Aug 16 16:49:34 2023
 */

#ifndef UAPI_HMKERNEL_KEXEC_H
#define UAPI_HMKERNEL_KEXEC_H

struct softreboot_info {
	unsigned long bootimg_start;
	unsigned long bootimg_size;
	unsigned long dtb_start;
	unsigned long dtb_size;
	unsigned long rootfs_start;
	unsigned long rootfs_size;
};

enum restart_type {
	__RESTART_KEXEC,
	__RESTART_SOFTREBOOT,
};

#endif
