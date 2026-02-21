/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Device header file for XPM
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jul 17 09:30:10 2023
 */

#ifndef VFS_DEV_XPM_H
#define VFS_DEV_XPM_H

#include <sys/ioctl.h>

#define DEV_XPM_MINOR				0x20
#define HM_XPM_REGION_IOCTL_BASE		'x'
#define HM_SET_XPM_REGION_ID			1
#define HM_SET_XPM_OWNERID_ID			2
#define HM_SET_JITFORT_ENABLE			3
#define HM_SET_JITFORT_DISABLE			4

#define SET_XPM_REGION_CMD _IOW(HM_XPM_REGION_IOCTL_BASE, HM_SET_XPM_REGION_ID, struct xpm_region_info_s)
#define SET_XPM_OWNERID_CMD _IOW(HM_XPM_REGION_IOCTL_BASE, HM_SET_XPM_OWNERID_ID, struct xpm_region_info_s)
#define SET_XPM_JITFORT_ENABLE_CMD _IOW(HM_XPM_REGION_IOCTL_BASE, HM_SET_JITFORT_ENABLE, unsigned long)
#define SET_XPM_JITFORT_DISABLE_CMD _IOW(HM_XPM_REGION_IOCTL_BASE, HM_SET_JITFORT_DISABLE, unsigned long)

#define MAX_OWNERID_LEN  64

struct xpm_region_info_s {
	uintptr_t addr_base;
	size_t length;

	uint32_t id_type;
	char ownerid[MAX_OWNERID_LEN];
};

#endif // VFS_DEV_XPM_H
