/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Mon May 16 10:30:00 2022
 */

#ifndef __DEVHOST_NET_SOCKET_H__
#define __DEVHOST_NET_SOCKET_H__

#include <linux/net.h>
#include <devhost/memory.h>

struct devhost_socket {
	struct socket *sock;
	int fd;
	int file_flags;
	struct file *file;
};

struct device_info;

extern int devhost_net_fops_poll(void *ctx, void *pinfo, unsigned int *revents);
extern int devhost_net_fops_mmap(void *ctx, struct devhost_vma_info *vma_info);
extern int liblinux_pal_is_compat_client(void);
extern int liblinux_pal_copy_msghdr_from_user(void *dst, void *src);
extern int liblinux_pal_copy_msghdr_to_user(void *dst, void *src, void *uaddr, void *ucmsg, int ulen);

#endif /* __DEVHOST_NET_SOCKET_H__ */
