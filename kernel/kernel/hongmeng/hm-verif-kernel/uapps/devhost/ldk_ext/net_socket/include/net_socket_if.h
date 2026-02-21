/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Implementation of net stack interfacesa
 * Author: Huawei OS Kernel Lab
 * Create: Mon Dec 18 15:53:30 2023
 */

#ifndef DEVHOST_LDK_EXT_NET_SOCKET_IF_H
#define DEVHOST_LDK_EXT_NET_SOCKET_IF_H

#include <hmnet/hm_socket.h>
#include <libsysif/devhost/api.h>

/* NOTE: transfer selfcall into function call in fusion mode */
/* declare drvhdlr devhost net socket directcall function here */
void backend_syscall_exit(void);
int backend_ctx_setup(void);
void backend_basic_syscall_enter(void);

struct raw_workqueue;
struct raw_workqueue *devhost_net_get_actvpool_wq(void);

typedef struct __actvret_drvcall_devhost_net_socket_socket net_socket_ret_t;
int devhost_net_socket_directcall_socket(unsigned int cnode_idx, int domain, int type, int protocol,
					 unsigned long ino, net_socket_ret_t *socket_ret);
int devhost_net_socket_directcall_close(unsigned int cnode_idx, int sockfd);
int devhost_net_socket_directcall_send(long sockfd, const void *buf, size_t len, int flags);
int devhost_net_socket_directcall_recv(long sockfd, void *buf, size_t len, int flags);
int devhost_net_socket_directcall_sendto(long sockfd, const void *buf, size_t len, int flags,
					const struct sockaddr *daddr, socklen_t addrlen);
int devhost_net_socket_directcall_recvfrom(long sockfd, void *buf, size_t len, int flags,
					      struct sockaddr *saddr, socklen_t *addrlen);
int devhost_net_socket_directcall_sendmsg(long sockfd, const struct msghdr *msg, int flags);
int devhost_net_socket_directcall_recvmsg(long sockfd, struct msghdr *msg, int flags);
int devhost_net_socket_directcall_getsockopt(long sockfd, int level, int optname, void *optval, socklen_t *optlen);
int devhost_net_socket_directcall_setsockopt(long sockfd, int level, int optname, const void *optval, socklen_t optlen);

#endif
