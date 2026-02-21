/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: The security isolation interface
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jun 30 19:48:30 2023
 */

#include "../../include/security/sec_hook.h"
#include <generated/hmsd/hook/net/net_admin.h>
#include <generated/hmsd/hook/net/net_bind_service.h>
#include <generated/hmsd/hook/net/net_dev.h>
#include <generated/hmsd/hook/net/net_fs.h>
#include <generated/hmsd/hook/net/vsock.h>
#include <generated/hmsd/hook/net/net_stack_init.h>
#include <generated/hmsd/hook/net/raw_socket.h>
#include <generated/hmsd/hook/net/reserved_port_range.h>
#include <generated/hmsd/hook/net/seharmony.h>
#include <generated/hmsd/hook/net/as_devhost.h>
#include <generated/hmsd/hook/net/as_netd.h>

int sec_chk_div_net_admin(void)
{
	return sec_chk_net_admin();
}

int sec_chk_div_net_dev(void)
{
	return sec_chk_net_dev();
}

int sec_chk_div_net_fs(void)
{
	return sec_chk_net_fs();
}

int sec_chk_div_vsock(void)
{
	return sec_chk_vsock();
}

int sec_chk_div_net_stack_init(void)
{
	return sec_chk_net_stack_init();
}

int sec_chk_div_raw_socket(void)
{
	return sec_chk_raw_socket();
}

int sec_chk_div_vsock_create(void)
{
	return sec_chk_vsock_create();
}

int sec_chk_div_as_devhost(void)
{
	return sec_chk_as_devhost();
}

int sec_chk_div_as_netd(void)
{
	return sec_chk_as_netd();
}

int sec_chk_div_socket_post_create(ux_socket_t *sock, int domain, int type, int protocol)
{
	return sec_chk_socket_post_create(sock, domain, type, protocol);
}

int sec_chk_div_socket_connect(ux_socket_t *sock, const struct sockaddr *addr, socklen_t addrlen)
{
	return sec_chk_socket_connect(sock, addr, addrlen);
}

int sec_chk_div_socket_bind(ux_socket_t *sock, const struct sockaddr *addr, socklen_t addrlen)
{
	return sec_chk_socket_bind(sock, addr, addrlen);
}

int sec_chk_div_socket_listen(ux_socket_t *sock)
{
	return sec_chk_socket_listen(sock);
}

int sec_chk_div_socket_accept(ux_socket_t *sock)
{
	return sec_chk_socket_accept(sock);
}


int sec_chk_div_socket_post_accept(ux_socket_t *sock, ux_socket_t *nsock)
{
	return sec_hook_socket_post_accept(sock, nsock);
}

int sec_chk_div_socket_shutdown(ux_socket_t *sock)
{
	return sec_chk_socket_shutdown(sock);
}

int sec_chk_div_socket_recvmsg(ux_socket_t *sock)
{
	return sec_chk_socket_recvmsg(sock);
}

int sec_chk_div_socket_sendmsg(ux_socket_t *sock)
{
	return sec_chk_socket_sendmsg(sock);
}

int sec_chk_div_socket_getsockopt(ux_socket_t *sock)
{
	return sec_chk_socket_getsockopt(sock);
}

int sec_chk_div_socket_setsockopt(ux_socket_t *sock)
{
	return sec_chk_socket_setsockopt(sock);
}

int sec_chk_div_socket_getsockname(ux_socket_t *sock)
{
	return sec_chk_socket_getsockname(sock);
}

int sec_chk_div_socket_getpeername(ux_socket_t *sock)
{
	return sec_chk_socket_getpeername(sock);
}

int sec_hook_div_sk_alloc_security(ux_sock_t *sk)
{
	return sec_hook_sk_alloc_security(sk);
}

int sec_hook_div_sk_free_security(ux_sock_t *sk)
{
	return sec_hook_sk_free_security(sk);
}

int sec_hook_div_unix_stream_connect(ux_sock_t *sock, ux_sock_t *other, ux_sock_t *newsk)
{
	return sec_hook_unix_stream_connect(sock, other, newsk);
}

int sec_hook_div_unix_may_send(ux_sock_t *sock, ux_sock_t *other)
{
	return sec_hook_unix_may_send(sock, other);
}

int sec_hook_div_socket_socketpair(ux_socket_t *socka, ux_socket_t *sockb)
{
	return sec_hook_socket_socketpair(socka, sockb);
}

int sec_chk_div_socket_create(int domain, int type, int protocol)
{
	return sec_chk_socket_create(domain, type, protocol);
}

int sec_chk_div_socket_ioctl(ux_socket_t *sock, int cmd)
{
	return sec_chk_socket_ioctl(sock, cmd);
}

int sec_chk_div_socket_fchown(ux_socket_t *sock)
{
	return sec_chk_socket_fchown(sock);
}

int sec_chk_div_socket_fcntl(ux_socket_t *sock, int cmd)
{
	return sec_chk_socket_fcntl(sock, cmd);
}

int sec_chk_div_socket_fstat(ux_socket_t *sock)
{
	return sec_chk_socket_fstat(sock);
}

int sec_chk_div_socket_getpeersec_stream(ux_socket_t *sock, void *optval, socklen_t optlen)
{
	return sec_hook_socket_getpeersec_stream(sock, optval, optlen);
}
