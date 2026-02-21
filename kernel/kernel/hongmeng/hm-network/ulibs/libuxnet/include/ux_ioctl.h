/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Header file for ux ioctl
 * Author: Huawei OS Kernel Lab
 * Create: Sun Apr 28 13:34:09 2019
 */

#ifndef __LIBUXNET_IOCTL_H
#define __LIBUXNET_IOCTL_H

#define DEVICE_SIZE 24
/* keep consistent with LINUX if_vlan.h */
struct vlan_ioctl_args {
	int cmd;
	char device1[DEVICE_SIZE];

	union {
		char device2[DEVICE_SIZE];
		int VID;
		unsigned int skb_priority;
		unsigned int name_type;
		unsigned int bind_type;
		unsigned int flag;
	} u;

	short vlan_qos;
};

int ux_ioctl_cmdlen(int cmd);

#endif
