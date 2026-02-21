/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: vhost user client channel header
 * Author: Huawei OS Kernel Lab
 * Create: Tue Sep 14 20:44:29 2021
 */

#ifndef __VHOST_CHANNEL_H__
#define __VHOST_CHANNEL_H__

#include "libvhost_user/vhost_msg.h"

#define HM_VHOST_IOCTL 0xFF

union vhost_path_s {
	char *path;
};

/**
 * @desc @vhost_channel is an abstraction of communication ways
 * between client and server, different vhost server may implements
 * different channel server. E.g hmnetd support generic netlink vhost server.
 */
struct vhost_channel_s {
	int (*init)(struct vhost_channel_s *c);
	int (*connect)(struct vhost_channel_s *c, const union vhost_path_s *path);
	int (*request)(struct vhost_channel_s *c,
		       const struct vhost_user_msg_s *request,
		       struct vhost_user_msg_s *response);
	void (*release)(struct vhost_channel_s *c);
};

struct vhost_ioctl_arg_s {
	const struct vhost_user_msg_s *request;
	struct vhost_user_msg_s *response;
};

#endif
