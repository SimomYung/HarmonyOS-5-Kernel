/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: liblinux PAL implement for net operations
 * Author: Huawei OS Kernel Lab
 * Create: Fri Feb 11 10:45:12 2019
 */
#include <liblinux/pal.h>

#include <hongmeng/errno.h>
#include <libhwsecurec/securec.h>
#include <libhmactv/actv.h>
#include <hmasm/lsyscall.h>
#include <devhost/net.h>
#include <hongmeng/syscall.h>
#include <sys/syscall.h>
#include <devhost/log.h>

int
liblinux_pal_native_net_rx(unsigned int pool_offset, int shm_len,
			   int rx_len, int offset, int ifindex)
{
	int ret;

	ret = devhost_native_net_rx(pool_offset, shm_len, rx_len, offset, ifindex);
	return -hmerrno2posix(ret);
}

int
liblinux_pal_native_net_rx_ex(int num, void *data, int data_size)
{
	int ret;

	ret = devhost_native_net_rx_ex(num, data, data_size);
	return -hmerrno2posix(ret);
}

int
liblinux_pal_netdev_alloc_ifindex(int ifindex)
{
	int ret;

	ret = devhost_netdev_alloc_ifindex(ifindex);
	if (ret < 0) {
		return -hmerrno2posix(ret);
	}
	return ret;
}

int
liblinux_pal_socket_create(int family, int type, int protocol)
{
	return (int)lsyscall_syscall3(__NR_socket, family, type, protocol);
}

int
liblinux_pal_socket_connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
	return (int)lsyscall_syscall3(__NR_connect, sockfd, ptr_to_type(addr, long), addrlen);
}

ssize_t
liblinux_pal_socket_sendmsg(int sockfd, const struct msghdr *msg, int flags)
{
	return (ssize_t)lsyscall_syscall3(__NR_sendmsg, sockfd, ptr_to_type(msg, long), flags);
}

int
liblinux_pal_socket_close(int sockfd)
{
	return (int)lsyscall_syscall1(__NR_close, sockfd);
}

int
liblinux_pal_is_compat_client(void)
{
#ifdef __arm__
	return false;
#else
	return hm_actv_local_is_compat32();
#endif
}
