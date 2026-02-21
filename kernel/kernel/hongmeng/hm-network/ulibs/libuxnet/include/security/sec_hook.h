/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: The security isolation interface header file
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jun 30 19:46:30 2023
 */

#ifndef __HMNETD_SECURITY_SE_H__
#define __HMNETD_SECURITY_SE_H__

#include "ux_socket.h"

int sec_chk_div_net_admin(void);
int sec_chk_div_net_dev(void);
int sec_chk_div_net_stack_init(void);
int sec_chk_div_net_fs(void);
int sec_chk_div_vsock(void);
int sec_chk_div_raw_socket(void);
int sec_chk_div_vsock_create(void);
int sec_chk_div_as_devhost(void);
int sec_chk_div_as_netd(void);

int sec_hook_div_sk_alloc_security(ux_sock_t *sk);
int sec_hook_div_sk_free_security(ux_sock_t *sk);
int sec_chk_div_socket_post_create(ux_socket_t *sock, int domain, int type, int protocol);
int sec_chk_div_socket_create(int domain, int type, int protocol);
int sec_chk_div_socket_connect(ux_socket_t *sock, const struct sockaddr *addr, socklen_t addrlen);
int sec_chk_div_socket_bind(ux_socket_t *sock, const struct sockaddr *addr, socklen_t addrlen);
int sec_chk_div_socket_listen(ux_socket_t *sock);
int sec_chk_div_socket_accept(ux_socket_t *sock);
int sec_chk_div_socket_post_accept(ux_socket_t *sock, ux_socket_t *nsock);
int sec_chk_div_socket_shutdown(ux_socket_t *sock);
int sec_chk_div_socket_recvmsg(ux_socket_t *sock);
int sec_chk_div_socket_sendmsg(ux_socket_t *sock);
int sec_chk_div_socket_getsockopt(ux_socket_t *sock);
int sec_chk_div_socket_setsockopt(ux_socket_t *sock);
int sec_chk_div_socket_getsockname(ux_socket_t *sock);
int sec_chk_div_socket_getpeername(ux_socket_t *sock);
int sec_hook_div_unix_stream_connect(ux_sock_t *sock, ux_sock_t *other, ux_sock_t *newsk);
int sec_hook_div_unix_may_send(ux_sock_t *sock, ux_sock_t *other);
int sec_hook_div_socket_socketpair(ux_socket_t *socka, ux_socket_t *sockb);
int sec_chk_div_socket_ioctl(ux_socket_t *sock, int cmd);
int sec_chk_div_socket_fchown(ux_socket_t *sock);
int sec_chk_div_socket_fcntl(ux_socket_t *sock, int cmd);
int sec_chk_div_socket_fstat(ux_socket_t *sock);
int sec_chk_div_socket_getpeersec_stream(ux_socket_t *sock, void *optval, socklen_t optlen);

#endif
