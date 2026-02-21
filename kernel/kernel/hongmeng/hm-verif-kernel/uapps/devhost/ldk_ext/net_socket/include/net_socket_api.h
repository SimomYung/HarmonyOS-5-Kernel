/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Prototypes and type definitions for LDK socket APIs
 * Author: Huawei OS Kernel Lab
 * Create: Mon May 16 10:51:12 2022
 */
#ifndef __DEVHOST_PLUGIN_NET_SOCKET_API_H__
#define __DEVHOST_PLUGIN_NET_SOCKET_API_H__

#if defined(MODULE)
#include <linux/types.h>
#include <linux/socket.h>
#elif defined(UDK_MODULE) || defined(UDK_FRAMEWORK)
#include <udk/stdint.h>
#include <stddef.h>
#else
#include <stdint.h>
#include <stddef.h>
#endif

#include <devhost/device.h>

#define ROOT_NET_NSID 8189

struct devhost_net_socket_ops {
	int (*close)(void *);
	int (*connect)(void *, const void *, unsigned);
	int (*bind)(void *, const void *, unsigned);
	int (*listen)(void *, int);
	int (*accept)(void *, void *, unsigned *, int, unsigned long, void **, void *);
	int (*shutdown)(void *, int);
	int (*ioctl)(void *, int, void *);
	int (*fcntl)(void *, int, int);
	int (*send)(void *, const void *, size_t, int);
	int (*recv)(void *, void *, size_t, int);
	int (*sendmmsg)(void *, const void *, size_t, int);
	int (*recvmmsg)(void *, const void *, size_t, int, void *);
	int (*sendmsg)(void *, const void *, int);
	int (*recvmsg)(void *, void *, int);
	int (*sendto)(void *, const void *, size_t, int, const void *, unsigned);
	int (*recvfrom)(void *, void *, size_t, int, void *, unsigned *);
	int (*readv)(void *, const void *, int);
	int (*writev)(void *, const void *, int, int);
	int (*getsockopt)(void *, int, int, void *, unsigned *);
	int (*setsockopt)(void *, int, int, const void *, unsigned);
	int (*getsockname)(void *, void *, unsigned *);
	int (*getpeername)(void *, void *, unsigned *);
	int (*fchown)(void *, unsigned int, unsigned int);
	int (*fstat)(void *, uid_t *);
	int (*socket)(int, int, int, unsigned long, void **, void *);
	void *(*fget)(void *);
};

struct devhost_net_handle_ops {
	void (*netcall_enter)(int pid, int tid, int vpid);
};

struct devhost_net_namespace_ops {
	int (*alloc_new_ns)(int);
	int (*destroy_ns)(int);
};

struct socket_device_info {
	struct device_info socket_devinfo;
	struct devhost_net_socket_ops *sock_ops;
	struct devhost_net_handle_ops *handle_ops;
};

struct devhost_net_ldk_ops {
	int (*bpf)(int cmd, void *attr, unsigned int size);
	int (*dev_ioctl)(int cmd, void *arg_buf);
	void *(*valloc)(unsigned int size);
	void (*vfree)(void *ptr);
};

/**
 * Interface for LDK to register it's socket ops and init devinfo.
 * Return 0 if LDK stack enabled in hm-netd and ops registered successfully,
 * return error code otherwise
 */
int devhost_net_socket_devinfo_init(struct devhost_fops *fops,
				    struct devhost_fops_ex *ex_fops,
				    struct device_info **devinfo,
				    struct devhost_net_socket_ops *ops,
				    struct devhost_net_handle_ops *handle,
				    struct devhost_net_ldk_ops *ldkops);
void devhost_net_namespace_register(struct devhost_net_namespace_ops *ops);
struct devhost_net_namespace_ops *devhost_get_net_namespace_ops(void);
int devhost_get_nsid_by_fd(int fd, unsigned int *nsid);
int devhost_get_net_nsid(unsigned int *nsid);
int devhost_get_nsid_by_pid(int pid, int flags, unsigned int *nsid);

#endif
/* __DEVHOST_PLUGIN_NET_SOCKET_API_H__ */
